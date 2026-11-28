# afterhours gaps — mailboy (Web / Emscripten backend)

Filed after wiring up Emscripten 6.0.3 + Raylib 5.5 web builds in `mailboy` and
comparing against `afterhours` (`216b0245`: `tools/web.mk`, `tools/web/shell.html`,
`src/graphics/web.h`).

## 1. `tools/web/shell.html` does not unlock WebAudio on `keydown`
- **Symptom:** On keyboard-driven games (e.g. pressing `SPACE` to start), audio
  stays completely silent in Chrome/Safari until the player clicks the mouse.
- **Root cause:** Raylib 5.5's `miniaudio.h` only registers `miniaudio.unlock` on
  `["touchend", "click"]` — not `keydown` or `pointerdown`. If a player starts
  and plays purely with the keyboard, `device.webaudio` remains `suspended`.
- **Suggested fix in `tools/web/shell.html`:** Track created `AudioContext`
  instances (or iterate `window.miniaudio.devices`) and call `ctx.resume()`
  inside `keydown`, `mousedown`, `pointerdown`, and `touchstart` listeners.

## 2. `tools/web/shell.html` does not prevent page scroll on `Space` / Arrow keys
- **Symptom:** When embedded in an itch.io iframe (or any scrollable page),
  pressing `Space` or `ArrowUp`/`ArrowDown` scrolls the host page instead of
  staying inside the game.
- **Suggested fix in `tools/web/shell.html`:** Add a non-passive `keydown`
  listener calling `e.preventDefault()` for `Space`, `ArrowUp`, `ArrowDown`,
  `ArrowLeft`, and `ArrowRight`, and auto-focus `#canvas` on
  `onRuntimeInitialized`.

## 3. Emscripten 6.0+ requires `HEAPF32` exported (already in `tools/web.mk`, easy to miss standalone)
- **Symptom:** Silent audio with `TypeError: Cannot read properties of undefined
  (reading 'buffer')` in `onaudioprocess` when building without `tools/web.mk`.
- **Note:** `tools/web.mk` already includes `-sEXPORTED_RUNTIME_METHODS=...,HEAPF32`
  and avoids `-sALLOW_MEMORY_GROWTH=1` (whose buffer reallocation detaches
  miniaudio's cached `intermediaryBufferView`). Documenting this prominently in
  `src/graphics/web.h` or `docs/` helps games not using `web.mk`.

## 4. `tools/web.mk` portability & bundle-size ergonomics
- **Windows-only defaults:** `EMSDK ?= F:/emsdk` and `RAYLIB_WEB_SRC ?= F:/raylib-src`
  fail out of the box on macOS/Linux (e.g. Homebrew `emcc` on PATH). Defaulting
  `RAYLIB_WEB_SRC` to a platform-neutral path (or auto-cloning Raylib 5.5 into
  `$(OBJ_DIR)/raylib-src` on `web-raylib`) would make `make web` zero-config.
- **512 MB default `WEB_INITIAL_MEMORY`:** `536870912` can OOM on mobile Safari
  and is overkill for small jam games; a smaller default (e.g. 64–128 MB,
  overridable per game) is safer for itch.io web embeds.
- **Always-on `-sASYNCIFY`:** Games using `emscripten_set_main_loop` don't need
  Asyncify's ~30% `.wasm` size/runtime overhead. Gating it behind
  `WEB_USE_ASYNCIFY ?= 1` lets callback-loop games ship smaller bundles.

## 5. `src/crash_handler.h` fails to compile on real Emscripten (`emscripten_get_callstack()` wrong arity)
- **Symptom:** Compiling any game including `afterhours/ah.h` with `em++` (Emscripten 6.0.3) fails at `vendor/afterhours/src/crash_handler.h:76` and `:94`:
  ```
  error: no matching function for call to 'emscripten_get_callstack'
  note: candidate function not viable: requires 3 arguments, but 0 were provided
  ```
- **Root cause:** `crash_handler.h` calls `const char *stack = emscripten_get_callstack()`, which only matches `tests/web_sim/emscripten.h`'s mock. Real `<emscripten.h>` declares `int emscripten_get_callstack(int flags, char *out, int maxbytes)`.
- **Suggested fix in `afterhours/src/crash_handler.h`:** Allocate a small stack buffer (`char buf[2048]`) and call `emscripten_get_callstack(EM_LOG_C_STACK | EM_LOG_JS_STACK, buf, sizeof(buf))` (and update `tests/web_sim/emscripten.h` to match the real 3-arg signature).
- **Local workaround in `mailboy`:** `src/components.h` defines a 0-arg C++ overload wrapping the 3-arg Emscripten function before `#include <afterhours/ah.h>`.

---

# afterhours gaps — mailboy (UI, graphics, plugins migration)

Filed while migrating Mailboy's UI, window/frame lifecycle, input, clipboard,
and reduced-motion handling from hand-rolled Raylib code onto `afterhours`
plugins (`AFTER_HOURS_USE_RAYLIB` + `AFTER_HOURS_IMM_UI`).

## 6. `plugins/toast.h` spawns entities in `EntityHelper` instead of `UICollectionHolder`
- **Symptom:** Under the default multi-collection `afterhours::ui` setup (i.e.
  without `AFTER_HOURS_UI_SINGLE_COLLECTION`), `toast::schedule()` creates
  `Toast` entities in `EntityHelper::createEntity()` and `ToastLayoutSystem`
  queues their `entity.id` into `UIContext::queue_render(...)`. However,
  `UIPluginRenderBridge` resolves queued `RenderInfo` IDs against
  `UICollectionHolder::get().collection`, so toasts never render (or resolve the
  wrong entity).
- **Also:** `toast::register_layout_systems` registers as a render system, so
  calling it after `ui::register_render_systems` runs layout *after*
  `UIPluginRenderBridge` has already flushed the render queue.
- **Suggested fix:** Create/query `Toast` entities in
  `UICollectionHolder::get().collection` (or route through `UIPluginPreUpdateBridge`
  / `UIPluginPostUpdateBridge`), and document or enforce ordering before
  `UIPluginRenderBridge`.

## 7. `plugins/settings.h` & `plugins/files.h` have no Emscripten `localStorage` backend
- **Symptom:** `settings.h` / `files.h` persist via `std::ofstream` + POSIX
  `fsync` + `rename`. On Emscripten/Web builds, those writes go to volatile
  in-memory MEMFS and disappear on page reload, so web games still have to
  hand-roll `localStorage` JS bridges.
- **Suggested fix:** Provide an `#ifdef __EMSCRIPTEN__` storage backend in
  `files.h` / `settings.h` (keyed by app/file name in `localStorage`) so
  `settings` works identically on desktop and web.

## 8. `plugins/clipboard.h` on `AFTER_HOURS_USE_RAYLIB` is a no-op on Emscripten
- **Symptom:** `clipboard::set_text()` calls `raylib::SetClipboardText()`, which
  in Raylib 5.5 web builds does not call `navigator.clipboard.writeText()`.
- **Suggested fix:** Under `#if defined(AFTER_HOURS_USE_RAYLIB) && defined(__EMSCRIPTEN__)`,
  call `navigator.clipboard.writeText(...)` via `EM_ASM` inside `clipboard::detail::set_text()`.

## 9. `DisplayMode::Headless` (`backends/raylib/headless.h`) crashes if any system polls Raylib input
- **Symptom:** `RaylibHeadless::init()` creates a `HeadlessGL` context without
  `InitWindow()`, so GLFW's window pointer is null. Running `afterhours::input`
  (`input::register_update_systems`) or any `raylib::IsKeyDown` / `GetMousePosition`
  call segfaults in `DisplayMode::Headless`.
- **Local workaround in `mailboy`:** Use `DisplayMode::Windowed` with
  `cfg.config_flags = raylib::FLAG_WINDOW_HIDDEN` + `graphics::capture_frame()`
  for offscreen screenshot capture (matching `examples/imm_visual_check.cpp`).
- **Suggested fix:** Guard Raylib input queries in `input_system.h` when
  `graphics::is_headless()` is true (return empty input state instead of calling
  into GLFW).

## 10. `ui::init_ui_plugin()` / `ui::setup_with_resolution()` leave singleton entities unmerged
- **Symptom:** Creating `InputCollector`, `ui_root`, and `TooltipState` during
  setup without calling `EntityHelper::merge_entity_arrays()` causes
  `[ERROR] query will miss N ents in temp` on the first frame unless the host
  manually calls `EntityHelper::merge_entity_arrays()` after init.
- **Suggested fix:** Call `EntityHelper::merge_entity_arrays()` at the end of
  `ui::setup_with_resolution()` (and `input::add_singleton_components()`).

## 11. `imm::div(...).with_texture(tex)` does not center under parent `AlignItems::Center`
- **Symptom:** Placing a fixed-size `imm::div` with `.with_texture(tex)` directly
  inside a `FlexDirection::Column` + `AlignItems::Center` container left-aligned
  the texture instead of centering it horizontally.
- **Local workaround in `mailboy`:** Wrap in a `percent(1.0f)` `FlexDirection::Row`
  with `JustifyContent::Center` and draw via `with_on_draw_fg`.

## 12. `plugins/input_system.h` deduplicates multiple simultaneous keys bound to one action
- **Symptom:** Rhythm games that support alternating multi-key rolls (e.g. pressing
  `J` and `K` on the same frame -> `tossCount == 2`) cannot read a per-frame hit
  count from `InputCollector::inputs_pressed`, because `InputSystem` emits at most
  one `ActionDone` per action ID per frame.
- **Suggested fix:** Either expose a `press_count` field on `ActionDone` (number
  of distinct bound inputs that fired on this frame) or emit one `ActionDone` per
  firing binding when configured.

## 13. `BeginUIContextManager::inputs_as_bits` crashes with `std::bad_optional_access` on non-UI action IDs
- **Symptom:** `vendor/afterhours/src/plugins/ui/systems.h:260` calls
  `magic_enum::enum_index<InputAction>(static_cast<InputAction>(input.action)).value()`
  without checking `.has_value()`. If a game uses `ui::DefaultAction` for UI and
  adds its own gameplay action IDs to `ProvidesInputMapping`, pressing any
  gameplay key throws `std::bad_optional_access`.
- **Suggested fix:** Guard with `if (auto idx = magic_enum::enum_index<InputAction>(...)) output[*idx] = true;`
  so `InputCollector` can safely carry actions outside the UI enum.
- **Local workaround in `mailboy`:** Unified UI + gameplay actions into a single
  `MailboyAction` enum passed to `UIContext<MailboyAction>`.

## 14. `graphics::begin_frame()` / `end_frame()` do not present to the window or poll events
- **Symptom:** Calling `graphics::begin_frame()` / `graphics::end_frame()` looks
  like a drop-in replacement for `BeginDrawing()` / `EndDrawing()`, but
  `RaylibWindowed` only binds/unbinds an offscreen `RenderTexture2D`. Without a
  manual `begin_drawing()` + Y-flipped `DrawTexturePro` + `end_drawing()` pass,
  nothing renders to the window, `GetFrameTime()` stays `0`, and GLFW never polls
  events.
- **Inverse footgun:** Calling `begin_drawing()` / `end_drawing()` when
  `FLAG_WINDOW_HIDDEN` is set (for offscreen screenshot capture) stalls on macOS
  waiting for display VSync.
- **Suggested fix:** Provide a single `graphics::present()` (or have `end_frame()`
  auto-present when visible and skip swap when hidden/headless) so callers don't
  have to hand-roll the Y-flipped FBO blit and hidden-window special case.

## 15. `plugins/reduced_motion.h` fails to link on macOS without explicit `-lobjc`
- **Symptom:** Including `plugins/reduced_motion.h` causes linker errors
  (`_objc_getClass`, `_objc_msgSend`, `_sel_registerName`) in C++ targets that
  link Raylib via `pkg-config` without `-lobjc` or `-framework AppKit`.
- **Suggested fix:** Document `-lobjc` prominently at the top of the header or
  add `#pragma comment` / Makefile snippet in `PLUGIN_API.md`.

## 16. `plugins/autolayout.h` requires external `fmt/format.h` (not vendored alongside `magic_enum`)
- **Symptom:** `vendor/afterhours/vendor/` bundles `magic_enum`, `stb`, `sokol`,
  etc., so `ah.h` works out of the box, but enabling `plugins/ui.h` fails with
  `fatal error: 'fmt/format.h' file not found` unless the consumer adds Homebrew's
  include path + `-DFMT_HEADER_ONLY` (which also significantly increases
  Emscripten compile time).
- **Suggested fix:** Either vendor `fmt` in `vendor/afterhours/vendor/` or use
  C++23 `<format>` when available.

## 17. Namespace split between `afterhours::ui` and `afterhours::ui::imm`
- **Symptom:** Layout/theme types (`FlexDirection`, `AlignItems`, `Padding`,
  `pixels`, `percent`, `Theme`) live in `afterhours::ui`, while `ComponentConfig`
  and `SliderHandleValueLabelPosition` live in `afterhours::ui::imm`, causing
  immediate compiler errors when writing `ui::ComponentConfig`.
- **Suggested fix:** Re-export `ComponentConfig` and widget option enums into
  `afterhours::ui` with `using` aliases.

---

# afterhours gaps — mailboy (Direct Raylib calls without afterhours wrappers)

Filed while auditing remaining direct `raylib::` calls in `mailboy` to remove
`using namespace raylib;` cleanly.

## 18. No 3D matrix stack or textured box/quad helpers in `drawing_helpers_3d.h`
- **Needed for:** `drawTexBox` (tiled UV boxes for carpet, walls, ceiling,
  cubicle fabric, desk laminate, parcels), `drawPlaqueQuad`, and rotated 3D
  transforms for the cart, CRT monitor, and tumbling mail-bin parcels.
- **Current direct calls:** `rlPushMatrix`, `rlPopMatrix`, `rlTranslatef`,
  `rlRotatef`, `rlBegin(RL_QUADS)`, `rlEnd`, `rlSetTexture`, `rlTexCoord2f`,
  `rlVertex3f`, `rlNormal3f`, `rlColor4ub`.
- **Suggested fix:** Add `push_matrix` / `pop_matrix` / `translate_3d` /
  `rotate_3d` and `draw_textured_cube` / `draw_textured_quad_3d` (with UV tiling)
  to `drawing_helpers_3d.h`.

## 19. No procedural `Image` baking + `SetTextureWrap` helper
- **Needed for:** Generating procedural textures, song labels, and desk name
  plaques at startup without shipping texture files on disk.
- **Current direct calls:** `GenImageColor`, `ImageDrawPixel`,
  `ImageDrawRectangle`, `ImageDrawRectangleLines`, `ImageTextEx`,
  `GetImageColor`, `LoadTextureFromImage`, `UnloadImage`, `SetTextureWrap`.
- **Suggested fix:** Add a small procedural image builder + `set_texture_wrap`
  wrapper in `src/image.h` / `src/graphics.h`.

## 20. Missing 2D rotated/ring/triangle/polygon primitives in `drawing_helpers.h`
- **Needed for:** Curved SongSelect playlist wheel (`DrawRing`, `DrawRingLines`,
  `DrawRectanglePro`, `DrawTextPro`), playhead arrow (`DrawTriangle`), coworker
  star popups (`DrawPoly`), and AudioSync beat ring (`DrawCircleLines`).
- **Suggested fix:** Expose `draw_rectangle_pro`, `draw_text_pro`, `draw_ring`,
  `draw_ring_lines`, `draw_triangle`, `draw_poly`, and `draw_circle_lines` in
  `drawing_helpers.h`.

## 21. No procedural PCM `Wave` synthesis or `LoadSoundAlias` pooling in `plugins/sound_system.h`
- **Needed for:** Mailboy synthesizes every drum, bass, Rhodes, synth, sax,
  bell, and door-whoosh `Wave` procedurally in C++ at startup (zero `.wav` files)
  and pools overlapping voices via `LoadSoundAlias`.
- **Current direct calls:** `InitAudioDevice`, `CloseAudioDevice`,
  `LoadSoundFromWave`, `LoadSoundAlias`, `PlaySound`, `SetSoundVolume`,
  `SetSoundPitch`, `SetMasterVolume`, `UnloadSound`, `UnloadSoundAlias`.
- **Suggested fix:** Add `load_sound_from_pcm(samples, sample_rate, channels)`
  and alias/voice-pool support to `plugins/sound_system.h`.

## 22. No cross-platform `open_url` helper
- **Needed for:** About screen's `choicehoney.itch.io` link (uses Raylib's
  `OpenURL` on desktop and `window.open` via `EM_ASM` on Emscripten).
- **Suggested fix:** Add `afterhours::os::open_url(std::string_view url)`
  (wrapping `OpenURL` on desktop and `window.open` on `__EMSCRIPTEN__`).

## 23. `BeginUIContextManager` stores `last_action` as a single scalar instead of a bitset
- **Symptom:** `vendor/afterhours/src/plugins/ui/systems.h:297` loops over
  `inpc.inputs_pressed()` and overwrites `context.last_action` with the last
  entry. If a physical key (e.g. `ENTER`, `TAB`, `DOWN`) is bound to both a UI
  action (`WidgetPress`, `WidgetNext`, `WidgetDown`) and a higher-numbered game
  action (`Confirm`, `Diff`, `NavDown`), the game action always overwrites
  `context.last_action`, so `ctx.pressed(WidgetPress)` / `WidgetNext` /
  `WidgetDown` silently return `false` and keyboard UI navigation breaks completely.
- **Suggested fix:** Store `pressed_actions` as an `InputBitset` alongside
  `all_actions`, and have `UIContext::pressed(action)` test that bitset instead
  of comparing against a single scalar `last_action`.
- **Local workaround in `mailboy`:** `filterUiInputForScreen()` strips gameplay
  actions (`> MenuBack`) from `InputCollector` on modal screens before
  `uiSystems.run()`.






