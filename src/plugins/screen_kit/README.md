# Screen Kit local plugin

WM-local plugin (Batch 3 Lane J). Not afterhours upstream.

## Purpose

Named home for the merged screen scaffold and the SettingsPage kit:
the shared `begin_screen` preamble (PR #3) and the data-driven
tabs+rows+dirty-state settings page built on the vendored
`imm::setting_row` (PR #4). Behavior-preserving package of the two
headers formerly under `src/systems/`; no scaffold math, layout,
label, or baseline changes, and no screen, afterhours, or vendor
edits.

## Public API

Canonical headers (in this directory):

- `screen_scaffold.h` — `include "plugins/screen_kit/screen_scaffold.h"`
  - `begin_screen(ctx, entity, theme, opts={})` — the shared screen
    preamble: `set_theme` (not assignment, so sizing reads metrics
    through `ThemeDefaults`), `ScalingMode::Proportional`,
    `s = min(screen_width/1280, screen_height/720)`, a full-screen
    background (`Theme::Usage::Background` or `opts.background`),
    and a centred 1280x720 stage root. Returns a `ScreenFrame`.
  - `ScreenFrame { s, root, ink, muted }` — `s` is the 720p min-scale;
    `root` is the stage element; `ink`/`muted` default to
    `theme.font`/`theme.font_muted` unless `opts.ink`/`opts.muted`.
  - `ScreenFrame::layout(x, y, w, h)` — 720p design-unit box:
    `pixels(v * s)` size + absolute position, sharp corners, no
    background, grid-snap skipped. Deliberately `pixels(v * s)`, not
    `with_720p_size`: `w1280`/`h720` scale each axis independently,
    so at 4:3 the 720p sizes outgrow their parents and e2e
    containment assertions fail; `pixels()` is also the one dim
    autolayout never snaps to its 4px grid (850 -> 852).
  - `ScreenOptions { background, ink, muted, bg_name="bg" }` —
    optional background flat colour and text colours in place of the
    theme roles.
  - `load_art(art, names, folder)` — the repeated art loop (one
    texture per name, `folder + name + ".png"`, bilinear);
    per-screen subfolder quirks stay at the call site.
- `SettingsPage.h` — `include "plugins/screen_kit/SettingsPage.h"`,
  namespace `settings_kit`
  - Data model: `Control { Stepper, Toggle, Slider, Display }`;
    `Row { label, control, options, index/saved_index, toggle/
    saved_toggle, slider/saved_slider, display, unavailable, help }`
    with `dirty()` (current vs saved snapshot; unavailable and
    `Display` rows are never dirty), `apply()`, `revert()`,
    `step(direction)` (false for unavailable/no-choice rows),
    `current_text()`, `saved_text()`. Row factories
    `stepper_row`, `toggle_row`, `slider_row`, `unavailable_row`.
  - `Tab { name, rows }`; `Page { tabs, active_tab, selected_row,
    status }` with `selected()`, `dirty()`, `apply()`, `revert()`,
    `revert_tab()`, `select_tab()` (resets row selection),
    `move_selection(direction)` (skips unavailable rows, wraps at
    both ends), `step_selected(direction)`.
  - `Style` — frame-scoped styling the caller computes each frame
    (row/tab sizes, font name/size, debug-name prefixes, focus
    requests, row/tab backgrounds and text colours, `slider_width`,
    `stepper_value_width`); the kit never knows which resolution a
    screen designs against.
  - `Result { tab_changed, row_changed, selection_changed,
    row_selected, changed_row, selected_row }`.
  - Renderers: `render_tabs(ctx, parent, page, container, style,
    vertical=false)` — tab button row/column (selecting a tab
    resets row selection); `render_rows(...)` — selectable wrapper
    `div` per row around a vendored `setting_row` (click selects
    without changing; arrows/switch/slider change the value);
    `render_status(...)` — one label for `page.status`, renders
    nothing when empty; `render_page(...)` — whole page (tab bar
    over rows over status) for screens without bespoke chrome.
    Pilots with their own layout call the three pieces into their
    own panels instead. Stepper arrows are renamed per row
    (`<base>_previous` / `<base>_next`) so callers and e2e can
    address one row's controls.

Compatibility: `src/systems/screen_scaffold.h` and
`src/systems/SettingsPage.h` are shims including these headers; all
existing include paths and APIs are unchanged. The canonical
`scaffold` header keeps its sibling includes via
`../../external.h`, `../../input_mapping.h`,
`../../theme_presets.h`, and `../gallery_shell/ExampleScreenRegistry.h`;
the canonical `SettingsPage` header uses `../../external.h`. No
pilot screen was edited.

## When to use each

- Use `begin_screen` / `ScreenFrame::layout` for a new screen that
  paints a full-screen background on a centred 1280x720 stage and
  sizes its boxes in 720p design units; keep only what is actually
  the screen's (inner cards, hand-tuned colours). Do not replace
  `layout()` with `with_720p_size` on letterboxed screens.
- Use `SettingsPage` for a data-driven settings page: caller
  supplies tabs of rows (label, control kind, value, saved
  snapshot, help); the kit owns the tab header, row stack,
  selection, dirty tracking, and status line. Keep bespoke chrome
  (artwork, preview panes, footers) at the call site and call
  `render_tabs` / `render_rows` / `render_status` into it.
- There is deliberately no `Action`/`Button` control kind:
  `setting_row` has none, and adding one here would fork the
  widget. A command row (e.g. "Restore Defaults") is drawn by the
  caller as its own button, outside the kit's row list. Likewise
  an unavailable row renders through `setting_row`'s read-only
  `Display` control, not a disabled stepper (the stepper would
  still report arrow clicks as changes).

## Current consumers

No screen header currently `#include`s these headers: PR #3 and
PR #4 landed the two headers header-only, and the designated pilot
screens below still hand-roll the equivalent preamble/rows; adopting
the kit in a screen body is a follow-up, not this batch. The pilots
are the behavior proof for this plugin:

- Scaffold pilots (designated): `Buttons.h` (`buttons`),
  `CheckboxShowcase.h` (`checkboxes`), `ExampleSimpleButton.h`
  (`simple_button`) — `src/systems/screens/`.
- SettingsPage pilots (designated): `FlightOptions.h`
  (`flight_options`), `PowerWashSettings.h`
  (`powerwash_settings`) — `src/systems/screens/`.

Targeted e2e covering those screens (all present):

- `02_button_click.e2e`, `08_checkbox_toggle.e2e`,
  `102_simple_button_click.e2e`, `134_flight_options.e2e`,
  `143_powerwash_settings.e2e`, plus the design companions
  `173_buttons_design.e2e`, `177_checkboxes_design.e2e`,
  `257_simple_button_design.e2e`, `202_flight_options_design.e2e`,
  `235_powerwash_settings_design.e2e`.

Headless screenshot baselines: `screenshot-baselines/screens/
{buttons,checkboxes,simple_button,flight_options,powerwash_settings}_720p.png`.

## Tests

```bash
PKG_CONFIG_PATH=$HOME/homebrew/lib/pkgconfig make -j4
./output/ui_tester.exe --run-all-tests
./output/ui_tester.exe --e2e --test-script tests/e2e_scripts/02_button_click.e2e --headless
./output/ui_tester.exe --e2e --test-script tests/e2e_scripts/08_checkbox_toggle.e2e --headless
./output/ui_tester.exe --e2e --test-script tests/e2e_scripts/102_simple_button_click.e2e --headless
./output/ui_tester.exe --e2e --test-script tests/e2e_scripts/134_flight_options.e2e --headless
./output/ui_tester.exe --e2e --test-script tests/e2e_scripts/143_powerwash_settings.e2e --headless
```

No new compile/unit smoke was added: the existing pilot e2e and
headless screenshot baselines are the behavior proof (behavior-
preserving move; zero baseline changes expected). Screenshot
compare: `python3 scripts/compare_baselines.py` (threshold 0.05%,
channel tolerance 8).

## Afterhours promotion gate (future candidate only)

No afterhours/vendor changes in this batch. Future candidate only
for the generic, data-driven settings-rows / scaffold pieces that go
beyond the vendored `setting_row`: i.e. a small `begin_screen`
preamble helper plus a data model (rows/tabs/dirty tracking) a
consumer can drive, and only after this plugin holds stable across
the full local suite, the targeted pilot e2e above, and headless
screenshot baselines. The vendored `setting_row` itself stays the
single widget: any promotion must not fork it, must not add an
`Action` control to the shared kit, and is a separate proposal with
its own review. Not claimed upstream-ready.
