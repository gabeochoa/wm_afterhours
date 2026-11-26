# Artwork Atlas + Frames local asset plugin

WM-local plugin (Batch P-A). Not afterhours upstream.

## Purpose

Single-texture atlas + named sprite frames for local screens. An `Atlas`
loads one texture from `resources/atlases/<name>` and pairs it with source
rectangles to make `Sprite`s; frame rectangles are `constexpr` so named
metadata is deterministic on every run and platform.

## Public API

Canonical headers (in this directory):

- `artwork_atlas.h` — `include "plugins/artwork/artwork_atlas.h"`
  - `artwork::Atlas` — non-copyable; `load(name)` (no-op if already loaded,
    bilinear filtering), `sprite(source)` returns `{texture, source}`,
    unloads on destruction (window-ready or headless only).
  - `artwork::Sprite` — `{raylib::Texture2D texture; raylib::Rectangle source;}`.
  - `artwork::plugin::descriptor` — name, version, `resource_dir="atlases"`,
    `frames_deterministic`; `Atlas`/`Sprite` re-exported under `artwork::plugin::`.
- `artwork_frames.h` — `include "plugins/artwork/artwork_frames.h"`
  - `artwork::cafe_frames` — 5 × 400×400 rects at y = 2, 406, 810, 1214, 1618.
  - `artwork::icons_frames` — 5 × 100×100 rects at y = 2, 106, 210, 314, 418.
  - `artwork::cafe_frame_names` = `star_filled, star_empty, flower, inventory, crafting`;
    `artwork::icons_frame_names` = `gear, star, trophy, home, fast_forward`
    (same index = same frame as the arrays).
  - `artwork::cafe_frame(i)`, `artwork::icon_frame(i)` — named accessors
    returning exactly `cafe_frames[i]` / `icons_frames[i]`.

Compatibility: `src/artwork_atlas.h` and `src/artwork_frames.h` are shims
including these headers; positional `cafe_frames[i]` / `icons_frames[i]` use
is unchanged.

## Current consumers

- `src/systems/screens/CozyCafe.h` (`cozy_cafe` screen) — `cafe.png` via
  `cafe_frames` (stars, flower, inventory, crafting icons).
- `src/systems/screens/ImageShowcase.h` (`images` screen) — `icons.png` via
  `icons_frames` (sprites, image buttons, icon rows).
- `tests/artwork_frames_plugin_test.cpp` — descriptor + named-metadata
  determinism (names parallel to arrays, exact rect values).

## Comparison with afterhours `texture_manager` (read-only)

Vendor `afterhours::texture_manager` was read for comparison only (nothing
copied, nothing changed there). It is ECS-based: `HasSprite`/`HasAnimation`
components plus `RenderSprites`/`RenderAnimation` systems sampling a uniform
grid (`idx_to_sprite_frame` by cell indices), with `AnimationUpdateCurrentFrame`
for looping. This local plugin is deliberately simpler and different: one
texture per `Atlas`, explicit per-frame source rects (not a uniform grid),
`Sprite`s consumed by the UI `sprite()`/`image_button()` primitives, no ECS
components or render systems, deterministic named metadata. If afterhours
gains a first-class atlas plugin it would be a different abstraction, not a
drop-in promotion of this one.

## Tests

```bash
clang++ -std=c++23 -I vendor -Isrc $(pkg-config --cflags raylib) \
  tests/artwork_frames_plugin_test.cpp
```

Targeted e2e: `129_cozy_cafe.e2e`, `19_image_showcase.e2e`.

## Afterhours promotion gate (future candidate only)

No afterhours changes in this batch. Future candidate only after this plugin
holds stable across the full local suite, targeted e2e (CozyCafe,
ImageShowcase), and headless screenshot baselines. Not claimed upstream-ready;
any promotion is a separate proposal with its own review.
