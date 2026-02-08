# Design Audit: kirby_options

## Screen Description

A game options/settings screen in a Kirby-style UI toolkit. The screen features a top navigation bar with colored icon buttons (L, WiFi, Control, Home, Mail, Star, Gear, R), a main content panel containing a "Name" input field, category tiles (Controls, Display, Accessibility, Sound, Network) grouped under a "Common" header, Edit/Erase side buttons, and a bottom instruction bar.

---

## Critical Issues (Fix Immediately)

### 1. Color Is the Primary (Often Only) Distinguisher Between States and Elements
**Principle Violated:** Color Guidelines §3.1 — "Color is supplementary, not required"; Accessibility §8.1 — "Color not the only distinguisher"

The active "Controls" tile is a solid green fill, while inactive tiles (Display, Accessibility, Network) use colored text on a pale background. The only reliable way to distinguish the selected tile from unselected ones is fill color. A colorblind user would struggle to parse which category is active. Similarly, the navigation bar uses seven distinct hues (blue, red, green, dark green, yellow, purple) with no shape, icon, or pattern differences to reinforce meaning.

→ **Fix**: Add a secondary indicator for the active state — a bold border, an underline, a checkmark badge, or a distinct shape change (e.g., raised/depressed appearance). Ensure the design reads correctly in grayscale.

### 2. "Accessibility" Tile Has Dangerously Low Text Contrast
**Principle Violated:** Color Guidelines §3.2 — "Text and thin lines have sufficient contrast to be easily visible"; Accessibility §8.1

The word "Accessibility" is rendered in a light lavender/purple on a pale pinkish-white tile background. This is nearly illegible at typical viewing distances. Ironically, the worst-contrast element is the one labeled "Accessibility."

→ **Fix**: Use dark text on the light tile background (or invert to light text on a dark fill) so the label meets at minimum a 4.5:1 contrast ratio against its background.

### 3. "Erase" Button Lacks Destructive-Action Visual Treatment
**Principle Violated:** Forgiveness §1.8 — "Users warned before irreversible actions"; Feedback §1.7; Alert Boxes §2.6

"Erase" is styled identically to "Edit" — same size, same plain bordered-text appearance, placed directly adjacent. Destructive actions must be visually distinct (e.g., red fill, warning icon, extra confirmation) so users don't accidentally trigger data loss. There is no visible confirmation step or undo mechanism indicated.

→ **Fix**: Style "Erase" with a red or cautionary color, add a warning icon, and ensure the action presents a confirmation dialog before executing. Separate it spatially from "Edit."

---

## Major Issues (Fix Soon)

### 4. Inconsistent Tile Visual Language — Mixed Fill vs. Outline Styles
**Principle Violated:** Consistency §1.4 — "Visual styles consistent throughout"

Controls and Sound tiles use solid green fills with white text. Display, Accessibility, and Network use colored text on pale/transparent backgrounds. There is no apparent semantic reason for this split — they are all settings categories at the same hierarchy level. This inconsistency makes the user question whether filled tiles are selected, locked, or functionally different.

→ **Fix**: Choose one consistent tile style for all categories. Use a single visual treatment (e.g., all outlined with colored icons, or all filled with a muted palette) and reserve the alternate style exclusively for the selected/active state.

### 5. Navigation Bar Colors Are Arbitrary and Carry No Semantic Meaning
**Principle Violated:** Color Guidelines §3.1 — "Limited palette (4-7 colors max for coding)"; Metaphors §1.1

The top bar uses at least 7 distinct saturated colors across 9 buttons (blue, red, green, dark green/teal, yellow, dark purple, light purple). These colors don't map to any recognizable system: WiFi is blue, Control is red, Home is green, Mail is teal, Star is yellow, Gear is dark purple. There is no mnemonic or metaphorical link. This creates visual noise rather than aiding recognition.

→ **Fix**: Either use a single accent color for all nav items (with the active item highlighted), or adopt a small, meaningful color vocabulary (e.g., warm = communication, cool = system). Reduce to 2–3 hues maximum.

### 6. Flat Visual Hierarchy — No Clear Primary Action or Entry Point
**Principle Violated:** Aesthetic Integrity §1.10 — "Clear visual hierarchy—most important elements stand out"

The bright yellow Name field, the green Controls tile, and the colorful navigation bar all compete for visual attention at roughly equal intensity. There is no clear reading order or focal point. A first-time user cannot quickly determine: "What should I do first?"

→ **Fix**: Establish a single focal point (likely the Name field, per the instruction text). Subdue competing elements — desaturate non-active tiles, use a neutral nav bar, and let the primary action area dominate through size and contrast.

### 7. Redundant Labeling — Text Inside Tiles AND Below Them
**Principle Violated:** Aesthetic Integrity §1.10 — "Graphics support understanding, not just decoration"; Plain Language §7.1

Every tile displays its name inside the tile AND repeats it as a caption below (e.g., the tile says "Controls" and the label below also says "Controls"). This wastes space, adds visual clutter, and provides no additional information.

→ **Fix**: Use one label per tile. If the tile text is legible, remove the caption below. If using icons instead of text inside tiles, keep the caption.

### 8. No Visible Back/Cancel/Close Mechanism
**Principle Violated:** User Control §1.6 — "User can always quit, save, or cancel"; Forgiveness §1.8 — "Cancel available in all dialogs"

There is no visible "Back," "Close," or "Cancel" button on this options screen. The L and R buttons on the nav bar might serve as shoulder-button navigation, but their function is not labeled. A user entering this screen has no obvious way to leave without saving unwanted changes.

→ **Fix**: Add a clearly labeled "Back" or "Close" button in a standard position (top-left or bottom of the panel). If L/R serve as back/forward, label them or add descriptive tooltips.

### 9. "Common" Group Label Is Weak and Incomplete
**Principle Violated:** See-and-Point §1.3; Perceived Stability §1.9

"Common" appears as a small, low-contrast label above the first row of tiles but does not visually enclose or group them. Worse, Sound and Network sit below the "Common" group without any section header — are they also "Common," or a different category? The information architecture is ambiguous.

→ **Fix**: Either visually enclose each group (bordered section, background fill, indentation) or use consistent headers for all groups. If all five tiles are "Common," extend the label or container to include Sound and Network.

### 10. Instruction Text Is Spatially Disconnected from Its Target
**Principle Violated:** Feedback §1.7; Metaphors §1.1 — actions and related elements should be proximate

The bottom bar reads "Enter a new name and customize your controls," but the Name field is at the top of the panel — the maximum distance apart. This violates spatial association: help text should be near the element it describes.

→ **Fix**: Place contextual hint text directly below the Name field (as placeholder text or a subtitle), or use the bottom bar only for global/contextual status, not field-specific instructions.

---

## Minor Issues (Consider Fixing)

### 11. Typography Uses Multiple Inconsistent Typefaces
**Principle Violated:** Consistency §1.4; Aesthetic Integrity §1.10

At least three typeface treatments are visible: a clean sans-serif for navigation labels, a handwritten/informal font for tile labels and the instruction bar, and a different weight/style for section headers. This creates a disjointed visual identity that feels unpolished.

→ **Fix**: Settle on one primary typeface (or at most one display + one body face). Use weight and size — not typeface changes — to create hierarchy.

### 12. Tile Sizes Are Not Uniform
**Principle Violated:** Consistency §1.4; Perceived Stability §1.9

The five category tiles are not the same dimensions. Controls appears slightly wider than Display; Accessibility is narrower and taller. Sound and Network are on a separate row with different spacing. A grid of interactive elements at the same hierarchy should use uniform sizing.

→ **Fix**: Normalize all category tiles to the same width and height. Use a consistent grid with equal gutters.

### 13. L and R Buttons Have Different Styling from Other Nav Items
**Principle Violated:** Consistency §1.4

L and R use a purple outline-only square style, while all other nav buttons are rounded, filled shapes with colored backgrounds. This makes L/R look like they belong to a different UI system.

→ **Fix**: Either unify L/R with the same pill/rounded style as other nav items, or clearly separate them as "hardware" indicators (e.g., place them outside the nav bar with a distinct gutter).

### 14. Corner Radii Are Inconsistent Across Components
**Principle Violated:** Consistency §1.4; Aesthetic Integrity §1.10

The outer panel has a large, soft corner radius. Tiles have a smaller, distinct radius. Navigation buttons use an even smaller pill shape. The Edit/Erase buttons appear to have sharp or minimal rounding. At least four different corner radius values are in play, giving the interface a patchwork feel.

→ **Fix**: Define 2–3 standard corner radius values (e.g., small for buttons, medium for cards, large for panels) and apply them consistently.

### 15. Active Navigation Item ("Gear") Highlight Is Ambiguous
**Principle Violated:** Feedback §1.7; Modes §6.1 — "Current mode is always visually indicated"

The Gear button has a yellow background highlight and an "Options" label below it, indicating it is the active tab. However, the highlight color (yellow) is unrelated to the button's own color (dark purple), and the "Options" label appears only for the active item. If other tabs also show labels when active, this is fine — but the static view suggests the label is an afterthought, not part of a consistent pattern.

→ **Fix**: Use a consistent active indicator across all nav items — an underline, a background tint using the item's own color, or a persistent label row where the active label is emphasized.

### 16. No Hover, Focus, or Pressed State Differentiation Visible
**Principle Violated:** Feedback §1.7 — "Immediate feedback confirms every action"; Direct Manipulation §1.2

The static screenshot reveals no evidence of hover, focus, or pressed states for any interactive element. While this is a game UI (and may use gamepad input), settings screens still benefit from clear focus indicators so the user always knows which element is targeted.

→ **Fix**: Add a visible focus ring, glow, or scale animation for the currently focused element, especially for gamepad/keyboard navigation.

### 17. The "@" Icon in the Name Field Has No Clear Meaning
**Principle Violated:** Metaphors §1.1 — "Icons and visual elements resemble real-world objects users already understand"; See-and-Point §1.3

The Name field shows an "@" symbol as a leading icon. In real-world convention, "@" signifies email or social handles, not a player/profile name. This metaphor mismatch may confuse users about whether this field expects an email address or a display name.

→ **Fix**: Replace the "@" icon with a person/profile silhouette icon, a pencil (for editing), or remove the icon entirely if the label "Name" is sufficient.

---

## Strengths

- **Clear conceptual model**: The screen communicates "settings" through familiar category names (Controls, Display, Sound, Network, Accessibility) — users understand the options taxonomy immediately.
- **Charming visual personality**: The handwritten font, soft rounded panel, and warm cream background create a friendly, approachable aesthetic consistent with a Kirby-style game.
- **Instruction text present**: The bottom bar provides contextual guidance, which is better than no guidance at all.
- **Edit and Erase affordances are visible**: Key actions are on-screen rather than hidden in menus, supporting See-and-Point.
- **Spatial grouping attempt**: The "Common" label and tile arrangement show an effort to organize settings into logical categories.

---

## Overall Score: 4/10

The kirby_options screen has a warm, inviting art direction that fits a casual game context, but it suffers from significant usability problems. The most critical issues are color-dependent state indicators (accessibility failure), dangerously low contrast on the Accessibility tile label, and a destructive "Erase" action with no visual safeguard. The visual hierarchy is flat — too many saturated colors compete for attention, and the user has no clear entry point. Redundant labels, inconsistent tile styling, mixed typography, and uneven component sizing compound the problem. With focused corrections to contrast, consistency, hierarchy, and destructive-action treatment, this screen could score significantly higher while retaining its charming personality.

---

*Audited against: Apple Human Interface Guidelines — The Apple Desktop Interface (1987)*
