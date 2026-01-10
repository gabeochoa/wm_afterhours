# UI Coverage Gaps

This document tracks which UI components have showcase screens and test coverage.

---

## Showcase Screen Coverage

Which components have dedicated demo screens vs. only appearing as part of other screens.

### Components with Dedicated/Primary Coverage

| Component | Dedicated Screen | Notes |
|-----------|-----------------|-------|
| `button` | `Buttons.h` | Comprehensive: sizes, states, themes |
| `button_group` | `Buttons.h` | Shown as part of button demos |
| `slider` | `Forms.h` | Multiple sliders with different configs |
| `checkbox` | `Forms.h` | Enabled, disabled, various themes |
| `dropdown` | `Forms.h` | Multiple dropdowns demonstrated |
| `div` | `Cards.h`, `ExampleLayout.h` | Layout containers shown extensively |
| Theme system | `Themes.h` | Theme switching demonstrated |
| Colors | `ExampleColors.h` | Theme color usage shown |
| Text/Typography | `ExampleText.h` | Font rendering demonstrated |
| Tabbing/Focus | `ExampleTabbing.h` | Keyboard navigation shown |
| CJK/Languages | `LanguageDemo.h` | Multi-language text rendering |

### Components Missing Showcase Screens

| Component | Currently Shown In | Needs Dedicated Screen | Priority |
|-----------|-------------------|----------------------|----------|
| `navigation_bar` | Not shown | Yes | High |
| `pagination` | Not shown | Yes | High |
| `checkbox_group` | Not shown | Yes | Medium |
| `image` | Not shown | Yes | Medium |
| `sprite` | Not shown | Yes | Medium |
| `icon_row` | Not shown | Yes | Medium |
| `image_button` | Not shown | Yes | Medium |
| Shadow effects | `Cards.h` (partial) | Expand coverage | Low |
| Focus clusters | Not explicitly | Yes | Medium |
| Disabled states | `Forms.h` (1 checkbox) | Expand coverage | Medium |
| Hidden elements | Not shown | Yes | Low |
| Opacity effects | Not shown | Yes | Low |
| Absolute positioning | Not shown | Yes | Low |

### Suggested New Showcase Screens

1. **`Navigation.h`** - Demonstrate `navigation_bar` and `pagination` components
   - Left/right navigation patterns
   - Page indicators
   - Keyboard control

2. **`Media.h`** - Demonstrate `image`, `sprite`, `icon_row`, `image_button`
   - Static images
   - Spritesheets
   - Icon grids
   - Clickable images

3. **`AdvancedForms.h`** - Demonstrate `checkbox_group` and complex form patterns
   - Checkbox groups with min/max selection
   - Form validation patterns
   - Multi-select scenarios

4. **`Effects.h`** - Demonstrate visual effects
   - Shadow types (hard vs soft)
   - Opacity variations
   - Focus clusters
   - Disabled states

---

## Test Coverage

Current test suite and what's missing.

### Currently Tested

| Test File | Components Tested | Interaction Type |
|-----------|------------------|------------------|
| `SimpleButtonClickTest.h` | `button` | Mouse click |
| `TabbingTest.h` | `button` | Keyboard (Tab, Enter) |
| `SnapshotTest.h` | `button` | Visual comparison |
| `FontConfigTest.h` | Font system | Configuration |

### Missing Test Coverage

#### Form Components

| Component | Mouse Interaction | Keyboard Interaction | State Changes | Priority |
|-----------|------------------|---------------------|---------------|----------|
| `checkbox` | Click to toggle | Enter to toggle | on/off state | **High** |
| `slider` | Drag handle | Left/Right arrows | value changes | **High** |
| `dropdown` | Click to open, select | Tab into, arrows | open/closed, selection | **High** |
| `navigation_bar` | Click arrows | Left/Right focus | index changes | Medium |
| `pagination` | Click pages | Tab, Enter | page changes | Medium |
| `checkbox_group` | Click items | Tab through | multi-select | Medium |
| `button_group` | Click items | Tab, Enter | which pressed | Medium |

#### Component States

| State | Description | Tested | Priority |
|-------|-------------|--------|----------|
| Disabled | `with_disabled(true)` | No | **High** |
| Hidden | `with_hidden(true)` | No | Medium |
| Focus | Focus ring appearance | Partial (tabbing test) | Medium |
| Hot/Hover | Mouse hover state | No | Medium |
| Active | Mouse down state | No | Low |

#### Visual Features

| Feature | Description | Tested | Priority |
|---------|-------------|--------|----------|
| Shadows | Hard/soft shadow rendering | No | Low |
| Opacity | Parent/child opacity inheritance | No | Low |
| Rounded corners | Per-corner configuration | No | Low |
| Theme colors | Correct color application | No | Medium |
| Auto text color | Contrast-based text color | No | Medium |

#### Complex Interactions

| Scenario | Description | Tested | Priority |
|----------|-------------|--------|----------|
| Focus clusters | Tab skips internal elements | No | Medium |
| Screen switching | Entities cleaned up | No | **High** |
| Dropdown close | Click outside closes | No (known gap) | Medium |
| Slider keyboard | Left/right adjusts value | No | Medium |
| Select on focus | Auto-select when focused | No | Low |

### Suggested New Tests

#### High Priority

1. **`CheckboxClickTest.h`**
   ```cpp
   TEST(checkbox_click) {
     // Click checkbox, verify state toggles
     // Click again, verify toggles back
     // Verify keyboard Enter also toggles
   }
   ```

2. **`SliderDragTest.h`**
   ```cpp
   TEST(slider_drag) {
     // Drag slider handle, verify value changes
     // Use keyboard left/right, verify value changes
     // Verify min/max clamping
   }
   ```

3. **`DropdownTest.h`**
   ```cpp
   TEST(dropdown_interaction) {
     // Click to open dropdown
     // Verify options appear
     // Click option, verify selection changes
     // Verify dropdown closes
   }
   ```

4. **`DisabledStateTest.h`**
   ```cpp
   TEST(disabled_buttons) {
     // Click disabled button, verify no action
     // Tab to disabled button, verify skipped or no action
   }
   ```

5. **`ScreenSwitchTest.h`**
   ```cpp
   TEST(screen_switch_cleanup) {
     // Load screen A
     // Tab to element
     // Switch to screen B
     // Verify focus resets, no stale entities
   }
   ```

#### Medium Priority

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

### Phase 3: Showcase Screens
9. Create `Navigation.h` showcase
10. Create `Media.h` showcase
11. Create `AdvancedForms.h` showcase
12. Expand disabled/hidden state demos

### Phase 4: Visual & Edge Cases
13. Focus cluster test
14. Screen switch cleanup test
15. Shadow/opacity snapshot tests
16. Theme color application tests

---

## Notes

- Snapshot tests are valuable for visual regression but require baseline images
- Mouse interaction tests should verify both click and release behavior
- Keyboard tests should cover Tab, Enter, Arrow keys, and Escape where applicable
- State tests should verify component state persists across frames

