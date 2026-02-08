# Design Audit: Flight Options Screen

**Audit Date:** 2026-02-07
**Screen:** Flight Options / Options Menu
**Framework:** Apple Human Interface Guidelines (1987)
**Audit Type:** Standard (Full Checklist)

---

## Audit Summary: Flight Options

This screen presents a game options menu with a left sidebar of categories (Flight System, Controls, Keyboard, etc.) and a right content panel showing settings for the selected category. The overall design uses a dark navy background with a monochromatic blue palette.

---

### Critical Issues (Fix Immediately)

1. **Color-Only Distinction (Accessibility §8.1, Color §3.1)**: The difference between the selected left-panel item ("FLIGHT SYSTEM") and unselected items is communicated almost entirely through color — a slightly different shade of blue and a thin left border. Users with color vision deficiencies would struggle to determine which category is active. Shape, weight, size, or iconography should provide a redundant, non-color signal.
   → **Fix**: Add a bold weight change, a filled background, an arrow indicator, or indentation shift to the selected category — not just a color/border change.

2. **Disabled State Illegibility (Feedback §1.7, Perceived Stability §1.9)**: The "Vibration [Unavailable]" item is barely distinguishable from the background. The "[Unavailable]" label and the subtext "requires compatible controller" are extremely low-contrast gray-on-dark-blue, making them nearly impossible to read. The HIG specifies that unavailable items should be *dimmed*, not invisible.
   → **Fix**: Use a clearly dimmed but still legible text color (e.g., 40–50% opacity of the normal text color). Ensure the explanatory subtext meets at least a 3:1 contrast ratio against the background.

3. **Ambiguous Focus/Selection on Right Panel (See-and-Point §1.3, Feedback §1.7)**: In the right content panel, "Control Type" appears slightly darker than "High-G Turn Settings" and "Reduced Collision Damage," but it is unclear whether this indicates the focused/selected item or just a different visual style. The user cannot confidently tell which setting is currently active or will respond to input.
   → **Fix**: Add a clear, unmistakable focus indicator — a bright outline, highlight bar, or cursor — on the currently focused right-panel item. Differentiate between "focused" (ready for input) and "non-focused" states with more than a subtle shade difference.

---

### Major Issues (Fix Soon)

4. **Monochromatic Palette Collapses Visual Hierarchy (Aesthetic Integrity §1.10, Color §3.2)**: The entire screen uses variations of a single blue hue — dark navy background, medium-blue buttons, light-blue text. This makes every element blend together, destroying visual hierarchy. The title, categories, settings items, help text, and key hints all feel like they exist on the same visual plane.
   → **Fix**: Introduce at least one contrasting accent color for interactive/focused elements. Use brightness and saturation differences more aggressively. Consider a warmer accent (the gold dots hint at this but are underused) for selection and focus states.

5. **ALL CAPS Overuse Reduces Readability (Plain Language §7.1, Aesthetic Integrity §1.10)**: Nearly every text element is rendered in ALL CAPS — "OPTIONS," "FLIGHT SYSTEM," "CONTROLS," "KEYBOARD," "MOUSE," "FLIGHT STICK," "DISPLAY," "GRAPHICS," "SOUND," "LANGUAGE," and "FLIGHT SYSTEM SETTINGS." ALL CAPS reduces reading speed by 13–20% compared to mixed case, and when everything is capitalized, nothing stands out as important.
   → **Fix**: Reserve ALL CAPS for the screen title ("OPTIONS") and section headers ("FLIGHT SYSTEM SETTINGS"). Use Title Case or Sentence case for list items and settings names to improve scanability and establish hierarchy.

6. **Decorative Dots Serve No Communicative Purpose (Aesthetic Integrity §1.10, Feedback §1.7)**: There are two yellow/gold dots — one on the horizontal rule below the title, and one floating between the left and right panels near the "Vibration" item. These dots appear purely decorative but could be mistaken for status indicators, progress markers, or interactive elements. They violate the principle that graphics should support understanding.
   → **Fix**: Either give the dots a clear functional meaning (e.g., indicating the current scroll position or active section) and document it, or remove them entirely. Ambiguous decorative elements create confusion.

7. **Help Text Disconnected from Context (Feedback §1.7, See-and-Point §1.3)**: The help text "Choose between standard or expert control schemes." sits at the bottom of the screen, far removed from the "Control Type" setting it presumably describes. The spatial separation makes it unclear which item the help text refers to, especially if the user moves focus to a different setting.
   → **Fix**: Position the help text immediately adjacent to or beneath the focused item, or use a tooltip/inline description pattern. If it must remain at the bottom, add a visual connection (line, arrow, or dynamic update label) to link it to the relevant item.

8. **Inconsistent Keyboard Hint Styling (Consistency §1.4)**: The bottom bar shows "Enter OK" with a small bordered key icon and "Esc BACK" with a noticeably larger, bolder "Esc" label. The two keyboard hints use different font sizes, weights, and box proportions for no apparent reason, violating internal consistency.
   → **Fix**: Standardize keyboard hint rendering — use the same box size, font size, padding, and weight for all key icons. If "Esc" needs emphasis, do so through layout (e.g., position) rather than inconsistent sizing.

9. **Dash Bullets Are Non-Standard and Wasteful (Consistency §1.4, See-and-Point §1.3)**: Each left-panel category is prefixed with a dash "–" character. This is a non-standard UI pattern for a navigation list — dashes typically indicate list items in text documents, not interactive menu items. The dashes consume horizontal space without adding navigational clarity.
   → **Fix**: Remove dash prefixes. Use indentation, icons, or selection indicators instead. If visual separation is needed, use spacing or subtle divider lines between items.

---

### Minor Issues (Consider Fixing)

10. **No Breadcrumb or Navigation Context (See-and-Point §1.3, Perceived Stability §1.9)**: The user is on "OPTIONS > FLIGHT SYSTEM > Control Type" but the screen provides no breadcrumb trail or structural indicator showing where they are in the navigation hierarchy. With 9 categories and multiple sub-settings, users may lose track of their position.
    → **Fix**: Add a breadcrumb or path indicator (e.g., "Options > Flight System") near the top of the right panel, or visually connect the selected left-panel item to the right-panel header more strongly.

11. **Left Panel Items Lack Hover/Pre-Selection Feedback (Feedback §1.7)**: The left-panel categories show no visible hover or pre-selection state. When navigating with a controller or keyboard, there's no indication of which item the cursor is approaching before committing to it.
    → **Fix**: Add a subtle hover or pre-focus state (e.g., slight background lightening, underline, or border highlight) to indicate which item is about to be selected.

12. **Title "OPTIONS" Uses Decorative Horizontal Rule (Aesthetic Integrity §1.10)**: The horizontal blue line beneath "OPTIONS" with the gold dot endpoint is a decorative flourish that doesn't serve a clear structural purpose. It creates visual noise at the top of the screen without meaningfully separating the title from the content.
    → **Fix**: Simplify to a clean separator or increase whitespace between the title and content. If the line stays, remove the gold dot endpoint or give it a clear meaning.

13. **Right Panel Items Have Inconsistent Heights (Consistency §1.4)**: "Control Type" appears to have a taller button height than "High-G Turn Settings" and "Reduced Collision Damage." The "Vibration" item is yet another height with its subtext. Inconsistent item sizing makes the interface feel unpolished.
    → **Fix**: Standardize the height of all right-panel settings items. Items with subtext can use a consistent expanded height, but the base interactive items should all match.

14. **Section Header "FLIGHT SYSTEM SETTINGS" Lacks Visual Weight (Aesthetic Integrity §1.10)**: The right panel's section header has the same color and similar size as the items beneath it. It doesn't stand out as a structural header, making it easy to overlook.
    → **Fix**: Increase the header's font size, add a bottom border/rule, use a different color, or add more whitespace below it to create clear separation from the list items.

15. **No Visual Connection Between Left Selection and Right Content (Consistency §1.4, Feedback §1.7)**: There is no visual bridge linking the selected "FLIGHT SYSTEM" category on the left to the "FLIGHT SYSTEM SETTINGS" panel on the right. The two panels appear as independent columns rather than a connected master-detail relationship.
    → **Fix**: Add a visual connector — a tab-like extension from the selected left item into the right panel, a matching background color band, or an arrow — to clearly show causality between the left selection and right content.

16. **Escape Key Mapping Lacks Visible Hierarchy (User Control §1.6)**: "Esc BACK" is shown but there's no indication of whether pressing Escape from different depths (category list vs. setting detail) behaves differently. The user doesn't know if Escape will go back one level, exit options entirely, or cancel a pending change.
    → **Fix**: Clarify the Escape behavior with more specific labeling (e.g., "Esc: Back to Categories" vs. "Esc: Close Options") or provide a consistent single-level-back behavior and document it.

17. **Tight Spacing in Left-Panel Category List (Aesthetic Integrity §1.10)**: The nine category items in the left panel are packed closely together with minimal vertical padding. This makes individual items harder to target (especially with a mouse or touch), and the list feels dense and cramped.
    → **Fix**: Increase vertical padding between items by 4–8px. Consider whether all 9 categories need to be visible simultaneously, or if a scrollable/paginated approach would reduce clutter.

---

### Strengths

- **Stable Layout**: The two-panel master-detail pattern is a well-understood navigational paradigm. Users familiar with settings menus will recognize the structure immediately.
- **Disabled Item Explanation**: The "Vibration" item includes a reason for its unavailability ("requires compatible controller"), which is better than silently disabling it with no context. The *intent* is good even if the *execution* (contrast) needs work.
- **Keyboard Shortcuts Visible**: Showing "Enter OK" and "Esc BACK" at the bottom provides discoverability for keyboard/controller navigation, following the see-and-point principle.
- **Contextual Help Text Present**: The description text at the bottom attempts to explain the focused setting, which supports learnability. Its placement just needs refinement.
- **Perceived Stability**: The screen layout is fixed and predictable — elements don't shift or animate unexpectedly. The left panel serves as a persistent navigational landmark.

---

### Overall Score: 4/10

The flight options screen has a solid structural foundation (master-detail layout, visible keyboard hints, contextual help) but is undermined by a monochromatic color scheme that collapses visual hierarchy, ambiguous selection/focus states, poor contrast for disabled elements, and inconsistent component styling. The most critical issue is that selection and focus states rely almost entirely on subtle color differences, creating both usability and accessibility problems. With focused improvements to contrast, focus indicators, typography hierarchy, and visual connection between panels, this screen could become significantly more usable.
