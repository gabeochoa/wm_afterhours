---
name: audit-sun-design
description: Audit any UI/UX design against Sun's Java Look and Feel Design Guidelines (1999). Provides comprehensive feedback on the flush 3D style, 8-color model, cross-platform consistency, accessibility, menus, toolbars, dialogs, controls, and visual design quality for Java Swing/JFC applications.
---

# Java Look and Feel Design Audit

Systematically evaluate UI/UX designs against Sun Microsystems' *Java Look and Feel Design Guidelines* (1999), covering the canonical "Metal" look and feel for cross-platform Java Swing/JFC applications.

## When to Use

- `/audit-sun-design` — Full audit of a screen, component, or interaction
- After completing major UI work on a Java Swing/JFC application
- When validating fidelity to the Java look and feel ("Metal") aesthetic
- To identify guideline compliance issues for cross-platform Java apps

## Scope

**Use for:** Any UI/UX targeting the Java look and feel — desktop applications, applets, dialog boxes, toolbars, menus, controls using JFC/Swing with the Metal L&F.

**Not for:** Modern flat/material UI (use `/audit-google-design`), Apple-platform apps (use `/audit-design`), Windows 95-native apps (use `/audit-win95-design`).

---

# The Process

## Step 1: Gather Context

Before auditing, you need to see and understand the design:

1. **Request the design** — Ask for screenshot, code, or description
2. **Understand the context** — What is this for? Who uses it? What platforms?
3. **Identify scope** — Are we auditing one screen, one flow, or the whole app?
4. **Determine type** — Standalone application or applet? MDI or SDI?
5. **Identify theme** — Default Metal, custom theme, or high-contrast?

## Step 2: Read the Guidelines

Load the comprehensive checklist:

```
Read: docs/sun_java_look_and_feel_hig.md
```

This contains the full audit framework based on Sun's 1999 Java Look and Feel Design Guidelines.

## Step 3: Systematic Audit

Walk through each category in the guidelines document:

1. **Design Fundamentals**
   - Visual Identity (flush 3D, drag texture, color model)
   - Cross-Platform Considerations
   - Accessibility (names, descriptions, keyboard, mnemonics)
   - Internationalization (resource bundles, layout managers)

2. **Visual Design**
   - 8-Color Theme Model (3 primary, 3 secondary, black, white)
   - Typography (4 type styles: control, system, user, small)
   - Capitalization (headline vs. sentence)
   - Layout and Spacing (6px base unit)
   - Animation and Progress

3. **Application Graphics**
   - Icons (16×16, 32×32, flush 3D, pre-dithered)
   - Button Graphics (16×16, 24×24, flush 3D effect)
   - Splash Screens and About Boxes

4. **Behavior**
   - Mouse Operations (button 1 = select, button 2 = context)
   - Keyboard Operations (Tab, Ctrl-Tab, arrows, spacebar, Enter, Escape)
   - Keyboard Shortcuts (Ctrl+N/O/S/P/Z/X/C/V/F/A)
   - Mnemonics (all menus, all controls)
   - Drag and Drop

5. **Windows and Containers**
   - Primary Windows (JFrame, native borders)
   - Secondary Windows (JDialog)
   - Scroll Panes, Tabbed Panes, Split Panes
   - Internal Frames (MDI)

6. **Dialog Boxes and Alerts**
   - Dialog design (title, spacing, buttons, traversal)
   - Command buttons (OK/Cancel, Apply/Reset/Close, Help)
   - Alert boxes (Info, Warning, Error, Question symbols)
   - Common dialogs (Find, Login, Preferences, Print, Progress)

7. **Menus**
   - Menu bar (position, titles, mnemonics)
   - Standard order (File, Object, Edit, Format, View, Help)
   - Menu items (caps, shortcuts, ellipsis, separators)
   - Contextual menus, submenus

8. **Toolbars**
   - Placement, drag area, button sizes
   - Spacing (2px individual, 11px groups)
   - Mouse-over borders, drop-down arrows
   - Tool tips

9. **Basic Controls**
   - Command buttons, toggle buttons
   - Checkboxes, radio buttons
   - Combo boxes (editable, noneditable)
   - Sliders, progress bars

10. **Text Components**
    - Labels (identification, status)
    - Text fields (editable, noneditable, password)
    - Text areas, editor panes

11. **Lists, Tables, and Trees**
    - Selection models (single, range, multiple ranges)
    - Column reordering, resizing, row sorting
    - Tree views (turners, root hidden, lines)

For each validation point, note: **PASS**, **FAIL**, or **N/A**

## Step 4: Generate Report

Use this format:

```markdown
## Audit Summary: [Screen/Feature Name]

### Critical Issues (Fix Immediately)
1. **[Category]**: [Specific observation]
   → **Fix**: [Actionable recommendation with JLF guideline reference]

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

### Overall Java Look and Feel Compliance: X/10
```

---

# The 3 Hallmarks (Quick Reference)

When time is limited, check these first:

| # | Hallmark | Question |
|---|---------|----------|
| 1 | **Flush 3D** | Do components appear at the same level as the canvas (not beveled)? |
| 2 | **Drag Texture** | Is the textured pattern used consistently for draggable elements? |
| 3 | **Color Model** | Does the app use the 8-color model with dark-to-light gradation? |

---

# Key Concepts

## Flush 3D vs. Beveled 3D
The Java look and feel uses **flush 3D** — components appear to be at the same level as the surrounding canvas, with subtle border effects. This is NOT the raised/sunken beveled style of Windows 95. The result is a cleaner, more modern appearance that reduces visual noise.

## 8-Color Model
All interface colors derive from 8 base colors (3 primary, 3 secondary, black, white). This ensures compatibility across platforms and devices with varying color depths. Colors maintain a dark-to-light gradation within primary and secondary groups.

| Color | Default RGB | Hex | Role |
|-------|-------------|-----|------|
| Primary 1 | 102-102-153 | #666699 | Active borders, labels |
| Primary 2 | 153-153-204 | #9999CC | Selection, focus |
| Primary 3 | 204-204-255 | #CCCCFF | Title bars, text selection |
| Secondary 1 | 102-102-102 | #666666 | Dark 3D border |
| Secondary 2 | 153-153-153 | #999999 | Inactive, dimmed |
| Secondary 3 | 204-204-204 | #CCCCCC | Background canvas |
| Black | 0-0-0 | #000000 | Text |
| White | 255-255-255 | #FFFFFF | Highlights, text entry bg |

## Standard Spacing
- **6px base unit** (perceived) — all spacing in multiples of 6
- **5px actual** within groups (6px perceived due to flush 3D white border)
- **11px actual** between groups (12px perceived)
- **12px** border padding (top/left); **11px** (bottom/right)

## Reserved Keyboard Shortcuts

| Key | Function |
|-----|----------|
| Ctrl+N | New |
| Ctrl+O | Open |
| Ctrl+S | Save |
| Ctrl+P | Print |
| Ctrl+W | Close |
| Ctrl+Z | Undo |
| Ctrl+Y | Redo |
| Ctrl+X | Cut |
| Ctrl+C | Copy |
| Ctrl+V | Paste |
| Ctrl+F | Find |
| Ctrl+G | Find Again |
| Ctrl+A | Select All |
| F10 | Focus menu bar |
| Escape | Cancel/dismiss |
| Ctrl+F1 | Show/hide tool tip |
| Shift+F10 | Show contextual menu |

## Standard Menu Order
File, Object, Edit, Format, View, [app-specific], **Help** (always last)

---

# Common Violations

## Critical
- **No Keyboard Access** — Functions unreachable without mouse
- **No Mnemonics** — Violates JLF standard; excludes keyboard users
- **Missing Accessible Names** — Screen readers cannot identify components
- **Hardcoded Text** — Blocks internationalization
- **No Escape = Cancel** — Users cannot dismiss dialogs with keyboard
- **Beveled Instead of Flush** — Wrong 3D style for Java L&F

## Major
- **Wrong Color Model** — Not using the 8-color theme
- **Missing Tool Tips** — Toolbar buttons without text or tips
- **Wrong Capitalization** — Sentence caps on menu items, headline caps on status
- **No Progress Indication** — Operations > 2 seconds with no feedback
- **Nested Tabbed Panes** — Explicitly forbidden
- **Unsafe Default Button** — Data-destroying action is the default
- **Help Not Last Menu** — Help menu not at rightmost position

## Minor
- **Non-6px Spacing** — Inconsistent inter-component spacing
- **Missing Pre-Dithering** — Solid colors dither on 8-bit displays
- **Icon Style Mixing** — 2D and 3D icons in the same family
- **Verbose Labels** — Labels longer than necessary
- **No Zoom Buttons** — Split panes without zoom convenience

---

# Audit Depth Levels

## Quick Audit (5 min)
- Check the 3 hallmarks (flush 3D, drag texture, color model)
- Check accessibility basics (keyboard access, mnemonics)
- Note 3-5 biggest issues

## Standard Audit (20 min)
- All 11 categories at high level
- Prioritized issue list
- Component spot-checks

## Deep Audit (60+ min)
- Full checklist, every item
- Every component evaluated
- Accessibility deep dive
- All spacing verified
- Color model compliance checked
- Keyboard navigation tested for all components
- Cross-platform display tested

---

# Commands

- `/audit-sun-design` — Start a new Java Look and Feel audit
- `/audit-sun-design:quick` — 5-minute quick audit, 3 hallmarks + accessibility
- `/audit-sun-design:deep` — Full comprehensive audit with all details

---

# Comparison with Other Guidelines

| Aspect | Apple HIG (1987) | Windows 95 | Java Look & Feel | Material Design 3 |
|--------|-----------------|------------|------------------|-------------------|
| Philosophy | Desktop metaphor | Data-centered | Cross-platform consistency | Physical paper surfaces |
| 3D Style | Minimal | Beveled borders | Flush 3D | Tonal elevation |
| Color | Conservative | System colors | 8-color model | 26 semantic roles |
| Motion | Subtle | Minimal | Animated pointers | Physics-based springs |
| Typography | Chicago | MS Sans Serif | Dialog (platform-mapped) | Roboto, 30 styles |
| Spacing | Various | DLU-based | 6px multiples | 8dp grid |
| Touch/Click | 44pt min | Not specified | Not specified | 48dp minimum |
| Cross-platform | No | No | Primary goal | Cross-platform |
| Pluggable L&F | No | No | Yes (core feature) | Via theming |
| Context Menus | Not standard | Required | Recommended | Long-press |

---

# Remember

The goal is **actionable feedback**. Every issue identified should have:

1. **What's wrong** — Specific observation with location
2. **Why it matters** — Which JLF guideline it violates
3. **How to fix it** — Concrete recommendation with specific values (colors, spacing, font styles)

Don't just list problems. Help the user fix them with specific values from the Java Look and Feel Design Guidelines.

---

*Based on "Java Look and Feel Design Guidelines", Sun Microsystems, Inc., 1999.*
*Full guidelines: `docs/sun_java_look_and_feel_hig.md`*
*Addison-Wesley, ISBN 0-201-61585-1.*
