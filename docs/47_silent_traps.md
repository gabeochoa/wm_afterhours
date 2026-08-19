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

## 2. Synthetic `scroll_wheel` does nothing in headless e2e

**Symptom:** an e2e script does `mouse_move` then `scroll_wheel`, asserts on the
scrolled state, and passes — including when the feature under test is disabled.

**Cause:** unknown; the injector plumbing reads correct on inspection
(`input_injector::set_mouse_wheel` is consulted by
`input_system.h: get_mouse_wheel_move_v()` under `test_mode`). Something
between the two eats the value.

**Where it bit:** `12_scroll_view.e2e:25` already carries a note that its own
scroll assertions are vacuous for this reason. The first version of
`97a_sync_scroll.e2e` used the wheel and passed identically with the sync
deliberately turned off.

**What to do instead:** drive scrolling through the scrollbar
(`mouse_down` / `mouse_move` / `mouse_up` on the thumb), which does work
headless — see `95_scrollbar_drag.e2e` and `97a_sync_scroll.e2e`.

**Still open.** Worth fixing: every wheel-driven assertion in the suite is
currently meaningless, and the failure mode is a green test.

---

## How to not get caught

A passing test proves nothing until you have seen it fail. Before trusting a
new assertion, break the thing it covers and re-run it. Every entry above was
a test that passed against a deliberately broken build.
