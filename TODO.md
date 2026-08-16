# TODO

## The short version

Fixed in afterhours: scroll views misplaced margined children; sideways
`expand()` gave 0px; centering was skipped when a thing was bigger than its
box; added `with_corner_radius(px)`; added right-click; menu shortcuts were
cut in half.

Fixed here: corners set to 10px so big panels stop looking like blobs;
`--update-baselines` no longer overwrites test images that were fine; 6 new
screens; the comparison tool had 5 bugs of its own.

**Worth remembering:** three "missing features" already existed and just failed
silently, so people assumed they were missing. Font weights work but need a
font named `"Inter@bold"` and nothing tells you that. Making the library speak
up when it quietly does nothing is probably worth more than the next feature.

**Biggest things left:** scrollbars, and moving the test helpers into
afterhours — cartographer, kart, hanabi and wm each built the same ones.

The rest of this file is the detail.

---

Open work from the 2026-08-15 session: the mock-tool layout sweep, the roundness
API, and the cross-project gap-doc audit. Grouped by where the fix lands, since
that decides who can do it.

Numbers here came from `./mocks/build.sh --no-serve && ./mocks/analyze.py` and
`./mocks/sheets.sh`. Detail lives in `mocks/ISSUES.md`.

---

## Where each fix lands

"Both" means the two halves are separable and want separate commits. "Unknown"
means the sweep found a disagreement but nobody has decided yet whether
afterhours is wrong or the screen is — that decision IS the work.

| # | issue | afterhours | wm | other |
|---|---|:--:|:--:|:--:|
| 1 | Font weights fall back silently | explicit family registration + warn | register variants | |
| 2 | Alpha blending on sokol | sokol blend mode | | verify from floatinghotel |
| 3 | No scrollbar for `HasScrollView` | render + hit-test | | |
| 4 | Overflow is silent | warning tolerance | over-specified screens | |
| 5 | ~~No right-click~~ **done** | `176ea8f` | `context_menu_lab`, e2e 94 | |
| 6 | ~~`context_menu` clips shortcuts~~ **done** | `c87b9d7` | | |
| 7 | Never-implemented gap requests | most of them | e2e pack upstreams **from** wm | |
| 8 | Slider knob, tab bars, hard wrap | all three | | |
| 9 | ~~Corner radiuses~~ **done** | `d265f0e` | `48db688`, 51 baselines | |
| 10 | ~~Family A~~ **resolved** | | was the mock: strictness + padding | |
| 11 | Family B (font metrics) | | won't-fix, documented | |
| 12 | ~~Family C~~ **done** | fixed, `17b982c` | 9 baselines | |
| 13 | Family D (heights) | likely for `file_tree`/`forms` | text metrics for `multiline_text_lab` | |
| 14 | Family E (diagonal offset) | **unknown** | border theory disproved | |
| 15 | Outliers, `islands_trains` 80 | **likely** | | |
| 16 | ~~Mock limitations~~ **done** | | `mocks/mock.js` | |
| 17 | Five `with_roundness(px)` sites | | | floatinghotel |
| 18 | Gap docs list shipped work | | | 5 docs, 4 repos |

Counts: **6 afterhours-only** (1, 2, 3, 6, 8, 12), **4 both** (4, 5, 7, 9),
**2 other-repo** (17, 18), **2 unknown** (13, 14, 15), **9 done or won't-fix**
(5, 6, 9, 10, 11, 12, 16, 19), plus 15 folded into the sweep numbers.

The two wm-only items are closed: 16 is done and 11 is a documented won't-fix.
Doing 16 also resolved item 10 outright and shrank 12 — the sweep went from 383
origins to 164, and the remaining unknowns are now much more likely to be real.

---

## afterhours (library)

### 1. Font weights fall back silently — **afterhours** (API), wm (content)
**Earlier entry here was wrong** and is corrected: I filed this as "the API
landed but the capability did not" and said the text path needed variant
selection. It already has it. `FontManager::resolve_weighted`
(`ui_core_components.h:420`) maps `(base, weight)` to a registered
`"<base>@bold"` and the render path calls it in four places.

All five weights render identically on `font_weight_720p` because that screen
uses the default font, and wm only registers one variant in the whole app
(`DGOne@bold`, `font_config.h:85`). Nothing is broken — the fallback is doing
what it says.

The real defect is the *shape of the contract*, and it is the same one
`is_right_click` had:

- The requirement is a **string naming convention** (`"Inter@bold"`) that
  appears nowhere in `with_font_weight`'s signature or docs. A caller has no
  way to discover it.
- `resolve_weighted` falls back to the base font **silently**. You ask for
  SemiBold, you get Regular, and nothing says why. floatinghotel filed "No
  Font Weight Support" — they almost certainly hit exactly this.

Worth fixing as an API question, not a font question: declare a family's
weights explicitly rather than string-encoding them, and warn once when a
requested weight falls back. See the design note below.
### 2. Alpha blending — verify on sokol — **afterhours** (sokol backend)
The one gap filed independently by two projects (floatinghotel, hanabi #15):
low-alpha `with_custom_background` reported to render opaque. It does **not**
reproduce on raylib — `alpha_blend_repro_720p` steps cleanly from 32 to 255 with
the text readable through the wash, for both a low-alpha `Color` and
`with_opacity`.

Both reporters are sokol. `RL_BLEND_ALPHA` is set only in
`backends/raylib/headless.h:52`; no blend mode is set in the sokol path. Run
`alpha_blend_repro` from floatinghotel before calling it fixed or broken.

### 3. `HasScrollView` renders no scrollbar — MEDIUM — **afterhours**
hanabi #26. Visible on `virtual_list_lab`: 10k rows scroll fine with no
indicator of position or extent. Every consumer hand-rolls one or does without.

### 4. Children that do not fit are silently overflowed — **both**
`mocks/ISSUES.md` issue 1. 21 nodes across `powerwash_settings` (9),
`flex_alignment` (5), `layout` (3), `deadspace_settings`, `meters_gauges`,
`rubber_bandits_menu`, `forms`.

Caller error in every case, so the library question is only whether it is
*visible* at authoring time. `strictness = 1` correctly refuses to shrink and
`flex_wrap` is NoWrap by intent, so what is actually left is that the overflow
warning does not fire: its tolerance is `sx + child_margin + 4px`, and
`powerwash_settings` overflows by 8px behind a 20px margin. That allowance was
deliberate (a `percent(1)` child plus margin overflows by exactly its margin and
the caller cannot act on it), so tightening it needs a way to tell that idiom
apart from real overflow — not a smaller constant.

### 5. No right-click trigger for `context_menu` — DONE
There is no secondary-click support anywhere in afterhours: `MOUSE_BUTTON_RIGHT`
does not appear in the source, and `HasClickListener` has a single `down` with
no button field. So `context_menu()` exists but cannot be opened the one way
users expect — `context_menu_lab` has to drive it from ordinary buttons.

Fixed in afterhours `176ea8f`. `MousePointerState` tracks the secondary button
and `UIContext::is_right_click(id)` answers "a secondary click finished over
this element or something inside it"; pair it with `mouse.pos` for the anchor.
`right_click x y` joins `click` in the e2e vocabulary, and
`94_right_click_context_menu.e2e` drives the whole path. `context_menu_lab` now
uses real right-clicks instead of stand-in buttons.

Caveat worth knowing: the target must be hit-testable for `hot` to land on it,
which today means carrying a click or drag listener. Fine for rows and buttons,
not for a bare panel.

### 6. `context_menu` clips the shortcut column — DONE
Fixed in afterhours `c87b9d7`. The shortcut is right-aligned, so its box's
right edge is where the last glyph lands, and the box ran out to `width` — the
final character was clipped in half. The gutter already reserved a pad for
this; it was being spent on width instead of on an inset.

### 7. Never-implemented requests from the gap docs — **afterhours** (one is wm→afterhours)
No upstream implementation at all, so a wm screen cannot be written until the
feature exists:

- **Synchronized scroll views** (floatinghotel: side-by-side diff)
- **Scroll anchoring / preserve-position-on-prepend** (hanabi #30)
- **Shared e2e command pack** — cartographer wants `click_btn`/`expect_label`
  without re-implementing; kart wants `disable_animations`, `--screenshot-dir`,
  `--e2e-speed`, case-insensitive `expect_text`. **wm already has all of these
  locally** and is the natural place to upstream them from.
- **Headless affordances** (kart, four separate workarounds): resolution returns
  (0,0), `GetFontDefault()` invalid, `bad_variant_access` on shutdown,
  `SINGLETON_FWD` fails in class scope.
- **Text metrics / measure+wrap API** — hanabi calls it "the #1 papercut".
- **Animation instant-mode / clear-all** (kart), per-item stagger, exit
  animations (hanabi AN-8..AN-12).
- `Margin`/`Padding` single-side helpers, e.g. `Margin::left(pixels(10))`
  (cartographer).

### 8. Long-standing, low priority — **afterhours**
From `docs/AFTERHOURS_GAPS.md`, unchanged: slider handle 0.75 compression;
crowded tab bars still need a smaller font at the call site; word-wrap has no
hard character break.

---

## wm_afterhours

### 9. Corner radiuses — make them consistent — DONE
afterhours `d265f0e` (precedence) + wm `48db688` (10px in the presets). Today `Theme::roundness = 0.5f` scales with the widget,
so the same theme value is ~8px on a row and 180px on a full-height panel; the
mock sheets show the big containers as blobs. `with_corner_radius(px)` now
exists, so the work is:

1. ~~Precedence~~ — fixed in afterhours. Order is caller px > caller roundness
   > theme px > theme roundness, and the two theme-level spellings now clear
   each other.
2. ~~A pixel default~~ — set to 10px in wm's four presets, **not** in
   afterhours. A default `Theme::corner_radius` would win over downstream code
   that assigns `theme.roundness` directly (wordproc zeroes it for square
   corners), so the value belongs in each app's presets.
3. ~~Re-baseline~~ — 51 of 98 screens repainted.

Still open, low priority: wm has ~350 per-call `with_roundness` sites, many of
which meant px. They are unaffected by the theme change (a caller fraction
wins) and can be converted opportunistically.

Pick the default off the current spread — 1329 painted nodes already resolve to
<4px, 1118 to 4-12px, and only 88 exceed 80px (`tray_bg`, `toggle_bg`,
`text_input_root`, `root_bg` are the 180px ones).

The ~/p/ survey backs the change: across nine projects `with_roundness(0.0f)` —
switching it off — is the single most common call at 307, eleven times more than
the `disable_rounded_corners()` helper that exists for it, and wordproc gives up
and zeroes `theme.roundness` globally.

---

## The html/mock sweep — every issue it found

`./mocks/build.sh --no-serve && ./mocks/sheets.sh` →
`mocks/sheets/all_disagreements.png` (44 screens) plus `sheet01..16.png`.
Dashed red is afterhours' rect over the CSS pane.

**Updated after the wm-side mock fixes (item 16, done).** Three modelling gaps
were doing most of the talking; closing them took the sweep from 44 screens /
718 nodes / 383 origins down to **26 screens / 198 nodes / 164 origins**, and
the nodes excluded for sitting under a scroll/clip container from 206 to 3.

Deltas below are `(dx, dy, dw, dh)` in px, CSS minus afterhours. Counts in the
family tables are POST-fix unless marked.

### 10. Family A — fixed-width children in a row that does not fit — RESOLVED, mock-side
Was 115 origins (`sports_settings` 62, `powerwash_settings` 36, `flex_alignment`
15, `adaptive_scaling` 4, plus singles). **Now zero.** Two mock bugs, both fixed
in `mocks/mock.js`:

- `strictness` was not modelled at all. CSS defaults `flex-shrink: 1` while
  afterhours defaults `strictness = 1` and refuses to shrink, so the mock was
  shrinking every overflowing row that afterhours deliberately let overflow.
  Now `flex-shrink: 1 - strictness`, plus `flex-grow: 1` for `strictness == 0`
  (which `tax_refund` grows) — suppressed when a sibling uses `Expand`, because
  `tax_refund` hands Expand all the slack and returns.
- **Padding larger than the box.** afterhours clamps the *content* area to zero
  (`fmaxf(0, computed - padd)`) and keeps the box at its stated size; CSS
  border-box instead floors the box at the padding. `sports_settings`' stepper
  arrows are `pixels(40)` with 25.6px of side padding, so CSS made them 51.2 and
  every sibling shifted. The mock now drops padding that exceeds a `Pixels` box.

Worth noting as an afterhours *design* question rather than a bug: padding
exceeding the element is accepted silently and yields a zero-width content area.
That is over-specification in the screens — `pixels(40)` with 51.2px of padding
is not meaningful — and nothing warns.

### 11. Family B — shrink-to-fit around a label — **wm (mock)**, won't-fix
**Mock-side, expected.** `Children`-sized boxes take their width from text, and
browser font metrics are not raylib's. Listed so they are not re-filed.

`horizontal_drag` (7), `setting_row_showcase` (some of its 11), and one node
each on `layout_bug_repros` (-186) and the `dialog_*` screens. Survives the
fixes above, as expected — nothing about `strictness` or padding changes what a
browser thinks a string is wide.

### 12. Family C — cross-axis align is skipped when the child overflows — DONE
Fixed in afterhours `17b982c`. **Root cause was verified before changing anything.** `compute_relative_positions` wraps the whole
align block in `if (cross_remaining > 0.f)`
(`vendor/afterhours/src/plugins/autolayout.h:1428`), so when a child is *larger*
than the parent's cross-axis content box, `align_items: Center` silently
degrades to FlexStart. CSS centers regardless, overflowing symmetrically.

Arithmetic, checked on three screens:

| screen | parent | content | child+margin | free | predicted dy | observed |
|---|---|--:|--:|--:|--:|--:|
| `images` | `row1_sprites` | 95.6 | 104.4 | -8.8 | -4.4 | **-4.4** x8 |
| `buttons` | `row1_states` | 51.2 | 59.4 | -8.2 | -4.1 | **-4** x7 |
| `layout` | `row_container` | 91.2 | 108.8 | -17.6 | -8.8 | **-9/-8** x3 |

All three parents are `Row` with `align_items: Center`. Also explains
`layout_patterns` (3), `button_variants` (2), `cards` (1) and part of
`meters_gauges` — about **27 origins across 7 screens**.

Fixed by letting Center and FlexEnd use a negative `cross_remaining` instead of
bailing — one line, `> 0.f` to `!= 0.f`, since both already compute correctly
from a negative remainder. Moved 9 screenshot baselines. Regression tests
`align_items_center_row_child_overflows_cross_axis` and the FlexEnd twin.

**Not** covered by this: `deadspace_settings` (9 origins, dy+9). Its parent
`sidebar` is a `Column` with `align_items: FlexStart` and **+281px** of free
space, so it is main-axis stacking, not cross-axis align. Still unexplained —
track it separately.

### 13. Family D — height disagreements — **unknown**, mixed
- `multiline_text_lab` — `Pixels/Text`, dh -54 and -72. The only `Dim::Text`
  users in the whole sweep, and the biggest height gaps. Wrapped line count
  differs, so this is text metrics again, but far larger than Family B.
- `file_tree` — x2 `Percent/Expand` (0,-17,0,22). Expand resolving to a
  different height, worth a look after the cross-axis expand fix.
- `decorators` — x2 `Percent/Children` (0,7,0,-14).
- `forms` — x3 `Percent/Percent` (0,-1,0,-13).
- `drag_drop` — x4 `Percent/Percent` (0,0,0,-3).

### 14. Family E — uniform diagonal offset — **unknown**
`decorative_frame` (2,2) and (8,8); `cozy_cafe` (4,4) and (14,14). dx equals dy
exactly.

**The border hypothesis is wrong.** `HasBorder` exists
(`components.h:398`) but `autolayout.h` never reads it — a border is drawn
inside the rect and does not inset the content box. So adding border width to
the dump would not help, and putting a CSS `border` on these nodes would
actively break them under border-box. Cause still unknown; the equal dx/dy is
the only lead.

### 15. Single-screen outliers — **unknown**
- **`islands_trains_settings` — 80 origins, the largest single screen.** raylib
  renders a 430px centred modal; the CSS re-solve spreads it far wider (Δx up to
  361 on the buttons). One of the two is wrong about `align_items: Center` on a
  `ScreenPercent` child. Not explained by Family A.
- `themes` — one node at dx 278.
- `meters_gauges` — one node at dx 115.
- `pagination` (x6 dw 3), `parcel_corps_settings` (x6 dw 7) — uniform small
  widths, possibly Family A.
- `cards`, `kirby_options`, `vstack_showcase`, `hstack_showcase` — 1-2 nodes
  each, sub-5px. Noise-adjacent; check last.

### 16. Known mock limitations — DONE
- ~~No `strictness`~~ — modelled, see item 10.
- ~~Padding exceeding the box~~ — modelled, see item 10.
- ~~No content-size floor~~ — `min-width/height: 0` on every node, since a flex
  item's automatic minimum is its content size and afterhours has no equivalent.
  No measurable effect on its own, but it is correct and stops a labelled
  fixed-size box reading as a disagreement later.
- ~~206 nodes unreachable under scroll/clip~~ — **down to 3** (`aim_chat`). They
  were never really a clipping problem; the cascade from the three bugs above
  was reaching into scroll subtrees.
- Border width in the dump — **not needed**, see item 14.
- Absolute nodes are replayed from afterhours' own answer rather than re-solved.
  Correct (a caller-supplied coordinate has no second opinion) but it means the
  mock cannot catch a bug in absolute placement. Unchanged, by design.

### 19. `--update-baselines` rewrites every e2e capture — DONE
`baseline_screenshots/` (24 PNGs, used by the `validate_screen` e2e command) is
separate from `screenshot-baselines/screens/` and is refreshed by running the
e2e suite with `--update-baselines`. That flag rewrites **every** capture
regardless of whether it was failing.

Found the hard way on item 12: the run rewrote 14 of the 24, including
`checkboxes` (6.9% different) and `themes` (4.6%) whose clean renders had not
moved at all. Stashing the fix and rebuilding showed pre-change was 100/100, and
re-applying it failed exactly **one** script — so 13 of the 14 were pre-existing
sub-threshold drift being silently baked in. They pass because the threshold is
1%, but they were never pixel-identical.

Fixed: `--update-baselines` now rewrites only captures that are missing or
actually over the threshold, and logs `Kept baseline: <name> (within threshold
at N%)` for the rest so the drift is visible instead of silently absorbed. The
failing threshold and the refresh threshold are now one constant, since they
have to agree or a refresh reintroduces exactly what the check let through.

Verified by corrupting `themes.png` and re-running: one file rewritten, the
other 23 kept, and the drift printed for each (`checkboxes` 0.53%, `forms`
0.33%, `toggle_switches` 0.32% — the pre-existing drift that used to get
committed unnoticed).

---

## Other repos (not ours to land)

### 17. floatinghotel: five `with_roundness(px)` sites render as pills — **floatinghotel**
`src/ui/diff_renderer.h:614`, `src/ecs/main_content_system.h:815`,
`src/ecs/sidebar_system.h:700`, `:706`, `:781` pass `4.0f` / `2.0f` into a 0..1
fraction. Both backends clamp the computed radius to half the short side, so on
a 28px row `4.0f` is a 14px radius — a full pill, not 4px corners.
`with_corner_radius(4.f)` is the fix. **Owner will do this on their next bump**;
the new warn will point at each one.

### 18. Reconcile the five gap docs — they list shipped work as blockers — **other repos**
`floatinghotel/docs/afterhours-gaps.md` (last touched Aug 1) lists six "Missing
Primitives", all with app-local workarounds. Every one now exists upstream —
`menu.h` and `overlay.h` were added Aug 3, two days later:

| doc says missing | actually in afterhours |
|---|---|
| Draggable Divider — BLOCKER P0 | `divider()` `imm_components.h:338` |
| Split Pane — BLOCKER P0 | `hsplit_pane` / `vsplit_pane` `:417` |
| Tree Node — BLOCKER P1 | `tree_view()` `tree_view.h:110` |
| Dropdown Menu — HIGH P0 | `dropdown_menu()` `menu.h:208` |
| Context Menu — HIGH P1 | `context_menu()` `menu.h:237` |
| Anchored Popover — MEDIUM P0 | `popover()` `overlay.h:248` |
| No rich text / multi-color | `with_styled_label(std::vector<TextSpan>)` |
| Virtualized list | `virtual_list()` `imm_components.h:141` |
| Row flex broken with `expand()` | 4 passing tests, incl. `expand_fills_remaining_row` |

hanabi is the same in places (#29 placeholder → `with_placeholder` exists; #23
virtualization → shipped; #18 "no flex-grow" → `expand()` is tested), though it
self-marks several RESOLVED already.

They are maintaining app-local `split_panel.h`, `tree_view.h`, `menu_setup.h`
and `context_menu.h` against gaps that closed. Worth a pass marking each shipped
item with the API to migrate to.

---

## Done this session, for reference

- afterhours `e8c1e18` — scroll-view child positions (the repositioning pass
  double-counted margins and existed twice, drifted); cross-axis `expand()`
  resolving to 0; `FlexWrap` doc said Wrap was the default, it is NoWrap.
- afterhours `49d5b74` — `with_corner_radius(px)`, and `with_roundness(r > 1)`
  now warns once per value.
- wm `cfad38d` — 8 stale baselines refreshed, 11 missing ones added.
- wm `bd7cb64`, `ee388bc` — mock contact sheets; fixed two mock bugs that faked
  ~95% of the reported diffs (a 1px `border` on every node accumulating with
  depth, and absolute nodes placed with screen-space coords as CSS `left/top`).
- wm `25c597e` — six screens covering the previously untested primitives.

---

## What's next

2. **Item 15** — `islands_trains_settings`, 47 origins and by far the largest
   single screen left.
3. **Item 9** — corner radiuses, in its own commit.
4. **Items 1 and 5** — font weight and right-click: missing capability rather
   than open questions, so they need no triage first.

Current sweep state: **18 screens / 84 origins**, of which 44 are text-metric
(a known mock limitation) and **40 are real candidates**. Started at 44 screens
/ 383 origins. Regenerate with `./mocks/build.sh --no-serve && ./mocks/sheets.sh`.

Cleared since: the cross-axis align fix (item 12, -31 across 7 screens), the
over-padding rule extended to ScreenPercent (`islands_trains_settings` 47 -> 7),
and cross-axis Expand no longer setting flex-grow in the mock
(`deadspace_settings` 9 -> 0).

A subagent triage pass on items 12-15 produced only hypotheses and reversed
itself on two of them (it asserted the Family E border theory at high
confidence after it had already been disproved). Nothing from it is recorded
here that was not independently verified.
