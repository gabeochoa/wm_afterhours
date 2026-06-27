# react-spring Analysis → afterhours Animation Improvements

Source: https://www.react-spring.dev/
GitHub: https://github.com/pmndrs/react-spring

react-spring is a physics-based animation library for React. It animates
outside the React render cycle (no re-renders per frame) using spring
physics as the default model. It's the most ergonomic animation library
in the React ecosystem.

---

## 1. Why react-spring Feels Good

The core philosophy: **springs, not durations**.

> "Animation APIs parameterized by duration and curve are fundamentally
> opposed to continuous, fluid interactivity."
> — Andy Matuschak (ex Apple UI-Kit developer)

Springs are interruptible. If you change the target mid-animation, the
spring naturally adjusts because it has velocity. Duration-based
animations must restart or awkwardly crossfade. This is why react-spring
animations feel "alive" — they respond to changing input continuously.

---

## 2. react-spring Architecture

```
  SpringValue          Controller          Hook Layer
  ───────────          ──────────          ──────────
  Single animated      Manages multiple    useSpring()
  value (float,        SpringValues        useSprings()
  color, array)        as a group          useTransition()
                                           useTrail()
                                           useChain()
```

- **SpringValue**: The atomic unit. A single value that changes over
  time via spring physics. It can be a number, color, array, or string
  with embedded numbers. It drives updates to the rendered element
  directly (bypassing React's render cycle).

- **Controller**: Manages a set of SpringValues that animate together.
  `useSpring({ x: 0, y: 0, opacity: 1 })` creates one Controller with
  three SpringValues.

- **SpringRef**: A handle to one or more Controllers. Provides the
  imperative API (`start`, `stop`, `pause`, `resume`, `set`).

- **Hooks**: React integration layer. Each hook creates Controllers and
  returns SpringValues + a SpringRef (the `api`).

### Key architecture insight

The animation engine is **completely decoupled from rendering**. The
SpringValue updates on its own schedule (via `requestAnimationFrame`)
and writes directly to the DOM/element. This is why it's fast — no
framework re-renders during animation.

afterhours already has this property naturally (ECS systems tick
independently), but the animation API could better leverage it.

---

## 3. What react-spring Gets Right (Ergonomics)

### 3a. Declarative `from` / `to` with Implicit Diffing

```jsx
// Just declare what you want. The library figures out what changed.
const springs = useSpring({
    from: { opacity: 0, x: -100 },
    to:   { opacity: 1, x: 0 },
})
```

When `to` changes on re-render, react-spring automatically starts
animating from the current value to the new target. No manual
"start animation" call needed.

**afterhours current pain point:** You have to explicitly call
`anim::start(track, from, to)` or use the `AnimHandle::from().to()`
chain. There's no "just set the target and it animates" mode.

### 3b. Spring Physics as Default (Not Easing Curves)

```jsx
// Default: spring physics, no duration needed
useSpring({ to: { x: 100 } })

// Override with config
useSpring({ to: { x: 100 }, config: { mass: 5, tension: 120, friction: 14 } })

// Or use duration mode explicitly
useSpring({ to: { x: 100 }, config: { duration: 300, easing: easings.easeOutQuad } })
```

Spring physics is the default because:
1. Springs are **interruptible** — changing target mid-flight works
2. Springs produce **natural motion** without tuning duration
3. Springs converge based on physics, not arbitrary time

**afterhours current state:** `animation_config.h` supports both spring
physics and easing curves (good!), but the `animation.h` standalone
manager only has `EasingType` with no spring option. The two animation
systems aren't unified.

### 3c. Named Config Presets

```jsx
import { config } from '@react-spring/web'

// Presets: no need to remember mass/tension/friction values
config.default  // { tension: 170, friction: 26 }
config.gentle   // { tension: 120, friction: 14 }
config.wobbly   // { tension: 180, friction: 12 }
config.stiff    // { tension: 210, friction: 20 }
config.slow     // { tension: 280, friction: 60 }
config.molasses // { tension: 280, friction: 120 }
```

These presets give you intuitive names for common spring feels without
needing to understand physics parameters.

**afterhours equivalent:** No spring presets exist. Users must specify
`spring_frequency` and `spring_decay` manually.

### 3d. Per-Property Config

```jsx
useSpring({
    opacity: 1,
    x: 100,
    config: (key) => {
        if (key === 'opacity') return { duration: 200 }  // Smooth fade
        return { mass: 5, friction: 120, tension: 120 }  // Bouncy move
    }
})
```

Different properties can have different spring configs. Opacity fades
smoothly while position bounces.

**afterhours equivalent:** Each `AnimationDef` is per-property, so this
is already possible. But there's no way to specify one config set for a
group of properties at once and override individual ones.

### 3e. Imperative API (api.start / api.stop / api.set)

```jsx
const [springs, api] = useSpring(() => ({ x: 0 }))

// Trigger from event handler — no state change, no re-render
const handleClick = () => {
    api.start({ x: 100 })
}

// Set immediately (no animation)
api.set({ x: 0 })

// Stop mid-animation
api.stop()

// Pause / resume
api.pause()
api.resume()
```

The imperative API is the recommended way to drive animations because
it doesn't trigger re-renders.

**afterhours current state:** `AnimHandle` has `.from()`, `.to()`,
`.sequence()`, `.hold()`, `.on_complete()` — similar but missing
`stop`, `pause`, `resume`, and `set` (instant jump).

### 3f. Transitions: Mount/Unmount Animation

```jsx
const transitions = useTransition(items, {
    from:  { opacity: 0, y: 20 },
    enter: { opacity: 1, y: 0 },
    leave: { opacity: 0, y: -20 },
})

return transitions((style, item) => (
    <animated.div style={style}>{item.text}</animated.div>
))
```

`useTransition` manages the **lifecycle of elements** — animating them
in when they appear and out when they disappear. The element stays in
the DOM during the leave animation and is removed after.

This is the pattern Ariakit calls the "presence state machine"
(`open → enter → leave → hidden`).

**afterhours current state:** `AnimTrigger::OnAppear` handles the enter
case, but there's no equivalent for leave/exit animations. When an
entity is removed, it just disappears.

### 3g. Trails: Staggered Animations

```jsx
const trails = useTrail(items.length, {
    from: { opacity: 0, y: 20 },
    to:   { opacity: 1, y: 0 },
})
```

`useTrail` creates N springs that stagger — each one starts when the
previous one begins (not ends). This creates a wave/cascade effect
that's extremely common in UI: list items appearing one by one, cards
fanning out, etc.

**afterhours equivalent:** No trail/stagger primitive. You'd have to
manually offset start times with delays.

### 3h. Chain: Sequenced Heterogeneous Animations

```jsx
useChain([springRef, transitionRef], [0, 0.4])
```

`useChain` orchestrates different animation types in sequence with
configurable timesteps (0-1 range × timeframe). Spring A starts at
t=0, transition B starts at t=400ms.

**afterhours equivalent:** `AnimSegment` queue handles sequential
segments on a single track, but there's no way to coordinate across
multiple independent animations/entities.

### 3i. Interpolation / Value Mapping

```jsx
// Map a 0-360 spring value to a CSS transform
springs.x.to(value => `rotateZ(${value}deg)`)

// Map ranges: input 0-1 → output 0-100
springs.progress.to([0, 1], [0, 100])

// Combine multiple spring values
to([springs.x, springs.y, springs.z],
   (x, y, z) => `rotate3d(${x}, ${y}, ${z}, 45deg)`)
```

Spring values can be interpolated/mapped to any output format. This is
how one spring value can drive multiple visual properties (e.g., a
single progress value controlling position, opacity, AND color).

**afterhours equivalent:** The `on_change` watcher with `quantize` is
a basic form of this. But there's no general-purpose interpolation
that maps one animated value to derived values.

### 3j. Reduced Motion Support

```jsx
import { useReducedMotion } from '@react-spring/web'

// Called once at app root — sets Globals.skipAnimation = true
useReducedMotion()
```

When the user has "Reduce motion" enabled, all animations instantly
jump to their target values. This is implemented via a global
`skipAnimation` flag that makes every spring resolve immediately.

**afterhours equivalent:** No reduced motion support exists. This ties
directly to the `enforce_reduced_motion_support` validation flag
proposed in the a11y notes.

### 3k. Lifecycle Events

```jsx
useSpring({
    to: { x: 100 },
    onStart:   (result, spring) => { ... },
    onChange:  (result, spring) => { ... },
    onRest:    (result, spring) => { ... },
    onPause:   (result, spring) => { ... },
    onResume:  (result, spring) => { ... },
    onResolve: (result, spring) => { ... },
})
```

Events can be per-spring or per-key within a spring. The `AnimationResult`
includes `{ value, finished, cancelled }`.

**afterhours current state:** Only `on_complete` exists on `AnimHandle`.
No `onChange`, `onStart`, `onPause`, or `onCancel`.

---

## 4. Current afterhours Animation System Assessment

afterhours has **two separate animation systems**:

### System A: `animation.h` (standalone plugin)

- Enum-keyed tracks in a global `AnimationManager`
- Fluent builder: `anim<Key>(k).from(0).to(100, 0.5f, EaseOutQuad)`
- Segment queuing (chained animations)
- Watchers (on_change with quantize)
- Loop support
- One-shot helper

**Strengths:** Simple, low overhead, global access via static methods.
**Weaknesses:** Only supports `float` values. Only easing-based (no
spring physics). No stop/pause/resume. No exit animations.

### System B: `animation_config.h` (UI animation component)

- Entity-attached `HasAnimationState` component with per-property tracks
- Trigger-based: `OnAppear`, `OnClick`, `OnHover`, `OnFocus`, `Loop`
- Spring physics AND easing curves
- Fluent builder: `Anim::on_click().scale(0.95f).spring()`
- Per-entity state tracking

**Strengths:** Integrated with UI entity system. Has spring physics.
Trigger system is convenient.
**Weaknesses:** Fixed set of properties (Scale, TranslateX/Y, Rotation,
Opacity). No exit animations. No trail/stagger. No cross-entity
coordination. No interpolation.

### Key gap: The two systems are disconnected

`animation.h` and `animation_config.h` don't share code. Spring
physics in one, easing in the other. The fluent APIs are different.
There's no unified animation model.

---

## 5. Proposed Improvements

### 5.1 Spring Presets (Quick Win)

Add named presets to eliminate physics parameter guessing:

```cpp
namespace SpringPreset {
    constexpr auto Default  = SpringConfig{ .frequency = 12.0f, .decay = 8.0f };
    constexpr auto Gentle   = SpringConfig{ .frequency = 8.0f,  .decay = 6.0f };
    constexpr auto Wobbly   = SpringConfig{ .frequency = 14.0f, .decay = 4.0f };
    constexpr auto Stiff    = SpringConfig{ .frequency = 20.0f, .decay = 12.0f };
    constexpr auto Slow     = SpringConfig{ .frequency = 6.0f,  .decay = 10.0f };
    constexpr auto Molasses = SpringConfig{ .frequency = 4.0f,  .decay = 14.0f };
    constexpr auto Snappy   = SpringConfig{ .frequency = 18.0f, .decay = 14.0f };
}

// Usage: much more discoverable
Anim::on_click().scale(0.95f).spring(SpringPreset::Wobbly)
```

### 5.2 Add Spring Physics to `animation.h` (Quick Win)

The standalone animation manager only has easing. Add spring as a mode:

```cpp
enum struct AnimMode { Easing, Spring };

struct AnimSegment {
    float to_value = 0.f;
    // For easing mode:
    float duration = 0.f;
    EasingType easing = EasingType::Linear;
    // For spring mode:
    AnimMode mode = AnimMode::Easing;
    float spring_freq = 12.0f;
    float spring_decay = 8.0f;
};

// In AnimHandle:
AnimHandle &spring_to(float value, float freq = 12.f, float decay = 8.f) {
    // ... create spring segment
}
AnimHandle &spring_to(float value, SpringConfig preset) {
    return spring_to(value, preset.frequency, preset.decay);
}
```

### 5.3 Stop / Pause / Resume / Set (Quick Win)

Missing imperative controls on `AnimHandle`:

```cpp
AnimHandle &stop() {
    AnimTrack &tr = mgr.ensure_track(key);
    tr.active = false;
    tr.queue.clear();
    return *this;
}

AnimHandle &pause() {
    AnimTrack &tr = mgr.ensure_track(key);
    tr.paused = true;  // New field
    return *this;
}

AnimHandle &resume() {
    AnimTrack &tr = mgr.ensure_track(key);
    tr.paused = false;
    return *this;
}

AnimHandle &set(float value) {
    // Jump immediately, no animation
    AnimTrack &tr = mgr.ensure_track(key);
    tr.current = value;
    tr.from = value;
    tr.to = value;
    tr.active = false;
    tr.queue.clear();
    return *this;
}
```

### 5.4 Exit / Leave Animations (Medium)

The biggest missing feature for UI. When an entity is removed, it
should animate out before actually being destroyed.

```cpp
enum class AnimTrigger {
    OnAppear,
    OnClick,
    OnHover,
    OnFocus,
    OnExit,    // NEW: when entity is about to be removed
    Loop,
};

// Usage:
Anim::on_exit().opacity(1.0f, 0.0f).ease_out(0.2f)
Anim::on_exit().translate_y(0.0f, -20.0f).spring(SpringPreset::Gentle)
```

Implementation: When an entity with an `OnExit` animation is marked
for cleanup, instead of immediately removing it:
1. Set `cleanup = true` but don't remove yet
2. Start the exit animation
3. Remove the entity when the exit animation completes

This ties into Ariakit's presence state machine: `visible → exiting →
removed`.

### 5.5 Trail / Stagger (Medium)

Animate a group of entities in sequence with configurable overlap:

```cpp
// Stagger children of a container
struct HasTrailAnimation : BaseComponent {
    float stagger_delay = 0.05f;  // Seconds between each child starting
    AnimationDef per_item;        // Animation to apply to each
};

// Or imperative:
namespace trail {
    // Given a list of anim keys, start each with stagger_delay offset
    template <typename Key>
    void start(const std::vector<Key>& keys,
               float to, float duration, EasingType easing,
               float stagger_delay = 0.05f) {
        for (size_t i = 0; i < keys.size(); ++i) {
            float delay = i * stagger_delay;
            animation::anim(keys[i])
                .hold(delay)
                .to(to, duration, easing);
        }
    }
}
```

### 5.6 Interpolation / Derived Values (Medium)

One animated value drives multiple visual properties:

```cpp
// Map a 0-1 progress value to multiple outputs
AnimTrack &progress = state.get(AnimProperty::Custom0);
float opacity = lerp(0.0f, 1.0f, progress.current);
float y_offset = lerp(20.0f, 0.0f, progress.current);
float scale = lerp(0.8f, 1.0f, progress.current);

// Or with a dedicated interpolation helper:
auto mapped = anim_map(progress.current,
    {0.0f, 0.3f, 1.0f},   // input range
    {0.0f, 1.0f, 1.0f});  // output range (opacity reaches 1 at 30%)
```

Implementation:

```cpp
inline float anim_map(float value,
                       const std::vector<float>& input_range,
                       const std::vector<float>& output_range) {
    // Find the segment
    for (size_t i = 0; i < input_range.size() - 1; ++i) {
        if (value <= input_range[i + 1]) {
            float t = (value - input_range[i]) /
                      (input_range[i + 1] - input_range[i]);
            return std::lerp(output_range[i], output_range[i + 1], t);
        }
    }
    return output_range.back();
}
```

### 5.7 Reduced Motion Global Flag (Quick Win)

```cpp
namespace animation {
    // Global flag — when true, all animations resolve immediately
    static inline bool skip_animation = false;

    // Check before starting any animation
    AnimHandle &to(float value, float duration, EasingType easing) {
        if (skip_animation) {
            // Jump immediately
            AnimTrack &tr = mgr.ensure_track(key);
            tr.current = value;
            tr.active = false;
            return *this;
        }
        // ... normal animation start
    }
}
```

### 5.8 Cross-Entity Coordination / Chain (Low Priority)

Sequence animations across different entities/managers:

```cpp
// Animate entity A, then entity B
animation::anim(AnimKey::PanelSlide)
    .from(0.f).to(1.f, 0.3f, EaseOut)
    .on_complete([]() {
        animation::anim(AnimKey::ContentFade)
            .from(0.f).to(1.f, 0.2f, EaseOut);
    });
```

This already works via `on_complete` chaining. A higher-level helper
could make it more ergonomic:

```cpp
animation::chain({
    { AnimKey::PanelSlide,  {.to=1.f, .dur=0.3f, .ease=EaseOut}, 0.0f },
    { AnimKey::ContentFade, {.to=1.f, .dur=0.2f, .ease=EaseOut}, 0.8f },
}, 1000.f);  // timeframe in ms, timesteps are 0-1
```

### 5.9 Declarative Target-Based Animation (Low Priority)

The react-spring "just set the target and it animates" model:

```cpp
// Instead of explicitly calling start(), just set the target.
// If the current value differs from target, animation starts.
AnimTrack &track = state.get(AnimProperty::Opacity);
track.target = is_visible ? 1.0f : 0.0f;
// The update system sees current != target and starts animating.
```

This requires the update system to detect when `target` changes and
auto-start animation. It's the most natural model for UI state-driven
animation (e.g., dialog open/close is just `target = open ? 1 : 0`).

---

## 6. Priority Summary

| Improvement | Effort | Impact | Priority |
|---|---|---|---|
| Spring presets | Tiny | High | **P0** — immediate ergonomics win |
| Stop/pause/resume/set on AnimHandle | Small | Medium | **P0** — basic controls |
| Add spring physics to `animation.h` | Small | High | **P0** — unify the two systems |
| Reduced motion global flag | Tiny | Medium | **P0** — accessibility |
| Exit/leave animations | Medium | Very High | **P1** — biggest missing UI feature |
| Trail/stagger | Small | High | **P1** — extremely common UI pattern |
| Interpolation (range mapping) | Small | Medium | **P1** — one value drives many |
| Richer lifecycle events | Small | Medium | **P2** — onStart, onChange |
| Cross-entity chain helper | Small | Low | **P2** — ergonomic convenience |
| Declarative target-based animation | Medium | High | **P2** — paradigm shift, big refactor |

---

## 7. What NOT to Adopt

- **React-specific HOC pattern** (`animated.div`): This is a rendering
  framework concern. afterhours' ECS approach (systems read animated
  values during render) is already equivalent.

- **SpringValue as observable/fluid**: react-spring's observable value
  propagation system is designed to bypass React's render cycle. In
  afterhours, the update loop already runs every frame, so there's no
  need for an event-driven propagation system.

- **Duration mode as first-class**: react-spring supports `duration` as
  an alternative to spring physics, but the library's philosophy is that
  springs are superior for interactive animation. afterhours should lean
  the same way — spring first, easing for specific cases.

---

## 8. Key Takeaway

react-spring's ergonomic advantage comes from three principles:

1. **Spring-first**: Physics-based animation as the default produces
   natural motion without tuning duration. Interruption works for free.

2. **Declarative targets**: "Set the goal, not the journey." The
   animation system figures out how to get there.

3. **Rich orchestration**: Trail, chain, transition — higher-level
   patterns for common multi-element animations are built-in, not
   assembled from raw primitives every time.

afterhours already has the physics engine (`animation_config.h` spring)
and the fluent builder pattern. The main gaps are: unifying the two
animation systems, adding exit animations, adding spring presets for
discoverability, and adding orchestration primitives (trail, chain).
