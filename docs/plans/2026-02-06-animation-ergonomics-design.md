# Animation Ergonomics Design

## Problem

Adding animations to UI components currently requires ~50+ lines of boilerplate:
- Manual enum keys per screen
- Animation manager setup and update calls
- Helper functions to get/set animation values
- Manual trigger detection (click time tracking)

## Solution

A declarative `.with_animation()` API on ComponentConfig that handles everything automatically.

## Usage

```cpp
// Before: 50+ lines of setup code

// After: Animation declared inline
button(context, mk(entity, 10),
       ComponentConfig{}
           .with_label("Click!")
           .with_animation(Anim::on_click().scale(0.85f, 1.0f).spring())
           .with_animation(Anim::on_hover().scale(1.0f, 1.05f).ease_out(0.1f)));
```

## Design

### 1. Animation Config Types

```cpp
enum class AnimTrigger {
  OnAppear,   // First render
  OnClick,    // When clicked/pressed
  OnHover,    // Mouse enters
  OnFocus,    // Keyboard focus
  Loop,       // Continuous
};

enum class AnimProperty {
  Scale,
  TranslateX,
  TranslateY,
  Opacity,
};

enum class AnimCurve {
  Spring,
  EaseOut,
  EaseIn,
  EaseInOut,
  Linear,
};

struct AnimationDef {
  AnimTrigger trigger;
  AnimProperty property;
  float from_value;
  float to_value;
  AnimCurve curve = AnimCurve::Spring;
  float duration = 0.3f;
  float spring_frequency = 3.0f;
  float spring_decay = 2.0f;
};
```

### 2. Fluent Anim Builder

```cpp
class Anim {
  AnimationDef def;

public:
  static Anim on_appear();
  static Anim on_click();
  static Anim on_hover();
  static Anim on_focus();
  static Anim loop();

  Anim& scale(float from, float to);
  Anim& scale(float to);  // shorthand: from current
  Anim& translate_x(float from, float to);
  Anim& translate_y(float from, float to);
  Anim& opacity(float from, float to);

  Anim& spring(float freq = 3.0f, float decay = 2.0f);
  Anim& ease_out(float dur = 0.3f);
  Anim& ease_in(float dur = 0.3f);
  Anim& linear(float dur = 0.3f);

  const AnimationDef& build() const;
};
```

### 3. Minimal State (Per-Entity)

```cpp
struct AnimTrack {
  float current = 0.0f;
  float target = 0.0f;
  float velocity = 0.0f;
  float elapsed = 0.0f;
};

struct HasAnimationState {
  AnimTrack scale;
  AnimTrack translate_x;
  AnimTrack translate_y;
  AnimTrack opacity;

  AnimTrack& get(AnimProperty prop);
};
```

### 4. Pure Animation Functions

```cpp
namespace anim {
  bool spring(AnimTrack& t, float freq, float decay, float dt);
  float ease_out(AnimTrack& t, float duration, float dt);
  float lerp(AnimTrack& t, float duration, float dt);
  void start(AnimTrack& t, float from, float to);
}
```

### 5. ComponentConfig Integration

```cpp
struct ComponentConfig {
  // ... existing fields ...
  std::vector<AnimationDef> animations;

  ComponentConfig& with_animation(const Anim& anim);
};
```

### 6. Apply Logic

In `apply_animations()`:
1. Check trigger conditions using `ctx.is_hot()`, `ctx.is_active()`, `ctx.has_focus()`
2. Start animation when trigger activates
3. Reverse animation when trigger deactivates
4. Update tracks using pure functions
5. Apply final values to `HasUIModifiers`

## Additive Animations

Multiple animations on the same property combine:
- Scale: multiplicative (0.9 * 1.05 = 0.945)
- Translate: additive (offset1 + offset2)
- Opacity: multiplicative

## Files to Create/Modify

1. **NEW**: `vendor/afterhours/src/plugins/ui/animation_config.h`
   - AnimTrigger, AnimProperty, AnimCurve enums
   - AnimationDef struct
   - Anim builder class
   - AnimTrack struct
   - HasAnimationState component
   - anim:: namespace functions

2. **MODIFY**: `vendor/afterhours/src/plugins/ui/component_config.h`
   - Add `std::vector<AnimationDef> animations`
   - Add `with_animation()` method

3. **MODIFY**: `vendor/afterhours/src/plugins/ui/component_init.h`
   - Add `apply_animations()` function
   - Call it from `_add_missing_components()` or create new system

## Relationship to Existing `animation.h`

**Both systems coexist:**

| Use Case | Recommended System |
|----------|-------------------|
| Button press/hover effects | New `.with_animation()` |
| Appear/entrance animations | New `.with_animation()` |
| Complex multi-step sequences | Old `animation.h` |
| Non-UI animations (game logic) | Old `animation.h` |

**Migration:** No breaking changes. Developers can adopt the new API incrementally. Future deprecation of the old system will be decided based on team feedback.

## Testing

Update AnimationInteractiveDemo to use new API as proof of concept.
