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

---

## 3D/Puffy Text Effect

### Problem

Cannot create 3D "puffy" text effects with depth/volume.

### Suggested Implementation

```cpp
ComponentConfig{}
    .with_puffy_text(outline_color, highlight_color, shadow_color, depth);
```

