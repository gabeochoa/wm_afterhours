#pragma once

// WORKAROUND: Gradient Backgrounds
// See LIBRARY_GAPS.md #30
//
// This helper approximates gradients by layering multiple semi-transparent
// horizontal strips. Not as smooth as real gradients but provides depth.
//
// Migration: When library adds with_gradient_background(), replace these calls
// with the native implementation.

#include <afterhours/ah.h>

namespace ui_workarounds {

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Renders a vertical gradient (top to bottom) using horizontal strips
template <typename Context, typename Entity>
inline void
vertical_gradient(Context &context, Entity &entity, int base_id, float x,
                  float y, float width, float height,
                  afterhours::Color top_color, afterhours::Color bottom_color,
                  int num_strips = 8 // More strips = smoother gradient
) {
  float strip_height = height / static_cast<float>(num_strips);

  for (int i = 0; i < num_strips; i++) {
    float t = static_cast<float>(i) / static_cast<float>(num_strips - 1);
    afterhours::Color strip_color = afterhours::colors::lerp(top_color, bottom_color, t);

    div(context, mk(entity, base_id + i),
        ComponentConfig{}
            .with_size(ComponentSize{
                pixels(static_cast<int>(width)),
                pixels(static_cast<int>(strip_height + 1))}) // +1 to avoid gaps
            .with_absolute_position()
            .with_translate(x, y + i * strip_height)
            .with_custom_background(strip_color)
            .with_debug_name("gradient_strip_" + std::to_string(i)));
  }
}

// Renders a horizontal gradient (left to right) using vertical strips
template <typename Context, typename Entity>
inline void horizontal_gradient(Context &context, Entity &entity, int base_id,
                                float x, float y, float width, float height,
                                afterhours::Color left_color,
                                afterhours::Color right_color,
                                int num_strips = 8) {
  float strip_width = width / static_cast<float>(num_strips);

  for (int i = 0; i < num_strips; i++) {
    float t = static_cast<float>(i) / static_cast<float>(num_strips - 1);
    afterhours::Color strip_color = afterhours::colors::lerp(left_color, right_color, t);

    div(context, mk(entity, base_id + i),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(static_cast<int>(strip_width + 1)),
                                     pixels(static_cast<int>(height))})
            .with_absolute_position()
            .with_translate(x + i * strip_width, y)
            .with_custom_background(strip_color)
            .with_debug_name("gradient_strip_" + std::to_string(i)));
  }
}

} // namespace ui_workarounds
