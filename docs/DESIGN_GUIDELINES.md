# Design defaults

Use one readable sans-serif family per ordinary screen, regular body text and
bold headings. WM uses Atkinson Hyperlegible with a 192px rasterized atlas;
requested UI sizes stay unchanged. Keep intentional imitation styles and font
demonstrations distinct from the default interface.

Use a 4px minor and 8px major spacing unit. Group related fields with space before
adding containers. Align labels and values to shared edges. Start with one clear
primary action; passive status text should not look clickable.

## Theme

`ui::Theme{}` supplies a neutral dark palette, blue primary actions, status colors,
8px control corners and 12px panel corners. Full-screen backgrounds stay sharp.
Open `default_theme` to see settings, navigation, validation, disabled controls,
confirmation, tooltips and toasts without screen-specific colors or fonts.

| Role | Use |
|---|---|
| Background | Page |
| Surface | Grouped content |
| Primary | Main action |
| FontMuted | Supporting text |
| Status colors | State fills with a readable foreground and a label/icon |

Load a regular font and its `@bold` companion through `FontManager`; select the
family through `UIStylingDefaults::set_default_font`. Afterhours does not bundle
font assets. Use `TypographyScale` and `DefaultSpacing`: micro 4px, tiny 8px,
then 16/24/32/48/64px. Proportional spacing remains height-relative and resolves
again on resize. Custom horizontal resolution needs
`resolve_to_pixels(size, axis_extent, screen_height)`.

`raised_surface()` and `subtle_border()` derive from the current surface;
`control_border(background)` targets 3:1 against that backing. Optional
`surface_raised`, `border`, and `border_muted` override these choices. Theme-file
persistence of new optional tokens and general styled-label line spacing remain
[open](../todo.md).

## Review checklist

- Compare the same viewport and state against the mock, including full composition,
  artwork, type, spacing, borders and depth. Passing E2E proves only its assertions.
- Keep body text left aligned and prefer sentence case. Evaluate 1.5–2 line height
  for long prose, not single-line controls. Use real weight variants.
- Target 4.5:1 for ordinary text, 3:1 for qualifying large text and essential
  control/state boundaries. Measure alpha-composited colors on every backing.
  Disabled controls have exceptions; this checklist is not a conformance claim.
- Pair color with text or shape. Avoid decorative borders and competing accents.
  Retain visible focus, hover, pressed, selected and disabled states.
- Treat 44px as a useful touch-target goal, not a universal WCAG AA requirement.
  Hit targets, clipping and keyboard navigation must match visible controls.
- Modal dialogs contain focus and restore it on close. Escape dismisses the
  innermost applicable popup first. Preserve entered data and offer recovery.
- Use short action labels and familiar shortcuts. Icon-only controls need names.
  Translation owns icon/key placement; test long strings and non-English text.
- Motion must be interruptible and have a reduced-motion route. Check keyframes
  in a contact sheet, including a second action during a transition.
- Inspect narrow and large layouts, overlays, long lists, blank/error states and
  nested scrolling. Check focus paint after opaque children, not just its bounds.

## Transparent controls

`transparent_controls` demonstrates light, dark and checkerboard backings. Source-over
byte compositing uses `round(fill*a/255 + backing*(1-a/255))` before sRGB contrast.
Measured default/hover text contrast was at least 7.61:1; disabled text at least
4.59:1. Focus against the backing was 3.23–4.31:1. These are color calculations;
`294_transparent_controls.e2e` and captures check actual interaction and pixels.
The 3px focus ring is outside the control at offset -5px at 720p; moving it inside
some hover fills fails 3:1. The Raylib example does not close Sokol alpha reports.

## Sources

The supplied Dannaway articles informed spacing, hierarchy and semantic colors.
Carbon suggests reviewing whole workflows; Atlassian suggests semantic elevation;
Spectrum emphasizes input-dependent sizing and clear copy; GOV.UK emphasizes
predictable, tested interactions. These are design references, not inherited
accessibility certification. [Research](research.md) records sources and limits.
