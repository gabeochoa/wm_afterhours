# UI Layout Bugs Found via Snapshot Testing

This document tracks bugs discovered in the UI framework by analyzing the captured UI state JSON from snapshot tests.

**Note:** This document contains both program issues and library issues. See:
- `PROGRAM_ISSUES.md` - Issues with the wm_afterhours program
- `EXISTING_ISSUES.md` - Issues with the vendor/afterhours library

## Test Setup

**Test:** `snapshot_test`  
**Screen:** 1280x720  
**Expected Layout:**
- Button "Click Me": 200x60px, centered at (540, 330)
- Counter "Clicks: 0": 200x30px, at (540, 410) - 20px below button

## Bugs Discovered

### Bug 1: Translate Values Not Captured in UI State ✅ FIXED
**Severity:** 🔴 Critical  
**Status:** ✅ Fixed (translate values now captured)

**Description:**  
Elements using `with_translate()` have their translation stored in `HasUIModifiers` component. Initially this was not being captured in the UI state dump, but has now been added.

**Evidence from JSON (After Fix):**
```json
{
  "entity_id": 3,
  "debug_name": "test_button",
  "x": 0.0,
  "y": 0.0,
  "translate_x": 540.0,
  "translate_y": 330.0,
  "is_absolute": true
}
```

**Note:** The `x` and `y` values still show as 0.0 because translate is applied during rendering via `HasUIModifiers`, not stored in the base `UIComponent` position. The translate values are now captured separately, which is correct.

**Expected Positions (Verified):**
- Button: `translate_x: 540.0, translate_y: 330.0` ✅ Correct
- Counter: `translate_x: 540.0, translate_y: 410.0` ✅ Correct

**Impact:**  
- ✅ Can now verify correct positioning of absolutely positioned elements
- ✅ Translate values are captured for debugging
- ⚠️ Still need to account for translate when comparing positions (x + translate_x)

---

### Bug 2: Computed Sizes Not Calculated (-1.0 values)
**Severity:** 🔴 Critical  
**Status:** Not Fixed

**Description:**  
All elements show `computed_x: -1.0` and `computed_y: -1.0`, which is the initial uncomputed value. This suggests autolayout either:
1. Hasn't run yet when the snapshot is captured
2. Is failing to compute sizes for absolutely positioned elements
3. Is not running at all

**Evidence from JSON:**
```json
{
  "computed_x": -1.0,
  "computed_y": -1.0,
  "width": -1.0,
  "height": -1.0
}
```

**Expected:** Should show computed sizes (e.g., `computed_x: 200.0, computed_y: 60.0`)

**Impact:**  
- Cannot verify autolayout calculations
- Elements may not be rendering correctly
- Negative sizes cause rendering failures (see Bug 3)

---

### Bug 3: Negative Width/Height Causes Invisible Elements
**Severity:** 🔴 Critical  
**Status:** Not Fixed

**Description:**  
Elements with `width: -1.0` and `height: -1.0` are marked as `visible: false` and don't render. This is because the rendering system likely skips elements with invalid sizes.

**Evidence from JSON:**
```json
{
  "width": -1.0,
  "height": -1.0,
  "visible": false
}
```

**Expected:** Elements should have positive widths/heights and be visible

**Impact:**  
- UI elements don't appear on screen
- Tests fail because elements can't be found
- No warning or error when sizes are invalid

**Related Issue:** See `afterhours_betterment.md` issue #22 "Negative Sizes Not Detected or Warned"

---

### Bug 4: Autolayout May Not Run for Absolutely Positioned Elements
**Severity:** 🟡 Medium  
**Status:** Not Fixed

**Description:**  
Absolutely positioned elements (`is_absolute: true`) may not be going through the autolayout calculation process, or autolayout is not computing their sizes correctly. The `computed_x` and `computed_y` values remain at -1.0.

**Evidence from JSON:**
```json
{
  "is_absolute": true,
  "computed_x": -1.0,
  "computed_y": -1.0,
  "computed_rel_x": 0.0,
  "computed_rel_y": 0.0
}
```

**Expected:** Even absolutely positioned elements should have computed sizes

**Impact:**  
- Absolutely positioned elements may not render
- Size calculations are incorrect
- Layout verification fails

---

### Bug 5: UI State Capture Timing Issue
**Severity:** 🟡 Medium  
**Status:** Not Fixed

**Description:**  
The snapshot is captured after only 5 frames (`wait_for_frames(5)`), which may not be enough time for:
1. Autolayout to complete
2. All UI systems to initialize
3. Components to be fully set up

**Evidence:** All elements show uninitialized values (-1.0, 0.0)

**Expected:** After sufficient frames, elements should have computed values

**Impact:**  
- Snapshots may capture incomplete state
- Tests may be flaky depending on timing
- Need to verify if more frames would fix the issue

---

## Missing Data in UI State Capture

### HasUIModifiers ✅ NOW CAPTURED
The `HasUIModifiers` component contains:
- `translate_x` - X translation offset ✅ Now captured
- `translate_y` - Y translation offset ✅ Now captured
- `scale` - Scale factor ✅ Now captured

**Status:** ✅ Fixed - All translate and scale values are now captured in the UI state

### Desired Sizes Not Captured
The `UIComponent` has `desired[Axis::X]` and `desired[Axis::Y]` which contain the requested size before computation. This would be useful for debugging.

**Action Needed:** Consider adding `desired_width` and `desired_height` to the capture

---

## Next Steps

1. ✅ Enhanced UI state capture to include margin, padding, absolute positioning, hierarchy
2. ✅ Add `HasUIModifiers` (translate_x, translate_y, scale) to capture
3. ⏳ Investigate why autolayout isn't computing sizes (Bug 2, 4)
4. ⏳ Verify if more frames are needed before capturing (Bug 5)
5. ⏳ Check if absolutely positioned elements need special autolayout handling (Bug 4)
6. ⏳ Add validation/warnings for negative sizes (Bug 3)
7. ⏳ Consider adding desired_width/desired_height to capture for debugging

---

## Test Results Summary

**Test:** `snapshot_test`  
**Status:** ❌ Failing  
**Failure Point:** Cannot find "Clicks: 1" element after button click

**Root Cause Analysis:**
1. ✅ Translate values now captured correctly (translate_x: 540.0, translate_y: 330.0)
2. ❌ Elements have negative sizes (-1.0) → not visible (Bug 2, 3)
3. ❌ Autolayout may not be running → sizes not computed (Bug 2, 4)
4. ❌ Elements not rendering → test can't find them (Bug 3)

**Expected Behavior:**
- Button should render at (540, 330) with size 200x60
- Counter should render at (540, 410) with size 200x30
- Both should be visible and clickable

