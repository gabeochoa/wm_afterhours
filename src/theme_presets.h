#pragma once

#include <afterhours/src/plugins/ui/theme.h>
#include <afterhours/src/plugins/ui/theme_defaults.h>

namespace afterhours {
namespace ui {

// Theme presets for the UI showcase
// Each preset includes: font, darkfont, font_muted, background, surface,
// primary, secondary, accent, error

namespace theme_presets {

// Cozy Kraft - Warm, paper-like aesthetic for cozy/casual games
inline Theme cozy_kraft() {
    return Theme(
        Color{92, 64, 51, 255},      // font - sepia brown
        Color{245, 240, 230, 255},   // darkfont - cream (for dark backgrounds)
        Color{139, 119, 101, 255},   // font_muted - light brown
        Color{245, 240, 230, 255},   // background - cream
        Color{255, 250, 240, 255},   // surface - warm white
        Color{196, 167, 125, 255},   // primary - kraft brown
        Color{180, 200, 170, 255},   // secondary - sage
        Color{212, 165, 165, 255},   // accent - dusty rose
        Color{180, 80, 80, 255}      // error - muted red
    );
}

// Neon Dark - Cyberpunk/tactical shooter aesthetic
// Dark enough colors to provide good contrast with light text
inline Theme neon_dark() {
    return Theme(
        Color{230, 250, 255, 255},   // font - cool white (readable on dark)
        Color{10, 15, 20, 255},      // darkfont - near black (for bright backgrounds)
        Color{100, 120, 140, 255},   // font_muted - cool gray
        Color{6, 8, 14, 255},        // background - deep blue-black
        Color{14, 18, 28, 255},      // surface - dark blue-gray
        Color{0, 110, 140, 255},     // primary - dark teal (high contrast with white)
        Color{80, 50, 140, 255},     // secondary - dark purple
        Color{200, 40, 90, 255},     // accent - dark pink/magenta
        Color{200, 60, 60, 255}      // error - muted red
    );
}

// Ocean Navy - Professional/business/tycoon aesthetic
inline Theme ocean_navy() {
    return Theme(
        Color{255, 255, 255, 255},   // font - white
        Color{15, 30, 55, 255},      // darkfont - deep navy
        Color{160, 175, 200, 255},   // font_muted - light blue-gray
        Color{15, 30, 55, 255},      // background - deep navy
        Color{25, 45, 80, 255},      // surface - navy
        Color{59, 130, 246, 255},    // primary - blue
        Color{34, 197, 94, 255},     // secondary - green
        Color{251, 191, 36, 255},    // accent - gold
        Color{239, 68, 68, 255}      // error - red
    );
}

// Midnight - Dark, dramatic aesthetic
inline Theme midnight() {
    return Theme(
        Color{255, 255, 255, 255},   // font - white
        Color{0, 0, 0, 255},         // darkfont - pure black
        Color{120, 120, 120, 255},   // font_muted - gray
        Color{0, 0, 0, 255},         // background - pure black
        Color{20, 20, 20, 255},      // surface - near black
        Color{185, 28, 28, 255},     // primary - deep red
        Color{100, 100, 100, 255},   // secondary - dark gray
        Color{255, 214, 10, 255},    // accent - yellow
        Color{255, 100, 100, 255}    // error - light red
    );
}

// Sage Natural - Organic, nature-inspired aesthetic
inline Theme sage_natural() {
    return Theme(
        Color{55, 65, 81, 255},      // font - charcoal
        Color{212, 229, 210, 255},   // darkfont - sage green
        Color{107, 114, 128, 255},   // font_muted - gray
        Color{212, 229, 210, 255},   // background - sage green
        Color{250, 250, 249, 255},   // surface - warm white
        Color{77, 124, 77, 255},     // primary - forest green
        Color{139, 90, 43, 255},     // secondary - brown
        Color{198, 123, 92, 255},    // accent - terracotta
        Color{185, 28, 28, 255}      // error - deep red
    );
}

// Apply a theme preset globally
inline void apply(const Theme &theme) {
    imm::ThemeDefaults::get().set_theme(theme);
}

// Convenience functions to apply specific presets
inline void apply_cozy_kraft() { apply(cozy_kraft()); }
inline void apply_neon_dark() { apply(neon_dark()); }
inline void apply_ocean_navy() { apply(ocean_navy()); }
inline void apply_midnight() { apply(midnight()); }
inline void apply_sage_natural() { apply(sage_natural()); }

}  // namespace theme_presets

}  // namespace ui
}  // namespace afterhours

