# Raylib usage audit

Direct `raylib::` uses in Gabe's afterhours projects, mapped to what afterhours offers on both the raylib and sokol (Metal on macOS, Vulkan on Windows) backends.
Data collected 2026-11-16: 8,196 uses of 514 raylib symbols across 16 projects (vendor/, output/ and worktrees excluded).
afterhours checked at `eab28aa` (2026-11-15). HAS = backend-neutral API on raylib and sokol; RAYLIB-ONLY = wrapper exists but sokol is a stub or no-op; MISSING = no afterhours API.
Three of the 16 are not independent games and are counted but flagged: `afterhours-template` and `template` (starter templates), and `armchair_coach-navi` (a second checkout of the armchair_coach repo on branch `navi-work`; its raylib usage is identical to `template`'s). Counts in parentheses below exclude those three.

## Priorities

Ranked by projects affected; ties broken by uses.

1. **`Vector4Type`, public `MatrixType`, short `Camera2D` alias** — 12 projects (9), 126 uses. Replaces `Vector4`, `Matrix`, `Quaternion`, `Camera2D`. Difficulty: trivial; plain structs, and the raylib backend aliases the raylib types as it already does for `Vector2Type`.
2. **Real audio on sokol, plus the missing wrappers** — 11 projects (10), 169 uses. Add `is_audio_device_ready`, `set_master_volume`, `stop_sound`, `is_music_playing`, stream buffer size, pan/pitch, and back `audio_helpers.h` with a real implementation instead of the non-raylib stubs. Replaces `IsAudioDeviceReady`, `InitAudioDevice`, `SetMasterVolume`, `SetAudioStreamBufferSizeDefault`, `PlaySound`, `StopSound`, `*MusicStream`, `Sound`/`Music`/`Wave`. Difficulty: moderate; vendor miniaudio (what raylib's raudio already uses), which covers CoreAudio and WASAPI and doesn't depend on Metal or Vulkan.
3. **One-line holes** — 11 projects (10), 56 uses. `draw_fps`, `get_key_pressed`, `unload_font`, `LOG_WARNING`/`LOG_INFO` constants, `keys::NULL_KEY` (and accepting `KEY_DELETE` as `DELETE_KEY`), trilinear filter and texture wrap. Replaces `DrawFPS`, `GetKeyPressed`, `UnloadFont`, `LOG_*`, `TraceLogLevel`, `KEY_NULL`, `SetTextureWrap`, `TEXTURE_WRAP_CLAMP`, `TEXTURE_FILTER_TRILINEAR`. Difficulty: trivial; sokol already tracks key events and builds samplers per filter.
4. **Cross-backend custom shaders** — 10 projects (7), 795 uses. Load arbitrary shaders on sokol, uniform setters for vec3/int/sampler/matrix/arrays. Replaces `LoadShader`, `GetShaderLocation`, `SetShaderValue(V/Texture/Matrix)`, `Begin/EndShaderMode`, `SHADER_UNIFORM_*`. Difficulty: hard; sokol needs each shader compiled per backend (MSL, SPIR-V) with sokol-shdc, uniforms live in blocks rather than per-name locations, and every project's GLSL 330 `.fs` must be ported. todo.md already plans "shaders as files" via sokol-shdc.
5. **Gamepads on sokol** — 10 projects (9), 238 uses. Implement the `input::` gamepad functions that currently return false/0 under sokol. Replaces `IsGamepad*`, `GetGamepadAxisMovement`, `SetGamepadMappings`, and makes the 194 `GAMEPAD_*` constant uses (bindings) do something on sokol. Difficulty: moderate; sokol_app has no gamepad API, so this needs GameController.framework on macOS and XInput on Windows.
6. **Runtime window control and display metrics** — 10 projects (7), 48 uses. Make `set_window_size`/`set_window_min_size` work on sokol; add `get_render_size`, `get_dpi_scale`, monitor size/position, window position, borderless toggle. Replaces `SetWindowSize`, `SetWindowMinSize`, `GetRenderWidth/Height`, `GetWindowScaleDPI`, `GetMonitor*`, `Set/GetWindowPosition`, `ToggleBorderlessWindowed`, `IsWindowState`. `window_manager` already calls the raylib versions internally. Difficulty: moderate; sokol_app has no runtime resize, so this needs NSWindow and Win32 calls via `sapp_macos_get_window`/`sapp_win32_get_hwnd`.
7. **CPU image API** — 8 projects (8), 446 uses. An `Image` (RGBA buffer) with load/gen/get-pixel/draw-rect/flip/export, `load_texture_from_image` and `update_texture` on both backends, and `load_image_from_render_texture` built on the existing readback. Replaces `Image`, `LoadImage`, `UnloadImage`, `GenImageColor`, `GetImageColor`, `ImageFlipVertical`, `ExportImage(ToMemory)`, `LoadImageColors`, `LoadTextureFromImage`, `LoadImageFromTexture`, `UpdateTexture`, `PIXELFORMAT_*`, `MemFree`. Difficulty: moderate; stb_image and stb_image_write are vendored and readback already works on raylib, Metal and Vulkan, so the work is plumbing plus reconciling GL vs Metal/Vulkan row order.
8. **Raylib-named color palette** — 8 projects (7), 143 uses. `colors::YELLOW`, `SKYBLUE`, `RAYWHITE`, `GOLD`, `GRAY`, `DARKGRAY`, `ORANGE`, `MAGENTA`, etc. with raylib's exact values on every backend. Also fix `colors::UI_RED/UI_GREEN/UI_BLUE`, which are raylib values on raylib and pure 255 primaries on sokol. Difficulty: trivial; constexpr constants.
9. **Public vector/matrix math** — 7 projects (5), 329 uses. `normalize`, `dot`, `cross`, `length`, matrix multiply/translate/scale/rotate, point-in-rect. Replaces `Vector3*`, `Matrix*`, `QuaternionToMatrix`, `CheckCollisionPointRec/CircleRec/Boxes`. Private copies already exist in `frustum.h` and `house_of_cards.h`. Difficulty: trivial; pure math, no backend.
10. **Matrix stack and immediate-mode geometry** — 5 projects (5), 171 uses. `push_matrix`/`translate`/`rotate`/`scale`, and a begin/vertex/color/texcoord/texture API. Replaces the `rlPushMatrix`… `rlSetTexture` subset and `rlDrawRenderBatchActive`. Difficulty: moderate; sokol_gl mirrors rlgl's model, but textured and depth-tested 3D need sgl pipelines and view/sampler handling.
11. **Cursor control** — 3 projects (3), 12 uses. `show/hide_cursor`, `lock_cursor`, `set_mouse_position`. Difficulty: trivial for show/lock (`sapp_show_mouse`, `sapp_lock_mouse`), moderate for warping the cursor (per-platform).
12. **Meshes, materials, models and raycasts on sokol** — 3 projects (3), 519 uses (cartographer, mailboy, pharmasea). Replaces `Model`, `Mesh`, `Material`, `GenMesh*`, `DrawMesh*`, `LoadModel*`, `Ray*`, `BoundingBox`, `DrawTriangle3D`, `DrawBillboard`. `plugins/models.h` is raylib-only today. Difficulty: hard; needs a mesh/material pipeline on sokol and depends on item 4 for lit shaders.
13. **Custom blend factors** — 2 projects (2), 35 uses. Replaces `BLEND_CUSTOM(_SEPARATE)`, `rlSetBlendFactors(Separate)`, `rlSetBlendMode`. Difficulty: moderate; sokol bakes blend state into pipeline objects, so this needs a pipeline cache keyed by factors.

Not afterhours work: `FileExists`, `LoadFileData`, `Compress/DecompressData`, clipboard (std::filesystem, `plugins/files.h`, `plugins/clipboard.h` already cover them), the rlgl framebuffer/depth/cull calls in cartographer's shadow pass, and custom font-atlas building (3 projects, 7 uses).

## By area

### Math / value types

| Symbol(s) | Projects | Uses | afterhours status | Equivalent / note |
|---|---|---|---|---|
| Color | 15 | 888 | HAS | `afterhours::Color` (= `raylib::Color` on raylib, `ColorType` elsewhere). Spelling change only. |
| Vector2 | 14 | 483 | HAS | `Vector2Type`. Same type on raylib. Sokol `MyVec2` has `+ - == <` only; raymath operators/functions do not carry over. |
| Rectangle | 13 | 311 | HAS | `RectangleType`. |
| Vector3 | 12 | 29 | HAS | `Vector3Type`. |
| Vector4 | 12 | 18 | MISSING | No `Vector4Type`. Mostly `typedef raylib::Vector4 vec4` copied from the template `rl.h`, used for vec4 uniforms. |
| Matrix, Quaternion | 2 | 94 | RAYLIB-ONLY | `MatrixType` exists only inside `plugins/models.h` (raylib) / stub elsewhere. No quaternion. |

### Math / collision helpers

| Symbol(s) | Projects | Uses | afterhours status | Equivalent / note |
|---|---|---|---|---|
| Vector3Normalize, Vector3DotProduct, Vector3CrossProduct, Vector3Length | 6 | 36 | MISSING | Private copies exist in `frustum.h` (`detail_frustum`) and `house_of_cards.h`; nothing public. |
| CheckCollisionRecs | 3 | 3 | HAS | `check_collision_recs` (drawing_helpers, all backends). |
| Matrix*, other Vector3*/Quaternion* | 2 | 267 | MISSING | Only `distance`/`lerp` for `Vector2Type` in `polyline.h`. Matrix math is cartographer + mailboy. |
| CheckCollisionPointRec, CheckCollisionCircleRec, CheckCollisionBoxes, GetRandomValue, Fade (5 symbols) | 5 | 28 | MISSING (collision); HAS (Fade, GetRandomValue) | No point-in-rect, circle-rect or box-box test (`get_collision_rec`, `check_collision_point_line` exist). Fade -> `colors::opacity_pct`; GetRandomValue -> `random_engine.h`. |

### Color constants

| Symbol(s) | Projects | Uses | afterhours status | Equivalent / note |
|---|---|---|---|---|
| WHITE, BLACK, BLANK | 13 | 229 | HAS | `colors::white()`, `colors::black()`, `colors::transparent()`. Values match raylib. |
| RED, GREEN, BLUE | 7 | 49 | HAS (value differs) | `colors::UI_RED/UI_GREEN/UI_BLUE` are raylib values on raylib but pure 255 primaries on sokol, so the same name draws different colors per backend. |
| YELLOW, SKYBLUE, RAYWHITE, GOLD, GRAY, ORANGE, DARKGRAY, MAGENTA, PINK, BROWN, LIME, LIGHTGRAY, PURPLE | 8 | 94 | MISSING | No raylib-palette names. `colors::UI_WHITE` is RAYWHITE on raylib only; `colors::gray(v)` is not DARKGRAY/LIGHTGRAY. |

### Input constants

| Symbol(s) | Projects | Uses | afterhours status | Equivalent / note |
|---|---|---|---|---|
| KEY_* (75 symbols) | 14 | 603 | HAS | `afterhours::keys::*` (`core/key_codes.h`), same integer values. Renames: `KEY_DELETE` -> `keys::DELETE_KEY`; `KEY_NULL` has no name (use 0). |
| GAMEPAD_BUTTON_*, GAMEPAD_AXIS_* | 10 | 194 | HAS (constants) | `gamepad_buttons::*`, `gamepad_axes::*`. The constants compile everywhere; gamepad input itself is raylib-only (see Input queries). |
| MOUSE_BUTTON_*, MOUSE_LEFT_BUTTON | 7 | 53 | HAS | `mouse_buttons::LEFT/RIGHT/MIDDLE`. |
| GamepadButton, GamepadAxis, MouseButton, KeyboardKey | 5 | 51 | HAS | `input::GamepadButton`, `input::GamepadAxis`, `input::MouseButton` (int); KeyboardKey -> int. |

### Input queries

| Symbol(s) | Projects | Uses | afterhours status | Equivalent / note |
|---|---|---|---|---|
| IsKeyPressed, IsKeyDown | 11 | 45 | HAS | `graphics::is_key_pressed/is_key_down` or `input::is_key_pressed` (latter honors e2e injection). Several projects wrap raylib in their own `input_wrapper.h`. |
| IsMouseButtonPressed, IsMouseButtonDown, GetMousePosition, GetMouseDelta, GetMouseWheelMove | 5 | 17 | HAS | `input::` / `graphics::` equivalents; `input::is_mouse_button_pressed` also carries the macOS edge-detection fix raylib lacks. |
| GetCharPressed | 3 | 5 | HAS | `graphics::get_char_pressed`, `input::get_char_pressed`. |
| GetKeyPressed | 3 | 6 | MISSING | No key-queue read. Trivial: sokol backend already tracks key events. |
| IsGamepadAvailable, GetGamepadAxisMovement, IsGamepadButton*, GetGamepadName, SetGamepadMappings | 2 | 8 | RAYLIB-ONLY | `input::is_gamepad_available` etc. exist; the sokol branch returns false/0 ("Gamepad not yet supported on Metal"). Only 2 projects call raylib directly, but every project that binds `GAMEPAD_*` through the input plugin loses gamepads on sokol. |
| HideCursor, ShowCursor, EnableCursor, DisableCursor, IsCursorHidden, SetMousePosition | 3 | 12 | MISSING | Only `set_mouse_cursor(shape)`. sokol has `sapp_show_mouse`/`sapp_lock_mouse`. |
| *_Real variants (7 symbols) | 1 | 9 | n/a | Projects' own e2e pass-through shims, not raylib API. |

### Window / lifecycle

| Symbol(s) | Projects | Uses | afterhours status | Equivalent / note |
|---|---|---|---|---|
| WindowShouldClose, BeginDrawing, EndDrawing, InitWindow, CloseWindow | 14 | 84 | HAS (via run) | `graphics::run(RunConfig)`. The raylib-style wrappers exist but on sokol `init_window` is a no-op and `window_should_close()` is always false, so a hand-written loop never runs there. Only endless-dance-chaos uses `graphics::run`. |
| SetExitKey, SetWindowState, ClearWindowState, SetConfigFlags, FLAG_VSYNC_HINT, FLAG_WINDOW_RESIZABLE | 13 | 72 | HAS | `graphics::set_exit_key`, `set_window_state`, `FLAG_*`. On sokol most are recorded-only or come from `RunConfig`. |
| IsWindowFullscreen, ToggleFullscreen, IsWindowReady | 9 | 47 | HAS | `graphics::` wrappers, real on sokol. |
| GetFrameTime, GetTime, SetTargetFPS | 10 | 59 | HAS | `graphics::get_frame_time/get_time/set_target_fps`. |
| GetScreenWidth, GetScreenHeight | 7 | 70 | HAS | `graphics::get_screen_width/height`. |
| SetWindowSize, SetWindowMinSize | 10 | 12 | RAYLIB-ONLY | Wrappers exist; sokol logs `@notimplemented` (sokol_app has no runtime resize). |
| FLAG_WINDOW_HIGHDPI | 3 | 3 | HAS | `Config.hidpi` / `RunConfig`. |
| GetRenderWidth/Height, GetWindowScaleDPI, GetMonitor*, Set/GetWindowPosition, ToggleBorderlessWindowed, IsWindowState, FLAG_WINDOW_HIDDEN, FLAG_BORDERLESS_WINDOWED_MODE (13 symbols) | 4 | 36 | MISSING | `window_manager` calls these raylib functions internally (raylib branch) but exposes no neutral API; sokol has `sapp_dpi_scale` only. |

### 2D drawing primitives

| Symbol(s) | Projects | Uses | afterhours status | Equivalent / note |
|---|---|---|---|---|
| ClearBackground | 14 | 58 | HAS | `graphics::clear_background`. |
| DrawRectangle, DrawRectangleRec, DrawRectangleV | 11 | 145 | HAS | `draw_rectangle(RectangleType, Color)`. |
| DrawRectangleLines, DrawRectangleLinesEx | 8 | 57 | HAS | `draw_rectangle_outline`. |
| DrawRectangleRounded (+Lines, +LinesEx) | 8 | 91 | HAS | `draw_rectangle_rounded`, `draw_rectangle_rounded_lines(_ex)`. `draw_rectangle_rounded_rotated` is raylib/none only. |
| DrawRectanglePro | 4 | 14 | HAS | `push_rotation` + `draw_rectangle`. |
| DrawRectangleGradientV/H | 4 | 21 | HAS | `draw_rectangle_gradient_v/h/ex`. |
| DrawLine, DrawLineEx, DrawSplineSegmentLinear, DrawSplineLinear | 10 | 215 | HAS | `draw_line`, `draw_line_ex` (thick lines are quads on sokol), `draw_line_strip`. |
| DrawCircle, DrawCircleV, DrawCircleLines(V), DrawCircleSector, DrawCircleGradient, DrawRing, DrawEllipse, DrawPolyLines | 9 | 119 | HAS | Same-named snake_case helpers on all backends. |
| DrawTriangle, DrawTriangleLines | 5 | 28 | HAS | `draw_triangle`, `draw_triangle_lines`. |
| BeginScissorMode, EndScissorMode | 4 | 15 | HAS | `begin_scissor_mode`/`end_scissor_mode`. |
| Camera2D, BeginMode2D, EndMode2D | 5 | 27 | HAS | `graphics::begin_mode_2d/end_mode_2d`, `get_screen_to_world_2d`. Type is only reachable as `graphics::PlatformAPI::Camera2D` (no short alias); `plugins/camera.h` is raylib-only. |
| BeginBlendMode, EndBlendMode, BLEND_ALPHA/ADDITIVE/ALPHA_PREMULTIPLY | 3 | 22 | HAS | `set_blend_mode(blend::Mode)` / `blend_scope`. Modes: Alpha, Additive, Multiplied, AlphaPremultiply. |
| BLEND_CUSTOM, BLEND_CUSTOM_SEPARATE | 2 | 8 | MISSING | No custom blend factors (see rlgl rows for rlSetBlendFactors*). |
| DrawTriangleStrip | 1 | 1 | MISSING | No strip helper. |

### Text / fonts

| Symbol(s) | Projects | Uses | afterhours status | Equivalent / note |
|---|---|---|---|---|
| DrawText | 14 | 61 | HAS | `draw_text(text, x, y, size, color)`. |
| DrawTextEx | 8 | 93 | HAS | `draw_text_ex` (+ `draw_text_centered/right_aligned/with_shadow`). |
| MeasureText | 10 | 29 | HAS | `graphics::measure_text(text, size)`. |
| MeasureTextEx | 7 | 35 | HAS | `measure_text(font, text, size, spacing)`. |
| Font | 9 | 91 | HAS | `afterhours::Font` (= `raylib::Font` on raylib, fontstash id on sokol). |
| GetFontDefault, LoadFont, LoadFontEx | 6 | 33 | HAS | `get_default_font`, `load_font_from_file`, `load_font_for_string`. |
| UnloadFont | 5 | 13 | MISSING | No `unload_font`. Trivial. |
| DrawFPS | 7 | 7 | MISSING | `graphics::get_fps` exists, no `draw_fps`. Trivial. |
| LoadCodepoints, GlyphInfo, LoadFontData, GenImageFontAtlas, GetGlyphIndex, FONT_DEFAULT (7 symbols) | 3 | 7 | MISSING | Custom atlas building (armchair_coach, kart, pharmasea). `font_has_glyph`/`set_font_atlas_config` cover part. |

### Textures / images / render textures

| Symbol(s) | Projects | Uses | afterhours status | Equivalent / note |
|---|---|---|---|---|
| Texture2D, Texture | 13 | 199 | HAS | `TextureType`. Sokol struct has width/height/ids only (no `.id`, `.mipmaps`). |
| LoadTexture, UnloadTexture | 9 | 78 | HAS | `load_texture`, `unload_texture`. |
| DrawTexturePro, DrawTextureRec, DrawTextureEx, DrawTexture | 13 | 88 | HAS | `draw_texture_pro`, `draw_texture_rec`, `draw_texture_npatch`. |
| SetTextureFilter, TEXTURE_FILTER_POINT/BILINEAR | 9 | 101 | HAS | `set_texture_filter`, `set_render_texture_filter`, `graphics::TEXTURE_FILTER_*`. |
| TEXTURE_FILTER_TRILINEAR, SetTextureWrap, TEXTURE_WRAP_CLAMP | 2 | 10 | MISSING | Only point/bilinear; no wrap control. |
| GenTextureMipmaps | 3 | 5 | HAS | `gen_texture_mipmaps`. |
| RenderTexture2D, LoadRenderTexture, UnloadRenderTexture, BeginTextureMode, EndTextureMode | 8 | 273 | HAS | `graphics::RenderTextureType`, `load_render_texture`, `begin_texture_mode`, `draw_render_texture`. Code that reads `rt.texture` must switch to `draw_render_texture`/`capture_render_texture` (no `.texture` on sokol). |
| Image, LoadImage, UnloadImage, GenImageColor, GetImageColor, ImageFlipVertical, ExportImage(ToMemory), LoadImageColors/UnloadImageColors, Image* edits | 8 | 376 | MISSING | No CPU image type. Pieces exist: `stb_image`/`stb_image_write` vendored, `capture_screen_to_memory`. |
| LoadTextureFromImage, UpdateTexture(Rec) | 7 | 26 | MISSING (sokol has half) | `load_texture_from_pixels(rgba,w,h)` exists on sokol only, not raylib; no update-in-place. |
| LoadImageFromTexture, LoadImageFromScreen | 6 | 29 | MISSING (readback exists) | `capture_render_texture`/`capture_screen_to_memory` read back on raylib, Metal and Vulkan but return PNG/file, not an image object. |

### Shaders

| Symbol(s) | Projects | Uses | afterhours status | Equivalent / note |
|---|---|---|---|---|
| Shader, LoadShader, UnloadShader | 10 | 128 | RAYLIB-ONLY | `graphics::ShaderType`, `graphics::load_shader`. Sokol accepts only `blur.fs` and `ps1_post.fs` and fails closed on anything else. |
| GetShaderLocation, SetShaderValue | 10 | 396 | RAYLIB-ONLY | `graphics::get_shader_location/set_shader_value`. Sokol knows two uniform names (`resolution`, `direction`) and only VEC2 values. |
| BeginShaderMode, EndShaderMode | 10 | 55 | RAYLIB-ONLY | `graphics::begin_shader_mode/end_shader_mode`. |
| SHADER_UNIFORM_VEC2/FLOAT/VEC4 | 9 | 114 | RAYLIB-ONLY | `graphics::SHADER_UNIFORM_*`. |
| SHADER_UNIFORM_VEC3/INT, SetShaderValueTexture, SetShaderValueV, SetShaderValueMatrix | 5 | 83 | MISSING | No constants or setters for these. |
| SHADER_LOC_*, GetShaderLocationAttrib (8 symbols) | 1 | 19 | MISSING | Model-shader plumbing (cartographer). |

### 3D / models / cameras

| Symbol(s) | Projects | Uses | afterhours status | Equivalent / note |
|---|---|---|---|---|
| Camera3D, BeginMode3D, EndMode3D, CAMERA_PERSPECTIVE/ORTHOGRAPHIC | 3 | 43 | HAS | `afterhours::Camera3D`, `begin_3d`/`end_3d`, `camera::PERSPECTIVE`. |
| DrawCube(Wires), DrawSphere, DrawPlane, DrawLine3D, DrawCylinderEx/Wires, GetWorldToScreenEx | 3 | 64 | HAS | `draw_cube`, `draw_sphere`, `draw_plane`, `draw_line_3d`, `draw_cylinder(_wires)`, `get_world_to_screen`. |
| Model, LoadModel, UnloadModel, DrawModel(Ex), GetModelBoundingBox, DrawMeshInstanced | 2 | 124 | RAYLIB-ONLY | `plugins/models.h` (`load_model`, `draw_model`, `InstanceBatcher`, `model_bounds`) is raylib; other backends are stubs. |
| Mesh, Material, MATERIAL_MAP_*, GenMesh*, UploadMesh, LoadModelFromMesh, LoadMaterialDefault, DrawMesh | 2 | 186 | MISSING | No mesh/material API. |
| Ray, RayCollision, GetRayCollision*, GetScreenToWorldRayEx, BoundingBox | 2 | 100 | MISSING | todo.md plans `raycast_model`; `frustum.h` has its own planes. |
| DrawBillboard(Rec) | 2 | 8 | MISSING | house_of_cards draws cards with raw rlgl (raylib only). |
| DrawTriangle3D, DrawText3D, animations, UpdateCamera, etc. (13 symbols) | 3 | 104 | MISSING | Mostly cartographer (DrawTriangle3D 78 uses) and pharmasea (example-derived 3D text). |

### rlgl (low-level GL)

| Symbol(s) | Projects | Uses | afterhours status | Equivalent / note |
|---|---|---|---|---|
| rlPushMatrix, rlPopMatrix, rlTranslatef, rlRotatef, rlScalef | 3 | 57 | MISSING (partial) | Only `push_rotation`/`pop_rotation`. sokol_gl has the same matrix stack. |
| rlBegin, rlEnd, rlVertex2f/3f, rlColor4ub, rlTexCoord2f, rlSetTexture | 3 | 82 | MISSING | No immediate-mode vertex API. `draw_quad`, `draw_texture_quad` cover fixed shapes. |
| rlDrawRenderBatchActive | 2 | 18 | MISSING | Batch flush; usually a workaround around raylib state changes. |
| rlSetBlendMode, rlSetBlendFactors(Separate), RL_BLEND_* | 2 | 27 | MISSING | Custom blend factors. |
| framebuffers, depth/cull state, matrices, vertex buffers (31 symbols) | 2 | 72 | MISSING | cartographer shadow pass, mailboy, wm_afterhours. Not worth wrapping one-to-one. |

### Audio

| Symbol(s) | Projects | Uses | afterhours status | Equivalent / note |
|---|---|---|---|---|
| InitAudioDevice, CloseAudioDevice | 9 | 18 | RAYLIB-ONLY | `afterhours::InitAudioDevice` etc. in `plugins/audio_helpers.h`; non-raylib backends are no-op stubs. |
| Sound, Music, Wave, LoadSound, PlaySound, IsSoundPlaying, SetSoundVolume, UnloadSound, LoadSoundFromWave | 8 | 82 | RAYLIB-ONLY | `SoundType`, `MusicType`, `WaveType`, same-named wrappers; `sound_system` plugin on top. Silent on sokol. |
| LoadMusicStream, PlayMusicStream, UpdateMusicStream, SetMusicVolume, UnloadMusicStream | 3 | 15 | RAYLIB-ONLY | Same-named wrappers; silent on sokol. |
| IsAudioDeviceReady, SetMasterVolume, SetAudioStreamBufferSizeDefault, StopSound, IsMusicStreamPlaying, SetSoundPan, SetSoundPitch | 11 | 54 | MISSING | No wrapper on any backend (sound_system has an internal master volume). |

### File / system / misc

| Symbol(s) | Projects | Uses | afterhours status | Equivalent / note |
|---|---|---|---|---|
| SetTraceLogLevel, LOG_ERROR | 7 | 19 | HAS | `graphics::set_trace_log_level`, `graphics::LOG_ERROR` (sokol: no-op, uses afterhours logging). |
| LOG_WARNING, LOG_INFO, TraceLogLevel, SetTraceLogCallback | 8 | 10 | MISSING | Only LOG_ERROR is exported; `logging.h` is the real replacement. |
| MemAlloc, MemFree | 3 | 15 | n/a | Pairs with raylib-allocated buffers (ExportImageToMemory etc.); disappears with an image API. |
| FileExists, DirectoryExists, LoadFileData, Compress/DecompressData, GetApplicationDirectory, clipboard, SetLoadFileDataCallback (10 symbols) | 3 | 13 | n/a | std::filesystem, `plugins/files.h`, `plugins/clipboard.h`. Not backend work. |
## Per project

| Project | Uses | Distinct symbols | Heaviest areas |
|---|---|---|---|
| cartographer | 3386 | 378 | math/value types 804; textures/images/render textures 456; 3D/models/cameras 436 |
| wm_afterhours | 881 | 152 | textures/images/render textures 260; 2D drawing 254; math/value types 124 |
| mailboy | 606 | 148 | math/value types 130; input constants 120; 3D/models/cameras 114 |
| pharmasea | 586 | 204 | input constants 122; 3D/models/cameras 79; textures/images/render textures 78 |
| MyNameChef | 584 | 131 | math/value types 151; color constants 79; shaders 71 |
| kart-afterhours | 553 | 137 | shaders 116; math/value types 102; textures/images/render textures 70 |
| prime_pressure | 517 | 91 | math/value types 244; text/fonts 82; input constants 68 |
| afterhours-template (template) | 298 | 102 | shaders 85; audio 37; window/lifecycle 34 |
| armchair_coach | 273 | 95 | math/value types 81; textures/images/render textures 80; input constants 22 |
| break-ross | 248 | 107 | math/value types 48; textures/images/render textures 41; input constants 31 |
| webhelper | 70 | 38 | 2D drawing 24; math/value types 18; input constants 12 |
| template (template, no git remote) | 64 | 47 | input constants 21; math/value types 9; window/lifecycle 9 |
| armchair_coach-navi (copy: armchair_coach `navi-work` checkout) | 64 | 47 | input constants 21; math/value types 9; window/lifecycle 9 |
| tetr-afterhours | 33 | 22 | input constants 10; window/lifecycle 7; math/value types 6 |
| endless-dance-chaos | 25 | 10 | textures/images/render textures 13; rlgl 9; math/value types 3 |
| voice_ah | 8 | 8 | audio 8 |

## Easy wins

5,261 of the 8,196 uses (64%) already have a backend-neutral afterhours API. On the raylib backend most of these are spelling-only changes, because the afterhours types alias the raylib types (`afterhours::Color` is `raylib::Color`, `Vector2Type` is `raylib::Vector2`, `keys::*` have raylib's values).

| Area | Uses | Switch to |
|---|---|---|
| Value types (`Color`, `Vector2`, `Vector3`, `Rectangle`, `Texture2D`, `RenderTexture2D`, `Font`, `Camera2D`, input enum types) | 2,156 | `Color`, `Vector2Type`, `Vector3Type`, `RectangleType`, `TextureType`, `graphics::RenderTextureType`, `afterhours::Font`, `int` |
| Input constants (`KEY_*`, `GAMEPAD_*`, `MOUSE_*`) | 844 | `keys::`, `gamepad_buttons::`, `gamepad_axes::`, `mouse_buttons::` |
| 2D drawing | 798 | `draw_rectangle*`, `draw_line*`, `draw_circle*`, scissor, `set_blend_mode`, `graphics::begin_mode_2d` |
| Textures and render textures | 455 | `load_texture`, `draw_texture_pro/rec`, `set_texture_filter`, `load_render_texture`, `begin_texture_mode`, `draw_render_texture` |
| Text | 251 | `draw_text`, `draw_text_ex`, `measure_text`, `get_default_font`, `load_font_from_file` |
| Window settings and timing | 248 | `graphics::set_window_state`, `toggle_fullscreen`, `get_frame_time`, `get_screen_width`, ... |
| Colors `WHITE`, `BLACK`, `BLANK` | 229 | `colors::white()`, `black()`, `transparent()` |
| Basic 3D (camera, cubes, spheres, lines) | 107 | `Camera3D`, `begin_3d`, `draw_cube`, `draw_line_3d`, `get_world_to_screen` |
| Main loop (`InitWindow`, `WindowShouldClose`, `Begin/EndDrawing`, `CloseWindow`) | 84 | `graphics::run(RunConfig)` |
| Input queries | 67 | `input::is_key_pressed`, `input::is_mouse_button_pressed`, ... |
| Misc (`CheckCollisionRecs`, `SetTraceLogLevel`, `LOG_ERROR`) | 22 | `check_collision_recs`, `graphics::set_trace_log_level` |

By project: cartographer 1,752 of 3,386; wm_afterhours 784/881; prime_pressure 497/517; MyNameChef 436/584; mailboy 380/606; pharmasea 356/586; kart-afterhours 346/553; break-ross 198/248; afterhours-template 156/298; armchair_coach 152/273; webhelper 70/70; template 50/64; armchair_coach-navi 50/64; tetr-afterhours 31/33; endless-dance-chaos 3/25; voice_ah 0/8.

What the spot checks found:

- **Mostly habit, inherited from the template.** `template/src/rl.h` wraps raylib, raymath and rlgl in `namespace raylib` and typedefs `vec4` to `raylib::Vector4`; `game_loop.cpp` runs a hand-written `while (!raylib::WindowShouldClose())` loop. That pattern shows up in every project. Fixing both templates first stops new projects starting out on raylib.
- **The main loop decides whether sokol works at all.** 14 of 15 projects run their own raylib loop; only endless-dance-chaos calls `graphics::run`. On sokol, `init_window` is a no-op and `window_should_close()` always returns false, so those 84 uses are not just a rename: each loop body has to move into a `RunConfig` callback.
- **Render textures: habit.** wm_afterhours (`systems/UpdateRenderTexture.h`, `game.cpp`) calls `raylib::LoadRenderTexture`/`UnloadRenderTexture`, though `load_render_texture`/`unload_render_texture` have the same shape. Code that touches `rt.texture` (e.g. the template's `SetTextureFilter(rt.texture, ...)` and `DrawTextureRec(rt.texture, ...)`) must switch to `set_render_texture_filter`/`draw_render_texture`, because the sokol `RenderTextureType` has no `.texture`.
- **Input: habit.** prime_pressure and break-ross wrap `raylib::IsKeyPressed` in their own `input_wrapper.h`; mailboy and break-ross test `raylib::KEY_ESCAPE` directly. `input::is_key_pressed` exists and also honors e2e input injection.
- **Shaders: no project uses the afterhours wrappers.** kart-afterhours (`library/shader_library.h`, `systems/systems.h`), MyNameChef, afterhours-template, cartographer and pharmasea all call raylib. Switching gains nothing on sokol today: sokol accepts only `blur.fs` and `ps1_post.fs`, and the wrappers have no VEC3, INT, sampler or matrix uniforms. The exception is the template's PS1 post pass, which uses `ps1_post.fs` and one VEC2 uniform and would work through `graphics::load_shader` now. Not counted as an easy win.
- **Audio: a real gap.** MyNameChef and break-ross call `raylib::IsAudioDeviceReady` in `preload.cpp` because afterhours has no wrapper for it, and the existing wrappers are silent on sokol.
- **Types to watch when targeting sokol:** sokol `Vector2Type` has only `+ - == <` (no raymath helpers), `TextureType` has no `.id` or `.mipmaps`, gamepad constants compile but read nothing, and `colors::UI_RED/GREEN/BLUE` change value between backends, so those three colors aren't listed as easy wins.
