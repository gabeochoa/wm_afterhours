# Program Issues (wm_afterhours)

This document tracks issues with the wm_afterhours program itself (not the vendor/afterhours library).

## Test Infrastructure Issues

### Test Timing May Be Insufficient
**Severity:** 🟡 Medium  
**Status:** Not Fixed

**Description:**  
The snapshot test waits only 5 frames before capturing the UI state. This may not be enough time for:
1. Autolayout to complete its calculations
2. All UI systems to fully initialize
3. Components to be fully set up

**Evidence:**  
- All elements show uninitialized values (`computed_x: -1.0`, `computed_y: -1.0`)
- Elements are not visible even though translate values are correct

**Location:** `src/testing/tests/SnapshotTest.h:8`

**Suggested Fix:**
- Increase `wait_for_frames(5)` to a higher value (e.g., 10-20 frames)
- Or add a condition-based wait that checks if autolayout has completed
- Consider waiting for elements to have valid computed sizes before capturing

**Impact:**  
- Snapshots may capture incomplete state
- Tests may be flaky depending on timing
- May be masking library issues (autolayout not running)

---

### Missing Desired Size Capture in UI State
**Severity:** 🟢 Low  
**Status:** Enhancement

**Description:**  
The UI state capture doesn't include `desired_width` and `desired_height` from `UIComponent.desired[Axis]`. These values show what size was requested before autolayout computation, which would be useful for debugging layout issues.

**Location:** `src/testing/test_snapshot.cpp:capture_ui_state()`

**Suggested Fix:**
- Add `desired_width` and `desired_height` fields to `UIState::Element`
- Capture `ui_comp.desired[afterhours::ui::Axis::X]` and `ui_comp.desired[afterhours::ui::Axis::Y]`

**Impact:**  
- Would help debug autolayout by comparing desired vs computed sizes
- Low priority - nice to have for debugging

---

## Test Setup Issues

### Test System Registration Order
**Severity:** 🟢 Low  
**Status:** Investigation Needed

**Description:**  
The test system is registered before UI systems in `game.cpp`. This might affect the order of system execution, but needs investigation to confirm if it's causing issues.

**Location:** `src/game.cpp:run_test()`

**Note:** This may not be an issue, but worth verifying that system execution order is correct.

---

## Notes

- Most critical issues are in the library (see `EXISTING_ISSUES.md`)
- Program issues are mostly related to test infrastructure and timing
- The main blocker is library autolayout not computing sizes (library issue)

