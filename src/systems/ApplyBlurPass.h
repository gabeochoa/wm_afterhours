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
    if (render_backend::draw_directly_to_window) { requests.clear(); return; }
    if (!pass) pass.emplace();
    for (const auto &r : requests) pass->apply(mainRT, r.rect, r.radius);
    requests.clear();
  }
};
