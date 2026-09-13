import argparse
import json
from pathlib import Path
from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
GROUPS = {
    "cafe": ("images/cozy_cafe", ["star", "star_empty", "flower", "inventory", "crafting"]),
    "icons": ("kenney/kenney_game-icons/PNG/White/2x", ["gear", "star", "trophy", "home", "fastForward"]),
}


def build(name, folder, names, check):
    sources = [Image.open(ROOT / "resources" / folder / f"{stem}.png").convert("RGBA") for stem in names]
    width = max(image.width + 4 for image in sources)
    height = sum(image.height + 4 for image in sources)
    atlas = Image.new("RGBA", (width, height))
    frames = []
    y = 2
    for image in sources:
        w, h = image.size
        atlas.paste(image, (2, y))
        for dx in range(-2, w + 2):
            for dy in (-2, -1, h, h + 1):
                atlas.putpixel((dx + 2, y + dy), image.getpixel((min(max(dx, 0), w - 1), min(max(dy, 0), h - 1))))
        for dx in (-2, -1, w, w + 1):
            atlas.paste(image.crop((min(max(dx, 0), w - 1), 0, min(max(dx, 0), w - 1) + 1, h)), (dx + 2, y))
        frames.append((2, y, w, h))
        y += h + 4
    path = ROOT / "resources/atlases" / f"{name}.png"
    if check:
        with Image.open(path) as existing:
            assert existing.size == atlas.size and existing.convert("RGBA").tobytes() == atlas.tobytes(), path
    else:
        atlas.save(path)
    print(json.dumps({"atlas": name, "source_textures": len(sources), "atlas_textures": 1,
                      "source_pixels": sum(im.width * im.height for im in sources),
                      "atlas_pixels": width * height, "pixel_preserving": True}))
    return frames


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    header = '#pragma once\n\n#include "external.h"\n#include <array>\n\nnamespace artwork {\n'
    for name, (folder, names) in GROUPS.items():
        frames = build(name, folder, names, args.check)
        header += f'inline constexpr std::array<raylib::Rectangle, {len(frames)}> {name}_frames{{{{\n'
        for frame in frames:
            header += '    {' + ', '.join(f'{value}.f' for value in frame) + '},\n'
        header += '}};\n'
    header += '}\n'
    path = ROOT / 'src/artwork_frames.h'
    if args.check:
        assert path.read_text() == header, path
        return
    path.write_text(header)


if __name__ == '__main__':
    main()
