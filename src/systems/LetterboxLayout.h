#pragma once

#include <afterhours/ah.h>

// Local WM letterbox helper (keep-local; migrate later upstream-API-first).
// Upstream `window_manager::content_viewport` /
// `graphics::letterbox_content_size` letterbox against a fixed singleton
// resolution, not against caller-supplied render-texture dimensions, so this
// explicit-size helper stays local. The only local fix here is a degenerate-size
// guard: minimized (0x0) windows previously divided by content width/height
// unchecked (inf/NaN rects; static_cast of inf is UB). Degenerate inputs now
// return an empty layout (see tests/letterbox_test.cpp).
struct LetterboxLayout {
  int bar_left = 0;
  int bar_right = 0;
  int bar_top = 0;
  int bar_bottom = 0;
  raylib::Rectangle dst{0.f, 0.f, 0.f, 0.f};
};

static inline LetterboxLayout
compute_letterbox_layout(const int window_width, const int window_height,
                         const int content_width, const int content_height) {
  LetterboxLayout layout;
  if (window_width <= 0 || window_height <= 0 || content_width <= 0 ||
      content_height <= 0) {
    // Empty layout: nothing to present into. Never divide by zero (the
    // minimized-window case). RenderRenderTexture draws layout.dst, which is
    // a 0x0 rect here -- a no-op draw, not an inf/NaN rect.
    return layout;
  }
  int dest_w = window_width;
  int dest_h = static_cast<int>(
      std::round(static_cast<double>(dest_w) * content_height / content_width));
  if (dest_h > window_height) {
    dest_h = window_height;
    dest_w = static_cast<int>(std::round(static_cast<double>(dest_h) *
                                         content_width / content_height));
  }

  const int bar_w_total = window_width - dest_w;
  const int bar_h_total = window_height - dest_h;
  layout.bar_left = bar_w_total / 2;
  layout.bar_right = bar_w_total - layout.bar_left;
  layout.bar_top = bar_h_total / 2;
  layout.bar_bottom = bar_h_total - layout.bar_top;
  layout.dst = raylib::Rectangle{
      static_cast<float>(layout.bar_left), static_cast<float>(layout.bar_top),
      static_cast<float>(dest_w), static_cast<float>(dest_h)};
  return layout;
}
