# Wordproc reference

These are historical consumer notes copied into WM, not a current Wordproc task list.
Original files remain in WM Git at `072a602a:docs/wordproc/<name>.md`.
Use Wordproc's own source and pin before changing it. Shared gaps belong in
[AFTERHOURS_GAPS.md](../AFTERHOURS_GAPS.md).

## UI direction

Wordproc combines Win95 structure with early Macintosh details: one readable UI
font, gray chrome, raised controls, sunken document fields, clear selection and
keyboard focus. Keep menus predictable, Help last, shortcuts visible, current format
state apparent and unsaved state named. Use consistent spacing, status text and
selection contrast; avoid color-only feedback and excessive stacked toasts.

The February audit covered toolbar icons/tooltips/separators, menu ordering and
mnemonics, title controls, document border/scrollbars, status labels, context menus,
ruler alignment/handles, responsive layout, motion and progress. Old scores and
completion claims describe those captures only.

## Migration boundaries

- Menu UI already builds afterhours entities; remove duplicate legacy painting only
  after checking open menus, accelerators, modal focus and outside dismissal.
- Status bar and ruler can compose existing UI/drawing APIs. Keep document units,
  page position, zoom and handle edits in Wordproc; test resize and selection updates.
- Remove unused legacy widgets first. Preserve titlebar/window behavior and the
  document renderer while migrating borders/dialogs. Compare both pixels and input.
- Shared candidates were split panes, docking, tree rows, popovers, command palette,
  tab strip, breadcrumbs, minimap, gutter, scroll decorations and fuzzy matching.
  Many now exist; verify current APIs rather than repeating missing-feature claims.
- Icons and custom drawing already have support. Access-key underlines remain a
  specific shared request. Art, document commands and formatting policy stay local.

## ECS proposals

Persist IDs/handles, not pointer graphs. A migration must preserve valid transient
references through the documented frame boundary, reject stale generations and test
undo/recreation. Query optimizations must preserve ordering/take semantics. Pools/SoA
need measured benefit and complete caller migration; they are not serialization prerequisites.

The tag RFC proposed lightweight bitsets and query predicates with app-owned typed
names. Define capacity, defaults, serialization and migration before changing marker
components. The separate cascade-delete plan remains unstarted.

Use the live [plugin API](../../vendor/afterhours/PLUGIN_API.md) and
[UI reference](../../vendor/afterhours/src/plugins/ui/README.md), not copied snapshots.
