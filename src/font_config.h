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
  return codepoints;
}

// Font definition - single source of truth for all fonts
struct FontDef {
  std::string name;
  std::string filename; // relative to fonts/ directory
  bool needs_codepoints = false;
  std::function<std::vector<int>()> get_codepoints = nullptr;
};

// Get all font definitions - this is the single source of truth
// Add new fonts here and they'll be available in both windowed and headless
// modes
inline std::vector<FontDef> get_all_fonts() {
  return {
      // Core UI fonts
      {"__default", "Gaegu-Bold.ttf"},
      {"__symbol", "Gaegu-Bold.ttf"},
      {"__unset", "Gaegu-Bold.ttf"},

      // Named fonts for screens
      {"Gaegu-Bold", "Gaegu-Bold.ttf"},
      {"EqProRounded", "eqprorounded-regular.ttf"},
      {"Garamond", "EBGaramond-Regular.ttf"},
      {"NerdSymbols", "SymbolsNerdFont-Regular.ttf"},
      {"Fredoka", "Fredoka-VariableFont_wdth,wght.ttf"},
      {"BlackOpsOne", "BlackOpsOne-Regular.ttf"},
      {"Atkinson", "AtkinsonHyperlegible-Regular.ttf"},

      // CJK fonts with special codepoint loading
      {"NotoSansKR", "NotoSansMonoCJKkr-Bold.otf", true, get_korean_codepoints},
      {"Sazanami", "Sazanami-Hanazono-Mincho.ttf", true,
       get_japanese_codepoints},
  };
}

} // namespace font_config
