# Vendor UI Library Sizing Issues

## Overview

Several UI components in the vendor `afterhours` library have internal sizing that ignores or overrides the sizes specified in `ComponentConfig`. This causes layout warnings when the internal components exceed their parent container bounds.

## Identified Issues

### 1. Dropdown Component - Hardcoded 200x50 Button Group ✅ VERIFIED

**File:** `vendor/afterhours/src/plugins/ui/imm_components.h`

**Symptom:** 
```
Layout overflow: 'button group 0' extends outside parent 'dropdown button group' bounds 
(child_size=[200.0,50.0], parent_size=[199.1,152.0])
```

**Root Cause (VERIFIED):**
The `dropdown` component at line 1067 falls back to `default_component_size` when no explicit size is provided:
```cpp
config.size = ComponentSize(children(default_component_size.x),
                            pixels(default_component_size.y));
```

The `button_group` then inherits this config and creates internal buttons with the 200x50 default size.

**Debug Logging Output:**
```
button_group 'dropdown button group': input max_width.value=200, max_width.dim=1, 
max_height.value=50, labels.size=5
button_group 'dropdown button group': resulting button size x=200, y=50, flex_dir=Column
```

This confirms:
- `max_width.dim=1` (Dim::Pixels) - the size IS coming from hardcoded pixels
- `max_width.value=200` - exactly matches `default_component_size.x`
- The dropdown is NOT passing down the user-specified size to the button_group

**Fix:** Modify `dropdown()` to pass its resolved size to the button_group, or modify `button_group()` to respect the parent container width.

### 2. Toggle Switch Component - Font Size 0.0px ✅ VERIFIED

**File:** `vendor/afterhours/src/plugins/ui/imm_components.h`, Line 682

**Symptom:**
```
Font size 0.0px is below minimum accessible size 12.0px for component 'toggle_check' - clamping to minimum
```

**Root Cause (VERIFIED):**
The code uses `h720(12.0f).value` for font size:
```cpp
.with_font(UIComponent::DEFAULT_FONT, h720(12.0f).value)
```

But `h720()` is defined as:
```cpp
inline Size h720(const float px) { return screen_pct(px / 720.f); }
```

So `h720(12.0f)` returns `screen_pct(0.0167)`, and `.value` gives `0.0167` - NOT pixels!

**Debug Logging Output:**
```
toggle_check: h720(12.0f).value = 0.016666668 (should be ~12px, not a fraction)
```

This confirms:
- `h720(12.0f).value` returns `0.0167` (the screen_pct fraction)
- This is interpreted as 0.0167 pixels ≈ 0px
- The font system clamps this to 12px minimum, generating the warning

**Fix (already applied):** Changed from `h720(12.0f).value` to `12.0f` at line 682.

### 3. Toggle Switch - Container Height Becomes Negative ✅ VERIFIED

**Symptom:**
```
Container too small for text: container=704x-4, margins=5x5, text='Circle Style (Checkb...'
```

**Root Cause (VERIFIED):**
The `rect()` method in `ui_core_components.h` line 109:
```cpp
.height = computed[Axis::Y] - computed_margin[Axis::Y],
```

When `computed_margin[Axis::Y]` exceeds `computed[Axis::Y]`, the height becomes negative.

**Debug Logging Output:**
```
DEBUG rect(): id=19, computed=[704, 28], margin=[0, 32], result=[704, -4]
```

This confirms:
- Component has `computed[Y] = 28px` (the `pixels(28)` we specified)
- `computed_margin[Y] = 32px` (sum of top + bottom margins from `Spacing::xs` or similar)
- Result: `28 - 32 = -4px`

**Root Issue:** The component height (28px) is smaller than the total vertical margin (32px). This is a configuration issue, not a bug in the layout engine.

**Fix Options:**
1. Increase component height: Change `pixels(28)` to `pixels(40)` or larger
2. Reduce margins: Use smaller margin values
3. The `fmaxf(0.f, ...)` guard now prevents negative sizes from propagating

## Verification Logging Added

1. **`button_group()` sizing** - Lines 311, 326 in `imm_components.h`
   - Logs input `max_width.value`, `max_width.dim`, `max_height.value`, `labels.size`
   - Logs resulting button sizes
   
2. **`toggle_check` font size** - Line 680 in `imm_components.h`
   - Logs the actual value returned by `h720(12.0f).value`

3. **Container height** - Line 270 in `rendering.h`
   - Logs when container height is less than `2 * margin_px.y`

## Proposed Fixes

1. **Dropdown:** Modify the dropdown component to pass its own resolved width to the button_group config, not rely on inheritance from a default-sized config.

2. **Toggle Switch Font (FIXED):** Changed from `h720(12.0f).value` to `12.0f` to use actual pixel values instead of screen_pct fractions.

3. **Flex Overflow:** Add bounds checking to ensure computed sizes never become negative after margin/padding subtraction.

## Fixes Applied

| File | Fix | Status |
|------|-----|--------|
| `imm_components.h:682` | Changed `h720(12.0f).value` to `12.0f` | ✅ Applied |
| `ui_core_components.h:108-109` | Added `fmaxf(0.f, ...)` to clamp width/height to 0 minimum | ✅ Applied |
| `autolayout.h:301` | Added `fmaxf(0.f, parent_size)` to prevent negative parent sizes | ✅ Applied |

## Debug Logging Added (should be removed before production)

| File | Line | Purpose |
|------|------|---------|
| `imm_components.h` | 311, 326 | Log button_group sizing |
| `imm_components.h` | 680 | Log h720() font size issue |
| `rendering.h` | 270-275 | Log negative container heights |
| `ui_core_components.h` | 110-113 | Log when rect() produces negative sizes |
| `autolayout.h` | 297-301 | Log negative parent_size |

