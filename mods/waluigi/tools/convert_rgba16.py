"""Convert a PNG to Nintendo DS/Nitro direct-color RGB5A1 texels.

SM64CoopDX's ``*.rgba16.png`` files are ordinary RGBA PNGs whose names describe
their intended N64 format. This converter emits the closely related Nintendo DS
direct-color layout: little-endian A1B5G5R5, one u16 per pixel. The result is a
raw texture payload for the Waluigi asset builder; model/material injection is a
separate step because BMD owns dimensions, UVs, and texture dictionaries.
"""

from __future__ import annotations

import argparse
import struct
from pathlib import Path

from PIL import Image


VALID_DIMENSIONS = {8, 16, 32, 64, 128, 256, 512, 1024}


def rgb5a1(r: int, g: int, b: int, a: int) -> int:
    """Pack RGBA8888 as Nitro direct-color A1B5G5R5."""
    return (
        (r >> 3)
        | ((g >> 3) << 5)
        | ((b >> 3) << 10)
        | ((1 if a >= 128 else 0) << 15)
    )


def convert(source: Path, destination: Path) -> tuple[int, int, int]:
    image = Image.open(source).convert("RGBA")
    width, height = image.size
    if width not in VALID_DIMENSIONS or height not in VALID_DIMENSIONS:
        raise ValueError(
            f"{source}: {width}x{height} is not a Nitro power-of-two texture"
        )

    transparent = 0
    destination.parent.mkdir(parents=True, exist_ok=True)
    with destination.open("wb") as stream:
        for r, g, b, a in image.getdata():
            transparent += a < 128
            stream.write(struct.pack("<H", rgb5a1(r, g, b, a)))
    return width, height, transparent


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("source", type=Path)
    parser.add_argument("destination", type=Path)
    args = parser.parse_args()
    width, height, transparent = convert(args.source, args.destination)
    print(
        f"{args.source} -> {args.destination}: "
        f"{width}x{height}, {width * height * 2} bytes, "
        f"{transparent} transparent pixels"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
