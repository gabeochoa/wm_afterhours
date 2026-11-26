# Font Registry local plugin

WM-local plugin (Batch 2 Lane F). Not afterhours upstream.

## Purpose

WM's font registry plus CJK codepoint tables, packaged as one WM-local
plugin. Behavior-preserving relocation only: no font name, filename,
alias, codepoint range, raster size, or screen-scoping value changes.

## Public API

Canonical header: `src/plugins/font_registry/font_config.h` —
`include "plugins/font_registry/font_config.h"`, namespace `font_config`.

- `generate_codepoint_range(start, end)` — inclusive `[start, end]` vector.
- `get_korean_codepoints()` — ASCII `0x0020-0x007F`, Hangul Jamo
  `0x1100-0x11FF`, Hangul Syllables `0xAC00-0xD7AF`, fullwidth
  `0xFF01-0xFF60`.
- `get_japanese_codepoints()` — ASCII, Hiragana `0x3040-0x309F`,
  Katakana `0x30A0-0x30FF`, CJK Unified Ideographs `0x4E00-0x9FFF`,
  fullwidth `0xFF01-0xFF60`.
- `get_language_japanese_codepoints()` — ASCII `0x0020-0x007E` plus the
  curated 38-codepoint demo set.
- `get_inline_prompt_codepoints()` — ASCII `0x0020-0x007E` plus the
  curated 25-codepoint Korean prompt set.
- `FontDef{name, filename, needs_codepoints, get_codepoints,
  raster_size=96, screens}` — single source of truth per font.
- `get_all_fonts()` — the registry vector (core UI fonts, named/mock
  families, weighted `@bold` entries, CJK entries with codepoint
  providers and raster sizes 32/96).
- `FontAlias{name, source}` and `aliases[]` — `__symbol`, `__default`,
  `__default@bold`, `__unset`, `__unset@bold`, `AtkinsonMockBold`.

Compatibility: `src/font_config.h` is a shim including this header;
existing `#include "font_config.h"` / `#include "../font_config.h"` call
sites are unchanged.

## Deterministic mock-font / codepoint notes

- Mock fonts are deterministic stand-ins, not separate typefaces:
  `GaeguMock`, `FredokaMockBold`, `AtkinsonMock`, `AtkinsonMock@bold`,
  `DGOneMock`, `ArchivoMock*` reuse a real face's file at
  `raster_size=192` (vs. 96 default and 32 for the CJK atlas fonts), so
  headless screenshots and tests do not depend on face availability or
  rasterizer variation between runs.
- The `@bold` / `@medium` / `@semibold` suffix convention is what
  `FontManager::resolve_weighted` looks for; entries without a weighted
  sibling (e.g. `Archivo` has no Light face) intentionally fall back to
  Regular and warn.
- Codepoint builders are pure functions of their ranges: no I/O, no
  locale, no font-file reads. Fullwidth `0xFF01-0xFF60` is included in
  both Korean and Japanese builders so fullwidth punctuation does not
  fall through to raylib's `?` fallback. Reordering or resizing a
  range changes rasterisation, so any range edit is a behavior change
  and must be proven by screenshot diff, not just a passing test.

## Current consumers

- `src/preload.cpp` — `font_config.h` (loads `get_all_fonts()` and
  registers `aliases`).
- `src/headless_screenshots.cpp` — `font_config.h` (headless font load).
- `src/devtools/wm_devtools.h` — `../font_config.h` (aggregate devtools
  include; manifest descriptor id `font_config` now points at the
  canonical `src/plugins/font_registry/font_config.h`).
- `src/testing/ui_tree_dump.h` — `../font_config.h` (alias/font
  resolution in the dump).
- `src/testing/tests/DevtoolsManifestTest.h` — indirectly via
  `wm_devtools.h` (registry non-empty, `AtkinsonMock` present).

## Tests

```bash
PKG_CONFIG_PATH=$HOME/homebrew/lib/pkgconfig make -j4
./output/ui_tester.exe --run-all-tests
```

`FontConfigTest.h` covers theme/font wiring through the shim;
`DevtoolsManifestTest.h` covers registry presence. Targeted e2e:
`30_themes.e2e`, `98_test_themes.e2e`, `266_themes_design.e2e`,
`283_default_theme.e2e` (theme presets consume `FONT_*` names that must
match registry keys).

## Afterhours promotion gate

No afterhours changes in this batch. Font registry only — not claimed
upstream-ready. Promotion candidate only when AH `FontCatalog` (or
`font_catalog.h` / `font_coverage.h`) accepts all of: (a) per-font
`raster_size` plus codepoint-provider callbacks, (b) the alias table
with the `FontManager::resolve_weighted` `@bold` convention, and (c)
per-screen font scoping — and the KR Hangul (`AC00-D7AF` + Jamo
`1100-11FF`), JP kanji (`4E00-9FFF`), and fullwidth (`FF01-FF60`)
ranges round-trip through the AH loader without raylib `?` fallback.
Until then the WM registry stays the source of truth, and any promotion
requires stability across the full local suite, targeted e2e, and
headless screenshot baselines.
