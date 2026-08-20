# Silent traps

Things that fail by doing nothing. Each one here cost real debugging time, and
each was found only because a test was checked against a deliberately broken
build rather than trusted for passing.

The common shape: **the code compiles, runs, logs nothing, and produces no
effect.** If you are debugging something that "should obviously work", check
this list before going deeper.

---

## 1. `EntityQuery` finds nothing from inside a system

**Symptom:** `EntityQuery().whereHasComponent<Foo>().gen()` returns an empty
list, while a `System<Foo>` in the same frame iterates those entities fine.

**Cause:** `EntityQuery()` defaults to `EntityHelper::get_default_collection()`.
UI entities are not in it. A system gets its entities from whichever collection
the manager runs against, so the two disagree.

**Where it bit:** the first version of `SyncScrollViews` was a `System<>` that
queried in `once()`. It found zero scroll views every frame and synced nothing.
No warning — an empty result is indistinguishable from "no views exist".

**What to do instead:** make it an iterating system and collect what you need
during `for_each_with`, then act in `after()`:

```cpp
struct MySystem : System<Foo> {
  std::vector<Foo *> seen;
  void once(float) override { seen.clear(); }
  void for_each_with(Entity &, Foo &f, float) override { seen.push_back(&f); }
  void after(float) override { /* whole-set work here */ }
};
```

`once()` runs before the pass, `after()` runs once at the end of it, and both
are called even when nothing matched. See `SyncScrollViews` in
`vendor/afterhours/src/plugins/ui/systems.h`.

**Related:** `EntityQuery` also skips entities still in the collection's *temp*
(staging) list, which is why calling it right after `EntityHelper::createEntity()`
trips an assert.

---

## 2. Synthetic `scroll_wheel` did nothing in headless e2e — FIXED

**Cause:** the reader ran before the writer. wm registers
`register_after_ui_updates` (which contains `HandleScrollInput`) *before*
`register_builtin_handlers` (which contains `HandleScrollWheelCommand`), so the
wheel was read, then set, then wiped by `reset_frame()` — never observed.

The injector's own comment asserted the opposite ("the wheel's consumer is a UI
system that always runs after the command that sets it"), which is why the
wheel never got the one-frame survival that pinch has.

**Fix:** the wheel now survives one `reset_frame`, like pinch. It is *not*
drained on read, because raylib's `GetMouseWheelMove()` returns the same value
for every call within a frame and the injector has to match. Pinch is the
opposite — drained on read, because `gestures::consume_pinch_delta()` drains.

**Lesson:** a comment asserting system order is a claim about registration in
every downstream app, and this one was never checked.

## 3. `expect_no_text` passed unconditionally

**Symptom:** asserting the absence of text plainly on screen still passed. 18
assertions across 6 scripts were vacuous.

**Two causes, both needed:**

1. `expect_no_text` was missing from `runner.h`'s per-command parse chain, so
   it fell to the generic branch, which splits on whitespace and keeps the
   quotes. `cmd.args[0]` was literally `"Synchronized`. This is the second time
   that parse chain has silently eaten a command's arguments (`expect_text_i`
   was the first).
2. It concluded "absent" against an empty registry. Handlers run before the
   render that fills it, so absence was trivially true. It now retries until
   something has rendered.

## 4. The visible-text registry ignored clip rects

`register_text_if_visible` tested only against the screen viewport, so text
scrolled out of a pane still counted as visible. That made `expect_no_text`
unable to express "scrolled away" *and* gave `expect_text` false positives for
clipped content — `95_scrollbar_drag.e2e` asserted a row that was rendered
below its pane's clip rect and had never been on screen.

It now intersects with `detail::compute_intersected_clip_rect`, the same helper
the render scissor and hit-testing use.

## 5. A script that resizes and does not resize back

`35_responsive_audit.e2e` ends at `resize 1920 1080`. Every later script ran at
1080p, where more content fits — so what is "visible" silently changed for the
rest of the run. Invisible until `expect_no_text` started working.

wm's per-script `reset_fn` now restores the run's starting resolution. Note it
has to write the `ProvidesCurrentResolution` singleton, not just call
`set_window_size`, which is a no-op headless.

## 6. A wait that renders nothing

`expect_text`/`expect_no_text` read `VisibleTextRegistry`, and only a **render**
refills it (`ClearVisibity`/the render pass clears it first). An app is free to
run several `tick()`s per rendered frame — floatinghotel's `e2e_tick_loop` runs
up to 200 — so `wait_frames 5` can burn its whole budget without one render.
The assertion after it then reads the frame the *preceding* click was made on.

Worse, waiting longer does not help: more ticks, still no render.

`expect_no_text` now waits on `VisibleTextRegistry::generation()` rather than a
frame count, so it always sees one render after itself. That is the floor, not
a cure: if your loop batches ticks, render between them, or the script is
asserting against a frame it never asked for.

## 7. Solver rules that should not apply inside a scroll view

`solve_violations` shrinks children that overflow the main axis. For a scroll
view that is exactly backwards — overflow is the content. It had no exemption,
so a 55-row list in a 220px viewport got every row squashed to 4px.

It stayed hidden because it only bites non-strict sizes (`h720`, `percent`);
strict `pixels()` children are left alone, and wm's rows are strict. It only
became reachable at all once `48f808d` made the solver recurse into
absolutely-positioned subtrees — floatinghotel builds every panel absolute, so
its whole UI had been skipping the solver.

**Lesson:** when a rule is "make the children fit", ask which containers mean
the opposite. Positioning already had the `!is_scroll_view` guard in three
places; sizing had none.

---

## How to not get caught

A passing test proves nothing until you have seen it fail. Before trusting a
new assertion, break the thing it covers and re-run it. Every entry above was
a test that passed against a deliberately broken build.
