# Animation in afterhours

Everything lives in `afterhours::motion` (`vendor/afterhours/src/plugins/animation/`). `afterhours::animation` keeps `set_instant`, `is_instant` and `register_update_systems` as forwarders.

## Core

**Spring** — closed-form damped oscillator. `Spring{.response = 0.3f, .bounce = 0.f}`; `response` is roughly the perceived duration in seconds, `bounce` in `[0, 1)` is overshoot. Presets: `Spring::smooth()`, `snappy()`, `bouncy()`, `gentle()`. `Spring::from_freq_decay(freq, decay)` maps the old UI spring numbers. Frame rate never changes where a spring lands; a stalled frame evaluates at the true time. Retargeting mid-flight keeps velocity.

**Timeline** — keys plus a repeat mode plus one optional curve. Output is dimensionless progress that the track lerps between its start and target, so `{{0,0},{0.08,1},{0.16,-1},{0.22,.667},{0.28,0}}` with target 6 is a ±6 px shake. `Repeat::Once | Loop | PingPong`. Curves: `curves::ease_in_quad`, `ease_out_quad`, `ease_in_out_quad`. A timed ease is `Timeline{.keys = {{0,0},{0.25f,1}}, .curve = curves::ease_out_quad}`.

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

```cpp
ComponentConfig{}
  .on_appear({.translate_y = {24.f, 0.f}, .opacity = {0.f, 1.f}}, Spring::gentle(), /*delay*/ 0.1f)
  .on_hover({.scale = 1.05f, .translate_y = -4.f})
  .on_press({.scale = 0.92f})
  .on_focus({.scale = 1.03f})
  .on_state(is_open, {.scale = {0.97f, 1.f}, .opacity = {0.f, 1.f}}, Timeline{...})
  .on_change(stamp, {.scale = {1.2f, 1.f}}, Spring::bouncy())
```

Properties: `scale`, `translate_x`, `translate_y`, `rotation`, `opacity`, `corner_radius`, `background` (a `ColorType`; replaces the fill and opts out of the theme hover fill). A single value means "target"; `{from, to}` supplies the other end for `on_appear`, `on_state` (false → `from`) and `on_change`.

Each frame the active triggers collapse into one target per property — press > hover > focus > state/rest, with `on_change` firing a one-frame impulse on top when its stamp differs from last frame — and each track is retargeted once. Falling back to rest reuses the last winning spring, so hover-out is a reversal, not a fresh animation. `on_appear` pins the rest value to its `to`; give hidden-at-rest elements an `on_appear` so their first frame starts where the state's `from` would put them.

`with_origin(x, y)` pins scale to a corner or edge (0..1 rect space). Rotation pivots on the centre. A parent's translate composes to its children, including hit-testing; scale does not compose yet.

## Patterns that work

- **Keep emitting.** Closed menus, tooltips, toasts and pages stay in the tree with `on_state(open, …)` driving opacity to 0; add `with_ignore_pointer_events()` and `with_skip_tabbing(true)` while closed. Nothing needs to be retained after a caller stops emitting.
- **Layout follows a track** when you feed it into `with_size()`; feed it into the modifier (`on_state({.scale…})`) for visual-only motion.
- **Absolute elements:** `with_absolute_position(x, y)` *is* their translate. To move an absolutely placed element, use a motion translate or offset the position from a track.
- **Buttons hosting absolutely placed children** need `.with_padding(Padding::all(pixels(0)))`; theme padding offsets the children otherwise.
- **Replay an entrance** by removing `HasMotionState` and `motion::HasTracks` from the entity; `on_appear` fires again next frame.
- **Timers:** a hold is a timeline with `on_complete` — `hold.from(0).to(1, Timeline{{0,0},{3,1}}).on_complete([this]{ shown = false; })`.
- **Modals:** `ModalConfig{}.with_motion(enter, exit)` gives `Hidden → Entering → Visible → Exiting → Hidden`; the result stays true while exiting so you keep emitting the body, input is blocked until it settles, focus is restored once, and reopening mid-exit reverses. Without `with_motion` nothing changes.

## Reduced motion

`Settings::set_reduced_motion_enabled(bool)` persists `reduced_motion_enabled` and drives `animation::set_instant`. Mark loops that must keep moving (spinners) `.essential()`.

## Testing

Headless dt is fixed (`1/target_fps × time_scale`) and springs are closed-form, so `wait_frames N` steps deterministically. Assert "in flight" immediately after the input — at `--time-scale 4` a tick is 67 ms and small moves settle in a few ticks. The e2e injector deactivates after `click`/`mouse_up` and hit-testing falls back to the platform cursor (screen centre headless), so re-park the mouse after each release. `expect_drawn_at rectangle_rounded x= y= w= h= color=#rrggbb tol=` reads geometry; `expect_draw_calls_below N` pins a primitive ceiling; circles, triangles, lines, rects and text are all captured.
