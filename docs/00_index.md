# Afterhours UI Library Gaps - Index

This directory contains detailed specifications for missing features, infrastructure gaps, and API improvement ideas for the Afterhours UI library.

---

## Text Editing & Input
| # | File | Status | Priority |
|---|------|--------|----------|
| 03 | `03_text_editing_widget.md` | Spec complete | High |
| ~~21~~ | ~~`21_text_input.md`~~ | ✅ Implemented | - |
| 22 | `22_rich_text_label.md` | Not implemented | Medium |
| 34 | `34_text_area.md` | Not implemented | Low |

> `text_input()` is now implemented in afterhours with UTF-8, cursor, and callbacks.

## Widgets - High Priority
| # | File | Status | Priority |
|---|------|--------|----------|
| 20 | `20_stepper_selector.md` | Not implemented | High |

## Widgets - Medium Priority
| # | File | Status | Priority |
|---|------|--------|----------|
| 23 | `23_tooltip.md` | Not implemented | Medium |
| 24 | `24_context_menu.md` | Not implemented | Medium |
| 25 | `25_tree_node.md` | Not implemented | Medium |
| 26 | `26_button_variants.md` | Partial (button exists) | Low |
| 27 | `27_input_number.md` | Not implemented | Medium |
| 28 | `28_meters_gauges.md` | Partial (progress_bar exists) | Low |
| ~~29~~ | ~~`29_tab_bars.md`~~ | ✅ Implemented | - |
| 09 | `09_modal_dialogs.md` | Spec complete | Medium |

> `tab_container()` is now implemented in afterhours.

## Widgets - Low Priority
| # | File | Status | Priority |
|---|------|--------|----------|
| 30 | `30_list_table.md` | Not implemented | Low |
| 31 | `31_color_picker.md` | Not implemented | Low |
| 32 | `32_drag_and_drop.md` | Not implemented | Low |
| 33 | `33_css_grid_layout.md` | Not implemented | Low |
| 35 | `35_decorative_elements.md` | Partial (decorative_frame) | Low |
| 36 | `36_game_ui_widgets.md` | Not implemented | Low |

## Infrastructure - Core
| # | File | Status | Priority |
|---|------|--------|----------|
| 05 | `05_render_system_const_constraint.md` | BLOCKER | High |
| 06 | `06_action_binding_system.md` | Spec complete | Medium |
| 07 | `07_renderer_abstraction.md` | Spec complete | Large |
| ~~08~~ | ~~`08_scrollable_containers.md`~~ | ✅ Implemented | - |
| 10 | `10_command_history.md` | Spec complete | Medium |
| 17 | `17_pluggable_backends.md` | Spec complete | Large |

> `scroll_view()` is now fully implemented in afterhours with mouse wheel, clipping, and scroll state.

## Infrastructure - UI
| # | File | Status | Priority |
|---|------|--------|----------|
| 11 | `11_status_notifications.md` | Spec complete | Small |
| 13 | `13_icon_registry.md` | Spec complete | Small |
| 14 | `14_uninitialized_element_size_warning.md` | Spec complete | Small |
| 40 | `40_focus_ring_styles.md` | Not implemented | Large |
| 41 | `41_font_loading.md` | Not implemented | Medium |
| 44 | `44_visual_effects.md` | Not implemented | Medium |
| 45 | `45_styling_infrastructure.md` | Not implemented | Medium |

## Testing
| # | File | Status | Priority |
|---|------|--------|----------|
| 42 | `42_test_input_hooks.md` | Not implemented | Medium |
| 43 | `43_e2e_testing.md` | Not implemented | Large |

## Drawing & Graphics
| # | File | Status | Priority |
|---|------|--------|----------|
| 16 | `16_drawing_tools.md` | Spec complete | Large |

## API Improvements & DX (Developer Experience)
| # | File | Status | Priority |
|---|------|--------|----------|
| 50 | `50_api_ergonomics.md` | Not implemented | Medium |
| 51 | `51_entity_id_management.md` | Not implemented | High |
| 52 | `52_absolute_positioning.md` | Critical bugs | High |
| 53 | `53_type_safety.md` | Not implemented | Low |
| 54 | `54_debug_tooling.md` | Partial | Medium |
| 55 | `55_component_config_issues.md` | Not implemented | High |
| 56 | `56_color_system.md` | Not implemented | Low |
| 57 | `57_responsive_design.md` | Not implemented | Medium |

## Standard UI Components (from Component Gallery)
| # | File | Status | Priority |
|---|------|--------|----------|
| 60 | `60_accordion.md` | Not implemented | Medium |
| 61 | `61_alerts_feedback.md` | Not implemented | Medium |
| 62 | `62_avatars_badges.md` | Not implemented | Low |
| 63 | `63_navigation.md` | Partial (nav_bar, pagination exist) | Low |
| 64 | `64_form_controls.md` | Mostly done (slider, checkbox, etc.) | Low |
| 65 | `65_date_time.md` | Not implemented | Low |
| 66 | `66_file_handling.md` | Not implemented | Low |
| 67 | `67_overlay_panels.md` | Not implemented | Medium |
| 68 | `68_loading_states.md` | Not implemented | Medium |
| 69 | `69_media.md` | Partial (image exists) | Low |
| 70 | `70_misc_widgets.md` | Not implemented | Low |
| 71 | `71_page_sections.md` | Not implemented | Low |

## Coverage & Testing
| # | File | Status | Priority |
|---|------|--------|----------|
| 80 | `80_showcase_screens.md` | Documentation | Medium |
| 81 | `81_test_coverage_gaps.md` | Documentation | High |

## Workarounds
| Directory | Description |
|-----------|-------------|
| `workarounds/06_action_binding/` | Action binding workaround from wordproc |

---

## Level of Effort (LOE)

Quick reference for what's easiest to implement first.

### 🟢 Quick Wins (< 1 hour)

| # | Item | Notes |
|---|------|-------|
| 14 | Uninitialized Size Warning | Single assert/log in render |
| 11 | Status Notifications (Toast) | Compose existing components |
| 13 | Icon Registry | Wrapper around texture manager |
| 26 | Button Variants | Add variant param to existing button |
| 28 | Inline Meter | Compose progress_bar + icon + label |
| 62 | Avatars/Badges | Image + styled div |

### 🟡 Small (1-4 hours)

| # | Item | Notes |
|---|------|-------|
| 20 | Stepper/Selector | Buttons + value display |
| 23 | Tooltip | Div positioned near hover target |
| 27 | Input Number | text_input + stepper buttons |
| 53 | Type Safety | Wrapper structs for floats |
| 56 | Color System | Helper functions, no new components |
| 60 | Accordion | Collapsible div, simple state |
| 61 | Alerts/Banners | Styled div with icon |
| 68 | Loading States | Skeleton loaders, shimmer effects |
| 70 | Rating | Row of icon buttons |
| 70 | Separator | Thin div |

### 🟠 Medium (4-8 hours / 1 day)

| # | Item | Notes |
|---|------|-------|
| 06 | Action Binding | KeyBinding struct + ActionMap |
| 10 | Command History | Generic undo/redo stack |
| 25 | Tree Node | Recursive collapsible sections |
| 41 | Font Loading | Glyph metrics API additions |
| 42 | Test Input Hooks | Input injection layer |
| 50 | API Ergonomics | Helper functions, builder patterns |
| 54 | Debug Tooling | Overlay system, hierarchy viewer |
| 57 | Responsive Design | Breakpoint helpers |
| 67 | Drawer/Popover | Overlay panel positioning |
| 71 | Page Sections | Header/footer/hero layouts |

### 🔴 Large (1-3 days)

| # | Item | Notes |
|---|------|-------|
| 05 | Render Const Constraint | System architecture change |
| 09 | Modal Dialogs | Focus trap, overlay, z-order |
| 22 | Rich Text Label | BBCode parsing, multi-style spans |
| 24 | Context Menu | Right-click trigger, positioning |
| 40 | Focus Ring Styles | Per-component customization |
| 51 | Entity ID Management | mk() API changes |
| 52 | Absolute Positioning Bugs | Debug + fix margin calc |
| 55 | Component Config Issues | Several config fixes |

### ⚫ Very Large (3+ days)

| # | Item | Notes |
|---|------|-------|
| 07 | Renderer Abstraction | Concept-based backend system |
| 17 | Pluggable Backends | IBackend interface + impls |
| 30 | List/Table | Virtual scrolling, columns |
| 31 | Color Picker | Hue wheel, RGB inputs, preview |
| 32 | Drag and Drop | Hit testing, ghost rendering |
| 33 | CSS Grid Layout | 2D grid algorithm |
| 34 | Text Area | Multiline with line numbers |
| 43 | E2E Testing | Full framework setup |
| 65 | Date/Time Inputs | Calendar widget |

---

## Priority Summary

### Critical (Bugs/Blockers)
- Absolute Positioning Bugs (52) - margins cause negative sizes
- Render System Const Constraint (05) - blocks scrollable containers, modals

### High Priority
- Entity ID Management (51) - mk() usability
- Component Config Issues (55) - text rendering, disabled state
- Stepper/Selector (20)

### Medium (~1-2 days)
- API Ergonomics (50) - helper functions, builder patterns
- Debug Tooling (54) - overlays, hierarchy viewer
- Responsive Design (57) - breakpoints, scaling
- Tooltip (23)
- Input Number (27)
- Test Input Hooks (42)
- Glyph Metrics API (41)
- Action Binding System (06)
- Command History (10)

### Large (~3-5 days)
- Rich Text Label (22)
- Focus Ring Styles (40)
- Tree Node (25)
- Modal/Dialog (09)
- Context Menu (24)
- CSS Grid Layout (33)
- Renderer Abstraction Layer (07)
- E2E Testing Framework (43)

### Low Priority
- Type Safety (53) - stronger typing for floats
- Color System (56) - helpers, palettes
- Avatars & Badges (62)
- Date/Time Inputs (65)
- File Handling (66)
- Media Components (69)
- Page Sections (71)

### Very Large (~1+ week)
- Text Area / Text Editor (34)
- List Box (30)
- Table (30)
- Drag and Drop (32)
- Color Picker (31)

### ✅ Recently Completed in Afterhours
- `scroll_view()` - Full scroll container with wheel input
- `text_input()` - Single-line with UTF-8 and cursor
- `tab_container()` - Horizontal tabs
- `navigation_bar()` - Arrow navigation
- `pagination()` - Page controls
- `slider()`, `checkbox()`, `radio_group()`, `dropdown()`, `toggle_switch()`
- `progress_bar()`, `circular_progress()`
- `image_button()`

---

## Component Gallery Coverage

Parsed **60 standard UI components** from the Component Gallery. Here's the coverage:

### ✅ Already Implemented in Afterhours
- Button, button_group, image_button
- Checkbox, radio_group, toggle_switch
- Slider
- Dropdown
- Progress bar, circular_progress
- Tabs (tab_container)
- Text input
- Scroll view
- Navigation bar, pagination
- Image, sprite, icon_row

### 📋 Documented (Not Yet Implemented)
- Color picker → `31_color_picker.md`
- Modal → `09_modal_dialogs.md`
- Stepper → `20_stepper_selector.md`
- Table → `30_list_table.md`
- Textarea → `34_text_area.md`
- Toast → `11_status_notifications.md`
- Tooltip → `23_tooltip.md`
- Tree view → `25_tree_node.md`

### 📝 New Docs Added (60-71)
- **60_accordion.md** - Collapsible sections
- **61_alerts_feedback.md** - Alert banners, Empty states
- **62_avatars_badges.md** - User avatars, Labels/tags
- **63_navigation.md** - Breadcrumbs, Nav menus, Pagination, Links
- **64_form_controls.md** - Select, Combobox, Slider, Checkbox, Radio, Fieldset
- **65_date_time.md** - Date input, Datepicker, Calendar
- **66_file_handling.md** - File display, File upload, File browser
- **67_overlay_panels.md** - Drawer, Popover, Dropdown menu
- **68_loading_states.md** - Spinner, Skeleton loaders
- **69_media.md** - Image, Video, Icon, Carousel
- **70_misc_widgets.md** - Rating, Segmented control, Search, Separator, Quote
- **71_page_sections.md** - Header, Footer, Hero, Heading, Card

### ⏭️ Skipped (not relevant for game UI)
- Skip link (accessibility pattern)
- Visually hidden (accessibility pattern)

