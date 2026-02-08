# Feature Request: Margin/Padding Static Factory Methods

**Type:** Afterhours vendor change
**File:** `vendor/afterhours/src/plugins/ui/component_config.h` (Margin/Padding structs)
**Decision:** YES -- approved for implementation
**Impact:** ~170 Margin constructions simplified across 21 screens

## Problem

Setting a single-side margin or padding requires verbose designated initializer syntax:

```cpp
.with_margin(Margin{.bottom = DefaultSpacing::small()})
.with_margin(Margin{.left = DefaultSpacing::medium()})
.with_margin(Margin{.top = pixels(0), .bottom = DefaultSpacing::small(),
                    .left = DefaultSpacing::tiny(), .right = DefaultSpacing::tiny()})
```

This appears ~170 times across 21 screens. The single-side case (which is ~80% of uses) is especially verbose for what it does.

## Proposed Solution

Add static factory methods to `Margin` and `Padding`:

```cpp
struct Margin {
    Size top{}, bottom{}, left{}, right{};

    // Single-side factories
    static Margin top(Size v)    { return Margin{.top = v}; }
    static Margin bottom(Size v) { return Margin{.bottom = v}; }
    static Margin left(Size v)   { return Margin{.left = v}; }
    static Margin right(Size v)  { return Margin{.right = v}; }

    // Axis factories
    static Margin vertical(Size v)   { return Margin{.top = v, .bottom = v}; }
    static Margin horizontal(Size v) { return Margin{.left = v, .right = v}; }

    // Uniform factory
    static Margin all(Size v) { return Margin{.top = v, .bottom = v, .left = v, .right = v}; }
};

// Same for Padding struct
```

## Usage

```cpp
// Before:
.with_margin(Margin{.bottom = DefaultSpacing::small()})
.with_margin(Margin{.left = DefaultSpacing::medium()})

// After:
.with_margin(Margin::bottom(DefaultSpacing::small()))
.with_margin(Margin::left(DefaultSpacing::medium()))

// Multi-side:
// Before:
.with_margin(Margin{.top = pixels(0), .bottom = DefaultSpacing::small(),
                    .left = DefaultSpacing::tiny(), .right = DefaultSpacing::tiny()})
// After:
.with_margin(Margin::vertical(DefaultSpacing::small()).horizontal(DefaultSpacing::tiny()))
```

## Notes

- Designated initializer syntax still works for complex cases
- No breaking changes -- purely additive
- Applies identically to both `Margin` and `Padding` structs
