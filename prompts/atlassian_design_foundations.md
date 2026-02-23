# Atlassian Design System — Design Audit & Validation Guide

This document provides a comprehensive set of instructions and validation criteria to audit a user interface (UI) or user experience (UX) against the Atlassian Design System foundations. It is intended to be used as a skill for AI assistants to provide detailed feedback on design implementations.

The Atlassian Design System is built for productivity apps (Jira, Confluence, Trello, etc.). Its principles emphasize semantic design tokens, consistent spacing rhythm, layered elevation, and accessibility-first color pairing. These patterns are broadly applicable to any interface that must handle information density while remaining clear and scannable.

Reference: https://atlassian.design/foundations

---

## How to Use This Guide

When auditing a design (screen, interaction, component, or full application):

1. Request a screenshot, description, or code of the UI being audited
2. Walk through each category below systematically
3. For each validation point, note: **PASS**, **FAIL**, or **N/A**
4. Provide specific, actionable feedback for every failure
5. Summarize with a priority-ranked list of issues to fix

---

# PART 1: COLOR

Atlassian's color system is built around semantic color tokens, not raw hex values. Colors are organized by role, emphasis, and interaction state. The system supports light and dark themes through token indirection.

## 1.1 Color Roles

Color roles describe the intention behind the color, not its appearance. Each role has a specific semantic meaning.

| Validation Point | Description |
| :--- | :--- |
| **Neutral** | Are default text, secondary UI elements (secondary buttons, navigation) using neutral colors? Neutral has no inherent meaning beyond "default." |
| **Brand** | Is the brand color reserved for primary actions and elements that communicate the Atlassian/app identity? Not overused? |
| **Information** | Are informative UI elements (info icons, progress indicators) using the information role, not brand or accent? |
| **Success** | Is the success color used only for favorable outcomes (success messages, completed states)? |
| **Warning** | Is the warning color used for caution states (preventing mistakes), not for decoration or emphasis? |
| **Danger** | Is the danger color reserved for destructive actions and serious error states? |
| **Discovery** | Is the discovery color used for new features, onboarding, or "what's new" UI — not for primary actions? |
| **Accent** | Are accent colors used without semantic meaning? Could you swap one accent for another without changing the meaning? |
| **Inverse** | On bold/emphasis backgrounds, are inverse tokens used for text and icons to ensure contrast? |
| **Input** | Are form fields using input-specific color tokens for borders, backgrounds, and focus states? |
| **Role Consistency** | Is the same role used for the same meaning throughout the entire interface? (e.g., danger = destructive everywhere) |

## 1.2 Emphasis Levels

Emphasis determines contrast against the default surface. Bolder = more attention.

| Validation Point | Description |
| :--- | :--- |
| **Subtlest → Boldest Range** | Does the UI use a range of emphasis levels to create visual hierarchy? Not everything at the same emphasis? |
| **Bold Backgrounds** | Are bold emphasis backgrounds used sparingly for high-priority information (alerts, selected states)? |
| **Subtle Backgrounds** | Are subtle/subtlest emphasis levels used for low-priority grouping (section backgrounds, hover states)? |
| **Inverse on Bold** | When using bold emphasis backgrounds (especially warning/yellow), are `warning.inverse` tokens used for text to maintain WCAG AA contrast? |
| **Emphasis Consistency** | Is the same emphasis level used for the same type of information across the interface? |

## 1.3 Interaction States

States communicate the status of interactive elements.

| Validation Point | Description |
| :--- | :--- |
| **Hovered** | Do interactive elements have a visible hover state with appropriate color change? |
| **Pressed** | Is there a pressed/active state that's visually distinct from hover? |
| **Selected** | Can users clearly see which element is currently selected? |
| **Focused** | Is there a visible focus indicator for keyboard navigation? (See focus ring requirements below) |
| **Disabled** | Are disabled elements visually distinct but still discernible? Not just invisible? |
| **No Icon Hover/Pressed Tokens** | For icons, is a subtle neutral background used to indicate state changes (since there are no dedicated hover/pressed icon tokens)? |

## 1.4 Accessibility in Color

| Validation Point | Description |
| :--- | :--- |
| **3:1 UI Components** | Do all UI components essential to understanding the experience have at least **3:1** contrast against their background? (WCAG 1.4.11) |
| **3:1 Large Text** | Does text at 24px or larger meet at least **3:1** contrast? |
| **4.5:1 Body Text** | Does text smaller than 24px meet at least **4.5:1** contrast? (WCAG 1.4.3) |
| **Color-Only Meaning** | Is color NEVER the sole means of conveying information? Always supplemented with icons, text, or patterns? |
| **Dark Mode Parity** | If dark mode is supported, do all color tokens maintain the same contrast relationships? (Tokens handle this automatically, but custom colors may not.) |

---

# PART 2: SPACING

Atlassian's spacing system is built around an 8px base unit. Consistent spacing creates visual rhythm, simplifies layouts, and lays the foundation for responsive density.

## 2.1 Base Unit

| Validation Point | Description |
| :--- | :--- |
| **8px Base** | Are all spacing values derived from the 8px base unit? |
| **No Arbitrary Values** | Are there any spacing values that don't appear in the spacing scale (e.g., 5px, 7px, 11px, 18px)? |

## 2.2 Spacing Scale (Space Tokens)

The spacing scale is a limited set of values. Each token is a percentage of the 8px base unit.

| Token | Multiplier | Pixels | Appropriate Use |
| :--- | :--- | :--- | :--- |
| `space.0` | 0× | 0px | No space |
| `space.025` | 0.25× | 2px | Micro adjustments only |
| `space.050` | 0.5× | 4px | Between icon and text, compact component internals |
| `space.075` | 0.75× | 6px | Tight spacing within small components |
| `space.100` | 1× | 8px | Default gap between small icons and text, table cell padding |
| `space.150` | 1.5× | 12px | Gap between repeating elements (button groups) |
| `space.200` | 2× | 16px | Standard component padding, gap between related sections |
| `space.250` | 2.5× | 20px | Comfortable padding for medium containers |
| `space.300` | 3× | 24px | Section dividers, card padding |
| `space.400` | 4× | 32px | Large section spacing |
| `space.500` | 5× | 40px | Major section gaps |
| `space.600` | 6× | 48px | Page-level spacing |
| `space.800` | 8× | 64px | Large page margins |
| `space.1000` | 10× | 80px | Maximum spacing value |

| Validation Point | Description |
| :--- | :--- |
| **Scale Adherence** | Do all padding, margin, and gap values fall on the spacing scale above? |
| **Small Values (0–8px)** | Are small spacing values (space.0 to space.100) used for compact, dense UI: icon-text gaps, badge padding, table cells, input internals? |
| **Medium Values (12–24px)** | Are medium values (space.150 to space.300) used for component spacing: button groups, card padding, section dividers? |
| **Large Values (32–80px)** | Are large values (space.400 to space.1000) reserved for page-level layout: section gaps, margins, hero spacing? |
| **Consistent Internal Spacing** | Within a single component, is the same spacing token used for equivalent gaps? |
| **Negative Space** | If negative spacing is used (overlapping elements, breaking out of container padding), is it intentional and limited to space.negative.025 through space.negative.400? |

## 2.3 Layout Primitives

Atlassian uses three composable layout primitives: Box, Inline, Stack.

| Validation Point | Description |
| :--- | :--- |
| **Box (Container)** | Are containers using managed spacing (padding, margin) from the spacing scale? Not arbitrary values? |
| **Inline (Horizontal)** | Are horizontally arranged elements using consistent gap spacing? |
| **Stack (Vertical)** | Are vertically stacked elements using consistent gap spacing? |
| **Nesting** | Are primitives composed correctly — Box for styling/padding, Inline/Stack for directional layout? Not mixing concerns? |

---

# PART 3: TYPOGRAPHY

Atlassian uses a minor third type scale (1.2×) with sizes rounded to the nearest multiple of 4, built around a 16px base unit. Line heights use ~1.2× for headings and ~1.5× for body, also rounded to 4px.

## 3.1 Typefaces

| Validation Point | Description |
| :--- | :--- |
| **App Font** | Is all in-app UI text using the app typeface (Atlassian Sans, or appropriate system font stack)? |
| **Code Font** | Are all code/monospace instances using the code typeface (Atlassian Mono, or appropriate monospace stack)? |
| **No Brand Font in UI** | Is the brand font (Charlie Sans) NOT used for in-app UI? It is reserved for marketing. |
| **Font Consistency** | Is only one UI typeface used throughout (not mixing multiple sans-serif families)? |

## 3.2 Type Scale

| Validation Point | Description |
| :--- | :--- |
| **Scale Factor** | Does the type scale use a minor third ratio (1.2×) or similar consistent multiplier? |
| **4px Rounding** | Are font sizes rounded to the nearest multiple of 4px for alignment with the spacing grid? |
| **Base Size** | Is the base body text size at least 14px (Atlassian default) or 16px (1rem)? |
| **Scale Range** | Does the type scale cover sufficient range from small (11px) through heading sizes without arbitrary jumps? |

## 3.3 Line Height

| Validation Point | Description |
| :--- | :--- |
| **Heading Line Height** | Do headings use approximately 1.2× the font size for line height? |
| **Body Line Height** | Does body text use approximately 1.5× the font size for line height? |
| **4px Grid Alignment** | Are line heights rounded to the nearest multiple of 4px? |
| **Consistent Within Level** | Does the same heading level always use the same font size and line height? |

## 3.4 Text Styles and Hierarchy

| Validation Point | Description |
| :--- | :--- |
| **Heading Styles** | Are heading, body, and code text styles used consistently? |
| **Visual Hierarchy** | Is there a clear typographic hierarchy — can users scan and understand the information structure at a glance? |
| **No Skipped Levels** | Does the heading hierarchy not skip levels (e.g., H1 → H3 without H2)? |
| **Readability** | Is body text at a comfortable reading size with sufficient line spacing? |

## 3.5 Rem Units

| Validation Point | Description |
| :--- | :--- |
| **Rem-Based Sizing** | Are font sizes specified in rem (or equivalent resolution-relative units) rather than fixed pixels? This ensures users who change browser/system font size settings see appropriate scaling. |
| **Accessibility Scaling** | Does text remain legible and properly laid out when the base font size is increased? |

---

# PART 4: ELEVATION

Elevations are layered surfaces that form the foundation of UI. They use surface colors and shadows to create depth. Atlassian defines four basic levels plus one special case.

## 4.1 Elevation Levels

| Level | Surface | Shadow | Description |
| :--- | :--- | :--- | :--- |
| **Sunken** | Darkened | None | Lowest level. Creates a backdrop/well for other content (e.g., kanban board columns). |
| **Default** | Baseline | None | Flat UI surface (e.g., a page body). Starting point for content. |
| **Raised** | Slightly lifted | Yes | Cards that can be moved (e.g., Jira cards). Higher than default. |
| **Overlay** | Highest | Yes (stronger) | Floating UI that obscures content beneath (e.g., modals, dropdown menus, popovers). |
| **Overflow** | Special | Special | Reserved for scroll shadows and edge indicators. |

| Validation Point | Description |
| :--- | :--- |
| **Correct Level** | Is each UI element at the appropriate elevation? Cards = raised, modals = overlay, page = default, wells = sunken? |
| **Shadow Pairing** | Do raised and overlay surfaces have shadows? Do sunken and default surfaces NOT have shadows? |
| **No Sunken on Raised** | Is sunken elevation only used on the default surface — never on raised or overlay surfaces? |
| **Dark Mode Elevation** | In dark mode, do higher elevations appear lighter (not darker)? Shadows are harder to see in dark mode, so surface color carries more of the depth signal. |
| **Flat Cards** | For flat/non-draggable cards on default surface, is a border used instead of elevation? |
| **Elevation.surface.sunken vs color.background.neutral** | Are these used correctly? `elevation.surface.sunken` is opaque and darkens in both themes. `color.background.neutral` is transparent and adapts to different elevations (lightens in dark mode on raised surfaces). |

---

# PART 5: RADIUS (BORDER RADIUS)

Radius tokens standardize corner roundness for consistency across the UI. Each token tier has specific intended use cases.

## 5.1 Radius Scale

| Token | Value | Suitable For |
| :--- | :--- | :--- |
| `radius.xsmall` | 2px | Small detail elements: badges, checkboxes, avatar labels, keyboard shortcuts |
| `radius.small` | 4px | Supporting elements: labels, lozenges, timestamps, tags, tooltips, compact buttons |
| `radius.medium` | 6px | Interactive elements: buttons, inputs, text areas, selects, navigation items, smart links |
| `radius.large` | 8px | Containment elements: cards, in-page containers, floating UI, dropdown menus |
| `radius.xlarge` | 12px | Large page elements: full-page containers, large containers, modals, kanban columns, tables |
| `radius.xxlarge` | 16px | Video player containers |
| `radius.full` | 999px | Circular elements: avatars, names, user-related UI, emoji reactions, pill shapes, dividers |
| `radius.tile` | 25% (calc) | Tile component system only (icon tiles, object tiles) |

| Validation Point | Description |
| :--- | :--- |
| **Scale Adherence** | Do all border-radius values correspond to a token in the scale above? No arbitrary values (3px, 5px, 10px, 15px)? |
| **Tier Appropriateness** | Is each element using the radius tier appropriate for its type? Badges → xsmall, buttons → medium, cards → large, modals → xlarge? |
| **Consistency Within Type** | Do all instances of the same component type use the same radius token? |
| **Focus Ring Alignment** | Does the focus ring's corner radius visually match the component it surrounds? |
| **No Over-Rounding** | Are containers and interactive elements NOT using `radius.full` (999px) unless they're intentionally circular/pill-shaped? |
| **Tile Exclusivity** | Is `radius.tile` (25%) used exclusively for tile components, not for other UI? |

---

# PART 6: GRID AND LAYOUT

Atlassian uses a responsive column grid with fluid and fixed variants.

## 6.1 Grid Structure

| Validation Point | Description |
| :--- | :--- |
| **Column Grid** | Is a 12-column grid used for aligning content on larger viewports? |
| **Gutters** | Are gutters consistent? 12px for small viewports, 16px for medium and above? |
| **Margins** | Are page margins appropriate? 16px for small viewports, 32px for medium and above? |
| **Content Span** | Does content span 3 or more columns (minimum) up to 12 (maximum)? |

## 6.2 Grid Types

| Validation Point | Description |
| :--- | :--- |
| **Fluid Grid** | For information-dense pages (kanban boards, dashboards), does the grid stretch to fill available space? |
| **Fixed-Narrow** | For content-focused pages (articles, blogs), is a narrow fixed-width grid used to limit line length? |
| **Fixed-Wide** | For data-dense pages (tables, admin), is a wide fixed-width grid used for balance? |
| **Appropriate Type** | Is the grid type appropriate for the content density? |

## 6.3 Breakpoints

| Breakpoint | Viewport | Columns | Gutters | Margins |
| :--- | :--- | :--- | :--- | :--- |
| xxs | 320–479px | 2 | 12px | 16px |
| xs | 480–767px | 6 | 12px | 16px |
| s | 768–1023px | 6 | 12px | 16px |
| m | 1024–1439px | 12 | 16px | 32px |
| l | 1440–1767px | 12 | 16px | 32px |
| xl | 1768+px | 12 | 16px | 32px |

| Validation Point | Description |
| :--- | :--- |
| **Responsive Behavior** | Does the layout adapt appropriately at each breakpoint? |
| **Column Count** | Does the number of columns match the breakpoint (2 for xxs, 6 for xs/s, 12 for m/l/xl)? |
| **No Horizontal Scroll** | Does content fit within the viewport at every breakpoint without horizontal scrolling? |

---

# PART 7: ICONOGRAPHY

Atlassian's icon style uses 1.5px stroke, rounded exterior corners paired with sharp interior corners, and square line caps.

| Validation Point | Description |
| :--- | :--- |
| **Style Consistency** | Do all icons follow the same visual style? No mixing of outline and filled icons, or different stroke weights? |
| **Simplicity** | Are icons simplified to the minimum detail required for comprehension? No excess detail that becomes illegible at small sizes? |
| **Universal Metaphors** | Do icons use widely recognized symbols? No culturally specific or ambiguous metaphors? |
| **Text Labels** | Are icons accompanied by text labels wherever possible? Icons alone are ambiguous for many users. |
| **Existing Icons First** | Before creating a new icon, has the existing icon set been checked for an appropriate match? |
| **Size Consistency** | Are icons used at consistent sizes (16px for inline/small, 24px for standard)? |
| **Icon-Only Hover** | For icons without labels, is a tooltip provided on hover/focus? |

---

# PART 8: DESIGN TOKENS

The Atlassian Design System is built on design tokens — named values that represent repeatable design decisions. Tokens enable theming (light/dark mode) and future customization.

## 8.1 Token Usage

| Validation Point | Description |
| :--- | :--- |
| **Semantic Names** | Are tokens chosen by meaning (what they communicate) rather than by visual appearance (what color they look like)? |
| **No Hard-Coded Values** | Are colors, spacing, radii, and elevations all using tokens rather than raw hex/px values? |
| **Correct Token** | Is each token used for its intended purpose? (e.g., don't use `color.background.danger` just because you want red — use it because something is dangerous) |
| **Theme Compatibility** | If tokens are used correctly, does the design work in both light and dark themes without manual adjustments? |

## 8.2 Token Naming Convention

Tokens follow the pattern: `{foundation}.{property}.{modifier}`

- **Foundation**: color, elevation, space, font, border, radius
- **Property**: background, text, border, icon, surface, shadow
- **Modifier**: role (danger, success), emphasis (subtle, bold), state (hovered, pressed)

| Validation Point | Description |
| :--- | :--- |
| **Naming Clarity** | Can you understand what a token is for from its name alone? |
| **No Appearance-Based Selection** | Were tokens selected based on their description and intended use, not because the current value happens to look right? |

---

# PART 9: ACCESSIBILITY

Accessibility is a core requirement across the entire Atlassian Design System, not an optional add-on.

## 9.1 Color and Contrast

| Validation Point | Description |
| :--- | :--- |
| **4.5:1 Body Text** | Does all body text (<24px) have at least 4.5:1 contrast against its background? |
| **3:1 Large Text** | Does large text (≥24px or ≥19px bold) have at least 3:1 contrast? |
| **3:1 UI Components** | Do non-text UI elements essential for understanding (icons, borders, form controls) have at least 3:1 contrast? |
| **Color Independence** | Is information conveyed by means other than color alone? |
| **Disabled Exemption** | Are disabled elements exempt from contrast requirements, but still visually discernible? |

## 9.2 Focus and Keyboard

| Validation Point | Description |
| :--- | :--- |
| **Focus Visibility** | Is there a visible focus indicator on every interactive element? |
| **Focus Ring Radius** | Does the focus ring's border-radius match the component it wraps? |
| **Focus Contrast** | Does the focus indicator have at least 3:1 contrast against surrounding colors? |
| **Tab Order** | Does the tab order follow the visual reading order? |
| **Keyboard Operability** | Can every interactive element be reached and activated via keyboard? |

## 9.3 Touch and Pointer

| Validation Point | Description |
| :--- | :--- |
| **Touch Target Size** | Are interactive elements at least 44×44px (or 48×48dp on mobile)? |
| **Target Spacing** | Is there at least 8px between adjacent touch targets? |

## 9.4 Text and Content

| Validation Point | Description |
| :--- | :--- |
| **Minimum Font Size** | Is body text at least 14px? |
| **Scalable Text** | Does the layout accommodate text scaling up to 200% without loss of content? |
| **Labels** | Do all form fields have visible, persistent labels (not placeholder-only)? |
| **Error Messages** | Are error messages specific, helpful, and visually associated with the relevant field? |

---

# PART 10: COMPONENT PATTERNS

While the Atlassian Design System has a full component library, these patterns apply broadly.

## 10.1 Buttons

| Validation Point | Description |
| :--- | :--- |
| **Primary Action** | Is there only one primary (bold/filled) button per context? |
| **Button Hierarchy** | Are button variants (primary, secondary, subtle, danger, link) used to communicate relative importance? |
| **Action Labels** | Do buttons have clear, verb-based labels that describe their action? |
| **States** | Do buttons have all states: default, hover, active, focus, disabled, loading? |

## 10.2 Forms

| Validation Point | Description |
| :--- | :--- |
| **Visible Labels** | Does every input have a visible label above or beside it? |
| **Error Indication** | Are errors shown inline next to the relevant field with specific guidance? |
| **Grouping** | Are related form fields grouped logically with clear section labels? |
| **Required Fields** | Are required fields clearly marked? |

## 10.3 Navigation

| Validation Point | Description |
| :--- | :--- |
| **Location Awareness** | Can users always tell where they are in the application? |
| **Consistent Placement** | Is navigation in the same location on every screen? |
| **Depth Limit** | Is navigation depth limited (ideally ≤3 levels)? |

## 10.4 Feedback

| Validation Point | Description |
| :--- | :--- |
| **Success** | Are successful actions confirmed visually? |
| **Error** | Are errors explained with clear, non-blaming language and specific guidance? |
| **Loading** | Are loading states shown for operations that take more than ~300ms? |
| **Empty States** | Do empty states explain what belongs here and how to add content? |

---

# SUMMARY: QUICK REFERENCE CHECKLIST

When time is limited, check these 12 fundamentals:

| # | Area | Key Check |
|---|------|-----------|
| 1 | **Color Roles** | Are semantic color roles (danger, success, warning, info) used consistently? |
| 2 | **Contrast** | 4.5:1 for body text, 3:1 for large text and UI? |
| 3 | **Spacing** | All spacing on the 8px-based scale (0, 2, 4, 6, 8, 12, 16, 20, 24, 32, 40, 48, 64, 80)? |
| 4 | **Typography** | Clear hierarchy with 1.2× scale factor, 4px-rounded sizes? |
| 5 | **Elevation** | Correct level (sunken/default/raised/overlay) with appropriate shadows? |
| 6 | **Radius** | Tokens used by component type (badges=2px, buttons=6px, cards=8px, modals=12px)? |
| 7 | **Focus** | Visible focus ring with matching border-radius? |
| 8 | **Touch Targets** | All interactive elements ≥44×44px? |
| 9 | **Token Usage** | Semantic tokens by meaning, not by appearance? |
| 10 | **Button Hierarchy** | One primary button per context, clear variant usage? |
| 11 | **Dark Mode** | Elevated surfaces lighter, contrast maintained, inverse tokens on bold bg? |
| 12 | **Labels** | All inputs labeled, all icons labeled (text or tooltip)? |

---

*Based on the Atlassian Design System Foundations.*
*Reference: https://atlassian.design/foundations*
