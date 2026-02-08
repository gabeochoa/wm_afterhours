# Feature Request: ComponentConfig Presets

**Type:** Project-level helpers (no afterhours changes needed)
**File:** `src/ui_helpers.h`
**Impact:** ~30-50 lines saved across screens with repeated config patterns

## Problem

Screens frequently repeat the same multi-property `ComponentConfig` chains for common element types: section headers, row backgrounds, icon containers, screen titles, etc.

```cpp
// This exact pattern (or near-identical) appears 5+ times in ParcelCorpsSettings alone:
ComponentConfig{}
    .with_background(Color{30, 30, 40, 255})
    .with_rounded_corners(RoundedCorners())
    .with_roundness(0.08f)
    .with_size(ComponentSize{pixels(600), pixels(52)})
```

## Proposed Solution

Named preset functions that return pre-configured `ComponentConfig` objects:

```cpp
// src/ui_helpers.h

// Common row background preset
inline ComponentConfig row_bg(float w = 600, float h = 52, Color bg = {30, 30, 40, 255}) {
    return ComponentConfig{}
        .with_background(bg)
        .with_rounded_corners(RoundedCorners())
        .with_roundness(0.08f)
        .with_size(ComponentSize{pixels(static_cast<int>(w)), pixels(static_cast<int>(h))});
}

// Screen title text preset
inline ComponentConfig screen_title(const std::string& label, float font_size = 36.0f) {
    return ComponentConfig{}
        .with_label(label)
        .with_font("EqProRounded", font_size)
        .with_custom_text_color(Color{255, 255, 255, 255});
}
```

## Usage

```cpp
// Before (5 lines):
div(ctx, mk(e, 10),
    ComponentConfig{}.with_background({30,30,40,255})
        .with_rounded_corners(RoundedCorners()).with_roundness(0.08f)
        .with_size(ComponentSize{pixels(600), pixels(52)}));

// After (1 line):
div(ctx, mk(e, 10), row_bg());
```

## Notes

- Presets should be screen-agnostic (colors/sizes via params with sensible defaults)
- Screens can still override individual properties after calling the preset via chaining
- Start small; add presets only for patterns that repeat 3+ times across 2+ screens
