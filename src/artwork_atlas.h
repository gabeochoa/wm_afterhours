#pragma once

#include "external.h"
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
}
