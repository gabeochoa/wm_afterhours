---
name: audit-design
description: Audit any UI/UX design against Apple's Human Interface Guidelines (1987 Classic Macintosh). Provides comprehensive feedback on visual hierarchy, direct manipulation, consistency, feedback, forgiveness, and design quality for desktop GUI applications.
---

# Apple Human Interface Guidelines Design Audit

Systematically evaluate UI/UX designs against Apple's *Human Interface Guidelines: The Apple Desktop Interface* (1987), covering the foundational principles of graphical user interface design.

## When to Use

- `/audit-design` — Full audit of a screen, component, or interaction
- After completing major UI work
- When validating design quality against classic GUI principles
- To identify usability issues before shipping

## Scope

**Use for:** Any UI/UX — desktop applications, game settings screens, component galleries, menus, dialog boxes, forms. The Apple HIG principles are universal enough to apply broadly.

**Not for:** Material Design-specific (use `/audit-google-design`), Java L&F-specific (use `/audit-sun-design`), Win95-specific (use `/audit-win95-design`).

---

# The Process

## Step 1: Gather Context

Before auditing, you need to see and understand the design:

1. **Request the design** — Ask for screenshot, code, or description
2. **Understand the context** — What is this for? Who uses it?
3. **Identify scope** — Are we auditing one screen, one flow, or the whole app?

## Step 2: Read the Guidelines

Load the comprehensive checklist:

```
Read: docs/apple_hig.md
```

This contains the full audit framework based on Apple's 1987 Human Interface Guidelines.

## Step 3: Systematic Audit

Walk through each category in the guidelines document:

1. **Metaphors and Mental Models** — Does the interface use familiar real-world metaphors?
2. **Direct Manipulation** — Can users interact directly with on-screen objects?
3. **See-and-Point** — Is every function accessible via visible, clickable elements?
4. **Consistency** — Same action = same result throughout?
5. **WYSIWYG** — Does the screen match the final output?
6. **User Control** — Is the user always in charge?
7. **Feedback and Dialog** — Immediate response to every action?
8. **Forgiveness** — Can users undo and recover from mistakes?
9. **Perceived Stability** — Does the interface feel solid and predictable?
10. **Aesthetic Integrity** — Is visual design purposeful and uncluttered?
11. **Modelessness** — Are unnecessary modes avoided?
12. **Visual Hierarchy** — Size, contrast, and position guide the eye correctly?
13. **Accessibility** — Color not sole indicator, sufficient contrast, readable text?

For each validation point, note: **PASS**, **FAIL**, or **N/A**

## Step 4: Generate Report

Use this format:

```markdown
## Audit Summary: [Screen/Feature Name]

### Critical Issues (Fix Immediately)
1. **[Category]**: [Specific observation]
   → **Fix**: [Actionable recommendation]

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

### Overall Design Quality: X/10
```

---

# Key Principles (Quick Reference)

| # | Principle | Question |
|---|-----------|----------|
| 1 | **Metaphors** | Does the UI use familiar real-world concepts? |
| 2 | **Direct Manipulation** | Can users grab, drag, and interact with objects? |
| 3 | **See-and-Point** | Is everything discoverable without memorization? |
| 4 | **Consistency** | Same look/behavior for same function everywhere? |
| 5 | **Feedback** | Immediate visual/audio response to every action? |
| 6 | **Forgiveness** | Can users undo? Are destructive actions warned? |
| 7 | **Stability** | Does the interface feel solid and predictable? |
| 8 | **Aesthetic Integrity** | Is visual design clear, purposeful, uncluttered? |

---

# Common Violations

## Critical
- **Color as sole indicator** — Status communicated only through color
- **No keyboard access** — Functions unreachable without mouse
- **Invisible state** — User can't tell what mode they're in
- **No undo** — Destructive actions without recovery
- **Illegible text** — Font too small or insufficient contrast

## Major
- **Inconsistent styling** — Same-level elements with different visual treatment
- **Missing feedback** — Actions complete without visual confirmation
- **Crowded layout** — Insufficient spacing between interactive elements
- **Ambiguous controls** — Buttons/labels that don't clearly communicate function
- **Trapped modes** — No obvious way to exit a state

## Minor
- **Verbose labels** — Text that could be more concise
- **Weak visual hierarchy** — Everything at same visual prominence
- **Misaligned elements** — Controls not on grid
- **Unnecessary decoration** — Visual elements that don't serve function

---

# Commands

- `/audit-design` — Start a new Apple HIG audit
- `/audit-design:quick` — 5-minute quick audit, key principles only
- `/audit-design:deep` — Full comprehensive audit with all details

---

# Remember

The goal is **actionable feedback**. Every issue identified should have:

1. **What's wrong** — Specific observation with location
2. **Why it matters** — Which principle it violates
3. **How to fix it** — Concrete recommendation

Don't just list problems. Help the user fix them.

---

*Based on "Human Interface Guidelines: The Apple Desktop Interface", Apple Computer Inc., 1987.*
*Full guidelines: `docs/apple_hig.md`*
