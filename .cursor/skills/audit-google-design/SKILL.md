---
name: audit-google-design
description: Audit any UI/UX design against Google's Material Design 3 guidelines. Provides comprehensive feedback on elevation, color system, typography, spacing, motion, accessibility, and design quality for modern applications.
---

# Material Design 3 Audit

Systematically evaluate UI/UX designs against Google's Material Design 3 guidelines, covering modern cross-platform design with dynamic color, tonal elevation, and responsive layouts.

## When to Use

- `/audit-google-design` — Full audit of a screen, component, or interaction
- After completing major UI work on a modern application
- When validating against current industry-standard design practices
- To identify Material Design compliance issues

## Scope

**Use for:** Any modern UI/UX — web apps, mobile apps, desktop apps, game settings screens, component galleries. Material Design 3 principles apply broadly to any modern interface.

**Not for:** Retro/classic aesthetics (use `/audit-win95-design` or `/audit-sun-design`), Apple-specific platform features (use `/audit-design`).

---

# The Process

## Step 1: Gather Context

Before auditing, you need to see and understand the design:

1. **Request the design** — Ask for screenshot, code, or description
2. **Understand the context** — What is this for? Who uses it? What platforms?
3. **Identify scope** — Are we auditing one screen, one flow, or the whole app?
4. **Determine density** — Desktop (compact), tablet (medium), or mobile (expanded)?

## Step 2: Read the Guidelines

Load the comprehensive checklist:

```
Read: docs/google_hig.md
```

This contains the full audit framework based on Google's Material Design 3 guidelines.

## Step 3: Systematic Audit

Walk through each category in the guidelines document:

1. **Color System**
   - Dynamic color (tonal palettes, semantic roles)
   - 26 semantic color roles (primary, secondary, tertiary, error, surface, etc.)
   - Color accessibility (3:1 non-text, 4.5:1 text contrast)
   - Light and dark theme support

2. **Typography**
   - Type scale (display, headline, title, body, label — each with L/M/S)
   - 15 type styles total
   - Font weight, size, tracking, line height
   - Responsive typography

3. **Elevation and Surface**
   - Tonal elevation (6 levels: 0-5)
   - Surface tint from primary color
   - No drop shadows in M3 (tonal approach)

4. **Layout and Spacing**
   - 8dp grid system
   - Responsive breakpoints (compact <600, medium 600-839, expanded 840+)
   - Content regions (body, navigation, app bar)
   - Consistent padding and margins

5. **Components**
   - Buttons (filled, outlined, text, elevated, tonal)
   - Cards, chips, dialogs, menus
   - Navigation (bar, rail, drawer)
   - Text fields, checkboxes, radio buttons, switches, sliders
   - FAB, bottom sheets, snackbars

6. **Motion and Animation**
   - Duration tokens (short 1-4, medium 1-4, long 1-4, extra-long 1-4)
   - Easing curves (emphasized, standard, legacy)
   - Meaningful transitions

7. **Interaction**
   - Touch targets (48dp minimum)
   - State layers (hover, focus, pressed, dragged)
   - Ripple effects
   - Focus indicators

8. **Accessibility**
   - Color contrast ratios (4.5:1 text, 3:1 non-text)
   - Touch target sizes
   - Screen reader support
   - Keyboard navigation
   - Color not sole indicator

9. **Iconography**
   - Material Symbols (outlined, rounded, sharp)
   - 24dp optical size
   - Consistent weight and grade

For each validation point, note: **PASS**, **FAIL**, or **N/A**

## Step 4: Generate Report

Use this format:

```markdown
## Audit Summary: [Screen/Feature Name]

### Critical Issues (Fix Immediately)
1. **[Category]**: [Specific observation]
   → **Fix**: [Actionable recommendation with M3 guideline reference]

### Major Issues (Fix Before Launch)
1. **[Category]**: [Specific observation]
   → **Fix**: [Actionable recommendation]

### Minor Issues (Nice to Have)
1. **[Category]**: [Specific observation]
   → **Fix**: [Actionable recommendation]

### Accessibility Violations
1. **[Category]**: [Specific observation]
   → **Fix**: [Actionable recommendation]

### Strengths
- [What's done well]

### Overall Material Design Compliance: X/10
```

---

# Key Concepts (Quick Reference)

## 8dp Grid
All spacing, sizing, and positioning should be multiples of 8dp. Small elements may use 4dp.

## Semantic Color Roles

| Role | Use |
|------|-----|
| Primary | Key actions, active states |
| On Primary | Text/icons on primary |
| Primary Container | Less prominent primary elements |
| Secondary | Less prominent actions |
| Tertiary | Contrasting accents |
| Error | Error states |
| Surface | Backgrounds |
| On Surface | Text/icons on surface |
| Outline | Borders, dividers |

## Type Scale

| Style | Size | Weight | Use |
|-------|------|--------|-----|
| Display Large | 57sp | 400 | Hero text |
| Headline Large | 32sp | 400 | Screen titles |
| Title Large | 22sp | 400 | Section headers |
| Body Large | 16sp | 400 | Primary content |
| Body Medium | 14sp | 400 | Secondary content |
| Label Large | 14sp | 500 | Buttons, tabs |
| Label Small | 11sp | 500 | Captions |

## Touch Targets
- Minimum: 48 x 48dp
- Recommended: 56dp for primary actions

---

# Common Violations

## Critical
- **Contrast below 4.5:1** — Text unreadable for low-vision users
- **Touch targets below 48dp** — Buttons too small to tap reliably
- **Color as sole indicator** — Status only communicated through color
- **No focus indicators** — Keyboard users can't see where they are
- **Missing error states** — Form errors not communicated clearly

## Major
- **Off-grid spacing** — Elements not aligned to 8dp grid
- **Wrong elevation model** — Using drop shadows instead of tonal elevation
- **Inconsistent type scale** — Font sizes not from the 15-style type scale
- **Missing state layers** — No hover/focus/pressed visual feedback
- **Wrong button hierarchy** — Multiple filled buttons competing for attention

## Minor
- **Non-standard icons** — Custom icons instead of Material Symbols
- **Verbose labels** — Button text that could be shorter
- **Missing motion** — State changes without transitions
- **Inconsistent corner radius** — Mixed rounding styles
- **Missing surface tint** — Elevated surfaces without primary tint

---

# Commands

- `/audit-google-design` — Start a new Material Design 3 audit
- `/audit-google-design:quick` — 5-minute quick audit, key principles only
- `/audit-google-design:deep` — Full comprehensive audit with all details

---

# Remember

The goal is **actionable feedback**. Every issue identified should have:

1. **What's wrong** — Specific observation with location
2. **Why it matters** — Which M3 guideline it violates
3. **How to fix it** — Concrete recommendation with specific values (colors, dp, sp)

Don't just list problems. Help the user fix them with specific Material Design 3 values.

---

*Based on Material Design 3 guidelines, Google.*
*Full guidelines: `docs/google_hig.md`*
