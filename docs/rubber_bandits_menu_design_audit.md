# Design Audit: Rubber Bandits Menu Screen

**Audit Date:** 2026-02-07
**Screen:** `rubber_bandits_menu` — Main Menu with Options/Character Select
**Methodology:** Apple Human Interface Guidelines (1987) — Full Audit
**Auditor:** Automated Design Audit

---

## Screen Description

A full-screen game main menu UI set against a warm golden-yellow background. The top-left displays the game title "RUBBER BANDIT$" split across two overlapping horizontal bars — a brown/olive bar with "RUBBER" in dark text and a green-tinted bar with "BANDIT$" in bright green-yellow text. Below the title, a left-aligned menu panel with a thin dashed border contains six vertically stacked items: OPTIONS (currently selected with a gray background highlight and a ">" cursor), CONTROLS, LEADERBOARD, ARMORY, PLAY CREDITS, and BACK. The top-right shows a user status element reading "thia9uers – Online" in dark text on a light teal/mint background with a yellow circle badge showing "1" and a yellow star icon below. The middle-right has a floating notification: "Supporter pack available" with "More characters in shop" sub-text and a small "X" dismiss button. The bottom-center contains a character selector panel with a light yellow/cream background showing "SELECT CHARACTER" as a header, "EARL THE BIRD" as the current character name flanked by dark "LB" and "RB" controller buttons, and "FAVOURITE" as a sub-label. The bottom-left displays green helper text reading "Change the game's settings." The lower portion of the background shifts to a darker gold/brown tone.

---

## Critical Issues (Fix Immediately)

### 1. Helper Text Has Dangerously Low Contrast Against Background
**Principle Violated:** Color Guidelines (3.2), Accessibility (8.1)
The "Change the game's settings" text at the bottom-left uses a muted green color against the golden-yellow background. The contrast ratio appears to be well below the minimum 3:1 needed for legibility, let alone the 4.5:1 recommended for body text. This text serves as a contextual hint explaining the currently selected menu item, yet it is nearly invisible — users must strain to read it, and under typical TV viewing distances it would disappear entirely. The HIG explicitly requires that "text and thin lines have sufficient contrast to be easily visible."
→ **Fix:** Change the helper text to white, dark brown, or black — any color that creates a strong contrast ratio against the golden background. Alternatively, place the text inside a semi-transparent dark card or banner to guarantee legibility regardless of background variation.

### 2. Title Bars Overlap Creating Visual Confusion and Poor Readability
**Principle Violated:** Aesthetic Integrity (1.10), Consistency (1.4)
The game title "RUBBER BANDIT$" is split across two separate horizontal bars that physically overlap each other. The "RUBBER" bar sits on top with a brown/olive fill, and the "BANDIT$" bar sits below and to the right with a greenish fill. The overlap creates a muddy intersection zone where neither bar's background cleanly reads. This stacking also makes the title feel like two separate elements rather than a unified brand mark. The HIG's principle that "different things look different" is inverted here — a single thing (the game title) looks like two disconnected things. The use of "$" replacing "S" further reduces readability.
→ **Fix:** Unify the title into a single visual element — one bar, one background color, one text treatment. If the staggered two-line layout is essential to the brand identity, eliminate the overlap so each bar has clean visual boundaries, and ensure both lines use the same typographic treatment to read as a cohesive title block.

### 3. Selection State Uses Only a Subtle Background Color Change
**Principle Violated:** Color Guidelines (3.1), Accessibility (8.1)
The currently selected menu item ("OPTIONS") is indicated solely by a slightly darker gray background fill behind the text. The unselected items have a nearly identical off-white/cream background. This violates the fundamental HIG color guideline that "color coding should be redundant with shape, position, or pattern." The ">" cursor to the left helps, but it is extremely small and easy to overlook. For colorblind users or in suboptimal viewing conditions (glare, distance, low brightness), the selected item becomes indistinguishable from unselected items.
→ **Fix:** Add multiple redundant selection indicators: a prominent left-edge accent bar or arrow, bold text weight on the selected item, a visible border or outline, and/or a text color change. The ">" cursor should be made significantly larger and more visually distinct. Ensure the selection is clearly identifiable in a grayscale rendering.

### 4. Monochromatic Yellow/Gold Palette Eliminates Visual Differentiation
**Principle Violated:** Color Guidelines (3.1), Aesthetic Integrity (1.10), Accessibility (8.1)
Nearly the entire screen is rendered in variations of the same yellow-gold-brown hue family: the background is golden yellow, the menu panel is cream/off-white, the character selector is pale yellow, the title bars are olive/brown, and the notification panel is beige. When everything exists in the same color temperature, individual elements fail to stand out from each other or from the background. The HIG warns against "subtle color differences for important distinctions" — this screen makes every distinction subtle. The design would be nearly unreadable in grayscale.
→ **Fix:** Introduce at least one contrasting accent color (blue, teal, or a strong red) for interactive and selected states. Use a neutral white or dark panel background for the menu to create separation from the warm background. Reserve the gold palette for the background and brand elements, not for every UI surface.

---

## Major Issues (Fix Soon)

### 5. Scattered Layout Lacks Spatial Logic and Grouping
**Principle Violated:** Aesthetic Integrity (1.10), Perceived Stability (1.9)
UI elements are distributed across the screen without a clear grid or spatial relationship: the title is top-left, the user status is top-right, the menu panel is left-center, the notification is center-right, the character selector is bottom-center, and the helper text is bottom-left. Each element appears to be placed independently rather than as part of an organized composition. There is no visible column grid, no consistent margins, and no alignment between elements. The large empty center of the screen amplifies the feeling of disconnection. The HIG requires that the interface feel stable with "familiar landmarks" — but the scattered placement makes orientation difficult.
→ **Fix:** Establish a clear layout grid. Consider a two-column approach: left column for navigation (title + menu), right column for content/status. Or center all elements with consistent vertical stacking. Align element edges to shared guidelines and use consistent margins from screen edges.

### 6. Character Selector Panel Feels Disconnected from Main Navigation
**Principle Violated:** Consistency (1.4), See-and-Point (1.3)
The character selector at the bottom-center ("SELECT CHARACTER / EARL THE BIRD / FAVOURITE") uses a completely different visual treatment from the main menu panel — it has a solid border, a pale yellow fill, centered text, and controller button labels (LB/RB). It is spatially disconnected from the menu, sitting at the bottom of the screen with no visual link to the navigation flow above. Users cannot easily understand whether this is part of the main menu interaction, a separate mode, or a persistent status display. The HIG principle that "related items should be grouped" is not met.
→ **Fix:** Visually connect the character selector to the main menu by using consistent border styles, padding, and backgrounds. Consider placing it adjacent to or within the menu panel. Alternatively, if it's a persistent status element, give it a clearly distinct visual treatment (e.g., a fixed bottom bar with its own background strip) that reads as a separate layer.

### 7. Notification Panel Floats Without Clear Visual Hierarchy
**Principle Violated:** Aesthetic Integrity (1.10), Feedback and Dialog (1.7)
The "Supporter pack available / More characters in shop" notification sits in the middle-right area of the screen with a subtle bordered card treatment. It competes with the main menu and character selector for attention but has no visual priority — it uses the same muted color scheme as everything else. The small "X" close button is barely visible. Notifications should either demand attention with strong visual contrast or remain unobtrusive — this one does neither. It occupies prime screen real estate while being easy to overlook.
→ **Fix:** If this notification is important, give it a contrasting background color (e.g., a warm accent or bright badge) and a prominent dismiss button. If it's secondary, move it to a corner position, reduce its size, and use a subtle slide-in animation to draw initial attention without permanently occupying space.

### 8. Inconsistent Border Styles Across UI Panels
**Principle Violated:** Consistency (1.4), Aesthetic Integrity (1.10)
Three bordered panels are visible on screen, each using a different border treatment: the menu panel has a thin dashed/dotted border on cream, the notification has a thin solid border on beige, and the character selector has a heavier solid border on pale yellow. The user status element in the top-right uses a different colored background with rounded corners and no visible border. These inconsistencies make the UI feel assembled from different component libraries rather than designed as a unified system. The HIG states that "visual styles should be consistent throughout."
→ **Fix:** Standardize all panel borders to use the same style — either all solid at the same weight, all with the same corner radius, or all borderless with consistent shadow/spacing. Choose one panel treatment and apply it uniformly.

### 9. LB/RB Controller Labels Lack Alternative Input Indication
**Principle Violated:** See-and-Point (1.3), Accessibility (8.3)
The character selector shows "LB" and "RB" labels in dark rounded squares, indicating gamepad bumper buttons. However, there is no alternative indication for keyboard or mouse users — no arrow buttons, no clickable affordance, no keyboard shortcut shown. The HIG requires that "keyboard shortcuts are alternatives, never the only way" and by extension, controller-specific labels should be accompanied by universal alternatives. Users without a gamepad cannot discover how to change characters.
→ **Fix:** Add left/right arrow indicators alongside or instead of the LB/RB labels, or display contextual input hints that change based on the detected input device. Show both controller and keyboard alternatives (e.g., "LB / ← " and "RB / →").

### 10. ">" Cursor Indicator Is Too Small and Low Contrast
**Principle Violated:** Feedback and Dialog (1.7), Color Guidelines (3.2)
The ">" symbol serving as the menu selection cursor is rendered in a very small size, positioned to the left of the menu panel border. Its dark color against the golden background makes it somewhat visible, but its tiny size means it provides minimal feedback about the user's current position. In a game menu viewed on a TV at distance, this cursor would be nearly invisible. The HIG requires "immediate feedback confirms every action" — the cursor needs to be a prominent, unmissable visual element.
→ **Fix:** Replace the ">" with a larger, more visible indicator — a solid arrow/triangle, an animated chevron, or a full-height left-edge accent bar on the selected item. Consider adding animation (pulsing or gentle bouncing) to ensure the cursor is always noticeable.

---

## Minor Issues (Consider Fixing)

### 11. User Status Element Uses Inconsistent Visual Language
**Principle Violated:** Consistency (1.4), Color Guidelines (3.1)
The "thia9uers – Online" display in the top-right uses a light teal/mint background — the only teal element on the entire screen. The yellow circle badge with "1" and the yellow star icon below add two more unique visual treatments found nowhere else. This element looks like it was imported from a different application's UI. The HIG requires that visual styles be consistent throughout — this status element breaks the screen's warm color palette with a cool mint tone that has no relationship to any other element.
→ **Fix:** Restyle the user status to use the same visual language as the other panels — the warm cream/yellow palette with consistent borders. The online status could use a small green dot indicator rather than an entirely different background color.

### 12. "FAVOURITE" Label Is Ambiguous Without Context
**Principle Violated:** Plain Language (7.1), See-and-Point (1.3)
Below the character name "EARL THE BIRD," the label "FAVOURITE" appears in small uppercase text. It is unclear whether this is a status (this character is marked as favourite), a category (this is the favourites section), or an action prompt (press to favourite). The HIG requires messages to be "concise and simple" with clear meaning — a single word label without visual context (no star icon, no toggle indicator, no button affordance) leaves users guessing.
→ **Fix:** Clarify the label's meaning with supporting visuals: if it's a status, add a filled star icon next to it; if it's an action, style it as a button with appropriate affordance. Consider using a phrase like "★ Favourite" or "Set as Favourite" to remove ambiguity.

### 13. Menu Item Spacing Is Too Tight for Comfortable Navigation
**Principle Violated:** Aesthetic Integrity (1.10), Direct Manipulation (1.2)
The six menu items (OPTIONS through BACK) are stacked with minimal vertical padding between them. The text appears cramped within the panel, and individual items lack clear visual separation. When navigating with a gamepad D-pad, tight spacing increases the cognitive load of tracking which item is selected, especially combined with the subtle selection highlight (Issue #3). The HIG's principle that elements should be easily targetable and visually distinct is undermined.
→ **Fix:** Increase vertical padding between menu items by at least 30-50%. Add subtle horizontal dividers or alternating background shading between items. Each menu item should feel like a distinct, independently targetable row with comfortable breathing room.

### 14. Title Uses "$" Symbol Replacing "S" — Harms Readability
**Principle Violated:** Plain Language (7.1), Metaphors (1.1)
The game title reads "BANDIT$" with a dollar sign replacing the final "S." While this is a stylistic brand choice evoking money/heist themes, it introduces a moment of cognitive friction during reading. Users must mentally substitute the symbol, which violates the HIG's plain language principle that interfaces should be immediately readable without interpretation. The "$" also renders poorly at small sizes and can be confused with the number "5" at a glance.
→ **Fix:** This is a brand identity decision that may be intentional, but if readability is a priority, consider using the "$" as a decorative element adjacent to the title rather than as a letter substitution. Alternatively, use a font where the "$" is designed to closely resemble an "S" to minimize the cognitive disconnect.

### 15. Background Color Transition Has No Visual Purpose
**Principle Violated:** Aesthetic Integrity (1.10), Perceived Stability (1.9)
The background shifts from a lighter golden yellow in the upper portion to a darker gold/brown in the lower portion of the screen. This gradient or color split has no apparent functional purpose — it doesn't correspond to a UI zone division, doesn't separate navigation from content, and doesn't align with any panel edges. It creates a subtle visual inconsistency that undermines the feeling of a stable, uniform canvas. The HIG warns against changes that don't feel like the user's doing.
→ **Fix:** Unify the background to a single consistent color, or make the color transition purposeful by aligning it with a clear UI division (e.g., darker tone for a bottom status bar area, lighter tone for the content area above).

### 16. Dismiss Button on Notification Is Too Small to Target Easily
**Principle Violated:** User Control (1.6), Direct Manipulation (1.2)
The "X" dismiss button on the "Supporter pack available" notification is rendered as a very small element in the top-right corner of the notification card. It appears to be approximately 12-16px in size, well below the recommended minimum touch/click target of 44px. Users who want to dismiss this notification — exercising their right to control their interface — will struggle to accurately target this button, especially with a gamepad or on a TV display.
→ **Fix:** Increase the dismiss button to at least 32x32px with a clear circular or square background. Consider also allowing the notification to be dismissed with a controller button (e.g., "B to dismiss") shown as a label within the notification.

### 17. Mixed Case and Typography Treatments Across Labels
**Principle Violated:** Consistency (1.4), Aesthetic Integrity (1.10)
The screen mixes multiple text case conventions: all-caps for menu items ("OPTIONS," "CONTROLS"), all-caps for the character name ("EARL THE BIRD"), all-caps for the label ("FAVOURITE"), small-caps for "SELECT CHARACTER," title case for the notification ("Supporter pack available"), and sentence case for the helper text ("Change the game's settings"). Each text element appears to use a different size, weight, and case treatment. The HIG requires consistent visual styles throughout — the current typographic inconsistency fragments the screen's visual identity into at least five different text styles.
→ **Fix:** Establish a clear typographic hierarchy: choose one case convention for headings (all-caps or title case), one for body/labels, and one for helper text. Use a maximum of 3 distinct type sizes with consistent weights. Apply these rules uniformly across all panels.

---

## Strengths

- **All menu options visible at once:** The six-item menu presents all available actions on screen simultaneously, supporting the "see-and-point" principle — users can scan and select without navigating through nested submenus or pages.
- **Contextual helper text present:** The "Change the game's settings" text attempts to provide context-sensitive descriptions tied to the selected menu item, which is a strong UX pattern even though the current contrast makes it hard to read.
- **Character selector provides persistent context:** Showing the current character at the bottom of the main menu keeps users aware of their selection without requiring navigation to a separate screen, supporting WYSIWYG principles.
- **Simple, flat menu structure:** The menu avoids unnecessary complexity — six clearly labeled items with no submenus, tabs, or multi-level navigation. Users can reach any destination in one selection.
- **Playful visual identity maintained:** The golden-yellow palette, hand-crafted title treatment, and character naming style ("EARL THE BIRD") successfully convey a lighthearted, casual game tone that matches the "Rubber Bandits" brand.

---

## Overall Score: 3/10

The Rubber Bandits menu screen achieves a recognizable brand identity with its warm golden palette and playful character, but this comes at significant cost to usability and visual clarity. The most damaging issues are the near-invisible helper text (critical contrast failure), the monochromatic color scheme that eliminates differentiation between interactive elements, and the sole reliance on a subtle background color change for selection state feedback. The scattered layout — with elements placed in all four corners and the center-bottom without grid alignment — makes spatial orientation difficult. Inconsistent border treatments, typography, and color temperatures across the five distinct panel types make the screen feel assembled from disparate components rather than designed as a cohesive system. The overlapping title bars, ambiguous labels ("FAVOURITE"), and undersized interactive targets (the ">" cursor, the "X" dismiss button) compound the accessibility and usability problems. To improve substantially, the design needs: (1) a contrasting accent color for interactive/selected states, (2) redundant selection indicators beyond color, (3) a defined spatial grid aligning all elements, and (4) a unified panel component style applied consistently across all bordered areas.
