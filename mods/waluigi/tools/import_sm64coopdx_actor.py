"""Extract editable OBJ parts and PPM textures from a CoopDX actor directory.

This is intentionally a source importer, not an N64 display-list runtime. It
turns each low-level display list into an OBJ group. Bone transforms from the
GeoLayout remain future work, so groups retain their local coordinates instead
of pretending the resulting file is already a rigged in-game replacement.
"""

from __future__ import annotations

import argparse
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
LOAD_RE = re.compile(r"gsSPVertex\(\s*(\w+)(?:\s*\+\s*(\d+))?\s*,\s*(\d+)\s*,\s*(\d+)\s*\)")
TRI1_RE = re.compile(r"gsSP1Triangle\(\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)")
TRI2_RE = re.compile(
    r"gsSP2Triangles\(\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\s*,[^,]*,\s*"
    r"(\d+)\s*,\s*(\d+)\s*,\s*(\d+)"
)


def parse_model(path: Path) -> tuple[dict[str, list[tuple[int, ...]]], list[tuple[str, list[tuple[int, int, int]]]]]:
    text = path.read_text(encoding="utf-8")
    arrays: dict[str, list[tuple[int, ...]]] = {}
    for name, body in ARRAY_RE.findall(text):
        arrays[name] = [tuple(map(int, match)) for match in VERTEX_RE.findall(body)]

    groups: list[tuple[str, list[tuple[int, int, int]]]] = []
    for name, body in DL_RE.findall(text):
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
    return arrays, groups


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
    arrays, groups = parse_model(args.actor_dir / "model.inc.c")
    faces = write_obj(args.output / "model-parts.obj", arrays, groups)
    textures = convert_textures(args.actor_dir, args.output)
    print(f"{args.actor_dir.name}: {len(arrays)} vertex arrays, {len(groups)} groups, "
          f"{faces} faces, {textures} textures")
    return 0 if faces else 1


if __name__ == "__main__":
    raise SystemExit(main())
