"""Extract editable host meshes and textures from a CoopDX actor directory."""

from __future__ import annotations

import argparse
import math
import re
from pathlib import Path

from PIL import Image


ARRAY_RE = re.compile(
    r"(?:static\s+)?(?:const\s+)?Vtx\s+(\w+)\s*\[[^]]*]\s*=\s*\{(.*?)\n\};",
    re.S,
)
VERTEX_RE = re.compile(
    r"\{\{\s*\{\s*(-?\d+)\s*,\s*(-?\d+)\s*,\s*(-?\d+)\s*\}\s*,\s*"
    r"-?\d+\s*,\s*\{\s*(-?\d+)\s*,\s*(-?\d+)\s*\}"
)
DL_RE = re.compile(r"(?:const\s+)?Gfx\s+(\w+)\s*\[\]\s*=\s*\{(.*?)\n\};", re.S)
CALL_RE = re.compile(r"gsSPDisplayList\(\s*(\w+)\s*\)")
LOAD_RE = re.compile(r"gsSPVertex\(\s*(\w+)(?:\s*\+\s*(\d+))?\s*,\s*(\d+)\s*,\s*(\d+)\s*\)")
TRI1_RE = re.compile(r"gsSP1Triangle\(\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)")
TRI2_RE = re.compile(
    r"gsSP2Triangles\(\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\s*,[^,]*,\s*"
    r"(\d+)\s*,\s*(\d+)\s*,\s*(\d+)"
)


def parse_model(path: Path):
    text = path.read_text(encoding="utf-8")
    arrays: dict[str, list[tuple[int, ...]]] = {}
    for name, body in ARRAY_RE.findall(text):
        arrays[name] = [tuple(map(int, match)) for match in VERTEX_RE.findall(body)]

    groups = []
    calls = {}
    for name, body in DL_RE.findall(text):
        calls[name] = CALL_RE.findall(body)
        slots: dict[int, tuple[str, int]] = {}
        faces: list[tuple[int, int, int]] = []
        for line in body.splitlines():
            load = LOAD_RE.search(line)
            if load:
                array, offset, count, slot = load.groups()
                for index in range(int(count)):
                    slots[int(slot) + index] = (array, int(offset or 0) + index)
                continue
            triangles = [tuple(map(int, tri)) for tri in TRI1_RE.findall(line)]
            for six in TRI2_RE.findall(line):
                values = tuple(map(int, six))
                triangles.extend((values[:3], values[3:]))
            for triangle in triangles:
                if all(index in slots for index in triangle):
                    # Store temporary slot indices; the writer resolves them.
                    faces.append(tuple((slots[index][0], slots[index][1]) for index in triangle))
        if faces:
            groups.append((name, faces))
    return arrays, groups, calls


def resolved_groups(groups, calls):
    direct = dict(groups)
    def resolve(name, seen=()):
        if name in seen:
            return []
        out = list(direct.get(name, ()))
        for child in calls.get(name, ()):
            out.extend(resolve(child, seen + (name,)))
        return out
    return {name: resolve(name) for name in set(direct) | set(calls)}


GEO_BLOCK_RE = re.compile(r"const\s+GeoLayout\s+(\w+)\[\]\s*=\s*\{(.*?)\n\};", re.S)
GEO_CMD_RE = re.compile(r"(GEO_[A-Z0-9_]+)\((.*?)\)")

def parse_geo(path: Path):
    blocks = {}
    for name, body in GEO_BLOCK_RE.findall(path.read_text(encoding="utf-8")):
        root = []
        stack = [root]
        last = None
        for raw in body.splitlines():
            line = raw.split("//", 1)[0].strip().rstrip(",")
            match = GEO_CMD_RE.search(line)
            if not match:
                continue
            kind, arg_text = match.groups()
            if kind == "GEO_OPEN_NODE":
                if last is not None:
                    stack.append(last[2])
                continue
            if kind == "GEO_CLOSE_NODE":
                if len(stack) > 1: stack.pop()
                continue
            args = [part.strip() for part in arg_text.split(",")]
            last = [kind, args, []]
            stack[-1].append(last)
        blocks[name] = root
    return blocks


def mat_mul(a, b):
    return tuple(tuple(sum(a[r][k] * b[k][c] for k in range(4)) for c in range(4)) for r in range(4))

IDENTITY = ((1.,0.,0.,0.), (0.,1.,0.,0.), (0.,0.,1.,0.), (0.,0.,0.,1.))

def node_matrix(kind, args):
    if kind not in ("GEO_ANIMATED_PART", "GEO_TRANSLATE_ROTATE", "GEO_ROTATION_NODE", "GEO_SCALE"):
        return IDENTITY
    if kind == "GEO_SCALE":
        s = int(args[-1], 0) / 65536.0
        return ((s,0.,0.,0.), (0.,s,0.,0.), (0.,0.,s,0.), (0.,0.,0.,1.))
    nums = [int(v, 0) for v in args[1:7] if re.fullmatch(r"-?\d+", v)]
    x, y, z = (nums + [0, 0, 0])[:3]
    out = ((1.,0.,0.,float(x)), (0.,1.,0.,float(y)), (0.,0.,1.,float(z)), (0.,0.,0.,1.))
    if len(nums) >= 6:
        rx, ry, rz = (math.radians(v) for v in nums[3:6])
        cx,sx,cy,sy,cz,sz = math.cos(rx),math.sin(rx),math.cos(ry),math.sin(ry),math.cos(rz),math.sin(rz)
        rot = ((cy*cz, sx*sy*cz-cx*sz, cx*sy*cz+sx*sz,0.), (cy*sz,sx*sy*sz+cx*cz,cx*sy*sz-sx*cz,0.), (-sy,sx*cy,cx*cy,0.), (0.,0.,0.,1.))
        out = mat_mul(out, rot)
    return out


def bind_pose(blocks, display_lists, actor):
    placements = []
    def walk(nodes, matrix, branch_seen=()):
        for kind, args, children in nodes:
            local = mat_mul(matrix, node_matrix(kind, args))
            dl = None
            if kind == "GEO_DISPLAY_LIST" and len(args) > 1: dl = args[1]
            if kind == "GEO_ANIMATED_PART" and len(args) > 4 and args[4] != "NULL": dl = args[4]
            if dl and display_lists.get(dl): placements.append((dl, local, display_lists[dl]))
            if kind == "GEO_BRANCH" and len(args) > 1 and args[1] not in branch_seen:
                walk(blocks.get(args[1], ()), local, branch_seen + (args[1],))
            elif kind == "GEO_SWITCH_CASE":
                if children: walk([children[0]], local, branch_seen)
            else:
                walk(children, local, branch_seen)
    candidates = (f"{actor}_geo_body", f"{actor}_geo")
    root = next((name for name in candidates if name in blocks), candidates[0])
    walk(blocks.get(root, ()), IDENTITY, (root,))
    return placements


def write_bind_obj(path, arrays, placements):
    path.parent.mkdir(parents=True, exist_ok=True)
    count = 0
    index = 1
    with path.open("w", encoding="utf-8", newline="\n") as out:
        out.write("# Generated CoopDX bind pose with GeoLayout transforms applied.\n")
        for name, matrix, faces in placements:
            out.write(f"g {name}\n")
            for face in faces:
                refs = []
                for key in face:
                    if key[0] not in arrays or key[1] >= len(arrays[key[0]]): continue
                    x,y,z,u,v = arrays[key[0]][key[1]]
                    p = (x,y,z,1.)
                    q = [sum(matrix[r][k] * p[k] for k in range(4)) for r in range(3)]
                    out.write(f"v {q[0]:.6f} {q[1]:.6f} {q[2]:.6f}\nvt {u/1024.0:.6f} {1.0-v/1024.0:.6f}\n")
                    refs.append(f"{index}/{index}"); index += 1
                if len(refs) == 3:
                    out.write("f " + " ".join(refs) + "\n"); count += 1
    return count


def write_obj(path: Path, arrays: dict[str, list[tuple[int, ...]]], groups) -> int:
    path.parent.mkdir(parents=True, exist_ok=True)
    vertex_ids: dict[tuple[str, int], int] = {}
    uv_ids: dict[tuple[str, int], int] = {}
    with path.open("w", encoding="utf-8", newline="\n") as out:
        out.write("# Generated from SM64CoopDX actor source; groups remain bone-local.\n")
        for array, vertices in arrays.items():
            for index, (x, y, z, u, v) in enumerate(vertices):
                key = (array, index)
                vertex_ids[key] = len(vertex_ids) + 1
                uv_ids[key] = len(uv_ids) + 1
                out.write(f"v {x} {y} {z}\n")
                out.write(f"vt {u / 1024.0:.6f} {1.0 - v / 1024.0:.6f}\n")
        face_count = 0
        for name, faces in groups:
            out.write(f"g {name}\n")
            for face in faces:
                if not all(key in vertex_ids for key in face):
                    continue
                out.write("f " + " ".join(
                    f"{vertex_ids[key]}/{uv_ids[key]}" for key in face
                ) + "\n")
                face_count += 1
    return face_count


def convert_textures(actor_dir: Path, output: Path) -> int:
    count = 0
    texture_dir = output / "textures"
    texture_dir.mkdir(parents=True, exist_ok=True)
    for source in sorted(actor_dir.glob("*.png")):
        image = Image.open(source).convert("RGB")
        destination = texture_dir / f"{source.stem}.ppm"
        with destination.open("wb") as stream:
            stream.write(f"P6\n{image.width} {image.height}\n255\n".encode("ascii"))
            stream.write(image.tobytes())
        count += 1
    return count


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("actor_dir", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()
    arrays, groups, calls = parse_model(args.actor_dir / "model.inc.c")
    faces = write_obj(args.output / "model-parts.obj", arrays, groups)
    displays = resolved_groups(groups, calls)
    placements = bind_pose(parse_geo(args.actor_dir / "geo.inc.c"), displays, args.actor_dir.name)
    bind_faces = write_bind_obj(args.output / "model.obj", arrays, placements)
    textures = convert_textures(args.actor_dir, args.output)
    print(f"{args.actor_dir.name}: {len(arrays)} vertex arrays, {len(groups)} groups, "
          f"{faces} part faces, {bind_faces} bind-pose faces, {textures} textures")
    return 0 if faces and bind_faces else 1


if __name__ == "__main__":
    raise SystemExit(main())
