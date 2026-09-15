"""Find likely four-character assumptions for the Waluigi expansion.

The output is intentionally conservative: a result is included only when a
four-way expression appears near character/player-model vocabulary. Every hit
still requires human classification before it becomes a patch.
"""

from __future__ import annotations

import re
from pathlib import Path


ROOT = Path(__file__).resolve().parents[3]
SEARCH_ROOTS = (ROOT / "include", ROOT / "src", ROOT / "port")
SOURCE_SUFFIXES = {".c", ".cpp", ".h", ".hpp"}

CARDINALITY = re.compile(
    r"(?:\[\s*4\s*\]|(?:<|<=|>|>=|==|!=)\s*[34]\b|%\s*4\b|&\s*3\b)"
)
CONTEXT = re.compile(
    r"character|mcharacter|char(?:acter)?id|bodymodel|hatcharacter|"
    r"mario|luigi|wario|yoshi|capicon|playscharvoice",
    re.IGNORECASE,
)


def source_files():
    for base in SEARCH_ROOTS:
        for path in base.rglob("*"):
            if path.is_file() and path.suffix.lower() in SOURCE_SUFFIXES:
                yield path


def main() -> int:
    hits: list[tuple[str, int, str]] = []
    for path in source_files():
        lines = path.read_text(encoding="utf-8", errors="replace").splitlines()
        for index, line in enumerate(lines):
            if not CARDINALITY.search(line):
                continue
            lo = max(0, index - 4)
            hi = min(len(lines), index + 5)
            if CONTEXT.search("\n".join(lines[lo:hi])):
                rel = path.relative_to(ROOT).as_posix()
                hits.append((rel, index + 1, line.strip()))

    for rel, line_no, text in hits:
        print(f"{rel}:{line_no}: {text}")
    print(f"\n{len(hits)} candidate character-cardinality assumptions")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
