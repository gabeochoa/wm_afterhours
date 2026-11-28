# Afterhours gaps and change requests

## E2E testing
- Provide built-in commands for menu UI flows: `click_btn`, `expect_label`, `expect_app_state`, `set_volume`, `check_volume` so every game does not re-implement them.
- Allow registering a shared command pack across multiple `SystemManager`s so menu and pause systems can reuse the same handlers without duplicate setup. Shipped as `testing::register_shared_handlers(sm, pack)` (afterhours 4509ffa) and adopted: `shared_e2e_commands()` holds the input, UI, save, wait and cleanup handlers once, and every state's manager runs it. Still open: `register_unknown_handler` can't go in the pack for Cartographer, because gameplay commands sent at the menu are meant to wait for gameplay, and the unknown handler fails them on the first frame. A per-command "wait for another state" marker would let it. Shipped as `register_unknown_handler(sm, {names...})` (#129, afterhours 7e7fd4f); adoption is item 2 of `afterhours_update.md`.
- Ship built-in e2e commands for the `terminal::Console` plugin (`expect_terminal_open`, `expect_terminal_closed`, `expect_terminal_input`, `expect_terminal_output_contains`) so games using the console don't hand-roll them like `src/e2e/e2e_terminal_commands.cpp`.

## UI entity management
- `EndUIContextManager` (`vendor/afterhours/src/plugins/ui/systems.h:656`) returns early when `context.focus_id == context.ROOT || context.focus_id == context.FAKE`, skipping `context.set_active(context.ROOT)` on mouse release (as well as `focused_ids.clear()` and `text_cache->end_frame()`). On screens that disable initial focus (`grab_initial_focus = false`, such as the main menu desk), clicking a button that transitions to another state leaves `active_id` stuck on the old button permanently, blocking `ResolveHitTarget` from ever granting `active_id` to any button on the new screen. Move the `focus_id` guard onto only the `focused_ids.contains(focus_id)` check so `active_id` always clears on release.
- Improve `mk()` ergonomics to avoid entity ID conflicts in immediate mode trees, such as a scoped auto-index helper or a `mk_next()` that is easy to use in nested lambdas.
- Emit structured diagnostics for ID conflicts that include a suggested fix and nearby hierarchy context.

## UI validation and accessibility
- Add a toggle to disable noisy per-frame validation logs or throttle repeated warnings per entity.
- Expose a built-in contrast checker that returns a ratio for a given text color and background to support custom widgets.
- Provide configurable minimum target sizes and optional auto-padding enforcement for interactive controls.

## UI components
- Add a standard value pill component that pairs with sliders and steppers.
- Add a tab control with an active indicator style and optional paper-tab visual variant.
- Add a compact settings list component with built-in dividers and label/value alignment.
- Add a divider component so we stop hand-rolling `div` with `percent(1.0f) x pixels(1.0f)` plus margin every time we need a horizontal rule between rows.
- Add a simple `with_720p_size(width, height)` convenience for scaled layouts.
- Expose theme palette colors directly (or a typed accessor) for shared UI styling.

## Tooltip integration

`HasTooltip`, `UpdateTooltips` and `RenderTooltip` already ship in `vendor/afterhours/src/plugins/ui/tooltip.h`. Use `ComponentConfig::with_tooltip()` for hover text rather than building another popup. The settings tabs now use this component and renderer.

- Add keyboard-focus support to `UpdateTooltips`. It only walks `UIContext::hot_id`; a focused icon has no tooltip unless the pointer also hovers it. Show its tooltip immediately on deliberate keyboard focus and dismiss it when focus leaves. Cartographer currently copies the focused component's tooltip into `TooltipState` after the library hover update.
- Expose tooltip corner radius and other visual overrides separately from the control's theme. `RenderTooltip` reads the whole context's theme; component rounding styles apply to the owning button. The settings screen sets the render-time theme radius after its controls have been built to give tooltips rounded corners without changing the icon tabs.
- Add a placement setter alongside `with_tooltip()`. `HasTooltip::placement` already supports Right, but `component_init.h` always initializes it to Below. The settings tabs currently set the component's placement directly.
- Refresh and remove tooltip configuration during immediate rebuilds. `component_init.h` only calls `addComponentIfMissing`, so changed text, size or delay is ignored after creation, and clearing the tooltip string leaves the old component attached. Changing Text size should resize an existing tooltip, and hiding a tab during the book animation should remove its tooltip without game-side component updates.

## Layout helpers
- Add static convenience constructors on `Margin` and `Padding` for single-side values, e.g. `Margin::left(pixels(10.0f))`, `Margin::top(h720(8.0f))`, `Padding::horizontal(pixels(16.0f))`. Already in afterhours (`Padding::Top`, `Left` and the rest, `layout_types.h:413-420`, b9414d5); not adopted.

## Custom-rendered scenes with imm hit areas
The desk menu draws rotated objects with raylib `Camera2D` and lays invisible imm buttons over them, because imm can't do any of the following:
- Rotate or transform components. Paint primitives and ui_motion rotate drawn output, and modifiers carry a rotation field. ComponentConfig has no `with_rotation(deg)`, and layout, hit testing, hover and the focus ring are all axis-aligned. A card tilted -3° needs a raylib render pass plus a transparent button that only roughly matches it.
- Opt a single component out of the focus ring: shipped as `with_focus_ring_thickness` (afterhours 1a54a7f) and adopted, so the desk hotspots opt out and the theme ring stays on everywhere else. Still missing: letting the app draw its own ring, so the render layer still copies `ctx.visual_focus_id` into a static.
- Style focus-ring states: outline offset, rounded corners, a spread halo like CSS `box-shadow: 0 0 0 13px`, hover-only offset changes, a brightness lift, or a 0.16s transition between states. The desk re-implements all of these in `draw_desk_outline()`.
- Report hover and focus state to render systems. `UIContext::is_hot` answers by entity id, and the desk already reads `visual_focus_id` directly. The render layer still re-runs its own mouse-in-rectangle check. An `is_hot` / `is_focus_visible` query keyed by debug name, readable from a render system, would remove that duplication.
- Choose the initial focus. The first button grabs focus on load without showing a ring, so Enter on a freshly loaded menu activates it. There is no way to say "no initial focus" or "focus this element first."

## Settings journal shell (from matching the selected mock)
- An absolute-positioned child of a button doesn't land at `with_translate(x, y)` relative to the button's box. A "←" div inside the Back button showed up below the label, so the arrow and text are now a sibling div laid over a button with a transparent label.
- `with_padding` on a label-only button does nothing (it logs a warning). `with_text_inset` works, but it isn't what `Padding` users reach for, and the warning doesn't mention buttons.
- Labels can't word-wrap. Only text areas have `with_word_wrap`. Settings hints that fit at 400px were cut off at 330px, so they switched to a smaller regular font instead of wrapping.
- There are no per-side borders and no clip-path or chamfered corners. The mock tabs have a 1px top edge and 3px chamfered left corners. We draw plain rects.
- `with_icon` sizes the icon at 60% of the button height with no override, and the icon is always a flex child aligned to the start. Centring a 24px icon in a 48px tab takes a hand-tuned left padding.
- Fonts loaded through `LoadFontEx` at 96px with extra codepoints fail to pack three glyphs ("Failed to package character"), and nothing reports it except a raylib warning. At 80px they pack fine. `FontManager::load_font` could check the packing result.

## E2E testing (from desk menu validation)
- The game's `click_btn` sets `HasClickListener.down` directly, so a test passes even when the button is invisible, covered or in the wrong place. A built-in click-by-label should hit-test at the element's centre and fail if a different element is on top. Shipped upstream as `click_label` (c22471d). Not adopted: the desk menu's buttons are invisible imm hit areas over raylib drawing, so a hit test there checks the overlay, not what the player sees.
- Add `expect_focus LABEL` / `expect_no_focus` commands. Checking Tab order and focus-visible state currently takes screenshots. Shipped (c22471d), not adopted yet.
- Add `expect_hovered LABEL` for hover-state checks. Shipped (c22471d), not adopted yet.
- The built-in `mouse_down` sets `press_frames = 0`, so the press edge exists only for systems that run after the command handler in the same frame. The gallery update system is registered before the e2e handlers and never saw injected clicks. The injector already has a one-frame survival window (`press_frames = 1`, re-raised only if unread), and `simulate_mouse_press` uses it, but `mouse_down` doesn't. The game's handler now sets `press_frames = 1`. Fixed upstream: the built-in `mouse_down` and `middle_down` set it too (230f56c).
- Nothing reports an injected press that no system read. The test step passes silently, so a click that did nothing looks the same as one that worked.
- A `screenshot` stalls its frame for about 0.6s at 2560x1440, and that stall arrives as the next frame's dt. Timed motion jumps ahead, so mid-animation screenshots don't show the time the script says. The harness has `time_scale` but no fixed-dt mode for capture. The journal motion now clamps its own step to 1/30s.
- `click_btn LABEL` matches entities from screens that aren't showing. In the in-game pause menu, `click_btn Settings` hit the desk's hidden "Settings" hotspot instead of the pause button. The game's handler now also requires `UIComponent::was_rendered_to_screen`. A built-in click should skip unrendered entities by default. The built-in `click_ui`, `click_label` and friends already do.

## Scene transitions (from the settings book motion)
- ComponentConfig has translate and scale, and ui_motion animates those properties plus rotation and opacity for painted output. It has no static rotation setter and no 3D rotate. The book cover flight and swing are drawn by hand with raylib and rlgl quads under the imm layer, and imm content is faded with `with_opacity` because it can't move with the cover.
- imm always draws after the app's render systems, so a hand-drawn layer can't sit on top of imm content. Right-page text would show through the swinging cover, so its opacity is tied to the swing angle instead of being occluded.
- There's no keyframe or timeline helper (offset-based keyframes with per-segment easing, reversible playback). Each opacity is a hand-written `keyframe(t, from, to)` call, and closing is a manual reverse of one float.
- Elements at opacity 0 still take clicks and focus. There is no "inert while fading" flag.
- There's no clip-path or non-rectangular clip, only scissor. The loading sweep's diagonal edge renders the wood and packing desk into a render texture and draws it as a textured quad.
- Nothing helps with offscreen layers. Drawing translucent content into a cleared render texture with the default blend squares its alpha, which darkens edges and lets the layer below show through. The sweep sets separate alpha blend factors while drawing and composites with premultiplied alpha. A layer helper (like CSS stacking contexts) would handle this.
- Render textures can't nest. `BeginTextureMode` inside another texture pass returns to the screen at `EndTextureMode`, so nested layers must be rendered first.
- BlurPass ships in effects, and components take `with_blur`. The game has no blur.fs in `resources/shaders`, and the vendor tree ships no shader files, so the sweep band still draws without the mock's 25px blur.
- `UIContext::set_focus` sets `focus_modality` to whatever source moved focus, including an automatic `Grab`. When a focused element goes away (e.g. returning from the settings book), `try_to_grab` refocuses the first button, and anything that shows a ring when `focus_modality != Pointer` lights it up without user input. The desk now ignores focus whose modality is `Grab`. An automatic grab should keep the previous modality.
- Translucent draws into the game render texture multiply down its alpha, so an overlay shade or text over the 3D view fades out or disappears when the texture is presented. The loading-screen cover sets separate alpha blend factors to keep the texture opaque. A layer/overlay helper should handle this by default.
- There is no way to record frames. Finding one-frame flashes needed a hand-rolled `--record` that pipes `LoadImageFromScreen` into ffmpeg with a fixed 1/60 dt. It must flush the rlgl batch first, or the last batch is missing from the frame. A built-in capture with fixed-dt playback would make transition bugs visible in tests. Half shipped: `RunConfig` now has `fixed_dt` and `sim_steps`, but only `graphics::run()` reads them, and Cartographer runs its own loop (`src/game_loop.cpp`). There is still no frame capture to a file or pipe.
- There's no incremental loader. The game now builds a `LoadPlan` (asset steps per level, then world setup steps) and runs slices of it within a per-frame time budget, so the loading screen is a normal system and the bar shows steps done. afterhours could ship this with async file reads, because one gltf still blocks its frame for about 25ms. Still open (Nov 2026). The afterhours todo lists budgeted `ModelLibrary` loads, which covers models only, not world setup steps or the progress labels the loading screen shows.
- `Library::load` parses the file before checking the name, then drops the result on `DUPLICATE_NAME` without unloading it. Loading a model twice leaks it. `ModelLibrary::load` now checks `contains` first. Fixed upstream (e8f5a82): `Library::load` checks the name first and warns.
- The e2e runner silently skips commands that no handler in the active `SystemManager` consumes. During a multi-second `Transitioning` state, `expect_app_state` and friends were skipped and passed, so test mode now loads the whole plan at once.

## Direct raylib usage
At 87f2141, `src` makes 1,062 direct raylib calls across 28 files and 166 distinct functions. The count is `raylib::Name(` occurrences in `src`. The menu, gallery, pause and letter screens account for 224 of those calls. The rest is game rendering, world building, and the map. At 2bdf9e5 the same count is 1,608 calls across 104 files and 189 functions. Most of the growth is 3D world code (terrain, tree field, water, shadows), which the sections below file as missing building blocks. The table is from 87f2141.

| File | Calls |
| --- | ---: |
| `src/map/map_systems.cpp` | 184 |
| `src/systems.cpp` | 180 |
| `src/menu_systems.cpp` | 113 |
| `src/entity_makers.cpp` | 89 |
| `src/entity_makers_maps.cpp` | 89 |
| `src/components.h` | 63 |
| `src/editor_systems.cpp` | 56 |
| `src/gallery_systems.cpp` | 53 |
| `src/journal_systems.cpp` | 44 |

Checked against the vendored tree at the same commit, the drawing end can move onto shipped helpers today. The shader, matrix, pixel and imm transform rows still need library work.

| Item | Already ships | Still missing |
| --- | --- | --- |
| Shapes, text, quads | Drawing helpers cover rounded rects, gradients, lines, circles, text, texture quads, and rotation push/pop | A gradient fill and a line primitive at imm component level |
| Blur | BlurPass in effects, plus with_blur on components | The game supplies blur.fs, because the vendor tree ships no shader files |
| Drop shadow | with_shadow with hard and soft styles | Nothing |
| Hover state | UIContext::is_hot by entity id, and the desk already reads visual focus | A query keyed by debug name for render systems |
| Fonts | The backend loader takes codepoints and size, uses a skyline atlas, prepares trilinear filtering, and FontManager can share one loaded face with hand-drawn passes | FontManager::load_font_with_codepoints hides the backend size argument and loads at 32 |
| Offscreen targets | Render texture load, begin, end, draw and capture helpers, plus a scoped blend setter with a premultiply mode | Nested render textures, custom separate blend factors, and a layer helper |
| Shaders | effects::Effect caches locations and sets float, vec2 and color uniforms inside a scope | Vec3, matrix and vertex shader support on Effect, which the game lighting and flare shaders need |
| Matrix composition | Nothing, and the graphics types alias the raylib types on this backend | A compose helper, or math types of its own |
| Pixel access | Nothing in files or the texture manager reads pixels back | An image load and sample helper |
| imm transforms | Paint primitives and ui_motion rotate drawn output, and modifiers carry a rotation field | with_rotation on ComponentConfig, plus rotated layout, hit testing and focus rings |
| Texture filtering | Filter and mipmap setters for textures and render textures | A per-draw filter on imm sprites |
| Own frames | Graphics begin and end drawing, plus begin_frame for the game render texture | One presentation path the menu, pause and loading screens can share |

Matrix composition, pixel sampling, and a typed shader wrapper can also be built inside `src` today. That keeps raylib in one file per job while the library requests wait.

- Shader plumbing (57 `GetShaderLocation`, 51 `SetShaderValue`). Locations are fetched at setup in `entity_makers.cpp` and `components.h`, then values are pushed per frame in `systems.cpp`. effects::Effect already resolves locations once and accepts float, vec2 and color uniforms for fragment-only effects. Vec3, matrix and vertex shader support on Effect would cover the game lighting and flare shaders and remove both halves from game code.
- Matrix composition (141 `Matrix*` calls, 58 of them `MatrixMultiply`). Entity makers and held-tool rendering build scale, rotate, translate chains by hand. A compose helper that takes position, rotation and scale and returns one matrix covers most of these chains.
- Pixel access (8 `LoadImage`, 23 `GetImageColor`, 22 `UnloadImage`). Heightmap sampling and map saves load an image, read pixels, and unload it at each site. An image helper that returns pixels or sampled heights would remove the manual load and unload pairing.
- Offscreen targets and raw quads (19 `BeginTextureMode`, 110 direct `rl*` calls). Render texture load, begin, end, draw and capture helpers already ship, with a scoped blend setter. The map renderer draws its layers with `rlVertex3f` and `rlColor4ub` (12 each in `map_systems.cpp`), and the pause backdrop downsamples through a render texture. A layer helper covers the stacking, and a quad path covers the raw vertices. The book cover swing and page flips draw quads the same way, and the blending problems sit in Scene transitions above.
- Text in hand-drawn scenes (29 `DrawTextEx` in the menu screens, 31 total). The drawing helpers include `draw_text_ex` and `draw_text`, so these sites can stop naming raylib today. Desk labels, gallery cards, the loading status and the letter are drawn under rotated `Camera2D` passes or into render textures. imm text still can't be placed inside a transformed scene or an offscreen layer.
- Font loading (5 `LoadFontEx` in the menu screens, 6 total). The backend loader takes codepoints and size, packs with a skyline atlas by default, and prepares trilinear filtering. `FontManager::load_font_with_codepoints` hides the size argument and loads at 32, so the desk, pages, gallery and letter still load NationalPark again at each size they need. A face loaded once can go through `FontManager::load_font` and serve imm text and hand-drawn passes alike.
- Texture filtering (13 `SetTextureFilter` in the menu screens, 18 total). Sprites have no per-draw filter or mipmap option. The game render texture is POINT for the pixel look, so the pause backdrop flips it to BILINEAR around each draw and back.
- Shapes under imm (in the menu screens, 13 `DrawRectangleRounded`, 12 `DrawRectangleRec`, 7 `DrawRectangleLinesEx`, 5 `DrawRectangleGradientH`, 6 `DrawLineEx`). The drawing helpers cover every one of these calls, and components take `with_shadow`. Cards, shadows, gradients and rules sit in hand-drawn scenes. imm still has no gradient fill and no line primitive.
- Frames drawn outside the pipeline (4 `BeginDrawing` in the menu screens, 7 total). Graphics begin and end drawing exist, and begin_frame manages the game render texture. The pause journal, the loading scene and the gallery still open their own frames, because no shared presentation path covers the non-game screens.
- Hit tests (3 `CheckCollisionPointRec`). The render layer re-checks hover with its own rectangle test. `UIContext::is_hot` answers by entity id, and a debug-name query readable from a render system would retire these calls (see Custom-rendered scenes).

## Slider and icon styling (from the settings restyle)
- The slider handle takes the track's background colour, and there's no fill segment or knob size. The settings sliders draw the track, fill and knob as three extra divs over a transparent `imm::slider`.
- `with_icon` works only on `button`. A `div` or `sprite` label can't carry an icon, so the tab icons have to be buttons.

## Settings plugin
- Support JSON comment-preserving writes or an official JSON5 output mode for grouped settings.
- Allow specifying a default settings file path override from code without redefining global variables. Shipped as `settings::set_path_override(dir)` (a0dd57d). Not adopted: Cartographer already passes an absolute file path to `settings::init`, which covers its test and `--settings-file` cases.

## Build and integration
- Document required include order when combining `raylib` with afterhours UI types.
- Provide a compile-time guard that fails fast with a clearer error if the type aliases for vector/rectangle/texture are not set.

## Frustrations from this work
- UI entity ID conflicts are easy to trigger with nested `mk(parent)` calls and the error shows after the fact.
- Type alias ordering around `raylib` and afterhours headers is fragile and fails with large compiler errors.
- E2E command coverage is inconsistent across worktrees, leading to copy-paste command handlers.
- Validation logs spam per frame even when a single element is known to be failing.
- Theme usage is easy to bypass accidentally and there is no lint to catch custom colors in shared UI.

## Trigger-axis presses are reported while held

`input_system.h` routes axes through `visit_axis` for both down and pressed checks. A held positive trigger therefore remains in `inputs_pressed`, unlike a button activation edge. Cartographer's pin repositioning now waits for trigger release after entering the mode, then confirms on the next down threshold; it does not use an axis pressed event.

Afterhours should expose distinct held values and activation edges for axes, with the configured deadzone applied before edge tracking. Its E2E layer also needs virtual-controller availability and persistent axis injection. Cartographer's test bridge covers these locally; physical controller hardware remains untested.

## SystemBase should_run overloads silently ignore const-only gates

`SystemBase` declares two `should_run` overloads, both defaulting to `true`: a non-const `should_run(const float)` and a const `should_run(const float) const`. `SystemManager::tick`, `fixed_tick` and `render` hold systems as `unique_ptr<SystemBase>` and call `system->should_run(dt)` on a non-const pointer, so overload resolution picks the non-const version. A system that overrides only the const overload gets its gate silently ignored — no warning, it just runs every frame.

This bit Cartographer twice, both times in render systems that relied solely on `should_run` for gating:
- `RenderSurveyScopeSystem` (scope PiP) drew every frame with tool=Compass, state=Holding, station=-1, even though its gate required the survey tool with a grabbed sight.
- `RenderLodShowcaseLegendSystem` drew the "LOD SHOWCASE" legend in the normal game even though its gate required `spawn_location == "lod_test"`.

An audit at the time found ~25 systems in `src` overriding only the const overload. Most gate internally in `once_impl`/`for_each`, so they don't visibly leak — but all of them are one refactor away from the same bug, and the silent failure mode makes it hard to spot.

Cartographer's local workaround is to override both overloads (same body) and re-check the condition at the top of `once_impl`.

Fixed upstream (26681cf): the base non-const `should_run` forwards to the const one. `CartSystem` keeps its two `final` overrides anyway, so no system can override `should_run` and skip the world-pass gate.

Afterhours should make overriding either overload work. The minimal non-breaking fix is for the base non-const `should_run` to forward to the const one (`return static_cast<const SystemBase&>(*this).should_run(dt);`): a const-only override then runs via virtual dispatch, and a non-const-only override keeps working as today. Alternatively, collapse to a single const `should_run` and have the manager call through a const reference. At minimum, document which overload the manager calls.

## Configurable default font

afterhours falls back to raylib's built-in bitmap font whenever no font is set: `get_default_font()` and `get_unset_font()` in `backends/raylib/drawing_helpers.h` (~558) return `GetFontDefault()`, `font_helper.h` returns it from four lookups (~243, ~302, ~310, ~321), `drawing_helpers.h` ~198 and the input debug overlay in `plugins/input_system.h` ~194 call `raylib::DrawText`, which always uses it. That font is a tiny pixel face that looks out of place next to a game's real fonts and doesn't scale.

Cartographer wants Paper Mono (OFL) as the fallback for debug tooling and any text drawn without an explicit font, so the profiler, input debug, tooltips and any unset-font UI share one readable face.

Suggested shape: a `set_default_font(Font)` (or a font path plus size on the existing font manager) that `get_default_font()` / `get_unset_font()` return when set, with the library's own debug overlays drawing through `DrawTextEx` with that font instead of `DrawText`. Keep `GetFontDefault()` as the fallback when nothing is set, so existing games don't change.

Shipped as `set_default_font(font)` / `clear_default_font()` (af6254d). Adopted: Paper Mono (`resources/fonts/PaperMono-Regular.ttf`, OFL) is set at startup, and the profiler, terminal, editor, test-name, LOD legend and gym label overlays draw with it through `src/debug_text.h`. Player-facing text keeps its own fonts. The input debug overlay and the `font_helper.h` error paths were fixed upstream (#133, 7e7fd4f). Still open: `measure_text` (filed below, "measure_text ignores the default font").

Alternatives: ship a bundled monospace font inside afterhours (adds an asset and a license to the library), or only document that games should pass a font everywhere (doesn't cover the library's own overlays, which is most of the problem). Today Cartographer draws its own debug text with raylib's default font in 11 files and has no way to change what the library's overlays use.


## E2E commands can't wait for a condition

The runner dispatches one command per tick and moves on, so every check has to come after a fixed `wait` long enough for the slowest machine. A state check like "the rope is Measuring after pressing F" either waits too long everywhere or flakes on slow frames, and `wait` was the biggest cost in Cartographer's suites.

Shipped natively (230f56c) and adopted: the probe workaround below is deleted, `wait_until` works in every state, and the inner command keeps its own argument parsing. It now counts against the per-script timeout.

What Cartographer wants is `wait_until SECONDS COMMAND [ARGS]`: run COMMAND every frame until it passes, fail only when SECONDS run out (with COMMAND's last error), and hold the next command until then.

The shape that fits the runner best, built and tried on a local afterhours branch (`cartographer-wait-until`, not merged):
- `parse_script` peels `wait_until SECONDS` off the line and parses the rest with the normal per-command rules, storing the timeout on the `ParsedCommand`.
- `PendingE2ECommand` gets a retry budget and a `rearm()` that undoes a failure (state back to Ready, success back to true, error and frame count cleared).
- `E2ECommandCleanupSystem` spends the budget with dt and rearms a failed command instead of logging it, until the budget runs out.
- The runner holds the next command while a `wait_until` is unconsumed.

Tradeoffs: rearming relies on handlers being safe to rerun, which holds for `expect_*` checks but not for commands with side effects (`press_key`, `teleport_player`), so it should stay documented as a check-only form. The held runner still counts against the per-script timeout. A check that already passes before an action resolves (`press_key F` then `wait_until 1 expect_rope_state Dropped` when it was already Dropped) passes at once, the same as a fixed wait would have hidden.

How Cartographer handles it today, without touching afterhours (`src/e2e/e2e_commands.h`, `src/e2e/e2e_runner.cpp`): the runner parses `wait_until` as an unknown command with every word as an arg. `HandleWaitUntilCommand` keeps the outer command alive and spawns the inner command as a separate probe entity each attempt. `WaitUntilProbeSystem`, registered after every handler and before the cleanup system, consumes the outer command when a probe passes, and drops a failed probe by removing its `PendingE2ECommand` before cleanup can log it, because `success` is private and can't be reset. `blocking_command_pending` holds the runner while a `wait_until` is pending, like `walk_to`. Costs of the workaround: the inner command gets the generic split-on-spaces parse (so `expect_text` and other rest-of-line commands don't work inside it), each retry takes a frame or two to spawn, and it only works on the gameplay `SystemManager`, not the menu one.

## Adopting the November 2026 update (02cb6af to 980f246)

Adopted: `Lod` for the collidable high/low switch (same 25 m / 35 m behaviour as threshold 30, margin 1/6), `draw_text_centered` on the title card, and per-component focus-ring opt-out on the desk. The items below are why the rest wasn't adopted, written so each can be built in.

### E2E serial dispatch breaks handlers that live in one SystemManager

4c86b3a runs script commands strictly in order. Cartographer registers its game commands per state: gameplay systems get everything, the menu, pause and gallery managers get the input and UI packs. `expect_autosave` lived only in the gameplay pack. Before, an `expect_autosave` at the main menu stayed pending, the script moved on to `click_btn New Game`, and the check ran once gameplay started, so it passed by accident. Now it times out after 30 frames at the menu, which failed `autosave_on_quit` and `map_autosave_continue`. Cartographer now registers `expect_autosave` in the menu pack too.

- Call out the dispatch change as breaking in CHANGELOG, with "commands that only one SystemManager handles now time out in the others" as the symptom.
- This raises the priority of the shared command pack gap at the top of this file: one registration that every manager runs would have avoided the split.
- Optional: when a command times out with no handler ever touching it, say so ("no handler recognised expect_autosave in this state") instead of the generic timeout, so the cause is obvious. Shipped (230f56c): the timeout asks whether the handler is registered on this SystemManager. The CHANGELOG still lists serial dispatch under e2e, not Breaking.

### Frustum: build from the active matrices

`make_frustum(camera, aspect, near, far)` needs the camera, aspect ratio and clip planes of the pass being culled. Cartographer culls in three passes with different projections: the main view, the scope picture-in-picture (its own camera, field of view and render-texture aspect) and the shadow pass (`rlSetClipPlanes` with its own near and far). `frame_cache::update_frustum` extracts the planes from `rlGetMatrixModelview() * rlGetMatrixProjection()` inside whichever pass is active, so it is right for all three without being told anything.

- Suggested shape: `Frustum make_frustum(MatrixType view_projection)` (Gribb-Hartmann plane extraction, normalised), alongside the camera form. Cartographer would then delete `FrustumPlanes` and use `Frustum::intersects_box`. Shipped (cc28e63) and adopted: `FrustumPlanes` is gone.
- Tradeoff: the matrix form needs a matrix type and only works where the backend exposes the current matrices; keep the camera form as the portable default.

### InstanceBatcher: groups need a key and a hook beyond the model

Cartographer already instances collidables: `RenderCollidablesSystem` groups transforms into `DrawBatch`es keyed by model name plus uniform state (in fog, wind sway), sets those uniforms on its own lit and fogged instancing shader before each group, and hands the same transforms to `shadow_pass::draw_instances`. `InstanceBatcher` keys groups by model address only and draws every group with one shader and no per-group setup, so replacing `DrawBatch` would take four batchers (fog by sway) and still could not set uniforms between groups or feed the shadow pass.

- Suggested shape: `add(model, transform, key)` with a small integer or hashable key, and `flush(shader, on_group)` where `on_group(model, key, transforms)` runs before each draw (to set uniforms, or to draw the shadow pass from the same span).
- The batcher's own TODO about linear group lookup also applies: the park has a few hundred distinct models.
- Shipped (d3be726): `add(model, transform, key)`, `flush(shader, on_group)`, and a hash map for group lookup. Not adopted: `flush` always draws through `draw_model_instanced`, and Cartographer draws its own way. A group with one copy draws un-instanced with the lit shader, and every mesh goes through `shadow_pass::draw_instances` with a per-group material. Adopting it needs a `drain(on_group)` that hands groups over without drawing. Groups also key by model address, and getting the address means a `ModelLibrary::get` per collidable (two `std::map` lookups) where `DrawBatch` hashes the name once.
- This alone doesn't fix the museum fps item (P0 in todo.md): the museum shows each of ~340 models once, so there are no copies to merge; its cost is one draw call per mesh.

### Lod takes distance, the render loop has distance squared

`Lod::update(distance, ...)` wants a linear distance. The collidable loop computes squared XZ distance for culling, so adopting it added a `sqrtf` per collidable per frame. Cheap, but avoidable.

- Suggested shape: `update_sq(distance_sq, thresholds, margin)` that squares the thresholds once per call instead. Shipped (6295fc2) and adopted.

### Text editing actions need a widget

The new platform keymap and `EditCommand`s (word and line delete, select all, copy, cut, paste, undo) are wired through the imm text input widget and the `Text*` names in the game's `InputAction` enum. Cartographer's only text field, the map layer rename, is drawn by hand on the map sheet and handles keys itself (`MapControlsSystem::edit_layer_name`), so adding the enum names would do nothing, and afterhours logs a warning at every start-up saying the features are compiled out.

- Suggested shape: a `text_input::edit_command_from_keys()` that reads this frame's keys with the platform keymap and returns the `EditCommand` (or none), so a hand-rendered field can call `apply_edit_command` on its own `HasTextInputState`.
- And a way to say "this app has no imm text input" that silences the start-up warning.
- Both shipped (fb25eed) and adopted: the layer rename calls `text_input::edit_command_from_keys()`, so it gets word and line delete, clipboard and undo, and `src/rl.h` defines `AFTERHOURS_NO_IMM_TEXT_INPUT`.

### files.cpp's Windows guard predates config.h

`plugins/files.cpp` keeps raylib out of its translation unit on Windows (`windows.h` collides with `raylib.h` on `CloseWindow`/`ShowCursor`) by undefining `AFTER_HOURS_USE_RAYLIB` before including `files.h`. The new `config.h` defaults to the raylib backend whenever no backend macro is defined, so the undef now re-triggers the default: the guard does nothing and the Windows build fails in `winuser.h`.

Cartographer's workaround pins that TU to the none backend from the build (`-DAFTER_HOURS_USE_NONE` for `files.cpp` in the makefile's Windows block and in `build.zig`).

- Suggested shape: `files.cpp` should define `AFTER_HOURS_USE_NONE` instead of undefining raylib before including `files.h`, then restore the raylib define after the include as it does today. The workaround flags can go away once it does.

### house_of_cards: adopted for far models (Nov 2026)

Cartographer draws every model past 60 m as a card (src/impostor_cards.cpp, resources/shaders/cards.vs/.fs, baked with `make bake-cards` from tools/bake_cards.cpp, which uses `bake()` and `save()`). It looks right day and night with the game's lighting and fog. What got in the way:

- One draw per model is too slow for a scene of unique models. On the museum (102 far models, one copy each) `draw_instanced` per model cost 9-12 ms against about 1 ms for the low meshes. It isn't the parallax steps, the texture binds or the uniform lookups: each instanced draw call costs about 0.1 ms on macOS OpenGL. Cartographer packs every card into one atlas at load and draws all far models in one instanced call: 0.3 ms, faster than the low meshes. To do that it forks the card shader: the spare bottom row of each instance matrix carries the atlas cell and bounding radius, and the bake centre is folded into the matrix. Suggested shape: an atlas mode in the plugin (`pack(std::vector<Baked>)` plus `draw_atlas(transforms_with_cells)`), so games don't fork the shader.
- `models.h` (pulled in by house_of_cards.h) declares `afterhours::ModelLibrary`, which collides with Cartographer's own `ModelLibrary` in every file that has `using namespace afterhours`. Suggested shape: a nested namespace for the plugin's model helpers, or include only what house_of_cards uses.
- `draw_instanced` takes a whole `Camera3D` but only reads `position`; in a second render pass (the survey scope) the caller has to pull the eye position out of the modelview matrix. Suggested shape: take the eye position.
- `draw_instanced` goes through raylib's `DrawMeshInstanced`, which creates and frees an instance buffer each call. Suggested shape: an optional caller-owned instance buffer.
- Cards don't sway or cast shadows; fine past 60 m, but a vertex offset hook would let them match closer.

## Positional audio

`sound_system` (`plugins/sound_system.h`) plays sounds by name, picks random variants and sets one library-wide volume. It has no listener, no per-sound position, no pan and no distance falloff.

Cartographer's birds do it by hand in `src/bird_audio.cpp` (`place_call`): every frame, each playing call gets a volume of `1 / (1 + (d / falloff)^2)` and a pan of `0.5 - 0.5 * dot(camera_right, dir_to_sound)`, pushed with raylib `SetSoundVolume` and `SetSoundPan`. It also keeps its own pool of sound copies so one call can play at several places at once, caps the songs playing at once, and cuts off past a hearing range. The valley's river, falls, torches and town will each need the same.

- Suggested shape: a listener set from the camera each frame, and `play_at(name, position)` that returns a handle the plugin keeps updated (volume and pan from distance and direction) until it ends, with per-sound falloff, max range and a voice cap. Sound copies (`LoadSoundAlias`) for overlap.
- Looping ambient sources (river, waterfall) tied to an entity position, faded by distance and stopped out of range.
- Stereo pan only today. No HRTF, Doppler or occlusion; those can wait.

## 3D world building blocks (Nov 2026 code audit)

The audit checked what Cartographer built against what the vendored afterhours ships. afterhours 3D support today is `models.h` (load, draw, instancing, one directional light with fog), `InstanceBatcher`, `Lod`, `Frustum` and `house_of_cards`. `collision.h` is 2D impulse physics on `vec2`, and `camera.h` wraps `Camera2D` only. Everything below had to be written in the game. Biggest first.

### Character controller

Cartographer built a first-person walker in `src/character_controller_systems.cpp` (1,088 lines), `src/player_components.h` and `src/climb.h`. It has a ground query, slope limit and slide, uphill slowdown, step-up to 1.6 m with a mantle, a lower step limit on slopes, headroom and overhead checks, an OBB push-out for rotated boxes, ladders and ropes, gravity, and mouse and stick look. The file has 39 commits, and about 42 commits in the log fix collision, steps, slopes or snags (a41d5dc OBB push-out, c3366ad climbing ramps from the side, 677bc20 walking under overhead boxes, 3205624 trunk sliding). Any first or third person 3D game needs the same thing.

- Suggested shape: a `character3d` plugin. A `CharacterController` component (radius, height, step height, max slope, snap distance) and `move_and_slide(controller, desired_velocity, dt, world)` against the collision world below, returning grounded state and ground normal. Optional hooks for mantle and climb volumes. A look camera component next to it (yaw, pitch clamp, sensitivity, invert) that reads the `MouseAxis` input mapping.

### 3D collision world: raycasts and nearby queries

Every raycast and contact check is a linear scan. `raycast_collidables` and `raycast_collidables_cached` walk every `Collidable` within 30 m on XZ, test its AABB, then call `GetRayCollisionMesh` per mesh. `NearbyCollidablesSystem` rebuilds the 30 m list each frame by scanning every collidable. `Collidable::compute_bounds` (`src/collision_components.h`, 173 lines) transforms eight corners by hand, and `push_out_of_rotated_box` does the OBB case. Tree trunks are cylinders. `EntityHelper::add_index` is a key index, not a spatial one. The afterhours todo lists `raycast_model(ray, model, transform)`, which covers one model, not the scene.

- Suggested shape: a `collision3d` module. Shapes: AABB, OBB, cylinder, mesh and heightfield. A uniform grid or BVH broadphase updated when a transform changes. `raycast(ray, max, filter)` returning entity, point, normal and distance; `overlap_sphere(center, radius)`; and a penetration test for push-out. Terrain as a heightfield shape, so one raycast answers for ground and props.

### Heightmap terrain

`TerrainData::sample_height` in `src/world_components.h` samples bilinearly from 8-bit or packed 16-bit (R high, G low) heightmaps. There are four more samplers (`sample_height`, `sample_height_central_park` and `terrain_height_at` in `src/entity_makers_maps.cpp`, `sample_height_for` in `src/map/map_loader.cpp`) plus a `TerrainSampler` copy of the numbers, across 33 call sites. Each sample calls `GetImageColor`. raylib's `GenMeshHeightmap` only reads 8 bits, so the valley decodes 16-bit heights and builds 8x8 tiled meshes with a colormap itself (`src/entity_makers_gym.cpp`, about 85 lines). de14ef7 fixed spawns sampling the wrong level's terrain, which one shared sampler would have prevented.

- Suggested shape: a `Heightfield` with `load(path, size, height, encoding)`, `height_at(x, z)`, `normal_at(x, z)`, `build_meshes(tiles, colormap)` and a raycast. One per level, so every system samples the same one.

### Chunked instance fields with distance LOD

The game has three versions of the same idea. `src/tree_field.cpp` (660 lines) and `src/tree_field.h` (90) bucket trees into 32 m chunks, frustum-cull each chunk, and draw full models to 60 m, cards to 250 m, cards thinned by a per-tree rank past that (kept cards grow to cover the gaps), and boxes past 400 m, batched by fog and sway. Grass is `VegetationField` (`src/vegetation_components.h`, 169) with `spawn_vegetation` in `src/map/map_loader.cpp` (about 175 lines: density-map scatter, path avoidance, chunk grid, scale ranges), `RenderVegetationSystem` (132), `src/vegetation_visibility.cpp` (146) and the grass shaders (161). Ground props are a third, `PropField`. afterhours has the parts (`Lod`, `Frustum`, `InstanceBatcher`, cards) but nothing that owns a static set of instances in a chunk grid.

- Suggested shape: `InstanceField`. Build it from (model, transform, rank) entries, bucket them into a chunk grid, and ask for `visible_chunks(frustum, eye)`. Each chunk gets an LOD band from `Lod` thresholds, rank-based thinning with a coverage scale, and a draw hook per (model, band, key) that feeds `InstanceBatcher` or cards. A scatter helper next to it: `scatter(bounds, count, density_image, reject_fn, rng)`.

### Player-centred pools

`TreeColliderPoolSystem` keeps 96 cylinder `Collidable`s on the trunks nearest the player within 40 m and moves them as the player walks (`update_tree_colliders` in `src/tree_field.cpp`). The birds in `src/bird_systems.cpp` (564) do the same with live entities: a pool of 4 lands on free perches 10-40 m away, and a perched bird more than 90 m away and behind the player moves to a perch 30-60 m away and out of view. Both keep N live entities chosen from a much larger static set by distance to a focus point. Valley rocks, fish and ambient sounds will need it too.

- Suggested shape: `ProximityPool` with capacity, radius, a candidate source (a spatial query), `assign(entity, candidate)` and `release` callbacks, and an optional rule to only swap entities that are out of view.

### Ambient creature steering

`src/bird_flight.h` (374) is mostly bird-specific, but its core is generic: a flight path that clears the ground through a height callback, arc-length progress, turn-rate-limited steering, a flee radius scaled by the player's speed, and picking a landing spot away from a threat. Lower value than the rest; file it if another game needs critters.

- Suggested shape: steering helpers next to `pathfinding.h`: `make_path_over(from, to, ground_fn, clearance)`, `steer(orientation, tangent, max_turn, dt)`, and a flee-radius helper.

## Rendering (Nov 2026 code audit)

### Scene lighting broadcast to many shaders

`SceneLighting` and `LightingUniforms` (`src/lighting_components.h`, 248 lines) keep a list of every lit shader. Each frame they push the sun direction and colour, ambient, fog and up to 16 point lights into all of them. `frame_cache::update_lights` picks lights in range and culls the rest. The point light and fog GLSL is copied into `lighting.fs`, `instancing.fs`, `grass.fs` and `cards.fs`, and the PS1 vertex snap into 11 shaders, because GLSL has no include. afterhours `Lighting` is one directional light and linear fog applied to one shader. `load_lit_shader(snippet)` taking a lighting snippet is the right direction. See the Shaders row of Direct raylib usage for the uniform plumbing half.

- Suggested shape: a `SceneLights` singleton (sun, ambient, fog, point lights with radius and flicker) that picks the N nearest lights each frame and uploads them to every registered shader, or to one uniform block. Plus a shader loader that resolves `#include "afterhours/lighting.glsl"`, so game shaders share one copy.

### Directional shadow map

`src/shadow_pass.cpp` (183) with `shadow_depth.vs/.fs` (68) renders an orthographic depth map around the player, redraws meshes and instance batches into it through `draw_mesh` and `draw_instances`, and binds the result into the lit shaders. `RenderBlobShadowSystem` (130) adds cheap contact blobs sampled on the terrain. The afterhours todo already notes a buffer bug in `draw_instances` and that cards cast no shadows.

- Suggested shape: `ShadowMap{size, extent}` with `begin(center, light_dir)` and `end()`, mesh and instanced draws that work in both the shadow and main passes, and the uniforms a lit shader reads. A blob shadow decal helper on top of the heightfield.

### Day and night cycle with sky

`TimeOfDay` (`src/time_components.h`, 91) holds the hour, speed, a faster night, and sun and moon elevation. `UpdateLightingFromTimeSystem` (114) keyframes sky and light colours by hour. Then `sky.fs` (128), `UpdateSkySystem`, clouds (`src/clouds.cpp` plus shaders), the lens flare (114 plus an 84-line shader), moon phases, and the e2e commands `set_time`, `expect_time` and `set_timescale` (`src/e2e/e2e_time_commands.cpp`, 113). Any outdoor game needs this.

- Suggested shape: a `time_of_day` plugin: a clock with scale and pause, sun and moon direction from the hour, a colour gradient keyed by hour (sky top and bottom, sun, ambient, fog) that feeds `SceneLights`, and the e2e commands.

### Render views and a world post-process chain

`src/render_pipeline.cpp` (287) draws the world into an offscreen target, then composites it with `world_composite.fs` (131): box-bounded depth fog, distance haze and PS1 colour quantising. The PS1 low-res target is three more systems (`PS1BeginLowResSystem`, `PS1EndLowResSystem`, `UpdatePS1UniformsSystem`, about 126 lines). The world is drawn from three views: the main camera, the survey scope picture-in-picture (own camera, field of view, aspect, and LOD distances scaled by magnification), and the shadow pass. Systems find out which pass they are in through globals (`frame_cache`, the world-pass gate in `src/cart_system.h`). `make_frustum(view_projection)` shipped for the culling part.

- Suggested shape: a `RenderView` (camera, target, frustum, LOD scale, flags) handed to render systems, so one draw routine serves the main view, a picture-in-picture and a shadow pass. A post-process chain over the world target (fog, low-res, blur) with scene depth available to each step.

### Water surfaces and floating objects

`water.vs/.fs` (149) shade ponds and rivers. `src/water_surface.cpp` (89) bakes a shoreline depth texture under each pond from the heightmap. `UpdateWaterSystem` (41) feeds it. `scripts/gen_valley_heightmap.py` writes the river ribbons. `BobbingSystem` (50, ae41f2d) makes pier decks and logs swell and dip when the player steps on them.

- Suggested shape: a water component (mesh or ribbon, level, deep and shallow colours, shoreline depth from the heightfield) with a shader that takes scene lights and fog. A `Floats` component that bobs a transform on it.

## Loading and level data (Nov 2026 code audit)

### Model library: shared textures, header bounds, streaming

`src/model_library.h` (360) wraps `Library<raylib::Model>` with three things afterhours `ModelLibrary` lacks. KayKit models share PNG atlases, so it loads each PNG once by content hash and points every material at it (4234df5, 166 lines). It reads glTF header bounds without loading meshes (`src/gltf_bounds.cpp`, 191), for `--skip-models` and for placing things before their model loads. It also had a streaming queue (`request_high`, `pump_high_loads(budget)`, pin and release grace, `ModelStreamingSystem`), now unused since b5df49f. Streamed loads are already in the afterhours todo.

- Suggested shape: `models::Library` that shares textures by path or hash across models, `bounds_from_header(path)`, and `load_deferred(name, path)` with `pump(budget_ms)`.

### Level folders with typed entity files

`src/map/map_loader.cpp` (1,267) loads a level from `config.json` plus one `entities/NNN.json` per entity. A `"type"` string goes through two if-chains, one for asset steps and one for spawning, over 17 types. `MapFileEntity{file_id, map_path, dirty}` (`src/map/map_loader.h`) lets the editor write an entity back to its own file with atomic writes, or delete it. Any game with authored levels needs the registry and the write-back.

- Suggested shape: a `level` plugin. `register_entity_type(name, append_assets_fn, spawn_fn, save_fn)`, a folder loader that feeds a load plan, and a `FromFile` component the editor uses to save.

## Editor (Nov 2026 code audit)

### 3D scene editor

`src/editor_systems.cpp` (1,495) is a full scene editor: free-fly camera with speed boost, mouse-ray picking against bounds, a translate gizmo (axis arrows and XZ drag), a rotate gizmo (rings), dirty tracking, Ctrl+S save to JSON, delete, a property panel, world labels, a status line and the Trees, Grass and Buildings view toggles (`src/view_toggles.*`). Tests need another 701 lines of editor e2e commands (`src/e2e/e2e_editor_commands.cpp`). It has 23 commits, including fixes for scale lost in drags (2296734), rotation not saving (885fe1d), drag state outliving the selection (bd6b635) and outlines on huge entities (ac50d87). It has no undo: `command_history.h` ships, but nothing connects it to entity transforms.

- Suggested shape: an `editor3d` plugin. Picking through the collision world, immediate-mode translate, rotate and scale gizmos, a selection and dirty set, undo through `CommandHistory`, a save hook from the level plugin, and e2e commands (`editor_select`, `editor_move_selected`, `expect_editor_dirty`).
- The feedback note (type a note, save it with a screenshot, the camera position and the selection) suits any playtest build. It could be a standalone `bug_note(dir)` helper that writes text, a PNG, the build id and caller-supplied context.

## Engine core (Nov 2026 code audit)

### App states with a SystemManager each

`src/game_loop.cpp` (560 lines, 74 commits) owns six `SystemManager`s (menu, game, pause, transition, gallery, editor) and switches between them every frame. Entering, leaving and transition progress are all hand-written (`src/app_state.h`). The per-manager e2e problems filed above (shared command packs, serial dispatch) come from this layout.

- Suggested shape: `app_states` with `add_state(id, SystemManager&)`, `on_enter` and `on_exit`, `transition_to(id, seconds)` with a progress value, and a shared manager that runs in every state (e2e, input, audio).

### Profiler zones and overlays

afterhours now times each system (`core/system_timing.h`, `profiling::Collector` with snapshots, p99 and counters) and ships `dump_profile` and `expect_fps_above` behind `perf_commands::set_provider`. Cartographer built its own first: `src/profiler.h` (343: nested `PROFILE("name")` scopes, a 100-sample ring per node, entity counts, collapsed flamegraph export), about 270 lines of overlay in `src/debug_systems.cpp` (foldable tree, frame-time graph, mini chart), per-system timing by type name in `src/cart_system.h`, `src/frame_time_tracker.h` (311: spike log and `--log-fps` summaries), `src/gpu_timing.cpp` (137: GL timer queries) and its own copies of the two e2e commands.

- Still missing upstream: nested zones inside a system, an overlay, flamegraph export, GPU timers and spike capture.
- Can adopt today: feed the game profiler through `perf_commands::set_provider` and drop the duplicate handlers. `scripts/sample_to_collapsed.py` is a copy of afterhours `tools/sample_to_collapsed.py`.

### Window sizing helpers

`src/window_resize.h` (148) and `src/windows_dpi.cpp` defer resizes to the start of a frame, fit a size to the monitor, enforce a 960x540 floor, centre the window and compute the DPI unit scale. afterhours `window_manager` has `set_window_size` and `content_viewport`.

- Suggested shape: `window_manager::set_min_size`, `center_on_monitor()`, `fit_to_monitor(resolution)`, and a deferred `request_size` applied at frame start.

### Shipped but not adopted: data folders, bundle paths, log file

`src/main.cpp` finds its own Application Support and `%APPDATA%` folders, changes into `Contents/Resources` inside a `.app`, and redirects stdout to a log file. `scripts/package_demo.py` (138) writes its own `Info.plist` and ad-hoc signs. afterhours ships `files::get_save_path` and `get_config_path` (via sago), `chdir_to_resource_root`, `log_file.h` and `tools/mk_bundle.sh` (plist, icon, signing). Not checked why the game didn't use them; most likely it predates them. `mk_bundle.sh` is macOS only and doesn't zip, so the README, build id and Windows zip steps would stay in the script.

## E2E testing: generic commands the game added (Nov 2026 code audit)

### Screenshot baselines with a tolerant diff

`expect_screenshot NAME [max_diff_pct] [delta]` in `src/e2e/e2e_commands.h` (about 120 lines, ac5c043) compares the frame to `tests/baselines/NAME.png` with a per-channel delta and a changed-pixel budget, and writes `.actual.png` and `.diff.png` on failure. afterhours `E2EArgs` parses `--update-baselines`, but no command uses it. Screenshots are written on background threads (`src/screenshot_writer.h`, 7249967) because one 2560x1440 PNG stalled a frame.

- Suggested shape: a built-in `expect_screenshot` sharing the `screenshot` path code, honouring `--update-baselines`, with async PNG writes.

### Script headers and a batching test runner

Scripts declare `# spawn:`, `# timeout:`, `# flags:`, `# requires: window` and `# batch: off`. `src/e2e/e2e_runner.cpp` parses two of these by hand (about 60 lines), and `scripts/run_tests_parallel.py` (201, aa4d21e) reads the rest. It groups tests by level and flags into one `-d` process, runs two processes at a time, re-runs batch failures alone, and gives each process its own settings and saves.

- Suggested shape: a runner `script_header(path, key)` API, plus a shipped parallel runner that reads standard headers (`flags`, `timeout`, `batch`, `requires`) and runs batch-then-isolate.

### Test isolation for settings and saves

Test mode points the settings file and saves folder at per-process temp paths (`src/main.cpp`, `map_save`), reseeded each test, with `clean_saves`. It took three fixes (c5fb26f, b5d303f, 4300516) before tests stopped overwriting the player's settings.

- Suggested shape: in test mode, `files::get_save_path` and `get_config_path` return a per-process temp folder by default, plus a `clean_saves` command.

### 3D player and camera commands

`teleport_player`, `set_player_yaw` and `set_player_pitch`, `expect_player_at`, `expect_player_grounded`, and `walk_to`, which holds W, steers, and fails with the nearest collidable when the player makes under 0.25 m of progress in 1 s (`src/e2e/e2e_player_commands.cpp`, 535). `move_camera`, `look_at` and `expect_camera_at` (`src/e2e/e2e_camera_commands.cpp`, 118). `raycast_down`, `raycast_at` and `expect_hit` (`src/e2e/e2e_raycast_commands.cpp`, 278). `scripts/snag_audit.py` walks every park path with `walk_to`. All of these are generic once a character controller and collision world exist.

- Suggested shape: ship them with the `character3d` and `collision3d` plugins.

### Small checks

- `record_screen_luma` and `expect_screen_brighter` / `expect_screen_darker` catch lighting regressions without a baseline. Suggested shape: a built-in `expect_screen_luma MIN MAX`.
- `set_window_size`, `expect_window_size` and `expect_window_centered` (`src/window_resize.h`). Suggested shape: built-ins next to the `resize` command.

## Tooling (Nov 2026 code audit)

- Mesh decimation: `tools/decimate_models.cpp` (449) and `src/mesh_decimate.h` (219). Already in the afterhours todo. The game no longer loads the output.
- Batch card baking: `tools/bake_cards.cpp` (79) bakes every model in a folder tree with one hidden window. `hoc_bake` takes one model per process, so a 340-model bake pays window start-up 340 times. Suggested shape: a folder mode on `hoc_bake` with a name pattern for the output.

## Performance defaults (Nov 2026 valley perf pass)

The valley (8.7k entities) ran at 7-25 fps. Most of the frame was CPU time in our code, the ECS and raylib. The GPU added only about 5 ms. These are the costs afterhours could remove by default, so the next game doesn't find them one at a time.

### Systems scan every entity

`run_system_over` (`core/system.h`) walks the whole entity list for every system, unless all its components are singletons. The game registers each e2e command as its own system, so in test runs 270 handlers each scanned 8.7k entities every frame: about 20 ms. Cartographer works around it with a cached pending-command scan and an `only_possible_match` override (64ee8f4). The water update ran two full `EntityQuery` scans per surface per frame (ab6b34f).

- Suggested shape: per-component membership lists (sparse sets or archetypes) kept up to date on add and remove, so a system or query visits only entities that have its components. Until then, cached query results invalidated by component changes.
- E2E dispatch by command name: one system that looks up the pending command's handler in a map, not one system per command.

### Instanced draws create a buffer per call

raylib's `DrawMeshInstanced` creates and deletes a VBO on every call. On macOS, GL runs over Metal, and each delete forces a command-buffer submit. Cartographer now keeps persistent, growing instance buffers (`src/instanced_draw.cpp`, 249f262) and draws plain boxes instanced by colour (06d710b).

- Suggested shape: an afterhours instanced-draw helper with persistent instance buffers, used by the 3D drawing helpers by default.
- Under investigation: raylib's single default immediate batch resubmits on every flush. If several batch buffers help, afterhours window setup could create them by default.

### Per-frame string hashing

The model streaming system hashed 8.7k model path strings every frame and then did nothing, because every model was loaded (0690913).

- Suggested shape: asset handles (interned integer ids) for library lookups, so hot paths never hash a path.

### Debug builds

`make run` builds at -O0, and the owner plays that build. The ECS's template-heavy hot paths (system dispatch, `EntityQuery`, `has<T>`) are several times slower unoptimised. Because afterhours is header-only, a game can't build just the library at -O2.

- Suggested shape: keep the per-entity hot loops small and non-templated where possible, or offer an optional compiled core that can be built at -O2 inside a -O0 game.

### Second valley pass (fb-perf2)

The -O0 valley frame went from 207-316 million instructions to 23-45 million without changing a pixel. Every big win was a default afterhours could have had:

- **EntityQuery ignores the component lists.** About 20 per-frame `EntityQuery().whereHasComponent<X>()` calls each walked all 8.7k entities through `std::function` filters, roughly 1 ms each at -O0, to find 1-20 matches (lights, birds, the player, water proxies, pending e2e commands). Cartographer now routes them through its per-frame component index (`component_index::with<...>()` and `first<...>()` in `src/component_index.h`). Suggested shape: `EntityQuery` seeds itself from the smallest per-component membership list when the first filter is `whereHasComponent`, so the fix is the default.
- **No cheap "did any component change" signal.** The component index was rebuilt every frame because `addComponent` and `removeComponent` bump nothing (`version` only moves when entities enter or leave). Cartographer now keeps each entity's two bitset words and compares them each frame before rebuilding. The opposite problem hit caches keyed on `version`: every e2e command and every bird spawn adds an entity, so a grid of 8.4k static props rebuilt (about 10 ms at -O0) on unrelated changes until it compared its own member list instead. Suggested shape: a collection-wide component generation counter bumped in `addComponent` and `removeComponent`, plus a per-component one, so a cache over `Collidable` only invalidates when a `Collidable` entity comes or goes.
- **Singleton lookups in inner loops.** `get_singleton_cmp_enforce` is two hash lookups plus `get<T>` checks. Grass called it once per chunk (about 40k chunks a frame) and that alone was 24% of the -O0 frame. Suggested shape: a typed singleton handle that caches the component reference and revalidates on a generation counter, so hot code can hold it.
- **Static entities pay for every system.** 8.4k of the 8.7k entities were props with only a `Collidable`. Cartographer now keeps a spatial grid of Collidable-only entities (`src/collidable_grid.h`, rebuilt on entity add or remove or an editor move) that drives collision queries, prop drawing and shadow casters, while the entities stay entities for the editor and e2e. Suggested shape: an afterhours spatial index over a component, with a static/dynamic split decided by component signature, and query results returned in entity order so physics stays deterministic.
- **raylib `DrawMeshInstanced` is still one call away.** One shadow billboard call still used it and cost 8 ms per shadow redraw on the optimised build (a VBO create and delete per call, each forcing a Metal submit). Suggested shape: afterhours' 3D helpers make the persistent-buffer instanced draw the only easy path, and a debug check warns when `DrawMeshInstanced` runs.
- **Per-draw uniform uploads.** Each receiver draw re-sent five shadow uniforms. Suggested shape: a shader uniform cache keyed by program and a per-frame generation.
- **e2e runner scans.** `E2ERunner::has_pending_commands` (called from `is_idle_waiting` every frame) and the shared command pack's pending gather walk every entity. Suggested shape: the runner keeps its own list of pending command entities.

## Profiling (Nov 2026 valley perf pass)

What would have made the perf pass faster to diagnose, by people or agents:

- **CPU time per frame, independent of machine load.** With other agents running (load average 100-200), wall-clock fps swung 2x between identical runs. Cartographer added main-thread CPU ms to its `--log-fps` lines (c36c853). Suggested shape: thread CPU time in the frame tracker and in `dump_profile`, plus an `expect_cpu_ms_below N` check.
- **Time outside systems.** Early frames were 55-75 ms with most of it in no zone. Suggested shape: report frame time minus the sum of system times as "outside systems", with present or swap wait (`EndDrawing`) split out.
- **GPU attribution.** Cartographer has GL timer queries (`src/gpu_timing.cpp`) and a switch that calls `glFinish` after each pass. Suggested shape: a GPU zone per render pass and a sync mode in the profiler, so CPU-bound and GPU-bound frames are told apart at a glance.
- **Render counters.** Per frame: draw calls, instanced draws, buffer creates and deletes, batch flushes and texture binds. A buffer-create count would have found the VBO churn immediately. Suggested shape: counters in the afterhours draw helpers, shown in `dump_profile`.
- **Entities visited per system.** Per system: entities visited and entities matched. A system visiting 8.7k entities to match 0 or 1 stands out at once. The `system_timing` collector is the natural place for this.
- **Machine-readable dumps and a diff.** Agents grep log lines today. Suggested shape: `dump_profile --json PATH` with zones, counters, CPU and GPU times, and a tool that diffs two dumps, sorted by regression.
- **Spike capture.** Suggested shape: write the zone tree for any frame over N ms, so a 70 ms hitch can be read after the run.
- **Allocations per frame.** A count and bytes per frame (`debug_allocator.h` exists), to catch per-frame vector growth in hot loops.
- **Instructions retired per frame.** On a loaded machine (load average 20-150) CPU ms swung 2-3x between identical runs, because the main thread lands on efficiency cores or contends for cache. Instructions retired (`proc_pid_rusage` `ri_instructions` on macOS) stayed within a few percent. Cartographer prints it in `--log-fps` lines. Suggested shape: instructions per frame next to CPU ms in the frame tracker and `dump_profile`, and an `expect_instructions_below N` check that is stable on any machine.
- **Zones that do not run every frame.** `dump_profile` averages a zone over the frames it ran, so the near shadow redraw (every 8 m of walking) read as 25 ms in a list of per-frame costs. Suggested shape: per zone, both the average per call and the average per frame, plus calls per frame.
- **Per-checkpoint windows.** `Avg frame` in `dump_profile` is the whole run's average. Suggested shape: a `reset_profile` command, or windowed frame stats since the last dump.

## State machines (Nov 2026 refactor plan)

The November 2026 audit (`docs/engineering/state_machine_audit_nov2026.md`, `docs/engineering/refactor_plan_nov2026.md`) found every mode in the game written as loose bools, timers and ints, with transitions scattered across files. Most bugs from it came from a field set in one place and not reset in another. afterhours has nothing between a raw enum and a whole `SystemManager` per state.

### A small `State<E>` helper

- Problem: `AppState` has 21 direct writes in 8 files (`game_loop.cpp`, `menu_systems.cpp`, `journal_systems.cpp`, `gallery_systems.cpp`, `entity_makers.cpp`, `main.cpp`, e2e). Leaving a state doesn't run cleanup: Gallery Replay jumps straight to Playing with no load (`gallery_systems.cpp:204`), and leaving the F1 editor didn't let go of a ladder until b7ac54c. Earlier reset-on-enter bugs: quit confirm, pause animation.
- Suggested shape: `State<E>` with `set(E)` that runs registered `on_exit(old)` / `on_enter(new)` hooks, plus `previous()`, `time_in_state()`, `changed_this_frame()`, and an optional magic_enum transition log in debug builds. An `allowed(from, to)` table that logs or asserts on a bad transition.
- Alternatives: a full HSM library (too heavy for a dozen small machines); `std::variant` states (good for data per state, but every caller needs a visitor).
- Today: plain enums plus hand-written cleanup at each call site.

### Open, opening, closing as one helper

- Problem: Closed/Opening/Open/Closing with a 0-1 `t` is hand-rolled six times: map (`open_anim` + `is_closing`), journal (`journal_motion`, `journal_closing`, `journal_was_showing`, `pause_just_opened`), gallery (`gallery_motion`, `gallery_closing`, `gallery_was_open`), boss letter (`is_showing` + `open_anim`), menu fade, work-check note. The pause animation bug and the map soft-lock came from these copies drifting apart. `ui_motion` has an `Appear` trigger for UI elements but no closing phase, and these screens are custom-drawn, not UI trees.
- Suggested shape: `Reveal` with `open()`, `close()`, `update(dt)`, `t()`, `phase()`, `just_opened()` / `just_closed()` edges, speed per direction, and the settings' reduce-motion flag snapping it.
- Today: six copies, each with its own `was_open` edge tracking.

### Run systems only in a state, and on enter or exit

- Problem: each `AppState` gets its own `SystemManager` (`src/game_loop.cpp`), so a system shared by two states is registered twice. Inside one manager, systems still check state by hand: 8 `app_state == Editor` checks in `editor_systems.cpp` (redundant, the editor manager only runs there) and 7 `== Playing` checks in survey, rope and tutorial systems (needed only because the headless loop keeps running game systems while paused). A `should_run` that only overrides the const overload is silently ignored (filed above).
- Suggested shape: `register_update_system(sys).run_if(in_state(AppState::Playing))` and `on_enter(state, fn)` / `on_exit(state, fn)` (Bevy's `run_if(in_state)` and `OnEnter`). One manager with gated systems instead of one manager per state.
- Today: six managers plus hand checks.

### Generic e2e state commands

- Problem: every machine gets its own hand-written pair of e2e commands: `expect_app_state`, `expect_rope_state`, `expect_tutorial_step` / `set_tutorial_step`, `expect_map_tool`, `expect_editor_tool`, `set_editor_tool`, `expect_climbing`, `expect_bird_state`, each parsing the enum name and printing its own error.
- Suggested shape: register an enum getter (and optional setter) under a name, then `expect_state NAME VALUE` and `set_state NAME VALUE` parse through magic_enum and list the valid names on a typo.
- Today: about a dozen near-identical handlers.

### Enum fields saved by number

- Problem: the tutorial step was saved as its enum number, and the enum was reordered three times with no save version bump, so old autosaves loaded the wrong step (fixed in bddaa45 by saving the name). Rope state is still saved by number.
- Suggested shape: the save and settings helpers write enums by name (magic_enum) by default, with a fallback value for unknown names.

## ECS ergonomics (Nov 2026 refactor plan)

Things the refactor sweep wrote by hand many times that the library could make one line.

### Find the one entity with these components

- Problem: `EntityQuery().whereHasComponent<Camera3DComponent>().whereHasComponent<PlayerControlled>().gen_first()` was written about 31 times before the game added `player_entity()` (89f5362); two private copies of that helper already existed. Each call builds a query and scans every entity (see "Systems scan every entity").
- Suggested shape: `EntityHelper::get_unique<A, B>()` returning `OptEntity`, cached and invalidated when either component is added or removed, or tag singletons (`registerSingleton<PlayerTag>` that also works for a non-singleton component pair).

### Add and register a singleton in one call

- Problem: `addComponent<T>()` followed by `registerSingleton<T>()` appears 16 times in `src/entity_makers.cpp`, and the game wrote `add_app_singleton<T>` in `src/app_entity.cpp` to pair them. Eight more file-private singletons each have their own 4-line `add_*_state`. Registration also logs one info line per singleton (about 45 lines at start, filed in `todo.md`).
- Suggested shape: `EntityHelper::add_singleton<T>(entity, args...)` that adds, registers and returns `T&`, with the registration log at debug level.

### Share system lists between managers

- Problem: `register_update_systems` and `register_editor_base_systems` in `src/systems.cpp` register the same 11 systems in the same order, and the game and editor render lists share 16. They drift when a system is added to one and not the other.
- Suggested shape: a named system group registered into several managers, or this goes away with run-if gating above.

### Settings fields declared once

- Problem: `src/settings.cpp` lists about 30 fields twice (to_json, from_json), and `src/e2e/e2e_ui_commands.cpp` maps the same key strings to the same members a third time for `set_setting` / `expect_setting` (`setting_bool_field`, `setting_int_field`, `setting_float_field`). Adding a setting means editing three places.
- Suggested shape: the settings plugin takes a field table (name, member pointer, default, optional clamp) and provides JSON load and save plus generic `set_setting` / `expect_setting` e2e commands. Fields with custom logic (window mode, resolution, key bindings) register their own codec.

### Boolean e2e checks from a getter

- Problem: seven on/off pairs differ only in a negation and the message: `expect_light_on` / `off`, `expect_map_pin_placed` / `not_placed`, `expect_map_open` / `closed`, `expect_map_ruler_placed` / `not_placed`, `expect_terminal_open` / `closed`, `expect_hit` / `no_hit`, `expect_preview_playing` / `stopped`. Newer commands already take `on|off` (`expect_scope_drawn`, `expect_climbing`).
- Suggested shape: `register_expect_flag("expect_light", [] { return ...; })` that parses `on|off`, retries until the runner's timeout and writes a standard message.

### One input gate for overlays

- Problem: map systems repeat `if (map.is_comparing || workspace.consumes_input) return;` about 13 times, and every tool, prompt and camera system has its own "is a menu, map or terminal open" check. This is the input layer idea in afterhours input, not wired to app overlays.
- Suggested shape: an input focus stack (gameplay, map, compare view, terminal) where a system declares its layer and only the top layer's systems see input.

## From the todo list (Nov 20 2026)

A pass over `todo.md` for features that need engine support or would be much smaller with it. afterhours already ships some of what the list needs, so these are adopt, not gaps: `translation.h` and `language.h` (localization), `command_history.h` (editor undo), `file_watcher.h`, `random.h`, `particles.h`, `pathfinding.h` (grazing deer and walking pigeons), `toast.h` and `modal.h`.

### Level unload and world reset

- Problem: the todo's "level teardown before Level 2" item. `game_load_plan()` is built once (`src/entity_makers.cpp`), and `reset_test_state` (`src/entity_makers.cpp:765`) only re-runs setup steps, so file statics, globals, system members, texture caches and shader uniforms survive a level change. Going park, then menu, then town needs a restart. The e2e batch runner reuses one level across scripts, so this kind of leak also makes tests fail only when they share a process (four visual tests did in November 2026).
- Suggested shape: entities tagged with a scope (app, level, test) and `destroy_scope(level)`; caches owned by a level-scoped singleton so they go with it; a rebuildable load plan; and an `on_level_unload` / `on_test_reset` hook list that systems and plugins register into, which the e2e batch runner calls between scripts.
- Today: hand-written reset in one function that every new piece of state has to remember to join.

### Audio buses and ambience

- Problem: the audio settings page wants Master, Music, Effects, Ambience, Dialogue and Character sliders. `sound_system` has one volume per library (sound, music) times master. Footsteps per surface (grass, gravel, wood, sand), several ambient music tracks instead of one loop, and night ambience all need the same routing.
- Suggested shape: named buses with a volume each (and a parent, so all go through master), every sound assigned to a bus at load or play; sound sets (`play_random("footstep_gravel")` exists, but a surface-to-set table would help); a music playlist with crossfade and silence gaps between tracks.
- Today: three volumes in `SettingsData`, birds route by hand through `src/bird_audio.cpp`.

### Settings presets that turn into Custom

- Problem: the graphics page (quality preset over fog, lens flare, glows, cards, shadows, shadow distance and resolution) and the per-aid navigation toggles (compass, footsteps, rope rings, work-checks, hints, with Relaxed, Standard and Purist presets) both need "a preset sets a group of fields, and editing any field flips the preset to Custom". Quality already does this by hand.
- Suggested shape: in the settings plugin, a preset group: a name per preset, a field-value table, and a derived current preset (or Custom) computed from the fields, plus the UI segmented control for it.

### Tunable values with live reload

- Problem: "way too many hardcoded numbers", and the bird tuning item wants every constant in `src/bird_systems.cpp` and `src/bird_audio.cpp` in a file. Tuning a value today needs a rebuild.
- Suggested shape: `tunable<float>("birds.flush_radius", 5.f)` registered in a table, loaded from a JSON file at start, reloaded through `file_watcher`, and settable from the terminal and e2e (`tune NAME VALUE`). Release builds read the file once or bake it in.

### Capture any view to an image

- Problem: map export (a high-resolution PNG of the sheet and a framed print layout), the camera tool's photo album (photos taken while walking, shown later as thumbnails) and `scripts/make_valley_loading_art.py` (renders an e2e view into `resources/textures/loading_valley.png`) all render something offscreen at a chosen size and save it. Screenshots only capture the window at window size.
- Suggested shape: `capture(view_or_draw_fn, width, height) -> Image`, with PNG save off the main thread and a thumbnail helper. Fits the `RenderView` gap above.

### Fonts at the display's pixel size

- Problem: the loading screen text is blurry at its on-screen size; the fix today is raising each `LoadFontEx` size by hand. On retina and scaled Windows displays every font has the same problem.
- Suggested shape: the font manager loads each face at size times the window's DPI scale (and reloads on scale change), or keeps an SDF atlas, so text is crisp at any size without per-call tuning.

### Lighting zones

- Problem: the valley town tunnel needs an interior lighting zone, and caves would need the same: inside a volume, ambient and sun drop and nearby lights take over, with a fade at the mouth.
- Suggested shape: a `LightingZone` (box or capsule, ambient and sun scale, fog override, blend distance) that the scene lighting singleton blends in by camera position. Builds on "Scene lighting broadcast to many shaders".

### Surface material on ray hits

- Problem: footsteps per surface need to know what the player stands on: terrain splat colour, a road, a bridge deck or a building porch. Nothing records a material today.
- Suggested shape: a material id on collision shapes, and for heightfields a lookup from a material map, returned in the raycast hit. Belongs with the `collision3d` gap.

### Skeletal animation

- Problem: the character model packs (farmers, townsfolk, horses) are only usable once skinned animation plays; birds swap whole poses instead. `models.h` loads static models only.
- Suggested shape: an animated model component (clip by name, time, speed, loop, crossfade) driven by raylib's `ModelAnimation`, with instanced skinning later for crowds.

### Multi-frame e2e commands block the runner

- Problem: the runner sends one command per frame and doesn't wait for a command that takes several frames, so the next commands run while it is still going. Cartographer holds the runner by hand while `walk_to` or `wait_until` is pending (`blocking_command_pending` in `src/e2e/e2e_runner.cpp`).
- Suggested shape: a handler marks its command blocking (or returns "still running"), and the runner waits for it to finish or fail before sending the next.

### Optional singleton lookups that warn

- Problem: `input_system.h` asks for `ProvidesInputConfig` through `get_singleton_cmp`, which is optional, yet the lookup logs "Singleton map is missing value for component 76" when no game registers one.
- Suggested shape: `get_singleton_cmp` returns null silently; only the enforcing lookup warns. Or the input plugin registers a default `ProvidesInputConfig`.
- Shipped (7e7fd4f): the input plugin registers a default `ProvidesInputConfig` (`input_system.h:1321`), and a missing singleton warns once per component (`entity_collection.h:423-429`).

### Closed captions

- Problem: closed captions are an accessibility basic, and every game rebuilds them. Cartographer has a `sound_captions` setting, but only the birds caption, with their own timer, text and draw code in `src/bird_audio.cpp` (one line at a time, "[birdsong]" or "[wings flutter]"). Every other sound (footsteps, rope, flags, survey beam, water, torches, menu, map, music) would need the same code again.
- Suggested shape: a caption text on each sound or sound set when it is registered with `sound_system` (or a lookup the game provides), shown automatically when it plays while a `captions` setting is on. A caption queue: several lines stacked, the same caption merged with a count instead of repeated, a minimum and maximum time on screen, priority so music and ambience don't push out important sounds, and for positional sounds (see Positional audio) a left or right arrow or distance hint. Style comes from the theme, and an e2e command (`expect_caption TEXT`) replaces per-game ones like `expect_bird_caption`.

## Status of `docs/afterhours-gaps.md`

- Mouse delta as an action: shipped as `MouseAxisWithDir` in the input mapping, scaled by `ProvidesInputConfig::mouse_delta_scale`. Not adopted: `src/input_action.h` and the camera still call `GetMouseDelta`.
- `get_input_collector()` uses the singleton registry now. Done.
- Per-manager handler registration: `register_shared_handlers` shipped (see E2E testing above).
- `synthetic_press_delay` is now documented in `input_injector.h`. The one-frame delay itself remains.

### Not evaluated yet

- `retry_with` on game handlers: game `expect_*` handlers could call `cmd.retry_with(msg)` instead of `cmd.fail` and retry for the runner's timeout. Cartographer keeps `wait_until` for now because it takes a timeout in seconds per call.

## Found adopting 7e7fd4f (Nov 23 2026)

Checked against `vendor/afterhours` at 7e7fd4f. Line numbers are afterhours files unless they start with `src/` in Cartographer. Small and concrete on purpose: the last bump merged 86 gaps of this size and none of the big subsystem asks above.

Filed upstream in afterhours `todo.md` under "Cartographer gaps (2026-11-23)" (afterhours 216b0245), after rechecking against afterhours main 12c17a82. Already on that main, so adopt on the next bump instead: sound aliases (`audio_helpers.h`), CPU image writing (`make_image`, PNG encode, `export_image` in `image.h`), `test_input::set_right_button_down` (only the e2e commands are filed), and `get_singleton` in `entity_helper.h`.

### Bugs

#### Mouse press edge is seen by one caller per frame
- Problem: `input::is_mouse_button_pressed` keeps `static bool prev[8]` inside the function (`input_system.h:114-136`, both branches). A second caller in the same frame gets false, and a caller that skips frames sees a stale or phantom press.
- Today: Cartographer bypasses it and passes `raylib::IsMouseButtonPressed` into `test_input::is_mouse_button_pressed` (`src/map/map_systems_internal.h:321,326`, `src/map/map_input_systems.cpp:1743,1798`, `src/gallery_systems.cpp:216`, `src/game_loop.cpp:141`).
- Suggested shape: keep the previous-frame button state in the input collector or backend, updated once at frame start, so the query is read-only.

#### measure_text ignores the default font
- Problem: `draw_text` uses the `set_default_font` face with spacing 1 (`drawing_helpers.h:219-229`), while `graphics::measure_text` still calls `raylib::MeasureText` with raylib's font (`backends/raylib/backend.h:256`). Measured widths don't match the drawn text.
- Today: `measure_debug_text` in `src/debug_text.h:13-18` calls `MeasureTextEx(get_default_font(), ..., 1.f)`. It and raw `MeasureText` have 23 call sites.
- Suggested shape: `measure_text` measures with the override face and the same spacing `draw_text` uses.

#### The mouse misses the injection fix keys got
- Problem:
  - `graphics::is_mouse_button_*`, `get_mouse_position` and `get_mouse_wheel_move` read the platform directly, so they never see injected input (`graphics.h:141-156`).
  - In test mode, `input::get_mouse_delta` and `is_mouse_button_down` fall back to the real OS mouse when `injector.mouse.active` is false (`input_system.h:83-112`), so a person touching the trackpad changes a test.
  - `set_mouse_position` overwrites the delta instead of adding to it, and `consume_mouse_delta` clears it on the first read (`input_injector.h:166-197`).
- Today:
  - Cartographer parks the injected mouse at (-10000, -10000) every frame (`src/e2e/e2e_runner.h:18-39`, called from `src/game_loop.cpp:86,88,412,415`).
  - It keeps its own delta in `scripted_mouse_delta` and `game_mouse_delta` (`src/input_action.h:545-557`).
- Suggested shape:
  - in test mode the injector always answers: buttons up, no delta, a parked or last scripted position;
  - the delta adds up until frame reset and is not cleared on read (the fix `scroll_wheel` got);
  - the `graphics::` mouse functions go through `test_input`, like `is_key_pressed` now does.

#### Headless get_time is wall-clock
- Problem: the headless backend simulates `get_delta_time` from fps and time scale (`headless.h:116`), but `get_time` is `raylib::GetTime()` (`backends/raylib/backend.h:253`). Anything animated from it (shader time) differs from run to run in screenshots.
- Today: `animation_clock::seconds()` (`src/animation_clock.h`) returns frame count times dt in headless runs. It feeds `src/render_pipeline.cpp:106` and `src/map/map_canvas_render.cpp:741,764,1039`.
- Suggested shape: headless `get_time()` returns the sum of simulated dt, honouring `fixed_dt` and time scale.

### e2e and input helpers

#### right_down and right_up commands
- Problem: the built-ins have `middle_down` / `middle_up` but no held right button; `test_input` only has `simulate_right_click`, which releases itself.
- Today: `src/e2e/e2e_input_commands.cpp:114-149` sets `input_injector::detail::mouse` fields by hand for right-drag camera look.
- Suggested shape: `simulate_right_press(x, y)` / `simulate_right_release()` and the two commands, mirroring middle.

#### The runner's current and next script
- Problem: script paths and `current_script_name()` are private (`runner.h:680,777`), and the reset callback gets no arguments.
- Today: to read the next script's `# spawn:` header before reset, Cartographer lists the test folder again in the same sort order (`collect_test_names`, `src/e2e/e2e_runner.cpp:69-100`). A smaller slice of "Script headers and a batching test runner" above.
- Suggested shape: public `current_script()` returning `{index, path, name}`, and `set_reset_callback(std::function<void(const ScriptInfo &next)>)`.

#### Cursor mode
- Problem: afterhours has no cursor wrappers (no `DisableCursor`, `EnableCursor`, `HideCursor` or `IsCursorHidden` anywhere).
- Today: `apply_cursor_mode` (`src/input_action.h:615-627`), plus `EnableCursor` at `src/game_loop.cpp:480,520` and `src/journal_systems.cpp:1884`, and an `expect_cursor_mode` e2e command.
- Suggested shape: `graphics::set_cursor_mode(CursorMode::Free | Hidden | Captured)`, which does nothing when the mode is unchanged, and `get_cursor_mode()`. On sokol it maps to `sapp_lock_mouse` / `sapp_show_mouse`.

#### Multi-click tracker
- Problem: the click counter lives only inside `text_input` (`text_input/component.h:521-533`).
- Today: the map Hand tool hand-rolls double-click with a time window and pixel slop (`src/map/map_input_systems.cpp:1713-1747`).
- Suggested shape: `input::MultiClick{max_interval, slop}` whose `update(pressed, pos, now)` returns the click count, like `input::Repeat`.

### Thin wrappers

#### Music streams and sound aliases
- Problem: #144 wrapped sound pitch, pan, stop, pause and resume, but music streams and aliases are still raw raylib.
- Today, in `src/sound_triggers.cpp`:
  - `SetMusicPitch` at :91, `SetMusicPan` at :138, `GetMusicTimePlayed` at :143;
  - `PauseMusicStream` at :148 and :679;
  - `LoadSoundAlias` at :546, `UnloadSoundAlias` at :515.
- Suggested shape: `PauseMusicStream`, `ResumeMusicStream`, `SetMusicPitch`, `SetMusicPan`, `GetMusicTimePlayed`, `SeekMusicStream`, `LoadSoundAlias` and `UnloadSoundAlias` in `audio_helpers.h`, with none-backend stubs.

#### Cylinder between two points, and world to screen in content size
- Problem:
  - `draw_cylinder_wires` takes start and end points, but solid `draw_cylinder` takes only a base (`drawing_helpers_3d.h:34-42`).
  - `get_world_to_screen` uses the window size (`:49-51`), which is wrong when drawing into the render texture.
- Today:
  - 16 `DrawCylinderEx` calls in 5 files (rope, survey beam, lighthouse beam, tools, editor);
  - `GetWorldToScreenEx(..., DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT)` at `src/editor_systems.cpp:1191`, `src/systems/RenderFlagRopeLengthSystem.h:46` and `src/systems/RenderGymLabelsSystem.h:37`.
- Suggested shape: `draw_cylinder_ex(start, end, r0, r1, sides, color)`, and `get_world_to_screen(pos, cam, w, h)` defaulting to the content size.

#### Writing CPU images
- Problem: `afterhours::Image` (#81) loads and reads pixels but can't be created, drawn into or saved.
- Today:
  - procedural sprites with `GenImageColor` (13 calls) and `ImageDrawPixel` loops (6 calls) for smoke, plumes and spray (`src/render_helpers.h:12-22`, `src/water_strips.cpp:860-894`, `src/water_surface.cpp`);
  - `ExportImage` 7 times and `ExportImageToMemory` twice;
  - a hand-written worker-thread PNG writer (`src/screenshot_writer.h`).
- Suggested shape: `make_image(w, h, color)`, `Image::set_pixel`, `encode_png(const Image &) -> std::vector<uint8_t>`, `save_png(image, path)` (atomic through `files`) and `save_png_async`; upload with the existing `load_texture_from_rgba`.

#### Texture update and unflipped readback
- Problem: there is `load_texture_from_rgba` but no update, and `capture_render_texture_rgba` always flips vertically.
- Today: `UpdateTexture` / `UpdateTextureRec` at 5 sites (`src/map/map_lasso.cpp:152`, `src/map/map_session_systems.cpp:838,842`, `src/map/map_history.cpp:276`, `src/valley_colormap.cpp:125`), and `LoadImageFromTexture` 19 times, mostly raw reads of map layers that must not flip.
- Suggested shape: `update_texture_rgba(tex, pixels, optional rect)` and `read_texture_rgba(tex, optional rect, bool flip = false)`.

### Other

#### Cursor motion and typing for hand-drawn text fields
- Problem: `edit_command_from_keys()` (fb25eed) covers edit commands only. Arrow keys, word jumps, Home/End, Shift selection and typed characters live only inside the imm widget (`text_input/component.h:646` on).
- Today: Cartographer copied them as `text_field::edit_line` (`src/text_field.h`, 115 lines) for the map label and the feedback note.
- Suggested shape: `text_input::motion_from_keys(state)` and `insert_typed_chars(state)`, or one `edit_line_from_keys(state)` that runs edit commands, motion and typing.

#### Default logger gaps
- Problem:
  - `log_once_per` is an empty function in the default logger ("For now, once per logging is disabled", `logging.h:86-91`), so it silently does nothing.
  - There is no runtime minimum level.
  - raylib `TraceLog` output bypasses `log_sink_fn`.
- Today: Cartographer replaces the logger with `AFTER_HOURS_REPLACE_LOGGING` (`src/log/log.h`: runtime level, quiet list, file:line) and routes raylib through its own `SetTraceLogCallback` (`src/main.cpp:115-179`) to drop the "u32 to u16" spam and write to the log file.
- Suggested shape: implement `log_once_per`; add `set_log_level(level)`; add `graphics::route_backend_logs(min_level, filter)`, which installs a trace callback that writes through `log_*` and the sink.

#### Render scale fixed at init
- Problem: the raylib windowed backend sets `render_scale_` and the render texture once in `init` (`windowed.h:55-66`). A window that grows or moves to another DPI keeps the start-up scale.
- Today: on Windows, Cartographer resizes the render texture, writes `graphics::render_scale()` directly and reapplies the projection, because the backend's own copy of the scale goes stale (`src/game_loop.cpp:157-186`, `src/window_resize.h:13-16`).
- Suggested shape: `graphics::set_render_scale(int)` or `resize_render_target(logical_w, logical_h, scale)`, which reallocates the texture and updates both copies.

#### Small helpers (lower odds)
- **Stateless hash and value noise:** `random::hash01(x, y, salt)` and `value_noise(x, y)`. Hand-written copies are at `src/rl.h:92`, `src/tree_field.cpp:118,132`, `src/map/map_session_systems.cpp:448,453`, `src/sound_triggers.cpp:312,317` and `src/map/map_loader.cpp:844`; `particles.h:102` has one but keeps it private.
- **Math:** `smoothstep` (3 copies in Cartographer), exponential `damp` (4 copies), `wrap_angle` / `lerp_angle`.
- **Relaunch:** `process::relaunch_self(args)` for restart-to-apply (Cartographer's `src/level_launch.h` uses `execv`).
- **Singleton shorthand:** `singleton<T>()`. `src/cart_system.h:169` has a "TODO: move into afterhours" over a helper with 283 uses.

