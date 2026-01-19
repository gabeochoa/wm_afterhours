# Showcase Screen Coverage

**Status:** Documentation  
**Priority:** Medium

This tracks which UI components have dedicated showcase/demo screens for visual testing and development.

---

## Components with Dedicated Coverage

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

---

## Components Missing Showcase Screens

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

---

## Suggested New Showcase Screens

### 1. `Navigation.h` - Navigation Components

Demonstrate `navigation_bar` and `pagination` components:
- Left/right navigation patterns
- Page indicators
- Keyboard control

### 2. `Media.h` - Media Components

Demonstrate `image`, `sprite`, `icon_row`, `image_button`:
- Static images
- Spritesheets
- Icon grids
- Clickable images

### 3. `AdvancedForms.h` - Complex Form Patterns

Demonstrate `checkbox_group` and complex form patterns:
- Checkbox groups with min/max selection
- Form validation patterns
- Multi-select scenarios

### 4. `Effects.h` - Visual Effects

Demonstrate visual effects:
- Shadow types (hard vs soft)
- Opacity variations
- Focus clusters
- Disabled states

---

## Priority Order

### Phase 1: Navigation Components
1. Create `Navigation.h` showcase
2. Demo `navigation_bar` with keyboard control
3. Demo `pagination` with page switching

### Phase 2: Media Components
4. Create `Media.h` showcase
5. Demo `image`, `sprite`, `icon_row`
6. Demo `image_button` interactions

### Phase 3: Advanced Forms
7. Create `AdvancedForms.h` showcase
8. Demo checkbox groups
9. Expand disabled/hidden state demos

### Phase 4: Visual Effects
10. Demo shadow variations
11. Demo opacity inheritance
12. Demo focus cluster behavior

