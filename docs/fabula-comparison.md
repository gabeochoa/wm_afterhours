# Fabula (FabulaItemsProvider) Comparison

Analysis of [FabulaItemsProvider](https://github.com/jasudev/FabulaItemsProvider)
-- a SwiftUI component showcase with ~286 community-contributed items -- and what
afterhours can learn from it.

Fabula is not a UI framework; it is a curated recipe book of SwiftUI patterns,
effects, and components. The value is in the *interaction patterns* and *visual
ideas*, not in API design.

---

## Component / Pattern Inventory

### Categories found in Fabula

| Category | Count | Examples |
|---|---|---|
| Animation / Effects | ~40 | Fold/Unfold, Glitch, Parallax, Shape Generator, Spring, Waves |
| Containers / Sheets | ~10 | BottomSheet, RightSheet, CustomPopup, CollapsibleView, CollapsibleText |
| Controls | ~15 | Joystick, CustomStepper, ImageSlider (before/after), DragAndResize |
| Data Display | ~10 | BarGraph, FinancialCard, HierarchyTree, RadialTree, Pyramid/Infographic |
| Scroll Effects | ~8 | ScrollOffset tracking, CurveScroll, CircularList, ShapeIndicator, Scroller |
| Tab / Segmented | ~5 | TabSegmentedView (animated indicator + swipeable), AxisTabView |
| Pickers / Forms | ~15 | HueColorPicker, DatePicker, TextFieldClear, DebouncedText, WarningTextEditor |
| Standard SwiftUI | ~80 | Wrappers demonstrating built-in SwiftUI views (TextField, Toggle, List, etc.) |
| Typography / Text | ~10 | CircleTypography, 3D Box Typo, AnimateText, SubstringHighlighter |
| Shapes | ~15 | PolygonShape, StarShape, RaindropShape, ShapeGenerator, ViscosityCanvas |

---

## Patterns Worth Learning From

### 1. Edge-Anchored Sheets (Bottom Sheet / Side Sheet)

Fabula's BottomSheet and RightSheet are drag-to-open panels with:
- Configurable min/max area (collapsed handle vs expanded content)
- Drag indicator capsule with tap-to-toggle
- Background dimming/blur behind the sheet
- Spring animation on open/close
- Gesture priority handling (drag wins over child taps)
- Adaptive: portrait uses bottom sheet, landscape/iPad uses right sheet

**Afterhours status:** Planned but not implemented (`docs/31_overlay_panels.md`).
Modals exist but don't anchor to edges or support drag-to-dismiss.

**Takeaway:** The gesture-to-open + spring-back pattern is a natural fit for
game inventory panels, chat drawers, and settings trays.

### 2. Toast Notifications

Auto-dismissing notifications that slide in from top or bottom:
- Configurable position (top/bottom)
- Auto-dismiss after timeout (2.5s default)
- Tap-to-dismiss
- Slide transition with opacity fade
- Material blur background on iOS

**Afterhours status:** No toast component. Achievement/notification popups are
a common game UI need.

**Takeaway:** A simple toast API would be useful:
```cpp
imm::toast(ctx, "Achievement Unlocked!", ToastConfig{}.position(ToastPosition::Top).duration(3.0f));
```

### 3. Collapsible Text (Truncation-Aware Expand/Collapse)

Truncates text to N lines, auto-detects whether truncation is needed, and shows
a "More"/"Less" button only when the full text exceeds the line limit.

The detection trick: render the text twice invisibly (once with line limit, once
without) and compare heights.

**Afterhours status:** No truncation-aware text. Tree view has expand/collapse
but for tree nodes, not text content.

**Takeaway:** Useful for quest descriptions, item tooltips, dialog logs.

### 4. Image Comparison Slider (Before/After)

Two images overlaid with a vertical dividing line. Drag the divider to reveal
more of one image vs the other. Each image is clipped to its half.

**Afterhours status:** Not present. Niche but useful for settings previews
(e.g., graphics quality comparison).

### 5. Drag-to-Resize with Corner/Edge Handles

A view with 8 resize handles (corners + edge midpoints). Each handle adjusts
size and position based on which corner/edge is dragged. Minimum size enforced.
Double-tap resets to initial size.

**Afterhours status:** Has drag-and-drop reorder (`draggable_children`) but no
resize handles. Planned resizable panels (`docs/37_resizable_panels.md`) but
not general-purpose resize handles on arbitrary views.

**Takeaway:** The resize-point abstraction (relative location enum: zero/middle/max)
is clean and could apply to resizable game UI panels.

### 6. Scroll-Position-Driven Effects

Several Fabula items use scroll position as an input to visual transforms:
- **CurveScroll:** Items curve horizontally based on vertical scroll position (sine wave offset)
- **CircularList:** Items rotate and fade based on scroll position (carousel feel)
- **ShapeIndicator:** Custom scroll indicators using Shape.trim() driven by scroll offset
- **ParallaxPoster:** Timeline items with parallax depth based on scroll

The pattern: read scroll offset via a preference key, then compute per-item
transforms as a function of `(item_y, viewport_height)`.

**Afterhours status:** Scroll views exist but no API to read scroll offset
programmatically or use it to drive per-item transforms.

**Takeaway:** A `scroll_offset()` query on `HasScrollView` would enable:
- Parallax backgrounds
- Carousel-style curved lists
- Snap-to-item scrolling
- Custom scroll indicators

### 7. Animated Tab Indicator (Segmented Control)

Tab bar where the active indicator (underline capsule) animates smoothly between
tabs using `matchedGeometryEffect`. Content area supports swipe-to-switch with
drag gesture.

**Afterhours status:** Has `tab_container` with index-based switching. No animated
sliding indicator between tabs, and no swipe gesture to switch tab content.

**Takeaway:** An animated underline/highlight that slides to the active tab is a
polished touch that improves perceived quality.

### 8. Joystick

Virtual joystick with circular boundary constraint. Stick is clamped to boundary
radius, reports (state, position) via callback, and springs back to center on
release.

**Afterhours status:** Has gamepad/keyboard input mapping but no on-screen virtual
joystick widget.

**Takeaway:** Directly relevant for mobile/touch game UIs. The circular boundary
math (clamp to radius, report angle + distance) is a clean pattern.

### 9. Hierarchy Tree Visualization

Interactive tree where nodes can be added/removed dynamically. Lines between
parent and child nodes are drawn using preference keys to track node positions.
Add/remove buttons on each node.

**Afterhours status:** Has `tree_view` composite for expand/collapse trees.
Fabula's version adds visual connecting lines and dynamic node creation/removal
which are useful for skill trees, tech trees, or debug visualizations.

### 10. Fold/Unfold 3D Animation

Content that folds like paper using `rotation3DEffect` on the X axis. Two halves
of the content rotate toward each other, with darkening overlays simulating
shadow/depth during the fold.

**Afterhours status:** Has spring/declarative animations but no 3D perspective
transforms on UI elements.

**Takeaway:** Not critical for standard game UI, but the visual effect is striking
for page transitions, card flips, or settings panel reveals.

---

## What Afterhours Already Covers

| Fabula Pattern | Afterhours Equivalent |
|---|---|
| Checkbox, Toggle, Radio | checkbox, toggle_switch, radio_group |
| Stepper | stepper |
| Slider | slider |
| Progress Bar | progress_bar, circular_progress |
| Dropdown/Picker | dropdown |
| Tab View | tab_container |
| Tree View | tree_view |
| Drag & Drop | draggable_children |
| Navigation Bar | navigation_bar |
| Modal/Popup | modal system (ModalShowcase) |
| Button Styles | button with ComponentConfig theming |
| Pagination | pagination |
| Separator | separator |
| Scroll View | HasScrollView |
| Form Validation | validation_config, validation_systems |

---

## Feature Gap Summary (Prioritized for Game UI)

| Feature | Fabula Has It | Game UI Relevance | Effort |
|---|---|---|---|
| Toast notifications | Yes | High -- achievements, status messages | Low |
| Bottom/side sheet (drawer) | Yes | High -- inventory, chat, settings | Medium |
| Scroll offset API | Yes (pattern) | High -- parallax, carousel, snap | Low-Medium |
| Accordion / collapsible sections | Yes | High -- settings, quests, inventory | Low |
| Virtual joystick | Yes | High -- mobile/touch games | Medium |
| Animated tab indicator | Yes | Medium -- polish | Low |
| Collapsible text (line-limited) | Yes | Medium -- descriptions, tooltips | Low |
| Hierarchy tree with lines | Yes | Medium -- skill/tech trees | Medium |
| Drag-to-resize handles | Yes | Low-Medium -- editor tools | Medium |
| Image comparison slider | Yes | Low -- settings preview | Low |
| 3D fold/unfold animation | Yes | Low -- visual flair | Medium |
| Custom scroll indicators | Yes | Low -- visual polish | Low |

---

## Key Architectural Observations

1. **Fabula is patterns, not a framework.** Each item is self-contained with
   `fileprivate` helper types. There's no shared layout engine, no ECS, no
   theming system. The value is purely in interaction recipes.

2. **Gesture composition is well-done.** The BottomSheet correctly uses
   `highPriorityGesture` to prevent child tap gestures from stealing drags.
   The joystick properly computes boundary-clamped positions. These gesture
   patterns translate directly to afterhours' input handling.

3. **Scroll-as-input is a recurring theme.** Multiple items treat scroll
   position as a continuous input that drives transforms, opacity, and rotation.
   This pattern is missing from afterhours and would unlock a class of visual
   effects (carousels, parallax, snap scrolling).

4. **Spring animations everywhere.** Nearly every interactive component uses
   spring animations for open/close/snap-back. Afterhours has spring animations
   in `animation_config.h` but they aren't as pervasively applied to UI
   interactions.

---

# Gosh Darn SwiftUI Comparison

Analysis of [goshdarnswiftui.com](https://goshdarnswiftui.com/) -- a SwiftUI
cheat sheet that maps every UIKit component to its SwiftUI equivalent with code
examples. Useful as a "standard component checklist" for any UI toolkit.

## SwiftUI Standard Component Map vs Afterhours

| SwiftUI Component | UIKit Equivalent | Afterhours Equivalent | Gap? |
|---|---|---|---|
| Text | UILabel | label text on any component | No |
| Label | UILabel (icon+text) | `icon_row` | No |
| TextField | UITextField | text_area (single-line) | No |
| TextEditor | UITextView | text_area (multi-line) | No |
| SecureField | UITextField (secure) | -- | **Yes** |
| Image | UIImageView | image, sprite | No |
| Button | UIButton | button, image_button | No |
| Link | -- | -- | **Yes** (not relevant for games) |
| NavigationLink | UINavigationController push | -- | **Yes** |
| Toggle | UISwitch | toggle_switch | No |
| Picker | UIPickerView | dropdown | No |
| Picker (segmented) | UISegmentedControl | -- | **Yes** |
| DatePicker | UIDatePicker | -- | **Yes** |
| ProgressView (linear) | UIProgressView | progress_bar | No |
| ProgressView (circular) | UIActivityIndicatorView | circular_progress | No |
| Slider | UISlider | slider | No |
| Stepper | UIStepper | stepper | No |
| Map | MKMapView | -- | N/A (game-specific) |

## SwiftUI Layout vs Afterhours

| SwiftUI Layout | UIKit Equivalent | Afterhours Equivalent | Gap? |
|---|---|---|---|
| HStack | UIStackView (.horizontal) | hstack | No |
| VStack | UIStackView (.vertical) | vstack | No |
| ZStack | -- (manual frame overlap) | z-index layering | Partial |
| LazyHStack | -- | -- | **Yes** (lazy/virtualized) |
| LazyVStack | -- | -- | **Yes** (lazy/virtualized) |
| List | UITableView | vstack in scroll_view | Partial |
| List (sections) | UITableView with sections | -- | **Yes** |
| ScrollView | UIScrollView | HasScrollView | No |
| LazyHGrid | UICollectionView | -- | **Yes** |
| LazyVGrid | UICollectionView | -- | **Yes** |
| Form | UITableView (grouped) | -- | **Yes** |
| Spacer | -- | spacer | No |
| Divider | -- | separator | No |
| NavigationView | UINavigationController | navigation_bar (partial) | **Yes** |
| TabView (tabs) | UITabBarController | tab_container | No |
| TabView (pages) | UIPageViewController | -- | **Yes** |

## SwiftUI Overlays vs Afterhours

| SwiftUI Overlay | UIKit Equivalent | Afterhours Equivalent | Gap? |
|---|---|---|---|
| Alert | UIAlertController (.alert) | -- | **Yes** |
| Sheet / Modal | presentViewController | modal system | No |
| FullScreenCover | presentViewController (full) | modal system | No |
| ActionSheet | UIAlertController (.actionSheet) | -- | **Yes** |
| ToolbarItem | UIBarButtonItem | -- | **Yes** |

---

## Combined Gap Analysis (Prioritized for Game UI)

Merging gaps from Fabula patterns, Gosh Darn SwiftUI's standard checklist, and
the earlier egui comparison into a unified priority list.

### High Priority (common game UI needs)

| Component | Source | Notes |
|---|---|---|
| Toast notifications | Fabula | Achievements, status, errors |
| Alert dialog | SwiftUI std | Confirm quit, overwrite save, delete character |
| Bottom/side sheet (drawer) | Fabula, SwiftUI std | Inventory, chat, settings |
| Accordion / collapsible sections | Fabula, egui | Settings categories, quest logs |
| Scroll offset API | Fabula, egui | Parallax, carousel, snap-to, stick-to-bottom |
| Virtualized scroll (lazy lists) | SwiftUI std, egui | Long lists without layout cost |
| Navigation stack (push/pop) | SwiftUI std | Screen flow with back navigation |
| Scroll-to API | egui | Chat windows, log viewers, jump-to-item |

### Medium Priority (polish and power-user features)

| Component | Source | Notes |
|---|---|---|
| Segmented control | SwiftUI std | Inline option selection (map/transit/satellite style) |
| Grid layout | SwiftUI std, egui | Item grids, inventory layouts |
| Action sheet | SwiftUI std | Bottom action lists, context actions |
| Secure text field | SwiftUI std | Password entry, unlock codes |
| Page view (swipeable) | SwiftUI std | Onboarding, tutorials, character selection |
| Virtual joystick | Fabula | Mobile/touch game controls |
| Animated tab indicator | Fabula | Tab switching polish |
| Per-widget tooltips | egui | Hover hints on any widget |
| List with sections | SwiftUI std | Grouped settings, categorized items |
| Collapsible text | Fabula | Quest descriptions, item lore |

### Low Priority (nice-to-have)

| Component | Source | Notes |
|---|---|---|
| Form container | SwiftUI std | Grouped control layout with headers/footers |
| DatePicker | SwiftUI std | Calendar selection (niche for games) |
| Resizable panels | egui, Fabula | Editor/tool UIs |
| Image comparison slider | Fabula | Graphics settings preview |
| Popup alignment API | egui | Custom overlay positioning |
| Undo/redo utility | egui | Settings, editors |
| 3D fold animation | Fabula | Visual flair |

---

## Key Takeaway

The three sources (Fabula, Gosh Darn SwiftUI, egui) converge on the same core
gaps in afterhours:

1. **Missing overlay types:** Toast, Alert dialog, Action sheet, Bottom sheet.
   These are the bread and butter of "something happened" feedback in any UI.

2. **No virtualization:** Every scroll child is laid out every frame. SwiftUI
   has Lazy stacks/grids, egui has `show_rows`/`show_viewport`. This is the
   single biggest performance gap for long lists.

3. **No scroll-as-input:** Scroll offset isn't exposed for programmatic use.
   This blocks scroll-to, stick-to-bottom, parallax, snap-to-item, and custom
   scroll indicators.

4. **No navigation stack:** Push/pop screen flow with automatic back buttons.
   `navigation_bar` exists but doesn't manage a screen stack.

5. **No grid layout:** Item inventories, icon grids, and data tables all need
   a grid container. Both SwiftUI (LazyVGrid/LazyHGrid) and egui (Grid,
   TableBuilder) have this.
