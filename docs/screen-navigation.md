# Screen navigation

In an interactive demo, click **Browse screens (`)** or press backtick to open
the screen browser. The current screen pauses while the browser is open.
Escape, backtick or **Close** returns to it. Enter or a row click opens the
selected screen; moving the cursor does not reload it.

Search matches names, categories and descriptions without regard to case.
It searches collapsed categories too. Clearing search restores their saved
collapse state. Up/down moves through visible rows. Left moves to a category
or collapses it; right expands it or enters it. Left/right still edit the text
when the search field has focus. Tab can return to the search field.

The white row is the browsing cursor; `*` identifies the currently loaded
screen. The detail area describes the cursor's destination. Category headings
appear once, including when several results match. Related component categories
are grouped under Component Galleries; Bug Reports joins System Demos.

Comma/period and Page Up/Page Down retain sequential cycling while the browser
is closed. They do not switch screens while typing in its search field.

Verification: the standalone `tests/screen_tree_test.cpp` covers deduplication,
category grouping, collapsed search, no-match states and cursor operations.
`155_screen_tree.e2e` drives search, both activation paths, collapse/expand,
Escape, Close, current markers, and 720p/1080p. Complete runtime cycles in both directions visited
all 117 registered destinations exactly once before wrapping.

The registry already provided unique destination names; no duplicate destination
was found in that cycle. The previous search did repeat category headings per
matching row. That repetition is fixed without deleting intentional screens.
