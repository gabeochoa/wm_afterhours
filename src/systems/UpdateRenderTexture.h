#pragma once

#include "../game.h"
#include "../render_backend.h"
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
    if (pcr && pcr->current_resolution != resolution) {
      resolution = pcr->current_resolution;
      if (afterhours::graphics::is_headless()) {
        // mainRT is owned by the graphics backend in headless mode
        raylib::UnloadRenderTexture(screenRT);
        screenRT = raylib::LoadRenderTexture(resolution.width,
                                             resolution.height);
      } else {
        raylib::UnloadRenderTexture(mainRT);
        mainRT = raylib::LoadRenderTexture(resolution.width, resolution.height);
        raylib::UnloadRenderTexture(screenRT);
        screenRT = raylib::LoadRenderTexture(resolution.width,
                                             resolution.height);
      }
    }
  }
};
