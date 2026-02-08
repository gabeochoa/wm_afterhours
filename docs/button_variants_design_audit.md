# Button Variants Showcase — Apple HIG Design Audit

*Audited against: Apple Human Interface Guidelines (1987 Desktop Interface)*
*Screen: Button Variants showcase from game UI toolkit*

---

## Audit Summary: Button Variants

### Critical Issues (Fix Immediately)

1. **[Disabled State Not Visually Dimmed — HIG 1.9 Perceived Stability]**: The "Disabled" button in the Filled row uses a saturated olive/yellow text color on a dark background. Per HIG 1.9, unavailable items should be *dimmed*, not rendered in a distinct saturated hue. The current disabled styling makes the button look like an alternative accent variant rather than an inactive control. Users will attempt to click it because it doesn't read as disabled — it reads as styled.
   → **Fix**: Dim disabled buttons by reducing opacity to 30-40% or using a desaturated gray. The disabled state must be visually *lesser* than any enabled state, never equally prominent or more colorful.

2. **[Ghost Buttons Indistinguishable from Plain Text Labels — HIG 1.10 Aesthetic Integrity]**: The Ghost row buttons ("Normal", "Accent", "Secondary", "Disabled") appear as bare text with no border, background, or any visual affordance indicating they are interactive controls. They are visually identical to the row labels ("Filled", "Outline", "Ghost") on the left side. The HIG states that "different things should look different" — interactive buttons and static labels must be visually distinguishable.
   → **Fix**: Give ghost buttons a subtle visual affordance: a faint border on hover, an underline, a slight background tint, or at minimum a different typographic treatment (e.g., different weight or tracking) from static labels. Users must be able to tell at a glance what is clickable and what is not.

3. **[Color as Sole Differentiator Between Variants — HIG 3.1 Color Fundamentals]**: Normal, Accent, Secondary, and Disabled buttons are differentiated almost entirely by color. If the screen were viewed in grayscale or by a user with color vision deficiency, Normal and Accent filled buttons would be nearly indistinguishable, and Outline/Ghost rows would collapse into identical-looking text. The HIG requires that "color coding is redundant with shape, position, or pattern."
   → **Fix**: Introduce secondary visual cues beyond color: vary border weight, add icons, use different border styles (solid vs dashed for secondary), or add a strikethrough/slash overlay for disabled states. The design must remain fully understandable on a monochrome display.

4. **[Outline Buttons Lack Visible Outlines — HIG 3.2 Color Usage / 1.4 Consistency]**: The "Outline" row buttons do not appear to have visible outlines or borders against the dark navy background. The buttons named "Outline" look identical to the Ghost buttons below them — both render as unstyled text. This violates consistency (the name promises an outline) and the HIG color usage rule that "outlines provide clear edge definition."
   → **Fix**: Ensure Outline buttons have a clearly visible 1-2px border in a color that contrasts with the background. The outline should be the defining visual characteristic that distinguishes this variant from Ghost and Filled styles.

5. **[Disabled Filled Button Resembles Accent Variant — HIG 1.4 Consistency]**: The Disabled Filled button uses a warm yellow-olive text color that is semantically similar to the Accent button's olive/gold fill color. A user scanning the row would reasonably interpret the Disabled button as another active variant rather than an inactive control. Disabled and Accent states share a color family, creating dangerous ambiguity between "this does something special" and "this does nothing."
   → **Fix**: Disabled states should use achromatic (gray) colors exclusively, never hues from the active palette. Use a mid-gray text at 40% opacity on the same dark background to create an unmistakable "unavailable" appearance.

### Major Issues (Fix Soon)

1. **[Inconsistent Vertical Column Alignment — HIG 1.10 Aesthetic Integrity]**: The four button columns (Normal, Accent, Secondary, Disabled) do not align vertically across the Filled, Outline, and Ghost rows. Each row's buttons appear to be positioned at slightly different horizontal offsets. The Sizes row breaks the column grid entirely. The HIG calls for clean visual hierarchy and spatial consistency — a grid of controls should snap to consistent vertical axes.
   → **Fix**: Establish a 4-column grid with fixed column centers. Align all Normal buttons to column 1, Accent to column 2, Secondary to column 3, and Disabled to column 4 across all rows. Size variants should use the same grid or a clearly separate section.

2. **["Lg Outline" Rendered as Plain Text — HIG 1.4 Consistency / 2.7 Controls]**: In the Sizes row, "Small", "Medium", and "Large" are all rendered as visible buttons with backgrounds and borders, but "Lg Outline" appears as bare unstyled text. This breaks the pattern — the user expects to see a button demonstrating the large outline style, but instead sees what looks like a label or annotation.
   → **Fix**: Render "Lg Outline" as an actual outlined button at the large size, matching the visual treatment of the other size examples. Every item in the Sizes row should be a live, styled button demonstrating its size variant.

3. **[Row Label Colors Are Inconsistent — HIG 1.4 Consistency]**: The row labels use at least three different color treatments: "Filled" appears in white, "Outline" and "Ghost" appear in a lighter steel-blue/gray, and "Sizes" appears in an olive/gold accent color. These labels serve the identical function (section heading for a row of buttons) yet use three different visual treatments. This violates the HIG consistency principle that same-purpose elements should look the same.
   → **Fix**: Use a single consistent style for all row labels — same color, same font weight, same size. A subtle contrast difference from the background is fine, but all labels should match each other exactly.

4. **[Low Contrast on Secondary Buttons — HIG 3.2 Color Usage]**: The "Secondary" buttons across all three rows (Filled, Outline, Ghost) use muted, desaturated colors that have poor contrast against the dark navy background. The Secondary Filled button's dull olive fill is particularly hard to read, and the Secondary Ghost/Outline text appears as a faint gray that may fall below a 3:1 contrast ratio for large text.
   → **Fix**: Increase the contrast of Secondary buttons. Even though "secondary" implies visual de-emphasis, buttons must still be clearly legible. Aim for at least 4.5:1 contrast for text, 3:1 for large text. Consider a lighter shade or a different hue that maintains readability.

5. **[Theme Card Text Too Small and Low Contrast — HIG 3.2 / 8.1 Accessibility]**: The three theme preview cards ("Neon Dark", "Cosy Kraft", "Sage") contain very small text for the theme name and the "Filled", "Outline", "Ghost" labels within each card. At this size, the text is difficult to read even at full resolution. The "Ghost" labels within the Neon Dark card (light text on dark) and the Sage card (light green text on light green) likely fail minimum contrast ratios.
   → **Fix**: Increase the font size within theme cards, or enlarge the cards themselves to accommodate legible text. Ensure all text within cards meets a minimum 4.5:1 contrast ratio against its card background.

6. **[No Interactive State Differentiation Shown — HIG 1.7 Feedback]**: The showcase displays only one interactive state — the "Normal" Filled button appears to have a white focus/selection border. No hover, pressed, or active states are visible for any other button. The HIG requires "immediate feedback confirming every action." A button component showcase should demonstrate all states (default, hover, pressed, focused, disabled) to ensure they are designed.
   → **Fix**: Add rows or columns showing hover, pressed/active, and focused states for each button variant. At minimum, indicate via annotation that these states exist. The single white-bordered Normal button hints at a focus state but should be explicitly labeled.

### Minor Issues (Consider Fixing)

1. **[Typography Mixing Across the Screen — HIG 1.4 Consistency]**: At least three distinct typographic treatments appear: the title and row labels use what appears to be a serif or decorative typeface, button labels use a different weight or family, and "Total clicks: 0" at the bottom uses a monospace or pixel-style font. This inconsistency across a single screen weakens the visual cohesion.
   → **Fix**: Establish a type system with at most 2-3 levels (heading, body, caption) using one font family. Apply consistently: one style for section titles, one for button labels, one for metadata.

2. **[Title Lacks Sufficient Visual Prominence — HIG 1.10 Aesthetic Integrity]**: The "Button Variants" title at the top is relatively small and uses a similar visual weight to the row labels below it. The title does not establish itself as the primary heading of the screen — it competes with "Filled", "Outline", "Ghost" for attention. Clear visual hierarchy demands the title be noticeably larger or bolder than any subordinate text.
   → **Fix**: Increase the title font size by 30-50%, add more vertical space below it, or use a heavier font weight. The title should be the unambiguous largest text on the screen.

3. **[Inconsistent Row Spacing — HIG 1.10 Aesthetic Integrity]**: The vertical spacing between the Filled/Outline/Ghost/Sizes rows appears inconsistent. The gap between "Ghost" and "Sizes" looks larger than between "Filled" and "Outline." The "Themes" section has a larger gap still, but without any visual separator to justify the change in rhythm.
   → **Fix**: Use a consistent vertical rhythm (e.g., 16px or 24px between rows). If the Themes section is a separate group, add a visible horizontal divider or section header to justify the spacing change.

4. **[Theme Cards Have Inconsistent Sizing — HIG 1.4 Consistency]**: The three theme preview cards ("Neon Dark", "Cosy Kraft", "Sage") are not identically sized. The Sage card appears slightly taller than the others, and horizontal spacing between cards is uneven. Since they serve the same function (theme preview), they should be identical containers.
   → **Fix**: Set all theme cards to the same fixed width and height. Use equal horizontal gaps between them. Center the group within the available space.

5. **["Total clicks: 0" Uses Inconsistent Typography — HIG 1.4 Consistency / 7.1 Plain Language]**: The "Total clicks: 0" counter at the bottom uses a monospace or pixel-style font that differs from every other text element on the screen. While functional as a debug/demo counter, it breaks the visual consistency. Additionally, "Total clicks" is somewhat developer-facing language rather than user-facing.
   → **Fix**: Match the counter font to the rest of the UI. If this is a demo/debug feature, consider visually separating it from the showcase (e.g., placing it outside the bordered panel) or using a style that clearly marks it as metadata.

6. **[Outer Panel Border Is Decorative Noise — HIG 1.10 Aesthetic Integrity]**: The entire showcase is wrapped in a rounded-rectangle border with a visible stroke. This border doesn't delineate functional regions (everything is inside one container) and adds visual weight without aiding comprehension. The HIG warns against "decoration that distracts from function."
   → **Fix**: Either remove the outer border entirely and let the dark background define the panel, or use it to create meaningful sub-regions (e.g., separate containers for each variant type).

7. **[Inconsistent Border Radius Across Buttons — HIG 1.4 Consistency]**: Examining the Filled row, the Normal button has more squared-off corners while the Accent button has fully rounded corners (pill shape). The Small button in the Sizes row has a different corner radius than Medium or Large. Button shape should be consistent within a variant — border radius should scale predictably or remain constant.
   → **Fix**: Define a standard border radius (e.g., 8px, or 50% for pill shapes) and apply it uniformly to all buttons. If radius scales with size, establish a clear formula (e.g., radius = 25% of height).

### Strengths

- **Comprehensive variant coverage**: The showcase displays three variant types (Filled, Outline, Ghost), four semantic states (Normal, Accent, Secondary, Disabled), three sizes (Small, Medium, Large), and three themes — providing thorough documentation of the button system.
- **Theme previews are a smart inclusion**: Showing how buttons render across different themes (Neon Dark, Cosy Kraft, Sage) within the same screen gives developers immediate cross-theme context, reducing the need to switch between views.
- **Clear organizational intent**: The layout is logically organized by category (variant → state → size → theme), making it easy to understand the taxonomy of the button system even if the visual execution has issues.
- **Interactive demo element**: The "Total clicks: 0" counter suggests the buttons are live and interactive, letting users test click behavior directly — a good practice for component showcases.
- **Dark background works well for showcasing**: The dark navy background provides a neutral stage that lets the colored button variants stand out, especially the Filled buttons with their distinct background fills.

### Overall Score: 3.5/10

The Button Variants showcase suffers from several critical issues that undermine its purpose as a component reference. The most damaging problems are: (1) disabled states that look like active accent buttons, creating dangerous semantic ambiguity; (2) Ghost and Outline variants that are visually indistinguishable from each other and from static text labels; and (3) near-total reliance on color alone to differentiate button states, which fails accessibility requirements. The inconsistent column alignment, row label colors, and typography further weaken the visual coherence. While the organizational structure and coverage are strong, the visual execution needs significant work — particularly around contrast, affordance, and consistency — before this showcase can serve as a reliable reference for the button system it documents.
