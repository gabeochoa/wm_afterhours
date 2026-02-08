# Feature Request: with_row_layout() Shorthand

**Type:** Afterhours change (small, low-risk)
**File:** `vendor/afterhours/src/plugins/ui/component_config.h`
**Impact:** ~200 call-site simplifications across all screens

## Problem

Horizontal layout is the most common multi-call pattern in the codebase:

```cpp
.with_flex_direction(FlexDirection::Row)
.with_align_items(AlignItems::Center)
// sometimes also:
.with_gap(pixels(8))
```

This 2-3 call chain appears in virtually every screen, often multiple times.

## Proposed Solution

A single convenience method on `ComponentConfig`:

```cpp
// In component_config.h
ComponentConfig& with_row_layout(AlignItems align = AlignItems::Center) {
    return with_flex_direction(FlexDirection::Row).with_align_items(align);
}

// Overload with gap:
ComponentConfig& with_row_layout(Size gap, AlignItems align = AlignItems::Center) {
    return with_flex_direction(FlexDirection::Row).with_align_items(align).with_gap(gap);
}
```

## Usage

```cpp
// Before:
.with_flex_direction(FlexDirection::Row).with_align_items(AlignItems::Center).with_gap(pixels(8))

// After:
.with_row_layout(pixels(8))
```

## Notes

- This is a very small change to afterhours (adds 2 methods)
- Similar to how `with_absolute_position(x, y)` was added to combine absolute + translate
- Could add `with_column_layout()` for symmetry, though column is the default
