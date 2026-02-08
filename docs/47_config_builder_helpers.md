# Feature Request: Config Builder Helpers

**Type:** Project-level helpers (no afterhours changes needed)
**File:** `src/ui_helpers.h` (new)
**Impact:** ~100 lines saved across all screens

## Problem

Several `ComponentConfig` patterns are repeated hundreds of times across screens:

1. **Pill shape**: `.with_rounded_corners(RoundedCorners()).with_roundness(0.5f)` — 2 calls for one intent
2. **Circle shape**: `.with_rounded_corners(RoundedCorners()).with_roundness(1.0f)` — same, even more common
3. **Size from floats**: `ComponentSize{pixels(static_cast<int>(w)), pixels(static_cast<int>(h))}` — verbose for a common operation
4. **Row layout**: `.with_flex_direction(FlexDirection::Row).with_align_items(AlignItems::Center)` — 2+ calls for common layout

## Proposed Solution

Free functions that return or modify `ComponentConfig`, usable inline:

```cpp
// src/ui_helpers.h
#pragma once
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Shape helpers - modify config in-place, return reference for chaining
inline ComponentConfig& as_pill(ComponentConfig& cfg, float roundness = 0.5f) {
    return cfg.with_rounded_corners(RoundedCorners()).with_roundness(roundness);
}

inline ComponentConfig& as_circle(ComponentConfig& cfg) {
    return cfg.with_rounded_corners(RoundedCorners()).with_roundness(1.0f);
}

// Size helper - returns ComponentSize from two floats
inline ComponentSize size_px(float w, float h) {
    return ComponentSize{pixels(static_cast<int>(w)), pixels(static_cast<int>(h))};
}
```

## Usage

```cpp
// Before:
.with_size(ComponentSize{pixels(static_cast<int>(icon_size)), pixels(static_cast<int>(icon_size))})
.with_rounded_corners(RoundedCorners())
.with_roundness(1.0f)

// After:
.with_size(size_px(icon_size, icon_size))
// + as_circle(cfg) applied to the config
```

## Notes

- These are NOT decorators (decorators add child elements; these modify config)
- Could later be promoted to afterhours if proven useful
- `pxf()` lambda already handles `pixels(static_cast<int>())` inside individual screens, but `size_px()` wraps the full `ComponentSize` construction
