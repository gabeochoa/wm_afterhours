# Afterhours UI Library Gaps

Issues and limitations discovered during UI Showcase development.

---

## 1. Font Configuration Not in Theme

**Location:** Theme struct in `vendor/afterhours/src/plugins/ui/theme.h`

**Issue:** Themes define colors but not fonts. Each screen must manually configure fonts, which cannot be changed when switching themes.

**Current Workaround:**
```cpp
// In each screen's setup:
// TODO: Add font configuration when fonts are selected
auto theme = afterhours::ui::theme_presets::ocean_navy();
```

**Suggested Fix:** Add font family and default sizes to the `Theme` struct:
```cpp
struct Theme {
    // ... existing color fields ...
    std::string font_family;
    float font_size_sm;
    float font_size_md;
    float font_size_lg;
};
```

---

## 2. Dropdown Close Behavior

**Location:** `vendor/afterhours/src/plugins/ui/imm_components.h` (line ~760)

**Issue:** No built-in way to close dropdown when clicking outside of it. Dropdowns remain open until user selects an option.

**Current Workaround:** None available - dropdowns stay open.

**Suggested Fix:** Add click-outside detection that closes any open dropdown.

---

## 3. Circular Dependency with Styling Defaults

**Location:** `vendor/afterhours/src/plugins/ui/context.h` (lines 60-61)

**Issue:** `UIStylingDefaults` cannot be stored in `UIContext` due to circular dependency.

**Code Comment:**
```cpp
// TODO: Add styling defaults back when circular dependency is resolved
// imm::UIStylingDefaults styling_defaults;
```

**Suggested Fix:** Refactor to break the circular dependency, possibly by forward declarations or moving types.

---

## 4. Focus Ring Positioning on Checkbox

**Location:** `vendor/afterhours/src/plugins/ui/imm_components.h` (line ~220)

**Issue:** Checkbox focus ring is drawn on the wrong element. The clickable area is `checkbox_no_label` but focus ring appears elsewhere.

**Code Comment:**
```cpp
// TODO the focus ring is not correct because the actual clickable area is the
// checkbox_no_label element, not the checkbox element.
```

**Suggested Fix:** Move focus ring rendering to the actual clickable element.

---

## 5. Checkbox Corner Config Merging

**Location:** `vendor/afterhours/src/plugins/ui/imm_components.h` (line ~258)

**Issue:** No way to merge user-provided corner configuration with internal checkbox styling.

**Code Comment:**
```cpp
// TODO - if the user wants to mess with the corners, how can we merge
// these
```

**Suggested Fix:** Allow corner config to be passed through and merged with component defaults.

---

## 6. Tabbing with Value Controls

**Location:** `vendor/afterhours/src/plugins/ui/context.h` (line ~123)

**Issue:** Unclear how to handle components that want to use Widget Value Down/Up to control values while also participating in tabbing navigation.

**Code Comment:**
```cpp
// TODO How do we handle something that wants to use
// Widget Value Down/Up to control the value?
```

**Suggested Fix:** Add a mode or flag to distinguish between navigation and value adjustment contexts.

---

## Priority

| Gap | Severity | User Impact |
|-----|----------|-------------|
| Font in Theme | Medium | Theming feels incomplete |
| Dropdown close | High | Poor UX, confusing behavior |
| Styling defaults | Low | Internal architecture issue |
| Focus ring | Medium | Accessibility concern |
| Corner merging | Low | Minor customization limit |
| Tabbing + values | Medium | Complex widget support |

