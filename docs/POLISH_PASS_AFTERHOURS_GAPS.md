# afterhours gaps found during the 2026-08-29 UI polish pass

Ground rule for this pass: **do not change afterhours**, because Gabe is not
around to review library commits. Everything here is either worked around in
wm or left alone. This file is the review queue for later.

Format: what the library does, why it is wrong, and what wm did instead.

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

### `progress_bar` trips the library's own fill_parent lint

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

### `tree_view` indentation does nothing

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

### `expand()` resolves 2px taller than the space left for it

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

### The baseline threshold hides half-percent regressions

`compare_baselines.py` defaults to 1.0%. A full-width dialog button changing
colour is ~0.4%, so four screens had drifted from intended changes made days
earlier and the suite stayed green the whole time. Found only by rendering
unmodified HEAD and diffing it against its own baselines.

**Wanted:** a much lower default, or a per-screen tolerance that has to be
opted into with a reason.

### Headless capture did not run the app's own init

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

### Headless renders too few frames for time-based animation

Capture runs ~2 frames at dt=0.016, so any easing that starts at zero is caught
mid-transition and can never settle.

**Workaround in wm:** seed animated state at its resting value and put motion
behind interaction.

**Wanted:** either a "settle" pass before capture, or a documented way to ask
an animation for its final value.

---

## Glyphs the bundled fonts do not have

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

## ThemeDefaults and UIStylingDefaults are process globals with no scoping

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

## RenderCommandBuffer::sort() is dead code

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

## sokol_blend_test fails once it can compile

The sokol backend did not compile (missing `warn_once.h`, fixed in `bc6522b`),
which hid the fact that `sokol_blend_test` has a real failing check:
`logical left half is green across the 2x image`. 18/19 pass. Verified
independent of the focus ring change by reverting that change and re-running.

**Wanted:** someone with a Metal machine to look at HiDPI blending. Filed
rather than fixed because wm is raylib-only and cannot exercise it.

---

## Worked around, no library change wanted

Nothing yet.
