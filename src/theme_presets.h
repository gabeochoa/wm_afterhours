#pragma once

#include <afterhours/src/plugins/translation.h>
#include <afterhours/src/plugins/ui/theme.h>

namespace afterhours {
namespace ui {

// Theme presets for the UI showcase
// Uses Theme::create() builder with palettes for clean, readable definitions

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
  Theme t = Theme::create()
                .with_palette({
                    .background = {215, 195, 175, 255}, // kraft paper tan
                    .surface = {255, 250, 242, 255},    // warm cream white
                    .primary = {125, 105, 85, 255},     // warm brown (panel titles)
                    .secondary = {165, 200, 165, 255},  // sage/mint green
                    .accent = {225, 165, 165, 255},     // dusty rose/pink
                    .error = {140, 50, 50, 255},        // darker red (5.5:1)
                    .font = {62, 44, 35, 255},          // dark sepia brown (7.5:1)
                    .darkfont = {245, 240, 230, 255},   // cream
                    .font_muted = {100, 85, 72, 255},   // medium brown (4.6:1)
                })
                .with_roundness(0.12f);
  add_default_language_fonts(t);
  return t;
}

// Cozy Kraft border color - matches panel outlines
inline Color cozy_border_color() { return Color{139, 119, 101, 255}; }

// Neon Dark - Cyberpunk/tactical shooter aesthetic
// All colors meet WCAG AA 4.5:1 contrast with light text
inline Theme neon_dark() {
  Theme t = Theme::create().with_palette({
      .background = {6, 8, 14, 255},       // deep blue-black
      .surface = {14, 18, 28, 255},        // dark blue-gray
      .primary = {0, 75, 95, 255},         // dark teal (5.8:1)
      .secondary = {50, 30, 95, 255},      // dark purple (5.5:1)
      .accent = {140, 25, 60, 255},        // dark magenta (5.0:1)
      .error = {155, 40, 40, 255},         // dark red (5.5:1)
      .font = {230, 250, 255, 255},        // cool white
      .darkfont = {10, 15, 20, 255},       // near black
      .font_muted = {140, 160, 180, 255},  // cool gray (4.8:1)
  });
  add_default_language_fonts(t);
  return t;
}

// Ocean Navy - Professional/business/tycoon aesthetic
inline Theme ocean_navy() {
  Theme t = Theme::create().with_palette({
      .background = {15, 30, 55, 255},     // deep navy
      .surface = {25, 45, 80, 255},        // navy
      .primary = {25, 70, 145, 255},       // dark blue (5.8:1)
      .secondary = {12, 95, 42, 255},      // dark green (5.3:1)
      .accent = {130, 95, 0, 255},         // dark amber (5.0:1)
      .error = {165, 35, 35, 255},         // dark red (5.5:1)
      .font = {255, 255, 255, 255},        // white
      .darkfont = {15, 30, 55, 255},       // deep navy
      .font_muted = {160, 175, 200, 255},  // light blue-gray
  });
  add_default_language_fonts(t);
  return t;
}

// Midnight - Dark, dramatic aesthetic
inline Theme midnight() {
  Theme t = Theme::create().with_palette({
      .background = {0, 0, 0, 255},        // pure black
      .surface = {20, 20, 20, 255},        // near black
      .primary = {185, 28, 28, 255},       // deep red
      .secondary = {100, 100, 100, 255},   // dark gray
      .accent = {255, 214, 10, 255},       // yellow
      .error = {255, 100, 100, 255},       // light red
      .font = {255, 255, 255, 255},        // white
      .darkfont = {0, 0, 0, 255},          // pure black
      .font_muted = {140, 140, 140, 255},  // lighter gray (5.3:1)
  });
  add_default_language_fonts(t);
  return t;
}

// Sage Natural - Organic, nature-inspired aesthetic
// Adjusted for WCAG AA compliance (4.5:1 contrast)
inline Theme sage_natural() {
  Theme t = Theme::create().with_palette({
      .background = {212, 229, 210, 255},  // sage green
      .surface = {250, 250, 249, 255},     // warm white
      .primary = {45, 90, 45, 255},        // darker forest green (5.0:1)
      .secondary = {110, 70, 30, 255},     // darker brown (5.2:1)
      .accent = {160, 85, 55, 255},        // darker terracotta (4.6:1)
      .error = {160, 25, 25, 255},         // darker red (5.8:1)
      .font = {40, 50, 60, 255},           // darker charcoal (6.5:1)
      .darkfont = {212, 229, 210, 255},    // sage green
      .font_muted = {80, 90, 100, 255},    // darker gray (4.5:1)
  });
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
