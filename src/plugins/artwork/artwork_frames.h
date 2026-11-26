#pragma once

// Local WM plugin: Artwork Atlas + Frames (Batch P-A, WM-local only).
// See artwork_atlas.h in this directory for the plugin overview.
//
// Named frame metadata is deterministic: every rectangle below is
// `constexpr`, never computed at runtime, so frame indices, names, and source
// rects are identical on every run and platform. Existing positional uses
// (cafe_frames[i], icons_frames[i]) are unchanged; the named accessors add a
// self-describing alternative without changing any pixel.

#include "../../external.h"
#include <array>
#include <cassert>
#include <string_view>

namespace artwork {
inline constexpr std::array<raylib::Rectangle, 5> cafe_frames{{
    {2.f, 2.f, 400.f, 400.f},
    {2.f, 406.f, 400.f, 400.f},
    {2.f, 810.f, 400.f, 400.f},
    {2.f, 1214.f, 400.f, 400.f},
    {2.f, 1618.f, 400.f, 400.f},
}};
inline constexpr std::array<raylib::Rectangle, 5> icons_frames{{
    {2.f, 2.f, 100.f, 100.f},
    {2.f, 106.f, 100.f, 100.f},
    {2.f, 210.f, 100.f, 100.f},
    {2.f, 314.f, 100.f, 100.f},
    {2.f, 418.f, 100.f, 100.f},
}};

// Names parallel to the frame arrays above (same index = same frame).
// CozyCafe order: star_filled, star_empty, flower, inventory, crafting.
// ImageShowcase order: gear, star, trophy, home, fast_forward.
inline constexpr std::array<std::string_view, 5> cafe_frame_names{{
    "star_filled", "star_empty", "flower", "inventory", "crafting",
}};
inline constexpr std::array<std::string_view, 5> icons_frame_names{{
    "gear", "star", "trophy", "home", "fast_forward",
}};

static_assert(cafe_frames.size() == cafe_frame_names.size(),
              "cafe frame names must stay parallel to cafe_frames");
static_assert(icons_frames.size() == icons_frame_names.size(),
              "icon frame names must stay parallel to icons_frames");

// Named accessors (preserved values; positional arrays remain canonical).
[[nodiscard]] inline constexpr raylib::Rectangle cafe_frame(std::size_t index) {
  assert(index < cafe_frames.size());
  return cafe_frames[index];
}

[[nodiscard]] inline constexpr raylib::Rectangle icon_frame(std::size_t index) {
  assert(index < icons_frames.size());
  return icons_frames[index];
}

} // namespace artwork
