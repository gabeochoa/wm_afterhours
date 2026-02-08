# Design Audit: Flexbox Alignment Demo

**Screen:** Flexbox Alignment Demo — game UI toolkit showcase  
**Audited Against:** Apple Human Interface Guidelines (1987)  
**Date:** 2026-02-07  

---

## Critical Issues (Fix Immediately)

### 1. Color-Only Distinctions Between Conceptual Groups (Color Guidelines §3.1)
Teal blocks represent vertical/column concepts, while maroon/red blocks represent horizontal/row and cross-axis concepts. Color is the **sole differentiator** between these groups — there is no redundant shape, pattern, icon, or label treatment to distinguish them. A colorblind user (particularly with red-green deficiency) would struggle to tell the groups apart.

→ **Fix**: Add a secondary visual encoding — use different shapes (e.g., tall rectangles for vertical, wide rectangles for horizontal), border styles, or pattern fills to reinforce the color distinction.

### 2. Developer Jargon Instead of Plain Language (Plain Language §7.1)
Labels throughout use raw CSS property names: "JustifyContent", "AlignItems", "SelfAlign". These are programmer vocabulary, not user vocabulary. The guidelines explicitly state: *"User vocabulary, not programmer vocabulary"* and *"No jargon or technical terms."*

→ **Fix**: Replace technical labels with human-readable descriptions. E.g., "Vertical Spacing (JustifyContent, Column)" → "Vertical Spacing" with a tooltip for the technical name. If this is a developer tool, at minimum provide both a plain description and the technical term, with the plain description given visual priority.

### 3. Insufficient Contrast for Section Headers (Color Guidelines §3.2)
The parenthetical subtitles — "(JustifyContent, Column)", "(JustifyContent, Row)", "(AlignItems)", "(SelfAlign)" — are rendered in a small, muted color against the dark panel backgrounds. These fail to meet readable contrast thresholds. Text and thin lines must have *"sufficient contrast to be easily visible."*

→ **Fix**: Increase the font size or brightness of subtitle text. Use at least a medium gray (#AAAAAA or brighter) against the dark background, or switch to a lighter panel background.

### 4. Weak Tab Selection Indicator (Feedback §1.7, Aesthetics §1.10)
The selected tab ("All") is indicated only by a thin, low-contrast border. The remaining tabs ("Vertical", "Horizontal", "Cross-Axis", "Self Align") have no visible border or differentiation. This provides almost no feedback about the current state. Users cannot quickly scan and determine which view is active.

→ **Fix**: Use a strong visual treatment for the selected tab: filled background, bold text, underline bar, or a significantly contrasted border. Unselected tabs should appear visually recessive but still readable.

---

## Major Issues (Fix Soon)

### 5. Flat Visual Hierarchy — Everything Competes (Aesthetics §1.10)
The screen title, tab labels, section headers, sub-labels ("Start", "End", "Center", etc.), and content blocks all exist at nearly the same visual weight. The guidelines warn against *"flat visual hierarchy — everything same prominence."* The eye has no clear entry point or reading order.

→ **Fix**: Establish a clear typographic scale: large bold title → medium bold section headers → regular sub-labels → small metadata. Use size, weight, and spacing to create at least 3 distinct hierarchy levels.

### 6. Excessive Nesting of Dark Panels Creates Visual Noise (Aesthetics §1.10)
The layout uses at least 4 layers of nested dark rectangles: outermost background → left/right panels → individual demo containers → blocks inside containers. The subtle tonal differences between these layers make boundaries hard to distinguish and create visual clutter. The guidelines flag *"visually confusing displays"* as a red flag.

→ **Fix**: Reduce nesting to 2 levels maximum. Use spacing (whitespace) instead of nested containers to group content. If containers are needed, increase the tonal contrast between nesting levels or use borders on only the most important grouping.

### 7. No Visible Affordances on Interactive Elements (See-and-Point §1.3)
The tab bar items do not visually communicate that they are clickable. They look like plain text labels. The guidelines require that *"all available actions are visible"* and users should be able to discover actions *"by looking, not remembering."*

→ **Fix**: Style tabs with clear button-like affordances — background fills, hover states, cursor changes, or underlines that signal interactivity.

### 8. Inconsistent Visual Treatment of Sub-Labels (Consistency §1.4)
In the left panel (Vertical Spacing), the sub-labels "Start", "End", "Center", "Between", "Around" appear as plain white text. In the right panel (Horizontal Spacing), the same conceptual labels appear as filled button-like elements with colored backgrounds (teal). This inconsistency violates the principle that *"visual styles consistent throughout."*

→ **Fix**: Use the same label treatment for both panels. Either both should use plain text labels or both should use styled label chips. The visual language for equivalent concepts must be uniform.

### 9. Cramped Spacing in the Left Panel (Aesthetics §1.10)
The Vertical Spacing section packs 5 demo columns into a single row, leaving minimal gutters between them. The blocks inside overlap visual boundaries and the "Between" / "Around" columns are particularly tight. This density makes it hard to parse individual examples.

→ **Fix**: Increase horizontal gutters between demo columns by at least 50%. Alternatively, reduce the number of visible examples and let users toggle between them, or allow the panel to scroll horizontally.

### 10. No Mode Indication for Tab Content (Modes §6.1, Stability §1.9)
The tabs represent view modes ("All", "Vertical", "Horizontal", "Cross-Axis", "Self Align"), but the current mode is barely indicated (see issue #4). The guidelines require that *"current mode is always visually indicated"* and that mode changes are obvious.

→ **Fix**: When switching tabs, provide a clear visual transition. The active tab should be unmistakable — consider a filled background color, a prominent underline, or a contrasting text color that makes the active state obvious at a glance.

---

## Minor Issues (Consider Fixing)

### 11. No Legend or Color Key (See-and-Point §1.3)
There is no explanation of what the teal and maroon/red color families mean. Users must infer that color corresponds to axis/direction. A demo screen should be self-documenting.

→ **Fix**: Add a small legend or inline annotation explaining the color coding (e.g., teal = column layout, maroon = row layout).

### 12. Inconsistent Corner Radius on Blocks (Consistency §1.4)
The numbered blocks ("1", "2", "3") appear to have slightly different corner radii depending on the section — the teal vertical blocks look more squared off, while the maroon horizontal blocks look more rounded. Consistency requires that *"same action produces same result throughout."*

→ **Fix**: Standardize corner radius across all demo blocks to a single value (e.g., 4px or 8px).

### 13. Title Placement Crowds the Tab Bar (Perceived Stability §1.9)
"Flexbox Alignment Demo" sits very close to the tab row below it. There is minimal vertical breathing room between the title and the navigation, making the top of the screen feel compressed.

→ **Fix**: Add at least 12–16px of padding between the title and the tab row. This gives the title prominence and separates navigation from identification.

### 14. Numbers Inside Blocks Lack Typographic Refinement (Aesthetics §1.10)
The numbers "1", "2", "3" inside blocks use what appears to be a hand-drawn or casual serif font. While stylistically charming, the varying baselines and sizes (the "2" in the Cross-Axis section floats higher) can look like rendering errors rather than intentional design in a technical demo context.

→ **Fix**: Use a clean, monospaced or sans-serif font for the block numbers to reinforce the technical/demo nature of the screen. Ensure consistent baseline alignment.

### 15. Right Panel Has Excessive Unused Space (Aesthetics §1.10)
The "Individual Override (SelfAlign)" section in the bottom-right has four items ("Start", "Auto", "Center", "End") scattered across a large empty area. The visual weight of this section is disproportionately low compared to the dense left panel, creating an unbalanced layout.

→ **Fix**: Either reduce the container size to better fit the content, or use the space intentionally with annotations/explanations. Balance visual weight between left and right panels.

### 16. Tab Labels Use Mixed Terminology (Consistency §1.4)
The tab "Self Align" uses a space, while the section header renders it as "SelfAlign" (camelCase). Similarly, "Cross-Axis" uses a hyphen in the tab but may not match other usages. Terminology should be consistent across all occurrences.

→ **Fix**: Standardize naming: pick either "Self Align" or "Self-Align" and use it everywhere. Same for "Cross-Axis" vs "Cross Axis."

### 17. No Hover or Focus States Visible (Feedback §1.7, Accessibility §8.3)
There is no indication of hover or focus states on any interactive element. The guidelines require *"immediate feedback confirms every action"* — even the pre-action of hovering over a clickable element should produce visual feedback.

→ **Fix**: Add hover states (subtle background color change or underline) to tabs and any other interactive elements. Add focus rings for keyboard navigation.

---

## Strengths

- **Clear conceptual grouping**: The left panel handles vertical/column concepts and the right panel handles horizontal/row concepts — the spatial metaphor is sound.
- **Effective use of live examples**: Rather than describing alignment behavior in text, the demo shows actual positioned blocks, making the concept concrete and tangible. This aligns with the WYSIWYG principle.
- **Consistent numbering**: All demo blocks use "1", "2", "3" consistently, making it easy to track element order across different alignment modes.
- **Tab-based filtering**: The top tab bar offers a reasonable progressive disclosure pattern — users can view everything at once or drill into a specific category.
- **Dark theme execution**: The overall dark palette is cohesive and suitable for a game UI toolkit context, reducing eye strain during extended use.

---

## Overall Score: 4/10

The Flexbox Alignment Demo effectively communicates its core concept — showing how different alignment properties position child elements — but it suffers from significant usability and visual design issues. The most critical problems are **color-only distinctions** (accessibility failure), **developer jargon** in labels (violating plain language), **dangerously low contrast** on subtitle text, and a **nearly invisible tab selection state**. The flat visual hierarchy and excessive dark-on-dark nesting make the screen harder to parse than it needs to be. Addressing the critical and major issues would substantially improve both readability and usability, likely raising the score to 6–7/10.
