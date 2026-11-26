#!/usr/bin/env python3
"""Audit every gallery screen for controls hidden behind the global chrome.

Owner todo: "on some screens the buttons to control the screen are hidden
behind other buttons". PR #24 fixed Trellis; this script catches the rest.

For each screen in the registry (ui_tester --list-screens, 162 screens) it
runs:

    ui_tester --layout-summary <screen> -w 1280 -h 720 --layout-output <tmp>

which prints one line per named element: `name x y w h`. Every element rect
is intersected with the three global chrome safe areas (derived from
src/plugins/gallery_shell/{ScreenNavigator.h,RenderScreenHUD.h}):

  1. Browse-screens launcher (ScreenNavigator "nav_open" button):
     x = width-212 .. width-12, y = 8 .. 40  → 1068..1268 x 8..40 @1280x720
  2. Bottom-left HUD band (RenderScreenHUD bg rect, description line):
     x < 760, y >= height-64                 → approx 660..720 @720p
  3. Bottom-right index chip (RenderScreenHUD index bg):
     x > width-140, y >= height-40

Hits are classified by debug name as interactive-control vs
container/background. Full-screen roots/backgrounds (>=85% of the screen)
and the chrome's own nav_* elements are excluded entirely. A "real"
offender is an interactive control intersecting a safe area.

Exit code is 0 unless --strict is passed and real offenders were found.

Usage:
    python3 scripts/audit_screen_control_overlaps.py [--strict] \
        [--out report.json] [--workers 4] [--screens a b c]
"""

from __future__ import annotations

import argparse
import json
import os
import re
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor
from typing import NamedTuple

WIDTH, HEIGHT = 1280, 720

# (name, x1, y1, x2, y2) — see docs/SCREEN_CONTROL_OVERLAP_AUDIT.md.
SAFE_AREAS = [
    ("launcher", WIDTH - 212, 8, WIDTH - 12, 40),
    ("hud_bottom_left", 0, HEIGHT - 64, 760, HEIGHT),
    ("index_chip", WIDTH - 140, HEIGHT - 40, WIDTH, HEIGHT),
]

CHROME_PREFIXES = ("nav_",)  # the chrome's own elements (nav_open, …)

# Substrings that mark a debug name as an interactive control.
INTERACTIVE_TOKENS = (
    "btn", "button", "slider", "toggle", "checkbox", "radio", "dropdown",
    "stepper", "input", "textbox", "text_field", "knob", "dial", "switch",
    "segment", "link", "play", "pause", "tab_", "_tab", "increment",
    "decrement", "arrow", "icon_btn", "chip_btn",
)
# Substrings that mark a debug name as a container / background / text.
CONTAINER_TOKENS = (
    "bg", "background", "card", "panel", "div", "row", "list", "vstack",
    "hstack", "scroll", "container", "frame", "wrap", "canvas", "root",
    "backdrop", "title", "label", "caption", "hint", "text", "divider",
    "guide", "hud", "deco", "image", "art", "preview", "stage", "sidebar",
    "header", "footer", "box", "map", "field", "ground", "grid", "legend",
    "note", "body", "content",
)

FULL_SCREEN_FRACTION = 0.85


class Rect(NamedTuple):
    x: int
    y: int
    w: int
    h: int

    @property
    def x2(self) -> int:
        return self.x + self.w

    @property
    def y2(self) -> int:
        return self.y + self.h

    @property
    def area(self) -> int:
        return self.w * self.h

    @property
    def cx(self) -> float:
        return self.x + self.w / 2

    @property
    def cy(self) -> float:
        return self.y + self.h / 2


class Hit(NamedTuple):
    screen: str
    element: str
    rect: Rect
    zone: str
    kind: str  # interactive | container | other
    overlap_px: int
    covered_frac: float  # fraction of the element inside the safe area
    center_inside: bool


def classify(name: str) -> str:
    low = name.lower()
    if any(tok in low for tok in INTERACTIVE_TOKENS):
        return "interactive"
    if any(tok in low for tok in CONTAINER_TOKENS):
        return "container"
    return "other"


def intersect(a: Rect, b: tuple[int, int, int, int]) -> int:
    ox = max(0, min(a.x2, b[2]) - max(a.x, b[0]))
    oy = max(0, min(a.y2, b[3]) - max(a.y, b[1]))
    return ox * oy


def layout_for(binary: str, screen: str, timeout: int) -> list[tuple[str, Rect]]:
    """Run --layout-summary for one screen, return [(name, rect)]."""
    with tempfile.NamedTemporaryFile(suffix=".txt", delete=False) as tmp:
        out_path = tmp.name
    try:
        subprocess.run(
            [binary, "--layout-summary", screen, "-w", str(WIDTH),
             "-h", str(HEIGHT), "--layout-output", out_path],
            stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
            timeout=timeout, check=True)
        rows = []
        with open(out_path) as fh:
            for line in fh:
                parts = line.split()
                if len(parts) != 5:
                    continue
                name, x, y, w, h = parts
                rows.append((name, Rect(int(x), int(y), int(w), int(h))))
        return rows
    finally:
        os.unlink(out_path)


def audit_screen(binary: str, screen: str, timeout: int) -> tuple[list[Hit], list[tuple[str, Rect]]]:
    """Return (hits, excluded) for one screen."""
    hits: list[Hit] = []
    excluded: list[tuple[str, Rect]] = []
    for name, rect in layout_for(binary, screen, timeout):
        if rect.w <= 0 or rect.h <= 0:
            continue
        if name.startswith(CHROME_PREFIXES):
            excluded.append((name, rect))
            continue
        if rect.area >= FULL_SCREEN_FRACTION * WIDTH * HEIGHT:
            excluded.append((name, rect))
            continue
        kind = classify(name)
        for zone, x1, y1, x2, y2 in SAFE_AREAS:
            overlap = intersect(rect, (x1, y1, x2, y2))
            if overlap <= 0:
                continue
            covered = overlap / rect.area if rect.area else 0.0
            center_inside = (x1 <= rect.cx <= x2 and y1 <= rect.cy <= y2)
            hits.append(Hit(screen, name, rect, zone, kind,
                            overlap, covered, center_inside))
            break  # one hit per element (first zone wins)
    return hits, excluded


def list_screens(binary: str, timeout: int) -> list[str]:
    out = subprocess.run([binary, "--list-screens"], capture_output=True,
                         text=True, timeout=timeout, check=True)
    names = re.findall(r"--screen=([A-Za-z0-9_]+)", out.stdout + out.stderr)
    return sorted(set(names))


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--binary", default="./output/ui_tester.exe")
    ap.add_argument("--screens", nargs="*", help="screen names (default: registry)")
    ap.add_argument("--width", type=int, default=WIDTH,
                    help="accepted for parity; safe rects are derived @1280x720")
    ap.add_argument("--height", type=int, default=HEIGHT)
    ap.add_argument("--workers", type=int, default=4)
    ap.add_argument("--timeout", type=int, default=120)
    ap.add_argument("--out", help="write JSON report to this path")
    ap.add_argument("--strict", action="store_true",
                    help="exit 1 when real (interactive) offenders are found")
    args = ap.parse_args()

    binary = os.path.abspath(args.binary)
    screens = args.screens or list_screens(binary, args.timeout)
    print(f"Auditing {len(screens)} screens @ {WIDTH}x{HEIGHT} …")

    all_hits: list[Hit] = []
    errors: list[str] = []
    with ThreadPoolExecutor(max_workers=args.workers) as pool:
        futures = {pool.submit(audit_screen, binary, s, args.timeout): s
                   for s in screens}
        done = 0
        for fut in futures:
            screen = futures[fut]
            try:
                hits, _ = fut.result()
                all_hits.extend(hits)
            except Exception as exc:  # noqa: BLE001 — report, keep sweeping
                errors.append(f"{screen}: {exc}")
            done += 1
            if done % 40 == 0:
                print(f"  …{done}/{len(screens)}")

    interactive = [h for h in all_hits if h.kind == "interactive"]
    containers = [h for h in all_hits if h.kind == "container"]
    others = [h for h in all_hits if h.kind == "other"]

    # Rank: centre inside the safe area first (actually hidden), then how
    # much of the element is covered, then raw overlap pixels.
    interactive.sort(key=lambda h: (not h.center_inside, -h.covered_frac,
                                    -h.overlap_px))
    containers.sort(key=lambda h: -h.overlap_px)
    others.sort(key=lambda h: -h.overlap_px)

    def fmt(h: Hit) -> str:
        r = h.rect
        return (f"{h.screen}:{h.element} rect={r.x},{r.y},{r.w}x{r.h} "
                f"zone={h.zone} overlap={h.overlap_px}px "
                f"covered={h.covered_frac:.0%} "
                f"center={'in' if h.center_inside else 'out'}")

    print(f"\n=== Real offenders (interactive controls under chrome): "
          f"{len(interactive)} ===")
    for i, h in enumerate(interactive, 1):
        print(f"{i:3d}. {fmt(h)}")

    print(f"\n=== Containers/backgrounds intersecting chrome "
          f"(false-positive class, {len(containers)}) — top 15 ===")
    for h in containers[:15]:
        print(f"  {fmt(h)}")

    print(f"\n=== Unclassified names ({len(others)}) — top 15 ===")
    for h in others[:15]:
        print(f"  {fmt(h)}")

    if errors:
        print(f"\n=== Screens that failed ({len(errors)}) ===")
        for e in errors:
            print(f"  {e}")

    print(f"\nSUMMARY: screens={len(screens)} audited, "
          f"errors={len(errors)}, real_offenders={len(interactive)}, "
          f"container_hits={len(containers)}, other_hits={len(others)}")

    if args.out:
        report = {
            "size": [WIDTH, HEIGHT],
            "screens_audited": len(screens),
            "errors": errors,
            "real_offenders": [h._asdict() for h in interactive],
            "container_hits": [h._asdict() for h in containers],
            "other_hits": [h._asdict() for h in others],
        }
        with open(args.out, "w") as fh:
            json.dump(report, fh, indent=2, default=str)
        print(f"JSON report: {args.out}")

    if args.strict and interactive:
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
