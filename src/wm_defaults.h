#pragma once

#include <afterhours/src/plugins/ui/theme.h>

// WM's adopted defaults, stamped over the library defaults on every theme
// this app builds. Library defaults upstream stay untouched; a value proven
// here is a candidate for promotion to an afterhours default later.
inline void apply_wm_defaults(afterhours::ui::Theme &theme) {
  theme.click_activation_mode = afterhours::ui::ClickActivationMode::Release;
  theme.focus_ring_thickness = 2.0f;
  theme.focus_ring_offset = -3.0f;
  theme.text_area_line_height_ratio = 1.45f;
}
