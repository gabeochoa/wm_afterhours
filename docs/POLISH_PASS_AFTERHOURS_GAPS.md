# afterhours gaps found during the 2026-08-29 UI polish pass

Ground rule for this pass: **do not change afterhours**, because Gabe is not
around to review library commits. Everything here is either worked around in
wm or left alone. This file is the review queue for later.

Format: what the library does, why it is wrong, and what wm did instead.

---

## Decisions (2026-08-30)

Gabe reviewed every open entry and picked a direction for each. Rows marked
FIXED are done; the rest is the agreed plan, not a changelog.

| # | Gap | Decision |
|---|-----|----------|
| 1 | Theme/styling globals | **PARTLY DONE.** `ThemeDefaults` split into `app_default` (persists) + `theme` (reset each frame), and `UIContext::set_theme` added. Making raw `context.theme` assignment reach layout is blocked -- see the font-resolution entry below. |
| 2 | `RenderCommandBuffer::sort()` dead | **FIXED.** Tiebreak on primitive type removed, so scissors keep their position; sorting is behind `UIStylingDefaults::sort_draws_by_layer`, off by default. Covered by `draw_sort_test`. |
| 3 | `MIN_FONT_SIZE` clamp | Stop clamping. Add a **configurable warn floor** on the context/theme so an app sets its own (e.g. 16 at 720p) and gets warned rather than silently resized. |
| 4 | `with_wrap()` naming | **Delete it.** `with_flex_wrap(FlexWrap)` already exists; `with_wrap()` is a redundant shorthand. One caller in wm. |
| 5 | `gen_first_enforce` | **Add an explicit crash.** afterhours' own `log_error` is a non-crashing `fprintf` (and a no-op in the lean path), and wm's `assert(false)` is stripped under `NDEBUG`, so the empty-vector return is reachable. |
| 6 | Nine-slice inset twice | **FIXED.** Renderer inset dropped; wm's 19 `with_text_inset(0, 0)` opt-outs deleted, baselines byte-identical. |
| 7 | `progress_bar` trips its own lint | **FIXED.** The label is a flow child now, not absolute, so the always-on warning is gone (4 -> 1 across wm). At 100% the bar now draws one box instead of two. |
| 8 | Non-ASCII glyph warning | **FIXED**, and `font_has_glyph` was broken too (`GetGlyphIndex` falls back to `?`, not 0, so it said yes to everything). Found a real missing fullwidth glyph in wm's Korean copy. |
| 9 | Screen swap from click handler | **FIXED.** `UIContext::defer` runs work at the top of the next frame. wm's `pending_pick` field and drain block are gone. |
| 10 | Baseline threshold 1.0% | **FIXED**, both suites. 1.0% -> 0.05%. It was hiding real drift: 76 of 108 screen baselines and 12 e2e baselines had crept sub-threshold during this session's library work. |
| 11 | `tree_view` indentation | **FIXED** by moving the row's label into a child, so the existing padding indents it. The general fix (padding insets an element's own label) was tried and reverted: 16 -> 72 warnings, because `Dim::Text` sizes to the label without reserving padding, so every hugging label overflows by construction. |
| 12 | `expand()` 2px | **FIXED.** Not padding: grid snapping rounded to nearest, so `expand()` rounded up past the space. Snaps down now. |
| 13 | `children()` measures short | **FIXED**, same cause as #12. Snaps up now, so it always contains its children. |
| 14 | `toggle_switch` label width | **FIXED.** The hand math existed to dodge the #12 expand() bug; with that fixed, `expand()` gives byte-identical output. |
| 15 | `toggle_switch` `\|`/`O` glyphs | **ALREADY FIXED** in `0ea239f`. The indicators are `std::optional` and opt-in; nothing hardcodes a glyph. Entry was stale. |
| 16 | `text_input` ambient theme | **FIXED** as `ThemeScope`, an RAII guard rather than a config field: children are built by their own calls after the parent's config is gone, so the theme has to be live for a scope. |
| 17 | Headless animation frames | **FIXED.** The knob already existed (`animation::set_instant`) but the UI's declarative animations ran on a second integrator that ignored it, and wm never called it. Both now honour it. |
| 18 | Metal re-init | **Fail loudly on a second `init()`** until someone root-causes it on a Metal consumer. |
| 19 | Stale entries | **DONE.** headless-init superseded by #1; glyph section is now a font-assets note; #15 was already fixed. |

---

## Open

### WITHDRAWN: Column and Row packing overlaps children

I recorded this as a library bug. It is not one, and the evidence was in the
warning I had already captured:

    'Start_2' in parent 'Start_inner' - NoWrap set but would overflow
    (child_size=26.0, offset=52.0, container=43.6)

Children at 0, 26 and 52 with a size of 26 means the advance equals the child
exactly. Packing is correct.

What actually happened on `flex_alignment` is that the container was 43.6px for
78px of content, so `NoWrap` stacked the children past its edge, and because
they are rounded rectangles sitting flush against each other the row read as
one scalloped blob rather than three boxes. Undersized container plus zero gap,
not a layout bug.

The `with_gap()` calls in that screen are still the right fix for how it looks,
but they are a design choice, not a workaround.

### FIXED: composites dropped the caller's border and roundness

`apply_inheritable_from` copied colour, font and alignment but not
`roundness`, `corner_radius` or `border_config`, so a border set on a
`context_menu` never reached the panel. Measured on `context_menu_lab`: no
border pixel on any of the four edges. Fixed in afterhours `aad267f`.

Two things fell out of it, both worth knowing:

- Adding the fields means every *child* inherits them too, which sprayed a
  1px border onto every menu row and turned the 8px separator into a solid
  slab. A composite now needs `without_border()` on its internals. CSS does
  not inherit border or radius for exactly this reason.
- Even on the panel alone, only the bottom edge rendered. A container's
  border is drawn inside its own rect (`rendering.h:1551`) and **layout does
  not inset children by the border width**, so any child at `percent(1.0)`
  paints over three of the four edges. `menu_list` works around it with
  padding equal to its own border width.

**Wanted:** border width subtracted from the content box during layout, so
the workaround is unnecessary. This is likely the same root as the focus-ring
"only two edges render" entry below.

### FIXED: a disabled menu row read as a gap, not a row

Measured on `context_menu_lab`: the disabled row came out `(23,31,42)`
against a panel surface of `(28,34,48)` -- 5/255, invisible. Since enabled
rows paint opaque Primary over the whole panel, surface is only ever visible
in the separator band, so a disabled row was pixel-indistinguishable from a
fat separator.

Cause is `Theme::disabled_variant` (`theme.h:367`) applying two dimming
operations at once: it mixes 70% toward the background *and* drops alpha to
`disabled_opacity`. Nothing survives both.

Fixed menu-locally in `aad267f` with an opaque underlay behind disabled rows.
The shared cause is untouched, so every other disabled control in the library
still nearly vanishes rather than dimming.

**Wanted:** `disabled_variant` picking one dimming operation, not both.

### Every font is clamped up to 16, silently

`rendering.h:404` is `font_size = std::max(explicit_font_size, MIN_FONT_SIZE)`
with `MIN_FONT_SIZE = 16`. A screen asking for a 12px caption gets 16px drawn
into a box that layout measured for 12, and then the renderer warns that the
text overflows a container the caller sized correctly.

Two costs. The showcase had no working small-text tier at all -- every caption
rendered at body size. And it was the single largest source of overflow
warnings: dropping the floor took the count from 99 to 65.

**Worked around in wm:** built with `-DAFTERHOURS_MIN_FONT_SIZE=12.0f`, which
is the floor the validation config already enforces.

**Wanted:** the floor to default to something that is not larger than sizes the
library's own `TypographyScale` hands out, or an auto-downscale rather than a
silent clamp plus a warning.

### The nine-slice text inset is applied twice

`component_init.h:104` sets `text_inset` to the slice size so the label clears
the frame art. `rendering.h:2225` (and `:1649` on the immediate path) has
already inset the text rect by that same slice. So the slice is subtracted
twice: a 70px box with a 16px slice keeps 6px for a 15px font.

**Worked around in wm:** `ExampleNineSliceBorders` passes an explicit
`with_text_inset(0, 0)`, which is honoured because the default is guarded on
`!config.text_inset.has_value()`. Every label on that screen got legible again.

**Wanted:** pick one of the two subtractions.

### MOSTLY FIXED: progress_bar tripped the library's own fill_parent lint

`progress_label` was `percent(1,1)` plus `with_absolute_position()`, which is
exactly the pattern `component_init.h:690` warns about, and it fired for every
caller. It did not need to be absolute: it is the track's only flow child, so
it covers the track either way, and `render_layer` already puts it above the
fill. wm went from 4 of these to 1, with byte-identical baselines.

**The 100% case**, which I had left as a residual and Gabe pointed at directly:
at full the fill covers the track exactly, so the component was drawing two
identical boxes. Colour the track and skip the fill. That removes the overdraw
and the last lint warning together, and needs no computed-size dependency.

Still true and worth doing separately: the lint's message is "may not reference
the expected parent" but it never checks whether the parent is explicitly
sized, which is the condition that makes the pattern dangerous.

(original)

`imm_components.h:2185` and `:2197` build `progress_fill` and `progress_label`
as `percent(1.0)` plus `with_absolute_position()`, which is exactly the pattern
`component_init.h:692` warns about. Four showcase screens log it and no caller
can suppress it, because both elements are internal.

**Wanted:** either the overlay pattern exempted from the lint, or the component
sized some other way.

### FIXED: containers did not lay out flow children added by the caller

Filed as one bug across two components. It was one bug in one of them, and the
shared explanation ("both have absolutely-positioned internals, which appears
to be what breaks flow layout") was a guess that turned out to be wrong.

**`popover` no longer reproduces.** Removing the `vstack` workaround from
`popover_lab` and parenting the caption and four buttons straight to the panel
lays them out correctly: y = 132, 160, 196, 232, 268. Some later change fixed
it. The `vstack` stays in that screen because it supplies the panel padding,
not because it is a workaround; the comment saying otherwise is gone.

**`decorative_frame` did reproduce**, and the cause was mundane rather than
anything to do with absolute internals. All three styles built layer 0 as a
*flow* child sized to the whole frame, then layers 1-6 as absolute. A caller's
flow child was laid out after a sibling that had already consumed the entire
main axis, so it landed just past the bottom edge:

    frame_0      y=150  h=268
    frame_outer  y=150  h=268   <- flow, full size
    probe_0      y=418          <- 150 + 268

Layout said so at the time and nobody read it:
`Layout wrap: 'probe_0' in parent 'frame_0' - offset=268.0, container=268.0`.

Fixed in `e98cdc3`: layer 0 is absolute like the other five. Frame rendering is
byte-identical (108/108 baselines), it just stops eating the flow.
`DecorativeFrameShowcase` now parents its card to the frame instead of
recomputing the frame's geometry onto absolutely-positioned siblings.

### FIXED: tree_view indentation did nothing

The row set `Padding::Left(indent_px)` and put the label on the same element.
Padding offsets an element's *children*, not its own label, so every depth
rendered flush left and `FileTreeShowcase` spelled depth into the string as
leading spaces. Fixed by making the label a child of the row, which the
padding already indents. wm's hack is gone.

**Tried and reverted:** the general form -- an element's padding insets its own
label -- fixes this and reads like CSS, but it took wm from 16 to 72 layout
warnings across 11 screens. A `Dim::Text` element sizes itself to its label
without reserving room for padding, so once rendering subtracts the padding
every hugging label overflows by construction. Doing it properly means teaching
`Dim::Text` sizing about padding first; that is a much larger change and is not
filed as blocked so much as not attempted.

(original)

`tree_view.h:67-79` computes `indent_px = depth * indent_width` and applies it
as `Padding::Left(pixels(indent_px))` on the row button. It has no effect on
where the label draws.

Measured on `file_tree` with a three-deep sample tree: every directory row, at
any depth, put its first ink at x=172, and every file row at x=190. That 18px
is the `"v "` / `"  "` arrow prefix, not padding. All depth information was
lost, so a nested tree rendered as a flat two-level list.

Probable cause: the label is drawn from the row rect using `text_inset` rather
than from the padded content box, so container padding never moves it. Worth
checking whether `Padding::Left` moves a label on any `button`, since that
would be the general version of this bug.

**Workaround in wm:** `FileTreeShowcase` spells the depth into the label as
leading spaces, computed from the path. Because `tree_view` builds the row as
`arrow + label`, the arrows stay in a left gutter and only the names indent.

**Wanted:** row padding that actually offsets the label, so callers do not have
to encode layout in their strings.

### `toggle_switch` draws literal `|` and `O` glyphs on its track

`imm_components.h:1313` and `:1326` put a `"|"` label on the left of the track
and an `"O"` on the right, so a pill toggle always has a stray character beside
its knob. At small sizes the `O` reads as a zero.

This is every toggle in the showcase, and it is a good part of why the toggles
were called ugly. Screens that look better here, like `parcel_corps_settings`,
avoid it by hand-rolling their own pill instead of using the component.

**No workaround applied:** `setting_row_toggle` calls `toggle_switch`
internally, so a caller cannot suppress them.

**Wanted:** the glyphs off by default, or a config flag. A plain track with a
knob is the conventional look.

### `toggle_switch` computes its label width by hand

`imm_components.h`, `toggle_switch`. The label is sized
`parent_width - track_w`, with a comment saying this dodges "expand()
resolution issues inside absolutely-positioned containers".

Two problems. The workaround itself forgot horizontal padding, so the track
rendered exactly `padding.left + padding.right` past the right edge (fixed in
`70cd11c`, before this ground rule was set). And the reason it exists at all is
that `expand()` does not resolve inside an absolutely-positioned parent, which
is the real bug. Any composite that lays out by hand will hit it again.

**Wanted:** `expand()` that resolves against an absolutely-positioned parent's
resolved size.

### FIXED: focus rings rendered under the widget, so only two edges showed

Measured on `self_align`: only the left edge (20px of 48) and the bottom
(156px of 184) rendered, and only on the 1px falling outside the button.
No top or right edge at all.

It was draw order after all. The first pass here checked `RenderImm`
(background `rendering.h:1521`, ring `:1613`, correctly ordered) and concluded
draw order was innocent, without checking that wm registers
`register_batched_render_systems` on all 7 call sites and never runs
`RenderImm`. In `RenderBatched` the ring was emitted *before* the fill and
relied on `layer + 199/+200` to sort above it -- but
`RenderCommandBuffer::sort()` is never called, so insertion order is paint
order and the ring went under the widget. Only the slivers rounding outward
past the fill escaped.

Fixed in `2581244` by emitting after fill and border, matching `RenderImm`.
352 changed pixels on focus became 4200. See the dead-`sort()` entry below.

**Lesson for the next one of these:** confirm which render path the app
actually runs before reasoning about draw order. The two diverge.

**Still wanted:** `focus_rect()` insets by its offset, so the ring lands
inside the widget rather than around it. A negative `focus_ring_offset`
outsets it, but the default of `+4.0f` reads as "too small".

### The non-ASCII warning never asks the font

`backends/raylib/font_helper.h:73` warns on the first byte >= 0x80 in any
string, without consulting the `raylib::Font` it was just handed. `language_demo`
loads NotoSansKR and Sazanami and draws its Korean and Japanese samples with
them, and still gets warned twice. The function has everything it needs to
check -- `GetGlyphIndex` on the decoded codepoint -- so the warning could be
accurate instead of advisory.

### `with_wrap()` is flex wrap, not text wrap

`with_wrap()` sets `flex_wrap`, which does nothing for a label. Text wrapping
is `with_text_overflow(TextOverflow::Wrap)`. Cost an hour here: adding
`with_wrap()` to an overflowing label changes nothing and gives no hint why.

**Wanted:** a name that does not read as the text one, or a warning when a
label-only element sets flex wrap.

### FIXED: expand() overflowed and children() under-contained, both from grid snapping

Filed as two separate layout bugs with unknown mechanisms. They are one bug.

`snap_to_8pt_grid` (`autolayout.h`) rounds to **nearest** and runs on every
non-`Pixels` dimension *after* it was computed to exactly fill or exactly
contain. So a computed 98 rounds up to 100 and overflows its parent by 2, and a
content sum of 105 rounds down to 104 so the box is shorter than the child it
exists to hold. Snapping is cosmetic alignment; it must not break containment.

Fixed by giving the snap a direction: `Dim::Expand` may only round **down**,
`Dim::Children` only **up**, everything else still rounds to nearest. Repro and
regression coverage in `tests/sizing_repro_test.cpp`, which also needed
`layout_only(grid_snap, resolution)` on the harness -- neither bug reproduces
with snapping off, which is why they stayed unexplained.

Note the old entry text below was wrong about the cause (it guessed padding);
kept for the symptom description only.

### (original symptom) expand() resolves 2px taller than the space left for it

On `deadspace_settings`, a root holding `main_area` (`expand()`) plus a
70px prompt bar put the bar at 652+70 = 722 on a 720 screen. Shrinking the bar
to 66 moved it to 656 -- `expand()` simply absorbed the difference and kept
the overflow at exactly 2px. Only an explicit fraction on `main_area` fixed it.

Possibly the same rounding as the nine-tab `tab_container` case, where each
content-fit tab rounded up and the row summed to 1105 against a 1104 parent.

### `children()` can measure short of the children

`text_input`'s `body` is `children()` tall and resolved 12px shorter than the
fields inside it, so the last field escaped. Trimming the field margins shrank
`body` by the same amount, so it never converged -- the only fix was an
explicit height. Same shape as the `decorators` badges, where a
`children()`-sized box came out 12px tall for a 13px font.

### FIXED: the baseline threshold hid half-percent regressions

Both thresholds are 0.05% now (`compare_baselines.py` and
`screenshot_validation.cpp`), down from 1.0%. On 1280x720 that is ~460 pixels
rather than ~9200.

The scale of what it was hiding, measured before changing it: **76 of the 108
screen baselines and 12 of the e2e baselines had drifted** during this
session's library work, every one of them under 1% and therefore reported as
passing. Each individual change had been verified as "108/108 passed", which
turned out to mean only "nothing moved more than 9200 pixels".

All baselines were refreshed and are now byte-exact, and re-running the suites
twice produces no diff at all, so the tighter bound does not flap. A screen
that genuinely needs slack should be named in the manifest rather than the
default being raised again.

(original)

`compare_baselines.py` defaults to 1.0%. A full-width dialog button changing
colour is ~0.4%, so four screens had drifted from intended changes made days
earlier and the suite stayed green the whole time. Found only by rendering
unmodified HEAD and diffing it against its own baselines.

**Wanted:** a much lower default, or a per-screen tolerance that has to be
opted into with a reason.

### FIXED: headless capture did not run the app's own init

Superseded by the ThemeDefaults/UIStylingDefaults globals entry, which is the
general form of this: the fix is no longer "call the app's init from capture"
but "restore the styling globals before every screen".

(original)

`headless_screenshots.cpp` built its own singletons and never called what
`Preload::make_singleton` does, so the committed baselines were rendered
without the app's theme colours, text inset, default font or grid snapping.
56 of 108 screens changed the moment the two were made to share a setup
function, and grid snapping alone accounted for every one of them.

Fixed in wm by extracting `apply_ui_styling_defaults()`. Worth knowing because
it means any baseline captured before that commit was never evidence of what
the app actually drew.

### `text_input` inherits the current screen's theme

Persistent chrome drawn over themed content changes colour per screen. The
navigator sidebar's filter box rendered green on `simple_button`.

**Workaround in wm:** pin explicit colours on the filter box.

**Wanted:** a way to opt a subtree out of the ambient theme.

### Swapping screens from a click handler is a use-after-free

`load_screen` frees the system that `ScreenCyclerSystem` is still iterating.

**Workaround in wm:** record the target index in the handler and drain it after
`systems.run` returns.

**Wanted:** a deferred-teardown path, so a widget callback can safely ask for a
screen change.

### `gen_first_enforce` returns `values[0]` on an empty vector

`entity_query.h:485-491` logs an error for the empty case and then indexes
anyway.

**Wanted:** return the optional/throw rather than reading element 0.

### FIXED: headless rendered too few frames for time-based animation

Two things were wrong, and the first hid the second.

`animation::set_instant` already existed, and its comment names this exact
caller: *"e2e wants screenshots of the settled state"*. wm simply never called
it. But calling it changed nothing, because the UI's declarative animations
(`with_animation(Anim::on_appear()...)`) run through a **second** integrator in
`plugins/ui/animation_config.h` -- `spring()` and `ease()` -- which knew nothing
about the flag. Two animation systems, one knob.

Both honour it now, and wm sets it on all four headless capture entry points.
`AnimationDeclarativeDemo` dropped its workaround and animates from 0.3/0.0
again; captured settled, it is a full-size vivid tile instead of the muted
95%-scale one the old baseline had frozen mid-flight.

**Still hand-rolled:** `AnimationSpringDemo` integrates its own spring physics
from a `trigger_time` float rather than using either system, so it keeps its
"four seconds in the past" seed. Not reachable by the flag.

**Not applied to e2e.** The e2e runner does not set the flag: some scripts step
frames deliberately, and 113/113 pass without it. Worth revisiting if an e2e
screenshot ever catches an animation mid-flight.

(original)

Capture runs ~2 frames at dt=0.016, so any easing that starts at zero is caught
mid-transition and can never settle.

**Workaround in wm:** seed animated state at its resting value and put motion
behind interaction.

**Wanted:** either a "settle" pass before capture, or a documented way to ask
an animation for its final value.

---

## FIXED: text_input resolved its font size through the wrong units

`text_input` took `field_cmp.font_size.value` and re-wrapped it in `pixels()`,
throwing the Dim away. An `h720()` font size carries the 20/720 fraction, so it
resolved to **0.028px**.

`MIN_FONT_SIZE` hid the drawn text, but the caret and selection geometry are
computed from the unclamped value, so a selection collapsed to a blob over the
first letter and the caret sat in the wrong place. Visible on `text_input_lab`
and `dialog_prompt` the whole time.

`resolve_to_pixels` itself was fine; the caller stripped the units.

---

## FIXED: the MIN_FONT_SIZE clamp, and what it was hiding

The clamp on explicit font sizes is gone, which is the rest of gap #3.

It could not be removed before because `sync_scroll_lab` lost every row label.
The cause was two guards, `rendering.h:796` and `:2263`, that skipped the draw
when `result.rect.height < MIN_FONT_SIZE`, under a comment reading "Don't
attempt to render if font size is effectively zero". The floor is not zero. Any
text below it was silently not drawn, and the clamp was the only thing keeping
that from happening.

Worse, the guard tests the *positioned height*, not the font size, so it fired
even with the clamp in place: `sync_scroll_lab` has been shipping with every row
label and its entire group-2 table invisible, and the committed baseline
recorded that as correct.

Both guards test for effectively zero now. Four screens changed, all gaining
text or rendering it at the size actually requested.

---

## WITHDRAWN: row flex broken with expand() children

floatinghotel reports that in a Row, an `expand()` child takes the full parent
width instead of what fixed siblings leave, so `[status(16px) | filename]`
pushes the status letter onto its own line. It does not reproduce.

`sizing_repro_test` covers both shapes they name. A plain div Row gives the
expander 184 of 200 with a 16px sibling. Their exact case, a `button` with Row
children, gives 148, and 148 is correct: the button has 36px of padding and
`expand()` fills the content box, not the border box. I asserted 184 there
first and was wrong for the same reason they may have been.

In neither case does the fixed sibling wrap. The test asserts that directly
rather than a width, since the width depends on padding nobody should have to
recompute.

Their doc predates the grid-snapping fix, which is the likeliest thing to have
changed it.

---

## Three more cross-project reports that do not reproduce

Checked against current afterhours with tests, not by reading.

**floatinghotel: div backgrounds render opaque.** Fixed. This is the D1 sokol
bug word for word: `sgl_defaults()` loaded a pipeline with blending off, so
every alpha byte was discarded. Blending is on, and `sokol_blend_test` case 4
asserts exactly their complaint, that a translucent div background is not
filled opaque. 13/13.

**kart: checkbox internal layout overflow.** Does not reproduce. A 240x32
checkbox with a label gives two 120x32 children in a 240x32 row, fitting
exactly. Covered in `sizing_repro_test`. kart already notes they stopped using
`imm::checkbox`, so their report may predate a fix.

**floatinghotel: row flex broken with expand() children.** Withdrawn, see its
own entry above.

The pattern across this survey: most of what the other repos have filed is
already fixed, and their pins are what is stale. Worth checking a report
against current afterhours before working it.

---

## Cartographer's e2e handlers, and what the library already does

Their report is that command handlers must be registered per `SystemManager`,
and a missed one makes the command silently do nothing.

The library already ships `register_unknown_handler`, whose message names this
exact cause: a custom handler registered after the builtins, or a command
defined but not registered. So the diagnostic exists; it just has to be
registered in each manager, which is the same per-manager burden the report is
about.

**Wanted:** a command registry that is not per-SystemManager, so registration
happens once. That is a design change to the e2e plugin, filed rather than
attempted. Their second ask, mouse delta through the action mapping system, is
the same shape: a real gap needing a design, not a fix.

---

## imm::slider overflows on paper and is corrected by the shrink pass

kart-afterhours reports `slider_text` and `slider_background` each overflowing
`slider`, 462 occurrences per run. The arithmetic is exactly as they describe:
with a label, `slider_text` asks for `0.5 * config.size.x` and
`slider_background` for `0.95 *` the same width, both in a `FlexDirection::Row`,
so the children ask for 1.45x the parent.

What the report does not say, and what an attempted fix showed, is that
`solve_violations` then shrinks both proportionally. The label lands at ~34.5%
and the track at ~65.5%, which is what every wm baseline encodes and what the
sliders look like. wm logs no slider overflow at all.

Giving the track the other half (`scale_x(0.5f)` when not compact) removes the
paper overflow and **makes it look worse**: the track shrinks to half the row
with a gap after it, because 0.5/0.5 is not the 34.5/65.5 the shrink pass was
producing. Reverted.

So this is warning noise plus arithmetic nobody chose, rather than a visual
defect. Any real fix has to pick the split deliberately and rebaseline every
slider, instead of making the numbers sum to 1.0 and accepting whatever falls
out.

**Fixed since:** the track, handle and label called `with_color_usage`
unconditionally, which sets the usage and leaves `custom_color` populated and
unread, so an inherited colour was accepted and ignored. They fill in the theme
usage only when the caller gave no colour. `inherit_from` forwarding no colour
or border was the other half of that report and is already fixed.

`on_draw_bg`/`on_draw_fg` are still not inherited, and should not be: a
composite would run the caller's callback once per internal part.

---

## FIXED: apply_overrides dropped half of ComponentConfig

43 of 86 members were never merged, so any element with a registered styling
default silently lost half its configuration. kart-afterhours reported ten of
them, found because an opacity-and-translate slide-in did nothing on styled
buttons; enumerating every member against the merge found the other 33.

Safe to fix because almost nobody registers a default: of the twenty repos that
vendor afterhours, four call `set_component_config`, cartographer's two are
empty `ComponentConfig{}`, and the rest set only size, padding, colour and
background, all of which were already forwarded. wm registers none, which is
why the first ten-field fix produced zero baseline diff.

**The list is still the defect.** A `static_assert` on `sizeof` now fires when
the struct grows, verified by adding a field and watching it break the build.
That is a reminder, not a fix: a set-flag per field, written by the `with_*`
setters, would make the merge a loop over flags and an omission impossible.
Filed as the TODO above the assert.

Two other kart reports checked and already fixed upstream: `with_opacity`
painting a dark box (`ad0390a`, scales alpha instead of replacing it) and
`disable_rounded_corners` giving a rounded focus ring (fixed here, it was live
on four wm screens).

---

## Size loses its units at the boundary, in at least three places

Three separate bugs, one shape: a `Size` gets reduced to `.value` and handed to
something expecting pixels. `Dim::ScreenPercent` stores a *fraction*, so
`h720(20)` is `0.0278`, and the result is a silently tiny number rather than an
error.

- `text_input` font size: re-wrapped `.value` in `pixels()`. Resolved to
  0.028px, which `MIN_FONT_SIZE` hid for the drawn text while the caret and
  selection stayed wrong. Fixed in `b3c94ba`.
- `with_gap`: three call sites passed `.value` to a float overload. A
  `h720(10)` gap became 0.014px, so the caller saw no gap. Reported by
  kart-afterhours, fixed.
- `translate_x`: see below, blocked on the axis question.

**Wanted:** the float overloads of `resolve_pixels` are the trap. Anything
holding a `Size` should pass the `Size`. Worth auditing every `.value` that
feeds a pixel computation.

---

## ScreenPercent does not know which axis it is relative to

`h720(px)` and `w1280(px)` both return `Dim::ScreenPercent` and differ only in
what they divided by, so at resolve time they are indistinguishable and the
caller has to supply the matching screen dimension. `layout_types.h:175` says so
outright: "given a screen dimension (height for h720, width for w1280)".

`component_init.h:462` resolves **both** translate axes against `screen_height`.
For `h720()` that is right. For `w1280(100)` on x it yields 56px at 1280x720,
and `screen_pct(0.5)` on x yields 360 instead of 640.

Reported by kart-afterhours as "`with_absolute_position()` resolves x against
the screen height". Confirmed, but the one-line fix (pass width for x) just
inverts which spelling is broken, because the type cannot say what it meant.

**Wanted:** an axis on `ScreenPercent`, set at construction, so `h720` is
Y-relative and `w1280` is X-relative and neither depends on the call site
passing the right dimension. That is a change to `Size`, so it is filed rather
than attempted.

---

## Gaps other projects filed that are still open here

Surveyed the ten gap docs across the twenty repos that vendor afterhours.
Most of hanabi's ranked top ten is already fixed upstream; its pin (`428047e`)
predates the fixes. Still live, verified against current afterhours:

| source | gap |
|---|---|
| kart | `GetFontDefault()` returns an invalid font headless |
| cartographer | e2e command handlers must be registered per SystemManager, and a missed one fails silently |
| cartographer | mouse delta cannot go through the action mapping system |

hanabi's `afterhours_gaps.md` is 14,792 lines and 251 numbered entries, with its
own triage index. Anyone working upstream should start from
`hanabi/afterhours_gaps_index.md` section 1 rather than this file.

---

## CHECKED: the progress bar was the only composite drawing a hidden box

The 100% progress bar painted a full-width fill over a track it covered
exactly, and nothing caught it because the pixels were right. The obvious
question is how many other composites do that, so the audit went from six
widgets to 22: every composite that stacks boxes, plus the edge states, since
100% was the state that had the bug.

**Answer: none of them.** All 22 clean, including 0% and 100% progress, both
circular ends, both checkbox and toggle states, and slider at 0, mid and 1.
The 0% bar already skips its fill rather than emitting a zero-width one.

The first run said otherwise, and it was worth more than the clean one:

- **39 "zero area" reports, all text.** The capture backend records text as a
  zero-width rect because it has no font to measure with
  (`backends/none/drawing_helpers.h:54`). A placeholder, read as a measurement.
- **3 radio_group "fully hidden" reports.** A filled circle under a
  `rectangle_rounded_lines` of the identical rect. The occlusion check tested
  alpha and called any opaque draw a filler, but an outline paints its border
  and leaves the middle alone. It hides nothing.

Both are now excluded by name in the detector, so the audit reports what it can
actually see rather than what it can merely list.

---

## Grid snapping: three fixes that measured worse than the bug

Every inconsistency below is real, and every attempt to remove one cost ~40 of
108 baselines while the equivalent caller-side fix cost 1. Recorded so the next
person does not spend the afternoon rediscovering it. **Measure the baseline
count before believing a snapping change is an improvement.**

| tried | why it looked right | what it cost |
|---|---|---|
| stop snapping the position accumulator | the comment beside it already claims it does not | 45 baselines, closed 4px of a 6px gap |
| snap `Dim::Percent` down | a share of the parent should not round past it, same as `Expand` | 42 baselines, fixed 1 screen a 1-line change also fixed |
| skip position snapping for `Dim::Pixels` | sizes already skip it, so positions disagree with sizes | 40 baselines, and `virtual_list_lab` went to **43% drift** |

That last one is the instructive one. Rows declared `pixels(36)` skip size
snapping, because an explicit pixel size means that number, but the running
offset is snapped anyway, and 36 is not a multiple of the 8px unit at 1440p, so
every row rounds up and the eighth lands 28px low. Making the accumulator
consistent fixes the arithmetic exactly and turns a 10,000-row list from evenly
spaced into alternating 24 and 28, which reads as banding.

**Even spacing is what snapping the accumulator buys, and it is worth more than
honouring each size exactly.** The inconsistency is load-bearing.

The caller-side fix is to size on the grid: `h720(36)` is 36 at 720p and 72 at
1440p, both multiples of their unit, and drifts at neither.

**Where that still is not enough.** `h720` snaps to nearest, so at 1366x768 a
4% larger screen makes a 36px row 40px, an 11% jump. Nine of them plus a footer
no longer fit a panel that grew 2%. powerwash still fails there. An `expand()`
spacer makes any drift fatal regardless of headroom, since it fills the parent
exactly and then the drift pushes the next sibling out.

---

## FIXED: parents measured children at a size the children did not draw

A child snaps its own size later in the layout pass, so a parent that reads
`child.computed` gets the number from before the snap. Both the sizing and the
placement did.

Found from the other end: at 1080p the deadspace tab strip put its last tab
outside the row. Eight `expand()` tabs drawn 114 wide, placed 120 apart, so
the strip measured 954 in a 948 row. It only shows above 720p, where the grid
unit (which scales with screen height) is bigger than the rounding that hides
it.

The same read in `_sum_children_axis_for_child_exp` made a `Dim::Children`
strip 120 wide around ten segments that drew 156.

Both now go through `snapped_extent`, which asks for the size the child will
end up with.

**Two things I tried and backed out**, because the screenshots disagreed with
the reasoning:

- *Dropping the remainder distribution.* Handing expand children whole grid
  units can make two equal-weight siblings differ by one unit, which looked
  like the wrong trade until `guess_who_lab` went from 2.8% to **19.5%**
  drift: without it the slack piles up at the end, an eight-card grid comes up
  47px short and the gaps between cards collapse. A row that is not a whole
  number of units cannot be filled by equal grid-aligned children. Take the
  one-unit difference.
- *Not snapping the position accumulator.* It matches what the comment there
  already claims, and it closed 4px of the residual. It also moved **45 of 108
  baselines**, against 1 for the targeted fix. Not worth it for 4px.

**The residual is arithmetic, not a bug.** A 2px margin has no representation
on a 4px grid, so snapped positions still run a little past the sum. The wm
side fixed that by scaling the margin (`w1280(4)`), which lands on the grid at
every resolution because the grid unit scales the same way.

---

## FIXED: the containment check called every wrapped label a violation

`assert_within_parents` measured the whole label on one line and failed it if
it exceeded the container. That is what wrapping is for, so every wrapping
element was a false positive: fighter_menu's card description wraps onto two
lines, fits, and was reported as needing 364px in a 250px box.

Ellipsis was already skipped. Wrap now gets the check that actually applies to
it -- the longest single word, which really cannot fit -- rather than a skip.

---

## FIXED: a capped box wrapped against the width it was not going to keep

hanabi #136 asked for `fit_content(max)`: hug the text, cap at a maximum, wrap
past it. That is a chat bubble, and they reported nothing in the library sized
a box to its own text.

Three quarters of it already existed. `Dim::Text` hugs, `with_max_width` caps,
`TextOverflow::Wrap` wraps, and combining them gets the width right: a short
label sizes to 17px, a long one stops at the 200px cap.

The height did not follow. Both bubbles came out one line tall, because
wrapping measured against `computed[Axis::X]` and `apply_size_constraints`
runs *after* text sizing. So the long label wrapped against its own unclamped
700px, found it fit on one line, and got a one-line height on a box that was
then cut to 200. The text spilled out the bottom.

The fix reads `max_size[Axis::X]` at measure time and wraps against the width
the box will actually end up with. The long bubble now measures 80px, five
lines. No new sizing mode: `fit_content(max)` is those three calls.

**wm sees no pixel change** -- no screen combines all three -- so this is
covered by `sizing_repro_test` rather than a baseline.

---

## FIXED: two measure functions answered different questions

On sokol, `measure_text` returned `bounds[2]-bounds[0]`, the box around the
painted pixels, while `measure_text_internal` beside it returned the advance.
Both are called "measure text". Layout and the shared `TextMeasureCache` went
through the ink one.

Ink is narrower than the pen travel by a side bearing each side, so a box sized
to its own text clips it, a trailing space measures as nothing, and summing
runs to wrap a line drops the interior bearings and comes up short. Typesetting
is done in advances; ink is for cropping a glyph.

hanabi #137, and the reason they could not adopt the cache the library ships
for exactly their case: switching to it moved every chat bubble 2px.

**Raylib differs for an unrelated reason.** There `measure_text_internal` is
`raylib::MeasureText`, which takes no font argument and measures the default
face whatever font the text will be drawn in. Its only caller is the debug
autolayout overlay's hover hitbox, which does draw in the default font, so it
is not wrong there, only misleadingly named.

**wm cannot see this change.** It is raylib, so all 108 baselines are unchanged.
floatinghotel and hanabi will shift about 2px and need a rebaseline where
someone can look at the result.

---

## FIXED: nothing asked whether a widget was inside its parent

`assert_no_overflow` measured every element against the **viewport**, so a row
overflowing a panel in the middle of the screen passed it. hanabi reported that
on "many buttons are going outside the bounds" it named 1 of 55.

`assert_within_parents` adds the content-box comparison. Under a new name
rather than inside `assert_no_overflow` as hanabi asked: that command means the
viewport in every script in every project that calls it, and wm's own
responsive audit says so in its header. Quietly changing what an assertion
asserts is worse than a second name.

It skips a scrolling or clipping parent, an absolutely positioned child, and
the root, since overflow is the point in all three, and uses a looser tolerance
than the viewport check because the content box is rect minus padding and each
term rounds separately above 1x scale.

**It immediately found real bugs in wm**, both mixed-unit:
- `casual_settings`' footer group was sized for three buttons plus two gaps,
  while each button also carried a 5px left margin, so the row was 234 wide in
  a 232 box.
- Its menu columns were `pixels(285)` holding `with_720p_size` buttons. At
  1920x1080 the buttons grew to 420 and the column stayed 285, so five buttons
  hung 135px outside it, on screen the whole time.

Both fixed, covered by `36_parent_containment` at 720p and 1080p.

**Not fixed, and the reason this is opt-in:** the other settings screens have
the same shape. `powerwash_settings`' `main_panel` is 570 tall in a 380 content
box at 1080p. Mixing `with_720p_size` children with fixed-pixel parents comes
apart at any scale above 1, and wm has that in several places. That is a
focused responsive pass, not a gap fix, and the assertion is ready for it.

---

## FIXED: text editing was opted into by enumerator name, silently

Twelve features, `if constexpr (enum_contains<InputAction>("TextWordLeft"))`
and friends, all through `text_input/component.h`. An `InputAction` that does
not carry the name compiles the feature out: no error, no warning, nothing to
grep. hanabi lost word editing for the life of the project and reported it as
"alt-backspace never landed".

The library already validated five widget action names this way and warned
about them. The twelve text ones were never checked. They are now, reported
together at init and naming exactly which features stopped existing.

They stay optional rather than becoming errors, because an app with no text
input should not be lectured twelve times.

wm's enum is complete, so the warning is silent here. That means it was
unexercised, so `text_action_names_test` pins both directions: `DefaultAction`
carries all twelve, and a widgets-only enum is detected as missing all twelve.

---

## DONE: audited composites for draws nobody can see

`overdraw_audit_test` renders progress_bar (full and half), toggle_switch,
checkbox, slider and button, and reports any draw whose rect is fully contained
by a later opaque draw on the same or a higher layer. All six are clean now
that the progress bar is fixed.

It reports rather than asserting a count, deliberately: a border under a fill
is occluded on purpose, so a number would either be wrong or need constant
updating. The list is the value.

**It found a different bug on its way.** The audit died on `checkbox` with
`map::at: key not found`, because `FontManager::get_active_font` warned that a
font was missing and then called `fonts.at()` anyway, and `get_font` did the
`.at()` with no check at all. A wrong font name took the app down instead of
drawing in the wrong face. Same shape as `gen_first_enforce`: log the problem,
then do the thing that fails. Both fall back now.

**Not covered:** the per-component draw-call counter that would catch a
regression as a number. Composites built from `div` rather than a named
component are also not enumerated here.

---

## Font assets (not library gaps)

Not library bugs so much as a trap the library walks into. Several components
use an ASCII letter as a stand-in for a symbol and expect the symbol font to
remap it. wm points `__symbol` at `Gaegu-Bold.ttf`, an ordinary handwriting
face, so the letter is what gets drawn.

- `ComponentConfig::DEFAULT_CHECKBOX_CHECKED` is `"V"`, so every unconfigured
  checkbox shows a floating capital V. Callers can override with
  `with_checkbox_indicators`, and the wm screens now pass `"[x]"` / `"[ ]"`,
  which reads as a checkbox in any font.
- `toggle_switch` hardcodes `"|"` and `"O"` on its track, with no override.
- Some faces draw `>` as a closing paren, so `"->"` renders `"-)"`. Two screens
  had that in copy; both now avoid the arrow.
- Non-ASCII punctuation draws as `?`. Em dashes and middots in labels have all
  been replaced; they are fine in comments.

**Wanted:** either a symbol font that actually maps these, or components that
draw their indicators rather than spelling them.

## FIXED: ThemeDefaults and UIStylingDefaults were process globals with no scoping

`ThemeDefaults` now separates `app_default` (persists) from `theme` (reset each
frame), `UIContext::set_theme` writes both the context and the frame slot, and
`PublishContextTheme` pushes the frame's theme to layout before it runs. A
per-screen theme applies to sizing as well as colour, and dies with the frame.
`ThemeScope` does the same for a subtree.

`UIStylingDefaults` is still a plain global; wm restores it per screen.

(original)

Both are singletons that any screen can write and none restores. Layout reads
`ThemeDefaults::get().theme.text_inset` and `ui_scale` directly
(`systems.h:430-434`), not the caller's `context.theme` -- and the dataflow is
one-way: `context.theme = theme_defaults.get_theme()` every frame
(`systems.h:249`), so a screen assigning `context.theme` never feeds back, while
a screen calling `ThemeDefaults::get().set_theme(...)` changes the process
until something changes it again.

The effect measured in wm: **32 of 108 screens rendered differently depending on
which screens had rendered before them.** `AutoTextColorShowcase` calls
`set_theme(ocean_navy())` behind a once-per-process static; `ocean_navy()`
carries the compile-time default `text_inset` of 0 rather than wm's `{5, 0}`, so
every screen after it laid out with a different inset than it would alone. Same
string, same font, same size measured identically (39.5px both ways) -- the
divergence was entirely downstream of measurement.

This also made `30_themes` pass only inside the full e2e suite and fail on its
own, and it is the real cause of the "casual_settings and layout render
nondeterministically" note: not nondeterminism, order dependence.

**Fixed in wm** by snapshotting both globals at startup and restoring them
before each screen builds (`restore_ui_styling_defaults()`, called from all four
screen-load paths). Re-applying was not enough: `apply_ui_styling_defaults()`
only patches individual colours onto the current theme and cannot undo a
whole-theme `set_theme()`. After the fix all 108 screens render identically
alone and in sweep, all 113 e2e scripts pass both ways, and the layout/overflow
warning count dropped 34 -> 16.

**Wanted:** either per-screen scoping for these defaults, or layout reading the
active context's theme rather than the global. A caller currently cannot tell
that assigning `context.theme` does not affect its own layout.

---

## WITHDRAWN: layout and rendering resolve fonts differently

I filed this as the blocker for #1 and repeated it several times. It is wrong.
`Theme::get_font_config` has no callers outside `theme.h`, so the language map
is never consulted by rendering and the two paths do not disagree about fonts.

What actually broke `toasts` when the theme was published to layout: the
screen's own theme carries `text_inset = 0` where the app's is `{5, 0}`, so its
`Dim::Text` buttons measured 8-12px narrower -- and `Dim::Text` was snapping to
*nearest*, landing below the string it was measured from. The third case of the
grid-snapping bug, not a font bug. Fixed in `73ae84a`, after which #1 landed
with no extra warnings.

Recorded because the wrong diagnosis survived three retellings before anyone
measured it.

(original)

Blocks the rest of gap #1. Layout measures a label with the font the element
names (`with_font(DEFAULT_FONT, pixels(28))`); rendering resolves the font
through the *theme's* `language_fonts` map. While the theme reaching layout and
the theme reaching rendering were different objects, the two happened to agree.

Publishing the context's theme to `ThemeDefaults` before layout -- the obvious
way to make `context.theme = t` affect sizing -- makes them disagree for any
screen whose theme sets `language_fonts`. On `toasts` the buttons are
`Dim::Text` sized and ask for `DEFAULT_FONT`, so layout measures that while
rendering draws `"Gaegu-Bold"` (a different FontManager key for the same file),
and four labels overflow boxes that were measured correctly for the font the
caller asked for.

So it is not that the screens were wrong and the change exposed it: the change
introduces the mismatch. Reverted the publish step; kept the `app_default` split
and the setter, which fix the leak without touching font resolution.

**Wanted:** one font-resolution path. Either an explicit `with_font` wins over
the theme's language font in both places, or both consult the language map.
Until then a theme cannot safely drive layout.

---

## FIXED: RenderCommandBuffer::sort() was dead code

Never called, so every `layer` the batched collectors passed to the buffer was
decorative -- which is how the focus ring shipped invisible. It also tiebroke on
primitive type, which would have moved a scissor off what it clips the moment
anyone called it.

Sorts by layer only and stably now, behind
`UIStylingDefaults::sort_draws_by_layer`, off by default. `draw_sort_test`
covers ordering, stability within a layer, and the scissor case.

(original)

`sort()` (`render_primitives.h:457`) is never called from anywhere.
`BatchedRenderer::render` walks `buffer.commands()` in insertion order and
reads `cmd.layer` only for capture attribution. So every `layer` argument
passed to a `buffer.add_*` inside the batched collectors is decorative:
insertion order is paint order.

The focus ring shipped broken on exactly this assumption for the whole life of
the batched path. It was emitted at `layer + 199/+200` before the background
fill, and therefore drawn under it (fixed in `2581244` by moving the emission).
Nothing else currently depends on the layer argument, but the next person to
reach for it will hit the same wall.

**Wanted:** either wire `sort()` up or delete it and stop passing `layer` to
the buffer. Wiring it up is not free: it tiebreaks on primitive *type* within a
layer, which would reorder `ScissorStart`/`ScissorEnd` away from the geometry
they bracket and scramble text-vs-fill order for every widget.

---

## The Metal backend cannot be re-initialised

`graphics::shutdown()` followed by `graphics::init()` in the same process hands
back a render target that silently never receives any draw. Readback is uniform
magenta `(255, 0, 255, 255)`.

Nothing reports an error. Probed immediately before the failing draw, the second
session is byte-identical to a working one: `sg_isvalid() == 1`, image state
VALID, same pool ids (`img=65539`, `sgl_ctx=65538`), `sgl_error().any == 0`. The
capture path is not caching stale Metal state either -- it queries
`sg_mtl_device()` and `sg_mtl_command_queue()` fresh on every call.

Isolated by bisecting the conditions:

| condition | result |
|---|---|
| the block as the process's only `init()` | passes, correct pixels |
| same block after `shutdown()` + `init()` | uniform magenta |
| same, `hidpi = false` | uniform magenta (so not hidpi) |
| same, two frames before capture | uniform magenta (so not first-frame) |

This surfaced as `sokol_blend_test` failing `logical left half is green across
the 2x image`, which pointed at HiDPI and the logical-vs-physical projection.
Both are innocent: that block passes in a fresh process. The failure lands on
whichever assertion happens to run after the re-init.

**Worked around**, not fixed: the D19 hidpi assertions moved to
`tests/sokol_hidpi_test.cpp`, which owns its process and calls `init()` once.
Both binaries pass. The re-init bug itself is untouched.

**Wanted:** either re-init made to work, or `init()` after `shutdown()` made to
fail loudly instead of returning a target that renders nothing. Left for
floatinghotel, which is the sokol/Metal consumer; wm is raylib-only and cannot
exercise a fix.

---

## Worked around, no library change wanted

Nothing yet.
