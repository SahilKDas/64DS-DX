"""Convert all locally imported SM64CoopDX Waluigi PNGs to Nitro RGB5A1."""

from __future__ import annotations

from pathlib import Path

from convert_rgba16 import convert


ROOT = Path(__file__).resolve().parents[3]
SOURCE = ROOT / "build" / "waluigi" / "sm64coopdx"
OUTPUT = ROOT / "build" / "waluigi" / "nitro" / "rgb5a1"


def main() -> int:
    sources = sorted(SOURCE.glob("*.png"))
    if not sources:
        raise SystemExit(f"no imported Waluigi PNGs in {SOURCE}")
    for source in sources:
        destination = OUTPUT / f"{source.stem}.rgb5a1"
        width, height, transparent = convert(source, destination)
        print(
            f"{source.name}: {width}x{height}, "
            f"{destination.stat().st_size} bytes, alpha0={transparent}"
        )
    print(f"converted {len(sources)} textures into {OUTPUT}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
