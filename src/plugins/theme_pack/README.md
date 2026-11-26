# Theme Pack local plugin

WM-local plugin (Batch 2 Lane F). Not afterhours upstream.

## Purpose

Named theme presets plus WM's adopted theme defaults, packaged as one
WM-local plugin. Behavior-preserving relocation only: no palette, token,
radius, font, or screen-behavior value changes.

## Public API

Canonical headers (in this directory):

- `theme_presets.h` — `include "plugins/theme_pack/theme_presets.h"`
  - `afterhours::ui::theme_presets::FONT_ENGLISH` = `"Gaegu-Bold"`,
    `FONT_KOREAN` = `"NotoSansKR"`, `FONT_JAPANESE` = `"Sazanami"`.
  - `theme_presets::add_default_language_fonts(Theme&)` — English 1.0f,
    Korean 0.95f, Japanese 0.92f via `FontConfig`.
  - `theme_presets::THEME_CORNER_RADIUS` = `10.f` (px, not the afterhours
    roundness fraction).
  - Presets: `cozy_kraft()`, `cozy_border_color()`, `neon_dark()`,
    `ocean_navy()`, `midnight()`, `sage_natural()` — each pins
    `surface_raised` / `border` / `border_muted` to the frozen fallback-math
    result for its palette.
  - `theme_presets::apply(const Theme&)`, `apply_cozy_kraft()`,
    `apply_neon_dark()`, `apply_ocean_navy()`, `apply_midnight()`,
    `apply_sage_natural()`.
- `wm_defaults.h` — `include "plugins/theme_pack/wm_defaults.h"`
  - `apply_wm_defaults(Theme&)` — stamps WM's adopted defaults over the
    library defaults: `click_activation_mode=Release`,
    `focus_ring_thickness=2.0f`, `focus_ring_offset=-3.0f`,
    `text_area_line_height_ratio=1.45f`.

Compatibility: `src/theme_presets.h` and `src/wm_defaults.h` are shims
including these headers; the 119+ existing `theme_presets.h` consumers
(120 screens plus `FontConfigTest.h` via `../../theme_presets.h`, and
`screen_scaffold.h` via `../theme_presets.h`) compile unchanged. The
canonical `theme_presets.h` keeps its sibling `#include "wm_defaults.h"`,
which resolves inside this directory.

## Current consumers

- `src/systems/screen_scaffold.h` — `../theme_presets.h` (shared screen scaffold).
- `src/systems/screens/*.h` (120 screens, e.g. `Themes.h`, `WmDefaultsLab.h`,
  `Buttons.h`, `Forms.h`) — `../../theme_presets.h` (theme switching /
  specimen previews).
- `src/testing/tests/FontConfigTest.h` — `../../theme_presets.h`
  (preset/font wiring assertions).
- `src/preload.cpp`, `src/systems/screens/WmDefaultsLab.h` — `wm_defaults.h`
  via the shim (`apply_wm_defaults` on the default/app themes).

## Tests

```bash
PKG_CONFIG_PATH=$HOME/homebrew/lib/pkgconfig make -j4
./output/ui_tester.exe --run-all-tests
```

Targeted e2e: `30_themes.e2e`, `98_test_themes.e2e`,
`266_themes_design.e2e`, `283_default_theme.e2e`. Headless screenshot
smoke on `themes`, `buttons`, and `screens` — zero baseline changes
expected (behavior-preserving move).

## Afterhours promotion gate

No afterhours changes in this batch. Candidate slices, each its own
proposal with its own review — nothing here is claimed upstream-ready:

- Generic language-font defaults: only the `add_default_language_fonts`
  shape (per-language `FontConfig` with scale factors) is a candidate,
  and only after the font-registry gate below holds.
- Px radius guidance: only the `THEME_CORNER_RADIUS = 10.f` px guidance
  (median-pixel radius vs. roundness fraction) is a candidate; the
  fraction-to-px conversion semantics must be agreed upstream first.
- WM palettes stay local: `cozy_kraft`, `neon_dark`, `ocean_navy`,
  `midnight`, `sage_natural` and their pinned derived colours are
  WM-local presets and are not promoted.
- `apply_wm_defaults` values stay WM-local until proven as library
  defaults by the same stability + e2e + screenshot gates as Batch P-A.

Any promotion requires the plugin to hold stable across the full local
suite, targeted e2e above, and headless screenshot baselines.
