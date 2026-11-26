# Screen-control-overlap audit (Batch 7 Lane U)

Audit-only. No screen fixes in this lane; fixes are a follow-up batch once
Gabe sees this list. Owner todo was *"on some screens the buttons to control
the screen are hidden behind other buttons"*. PR #24 fixes the Trellis cases
(toolbar clamped before the Browse launcher, workspace ends 48px above the
HUD); this sweep catches the rest systematically.

* Tool: `scripts/audit_screen_control_overlaps.py` (rerunnable, ranked table,
  JSON via `--out`, `--strict` exits 1 when real offenders exist).
* Geometry: `ui_tester --layout-summary <screen> -w 1280 -h 720`
  (`src/devtools/layout_audit/layout_summary.cpp` — one `name x y w h` line
  per rendered, debug-named element).
* Registry: `ui_tester --list-screens` → **162 screens**. The
  `screenshot-baselines/screens/` dir holds 161 files; the layout registry is
  authoritative (`blend_examples` has no baseline PNG).
* Head SHA: `fdf5d6b5c75cddc61e4f51e9f05402f263f91290` (origin/main).

## Method

For every screen, run `--layout-summary` and intersect each element rect
with the three global chrome safe areas below. Classify hits by debug name
as **interactive-control** (buttons, sliders, toggles, inputs, tabs,
steppers…) vs **container/background** (cards, panels, rows, backgrounds,
titles, labels…) vs **other** (unrecognized). Exclude the chrome's own
`nav_*` elements and full-screen roots/backgrounds (rect ≥ 85% of the
1280×720 frame). A *real offender* is an interactive control intersecting a
safe area.

## Safe-area derivation (from code)

| Zone | Code | Rect @1280×720 |
|---|---|---|
| Browse-screens launcher (`nav_open`) | `src/plugins/gallery_shell/ScreenNavigator.h:54-59` — `button("Browse screens (\`)")`, size 200×32 at `x = width-212, y = 8`, render layer 2000 | **1068–1268 × 8–40** |
| Bottom-left HUD band | `src/plugins/gallery_shell/RenderScreenHUD.h:46-79` — name + nav hint + description; with description `total_height = 36`, `y_pos = height-36-10 = 674`, bg `671–713`, x starts 5, width text-dependent | **x < 760, y ≥ 656** (`height-64`) |
| Bottom-right index chip | `RenderScreenHUD.h:101-113` — `"i/162"` bg at `y_pos-3 = 671`, height 22, right edge ≈ `width-10` | **x > 1140, y ≥ 680** (`width-140`, `height-40`) |

Caves: `RenderScreenHUD` returns early in headless mode
(`RenderScreenHUD.h:31-33`), and the launcher is not rendered in the
headless layout path — so headless *screenshots* show no chrome. The rects
above are live-mode geometry; verify fixes in live `--screen` mode.

## Result (162 screens, 0 runner errors)

* **45 interactive hits on 27 screens** (30 HUD band, 13 launcher, 2 index chip).
* 10 hits have their **center inside** a safe area — actually hidden.
* 161 container/background hits, 261 unclassified hits (long tail, mostly
  display panels and scroll rows — see false-positive classes).

### Ranked real offenders (script order: center-inside, covered %, overlap px)

| # | Screen : element | Rect | Zone | Covered | Center |
|---|---|---|---|---|---|
| 1 | `tooltip_lab:ttl_btn_3` | 67,657 160×40 | HUD | 100% | in |
| 2 | `transitions_lab:replay_btn` | 40,660 120×40 | HUD | 100% | in |
| 3 | `transitions_lab:reset_btn` | 172,660 120×40 | HUD | 100% | in |
| 4 | `forms:forms_antialiasing_toggle` | 660,668 44×44 | HUD | 100% | in |
| 5 | `scroll_click_bug:scroll_btn_10` | 76,672 770×40 | HUD | 89% | in |
| 6 | `motion_button:child_btn` | 288,648 120×40 | HUD | 80% | in |
| 7 | `buttons:buttons_counter` | 88,650 362×28 | HUD | 79% | in |
| 8 | `config_gap_gallery:cg_pause` | 88,644 328×38 | HUD | 68% | in |
| 9 | `forms:copy_btn` | 652,650 162×36 | HUD | 56% | in |
| 10 | `animation_basic:restart_btn` | 68,636 240×44 | HUD | 55% | in |
| 11 | `podcast_library:slider_handle` | 72,643 176×24 | HUD | 46% | out |
| 12 | `podcast_library:slider_background` | 72,643 712×24 | HUD | 44% | out |
| 13 | `kart_select:kart_player` | 1198,27 32×32 | launcher | 41% | out |
| 14 | `text_motion_lab:reset_all_btn` | 1080,26 140×40 | launcher | 35% | out |
| 15 | `virtual_list_lab:text_input_field` | 986,15 144×34 | launcher | 32% | out |
| 16 | `buttons:buttons_feedback` | 472,650 720×28 | HUD | 31% | out |
| 17 | `rubber_bandits_menu:rb_player_count` | 980,4 136×32 | launcher | 31% | out |
| 18 | `colour_track_lab:replay_btn` | 60,624 160×44 | HUD | 27% | out |
| 19 | `forms:forms_motion_blur_toggle` | 660,624 44×44 | HUD | 27% | out |
| 20 | `scroll_click_bug:scroll_btn_9` | 76,628 770×40 | HUD | 27% | out |
| 21 | `meters_gauges:mg_row_slider` | 68,536 702×156 | HUD | 23% | out |
| 22 | `forms:forms_ambient_toggle` | 660,712 44×44 | HUD | 18% | out |
| 23 | `dropdowns:dropdown_status` | 68,558 1144×138 | HUD | 18% | out |
| 24 | `toggle_switches:toggle_reset` | 706,649 246×32 | HUD | 17% | out |
| 25 | `tooltip_lab:ttl_btn_4` | 1040,657 160×40 | index chip | 16% | out |
| 26 | `text_stroke:stroke_detail_toggle` | 978,29 234×43 | launcher | 16% | out |
| 27 | `podcast_library:podcast_player` | 48,562 1184×120 | HUD | 13% | out |
| 28 | `typography_composition:type_replay` | 68,610 180×52 | HUD | 12% | out |
| 29 | `sports_settings:sports_tabs` | 0,0 1280×50 | launcher | 10% | out |
| 30 | `checkboxes:checkbox_reset` | 982,34 210×36 | launcher | 10% | out |
| 31 | `scroll_click_bug:scroll_btn_11` | 76,716 770×40 | HUD | 9% | out |
| 32 | `config_gap_gallery:text_input_field` | 864,664 328×32 | index chip | 8% | out |
| 33 | `theme_builder:tb_table` | 24,80 816×624 | HUD | 7% | out |
| 34 | `text_input:text_input_title` | 48,20 1184×48 | launcher | 6% | out |
| 35 | `text_input:btn_cancel` | 344,618 118×40 | HUD | 5% | out |
| 36 | `text_input:btn_save` | 478,618 118×40 | HUD | 5% | out |
| 37 | `fit_content_lab:fc_narrow_metrics` | 736,644 304×29 | HUD | 5% | out |
| 38 | `decorative_frame:frames_toggle_guides` | 942,37 250×34 | launcher | 4% | out |
| 39 | `toggle_switches:toggle_card` | 304,24 672×672 | HUD | 4% | out |
| 40 | `button_variants:bv_toggle_bounds` | 1018,38 174×36 | launcher | 4% | out |
| 41 | `radio_buttons:radio_bg` | 60,46 1160×628 | HUD | 2% | out |
| 42 | `buttons:buttons_title` | 88,34 1104×42 | launcher | 2% | out |
| 43 | `dropdowns:dropdown_title` | 88,34 1104×42 | launcher | 2% | out |
| 44 | `text_input:input_states` | 644,124 588×550 | HUD | 1% | out |
| 45 | `buttons:buttons_bg` | 68,24 1144×676 | launcher | 0% | out |

Full machine-readable list: rerun with `--out report.json`.

### Screenshot verification (headless captures, chrome not painted)

* **Verified real**: `transitions_lab` Replay/Reset (bottom-left,
  y≈660–700 — squarely in the HUD band), `tooltip_lab:ttl_btn_3`
  ("bottom left" button at 67,657), `motion_button:child_btn` ("Child"
  button at 288,648). All three are visibly in the bottom band; in live
  mode the HUD name/description strip covers them.
* **Scroll-clipped, not chrome-covered**: `scroll_click_bug` rows 9–11 —
  the screen itself says "300 px viewport / bottom edge intentionally
  clips partial rows". Their layout rects extend into the band, but the
  scroll viewport clips them first. (The name even says *scroll_click_bug*.)
* **Below-fold in capture**: `forms:forms_antialiasing_toggle` — reported
  at 660,668 but not visible in the canonical headless frame (the forms
  columns scroll; headless captures at scroll offset 0). Reported, needs
  a live check before fixing.

### Trellis note

PR #24 is **open, not merged**, so main still has the pre-fix dock
positions: `dock_step_out` (1072,10 96×30) and `dock_reset` (1176,10 96×30)
sit 96–100% inside the launcher rect on all 4 Trellis screens
(`src/systems/screens/trellis/DockWorkspace.h:783-784`, `mkbtn` buttons).
The classifier leaves them as "other" (no control token in the name), so
they are outside the 45. Once PR #24 lands, a re-run should show zero
Trellis hits — that's the regression check for that PR.

## False-positive classes (why 45 is not 45 fix jobs)

1. **Full-screen roots/backgrounds** — excluded by the 85%-of-frame rule
   (e.g. `buttons_canvas` 0,0 1280×720, `bg` 0,0 1280×720).
2. **Large container cards** — a card spanning most of the frame
   (classified `container`): `buttons_bg` 68,24 1144×676,
   `adaptive_scaling:content` 0,130 1280×590, `sports_footer`,
   `flight_footer`, `secure_tunnel:tn_sidebar`. 161 such hits; do not fix.
3. **Scroll-clipped / scrolled-out rows** — layout-summary reports every
   rendered element's laid-out rect, including rows a scroll viewport
   clips (`scroll_btn_9/10/11`, `command_picker_row_*`, `vlist_row`,
   `vlh_row_9`). Scroll-clip ≠ chrome-cover.
4. **Below-fold elements** — rects priced while scrolled at 0
   (`forms_antialiasing_toggle`, `forms_motion_blur_toggle`,
   `forms_ambient_toggle`).
5. **Token misclassification** — display/status panels whose names contain
   control tokens are counted interactive: `slider_background` (a track,
   not a handle), `dropdown_status`, `tb_table` (a table),
   `toggle_card` (a card), `buttons_counter` (a counter readout),
   `mg_row_slider` (a 702×156 slider region, mostly chrome-free),
   `sports_tabs` (a full-width tab bar — the 10% is a 16px strip at the
   top-right corner, not a hidden control).
6. **Edge slivers** — ≤5% coverage with center outside the chrome
   (`text_input:btn_save/cancel` at 618–658 vs band start 656,
   `button_variants:bv_toggle_bounds` 4%). Real geometry, near-zero user
   impact; fix only if the row moves anyway.
7. **Chrome self-hits** — `nav_open` and friends excluded by the `nav_`
   prefix; full-screen panels like `radio_bg`/`buttons_bg` land here.

## Recommended fix order (follow-up batch, not this lane)

1. **HUD band first (30 hits)** — move control rows above y=656 or shrink
   scroll viewports to `height-64`. Quick wins (one row each, center 100%
   covered): `transitions_lab` (2), `tooltip_lab` (bottom-left button),
   `motion_button` (child button row), `config_gap_gallery:cg_pause` +
   podcast player, `animation_basic:restart_btn`, `buttons` (counter/
   feedback rows), `forms` (scroll column leaves the antialiasing toggle
   in the band — pin the toggle row out of the scroll area or above
   y=656).
2. **Launcher zone (13 hits)** — clamp right-anchored rows to
   `x + width ≤ 1068` or shift the whole row left of the launcher, same
   pattern as PR #24's `toolbar_x_ending_before_launcher`:
   `text_motion_lab:reset_all_btn`, `virtual_list_lab:text_input_field`,
   `kart_select:kart_player`, `rubber_bandits_menu:rb_player_count`,
   `checkboxes:checkbox_reset`, `text_stroke:stroke_detail_toggle`,
   `decorative_frame:frames_toggle_guides`,
   `button_variants:bv_toggle_bounds`.
3. **Index chip (2 hits)** — `tooltip_lab:ttl_btn_4`,
   `config_gap_gallery:text_input_field`: pull bottom rows in from the
   right edge or raise them above the chip band.
4. **Ignore for now** — edge slivers ≤5% and Class-5 misclassifications
   (`slider_background`, `dropdown_status`, `tb_table`, `toggle_card`,
   `buttons_counter`, `mg_row_slider`, `sports_tabs` top strip).
   Scroll-clipped rows need a scroll-position-aware check, not a chrome fix.

## Repro

```bash
export PKG_CONFIG_PATH=$HOME/homebrew/lib/pkgconfig
make -j4
python3 scripts/audit_screen_control_overlaps.py            # exit 0, ranked table
python3 scripts/audit_screen_control_overlaps.py --out report.json --strict  # exit 1 while offenders exist
./output/ui_tester.exe --run-all-tests                      # suite stays green (audit-only)
```

`--strict` is opt-in only: the default run never fails main, and nothing
here is wired into the test suite or baselines.
