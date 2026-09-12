#!/usr/bin/env python3
"""Check literal font names in screen headers against wm's font registry."""

import argparse
import re
from pathlib import Path

root = Path(__file__).resolve().parents[1]
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("screens", nargs="+", type=Path)
args = parser.parse_args()
registered = set(re.findall(r'^\s*\{"([^\"]+)",',
                            (root / "src/font_config.h").read_text(), re.MULTILINE))
failed = False
for path in args.screens:
    text = path.read_text()
    local = set(re.findall(r'load_font\("([^\"]+)"', text))
    for match in re.finditer(r'with_font\("([^\"]+)"', text):
        if match[1] not in registered | local:
            line = text.count("\n", 0, match.start()) + 1
            print(f"{path}:{line}: unregistered font {match[1]!r}")
            failed = True
raise SystemExit(1 if failed else 0)
