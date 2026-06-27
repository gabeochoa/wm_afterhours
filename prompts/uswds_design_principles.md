# U.S. Web Design System (USWDS) - Design Audit & Validation Guide

This document provides a comprehensive set of instructions and validation criteria to audit a user interface (UI) or user experience (UX) against the U.S. Web Design System (USWDS) design principles and guidelines. It is intended to be used as a skill for AI assistants to provide detailed feedback on design implementations.

The USWDS is the design system for the U.S. federal government. Its principles and patterns are broadly applicable to any interface that must prioritize clarity, accessibility, trust, and inclusivity.

Reference: https://designsystem.digital.gov/design-principles/

---

## How to Use This Guide

When auditing a design (screen, interaction, component, or full application):

1. Request a screenshot, description, or code of the UI being audited
2. Walk through each category below systematically
3. For each validation point, note: **PASS**, **FAIL**, or **N/A**
4. Provide specific, actionable feedback for every failure
5. Summarize with a priority-ranked list of issues to fix

---

# PART 1: DESIGN PRINCIPLES

The USWDS is built on five core design principles. These should guide every design decision.

## 1.1 Start with Real User Needs

> "Real user needs should inform product decisions. Whether your audience includes members of the public or government employees, decision-makers must include real people from the beginning of the design process. Test your team's assumptions and the products and services you build with real people to keep focused on what is most useful and important."

**Question:** Does the design solve actual problems that real users have, based on evidence rather than assumptions?

**Check for:**
- [ ] The interface addresses a clear, observable user need
- [ ] Features exist because users need them, not because they're technically possible
- [ ] The most important tasks are the easiest to find and complete
- [ ] The design doesn't force users to adapt to the system — the system adapts to users
- [ ] Navigation and layout reflect how users think about tasks, not how the system is organized internally
- [ ] The people who will have the most difficulty with the product have been considered

**Red Flags:**
- Features that exist "just in case" with no clear user need
- Organization that mirrors the backend/database structure rather than user mental models
- Important actions buried behind multiple clicks or hidden in menus
- Jargon or internal terminology exposed to users

## 1.2 Earn Trust

> "Trust has to be earned every time. Federal websites and digital services can't assume it. Trust is about understanding and meeting or exceeding expectations, a process that can be established quickly and maintained over continued interactions but is easily damaged. Be reliable, consistent, and honest. Reduce the impact of failure with solid design and engineering. Be a good steward of your audience's data, resources, and time."

**Question:** Does the design communicate reliability, transparency, and competence?

**Check for:**
- [ ] The interface looks professional and well-crafted — no broken layouts, misaligned elements, or sloppy typography
- [ ] Information is presented clearly and honestly — no dark patterns, no deceptive copy
- [ ] The design is consistent — the same patterns are used for the same purposes throughout
- [ ] Error states are handled gracefully with helpful, non-blaming language
- [ ] The system explains what it's doing and why (especially for data collection, permissions, or waiting states)
- [ ] The design is a good steward of the user's time — no unnecessary steps or friction

**Red Flags:**
- Broken or inconsistent visual elements that undermine credibility
- Misleading button labels or deceptive interaction patterns
- Unexplained loading states or silent failures
- Inconsistent styling that looks like multiple unfinished designs stitched together

## 1.3 Embrace Accessibility

> "Accessibility affects everybody. Build it into every decision. Accessibility is not just the law — it's a core design principle. Embrace accessibility requirements as a set of design constraints that help us create a better product for all users."

**Question:** Is the interface usable by people with the widest possible range of abilities and situations?

**Check for:**
- [ ] All text meets WCAG 2.1 AA contrast requirements (4.5:1 for normal text, 3:1 for large text 19px+ bold or 24px+ normal)
- [ ] Interactive elements have sufficient size (minimum 44×44px touch targets)
- [ ] Color is never the sole means of conveying information — per NIH, ~4.5% of the population has color insensitivity
- [ ] The interface is fully operable via keyboard alone
- [ ] Focus indicators are clearly visible
- [ ] Text can be resized up to 200% without loss of content or functionality
- [ ] All images have appropriate alt text
- [ ] Form fields have visible, associated labels (not placeholder-only)
- [ ] Error messages are specific, helpful, and associated with the field that has the error
- [ ] Content is written in plain language and headings, images, and links are accurately labeled
- [ ] Users with screen readers can access all page content

**Red Flags:**
- Low-contrast text (especially light gray on white)
- Tiny click/tap targets
- Color-only state indicators (red = error, green = success with no other cue)
- Missing or invisible focus indicators
- Placeholder text used as the only label
- Custom controls that don't work with keyboard or screen readers

## 1.4 Promote Continuity

> "Minimize disruption, and provide a consistent experience throughout services; over time; and across agencies, platforms, and devices. Consistency is not necessarily conformity. Agencies, sites, and services may have different audiences, missions, and goals — and the way we implement our solutions may differ — but we promote continuity by starting from shared solutions and values."

**Question:** Does the design feel like a coherent, unified experience — consistent with itself and with established conventions?

**Check for:**
- [ ] Visual elements are consistent throughout (same buttons, same spacing, same typography for same purposes)
- [ ] Interaction patterns are predictable — the same action produces the same result everywhere
- [ ] The design uses established, familiar conventions (standard form patterns, expected navigation placement)
- [ ] Transitions between screens or states feel smooth and connected
- [ ] The design builds on what users already know from other interfaces
- [ ] Users can always tell where they are in a process and what to do next
- [ ] The site or service has a consistent experience on any device or browser

**Red Flags:**
- Different button styles for the same type of action
- Navigation that changes location or style between screens
- Unique interaction patterns where a standard one would work
- Abrupt, jarring transitions between states
- Content moved or removed without notice or forwarding

## 1.5 Listen

> "Evaluate and improve your product by listening to your audience and learning from what you hear. Continuous feedback drives continuous improvement. Measure customer experience — how well what you've built is working for your audience — at every stage of your work and as projects grow and mature. Listen to what people say and observe how they interact with your products or services through direct observation or through analytics data."

**Question:** Does the design support continuous evaluation and improvement based on user feedback?

**Check for:**
- [ ] The interface provides clear feedback mechanisms (success messages, error messages, progress indicators)
- [ ] The design is measurable — key tasks have clear start and end points that could be tracked
- [ ] The interface doesn't make assumptions about user preferences without providing options
- [ ] Content is written in plain language that real users can understand
- [ ] There is a mechanism for users to report bugs, issues, or provide feedback
- [ ] Success metrics are tied to positive user outcomes

**Red Flags:**
- No confirmation that actions succeeded
- Complex language where simple language would work
- One-size-fits-all approaches where users have varying needs
- No way to provide feedback or get help

---

# PART 2: ACCESSIBILITY (WCAG 2.1 AA)

Accessibility is not optional in USWDS — it is a core requirement. The USWDS targets WCAG 2.1 Level AA compliance as a minimum.

## 2.1 Color and Contrast

| Validation Point | Description |
| :--- | :--- |
| **Normal Text Contrast** | Does all normal-sized text (under 18pt / 14pt bold) have at least **4.5:1** contrast against its background? |
| **Large Text Contrast** | Does all large text (18pt+ / 14pt+ bold) have at least **3:1** contrast against its background? |
| **UI Component Contrast** | Do all UI components (form fields, buttons, icons) have at least **3:1** contrast against adjacent colors? |
| **Color Independence** | Is color NEVER the sole means of conveying information? (Always pair with text, icons, patterns, or shape) |
| **Grayscale Test** | Does the interface remain fully usable when viewed in grayscale? |
| **Focus Indicator Contrast** | Do focus indicators have at least **3:1** contrast against their background? |

## 2.2 Typography and Readability

| Validation Point | Description |
| :--- | :--- |
| **Minimum Text Size** | Is body text at least **16px** (1rem)? USWDS recommends this as the minimum for comfortable reading. |
| **Line Length** | Are lines of text between **45–75 characters** wide for optimal readability? |
| **Line Height** | Is line height at least **1.5× the font size** for body text? |
| **Text Resize** | Does text remain readable and the layout remain functional when text is resized to **200%**? |
| **Font Legibility** | Are typefaces chosen for legibility? USWDS recommends Source Sans Pro, Merriweather, or Public Sans. |
| **Heading Hierarchy** | Are headings used in a logical, sequential order (H1→H2→H3) without skipping levels? |

## 2.3 Keyboard and Focus

| Validation Point | Description |
| :--- | :--- |
| **Full Keyboard Access** | Can every interactive element be reached and activated using only a keyboard? |
| **Visible Focus** | Is there a clearly visible focus indicator on every focusable element? |
| **Focus Order** | Does the tab order follow the logical reading order of the page (typically left-to-right, top-to-bottom)? |
| **No Keyboard Traps** | Can users tab into AND out of every component without getting stuck? |
| **Skip Navigation** | Is there a "skip to main content" link (or equivalent mechanism) for keyboard users? |
| **No Hover-Only** | Is all functionality available without relying on mouse hover? (Hover can enhance, but never gate) |

## 2.4 Forms and Inputs

| Validation Point | Description |
| :--- | :--- |
| **Visible Labels** | Does every form field have a visible label that remains visible when the field is focused/filled? |
| **No Placeholder-Only** | Are placeholders NOT used as the sole label? (Placeholders disappear on focus) |
| **Error Identification** | Are errors identified specifically? ("Enter your email address" not just "Invalid input") |
| **Error Association** | Are error messages programmatically associated with their form field (aria-describedby or equivalent)? |
| **Required Fields** | Are required fields clearly marked and announced to assistive technology? |
| **Input Purpose** | Do form fields use appropriate input types (email, tel, url) and autocomplete attributes? |

## 2.5 Images and Media

| Validation Point | Description |
| :--- | :--- |
| **Alt Text** | Do all meaningful images have descriptive alt text? |
| **Decorative Images** | Are purely decorative images marked to be ignored by assistive technology (alt="" or role="presentation")? |
| **Text in Images** | Is text NOT embedded in images? (Use real text instead for scalability and accessibility) |
| **Captions/Transcripts** | Do videos have captions? Do audio files have transcripts? |

---

# PART 3: COLOR SYSTEM

The USWDS color system is designed for accessibility, consistency, and clarity.

## 3.1 Color Tokens and Usage

| Validation Point | Description |
| :--- | :--- |
| **Semantic Color Usage** | Are colors used semantically? (e.g., blue for links/actions, red for errors/destructive, green for success, yellow/orange for warnings) |
| **Theme Consistency** | Are the same color tokens used consistently for the same purposes throughout the interface? |
| **Limited Palette** | Is the color palette restrained? USWDS recommends using a small, intentional set of colors rather than many arbitrary ones. |
| **Neutral Backgrounds** | Are backgrounds predominantly neutral (white, light gray) to let content and actions stand out? |
| **State Colors** | Do interactive states (hover, active, focus, disabled) use consistent, distinguishable color shifts? |
| **60/30/10 Proportion** | Does the non-base color usage follow approximately 60% primary, 30% secondary, 10% accent? |

### USWDS Theme Color Families

USWDS theme color tokens are divided into five high-level role-based color families. Each family has seven possible lightness grades (lightest, lighter, light, base, dark, darker, darkest), plus primary and secondary have a `vivid` grade.

| Family | Grades Available | Use |
|--------|-----------------|-----|
| **Base** | lightest, lighter, light, base, dark, darker, darkest, ink | Text, borders, neutral UI elements |
| **Primary** | lighter, light, primary, vivid, dark, darker | Primary actions, links, key interactive elements (~60% of non-base color) |
| **Secondary** | lighter, light, secondary, vivid, dark, darker | Secondary actions, accents (~30% of non-base color) |
| **Accent-warm** | lighter, light, accent-warm, dark, darker | Warm accents, highlights (~5% of non-base color) |
| **Accent-cool** | lighter, light, accent-cool, dark, darker | Cool accents, informational elements (~5% of non-base color) |

### USWDS State Color Tokens

| Token | Use |
|-------|-----|
| **Error** | Error states, destructive actions, required indicators |
| **Warning** | Caution states, important notices |
| **Success** | Success states, confirmation, positive actions |
| **Info** | Informational states, tips, neutral alerts |
| **Disabled** | Disabled states, inactive elements |
| **Emergency** | Emergency alerts, critical system notices |

## 3.2 Color Accessibility — The Grade System and Magic Number

USWDS uses a 100-point grade scale to express how light or dark a color is (0 = pure white, 100 = pure black). Grades are regularized across color families — grade 50 in one family has the same lightness as grade 50 in another.

The **magic number** is the difference in grade between any two colors:

| Magic Number | Contrast Level |
|-------------|---------------|
| **40+** | WCAG 2.0 AA Large Text (3:1) |
| **50+** | WCAG 2.0 AA (4.5:1) or AAA Large Text |
| **70+** | WCAG 2.0 AAA (7:1) |

Colors of grade **50** are accessible against both pure white (grade 0) and pure black (grade 100).

| Validation Point | Description |
| :--- | :--- |
| **Magic Number 50+** | For normal text, is the grade difference between text and background **50 or more**? This ensures WCAG 2.0 AA compliance. |
| **Magic Number 40+** | For large text (19px+ bold or 24px+ normal), is the grade difference **40 or more**? |
| **Consistent Pairings** | Are text/background color pairings consistent? (e.g., always use the same foreground token on the same background token) |
| **Dark-on-Light / Light-on-Dark** | When using colored backgrounds, is text color appropriately inverted for readability? |
| **Color Independence** | Is color NEVER the sole means of conveying information? (~4.5% of the population has color insensitivity, especially red-green) |

---

# PART 4: TYPOGRAPHY

## 4.1 Type Scale and Hierarchy

| Validation Point | Description |
| :--- | :--- |
| **Type Scale** | Does the typography follow a consistent scale? USWDS uses a normalized type scale from `3xs` to `3xl`. |
| **Clear Hierarchy** | Is there a clear visual hierarchy? Can you tell at a glance what's a heading, subheading, body text, and caption? |
| **Consistent Application** | Are the same type styles used for the same purposes throughout? (All H2s look the same, all body text looks the same, etc.) |
| **Responsive Type** | Does type size adjust appropriately for different screen sizes? |

### USWDS Type Scale Reference (Default Theme Settings)

| Token | Default Size | Use |
|-------|-------------|-----|
| `3xs` | 13px | Fine print, legal, captions |
| `2xs` | 14px | Small metadata, footnotes |
| `xs` | 15px | Small UI text, labels |
| `sm` | 16px | Secondary body text |
| `md` | 17px | Default body text |
| `lg` | 22px | Lead text, large body |
| `xl` | 32px | Section headings (H3) |
| `2xl` | 40px | Page headings (H2) |
| `3xl` | 48px | Display headings (H1) |

Note: These are the USWDS defaults. The system also has 21 system-level size tokens (from 10px to 140px) available for projects that need more granularity. Final output is in `rem`, normalized per typeface.

## 4.2 Readability

| Validation Point | Description |
| :--- | :--- |
| **Measure (Line Length)** | Is the measure (line length) between 45–75 characters for body text? |
| **Paragraph Spacing** | Is there adequate spacing between paragraphs (at least 1.5× font size)? |
| **List Formatting** | Are lists formatted as actual lists (with bullets or numbers), not as run-on sentences? |
| **ALL CAPS** | Is ALL CAPS text used sparingly (if at all) and only for very short labels? |
| **Text Alignment** | Is body text left-aligned (not centered or justified) for optimal readability in LTR languages? |

---

# PART 5: SPACING AND LAYOUT

## 5.1 Spacing System

| Validation Point | Description |
| :--- | :--- |
| **Consistent Spacing** | Is spacing consistent throughout the interface? USWDS uses a spacing scale based on **8px** increments (0.5, 1, 1.5, 2, 2.5, 3, 4, 5, 6, 7, 8, 9 units). |
| **Breathing Room** | Do elements have adequate padding and margin? Is the interface free of cramped, claustrophobic areas? |
| **Grouping by Proximity** | Are related items closer together and unrelated items farther apart? (Gestalt proximity principle) |
| **Consistent Gutters** | Are gutters/gaps between columns and sections consistent? |

### USWDS Spacing Scale Reference

USWDS spacing unit tokens are based on multiples of 8px, with additional tokens for small sizes and named tokens for large sizes. Output above 2px is in `rem`.

| Token | Value | Common Use |
|-------|-------|------------|
| `1px` | 1px | Borders, hairlines |
| `2px` | 2px | Thin borders |
| `0.5` | 4px | Tight internal spacing |
| `1` | 8px | Base unit, small gaps |
| `1.5` | 12px | Input padding, small margins |
| `2` | 16px | Standard component padding |
| `2.5` | 20px | Comfortable internal spacing |
| `3` | 24px | Section separation |
| `4` | 32px | Component group separation |
| `5` | 40px | Section breaks |
| `6` | 48px | Major section separation |
| `7` | 56px | Large component spacing |
| `8` | 64px | Page-level spacing |
| `9` | 72px | Large layout spacing |
| `10` | 80px | Major layout spacing |
| `15` | 120px | Extra-large layout spacing |

Named breakpoint tokens: `card` (160px), `card-lg` (240px), `mobile` (320px), `mobile-lg` (480px), `tablet` (640px), `tablet-lg` (880px), `desktop` (1024px), `desktop-lg` (1200px), `widescreen` (1400px).

Negative spacing tokens are also available (e.g., `neg-1` = -8px, `neg-2` = -16px, etc.).

## 5.2 Layout Grid

| Validation Point | Description |
| :--- | :--- |
| **Grid Alignment** | Are elements aligned to a consistent grid? USWDS uses a **12-column** grid with responsive breakpoints. |
| **Responsive Breakpoints** | Does the layout adapt at USWDS standard breakpoints? `card` (160px), `card-lg` (240px), `mobile` (320px), `mobile-lg` (480px), `tablet` (640px), `tablet-lg` (880px), `desktop` (1024px), `desktop-lg` (1200px), `widescreen` (1400px) |
| **Max Width** | Is content constrained to a readable maximum width? USWDS recommends a max content width to prevent overly long lines. |
| **Alignment** | Are elements consistently aligned? Left-aligned for text-heavy content, centered for hero/call-to-action sections. |

---

# PART 6: COMPONENTS

## 6.1 Buttons

| Validation Point | Description |
| :--- | :--- |
| **Clear Hierarchy** | Is there a clear button hierarchy? One primary action per context, with secondary and tertiary options visually subordinate. |
| **Button Types** | Are button types used correctly? **Default** (primary action), **Secondary** (outline), **Accent** (warm/cool), **Outline** (tertiary), **Unstyled** (inline/link-like). |
| **Button Labels** | Are button labels clear, concise action verbs? ("Save changes" not "Submit", "Delete account" not "Yes") |
| **Button Size** | Are buttons large enough to tap/click comfortably? (Minimum 44×44px target area) |
| **Disabled State** | Are disabled buttons visually distinct (dimmed) and do they explain WHY they're disabled (via tooltip or nearby text)? |
| **Destructive Actions** | Are destructive actions visually distinct (e.g., red) and require confirmation? |
| **Icon + Label** | If buttons use icons, do they also include a text label? (Icons alone are often ambiguous) |

## 6.2 Forms

| Validation Point | Description |
| :--- | :--- |
| **Label Position** | Are labels positioned above or to the left of their fields? (Above is preferred for scannability) |
| **Field Width** | Do field widths hint at expected input length? (Short for zip code, long for address) |
| **Helper Text** | Is helper text provided where the expected format isn't obvious? |
| **Validation Timing** | Are errors shown after the user finishes with a field (on blur), not while they're still typing? |
| **Success States** | Are successfully completed fields acknowledged (especially in multi-step flows)? |
| **Logical Grouping** | Are related fields grouped together with clear section headings? |
| **Single Column** | Are forms laid out in a single column? (Multi-column forms slow users down and cause errors) |

## 6.3 Navigation

| Validation Point | Description |
| :--- | :--- |
| **Clear Location** | Can users always tell where they are? (Active nav item highlighted, breadcrumbs, page title) |
| **Consistent Position** | Is navigation in the same position on every screen? |
| **Logical Order** | Are nav items ordered by user priority/frequency, not alphabetically or by internal org structure? |
| **Limited Depth** | Is navigation depth limited? (Avoid more than 2–3 levels of hierarchy) |
| **Current Page Indicator** | Is the current page/section clearly indicated in the navigation? |
| **Responsive Nav** | Does navigation adapt appropriately on small screens? (Hamburger menu, bottom nav, etc.) |

## 6.4 Alerts and Feedback

| Validation Point | Description |
| :--- | :--- |
| **Alert Types** | Are the correct alert types used? **Info** (cyan), **Warning** (gold), **Error** (red), **Success** (green) |
| **Alert Content** | Do alerts explain WHAT happened and WHAT to do next? |
| **Alert Placement** | Are alerts placed near the relevant content (inline) or at the top of the page (for page-level issues)? |
| **Dismissibility** | Can non-critical alerts be dismissed? Are critical alerts persistent? |
| **Progress Indicators** | Do long operations show progress? (Loading spinner, progress bar, skeleton screens) |
| **Confirmation** | Do important or destructive actions require explicit confirmation? |

## 6.5 Cards and Containers

| Validation Point | Description |
| :--- | :--- |
| **Visual Separation** | Are cards/containers clearly distinguished from their background? (Border, shadow, or background color difference) |
| **Consistent Structure** | Do all cards in a set follow the same layout structure? (Same heading position, same metadata placement) |
| **Actionable Area** | If a card is clickable, is the entire card the click target (not just a small link within it)? |
| **Information Density** | Do cards present the right amount of information? (Not too sparse, not overcrowded) |

---

# PART 7: PLAIN LANGUAGE

USWDS strongly emphasizes plain, user-centered language.

## 7.1 Content Guidelines

| Validation Point | Description |
| :--- | :--- |
| **Plain Language** | Is all text written in plain language? (No jargon, no legalese, no bureaucratic language) |
| **Active Voice** | Is active voice used? ("Enter your email" not "Your email should be entered") |
| **Short Sentences** | Are sentences concise? (Target under 20 words per sentence for UI text) |
| **Scannable Content** | Is content structured for scanning? (Headings, bullet points, short paragraphs) |
| **User-Centered** | Does the copy address the user directly? ("You can..." or "Enter your..." not "The user must...") |
| **Action-Oriented** | Do headings and labels describe actions or outcomes, not system processes? ("Choose your plan" not "Plan selection interface") |
| **Error Messages** | Are error messages helpful, specific, and non-blaming? ("Enter an email address" not "Error 422: Invalid input format") |
| **Consistent Terminology** | Is the same term used for the same concept throughout? (Don't switch between "log in", "sign in", and "authenticate") |

---

# PART 8: VISUAL DESIGN

## 8.1 Composition and Hierarchy

| Validation Point | Description |
| :--- | :--- |
| **Visual Hierarchy** | Is there a clear visual hierarchy? Can you tell what's most important at a glance? |
| **F-Pattern / Z-Pattern** | Does the layout accommodate natural reading patterns? (Most important content at top-left, actions at natural end-points) |
| **White Space** | Is white space used intentionally to create breathing room and separate content groups? |
| **Alignment** | Are elements aligned to a consistent grid? Are there no "off by a pixel" misalignments? |
| **Balance** | Does the design feel balanced — not top-heavy, not lopsided? |

## 8.2 Iconography

| Validation Point | Description |
| :--- | :--- |
| **Consistent Style** | Are all icons from the same family/style? (Don't mix filled and outlined, or different stroke weights) |
| **Meaningful Icons** | Do icons clearly communicate their meaning? Are ambiguous icons paired with text labels? |
| **Appropriate Size** | Are icons sized appropriately for their context? (Navigation icons, inline icons, and decorative icons at different sizes) |
| **Icon + Text** | Are icon-only buttons and controls supplemented with text labels or tooltips? |

## 8.3 States and Interactivity

| Validation Point | Description |
| :--- | :--- |
| **Hover States** | Do interactive elements have visible hover states? |
| **Active/Pressed States** | Do buttons and controls show a pressed/active state? |
| **Focus States** | Are focus states clearly visible and consistent? |
| **Disabled States** | Are disabled elements visually distinct (reduced opacity, muted colors) and NOT confusable with active elements? |
| **Selected States** | Are selected items clearly indicated? (Active tab, current page, checked items) |
| **Loading States** | Do elements that load asynchronously show loading indicators? |
| **Empty States** | Are empty states handled with helpful messages and suggested actions? ("No results found. Try adjusting your search.") |

---

# PART 9: RESPONSIVE AND MOBILE

## 9.1 Responsive Behavior

| Validation Point | Description |
| :--- | :--- |
| **Mobile-First** | Does the design work well on small screens first, then enhance for larger screens? |
| **Touch Targets** | Are all interactive elements at least **44×44px** on touch devices? |
| **Thumb Zone** | Are primary actions within easy thumb reach on mobile? (Bottom of screen, center) |
| **No Horizontal Scroll** | Does the layout avoid horizontal scrolling at any supported viewport width? |
| **Readable Without Zoom** | Is text readable without pinch-to-zoom on mobile? (Minimum 16px body text) |
| **Tap vs Click** | Are hover-dependent interactions replaced with tap-friendly alternatives on touch devices? |

---

# PART 10: TRUST AND CREDIBILITY

This section is unique to USWDS — as a government design system, trustworthiness is paramount.

## 10.1 Professional Appearance

| Validation Point | Description |
| :--- | :--- |
| **Polish** | Does the interface look finished and professional? No placeholder text, broken images, or unstyled elements? |
| **Consistency** | Is the visual language consistent throughout? (Same fonts, colors, spacing, component styles) |
| **Alignment** | Are all elements properly aligned? Misalignment erodes trust. |
| **Error Handling** | Are errors handled gracefully? (No raw stack traces, no "undefined", no blank screens) |

## 10.2 Transparency

| Validation Point | Description |
| :--- | :--- |
| **Honest UI** | Are there any dark patterns? (Trick questions, hidden costs, forced continuity, bait and switch) |
| **Clear Pricing/Costs** | If applicable, are all costs and consequences clearly stated upfront? |
| **Data Collection** | If collecting user data, is the purpose clearly explained? |
| **Progress Clarity** | In multi-step flows, do users know how many steps remain? |

---

# Common Violations

These are the issues you'll find most often in a USWDS audit:

## Critical
- **Low Contrast Text** — Body text below 4.5:1 ratio, especially light gray on white
- **No Keyboard Access** — Interactive elements unreachable without a mouse
- **Color-Only Information** — Status conveyed only through color (red/green) with no secondary cue
- **Missing Form Labels** — Placeholders used as labels, or labels missing entirely
- **Tiny Touch Targets** — Interactive elements smaller than 44×44px

## Major
- **Inconsistent Visual Language** — Different button styles, spacing, or typography for equivalent elements
- **Poor Error Messages** — Generic ("Invalid input") or blaming ("You entered an invalid value")
- **No Focus Indicators** — Focus ring removed or invisible
- **Cramped Layout** — Insufficient spacing between elements, no breathing room
- **Jargon / Technical Language** — System-oriented text instead of plain language
- **Missing Loading/Empty States** — No feedback during async operations, blank areas with no guidance

## Minor
- **Weak Visual Hierarchy** — Everything at the same visual prominence, hard to scan
- **Inconsistent Spacing** — Spacing varies without purpose
- **ALL CAPS Overuse** — Too much uppercase text, reduces readability
- **Icon-Only Controls** — Icons without labels or tooltips
- **Overly Long Lines** — Text lines exceeding 75 characters
- **Center-Aligned Body Text** — Body text that should be left-aligned

---

# Audit Depth Levels

## Quick Audit (5 min)
- Check the 5 design principles
- Spot-check contrast on 3–5 text elements
- Verify keyboard focus is visible
- Note 3–5 biggest issues

## Standard Audit (20 min)
- All 10 categories at high level
- Accessibility deep check (contrast, keyboard, labels)
- Component spot-checks
- Plain language review

## Deep Audit (60+ min)
- Full checklist, every item
- Every component evaluated
- Full accessibility audit (WCAG 2.1 AA)
- Responsive behavior at all breakpoints
- Content/language review
- Trust and credibility assessment

---

# Comparison with Other Guidelines

| Aspect | Apple HIG (1987) | Windows 95 | Java Look & Feel | Material Design 3 | **USWDS** |
|--------|-----------------|------------|------------------|-------------------|-----------|
| Philosophy | Desktop metaphor | Data-centered | Cross-platform | Physical surfaces | **User-centered, trust** |
| Primary Focus | Usability | Consistency | Portability | Expression | **Accessibility, clarity** |
| Accessibility | Basic | Basic | Names, keyboard | WCAG 2.1 | **WCAG 2.1 AA required** |
| Color | Conservative | System colors | 8-color model | 26 semantic roles | **Semantic tokens, magic number** |
| Typography | Chicago | MS Sans Serif | Dialog (mapped) | Roboto, 30 styles | **Public Sans / Source Sans Pro** |
| Spacing | Various | DLU-based | 6px multiples | 8dp grid | **8px grid, token scale** |
| Touch/Click | 44pt min | Not specified | Not specified | 48dp minimum | **44px minimum** |
| Language | Brief | Technical OK | Technical OK | Conversational | **Plain language required** |
| Trust | Implicit | Implicit | Implicit | Implicit | **Explicit design principle** |
| Grid | None specified | None | None | Responsive | **12-column responsive** |
| Contrast | Not specified | Not specified | Not specified | 4.5:1 / 3:1 | **4.5:1 / 3:1 (enforced)** |

---

# Remember

The goal is **actionable feedback**. Every issue identified should have:

1. **What's wrong** — Specific observation with location
2. **Why it matters** — Which USWDS principle or guideline it violates
3. **How to fix it** — Concrete recommendation with specific values (contrast ratios, spacing tokens, component patterns)

Don't just list problems. Help the user fix them with specific values from the USWDS specification.

---

*Based on the U.S. Web Design System (USWDS) Design Principles and Guidelines.*
*Reference: https://designsystem.digital.gov/design-principles/*
*Components: https://designsystem.digital.gov/components/overview/*
*Design tokens: https://designsystem.digital.gov/design-tokens/*
