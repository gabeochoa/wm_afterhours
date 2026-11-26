// Batch P-A plugin API hardening test for the Artwork Atlas + Frames plugin.
// Covers the new named/deterministic metadata surface; sprite/atlas behavior
// itself is covered by the CozyCafe + ImageShowcase e2e + baselines.
#include "../src/plugins/artwork/artwork_atlas.h"
#include "../src/plugins/artwork/artwork_frames.h"
#include <cassert>
#include <string_view>

int main() {
  using namespace artwork;

  // Descriptor is stable.
  assert(std::string_view(plugin::descriptor.name) == "artwork_atlas");
  assert(std::string_view(plugin::descriptor.version) == "1.0.0");
  assert(std::string_view(plugin::descriptor.resource_dir) == "atlases");
  assert(plugin::descriptor.frames_deterministic);

  // Named metadata stays parallel to the deterministic constexpr rects.
  assert(cafe_frame_names.size() == cafe_frames.size());
  assert(icons_frame_names.size() == icons_frames.size());
  assert(cafe_frame_names[0] == "star_filled");
  assert(cafe_frame_names[1] == "star_empty");
  assert(cafe_frame_names[2] == "flower");
  assert(cafe_frame_names[3] == "inventory");
  assert(cafe_frame_names[4] == "crafting");
  assert(icons_frame_names[0] == "gear");
  assert(icons_frame_names[1] == "star");
  assert(icons_frame_names[2] == "trophy");
  assert(icons_frame_names[3] == "home");
  assert(icons_frame_names[4] == "fast_forward");

  // Exact rect values (x, y, w, h) per frame.
  const auto check = [](raylib::Rectangle r, float x, float y, float w, float h) {
    assert(r.x == x && r.y == y && r.width == w && r.height == h);
  };
  check(cafe_frame(0), 2.f, 2.f, 400.f, 400.f);
  check(cafe_frame(1), 2.f, 406.f, 400.f, 400.f);
  check(cafe_frame(2), 2.f, 810.f, 400.f, 400.f);
  check(cafe_frame(3), 2.f, 1214.f, 400.f, 400.f);
  check(cafe_frame(4), 2.f, 1618.f, 400.f, 400.f);
  check(icon_frame(0), 2.f, 2.f, 100.f, 100.f);
  check(icon_frame(1), 2.f, 106.f, 100.f, 100.f);
  check(icon_frame(2), 2.f, 210.f, 100.f, 100.f);
  check(icon_frame(3), 2.f, 314.f, 100.f, 100.f);
  check(icon_frame(4), 2.f, 418.f, 100.f, 100.f);

  // Named accessors agree with the positional arrays.
  for (std::size_t i = 0; i < cafe_frames.size(); ++i) {
    const auto one = cafe_frame(i);
    const auto two = cafe_frames[i];
    assert(one.x == two.x && one.y == two.y && one.width == two.width && one.height == two.height);
  }
  for (std::size_t i = 0; i < icons_frames.size(); ++i) {
    const auto one = icon_frame(i);
    const auto two = icons_frames[i];
    assert(one.x == two.x && one.y == two.y && one.width == two.width && one.height == two.height);
  }
}
