"""Convert a PNG into the dependency-free P6 texture HostMesh consumes."""

from __future__ import annotations

import argparse
from pathlib import Path

from PIL import Image


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("source", type=Path)
    parser.add_argument("destination", type=Path)
    args = parser.parse_args()
    image = Image.open(args.source).convert("RGB")
    args.destination.parent.mkdir(parents=True, exist_ok=True)
    with args.destination.open("wb") as output:
        output.write(f"P6\n{image.width} {image.height}\n255\n".encode("ascii"))
        output.write(image.tobytes())
    print(f"{args.source} -> {args.destination} ({image.width}x{image.height})")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
