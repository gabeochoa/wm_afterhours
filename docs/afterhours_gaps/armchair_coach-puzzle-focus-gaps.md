# afterhours gaps: menu focus + keyboard nav

Reported by **puzzle** (broadcast_operator), raylib backend, 2026-08-10, against
afterhours `eb56ae0`.

Context: we built one shared focus ring and section-scoped keyboard navigation
across three menu screens. Almost every piece started life as a workaround for
something in the library. This is what blocked each one, what we changed, and
what we deliberately did not.

Unlike the other consumers listed in afterhours' `todo.md`, **we patched
vendor** — the changes are sitting uncommitted in the submodule for review. Each
one has a repro in `tests/downstream_gaps_test.cpp` (D33-D38) that fails without
it.

## Fixed, with the app-side workaround now deleted

### D33 — `focus_ring_thickness = 0` did not disable the ring

The only way to ask for no ring. The batched renderer routes `0` to
`render_rounded_outline_batch`'s `else` branch (`thickness > 1.0f` is false) and
draws a 1px line anyway; both renderers drew the contrast outline
unconditionally regardless of thickness.

We set `0` to opt out and shipped a doubled ring — our own, plus a faint amber
line inset 2px — without noticing for a day. Nobody would notice; that's the
problem.

*Fix:* bail out of the ring block when `thickness <= 0`.

### D34 — the immediate renderer drew the ring under the widget's fill

`RenderImm` emitted the focus ring *before* the `HasColor` fill, and
`focus_rect` is inset inside `draw_rect` — so any opaque background painted over
it completely. That is every default `button()`. `RenderBatched` was fine
(layer+199/+200), which is why this survived: the two renderers had drifted.

*Fix:* move the block below the fill and border. Also extracted
`detail::focus_ring_for()` so the ~95 duplicated lines can't drift a fifth way.

### D34b — the ring couldn't be positioned or made visible on a filled widget

Three separate things, all of which pushed us into hand-painting:

- `focus_rect(int rw)` insets, and `focus_ring_offset` was truncated with
  `static_cast<int>` — so a sub-pixel *outset* (we wanted 1.5px, to give a 22px
  toggle the same presence as a full-width row) was unreachable. The theme
  field is a float; the comment said an outset was impossible; a negative value
  actually worked. *Fix:* `focus_rect(float)`, drop the casts, correct the
  comment.
- The contrast outline was drawn outside the ring only, so an amber ring on an
  amber-filled widget (an active segment, a selected row, APPLY) vanished.
  *Fix:* draw the contrast edge on the inside as well — the two-tone ring the
  code comment already claimed to be.
- The ring's `FocusClusterRoot` fallback scanned 2 levels while
  `ComputeVisualFocusId` climbs 64. *Fix:* deleted; `visual_focus_id` already
  resolves clusters.

### D35 — hover-follow silently overwrote keyboard focus

The one that actually shipped a broken feature. `ComputeVisualFocusId` runs
*after* every user system, and under `FollowsMostRecentInput` it re-grabs focus
for whatever the cursor is over. So a game that moves focus during its build has
that move undone in the same frame whenever the cursor happens to be resting on
a widget — and on a menu, the cursor is always resting on a widget. Our arrow
keys did nothing on the level list. There was no suppression hook, no
input-source tracking; `set_focus` was a bare setter.

*Fix:* `FocusSource { Grab, Pointer, Explicit }`, recorded by `set_focus`
(defaulting to `Explicit`) and reset each frame. Hover-follow defers to an
`Explicit` claim. The `Grab`/`Explicit` distinction is load-bearing:
`try_to_grab` calls `set_focus` every frame, so treating that as intent would
disable hover-follow permanently.

Also fixes `HandleFocusUICommand` (the e2e `focus_ui`), which was being undone
the same way.

### D35b — `moved_this_frame` was an exact float compare

No dead zone, so a hi-DPI position rescaled into render space could jitter in
the last mantissa bit and read as intent. And with no cursor at all the position
is NaN — `NaN != NaN` is true, so it reported movement every frame forever.

*Fix:* a squared-distance test against `MousePointerState::move_dead_zone_px`,
with both non-finite cases handled explicitly. Worth stating plainly: a plain
distance test is *also* wrong, because one NaN frame then poisons `prev` and it
reports no movement forever. NaN has to be decided, not left to fall out of a
comparison.

### D36 — arrows were welded to Tab

`process_tabbing` assigns `WidgetDown` into the same `forward` that
`WidgetNext` sets, so no app can have "arrows move within a group, Tab moves
between groups". The documented escape hatch, `AcceptsValueInput`, had three
references in the entire library — all declarations. Nothing ever attached it
and there was no config setter.

The shipped casualty: **a Column tray cannot be arrow-navigated at all**,
because `process_tabbing` consumes `WidgetDown` nine systems before
`HandleTrayNavigation` looks for it. Dropdown option lists are Column trays.

Caveat worth stating: `process_tabbing` guards every action with
`if constexpr (magic_enum::enum_contains<InputAction>(...))`, so this only ever
bit apps whose `InputAction` actually declares `WidgetUp`/`WidgetDown`.
wm_afterhours does (`src/input_mapping.h:7-8`).

*Fix:* the component is now `ConsumesDirectionalInput` (with `AcceptsValueInput`
kept as an alias so the existing consumers keep compiling), exposed as
`ComponentConfig::with_consumes_directional_input()` and set by `tray()`. The
rename is the point: a tray stepping its children is not adjusting a value, and
the "value" framing is exactly why nobody wired the hatch up for eight months.
Plus `theme.arrows_tab` (default true) so an app with its own directional nav
can turn the folding off wholesale instead of racing `process_tabbing` for a
single-slot `last_action`.

**There is no `ValueUp`/`ValueDown` action pair.** Navigation and value
adjustment are the same two keys, and this component is the only thing telling
them apart. If dedicated value actions are ever added they collide with it, and
silently, because `process_tabbing` resolves actions by name at compile time.

### Tray selection desync

Tray children are force-marked `SkipWhenTabbing`, so the hover and click focus
paths can't reach them: clicking an item left `selection_index` on the previous
one and the next arrow press jumped somewhere unrelated.

*Fix:* `HandleTrayNavigation` follows `hot_id` when the mouse moves or clicks.

### e2e: `tab` / `shift_tab` worked headless and silently failed windowed

`test_input::push_key` used a queue with a **global one-consumer-per-frame
latch**, while `InputSystem` polls each action three times per gamepad id. So a
queued key was visible to whichever caller asked first and invisible to everyone
else — fine in a minimal example, silently dead as soon as a second binding, a
hotkey, or a controller exists. `input_injector::consume_press` is explicitly
multi-reader and documents this exact hazard.

We hit it as: Tab assertions passing headless while the feature did nothing in
the real app.

*Fix:* `push_key` routes through the injector; the queue stays for `push_char`,
where FIFO order is the point. One behaviour change: the injector's press lands
a frame later, so `input_injector_mouse_delta_test`'s interleaving test was
rewritten (the property it protected is now true by construction).

Separately, `register_ui_commands` has no caller **inside afterhours** — but it
has five downstream (`floatinghotel/src/main.cpp:352`,
`kart-afterhours/src/e2e_integration.h:76`, `wm_afterhours/src/game.cpp:852` and
`.../testing/e2e_integration.h:40`, `wordproc/src/testing/e2e_integration.h:56`).
So it is load-bearing, not dead: wire it into the e2e setup entry point or
document it as required, but **do not remove it**. (An earlier draft of this doc
said "zero callers repo-wide" — that was scoped to afterhours only and would
have been a dangerous thing to act on.)

### Backend: outlines were invisible to the test harness

`draw_rectangle_rounded_lines[_ex]` were `@notimplemented` stubs in the `none`
backend. Every outline in the library — borders *and* the focus ring — recorded
no draw call, which is why none of the above had test coverage. *Fix:* record
them. This is the change that made D33/D34 testable at all.

## Impact on the other consumers

Surveyed all 19 projects under `~/p/` that vendor afterhours. **Nothing breaks
today** — every one pins a fixed SHA (7 distinct SHAs), so all exposure lands on
each project's next bump.

**Verified, not predicted** — `wm_afterhours` at its pinned `14222a3`, with this
patch applied in a scratch copy (their working tree was left untouched; it has
484 lines of uncommitted split-pane work in the same files):

| | before | after |
|---|---|---|
| `tray_vertical_navigation` | **FAIL** — "Expected visual focus on 'V-Beta', but visual focus is on 'H-Alpha'" | **PASS** |
| rest of the suite | 11 passed | 12 passed, 0 failed |
| `validate-screenshots` (81 baselines) | 73 pass / 8 fail | 73 pass / 8 fail, same 8 files |

The Down key was tabbing out of the vertical tray to the horizontal one, exactly
as predicted. The 8 screenshot failures are **pre-existing baseline drift, not
ours**: identical file list before and after, and this patch moves only three of
them, by ≤0.0014 percentage points (`forms` 1.9833→1.9841, `hstack_showcase`
1.4362→1.4376, `parcel_corps_settings` 2.2099→2.2103) — a handful of pixels from
the new inner contrast edge. Nothing crosses the 1% threshold because of us.

**Why this survived so long:** `wm_afterhours/src/testing/tests/all_tests.h`
includes only `FontConfigTest.h`. `TrayTest.h`, `TabbingTest.h`,
`RadioGroupTest.h`, `TextInputSelectOnFocusTest.h` and three others sit in that
directory uncompiled. The test that catches this bug was written and then never
run. Adding two `#include`s took the suite from 7 tests to 12 and immediately
turned the bug red. That is probably the single highest-value thing on this list.

Everyone else, by category:

- **Focus rings become visible for the first time** — the immediate-renderer fix
  means any opaque focused widget now shows a ring it was previously painting
  over. Affects `floatinghotel`, `hanabi`, `wordproc`, `break-ross`,
  `prime_pressure`, `ui-afterhours`, `MyNameChef`, `kart-afterhours`,
  `afterhours-template`, and our own in-game canvas. No test anywhere catches
  it; it wants an eyeball on bump. (We checked ours: the module store and
  toolbar pick up no unwanted ring.)
- **`kart-afterhours`** ships `scripts/compare_baselines.py` and
  `make update-baselines` / `validate-screenshots`, but `screenshot-baselines/`
  doesn't exist. Whoever runs `update-baselines` first bakes in whatever the
  ring looks like at that moment.
- **`floatinghotel`** resets focus with direct field writes
  (`src/ecs/app_reset.h:94-95`, `ctx.focus_id = ctx.ROOT`) rather than
  `set_focus`, so it never sets `focus_source`. Harmless — the field resets to
  `Grab` every frame in `BeginUIContextManager` — but it means the reset does
  not clear an explicit claim made earlier in the same frame.
- **The `push_key` frame delay** hits nobody: every affected e2e script already
  waits ≥2 frames. The one hand-rolled pump,
  `wm_afterhours/src/headless_screenshots.cpp:1062`, calls `reset_frame()` at
  the top of its loop and survives.
- **No project inspects draw calls**, so making the `none` backend record
  outlines is invisible downstream.
- **No usage of any changed API at all**: `armchair_coach` (root),
  `armchair_coach-navi`, `endless-dance-chaos`, `mailboy`, `pharmasea`,
  `pong-afterhours`, `tetr-afterhours`, `webhelper`, `cartographer`.

Nobody calls `UIComponent::focus_rect` (two apparent hits, in `kart-afterhours`
and `pong-afterhours`, are their own unrelated `Transform::focus_rect`), and
nobody except us sets `focus_ring_thickness = 0`, so the `int`→`float` signature
change and the disable fix are both no-ops downstream.

## Not fixed — needs a maintainer decision

### Hover cannot reach tray children, and the obvious fix is wrong

`HandleTrayNavigation` force-marks every tray child `SkipWhenTabbing`, and
`can_be_focused` — shared by tabbing *and* hover-follow — rejects those. So the
mouse is inert over a tray.

The tempting fix is to split `can_be_focused` into tab and pointer variants and
have hover ignore `SkipWhenTabbing`. **Don't.** `with_skip_tabbing` is a public
API that apps use to mean "not reachable", full stop — this game marks locked
level rows and a disabled menu item that way, and making them hover-focusable
would be a visible regression. The narrower fix is for tabbing to skip
tray children structurally (parent has `HasTray`) rather than by branding them
with a public opt-out flag. We left this alone rather than guess.

### No scroll-into-view for keyboard focus

`HandleScrollInput` is gated on the cursor being inside the view and never reads
`focus_id`, so Tab can focus a row scrolled far out of sight. Doesn't bite us
(our roster fits; a 10th level is deliberately omitted), so we have no repro to
offer, but it will bite the first app with a long focusable list.

### No focus groups

The only grouping primitives are `HasTray` (one tab stop, children removed from
focus entirely) and `FocusClusterRoot` (purely cosmetic). What we actually
wanted — arrows scope to a group, Tab moves between groups, children stay
mouse-focusable — has no primitive, so it lives in the app
(`src/systems/menu_common.cpp`, `menu_section_nav`). A `FocusGroup` marker plus
group-aware `process_tabbing` would delete it. That's a feature, not a bug fix;
maintainer's call.

## Also worth knowing

`ComponentConfig::with_roundness()` is *not* inert without
`with_rounded_corners()` — `overwrite_defaults` back-fills the bitset from
`theme.rounded_corners` (default all-set), so essentially every imm widget
carries `HasRoundedCorners`. We initially believed the opposite and wrote a
`rad_for()` corner fallback that could never be reached. Consequence for
anything drawing alongside a widget: you cannot infer a widget's corner radius
from "does it have the component"; unset means `theme.roundness`, not your
fallback.
