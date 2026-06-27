# Visual Effects

**Status:** Not implemented  
**Priority:** Medium

---

## Gradient Backgrounds

### Problem

Cannot render gradient fills (linear, radial).

### Workaround

`src/ui_workarounds/GradientBackground.h` - layered strips

### Suggested Implementation

```cpp
ComponentConfig{}
    .with_gradient_background(
        GradientType::Linear,
        Color{155, 195, 235, 255},  // start
        Color{200, 220, 245, 255},  // end
        0.0f);                       // angle
```

### Example Screen: GradientShowcase

**File:** `src/systems/screens/GradientShowcase.h`
**CLI:** `--screen=gradients`
**Category:** Visual Effects

#### Layout

1. **Linear Gradients** — Three panels side by side:
   - Horizontal blue→purple (angle 0°)
   - Vertical green→teal (angle 90°)
   - Diagonal orange→red (angle 45°)
   Each labeled with its angle and colors.

2. **Radial Gradients** — Two panels:
   - Light center → dark edges (default)
   - Dark center → light edges (inverted)

3. **Angle Slider** — A large demo panel with a slider (0–360°) that rotates the gradient direction in real time. Current angle displayed as text.

4. **Practical Usage** — A game menu card using a gradient background behind buttons: "New Game", "Continue", "Settings". Shows gradients in context.

#### Features Exercised

- `with_gradient_background(GradientType::Linear, start, end, angle)`
- `with_gradient_background(GradientType::Radial, start, end)`
- Real-time angle parameter changes
- Gradient behind interactive elements

#### Verification

- Linear gradient smoothly transitions between colors
- Angle rotation visually matches the slider value
- Radial gradient is centered in the element
- Gradient doesn't interfere with child element rendering

#### E2E Test Plan

**Test file:** `src/testing/tests/GradientTest.h`

##### New Custom Commands Needed

None — visual-only, snapshot comparison is the primary verification.

##### Screenshots

1. `gradient_linear_horizontal` — horizontal blue→purple
2. `gradient_linear_angled` — 45° diagonal orange→red
3. `gradient_radial` — radial light center, dark edges
4. `gradient_angle_slider` — demo panel with angle at 135°
5. `gradient_menu_card` — practical usage with buttons over gradient

##### Test Script

```cpp
TEST(gradient_linear) {
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("0°");
  TestApp::expect_ui_exists("90°");
  TestApp::expect_ui_exists("45°");

  auto snap = TestApp::capture_snapshot("gradient_linear_horizontal");
  auto snap2 = TestApp::capture_snapshot("gradient_linear_angled");
}

TEST(gradient_radial) {
  co_await TestApp::wait_for_frames(5);

  auto snap = TestApp::capture_snapshot("gradient_radial");
}

TEST(gradient_interactive_angle) {
  co_await TestApp::wait_for_frames(5);

  // Drag angle slider to 135°
  TestApp::click_button("Angle Slider");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  auto snap = TestApp::capture_snapshot("gradient_angle_slider");
}

TEST(gradient_with_children) {
  co_await TestApp::wait_for_frames(5);

  // Verify buttons are clickable over gradient
  TestApp::expect_ui_exists("New Game");
  TestApp::expect_ui_exists("Continue");

  auto snap = TestApp::capture_snapshot("gradient_menu_card");
}
```

---

## 3D/Puffy Text Effect

### Problem

Cannot create 3D "puffy" text effects with depth/volume.

### Suggested Implementation

```cpp
ComponentConfig{}
    .with_puffy_text(outline_color, highlight_color, shadow_color, depth);
```

### Example Screen: PuffyTextShowcase

**File:** `src/systems/screens/PuffyTextShowcase.h`
**CLI:** `--screen=puffy_text`
**Category:** Visual Effects

#### Layout

1. **Basic Puffy Text** — The word "ADVENTURE" rendered large with default puffy settings: colored fill, darker outline, lighter highlight on top edge, shadow offset below.

2. **Depth Comparison** — The same word at 4 depth values (1px, 3px, 6px, 10px) side by side, showing how depth affects the 3D appearance. Each labeled with its depth.

3. **Color Variations** — Three puffy text examples with different color schemes:
   - Gold/amber (treasure theme): gold fill, dark brown outline, yellow highlight, brown shadow
   - Ice/frost: light blue fill, white outline, white highlight, dark blue shadow
   - Fire: red fill, dark red outline, orange highlight, black shadow

4. **Parameter Sliders** — An interactive panel with sliders for: depth (0–15), outline thickness, highlight opacity, shadow opacity. Drives a preview label that updates in real time.

#### Features Exercised

- `with_puffy_text(outline, highlight, shadow, depth)` at various settings
- Depth parameter effect on visual volume
- Different color scheme combinations
- Real-time parameter adjustment

#### Verification

- Puffy text has visible 3D depth (shadow below, highlight above)
- Higher depth values produce more pronounced shadow offset
- Each color scheme is visually distinct
- Sliders produce visible real-time changes

#### E2E Test Plan

**Test file:** `src/testing/tests/PuffyTextTest.h`

##### New Custom Commands Needed

None.

##### Screenshots

1. `puffy_text_basic` — "ADVENTURE" with default puffy settings
2. `puffy_text_depth_compare` — same word at 4 depth values
3. `puffy_text_gold` — gold/amber color scheme
4. `puffy_text_ice` — ice/frost color scheme
5. `puffy_text_fire` — fire color scheme
6. `puffy_text_slider_preview` — interactive preview with custom slider values

##### Test Script

```cpp
TEST(puffy_text_renders) {
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("ADVENTURE");
  auto snap = TestApp::capture_snapshot("puffy_text_basic");
}

TEST(puffy_text_depth_variants) {
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("1px");
  TestApp::expect_ui_exists("10px");
  auto snap = TestApp::capture_snapshot("puffy_text_depth_compare");
}

TEST(puffy_text_color_schemes) {
  co_await TestApp::wait_for_frames(5);

  auto snap_gold = TestApp::capture_snapshot("puffy_text_gold");
  auto snap_ice = TestApp::capture_snapshot("puffy_text_ice");
  auto snap_fire = TestApp::capture_snapshot("puffy_text_fire");
}

TEST(puffy_text_interactive) {
  co_await TestApp::wait_for_frames(5);

  // Adjust depth slider
  TestApp::click_button("Depth Slider");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  auto snap = TestApp::capture_snapshot("puffy_text_slider_preview");
}
```

---

## Opacity Cascade

### Problem

There's no built-in way to set opacity on a parent and have it propagate multiplicatively to children. A parent at 50% opacity with a child at 50% opacity should yield 25% effective opacity.

### Suggested Implementation

```cpp
ComponentConfig{}
    .with_opacity(0.5f);  // 0.0 = fully transparent, 1.0 = fully opaque
```

Children inherit the parent's effective opacity and multiply by their own.

### Example Screen: OpacityShowcase

**File:** `src/systems/screens/OpacityShowcase.h`
**CLI:** `--screen=opacity`
**Category:** Visual Effects

#### Layout

1. **Opacity Levels** — A row of 5 identical colored panels at 100%, 75%, 50%, 25%, 10% opacity on a checkered background (so transparency is visible). Each labeled with its opacity value.

2. **Inheritance Demo** — A parent div at 50% opacity containing:
   - A child at 100% → effective 50%
   - A child at 50% → effective 25%
   - A child at 25% → effective 12.5%
   Each child labeled with both its own opacity and effective opacity.

3. **Interactive Slider** — A parent panel with an opacity slider (0–100%). Inside it, 3 children at fixed opacities. Dragging the slider shows how parent opacity multiplicatively affects all children in real time.

4. **Text Readability** — Text at 100%, 75%, 50%, 25% opacity on a dark background. Demonstrates when text becomes unreadable.

#### Features Exercised

- `with_opacity()` at various values
- Multiplicative parent→child opacity inheritance
- Real-time opacity changes via slider
- Opacity applied to text, backgrounds, borders

#### Verification

- 50% opacity panel is visibly half-transparent over checkered background
- Parent 50% × child 50% = 25% effective opacity (visually matches the 25% reference panel)
- Slider changes parent opacity and all children update simultaneously
- Text below ~25% opacity is nearly invisible

#### E2E Test Plan

**Test file:** `src/testing/tests/OpacityTest.h`

##### New Custom Commands Needed

None.

##### Screenshots

1. `opacity_levels` — 5 panels at decreasing opacity on checkered background
2. `opacity_inheritance` — parent at 50% with children showing effective opacities
3. `opacity_slider_full` — slider at 100%, all children fully visible
4. `opacity_slider_half` — slider at 50%, children dimmed
5. `opacity_text` — text at various opacity levels

##### Test Script

```cpp
TEST(opacity_levels_visible) {
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("100%");
  TestApp::expect_ui_exists("75%");
  TestApp::expect_ui_exists("50%");
  TestApp::expect_ui_exists("25%");
  TestApp::expect_ui_exists("10%");

  auto snap = TestApp::capture_snapshot("opacity_levels");
}

TEST(opacity_inheritance) {
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("Effective: 50%");
  TestApp::expect_ui_exists("Effective: 25%");
  TestApp::expect_ui_exists("Effective: 12.5%");

  auto snap = TestApp::capture_snapshot("opacity_inheritance");
}

TEST(opacity_interactive) {
  co_await TestApp::wait_for_frames(5);

  // Drag parent opacity slider to midpoint
  TestApp::click_button("Parent Opacity Slider");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  auto snap_full = TestApp::capture_snapshot("opacity_slider_full");

  // Move slider to ~50%
  test_input::move_mouse(slider_mid_x, slider_y);
  co_await TestApp::wait_for_frames(1);
  test_input::press_mouse_button(MouseButton::Left);
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  auto snap_half = TestApp::capture_snapshot("opacity_slider_half");
}
```

---

## Shadows

### Problem

No built-in shadow support. Elements can't have drop shadows with configurable offset, blur, and color.

### Suggested Implementation

```cpp
ComponentConfig{}
    .with_shadow(Color{0, 0, 0, 128}, offset_x, offset_y, blur_radius);
```

### Example Screen: ShadowShowcase

**File:** `src/systems/screens/ShadowShowcase.h`
**CLI:** `--screen=shadows`
**Category:** Visual Effects

#### Layout

1. **Hard vs Soft** — Two cards side by side on a light background:
   - Hard shadow: offset (4, 4), blur 0 — sharp edge
   - Soft shadow: offset (4, 4), blur 8 — feathered edge
   Labels indicate shadow type and parameters.

2. **Offset Direction** — A grid of 9 cards (3×3). The center card has no shadow. The 8 surrounding cards each have a shadow offset in their respective direction (top-left, top, top-right, etc.). Shows all offset directions at a glance.

3. **Blur Progression** — A row of 5 identical cards with blur values 0, 2, 4, 8, 16. Shows how blur affects shadow softness. Each labeled with its blur radius.

4. **Colored Shadows** — Three cards with different shadow colors: black (classic), blue (stylized), red (alert/danger). Demonstrates shadow color parameter.

5. **Layered Shadows** — A card with two shadows stacked: a tight dark shadow for depth + a wide soft shadow for ambient. Shows composing multiple shadow passes.

#### Features Exercised

- `with_shadow(color, offset_x, offset_y, blur_radius)`
- Hard shadows (blur = 0) vs soft shadows (blur > 0)
- All 8 offset directions
- Shadow color variations
- Multiple shadows on one element

#### Verification

- Hard shadow has pixel-sharp edges
- Soft shadow smoothly fades out
- Shadow direction matches offset parameters
- Higher blur = wider, softer shadow
- Colored shadows tint correctly

#### E2E Test Plan

**Test file:** `src/testing/tests/ShadowTest.h`

##### New Custom Commands Needed

None — visual-only, snapshot comparison is the primary verification.

##### Screenshots

1. `shadow_hard_vs_soft` — hard and soft shadow cards side by side
2. `shadow_directions` — 3×3 grid showing all offset directions
3. `shadow_blur_progression` — 5 cards with increasing blur
4. `shadow_colored` — black, blue, red shadow cards
5. `shadow_layered` — card with two composed shadows

##### Test Script

```cpp
TEST(shadow_hard_vs_soft) {
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("Hard Shadow");
  TestApp::expect_ui_exists("Soft Shadow");

  auto snap = TestApp::capture_snapshot("shadow_hard_vs_soft");
}

TEST(shadow_directions) {
  co_await TestApp::wait_for_frames(5);

  auto snap = TestApp::capture_snapshot("shadow_directions");
}

TEST(shadow_blur_progression) {
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("blur: 0");
  TestApp::expect_ui_exists("blur: 16");

  auto snap = TestApp::capture_snapshot("shadow_blur_progression");
}

TEST(shadow_colored) {
  co_await TestApp::wait_for_frames(5);

  auto snap = TestApp::capture_snapshot("shadow_colored");
}

TEST(shadow_layered) {
  co_await TestApp::wait_for_frames(5);

  auto snap = TestApp::capture_snapshot("shadow_layered");
}
```
