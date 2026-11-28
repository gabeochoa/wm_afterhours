# afterhours gaps, open as of 2026-09-07

Every upstream item `puzzle/` is currently blocked on or working around, in one
place. Each entry says what we hit, how to reproduce it, and what we think the
fix is -- so this can be handed over without the surrounding conversation.

**Seventeen gaps are open: 5-12 and 14-22.** 13 is fixed upstream and kept for the
record. (14, 16, 17 and 21 landed upstream after this doc's last pass -- the
game todo records the commits -- but their entries below are not yet marked;
that audit is still owed.)

**Three of them we are offering to hand over rather than just report** -- 9
(the profiler), 15 (the log file sink) and 5's `reveal` half. Each already
exists here, works, and was written because the library had nothing; the entries
say where the code is. See "What we would upstream" at the bottom.

> **7 and 8 are URGENT and live in their own file.** They are a performance
> regression the 2026-09-07 bump itself introduced, and
> `afterhours_urgent_perf_regression.md` carries the full measurement and a
> ready-to-apply patch. They are summarised here only so the table is complete. Gaps 1-4 were all fixed upstream and landed
here in the 2026-09-07 submodule bump (`15d57cf` -> `0159d35`, 107 commits);
their entries are kept below with a FIXED banner rather than deleted, so the
fix is findable from the report that asked for it. Verified present in source,
not just in the CHANGELOG.

Fixed earlier, over 2026-08-23/24, and no longer listed: the injected click and
wheel each arriving twice, the missing middle mouse button (all three in
`ae5c101`), both settings-logger bugs -- the printf `%s` in a `std::format`
call and the hardcoded "Settings" type name (`0e1cd8f`) -- and `with_label("")`
being ignored instead of clearing the label (`tests/label_clear_test.cpp`
covers it).

Also fixed earlier and not listed: native pinch-to-zoom, the pinch injector
double-delivery, and the menu focus-ring gaps (see `afterhours-focus-gaps.md`).

Withdrawn as never-real: `with_roundness` was reported as inert without
`with_rounded_corners`. It is not -- `overwrite_defaults` back-fills the corner
mask from the theme, whose default is all four corners set.

| # | Area | Severity | Also hits | Costs us |
|---|------|----------|-----------|----------|
| 1 | ~~no way to ask why a widget has focus~~ | FIXED | | `set_focus` records caller file/line; `dump_focus` |
| 2 | ~~e2e cannot assert on what was DRAWN~~ | FIXED | | `capture::enable()`, `expect_drawn`/`_not_drawn`/`_at` |
| 3 | ~~no dashed/patterned line primitive~~ | FIXED | | `polyline::draw_dashed` |
| 4 | ~~no arc-length resampling for a sampled curve~~ | FIXED | | `polyline::resample_by_arclength` |
| 5 | no path for files the PLAYER is meant to find | medium | anything a user exports or shares | exported gifs land in a Finder-hidden folder |
| 6 | `platform_folders.h` cannot be included twice | low | any game wanting a folder `files` omits | blocks the workaround for 5 |
| 7 | `capture::record` builds a `std::string` per draw even when disabled | high | every consumer, every frame | RenderNodeSystem 1.37ms -> 2.34ms |
| 8 | `warn_on_missing_glyphs` re-decodes every string every draw | high | every consumer drawing text | same regression as 7 |
| 9 | `SystemManager` cannot be profiled | high | every consumer | 149 local systems wrapped by hand; 139 library systems unmeasurable |
| 10 | no rectangle algebra (pad/cut/align) | medium-high | WMs, word processors, any UI | a 224-line `rect.h` per consumer, 613 uses here |
| 11 | letterbox transform is private, duplicated, and absent on sokol | medium-high | any fixed-logical-resolution app | `render_space.h` re-derives it, kept in sync by a comment |
| 12 | no blend-mode API, so its cost is unwrappable and invisible | medium | anything compositing layers | 3.8 ms/frame of batch flushes to issue 0.005 ms of blits |
| 13 | ~~e2e `set_slider` could not set or address a slider~~ | FIXED | | `876497c`; we had already worked around it |
| 14 | the UI plugin is 37% of a frame with no visible UI | high | every consumer, scales with entity count | 741-tile scene stuck at ~9 fps |
| 15 | `logging.h` cannot write to a file | medium-high | anything shipped to a non-developer | a tester's crash leaves nothing behind |
| 16 | `VALIDATE` is a variadic no-op that still evaluates its arguments | medium | every consumer | invariants unenforceable; side effects run in release |
| 17 | nothing catches a crash, including the library's own `abort()`s | medium-high | every consumer; worse for document apps | a tester crash leaves no report, so the session is wasted |
| 18 | 66 of 91 raw `raylib::` sites have no wrapper; CPU-side Image handling has none at all | medium-high | any "wrap the backend" rule; anyone wanting the sokol backend | two files cannot be written cleanly either way |
| 19 | `build_font_atlas` hardcodes `packMethod 0`, which overflows on large atlases | medium | every consumer; worst for the library's own 556-codepoint default | 41 lines of `font_loading.h` we cannot delete |
| 20 | `setting_row` unconditionally overwrites the caller's font size and label font | low | anyone whose text is not 22px `DEFAULT_FONT` | blocks adoption on our >28px floor |
| 21 | the UI render pass clears `capture`, so only UI draws are assertable | high | every consumer drawing outside imm | gap 2's own motivating case still needs pixel archaeology |
| 22 | `terminal::Console` history cannot be reset | low | anyone testing recalled input, or reopening a console per document | history leaks across e2e scripts; `clear()` clears output only |

---

## 1. There is no way to ask why a widget has focus

> **FIXED upstream, in the 2026-09-07 bump.** `set_focus` records the caller's
> file and line alongside `FocusSource`, and only when focus actually moves, so
> the per-frame re-grab does not bury whatever moved it. New `dump_focus`
> prints holder and origin, and `expect_focused` now names what DOES hold focus
> -- the exact follow-up this entry asked for. `ui/context.h:150,281`.

Adding one focusable column to Level Select moved entry focus off the roster and
stopped Tab advancing. Ruled out, each with a measurement: `MenuCursor`
(restores only on the entry frame), `menu_section_nav` (runs only on input, and
the new panel was added to its section list), and hover-follow (parking the
mouse elsewhere changed nothing). `dump_ui` reports *what* holds focus; nothing
reports *what set it*, so the search is guess-and-rebuild. Still unresolved.

**Suggested fix:** record the last writer -- a `FocusSource` plus a caller tag
-- and print it in `dump_ui`. One "focus set by X" line would have replaced
hours here.

Related: `focus_ui` in e2e sets focus the screen's own model can then overrule,
silently. `expect_focused` should report what holds focus instead of only that
the expected widget does not.

## 2. e2e cannot assert on what was actually drawn

> **FIXED upstream, in the 2026-09-07 bump.** `capture::enable()` (`capture.h`)
> records draws on ANY backend, not just `none`, and each `DrawnCall` carries
> `entity_id` and `layer` -- so a test asks "did this row's border get painted"
> rather than "did anything paint blue". `expect_drawn`, `expect_not_drawn`,
> `expect_drawn_at` and `dump_draws` are registered inside
> `register_ui_commands`, which we DO call (`src/e2e/e2e_commands.cpp:102`) --
> so all four are reachable from our scripts today, and `draw_line_ex` records.
> **But see gap 21:** the UI render pass clears the capture buffer, so canvas
> draws never survive to be asserted on. The cable-pulse case below is still
> blocked, and the pixel-profiling described here is NOT yet obsolete.

Found 2026-08-25, adding flow pulses to live patch cables. The feature IS the
rendering -- short bright dashes travelling along a cable that carries signal --
so "does it build" and "does the suite pass" say nothing about whether it works.

afterhours already solves this for its own tests. Under
`AFTER_HOURS_BACKEND_NONE` the renderer records `DrawCall`s instead of touching
a GPU, and `ImmTestHarness::render()` hands back the list; that is how
`label_clear_test` asserts on real rendering rather than on layout.

A game builds against raylib (`AFTER_HOURS_USE_RAYLIB`), so none of it is
reachable from an app-level e2e run. What we did instead: screenshot to PNG and
profile pixel columns with Python PIL, looking for bright runs. That is slow to
write, brittle against any palette change, and it silently measured the wrong
thing twice -- once picking up the timeline drawer instead of the cable.

**Suggested fix:** let a raylib build opt into recording draw calls alongside
issuing them (a compile flag or a runtime `capture_draws` toggle), and expose
the buffer so an e2e command can assert against it. Even a coarse
`draws_matching(color, min_count)` would beat pixel archaeology.

## 3. There is no dashed or patterned line primitive

> **FIXED upstream, in the 2026-09-07 bump.**
> `polyline::draw_dashed(pts, thickness, color, dash, gap, phase)`
> (`polyline.h:72`) -- including the `phase` argument this entry asked for.
> Arc length accumulates across vertices, so a dash straddling a corner stays
> one dash.

`draw_line_ex` draws a solid segment and there is nothing patterned, so a dashed
run has to be hand-rolled: accumulate arc length along the polyline, and emit a
segment when its midpoint falls inside a dash window.

We now do that for live cables. The same thing wants doing for selection
marquees, timeline tick runs and any "provisional" line.

**Suggested fix:** `draw_polyline_dashed(pts, width, color, dash, gap, phase)`.
The `phase` argument is what makes it animate, and is easy to leave out.

## 4. A sampled curve cannot be resampled by arc length

> **FIXED upstream, in the 2026-09-07 bump.**
> `polyline::resample_by_arclength(pts, step)` (`polyline.h:41`), exactly the
> suggested signature.

`sample_wire_bezier` (and any bezier sampler) walks uniformly in `t`, which is
not uniform in screen distance: points bunch where the curve is tight. Anything
spaced along the result -- dashes, arrowheads, flow pulses, labels -- inherits
that bunching, so our pulses are visibly closer together through a cable's bend.

**Suggested fix:** `resample_by_arclength(pts, step)` returning evenly spaced
points, or an arc-length lookup table alongside the sampler.

## 5. There is no path for files the player is meant to find

`files` gives three roots -- `get_resource_path`, `get_save_path`,
`get_config_path`. All three are right for what they name, and none of them is
right for an artifact the player MAKES and then wants to look at, attach to a
bug report, or post.

We hit it exporting solution GIFs. The only writable root available is
`get_save_path()`, which on macOS is
`~/Library/Application Support/<game>` -- and Finder hides `~/Library` by
default, so a tester cannot reach it without being talked through
Go > Go to Folder. Writing next to the executable instead is worse: a packaged
.app is either read-only or somewhere equally unfindable.

**Repro:** call `files::get_save_path()` on macOS, write a file, then ask
someone non-technical to open it.

**Suggested fix:** expose the folders sago already implements —
`getDocumentsFolder()` and `getDesktopFolder()` are right there in
`vendor/sago/platform_folders.h` and nothing surfaces them. A
`files::get_documents_path()` alongside the existing three would cover it.

Worth considering with it: a `files::reveal(path)` that opens the platform file
manager with the file selected (`open -R` / `explorer /select,` /
`xdg-open` on the parent). That is per-platform shelling out, which is engine
work rather than game work, and it is the thing that actually closes the loop
for a tester.

**Downstream workaround, 2026-09-07:** puzzle now has `paths::reveal()` (12
lines of `std::system` shelling out, `src/paths.h`) behind an OPEN SAVE FOLDER
button in Settings. It closes the tester loop, so the urgency here drops — but
it is the `reveal` half only, and every consumer will rewrite it. The
`get_documents_path()` half is untouched and still wanted.

## 6. `platform_folders.h` can only be included once

The header carries 14 non-inline free-function definitions
(`std::string getDataHome() { ... }` at :532 and similar) and no
`..._IMPLEMENTATION` guard, so `files.cpp` is its only legal include site. A
second TU including it is a duplicate-symbol link error.

That is fine while `files` exposes everything a game needs. It stops being fine
the moment it doesn't — gap 5 above has an obvious local workaround (include
sago, call `getDocumentsFolder()`) and this is what blocks it, which is why the
two are filed together.

**Repro:** `#include <afterhours/vendor/sago/platform_folders.h>` in any second
TU and link.

**Suggested fix:** mark the definitions `inline`, or wrap them in the
conventional single-TU implementation macro.

## 7 and 8. Per-draw regression introduced by the 2026-09-07 bump

**See `afterhours_urgent_perf_regression.md`** — it has the numbers, the repro
and the patch. In one line each:

- **7:** `capture::record` takes `const std::string &`, so every `const char *`
  call site constructs (and heap-allocates) a string before the
  `if (!enabled()) return;` inside can fire. `capture.h:75`.
- **8:** `warn_on_missing_glyphs` re-decodes every string with a
  `GetCodepointNext` call per character, on every draw, every frame.
  `font_helper.h:113`, called unconditionally from `drawing_helpers.h:177`.

Measured 1.7x on our node renderer and 2.1x on wire rendering, min of 5, `-Og`.
Wire rendering draws no text, which is what implicates the shared per-draw cost
rather than the glyph scan alone.

## 9. `SystemManager` cannot be profiled, so the library's own frame cost is invisible

`SystemBase` (`core/system.h:91-116`) has no `name()`, and `SystemManager::tick`
/ `fixed_tick` / `render` (`:464`, `:488`, `:511`) loop with no instrumentation
point. `e2e_testing/perf_commands.h` acknowledges this and pushes it to the
consumer: `PerfProvider` is three `std::function`s the app must fill in, and
`dump_profile` fails with "no perf provider configured" until it does.

So we wrote `src/game_system.h:8-55` -- a local `GameSystem<...>` that demangles
`typeid(*this).name()` and hijacks `should_run()`/`after()` as begin/end
brackets -- plus the 518-line `src/profiler.h` behind it. 149 of our systems
derive from it.

It has a hard ceiling: it only sees systems WE wrote. Everything from
`ui::register_before_ui_updates`, `register_after_ui_updates`,
`register_render_systems` and `layered_input<>::register_update_systems`
(`src/systems/core_systems.cpp:18-47`) derives from `afterhours::System`
directly and is untimed. Our entire measured baseline
(`design/docs/profiling-baseline.md:37-52`) contains zero afterhours systems --
not because they are cheap, but because they are unobservable. Gaps 7 and 8
above were found by elimination against a git bisect, which is what you do when
you cannot just read the timing.

**Repro:** register any afterhours-provided system alongside your own, run
`dump_profile`, and note it cannot appear. Then try to instrument it without
editing the submodule.

**Suggested fix:**
```cpp
virtual std::string_view SystemBase::name() const;   // default type_name<Self>()
enum struct Phase { Update, FixedUpdate, Render };
struct SystemProfileHook {
    std::function<void(std::string_view, Phase)> begin, end;
};
static void SystemManager::set_profile_hook(SystemProfileHook);
```
Wire the existing `PerfProvider` to it so `dump_profile` works out of the box.

**Also hits:** every consumer. For a word processor or note tool the frame is
*mostly* library -- autolayout solve, text measure, the imm render pass -- so
"where did my frame go" is a question only the library can answer. It would also
let the library ship its own timing-regression numbers, the way the CHANGELOG
already ships `dynamic_cast` counts and RSS soaks.

**Offer: take `src/profiler.h` rather than write one.** 517 lines, no game
types in it -- nested scopes via a `PROFILE(name)` RAII macro, per-scope
min/mean/p99, an entity-count column, a `.collapsed.tsv` export that loads in
any flamegraph viewer, and console commands (`profile_start` / `_stop` /
`_report` / `_export`) already wired to `PerfProvider`. It is the consumer half
of the hook above; if the hook lands, this is what should sit behind
`dump_profile` out of the box. Two things to change on the way in: it uses our
`log_me`, and `Profiler::instance()` is a plain static rather than a singleton
component.

## 10. There is no rectangle algebra, so a 224-line geometry header exists in every consumer

`src/rect.h:3` is literally `// TODO move to afterhours?`, and `:5` says
"afterhours has no rect geometry utils (only draw_* fns), so this fills the
gap". It provides `pad`/`inset`, `margin`/`expand`, `cut_top/bottom/left/right`
returning both pieces in spatial order, `align_center/top/bottom/left/right`,
`with_size`/`with_pos`/`offset`, `center`/`contains`/`overlaps` -- constexpr,
value-returning, implicitly convertible both ways with `RectangleType`.
**613 uses across 37 of our files.**

The library side is one `intersect_rects` (`plugins/ui/systems.h:69`) and a
`get_collision_rec` per backend. Nothing compositional.

This is NOT the imm box model. It is the layout arithmetic a *render pass* does
after imm hands back a rect -- exactly where the library gives you a bare struct
and stops.

**Repro:** write "take a 22px header strip off this panel, inset the remainder
by 6, and centre a 64x36 box in it" using only afterhours APIs.

**Suggested fix:** free functions over `RectangleType`, since it is `#define`d
(`developer.h:34`) and cannot take members:
```cpp
RectangleType pad(RectangleType, Sides);
RectangleType expand(RectangleType, Sides);
struct Split { RectangleType first, second; };
Split cut_top(RectangleType, float h);              // + bottom/left/right
RectangleType align_center(RectangleType, float w, float h);   // + four edges
```
Match `Margin`'s new `{top, left, bottom, right}` order or state the difference
loudly -- ours is CSS order, and a silent mismatch would be nasty. Our file can
be taken verbatim; the box-model diagram at `rect.h:27-45` is the part worth
keeping, because pad-vs-margin direction is what people get backwards.

**Also hits:** the most portable item in this report. A WM tiling a screen is
`cut_left`/`cut_right` and nothing else. A word processor's page geometry is
`pad(margins)` then `cut_top(header)` then `cut_bottom(footer)`. A note tool
laying out a card is `align_*`. Games are the least interesting consumer.

## 11. The letterbox transform is private, duplicated by consumers, and absent on sokol

`plugins/input_system.h:68-79` maps the OS cursor into content space by deriving
a letterbox from `ProvidesCurrentResolution`, inline inside
`get_mouse_position()` and not exposed.

Presentation is the app's job, so we re-derived it. `src/render_space.h:22-33`
is line-for-line the same arithmetic, and `render_space.h:13` says why:
"Math must match afterhours::input::get_mouse_position() so hit tests line up".
An undocumented contract between two files, one of them in a submodule, enforced
by a comment.

Two consequences beyond the duplication:

- **It diverges by backend.** `backends/sokol/backend.h:708-714` returns
  `{mouse_x / dpi, mouse_y / dpi}` -- DPI only, no letterbox, no
  `ProvidesCurrentResolution`. Meanwhile the UI treats `ProvidesCurrentResolution`
  as its layout space in 20+ places. A sokol app whose content resolution differs
  from its window has UI laid out in content space and a cursor in window space.
  We ship raylib only, so this is read from source, not run.
- **There is no inverse.** Nothing maps a content-space rect back to window
  space -- which is what an OS cursor rect, an IME candidate window or a
  drag-and-drop hit region needs.

**Repro:** set `ProvidesCurrentResolution` to something other than the window
size, present a render texture yourself, and draw a hit-test debug overlay in
window coordinates without copying `input_system.h:68-79`.

**Suggested fix:**
```cpp
struct Viewport { RectangleType dest; float scale; };
Viewport window_manager::content_viewport();
Vector2Type window_manager::window_to_content(Vector2Type);
Vector2Type window_manager::content_to_window(Vector2Type);
```
used by BOTH backends' `get_mouse_position`, so present and hit test cannot
drift.

**Also hits:** any app with a fixed logical resolution, and more generally this
is the logical/physical pixel mapping. A word processor's 100/150/200% document
zoom is the same transform and needs the inverse for caret and IME placement. A
WM needs it per output. Fixing sokol also unblocks the non-raylib backends.

## 12. There is no blend-mode API, so the most expensive thing in our render pass was unwrappable and invisible

The wrapper surface (`backends/raylib/drawing_helpers.h`) covers shapes,
textures, render targets, scissor and rotation. Blend state appears nowhere in
`vendor/afterhours/src/` except one internal `rlSetBlendMode` in
`backends/raylib/headless.h:52`. So we call raylib directly --
`BeginBlendMode`/`EndBlendMode` at `src/systems/feed_capture.cpp:58,61,70,72,439,443`
and `src/systems/nodes/chrome.cpp:999,1001`, plus `rlSetBlendFactorsSeparate` at
`feed_capture.cpp:256,436,445`, reaching past `rlgl` as well as past the wrapper.

The measurement is the point (`design/docs/profiling-baseline.md:86-98`):

| sub-scope | avg ms/frame |
|---|---|
| `BeginBlendMode` | 2.372 |
| `EndBlendMode` | 1.452 |
| `DrawTexturePro` (the actual blit) | **0.005** |

`rlSetBlendMode` calls `rlDrawRenderBatch` unconditionally when the mode
differs, and our per-blit pairs alternate against straight-alpha chrome, so
every node paid two full GPU batch flushes: 3.8 ms/frame to issue 0.005 ms of
drawing.

The gap is not "we couldn't do it". It is that the library owns the draw path,
and because blend state sits outside that path the library cannot see the
transitions, cannot coalesce them, and cannot report them.

**Repro:** wrap each of N textured blits in its own
`BeginBlendMode(BLEND_ALPHA_PREMULTIPLY)`/`EndBlendMode` and time the pair
against the blit.

**Suggested fix:**
```cpp
enum struct BlendMode { Alpha, Additive, Multiplied, AlphaPremultiply, Custom };
struct blend_scope { explicit blend_scope(BlendMode); ~blend_scope(); };
void draw_texture_pro(..., std::optional<BlendMode> = std::nullopt);
size_t graphics::blend_transitions_this_frame();   // pairs with gap 9
```
Skipping the state change when the mode already matches is the cheap half;
counting transitions is the half that would have saved us the profiling pass.

**Also hits:** anything compositing translucent layers -- a word processor's
selection highlight and track-changes overlays, a WM's shadows and translucent
decorations, anything drawing to intermediate render targets. Also portability:
today the only way to blend is to name `raylib::`, which forfeits sokol.

## 13. e2e `set_slider` could not set, or even address, a slider

> **FIXED upstream in `5608aa9` ("Make set_slider set the slider").** Kept
> because the second defect was worse than this entry originally said, and the
> same shape can recur in any command that resolves a widget by debug name.

`e2e_testing/ui_commands.h:482-512`. The handler parses `value`, then:

```cpp
      // Click the center of the slider for now
      // TODO: calculate pct from slider min/max when HasSliderState supports
      // range
      (void)value;
      auto rect = get_screen_rect(entity);
      float x = rect.x + rect.width / 2;
```

`set_slider volume 0.9` and `set_slider volume 0.1` do the same thing: ~50%. It
reports success either way, so a test asserting a specific slider value passes
for the wrong reason -- worse than failing.

**Second defect, and the worse one: it could not address a specific slider at
all.** The query required `UIComponentDebug` AND `HasSliderState` on the *same*
entity (`:497-503`), and no slider is built that way -- the caller's debug name
goes on the outer element, the state on a child named `slider_background`. So
naming your own slider failed outright with "Slider not found", and naming
`slider_background` hit whichever slider happened to come first. `expect_slider`
shared the defect, so assertions on a slider were equally unreliable.

That combination is nastier than either half: the addressing bug means a script
either errors or silently drives the wrong widget, and the `(void)value` bug
means that even when it lands, it lands at 50%.

We were never bitten because we never used it: `HandleSetSliderValueCommand`
(`src/e2e/e2e_commands.h:1475-1512`) matches on the parent's debug name via
`UIComponent::parent` and writes `HasSliderState::value` directly, and our
scripts call that (`tests/settings.e2e:17,28` use `set_slider_value`). Writing
the workaround first is the only reason this never showed up as a failing test
here.

**Repro:** `set_slider <name> 0.9`, then read the bound value -- it is ~0.5.

**Suggested fix:** compute the click x from the value
(`rect.x + value * rect.width`), or write `HasSliderState::value` directly and
set `changed_since`; resolve the target by walking to the nearest named ancestor
when the state-bearing entity is unnamed. Add the read side too --
`expect_slider <name> <value> [tol=]` -- since there is no way to assert a
slider's value at all today.

**Also hits:** every consumer with a settings screen. A word processor testing a
zoom or line-spacing control, a note tool testing a font size: the value IS the
assertion, and the harness cannot express it.

## 14. The UI plugin costs 37% of a frame on a screen with no visible UI

Found profiling our cold open: a 741-tile level, 5,431 entities, stuck at ~9 fps.
`sample` on a windowed run, 2 s, 1,439 main-thread samples:

| branch | samples | % main thread |
|---|---|---|
| `run_systems_on_ui_entities` | 538 | **37%** |
| via `UIPluginRenderBridge` | 351 | 24% |
| via `UIPluginPostUpdateBridge` | 180 | 13% |
| `std::bitset<128>` accessor leaves | ~409 | 28% |

GPU is not the bottleneck: `Present`/`swap` is 6.6 ms.

### The chain

1. `UIPluginRenderBridge::once()` and `UIPluginPostUpdateBridge::once()` each
   call `run_systems_on_ui_entities` (`ui/utilities.h:231`), which loops every
   entity in `ui_coll` for every UI system.
2. `RenderImm` / `RenderBatched` are
   `System<UIContext<InputAction>, FontManager>` and set
   `include_derived_children = true` (`ui/rendering.h:1314`), so every entity
   goes `for_each_derived` -> `HasAllComponents::value_child` ->
   `check_component_child` (`core/system.h:173`) -> `has_child_of<T>()`.
3. `has_child_of` (`core/entity.h:78`) loops **all 128 component slots**.
4. At `-Og` each `componentSet[i]` is four un-inlined calls — `operator[]` ->
   `__make_ref` -> `__bit_const_reference` ctor -> `operator bool` — two through
   **DYLD stubs**, which can never inline.

**164 of 168 samples** under one `run_systems_on_ui_entities` node are that
chain.

**Repro:** any scene with a few thousand entities, built `-Og`, sampled while a
UI plugin bridge runs. The UI does not need to be visible; retired-but-not-yet-
cleaned imm widgets are iterated at full price too (our `dump_ui` prints an
empty tree while this cost is being paid).

### Suggested fixes, most valuable first

**A. A system whose components are all singletons should not iterate.**
`RenderImm` wants `UIContext` and `FontManager`. Both are singletons registered
on `ui_root` (`ui/utilities.h:149-160`). Visiting N entities to find the 1 that
holds them is O(N) for an O(1) answer, and it is the dominant cost above.
`should_iterate() == false` plus `get_singleton_cmp<T>()` — the pattern the two
bridges already use — removes the per-entity path entirely. Generalisable: if
every component in a system's type list is a registered singleton, skip
iteration in the framework rather than relying on each system to remember.

**B. `std::bitset` is the wrong primitive for a hot path, because consumers
develop at `-Og`.** `std::bitset<128>::operator[]` is four function calls there,
two unavoidably cross-image. Any ECS hot path built on it is ~4x worse than the
source reads, and no consumer-side care fixes it. `componentSet` as a plain
`uint64_t[2]` with hand-written test/set is identical at `-O2` and one
instruction at `-Og`. Related: the CHANGELOG's "18x fewer `dynamic_cast`s" in
`has_child_of` traded one un-inlined call for 128 cheaper ones — a clear win at
`-O2`, much less of one at `-Og`.

**C. `has_child_of` scans 128 slots to answer "no".** Its comment says
`componentSet` means "only those get the RTTI check rather than every slot" —
true for the RTTI, but the loop still visits all 128 and pays a subscript each.
The common answer is "no", so the miss path is the hot one and is the one left
unoptimised. Two options: an exact-type fast path
(`if (componentSet[get_type_id<T>()]) return true;`) makes hits O(1); keeping a
small packed list of occupied slot indices takes the miss path from 128 probes
to ~5, since a typical entity holds ~5 components.

**D. `include_derived_children` is per-system, not per-component.** `RenderImm`
sets it once, so `FontManager` takes the expensive derived path too, though only
`UIContext` plausibly has subclasses. Marking it per component in the type list
(`Derived<UIContext>`) lets the other use the O(1) exact check.

**E. Retired UI entities are iterated before they are cleaned.** Between
`retire_unbuilt_ui_elements()` and `cleanup()` a dead widget costs the same as a
live one. Skipping retired entities in `run_systems_on_ui_entities`, or cleaning
before rather than after the render bridge, is free.

**Also hits:** every consumer, and worse the more entities they have. A word
processor with a large document model, a WM with many tracked windows: the cost
is per entity in the UI collection and per UI system, whether or not any of it
is on screen. Gap 9 (no `SystemManager` profiling hook) is why this took a
sampler to find — none of it appears in a consumer's own per-system profile.

## 15. `logging.h` cannot write to a file, so a shipped build's log is unrecoverable

`logging.h` prints to stdout (or stderr under `AFTER_HOURS_ENABLE_MCP`) and
nothing else -- there is no `ofstream`, no sink list, no `log_to_file`. That is
fine in a terminal and useless the moment the build leaves the developer's
machine: a double-clicked `.app` on macOS has no stdout at all, so a tester who
hits a bug has literally nothing to send back.

There is a second-order problem that makes this more than "add an ofstream": the
log path is not knowable at static-init time. It belongs under
`files::get_save_path()`, which is not valid until `files::init` runs, and by
then the startup banner, the platform report and any early failure have already
been logged and lost. A file sink that only starts working after init drops
exactly the lines you need when the failure is at startup.

**Repro:** package any afterhours game as a `.app`, double-click it, make it
log, and try to read the output.

**Suggested fix:** a sink alongside the existing print, plus a buffer for
pre-init lines:
```cpp
void log_open_file(const std::filesystem::path &);  // truncate, flush per line
```
Buffer lines logged before that call and flush them on open, with a hard cap so
a run that never opens a file cannot grow without bound. Flush per line, not per
buffer -- the log matters most when the process dies, which is the case where a
buffered tail is lost.

**Offer: `src/log/log.h:66-96` is this, and it is 30 lines.** `log_file::write`
buffers into a vector capped at 512 entries until `log_open_file` runs, then
flushes and shrinks. Colour escapes are stripped for the file copy. We call it
from `main.cpp` right after `files::init` at `paths::user("log.txt")`. Verified
against a deleted save directory -- `files::init` creates it. Ours sits in a
`AFTER_HOURS_REPLACE_LOGGING` fork, so it would need porting onto the
`std::format` path rather than dropping in.

**Also hits:** everything you ship to someone who is not you. A word processor
or note tool has the same problem in a sharper form -- the user's document is
involved, so "what happened" has to be answerable after the fact.

## 16. `VALIDATE` is a no-op function, so its arguments are still evaluated

`logging.h:89-90`:
```cpp
#if !defined(AFTER_HOURS_REPLACE_VALIDATE)
inline void VALIDATE(...) {}
```
Two problems, and the second is the real one.

First, invariants are unenforceable out of the box. A library-provided
`VALIDATE` that does nothing trains everyone to not reach for it; afterhours
itself calls it in ~6 live places (`core/entity.h` duplicate-component, five in
`plugins/autolayout.h`) and none of those can fire.

Second, **it is a function, not a macro, so the condition and the message are
evaluated at every call in every build.** `VALIDATE(expensive_check(), ...)` runs
`expensive_check()` in release and throws the answer away. A macro that expands
to `((void) 0)` does not even evaluate its arguments. The existing opt-out hook
(`AFTER_HOURS_REPLACE_VALIDATE`) shows the gap was anticipated -- it just ships
the wrong default.

**Repro:** `VALIDATE(counter++ > 0, "x");` under `NDEBUG`, then read `counter`.
It is 1. With a macro no-op it is 0.

**Suggested fix:** ship the macro form as the default -- abort with
file/line/condition/message in debug, expand to `((void) 0)` under `NDEBUG`.
Keep `AFTER_HOURS_REPLACE_VALIDATE` for consumers who want their own.

**Evidence it is safe to turn on:** we did (`src/log/log.h:182-195`). Our whole
suite went 161/161 green with it live, which exercised afterhours' own six call
sites too -- they hold. Proven both ways with a scratch TU: debug exits 134,
release runs with the condition evaluated 0 times.

## 17. Nothing catches a crash, including the ones the library itself causes

There is no `signal()`, no `std::set_terminate`, no `backtrace` anywhere in
`vendor/afterhours/src/`. A consumer who ships a build and hands it to a tester
gets nothing back from a crash: no stack, no state, no indication of which
system was running. That session is simply lost, and the bug is unreproducible.

**What makes this the library's problem rather than the consumer's** is that
afterhours is where the aborts come from, and where the context lives:

- `core/entity_collection.h:473` --
  `get_singleton_cmp_enforce` aborts on a missing singleton.
- `core/entity_query.h:532` -- `gen_first_enforce` aborts on an empty result,
  with a comment explaining that aborting is *deliberate* because `log_error`
  alone would return a reference into an empty vector.
- Gap 16 adds a third once `VALIDATE` gets teeth -- six library call sites.

So the library deliberately calls `std::abort()` in the normal course of
enforcing its own invariants, and then provides no way to find out that it did.
The `gen_first_enforce` comment reasons carefully about the abort being the
right call and stops one step short: right call, no report.

It is also the only thing that *can* answer the first question a crash raises.
"Which system was ticking?" is knowable only inside `SystemManager::tick`, and
"which entity, holding what components?" only from the collection. A consumer
handler can dump its own state and a raw stack; it cannot name the system.
Note this is the same instrumentation point gap 9 wants for profiling -- one
"currently running system" hook serves both.

**Repro:** call `EntityHelper::get_singleton_cmp_enforce<T>()` for a `T` that
was never registered, in a packaged build. Observe that nothing is written
anywhere, and that a double-clicked `.app` has no stdout to have printed to
either (gap 15).

**Suggested fix.** The split that keeps game types out of the library:

```cpp
namespace afterhours::crash {
// Installs SIGSEGV/SIGABRT/SIGBUS/SIGILL/SIGFPE handlers + std::set_terminate.
void install(const std::filesystem::path &report_path);
// Consumer appends its own payload; called from the handler, so it must be
// async-signal-safe-ish: no allocation if you can help it.
void add_context(std::function<void(std::string &)>);
}
```
The library writes: signal, backtrace (`<execinfo.h>` on macOS/Linux,
`DbgHelp` on Windows), the currently-running system name, entity count, and
the log tail. The consumer's callback appends what only it knows -- for us
that is the level id and the node graph.

Two details worth stating because they are what makes a crash handler actually
work rather than merely exist: write to a path resolved BEFORE the crash (you
cannot safely compose one inside a signal handler), and prefer
`write(2)` on a pre-opened fd over `ofstream` in the handler.

**Also hits:** every consumer, and much harder for anything document-shaped. A
word processor crashing without a report loses the user's work AND the evidence
of why. Pairs with 15 -- a crash report and a log tail are the same artifact
from a tester's point of view, and both need the same "somewhere findable"
answer that gap 5 is about.

## 18. Wrapper coverage has holes that quietly push consumers back to raw raylib

We counted every `raylib::` call in `src/` and checked each against the
wrappers. **91 call sites. 25 have a wrapper we simply had not adopted -- our
problem. The other 66 have no wrapper at all.**

| area | sites | status |
|---|---|---|
| CPU-side **Image** manipulation | 43 | **nothing upstream** |
| blend modes (`Begin`/`EndBlendMode`) | 8 | nothing upstream -- this is gap 12 |
| font atlas build (`LoadFontData`, `LoadFileData`, `GenImageFontAtlas`) | 5 | partial; gap 19 |
| audio (`IsAudioDeviceReady`, `StopSound`, `IsSoundPlaying`) | 5 | 3 of 8 audio calls unwrapped |
| window state (`ToggleBorderlessWindowed`, `IsWindowState`) | 3 | 2 of 5 window calls unwrapped |
| `GetKeyPressed` | 2 | nothing test-aware upstream |

**The Image family is the big one and it is a real absence, not an oversight
on our side.** `UnloadImage` x10, `ImageDrawRectangle` x7,
`LoadTextureFromImage` x5, `LoadImageFromTexture` x4, `GenImageColor` x3,
`LoadImage`/`ImageFormat`/`ImageFlipVertical`/`GetPixelDataSize`/`GetImageColor`
x2 each, plus `ImageResizeNN`, `ImageDrawTextEx`, `LoadImageColors`,
`UnloadImageColors`. They live in `gif_export.cpp`, `level_loading.cpp`,
`node_feed.cpp` and `systems_internal.h` -- gif encoding, sprite-sheet slicing,
reading a pixel back off a feed, and generating placeholder art. afterhours
wraps *drawing* and *textures* thoroughly and has nothing at all for the
CPU-side image buffer those go through. Whether that should be wrapped or
declared out of scope is a real design call, but right now it is neither, so
every consumer doing offline image work silently leaves the wrapper layer.

**The partial ones are worse than the absent ones, and they are why this is
filed as one gap rather than five.** Wrapping 5 of 8 audio calls and 3 of 5
window calls means `sfx.cpp` and `game_loop.cpp` cannot be written cleanly
either way: adopt the wrappers and the file mixes both styles, skip them and
the wrappers go unused. We looked at doing the 25 available swaps and stopped
for exactly this reason -- it would have left two files half-converted, which
reads as sloppiness rather than as a pending upstream fix. Finishing a family
is worth more than starting another.

The specific small ones, each with a repro:

- **Audio stops three calls short.** `plugins/audio_helpers.h:15-21` wraps
  `SetSoundVolume` / `PlaySound`, and `sound_system.h:344` has
  `set_master_volume` -- but `StopSound` (`src/sfx.cpp:59`), `IsSoundPlaying`
  (`:174`) and `IsAudioDeviceReady` (3 sites) have nothing. Stopping a sound is
  the obvious neighbour of playing one, and the non-raylib no-op stubs that
  make the existing wrappers portable are exactly what is missing here.
- **Window state stops two calls short.** `graphics.h:37,40` wrap
  `is_window_fullscreen` and `toggle_fullscreen`; `ToggleBorderlessWindowed`
  and `IsWindowState` (`src/game_loop.cpp:167-172`) have nothing, and they sit
  in the same four-line `if` as the two that do.
- **No test-aware `get_key_pressed()`.** `input_system.h` has test-aware
  `is_key_pressed` (:156) and `get_char_pressed` (:179) but no queue-draining
  `get_key_pressed`. An "any key continues" screen is a normal thing to build
  and cannot be written test-aware today.
- **`developer::distance_sq` is hidden behind `#ifndef Vector2Type`.**
  It is at `developer.h:119`, inside the arm that only compiles when afterhours
  supplies its own `Vector2Type` (`:98`). Any consumer that sets
  `-DVector2Type=raylib::Vector2` -- which is the documented raylib path --
  loses it. Our 7-line `vec_utils.h` exists purely because of this. The
  geometry does not depend on which vector type is in use, so the guard looks
  accidental.
- **No `action_pressed(E)` / `action_down(E)` convenience.**
  `src/input_action.h:71-107` (37 lines) walks `PossibleInputCollector` by hand
  because only the raw collector is exposed (`input_system.h:1068-1097`). Every
  consumer writes this same walk. Related, and possibly just undocumented:
  `inputs_pressed_repeat` exists at `:1086` and nothing told us it was there.
- **e2e cannot hold the right mouse button.** `right_click` exists
  (`command_handlers.h:159`), and `input_injector` has
  `is_mouse_right_button_down()` as a READER (`:209`) with no public setter, so
  our `right_down`/`right_up` (`e2e_commands.h:87-108`, 9 uses each) reach into
  `input_injector::detail::`. Upstream's own `mouse_down` handler does the same
  (`command_handlers.h:379`), which suggests the setter was just never added.
  A click is not a substitute: our `DeleteWireSystem` polls
  `is_mouse_button_down(1)` every frame.

**Also hits:** any consumer with a "wrap the backend" rule, and any consumer
who might one day want the sokol backend -- 66 unwrapped sites is 66 things
that will not compile. The Image family in particular is not game-specific:
thumbnail generation, export, and screenshot paths in a word processor or an
image tool go through exactly those calls.

## 19. `build_font_atlas` hardcodes the packer that fails on large atlases

`backends/raylib/font_helper.h:48-74` is the shared font-atlas routine, and its
comment says it was hoisted because "both wm_afterhours and kart-afterhours had
written this same routine in their own preload" -- so it is already the
consolidation point. It hardcodes two things:

```cpp
glyphPadding = 1;
GenImageFontAtlas(..., /*padding*/ 1, /*packMethod*/ 0);
```

`packMethod 0` is raylib's simple row packer. `src/font_loading.h:22-27`
documents it failing on us: at 96px with 99 codepoints the last glyphs overflow
and raylib logs "Failed to package character". We pass `packMethod 1` (skyline)
and `padding 4` instead -- padding 4 because we downscale ~6x and 1px bleeds
between glyphs under mipmapping.

This will bite the library's own default harder than it bites us.
`default_codepoints()` (`:27`) is 556 codepoints (95 + 96 + 24 + 112 + 128 + 96
+ 5 across its seven ranges) -- a strict superset of our 99, and a 5.6x bigger
atlas -- so the shipped default is the case most likely to overflow the packer
that is hardcoded alongside it.

**Repro:** `build_font_atlas(font, 96, default_codepoints(), 556)` and read
raylib's log.

**Suggested fix:** `padding` and `pack_method` as defaulted parameters. Two
lines, and it lets us delete 41 lines (`load_font_packed` + `ui_codepoints`)
and adopt the shared routine.

**Same shape, one line down:** both loaders end with a hardcoded
`SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR)` (`:89` and `:164`)
with no way to ask for another. Our three CJK-path call sites
(`src/systems/systems_internal.h:256/275/296`) each call
`load_font_from_file_with_codepoints` and then immediately `mipmap_font()` to
replace it with mipmaps + trilinear, so the library's filter set is dead work
every time. A pixel-art face wanting `POINT` has the same fight. Make it the
third defaulted parameter.

**Knock-on for us once it lands:** `ascii_objective`
(`src/systems/level_loading.cpp:446-460`) exists because "the UI font lacks the
arrow / em-dash / en-dash glyphs". That is already half stale -- our
`ui_codepoints()` loads U+2014 and U+2013, and only U+2192 is genuinely absent
-- and `default_codepoints()` covers all three, so that function dies too.

## 20. `setting_row` is styleable, but it clobbers the caller's font

*Low severity, and narrower than it first looks -- the first draft of this
section claimed the row's look was not overridable at all, which is false.*

`plugins/ui/setting_row.h` (486 lines) models exactly the data shape a settings
screen needs: `SettingRowValue = variant<bool*, pair<size_t*, vector<string>>,
float*, string>`. It **is** broadly restyleable: `slot_icon_config`,
`slot_label_config` and `slot_control_config` are merged into the built-in
defaults via `apply_overrides()` at seven sites, alongside `with_toggle_colors`,
`with_row_height`, `with_slider_width`, `with_icon_bg_color` and per-control
size knobs. Credit where due.

Three real holes remain, all of the "unconditional write" kind:

1. **`:188` `config.font_size = pixels(22.0f);`** -- not guarded on
   `is_default` the way `size` is at `:180`, so a caller's font size is
   silently discarded. It then propagates into the label (`:252`), stepper
   (`:306`) and display (`:359`). 22px violates our own >28px-at-1080p
   accessibility floor and there is no way to say so.
2. **`:252`** hardcodes the label to `UIComponent::DEFAULT_FONT`, ignoring
   `config.font_name` -- which the icon (`:217`) and stepper (`:306`) both
   honour. The label is the row's primary text, so it is the one that most
   needs the app's face.
3. **`:284`** applies `toggle_cfg.with_margin(...)` *after* the
   `apply_overrides` merge, so a caller's margin override is overwritten.

**Suggested fix:** guard 1 and 2 on `is_default` / empty, and move 3 above the
merge. Small, and it makes the difference between "restyleable" and
"restyleable including the text".

Separately, and by design rather than by bug: the slot configs restyle the
control but cannot *replace* it -- Toggle is always `toggle_switch`, Slider
always `slider`. Our screen is teal LCD readouts, segmented keycap chips and a
separate overlay pass for the fader fills, so we hand-build the five row kinds
in `src/systems/settings_systems.cpp` instead and do not call `setting_row`
anywhere. A widget-level hook would be a bigger ask than the three fixes above;
noting it as the reason adoption is not just a matter of styling.

---

## 21. `capture` only survives for UI draws -- the UI pass clears the buffer

Gap 2 shipped draw assertions and they work. But the buffer they read is
cleared **at the top of the imm UI render pass** (`ui/rendering.h:1856` and
`:2558`, both `if (capture::enabled()) capture::clear();`). Anything an app
draws before the UI pass -- which for a game is the entire world -- is
recorded and then discarded before a command can assert on it.

`capture.h`'s own header says it records "what the app drew, on whatever
backend it drew with", and `record()` is correctly wired into the shared
primitives: `draw_line_ex` records every line
(`backends/raylib/drawing_helpers.h:388`). The recording is right; the
**lifetime** is owned by one consumer of the buffer.

**Repro**, with real output from our tree. Load a level, wire two nodes so the
canvas draws ~40 bezier segments per cable through `draw_line_ex`, then run
`dump_draws line`:

```
[E2E] dump_draws 'line': 6 draws this frame
  | rectangle_rounded (1184,4) 88x22 #1e2128ff eid=-1
  | rectangle (1184,4) 88x22 #1e2128ff eid=-1
  | rectangle_rounded_lines (1184,4) 88x22 #2f333bff eid=-1
  | rectangle_outline (1184,4) 88x22 #2f333bff eid=-1
  | text (1211,9) 0x12 #c9d3d1ff eid=-1 'SUBMIT'
  | text (8,704) 0x12 #8a9099ff eid=-1 'v2026.08.w33'
```

Six draws, all of them the SUBMIT button and the version stamp -- the last
things painted, because they are the UI. Not one `line`, on a screen with a
visible patch cable. (Second, smaller bug visible here: the filter argument is
ignored by the dump listing -- `dump_draws line` printed rectangles and text.)

**This blocks the exact case gap 2 was filed for.** That entry is about cable
flow-pulses, whose entire content is rendering, and which we tested by
screenshotting to PNG and profiling pixel columns in Python. The pulses go
through `polyline::draw_dashed` -> `draw_line_ex`, in the canvas pass. They are
recorded, then cleared microseconds later. So the pixel archaeology that gap 2
declared obsolete is still the only way to test them.

**Suggested fix:** clear on a frame boundary, not inside a render system -- a
`capture::begin_frame()` the app calls at frame start, or a clear in
`begin_drawing`, so every pass shares one buffer. If the UI genuinely wants an
isolated view, that is `capture::mark()` returning an index to filter from, not
a clear that silently discards other passes' work.

**Also hits:** every consumer that draws outside imm. For a game that is the
whole world; for a word processor it is the document canvas, which is the part
worth asserting on. The behaviour is most misleading for exactly the apps that
need it most, because the feature looks like it works -- `dump_draws` prints
draws, assertions pass -- while being blind to the pass you care about.

## 22. `terminal::Console` history cannot be reset

Found 2026-10-31 running the suite against the new pin (not a bump
regression -- nothing in range touches `terminal/`; the run just made the
leak legible). `Console::clear()` (`plugins/terminal/console.h:57`) clears
the output deque and nothing else. `history_`, `distance_` and `draft_`
have no mutator at all -- the only consumer-visible handle on history is
the `history()` getter.

What we hit: console history leaks across e2e scripts in one process.
`reset_test_state` (`src/main.cpp:63`) resets progress, tutorial state,
bindings, camera, input layer and level between scripts, but it cannot
reset the console: there is no call that does it. In-suite,
`console_history.e2e` recalls `help` -- submitted by an earlier script --
instead of its own line. (That test has a second, game-side defect:
unknown lines never reach `submit()`, so its own input was never recorded
at all. The leak is what made the failure read `help` instead of empty.)

The only workaround is destroying the `Console` and re-registering every
command -- our `Terminal` wrapper re-arms registration on singleton
recreation (`src/systems/console_systems.cpp:350-364`), so it is possible,
but "recreate the widget to clear its history" is not a reset API.

**Repro:** submit a line, call `clear()`, press Up. The submitted line
comes back: output was cleared, history was not.

**Suggested fix:** `void clear_history() { history_.clear(); distance_ = 0;
draft_.clear(); }`, or fold history into `clear()` behind a defaulted flag.
All three fields must reset together, so Up-after-clear starts from the
live line rather than a stale draft and offset.

Related: there is also no way to *record* a history entry without
executing it -- `record()` is private and coupled to print/execute -- so a
consumer that reports unknown commands in its own words (as we do) cannot
keep them out of execution but in history. A public record step would close
that half too.

**Also hits:** any consumer reopening a console per document or per
session, and any consumer test asserting on recalled input. Severity is
low only because the destroy-and-reregister workaround exists.

## Design constraint on everything above: opt-in, and free when off

Several of these gaps ask the library to take on work it does not do today
(profiling, a log file, crash handling). None of them should cost anything to a
consumer who does not switch them on -- and "cost" means the disabled path too,
not just the enabled one.

**This document already contains the counter-example.** Gap 7 is
`capture::record` building a `std::string` per draw call *while disabled*, which
took `RenderNodeSystem` from 1.37 ms to 2.34 ms. The feature was off and we paid
for it anyway. That is the exact failure mode to design out of the new ones.

What that means concretely:

- **Profiler (9).** The hook is a null function pointer until someone sets it,
  so the check is a predictable branch. The `PROFILE()` macro compiles to
  nothing without its define -- which is how ours works today
  (`-DENABLE_PROFILING`), so the shipping build has no scope objects at all.
- **Crash handler (17).** `install()` must be an explicit call, never automatic
  on startup. Signal handlers are a process-global resource and an engine has no
  business claiming them from an app that wants its own -- or from a host
  process embedding it. Opt-in here is a correctness requirement, not just a
  perf one.
- **File sink (15).** Inactive until `log_open_file()` is called, and the
  pre-init buffer needs its hard cap precisely so a consumer who never calls it
  does not accumulate forever. That cap is the "free when off" part.
- **`VALIDATE` (16) is the one that inverts.** Here the *current* default is the
  one that costs -- a variadic function evaluates its arguments in every build,
  including release. A macro that expands to `((void) 0)` is strictly cheaper
  than what ships now. So making it real is not adding a cost, it is removing
  one, and the opt-out stays for anyone who wants their own.
- **`reveal` (5) and the wrapper holes (18)** are pay-per-call: nothing runs
  unless you call them.

The general rule we would want stated in the library: a feature that is off
should cost at most one predictable branch, and ideally not compile in at all.
Consumers develop at `-Og` (see gap 14D), where "the optimiser will erase it" is
not true.

## What we would upstream

Three pieces of `puzzle/` exist only because the library had nothing, are free
of game types, and would be better maintained upstream. Named here so the offer
is concrete rather than a suggestion someone else has to implement.

| ours | lines | gap it closes | note |
|---|---|---|---|
| `src/profiler.h` + `src/game_system.h` | 517 + 71 | 9 | the consumer half of the profile hook; already wired to `PerfProvider` |
| `src/log/log.h:66-96` (file sink) | ~30 | 15 | needs porting off our fmt fork onto `std::format` |
| `src/paths.h:39-59` (`reveal`) | 12 | 5 (the `reveal` half) | `open` / `explorer` / `xdg-open`; POSIX arg must be single-quoted |

Two more that are close but not offered, for honesty about what "reusable"
means here:

- **`src/rect.h`** (gap 10) is already written up as wanted upstream and is the
  cleanest of the lot, but its 613 call sites here mean we would be the ones
  churning, so it is a "when you are ready" rather than a handover.
- **`src/paths.h`'s `user()`/`dev()`** are NOT worth taking. `user()` is
  `get_save_path() / rel` and nothing else -- if anything, add that overload to
  `files`. `dev()` is our repo's `output/` convention, not engine policy.

## Not a gap, recorded so it isn't re-reported

- **`imm::text_input` for simple form fields** — we tried it and reverted. Not a
  defect; it was a lateral move for our use case. See
  `afterhours-text-input-feedback.md` reasoning in the memory notes.
- **Modifier matching is a subset test** (`Cmd+Shift+Z` also matches `Cmd+Z`).
  This is deliberate and documented, and `suppress_permissive_duplicates`
  handles the bare-key collision it implies — including over the *down* list,
  which is what made `Cmd+S` for Submit safe alongside `S` for pan.
