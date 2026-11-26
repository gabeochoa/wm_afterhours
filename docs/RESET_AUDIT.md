# Editable-screen reset audit

Owner ask: screens where you edit values should reset when you leave or
provide reset-to-default. Trellis screens are covered by PR #24 and were
excluded here; this audit covers the non-Trellis remainder (`GummyShop`
first).

## How recreation works (verified)

`src/game.cpp` rebuilds the active screen on every switch. Both `load_screen`
lambdas (lines ~486 and ~764) do:

```cpp
current_screen_system = ExampleScreenRegistry::get().create_screen(new_screen_name);
```

and the e2e `goto_screen` command drives the same `load_screen` (line ~1006).
`reset_e2e_state` (line ~1136) clears UI focus/modal/text/input queues between
scripts. Plain struct members of a screen system therefore reset on every
leave/return, in both the interactive demo and headless/e2e modes.

## Inventory method

From `~/w/wm-reset-audit` on `origin/main`:

- `grep -rn "motion::anim" --include="*.h" src/systems/screens/` — global
  motion tracks. 6 files (table below).
- `grep -rn "Settings::get().set\|Settings::get().toggle" src/` — writes to
  the persisted `Settings` singleton. 2 screens (table below).
- `grep -rn "static bool\|static int\|static float" src/systems/screens/`
  — mutable static member state: none (remaining hits are `static` helper
  functions and `static const` data).
- `grep -rn "EntityHelper::get_singleton_cmp" src/systems/screens/` —
  framework singletons (FontManager, TextMeasureCache, DragGroupState, ...):
  shared caches/input state, not screen-editable values. `DragGroupState`
  holds in-flight drag state; a drag interrupted by a screen switch is an
  edge case, not an edited-value leak.
- File-scope state: `HostLoopLab.h` atomic frame-loop counters
  (`host_loop_async_pending/done`) — frame-loop stats, not edited values;
  the lab re-asserts `frame_loop::set_mode(Continuous)` +
  `reset_stats()` on entry.
- `vendor/afterhours/src/plugins/animation/store.h`: `motion::anim(key)`
  (no entity arg) stores `Track`s in a `HasTracks` component on a permanent
  `MotionRoot` singleton — process-global, survives screen switches and
  e2e script boundaries (`reset_e2e_state` does not clear it).

## Motion-track screens

| Screen | Tracks | Seeded how | Verdict |
| --- | --- | --- | --- |
| `GummyShop.h` | HeartFill, TogFront/TogBack, SliderBlob, SegFront/SegBack, ProgFill | `if (!track.started()) from(...)` — **process-global gate** | **LEAK, fixed.** Tracks persisted across switches: size pill and toggle knob kept the previous visit's position (proven by e2e: pill `x=817` instead of default `544` after leave/return). Slider blob and progress bar share the same global tracks; blob/prog had resync loops so they self-corrected within ~1 s, seg/tog/heart had none and persisted indefinitely. Heart color likewise showed the previous visit's liked fill. |
| `AnimationInteractiveDemo.h` | 3 button scales | `first_run` **member** → `from(1.f)` per instance | Resets by member-driven reseed. |
| `AnimationBasicDemo.h` | FadeIn, SlideIn, ScaleUp | `first_run` **member** | Resets by member-driven reseed. |
| `AnimationStressLab.h` | Phase (ping-pong loop) | `started` **member** | Resets by member-driven reseed. |
| `ColourTrackLab.h` | Progress, Alpha, Row0..4 | `started` **member** → `restart()` reseeds all tracks | Resets by member-driven reseed. |
| `TransitionsLab.h` | ~45 audition tracks | only inside `reset_examples()` (Reset button) | Persists across switches, but this is a motion-audition lab, not an edited-value screen; it provides an explicit Reset that reseeds every track. Left untouched to keep this change minimal (bulk-edit prohibition); same one-line member-gated reseed could be added if desired. |

## Fix (GummyShop only)

`src/systems/screens/GummyShop.h`: new `bool tracks_seeded` member; on the
first frame of each screen instance, `from()` re-seeds all seven tracks from
the member defaults (heart `red_core`, toggle `1.f`, blob `volume=1.f`,
seg `0.f`, prog `0.f`). The five `if (!track.started())` guards were
removed. First-entry behavior is unchanged (the old guards seeded the same
defaults on first launch); there is no default-layout/visual change, and no
draw lambda was touched.

## Plain-member editable screens (reset by recreation, no fix needed)

All other editable screens store their state in plain members, which reset on
recreation. Audited examples (checked for globals/motion tracks/Settings
writes; none found):

- `GummyShop.h` plain state: `flavor`, `toggle_on`, `volume`,
  `slider_dragging`, `size_index`, `qty`, `extras`, `gift_check`, `liked`,
  `likes`, `rating`, `elapsed`, texture/effect handles (reloaded per
  instance). Verified by the leave/return e2e text assertions.
- Settings-mock screens: `AngryBirdsSettings.h`, `SportsSettings.h`,
  `PowerWashSettings.h`, `ParcelCorpsSettings.h`, `DeadSpaceSettings.h`,
  `IslandsTrainsSettings.h`, `MiniMotorwaysSettings.h`, `KirbyOptions.h`,
  `CozyCafe.h`, `EmpireTycoon.h`, `ShopInterface.h`, `ModalAdvanced.h`,
  `SecureTunnelMockup.h`, … (state such as `music_on`, `current_level`,
  `saved_level`, slider values; plain members).
- Showcases/labs with sliders, steppers, toggles, text inputs
  (`Forms.h`, `SmallComponentsLab.h`, `ToggleSwitchShowcase.h`,
  `CheckboxShowcase.h`, `TextInputLab.h`, `TextInputDemo.h`, `Themes.h`,
  `ThemeBuilderLab.h`, `SettingRowShowcase.h`, `ConfigGapGallery.h`,
  …). Any unlisted showcase follows the same pattern: recreation +
  no global/static/Settings state found by the greps above.

## Intentionally persistent (excluded by design)

- `TransitionsLab.h`, `MotionButtonCheckpoint.h`: write the global
  `reduced_motion` preference via `Settings::get().set_reduced_motion_enabled`.
  App-level preference persisted in `settings.json` — not a screen value to
  reset.
- `Settings` singleton resolution/volumes/fullscreen/post-processing:
  app-shell preferences, loaded/saved by `main.cpp`/`Settings::load_save_file`
  + `write_save_file`. App-level, not per-screen.

## Test

New `tests/e2e_scripts/gummy_shop_reset.e2e`: edits flavor, toggle, size,
qty (×2), chips, gift, heart, and drags the slider; leaves to `buttons`;
returns to `gummy_shop`; asserts defaults via `expect_text`/`expect_no_text`
(qty back to 1 — no `3`; likes back to 12 — no `13`; `$4.50` and
`Free delivery at $25` present) and `assert_ui` rects of the size pill
(`gummy_seg_pill` x=544), toggle knob (`gummy_toggle_knob` x=1160), and
slider knob (`gummy_slider_knob` x=1152). Pre-fix run failed with
`assert_ui 'gummy_seg_pill': x=544 but got 817`; post-fix it passes.
