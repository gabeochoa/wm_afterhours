from pathlib import Path
from PIL import Image

root = Path(__file__).resolve().parents[1]
folder = root / "resources/images/kirby_options"
names = ["desk", "board", "notebook", "name", "delete", "options_label", "common_label", "message_line"]
names += [f"option_{i}" for i in range(5)]
names += [f"tab_{i}_{state}" for i in range(6) for state in ("off", "on")]
images = {name: Image.open(folder / f"{name}.png").convert("RGBA") for name in names if name not in {"options_label", "common_label", "message_line"}}
free = [(0, 0, 3072, 4096)]
frames = {}
for name, image in sorted(images.items(), key=lambda entry: -max(entry[1].size)):
    width, height = image.width + 4, image.height + 4
    fits = [(y + height, x, i) for i, (x, y, w, h) in enumerate(free) if width <= w and height <= h]
    _, _, slot = min(fits)
    x, y, _, _ = free[slot]
    frames[name] = (x + 2, y + 2, image.width, image.height)
    remaining = []
    for fx, fy, fw, fh in free:
        if x >= fx + fw or x + width <= fx or y >= fy + fh or y + height <= fy:
            remaining.append((fx, fy, fw, fh))
            continue
        if x > fx:
            remaining.append((fx, fy, x - fx, fh))
        if x + width < fx + fw:
            remaining.append((x + width, fy, fx + fw - x - width, fh))
        if y > fy:
            remaining.append((fx, fy, fw, y - fy))
        if y + height < fy + fh:
            remaining.append((fx, y + height, fw, fy + fh - y - height))
    free = [rect for i, rect in enumerate(remaining) if not any(i != j and rect[0] >= other[0] and rect[1] >= other[1] and rect[0] + rect[2] <= other[0] + other[2] and rect[1] + rect[3] <= other[1] + other[3] for j, other in enumerate(remaining))]
height = max(y + h + 2 for x, y, w, h in frames.values())
atlas = Image.new("RGBA", (3072, height))
for name, (x, y, w, h) in frames.items():
    image = images[name]
    atlas.paste(image, (x, y))
    atlas.paste(image.crop((0, 0, 1, h)).resize((2, h)), (x - 2, y))
    atlas.paste(image.crop((w - 1, 0, w, h)).resize((2, h)), (x + w, y))
    atlas.paste(image.crop((0, 0, w, 1)).resize((w, 2)), (x, y - 2))
    atlas.paste(image.crop((0, h - 1, w, h)).resize((w, 2)), (x, y + h))
atlas.save(folder / "atlas.png")
header = '#pragma once\n\n#include <array>\n\ninline constexpr std::array<raylib::Rectangle, 25> kirby_artwork_frames{{\n'
for name in names:
    header += '    {' + ', '.join(f'{value}.f' for value in frames.get(name, (0, 0, 0, 0))) + '},\n'
header += '}};\n'
(root / "src/systems/screens/KirbyArtwork.h").write_text(header)
print(f"Packed {len(images)} images into {atlas.width}x{atlas.height}")
