# File Tree local plugin

WM-local plugin (Batch 3 Lane I). Not afterhours upstream.

## Purpose

Reusable filesystem file-tree data source: directory scan / curated
tree, tree nodes, expand/collapse state helpers, file sizes,
selection lookup, and text preview loading — plus separable
icon/kind classification. Extracted behavior-preservingly from
`src/systems/screens/FileTreeShowcase.h`. All presentation (theme,
layout, `tree_view` rendering, focus, debug labels) stays in the
showcase.

## Public model API

Canonical headers (in this directory):

- `file_tree_source.h` — `include "plugins/file_tree/file_tree_source.h"`
  - `wm::plugins::file_tree::FileEntry { name, path, is_directory, file_size }`
    — one file/directory row.
  - `wm::plugins::file_tree::Node` — alias for
    `afterhours::ui::imm::TreeNode<FileEntry>` (the node `tree_view`
    consumes; no adapter copy).
  - `wm::plugins::file_tree::EntryKind { Directory, File }` —
    `classify(entry)`; `is_directory` / `is_file` /
    `is_expandable` / `should_show_size`; `extension_of(entry)`.
    Showcase keeps all drawing (folder vs file glyph) and only
    branches on these.
  - `wm::plugins::file_tree::project_root()` —
    `std::filesystem::current_path()` as string.
  - `make_dir(parent, name, kids)` / `make_file(parent, name, size)` —
    node builders (path join `parent + "/" + name`).
  - `synthetic_tree()` / `synthetic_tree(root)` — the curated
    "Project sources" tree (6 folders / 12 files: `src/engine`,
    `src/systems/screens`, `tests/e2e_scripts`, …) with real file
    sizes looked up from disk (`0` when missing).
  - `scan_directory(dir_path)` — one-level real-fs scan: skips
    dotfiles, records sizes for files (`0` on error), sorts
    directories-first then alphabetically.
  - Expand/collapse: `ExpandedSet = unordered_set<string>` —
    `is_expanded(set, id)`, `toggle_expanded(set, id)`,
    `expand_all(nodes, set)` (insert every directory path).
    The showcase passes `HasTreeViewState::expanded_nodes` straight
    in, so AH `tree_view` and the model share one set — no sync copy.
  - `lazy_load_children(node, expanded, use_real_fs)` — scan expanded
    real-fs dirs that have no children yet; recurse. No-op for the
    curated tree, files, and collapsed dirs.
  - `find_by_path(nodes, path)` — recursive selection lookup.
  - `count_nodes(nodes) -> { folders, files }` — counts every node.
  - `visible_rows(nodes, expanded) -> [{ entry, depth }]` — pre-order
    visible rows (a node is visible iff all ancestors are expanded;
    roots always shown).
  - `format_size(bytes)` — `"123 B"`, `"1.5 KB"`, `"2.0 MB"`,
    `"3.0 GB"` (1024-based, `%.1f`).
  - `relative_label(path)` / `relative_label(path, root)` —
    `"wm_afterhours/…"` for paths under the root, else unchanged.
  - Plugin facade: `wm::plugins::file_tree::plugin::descriptor`
    (`name`, `version`, `dirs_first`, `skip_dotfiles`,
    `preview_limit_bytes`).
- `file_tree_preview.h` —
  `include "plugins/file_tree/file_tree_preview.h"`
  - `wm::plugins::file_tree::preview::kLimitBytes` (128 KB).
  - `preview::Preview { path, status, lines }`.
  - `preview::load_text(path)` — read/cap/split a file (binary
    detection via embedded NUL, `\r` strip, tab→4 spaces; statuses:
    `"Could not open this file."`,
    `"Binary file / text preview unavailable"`,
    `"Preview limited to first 128 KB"`, `"Read only"`).
  - `preview::load_for_selection(entry*)` — nullptr/directory ⇒
    empty preview.

Callers must include `<afterhours/ah.h>` (or `src/external.h`)
before these headers (AH prelude convention, e.g. `rolling_number`).

## Showcase consumer

- `src/systems/screens/FileTreeShowcase.h` (`file_tree` screen) —
  the only consumer. It keeps `current_root`, `cached_roots`,
  `needs_refresh`, `use_real_fs`, `seeded_expansion`, the preview
  cache fields, the whole `for_each_with` render (header, toolbar
  Home/Up/Refresh/source toggle, path/status labels, `tree_view`
  + row decoration, preview pane, counts footer), and every
  `file_row_*` / `file_preview*` / `file_tree` / `path_label` /
  `status_bar` debug name. Its `FileEntry` alias, `make_dir` /
  `make_file` / `synthetic_tree` / `scan_directory` /
  `lazy_load_children` / `format_size` / `relative_label` /
  `read_preview` / `project_root` wrappers delegate to this plugin;
  the seeded-expand walk and the visible/count walk call
  `expand_all` / `visible_rows` / `count_nodes` on
  `state.expanded_nodes`.

## Tests

```bash
PKG_CONFIG_PATH=$HOME/homebrew/lib/pkgconfig make -j4
./output/ui_tester.exe --run-all-tests
./output/ui_tester.exe --e2e --test-script tests/e2e_scripts/44_file_tree.e2e --headless
./output/ui_tester.exe --e2e --test-script tests/e2e_scripts/199_file_tree_design.e2e --headless
./output/ui_tester.exe --e2e --test-script tests/e2e_scripts/292_file_tree_source_switch.e2e --headless
```

Unit test: `src/testing/tests/FileTreeSourceTest.h` (registered via
`src/testing/tests/all_tests.h`) builds a temp directory tree with
nested dirs/files, exercises `scan_directory` (dirs-first, dotfile
skip, sizes), `expand_all` / collapse visibility via
`visible_rows`, `count_nodes` / `find_by_path`, `format_size` /
`relative_label`, kind classification, and preview loading
(text, binary, missing, missing-selection). Targeted e2e:
`44_file_tree`, `199_file_tree_design`, `292_file_tree_source_switch`.

## Afterhours promotion gate (future candidate only)

No afterhours/vendor edits in this batch. Promotion candidate only
as a future afterhours file-tree data-source adapter on `tree_view`
— and only if consumers keep hand-rolling filesystem tree data
(i.e. a second consumer needs scan/expand/preview) — after this
plugin holds stable across the full local suite, the three file-tree
e2e scripts, and headless screenshot baselines. The adapter would
re-export this model against `tree_view` rather than shipping a
competing tree helper. Not claimed upstream-ready; any promotion is
a separate proposal with its own review.
