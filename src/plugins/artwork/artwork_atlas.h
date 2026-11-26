#pragma once

// Local WM plugin: Artwork Atlas + Frames (Batch P-A, WM-local only).
//
// Canonical implementation for the single-texture artwork atlas. The
// historical paths src/artwork_atlas.h and src/artwork_frames.h are now
// compatibility shims that include the headers in this directory, so existing
// consumers (CozyCafe, ImageShowcase) keep working unchanged.
//
// Behavior is unchanged: `Atlas` loads one texture per instance from
// resources/atlases/<name> (via afterhours::files::get_resource_path), applies
// bilinear filtering, and unloads on destruction when a window is ready or in
// headless mode. `sprite(source)` pairs the atlas texture with a source
// rectangle. Frame rectangles live in artwork_frames.h as constexpr values.
//
// Promotion gate: future candidate for afterhours only after this plugin has
// held stable across the local test suite + targeted e2e; no afterhours changes
// in this batch. See README.md in this directory.

#include "../../external.h"
#include <afterhours/src/plugins/files.h>
#include <afterhours/src/graphics.h>

namespace artwork {
struct Sprite {
  raylib::Texture2D texture{};
  raylib::Rectangle source{};
};

class Atlas {
  raylib::Texture2D texture_{};

public:
  Atlas() = default;
  Atlas(const Atlas &) = delete;
  Atlas &operator=(const Atlas &) = delete;

  ~Atlas() {
    if (texture_.id != 0 && (raylib::IsWindowReady() || afterhours::graphics::is_headless()))
      raylib::UnloadTexture(texture_);
  }

  void load(const char *name) {
    if (texture_.id != 0) return;
    texture_ = raylib::LoadTexture(afterhours::files::get_resource_path("atlases", name).string().c_str());
    if (texture_.id == 0) return;
    raylib::SetTextureFilter(texture_, raylib::TEXTURE_FILTER_BILINEAR);
  }

  Sprite sprite(raylib::Rectangle source) const { return {texture_, source}; }
};

// Plugin facade: a named, versioned entry point so future consumers (and a
// possible afterhours promotion) have one stable surface to depend on.
namespace plugin {

using ::artwork::Atlas;
using ::artwork::Sprite;

struct Descriptor {
  const char *name = "artwork_atlas";
  const char *version = "1.0.0";
  const char *resource_dir = "atlases";
  // True: frame rectangles are constexpr (deterministic, never computed).
  bool frames_deterministic = true;
};

inline constexpr Descriptor descriptor{};

} // namespace plugin
} // namespace artwork
