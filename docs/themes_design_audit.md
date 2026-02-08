# Design Audit: Themes

**Screen:** Theme Switcher with Component Preview  
**Source:** Game UI toolkit showcase  
**Audited Against:** Apple Human Interface Guidelines (1987 Desktop Interface)  
**Date:** 2026-02-08

---

## Audit Summary: Themes

### Critical Issues (Fix Immediately)

1. **Title Text Clipped and Obscured (Aesthetic Integrity §1.10, WYSIWYG §1.5)**: The screen title is partially hidden — only "itcher" is visible, with the rest (likely "Theme Switcher") cut off behind or overlapping with the left panel. Users cannot read the title of the screen they are on. The HIG requires that what is on screen is legible and accurately represents the interface state. A clipped title signals broken layout and undermines the user's sense of where they are.  
   → **Fix**: Ensure the full title is visible and does not overlap with any panel. Increase the header area width or reposition the left panel so the title has adequate space. The title should be the first thing a user reads when the screen opens.

2. **Theme Name "Ocean Navy" Is Truncated (WYSIWYG §1.5, Feedback §1.7)**: The third theme option displays as "Ocean Navy (" — the text overflows its container and is visibly clipped mid-character, with a stray parenthesis suggesting additional content that doesn't fit. Users cannot read the full theme name, making it impossible to make an informed selection. This violates the core WYSIWYG principle: what's on screen must represent reality without hidden content.  
   → **Fix**: Widen the theme list buttons to accommodate the longest theme name, use text truncation with an ellipsis if space is constrained, or shorten theme names to fit. Never show partial text with a dangling parenthesis.

3. **No Close, Back, or Cancel Button (User Control §1.6, Forgiveness §1.8)**: The theme switcher screen has no visible close button, back arrow, cancel option, or any mechanism to dismiss the screen and return to the previous view. The HIG mandates that the user must always be able to quit, save, or cancel. Being trapped in the theme switcher with no exit violates fundamental user control.  
   → **Fix**: Add a clearly visible close or back button (e.g., an "X" in the top-right corner, or a "Back" / "Done" button). Ensure the user can always leave this screen.

4. **Pervasive Low Contrast — Dark Blue on Dark Blue (Color §3.2, Accessibility §8.1)**: The entire interface suffers from a narrow color value range. Panel backgrounds, borders, container backgrounds, and text all occupy similar dark blue tones. The "Component Preview" panel border is barely distinguishable from the outer background. The slider track, toggle background, and checkbox row all blend together. The HIG explicitly warns that text and thin lines must have sufficient contrast to be easily visible, and that edges require clear definition.  
   → **Fix**: Increase contrast between background layers. Use a lighter panel background or darker outer background to create clear depth separation. Ensure all text meets a minimum contrast ratio (WCAG AA recommends 4.5:1 for normal text). Add stronger borders or shadows to distinguish panels from the surrounding area.

---

### Major Issues (Fix Soon)

5. **Color Swatch Labels Are Cryptic Single Letters (See-and-Point §1.3, Plain Language §7.1)**: The color palette row displays four swatches labeled "P," "S," "A," and "D." These are design-system abbreviations (Primary, Secondary, Accent, Disabled) that mean nothing to end users. The HIG principle of "recognition over recall" requires that users understand elements by looking at them, not by decoding abbreviations. Users should never need to memorize or guess what single-letter codes mean.  
   → **Fix**: Use full labels: "Primary," "Secondary," "Accent," "Disabled" — either beneath each swatch or as tooltips on hover. If space is limited, use recognizable two-letter or three-letter abbreviations (e.g., "Pri," "Sec," "Acc," "Dis") or remove labels entirely and rely on the swatch colors alone with a legend.

6. **Button Labels "Hard," "Soft," "Accent" Are Design Jargon (Plain Language §7.1)**: The three button variants are labeled with design-system terminology ("Hard," "Soft," "Accent") rather than user-facing vocabulary. End users selecting a theme don't think in terms of button hardness or softness. These terms are meaningful to designers and developers but opaque to everyone else.  
   → **Fix**: If this is a developer-facing component preview, label them as "Button Variant: Solid," "Button Variant: Outline," "Button Variant: Accent" or similar descriptive terms. If user-facing, replace with action labels that describe what the buttons do (e.g., "Save," "Cancel," "Submit") to show how the theme affects real UI.

7. **Checkbox Uses a Text "v" Instead of a Standard Checkmark (Consistency §1.4, Metaphors §1.1)**: The checkbox component displays a lowercase "v" character as its checked indicator rather than a standard checkmark symbol (✓) or filled square. This breaks the real-world metaphor of a checkbox — users expect a recognizable checkmark, not an arbitrary letter. The inconsistency with standard controls makes the interface feel unpolished and potentially confusing.  
   → **Fix**: Replace the "v" with a proper checkmark glyph (✓ or ✔), or use a filled/indented visual state that clearly communicates "checked" without relying on a letter character.

8. **Selected Theme Indicator Is Nearly Invisible (Feedback §1.7, Selection §5.1)**: The currently selected theme ("Cozy Kraft") is indicated by a thin border around the button, but the border color is very subtle against the dark background. Users need immediate, unambiguous visual feedback about what is selected. The current indicator is so understated that a user scanning quickly could miss which theme is active.  
   → **Fix**: Make the selected theme visually prominent: use a bold border, a filled/highlighted background, a checkmark icon, or a distinctly different color state. The selected item should be instantly recognizable at a glance without close inspection.

9. **Slider/Progress Bar Has No Numeric Value (WYSIWYG §1.5)**: The slider component in the preview area shows a partially filled bar but displays no percentage, numeric value, or label indicating what it represents or its current position. Users cannot determine the exact value. The HIG requires the screen to fully represent the current state — hidden information violates WYSIWYG.  
   → **Fix**: Add a numeric value label (e.g., "65%") adjacent to the slider, or display the value in a tooltip on hover. If this is a preview-only component, label it (e.g., "Slider Preview") so users understand it is not interactive.

10. **"Surface Text" Label Has Dangerously Low Contrast (Color §3.2, Accessibility §8.1)**: The "Surface Text" sample in the component preview is rendered in a light gray tone on the dark panel background. While this may intentionally demonstrate a surface/muted text color, the resulting contrast is extremely low and difficult to read. If this color is used in actual UI text, it will fail accessibility standards. The HIG states that small objects and text must use high-contrast colors.  
    → **Fix**: Ensure the "Surface Text" demonstration color still meets minimum contrast requirements (WCAG AA 4.5:1). If the theme's surface text color is inherently low-contrast, flag it visually in the preview (e.g., with a warning icon or "Low contrast" note).

---

### Minor Issues (Consider Fixing)

11. **Toggle Label Overlaps the Control Area (Aesthetic Integrity §1.10, Controls §2.7)**: The "Toggle" text label is positioned inside or overlapping the toggle switch's track area rather than appearing as a separate adjacent label. This creates visual confusion about whether the text is part of the control or a label describing it. Standard practice (and the HIG's guidance on controls) calls for labels to be clearly separated from the interactive control element.  
    → **Fix**: Position the "Toggle" label to the left of the switch, clearly separated from the interactive track and thumb. The label and control should be visually distinct elements.

12. **No Theme Descriptions or Previews Before Selection (See-and-Point §1.3)**: Users see only theme names ("Cozy Kraft," "Neon Dark," "Midnight," etc.) with no thumbnail, color sample, or description of what each theme looks like. To preview a theme, users must select it and observe the component preview panel. This forces a trial-and-error workflow rather than the HIG's "see-and-point" principle of recognizing options by looking at them.  
    → **Fix**: Add small color swatches, thumbnail previews, or brief descriptions next to each theme name in the selection list. Even two or three colored dots showing the theme's primary colors would help users make informed choices without clicking through each option.

13. **Inconsistent Vertical Spacing in Component Preview (Aesthetic Integrity §1.10)**: The vertical spacing between preview components is irregular. The gap between the color swatches and the slider differs from the gap between the slider and checkbox, which differs again from the gap between the checkbox and toggle. Inconsistent spacing creates visual noise and makes the layout feel unstructured.  
    → **Fix**: Establish a consistent vertical rhythm (e.g., 12px or 16px between each component row) and apply it uniformly throughout the component preview panel.

14. **Loading Bar Context Is Ambiguous (Feedback §1.7)**: A "Loading: 72%" bar appears at the bottom of the screen. It is unclear whether this indicates an actual loading operation (e.g., theme assets loading) or is simply a preview of the loading bar component styled in the current theme. This ambiguity violates the feedback principle — users should always know what is happening. If it's real, they'll wait; if it's a demo, they'll be confused about why something is loading.  
    → **Fix**: If this is a component preview, label it clearly (e.g., "Loading Bar Preview" or place it within the Component Preview panel). If it represents actual loading, provide context about what is being loaded and an estimated time.

15. **No Visible Focus or Hover State on Theme Buttons (Feedback §1.7, Selection §5.1)**: The theme selection buttons show no hover, focus, or keyboard-navigation indicator. In a game context (likely controller-navigated), users need to know which element is currently focused. Without a visible focus state, keyboard and controller users have no way to determine where they are in the list.  
    → **Fix**: Add a visible focus ring, highlight background, or border change to the currently focused/hovered theme button. This is essential for both keyboard accessibility and controller navigation.

16. **Overlapping Panel Edges Create Visual Confusion (Perceived Stability §1.9, Windows §2.1)**: The left "Select Theme" panel and the center header area overlap or share ambiguous boundaries. The header text "itcher" appears behind or between panels, and the rounded corners of both panels intersect at unclear boundaries. This creates a sense of visual instability — the user cannot clearly parse where one panel ends and another begins.  
    → **Fix**: Give each panel clear, non-overlapping boundaries with adequate spacing between them. Use distinct elevation (shadow or border) to establish a clear front-to-back panel ordering. Panels should never overlap in a way that clips content.

17. **Theme List Has No Scrollbar or Count Indicator (Feedback §1.7, Scroll Bars §2.2)**: The theme list shows five themes, but there is no scrollbar, scroll indicator, or count (e.g., "5 themes") to tell users whether more themes exist below the visible area. If there are additional themes, users have no way to discover them. The HIG requires scroll bars to represent the entire document dimension.  
    → **Fix**: If the list is complete, no action is needed. If additional themes exist off-screen, add a scrollbar or visual indicator (e.g., a fade-out at the bottom edge, or a "↓ more" cue). Consider showing a count like "5 of 12 themes."

---

### Strengths

- **Clear Two-Panel Layout Concept**: The screen correctly separates theme selection (left) from theme preview (right), establishing a logical spatial relationship between choosing and seeing the result. This supports the direct manipulation principle.
- **Live Component Preview**: Showing actual UI components (buttons, toggles, checkboxes, sliders, text) styled in the selected theme gives users immediate WYSIWYG feedback about how the theme will look in practice. This is a strong application of the feedback principle.
- **Dark Theme Appropriate for Game Context**: The dark color scheme is appropriate for a game UI toolkit, reducing eye strain during gameplay and maintaining visual consistency with the game environment.
- **Themed Color Palette Display**: Showing the primary, secondary, accent, and disabled color swatches gives users a quick overview of the theme's color system, supporting informed decision-making.
- **Loading Bar as Preview Component**: Including a loading bar in the component preview demonstrates how the theme affects progress indicators, which is a thoughtful addition to the preview set.
- **Descriptive Theme Names**: Names like "Cozy Kraft," "Neon Dark," "Ocean Navy," "Midnight," and "Sage Natural" are evocative and give users a general sense of each theme's character, which is better than generic names like "Theme 1."

---

### Overall Score: 3.5/10

The Themes screen has a sound conceptual foundation — a two-panel layout with theme selection on the left and live component preview on the right is the right approach. However, the execution has serious problems across nearly every HIG category. The most damaging issues are the pervasive low contrast (dark blue on dark blue makes the entire interface hard to parse), clipped/truncated text in both the title and theme names, and the absence of any close or back button. The component preview uses design-system jargon ("P," "S," "A," "D," "Hard," "Soft") that is meaningless to end users, and the checkbox's text "v" violates standard control conventions. The selected theme state is nearly invisible, spacing is inconsistent, and panels overlap in confusing ways. Addressing the contrast issues alone would dramatically improve readability, and adding full labels, proper checkmarks, and navigation controls would bring this screen from a rough prototype to a usable theme selection experience.
