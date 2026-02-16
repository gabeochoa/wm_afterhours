# Color System

**Status:** Implemented
**Priority:** Low
**Source:** break_ross migration experience

---

## Summary

All core color system features have been implemented in `vendor/afterhours/src/plugins/color.h`.

### What's implemented

- **Named colors:** `white()`, `black()`, `transparent()`, `gray()`, `gray_25()`, `gray_50()`, `gray_75()`, `error()`, `warning()`, `success()`, `info()`
- **Constructors:** `rgb()`, `rgba()`, `hex()`
- **Manipulation:** `darken()`, `lighten()`, `lerp()`, `mix()`, `set_opacity()`, `opacity_pct()`, `get_opposite()`
- **Accessibility (WCAG 2.1):** `luminance()`, `brightness()`, `is_light()`, `is_dark()`, `contrast_ratio()`, `wcag_compliance()`, `meets_wcag_aa()`, `meets_wcag_aaa()`, `auto_text_color()`, `ensure_contrast()`
- **Theme::Usage enum:** `Font`, `DarkFont`, `FontMuted`, `Background`, `Surface`, `Primary`, `Secondary`, `Accent`, `Error`, `Focus`, `Custom`, `Default`, `None`
- **ComponentConfig API:** `with_background(Theme::Usage)`, `with_custom_background(Color)`, `with_custom_text_color(Color)`, `with_auto_text_color()`, `with_text_color(Theme::Usage)`
- **Deprecated API:** `with_color_usage()` and `with_custom_color()` still work but are marked `[[deprecated]]`

### Not implemented (deferred)

- **Color palette presets** (`palettes::material_blue()`, `palettes::nord()`, etc.) — not needed yet
- **ColorSystemShowcase screen** — no showcase screen exists for this feature
