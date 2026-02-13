# Design Guidelines

Consolidated design guidelines for the afterhours UI framework. This document covers principles, color, typography, spacing, layout, accessibility, interaction patterns, and style conventions.

---

## 1. Core Principles

### 1.1 Clarity Over Ornament

Every visual element must reduce cognitive load. If an element doesn't help the user understand or act, remove it.

### 1.2 Consistency Builds Trust

Use the same patterns for the same purposes throughout the application. Same buttons, same spacing, same typography for equivalent elements. Inconsistent styling looks like multiple unfinished designs stitched together.

### 1.3 Text-First Actions

Default to text labels for actions. Icons are optional aids, not requirements. If an icon cannot be identified without its label, it should not exist.

### 1.4 Accessibility Is Not Optional

Build accessibility into every decision. Color is never the sole means of conveying information. All interactive elements must be keyboard-reachable. Focus indicators must be visible. Minimum touch/click targets are 44x44px.

### 1.5 Evidence-Based Design

Ground feedback in observed behavior, not theory. Use screenshots, interaction states, and real user testing to validate decisions. Avoid generic aesthetic opinions -- tie every critique to a specific screen, state, or interaction.

---

## 2. Color System

### 2.1 Theme Colors

Use `Theme::Usage` for UI elements that should adapt to theme changes (light/dark mode, palette switches):

| Usage | Purpose |
|-------|---------|
| `Primary` | Main action buttons, key interactive elements |
| `Secondary` | Secondary actions, accents |
| `Background` | Panel backgrounds |
| `Foreground` | Text color |
| `Accent` | Highlights, focus rings |
| `Disabled` | Disabled elements |
| `Focus` | Focus ring color (defaults to white for high contrast) |

### 2.2 Fixed Colors

Use `with_fixed_color()` / `with_custom_color()` for colors that should never change with theme:

- Brand colors
- Game-specific colors (health bar red, mana blue)
- Decorative elements with fixed colors
- Colors from external data (user-selected, loaded from file)

### 2.3 Semantic State Colors

| State | Use |
|-------|-----|
| Error | Error states, destructive actions, required indicators |
| Warning | Caution states, important notices |
| Success | Success states, confirmation, positive actions |
| Info | Informational states, tips, neutral alerts |

### 2.4 Color Rules

- **Never use color alone to convey information.** ~4.5% of the population has color insensitivity (especially red-green). Always pair with text, icons, patterns, or shape.
- **Maintain contrast for readability.** WCAG AA minimum: 4.5:1 for normal text, 3:1 for large text (19px+ bold or 24px+ normal).
- **Limit accent colors.** Use accent colors only for purposeful states (alert, selection, focus). Aim for roughly 60% primary / 30% secondary / 10% accent in non-background color usage.
- **Test in grayscale.** The interface should remain fully usable when viewed in grayscale.
- **Consider APCA.** For new work, prefer APCA (Accessible Perceptual Contrast Algorithm) over WCAG 2 ratio math. APCA is polarity-aware and perceptually uniform. Key thresholds: Lc 75+ for body text, Lc 60+ for large text, Lc 45+ for headlines, Lc 30+ for placeholder/disabled.

### 2.5 Color Palette Reference (Wordproc / Win95)

| Name | Hex | Usage |
|------|-----|-------|
| Window Gray | `#C0C0C0` | Main window background, status bar |
| Title Bar Blue | `#000080` | Active title bar |
| White | `#FFFFFF` | Text area background, title text |
| Black | `#000000` | Primary text |
| Border Light | `#FFFFFF` | 3D highlight (top/left) |
| Border Dark | `#808080` | 3D shadow (bottom/right) |

### 2.6 Color Palette Reference (Scrapbook)

Four palettes available, selected per-project:

- **Kraft & Cream** -- warm, nostalgic (cream/kraft/manila/sepia/terracotta)
- **Pastel Dream** -- soft, feminine (blush/lavender/pink/mint)
- **Sage & Stone** -- earthy, organic (sage/olive/terracotta/rust)
- **Coral Pop** -- bright, playful (coral/yellow/blue/purple)
- **Midnight Journal** -- dark mode (midnight/charcoal/gold/rose/teal)

---

## 3. Typography

### 3.1 Type Hierarchy

Maintain a clear type scale with consistent hierarchy. The reader should tell at a glance what is a heading, subheading, body text, and caption.

| Role | Size Range | Weight |
|------|-----------|--------|
| Display / H1 | 36-48px | Bold |
| Section / H2 | 28-40px | Bold |
| Subsection / H3 | 22-32px | Semibold |
| Body | 16-17px | Regular |
| Secondary | 14-16px | Regular |
| Caption / Fine | 12-14px | Regular |

### 3.2 Typography Rules

- **Minimum body text: 16px.** Smaller sizes only for captions, fine print, or metadata.
- **Line height: 1.5x font size** for body text. 1.25x is acceptable for headings.
- **Line length: 45-75 characters** for body text.
- **Left-align body text.** Centered text is only for headings, hero sections, or short labels.
- **Avoid ALL CAPS** except for very short labels or category headers.
- **Max 3 fonts per screen.** More creates visual chaos.
- **Consistent application.** All H2s look the same. All body text looks the same.

### 3.3 Font Families

**UI/Body:** `Inter`, `SF Pro`, system-ui, sans-serif

**Wordproc project:**
- UI: System default
- Document: Gaegu-Bold (default), EBGaramond-Regular (alternate)
- Mono: NotoSansMonoCJK

**Scrapbook project (extended stack):**
- Display: `Anton`, `Bebas Neue`, `Oswald`
- Script: `Pinyon Script`, `Great Vibes`, `Dancing Script`
- Pixel: `Press Start 2P`, `VT323`, `Silkscreen`
- Handwritten: `Caveat`, `Patrick Hand`, `Kalam`
- Marker: `Permanent Marker`, `Rock Salt`
- Typewriter: `Courier Prime`, `Special Elite`

### 3.4 Responsive Font Sizing

Font sizes should scale with screen resolution. Use `ui::responsive::font_px()` or the responsive font presets (`font_xs()` through `font_2xl()`) to ensure proportional scaling across resolutions.

---

## 4. Spacing & Layout

### 4.1 Spacing Scale

Use a coherent spacing system based on 4/8px increments:

| Token | Value | Common Use |
|-------|-------|------------|
| `xs` | 4px | Tight internal spacing |
| `sm` | 8px | Small gaps, base unit |
| `md` | 12-16px | Standard component padding |
| `lg` | 24px | Section separation |
| `xl` | 32-40px | Component group separation |
| `2xl` | 48-64px | Major section spacing |

### 4.2 Layout Rules

- **Align to a grid.** All elements should sit on consistent alignment lines.
- **Group by proximity.** Related items closer together, unrelated items farther apart (Gestalt proximity).
- **Preserve vertical scan lines** in lists and menus.
- **Keep safe margins from screen edges.** Never clip or overflow.
- **No horizontal scrolling** at any supported viewport width.

### 4.3 Screen Safety & Boundaries

- UI elements must never be cut off by screen edges.
- Elements must never render outside their intended containers.
- Respect safe-area boundaries (TV overscan, mobile notches, UI margins).
- Minimum padding from screen edges for readability.
- Test edge cases: long text, max values, empty states.

### 4.4 Responsive Layout

Use the breakpoint system for different screen sizes:

| Breakpoint | Width |
|-----------|-------|
| XS | 480px |
| SM | 768px |
| MD | 1024px |
| LG | 1280px |
| XL | 1920px |

- Use `ui::responsive::px()` for sizes that scale proportionally with screen resolution.
- Use `ui::responsive::at_breakpoint<T>()` for values that change at thresholds.
- Sidebars should collapse below MD. Grid columns should reduce at smaller breakpoints.

### 4.5 3D Border Effects (Win95 Style)

For retro-styled UI, use consistent beveled borders:

- **Raised** (buttons, menu bars): Top/left = light (`#FFFFFF`), bottom/right = dark (`#808080`)
- **Sunken** (text areas, inputs): Top/left = dark, bottom/right = light

---

## 5. Menus & Iconography

### 5.1 Menu Rules

- **Icons in menus are opt-in.** Only use when the icon adds meaning that text cannot.
- **Never use arbitrary symbols** in menus. Only standard marks.
- **Ellipsis (...)** only when additional input is required before execution.
- **Checkmarks** indicate current selection. **Dashes** indicate partial selection.
- Group related items with standard dividers (use sparingly).
- If icons are used, reserve a fixed icon column for all items to avoid scan breaks.
- Keep menu titles stable and predictable.

### 5.2 Iconography Rules

- Use appropriate, recognizable metaphors. Avoid confusing or overly clever visuals.
- **No text inside icons** (not localizable, hard to read).
- Design for small sizes first: minimal detail, pixel-aligned, clear silhouettes.
- Maintain a consistent icon family (stroke weight, perspective, lighting).
- **One action = one icon.** Never reuse one icon for multiple meanings.
- Paired actions should use mirrored/symmetrical metaphors (undo/redo).
- All icons must be from the same style family (don't mix filled and outlined).

### 5.3 Icons in Menus (Special Rules)

- Only include icons for spatial/visual outcomes (layout positions, alignment).
- Do not add icons to fill space.
- If a menu uses checkmarks/dashes, verify combined alignment and clarity.

---

## 6. Controls & Interaction

### 6.1 General Controls

- Prefer modeless UI when possible to preserve user control.
- Always provide clear feedback for long-running actions.
- Match dialog titles to their triggering menu item (minus ellipsis).
- Use standard controls and states. Avoid novel behaviors without strong user value.

### 6.2 Buttons

- **Clear hierarchy:** One primary action per context, with secondary/tertiary visually subordinate.
- **Variants:** Filled (primary), Outline (secondary), Ghost (tertiary), Icon (icon-only).
- **Labels:** Clear, concise action verbs. "Save changes" not "Submit".
- **Min target:** 44x44px.
- **Disabled state:** Visually muted. Explain WHY via tooltip or nearby text.
- **Destructive actions:** Visually distinct (e.g., red) and require confirmation.
- **Icon + text:** If buttons use icons, always include a text label too.

### 6.3 Forms

- Labels above fields (preferred for scannability).
- Field widths hint at expected input length.
- Validate on blur, not while typing.
- Required fields clearly marked.
- Error messages specific and helpful ("Enter your email address" not "Invalid input").
- Single-column layout preferred.

### 6.4 Alerts & Feedback

- Use correct alert types: Info, Warning, Error, Success.
- Alerts explain WHAT happened and WHAT to do next.
- Place alerts near relevant content (inline) or at page top (page-level).
- Long operations show progress indicators.
- Important/destructive actions require confirmation.

---

## 7. Accessibility

### 7.1 Contrast Requirements

| Content | WCAG 2 Minimum | APCA Minimum |
|---------|---------------|-------------|
| Normal text (<18px) | 4.5:1 ratio | Lc 75 |
| Large text (18px+ or 14px+ bold) | 3:1 ratio | Lc 60 |
| Headlines (24px+ bold or 36px+) | 3:1 ratio | Lc 45 |
| Placeholder / disabled text | -- | Lc 30 |
| UI components (borders, icons) | 3:1 ratio | Lc 30 |
| Focus indicator | 3:1 change-of-contrast | -- |

### 7.2 Keyboard Navigation

Every interactive element must be reachable and activable via keyboard:

| Widget | Keyboard Behavior |
|--------|------------------|
| Button | Enter/Space activate |
| Checkbox | Space toggles (Enter does NOT toggle) |
| Radio group | Arrows select within group, Tab exits (roving focus) |
| Tabs | Arrows switch tabs, Tab goes to panel (roving focus) |
| Slider | Arrows adjust, Home/End min/max, PageUp/PageDown large step |
| Modal | Escape closes, Tab trapped within, focus returns on close |
| Menu | Arrows navigate, Enter activates, Escape closes, type-ahead |
| Tooltip | Shows on focus (not just hover), Escape dismisses |
| Toolbar | Arrows move between tools, Tab exits (roving focus) |
| Accordion | Enter/Space toggle, Arrows between headers |

### 7.3 Focus Management

- **Focus indicators:** Every focused element must show a visible ring. Minimum 2px thick. Use two-color indicators (dark + light) to work against any background.
- **Focus ring config:** `theme.focus_ring_thickness` (default 3px), `theme.focus_ring_offset` (default 4px).
- **Focus return:** When a modal/popup closes, focus MUST return to the element that opened it.
- **Focus trapping:** While a modal is open, Tab must cycle within the modal only.
- **Initial focus:** When a dialog opens, focus moves to the first focusable element inside.

### 7.4 Roving Focus Pattern

Composite widgets (radio groups, tab bars, toolbars, menus) use the "roving focus" pattern:
- The entire group is one tab stop.
- Arrow keys move focus within the group.
- Tab exits the group entirely.

Non-composite widgets (checkboxes, buttons, form fields) each get their own tab stop.

### 7.5 Touch & Click Targets

- Minimum 44x44px for all interactive elements.
- Primary actions should be within thumb reach on mobile (bottom center of screen).
- Hover-dependent interactions must have tap-friendly alternatives on touch devices.

---

## 8. Visual Hierarchy & Composition

### 8.1 Hierarchy

- Clear visual hierarchy: the reader knows what's most important at a glance.
- Accommodate natural reading patterns (F-pattern / Z-pattern).
- Use white space intentionally to separate content groups.
- Balance the design -- not top-heavy, not lopsided.

### 8.2 States & Interactivity

Every interactive element must have visually distinct states:

| State | Requirement |
|-------|------------|
| Default | Clear resting appearance |
| Hover | Visible change (highlight, cursor change) |
| Active/Pressed | Pressed appearance (sunken, darker) |
| Focus | Visible focus indicator |
| Disabled | Muted/dimmed, NOT confusable with active |
| Selected | Clearly indicated (active tab, checked item) |
| Loading | Show loading indicator |
| Empty | Helpful message + suggested action |

### 8.3 Animations & Timing

- Animations should be purposeful, not decorative.
- Win95-style UIs: instant open/close (no animation, authentic to era).
- Scrolling: discrete steps (line-by-line), no smooth scroll for retro style.
- Caret blink: 500ms on / 500ms off, reset on input.

---

## 9. Content & Language

- **Plain language.** No jargon, no legalese, no bureaucratic language.
- **Active voice.** "Enter your email" not "Your email should be entered."
- **Short sentences.** Target under 20 words for UI text.
- **Scannable content.** Headings, bullet points, short paragraphs.
- **Consistent terminology.** Same term for same concept throughout (don't switch between "log in" / "sign in" / "authenticate").
- **Error messages.** Helpful, specific, non-blaming. "Enter an email address" not "Error 422: Invalid input format."

---

## 10. Vendor Library (afterhours) Constraints

When working with `vendor/afterhours`, we do not modify vendor code. Instead:

- Document gaps in `AFTERHOURS_GAPS.md`.
- Add workarounds in application source code.
- Use `ComponentConfig` for customization where possible.
- When vendor components misbehave (e.g., `tab_container` rendering tabs at screen-absolute position, `toggle_switch` creating sibling entities), build equivalent behavior manually.

---

## 11. Review Checklist (Quick Pass)

- [ ] Menu items use only standard marks (checkmark/dash/ellipsis)
- [ ] Icons are used only when they add meaning, consistent across the app
- [ ] Actions have clear text labels; icons are not the only cue
- [ ] Small-size icons remain legible without micro-detail
- [ ] Visual scan lines are preserved; alignment is consistent
- [ ] Color is redundant, contrast is adequate, states are unambiguous
- [ ] All interactive elements have 44x44px minimum target
- [ ] Focus indicators visible on all focusable elements
- [ ] Keyboard navigation works for all interactive elements
- [ ] No elements clip or overflow their containers
- [ ] Typography hierarchy is clear and consistent
- [ ] Spacing follows the 4/8px scale
- [ ] Error messages are specific and helpful
