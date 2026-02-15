# Focus Ring Audit - Batch 4

Audit date: 2026-02-15
Screens analyzed: 12
Screenshots per screen: 16 (tab_0 through tab_15)

---

## accessibility

**Focus progression:** works
**Visibility:** needs improvement
**Issues:**
- Focus starts on "Dark BG - Works Fine" element (tab_0), then moves to "Light BG - Auto Dark Text" (tab_1), "Dark BG - Auto Light Text" (tab_2), then progresses left-to-right through the first row of Auto-Contrast color blocks (tab_3 through tab_9), then continues through the second row (tab_10 through tab_14), before wrapping back. The progression skips the upper elements ("Theme Passes WCAG AA" button and the "Without/With Automatic Contrast" headers) -- focus only visits the color swatch blocks and the two right-side demo panels.
- The focus ring is a thin dashed/dotted white outline. On lighter-colored blocks (yellow, light green, beige at tab_9-tab_10) the white focus ring has reduced contrast and can be hard to spot.
- Focus wraps correctly: tab_15 shows focus on row 2 position 5 ("Auto" on a gray-ish block), and the sequence cycles through consistently.
- No stuck focus detected; each tab moves to a new element.

## adaptive_scaling

**Focus progression:** works
**Visibility:** good
**Issues:**
- Focus cycles through only 4 interactive elements: "Mode: Adaptive" button (tab_0), minus button "-" (tab_1), plus button "+" (tab_2), "Button A" (tab_3), "Button B" (tab_4). The sequence then wraps back to "Mode: Adaptive" (tab_5) and repeats the same 5-element cycle (tabs 5-9 = same as 0-4, tabs 10-14 = same again, tab_15 = wraps to "Mode: Adaptive").
- The focus ring is clearly visible on all elements -- dark dashed outline on "Mode: Adaptive" (blue button) and a clear outline on the "-" / "+" buttons.
- Focus wraps correctly after the last interactive element ("Button B") back to the first ("Mode: Adaptive").
- Focus appears stuck in a repeating 5-element cycle. The many non-interactive display elements (color swatches, Expand & Flex bars, px/h720/scr% blocks, breakpoint labels) are correctly skipped. However, this means 16 tab presses cycle through the same 5 elements 3+ times, which is correct behavior but worth noting.

## absolute_positioning

**Focus progression:** broken
**Visibility:** invisible
**Issues:**
- No visible focus ring appears on any element across all 16 screenshots (tab_0 through tab_15). Every screenshot looks completely identical with no dashed outline, highlight, or any other focus indicator visible on any element.
- The screen contains multiple interactive-looking elements (translate buttons, corner pins TL/TR/BL/BR, Flow A/B/C, BADGE, and two percentage-sizing blocks) but none ever show a focus ring.
- This is a critical accessibility failure: a keyboard-only user cannot tell which element is focused.
- Cannot assess focus progression or wrapping since no focus ring is visible.

## layout

**Focus progression:** broken
**Visibility:** invisible
**Issues:**
- No visible focus ring on any element across all 16 screenshots (tab_0, tab_5, tab_10, tab_15 sampled and all identical). Every screenshot looks exactly the same.
- The screen has "Left Panel", "Center Panel", "Right Panel" buttons and "Stacked Item 1/2/3" elements, but no focus indicator is visible on any of them.
- This is a display/informational demo using the Sage Natural theme (light green background). The lack of any focus ring across 16 tabs suggests either no elements are focusable or the focus ring is not rendering.
- Cannot assess wrapping or progression.

## layout_bug_repros

**Focus progression:** works
**Visibility:** good
**Issues:**
- Focus progresses through interactive elements on the screen: Tab A (tab_1), Tab B (tab_2, the tab_container tabs at top right), Tab C (tab_3), first toggle switch "I" (tab_4), second toggle "O" (tab_5), third toggle "I" (tab_6), then down to the green "Settings & Configuration" button (tab_7, the bottom-left green one), blue "Settings & Configuration" button (tab_8, the blue one below). The cycle then wraps back, repeating from Tab A onward.
- The focus ring is a white/light outline that is clearly visible against the dark background for most elements. On the tab container tabs (Tab A, Tab B, Tab C) it appears as a thin white outline. On the toggle switches, the focus ring appears as a large white rectangle surrounding the toggle row.
- Focus wraps correctly after the last element (blue "Settings & Configuration" button at tab_8/tab_14) back to Tab A (tab_9/tab_15).
- Toggle switch focus ring (tabs 4-6, 10-12) is oversized -- it wraps the entire toggle row rather than just the individual toggle, which could be confusing. The focus ring for the first toggle (tab_4) encompasses the entire toggle area rather than just the one toggle switch.
- Tab_0 shows focus on the blue "Settings & Configuration" button at the bottom, establishing the initial state.

## flex_alignment

**Focus progression:** partially works
**Visibility:** needs improvement
**Issues:**
- Focus appears stuck on the "All" tab button (bottom-left of the tab bar) across all 16 screenshots examined (tab_0, tab_5, tab_10, tab_15). The focus ring (a dark outline around the "All" tab) never moves to any other element.
- The screen has 5 tab buttons (All, Vertical, Horizontal, Cross-Axis, Self Align) but focus does not appear to progress to any of them -- the same "All" tab remains highlighted in every screenshot.
- This is a critical focus navigation bug: focus is completely stuck on a single element and pressing Tab does not advance it.
- The focus ring on the "All" tab is a dark outline with a red underline, which is somewhat visible against the dark background but could be stronger.

## hstack_showcase

**Focus progression:** works
**Visibility:** good
**Issues:**
- Focus cycles through only the toolbar pattern items: File (tab_3/tab_15), Edit (tab_0/tab_4), View (tab_1/tab_5), Settings (tab_2/tab_10). The focus ring cycles through these 4 toolbar buttons repeatedly.
- The focus ring is a clear white/light dashed outline around each toolbar button text, visible against the dark blue toolbar background.
- Focus wraps correctly from Settings back to File.
- Many interactive-looking elements on the screen are skipped entirely: Item 1/2/3 bars, the colored card row (Users, Revenue, Orders, Growth, Active, Churn), and the small cyan/yellow "A" buttons on the right side. Only the toolbar's File/Edit/View/Settings items receive focus.
- The number of focusable elements (4) seems very low for a screen with this many visible controls.

## vstack_showcase

**Focus progression:** works
**Visibility:** good
**Issues:**
- Focus progresses through the sidebar navigation buttons in the "Sidebar + Content" section: Home (tab_0/tab_4), Settings (tab_1/tab_5), Profile (tab_2/tab_10), Help (tab_3/tab_15). Focus cycles through these 4 items.
- The focus ring is a clear white outline around each sidebar button, well-contrasted against the dark background.
- Focus wraps correctly from Help back to Home.
- Similar to hstack_showcase, many visible elements on the screen are not receiving focus -- the colored blocks in the JustifyContent and AlignItems demo sections, the content area items (Content, Alerts, Activity, Stats) -- none of these are focusable.
- Only 4 focusable elements on a screen with many visual widgets.

## self_align

**Focus progression:** broken
**Visibility:** invisible
**Issues:**
- No visible focus ring on any element across all 16 screenshots (tab_0, tab_5, tab_10, tab_15 all identical). Every screenshot looks exactly the same.
- The screen contains "FlexStart (left)", "Center", "FlexEnd (right)" buttons in the demo area, plus "Left", "Center", "Right" buttons below. None ever show any focus indicator.
- This is a complete focus navigation failure -- either no elements are marked focusable or the focus ring is not rendering.
- Cannot assess wrapping or progression.

## separators

**Focus progression:** broken
**Visibility:** invisible
**Issues:**
- No visible focus ring on any element across all 16 screenshots (tab_0, tab_5, tab_10, tab_15 all identical). Every screenshot appears exactly the same.
- The screen is a Separator Widget Demo with a light/white theme. The navigation items (Home, Settings, Profile, Help) and the labeled separator "Settings" text appear to be static text rather than interactive elements.
- This screen may be intentionally non-interactive (purely a visual demo of separator widgets), which would explain the lack of focusable elements. However, if the navigation items are intended to be interactive, this is a bug.
- Cannot assess wrapping or progression.

## horizontal_drag

**Focus progression:** broken
**Visibility:** invisible
**Issues:**
- No visible focus ring on any element across all 16 screenshots (tab_0, tab_5, tab_10, tab_15 all identical). Every screenshot looks exactly the same.
- The screen contains draggable tags ("Security patch", "Data loss fix", "Refactor auth", "Update deps", "Add logging") and a partially visible "Rea..." tag in the High Priority lane, but none show any focus indicator.
- Drag-and-drop elements typically need keyboard accessibility for full compliance. The complete absence of focus rings means keyboard users cannot interact with any drag targets.
- Cannot assess wrapping or progression.

## drag_drop

**Focus progression:** broken
**Visibility:** invisible
**Issues:**
- No visible focus ring on any element across all 16 screenshots (tab_0, tab_5, tab_10, tab_15 all identical). Every screenshot looks exactly the same.
- The screen is a Kanban-style board with columns (To Do, In Progress, Done) and cards (Design mockups, Write tests, Fix login bug, API endpoints, DB schema, Code review, Setup CI, Update docs). No card or column ever shows a focus ring.
- Like horizontal_drag, drag-and-drop components should support keyboard navigation for accessibility. The complete lack of focus indicators is a critical issue.
- Cannot assess wrapping or progression.

---

# Summary

## Screens with working focus navigation (3/12):
| Screen | Elements in cycle | Notes |
|--------|------------------|-------|
| accessibility | ~16 (color blocks + demo panels) | Full progression through color swatch grid |
| adaptive_scaling | 5 (mode button, +/-, two pixel buttons) | Clean 5-element cycle |
| layout_bug_repros | ~8 (tabs, toggles, buttons) | Good progression, oversized toggle focus ring |

## Screens with partially working focus (1/12):
| Screen | Issue |
|--------|-------|
| flex_alignment | Focus stuck on single "All" tab; never advances |

## Screens with limited focusable elements (2/12):
| Screen | Elements in cycle | Notes |
|--------|------------------|-------|
| hstack_showcase | 4 (File/Edit/View/Settings toolbar only) | Many visible elements skipped |
| vstack_showcase | 4 (Home/Settings/Profile/Help sidebar only) | Many visible elements skipped |

## Screens with no visible focus ring (6/12):
- **absolute_positioning** - Multiple interactive-looking elements, none receive focus
- **layout** - Panel and stacked item buttons show no focus
- **self_align** - Alignment demo buttons show no focus
- **separators** - May be intentionally non-interactive
- **horizontal_drag** - Drag targets need keyboard accessibility
- **drag_drop** - Kanban cards need keyboard accessibility

## Key findings:
1. **50% of screens have no focus ring at all** -- this is a major accessibility concern.
2. **Focus ring visibility on light backgrounds** needs improvement (accessibility screen on light-colored blocks).
3. **Toggle switch focus ring is oversized** in layout_bug_repros -- it wraps the entire toggle group instead of individual toggles.
4. **flex_alignment has stuck focus** -- Tab key does not advance past the "All" tab.
5. **Drag-and-drop screens lack keyboard support entirely** (both horizontal_drag and drag_drop).
6. **hstack_showcase and vstack_showcase** only focus toolbar/sidebar items, ignoring many other visual elements that appear interactive.
