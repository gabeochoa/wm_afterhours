# Design Audit: Meters & Gauges

**Screen:** `meters_gauges` — Game UI Toolkit Showcase
**Audited Against:** Apple Human Interface Guidelines (1987)
**Date:** 2026-02-07

---

## Critical Issues (Fix Immediately)

### 1. Color-Only Semantics for HP / MP / XP Meters (HIG 3.1 — Color Fundamentals)
The three inline meters rely **entirely on color** to communicate their identity and state. HP is green-with-red, MP is green-with-blue, XP is blue-with-yellow. Remove color and all three bars become indistinguishable filled rectangles. The HIG requires that color coding be redundant with shape, position, or pattern — here it is the sole differentiator.
→ **Fix**: Add unique icons (heart for HP, star for MP, diamond for XP), distinct fill patterns (solid, striped, dotted), or vary the bar shape/caps per stat so each meter remains identifiable without color.

### 2. Traffic-Light Color Scheme on Circular Gauges Implies Judgment Without Explanation (HIG 3.1, 7.1 — Color Fundamentals, Plain Language)
The three circular gauges use a red ring at 25%, a yellow ring at 50%, and a green ring at 90%. This traffic-light mapping (red = bad, yellow = okay, green = good) implies a health/status judgment, but no legend, label, or description explains the thresholds or semantics. A colorblind user sees three identical gray rings with different fill levels. A sighted user is left guessing whether the colors are meaningful or decorative.
→ **Fix**: Add explicit labels ("Low", "Medium", "High" or a semantic label for each gauge). If the color-to-value mapping is intentional, provide a legend. Add pattern or stroke-style differentiation (dashed, segmented, solid) so the gauges are distinguishable without color.

### 3. Low-Contrast Section Headers and Percentage Labels (HIG 3.2 — Text Contrast)
The section headers ("Inline Meters", "Stacked Bars (Health + Shield)", "Animated Progress", "Circular Gauges", "Interactive Control") are rendered in a small, dim pixel font against the dark navy background. The estimated contrast ratio is approximately 2.5–3:1, well below the 4.5:1 minimum needed for readable body text. These headers are the primary navigational anchors for the screen but are nearly invisible.
→ **Fix**: Increase section header text lightness to at least `#C0C0C0` or brighter. Consider increasing font size for section headers to at least 1.5× body text size to establish clear hierarchical separation.

### 4. Excessive and Semantically Inconsistent Color Palette (HIG 3.1 — Limited Palette)
The screen uses at minimum 9 distinct hues: green (HP fill), red (HP empty), dark green (MP fill), blue (MP empty, shield, animated progress), light blue (XP fill), yellow/gold (XP empty), teal (animated fill), red-orange (25% gauge), and gold-yellow (50% gauge). The HIG recommends 4–7 colors maximum, each with a consistent semantic role. Here, green appears in HP, MP, stacked health, and the 90% gauge — meaning different things each time. Blue appears as MP empty, shield, and animated progress fill — again with no consistent meaning.
→ **Fix**: Establish a semantic color system: one color for health-related metrics, one for resource/mana, one for progression/XP, and a neutral for empty/unfilled tracks. Apply consistently across all sections.

---

## Major Issues (Fix Soon)

### 5. HP Bar Uses Red for Empty Portion — False Alarm Signal (HIG 3.2, 1.10 — Color Usage, Aesthetic Integrity)
The HP meter fills green to 65% and shows bright red for the remaining 35%. Red universally signals danger, damage, or critical state, but here it merely represents the unfilled portion of the bar. At a glance, a player would read this as "I'm taking damage" or "health is critical" rather than "I have 65% health remaining." The red creates a false urgency that contradicts the actual state.
→ **Fix**: Use a dark, desaturated track color (dark gray, dimmed version of the fill color) for the unfilled portion of all bars. Reserve red exclusively for genuinely critical/low states (e.g., below 20%).

### 6. Inconsistent Label Placement Across Sections (HIG 1.4 — Consistency)
Labels appear in different positions depending on the section: HP/MP/XP labels are large text **to the left** of the bars; stacked bar labels ("Health: 65%", "Shield: 29%") are **inside** the bars; the animated progress label ("60%") floats **above** the bar; circular gauge percentages sit **below** the rings; and the interactive control label ("Value: 65%") is **inside** the slider. This inconsistency forces the user to re-learn where to find the label for each meter type. The HIG requires the same action/pattern to work the same way everywhere.
→ **Fix**: Standardize label placement. Place all percentage values either consistently above, inside, or below their respective meters. Use a single pattern for all bar-type indicators and a single pattern for all circular indicators.

### 7. Stacked Bars Do Not Visually Communicate "Stacking" (HIG 1.1, 1.10 — Metaphors, Aesthetic Integrity)
The section is titled "Stacked Bars (Health + Shield)" but the two bars are displayed as separate, side-by-side horizontal bars with no visual overlap or stacking. In game UI, "stacked" bars typically overlay shield on top of health to show the combined defensive total. The current presentation is indistinguishable from two independent bars, which contradicts the "stacked" label.
→ **Fix**: Either overlay the shield bar on top of the health bar (true stacking), or use a segmented single bar where shield and health are adjacent segments of one bar. If they are intentionally separate, rename the section to "Paired Bars" or "Health & Shield."

### 8. Monospace/Pixel Font Reduces Readability at Small Sizes (HIG 1.10 — Aesthetic Integrity)
All text — section headers, labels, percentages, and the main title — uses a monospace pixel-art typeface. While thematically consistent with a game UI, the low x-height, uniform character width, and pixel aliasing make small labels ("Inline Meters", "Health: 65%") difficult to read, particularly at the section-header size. The HIG states that graphics and typography should support understanding, not just decoration.
→ **Fix**: Reserve the pixel/display font for the main title and stat abbreviations (HP, MP, XP). Use a proportional sans-serif for section headers, descriptions, and percentage values to improve legibility at small sizes.

### 9. "Interactive Control" Lacks Visible Interaction Affordances (HIG 1.2, 1.7 — Direct Manipulation, Feedback)
The "Interactive Control" slider at the bottom shows a filled bar with "Value: 65%" but has no visible thumb, grab handle, hover state, or any indication that it is draggable. The HIG requires that interactive elements provide clear affordances so users know what they can grab, move, or manipulate. This bar looks identical to the non-interactive "Animated Progress" bar above it.
→ **Fix**: Add a visible slider thumb/handle at the current value position. Add a subtle border or shadow to distinguish it from display-only bars. Show cursor changes on hover to signal interactivity.

### 10. Flat Visual Hierarchy — Sections Compete for Attention (HIG 1.10 — Aesthetic Integrity, Visual Hierarchy)
All five sections have roughly equal visual weight: same background darkness, same header size, same content density. There is no primary focal point. The user's eye has no natural entry point or reading path. The HIG emphasizes that the most important elements should stand out and different things should look different.
→ **Fix**: Create a clear hierarchy by varying section sizes, background values, or content prominence. Consider making the section that demonstrates the primary use case (e.g., inline meters or interactive control) more visually prominent, with secondary sections receiving less visual weight.

### 11. HP / MP / XP Labels Vary in Typographic Weight vs. Bar Proportion (HIG 1.4, 1.10 — Consistency, Aesthetics)
"HP", "MP", and "XP" are rendered in large, bold text to the left of their bars, creating a heavy visual anchor. But the bars themselves vary in width and fill, and the percentage labels inside the bars are much smaller. The large labels draw disproportionate attention away from the actual data (the fill level). This is further inconsistent with other sections where labels are smaller and integrated into the bars.
→ **Fix**: Reduce the HP/MP/XP label size to match the weight of other section labels, or increase the bar height so the proportions feel balanced. Ensure the percentage value — the actual data — is the most prominent element.

---

## Minor Issues (Consider Fixing)

### 12. Percentage Text Inside Bars is Partially Obscured by Fill Color (HIG 3.2 — Text Contrast)
In the inline meters, percentage labels ("65%", "39%", "84%") are placed inside the bar overlapping the filled and unfilled portions. In the HP bar, "65%" sits against a green background; in the XP bar, "84%" sits against a light blue background. The text-on-fill contrast varies wildly depending on the fill color, and in some cases the text nearly blends into the fill. This is especially problematic for the green-on-green combination in the stacked health bar.
→ **Fix**: Use a consistent, high-contrast text color (white with a subtle drop shadow or dark outline) for all in-bar labels, or move percentage labels outside the bar entirely.

### 13. Inconsistent Unfilled Track Colors Across Bar Types (HIG 1.4 — Consistency)
The HP bar's unfilled track is red, MP's is blue, XP's is yellow, the stacked bars use a subtle dark teal, and the animated progress bar uses blue. There is no consistent "empty/unfilled" visual language. The HIG requires consistency — the same concept (unfilled meter) should have the same visual treatment everywhere.
→ **Fix**: Use a single, neutral unfilled-track color across all bar types — a dark gray or 20% opacity of the bar's fill color — to establish "unfilled" as a consistent visual concept.

### 14. Circular Gauge Track Rings Have Inconsistent Styling (HIG 1.4 — Consistency)
The three circular gauges have different track (unfilled arc) treatments. The 25% gauge shows a thick dark gray track, the 50% gauge has a thinner track with a slightly different color, and the 90% gauge's track is barely visible because the fill nearly completes the circle. The track should be a consistent visual anchor across all gauges.
→ **Fix**: Use the same track color, thickness, and opacity for all circular gauges regardless of fill level, so the visual structure remains stable and comparable.

### 15. Section Dividers Are Inconsistent in Styling (HIG 1.9 — Perceived Stability)
The sections are separated by alternating bands of slightly different dark blue shades, but the distinction is very subtle — some bands appear darker, others barely differ from their neighbors. The title area has a visible decorative line underneath, but no other section uses this treatment. This inconsistency in separators undermines the stable, predictable structure users expect.
→ **Fix**: Use a consistent section divider — either a uniform thin horizontal line, a consistent background shade change, or consistent vertical spacing — applied identically between every section.

### 16. No Contextual Help, Legend, or Usage Guidance (HIG 1.3 — See-and-Point)
The screen showcases five different meter/gauge styles but provides no guidance on when to use each one. A designer or developer viewing this toolkit cannot determine from this screen alone whether "Inline Meters" are preferred over "Stacked Bars" for health displays, or when circular gauges are more appropriate than linear bars. The HIG emphasizes that users should discover meaning by looking, not guessing.
→ **Fix**: Add brief usage guidance beneath each section header (e.g., "Best for quick at-a-glance status" under Inline Meters, "Use when showing combined defensive stats" under Stacked Bars) or provide a companion legend.

### 17. Title Decoration Line Serves No Functional Purpose (HIG 1.10 — Aesthetic Integrity)
The "Meters & Gauges" title has a horizontal decorative line beneath it. While it provides some visual separation, it serves no interactive or informational purpose and is not consistently applied to section headers below. The HIG cautions against decoration that distracts from function.
→ **Fix**: Either remove the decorative line and rely on spacing/size for hierarchy, or apply the same treatment consistently to all section headers for visual rhythm.

### 18. Animated Progress Bar Lacks Animation Affordance in Static View (HIG 1.7 — Feedback)
The "Animated Progress" section shows a bar at 60% that looks identical to a static bar. In a static screenshot or initial render, there is no visual indicator that this bar will animate — no pulsing glow, no striped pattern, no shimmer effect visible in the resting state. Users cannot tell this bar is different from the non-animated bars.
→ **Fix**: Add a visual cue that signals animation potential even at rest — a subtle gradient shimmer, diagonal stripes (barbershop pole pattern), or a pulsing glow on the leading edge of the fill.

---

## Strengths

- **Clear conceptual organization**: The screen groups meters by type (inline, stacked, animated, circular, interactive) in a logical top-to-bottom progression. Each section is visually separated and labeled.
- **Percentage values are always shown**: Every meter and gauge displays a numeric percentage alongside the visual fill, providing an accessible text alternative to the graphical representation.
- **Consistent dark theme**: The overall dark navy palette is cohesive and appropriate for a game UI context. The darker background panels create clear visual regions for each section.
- **HP / MP / XP labeling is intuitive**: Using familiar gaming abbreviations (HP, MP, XP) with large, bold labels immediately communicates the meter type to the target audience.
- **Variety of meter types demonstrated**: The screen successfully showcases five distinct meter patterns (inline, stacked, animated, circular, interactive) in a compact layout, giving toolkit users a comprehensive overview.
- **Circular gauges show a clear value progression**: The left-to-right arrangement (25% → 50% → 90%) intuitively demonstrates how the gauge behaves at different fill levels.

---

## Overall Score: 4/10

The Meters & Gauges screen successfully organizes a diverse set of indicator types into a coherent showcase layout, but it suffers from fundamental HIG violations that undermine both usability and accessibility. The most critical failures are the complete reliance on color as the sole differentiator between meter types and states, dangerously low text contrast on section headers and labels, and an excessive 9+ color palette with no semantic consistency. The HP bar's red unfilled track actively misleads users by signaling danger when none exists. Label placement, track colors, and typographic weight vary arbitrarily across sections, violating the consistency principle repeatedly. The "Interactive Control" section fails to communicate its interactivity, and the "Stacked Bars" section fails to communicate its stacking. Addressing the color-dependency, contrast, and consistency issues would significantly improve the screen's adherence to HIG principles and its practical usability.
