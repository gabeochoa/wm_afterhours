# afterhours gaps found during the 2026-08-29 UI polish pass

Ground rule for this pass: **do not change afterhours**, because Gabe is not
around to review library commits. Everything here is either worked around in
wm or left alone. This file is the review queue for later.

Format: what the library does, why it is wrong, and what wm did instead.

---

## Open

### Column and Row packing overlaps children under FlexStart/End/Center

`flex_alignment` puts three fixed-height boxes in a `vstack` with
`justify_content` set. Under `FlexStart`, `FlexEnd` and `Center` they draw on
top of each other, each advancing by noticeably less than its own height.
`SpaceBetween` and `SpaceAround` look correct, because the spacing they insert
is larger than the shortfall and hides it.

Measured with `pixels(30)` children as well as `percent(0.22)`, so it is the
packing, not percent resolution. The same overlap appears in the Row direction
on the cross-axis demo.

This one is worth taking seriously: it is not an exotic component, it is a
stack with a justify set, and any screen doing that has been quietly overlapping.

**Workaround in wm:** an explicit `with_gap()` on each demo container, which
spaces them far enough apart to hide it. That is a cover, not a fix.

**Wanted:** the advance to be the child's full outer height/width.

### `modal::fyi` styles its tertiary and its dismiss identically

`modal.h:641` and `:647` both pass `DialogButton::Neutral`, so a three-button
dialog renders two visually identical grey buttons. On `dialog_fyi` those were
"Don't Save" and "Cancel": one throws the user's work away, the other does
nothing, and nothing on screen distinguishes them.

**Workaround in wm:** the labels now carry the difference ("Discard changes" /
"Keep editing"), which helps but does not fix it. A destructive tertiary should
not look like a dismiss.

**Wanted:** a distinct style for the tertiary, or let the caller pass one.

### `context_menu` ignores border and roundness, and dims backwards

Passing `.with_border(...)` and `.with_roundness(...)` in the config has no
effect: the menu still draws as a square, edgeless slab. On a dark screen that
reads as painted onto the background rather than floating above it, which is
the one thing a menu needs to communicate.

Separately its disabled row is drawn *lighter* than its enabled rows, so
"Discard changes" looks hovered while the live items look inert. Disabled
should be dimmer, not brighter.

The separator is also a full-width 12px slab rather than a hairline, so it
reads as a broken row.

**No workaround applied:** all three are inside the component.

### `percent(1.0)` plus a margin overflows by the margin

`modal.h:701` sizes the prompt's text input `percent(1.0f)` and then gives it
`Margin{.left = medium, .right = medium}`. The percent resolves against the
full parent width and the margins are added on top, so the field starts after
its left margin and its right edge lands past the parent's.

Measured on `dialog_prompt`: modal spans x=430..910, field spans x=456..909.
26px of padding on the left, 1px on the right, which reads as the field having
fallen out of the dialog.

This is the same shape as the `toggle_switch` padding bug, so it is probably
one rule: percent sizing should resolve against the content box, after margins.

**No workaround applied:** the caller cannot configure the field.

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
