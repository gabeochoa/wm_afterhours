# Decide: selectable read-only text (hanabi #37/#52, floatinghotel U8)

**Problem.** A label cannot be selected or copied. hanabi built ~230 lines of app-side selection for its transcript, and floatinghotel built its own for diffs, including selection that survives virtualized rows scrolling out of view. Afterhours already has the geometry (`text_selection.h`: `Selection`, `byte_offset_at`, `selection_rects`, `substring`) and a copy path, but only the editable widgets use them. `with_readonly()` exists on `text_input` and gives focus, select and Cmd+C without editing. `text_area` never reads `text_readonly`, and both widgets draw as fields.

Doing this well means deciding where selection state lives, how press and drag interact with hot/active and focus, how Cmd+C is consumed (the same `TextCopy` re-fire bug floatinghotel hit), what the highlight theme token is, and click counting. That is too much for one small PR, and it overlaps the input/focus work another agent owns.

## Options

**A. Per-label opt-in: `ComponentConfig::with_selectable_text()`**
- `UIContext` holds the one active selection: entity, anchor and cursor byte offsets into the label's source string. A press anywhere else clears it.
- Press, drag and shift-click seed and extend it through `byte_offset_at`.
- The renderer paints `selection_rects` behind the glyphs using a new `Theme::selection` colour.
- `TextCopy` copies `substring` and consumes the action.
- Adds `UIContext::selected_text()` / `has_selection()` and an I-beam cursor.
- Offsets come from `wrap_lines` source ranges, so this is **built on gap/wraptext** (#79).
- Double-click word / triple-click line reuses text_input's 0.4s multi-click code. That code should move into a shared helper rather than be copied.
- Covers hanabi's #37 completely. Does not cover cross-element selection (#52) or floatinghotel's virtualized source.
- Size: ~300–400 lines src plus a test and an example.

**B. A "plain" readonly text_area as the selectable label**
- Wire `text_readonly` into `text_area`, which is a bug fix anyway.
- Add a style that drops the field background, border and focus ring.
- Reuses the existing selection, copy, multi-click and keyboard code.
- Size: ~50–100 lines.
- Costs: every selectable paragraph becomes a focusable widget in tab order, with heavier per-entity state. A transcript of hundreds of turns pays that cost per turn, and it still does not survive virtualization.

**C. Source-level selection controller (floatinghotel U8)**
- A `TextSource` adapter with stable positions, bounded range reads and source→display mapping for tabs and soft wraps.
- A controller that owns caret, drag, edge autoscroll and copy limits independently of which rows are rendered.
- This is the complete answer for logs, diffs and transcripts, including cross-element selection when elements map to one source.
- Size: 800+ lines and an API design pass. floatinghotel already ships an app-owned version.

## Recommendation

**A**, after gap/wraptext lands, limited to per-element selection. Fold the `text_area` readonly wiring from B into it as a separate small fix, since that one is just a bug. Leave C until a second project needs virtualized selection, because floatinghotel's app-owned version works today.

Question for Gabe: is the per-element scope OK, and should selection state live on `UIContext` (one active selection) or on the entity?
