# Meters and Gauges

**Status:** Not implemented  
**Priority:** Medium

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

