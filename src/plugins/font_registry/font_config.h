#pragma once

#include <functional>
#include <string>
#include <vector>

namespace font_config {

// Generate codepoint array for a Unicode range
inline std::vector<int> generate_codepoint_range(int start, int end) {
  std::vector<int> codepoints;
  for (int i = start; i <= end; ++i) {
    codepoints.push_back(i);
  }
  return codepoints;
}

// Build codepoints for Korean: Hangul Syllables + Hangul Jamo + ASCII
inline std::vector<int> get_korean_codepoints() {
  std::vector<int> codepoints;
  // ASCII (0x0020-0x007F)
  auto ascii = generate_codepoint_range(0x0020, 0x007F);
  codepoints.insert(codepoints.end(), ascii.begin(), ascii.end());
  // Hangul Jamo (0x1100-0x11FF)
  auto jamo = generate_codepoint_range(0x1100, 0x11FF);
  codepoints.insert(codepoints.end(), jamo.begin(), jamo.end());
  // Hangul Syllables (0xAC00-0xD7AF) - most common Korean characters
  auto syllables = generate_codepoint_range(0xAC00, 0xD7AF);
  codepoints.insert(codepoints.end(), syllables.begin(), syllables.end());
  // Fullwidth forms (0xFF01-0xFF60). CJK copy uses fullwidth punctuation, and
  // without these a fullwidth ! or ? silently draws raylib's '?' fallback --
  // which is how the Korean string shipped looking almost right.
  auto fullwidth = generate_codepoint_range(0xFF01, 0xFF60);
  codepoints.insert(codepoints.end(), fullwidth.begin(), fullwidth.end());
  return codepoints;
}

// Build codepoints for Japanese: Hiragana + Katakana + common Kanji + ASCII
inline std::vector<int> get_japanese_codepoints() {
  std::vector<int> codepoints;
  // ASCII (0x0020-0x007F)
  auto ascii = generate_codepoint_range(0x0020, 0x007F);
  codepoints.insert(codepoints.end(), ascii.begin(), ascii.end());
  // Hiragana (0x3040-0x309F)
  auto hiragana = generate_codepoint_range(0x3040, 0x309F);
  codepoints.insert(codepoints.end(), hiragana.begin(), hiragana.end());
  // Katakana (0x30A0-0x30FF)
  auto katakana = generate_codepoint_range(0x30A0, 0x30FF);
  codepoints.insert(codepoints.end(), katakana.begin(), katakana.end());
  // Common CJK Unified Ideographs subset (0x4E00-0x9FFF) - full range
  auto kanji = generate_codepoint_range(0x4E00, 0x9FFF);
  codepoints.insert(codepoints.end(), kanji.begin(), kanji.end());
  // Fullwidth forms; see the Korean builder.
  auto fullwidth = generate_codepoint_range(0xFF01, 0xFF60);
  codepoints.insert(codepoints.end(), fullwidth.begin(), fullwidth.end());
  return codepoints;
}

inline std::vector<int> get_language_japanese_codepoints() {
  auto codepoints = generate_codepoint_range(0x0020, 0x007E);
  const std::vector<int> japanese{
      0x3002, 0x3044, 0x304f, 0x3051, 0x3053, 0x3055, 0x3059, 0x3060,
      0x3061, 0x3067, 0x306b, 0x306e, 0x306f, 0x307e, 0x307f, 0x308b,
      0x308c, 0x3092, 0x3093, 0x30c7, 0x30e2, 0x4e86, 0x5831, 0x59cb,
      0x5b9a, 0x60c5, 0x65e5, 0x672c, 0x7528, 0x7d42, 0x7d9a, 0x8a00,
      0x8a2d, 0x8a9e, 0x9069, 0x9078, 0x958b, 0xff01};
  codepoints.insert(codepoints.end(), japanese.begin(), japanese.end());
  return codepoints;
}

// Font definition - single source of truth for all fonts
struct FontDef {
  std::string name;
  std::string filename; // relative to fonts/ directory
  bool needs_codepoints = false;
  std::function<std::vector<int>()> get_codepoints = nullptr;
  int raster_size = 96;
  std::vector<std::string> screens;
};

inline std::vector<int> get_inline_prompt_codepoints() {
  auto codepoints = generate_codepoint_range(0x0020, 0x007E);
  const std::vector<int> korean{
      0xad6d, 0xb118, 0xb204, 0xb2c8, 0xb2e4, 0xb3c4, 0xb974, 0xb97c,
      0xba74, 0xbc1c, 0xc138, 0xc5b4, 0xc5d0, 0xc694, 0xc744, 0xc74c,
      0xc810, 0xcc29, 0xd0a4, 0xd2c8, 0xd310, 0xd504, 0xd558, 0xd55c, 0xd569};
  codepoints.insert(codepoints.end(), korean.begin(), korean.end());
  return codepoints;
}

// Get all font definitions - this is the single source of truth
// Add new fonts here and they'll be available in both windowed and headless
// modes
inline const std::vector<FontDef> &get_all_fonts() {
  static const std::vector<FontDef> fonts{
      // Core UI fonts

      // Named fonts for screens
      {"Gaegu-Bold", "Gaegu-Bold.ttf"},
      {"GaeguMock", "Gaegu-Bold.ttf", false, nullptr, 192},
      {"EqProRounded", "eqprorounded-regular.ttf"},
      {"Garamond", "EBGaramond-Regular.ttf"},
      {"NerdSymbols", "SymbolsNerdFont-Regular.ttf"},
      {"Fredoka", "Fredoka-VariableFont_wdth,wght.ttf"},
      {"FredokaMockBold", "Fredoka-MockBold.ttf", false, nullptr, 192},
      {"BlackOpsOne", "BlackOpsOne-Regular.ttf"},
      {"Atkinson", "AtkinsonHyperlegible-Regular.ttf"},
      {"AtkinsonMock@bold", "AtkinsonHyperlegible-Bold.ttf", false, nullptr, 192},
      {"AtkinsonMock", "AtkinsonHyperlegible-Regular.ttf", false, nullptr,
       192},

      // The "@bold" suffix is the convention FontManager::resolve_weighted
      // looks for, so a TextSpan asking for Bold resolves to the second entry
      // and falls back to the first when it is absent.
      // Oldschool PC Font Pack, CC BY-SA 4.0 -- see OLDSCHOOL_PC_FONTS_*.txt.
      {"DGOne", "Px437_DG_One.ttf"},
      {"DGOne@bold", "Px437_DG_One_bold.ttf"},
      {"DGOneMock", "Px437_DG_One.ttf", false, nullptr, 192},

      // The only bundled family with more than two weights, so it is the one
      // that can actually show what with_font_weight does. OFL, see
      // ARCHIVO_NARROW_OFL.txt. No Light face ships, so FontWeight::Light
      // falls back to Regular and warns, which is the documented behaviour.
      {"Archivo", "ArchivoNarrow-Regular.ttf"},
      {"Archivo@medium", "ArchivoNarrow-Medium.ttf"},
      {"Archivo@semibold", "ArchivoNarrow-SemiBold.ttf"},
      {"Archivo@bold", "ArchivoNarrow-Bold.ttf"},
      {"ArchivoMock", "ArchivoNarrow-Regular.ttf", false, nullptr, 192},
      {"ArchivoMockBold", "ArchivoNarrow-Bold.ttf", false, nullptr, 192},
      {"ArchivoMockBoldItalic", "ArchivoNarrow-BoldItalic.ttf", false, nullptr, 192},

      // CJK fonts with special codepoint loading
      {"NotoSansKRInline", "NotoSansMonoCJKkr-Bold.otf", true,
       get_inline_prompt_codepoints, 96, {"localized_input_prompts"}},
      {"NotoSansKR", "NotoSansMonoCJKkr-Bold.otf", true,
       get_korean_codepoints, 32,
       {"language_demo", "forms", "sync_scroll_lab", "curved_text_lab", "curved_text_stress_lab"}},
      {"NotoSansJP", "NotoSansMonoCJKjp-Bold.otf", true,
       get_language_japanese_codepoints, 96, {"language_demo"}},
      {"Sazanami", "Sazanami-Hanazono-Mincho.ttf", true,
       get_japanese_codepoints, 32,
       {"forms", "mini_motorways_settings", "curved_text_lab", "curved_text_stress_lab"}},
  };
  return fonts;
}

struct FontAlias {
  const char *name;
  const char *source;
};

inline constexpr FontAlias aliases[] = {
    {"__symbol", "Atkinson"},
    {"__default", "AtkinsonMock"},
    {"__default@bold", "AtkinsonMock@bold"},
    {"__unset", "AtkinsonMock"},
    {"__unset@bold", "AtkinsonMock@bold"},
    {"AtkinsonMockBold", "AtkinsonMock@bold"},
};

} // namespace font_config
