# Decide: sokol letter spacing (wm_afterhours, Tier 3)

**Problem.** On sokol, `measure_text` and `draw_text_ex` ignore their `spacing` argument, and the measure memo keys on spacing 1. So `with_letter_spacing` does nothing on sokol (floatinghotel, hanabi) and works on raylib. fontstash supports it: `fonsSetSpacing` adds pixels between glyph pairs, which matches raylib's `(n-1) * spacing`. The question is what sokol's *current* look should map to. The renderer passes `spacing = 1 + letter_spacing` because raylib's default gap is 1px. Today sokol draws as if spacing were 0.

## Options

**A. Honour `spacing` exactly, matching raylib.** `fonsSetSpacing(spacing * dpi)` in draw and measure, and the memo keys on spacing.
- Backends agree on the number.
- Cost: every sokol label gets 1px wider per glyph gap at the default letter spacing (~10–20px on a long label). floatinghotel and hanabi re-baseline every capture, and their hand-calibrated sizes move.

**B. Treat raylib's default 1 as sokol's 0.** `fonsSetSpacing((spacing - 1) * dpi)`.
- Nothing on sokol moves at `letter_spacing = 0`. `with_letter_spacing(n)` adds exactly n px on both backends, and measure and draw agree.
- Cost: the raw argument means different things per backend. A game calling `draw_text_ex(..., spacing = 0)` gets glyphs 1px *tighter* on sokol. Clamping at 0 would avoid that, but then negative letter spacing would not work.

**C. Honour only `HasLabel::letter_spacing`.** The renderer passes letter spacing to sokol separately, and backends keep their native default gap.
- The cleanest semantics, but it changes the `measure_text` / `draw_text_ex` signature or adds a parallel one. That is a bigger API change.

## Recommendation

**B, unclamped.** It is about 10 lines plus a sokol CPU-fontstash test like `font_atlas_measure_test`: spacing widens measure by `(n-1)*extra`, and measure equals `fonsTextBounds` with spacing set. It changes no existing sokol pixels, and it makes the feature work. Document in CHANGELOG that sokol's baseline is raylib's 1. Choose A instead if you want the backends identical and accept a re-baseline of floatinghotel and hanabi.
