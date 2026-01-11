#pragma once

#include <afterhours/src/plugins/translation.h>
#include <afterhours/src/plugins/ui/theme.h>

namespace afterhours {
namespace ui {

// Theme presets for the UI showcase
// Each preset includes: font, darkfont, font_muted, background, surface,
// primary, secondary, accent, error
// And now also: language_fonts for per-language font configuration

namespace theme_presets {

// Font name constants (must match keys used in FontManager::load_font)
inline constexpr const char *FONT_ENGLISH = "Gaegu-Bold";
inline constexpr const char *FONT_KOREAN = "NotoSansKR";
inline constexpr const char *FONT_JAPANESE = "Sazanami";

// Helper to add default language fonts to a theme
inline void add_default_language_fonts(Theme &theme) {
  using Lang = translation::Language;
  theme.language_fonts = {
      {Lang::English, FontConfig{FONT_ENGLISH, 1.0f}},
      {Lang::Korean,
       FontConfig{FONT_KOREAN, 0.95f}}, // Korean renders slightly larger
      {Lang::Japanese,
       FontConfig{FONT_JAPANESE, 0.92f}}, // Japanese needs slight scaling
  };
}

// Cozy Kraft - Warm, paper-like aesthetic for cozy/casual games
// Matches "Blossom Cafe" inspiration image
inline Theme cozy_kraft() {
  Theme t(Color{62, 44, 35, 255},    // font - dark sepia brown (contrast 7.5:1)
          Color{245, 240, 230, 255}, // darkfont - cream (for dark backgrounds)
          Color{100, 85, 72, 255},   // font_muted - medium brown (contrast 4.6:1)
          Color{215, 195, 175, 255}, // background - kraft paper tan
          Color{255, 250, 242, 255}, // surface - warm cream white
          Color{125, 105, 85, 255},  // primary - warm brown (panel titles)
          Color{165, 200, 165, 255}, // secondary - sage/mint green (menu buttons)
          Color{225, 165, 165, 255}, // accent - dusty rose/pink (serve button)
          Color{140, 50, 50, 255}    // error - darker red (contrast 5.5:1)
  );
  t.roundness = 0.15f; // Subtle rounding to match cozy aesthetic
  add_default_language_fonts(t);
  return t;
}

// Cozy Kraft border color - matches panel outlines
inline Color cozy_border_color() { return Color{139, 119, 101, 255}; }

// Neon Dark - Cyberpunk/tactical shooter aesthetic
// All colors meet WCAG AA 4.5:1 contrast with light text
inline Theme neon_dark() {
  Theme t(
      Color{230, 250, 255, 255}, // font - cool white (readable on dark)
      Color{10, 15, 20, 255}, // darkfont - near black (for bright backgrounds)
      Color{100, 120, 140, 255}, // font_muted - cool gray
      Color{6, 8, 14, 255},      // background - deep blue-black
      Color{14, 18, 28, 255},    // surface - dark blue-gray
      Color{0, 75, 95, 255},   // primary - dark teal (contrast 5.8:1)
      Color{50, 30, 95, 255},  // secondary - dark purple (contrast 5.5:1)
      Color{140, 25, 60, 255}, // accent - dark magenta (contrast 5.0:1)
      Color{155, 40, 40, 255}  // error - dark red (contrast 5.5:1)
  );
  add_default_language_fonts(t);
  return t;
}

// Ocean Navy - Professional/business/tycoon aesthetic
  inline Theme ocean_navy() {
    Theme t(Color{255, 255, 255, 255}, // font - white
            Color{15, 30, 55, 255},    // darkfont - deep navy
            Color{160, 175, 200, 255}, // font_muted - light blue-gray
            Color{15, 30, 55, 255},    // background - deep navy
            Color{25, 45, 80, 255},    // surface - navy
            Color{25, 70, 145, 255},   // primary - dark blue (contrast 5.8:1)
            Color{12, 95, 42, 255},    // secondary - dark green (contrast 5.3:1)
            Color{130, 95, 0, 255},    // accent - dark amber (contrast 5.0:1)
            Color{165, 35, 35, 255}    // error - dark red (contrast 5.5:1)
    );
    add_default_language_fonts(t);
    return t;
  }

// Midnight - Dark, dramatic aesthetic
inline Theme midnight() {
  Theme t(Color{255, 255, 255, 255}, // font - white
          Color{0, 0, 0, 255},       // darkfont - pure black
          Color{120, 120, 120, 255}, // font_muted - gray
          Color{0, 0, 0, 255},       // background - pure black
          Color{20, 20, 20, 255},    // surface - near black
          Color{185, 28, 28, 255},   // primary - deep red
          Color{100, 100, 100, 255}, // secondary - dark gray
          Color{255, 214, 10, 255},  // accent - yellow
          Color{255, 100, 100, 255}  // error - light red
  );
  add_default_language_fonts(t);
  return t;
}

// Sage Natural - Organic, nature-inspired aesthetic
// Adjusted for WCAG AA compliance (4.5:1 contrast)
inline Theme sage_natural() {
  Theme t(Color{40, 50, 60, 255},    // font - darker charcoal (contrast 6.5:1)
          Color{212, 229, 210, 255}, // darkfont - sage green
          Color{80, 90, 100, 255},   // font_muted - darker gray (contrast 4.5:1)
          Color{212, 229, 210, 255}, // background - sage green
          Color{250, 250, 249, 255}, // surface - warm white
          Color{45, 90, 45, 255},    // primary - darker forest green (contrast 5.0:1)
          Color{110, 70, 30, 255},   // secondary - darker brown (contrast 5.2:1)
          Color{160, 85, 55, 255},   // accent - darker terracotta (contrast 4.6:1)
          Color{160, 25, 25, 255}    // error - darker red (contrast 5.8:1)
  );
  add_default_language_fonts(t);
  return t;
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

} // namespace theme_presets

} // namespace ui
} // namespace afterhours
