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

### Containers do not lay out flow children added by the caller

Two components hit this. A flow child added to a `decorative_frame` renders
*below* the frame rather than inside it, and the three children of a `popover`
all drew on top of each other instead of stacking.

Both have absolutely-positioned internals of their own, which appears to be
what breaks ordinary flow layout for anything the caller adds.

**Workaround in wm:** `popover_lab` wraps its contents in an explicit `vstack`,
which lays out correctly. `decorative_frame` could not be worked around that
way, so that screen switched to absolute positioning throughout.

**Wanted:** a documented content slot, or flow children that lay out normally.

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

### Focus rings draw inside the element, and only two edges

`UIComponent::focus_rect()` insets by its offset, so the ring lands inside the
widget rather than around it, which reads as "too small". Measured on
`self_align`: only the left edge (20px of 48) and the bottom (156px of 184)
render, colour (193,195,196), and only on the 1px falling outside the button.
No top or right edge at all.

Draw order does not explain it (background `rendering.h:1521`, ring `:1613`),
so the cause is still unknown.

**Wanted:** a ring that draws outside the widget bounds on all four edges.

Update: the border work above found the same shape elsewhere -- a container's
edge is drawn inside its own rect and children are not inset, so children
paint over it. Worth checking whether the ring's missing edges are the same
cause before hunting further.

### The baseline threshold hides half-percent regressions

`compare_baselines.py` defaults to 1.0%. A full-width dialog button changing
colour is ~0.4%, so four screens had drifted from intended changes made days
earlier and the suite stayed green the whole time. Found only by rendering
unmodified HEAD and diffing it against its own baselines.

**Wanted:** a much lower default, or a per-screen tolerance that has to be
opted into with a reason.

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

## Worked around, no library change wanted

Nothing yet.
