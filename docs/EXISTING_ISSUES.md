# Existing Issues

This document tracks known issues with the test suite. These tests are expected to fail for now. Once all tests are set up, we can go in and fix them.

## Test Failures

### simple_button_click
**Status:** ❌ Failing  
**Error:** `Expected UI element to exist: Clicks: 1`

**Description:**  
The test clicks the "Click Me" button and expects to see "Clicks: 1" appear, but the UI element is not found. 

**Root Cause:** Elements have negative computed sizes (-1.0) due to autolayout not running/computing sizes (see "Autolayout Not Computing Sizes for Elements" below). This causes elements to be invisible, so the test cannot find them.

**Test Location:** `src/testing/tests/SimpleButtonClickTest.h`

### snapshot_test
**Status:** ❌ Failing  
**Error:** `Expected UI element to exist: Clicks: 1`

**Description:**  
Similar to `simple_button_click`, the snapshot test fails because it cannot find the "Clicks: 1" element after clicking the button. This test also attempts to capture and compare snapshots, but fails before reaching that point.

**Root Cause:** Same as `simple_button_click` - elements have negative sizes and are invisible. Additionally, the captured snapshot shows all elements with `computed_x: -1.0, computed_y: -1.0`, confirming autolayout is not computing sizes.

**Test Location:** `src/testing/tests/SnapshotTest.h`

### tabbing
**Status:** ❌ Failing  
**Error:** `Expected element to have focus, but no element is focused`

**Description:**  
The tabbing test simulates Tab key presses to navigate between buttons, but the focus system is not working as expected. After pressing Tab, no element receives focus, causing the test to fail when checking for focus on "Button 1".

**Test Location:** `src/testing/tests/TabbingTest.h`

## Other Issues

### Files Plugin Not Initialized
**Status:** ⚠️ Warning  
**Error:** `Files plugin not initialized. Call files::init() first.`

**Description:**  
The files plugin is not being initialized before use. This may affect file operations in tests, though it doesn't appear to be causing test failures directly.

### Settings File Not Found
**Status:** ⚠️ Warning  
**Error:** `Failed to find settings file (Read): "/Users/gabe/p/wm_afterhours/settings.json"`

**Description:**  
The application is looking for a settings file that doesn't exist. This is expected for a fresh setup, but the warning appears during test execution.

### JSON Config File Write Error
**Status:** ⚠️ Warning  
**Error:** `write_json_config_file error: Couldn't open file for writing: ""`

**Description:**  
An attempt to write a JSON config file is failing because the file path is empty. This may be related to the files plugin not being initialized.

## Library Issues (vendor/afterhours)

### Autolayout Not Computing Sizes for Elements
**Severity:** 🔴 Critical  
**Status:** Not Fixed

**Description:**  
The autolayout system is not computing sizes for UI components. All elements show `computed_x: -1.0` and `computed_y: -1.0`, which are the initial uncomputed values. This affects both absolutely positioned and normally positioned elements.

**Evidence from UI State JSON:**
```json
{
  "computed_x": -1.0,
  "computed_y": -1.0,
  "width": -1.0,
  "height": -1.0,
  "is_absolute": true
}
```

**Expected:** Elements should have computed sizes (e.g., `computed_x: 200.0, computed_y: 60.0`)

**Impact:**  
- Elements cannot render (negative sizes)
- UI elements are invisible
- Tests fail because elements can't be found
- Cannot verify autolayout calculations

**Related Files:**
- `vendor/afterhours/src/plugins/ui/systems.h:RunAutoLayout`
- `vendor/afterhours/src/plugins/autolayout.h`

**See Also:** `UI_LAYOUT_BUGS.md` Bug 2, 3, 4

---

### Negative Sizes Not Detected or Warned
**Severity:** 🔴 Critical  
**Status:** Not Fixed

**Description:**  
The UI system allows elements to have negative widths/heights without any warning, causing silent rendering failures. Elements with negative sizes are marked as `visible: false` and don't render, but there's no validation or error message.

**Evidence from UI State JSON:**
```json
{
  "width": -1.0,
  "height": -1.0,
  "visible": false
}
```

**Expected:**  
- Validation that warns/errors when sizes are negative
- Early detection after autolayout calculation
- Helpful error messages when elements don't render

**Impact:**  
- Silent failures - elements don't render without explanation
- Difficult to debug layout issues
- No feedback to developers about invalid sizes

**Related Files:**
- `vendor/afterhours/src/plugins/ui/systems.h:TrackIfComponentWillBeRendered`
- `vendor/afterhours/src/plugins/ui/components.h:UIComponent`

**See Also:** `afterhours_betterment.md` issue #22, `UI_LAYOUT_BUGS.md` Bug 3

---

### Autolayout May Not Run for Absolutely Positioned Elements
**Severity:** 🟡 Medium  
**Status:** Not Fixed

**Description:**  
Absolutely positioned elements (`is_absolute: true`) may not be going through the autolayout calculation process, or autolayout is not computing their sizes correctly. Even though they use `with_translate()` for positioning, they still need computed sizes to render.

**Evidence from UI State JSON:**
```json
{
  "is_absolute": true,
  "computed_x": -1.0,
  "computed_y": -1.0,
  "computed_rel_x": 0.0,
  "computed_rel_y": 0.0,
  "translate_x": 540.0,
  "translate_y": 330.0
}
```

**Expected:** Even absolutely positioned elements should have computed sizes

**Impact:**  
- Absolutely positioned elements may not render
- Size calculations are incorrect
- Layout verification fails

**Related Files:**
- `vendor/afterhours/src/plugins/ui/systems.h:RunAutoLayout`
- `vendor/afterhours/src/plugins/autolayout.h`

**See Also:** `UI_LAYOUT_BUGS.md` Bug 4

---

## Notes

- All three tests are currently failing, but this is expected during test setup
- Focus on getting the test infrastructure working first, then fix individual test failures
- The button click functionality appears to be the main issue affecting multiple tests
- The tabbing/focus system needs investigation to understand why focus is not being set
- **Root cause:** Autolayout not computing sizes is preventing elements from rendering, which causes all test failures

