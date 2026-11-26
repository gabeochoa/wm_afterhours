#pragma once

#include "../../plugins/font_registry/font_config.h"
#include "../../plugins/theme_pack/theme_presets.h"
#include "../../plugins/theme_pack/wm_defaults.h"
#include "../../theme_presets.h"
#include "../test_macros.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/translation.h>
#include <afterhours/src/plugins/ui/theme.h>
#include <stdexcept>

using namespace afterhours::translation;
using namespace afterhours::ui;

// Helper to assert conditions in tests
inline void assert_true(bool condition, const std::string &message) {
  if (!condition) {
    throw std::runtime_error(message);
  }
}

// Test that FontConfig struct works correctly
TEST(fontconfig_struct) {
  // Test default constructor
  FontConfig default_config;
  assert_true(default_config.font_name.empty(),
              "Default FontConfig should have empty font_name");
  assert_true(default_config.size_scale == 1.0f,
              "Default FontConfig should have size_scale of 1.0");

  // Test parameterized constructor
  FontConfig custom_config("TestFont", 0.85f);
  assert_true(custom_config.font_name == "TestFont",
              "FontConfig should store font_name");
  assert_true(custom_config.size_scale == 0.85f,
              "FontConfig should store size_scale");

  co_return;
}

// Test that Theme has font configuration fields
TEST(theme_font_fields) {
  Theme theme;

  // Check default font sizes from FontSizing
  assert_true(theme.font_size_sm() == 14.f,
              "Theme should have default font_size_sm of 14");
  assert_true(theme.font_size_md() == 20.f,
              "Theme should have default font_size_md of 20");
  assert_true(theme.font_size_lg() == 28.f,
              "Theme should have default font_size_lg of 28");
  assert_true(theme.font_size_xl() == 38.f,
              "Theme should have default font_size_xl of 38");

  // Check language_fonts map exists and is empty by default
  assert_true(theme.language_fonts.empty(),
              "Theme language_fonts should be empty by default");

  co_return;
}

// Test Theme::get_font_config with populated language_fonts
TEST(theme_get_font_config) {
  Theme theme;
  theme.language_fonts = {
      {Language::English, FontConfig{"English-Font", 1.0f}},
      {Language::Korean, FontConfig{"Korean-Font", 0.95f}},
      {Language::Japanese, FontConfig{"Japanese-Font", 0.9f}},
  };

  // Test getting config for each language
  auto en_config = theme.get_font_config(Language::English);
  assert_true(en_config.font_name == "English-Font",
              "Should get correct English font name");
  assert_true(en_config.size_scale == 1.0f,
              "Should get correct English size_scale");

  auto ko_config = theme.get_font_config(Language::Korean);
  assert_true(ko_config.font_name == "Korean-Font",
              "Should get correct Korean font name");
  assert_true(ko_config.size_scale == 0.95f,
              "Should get correct Korean size_scale");

  auto ja_config = theme.get_font_config(Language::Japanese);
  assert_true(ja_config.font_name == "Japanese-Font",
              "Should get correct Japanese font name");
  assert_true(ja_config.size_scale == 0.9f,
              "Should get correct Japanese size_scale");

  co_return;
}

// Test Theme::get_scaled_font_size
TEST(theme_get_scaled_font_size) {
  Theme theme;
  theme.language_fonts = {
      {Language::English, FontConfig{"Font", 1.0f}},
      {Language::Korean, FontConfig{"Font", 0.8f}},
  };

  float base_size = 20.0f;

  float en_size = theme.get_scaled_font_size(Language::English, base_size);
  assert_true(en_size == 20.0f, "English scaled size should be 20 (20 * 1.0)");

  float ko_size = theme.get_scaled_font_size(Language::Korean, base_size);
  assert_true(ko_size == 16.0f, "Korean scaled size should be 16 (20 * 0.8)");

  co_return;
}

// Test Theme::get_font_name convenience method
TEST(theme_get_font_name) {
  Theme theme;
  theme.language_fonts = {
      {Language::English, FontConfig{"MyEnglishFont", 1.0f}},
      {Language::Korean, FontConfig{"MyKoreanFont", 0.95f}},
  };

  assert_true(theme.get_font_name(Language::English) == "MyEnglishFont",
              "get_font_name should return correct English font");
  assert_true(theme.get_font_name(Language::Korean) == "MyKoreanFont",
              "get_font_name should return correct Korean font");

  co_return;
}

// Test theme presets include font configurations
TEST(theme_presets_have_fonts) {
  auto cozy = theme_presets::cozy_kraft();
  assert_true(!cozy.language_fonts.empty(),
              "cozy_kraft should have language_fonts configured");
  assert_true(cozy.language_fonts.contains(Language::English),
              "cozy_kraft should have English font");
  assert_true(cozy.language_fonts.contains(Language::Korean),
              "cozy_kraft should have Korean font");
  assert_true(cozy.language_fonts.contains(Language::Japanese),
              "cozy_kraft should have Japanese font");

  auto neon = theme_presets::neon_dark();
  assert_true(!neon.language_fonts.empty(),
              "neon_dark should have language_fonts configured");

  auto ocean = theme_presets::ocean_navy();
  assert_true(!ocean.language_fonts.empty(),
              "ocean_navy should have language_fonts configured");

  auto mid = theme_presets::midnight();
  assert_true(!mid.language_fonts.empty(),
              "midnight should have language_fonts configured");

  auto sage = theme_presets::sage_natural();
  assert_true(!sage.language_fonts.empty(),
              "sage_natural should have language_fonts configured");

  co_return;
}

// Test presets pin surface_raised/border/border_muted to the colours Theme's
// fallback math (theme.h) derived for them, so screens reading the optionals
// render exactly what they rendered before the optionals were set.
TEST(theme_presets_pin_derived_surfaces) {
  const auto eq = [](afterhours::Color a, afterhours::Color b) {
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
  };
  for (const Theme &preset : {theme_presets::cozy_kraft(),
                              theme_presets::neon_dark(),
                              theme_presets::ocean_navy(),
                              theme_presets::midnight(),
                              theme_presets::sage_natural()}) {
    assert_true(preset.surface_raised.has_value() && preset.border.has_value() &&
                    preset.border_muted.has_value(),
                "every preset must pin surface_raised/border/border_muted");
    Theme derived = preset;
    derived.surface_raised.reset();
    derived.border.reset();
    derived.border_muted.reset();
    assert_true(eq(preset.raised_surface(), derived.raised_surface()),
                "pinned surface_raised must equal the derived fallback");
    assert_true(eq(preset.control_border(), derived.control_border()),
                "pinned border must equal the derived fallback");
    assert_true(eq(preset.subtle_border(), derived.subtle_border()),
                "pinned border_muted must equal the derived fallback");
  }
  co_return;
}

// Test that preset fonts have expected names from theme_presets constants
TEST(theme_presets_font_names) {
  auto theme = theme_presets::ocean_navy();

  auto en_config = theme.get_font_config(Language::English);
  assert_true(en_config.font_name == theme_presets::FONT_ENGLISH,
              "English font should match FONT_ENGLISH constant");

  auto ko_config = theme.get_font_config(Language::Korean);
  assert_true(ko_config.font_name == theme_presets::FONT_KOREAN,
              "Korean font should match FONT_KOREAN constant");

  auto ja_config = theme.get_font_config(Language::Japanese);
  assert_true(ja_config.font_name == theme_presets::FONT_JAPANESE,
              "Japanese font should match FONT_JAPANESE constant");

  co_return;
}

// Batch 2 Lane F: canonical plugin headers compile directly and the
// constants / codepoint packs are unchanged from the pre-move values.
TEST(plugin_canonical_theme_font_identity) {
  // theme_pack canonical constants (via the canonical header, not the shim).
  assert_true(theme_presets::FONT_ENGLISH == std::string("Gaegu-Bold"),
              "canonical FONT_ENGLISH must stay Gaegu-Bold");
  assert_true(theme_presets::FONT_KOREAN == std::string("NotoSansKR"),
              "canonical FONT_KOREAN must stay NotoSansKR");
  assert_true(theme_presets::FONT_JAPANESE == std::string("Sazanami"),
              "canonical FONT_JAPANESE must stay Sazanami");
  assert_true(theme_presets::THEME_CORNER_RADIUS == 10.f,
              "canonical THEME_CORNER_RADIUS must stay 10px");

  // wm_defaults canonical stamps.
  Theme stamped;
  apply_wm_defaults(stamped);
  assert_true(stamped.click_activation_mode ==
                  afterhours::ui::ClickActivationMode::Release,
              "wm_defaults must stay Release activation");
  assert_true(stamped.focus_ring_thickness == 2.0f,
              "wm_defaults focus_ring_thickness must stay 2.0");
  assert_true(stamped.focus_ring_offset == -3.0f,
              "wm_defaults focus_ring_offset must stay -3.0");
  assert_true(stamped.text_area_line_height_ratio == 1.45f,
              "wm_defaults line height must stay 1.45");

  // font_registry canonical codepoint packs: exact sizes pin the ranges.
  const auto kr = font_config::get_korean_codepoints();
  assert_true(kr.size() == 11632,
              "korean codepoint count must stay 11632");
  assert_true(kr.front() == 0x0020 && kr.back() == 0xFF60,
              "korean codepoints must span ASCII..fullwidth");
  const auto jp = font_config::get_japanese_codepoints();
  assert_true(jp.size() == 21376,
              "japanese codepoint count must stay 21376");
  assert_true(jp.front() == 0x0020 && jp.back() == 0xFF60,
              "japanese codepoints must span ASCII..fullwidth");
  assert_true(font_config::get_language_japanese_codepoints().size() == 133,
              "language-japanese pack must stay 133");
  assert_true(font_config::get_inline_prompt_codepoints().size() == 120,
              "inline-prompt pack must stay 120");

  // Registry + alias table unchanged (names and mock raster sizes).
  const auto &fonts = font_config::get_all_fonts();
  bool has_atkinson_mock = false;
  bool has_korean_32 = false;
  for (const auto &def : fonts) {
    if (def.name == "AtkinsonMock")
      has_atkinson_mock = def.raster_size == 192;
    if (def.name == "NotoSansKR")
      has_korean_32 = def.raster_size == 32;
  }
  assert_true(has_atkinson_mock,
              "AtkinsonMock must stay at raster 192");
  assert_true(has_korean_32, "NotoSansKR must stay at raster 32");
  assert_true(sizeof(font_config::aliases) / sizeof(font_config::aliases[0]) == 6,
              "alias table must stay 6 entries");

  co_return;
}
