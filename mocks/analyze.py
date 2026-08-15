#!/usr/bin/env python3
"""Scan the UI dumps for layout pathologies and rank them by blast radius.

Each check is a property the layout engine should never violate regardless of
what the screen asked for, so a hit is a question for afterhours rather than
for the screen. Ranked by how many distinct screens a fix would touch.

  ./mocks/analyze.py [--detail CHECK]
"""
import json, pathlib, sys
from collections import defaultdict

TREES = pathlib.Path(__file__).parent / "trees"
EPS = 1.0  # sub-pixel rounding between the solver and the captured rect

def nodes(root, parent=None, under_clip=False):
    """Walk the tree carrying whether ANY ancestor clips or scrolls, since
    content is free to sit outside the viewport anywhere below one."""
    yield root, parent, under_clip
    clipped = under_clip or root.get("clips") or root.get("scrolls")
    for c in root["children"]:
        yield from nodes(c, root, clipped)

def r(n): return n["rect"]
def vis(n): return n.get("visible", False)
def named(n): return n.get("name") or f"#{n['id']}"

def flow_children(n):
    """Children that participate in the parent's flow."""
    return [c for c in n["children"] if not c.get("absolute") and vis(c)]

# --- checks: each yields (label, detail) -----------------------------------

def check_escapes(n, parent, screen):
    """A flow child laid outside its parent's box."""
    if parent is None or n.get("absolute") or not vis(n) or not vis(parent):
        return
    # A clipper/scroller is meant to hold oversized content.
    if parent.get("clips") or parent.get("scrolls"):
        return
    a, b = r(n), r(parent)
    if a["width"] <= 0 or a["height"] <= 0:
        return
    over = []
    if a["x"] < b["x"] - EPS: over.append(f"left by {b['x']-a['x']:.0f}")
    if a["y"] < b["y"] - EPS: over.append(f"top by {b['y']-a['y']:.0f}")
    if a["x"]+a["width"] > b["x"]+b["width"] + EPS:
        over.append(f"right by {a['x']+a['width']-b['x']-b['width']:.0f}")
    if a["y"]+a["height"] > b["y"]+b["height"] + EPS:
        over.append(f"bottom by {a['y']+a['height']-b['y']-b['height']:.0f}")
    if over:
        yield "child-escapes-parent", f"{named(n)} out of {named(parent)}: {', '.join(over)}"

def check_collapsed(n, parent, screen):
    """A sizing spec that promises space but resolved to nothing."""
    for axis, key in (("x", "width"), ("y", "height")):
        d = n["desired"][axis]
        if d["dim"] in ("Expand", "Percent", "ScreenPercent") and d["value"] > 0:
            if r(n)[key] <= 0.5 and vis(parent) if parent else False:
                yield "sizing-collapsed-to-zero", \
                      f"{named(n)} {axis}={d['dim']}({d['value']:g}) -> {r(n)[key]:.0f}px"

def check_offscreen(n, parent, screen, under_clip=False, vw=1280, vh=720):
    if not vis(n) or r(n)["width"] <= 0 or r(n)["height"] <= 0 or under_clip:
        return
    a = r(n)
    if a["x"] + a["width"] < -EPS or a["y"] + a["height"] < -EPS \
       or a["x"] > vw + EPS or a["y"] > vh + EPS:
        yield "laid-out-offscreen", \
              f"{named(n)} at ({a['x']:.0f},{a['y']:.0f}) {a['width']:.0f}x{a['height']:.0f}"

def check_sibling_overlap(n, parent, screen):
    """Two flow siblings occupying the same main-axis span."""
    kids = flow_children(n)
    if len(kids) < 2 or n["flex_direction"] not in ("Row", "Column"):
        return
    row = n["flex_direction"] == "Row"
    pos, size = ("x", "width") if row else ("y", "height")
    lead_edge, tail_edge = ("left", "right") if row else ("top", "bottom")
    ordered = sorted(kids, key=lambda c: r(c)[pos])
    for a, b in zip(ordered, ordered[1:]):
        ra, rb = r(a), r(b)
        if ra[size] <= 0 or rb[size] <= 0:
            continue
        # A negative margin is the caller deliberately pulling a sibling over
        # its neighbour (toggle knobs, badges). Overlap is the request.
        if min(a["margin"][lead_edge], a["margin"][tail_edge],
               b["margin"][lead_edge], b["margin"][tail_edge]) < 0:
            continue
        ov = (ra[pos] + ra[size]) - rb[pos]
        if ov > EPS:
            yield "flow-siblings-overlap", \
                  f"in {named(n)} ({n['flex_direction']}): {named(a)} and {named(b)} overlap {ov:.0f}px"

def check_gap(n, parent, screen):
    """Declared gap not present between adjacent flow siblings."""
    gap = n.get("gap") or 0
    kids = flow_children(n)
    if gap <= 0 or len(kids) < 2 or n["flex_direction"] not in ("Row", "Column"):
        return
    row = n["flex_direction"] == "Row"
    pos, size = ("x", "width") if row else ("y", "height")
    ordered = sorted(kids, key=lambda c: r(c)[pos])
    # Only meaningful when the container packs at the start; the space-*
    # justify modes legitimately add more.
    if n["justify_content"] not in ("FlexStart", "FlexEnd"):
        return
    lead_edge, tail_edge = ("left", "right") if row else ("top", "bottom")
    for a, b in zip(ordered, ordered[1:]):
        # Margins stack with gap, in afterhours as in CSS, so the expected
        # spacing is gap PLUS the facing margins -- not gap alone.
        expect = gap + a["margin"][tail_edge] + b["margin"][lead_edge]
        actual = r(b)[pos] - (r(a)[pos] + r(a)[size])
        if abs(actual - expect) > max(1.5, expect * 0.25):
            yield "gap-not-applied", \
                  f"in {named(n)}: expected {expect:g}px (gap {gap:g}+margins), " \
                  f"got {actual:.0f}px between {named(a)}/{named(b)}"
            return  # one report per container

def check_justify(n, parent, screen):
    """justify_content that made no difference to where children landed."""
    j = n["justify_content"]
    kids = flow_children(n)
    if j == "FlexStart" or len(kids) < 2 or n["flex_direction"] not in ("Row", "Column"):
        return
    row = n["flex_direction"] == "Row"
    pos, size = ("x", "width") if row else ("y", "height")
    lead_edge, tail_edge = ("left", "right") if row else ("top", "bottom")
    pad = n["padding"][lead_edge]
    inner = r(n)[size] - pad - n["padding"][tail_edge]
    # Gaps and child margins are consumed space, not slack. Leaving either out
    # made every gapped or margined container look like justify was ignored --
    # the dialog button rows were flush right all along.
    used = (sum(r(c)[size] + c["margin"][lead_edge] + c["margin"][tail_edge]
                for c in kids)
            + (n.get("gap") or 0) * (len(kids) - 1))
    slack = inner - used
    if slack <= 2:
        return  # nothing to distribute; any mode looks the same
    ordered = sorted(kids, key=lambda c: r(c)[pos])
    # Measure to the first child's margin edge, matching `used`.
    lead = (r(ordered[0])[pos] - ordered[0]["margin"][lead_edge]) - (r(n)[pos] + pad)
    expect = {"FlexEnd": slack, "Center": slack / 2,
              "SpaceBetween": 0, "SpaceAround": slack / (2 * len(kids))}.get(j)
    if expect is None:
        return
    if abs(lead - expect) > max(2.0, slack * 0.1):
        yield "justify-content-ignored", \
              f"{named(n)} {j}: leading space {lead:.0f}px, expected {expect:.0f}px (slack {slack:.0f})"

def check_label_no_room(n, parent, screen):
    """A visible label in a box with no space to draw it."""
    if not n.get("label") or not vis(n):
        return
    a = r(n)
    if a["width"] <= 1 or a["height"] <= 1:
        yield "label-box-has-no-size", \
              f"{named(n)} {a['width']:.0f}x{a['height']:.0f} holds {n['label'][:32]!r}"

CHECKS = [check_escapes, check_collapsed, check_offscreen, check_sibling_overlap,
          check_gap, check_justify, check_label_no_room]

def main():
    detail = None
    if "--detail" in sys.argv:
        detail = sys.argv[sys.argv.index("--detail") + 1]

    by_check = defaultdict(lambda: {"screens": defaultdict(list), "n": 0})
    files = sorted(TREES.glob("*.json"))
    if not files:
        sys.exit("no dumps -- run ./mocks/build.sh --no-serve first")

    for f in files:
        doc = json.load(f.open())
        screen = doc.get("screen", f.stem)
        vw, vh = doc["viewport"]["width"], doc["viewport"]["height"]
        for root in doc["tree"]:
            for n, parent, under_clip in nodes(root):
                for chk in CHECKS:
                    kw = ({"vw": vw, "vh": vh, "under_clip": under_clip}
                          if chk is check_offscreen else {})
                    for label, det in chk(n, parent, screen, **kw) or ():
                        e = by_check[label]
                        e["screens"][screen].append(det)
                        e["n"] += 1

    ranked = sorted(by_check.items(), key=lambda kv: (-len(kv[1]["screens"]), -kv[1]["n"]))

    if detail:
        e = by_check.get(detail)
        if not e:
            sys.exit(f"no such check: {detail}")
        for s in sorted(e["screens"]):
            print(f"\n## {s}")
            for d in e["screens"][s][:12]:
                print("   ", d)
        return

    print(f"{len(files)} screens scanned\n")
    print(f"{'check':28} {'screens':>8} {'nodes':>7}   worst screens")
    print("-" * 96)
    for label, e in ranked:
        top = sorted(e["screens"].items(), key=lambda kv: -len(kv[1]))[:4]
        print(f"{label:28} {len(e['screens']):>8} {e['n']:>7}   "
              + ", ".join(f"{s}({len(v)})" for s, v in top))

if __name__ == "__main__":
    main()
