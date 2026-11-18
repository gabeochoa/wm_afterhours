# afterhours gap work: status

Snapshot 2026-11-16, before a machine restart. PRs are open against gabeochoa/afterhours, and nothing is merged. The ranked gap list is in [PRIORITIZED.md](PRIORITIZED.md).

## Review order

1. **Stacked PRs: merge the base first.** #86 needs #85; #104 needs #88; #121 needs #92; #125 needs #95; #127 needs #94; #138 needs #79. Each says so in its first line.
2. **Visible change:** #119 (colors) moves `UI_RED`/`UI_GREEN`/`UI_BLUE` to raylib's values on sokol, so floatinghotel and hanabi pixels change. `UI_WHITE`/`UI_PINK` are still per-backend and flagged in the PR.
3. **Expect CHANGELOG.md conflicts:** almost every PR adds a line at the top. When merging in sequence, keep both sides.
4. **Full suite:** `make test` was run once with #78 to #86 combined. The only failures were `settings_save_test` (fixed in #82) and `config_size_test` (already failing on main, fixed in #87). Later PRs ran only their own tests and the existing tests for the code they touch. Run the full suite before merging a batch.
5. **Your unpushed local commit** `eab28aa` (DPI mouse fix) on ~/p/afterhours main is not in any PR. Every branch is based on origin/main.

## Open PRs (91, 17 merged 2026-11-17)

| PR | Branch | Base | Title |
|---|---|---|---|
| #78 | `gap/jobs` | main | jobs: worker pool that delivers results on the owner thread |
| #79 | `gap/wraptext` | main | text: wrap_lines returns byte ranges and widths per wrapped line |
| #80 | `gap/e2e-waits` | main | e2e: wait_wall and wait_until_wall wait in real time |
| #81 | `gap/image` | main | image: public CPU decode, size query, and RGBA texture upload |
| #82 | `gap/atomic` | main | files: durable atomic writes with a temp per write |
| #83 | `gap/logging` | main | docs: spell out the replaced-logging host contract |
| #84 | `gap/validate-enum` | main | ui: validate_enum_has_value expands to one statement |
| #85 | `gap/e2e-keys` | main | e2e: graphics key reads go through the injector |
| #86 | `gap/e2e-gamepad` | **gap/e2e-keys** | e2e: gamepad injection for buttons, axes and pad availability |
| #87 | `gap/config-size` | main | ui: merge render_layer_offset in apply_overrides |
| #88 | `gap/textfit` | main | text: ellipsize_text and fit_text_prefix, shared with the renderer |
| #89 | `gap/axis-edges` | main | input: gamepad axes report a pressed edge, not every held frame |
| #90 | `gap/controlled-widgets` | main | ui: slider and toggle_switch follow the caller's value |
| #91 | `gap/test-file-roots` | main | files: TestRoots redirects save, config and temp for a test |
| #92 | `gap/glyph-placeholder` | main | text: draw a visible replacement for codepoints no face covers |
| #93 | `gap/menu-rows` | main | menu: rows take padding, theme usage and hover from the menu config |
| #94 | `gap/font-points` | main | text: font_size_per_point converts point sizes to font sizes |
| #95 | `gap/e2e-command-shape` | main | e2e: register_command with typed arguments, and expect() |
| #96 | `gap/focus-ring-contrast` | main | ui: Theme::focus_ring_contrast turns off the ring's contrast edges |
| #97 | `gap/focus-text-input` | main | ui: set_focus on a text input's returned entity focuses its field |
| #98 | `gap/layout-inspect` | main | e2e: dump_ui writes resolved padding, margin and gap |
| #99 | `gap/hover-tint` | main | ui: with_hoverable and with_skip_hover_override config setters |
| #100 | `gap/drag-hit` | main | ui: drag groups hit-test the scrolled, clipped rect |
| #101 | `gap/grid-snap-extents` | main | autolayout: children() sizes and scroll extents follow snapped placement |
| #102 | `gap/tree-row-renderer` | main | tree_view: render_row callback builds custom row contents |
| #103 | `gap/e2e-type-codepoints` | main | e2e: type queues Unicode codepoints, not UTF-8 bytes |
| #104 | `gap/graphemes` | **gap/textfit** | text: step carets and deletes by grapheme cluster |
| #105 | `gap/font-headless-guard` | main | fonts: raylib file loaders no longer crash without a window |
| #106 | `gap/catalog-builds` | main | examples: catalog makefiles name the none backend |
| #107 | `gap/translation-cstdlib` | main | translation: include <cstdlib> before fmt |
| #108 | `gap/fixed-timestep` | main | core: SystemManager fixed tick rate and catch-up cap are settable |
| #109 | `gap/key-repeat` | main | input: Repeat paces a held input with a delay and a rate |
| #110 | `gap/host-types` | main | docs: host type macros |
| #111 | `gap/none-measure` | main | text: NONE backend estimates text width instead of zero |
| #112 | `gap/e2e-results` | main | e2e: results(), --results-json, single-script verdict line |
| #113 | `gap/rng-streams` | main | random: RngStream, an owned seeded generator that saves and resumes |
| #114 | `gap/enum-actions` | main | input: is_held / is_pressed and an enum-keyed mapping |
| #115 | `gap/cache-dir` | main | files: get_cache_path for regenerable data, apart from config |
| #116 | `gap/exact-mods` | main | input: the more specific chord wins when two match one key |
| #117 | `gap/e2e-screenshot-fail` | main | e2e: a screenshot callback returning false fails the script |
| #118 | `gap/measure-inputs` | main | autolayout: measure text with the drawn weight and letter spacing |
| #119 | `gap/colors` | main | colors: raylib's palette names on every backend |
| #120 | `gap/local-time` | main | time: local_time_now returns the wall-clock local date and time |
| #121 | `gap/raylib-fallback` | **gap/glyph-placeholder** | text: raylib falls back across loaded faces per codepoint |
| #122 | `gap/frame-rate-stats` | main | stats: FrameRateStats averages fps over a window |
| #123 | `gap/value-types` | main | developer: Vector4Type, MatrixType and graphics::Camera2D |
| #124 | `gap/disabled` | main | ui: disabled state no longer depends on a HasLabel |
| #125 | `gap/e2e-shadowed-command` | **gap/e2e-command-shape** | e2e: app commands registered first win; shadowed ones warn |
| #126 | `gap/crash-reentry` | main | crash: report without log_error, and only once |
| #127 | `gap/font-memory` | **gap/font-points** | fonts: load_font_from_memory, and sokol loads each face once |
| #128 | `gap/initial-focus` | main | ui: grab_initial_focus, and an automatic grab keeps the modality |
| #129 | `gap/e2e-defer-unknown` | main | e2e: register_unknown_handler can name commands another state handles |
| #130 | `gap/wrappers` | main | backends: draw_fps, get_key_pressed, unload_font, NULL_KEY, texture wrap |
| #131 | `gap/e2e-is-running` | main | e2e: is_e2e_running() for hiding nondeterministic HUD readouts |
| #132 | `gap/cxxabi-has-include` | main | core: include cxxabi.h only when the header exists |
| #133 | `gap/default-font-leftovers` | main | fonts: debug overlay and load errors use the set default font |
| #134 | `gap/concept-declval` | main | graphics: concepts use declval, not C compound literals |
| #135 | `gap/frame-dt` | main | sokol: get_frame_time is the time since the last admitted frame |
| #136 | `gap/e2e-hold-key` | main | E2E: hold_key / release_key builtins |
| #137 | `gap/wheel-input` | main | ui: wheel hit-tests the drawn scroll view; Shift+wheel goes sideways |
| #138 | `gap/wrap-hard-breaks` | **gap/wraptext** | text: wrap breaks words wider than the line between graphemes |
| #139 | `gap/seed-from-id` | main | query: seed_from_id outside the SKIP_ENTITY_QUERY_MODIFICATIONS split |
| #140 | `gap/e2e-ui-gestures` | main | e2e: mouse_down_ui, mouse_move_ui and scroll_wheel_ui by element name |
| #141 | `gap/sokol-headless-run` | main | sokol: run() honours DisplayMode::Headless |
| #142 | `gap/middle-click` | main | ui: middle button in MousePointerState and is_middle_click |
| #143 | `gap/catalog-fmt-include` | main | examples: catalog common.mk finds Homebrew headers |
| #144 | `gap/sound-wrappers` | main | audio: stop, pause, resume, pitch, pan and device wrappers |
| #145 | `gap/inert` | main | ui: with_inert keeps a subtree from taking clicks or focus |
| #146 | `gap/audio-lifecycle` | main | graphics: Config/RunConfig audio flag opens the audio device |
| #147 | `gap/takes-focus` | main | ui: with_takes_focus(false) clicks without moving keyboard focus |
| #148 | `gap/consumer-builds` | main | docs: minimal consumer builds and which one a test suite wants |
| #149 | `gap/field-arrows` | main | text_input: the field owns Up and Down while focused |
| #150 | `gap/input-remap` | main | input: remapping for the non-layered input map |
| #151 | `gap/click-count` | main | ui: click_count in MousePointerState and is_double_click |
| #152 | `gap/harness-spellings` | main | e2e: --key=value spellings and --e2e-timeout |
| #153 | `gap/terminal-commands` | main | e2e: expect_terminal_* commands |
| #154 | `gap/profiling-warn` | main | e2e: warn when profiling is compiled out or a provider is replaced |
| #155 | `gap/point-in-element` | main | e2e: expect_point_in asserts a point lies inside a named element |
| #156 | `gap/toast-fade` | main | toast: add HasOpacity at schedule time so toasts fade |
| #157 | `gap/pagination-index` | main | ui: pagination Previous and page buttons land on the right page |
| #158 | `gap/text-area-readonly` | main | text_area: honour readonly and disabled from config |
| #159 | `gap/ease-scroll-instant` | main | ui: ease_scroll snaps under instant motion |
| #160 | `gap/dropdown-x` | main | ui: dropdown tray clamps at the right edge too |
| #161 | `gap/virtual-overscan` | main | ui: with_virtual_list_overscan makes the virtual-list window configurable |
| #162 | `gap/percent-margin` | main | autolayout: charge Dim::Percent sizes their own margin |
| #163 | `gap/particle-drag` | main | particles: drag no longer flips sign at low fps |
| #164 | `gap/staging-texture-leak` | main | capture: release the staging texture after readback |
| #165 | `gap/translation-4byte` | main | translation: load 4-byte UTF-8 glyphs in load_cjk_fonts |
| #166 | `gap/letter-spacing-zero` | main | ui: explicit zero letter spacing clears inherited spacing |
| #167 | `gap/load-font-size` | main | ui: load_font_with_codepoints takes an explicit load size |
| #168 | `gap/monospace-cell` | main | ui: add monospace cell-advance measurement |

## Waiting on a decision (19)

Each note in [decisions/](decisions/) has the problem, options, a recommendation and a size estimate. Pick an option and it becomes a PR.

| Gap | Recommendation (short) |
|---|---|
| [accessibility](decisions/decide-accessibility.md) | Recommendation  Option 1. It's the testable half, it fixes the "a space is the only label" problem for every consumer's tests and tooling, and it's th… |
| [distance-sq](decisions/decide-distance-sq.md) | Recommendation  Option 1 if Gabe is fine with three projects deleting their copies on bump (the error is immediate and the fix is obvious). Otherwise … |
| [e2e-live-resize](decisions/decide-e2e-live-resize.md) | Recommendation C, done as two PRs. B first (small, testable headless, useful to both projects), then the #595 hook as its own sokol/macOS PR, verified… |
| [e2e-name-lookup](decisions/decide-e2e-name-lookup.md) | Recommendation C. B is about 120 lines: one `resolve_ui(name_or_text, scope)` used by every handler, an `under=` key parsed from the args the way `ass… |
| [e2e-one-call-setup](decisions/decide-e2e-one-call-setup.md) | Recommendation B. pong-gaps is already the answer for this gap and for parts of #98 and gap/e2e-hold-key. Splitting it lets the e2e piece land without… |
| [focus-shortcuts](decisions/decide-focus-shortcuts.md) | Recommended.** `ComponentConfig::with_shortcut(action, fn)` stores handlers in a `HasShortcuts` component. A `DispatchShortcuts` system in the post-up… |
| [input-collector-base](decisions/decide-input-collector-base.md) | Recommended.** gap/enum-actions (`input::is_pressed(Action)` / `is_held(Action)`) already removes the collector from `ImpulseBall`: its `any_of` over … |
| [macos-desktop](decisions/decide-macos-desktop.md) | Recommendation  Option 1 now, and option 3 if a third desktop app shows up. Only the two desktop-style apps want this, and both already ship working a… |
| [memory-budgets](decisions/decide-memory-budgets.md) | Recommendation  Do option 1 now as its own small PR; it's concrete and two caches move onto it. Option 2 next, if Gabe agrees afterhours should own an… |
| [scrollbar-policy](decisions/decide-scrollbar-policy.md) | Recommendation  Option 1 now, then auto-hide (the visibility half of option 2) as a follow-up PR. The gutter is small and opt-in. Auto-hide needs Gabe… |
| [selectable-text](decisions/decide-selectable-text.md) | Recommendation  **A**, after gap/wraptext lands, limited to per-element selection. Fold the `text_area` readonly wiring from B into it as a separate s… |
| [slider-parts](decisions/decide-slider-parts.md) | Recommendation  Option 1. It's opt-in, so no baselines move. It matches how toggle_switch and checkbox are styled, and it lets cartographer delete its… |
| [sokol-antialiasing](decisions/decide-sokol-antialiasing.md) | Recommendation  Option 2, in two PRs: (a) the MSAA render target + resolve with a headless coverage test, then (b) the windowed flag. That keeps E2E c… |
| [sokol-audio](decisions/decide-sokol-audio.md) | Recommendation  Option 3 now (tiny, removes the silent failure), and option 1 when the first sokol game needs sound. miniaudio is the same engine rayl… |
| [sokol-spacing](decisions/decide-sokol-spacing.md) | Recommendation  **B, unclamped.** It is about 10 lines plus a sokol CPU-fontstash test like `font_atlas_measure_test`: spacing widens measure by `(n-1… |
| [sokol-window-resize](decisions/decide-sokol-window-resize.md) | Recommendation  Option 1, ported by whoever can run floatinghotel's native resize probe (Metal changes have to be tested from floatinghotel). The code… |
| [storage-codecs](decisions/decide-storage-codecs.md) | recommended).**    - `struct Codec<T> { int version; std::function<std::string(const T&)> encode; std::function<std::expected<T, DecodeError>(std::str… |
| [tab-order](decisions/decide-tab-order.md) | Recommended.** Each frame, build the ordered list of focusable ids instead of streaming. Tab and Shift+Tab move to the next or previous id within the … |
| [transform-velocity](decisions/decide-transform-velocity.md) | recommendation). Revisit when a second 2D game wants the same `{pos, prev}` + `{vel}` shape. 2. **Add a minimal 2D pair.** `afterhours::Position2D { v… |

## In progress

- Input queue done: `gap/input-remap` is [#150](https://github.com/gabeochoa/afterhours/pull/150) and the click-count gesture is `gap/click-count` [#151](https://github.com/gabeochoa/afterhours/pull/151). Both are based on origin/main and wait on review like the rest.
- E2E batch done: [#152](https://github.com/gabeochoa/afterhours/pull/152) harness-spellings, [#153](https://github.com/gabeochoa/afterhours/pull/153) terminal-commands, [#154](https://github.com/gabeochoa/afterhours/pull/154) profiling-warn, [#155](https://github.com/gabeochoa/afterhours/pull/155) point-in-element. Left in [triage/triage-e2e.md](triage/triage-e2e.md): modifier-reader (touches `input_system.h`, best after #150 merges) and test-reset-focus (touches `UIContext`, best after #151 merges).
- Review pass on #152 to #164 (2026-11-16): merge-ready #152, #153, #154, #155, #156, #157, #159, #160, #164; nits fixed and pushed on #161 (VIRTUAL_LIST_OVERSCAN_DEFAULT, pool slack scales with overscan) and #163 (drag exp hoisted out of the particle loop), both merge-ready. Held as todos: #158 (verify wm + WM demo screen for readonly/disabled; should disabled opacity 0.5 be configurable; constexpr-if helpers) and #162 (run wm suite/screenshots, confirm no visual change, before merging).
- Merged 2026-11-17 after Gabe's approval pass: #140, #141, #143, #144, #148, #150, #152, #153, #154, #155, #156, #157, #159, #160, #161, #163, #164. CHANGELOG/tests-Makefile union conflicts resolved per PR (keep both sides). Full suite on merged main (6eb6f9e, /tmp/ah-merged-check): `nice -n 10 make -j1 test` in tests/ exited 0, all checks passed.
- Merged 2026-11-17 (second approval pass): #79, #80, #81, #82, #83, #84, #85, #86, #135, #136, #137, #138, #139, #142, #165, #166, #167, #168. Nits first: #86 gamepad names now map through the named constants (pharmasea-style explicit pairs), #139 comment shortened. One bad union: tests/config_size_test.cpp conflict markers reached main; fixed in 08b2461 (kExpectedSize 1552, verified 1/1) and a marker scan found no other file affected. Full suite on 08b2461: `nice -n 10 make -j1 test` exited 0, all checks passed.
- Merged 2026-11-17 (third pass): #114, #115, #116, #117, #118, #119, #122, #123, #124, #126, and #125 into its stack base gap/e2e-command-shape. Todos noted at approval: #116 chord matching memory/CPU (approved anyway), #118 consider moving text measurement into its own file, #124/.#145/#147 disabled-family design pass.
- Merged 2026-11-17 (fourth pass): #87, #88 (+#104), #91, #92 (+#121), #93, #94 (+#127), #95 (+#125), #97. Library suite on fe908c3: exit 0. Two union fixes on main: duplicated kExpectedSize and sokol font-loader braces. Open with Gabe comments answered: #90 (wm gate running), #145/#146/#149/#151/#158 (replies posted), #147 fix pushed (tab order now respects takes_focus). #78, #89, #99 have no comments yet.
- Regressions from the merge batches, found by the wm test set (both open, awaiting approval): #169 disabled controls block clicks (from #124; script 220), #170 headless font atlas + textureless-draw guard (from #105; DrawTextEx segfault). With both vendored, wm E2E has zero segfaults and 220 passes; 31 validate_screen failures remain, all baseline drift (0.3-4.5%) from merged text changes — regenerate baselines on the wm bump after reviewing.




- Todo from #141 approval: file the remaining raylib-backend rendering options (vs sokol run/headless) as gap todos for later.


## Tier 4 triage

Input, e2e and ui-widgets have been triaged: [triage/triage-input.md](triage/triage-input.md), [triage/triage-e2e.md](triage/triage-e2e.md) and [triage/triage-ui-widgets.md](triage/triage-ui-widgets.md). Input: of 25 gaps, 8 are worth a PR, 7 need a decision, 8 are app-specific and 2 are already fixed; #137, #142, #145, #147, #149, #150 and #151 came out of it. E2E: of 12 gaps, 6 are worth a PR, 5 need a decision and 1 is already fixed; #152 to #155 came out of it. UI-widgets: of 39 gaps, 25 are worth a PR, 8 need a decision, 3 are app-specific and 3 are already fixed; [#156](https://github.com/gabeochoa/afterhours/pull/156) toast-fade, [#157](https://github.com/gabeochoa/afterhours/pull/157) pagination-index and [#158](https://github.com/gabeochoa/afterhours/pull/158) text-area-readonly are open, next are ease-scroll-instant and dropdown-x (caret-blink waits on #158, same file). [triage/triage-ui-layout.md](triage/triage-ui-layout.md): of 26 gaps, 13 are worth a PR and 13 need a decision; [#161](https://github.com/gabeochoa/afterhours/pull/161) virtual-overscan and [#162](https://github.com/gabeochoa/afterhours/pull/162) percent-margin are open. [triage/triage-rendering.md](triage/triage-rendering.md): of 25 gaps, 14 are worth a PR, 9 need a decision, 1 is app-specific and 1 is already fixed; [#163](https://github.com/gabeochoa/afterhours/pull/163) particle-drag and [#164](https://github.com/gabeochoa/afterhours/pull/164) staging-texture-leak are open. [triage/triage-text.md](triage/triage-text.md): of 18 gaps, 11 are worth a PR and 7 need a decision; #165 to #168 are open. Held ready: `gap/styled-ellipsis` (complete, tests pass) waits on a sequencing call: ship standalone off main with its run-level ellipsize_runs, or rebase onto #88 gap/textfit if that merges first. [triage/triage-ecs-core.md](triage/triage-ecs-core.md): of 13 gaps, 5 are worth a PR, 7 need a decision and 1 is app-specific. [triage/triage-networking.md](triage/triage-networking.md): all 11 belong to afterhours-online or the game, not afterhours core (3 pr there, 7 decide, 1 app-specific). platform (10) and the smaller areas haven't been triaged.

## Not started

- Tier 2 Metal gaps: live resize painting stale frames, per-frame autorelease pool, GPU object retirement timing, non-rectangular child clipping. Test these from floatinghotel.
- Audit follow-ups ([../RAYLIB_USAGE_AUDIT.md](../RAYLIB_USAGE_AUDIT.md)): sokol audio (miniaudio), sokol gamepads, custom shaders on both backends, runtime window/DPI queries, CPU image flip/export/readback.
- Recommended: split the unmerged `pong-gaps` branch into themed PRs. It already solves one-call E2E setup and overlaps #98 and #136.

## Notes for whoever resumes

- After the reboot, run `git -C ~/p/afterhours worktree prune`: the `/tmp/ah-gap-*` worktrees will be gone, but their branches are in ~/p/afterhours/.git.
- [pr-tooling/](pr-tooling/) has the agent rules (RULES.md) and the scripts used to review and open each PR: `review_open.sh` flags new comments and TODOs, and requires an example.
- Never use `git stash` across agents. The stash is shared by every worktree of a repo, and it swapped work between agents twice; both swaps were caught and repaired. The rescue patches are in pr-tooling/.
- Found while working, also fixed in PRs: a runner bug where `register_before_builtins` never overrode a builtin (#125, #129), a headless raylib font-load segfault (#105), `seed_from_id` missing in one build branch (#139), 17 catalog examples that didn't build (#106, #143), and `render_layer_offset` dropped by `apply_overrides` (#87).
