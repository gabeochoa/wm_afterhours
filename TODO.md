# TODO

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
| 1 | `with_font_weight` is a no-op | font variants + text path | | |
| 2 | Alpha blending on sokol | sokol blend mode | | verify from floatinghotel |
| 3 | No scrollbar for `HasScrollView` | render + hit-test | | |
| 4 | Overflow is silent | warning tolerance | over-specified screens | |
| 5 | No right-click | button on the click path | `context_menu_lab` after | |
| 6 | `context_menu` clips shortcuts | reserve the column | | |
| 7 | Never-implemented gap requests | most of them | e2e pack upstreams **from** wm | |
| 8 | Slider knob, tab bars, hard wrap | all three | | |
| 9 | Corner radiuses | theme default + precedence | call sites + baselines | |
| 10 | Family A (115 origins) | *maybe, after* | model `strictness` in the mock | |
| 11 | Family B (font metrics) | | won't-fix, documented | |
| 12 | Family C (vertical drift) | **likely** | | |
| 13 | Family D (heights) | likely for `file_tree`/`forms` | text metrics for `multiline_text_lab` | |
| 14 | Family E (diagonal offset) | *maybe, after* | put border width in the dump | |
| 15 | Outliers, `islands_trains` 80 | **likely** | | |
| 16 | Mock limitations | | `mocks/`, `ui_tree_dump.h` | |
| 17 | Five `with_roundness(px)` sites | | | floatinghotel |
| 18 | Gap docs list shipped work | | | 5 docs, 4 repos |

Counts: **5 afterhours-only** (1, 2, 3, 6, 8), **2 wm-only** (11, 16),
**4 both** (4, 5, 7, 9), **2 other-repo** (17, 18), **5 unknown until triaged**
(10, 12, 13, 14, 15).

Every unknown is a sweep finding, and each one needs a wm-side mock change
*before* it can be assigned to anybody — which is why item 10 leads the ordering
at the bottom. Expect most of the 5 to resolve to afterhours or to won't-fix
once `strictness` is modelled.

---

## afterhours (library)

### 1. `with_font_weight` is a visual no-op — HIGH — **afterhours**
All five weights render pixel-identically on `font_weight_720p`. The API landed
but the capability did not: the loaded font has no weight variants, so nothing
selects one. floatinghotel asked for this (semi-bold diff headers) and would
reasonably think it works.

Needs font files per weight plus variant selection in the text path, or the
builder should warn that the weight was ignored. **Do not close floatinghotel's
"No Font Weight Support" on the strength of the API existing.**

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

### 5. No right-click trigger for `context_menu` — HIGH — **afterhours**, then wm
There is no secondary-click support anywhere in afterhours: `MOUSE_BUTTON_RIGHT`
does not appear in the source, and `HasClickListener` has a single `down` with
no button field. So `context_menu()` exists but cannot be opened the one way
users expect — `context_menu_lab` has to drive it from ordinary buttons.

Wants a button on the click path (`HasClickListener` gaining which button, or a
`HasRightClickListener`) plumbed through `ResolveHitTarget`/`HandleClicks`, and
then `context_menu` opening at `ctx.mouse.pos` on a right-click within the
target. Update `context_menu_lab` to use it once it exists.

### 6. `context_menu` clips the shortcut column — **afterhours**
`context_menu_lab`: "Cmd+C" loses a glyph at the panel's right edge. The
shortcut is right-aligned to the panel without reserving room for itself.

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

### 9. Corner radiuses — make them consistent — **both**
Decided, not yet done. Today `Theme::roundness = 0.5f` scales with the widget,
so the same theme value is ~8px on a row and 180px on a full-height panel; the
mock sheets show the big containers as blobs. `with_corner_radius(px)` now
exists, so the work is:

1. Give `Theme` an absolute default radius and let it win where the caller did
   not ask for a fraction. **Precedence has to be explicit-px > explicit-
   roundness > theme-px > theme-roundness** — right now `component_init.h:119`
   fills both from the theme unconditionally and `resolve_roundness` prefers px,
   so a theme px would silently beat a caller's explicit `with_roundness`.
2. Sweep wm's own `with_roundness` call sites to px where they meant px.
3. Re-baseline. This is why it is its own commit: it repaints every screen.

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

718 nodes differ. 206 sit under a scroll/clip container, where the mock has
nothing to compare against because it does not clip. Of the rest, **383
originate a difference** — the others merely moved with a parent that already
had. Deltas below are `(dx, dy, dw, dh)` in px, CSS minus afterhours.

Nobody has been through these. Each is either an afterhours bug or a screen
over-specifying itself, and the families below are a guess at the grouping, not
a verdict.

### 10. Family A — fixed-width children in a row that does not fit — **wm (mock)** first
**Suspected mock-side. Resolve this first: it is the single biggest group and
until it is settled none of these can be filed.**

The mock **ignores `strictness` entirely**. CSS flex items default to
`flex-shrink: 1` so the browser shrinks an overflowing row; afterhours honours
`strictness = 1` and refuses. Separately `tax_refund` *grows* `strictness = 0`
children, which has no CSS equivalent either. Both would show up exactly as
these do — same row template repeated, children trading a few px.

| screen | origins | shape |
|---|--:|---|
| `sports_settings` | 62 | x12 `Pixels/Percent` (0,0,-5,0), x12 `Pixels/Pixels` (-6,0,11,0) |
| `powerwash_settings` | 36 | x9 (0,0,-9,0), x9 (-9,0,7,0) |
| `stepper_showcase` | 32 | x6 `Pixels/Percent` (0,0,27,0), x6 (-27,0,27,0) |
| `flex_alignment` | 15 | x5 (0,0,-9,0), x5 (-19,0,-9,0) |
| `adaptive_scaling` | 4 | (0,0,15,0) / (15,0,0,0) |
| `rubber_bandits_menu` | 3 | `Expand/Pixels` (0,0,-27,0) |
| `angry_birds_settings` | 2 | `Expand/Pixels` (0,0,-7,0) |
| `casual_settings` | 1 | (-11,0,11,0) |

Fix: map `strictness` onto `flex-shrink`/`flex-grow` in `mocks/mock.js`, re-run,
and see what survives.

### 11. Family B — shrink-to-fit around a label — **wm (mock)**, won't-fix
**Mock-side, expected.** `Children`-sized boxes take their width from text, and
browser font metrics are not raylib's. Listed so they are not re-filed.

`dialog_fyi` (-136), `dialog_confirm` (-125), `dialog_info` (-123),
`dialog_danger` (-106), `dialog_prompt` (-97) — all one node, `Children/Percent`;
`layout_bug_repros` (-186), `horizontal_drag` (-141), `setting_row_showcase`
(-52 on 2 of its 18). 44 of the 383 carry the text flag.

### 12. Family C — small vertical drift, no size change — **unknown**, likely afterhours
**Unexplained. Most likely to be real.** One element sits a few px off and
everything after it follows; the origin count is small because the fallout is
discounted.

| screen | origins | shape |
|---|--:|---|
| `images` | 10 | x8 (0,-4,0,0) |
| `deadspace_settings` | 9 | x8 `Percent/Pixels` (0,9,0,0) |
| `buttons` | 8 | x7 (0,-4,0,0) |
| `toggle_switches` | 12 | (0,-3,0,-2), (0,-5,0,-2) |
| `layout` | 6 | x3 (0,-9,0,0), x3 (0,-8,0,0) |
| `layout_patterns` | 3 | x3 (0,-6,0,0) |
| `button_variants` | 2 | x2 (0,-3,0,0) |

### 13. Family D — height disagreements — **unknown**, mixed
- `multiline_text_lab` — `Pixels/Text`, dh -54 and -72. The only `Dim::Text`
  users in the whole sweep, and the biggest height gaps. Wrapped line count
  differs, so this is text metrics again, but far larger than Family B.
- `file_tree` — x2 `Percent/Expand` (0,-17,0,22). Expand resolving to a
  different height, worth a look after the cross-axis expand fix.
- `decorators` — x2 `Percent/Children` (0,7,0,-14).
- `forms` — x3 `Percent/Percent` (0,-1,0,-13).
- `drag_drop` — x4 `Percent/Percent` (0,0,0,-3).

### 14. Family E — uniform diagonal offset — **wm (dump)** first
`decorative_frame` (2,2) and (8,8); `cozy_cafe` (4,4) and (14,14). dx equals dy
exactly, which smells like a border or frame inset the mock does not model —
these screens use the bevel/nine-slice borders. Check whether border width is
in the dump at all.

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

### 16. Known mock limitations to fix before the next sweep — **wm (mock)**
- No `strictness` → see item 10.
- No clipping, so 206 nodes under scroll/clip cannot be compared at all.
- Absolute nodes are replayed from afterhours' own answer rather than
  re-solved — correct (a caller-supplied coordinate has no second opinion) but
  it means the mock cannot catch a bug in absolute placement.
- Border width is not in the dump — see item 14.

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

1. **Item 10** — map `strictness` in `mocks/mock.js`. 115 of the 383 origins are
   in Family A and none of them can be judged until this is done.
2. **Item 9** — corner radiuses, in its own commit.
3. **Item 15** — `islands_trains_settings`, the 80-origin outlier Family A does
   not explain.
4. **Items 1 and 5** — font weight and right-click, the two HIGHs that are
   plainly missing capability rather than open questions.
