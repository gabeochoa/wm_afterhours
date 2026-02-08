# Vendor Features -- Needs Design

These ideas were identified during code review but need more design work before implementation.

## 1. `.with_text()` combo method (from ParcelCorpsSettings)

**Status:** MAYBE -- users should probably use themes instead

**Idea:** Combine `.with_label()` + `.with_font()` + `.with_custom_text_color()` into one call.

**Open question:** If themes are working correctly, users shouldn't need to specify font and color on every element. The real fix might be improving theme adoption / making themes easier to use rather than adding another shorthand. Investigate why screens aren't using themes more.

---

## 2. Generic `fallback()` higher-order component (from CozyCafe)

**Status:** MAYBE -- needs design on the generic fallback API

**Idea:** Instead of a sprite-specific `sprite_or_fallback()`, design a generic fallback pattern:

```cpp
// Possible APIs:
fallback(primary_component, fallback_component)
fallback(component, spinner)
fallback(component, nothing)
```

**Open questions:**
- What does "can render" mean generically? For sprites it's `tex.id != 0`, but what about other components?
- Does the fallback receive the same config as the primary?
- Is this a wrapper component, a decorator, or a free function?
- Could this tie into a loading/async state system?

---

## 3. `with_720p_size(w, h)` resolution-aware size shorthand (from NeonStrike)

**Status:** MAYBE -- needs design on reference resolution

**Idea:** Instead of raw pixel sizes, tie sizes to the 720p reference resolution that `h720()` already uses:

```cpp
.with_720p_size(200.f, 50.f)  // 200x50 at 720p, scales on other resolutions
```

**Open questions:**
- Should it be `with_720p_size` specifically, or a more general `with_ref_size(w, h, ref_height)`?
- What about width? `h720()` is height-based. Do we need `w1280()` parity?
- Does this replace `pxf()` lambdas entirely or complement them?
- Should `ComponentSize` itself have a `from_720p(w, h)` static factory?

---

## 4. `TextStyle` struct with enum-based keys (from ShopInterface)

**Status:** MAYBE -- needs design on enum-to-style mapping

**Idea:** Bundle font + size + color + alignment into a reusable `TextStyle`, keyed by a user-defined enum for fast lookup:

```cpp
enum class MyStyles { Body, Header, Caption };

theme.set_text_style(MyStyles::Body, {"EqProRounded", 14.f, white});
// ...
.with_text_style(MyStyles::Body)
```

**Open questions:**
- How does afterhours accept a user-defined enum generically? Template parameter? Integer cast?
- Is this just an extension of the Theme system?
- Performance: array lookup by enum value is O(1), but how to size the array for an unknown enum?
- Should `TextStyle` be a separate concept or folded into `Theme::Usage`?
- Related: `ComponentConfig::inherit_style()` (ParcelCorps V4) is the same design space -- reusing a bundle of styling per-config rather than screen-global. Design these together.

---

## 5. Default font per-screen / Theme font inheritance (from CozyCafe)

**Status:** MAYBE -- investigate whether theme font inheritance already works

**Idea:** CozyCafe sets `.with_font("Gaegu-Bold", h720(X))` on nearly every element. If the Theme system already supports default font inheritance, this is just a docs/migration problem. If it doesn't, it's a feature gap.

**Next step:**
- Check if `theme.font` / `Theme::Usage` already applies a default font to elements without `.with_font()`
- If yes: add docs explaining how, migrate CozyCafe and similar screens
- If no: design a `theme.default_font` mechanism

---

## 6. `ComponentConfig::inherit_style()` (from ParcelCorps)

**Status:** MAYBE -- same design space as TextStyle (#4)

**Idea:** Copy visual properties (font, colors, alignment, roundness) from one config to another:

```cpp
auto base = ComponentConfig{}.with_font("X", 14).with_custom_text_color(white);
// Later:
ComponentConfig::inherit_from(base).with_label("Score")
```

**Open questions:**
- Which properties does "style" include? Font, color, alignment, roundness? Or everything except size/position?
- Is this just `TextStyle` with a different name?
- Should this be a copy constructor pattern or an explicit method?
- Design alongside TextStyle (#4) to avoid overlapping solutions
