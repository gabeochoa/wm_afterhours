# Test Coverage Gaps

**Status:** Documentation  
**Priority:** High

This tracks test coverage for UI components and identifies missing tests.

---

## Currently Tested

| Test File | Components Tested | Interaction Type |
|-----------|------------------|------------------|
| `SimpleButtonClickTest.h` | `button` | Mouse click |
| `TabbingTest.h` | `button` | Keyboard (Tab, Enter) |
| `SnapshotTest.h` | `button` | Visual comparison |
| `FontConfigTest.h` | Font system | Configuration |

---

## Missing Test Coverage

### Form Components

| Component | Mouse Interaction | Keyboard Interaction | State Changes | Priority |
|-----------|------------------|---------------------|---------------|----------|
| `checkbox` | Click to toggle | Enter to toggle | on/off state | **High** |
| `slider` | Drag handle | Left/Right arrows | value changes | **High** |
| `dropdown` | Click to open, select | Tab into, arrows | open/closed, selection | **High** |
| `navigation_bar` | Click arrows | Left/Right focus | index changes | Medium |
| `pagination` | Click pages | Tab, Enter | page changes | Medium |
| `checkbox_group` | Click items | Tab through | multi-select | Medium |
| `button_group` | Click items | Tab, Enter | which pressed | Medium |

### Component States

| State | Description | Tested | Priority |
|-------|-------------|--------|----------|
| Disabled | `with_disabled(true)` | No | **High** |
| Hidden | `with_hidden(true)` | No | Medium |
| Focus | Focus ring appearance | Partial (tabbing test) | Medium |
| Hot/Hover | Mouse hover state | No | Medium |
| Active | Mouse down state | No | Low |

### Visual Features

| Feature | Description | Tested | Priority |
|---------|-------------|--------|----------|
| Shadows | Hard/soft shadow rendering | No | Low |
| Opacity | Parent/child opacity inheritance | No | Low |
| Rounded corners | Per-corner configuration | No | Low |
| Theme colors | Correct color application | No | Medium |
| Auto text color | Contrast-based text color | No | Medium |

### Complex Interactions

| Scenario | Description | Tested | Priority |
|----------|-------------|--------|----------|
| Focus clusters | Tab skips internal elements | No | Medium |
| Screen switching | Entities cleaned up | No | **High** |
| Dropdown close | Click outside closes | No (known gap) | Medium |
| Slider keyboard | Left/right adjusts value | No | Medium |
| Select on focus | Auto-select when focused | No | Low |

---

## Suggested New Tests

### High Priority

#### 1. `CheckboxClickTest.h`

```cpp
TEST(checkbox_click) {
  // Click checkbox, verify state toggles
  // Click again, verify toggles back
  // Verify keyboard Enter also toggles
}
```

#### 2. `SliderDragTest.h`

```cpp
TEST(slider_drag) {
  // Drag slider handle, verify value changes
  // Use keyboard left/right, verify value changes
  // Verify min/max clamping
}
```

#### 3. `DropdownTest.h`

```cpp
TEST(dropdown_interaction) {
  // Click to open dropdown
  // Verify options appear
  // Click option, verify selection changes
  // Verify dropdown closes
}
```

#### 4. `DisabledStateTest.h`

```cpp
TEST(disabled_buttons) {
  // Click disabled button, verify no action
  // Tab to disabled button, verify skipped or no action
}
```

#### 5. `ScreenSwitchTest.h`

```cpp
TEST(screen_switch_cleanup) {
  // Load screen A
  // Tab to element
  // Switch to screen B
  // Verify focus resets, no stale entities
}
```

### Medium Priority

6. **`NavigationBarTest.h`** - Test arrow clicks and keyboard navigation
7. **`CheckboxGroupTest.h`** - Test multi-select with min/max constraints
8. **`FocusClusterTest.h`** - Test tab navigation skips cluster internals
9. **`ThemeColorTest.h`** - Snapshot tests for correct theme application

---

## Coverage Summary

| Category | Covered | Missing | Coverage % |
|----------|---------|---------|------------|
| Button interactions | 2 | 1 (group) | 67% |
| Form components | 0 | 6 | 0% |
| Component states | 0 | 5 | 0% |
| Visual features | 0 | 5 | 0% |
| Complex interactions | 0 | 5 | 0% |
| **Total** | **2** | **22** | **~8%** |

---

## Priority Order for Adding Coverage

### Phase 1: Core Form Components (High Impact)
1. Checkbox click and keyboard test
2. Slider drag and keyboard test
3. Dropdown open/select/close test
4. Disabled state behavior test

### Phase 2: Navigation Components
5. Navigation bar test
6. Pagination test
7. Button group test
8. Checkbox group test

### Phase 3: Visual & Edge Cases
9. Focus cluster test
10. Screen switch cleanup test
11. Shadow/opacity snapshot tests
12. Theme color application tests

---

## Notes

- Snapshot tests are valuable for visual regression but require baseline images
- Mouse interaction tests should verify both click and release behavior
- Keyboard tests should cover Tab, Enter, Arrow keys, and Escape where applicable
- State tests should verify component state persists across frames

