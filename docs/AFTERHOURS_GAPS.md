# Afterhours Library Gaps & Workarounds

wm_afterhours vendors afterhours (git submodule) **and** maintains it, so gaps
found during UI work get fixed upstream, merged, and pulled in via a submodule
pin bump — not worked around in wm source. This file tracks only what's still
**open**; resolved gaps are listed compactly for the record.

**Every entry here was rechecked against `main` on 2026-09-12.** That sweep is
worth repeating before trusting any of it, because the consumer docs these were
collected from are all months stale: of everything collected this cycle, hanabi
had 6 of their top 10 already fixed, cartographer's component asks were almost
entirely in, floatinghotel's footgun list was half done, and three of their four
sokol items had landed. The most valuable output of a collection pass is now
telling the projects what they can delete, not finding new work.

See also: `docs/vendor_ui_sizing_issues.md`

---

## Screen mock audit

This section also records wm-owned findings from the 26-screen mock audit.
It does not classify a visual mismatch as a library defect. Library changes
and the submodule pin are frozen for this pass; proven upstream blockers
remain open for user review. Delivery status: `docs/screen-audit.md`.

| Screen / state | Finding | Owner | Disposition / evidence |
|---|---|---|---|
| Hosted gallery / comparison views | Relative image and font URLs break outside the repository. | wm tooling | Standalone export embeds all 48 assets; 26 canvases and all comparison images verified outside the repo. Pixelcloud revision 4 opens directly in a draggable Original vs Mock/Current comparison; pointer and keyboard checks passed. |
| potion_crafting / recipe view | Dark panels, text initials for ingredients, and a rectangular flask did not match the parchment composition. | wm | Replaced with parchment layout, botanical/bottle artwork, scaled geometry and Garamond text; reviewed at 720p and 1080p. |
| potion_crafting / crafting | Brew ignored clicks; tabs did not change content; recipe row IDs overlapped. | wm | Unique IDs, live stock consumption, shortage handling, recipe-aware ingredient requirements, inventory and journal views. E2E 40 exercises pointer, real Tab traversal, Enter, repeated shortage and resize. |
| potion_crafting / fine styling | Secondary copy lacks the target's italic face; brew button's inset border is subtler; footer gives keyboard guidance instead of the decorative Close workshop hint. | wm | Open visual polish. No library blocker claimed; gameplay and layout checks pass. |
| angry_birds_settings / layout | Flat board, mismatched icons and uneven button caps did not resemble the mobile settings mock. | wm | Shared label-free forest, board and button artwork, high-resolution Fredoka atlas, and explicit scaled geometry. Fresh 720p/1080p captures reviewed independently. |
| angry_birds_settings / interactions | Language, help and progress actions lacked useful views. | wm | Real language selection, separate campaign save/load state, credits/support/privacy views, notification/audio settings, apply/cancel, close/reopen and Escape. E2E 128 and responsive/containment regressions pass. |
| angry_birds_settings / fine styling | Native font outlines are heavier; hint baseline is about 7px higher; faint background currency pill is absent. | wm | Open cosmetic differences; primary layout and control geometry match. |
| cozy_cafe / layout | Mis-sized panels, substitute icons, faint borders and clipped footer/music labels differed from the cafe mock. | wm | Matched panel geometry, isolated flower/star/tool artwork, scaled outlines and improved type. Fresh 720p/1080p captures; independent reviews. |
| cozy_cafe / service and selection | Serving incremented counters without removing orders; selected specials lacked a visible state. | wm | Serve consumes the next order and disables when empty, promotion uses the selected special, visible selection outline and live music control. E2E 129 checks pointer, Tab/Enter, depleted queue and resize. |
| cozy_cafe / fine styling | Paper lacks the mock's grain, radial glow, inset arcs and corner fold; native text outlines are stronger and title slightly wider; customer separators use hyphens. | wm | Open cosmetic differences. Correctness, overflow and parent containment checks pass. |
| casual_settings / layout and dialogs | Generic colors, substitute icons and inert information actions differed from the mobile mock. | wm | Shared decorative board/icons, matched geometry, high-resolution rounded type, language apply/cancel, campaign save/load, credits, support FAQ, terms and about views. E2E 130 passes at both resolutions, including dialog containment and keyboard use. |
| casual_settings / fine styling | Faint lives/currency pills are simplified to a currency label; native label outlines differ slightly. | wm | Open cosmetic differences. Fresh 720p/1080p captures independently reviewed; text-overflow warnings resolved. |
| fighter_menu / rendering and navigation | Missing lobby, invisible slanted triangles, blurred display type and letter substitutes for icons weakened the fighter layout. | wm | Fixed triangle winding and background layering, added isolated lobby/icons and a 96px display font. Live tabs and option descriptions; E2E 131 passes pointer, Enter/arrows and 1080 resize. Fresh images independently reviewed. |
| fighter_menu / fine styling | Title/hologram lack the mock's rotation and perspective; serif weight is thinner; lobby and panel shading are approximate. | wm | Open cosmetic gaps. No clipping or interaction blocker found in the final 720p/1080p review. |


| flight_options / layout and settings | Generic options layout and shallow interactions did not match the flight-system mock. | wm | Matched the airspace grid, selection geometry, nine categories and setting editors. Apply/cancel retains or discards values; keyboard and pointer coverage in E2E 134. |
| flight_options / fine styling | Native lettering is thinner and lacks the browser glow; keycap labels are smaller. Vibration is explicitly marked unavailable. | wm | Open cosmetic differences and a stated hardware limitation; final 720p/1080p review found no clipping or interaction blocker. |


| deadspace_settings / hologram and controls | Rectangular slabs, invisible corner triangles and inert categories missed the hologram design. | wm | Winding-correct clipped polygons, scan lines, layered menus and live category detail settings; E2E 132 passes pointer/keyboard/back and resize. |
| deadspace_settings / fine styling | Rows lack the mock's perspective skew and stronger glow; rear panel contrast and keycaps differ. | wm | Open cosmetic differences. Final 720p/1080p captures independently reviewed with no clipping or unreadable selection. |


| kirby_options / notebook and preferences | Plain panels and decorative-only options missed the notebook composition and useful interactions. | wm | Isolated paper, tab and icon artwork with native labels; editable name, per-category preferences, favorite mode, tab views and confirmed profile reset. E2E 137 covers pointer, Tab/Enter and resize. |
| kirby_options / fine styling | Native type spacing/weight and pencil artwork differ slightly; bumper hints use purple rather than dark keycaps. Online view is explicitly local. | wm | Open cosmetic differences and stated simulation scope. Final 720p/1080p images independently reviewed. |


| minesweeper_lab / desktop and game | Generic chrome, unreadable counters and overlapping taskbar text missed the classic desktop mock. | wm | Native desktop/window/taskbar, high-resolution font aliases, separate labels for icon buttons, real game reset and window state controls. Existing play test 120 and new 139 pass. |
| minesweeper_lab / fine styling | Counter glyphs are heavier, bevel corners are square instead of diagonal, menu underlines are absent and window text spacing differs. | wm | Open cosmetic differences; board gameplay preserved and 720p/1080p reviewed. Access-key underlines remain an existing library gap below. |


| empire_tycoon / dashboard and state | Hidden park art, substitute icons, misplaced controls and faint borders weakened the tycoon composition. | wm | Isolated park/icons, winding/layering fixes, scaled outlines and dashboard geometry. Live production, cash, projects, gauges and milestone react to actions; E2E 133 passes. |
| empire_tycoon / fine styling and scope | Logo remains upright; plates/buttons lack target gradients and inset highlights; body text uses a heavier rounded face; gauges and chat tail are simplified. Tool buttons report selection without full destination screens, as in the mock. | wm | Open visual polish and bounded demo navigation. 720p/1080p reviewed; no library changes. |

| mini_motorways_settings / layout and categories | Typography, detached controls and road bend artifacts missed the map-like settings mock. | wm | Native grid/road geometry, scaled controls and real per-tab content; ring-based bend eliminates draw-segment artifacts. |
| mini_motorways_settings / fine styling | Font tracking, symbol edges and exact tutorial/version text dimensions differ slightly. | wm | Open cosmetic differences; E2E 140 covers pointer, keyboard, values and containment at both resolutions. |

| neon_strike / HUD and actions | Generic HUD placement, missing artwork and inert controls differed from the mock. | wm | Matched map/compass/equipment geometry with isolated text-free art and native high-resolution labels. Reload transfers reserve ammunition; abilities and equipment have visible selection. |
| neon_strike / fine styling | Ammo uses one font size instead of mixed sizes; objective chevrons and subtle background glow differ. | wm | Open cosmetic differences. E2E 141 and final 720p/1080p reviews pass. |


| marlo_kart / six-phase presentation | Driver/kart/cup screens and racing presentation did not match the mock; custom roads/maps were accidentally covered by panel fills in the first audit draft. | wm | Isolated driver/kart/trophy art, live text and HUD, perspective road with moving stripes, visible shared-path minimap/cup previews, pause above countdown. Existing fixed-step racing, items, drift, eight racers, four cups and points remain intact. E2E 126/127/138 pass. |
| marlo_kart / fine styling | Card rounding, label proportions and key hints differ slightly from the mock; native race view follows the real simulation rather than the gallery's phase shortcuts. | wm | Open cosmetic differences; all six phases visually reviewed, including actual cup completion and trophy standings. High-resolution regular font removes enlarged-text blur. |

---

### Capture runner: multiple resolutions in one process

`--headless-screenshots --screen example_borders --resolution 720p,1080p`
crashes at the second resolution in `EntityCollection::invalidate_entity_slot_if_any`
with an out-of-range slot. Reproduced on unchanged `example_borders`, independently
of the new screen layout. Separate 720p and 1080p invocations work. Owner is not
established between wm reset sequencing and the library collection lifetime;
afterhours remains frozen. This is not an implementation of the unrelated
cascade-delete plan.

### Absolute horizontal positions resolve against screen height

At the frozen pin, `component_init.h` resolves both `translate_x` and
`translate_y` using `screen_height`. Passing `w1280(1047)` as an absolute x
therefore produces 589px at 1280×720 instead of 1047px. The first Angry Birds
audit draft reproduced the misplaced controls. wm now supplies explicit pixel
positions scaled from the current UI context. Library behavior remains open
for review; no vendor changes were made.

### Uniform solid borders ignore requested thickness

`rendering.h`'s uniform-border branch calls `draw_rectangle_rounded_lines`
without passing `BorderSide::thickness`. The rounded helper renders a 1px
outline; the sharp helper uses a fixed 3px. Cozy Cafe's requested 4px paper
border and 3px panel borders consequently appeared as faint 1px lines.
The wm screen draws its outlines with the existing thickness-aware helper.
A library fix is deferred for user review. This also explains the subtler
Potion Crafting inset border noted above.

## Open, asked for by other projects

### From floatinghotel's footguns list

Their July list, rechecked against `main`. **All of it has now landed** --
label word-wrap, `Dim::Text` as a measured-width unit, `with_corner_radius`,
row-flex `expand()`, index-based tick iteration (their heap-use-after-free),
the button-inside-a-clickable-row hit priority, the sokol group, and the
absolute-stacking one via `with_overlay`.

### From hanabi's triage

hanabi keeps a 14,000-line gap file and an index ranking their top ten by pain
per line of upstream change. Rechecked against `main`, not their pin
(`428047e`): **all ten are now closed** -- six were already in when first
collected, and the rest landed this cycle. Nothing outstanding from that list.
Worth telling them: they can delete `src/util/atlas_guard.h` (~50 lines) and
`src/ui/focus_visible.h`, and drop three hand-rolled virtualization windows for
the `height_of` overload.

### Components

- **access-key underlines** (wordproc) — per-character decoration, so `&File`
  can underline the F. Needs the renderer to decorate one glyph in a run, which
  the run machinery from the wrap work could carry. Filed as accessibility.

### E2E

- **command handlers are registered per SystemManager** (cartographer) — each
  manager needs `register_all_handlers`. Not silent, as their doc says: the
  runner blocks on an unconsumed command and times out, and the timeout now
  names what stalled and points at an unregistered pack. The boilerplate
  remains. Both automatic fixes were costed and rejected -- a registry of live
  managers needs hand-written move members (`SystemManager` is returned by
  value, wm builds ~92 a run), and lazy first-tick registration inverts the
  documented order.

### Harness

- **`wait` resolution is quantised by the substep batch**
  (endless-dance-chaos, minor) — a `wait` cannot land inside a batch of
  substeps, so its resolution is the batch. They rated it low and documented it
  on their side; keeping `sim_steps` small is the whole workaround.

### Wrappers

- **raw `raylib::` calls with no wrapper** (puzzle gap 18) — they count 79 raw
  sites, but the count overstates it: the top entries are *types*
  (`raylib::Color` x57, `Image` x18, `Vector` x10, `Font`, `Rectangle`,
  `Camera`) and enum constants, which afterhours already aliases and they could
  swap today. Rechecked across `backends/` and `plugins/`, what is genuinely
  unwrapped is: the image pixel API (`ImageDrawRectangle`, `GenImageColor`,
  `ImageFormat`, `GetPixelDataSize`, `GetImageColor`), plus
  `ToggleBorderlessWindowed`, `IsAudioDeviceReady` and `GetKeyPressed`.
  `LoadImageFromTexture` and `ImageFlipVertical` exist inside afterhours but
  are not exposed.
  Only the image group is a coherent abstraction; the rest are one-function
  wrappers in unrelated areas. **Not started**: wm manipulates no images, so it
  would be unexercised API designed from a grep of a consumer that cannot be
  built here. Ask puzzle which calls actually block them first.
  (Blend mode is *not* among them -- `set_blend_mode`/`blend_scope` are wrapped
  in all three backends and raylib's reaches `rlSetBlendMode`.)

### Diagnostics

- **no lint for custom colours bypassing the theme** (cartographer) — theme
  usage is easy to skip by accident and nothing catches it in shared UI.

### Core

- **relationship integrity has no index** (puzzle) — they hand-wrote one after
  a node-delete sweep reaped ports and wires and forgot knobs and sliders.
  Their point was explicitly not speed: an indexed relationship "cannot be
  half-written in the first place". This is a data-model change, not a query
  optimisation, and the two were filed here as one bullet for a while. There is
  **no** written plan -- an earlier one was overwritten. Wants a planning pass
  against puzzle's actual code before any implementation.

- **EntityQuery allocation and sorting** (MyNameChef) — mostly already done and
  their doc predates it: `run_query` has a `stop_on_first` path that allocates
  nothing, reserves upfront, and sorts only when `orderby` is set with more
  than one result. Their remaining items are the smaller ones -- consolidating
  the tag query methods, and the `permanant_ids` typo, which is fixed. They
  also ask to remove the `OptEntity` wrapper, which is the opposite of the
  direction this repo went.

- **kart's remaining component extractions** — `HasLabels`, `CanWrapAround`,
  `TeamID`, `ManagesAvailableColors`. Arguably game-specific; listed for
  completeness, not recommended.

---

## Known limitations (open, low priority)

- **tab order is allocation order, not tree order** — `process_tabbing` moves
  focus by setting it to `ROOT` and letting the next entity the iteration
  reaches grab it, so Tab follows the entity collection's storage order rather
  than the widget tree. Two widgets side by side tab in whatever order they
  were allocated, which is build order today and need not stay that way; there
  is no tab index and no way for a caller to state the order it wants.
  Order-preserving `cleanup()` (`58e1613`) stops widget retirement from
  reshuffling it, which **hides this rather than fixing it** — the order is
  stable now, but it is still the wrong order to be deriving from. A real fix
  sorts focusables by tree position, with an explicit override.

- **a virtualized list still builds a real div for the rows above the fold** —
  `virtual_list` folds the rows *below* the window straight into
  `content_size` via `HasScrollView::unbuilt_content_size`, but the rows above
  it are a `vlist_skipped_above` div of the right height. It cannot be folded
  the same way because the scroll offset is applied when drawing rather than
  when laying out: with nothing in front of it the first built row lays out at
  the top of the content and is then drawn off screen. Folding it needs
  children offset during the layout pass. One div, so the cost is negligible;
  it is the asymmetry that will confuse the next reader.

- **slider handle 0.75 compression** — the knob's center never quite reaches the
  value position at 100% (cosmetic). `imm_components.h` `slider`. Revisit the
  handle width/position model only if it becomes visible.
- **crowded tab bars still need a smaller font at the call site** — `tab_container`
  content-fit (`expand()` + `min_width = Dim::Text`) removes truncation only when
  the labels *can* fit. A bar with many long labels (e.g. `flight_options`' 9 tabs
  at the default font) still overflows; set a smaller font on the tab_container
  config for those.
- **word-wrap has no hard character break** — with `TextOverflow::Wrap`, a single
  word wider than its box goes on its own line (not split), and wrap requires an
  explicit font size (auto-fit + wrap is ambiguous).

Batch/headless screenshot determinism is a wm-side tooling concern, not an
afterhours gap — tracked in `docs/LAYOUT_AUDIT.md`.

---

## Resolved & upstreamed (merged into afterhours main, pinned `e348efb`)

Six of hanabi's top ten are already in and they do not know it -- their pin is
`428047e`. Worth telling them rather than waiting for the next bump:

- **font atlas exhaustion** (their #1, #351/#350/#352/#353) —
  `fonsSetErrorCallback` is registered and `FONS_ATLAS_FULL` is reported. Their
  fifty-line `src/util/atlas_guard.h` can go.
- **widget retirement** (#115) — the frame stamp and end-of-frame sweep landed
- **GPU pool sizes** (#210) — `AFTERHOURS_SG_{PIPELINE,IMAGE,SAMPLER,BUFFER}_POOL_SIZE`
- **parent containment** (#275) — `assert_within_parents`, which is the
  assertion they asked for: `assert_no_overflow` measured against the viewport
- **measure without building** (#224) — `plugins/ui/measure_config.h`
- **diagnostics** (#192/#161/#113) — landed in `2caf525`

- **`--headless` was parsed and then dropped** (endless-dance-chaos) —
  `RunConfig::display` plus `E2EArgs::display_mode()`, and raylib's `run()`
  skips window setup and teardown when headless. `Config::display` already
  existed and `raylib_init` already honoured it, so backend selection worked
  all along -- the `run()` path was the whole gap, which is why the flag looked
  wired up. Worth 11.7x to them. Sokol is untouched: it has no headless backend
  to select.
- **no fixed timestep** (endless-dance-chaos) — `RunConfig::fixed_dt` and
  `sim_steps`, with `frame_steps()` returning the schedule. The consumer still
  writes the loop, because `RunConfig::frame` takes no dt and calling it N
  times would render N times rather than simulate N times. `time_scale`
  deliberately does not stretch a fixed step; doing so would silently put back
  the frame-rate dependence the fixed step exists to remove.
- **`synthetic_press_delay` was undocumented and load-bearing** (cartographer)
  — an injected press is not readable on the frame it was injected, which
  matches a real keyboard but read as a broken feature twice. Documented at the
  field and at `set_key_down`, with the timing pinned by test.
- **text editing opted into by enumerator name** (hanabi #255) —
  `has_editing_action<InputAction>()` to static_assert on, and a run-once
  warning naming whatever resolved to nothing.
- **the label origin was an unnamed 5px literal** (hanabi #85) — `kTextInset`
  and `text_inset_for()`. Padding on a label with no children still does
  nothing -- measured, `pixels(12)` and `pixels(40)` give the same 75px box --
  but it now says so once per run instead of never.
- **nothing sized a box to its own text** (hanabi #136) —
  `with_fit_content(max_w, font_size)`. `Dim::Text` and `max_width` already did
  most of it; the gap was that it takes four settings that must agree and three
  of four caps the width while silently not wrapping.

- **an ordered `gen_first` sorted the whole list** (MyNameChef) — it scanned
  and then `std::sort`ed to return one entity. Takes the minimum now, except
  where a stateful mod like `take()` sits after the orderby and needs the real
  order.
- **absolute children stacking** (floatinghotel) — `with_overlay(levels)`:
  absolute plus a layer relative to the parent, so a nested overlay clears the
  one it sits in. Opt-in, because absolute is also how underlays are built --
  a blanket default blanked `file_tree` and `islands_trains_settings`.
- **card preset** — `with_card(pad)`: Surface, rounded corners, padding.
- **font sizes off the type scale** — `ValidationConfig::enforce_font_size_tiers`,
  opt-in.

- **sokol headless, wheel injection, GPU sync** (floatinghotel) — all three
  landed. `g_headless_rt` and the headless branches exist; `consume_wheel()`
  does not consume (it flags `wheel_read`, every reader in a frame sees the
  same value, cleared at the next `reset_frame`); `capture_impl.h` does the
  blit-to-resolve. The wheel one keeps a misleading name, which is likely why
  their doc still lists it.
- **virtualization with variable row heights** (hanabi #326/#170/#224) —
  `virtual_list` has a `height_of(index)` overload that binary-searches a
  running total. They hand-rolled the same window three times.
- **mouse delta through the action mapping** (cartographer) — `MouseAxisWithDir`,
  normalised by `MOUSE_DELTA_SCALE`.
- **advance vs ink measure** (hanabi #137) — sokol already returned the advance;
  the raylib odd-one-out, `measure_text_internal`, is deleted.
- **focus ring painted at rest** (hanabi #83) — off until first interaction.
  Their hand-rolled `focus_visible.h` can go.

- **the two floatinghotel blockers** — the sokol include-order break and the
  flex solver budgeting raw child sizes. Both fixed, with a third snapping site
  they had not found; `sokol_include_order_test` compiles `window_manager.h`
  under `SOKOL_METAL`, which nothing did before.
- **tooltip** — `plugins/ui/tooltip.h`, with `TooltipLab` and a baseline
- **table / grid layout** — `plugins/ui/grid.h`; `RaceResults` and
  `MinesweeperLab` converted to it, plus `GridLab`
- **scrollbar colour and style** — track/thumb usages and explicit colour
  overrides on `HasScrollView`, with `ScrollbarStyleLab`
- **singleton-only systems skip the entity scan** — a system whose components
  are all registered singletons resolves through the singleton map instead of
  walking every entity (puzzle profiled that scan at 37% of a frame)
- tab_container tab strip bounds under `with_absolute_position`
- **render-command sort recycled-id tiebreak** (SEVERE — fixed 76/79 screens;
  root cause behind the modals breakage, first-child-missing-control, and
  cross-rebuild screenshot non-determinism)
- progress_bar fill/label percent compounding, and track percent compounding
- slider handle position on percent-sized tracks
- stepper multi-visible label separation
- tab_container equal-width → long-label ellipsize (now `expand()` + `Dim::Text`
  min width; added `Dim::Text` to `resolve_constraint`)
- static-label word-wrap — new `TextOverflow::Wrap` feature
- `children()` sizing now includes `flex_gap`
- checkbox/toggle focus ring — confirmed **handled**, not a bug (checkbox/toggle
  are focus clusters via `FocusClusterRoot`/`InFocusCluster`; the ring render is
  type-agnostic with an explicit cluster branch)
- toggle_switch "sibling entities consume layout space" — **not a bug** (internal
  entities are children, not siblings)

Regression tests for these live in the afterhours `tests/` suite: `autolayout_test`,
`progress_bar_test`, `slider_test`, `stepper_test`, `tab_container_test`,
`text_wrap_test`, `render_order_test`.
