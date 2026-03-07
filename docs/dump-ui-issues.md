# dump_ui Issues: simple_button screen

Found via `dump_ui` E2E test on the `simple_button` screen.
Debug logging in `src/game.cpp` dump callback validates each issue.

---

## Issue 1: X-axis pixel values had fractional computed sizes

**Observed:** Elements sized with `pixels()` had their X-axis `computed` value
reduced by a consistent factor (~0.44%), producing fractional pixel values.
Y-axis was unaffected because 720 divides evenly.

```
BEFORE FIX:
CARD:    desired=(400.00, 440.00) computed=(398.222229, 440.000000)
DIVIDER: desired=(300.00, 1.00)   computed=(298.666656, 0.000000)
```

**Root cause:** Grid snapping in `autolayout.h` (`snap_to_8pt_grid`).

The grid unit was `4.0 * (screen_value / 720.0)`, producing a fractional
grid unit on X-axis: `4.0 * (1280/720) = 64/9 ≈ 7.1111`. Snapping to a
fractional grid unit produces fractional results.

**Fix:** Round the grid unit to the nearest integer (`autolayout.h:231-244`):
```cpp
float grid_unit = fmaxf(1.f, std::round(GRID_UNIT_720P * (screen_value / 720.0f)));
```

At 1280px width, grid unit is now 7 (integer). All snapped values are integers.

```
AFTER FIX:
CARD:    desired=(400.00, 440.00) computed=(399.000000, 440.000000)
DIVIDER: desired=(300.00, 1.00)   computed=(301.000000, 4.000000)
```

**Status:** Fixed.

---

## Issue 2: Divider height `pixels(1)` computed to exactly 0

**Observed:** The divider element had `desired=(300, 1)` but `computed=(299, 0)`.
The 1px height was snapped to 0 by grid snapping because `round(1/4) = 0`.

**Root cause:** `snap_to_8pt_grid` with Y-axis grid unit of 4.0px:
`round(1.0 / 4.0) * 4.0 = 0 * 4.0 = 0.0`

**Fix:** Added a floor in `snap_to_8pt_grid` — positive values never snap to zero:
```cpp
if (value > 0.f && snapped == 0.f)
    snapped = grid_unit;
```

The divider now computes as `h=4` (snapped up to the grid unit) instead of `h=0`.

**Status:** Fixed.

---

## Issue 3: Auto-derived debug names from labels

**Observed:** Elements without an explicit `with_debug_name()` call get their
debug name auto-derived from their label text:

- `div` with label "Simple Button Demo" → `name="Simple Button Demo"`
- `button` with label "Click Me!" → `name="Click Me! button"`

**Root cause:** `component_init.h:532-538` — when `config.debug_name` is empty
but `config.label` is not, the framework auto-sets the debug name:
- For divs (no component type string): `name = label`
- For buttons: `name = label + " button"`

Both get `UIComponentDebug::Type::custom`, so `dump_ui_node` emits them as
`name="..."` attributes indistinguishable from explicitly-set names.

**Impact:** Not a bug, but makes it harder to distinguish "intentionally named"
elements from "happened to have a label" elements in dump output. Could add a
`name_source` field to UIComponentDebug if needed.
