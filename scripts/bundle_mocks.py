#!/usr/bin/env python3
"""Build the Pixelcloud HTML with all local images and fonts embedded."""

import argparse
import base64
import hashlib
import json
import mimetypes
import re
from pathlib import Path


def bundle(root: Path, destination: Path) -> None:
    html = (root / "mocks.html").read_text()
    assets = {}
    manifest = {}

    def embed(relative: str) -> str:
        if relative not in assets:
            data = (root / relative).read_bytes()
            mime = mimetypes.guess_type(relative)[0] or "application/octet-stream"
            assets[relative] = f"data:{mime};base64,{base64.b64encode(data).decode()}"
            manifest[relative] = hashlib.sha256(data).hexdigest()
        return assets[relative]

    paths = set(re.findall(r"(?:resources|inspiration)/[^\s\"'<>`)]+", html))
    for relative in sorted(paths, key=len, reverse=True):
        html = html.replace(relative, embed(relative))

    screen_ids = sorted(set(re.findall(r"(?:\bid\s*:|^add\()\s*'(\w+)'", html,
                                       flags=re.MULTILINE)))
    if not screen_ids:
        raise ValueError("No mock screens found")
    current = {
        name: embed(f"screenshot-baselines/screens/{name}_720p.png")
        for name in screen_ids
    }
    payload = "<script>window.mockAssets=" + json.dumps(current) + ";</script>\n"
    html = html.replace("<script>window.mockScreens=[];</script>", payload +
                        "<script>window.mockScreens=[];</script>")
    destination.parent.mkdir(parents=True, exist_ok=True)
    destination.write_text(html)
    destination.with_suffix(".assets.json").write_text(json.dumps(manifest, indent=2) + "\n")
    print(f"{destination}: {len(screen_ids)} screens, {len(assets)} assets, "
          f"{destination.stat().st_size / 1024 / 1024:.1f} MiB")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=Path("output/mocks-standalone.html"))
    args = parser.parse_args()
    bundle(Path(__file__).resolve().parents[1], args.output.resolve())
