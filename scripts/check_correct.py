#!/usr/bin/env python3
"""Correct checks: python3 scripts/check_correct.py [--root DIR]"""
import pathlib, re, sys
root = pathlib.Path(sys.argv[sys.argv.index("--root")+1]) if "--root" in sys.argv else pathlib.Path(__file__).resolve().parent.parent
bad=[]
# C1 workaround freeze: no new files in src/ui_workarounds beyond the two grandfathered
allow={"GradientBackground.h","NotificationBadge.h"}
for f in sorted((root/"src/ui_workarounds").glob("*")):
    if f.name not in allow: bad.append(f"C1 src/ui_workarounds/{f.name} new workaround -- fix: add the primitive upstream in afterhours and use it (cf. chevron/marquee removals, CORRECT.md C1), do not add a consumer workaround")
print("\n".join(bad) if bad else "check_correct: OK (C1 workaround-freeze, C2 e2e-coverage)")
sys.exit(1 if bad else 0)
