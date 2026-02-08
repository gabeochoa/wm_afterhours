---
name: audit-win95-design
description: Audit any UI/UX design against Microsoft's Windows Interface Guidelines (1995). Provides comprehensive feedback on windows, menus, controls, toolbars, visual design, 3D borders, keyboard access, and design quality issues for Win95-era applications.
---

# Windows 95 Interface Design Audit

Systematically evaluate UI/UX designs against Microsoft's *The Windows Interface Guidelines — A Guide for Designing Software* (February 1995), covering the canonical Win95 look-and-feel for desktop applications.

## When to Use

- `/audit-win95-design` — Full audit of a screen, component, or interaction
- After completing major UI work on a Win95-styled application
- When validating fidelity to the classic Windows 95 / Word 6.0 aesthetic
- To identify guideline compliance issues before shipping

## Scope

**Use for:** Any UI/UX targeting the Windows 95 / Windows 3.1 design language — word processors, file managers, dialog boxes, toolbars, menus, controls, games with Win95-style settings screens.

**Not for:** Modern flat/material UI, Apple-platform apps (use `/audit-design`), Material Design apps (use `/audit-google-design`).

---

# The Process

## Step 1: Gather Context

Before auditing, you need to see and understand the design:

1. **Request the design** — Ask for screenshot, code, or description
2. **Understand the context** — What is this for? Who uses it? What task does it support?
3. **Identify scope** — Are we auditing one screen, one flow, or the whole app?
4. **Determine app type** — Document-centric (like Word), tool-based (like Calculator), or MDI?

## Step 2: Read the Guidelines

Load the comprehensive checklist:

```
Read: docs/microsoft_win95_hig.md
```

This contains the full audit framework based on Microsoft's 1995 Windows Interface Guidelines.

## Step 3: Systematic Audit

Walk through each category in the guidelines document:

1. **Design Principles (7 Principles)**
   - User in Control
   - Directness
   - Consistency
   - Forgiveness
   - Feedback
   - Aesthetics
   - Simplicity

2. **Windows**
   - Primary window components
   - Title bars (icon, text order, buttons)
   - Window operations (move, resize, activate)
   - Scroll bars (proportional, always visible)

3. **Menus**
   - Menu bar (position, access keys, standard menus)
   - Common drop-down menus (File, Edit, View, Help)
   - Menu items (ellipsis, disabled state, check marks, shortcuts)
   - Pop-up (context) menus
   - Cascading menus

4. **Controls**
   - Command buttons (labels, pressed state, unavailable)
   - Option buttons (radio buttons)
   - Check boxes
   - List boxes and drop-down lists
   - Text boxes (edit controls)
   - Combo boxes
   - Group boxes, tabs, sliders, progress indicators

5. **Toolbars and Status Bars**
   - Toolbar (tooltips, button sizes, standard images)
   - Status bar (position, contextual info, menu descriptions, size grip)

6. **Secondary Windows**
   - Dialog boxes (OK/Cancel, default button, layout, tab navigation)
   - Property sheets (tabs, Apply button)
   - Message boxes (symbols, clear text, safe defaults)
   - Common dialog boxes (Open, Save, Find, Print)

7. **Visual Design**
   - Composition and organization
   - Color (system colors, limited palette, monochrome compatibility)
   - Fonts (system font, limited styles)
   - 3D dimensionality (light source upper-left)
   - Border styles (window, button, field, status field, grouping)
   - Visual states (pressed, option-set, mixed-value, unavailable)
   - Icon design (multiple sizes, 3D perspective)
   - Layout and spacing (DLU-based)

8. **User Assistance**
   - Context-sensitive Help (What's This?)
   - Tooltips
   - Status bar messages
   - Wizards

9. **Input and Interaction**
   - Mouse input (pointer feedback, button 2 = pop-up)
   - Keyboard input (access keys, standard shortcuts, tab navigation)
   - Selection (object-action paradigm, visual feedback)
   - Transfer operations (Cut/Copy/Paste, drag and drop)

10. **Accessibility**
    - Color not only cue
    - High Contrast support
    - Full keyboard access
    - Screen reader compatibility
    - No critical time-outs

11. **System Integration**
    - Standard keyboard shortcuts (reserved, must not reassign)
    - Title bar conventions

For each validation point, note: **PASS**, **FAIL**, or **N/A**

## Step 4: Generate Report

Use this format:

```markdown
## Audit Summary: [Screen/Feature Name]

### Critical Issues (Fix Immediately)
1. **[Category]**: [Specific observation]
   → **Fix**: [Actionable recommendation with specific Win95 guideline reference]

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

### Overall Windows Interface Compliance: X/10
```

---

# The 7 Design Principles (Quick Reference)

When time is limited, focus on these:

| # | Principle | Question |
|---|-----------|----------|
| 1 | **User in Control** | Does the user always feel in charge, not the software? |
| 2 | **Directness** | Can users directly manipulate objects? Are metaphors familiar? |
| 3 | **Consistency** | Same action = same result? Within product, with OS, with metaphors? |
| 4 | **Forgiveness** | Can users recover from mistakes? Undo? Warnings? |
| 5 | **Feedback** | Is there immediate response to every action? Progress for long ops? |
| 6 | **Aesthetics** | Is the visual design clear, purposeful, and uncluttered? |
| 7 | **Simplicity** | Is the interface easy to learn? Progressive disclosure? Concise? |

---

# Win95 Key Concepts

## 3D Border Styles
Five distinct border styles, each used for specific purposes:

| Style | Light/Shadow | Used On |
|-------|-------------|---------|
| **Window Border** | Raised outer + raised inner | Primary and secondary windows |
| **Button Border** | Raised outer + raised inner (swapped); sunken when pressed | Command buttons |
| **Field Border** | Sunken outer + sunken inner | Text boxes, list boxes, check boxes |
| **Status Field Border** | Sunken outer only | Status bar fields |
| **Grouping Border** | Sunken outer + raised inner | Group boxes, menu separators |

Light source is always upper-left: highlights on top/left edges, shadows on bottom/right edges.

## System Colors
All interface colors reference system color settings, never hardcoded:

| Color | Typical RGB | Use |
|-------|-------------|-----|
| COLOR_BTNFACE | (192, 192, 192) | Button/toolbar backgrounds |
| COLOR_BTNHIGHLIGHT | (255, 255, 255) | Light 3D border edges |
| COLOR_BTNSHADOW | (128, 128, 128) | Dark 3D border edges |
| COLOR_WINDOW | (255, 255, 255) | Text field/list backgrounds |
| COLOR_WINDOWTEXT | (0, 0, 0) | Text in windows |
| COLOR_ACTIVECAPTION | (0, 0, 128) | Active title bar |
| COLOR_CAPTIONTEXT | (255, 255, 255) | Active title bar text |
| COLOR_MENU | (192, 192, 192) | Menu backgrounds |
| COLOR_HIGHLIGHT | (0, 0, 128) | Selection highlight |
| COLOR_HIGHLIGHTTEXT | (255, 255, 255) | Selected text |
| COLOR_GRAYTEXT | (128, 128, 128) | Disabled text |

## Standard Sizes
| Element | Size |
|---------|------|
| Toolbar button (small) | 24 × 22 px |
| Toolbar button (large) | 32 × 30 px |
| Toolbar icon (small) | 16 × 16 px |
| Toolbar icon (large) | 24 × 24 px |
| Full icon | 32 × 32 px |
| Small icon | 16 × 16 px |
| System font | MS Sans Serif, 8pt |

## Reserved Keyboard Shortcuts
These MUST NOT be reassigned:

| Key | Function |
|-----|----------|
| Ctrl+N | New |
| Ctrl+O | Open |
| Ctrl+S | Save |
| Ctrl+P | Print |
| Ctrl+Z | Undo |
| Ctrl+X | Cut |
| Ctrl+C | Copy |
| Ctrl+V | Paste |
| Ctrl+A | Select All |
| Ctrl+F | Find |
| F1 | Help |
| Alt+F4 | Close window |
| ESC | Cancel |

## Standard Menu Order
File, Edit, View, Insert, Format, Tools, Table, Window, **Help** (always last)

---

# Common Violations

## Critical
- **No Undo** — Users can't recover from mistakes
- **No Keyboard Access** — Cannot reach controls via keyboard
- **Missing Scroll Bars** — Content exceeds window but no way to scroll
- **Blocked User** — Modal prevents Save/Quit
- **Silent Failures** — Errors without any feedback
- **Non-Standard Shortcuts** — Ctrl+C, Ctrl+V, etc. reassigned to other functions

## Major
- **Inconsistent Border Styles** — Wrong border type for controls (raised for fields, sunken for buttons)
- **No Tooltips** — Toolbar buttons without labels or tooltips
- **Wrong Capitalization** — Sentence caps on menu items, title caps on check boxes
- **No Context Menus** — Missing pop-up menus on right-click
- **Generic Button Labels** — "Yes/No" instead of specific action names in message boxes
- **Technical Error Messages** — Jargon, error codes, blame language
- **Help Not Last Menu** — Help menu not at rightmost position

## Minor
- **Verbose Labels** — Menu items or messages that could be more concise
- **Non-Standard Menu Order** — Menus not following File, Edit, View convention
- **Missing Accelerators** — Common operations without shortcut keys
- **No State Preservation** — Dialog boxes not remembering last settings
- **Weak Visual Hierarchy** — Everything at same visual prominence
- **No Size Grip** — Sizable windows missing the size grip

---

# Audit Depth Levels

## Quick Audit (5 min)
- Check the 7 design principles only
- Note 3-5 biggest issues
- Immediate recommendations

## Standard Audit (20 min)
- All 11 categories at high level
- Prioritized issue list
- Component spot-checks

## Deep Audit (60+ min)
- Full checklist, every item
- Every component evaluated
- Accessibility deep dive
- All border styles verified
- System color compliance checked
- Keyboard navigation tested
- All menu items checked for access keys and shortcuts

---

# Commands

- `/audit-win95-design` — Start a new Windows 95 Interface audit
- `/audit-win95-design:quick` — 5-minute quick audit, 7 principles only
- `/audit-win95-design:deep` — Full comprehensive audit with all details

---

# Comparison with Other Guidelines

| Aspect | Apple HIG (1987) | Windows 95 Guidelines | Material Design 3 |
|--------|------------------|-----------------------|-------------------|
| Philosophy | Desktop metaphor | Data-centered design | Physical paper surfaces |
| Elevation | Minimal, flat | 3D beveled borders | 6 tonal levels |
| Color | Conservative | System color-based | 26 semantic roles |
| Motion | Subtle | Minimal | Physics-based springs |
| Typography | Chicago | MS Sans Serif | Roboto, 30 styles |
| Spacing | Various | DLU-based | 8dp grid |
| Touch/Click | 44pt minimum | Not specified (desktop) | 48dp minimum |
| Windows | MDI not recommended | MDI fully supported | N/A (mobile) |
| Help | Apple Guide | What's This?, Wizards | N/A |
| Context Menus | Not standard | Required everywhere | Long-press |
| Property Sheets | N/A | Tabbed, modeless | Bottom sheets |

---

# Remember

The goal is **actionable feedback**. Every issue identified should have:

1. **What's wrong** — Specific observation with location
2. **Why it matters** — Which Win95 guideline it violates
3. **How to fix it** — Concrete recommendation with specific values (colors, sizes, border styles)

Don't just list problems. Help the user fix them with specific values from the Windows Interface Guidelines.

---

*Based on "The Windows Interface Guidelines — A Guide for Designing Software", Microsoft Corporation, February 1995.*
*Full guidelines: `docs/microsoft_win95_hig.md`*
*Reference PDF: https://ics.uci.edu/~kobsa/courses/ICS104/course-notes/Microsoft_WindowsGuidelines.pdf*
