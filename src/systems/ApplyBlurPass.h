#pragma once

#include "../game.h"
#include "../render_backend.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/effects.h>
#include <afterhours/src/plugins/ui.h>

struct ApplyBlurPass : afterhours::System<> {
  std::optional<afterhours::effects::BlurPass> pass;
  virtual ~ApplyBlurPass() {}
  void once(float) override {
    auto &requests = afterhours::ui::blur_requests();
    if (requests.empty()) return;
    if (!pass) pass.emplace();
    if (render_backend::draw_directly_to_window) {
      const Vector2Type screen_px{static_cast<float>(raylib::GetRenderWidth()), static_cast<float>(raylib::GetRenderHeight())};
      const float scale = screen_px.x / static_cast<float>(std::max(1, raylib::GetScreenWidth()));
      for (const auto &r : requests) pass->apply_screen(r.rect, r.radius, scale, screen_px);
    } else {
      for (const auto &r : requests) pass->apply(mainRT, r.rect, r.radius);
    }
    requests.clear();
  }
};
