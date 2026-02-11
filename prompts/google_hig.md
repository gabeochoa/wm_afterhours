# Google Material Design 3 (M3) - Design Audit & Validation Guide

This document provides a comprehensive set of instructions and validation criteria to audit a user interface (UI) or user experience (UX) against Google's Material Design 3 (M3) guidelines. It is intended to be used as a skill for AI assistants like Claude to provide detailed feedback on design implementations.

The guide is structured into key areas of the Material Design system, with each section containing specific, actionable validation points. These points are formulated as questions to facilitate a systematic design audit.

---

## How to Use This Guide

When auditing a design (screen, interaction, component, or full application):

1. Request a screenshot, description, or code of the UI being audited
2. Walk through each category below systematically
3. For each validation point, note: **PASS**, **FAIL**, or **N/A**
4. Provide specific, actionable feedback for every failure
5. Summarize with a priority-ranked list of issues to fix

---

# PART 1: ACCESSIBILITY

Accessibility is a foundational principle of Material Design, ensuring that applications are usable by people with the widest possible range of abilities.

## 1.1. Color and Contrast

| Validation Point | Description |
| :--- | :--- |
| **Contrast Ratios** | Does all text meet the minimum contrast ratios against its background? <br> - **Large text** (18pt+ or 14pt+ bold): At least **3:1** <br> - **Small text**: At least **4.5:1** |
| **Graphics Contrast** | Do all graphical objects and UI components (like icons and form controls) have a contrast ratio of at least **3:1** against their background? |
| **Disabled States** | Are disabled states exempt from contrast requirements? (Note: While not required, they should still be reasonably discernible). |
| **Clustered Elements** | For groups of related elements (e.g., a button group), is there at least a **3:1** contrast ratio between the container and the background? |
| **Color-Only Meaning** | Is color NEVER the only means of conveying information? (Always supplement with icons, text, or patterns) |
| **Grayscale Test** | Does the design remain usable when viewed in grayscale? |

## 1.2. Structure and Hierarchy

| Validation Point | Description |
| :--- | :--- |
| **Visual & Textual Cues** | Does the design use multiple cues (color, shape, text, motion) to convey importance and hierarchy? |
| **Information Hierarchy** | Is key information discernible at a glance? Are important actions placed at the top or bottom of the screen for easy access? |
| **HTML Structure (Web)** | Is the HTML structured logically and sequentially, independent of the visual layout? (Screen readers follow HTML order, not CSS). |
| **ARIA Landmarks (Web)** | Does the web page use ARIA landmark roles (`<nav>`, `<main>`, `<header>`, `<footer>`, etc.) to define regions? |
| **Headings** | Are headings used to create a logical content hierarchy, without skipping levels (e.g., H1 -> H2 -> H3)? Is there only one H1 per page? |
| **Focus Order** | Does the tab/focus order follow the visual reading order (left-to-right, top-to-bottom for LTR languages)? |

## 1.3. Touch and Interaction

| Validation Point | Description |
| :--- | :--- |
| **Touch Target Size** | Are all interactive elements at least **48x48dp** in size for touch targets? (iOS minimum is 44x44pt). |
| **Pointer Target Size** | For mouse or stylus input, are all targets at least **44x44dp**? |
| **Target Spacing** | Is there at least **8dp** of space between touch targets to prevent accidental activation? |
| **Navigation Flow** | Is the task flow clear and efficient, with minimal steps? Are controls easy to locate and clearly labeled? |
| **Feedback** | Does the interface provide clear visual (labels, colors, icons) and touch feedback for all interactions? |
| **Keyboard Navigation** | Can all interactive elements be reached and activated via keyboard? |
| **Focus Indicators** | Are focus indicators clearly visible with at least **3:1** contrast? |

## 1.4. Motion and Vestibular

| Validation Point | Description |
| :--- | :--- |
| **Reduced Motion** | Does the design respect the `prefers-reduced-motion` system setting? |
| **Essential Motion** | When motion is reduced, is essential information still conveyed? |
| **Auto-Playing** | Are there no auto-playing animations that loop indefinitely? |
| **Parallax Effects** | Are parallax effects disabled when reduced motion is requested? |

## 1.5. Screen Reader Support

| Validation Point | Description |
| :--- | :--- |
| **Accessible Labels** | Do all interactive elements have accessible labels (visible or `aria-label`)? |
| **Image Alt Text** | Do all meaningful images have descriptive alt text? Are decorative images marked as such? |
| **Dynamic Content** | Are dynamic content changes announced to screen readers via live regions? |
| **Error Announcements** | Are form errors announced to screen reader users? |

---

# PART 2: COLOR

The Material Design color system is designed to be expressive, accessible, and adaptable. It uses a system of color roles and tonal palettes to create a harmonious and consistent color scheme.

## 2.1. Color System & Roles

| Validation Point | Description |
| :--- | :--- |
| **Key Colors** | Does the color scheme originate from a set of 5 key colors (Primary, Secondary, Tertiary, Neutral, Neutral variant)? |
| **Color Roles** | Are the **26 standard color roles** used semantically and consistently across the UI? |
| **Tonal Palettes** | Is the color system built upon tonal palettes generated from the key colors, ensuring accessible color relationships? |
| **Dark Theme** | Does the design include a dark theme that is automatically generated from the same color roles and tonal palettes? |

### The 26 Color Roles Reference

| Role | Use Case |
|------|----------|
| `primary` | Key interactive elements, FAB, prominent buttons |
| `on-primary` | Text/icons on primary color |
| `primary-container` | Standout containers, selected states |
| `on-primary-container` | Text/icons on primary container |
| `secondary` | Less prominent interactive elements |
| `on-secondary` | Text/icons on secondary color |
| `secondary-container` | Chips, filter buttons when selected |
| `on-secondary-container` | Text/icons on secondary container |
| `tertiary` | Accent elements, visual interest |
| `on-tertiary` | Text/icons on tertiary color |
| `tertiary-container` | Decorative containers |
| `on-tertiary-container` | Text/icons on tertiary container |
| `error` | Error states, destructive actions |
| `on-error` | Text/icons on error color |
| `error-container` | Error message containers |
| `on-error-container` | Text/icons on error container |
| `surface` | Background surfaces |
| `on-surface` | Text/icons on surface |
| `surface-variant` | Alternate surfaces |
| `on-surface-variant` | Secondary text, icons |
| `outline` | Borders, dividers |
| `outline-variant` | Subtle borders |
| `inverse-surface` | Snackbars, tooltips |
| `inverse-on-surface` | Text on inverse surface |
| `inverse-primary` | Primary on inverse surface |
| `scrim` | Modal overlays (32% opacity) |

## 2.2. Dynamic vs. Static Color

| Validation Point | Description |
| :--- | :--- |
| **Dynamic Color** | If the application supports it, does the color scheme adapt dynamically based on user-generated sources (like wallpaper) or in-app content? |
| **Static Color** | If using a static color scheme, is it a deliberate branding choice? Does it follow the baseline Material color scheme or a well-defined custom scheme? |
| **User Override** | Can users override dynamic color with a custom theme if desired? |

## 2.3. Contrast Levels

| Validation Point | Description |
| :--- | :--- |
| **Standard Contrast** | Does the default theme use the standard contrast level, providing a balanced and comfortable viewing experience? |
| **Medium Contrast** | Does the application offer a medium contrast mode (**3:1** minimum)? |
| **High Contrast** | Does the application offer a high contrast mode (**7:1** minimum) for users who need enhanced visibility? |

## 2.4. Color States

| Validation Point | Description |
| :--- | :--- |
| **Enabled** | Is the default appearance clear and inviting? |
| **Disabled** | Are disabled elements clearly dimmed to indicate unavailability while remaining discernible? |
| **Hovered** | Is there a subtle surface color change on hover? |
| **Focused** | Is there a visible focus indicator with **3:1** contrast? |
| **Pressed** | Is the pressed state distinct from hover? |
| **Dragged** | Is there a distinct visual state during drag operations? |
| **Selected** | Is the selected state clearly distinguishable? |

---

# PART 3: TYPOGRAPHY

Material Design's typography system is designed for readability, hierarchy, and expression. It is based on a type scale with a set of predefined styles.

## 3.1. Type Scale & Styles

| Validation Point | Description |
| :--- | :--- |
| **Type Scale Usage** | Does the design consistently use the M3 type scale, which includes **15 baseline and 15 emphasized styles**? |
| **Type Style Roles** | Are the five main type style roles (Display, Headline, Title, Body, Label) used appropriately to create a clear typographic hierarchy? |
| **Emphasized Styles** | Are emphasized styles (higher weight) used intentionally for emphasis, selection, or to denote importance (e.g., in buttons, selected list items)? |
| **Token Usage** | Are typography styles applied using Material Design tokens (e.g., `md.sys.typescale.display-large`)? |

### Type Scale Reference

| Role | Sizes | Use Case |
|------|-------|----------|
| **Display** | Large, Medium, Small | Hero text, large numerals (rare) |
| **Headline** | Large, Medium, Small | Screen titles, section headers |
| **Title** | Large, Medium, Small | Smaller section titles, prominent labels |
| **Body** | Large, Medium, Small | Primary reading content |
| **Label** | Large, Medium, Small | Buttons, tabs, form labels, captions |

## 3.2. Typeface & Readability

| Validation Point | Description |
| :--- | :--- |
| **Typeface Choice** | Does the design use appropriate typefaces for brand expression (Brand typeface for large styles) and readability (Plain typeface for smaller styles)? (Default is Roboto). |
| **Font Sizing** | Are font sizes specified in appropriate units (sp for Android, rem for web)? |
| **Letter Spacing** | Is letter spacing (tracking) applied correctly, especially for different font weights and sizes? |
| **Line Height** | Is the line height appropriate for the font size and line length (typically 1.25-1.5)? |
| **Body Text Minimum** | Is body text at least **14sp** for readability? |
| **Label Minimum** | Is label text at least **12sp**? |
| **Line Length** | Is the maximum line length approximately **80 characters** for body text? |

## 3.3. Text Accessibility

| Validation Point | Description |
| :--- | :--- |
| **Scalable Text** | Can text scale to **200%** without breaking layout or clipping? |
| **Paragraph Spacing** | Is there sufficient spacing between paragraphs? |
| **Text Alignment** | Is body text left-aligned for LTR languages (not centered or justified)? |
| **All Caps** | Is ALL CAPS used sparingly (harder to read for longer text)? |
| **Responsive Font Weights** | Are font weights adjusted appropriately for dark theme (slightly thinner for light-on-dark)? |

---

# PART 4: LAYOUT

Material Design's layout system is responsive and adaptive, ensuring a consistent user experience across different screen sizes and devices.

## 4.1. Responsive Grid & Breakpoints

| Validation Point | Description |
| :--- | :--- |
| **Window Size Classes** | Does the layout adapt to the **five window size classes** (Compact, Medium, Expanded, Large, Extra-large)? |
| **Canonical Layouts** | Is the layout built upon one of the established canonical layouts (e.g., list-detail, supporting pane, feed)? |
| **Margins & Padding** | Are margins and padding used consistently? Are margins wider on larger screens? Is padding applied in increments of **4dp**? |
| **Spacers** | In multi-pane layouts, is there a **24dp** spacer between panes? |
| **8dp Grid** | Is all spacing and sizing based on the **8dp baseline grid** (with 4dp allowed for small elements)? |

### Window Size Classes Reference

| Class | Width | Typical Device | Columns |
|-------|-------|----------------|---------|
| **Compact** | < 600dp | Phone portrait | 4 |
| **Medium** | 600-840dp | Tablet portrait, foldable | 8 |
| **Expanded** | 840-1200dp | Tablet landscape | 12 |
| **Large** | 1200-1600dp | Desktop | 12 |
| **Extra-large** | > 1600dp | Large desktop | 12 |

## 4.2. Structure & Grouping

| Validation Point | Description |
| :--- | :--- |
| **Grouping** | Are related elements grouped together, either explicitly (with containers, dividers) or implicitly (through proximity and white space)? |
| **Panes & Columns** | Is content organized into logical panes and columns? |
| **Folds & Hinges** | For foldable or dual-screen devices, does the layout account for folds and hinges? |
| **Content Reflow** | Does content reflow appropriately when screen size changes (single column → multi-column)? |

## 4.3. Navigation Adaptation

| Validation Point | Description |
| :--- | :--- |
| **Compact** | Is bottom navigation or navigation drawer used? |
| **Medium** | Is navigation rail used? |
| **Expanded+** | Is persistent navigation drawer appropriate? |
| **Consistency** | Is navigation consistent across the application? |

## 4.4. Canonical Layouts Reference

| Layout | Description | Best For |
|--------|-------------|----------|
| **List-Detail** | Master list + detail view side by side | Email, file browsers, settings |
| **Feed** | Scrolling content stream | Social media, news |
| **Supporting Pane** | Primary content + helper panel | Documents, media players |

---

# PART 5: MOTION

Motion in Material Design should be expressive, fluid, and natural. The M3 motion system is **physics-based**, using springs to create intuitive animations.

## 5.1. Motion System & Schemes

| Validation Point | Description |
| :--- | :--- |
| **Physics-Based Motion** | Are animations and transitions based on the M3 **motion physics system**, rather than a simple easing/duration system? |
| **Motion Schemes** | Does the motion primarily use the **Expressive** scheme (with bounce/overshoot for key moments) or the **Standard** scheme (more functional, minimal bounce)? Is the choice consistent with the product's character? |
| **Spring Attributes** | Are animations defined by spring physics (stiffness, damping, initial velocity) rather than static curves? |

### Motion Scheme Comparison

| Aspect | Expressive | Standard |
|--------|------------|----------|
| Character | Playful, engaging | Functional, efficient |
| Overshoot | Yes, bounce on landing | Minimal to none |
| Use Case | Consumer apps, creative tools | Productivity, enterprise |
| Feel | Delightful, personality | Professional, focused |

## 5.2. Spring Tokens & Application

| Validation Point | Description |
| :--- | :--- |
| **Token Usage** | Is motion applied using the standard spring tokens (e.g., `md.sys.motion.spring.fast.spatial`)? |
| **Spatial vs. Effects** | Is the **Spatial** spring type (with overshoot) used for movement (position, rotation, size), and the **Effects** spring type (no overshoot) used for properties like color and opacity? |
| **Speed Tiers** | Are the three speed tiers (Fast, Default, Slow) used appropriately based on the size and context of the animated element? |
| **Customization Level** | Is motion customization applied at the appropriate level (Level 1: default scheme, Level 2: custom scheme, Level 3: per-element override)? |

### Spring Speed Tiers

| Speed | Use Case |
|-------|----------|
| **Fast** | Micro-interactions, small elements, ripples |
| **Default** | Standard transitions, medium elements |
| **Slow** | Large elements, complex orchestrated transitions |

## 5.3. Motion Patterns

| Validation Point | Description |
| :--- | :--- |
| **Container Transform** | Do shared elements smoothly morph between views? |
| **Shared Axis** | Do related items move along a shared axis to show relationship? |
| **Fade Through** | Is fade-through used for transitions without strong relationship? |
| **Shape Morphing** | Does shape change communicate state transitions? |

## 5.4. Motion Quality

| Validation Point | Description |
| :--- | :--- |
| **Snappy Response** | Does motion feel responsive, not sluggish? |
| **Exit ≤ Entry** | Are exit animations equal to or shorter than entry animations? |
| **No Competing Animations** | Are multiple animations choreographed, not competing? |
| **Proportional Duration** | Is duration proportional to travel distance? |

---

# PART 6: ELEVATION

Elevation is the relative distance between surfaces along the z-axis. It helps create a sense of depth and hierarchy in the UI.

## 6.1. Elevation Levels & Depiction

| Validation Point | Description |
| :--- | :--- |
| **Elevation Levels** | Does the design use the **six defined elevation levels** (0, +1, +2, +3, +4, +5) thoughtfully and sparingly? |
| **Resting vs. Interactive** | Are resting states of components on levels **0 to +3**, while interactive states (hover, drag) are on levels **+4 and +5**? |
| **Depiction Method** | Is elevation depicted primarily through **tonal difference** (the default in M3), or are shadows and scrims used where appropriate? |
| **Surface Edges** | Do all elevated surfaces have clear edges that contrast with the surface behind them? |

### Elevation Levels Reference

| Level | dp | Use Case |
|-------|-----|----------|
| 0 | 0dp | Base surface background |
| +1 | 1dp | Cards, elevated surfaces |
| +2 | 3dp | Elevated cards, bottom sheets |
| +3 | 6dp | FAB resting, navigation drawer |
| +4 | 8dp | Bottom navigation, menus, hover states |
| +5 | 12dp | Dialogs, modal bottom sheets, drag states |

## 6.2. Shadows & Scrims

| Validation Point | Description |
| :--- | :--- |
| **Shadow Usage** | Are shadows used sparingly to express the degree of elevation? (Small, sharp shadows for close proximity; large, soft shadows for greater distance). |
| **Shadow Application** | Are shadows used to protect elements on busy backgrounds or to encourage interaction (e.g., lifting on focus)? |
| **Scrim Usage** | Are scrims (at **32% opacity**) used to bring focus to elements like modals and navigation drawers? |
| **Dark Theme Elevation** | In dark theme, is elevation shown via **surface tint** rather than shadows alone? |

## 6.3. Tonal Elevation (M3 Default)

| Validation Point | Description |
| :--- | :--- |
| **Surface Tint** | Does higher elevation result in lighter surface tint? |
| **Primary Tint** | Is the tint derived from the primary color? |
| **Consistent Application** | Is tonal elevation applied consistently across the app? |

---

# PART 7: SHAPE

Shape is a key expressive element in Material Design. The M3 shape system includes a corner radius scale, a library of shapes, and built-in shape morphing.

## 7.1. Corner Radius & Shape Scale

| Validation Point | Description |
| :--- | :--- |
| **Corner Radius Scale** | Are corner radii based on the **10-level shape scale** (from None/0dp to Full)? |
| **Default Shape** | Are rectangular components fully rounded by default, or is a different shape style a deliberate choice? |
| **Symmetry** | Are symmetric shapes used for standalone components, and asymmetric shapes used for grouped items (e.g., menus, split buttons)? |
| **Optical Roundness** | When nesting rounded shapes, are the corner radii adjusted to be proportional to each other (**Outer radius - padding = inner radius**)? |

### Shape Scale Reference

| Level | Radius | Use Case |
|-------|--------|----------|
| None | 0dp | Sharp edges (rare) |
| Extra Small | 4dp | Small components, chips |
| Small | 8dp | Small buttons, small cards |
| Medium | 12dp | Standard cards, dialogs |
| Large | 16dp | Large cards |
| Extra Large | 28dp | Large surfaces, bottom sheets |
| Full | 50%+ | Pills, circular FAB, avatar |

## 7.2. Shape Application & Expression

| Validation Point | Description |
| :--- | :--- |
| **Shape & Text Harmony** | Do the chosen shapes visually harmonize with the typography? |
| **Shape Morphing** | Is shape morphing used to communicate interaction states, actions in progress, or environmental changes? |
| **Tension & Expression** | Does the design use shape to create visual interest and expression, for example by creating tension between rounded and cut corners? |
| **Abstract Shapes** | Are abstract shapes from the Material Shape Library used sparingly and intentionally for decorative flair, without compromising clarity? |
| **Shape for Components** | Are unconventional shapes avoided for text-heavy or information-dense components like cards? |
| **Consistency** | Is the same corner radius used for similar component types throughout the app? |

## 7.3. Shape Morphing

| Validation Point | Description |
| :--- | :--- |
| **State Communication** | Does shape change communicate state changes (selected, pressed)? |
| **Smooth Transitions** | Are shape transitions smooth and spring-based? |
| **Meaningful Changes** | Does shape change convey meaning, not just decoration? |

---

# PART 8: COMPONENTS

Material Design provides a rich library of components that serve as the building blocks of a UI.

## 8.1. Buttons

| Validation Point | Description |
| :--- | :--- |
| **Button Types** | Is the correct button type used for the action? (e.g., `Filled` for the most important action, `Text` for low-emphasis actions). Are `Toggle` buttons used for selection states? |
| **Color Options** | Are the five color options (Elevated, Filled, Tonal, Outlined, Text) used correctly to convey hierarchy and emphasis? |
| **Sizing** | Are the five size recommendations (Extra small, Small, Medium, Large, Extra large) used consistently? |
| **Shape** | Do buttons use the correct shape (Round or Square)? Is the default fully rounded corner radius used, or is a different shape a deliberate design choice? |
| **Anatomy** | Do buttons follow the correct anatomy? Is the icon size **20dp**? Is the padding correct (**16dp** for small buttons)? |
| **Content** | Are button labels concise and written in **sentence case**? If an icon is used, is it a leading icon? |
| **State Changes** | Does the button's shape morph on press or selection, as per the M3 Expressive guidelines? |
| **Touch Target** | Is the touch target at least **48dp**? |
| **Single Primary** | Is there only ONE filled/primary button per major action context? |

### Button Hierarchy Reference

| Type | Emphasis | Use Case |
|------|----------|----------|
| **Filled** | Highest | Primary action, one per screen |
| **Filled Tonal** | Medium-High | Secondary actions needing emphasis |
| **Elevated** | Medium | Need prominence on patterned backgrounds |
| **Outlined** | Medium | Secondary actions |
| **Text** | Low | Tertiary actions, less important |

## 8.2. FAB (Floating Action Button)

| Validation Point | Description |
| :--- | :--- |
| **Single Action** | Is there only ONE FAB per screen (maximum)? |
| **Constructive Action** | Is the FAB action constructive (create, add), NOT destructive? |
| **Position** | Is the FAB in the bottom-right (or bottom-center for primary action)? |
| **Size** | Standard: **56dp**, Large: **96dp**, Small: **40dp**? |
| **Content Visibility** | Does the FAB NOT overlap critical content? |
| **Icon** | Is the FAB icon universally understood (often "+"")? |

## 8.3. Cards

| Validation Point | Description |
| :--- | :--- |
| **Container** | Does the card have a clear container with appropriate elevation? |
| **Content Hierarchy** | Is there clear visual hierarchy within the card? |
| **Primary Action** | Does tapping the card trigger the primary action (when applicable)? |
| **Supplemental Actions** | Are supplemental actions at the bottom of the card? |
| **Corner Radius** | Is the corner radius consistent (**12dp** typical)? |
| **Padding** | Is internal padding consistent (**16dp** typical)? |
| **Appropriate Use** | Is the card not used for single-item lists (use list items instead)? |

## 8.4. Dialogs

| Validation Point | Description |
| :--- | :--- |
| **Title** | Is the title concise and descriptive (not "Warning" or "Alert")? |
| **Body** | Does the body explain what the user needs to know/do? |
| **Actions** | Are there maximum **2 buttons** (confirm/dismiss)? |
| **Button Position** | Is the confirm button on the right, dismiss on left? |
| **Scrim** | Does the scrim block interaction with content behind? |
| **Corner Radius** | Is the corner radius **28dp**? |
| **Size** | Min width: **280dp**, Max width: **560dp**? |
| **Dismissal** | Can the dialog be dismissed by tapping outside or pressing back? |

## 8.5. Navigation Components

### Bottom Navigation

| Validation Point | Description |
| :--- | :--- |
| **Destinations** | Are there **3-5 destinations** maximum? |
| **Labels** | Do all items have text labels (not icon-only)? |
| **Active State** | Is the active destination highlighted (filled icon + color)? |
| **Height** | Is the height **80dp**? |
| **Top-Level** | Does each destination lead to top-level content? |

### Navigation Rail

| Validation Point | Description |
| :--- | :--- |
| **Width** | Is the rail **80dp** wide? |
| **Destinations** | Are there **3-7 destinations**? |
| **FAB** | Is a FAB included at top if needed? |
| **Labels** | Are labels shown (optional but recommended)? |

### Navigation Drawer

| Validation Point | Description |
| :--- | :--- |
| **Width** | Is the drawer **360dp** wide (standard)? |
| **Groups** | Are destinations grouped with headers? |
| **Selection** | Is the selected item clearly indicated? |

## 8.6. Text Fields

| Validation Point | Description |
| :--- | :--- |
| **Type** | Is the correct type used (Filled default, Outlined for visual simplicity)? |
| **Label** | Is there a clear label that floats on focus? |
| **Helper Text** | Is helper text provided when needed? |
| **Error State** | Does the error state include message AND icon? |
| **Character Counter** | Is there a counter for limited fields? |
| **Height** | Is the minimum height **56dp**? |
| **Icons** | Are leading/trailing icons used appropriately? |
| **Keyboard** | Is the correct keyboard type specified (email, number, etc.)? |

### Text Field States

| State | Description |
|-------|-------------|
| **Enabled** | Clearly interactive |
| **Focused** | Label floats, underline/outline emphasized |
| **Error** | Error-colored underline/outline + error icon + message |
| **Disabled** | Clearly dimmed appearance |

## 8.7. Lists

| Validation Point | Description |
| :--- | :--- |
| **Item Height** | One-line: **56dp**, Two-line: **72dp**, Three-line: **88dp**? |
| **Leading Element** | Is avatar/icon used when useful? |
| **Trailing Element** | Is supplemental action appropriately placed? |
| **Dividers** | Are dividers only between groups, NOT every item? |
| **Touch Feedback** | Is there ripple feedback on touch? |
| **Consistency** | Are item heights consistent within a list? |

## 8.8. Chips

| Validation Point | Description |
| :--- | :--- |
| **Type** | Is the correct type used (Assist, Filter, Input, Suggestion)? |
| **Height** | Is the height **32dp**? |
| **States** | Are selected/unselected states clear? |
| **Remove** | Do input chips have an X icon for removal? |
| **Padding** | Is horizontal padding **16dp**? |

### Chip Types Reference

| Type | Purpose |
|------|---------|
| **Assist** | Action suggestions |
| **Filter** | Multi-select filtering |
| **Input** | User-entered tokens |
| **Suggestion** | Dynamic recommendations |

## 8.9. Snackbars

| Validation Point | Description |
| :--- | :--- |
| **Message Length** | Is the message brief (**1-2 lines** max)? |
| **Action** | Is there only a single optional action? |
| **Duration** | Does it display for **4-10 seconds**? |
| **Position** | Is it at bottom center (mobile) or bottom left (desktop)? |
| **Non-Blocking** | Does it NOT block important content? |
| **Single Instance** | Is there only one snackbar at a time? |
| **Dismissible** | Can the user dismiss it? |

## 8.10. Menus

| Validation Point | Description |
| :--- | :--- |
| **Position** | Does the menu appear near the trigger element? |
| **Item Height** | Are items **48dp** height? |
| **Icons** | Are icons on left, shortcuts on right? |
| **Grouping** | Are related items grouped with dividers? |
| **Dismissal** | Does it dismiss on selection or outside click? |
| **Elevation** | Is the elevation Level +4 (**8dp** shadow)? |

## 8.11. Progress Indicators

| Validation Point | Description |
| :--- | :--- |
| **Visibility** | Is an indicator visible during all loading states? |
| **Type** | Is linear used for page loading, circular for buttons/refresh? |
| **Determinate/Indeterminate** | Is determinate used when % is known, indeterminate when unknown? |
| **Color** | Does it use the primary color? |
| **Linear Height** | Is linear indicator **4dp** height? |
| **Circular Size** | Is circular indicator **48dp** default? |

## 8.12. Sliders

| Validation Point | Description |
| :--- | :--- |
| **Track** | Is active/inactive portion visible? |
| **Thumb** | Is the touch area at least **48dp**? |
| **Value Label** | Does a value label appear on interaction (optional)? |
| **Tick Marks** | Do discrete sliders have tick marks? |
| **Range** | Are range sliders used for min/max values? |

## 8.13. Switches, Checkboxes, Radio Buttons

| Validation Point | Description |
| :--- | :--- |
| **Switch Use** | Are switches used for immediate on/off settings (no save needed)? |
| **Switch Size** | Are switches **52 × 32dp**? |
| **Checkbox Use** | Are checkboxes used for selecting items from a list? |
| **Checkbox Size** | Are checkboxes **18dp** visual, **48dp** touch? |
| **Radio Use** | Are radio buttons used for single selection from a group? |
| **Radio Grouping** | Are radio buttons always in groups of **2+** with one always selected? |
| **State Clarity** | Are all states clearly distinguishable? |

## 8.14. Tabs

| Validation Point | Description |
| :--- | :--- |
| **Minimum** | Are there at least **2 tabs**? |
| **Active Indicator** | Is the active tab clearly indicated (underline, color)? |
| **Label Length** | Are labels short (**1-2 words**)? |
| **Scrollable** | Are tabs scrollable if **> 4-5** items? |
| **Height** | Is the height **48dp**? |

---

# PART 9: DARK THEME

## 9.1. Surface Colors

| Validation Point | Description |
| :--- | :--- |
| **Avoid Pure Black** | Are surfaces a slightly lifted dark tone rather than pure black? (Pure black can cause visual issues on OLED and reduces perceived depth). |
| **Tonal Elevation** | Do higher elevations result in lighter surfaces (surface tint)? |
| **Contrast** | Is adequate contrast maintained for all text and UI elements? |
| **Primary Desaturation** | Are primary colors adjusted if too vibrant for dark backgrounds? |

## 9.2. Text & Icons

| Validation Point | Description |
| :--- | :--- |
| **High Emphasis** | Is high-emphasis text clearly readable with strong contrast? |
| **Medium Emphasis** | Is medium-emphasis text visually distinct but less prominent than high-emphasis? |
| **Disabled** | Is disabled text clearly de-emphasized while still discernible? |
| **Icon Opacity** | Do icons follow the same emphasis hierarchy as text? |
| **Avoid Pure White** | Is text slightly softened to reduce eye strain on dark backgrounds? |

## 9.3. Elevation Representation

| Validation Point | Description |
| :--- | :--- |
| **Surface Tint** | Is elevation shown via surface tint, not just shadow? |
| **Tint Application** | Is primary color tint applied to elevated surfaces? |
| **Card Distinction** | Are cards/dialogs visually distinct from background? |
| **Shadow Visibility** | If shadows are used, are they still somewhat visible? |

---

# PART 10: ICONOGRAPHY

## 10.1. Icon Style

| Validation Point | Description |
| :--- | :--- |
| **Icon Set** | Is Material Symbols or a consistent custom set used? |
| **Style Consistency** | Is one style used throughout (Outlined, Rounded, or Sharp)? |
| **Stroke Weight** | Is stroke weight consistent across all icons? |
| **Size** | Standard: **24dp**, smaller: **20dp/18dp**? |
| **Filled Variants** | Are filled variants used for selected/active states? |

## 10.2. Icon Usage

| Validation Point | Description |
| :--- | :--- |
| **Labels** | Are icons paired with text labels when meaning isn't obvious? |
| **Touch Target** | Does the touch target include icon + padding (**≥48dp**)? |
| **State Color** | Does icon color follow component state? |
| **Decorative** | Are decorative icons excluded from focus order? |
| **Meaning** | Is the icon meaning universally understood? |

---

# PART 11: WRITING & VOICE

## 11.1. UI Text

| Validation Point | Description |
| :--- | :--- |
| **Concise** | Is text as brief as possible while remaining clear? |
| **Clear** | Is jargon and technical language avoided? |
| **Useful** | Does text tell users what they need to know? |
| **Action-Oriented** | Are button labels action verbs (Save, Delete, not "OK")? |
| **Sentence Case** | Is sentence case used (not Title Case or ALL CAPS)? |
| **Consistent** | Is terminology consistent throughout? |

## 11.2. Error Messages

| Validation Point | Description |
| :--- | :--- |
| **Explanation** | Does the message explain what happened? |
| **Solution** | Does it suggest how to fix the problem? |
| **Human Language** | Are error codes avoided in favor of plain language? |
| **Non-Blaming** | Does the message avoid blaming the user? |
| **Actionable** | Is there a clear next step? |

## 11.3. Empty States

| Validation Point | Description |
| :--- | :--- |
| **Explanation** | Does it explain what this area is for? |
| **Action** | Is there a CTA to populate the area? |
| **Illustration** | Is illustration used appropriately (optional)? |
| **Tone** | Does the message match brand voice? |
| **Positive** | Is negative/sad imagery avoided? |

---

# PART 12: PLATFORM CONVENTIONS

## 12.1. Android-Specific

| Validation Point | Description |
| :--- | :--- |
| **System Back** | Is the system back gesture supported? |
| **Edge-to-Edge** | Does content extend under status/nav bars with proper insets? |
| **Predictive Back** | Are predictive back animations implemented? |
| **Adaptive Icons** | Are adaptive icons provided? |
| **Navigation Types** | Does it support both gesture and 3-button navigation? |

## 12.2. Web-Specific

| Validation Point | Description |
| :--- | :--- |
| **Keyboard Navigation** | Is full keyboard navigation supported? |
| **Focus Management** | Are focus traps and restoration handled correctly? |
| **Responsive** | Does the layout adapt to all viewport sizes? |
| **Touch & Mouse** | Are both input types supported appropriately? |

## 12.3. Cross-Platform

| Validation Point | Description |
| :--- | :--- |
| **Platform Conventions** | Are platform-specific conventions respected? |
| **Material Consistency** | Are core Material patterns consistent across platforms? |
| **Appropriate Adaptation** | Are platform-specific components used where appropriate? |

---

# AUDIT SUMMARY TEMPLATE

After completing the audit, summarize findings using this format:

```markdown
## Audit Summary: [Screen/Feature Name]

### Critical Issues (Fix Immediately)
1. **[Category]**: [Specific violation]
   → **Fix**: [Actionable recommendation with M3 values]

### Major Issues (Fix Before Launch)
1. **[Category]**: [Specific violation]
   → **Fix**: [Actionable recommendation]

### Minor Issues (Nice to Have)
1. **[Category]**: [Specific violation]
   → **Fix**: [Actionable recommendation]

### Accessibility Violations
1. **[WCAG Reference]**: [Specific violation]
   → **Fix**: [Actionable recommendation]

### Strengths
- [What's done well]

### Overall Material Design Compliance: [X/10]
```

---

# QUICK REFERENCE: The 12 Fundamentals

When time is limited, focus on these:

| # | Principle | Quick Check |
|---|-----------|-------------|
| 1 | **Touch Targets** | Everything ≥ 48dp? |
| 2 | **Contrast** | 4.5:1 text, 3:1 UI components? |
| 3 | **Color Roles** | 26 roles used semantically? |
| 4 | **Type Hierarchy** | 5 roles, clear size/weight progression? |
| 5 | **Elevation** | 6 levels, tonal depiction? |
| 6 | **Spacing** | 8dp grid, consistent gaps? |
| 7 | **Shape** | 10-level scale, morphing for states? |
| 8 | **Motion** | Physics-based springs, not easing curves? |
| 9 | **Button Hierarchy** | One primary action clear? |
| 10 | **States** | All 7 states distinguishable? |
| 11 | **Responsive** | 5 window size classes supported? |
| 12 | **Focus Indicators** | Visible, 3:1 contrast? |

---

# COMMON VIOLATIONS

## Critical
- **Touch targets < 48dp** — Frustrating to tap, accessibility failure
- **Text contrast < 4.5:1** — Can't read the content
- **No focus indicators** — Keyboard users can't navigate
- **Color-only meaning** — Colorblind users excluded
- **No loading states** — Users think it's broken

## Major
- **Not on 8dp grid** — Inconsistent spacing throughout
- **Too many primary buttons** — Nothing stands out
- **Wrong navigation pattern** — Bottom nav on tablet, etc.
- **Missing error states** — Users don't know what went wrong
- **Dark theme surface issues** — Pure black or overly dark surfaces cause visual problems
- **Non-physics motion** — Easing curves instead of springs

## Minor
- **Random corner radii** — Not using 10-level shape scale
- **Icon style mixing** — Outlined + filled without system
- **Excessive motion** — Slow and distracting
- **Verbose labels** — Could be more concise
- **Placeholder-only labels** — Accessibility issue when focused

---

# AUDIT DEPTH LEVELS

## Quick Audit (5 min)
- Check the 12 fundamentals only
- Note 3-5 biggest issues
- Provide immediate recommendations

## Standard Audit (20 min)
- All 12 categories at high level
- Prioritized issue list
- Component spot-checks

## Deep Audit (60+ min)
- Full checklist, every item
- Every component evaluated
- Accessibility deep dive (manual + automated testing)
- Responsive testing at all 5 breakpoints
- Motion review (spring physics verification)
- Both light and dark theme evaluation
- Platform-specific convention check

---

# COMMANDS

- `/audit-google-design` — Start a new Material Design audit
- `/audit-google-design:quick` — 5-minute quick audit, 12 fundamentals only
- `/audit-google-design:deep` — Full comprehensive audit with all details

---

# REMEMBER

The goal is **actionable feedback**. Every issue identified should have:

1. **What's wrong** — Specific observation with location
2. **Why it matters** — Which M3 guideline it violates
3. **How to fix it** — Concrete recommendation with specific M3 values (dp, opacity, spring tokens)

Don't just list problems. Help the user fix them with specific values and patterns from the M3 specification.

---

*Based on Material Design 3 (M3) Guidelines by Google*
*Reference: [m3.material.io](https://m3.material.io/)*
