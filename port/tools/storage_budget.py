#!/usr/bin/env python3
"""Audit source/Git/build or consumer-kit bytes; never deletes or moves files."""
import argparse
import collections
import json
import os
from pathlib import Path
import subprocess


def measure(root, asset_dirs=()):
    root = Path(root).resolve(strict=True)
    excluded = {root / "extracted", root / "build" / "assets"}
    excluded.update((root / p).resolve() for p in asset_dirs)
    totals = collections.Counter()
    seen = set()
    errors = []
    excluded_bytes = 0

    def walk(directory, group=None, omit=False):
        nonlocal excluded_bytes
        try:
            with os.scandir(directory) as entries:
                for entry in entries:
                    path = Path(entry.path)
                    category = group or entry.name
                    ignored = omit or path in excluded or path.suffix.lower() == ".nds"
                    if entry.is_symlink() or (hasattr(entry, "is_junction") and entry.is_junction()):
                        if not ignored:
                            errors.append(f"Cannot certify linked path: {path}")
                        continue
                    if entry.is_dir(follow_symlinks=False):
                        walk(path, category, ignored)
                    elif entry.is_file(follow_symlinks=False):
                        info = entry.stat(follow_symlinks=False)
                        identity = (info.st_dev, info.st_ino)
                        if info.st_ino and identity in seen:
                            continue
                        if info.st_ino:
                            seen.add(identity)
                        if ignored:
                            excluded_bytes += info.st_size
                        else:
                            totals[category] += info.st_size
        except OSError as exc:
            errors.append(str(exc))

    walk(root)
    # A worktree's .git file points at storage elsewhere. Count that shared
    # database too, so a worktree cannot appear small by hiding its history.
    if (root / ".git").is_file():
        result = subprocess.run(["git", "-C", str(root), "rev-parse", "--path-format=absolute",
                                 "--git-common-dir"], capture_output=True, text=True)
        if result.returncode:
            errors.append("Cannot locate the worktree's shared Git database")
        else:
            walk(Path(result.stdout.strip()), ".git-shared")
    return {"root": str(root), "bytes": sum(totals.values()),
            "excluded_rom_asset_bytes": excluded_bytes,
            "groups": dict(totals.most_common()), "errors": errors,
            "accounting": "logical file bytes; hardlinks counted once; filesystem metadata not included"}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("root", type=Path)
    parser.add_argument("--kind", choices=("developer", "consumer"), required=True)
    parser.add_argument("--asset-dir", action="append", default=[],
                        help="Additional directory of user ROM assets, relative to root")
    args = parser.parse_args()
    report = measure(args.root, args.asset_dir)
    report["limit_bytes"] = 500_000_000 if args.kind == "developer" else 10_000_000
    report["within_budget"] = not report["errors"] and report["bytes"] < report["limit_bytes"]
    print(json.dumps(report, indent=2))
    return 0 if report["within_budget"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
