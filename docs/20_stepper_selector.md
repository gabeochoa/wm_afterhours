# Stepper/Selector with Arrows

**Status:** Not implemented  
**Priority:** High

## Problem

The pattern `< value >` with left/right arrows to cycle through discrete options is reimplemented in almost every settings screen.

## Screens Using This Pattern
- ParcelCorps: `render_language_row()`, `render_selector_row()`
- MiniMotorways: Sensitivity selector
- FlightOptions, KirbyOptions, DeadSpace, etc.

## Suggested Implementation

```cpp
template <typename Container>
ElementResult stepper(HasUIContext auto &ctx, EntityParent ep_pair,
                      const Container &options, size_t &option_index,
                      ComponentConfig config = ComponentConfig());

// Numeric variants
ElementResult stepper_int(HasUIContext auto &ctx, EntityParent ep_pair,
                          int &value, int min, int max, int step = 1,
                          ComponentConfig config = ComponentConfig());

ElementResult stepper_float(HasUIContext auto &ctx, EntityParent ep_pair,
                            float &value, float min, float max, float step = 0.1f,
                            ComponentConfig config = ComponentConfig());
```

## Features Needed
- [-] and [+] buttons on sides
- Current value display in center
- Long-press for rapid increment
- Keyboard: left/right arrows when focused

