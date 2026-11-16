# Afterhours gaps and change requests

## E2E testing
- Provide built-in commands for menu UI flows: `click_btn`, `expect_label`, `expect_app_state`, `set_volume`, `check_volume` so every game does not re-implement them.
- Allow registering a shared command pack across multiple `SystemManager`s so menu and pause systems can reuse the same handlers without duplicate setup. Shipped as `testing::register_shared_handlers(sm, pack)` (afterhours 4509ffa) and adopted: `shared_e2e_commands()` holds the input, UI, save, wait and cleanup handlers once, and every state's manager runs it. Still open: `register_unknown_handler` can't go in the pack for Cartographer, because gameplay commands sent at the menu are meant to wait for gameplay, and the unknown handler fails them on the first frame. A per-command "wait for another state" marker would let it.
- Ship built-in e2e commands for the `terminal::Console` plugin (`expect_terminal_open`, `expect_terminal_closed`, `expect_terminal_input`, `expect_terminal_output_contains`) so games using the console don't hand-roll them like `src/e2e/e2e_terminal_commands.cpp`.

## UI entity management
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
- Add static convenience constructors on `Margin` and `Padding` for single-side values, e.g. `Margin::left(pixels(10.0f))`, `Margin::top(h720(8.0f))`, `Padding::horizontal(pixels(16.0f))`. Currently setting one side requires designated initializers with all other sides defaulting, which is verbose.

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
- There is no way to record frames. Finding one-frame flashes needed a hand-rolled `--record` that pipes `LoadImageFromScreen` into ffmpeg with a fixed 1/60 dt. It must flush the rlgl batch first, or the last batch is missing from the frame. A built-in capture with fixed-dt playback would make transition bugs visible in tests.
- There's no incremental loader. The game now builds a `LoadPlan` (asset steps per level, then world setup steps) and runs slices of it within a per-frame time budget, so the loading screen is a normal system and the bar shows steps done. afterhours could ship this with async file reads, because one gltf still blocks its frame for about 25ms.
- `Library::load` parses the file before checking the name, then drops the result on `DUPLICATE_NAME` without unloading it. Loading a model twice leaks it. `ModelLibrary::load` now checks `contains` first. Fixed upstream (e8f5a82): `Library::load` checks the name first and warns.
- The e2e runner silently skips commands that no handler in the active `SystemManager` consumes. During a multi-second `Transitioning` state, `expect_app_state` and friends were skipped and passed, so test mode now loads the whole plan at once.

## Direct raylib usage
At 87f2141, `src` makes 1,062 direct raylib calls across 28 files and 166 distinct functions. The count is `raylib::Name(` occurrences in `src`. The menu, gallery, pause and letter screens account for 224 of those calls. The rest is game rendering, world building, and the map.

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

Shipped as `set_default_font(font)` / `clear_default_font()` (af6254d). Adopted: Paper Mono (`resources/fonts/PaperMono-Regular.ttf`, OFL) is set at startup, and the profiler, terminal, editor, test-name, LOD legend and gym label overlays draw with it through `src/debug_text.h`. Player-facing text keeps its own fonts. Still open upstream: the input debug overlay in `input_system.h` calls `raylib::DrawText` (the header can't see the drawing helpers), and the `font_helper.h` error paths return raylib's font.

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

### Not evaluated yet

- `house_of_cards` impostor cards: a possible fit for distant trees and the skyline buildings (P2 in todo.md), but it needs Cartographer's fog and lighting on the cards; not tried.
- `retry_with` on game handlers: game `expect_*` handlers could call `cmd.retry_with(msg)` instead of `cmd.fail` and retry for the runner's timeout. Cartographer keeps `wait_until` for now because it takes a timeout in seconds per call.

