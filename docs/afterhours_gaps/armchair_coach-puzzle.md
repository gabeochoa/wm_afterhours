# afterhours gaps

Upstream bugs and friction hit while onboarding `afterhours` (`7424d9f3`) in
Broadcast Operator (`puzzle`).

## Fonts / Headless Raylib

- **Headless SIGSEGV on `draw_text()` with the bundled default font.**
  `detail::load_bundled_default_font()` calls `load_font_from_memory()`, which
  does not call `build_font_atlas()` when `font.texture.id == 0 && font_gl_ready()`
  (unlike `load_font_from_file()`). In headless GL mode, `IsWindowReady()` is
  `false` so `LoadFontFromMemory()` leaves `font.texture.id == 0`, while
  `font_gl_ready()` is `true`. `draw_text_ex()` only guards
  `if (font.texture.id == 0 && !font_gl_ready()) return;`, so it passes a
  textureless font straight into `raylib::DrawTextEx` and segfaults. Workaround
  in game: call `afterhours::set_default_font()` with a `build_font_atlas`-loaded
  font at startup. Fix upstream: either build the atlas in
  `load_font_from_memory()` when `font.texture.id == 0 && font_gl_ready()`, or
  guard `if (font.texture.id == 0) return;` unconditionally in `draw_text_ex()`.
- **Headless Windows GL is still stubbed.** `HeadlessGLWindows::init` returns
  `false`, so Windows headless test runs remain blocked.

## Logging

- **`src/log_file.h` is not thread-safe.** `detail::write_line()` and `open()`
  mutate the `pending()` vector and `FILE *` without a mutex. Games with
  background worker threads that log (e.g. our background camera-feed prefetch
  thread) cannot drop their own mutex-guarded `log_file` wrapper until
  `log_file.h` guards its state with a `std::mutex`.

## Core / ECS

- **Default `AFTER_HOURS_MAX_COMPONENTS` (128) is easy to exhaust with imm UI + E2E.**
  New UI components (`HasAccessibility`, `FocusProxy`, `IsDisabled`,
  `FocusScope`, `HasShortcuts`, `HasHitExpand`, `HasBlend`, `IsSubmittable`)
  pushed this game over 128 and crashed with SIGBUS in `LevelEditorUIBuildSystem`.
  Also, the overflow log in `src/core/base_component.h:26` uses printf `%zu`
  instead of `{}` so the error prints literal `(max: %zu)`. Raising the default
  to 256 and fixing the format string would prevent silent OOB crashes.

## Geometry

- **`afterhours::rect::Sides` order & free-function API.** `Sides` is
  `{top, left, bottom, right}` instead of CSS `{top, right, bottom, left}`, and
  the API is free functions rather than a fluent `Rect` wrapper, making
  migration from a fluent `src/rect.h` high-churn and error-prone on positional
  initializers.

## Direct `raylib::` reach-ins (missing `afterhours` wrappers)

Audited all `raylib::` calls in `src/` against `afterhours` `7424d9f3` (excluding
items already wrapped like `gen_texture_mipmaps`, `toggle_fullscreen`,
`measure_text`, `get_key_pressed`, `audio_helpers.h`, `core/math3d.h`, and
`image.h`). Remaining genuine wrapper gaps:

- **Borderless-windowed mode** (`src/game_loop.cpp:170-175`):
  `graphics::toggle_fullscreen()` and `graphics::is_window_fullscreen()` exist,
  but there is no `graphics::toggle_borderless_windowed()` or backend-agnostic
  `FLAG_BORDERLESS_WINDOWED_MODE` constant, so window-mode switching calls
  `raylib::IsWindowState(raylib::FLAG_BORDERLESS_WINDOWED_MODE)` and
  `raylib::ToggleBorderlessWindowed()` directly.
- **Trace-log level enum** (`src/game_loop.cpp:429`):
  `graphics::set_trace_log_level(int)` is wrapped, but callers still pass
  `raylib::LOG_WARNING` because there is no backend-agnostic `TraceLogLevel`
  enum/constant.
- **Custom separate-alpha blend modes** (`src/systems/feed_capture.cpp:70-72, 247, 425-434`):
  `blend::Mode` covers `Alpha`, `Additive`, `Multiplied`, and
  `AlphaPremultiply`, but has no separate RGB/alpha blend-factor API (needed to
  composite onto offscreen RTs without punching alpha holes or to multiply RGB
  while preserving destination alpha). Callers reach into
  `raylib::rlSetBlendFactorsSeparate` + `BeginBlendMode(BLEND_CUSTOM_SEPARATE)`.
- **Shader `int` uniforms & uniform-type constants** (`src/accessibility.h:70-73`,
  `src/systems/feed_capture.cpp:114, 124, 394, 407`):
  `effects::Effect::set(...)` handles `float`, `vec2`, `vec3`, and `Color`, but
  not `int`, forcing callers to use `graphics::set_shader_value(..., raylib::SHADER_UNIFORM_INT)`.
  There are also no backend-agnostic `ShaderUniformType` constants.
- **GPU `TextureType` readback to CPU `Image`** (`src/gif_export.cpp:134, 197`,
  `src/systems/node_feed.cpp:825`, `src/e2e/e2e_commands.h:409`):
  `capture_render_texture_rgba(rt)` only reads a `RenderTextureType`, not an
  arbitrary `TextureType` (needed for pixel inspection and GIF export from
  cached feed textures), so callers still use `raylib::LoadImageFromTexture()`.
- **CPU `Image` drawing primitives** (`src/systems/systems_internal.h:450-494`,
  `src/systems/intro_systems.cpp:175`):
  `src/image.h` covers decode/generate/upload (`load_image`, `make_image`,
  `load_texture_from_rgba`), but has no CPU-side `draw_rectangle` or
  `draw_text` into an `Image` for procedural test-card textures and baked title
  masks, so callers use `raylib::ImageDrawRectangle` and `raylib::ImageDrawTextEx`.
