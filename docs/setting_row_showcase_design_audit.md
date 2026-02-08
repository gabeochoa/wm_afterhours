# Design Audit: Setting Row Showcase

**Audited Against:** Apple Human Interface Guidelines (1987)
**Screen:** Setting Row Showcase — game UI toolkit settings panel
**Date:** 2026-02-07

---

## Audit Summary

The Setting Row Showcase presents a game settings panel with three control sections (Toggles, Options, Volume) using a dark theme with a handwritten typeface. While the layout demonstrates a reasonable organizational structure, the screen suffers from significant contrast, affordance, and consistency issues that undermine usability across multiple HIG categories.

---

## Critical Issues (Fix Immediately)

### 1. Extremely Low Section Header Contrast (Color Guidelines §3.2 — Text Contrast)
**Observation:** The section headers "TOGGLES", "OPTIONS", and "VOLUME" are rendered in a dim teal/cyan on the near-black background. They are barely legible and nearly invisible at normal viewing distances.
→ **Fix:** Increase section header brightness to at least a 4.5:1 contrast ratio. Use a lighter tint or white with reduced opacity (e.g., 70% white) so headers are clearly readable without competing with row labels.

### 2. Color-Only Toggle State Distinction (Color Guidelines §3.1 / Accessibility §8.1)
**Observation:** The ON/OFF state of toggles is communicated primarily through color — blue for ON, dark charcoal for OFF. While small "ON"/"OFF" text appears beside each toggle, it is extremely small and low-contrast, making color the de facto only distinguisher. A colorblind user (deuteranopia or protanopia) may struggle to differentiate states.
→ **Fix:** Add a secondary non-color indicator: move the toggle knob visibly left/right, add a checkmark icon inside the ON state, or make the ON/OFF label text substantially larger and higher contrast. The design should be fully comprehensible on a monochrome display.

### 3. Sliders Lack a Visible Thumb/Handle (Direct Manipulation §1.2)
**Observation:** The Master Volume and Music Volume sliders show a green filled region and a gray unfilled track, but there is no visible drag handle or thumb. Users cannot see what to "grab" to adjust the value. This directly violates the Direct Manipulation principle — users need to see a physical object they can grab and move.
→ **Fix:** Add a visible circular or rectangular thumb at the boundary between the filled and unfilled portions of each slider. The thumb should have a distinct color and slight elevation (shadow or border) to communicate that it is draggable.

### 4. Toggle Controls Visually Overlap (Aesthetic Integrity §1.10 / Perceived Stability §1.9)
**Observation:** The three toggle switches for Music, Sound Effects, and Vibration appear to vertically overlap or crowd into each other's space. The toggle for "Sound Effects" clips into the rows above and below it, creating visual noise and making it hard to associate each toggle with its label.
→ **Fix:** Increase vertical spacing between toggle rows so that each toggle has clear breathing room. Each row should have at least 8–12px of clear space above and below the toggle control.

---

## Major Issues (Fix Soon)

### 5. Decorative Font Reduces Legibility (Plain Language §7.1 / Aesthetic Integrity §1.10)
**Observation:** The entire screen uses a handwritten/casual typeface (appears to be a monospaced handwriting font). While stylistically fitting for a game, it significantly reduces legibility — particularly for the smaller text like ON/OFF labels, percentage values, and section headers. HIG emphasizes that text must be easily readable.
→ **Fix:** Use the decorative font only for the title ("Setting Row Showcase"). Switch body labels, values, and section headers to a clean, legible sans-serif or the system UI font. If the game aesthetic requires the handwritten look everywhere, at minimum increase font size and weight for all functional text.

### 6. Inconsistent Control Color Language (Consistency §1.4)
**Observation:** Active toggles use a medium blue color, while active slider fills use a muted green. These are the two primary accent colors on screen, but they serve the same semantic role (indicating an "active" or "filled" state). Using two different colors for the same meaning violates internal consistency.
→ **Fix:** Unify the accent color across all interactive controls. Either use blue for both toggles and slider fills, or green for both. A single accent color creates a coherent visual language.

### 7. Stepper Chevrons Have Weak Affordance (See-and-Point §1.3)
**Observation:** The Language and Graphics options use thin angle brackets `〈 English 〉` and `〈 High 〉` as stepper controls. These chevrons are thin, low-contrast, and easy to mistake for decorative punctuation rather than interactive controls. Users must recognize these as buttons rather than being able to see them as buttons.
→ **Fix:** Make the chevrons larger and bolder, or replace them with filled arrow buttons with a visible hit target (e.g., a subtle rounded rectangle background on hover/focus). The interactive elements should look tappable/clickable at a glance.

### 8. No Visible Focus or Selection State (Feedback §1.7)
**Observation:** The Music toggle row appears to have a faint white rectangular outline, which may indicate focus or selection. However, this indicator is extremely subtle — barely visible against the dark background. No other row shows any hover, focus, or active state styling. HIG requires immediate, clear feedback for every user action.
→ **Fix:** Implement a clearly visible focus ring (2px+ solid outline or background highlight) that contrasts strongly with the dark background. Apply consistent focus/hover/active states to all interactive rows.

### 9. No Cancel/Back/Close Affordance (User Control §1.6 / Forgiveness §1.8)
**Observation:** The settings panel has no visible close button, back arrow, or cancel option. Users have no obvious way to dismiss the panel or revert changes. This violates User Control (user should always be able to quit or cancel) and Forgiveness (users need a way to back out).
→ **Fix:** Add a clearly visible close/back button in the top-left or top-right corner of the panel. If settings are applied immediately, consider adding a "Reset to Defaults" option so users can recover from unwanted changes.

### 10. Percentage Labels Are Too Small and Low-Contrast (Color Guidelines §3.2)
**Observation:** The "80%" and "65%" values next to the volume sliders are small, thin, and rendered in a medium gray that provides insufficient contrast against the dark background. These are the primary feedback for the slider's current value and should be prominently readable.
→ **Fix:** Increase the font size of percentage labels by at least 2 points and use a brighter color (white or near-white). The value is the most important piece of information for a slider — it should be the most readable element in the row.

### 11. No Visible Panel Boundary or Frame (Aesthetic Integrity §1.10 / Windows §2.1)
**Observation:** The settings panel has only a very subtle dark background differentiation from the surrounding black area. There is no clear border, shadow, or visual frame defining the panel's boundaries. This makes the panel feel like it floats ambiguously rather than presenting as a stable, defined surface.
→ **Fix:** Add a subtle but visible border (1px lighter stroke) or a slight drop shadow around the panel to clearly define its boundaries. The panel should feel like a tangible surface — a "window" the user is interacting with.

---

## Minor Issues (Consider Fixing)

### 12. ALL-CAPS Section Headers Reduce Readability (Plain Language §7.1)
**Observation:** "TOGGLES", "OPTIONS", and "VOLUME" are rendered in all-uppercase small text. All-caps text is harder to read than mixed-case because it removes word-shape cues. Combined with the already low contrast, this makes headers nearly invisible.
→ **Fix:** Use title case ("Toggles", "Options", "Volume") and increase font size slightly. If all-caps is desired for stylistic reasons, add letter-spacing (tracking) to improve legibility.

### 13. Footer Text Lacks Purpose or Interactivity (Aesthetic Integrity §1.10)
**Observation:** The bottom of the screen displays "Toggle, Stepper, Slider, Dropdown, Display" — this appears to be a developer-facing label listing the control types on screen. It provides no value to an end user and adds visual clutter. It also implies "Dropdown" and "Display" controls exist but are not shown.
→ **Fix:** Remove this footer text for production UI. If this is a showcase/demo screen, visually separate it from the settings panel (e.g., render it outside the panel frame in a clearly distinct developer annotation style).

### 14. Inconsistent Vertical Spacing Between Rows (Consistency §1.4 / Perceived Stability §1.9)
**Observation:** The vertical spacing between rows appears inconsistent. The toggle rows are tightly packed (overlapping), while the stepper rows and slider rows have more generous spacing. Inconsistent rhythm makes the interface feel unstable and poorly structured.
→ **Fix:** Establish a consistent row height and spacing value (e.g., 48px row height with 8px gap) and apply it uniformly to all setting rows regardless of control type.

### 15. Label-to-Control Alignment Is Inconsistent (Aesthetic Integrity §1.10)
**Observation:** Labels are left-aligned while their corresponding controls (toggles, steppers, sliders) are placed at varying horizontal positions. The toggles sit further right than the steppers, and the sliders start at yet another horizontal position. This creates a ragged, unstructured right column.
→ **Fix:** Establish a consistent left margin for all controls, or use a two-column layout with a fixed split point. All controls should start at the same horizontal position to create a clean, grid-aligned layout.

### 16. ON/OFF Labels Inconsistently Positioned Relative to Toggles (Consistency §1.4)
**Observation:** The "ON" and "OFF" text labels appear to the right of each toggle, but their exact positioning shifts slightly between rows — partly due to the toggle overlap issue. The text should be at a fixed offset from the toggle in every row.
→ **Fix:** Position state labels at a consistent horizontal offset from the right edge of each toggle control, or integrate the state label directly into the toggle (e.g., text inside the toggle track).

### 17. Title Underline Decoration Is Arbitrary (Aesthetic Integrity §1.10)
**Observation:** The title "Setting Row Showcase" has a horizontal rule beneath it. This line has no consistent styling — it appears to be a simple thin white line. It doesn't match any other visual element in the panel and feels like an afterthought rather than an intentional design element.
→ **Fix:** Either remove the line (using spacing alone to separate the title from content) or style it to match the panel's visual language (e.g., use the same teal accent color as section headers, or a subtle gradient fade).

---

## Strengths

- **Clear organizational grouping:** Settings are divided into logical sections (Toggles, Options, Volume) which helps users find what they need.
- **WYSIWYG toggle states:** The toggle ON/OFF and slider percentage values provide immediate, visible feedback about current settings — the user can see the current state at a glance.
- **Readable primary labels:** Despite the decorative font, the main setting labels ("Music", "Sound Effects", "Vibration", etc.) are large enough to be legible.
- **Appropriate control types:** Toggles for binary choices, steppers for discrete options, and sliders for continuous values demonstrate correct control-to-data-type mapping.
- **Dark theme is appropriate for games:** The dark background is standard for game settings screens and reduces eye strain during gameplay sessions.

---

## Overall Score: 4/10

The Setting Row Showcase demonstrates sound structural thinking — settings are logically grouped and appropriate control types are used for each data type. However, the screen is undermined by pervasive contrast failures, missing affordances (no slider thumbs, weak stepper buttons), inconsistent control coloring, and overlapping toggle elements. The decorative typography, while on-brand for a game, comes at a significant legibility cost. Most critically, the lack of a close/back button and the color-only toggle state distinction represent functional gaps that would impede real users. Addressing the critical and major issues would substantially improve both usability and visual polish.
