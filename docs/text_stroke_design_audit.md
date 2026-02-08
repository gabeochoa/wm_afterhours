# Design Audit: Text Stroke / Outline

**Screen:** Text Stroke / Outline showcase  
**Source:** Game UI toolkit showcase  
**Audited Against:** Apple Human Interface Guidelines (1987 Desktop Interface)  
**Date:** 2026-02-08

---

## Audit Summary: Text Stroke / Outline

### Critical Issues (Fix Immediately)

1. **Low-Contrast Descriptive Text (Color §3.2, Accessibility §8.1)**: The small gray descriptive captions beneath each example ("Same yellow color with dark outline for contrast," "Thick 8px stroke creates a chunky game-style effect," etc.) are rendered in a very low-contrast light gray against the dark navy background. These lines are nearly illegible at smaller display sizes and fail WCAG contrast minimums. The HIG states that text and thin lines must have sufficient contrast to be easily visible.  
   → **Fix**: Increase the caption text color brightness to at least a medium gray (#AAAAAA or lighter) or white with reduced opacity (rgba 255,255,255,0.7). Ensure a minimum 4.5:1 contrast ratio against the background.

2. **"Invisible" White Text Demonstrates a Problem Without Solving It (WYSIWYG §1.5, Feedback §1.7)**: The bottom-left section shows white text on a slightly lighter background explicitly labeled "Invisible." While this is intended as a cautionary example, it actually produces unreadable content on screen — the word "WHITE" is genuinely invisible on the left side of the comparison box. The HIG principle of WYSIWYG says what's on screen should be legible and meaningful. Displaying intentionally unreadable text without any visual warning indicator (e.g., a cautionary icon, strikethrough, or red border) confuses whether this is a feature or a bug.  
   → **Fix**: Add a warning icon or a red "avoid" overlay to the "Invisible" example to clearly mark it as an anti-pattern. Alternatively, use a subtle crosshatch or strikethrough pattern to signal "don't do this" rather than simply showing broken text.

3. **Color Is the Primary Way to Distinguish Stroke Examples (Color §3.1, Accessibility §8.1)**: Each example pair — yellow/no-stroke vs. yellow/stroked, orange/bold, cyan/red contrast, dark/glow — relies entirely on color differences to demonstrate the stroke effect. A colorblind user (deuteranopia or protanopia) would struggle to distinguish the "NO STROKE" yellow from the "WITH STROKE" yellow-with-dark-outline, and the cyan-vs-red "CONTRAST" example would appear as similar muddy tones. The HIG requires that color coding be redundant with shape, position, or pattern.  
   → **Fix**: Add non-color indicators to each comparison: labels like "off" / "on," side-by-side arrows, or outline the stroked version with a dashed border. Use shape differences (e.g., the no-stroke example could be plain while the stroked example gets a subtle background card) so the distinction is clear even without color vision.

4. **All Examples Use ALL-CAPS, Hiding Stroke Behavior on Mixed-Case Text (WYSIWYG §1.5)**: Every single example — "NO STROKE," "BOLD," "CONTRAST," "GLOW," "STROKE," "WHITE" — is rendered entirely in uppercase. This means users cannot evaluate how stroke effects will look on lowercase letters, descenders (g, p, y), or mixed-case titles, which are common in real game UIs. The screen doesn't represent what users will actually get in typical usage.  
   → **Fix**: Include at least one mixed-case example (e.g., "Game Title" or "Player Name") to show how strokes interact with ascenders, descenders, and varied letterforms.

---

### Major Issues (Fix Soon)

5. **No Section Headers for Left Column (Aesthetic Integrity §1.10, Consistency §1.4)**: The right column has a clear header — "Outline Thickness:" — but the left column of example pairs has no equivalent heading. The left side implicitly demonstrates "stroke styles" or "use cases" but this is never labeled. Users must infer the purpose by reading the individual captions. Different sections should look different and be clearly identified.  
   → **Fix**: Add a section header above the left column examples, such as "Stroke Styles:" or "Use Cases:" to match the "Outline Thickness:" header on the right.

6. **Uneven Vertical Spacing Between Left-Column Examples (Aesthetic Integrity §1.10, Perceived Stability §1.9)**: The vertical gaps between the four left-column examples (NO STROKE/WITH STROKE, BOLD, CONTRAST, GLOW) and the white comparison box vary noticeably. The gap between the "GLOW" description and the white box is larger than the gap between other examples, and the caption text sits at inconsistent distances from its parent example. This creates a visually unstable layout where elements don't feel anchored to a grid.  
   → **Fix**: Establish a consistent vertical rhythm — equal spacing between each example block (title + caption). Use a baseline grid (e.g., 8px or 16px increments) to align all elements uniformly.

7. **Tip Bar Buried at Bottom (See-and-Point §1.3, Feedback §1.7)**: The most actionable guidance on the entire screen — "Tip: Use 2-6px for readable text, 6-10px for decorative titles and headers" — is placed in a small bar at the very bottom of the panel. Users scanning top-to-bottom may never reach it, or may not notice it against the dark background. Important guidance should be prominent and early, not buried.  
   → **Fix**: Move the tip to the top of the screen below the subtitle, or repeat it inline next to the thickness examples. Alternatively, use a more prominent visual treatment (larger text, an icon, or a highlighted callout box).

8. **Noisy Textured Backgrounds on Stroke Examples (Aesthetic Integrity §1.10)**: Several of the "STROKE" examples in the right column (particularly the 6px, 8px, and 10px variants) appear to have a textured or noisy fill pattern inside the letterforms. This visual noise makes it harder to evaluate the actual stroke effect and distracts from the purpose of the demonstration. The HIG advises that graphics should support understanding, not just serve as decoration.  
   → **Fix**: Use clean, solid fills for the text in thickness examples so the stroke effect is the sole visual focus. Save textured fills for a separate "advanced effects" section if needed.

9. **Two-Column Layout Has No Visual Divider (Aesthetic Integrity §1.10)**: The left column (style examples) and right column (thickness reference) sit side by side with only whitespace separating them. There is no divider line, background color change, or card boundary to indicate these are two distinct sections. A user scanning quickly may perceive them as one undifferentiated mass of stroked text.  
   → **Fix**: Add a subtle vertical divider line, a faint background card behind each column, or increase the horizontal gap between the two sections to create clear visual separation.

10. **Caption Text Uses Inconsistent Typographic Style (Consistency §1.4)**: The descriptive captions under the left-column examples appear to use a small, monospace-like font, while the "Outline Thickness:" header and thickness labels ("2px — subtle," "4px — medium") use a different typeface and weight. The subtitle under the title ("Create bold outlines for game titles and headers") uses yet another style. Three or more competing text styles without a clear hierarchy creates visual confusion.  
    → **Fix**: Establish a maximum of three typographic levels: (1) title, (2) section headers/labels, (3) body/caption text. Apply them consistently across both columns.

---

### Minor Issues (Consider Fixing)

11. **Title Uses Stroke Effect, Blurring Chrome vs. Content (Metaphors §1.1)**: The page title "Text Stroke / Outline" itself uses a stroke/outline effect, making it simultaneously an example of the feature and the page heading. This blurs the line between the interface chrome (the title telling you what page you're on) and the content (demonstrations of the feature). Users may momentarily wonder if the title is another example to evaluate.  
    → **Fix**: Render the page title in a clean, un-stroked style to clearly separate it as navigation chrome. Let the examples below be the sole demonstrations of the effect.

12. **"Invisible" and "Visible" Labels Are Too Small (Accessibility §8.1, Plain Language §7.1)**: The labels "Invisible" and "Visible" beneath the white comparison box are rendered in extremely small text, smaller than the already-small captions elsewhere. They are critical to understanding the example's purpose but are the hardest text on the entire screen to read.  
    → **Fix**: Increase the size of these labels to match the caption text used elsewhere. Consider making them bold or using a distinct color to draw attention.

13. **No Indication of Interactivity or Context (User Control §1.6, Direct Manipulation §1.2)**: The screen is a static showcase with no interactive affordances. Users cannot adjust stroke thickness, change colors, or experiment with their own text. While this may be intentional as a reference card, the HIG values direct manipulation — letting users see effects by doing, not just by looking at pre-rendered examples.  
    → **Fix**: Consider adding an interactive "Try it" section where users can type text and adjust stroke thickness/color with sliders, seeing the result in real time. Even a simple live preview would dramatically increase the educational value.

14. **White Comparison Box Breaks the Dark Theme (Consistency §1.4, Aesthetic Integrity §1.10)**: The white/light-gray comparison box at the bottom-left is the only light-background element on an otherwise dark-themed screen. While it serves to demonstrate the visibility problem of white text on white backgrounds, its stark brightness is visually jarring and breaks the cohesive dark aesthetic. It draws disproportionate attention relative to the main examples above it.  
    → **Fix**: Reduce the brightness of the comparison box slightly (use a light gray, ~#E0E0E0, instead of pure white) or add a subtle dark border around it to integrate it better with the dark theme. Alternatively, frame it as a clearly marked inset "caution" panel with its own visual treatment.

15. **Thickness Labels Use Inconsistent Dash Formatting (Consistency §1.4)**: The thickness labels read "2px — subtle," "4px — medium," etc., using an em-dash with spaces. However, the visual weight and spacing of these dashes varies slightly across lines, likely due to font rendering at different positions. More importantly, mixing "px" units (developer terminology) with plain-English descriptors ("subtle," "medium") without clear visual separation creates a hybrid technical/casual tone.  
    → **Fix**: Use a consistent separator — either a colon ("2px: subtle"), a pipe ("2px | subtle"), or place the descriptor on a second line. Consider replacing "px" with a more user-friendly term like "thin" / "thick" or just use the descriptive word alone with a visual reference.

16. **No Visual Hierarchy Among Thickness Examples (Aesthetic Integrity §1.10)**: The five thickness examples (2px through 10px) are presented with equal visual weight — same font size, same position, same treatment. There is no indication of which thickness is recommended or most commonly used. Users scanning quickly get a flat list with no guidance beyond the buried tip at the bottom.  
    → **Fix**: Visually emphasize the recommended range (e.g., highlight the 4px "medium" example as a default or add a "recommended" badge). Use subtle background shading or a border to call out the most commonly useful option.

17. **Subtitle Text Is Redundant and Adds Clutter (Aesthetic Integrity §1.10)**: The subtitle "Create bold outlines for game titles and headers" restates what the title and examples already communicate. It adds a line of text without providing new information, pushing the actual examples further down the screen. The HIG advises avoiding visual clutter.  
    → **Fix**: Remove the subtitle or replace it with genuinely useful guidance (e.g., "Choose a stroke style and thickness for your game text") that adds informational value beyond what the title conveys.

---

### Strengths

- **Clear Demonstration of Before/After**: The "NO STROKE" vs. "WITH STROKE" comparison at the top immediately communicates what text stroke does, making the feature self-evident without requiring explanation.
- **Progressive Thickness Scale**: The right column's 2px → 10px thickness scale with descriptive labels ("subtle," "medium," "bold," "chunky," "heavy") provides a well-organized, easy-to-scan reference that helps users choose appropriate values.
- **Cohesive Dark Theme**: The dark navy background is appropriate for a game UI toolkit and reduces eye strain. The bright, colorful text examples pop effectively against the dark canvas.
- **Practical Tip Included**: The tip at the bottom ("Use 2-6px for readable text, 6-10px for decorative titles and headers") provides actionable, specific guidance that helps users make informed design decisions — even if its placement could be improved.
- **Variety of Use Cases Shown**: The four left-column examples (contrast enhancement, bold/chunky, high-contrast color pairing, glow effect) cover distinct real-world applications, giving users a sense of the feature's versatility.
- **White Text Cautionary Example**: Including the "Invisible" vs. "Visible" white-on-white comparison is a thoughtful addition that warns users about a common pitfall, even if its execution could be improved.

---

### Overall Score: 4/10

The Text Stroke / Outline showcase communicates its core concept effectively — users can immediately understand what text stroke is and how thickness affects the result. The progressive thickness scale and variety of style examples are well-conceived. However, the screen suffers from significant accessibility and readability issues: critical caption text is nearly invisible due to low contrast, the page relies heavily on color distinctions without non-color alternatives, and the all-caps-only examples hide how strokes behave on real mixed-case text. The layout lacks clear sectioning (no left-column header, no column divider, inconsistent spacing), and the most actionable guidance is buried at the bottom. The "Invisible" white text example, while conceptually valuable, is executed in a way that creates genuinely unreadable content without clearly marking it as an anti-pattern. Addressing the contrast and accessibility critical issues, adding section structure, and including at least one mixed-case example would substantially improve this screen's utility as a design reference.
