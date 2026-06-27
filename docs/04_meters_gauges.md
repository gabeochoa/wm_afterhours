# Meters and Gauges

**Status:** Partially implemented  
**Priority:** Low

> ✅ `progress_bar()` and `circular_progress()` exist in afterhours.  
> ❌ Inline meters with icons and stacked bars not yet implemented.

---

## Inline Meter/Gauge

### Problem

Small inline progress indicators with icon and label are common in game UIs.

### Pattern Structure
```
┌─────────────────────────────────┐
│ [Icon] Label   [███████░░░░░░] │
└─────────────────────────────────┘
```

### Suggested Implementation

```cpp
ElementResult inline_meter(HasUIContext auto &ctx, EntityParent ep_pair,
                           float value,  // 0.0 to 1.0
                           ComponentConfig config = ComponentConfig());
```

---

## Dual/Stacked Progress Bars

### Problem

Health + Armor/Shield bars stacked together are common in action games.

### Suggested Implementation

```cpp
ElementResult stacked_bars(HasUIContext auto &ctx, EntityParent ep_pair,
                           float primary_value, float secondary_value,
                           ComponentConfig config = ComponentConfig());
```

---

## Example Screen: MetersGaugesShowcase

**File:** `src/systems/screens/MetersGaugesShowcase.h`
**CLI:** `--screen=meters_gauges`
**Category:** Widgets

### Layout

A game HUD-style screen demonstrating meter components:

1. **Inline Meters** — Three inline meters in a row: HP (red, 75%), MP (blue, 40%), XP (gold, 90%). Each shows `[Icon] Label [███████░░░]` pattern with `progress_bar` composed with icon and text.

2. **Stacked Bars** — A health/shield combo: green health bar (80%) with blue shield bar (30%) stacked on top, partially overlapping. Demonstrates `stacked_bars()`.

3. **Animated Progress** — A progress bar that smoothly animates from 0% to 100% over 3 seconds, then resets. Shows `progress_bar` and `circular_progress` with animated values.

4. **Circular Gauges** — Three `circular_progress` indicators showing different values (25%, 50%, 90%) with value labels in the center.

5. **Interactive Slider→Meter** — A slider that controls the value of all meters above. Dragging the slider updates HP, MP, XP, and circular gauges in real time.

### Features Exercised

- `progress_bar()` with themed colors
- `circular_progress()` with value display
- `inline_meter()` composing icon + label + progress_bar
- `stacked_bars()` for dual-layer display
- Animated value transitions

### Verification

- Inline meters show icon, label, and colored fill proportional to value
- Stacked bars: shield bar renders above health bar
- Slider at 0%: all meters empty; slider at 100%: all meters full
- Circular progress shows arc proportional to value

### E2E Test Plan

**Test file:** `src/testing/tests/MetersGaugesTest.h`

#### New Custom Commands Needed

- `drag_element_by(label, dx, dy, hold_frames)` — click element, drag by pixel offset, release. Needed for dragging the interactive slider that controls meter values.

#### Screenshots

1. `meters_initial` — all meters at default values (HP 75%, MP 40%, XP 90%)
2. `meters_slider_at_zero` — slider dragged to 0%, all meters empty
3. `meters_slider_at_full` — slider at 100%, all meters full
4. `meters_stacked_bars` — stacked health+shield bars visible
5. `meters_circular` — circular gauges at different values
6. `meters_animated_progress` — progress bar mid-animation

#### Test Script

```cpp
TEST(meters_gauges_display) {
  co_await TestApp::wait_for_frames(5);

  // Verify all meters exist
  TestApp::expect_ui_exists("HP");
  TestApp::expect_ui_exists("MP");
  TestApp::expect_ui_exists("XP");

  auto snap = TestApp::capture_snapshot("meters_initial");
}

TEST(meters_gauges_slider_control) {
  co_await TestApp::wait_for_frames(5);

  // Verify stacked bars
  auto snap_stacked = TestApp::capture_snapshot("meters_stacked_bars");
  auto snap_circular = TestApp::capture_snapshot("meters_circular");
}

TEST(meters_gauges_animated) {
  // Wait for animation to be mid-progress
  co_await TestApp::wait_for_frames(90);  // ~1.5s at 60fps, mid-animation

  auto snap = TestApp::capture_snapshot("meters_animated_progress");
}
```

