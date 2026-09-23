# Animation in afterhours

Everything lives in `afterhours::motion` (`vendor/afterhours/src/plugins/animation/`). `afterhours::animation` keeps `set_instant`, `is_instant` and `register_update_systems` as forwarders.

## Core

**Spring** — closed-form damped oscillator. `Spring{.response = 0.3f, .bounce = 0.f}`; `response` is roughly the perceived duration in seconds, `bounce` in `[0, 1)` is overshoot. Presets: `Spring::smooth()`, `snappy()`, `bouncy()`, `gentle()`. `Spring::from_freq_decay(freq, decay)` maps the old UI spring numbers. Frame rate never changes where a spring lands; a stalled frame evaluates at the true time. Retargeting mid-flight keeps velocity.

**Timeline** — keys plus a repeat mode plus one optional curve. Output is dimensionless progress that the track lerps between its start and target, so `{{0,0},{0.08,1},{0.16,-1},{0.22,.667},{0.28,0}}` with target 6 is a ±6 px shake. `Repeat::Once | Loop | PingPong`. Curves: `curves::ease_in_quad`, `ease_out_quad`, `ease_in_out_quad`, `ease_out_cubic`. A timed ease is `Timeline{.keys = {{0,0},{0.25f,1}}, .curve = curves::ease_out_quad}`.

**Track<T>** — one value over springs and timelines; `T` is `float`, `Vector2Type`, `RectangleType` or `ColorType` (per-component, colours in sRGB).

```cpp
tr.from(v)                       // set value, clear everything
tr.to(target, mode)              // interrupt: retarget from the current sample, drop the queue
tr.then(target, mode)            // chain
tr.delay(s)                      // on the step just added
tr.repeat()                      // replay the chain forever
tr.essential()                   // keep running under instant (spinners)
tr.on_complete(fn)  tr.on_step(step, fn)  tr.on_change(quantize, fn)   // float only
tr.value()  tr.value_or(d)  tr.target()  tr.active()  tr.started()  tr.elapsed()
```

`instant` (`animation::set_instant(true)`) lands every non-essential track on its chain's last target on the next advance, skipping delays; `on_complete` still fires. `motion::pause(bool)` and `motion::set_time_scale(f)` apply once in the update system.

## Where tracks live

Tracks are stored in `motion::HasTracks` on an entity and die with it.

```cpp
motion::anim(Key::Slide, entity.id)   // Track<float> on that entity
motion::anim<ColorType>(Key::Tint, id)
motion::anim(Key::Score)              // no entity: a hidden permanent root entity
```

Register once per system manager: `afterhours::animation::register_update_systems(systems)` (or `motion::register_update_systems`). UI-collection entities are advanced by the UI plugin's pre-update bridge, before the build, so layout, modifiers and hit-testing read one value per frame. A `to()` issued during a build starts advancing on the next frame.

Extra per-entity tracks that a widget's own code drives should use keys ≥ 100; keys 0–5 are the `MotionProperty` slots and key 0 of the colour map is the background.

## Widgets: trigger blocks

Trigger blocks live in the opt-in `ui_motion` bridge (`plugins/ui_motion.h`, alias `um`); `ui` stays a leaf and apps call `ui_motion::register_bridge<UIContext<InputAction>>()` once at setup.

```cpp
ComponentConfig{}
  .with(um::on_appear({.translate_y = {24.f, 0.f}, .opacity = {0.f, 1.f}}, Spring::gentle(), /*delay*/ 0.1f))
  .with(um::on_hover({.scale = 1.05f, .translate_y = -4.f}))
  .with(um::on_press({.scale = 0.92f}))
  .with(um::on_focus({.scale = 1.03f}))
  .with(um::on_state(is_open, {.scale = {0.97f, 1.f}, .opacity = {0.f, 1.f}}, Timeline{...}))
  .with(um::on_change(stamp, {.scale = {1.2f, 1.f}}, Spring::bouncy()))
```

Properties: `scale`, `translate_x`, `translate_y`, `rotation`, `opacity`, `corner_radius`, `blur` (px of region blur, capped at 8), `background` (a `ColorType`; replaces the fill and opts out of the theme hover fill). A single value means "target"; `{from, to}` supplies the other end for `on_appear`, `on_state` (false → `from`) and `on_change`.

Each frame the active triggers collapse into one target per property — press > hover > focus > state/rest, with `on_change` firing a one-frame impulse on top when its stamp differs from last frame — and each track is retargeted once. Falling back to rest reuses the last winning spring, so hover-out is a reversal, not a fresh animation. `on_appear` pins the rest value to its `to`; give hidden-at-rest elements an `on_appear` so their first frame starts where the state's `from` would put them.

`with_origin(x, y)` pins scale to a corner or edge (0..1 rect space). Rotation pivots on the centre. A parent's scale and translate compose to its children through `detail::apply_ancestor_transform`, which render, hit-testing, clip rects, focus rings and `click_ui` all share; text size does not scale.

## Patterns that work

- **Keep emitting.** Closed menus, tooltips, toasts and pages stay in the tree with `on_state(open, …)` driving opacity to 0; add `with_ignore_pointer_events()` and `with_skip_tabbing(true)` while closed. Nothing needs to be retained after a caller stops emitting.
- **Layout follows a track** when you feed it into `with_size()`; feed it into the modifier (`on_state({.scale…})`) for visual-only motion.
- **Absolute elements:** `with_absolute_position(x, y)` *is* their translate. To move an absolutely placed element, use a motion translate or offset the position from a track.
- **Buttons hosting absolutely placed children** need `.with_padding(Padding::all(pixels(0)))`; theme padding offsets the children otherwise.
- **Replay an entrance** by removing `HasMotionState` and `motion::HasTracks` from the entity; `on_appear` fires again next frame.
- **Timers:** a hold is a timeline with `on_complete` — `hold.from(0).to(1, Timeline{{0,0},{3,1}}).on_complete([this]{ shown = false; })`.
- **Modals:** `ModalConfig{}.with_motion(enter, exit)` gives `Hidden → Entering → Visible → Exiting → Hidden`; the result stays true while exiting so you keep emitting the body, input is blocked until it settles, focus is restored once, and reopening mid-exit reverses. Without `with_motion` nothing changes.

## Effects

- **`presets::effect_presets`** lists 17 effects beyond the basic set (blur, unblur, dissolve, wipe, curtain, sweep, shear, stretch, iris, spotlight, swing, recede, unroll, blinds, flip, emerge, tumble). `effect_timeline`, `effect_blur_radius` and `effect_quad` drive one progress value through `effect_presets.fs` shader modes, region blur and `draw_quad` corners; the Transitions Lab `Effect presets` group shows each one.
- **`with_shader(effect.shader)`** wraps one widget's draws (fill, border, label) in a shader scope; the batched renderer emits `ShaderStart`/`ShaderEnd` around its commands. `effects::Effect::load("name")` reads `resources/shaders/name.fs`; `set(uniform, float|Vector2Type|ColorType)` each frame, `Effect::Scope` for custom draws, `effects::reload_all()` re-reads from disk. Shaders that need widget space take `origin`/`extent` uniforms in framebuffer coordinates (y up) and work from `gl_FragCoord`, since solid shapes carry no useful texcoords.
- **`with_blur(px)` / the `blur` property** queue a region blur; wm's `ApplyBlurPass` runs `effects::BlurPass` over `mainRT` after the UI render. It blurs everything in the rect, so a glow under a button is a shader with a soft rim (see `gradient.fs`), not a blur.
- **`particles::Emitter<N>`** is a fixed pool with gravity, drag, floor and restitution; draw it from `with_on_draw_fg`.
- **`animation_presets.h`** collects `fade_up`, `pop_in`, `hover_lift`, `press_squash`, `slide_in`, `shake`, `spin`, `pulse`.
- **`with_unit_motion(TextUnitMotion)`** animates a label per grapheme or word with a stagger; `text_units_active()` reports flight.
- **`draw_quad` / `draw_texture_quad`** take four corners for projected or sheared shapes (the Lab's 3D tilt).
- The Transitions Lab (`transitions_lab`, scripts 300–346) has one working example per transition in the original web reference; start there before writing a new one.

## Reduced motion

`Settings::set_reduced_motion_enabled(bool)` persists `reduced_motion_enabled` and drives `animation::set_instant`. Mark loops that must keep moving (spinners) `.essential()`.

## Testing

Headless dt is fixed (`1/target_fps × time_scale`) and springs are closed-form, so `wait_frames N` steps deterministically. Assert "in flight" immediately after the input — at `--time-scale 4` a tick is 67 ms and small moves settle in a few ticks. The e2e injector deactivates after `click`/`mouse_up` and hit-testing falls back to the platform cursor (screen centre headless), so re-park the mouse after each release. `expect_drawn_at rectangle_rounded x= y= w= h= color=#rrggbb tol=` reads geometry; `expect_draw_calls_below N` pins a primitive ceiling; circles, triangles, lines, rects, quads and text are all captured. Screenshots of a mid-flight frame go right after the input, before any `expect_text`; a fast transition is over within a couple of polls.

Every commit that touches motion checks: 30/60/120 fps and irregular frames, rapid reversals, reset, teardown and reduced motion mid-transition; Tab, Escape, clicks and wheel during an overlay exit; start/middle/end screenshots in both renderers; reduced motion settling to the requested state with cleanup run. Build with two jobs and run suites one at a time.

Headless e2e draws to a render texture, so it does not prove the windowed shader or blur path. Capture that path with `--test-script-dir <dir> --direct-window` and a window screenshot.
