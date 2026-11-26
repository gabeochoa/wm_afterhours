# LetterboxLayout duplicate audit — WM-local vs afterhours

Branch: `wm/letterbox-content-viewport-diff` (finishes the audit opened in
PR #16, which landed the degenerate-size guard + `tests/letterbox_test.cpp`).
Verdict: **DIVERGENT overall — KEEP LOCAL. No caller migration.**

## What was compared

- WM: `src/systems/LetterboxLayout.h::compute_letterbox_layout(window_w,
  window_h, content_w, content_h)` — explicit-dims pure helper. Single
  production caller: `src/systems/RenderRenderTexture.h`, which feeds it
  `raylib::GetScreenWidth/Height` and `mainRT.texture.width/height` and draws
  `layout.dst` via `DrawTexturePro`.
- AH pure formula: `afterhours::graphics::letterbox_content_size(content_w,
  content_h, window_w, window_h)` + `graphics::window_centre_position`
  (`vendor/afterhours/src/window_helpers.h`, header-only, std-only, no
  global state) — directly callable from `tests/letterbox_test.cpp`.
- AH stateful API: `window_manager::content_viewport(window_w, window_h)`
  (`vendor/afterhours/src/plugins/window_manager.h:268`). It letterboxes
  against `ProvidesCurrentResolution::current_resolution` fetched from the
  entity singleton (`EntityHelper::get_singleton_cmp`), optionally refetched
  via `fetch_current_resolution()`, and ignores any render-texture size the
  caller actually presents. A direct comparison in the standalone test would
  therefore be stateful (needs singleton/entity setup) and would not test the
  dims the WM caller passes — it is documented here, not called in the test.

## Equivalence (valid inputs, all dims > 0): EXACT

For every valid case the WM helper and the AH pure pair agree on size,
offset, and bar split — including rounding and odd totals:

| case | window | content | WM dst | AH size | offset (both) | result |
|---|---|---|---|---|---|---|
| same-aspect | 1920x1080 | 1280x720 | 1920x1080 @0,0 | 1920x1080 | 0,0 | EQUIVALENT |
| pillarbox | 1920x1080 | 720x720 | 1080x1080 @420,0 | 1080x1080 | 420,0 | EQUIVALENT |
| letterbox | 800x600 | 1920x1080 | 800x450 @0,75 | 800x450 | 0,75 | EQUIVALENT |
| odd-round | 1000x1000 | 3x2 | 1000x667 @0,166 (bars T166/B167) | 1000x667 | 0,166 | EQUIVALENT |
| odd-round | 999x777 | 16x9 | 999x562 @0,107 (T107/B108) | 999x562 | 0,107 | EQUIVALENT |
| odd-round (2nd branch) | 1920x1080 | 1000x999 | 1081x1080 @419,0 (L419/R420) | 1081x1080 | 419,0 | EQUIVALENT |
| odd-round | 1x1 | 3x2 | 1x1 @0,0 | 1x1 | 0,0 | EQUIVALENT |
| odd-round | 5x5 | 3x2 | 5x3 @0,1 | 5x3 | 0,1 | EQUIVALENT |
| odd-round | 7x3 | 16x9 | 5x3 @1,0 | 5x3 | 1,0 | EQUIVALENT |
| odd-round | 1921x1081 | 1280x720 | 1921x1081 @0,0 | 1921x1081 | 0,0 | EQUIVALENT |

Why exact: both compute `dest_w = window_w`,
`dest_h = round(dest_w * content_h / content_w)` (double, `std::round`),
fall back to the height-bound branch identically, and centre with integer
`(window - dest) / 2` (WM: `bar = total / 2`, remainder to right/bottom;
AH: `window_centre_position = max(0, (display - win) / 2)` — same floor
split for the non-negative totals valid inputs produce). The full table is
printed by the test on every run.

## Divergence (exact divergent cases)

Non-positive inputs — the minimized-window defect PR #16 fixed:

| case | WM `compute_letterbox_layout` | AH `letterbox_content_size` | AH `content_viewport` |
|---|---|---|---|
| content 0x0, window 1920x1080 | empty layout, `dst` 0x0, bars 0 | `{1920, 1080}` (full window) | identity `dst` 1920x1080 (no/degenerate resolution) |
| content 1280x0, window 1920x1080 | empty layout | `{1920, 1080}` | identity |
| content -1x720, window 1920x1080 | empty layout | `{1920, 1080}` | identity |
| window -1x1080, content 1280x720 | empty layout | `{-1, 1080}` (negative passed through) | identity-ish `dst` -1x1080 |
| window 0x0, content 1280x720 | empty layout, `dst` 0x0 | `{0, 0}` (size coincidentally matches; no empty-layout/bar contract) | `dst` 0x0 |

Migrating the caller to either AH entry point would therefore change
degenerate-input behaviour: the render texture would be drawn stretched to
the full window (or with a negative rect) instead of as a no-op 0x0 draw,
reintroducing the pre-#16 minimized-window hazard.

## Why keep local (precise)

1. **Degenerate contract differs.** WM returns an empty layout for any
   `<= 0` dim; AH pure returns `{window_w, window_h}` verbatim and
   `content_viewport` returns the identity viewport. Preserving behaviour
   would require retaining the WM guard in front of any AH call anyway.
2. **The drop-in viewport API is singleton-coupled and ignores the caller's
   dims.** `content_viewport` letterboxes against
   `ProvidesCurrentResolution::current_resolution`, not
   `mainRT.texture.width/height`. `RenderRenderTexture` presents an RT whose
   size is an app-side choice; coupling it to the global resolution would
   couple presentation to whatever resolution the entity system currently
   holds (and `should_refetch` can even re-fetch it mid-frame). That is a
   new global-state dependency for a ~30-line pure helper.
3. **No net dedup.** Replacing the helper with `letterbox_content_size` +
   `window_centre_position` + a retained degenerate guard + bar derivation
   re-implements `LetterboxLayout` around upstream calls of equal size —
   the duplicate is the bar/empty-layout contract, which upstream does not
   provide. Keeping the local helper keeps that contract in one tested place.

## Change in this PR

- `tests/letterbox_test.cpp`: equivalence/divergence table vs the AH pure
  helper (valid cases asserted equivalent; non-positive cases pin the
  divergent AH return values so a future upstream change is noticed).
  Existing guard/same-aspect/pillarbox/letterbox assertions unchanged.
- `docs/LETTERBOX_AUDIT.md`: this audit.
- No caller, header, or behaviour change for valid inputs. Header note in
  `LetterboxLayout.h` already states the keep-local rationale; left as-is.

## How to run the test

Standalone compile mirroring the `makefile` flags (no dedicated make
target exists for `tests/*_test.cpp`):

```
PKG_CONFIG_PATH=$HOME/homebrew/lib/pkgconfig
clang++ -std=c++23 ... $(pkg-config --cflags raylib) \
  -isystem vendor/ -Isrc tests/letterbox_test.cpp \
  $(pkg-config --libs raylib) -framework CoreFoundation -framework OpenGL \
  -framework AppKit -framework IOKit -o /tmp/letterbox_test && /tmp/letterbox_test
```
