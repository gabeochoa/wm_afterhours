# Feature Request: .with_transparent_bg()

**Type:** Afterhours vendor change
**File:** `vendor/afterhours/src/plugins/ui/component_config.h`
**Decision:** YES -- approved for implementation
**Impact:** ~19 call sites across 9 screens, plus improved DX for newcomers

## Problem

Text elements often require a background color to render correctly (see doc 07). The workaround is setting a transparent background:

```cpp
.with_background(afterhours::Color{0, 0, 0, 0})
```

This magic constant appears 19 times across 9 screens. It's not obvious to newcomers why it's needed, and `Color{0, 0, 0, 0}` doesn't communicate intent.

## Proposed Solution

Add a convenience method to `ComponentConfig`:

```cpp
// In component_config.h
ComponentConfig& with_transparent_bg() {
    return with_background(afterhours::Color{0, 0, 0, 0});
}
```

## Usage

```cpp
// Before:
.with_background(afterhours::Color{0, 0, 0, 0})

// After:
.with_transparent_bg()
```

## Notes

- Trivial implementation -- one-liner that delegates to existing method
- Makes intent explicit: "I need a background for rendering, but I want it invisible"
- Helps newcomers who hit the "text doesn't render without background" issue (doc 07)
- No breaking changes
- Consider also fixing the root cause in the renderer so text elements don't need a background at all (separate issue)
