#pragma once

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

  // Check default font sizes exist
  assert_true(theme.font_size_sm == 14.f,
              "Theme should have default font_size_sm of 14");
  assert_true(theme.font_size_md == 18.f,
              "Theme should have default font_size_md of 18");
  assert_true(theme.font_size_lg == 28.f,
              "Theme should have default font_size_lg of 28");
  assert_true(theme.font_size_xl == 36.f,
              "Theme should have default font_size_xl of 36");

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
