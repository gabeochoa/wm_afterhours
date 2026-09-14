# Research and historical proposals

These notes replace old implementation sketches and copied inventories. They are
ideas to recheck against current code, not missing-feature claims or approvals.
Active work is in [todo.md](../todo.md) and [gaps](AFTERHOURS_GAPS.md). Original
specifications remain at WM `072a602a:docs/<old-name>.md`.

## Component ideas

Historical proposal numbers identify the original `docs/NN_*.md` in Git. Check the
current component inventory before work; many base widgets below have since shipped.

| Original | Idea or remaining design question |
|---|---|
| 01, 07 | Break styling-default dependency cycles; make config conflicts, disabled state and value-control tabbing explicit. |
| 02 | Corner brackets, patterned backgrounds and blockquotes with attribution. |
| 04 | Icon meters and stacked/dual progress bars. |
| 06 | Typed font size, opacity, percent and dimensions; preserve units and const correctness. |
| 09 | Numeric input with validation, bounds, +/- steps and optional drag adjustment. |
| 10 | Tooltip delay and anchored rich popovers with edge flipping, dismissal and keyboard behavior. |
| 12 | Accordion expansion, optional single-open policy, animated height and keyboard toggling. |
| 13 | Avatar shapes/fallback initials/groups/status; badges with counts, variants and overlay placement. |
| 14 | Alerts with severity, dismissal/actions; empty states with title, explanation and recovery. |
| 15 | Styled text, inline icons and optional effects; preserve localization and unsupported-tag fallback. |
| 16 | Configurable/custom-path focus rings whose paint and bounds match the component. |
| 17 | CJK ranges, symbols, glyph metrics, explicit font configuration and inheritance. |
| 18 | Action-to-icon registry with text fallback, mirrored pairs and caller-owned artwork. |
| 19 | Simpler includes, reusable configs, sizing/position helpers and working examples. |
| 20 | Stable `mk` identity, scoped/string IDs and collision diagnostics naming the call site. |
| 21 | Breadcrumbs, navigation/link states and collapsing long paths. |
| 22 | Filtered combobox, fieldset/label/help/error association, ratings and segmented controls. |
| 23 | Gradient fills, text effects, opacity inheritance and shadows with renderer parity. |
| 24 | Shared menu items for context/dropdown menus, shortcuts, submenus and predictable dismissal. |
| 25 | Selectable lists and tables with headers, sorting, resizing and virtualization. |
| 26 | Color selection, alpha, typed values and consistent preview. |
| 28 | Useful diagnostics, bounds/hover/tree inspection and profiling. |
| 29 | Reference scaling, aspect ratio, breakpoints and responsive layouts/fonts. |
| 30 | Translation-aware sizing across a chosen language set, coverage policy and per-language fonts. |
| 31 | Edge drawers/sheets with focus, dismissal, transition and available-space rules. |
| 32 | Spinners, skeletons and loading/progress overlays; reduced-motion behavior. |
| 33 | Image fit, video boundary, icons and carousel navigation. |
| 34 | Validated date/time input and calendar localization; game calendars stay app-owned. |
| 35 | File metadata/display, drag/drop and portable open/save/folder requests. |
| 36 | Reusable header/footer/hero/card composition with clear hierarchy. |
| 37 | Split and resizable panes, min/max constraints, collapse and drag feedback. |
| 38, 39 | Grid tracks/spans and drag/drop with live previews, valid targets and keyboard alternatives. |
| 40 | Text-editing shortcuts and search-input clear/filter behavior. |
| 42 | Shape primitives, stroke/fill/arrowheads, resize/rotate/hit testing, z-order and text-wrap geometry. |
| 44 | Portable rendering/platform APIs with injectable test providers and explicit unsupported results. |
| 46 | Typewriter/dialogue, world anchors/progress, inventory, menu/status/shortcut UI, timeline/minimap/tool palette, canvas/layers and styled documents. |
| 47 | Detect silent failures with broken-build checks, correct collection ownership and real render/clip evidence. |
| 49 | Native screen-reader roles, focus, announcements and platform verification. |
| 51 | Optional declarative layout DSL: typed C++ bindings, error locations, IDs, debugging and reload need evaluation. |

## Cross-project review

September 12 covered 38 other top-level projects plus `armchair_coach/puzzle`.
September 13 re-inventoried 38 directories, found 2,116 native paths and searched
1,897 app-source files across 19 projects. Focused reads covered floatinghotel,
Hanabi, kart, puzzle, cartographer, Wordproc, MyNameChef and pharmasea. This was not
an exhaustive line-by-line or runtime audit, and the refresh did not repeat web-app review.

Existing APIs already cover binding lookup, atomic files, profiling hooks, grids,
tabs, progress, tooltips, toasts, viewport mapping and much of E2E. New requests
became UP-01–22. Keep gameplay, save schemas, Git refresh policy, transport and art
in consumers. Presence in a newer library is distinct from adoption at an older pin.

## Other design references

- [egui](https://github.com/emilk/egui): virtual scrolling, scroll-to, reusable widget
  responses, popup placement, scene pan/zoom, undo and demo isolation. Recheck existing
  implementations before extracting an old missing-widget list.
- [Fabula](https://github.com/jasudev/FabulaItemsProvider): sheets, expandable text,
  image comparison, corner resize, scroll effects, animated indicators, joystick,
  hierarchy visualization and fold transitions. Keep content-specific demos in WM.
- [Dannaway design systems](https://www.adhamdannaway.com/blog/design-systems/design-system-examples):
  semantic tokens, readable type, spacing rhythm and complete workflow examples.
  The supplied article text was reviewed; external article images were not retrieved.
- [sf-windows](https://github.com/chrstph-gg/sf-windows): review remains blocked by the
  source loader's input policy. No claims about its implementation or license.
- Accessibility references: [APG](https://www.w3.org/WAI/ARIA/apg/),
  [WCAG 2.2](https://www.w3.org/TR/WCAG22/), [A11y Project](https://www.a11yproject.com/checklist/).
  Native semantics require platform adapters; ARIA is a web API. APCA research does
  not replace WCAG 2 contrast requirements or establish conformance.

## Keyboard and accessibility contracts to verify

Buttons activate with Enter/Space; checkbox/switch with Space; grouped radios/tabs
use arrows with an intentional Tab policy. Sliders expose steps and bounds. Modal
focus stays inside the top dialog and returns to its opener; menus and comboboxes
need local Escape, selection and typeahead rules. Tooltips must not trap focus.
Disabled discoverability and disabled activation are different policies.

Potential diagnostics include missing names/roles, contrast, target size, focus
visibility/order, inaccessible composites, nondismissable modals, color-only states,
reduced motion, live announcements and clipping. Verify a validator actually runs
before claiming coverage; [current gaps](AFTERHOURS_GAPS.md) identify known dead flags.

## Older architecture plans

- Layout QA: pair screenshots with layout intent and compact findings; distinguish
  model limitations from defects. The current CLI/testing guide supersedes MCP plans.
- Text editing: retain modifier-aware selection, clipboard isolation, undo, readonly,
  caret scrolling and pointer placement. Shared rendering/edit geometry is the open direction.
- Entity pools: reset recycled components/IDs, preserve handle invalidation and iteration
  semantics. Reuse capacity only after profiling allocation cost.
- UI/file-tree refactor: hide implementation helpers in detail namespaces, keep composites
  on public APIs and offer tree customization without child-order dependence.
- Old prioritized backlogs and code snippets are historical estimates, not a second todo list.

Asset requests covered navigation/action/status/file/game icons and missing font variants.
Check `resources/` and its attribution files before adding art; do not replace licensed
assets or claim a glyph is available without checking the loaded font.
