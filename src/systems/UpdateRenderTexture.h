#pragma once

#include "../game.h"
#include "../render_backend.h"
#include "../settings.h"
#include <afterhours/ah.h>
#include <afterhours/src/graphics.h>
#include <afterhours/src/plugins/window_manager.h>

struct UpdateRenderTexture : afterhours::System<> {
  afterhours::window_manager::Resolution resolution;

  virtual ~UpdateRenderTexture() {}

  void once(float) override {
    const afterhours::window_manager::ProvidesCurrentResolution *pcr =
        afterhours::EntityHelper::get_singleton_cmp<
            afterhours::window_manager::ProvidesCurrentResolution>();
    if (!pcr || pcr->current_resolution == resolution) return;
    resolution = pcr->current_resolution;
    Settings::get().update_resolution(resolution);
    if (afterhours::graphics::is_headless()) {
      // mainRT is owned by the graphics backend in headless mode
      auto &target = afterhours::graphics::get_render_texture();
      raylib::UnloadRenderTexture(target);
      target = raylib::LoadRenderTexture(resolution.width, resolution.height);
      mainRT = target;
    } else {
      raylib::UnloadRenderTexture(mainRT);
      mainRT = raylib::LoadRenderTexture(resolution.width, resolution.height);
    }
    raylib::UnloadRenderTexture(screenRT);
    screenRT = raylib::LoadRenderTexture(resolution.width, resolution.height);
  }
};
