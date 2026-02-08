# Design Audit: animation_spring ("Springy!")

**Audit Date:** 2026-02-07
**Screen:** animation_spring — Spring animation showcase with five animation presets
**Guidelines:** Apple Human Interface Guidelines (1987)
**Audit Type:** Standard (Full Checklist)

---

## Screen Description

A dark-themed showcase screen titled "Springy!" displaying five colored boxes (pink, cyan, yellow, lime, purple) labeled with animation type names ("Boing!", "Jelly", "Balloon", "Drop", "Rocker"). A bounce counter reads "Bounces: 0" and a "Restart!" button sits at the bottom. The first box appears selected with a white border. The entire layout sits inside a container with extremely rounded corners on a near-black background.

---

## Critical Issues (Fix Immediately)

### 1. Color-Only Distinction Between Animation Types
**Principle Violated:** Color Guidelines §3.1, Accessibility §8.1
**Observation:** The five animation presets are differentiated exclusively by color (pink, cyan, yellow, lime, purple). No shape, pattern, icon, or other redundant visual cue distinguishes them. A colorblind user relying on deuteranopia or protanopia vision would struggle to differentiate the yellow, lime, and pink boxes.
→ **Fix:** Add unique icons, patterns, or shape variations to each box so they can be distinguished without color. For example, give each box a small icon or distinct border pattern.

### 2. Label Text Critically Low Contrast and Size
**Principle Violated:** Color Guidelines §3.2, Accessibility §8.1
**Observation:** The labels above each box ("Boing!", "Jelly", "Balloon", "Drop", "Rocker") use an extremely small, light-colored font against the dark background. They are barely legible and would be unreadable for users with any degree of visual impairment. The contrast ratio appears well below 4.5:1 minimum.
→ **Fix:** Increase label font size to at least match the instruction text size. Use a bright white or high-contrast color (not gray) for the labels. Consider placing labels inside or below the boxes for better association.

### 3. No Visual Affordance for Clickable Boxes
**Principle Violated:** Direct Manipulation §1.2, See-and-Point §1.3
**Observation:** The colored boxes appear as flat, static colored squares. Nothing about their visual treatment (no shadow, no gradient, no border, no hover cursor indication) communicates that they are interactive/clickable elements. Users must read the instruction text to discover interactivity.
→ **Fix:** Add depth cues such as subtle drop shadows, border highlights, or a slight gradient to suggest the boxes are pressable. Add a hover state (e.g., slight scale-up or glow) and a cursor:pointer indication.

### 4. Selection State Ambiguous and Inconsistent
**Principle Violated:** Feedback §1.7, Selection §5.1
**Observation:** The first box ("Boing!") shows a thin white border indicating selection, but no other visual change (no bold label, no checkmark, no background shift). The remaining four boxes have no border at all, making it unclear whether they are selectable, already deselected, or simply decorative. The selection feedback is too subtle.
→ **Fix:** Make the selected state dramatically more visible — use a thicker border, a glow effect, an underlined or bolded label, or a checkmark badge. Ensure unselected boxes still have a visible border (e.g., dark gray) so the contrast with the selected state is clear.

---

## Major Issues (Fix Soon)

### 5. Severe Vertical Spacing Imbalance
**Principle Violated:** Aesthetic Integrity §1.10
**Observation:** Content clusters at the top ~40% of the screen. A massive empty void (~35% of screen height) separates the "Bounces: 0" counter from the "Restart!" button. The layout feels top-heavy and poorly distributed. The empty space serves no purpose and makes the interface feel incomplete.
→ **Fix:** Either vertically center the content group as a whole, reduce the gap between counter and button, or fill the space with additional contextual information (e.g., animation parameter display, timeline visualization).

### 6. Inconsistent Typography — Four Competing Styles
**Principle Violated:** Consistency §1.4, Aesthetic Integrity §1.10
**Observation:** At least four distinct typographic treatments are used: (1) "Springy!" in a large decorative/handwritten font, (2) box labels in a tiny monospace/pixel font, (3) "Bounces: 0" in a medium serif-like font, and (4) instruction text in a small sans-serif style. This many competing typefaces undermine visual coherence.
→ **Fix:** Establish a clear typographic hierarchy using at most two font families. Use size, weight, and color to create hierarchy rather than switching fonts. The decorative title font can remain if box labels, counter, and instruction text all share a single secondary font.

### 7. No Feedback for Interaction Outcome
**Principle Violated:** Feedback §1.7
**Observation:** The screen shows "Bounces: 0" but provides no visual indication of what will happen when a box is clicked. There's no preview, no animation cue, and no state change visible in this static state. Users cannot anticipate the result of their interaction.
→ **Fix:** Add subtle idle animations (a slight breathing/pulse) to hint at the animation behavior each box will produce. Consider showing a small preview animation loop on hover.

### 8. Instruction Text Lacks Visual Prominence
**Principle Violated:** Aesthetic Integrity §1.10, Plain Language §7.1
**Observation:** "Click the boxes or restart to see them bounce" is critical instructional text — it's the only explanation of how to interact with the screen. Yet it's rendered in a small, low-emphasis font below the title divider. First-time users could easily overlook it.
→ **Fix:** Increase the instruction text size or give it more visual weight (e.g., slightly brighter color, more spacing above/below). Position it closer to the interactive elements it describes rather than near the title.

### 9. Decorative Title Font Reduces Legibility
**Principle Violated:** Plain Language §7.1, Aesthetic Integrity §1.10
**Observation:** "Springy!" uses a stylized, handwritten-style font. While charming, the letterforms are less legible than standard fonts — the 'S' and 'p' are particularly stylized. This prioritizes decoration over function.
→ **Fix:** Consider a rounded sans-serif font that conveys playfulness while maintaining legibility, or increase the size further to compensate for the decorative font's lower readability.

### 10. Button Styling Disconnected from Design Language
**Principle Violated:** Consistency §1.4
**Observation:** The "Restart!" button uses a teal/dark cyan color that doesn't appear anywhere else in the interface. Its color, shape, and styling feel disconnected from the five colorful boxes above. The button doesn't share the same rounded-corner radius or visual treatment as the boxes.
→ **Fix:** Align the button's styling with the overall design system. Use a neutral color (white outline on dark) or one of the existing palette colors. Match the corner radius to the boxes. Consider adding a subtle border treatment consistent with the selected-box style.

### 11. Exclamation Mark Overuse Creates Noise
**Principle Violated:** Plain Language §7.1
**Observation:** Three elements use exclamation marks: "Springy!", "Boing!", and "Restart!". This creates an over-excited, noisy tone that dilutes the impact of each. Exclamation marks should be reserved for genuinely emphatic moments.
→ **Fix:** Remove exclamation marks from at least two of the three. "Springy" works as a title without one. "Restart" is a standard action that doesn't need emphasis. Keep "Boing!" if desired since it's an onomatopoeia.

---

## Minor Issues (Consider Fixing)

### 12. Outer Container Border Barely Perceptible
**Principle Violated:** Aesthetic Integrity §1.10, Color Guidelines §3.2
**Observation:** The outer container is a barely-perceptible dark gray (#2a2a2a-ish) against a near-black (#1a1a1a-ish) background. The extremely low contrast between container and background makes the container boundary nearly invisible. It adds visual noise without clear purpose.
→ **Fix:** Either increase the contrast of the container border/background so it's clearly visible, or remove the container entirely if the content doesn't need visual grouping.

### 13. Color Saturation Imbalance Across Boxes
**Principle Violated:** Aesthetic Integrity §1.10, Color Guidelines §3.1
**Observation:** The five box colors have inconsistent perceived brightness and saturation. The yellow and lime-green boxes are significantly brighter/lighter than the pink, cyan, and purple boxes. This creates uneven visual weight — the eye is drawn to the center-right boxes disproportionately.
→ **Fix:** Balance the perceived brightness of all five colors. Use colors with similar luminance values so no single box dominates visually. Test with a grayscale filter to verify even visual weight.

### 14. Inconsistent Corner Radii Across Elements
**Principle Violated:** Consistency §1.4
**Observation:** At least three different corner radii are visible: (1) the outer container has extremely large/oval radii, (2) the colored boxes have medium radii, and (3) the button has smaller radii. This inconsistency in geometric treatment weakens visual cohesion.
→ **Fix:** Establish a corner radius scale (e.g., small: 4px, medium: 12px, large: 24px) and apply consistently. The boxes and button should share the same radius. The outer container can use a larger one if intentionally distinct.

### 15. No Grouping Container for Interactive Elements
**Principle Violated:** Aesthetic Integrity §1.10, Perceived Stability §1.9
**Observation:** The boxes, labels, and bounce counter float freely in dark space without a clear visual container grouping them as related interactive elements. The "Restart!" button floats independently far below. Users can't immediately see what belongs together.
→ **Fix:** Add a subtle panel or card background behind the interactive group (boxes + labels + counter) to visually group related elements. Position the button closer to or within this group.

### 16. "Bounces: 0" Counter Lacks Context
**Principle Violated:** Feedback §1.7, WYSIWYG §1.5
**Observation:** The "Bounces: 0" counter is displayed but its purpose is unclear without interaction. Is it a score? A count of total animations played? Per-box or global? The label doesn't explain its role in the interface.
→ **Fix:** Add a brief subtitle or tooltip explaining what the counter tracks. Consider "Total bounces:" or placing it in a more contextual position (e.g., next to the boxes, not isolated below them).

### 17. No Visible Disabled/Loading States
**Principle Violated:** Feedback §1.7, Perceived Stability §1.9
**Observation:** There is no indication of what the boxes look like when disabled, during animation, or in a loading state. All five boxes appear identical (minus the selection border on the first) regardless of state.
→ **Fix:** Define and implement distinct visual states: default, hover, active/pressed, selected, and disabled. During animation playback, the active box should show a distinct "playing" indicator.

---

## Strengths

- **Clear primary instruction:** The instruction text "Click the boxes or restart to see them bounce" concisely explains the interaction model
- **Labeled presets:** Each animation type has a descriptive name, aiding discoverability and recall
- **User control maintained:** The "Restart!" button gives users explicit control over the animation cycle — users initiate actions, not the computer
- **Forgiveness:** The restart concept implies non-destructive interaction — users can freely explore without fear of breaking anything
- **Spatial layout is left-to-right:** The box arrangement follows natural reading order, making it easy to scan

---

## Overall Score: 3.5/10

The "Springy!" screen demonstrates a playful concept but suffers from significant usability issues. The most critical problems are accessibility-related: color-only differentiation, near-illegible labels, and missing interactive affordances. The visual design shows inconsistency across typography (four competing styles), corner radii (three different values), and color treatment (saturation imbalance, disconnected button color). The layout wastes substantial vertical space and groups elements poorly. While the interaction model is simple and forgiving (a strength), the lack of feedback, hover states, and clear selection indicators means users must discover the interface through trial and error rather than visual cues — a direct violation of the See-and-Point principle. Fixing the critical contrast and affordance issues would have the highest impact on usability.
