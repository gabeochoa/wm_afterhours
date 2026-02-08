# Design Audit: Buttons Screen

*Audited against the Apple Human Interface Guidelines (1987 Desktop Interface)*
*Date: February 7, 2026*

---

## Audit Summary: Button Gallery

### Critical Issues (Fix Immediately)

1. **[Low Contrast — Normal Button — HIG 3.2, 8.1]**: The "Normal" button uses dark blue text on a dark navy background, distinguished only by a thin white/light border. The text-to-background contrast ratio is extremely low — the label is barely readable against its own fill. HIG 3.2 explicitly requires that "text and thin lines have sufficient contrast to be easily visible," and HIG 8.1 requires high contrast availability for vision accessibility.
   → **Fix**: Give the Normal button either a lighter fill color or a lighter text color so the label is clearly legible without straining. A minimum 4.5:1 contrast ratio between text and background should be the target. Alternatively, use a lighter outline-only style with clearly visible text on the dark page background.

2. **[Color as Sole Differentiator Between States — HIG 3.1, 8.1]**: The States row (Normal, Accent, Secondary, Disabled) relies **entirely** on fill color to communicate the difference between button states. There is no variation in shape, pattern, icon, border weight, or size to distinguish them. A colorblind user or monochrome display would see four nearly identical rectangles. HIG 3.1 states that "color coding is redundant with shape/position/pattern" and the design must "work in black and white first."
   → **Fix**: Add a secondary visual differentiator for each state. For example: Normal gets a visible border only (no fill), Accent gets a filled background, Secondary gets a dashed or lighter border, and Disabled gets a hatched pattern or reduced opacity with a strike-through. Each state must be distinguishable without color.

3. **[Disabled Button Insufficient Contrast — HIG 3.2, 1.9]**: The "Disabled" button uses medium gray text on a medium gray background. While disabled elements should appear visually muted, the label must remain legible so users can understand *what* is disabled. Currently, the text nearly disappears into the fill. HIG 1.9 (Perceived Stability) also requires that unavailable items be "dimmed, not removed" — if the label is unreadable, the element is effectively removed from the user's understanding.
   → **Fix**: Increase the contrast between the disabled button's text and background. Use a lighter gray text on a darker gray fill (or vice versa). The label should be clearly readable even though the interactive affordance is suppressed.

4. **[Flat Visual Hierarchy — HIG 1.10]**: All five rows (title, States, Sizes, Group, Custom) have nearly identical visual weight. Each row uses the same dark container box with similar proportions, making the layout feel like a flat list rather than a structured showcase. The title "Button Gallery" does not meaningfully stand out from the content rows. HIG 1.10 requires "clear visual hierarchy — most important elements stand out" and warns against "flat visual hierarchy — everything same prominence."
   → **Fix**: Differentiate the title row significantly — make it larger, use a different background treatment, or remove its container box so it reads as a page heading. Give each content section distinct visual grouping (e.g., varying container sizes, adding section descriptions, or using progressive disclosure) so the eye is guided through the content.

5. **[No Default Button Indicated — HIG 2.7]**: None of the buttons in any row is visually marked as a default action. HIG 2.7 requires that a "default button [is] doubly outlined" to indicate the primary action that Return/Enter would activate. The States row in particular should demonstrate what a default button looks like as part of its showcase, but no double-outline treatment appears anywhere.
   → **Fix**: Add a clearly identified default button state to the showcase — either as a separate item in the States row or as a visual treatment on the most prominent button. Use the standard double-outline convention (or the modern equivalent: a bolder, heavier border or distinct fill treatment that reads as "primary action").

### Major Issues (Fix Soon)

6. **[Inconsistent Border Radius Across Button Variants — HIG 1.4]**: The "Normal" button appears to have sharp/square corners, while "Accent" and "Secondary" buttons have rounded corners. The Group buttons (One–Four) use rounded corners, but the "Sharp" button in the Custom row explicitly uses square corners. At least three distinct corner radius values are visible. HIG 1.4 (Consistency) requires that the same type of element receives the same visual treatment throughout.
   → **Fix**: Establish a single default border radius for all standard buttons. If "Sharp" is intentionally showcasing a variant, label it explicitly as a variant (e.g., "Sharp Corners") and ensure all other buttons share a consistent radius. The inconsistency between Normal (square) and Accent/Secondary (rounded) in the same row is the most confusing instance.

7. **[Excessive Color Palette — HIG 3.1]**: The screen uses at least seven distinct button fill colors: dark navy (Normal), dark orange (Accent), forest green (Secondary), gray (Disabled), blue (Group and Sizes), coral/salmon (Coral), and teal (Teal). The HIG recommends a "limited palette (4–7 colors max for coding)" and warns against a "rainbow of colors overwhelming the interface." The combination of standard button colors plus the custom colors pushes the palette beyond what users can meaningfully parse.
   → **Fix**: Reduce the core button palette to 3–4 semantic colors (primary, secondary, disabled, destructive). If the Custom row is demonstrating theming capability, visually separate it more clearly from the standard buttons and add explanatory text noting these are theme customizations, not standard states.

8. **[Row Labels Not Vertically Aligned — HIG 1.4, 1.10]**: The row labels ("States:", "Sizes:", "Group:", "Custom:") appear at slightly different vertical positions relative to their adjacent buttons. Some labels appear vertically centered with the button row, others appear baseline-aligned. This inconsistency disrupts the visual grid and makes the layout feel unpolished.
   → **Fix**: Vertically center all row labels relative to their button row, or consistently baseline-align them. Use a fixed left column width for labels so all buttons start at the same horizontal position across rows, creating a clean two-column grid.

9. **[Inconsistent Horizontal Spacing Between Buttons — HIG 1.4]**: The gaps between buttons vary across rows. The States row has relatively generous spacing between Normal, Accent, Secondary, and Disabled. The Group row (One, Two, Three, Four) has tighter, more uniform spacing. The Custom row has an even larger gap between "Coral" and "Teal" than between "Teal" and "Sharp." Consistent internal spacing is a basic requirement of HIG 1.4.
   → **Fix**: Standardize the horizontal gap between buttons across all rows. Use a single spacing token (e.g., 12px or 16px) as the gap between adjacent buttons in every row. If the Custom row needs different spacing for visual reasons, document why.

10. **[Monospace Typography Reduces Readability — HIG 7.1, 1.10]**: The entire screen uses a monospace/pixel-style typeface for all text — title, row labels, and button labels alike. Monospace fonts have lower readability for labels and body text because character widths are unnaturally uniform, making word shapes harder to recognize at a glance. While thematically appropriate for a retro game aesthetic, it makes button labels harder to scan quickly, particularly for longer labels like "Large Button" and "Secondary."
    → **Fix**: If the monospace font is a deliberate game-UI design choice, ensure button labels are kept short (1–2 words max) and increase font size or letter spacing to compensate for reduced readability. Consider using a proportional font for labels while keeping monospace for decorative/title elements, or at minimum ensure the chosen monospace font has clear, well-differentiated letterforms.

11. **["Total clicks: 0" Lacks Clear Association — HIG 1.7, 1.10]**: The click counter at the bottom of the screen ("Total clicks: 0") is placed in its own row with no visual connection to any specific button or group. It's unclear whether it tracks clicks on all buttons, only the last row, or something else. HIG 1.7 (Feedback and Dialog) requires that feedback be "brief, direct" and clearly connected to the action that produced it.
    → **Fix**: Either (a) associate the click counter with a specific button group by placing it visually within or adjacent to that group, (b) add per-button or per-row click counts so the feedback is specific, or (c) add a brief label like "Total button clicks across all groups: 0" to clarify scope.

### Minor Issues (Consider Fixing)

12. **[No Hover/Focus/Pressed States Demonstrated — HIG 1.7, 5.1]**: The gallery showcases button types but does not visually demonstrate interactive states — hover, focus (keyboard navigation), or pressed/active. HIG 1.7 requires "immediate feedback confirms every action" and HIG 5.1 requires "visual feedback shows what's selected." A button gallery that omits these states is incomplete as a showcase.
    → **Fix**: Add a row or a column demonstrating interactive states: hover (lightened fill or underline), focus (visible focus ring), pressed (darkened fill or inset shadow), and selected/active (distinct fill or border). These are essential button behaviors that should be documented alongside static states.

13. **[Inconsistent Button Internal Padding — HIG 1.4]**: The "Small" button has minimal horizontal and vertical padding, making the text feel cramped inside its container. The "Large Button" has generous padding. The Group buttons (One–Four) have moderate, equal padding. The Custom buttons have inconsistent padding — "Coral" appears to have more horizontal padding than "Sharp." Internal padding should scale proportionally and consistently across all button sizes.
    → **Fix**: Define padding tokens for each size variant (small, medium, large) and apply them consistently. For same-size buttons within a row (Group, Custom), ensure identical internal padding so the buttons feel like members of the same family.

14. **[Section Containers Lack Visual Differentiation — HIG 1.10]**: Each row is wrapped in an identically styled dark container box with the same background color, border radius, and border treatment. While consistency in container styling is generally good, the identical treatment for five fundamentally different sections (states, sizes, groups, custom, counter) makes the layout monotonous and prevents users from quickly locating a specific section.
    → **Fix**: Consider subtle differentiation — varying the container background lightness slightly per section, adding section icons, or using different border treatments for different categories. Alternatively, add brief descriptive text under each section header to give each row distinct informational content.

15. **[Button Labels Are Nouns, Not Verbs — HIG 2.7]**: Button labels like "Normal," "Accent," "Secondary," "One," "Two," "Coral," "Teal," and "Sharp" are all nouns or adjectives describing the button's style, not actions. HIG 2.7 states that button "labels describe the action that will occur." While this is a showcase (not a functional dialog), using verb-style labels like "Save," "Cancel," "Submit," "Delete" would better demonstrate real-world button usage and help developers understand how buttons should be labeled in practice.
    → **Fix**: Consider using realistic action labels for at least one row (e.g., "Save," "Cancel," "Delete," "Export") to demonstrate proper button labeling conventions. Keep the style-descriptive labels for the variants that specifically showcase styling.

16. **[Title Row Visually Identical to Content Rows — HIG 1.10]**: The "Button Gallery" title is rendered inside a container box that uses the same styling as the content rows below it. This makes the title feel like just another row of content rather than a page heading. The only differentiator is that the title text is slightly larger — but the container treatment neutralizes this distinction.
    → **Fix**: Remove the container box from the title or give it a distinctly different treatment (wider, no background, larger font, different alignment). The title should read as a heading, not as a content card.

17. **[No Grouping Indicator for Radio-Style Buttons — HIG 2.7]**: The Group row (One, Two, Three, Four) appears to represent a mutually exclusive selection group (like radio buttons), but there is no visual indicator showing which option is currently selected. All four buttons appear identical. HIG 2.7 requires that in a group of mutually exclusive choices, "exactly one [is] always selected" with visual feedback.
    → **Fix**: Show one button in the Group row as visually selected (filled, highlighted, or with a distinct border) and the others as unselected. This demonstrates the button group's selection behavior and follows the radio button convention.

18. **[Outer Container Adds Unnecessary Nesting Depth — HIG 1.10]**: The screen has three levels of visual nesting: the outer dark page background, a slightly lighter outer container, and then the individual row containers within it. This creates a "box in a box in a box" effect that adds visual complexity without communicating meaningful structure. HIG 1.10 warns against "visual clutter."
    → **Fix**: Consider reducing nesting to two levels — either remove the outer container (let rows float directly on the page background) or remove the individual row containers (let content sit directly inside the outer container with dividers instead of boxes).

### Strengths

- **Clear Section Organization**: The gallery is logically organized into meaningful categories (States, Sizes, Group, Custom), making it easy to understand what each row demonstrates.
- **Consistent Dark Theme**: The overall dark navy color scheme is cohesive and gives the gallery a unified visual identity appropriate for a game UI toolkit.
- **Row Labels Are Present and Descriptive**: Each row has a clear label ("States:", "Sizes:", "Group:", "Custom:") that immediately communicates the purpose of that row.
- **Size Variants Are Clearly Differentiated**: The Sizes row effectively demonstrates three distinct button sizes — Small, Medium, and Large — with proportional scaling that is easy to understand.
- **Interactive Feedback Mechanism**: The "Total clicks: 0" counter demonstrates that the buttons are interactive and the gallery tracks user engagement, supporting HIG 1.7's requirement for feedback.

### Overall Score: 3/10

The Button Gallery has sound organizational bones — logical grouping, clear labels, and a cohesive theme — but suffers from fundamental contrast and accessibility failures that would make several buttons unusable for significant portions of users. The Normal and Disabled buttons violate basic legibility requirements. Color is used as the sole differentiator between button states, ignoring the HIG's core principle that designs must work without color. The visual hierarchy is flat, spacing is inconsistent, and the typography choice prioritizes aesthetic theme over readability. For a component showcase — which should be a reference implementation of best practices — these issues are particularly concerning because they propagate bad patterns to developers who use the toolkit.
