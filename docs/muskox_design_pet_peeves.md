# Muskox Design Pet Peeves

A comprehensive checklist of subtle, insidious design craft defects — the last 1% of polish that makes 99% of the difference between amateur and professional.

> As designers, small (and large) imperfections drive us nuts. Having sensitive eyes to identify and fix craft defects or jank is actually a superpower.

---

## 1. Visual Design

### 1.1 Incorrect Inset Corner Radii
**Severity: Critical**

Nested elements with corner radii that don't follow the inset formula: **Inner Radius = Outer Radius - Padding**. This topped our survey. Watch for:
- Badges inset on tiles
- Buttons on cards
- Banners inside windows or containers
- Any nested rounded element where the inner radius doesn't decrease proportionally

**Rule:** Inner Radius = Outer Radius - Padding. If padding exceeds outer radius, inner radius = 0.

### 1.2 Slight Misalignments
**Severity: Major**

Padding mismatches, especially built-in padding on icons causing optical misalignment. People won't complain directly, but unconsciously register the product as less premium. Check:
- Icon padding causing off-center appearance
- Text baselines not aligned across columns
- Elements that are mathematically centered but optically off
- Inconsistent margins between siblings

### 1.3 Container Overuse
**Severity: Major**

Containers in containers in containers. Instead of nesting boxes, use gestalt principles:
- **Proximity** — group related items by spacing alone
- **Similarity** — use consistent styling to show relatedness
- **Common region** — only when spacing/similarity aren't enough
- **Rule:** If you can remove a container and the grouping still reads, remove it

### 1.4 Rule Line Overuse
**Severity: Minor**

Sectioning a composition with unnecessary rule lines instead of using hierarchy and spacing. Rule lines:
- Pack visual tension
- Add one more element to process
- Are often replaceable by spacing and typography hierarchy
- **Rule:** Only use a rule line if removing it makes the grouping ambiguous

### 1.5 Lack of Clear Hierarchy
**Severity: Critical**

No visual balance or clear reading order to guide users' eyes. Check:
- Is there a single dominant element on the page?
- Can you identify primary, secondary, and tertiary content?
- Does the eye flow naturally through the composition?
- Are competing elements fighting for attention?

### 1.6 Bitmap and Resolution Issues
**Severity: Major**

Bitmapping, texture resolution, noisy images, pixel density or sharpness problems. Check:
- Icons or graphics that are blurry or stretched in production
- Assets that look fine in design tools but degrade in implementation
- Raster graphics used where vectors would stay crisp
- Images not provided at appropriate density (1x, 2x, 3x)

### 1.7 Inconsistent Visual Language
**Severity: Critical**

Colors, iconography, or typography that vary across screens, flows, or functional areas:
- Different icon styles (filled vs. outline) within the same product
- Shades of a color that are slightly different screen-to-screen
- Metadata treated differently across screens
- Profile pictures or avatars rendered inconsistently
- **Rule:** Every seam slowly and persistently erodes trust

### 1.8 Mixed Design Systems
**Severity: Critical**

Legacy components or mixed design system parts appearing in the same product. This damages overall cohesion and signals neglect.

### 1.9 Unclear Information Grouping
**Severity: Major**

When an element appears tied to, related to, or a modifier of the wrong element. Spatial proximity implies relationship — verify every element is closest to what it actually modifies.

---

## 2. Typography

### 2.1 Too Many Type Sizes or Styles
**Severity: Major**

Excessive type variation on a single screen. This happens:
- By accident as new designers join
- Slowly over time as features accrete
- **Rule:** Audit your type ramp. If a screen uses more than 4-5 distinct styles, question each one.

### 2.2 Poorly Set Typography
**Severity: Minor**

Watch for:
- **Widows** — single word on the last line of a paragraph
- **Orphans** — single line of a paragraph at the top/bottom of a column
- **Overly raggy** right edges on left-aligned text
- **Random alignment** — text that's centered when it should be left-aligned, or vice versa
- **Unintentional mixed alignment** across related elements

### 2.3 Wrong Typeface
**Severity: Minor**

System or fallback typefaces appearing unexpectedly (e.g., Roboto showing up in a non-Material context). Verify fonts are loading correctly and fallbacks are intentional.

### 2.4 Too Much Text
**Severity: Major**

Unnecessary subheads, explanatory paragraphs that users skip, verbose descriptions that could be trimmed. Question every word:
- Is that extra subhead needed?
- Do users actually read that paragraph?
- Can the same information be conveyed with fewer words?

### 2.5 Literal vs. Optical Centering
**Severity: Minor**

Elements that are mathematically centered but visually appear off-center. Common with:
- Numbers (which tend to appear heavier on one side)
- Icons with asymmetric visual weight
- Text with descenders appearing lower than it is
- **Rule:** Trust your eyes, not the alignment tool

---

## 3. Usability, Flow, and Content

### 3.1 Content Bandaids
**Severity: Critical**

Dialogs, banners, or toasts used to explain overly complex steps:
- Telling the user what to do instead of structuring cues to guide them
- Explaining why something doesn't work as expected (instead of fixing it)
- Adding instructional overlays on first launch instead of making the UI self-evident
- **Rule:** If you need a tooltip to explain a button, the button is wrong

### 3.2 Too Many CTAs
**Severity: Major**

One call-to-action is great. Two is OK. Three or more demands careful justification:
- Primary action should be unambiguous
- Secondary action should be clearly subordinate
- If you have 3+ CTAs, the user doesn't know what to do
- **Rule:** One screen, one job

### 3.3 Losing User Input
**Severity: Critical**

Users having to input information multiple times because the product doesn't remember, transfer, or auto-populate it. Respect the user's time — data entered once should persist.

### 3.4 Bad Error Messages
**Severity: Major**

Error messages or states that:
- Don't explain what went wrong in plain language
- Don't provide actionable steps to resolve the issue
- Use technical jargon, error codes, or blame the user
- Show a generic "Something went wrong" with no next steps

### 3.5 Cluttered Interface / Busy IA
**Severity: Major**

Information architecture problems:
- Elements covering all four corners and edges of the screen
- Menus on menus on sub-menus
- Left, right, top, and bottom navigation simultaneously
- No clear spatial model or hierarchy

### 3.6 Inconsistent Capitalization
**Severity: Minor**

Title Case mixed with sentence case within the same product. Pick one convention and stick to it. Sentence case is strongly preferred for most UI text.

### 3.7 Internal Jargon
**Severity: Major**

Internal project names, incorrect brand terminology, or technical terms that mean nothing to users. Audit all user-facing text for:
- Code names or internal project names
- Deprecated brand terms
- Technical terminology without plain-language alternatives

### 3.8 Modal and Popup Overuse
**Severity: Major**

Modals or popups that:
- Obstruct user flow unnecessarily
- Stack awkwardly on top of each other
- Could be inline content instead
- Interrupt without justification
- **Rule:** Every modal is an interruption. Justify each one.

### 3.9 Even-Numbered Short Lists
**Severity: Minor**

Even-numbered lists of fewer than six items, with two-item lists being the worst offender. Odd numbers (3, 5) create better visual rhythm and avoid the "binary choice" trap.

### 3.10 Non-Intuitive Core Functionality
**Severity: Critical**

Tablestakes functionality that requires too much friction:
- Core tasks (adding friends, starting a session, managing inventory) should be near-zero friction
- If the most common action takes more than 2 taps/clicks, investigate
- **Rule:** The most important things should be the easiest things

### 3.11 Ambiguous Icons
**Severity: Major**

Icons that are unclear, ambiguous, or cause user confusion. If you have to label every icon to make it understandable, reconsider the icon or add persistent labels.

### 3.12 Missing System Feedback
**Severity: Critical**

Violation of a classic usability heuristic. Always show:
- Current system state
- Whether an action was successful
- What's happening during a wait
- **Rule:** Silence is the worst feedback

### 3.13 No Undo / Broken Back
**Severity: Critical**

Missing or janky undo/back functionality. Users must feel they can try things safely:
- Every destructive action should be reversible or confirmed
- Back button should always work predictably
- Navigation state should be restorable

### 3.14 Missing or Inconsistent Interactive States
**Severity: Major**

Buttons, links, form fields, or interactive elements without visible:
- Hover states
- Focus states (accessibility failure)
- Active/pressed states
- Disabled states
- Loading states
- **Rule:** Every interactive element needs at minimum: default, hover, pressed, focused, disabled

### 3.15 Visible Scrollbars on Mobile
**Severity: Minor**

Scrollbars that are permanently visible on touch interfaces where they should auto-hide. This looks unpolished and wastes screen real estate.

---

## 4. Motion, Transitions, and Performance

### 4.1 Shimmer Mismatch
**Severity: Major**

Loading placeholder silhouettes (shimmers) that don't match the layout of the real content once it loads. This creates a jarring pop when content replaces the placeholder. Also watch for:
- Assets rendering in random order
- Layout shifts when content loads

### 4.2 Poor Scroll Performance
**Severity: Critical**

Scrolling must:
- Feel responsive and immediate
- Align to touch/mouse gestures
- Have built-in dampening and momentum
- Never stutter, hitch, or lag
- **Rule:** If scrolling feels off, raise a ruckus

### 4.3 Hesitant or Nonlinear Animations
**Severity: Major**

Animations that:
- Hesitate at the beginning or end
- Move in a non-natural way
- Break the illusion of physical motion
- Have inconsistent timing across similar transitions

### 4.4 Over-Reliance on Loading Screens
**Severity: Major**

Using loading screens instead of:
- Graceful degradation (show what you have, load the rest)
- Progressive enhancement (skeleton → partial → full)
- Optimistic UI (assume success, roll back on failure)
- **Rule:** The best loading screen is no loading screen

### 4.5 General Lag and Sluggishness
**Severity: Critical**

Report and fix:
- Dropped frames
- Loss of tracking (head-tracking, cursor tracking)
- Delayed state changes
- UI elements popping in or out
- Visual holes or rendering hitches
- **Rule:** These are easy to overlook because they're hard to attribute to specific functions, but they are critical to log and fix

---

## Severity Guide

| Severity | Definition | Action |
|----------|-----------|--------|
| **Critical** | Directly harms usability, trust, or perceived quality | Fix immediately |
| **Major** | Noticeable quality gap; users register it consciously or unconsciously | Fix before shipping |
| **Minor** | Polish issue; separates good from great | Fix when possible |

---

## Quick Tally Checklist

Use this for rapid assessment. Score each area 0-2 (0 = failing, 1 = acceptable, 2 = excellent):

| # | Category | Score |
|---|----------|-------|
| 1 | Corner radii (inset formula followed) | /2 |
| 2 | Alignment (optical + mathematical) | /2 |
| 3 | Container nesting (minimal, uses gestalt) | /2 |
| 4 | Visual hierarchy (clear reading order) | /2 |
| 5 | Asset quality (crisp, correct density) | /2 |
| 6 | Visual consistency (icons, colors, type) | /2 |
| 7 | Typography (ramp discipline, no jank) | /2 |
| 8 | Copy quality (concise, no jargon) | /2 |
| 9 | Error handling (clear, actionable) | /2 |
| 10 | Interactive states (all states present) | /2 |
| 11 | Feedback (system state always visible) | /2 |
| 12 | Undo / reversibility | /2 |
| 13 | Loading / perceived performance | /2 |
| 14 | Motion quality (smooth, natural) | /2 |
| 15 | Scroll performance | /2 |
| | **Total** | **/30** |

| Score | Rating |
|-------|--------|
| 27-30 | Excellent — ship it |
| 22-26 | Good — minor polish needed |
| 16-21 | Acceptable — several issues to address |
| 10-15 | Needs work — significant craft debt |
| 0-9 | Critical — major quality problems |

---

*Source: "Design pet peeves we all love to hate" — a survey of the most subtle or insidious design issues that separate amateur from professional craft.*
