# Default theme

`ui::Theme{}` supplies a dark, neutral palette with blue primary actions, readable
text, distinct status colors, an 8px control radius and a 12px panel radius.
Ordinary labels remain transparent. Text fields have visible boundaries; filled,
outline and ghost buttons provide action hierarchy. Dialogs, tooltips and toasts
use the same theme and configured font family.

Open **Component Galleries → default_theme** for a working workspace page. It
includes editable settings, navigation, a disabled action, a save confirmation,
validation feedback, a tooltip and a success toast. Its code chooses layout and
semantic roles rather than custom colors or font faces. It resets WM's showcase
palette to `Theme{}` so the screen demonstrates the library defaults.

## Fonts and spacing

Load a readable regular face and its `@bold` companion through `FontManager`, then
select that family once with `UIStylingDefaults::set_default_font`. Font loading
remains application resource setup; the library does not bundle a font asset.
WM uses Atkinson Hyperlegible, rasterized at 192px and drawn at the requested UI
size. Its fallback aliases now use the same regular/bold family.

Use `TypographyScale` for type and line-height tokens. Use regular body text and
bold headings. `DefaultSpacing::micro()` is the 4px minor unit; `tiny()` is the
8px major unit. Larger spacing tokens are 16, 24, 32, 48 and 64px. These helpers
follow the application scaling mode. Proportional spacing resolves from screen
height during layout, including horizontal spacing and stored configurations. Keep full-screen backgrounds sharp, and use
spacing to group related fields before adding borders.

For custom layout code, pass the screen height to
`resolve_to_pixels(size, axis_extent, screen_height)` when resolving spacing
against a horizontal axis. `screen_height_pct()` carries that reference explicitly;
existing `h720()`, `w1280()` and `screen_pct()` retain their previous semantics.

## Surfaces and contrast

Use `Theme::Usage::Background` for the page, `Surface` for grouped content,
`Primary` for the main action, and `FontMuted` for supporting text. Status colors
are fills: let automatic foreground selection choose readable text. A status
color used directly as small text needs a contrast check against its actual
background.

`raised_surface()` and `subtle_border()` derive colors from the active surface,
including light themes. `control_border(background)` derives an essential edge
with at least 3:1 contrast against that background. The optional `surface_raised`,
`border` and `border_muted` fields override those choices. Explicit component
colors and modal panel configuration retain precedence.

The default-theme tests verify ordinary text contrast of at least 4.5:1 and
essential control/focus contrast of at least 3:1. Disabled actions retain the
existing opacity treatment; they are excluded from those contrast guarantees.

## Remaining extensions

General styled-label line-height configuration remains tracked separately in
`todo.md`; the existing `with_line_height()` setting applies to text areas.
Theme-file persistence also needs support for optional radius/color tokens and
`panel_corner_radius`. The new defaults and overrides work through the C++ API.
