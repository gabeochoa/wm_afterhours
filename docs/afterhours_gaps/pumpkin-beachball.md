# Afterhours gaps and adversarial rendering review

Project: Pumpkin Beachball (`~/p/pumpkin-beachball`), at commit `592a719`.
Build surface reviewed: native raylib game (not a browser build), 1280x720
logical field rendered through a 2x camera into a 2560x1440 canvas.

This is a findings document only. No code was changed for this review.

Resolution status (updated later): findings 1-2 fixed in `7d74bc9`
(ellipsized menu rows, wider MatchEnd rows, Pause-style panel),
finding 3 in `77a0332` (top-edge offscreen triangle), findings 4-6 in
`f04fbcd` (full-width Dock + sprite-aware wall clamp, an initially
env-gated FPS readout, real local clock + shadowed HUD text). The FPS
counter later became visible by default in `0677d80`;
`PUMPKIN_SHOW_FPS=0` hides it. Menu button subtitles were then removed
entirely in `47e334b`. Finding 7 accepted as slapstick. Gap G got a
game-side workaround in `79fbdc0` (hand-rolled target text field; see
L). A full-code audit follows the A-H entries; library gaps remain
open unless an entry says otherwise. Online-specific gaps now have a
separate tracker in `docs/afterhours-online-gaps.md`. A second
audit round (T-Z plus ON-1 to ON-6) is appended at the end.

## How the evidence was captured

Commands run from the repo root:

```sh
make e2e
./build/make/slime --e2e --test-script /tmp/worst_case.e2e \
  --screenshot-dir build/e2e-screenshots --headless
./build/make/slime-online --join 192.168.1.42 --port 7777 --e2e \
  --test-script /tmp/worst_case_online.e2e \
  --screenshot-dir build/e2e-screenshots --headless
```

Results:

- `make e2e`: unit tests pass; all 12 E2E scripts pass on the NONE/headless
  stage and on the raylib headless stage.
- 36 PNG screenshots in `build/e2e-screenshots/`, all 2560x1440 RGBA:
  26 standard E2E shots, 8 worst-case roster/physics/HUD shots
  (`wc_*.png`), and 2 online-menu shots.
- The online shots were captured without a server listening, so they cover
  the "connecting / cancel / offline menu" states, not a live online rally.
- Screenshots are captures of the internal 2560x1440 render-target canvas,
  not of the final 1280x720 window presentation. Coordinates below are
  logical 1280x720 units unless stated otherwise.

## Verdict

Rendering is mostly correct. Sprites, fonts, panels, character select, HUD,
pause, campaign, and match-end states all draw. No missing-texture
placeholders appeared in the raylib screenshots.

The worst real visual bug is menu text overlap on the champion screen.
The worst Afterhours-side issues are test/render semantics: screenshots can
fail in the NONE backend while E2E still reports PASS, text measurement
silently returns zero there, and 2D triangle winding differs between the
recording backend and raylib.

## Confirmed rendering defects (ranked)

### 1. Menu label and hint overlap on narrow rows

- Evidence: `build/e2e-screenshots/champion.png` — the selected
  `PLAY AGAIN` label collides with its hint, `New run at all five
  challengers`; the two strings render on top of each other mid-row.
- Source: the item is defined at `src/main.cpp:2168-2170`; MatchEnd rows
  are only 400px wide (`src/main.cpp:2209-2210`); the renderer draws the
  label left-aligned at `rect.x + 44` and the hint right-aligned
  independently (`src/main.cpp:2250-2255`). Neither side reserves width
  for the other, and there is no truncation.
- Inferred, not screenshot-confirmed: the online-client match-end item
  `WAITING FOR HOST` / `Host controls the rematch`
  (`src/main.cpp:2164-2167`) uses the same 400px row with an even longer
  label, so it should overlap worse.
- Suggested fix (game): shorten the hint, widen MatchEnd rows, stack the
  hint under the label, or measure both strings and ellipsize the hint
  (then the label) before drawing.
- Library angle: the raw text helpers in
  `vendor/afterhours/src/drawing_helpers.h:21-62` only offer left /
  centered / right-aligned / shadowed draws. The full UI renderer already
  has ellipsis truncation (`vendor/afterhours/src/plugins/ui/rendering.h:1003-1038`),
  but raw-draw games cannot reuse it. A bounded
  `draw_text_fitted(...)` / `ellipsize_text(...)` helper for the raw API
  would prevent this class of bug.

### 2. Match-end screens have no backing panel; the net shows through Quit

- Evidence: `build/e2e-screenshots/win.png`,
  `build/e2e-screenshots/level_clear.png`, and
  `build/e2e-screenshots/champion.png` — headline text and the
  Rematch / Main Menu / Quit buttons float directly over the live court.
  The center net pole runs through the button column and is visible
  through the Quit button.
- Source: the MatchOver and CampaignOver branches draw only text plus
  `draw_menu_items(state)` (`src/main.cpp:2078-2108`); unlike Main,
  HowTo, Opponents, Online, and Pause, they never call `draw_panel`.
  Unselected buttons are drawn at alpha 238 (`src/main.cpp:2231-2234`),
  so the white net drawn earlier in `draw_court` bleeds through. Render
  order itself is correct (court, actors, HUD, overlay at
  `src/main.cpp:2773-2777`); this is translucency plus a missing
  container, not a z-order bug.
- Suggested fix (game): give MatchEnd the same treatment as Pause —
  dim the court and put the result text and buttons in a `draw_panel`
  macOS window. At minimum, make match-end buttons fully opaque.

### 3. An offscreen ball is nearly untrackable

- Evidence: `build/e2e-screenshots/ball_offscreen_top.png` and
  `build/e2e-screenshots/wc_ball_way_offscreen.png` — the ball is above
  the screen and the only cue is a small dark smudge on the Dock under
  the net pole.
- Source: the no-ceiling rule is intentional (`src/main.cpp:674-675`).
  The ball shadow shrinks and fades with height
  (`src/main.cpp:854-859`: radius and alpha both decrease as `height01`
  approaches 1), which is exactly backwards for readability when the ball
  itself is invisible, and the shadow lands on the dark Dock / net
  divider.
- Suggested fix (game): clamp a minimum shadow size/alpha, brighten the
  shadow, or add a top-edge offscreen indicator (arrow or pinwheel glyph)
  at the ball's x while `ball.y < 0`.

### 4. The visible Dock floor is narrower than the playable floor

- Evidence: `build/e2e-screenshots/wc_walls.png` — at the walls, Dogcow's
  rear half and Football's right tip stand past the rounded ends of the
  Dock on invisible floor; the identity rings extend beyond the Dock and
  sprite tips touch the screen edge.
- Source: the Dock spans x=64..1216 (`src/main.cpp:1745`), but movement
  clamps only by collider radius (`src/main.cpp:524-529`, also
  `src/main.cpp:1241` for prediction) across the full 1280 width. Sprite
  width comes from texture aspect times `draw_height`
  (`src/main.cpp:785-801`), which for wide characters in
  `src/slime_rules.h:69-90` (Dogcow, Turkey) is wider than the collider
  diameter, so art can overhang both the Dock end and, by a few px, the
  screen edge.
- Suggested fix (game): extend a floor surface to the full court width
  (keep the Dock styling inset if desired), and/or clamp movement by
  `max(radius_x, sprite_half_width)`.

### 5. FPS counter ships in the 2P HUD and is nondeterministic in tests

- Evidence: the top-right FPS readout varies wildly across screenshots
  from the same deterministic E2E run — e.g. `two_player.png` (1446),
  `paused.png` (723), `jump.png` (187), and the worst-case set (66-82).
- Source: `draw_hud` draws `graphics::get_fps()` for every non-1P mode
  (`src/main.cpp:1887-1891`). The raylib backend forwards to
  `raylib::GetFPS()` (`vendor/afterhours/src/backends/raylib/backend.h:230`),
  a wall-clock value, while E2E only fixes the simulation dt
  (`vendor/afterhours/src/plugins/e2e_testing/app.h:98-100`). Target is
  200 FPS with VSync hint (`src/main.cpp:2726-2728`), so the number is
  neither a stable test artifact nor meaningful player UI.
- Suggested fix (game): remove the FPS readout from the player HUD or
  gate it behind a debug flag/env var. Library angle below.

### 6. Menu-bar clock is fake game time; level text washes out

- Evidence: `build/e2e-screenshots/wc_hud_extremes.png` — with match time
  forced to 754s, the macOS menu bar reads `12:12:34` while the HUD timer
  reads `TIME 12:34`; in menus it sits at `12:00:00`. The `LEVEL 5` text
  at top-right is nearly invisible over the bright wallpaper.
- Source: `draw_menu_bar` hard-codes `Sat Nov 1` and derives the clock
  from `menu_time`/`match_time` modulo 3600 (`src/main.cpp:1780-1788`),
  so it resets per match and wraps every hour. The 1P level line is drawn
  in plain white with no shadow or backing (`src/main.cpp:1878-1881`),
  unlike the shadowed center headlines.
- Suggested fix (game): either use real local time for the fake menu bar
  or make it obviously a match clock; add shadow/backing to the level
  line (and audit other top-edge HUD text over bright wallpaper).

### 7. Long characters balance on the net by their front feet only

- Evidence: `build/e2e-screenshots/wc_dogcow_on_net.png` — Dogcow stands
  on the net cap with front feet on the pole and rear feet hanging in
  air.
- Source: net landing snaps the body bottom anchor to the pole top
  (`src/main.cpp:461-480`) and sprites are bottom-anchored as one rigid
  quad (`src/main.cpp:785-801`), so any sprite wider than the 18px pole
  overhangs. Physics and collider agree; only the pose reads wrong.
- Severity: cosmetic. A slight teeter/tilt while net-standing, or
  accepting it as slapstick, are both fine. Noted so it is a decision,
  not a surprise.

## Unintuitive behavior / Afterhours gaps

### A. E2E screenshot failure does not fail the test (NONE backend)

During `make e2e`, the NONE/headless stage logged
`e2e: screenshot '<name>' failed to capture` for every screenshot and
still reported all 12 scripts as PASS. Source:
`E2EApp::post_render` only `log_warn`s when
`capture_render_texture_png` returns nothing
(`vendor/afterhours/src/plugins/e2e_testing/app.h:146-150`), the NONE
backend always returns `std::nullopt`
(`vendor/afterhours/src/backends/none/drawing_helpers.h:280-281`), and
`finish()` grades only the runner (`app.h:161-164`).

A suite that asks for screenshots and gets none should fail, or the
command should have an explicit "optional in this backend" spelling.
Right now a logic-only stage silently masquerades as visual coverage;
this project only gets real screenshots because the Makefile also runs
the raylib binary.

### B. Text measurement returns {0,0} in the NONE backend

`measure_text` in the NONE backend returns `{0,0}` unless the game
installs `set_measure_text_fn`
(`vendor/afterhours/src/backends/none/font_helper.h:57-65`); the header
comment itself notes this made overflow branches silently take the
"it fits" path (`font_helper.h:34-44`). The headless run of this game
emits the warning once and then every `measure_text`-based centering and
right-alignment in that build is computed from zero widths, while tests
still pass because nothing asserts pixels.

Confusingly, the NONE `PlatformAPI::measure_text(const char*, int)` does
return a `strlen * size / 2` estimate
(`vendor/afterhours/src/backends/none/backend.h:109-111`) — two APIs with
the same name and different failure modes. A deterministic built-in
approximation for the free function (even the strlen estimate) would make
headless layout tests meaningful instead of vacuous.

### C. Triangle winding: NONE records it, raylib culls it

`draw_triangle` forwards straight to `raylib::DrawTriangle`
(`vendor/afterhours/src/backends/raylib/drawing_helpers.h:525-531`),
which culls one winding order, while the NONE backend records the
bounding box regardless of winding
(`vendor/afterhours/src/backends/none/drawing_helpers.h:217-222`). This
already bit this project once: the character-select left arrow was
invisible until vertices were manually ordered counter-clockwise
(comment at `src/main.cpp:1950-1957`). A headless draw-call test would
have sworn the arrow existed.

The wrapper should normalize winding (swap two vertices when the signed
area is negative) or at least document/assert the required order in
debug builds, so the backends cannot disagree about whether a triangle
is visible.

### D. Texture loads log errors and have no usability query in NONE builds

The NONE `load_texture` logs `@notimplemented load_texture` and returns
an empty texture (`vendor/afterhours/src/backends/none/drawing_helpers.h:291-294`),
once per asset — 12 errors at startup for this game — and the game must
infer "no textures in this backend" from `tex.width <= 0`
(`src/main.cpp:758-781`). Fonts have `font_is_usable()` for exactly this
(`vendor/afterhours/src/backends/none/font_helper.h:17`,
`vendor/afterhours/src/backends/raylib/font_helper.h:347`); textures have
no equivalent, and "backend cannot load" is indistinguishable from
"file missing". A `texture_is_usable()` / capability query, or downgrading
the NONE load to a once-only warning, would make expected fallback paths
quiet and real missing files stand out.

### E. No first-class deterministic-render / test-mode signal for HUDs

E2E fixes simulation dt (`app.h:98-100`) but `graphics::get_fps()` and
`get_time()` remain wall-clock in every backend
(`vendor/afterhours/src/backends/raylib/backend.h:230-231`,
`vendor/afterhours/src/backends/none/backend.h:100-106`), so any HUD that
draws them makes screenshots nondeterministic (finding 5). A test-mode
flag exists, but it lives in the input-injection plumbing
(`vendor/afterhours/src/plugins/e2e_testing/platform_test_input.h:36-41`)
rather than as an app/render-level "you are being screenshotted" query.
Exposing something like `testing::is_e2e()` next to `E2EApp::attach`, or
documenting that HUDs should consult it, would help games keep debug
readouts out of golden screenshots. Separately, the E2E perf commands use
their own profiling collector for FPS
(`vendor/afterhours/src/plugins/e2e_testing/perf_commands.h:53-57`), so
"the FPS the test asserts" and "the FPS the game draws" are different
numbers from different clocks.

### F. Screenshots capture the canvas, not the presented frame

`E2EApp` reads `graphics::canvas()` (`app.h:101`), and with
`canvas_width/height` set that is the supersampled render target
(`vendor/afterhours/src/drawing_helpers.h:77-110`). Here that means
2560x1440 PNGs of a game whose window is 1280x720 and whose E2E mouse
coordinates are logical. That is useful for crisp inspection, but it is
not what a player sees after letterboxing/downsampling, and the size
doubling is easy to misread as a scaling bug. Worth stating in the E2E
docs, and ideally offering a presented-frame capture mode.

### G. Text input exists, but only inside the full UI system

Joining an online game still requires quitting to a shell: the Online
screen tells the player to relaunch with `--join <ip> --port <n>`
(`build/e2e-screenshots/wc_online_charselect.png`;
`src/main.cpp:2020-2027`, target set only from argv at
`src/main.cpp:2706-2716`). Afterhours does have a capable `text_input`
widget (`vendor/afterhours/src/plugins/ui/text_input/text_input.h:20-42`),
but it requires the immediate-mode UI context and entity tree
(`text_input(HasUIContext auto&, EntityParent, ...)` in
`vendor/afterhours/src/plugins/ui/text_input/component.h:106-108`).
This game — like the raw `drawing_helpers.h` text API it is built on —
draws its menus by hand, so the only path to an IP field is rolling its
own editing on top of low-level `input::get_char_pressed()`
(`vendor/afterhours/src/plugins/input_system.h:153-159`). A small
raw-draw text-field helper (rect + string + cursor state), or an example
showing the get_char_pressed pattern for custom menus, would close the
gap between "raw drawing" games and the UI plugin.

### H. No wall-clock/date in the platform API

The platform surface exposes only elapsed `get_time()` (see E), so a game
themed as a desktop OS cannot ask Afterhours for local date/time and ends
up faking it from match time (finding 6). `std::chrono` is available to
the game directly, so this is minor, but a cross-backend local-time
helper would keep platform differences (especially future backends)
inside the library.

## Full-code audit (2026-11-05): what this game hand-rolls that afterhours could provide

Scope: `src/` (4,967 lines), `tests/online_e2e.py` (1,199 lines), build
files. Each candidate was checked against the pinned library before
being written up. Things the library already does well are listed at
the end of this section so they are not re-asked for.

### Updates to A-H from later work

- D (texture usability): character wall clamps need sprite widths in
  the NONE build, where textures never load, so `CharacterDef` now
  duplicates every PNG aspect ratio as data (`sprite_aspect`,
  `wall_half_extent`, `src/slime_rules.h:83`). A metadata-only image
  size query (header parse, no GPU upload) would delete the table.
- E (deterministic render signal): windowed online E2E scripts count
  frames (`wait_frames 6000`), which is about 1 s of NONE fast-forward
  and about 100 s of vsync-paced wall clock. The harness needed a
  windowed-only timeout multiplier (`exit_timeout`,
  `tests/online_e2e.py`) because frame count is not a unit of time in
  windowed E2E. A `wait_seconds` primitive (sim time headless, wall
  time windowed) would fix this class of test.
- F (canvas vs presented): the game still reverse-engineers the
  presenter in `logical_mouse_position` (`src/main.cpp:2939`), screen
  size, letterbox offsets, and render scale recomputed by hand. The
  ask stands: expose presented size and a screen-to-canvas point map.
- G (raw-draw text input): worked around game-side (see L). The
  library gap stands.
- H (wall clock): the menu bar now uses `std::localtime` game-side
  (`draw_menu_bar`). There is still no library date/time API.

### I. No lightweight frame-rate statistics or periodic FPS logging

Severity: low. Effort: small.

Evidence: the game hand-rolls a `--log-fps` average logger
(`FpsLogger`, `src/main.cpp:1377`), a steady-clock window that prints
`[fps] frame avg N fps over 2.0s`. `plugins/profiling.h` computes
`Snapshot.fps` with p50/p95/p99, but it is an opt-in per-system
profiler (`AFTERHOURS_ENABLE_PROFILING`, history vectors, snapshot
polling). There is no frame counter a game can read for a HUD or a
log line. `graphics::get_fps()` is instantaneous only.

Suggested fix: a small always-available frame-stats read in
`graphics` or `frame_loop`: frames and wall seconds in the trailing
window (or since reset), so average FPS is one division. Optionally a
built-in periodic log sink on top of it.

### J. No fitted/ellipsized text draw (formalizing finding 1)

Severity: medium. Effort: small.

Evidence: `ellipsize_text` in `src/main.cpp` clips strings with
repeated `measure_text` calls so labels and ability blurbs cannot
overdraw their rows. It is about 15 lines every raw-draw game will
rewrite.

Suggested fix: `draw_text_fitted(font, text, rect, size, color)` and a
measure-only ellipsize helper in `font_helper.h`, with the NONE
backend estimating the same way it estimates measurement today.

### K. No list-menu navigation for raw-draw games

Severity: medium. Effort: medium.

Evidence: this game's title, pause, and match-end menus are about 570
lines of hand-rolled state (`src/main.cpp:2665-3268`): item tables,
per-screen layouts, disabled-item skipping in both directions,
keyboard plus mouse-hover plus mouse-moved tracking, gamepad stick
edge detection, shortcuts, back indices. `plugins/ui/menu.h` exists,
but it is dropdown and context menus inside the full retained UI.
Adopting it means adopting the UI system, layouts, and theming.

Suggested fix: a header-only list-menu helper in the raw-draw family:
items in, selected index and activated index out, given a rect.
Keyboard, mouse, and gamepad handled the way `input_system` already
abstracts actions. Drawing stays the game's; it gets a rect per item.

### L. Text entry for raw-draw games (extends G)

Severity: medium. Effort: small-medium.

Evidence: the Online screen's join-address field is hand-rolled
(`handle_target_edit`, about 45 lines): char queue draining,
backspace, replace-on-first-keystroke, blink cursor, gamepad
done/cancel, plus edit-state plumbing in `GameState`.
`plugins/clipboard.h` exists, so even paste could be one call if a
field existed.

Suggested fix: a `text_edit` state struct with an update function in
the input/raw-draw family (chars, backspace, delete, arrows, home/end,
clipboard paste) that returns the edited string. Rendering stays with
the caller, like other raw-draw helpers. E2E already has the `type`
command to drive it.

### M. E2E is single-process; multi-process orchestration is hand-rolled per game

Severity: medium, for any networked game. Effort: medium.

Evidence: `tests/online_e2e.py` is 1,199 lines of Python doing what
the E2E runner cannot: spawn N game processes, allocate free UDP
ports, barrier on log lines (`wait_for_log`), harvest per-process
logs on failure, and sequence join orders. `e2e_testing` itself is
one process, one script.

Suggested fix: a companion multi-process runner in `e2e_testing`
(C++ or a shipped Python module): declare processes with argv and
script, `wait_log(proc, needle)`, `wait_exit`, unified failure tails.
Every networked afterhours game currently rewrites this file.

### N. No fixed-timestep option for the game loop

Severity: low-medium. Effort: small-medium.

Evidence: three step regimes by hand. Variable `dt` in the windowed
update, a manual 60 Hz `sleep_until` loop for dedicated
(`src/main.cpp:3526`), and the E2E runner's own fixed dt.
`frame_loop` offers Continuous/OnDemand request coalescing, not a
fixed-step accumulator.

Suggested fix: `RunConfig.fixed_dt`, an accumulator with a max-steps
clamp, so windowed, headless, and dedicated all step the sim the same
way and E2E frame counts mean the same thing everywhere (see E).

### O. Gamepad stick-edge helpers and E2E gamepad injection are missing

Severity: low. Effort: small.

Evidence: stick-as-dpad edge detection is hand-rolled twice
(`menu_stick_y`, per-pad `select_stick_x` in `src/main.cpp`), each
with its own previous-axis state. In E2E, `test_input.h` queues keys,
chars, and mouse, but no gamepad buttons or axes. Every gamepad path
in this game (menu nav, per-pad select browse and ready) is
untestable without hardware, which this machine does not have.

Suggested fix: an `is_gamepad_axis_pressed(pad, axis, dir)` edge
helper in `input_system`, and `pad_button` / `pad_axis` commands in
the E2E runner feeding the same injection queue.

### P. Settings persistence exists but is heavyweight for small games

Severity: low. Effort: small.

Evidence: this game persists nothing. Join target, characters, and
battle type reset every launch. The one settings plugin requires
compiling `settings.cpp` in and pulls nlohmann/json by default
(`plugins/settings.h:14`). For a single-translation-unit game that is
a real adoption cliff for three strings.

Suggested fix: a header-only string-to-string settings file under the
same folder conventions as `files.h`, with no JSON dependency. Keep
the full settings plugin for structured data.

### Q. (afterhours-online) No snapshot-interpolation or send-pacing helpers

Severity: medium for the next networked game. Effort: medium.

Evidence: the game hand-rolls the standard client recipe. A snapshot
deque sampled about 100 ms in the past (`sample_snapshots`,
`src/main.cpp:1459`, buffer at `src/main.cpp:1357`), inputs paced to
30 Hz by wall clock, and 20 Hz snapshots. The wall-clock pacing was
learned the hard way: fixed-dt E2E loops otherwise flood the
connection. afterhours-online provides transport, serialization, and
fault injection, but every game rewrites the buffer, the lerp, and
the pacing.

Suggested fix: in afterhours-online, a header-only
`SnapshotBuffer<T>` (push with server tick/time, sample(delay) with a
caller-supplied lerp) and a wall-clock `RateGate` for send pacing.
Both are transport-agnostic and unit-testable like the existing
matrix.

### Checked during the audit: already provided, not gaps

- Rounded rectangles: `draw_rectangle_rounded` exists in both the
  raylib and NONE drawing helpers. This game still composes its own
  from rects and fans (`draw_rounded_rect`). That is a discoverability
  miss in the game. A pointer from the `drawing_helpers.h` docs is
  the most this needs.
- Sound loading: `LoadSound` and `sound_system` exist. The game's
  synth beeps are a content choice.
- Resource paths: `plugins/files.h` has resource folders and groups.
  The game uses CWD-relative paths by choice.
- Profiling percentiles: exist in `plugins/profiling.h` (see I for
  the lightweight-read gap).
- Crash handler, E2E seed, input action mapping: used heavily in this
  game and worked as documented.

## What held up

- All 12 E2E scripts pass on both backends; unit tests pass.
- Text and sprites are crisp: fonts rasterize at 128px/96px with mipmaps
  and all 12 PNG assets load at 640px wide in the raylib run — no repeat
  of the old "everything is blurry" problem.
- Character select is solid: both arrows render, `PLAYER 1/2 - CHOOSE`
  titles are correct (`character_select.png`, `character_select_p2.png`),
  stat bars and pagination dots fit the panel.
- Main menu, How to Play, Opponents, Online, and Pause panels keep their
  contents inside the window chrome; the disabled Online item in a
  non-GNS build is clearly dimmed with a reason (`online_disabled.png`).
- The online target line fits its panel at a realistic LAN address
  (`wc_online_long_target.png`), and the connecting state offers a
  working Cancel.
- Worst-case HUD values (5:5, level 5, 12:34 timer) do not overflow;
  only contrast and the fake clock look wrong (`wc_hud_extremes.png`).
- Ball/sprite contact reads correctly across the roster, including wide
  and tall colliders (`wc_pilgrim_vs_turkey.png`, `wc_pie_vs_finder.png`,
  `wc_compass_vs_football.png`, `wc_ball_on_turkey.png`); size differences
  between characters are intentional per-character colliders.
- Crossing sides, jump-serve, and net-standing all render as designed
  (`crossed_net.png`, `jump_serve.png`, `wc_dogcow_on_net.png`).

## Needs a decision

1. Should a failed screenshot capture fail E2E (gap A), with the NONE
   stage explicitly relabeled logic-only in the Makefile?
2. Match-end: adopt the Pause-style dim + panel (finding 2), or keep the
   floating-buttons look with fully opaque buttons?
3. Floor: full-width ground, or clamp by sprite width (finding 4)?
   Doing both is safest.
4. Is the FPS readout player-facing or debug (finding 5)? Recommend
   debug-only.
5. Is in-game IP entry required before online is called shippable
   (gap G)? It is the only menu flow that currently requires a shell.

## Not verified by this review

- Physical gamepad input and gamepad menu navigation (no controller
  attached; covered only by shared input code paths).
- Real windowed presentation: resizing, fullscreen, letterboxing, and
  Retina downsampling of the 2x canvas (screenshots bypass presentation).
- A live online rally's rendering (only connecting/menu states captured
  here; the loopback harness covers online logic separately).
- Animation quality over time (squash/stretch, ball spin), audio, and
  color-vision accessibility beyond eyeballing contrast.
- Windows/Linux rendering; all captures are macOS raylib headless.

## Later status updates (2026-11-05)

The review above is a snapshot at `592a719`; the notes below record
what changed after it without rewriting that evidence.

Decision outcomes:

1. Screenshot-failure policy: still undecided. The NONE stage still
   warns that screenshots failed to capture and continues; it has not
   been relabeled logic-only in the Makefile. Gap A remains the live
   decision.
2. Match-end presentation: decided and implemented as dim + Pause-style
   panel (`7d74bc9`).
3. Floor: both halves were implemented (`f04fbcd`): the Dock spans
   nearly the full court and movement clamps by the larger of collider
   and sprite width.
4. FPS: the debug-only recommendation was superseded by an explicit
   later request for an FPS counter. It is player-facing by default in
   `0677d80`; `PUMPKIN_SHOW_FPS=0` is the deterministic-screenshot
   opt-out. `--log-fps` separately prints a two-second average.
5. In-game IP entry: implemented (`79fbdc0`). A menu flow no longer
   needs a shell to change the direct-IP target, though building the
   separate online binary still does.

Superseded evidence and verification:

- There are now 17 game E2E scripts, not the 12 counted by the review.
- Character select is no longer sequential. Two local players browse
  side-by-side panels and ready independently (`5c25ebb`); the old
  `PLAYER 1/2 - CHOOSE` screenshots describe the review revision only.
- Menu button subtitles were removed after the overlap fix
  (`47e334b`). The disabled Online row is still dimmed and skipped in a
  non-GNS build, but it no longer carries the explanatory subtitle
  quoted as held up above. That is an accepted copy tradeoff, not a
  rendering regression.
- A live online rally has since been captured in real raylib client
  processes by `dedicated_windowed` and `listen_windowed`, including an
  `ONLINE - CLIENT - 15 MS` HUD sample. Those captures still use the
  E2E screenshot path, so final OS presentation, resizing, fullscreen,
  and Retina downsampling remain unverified.
- Physical gamepad input, animation/audio quality over time,
  color-vision accessibility, and Windows/Linux rendering remain
  unverified. Online-specific follow-up lives in
  `docs/afterhours-online-gaps.md`, especially OG-12.

## Windows port notes (2026-11-14)

Found while bringing the `windows` branch up (`make` and `zig build`
on native Windows, clangd editor config). Lettering continues A-Q.

### R. `<cxxabi.h>` is guarded by compiler, not by the header's existence

Severity: low (breaks only non-GNU-STL Windows setups). Effort: tiny.

Evidence: the include block
(`vendor/afterhours/src/core/system.h:15-18`) and the matching use in
`detail::demangle_type` (`system.h:106-114`) were guarded by
`#if defined(__GNUC__) || defined(__clang__)`. Clang targeting MSVC
always defines `__clang__`, but MSVC's STL ships no `<cxxabi.h>`, so
any parse with clang + MSVC STL — clangd without a GNU sysroot, a
direct `clang++` invocation — dies at the include
(`pp_file_not_found`) and the cascade hides everything downstream.
Reproduced with `clangd --check=src/main.cpp` on a compilation
database using clang's default (MSVC) target. The project's own zig
builds never saw it: zig bundles a libc++ that provides the header,
and MinGW GCC/libc++ setups have a working `__cxa_demangle`.

Suggested fix: guard both sites with `#if __has_include(<cxxabi.h>)`
instead. The return-mangled fallback already exists below the
`#endif`, so this changes behavior nowhere it compiles today, keeps
demangling under MinGW, and fixes MSVC-STL consumers. The blunter
`&& !defined(_WIN32)` (the `crash_handler.h:17` idiom) would also
compile but would needlessly silence demangling on MinGW.

Status: fixed in the `vendor/afterhours` working tree on this branch
(uncommitted); needs upstreaming to afterhours.

### Checked on Windows: already fine, not gaps

- `crash_handler.h:17` already excludes `_WIN32` from the
  `<execinfo.h>` block, so the build needed no shim. The repo still
  vendors cartographer's `platform/windows/execinfo.h` defensively
  (on the include path, unreferenced).
- `type_name.h`'s `__PRETTY_FUNCTION__` branches and `entity.h`'s
  diagnostic-pragma branches parse and compile under clang with both
  MSVC STL and GNU STL.
- `make e2e` passes on Windows (19/19 headless, 19/19
  raylib-headless). Gaps A, B, and D reproduce there unchanged
  (screenshot warnings, the measure-text warning, per-asset
  `load_texture` errors), so their backend semantics are not
  macOS-specific.

### S. Concept requirements use C-style compound literals

Severity: low (breaks only strict compilers). Effort: tiny.

Evidence: the backend concepts in
`vendor/afterhours/src/graphics_common.h` spelled placeholder
arguments as `(const char *){}` / `(const void *){}` (six sites).
Clang accepts that in an unevaluated requires-expression, but GCC 16
rejects it under `-Wpedantic` (`-Werror=pedantic`: ISO C++ forbids
compound-literals), which broke the game's mingw64 online build.

Suggested fix: `std::declval<const char *>()`, already used
elsewhere in the same file. Semantically identical in unevaluated
context, accepted by every compiler.

Status: fixed in the `vendor/afterhours` working tree on this branch
(uncommitted); needs upstreaming to afterhours alongside gap R.

## Full-code audit round 2 (2026-11-15)

Second `src/` audit after the `main.cpp` split into focused
translation units plus `src/systems/`. Lettering continues T-Z for
core afterhours. The afterhours-online candidates are lettered
ON-1 to ON-6 in this file (all in one place per request); mirror
them into `docs/afterhours-online-gaps.md` as OG entries when they
are picked up. Previously tracked asks reconfirmed during this
audit (J, K, L, O, I, D, H, F, Q) are listed at the end, not
re-argued.

### T. No logical-field presenter or screen-to-logical mouse map

Severity: medium. Effort: small.

Evidence: the game renders a fixed 1280x720 field through a 2x
camera into a 2560x1440 canvas (`kRenderScale`,
`src/components.h:26-28`; camera block `src/game_loop.cpp:230-236`)
and then reverse-engineers the same letterbox by hand to map mouse
clicks back to logical units (`logical_mouse_position`,
`src/menu.cpp:292-306`, used for menu hover/click and character
select). This is gap F grown up: the presenter math now lives in
two places that must agree by inspection.

Suggested fix: a presenter helper in `graphics` —
`present_logical(width, height, scale)` (or canvas + camera in one
call) plus `screen_to_logical()` for input. Every fixed-field 2D
game needs both halves.

### U. E2E command definitions are hundreds of lines of boilerplate

Severity: medium. Effort: small.

Evidence: `src/e2e_commands.h` is 719 lines of near-identical
`register_command` lambdas, each repeating the same shape:
find-state null check, `maybe_arg_as` parsing, `fail("<name> needs
...")`, single assertion or mutation. The pattern is visible in
every command from `start_one_player` through `assert_net_rtt`.

Suggested fix: typed-arg accessors plus a `require_state()` helper
(or one small macro) in `e2e_testing`, so a command reads as its
name, its args, and its body. Would roughly halve this file and
make every game's scripts uniform.

### V. No character-controller horizontal-velocity helper

Severity: low-medium. Effort: small.

Evidence: `horizontal_velocity` + `approach_velocity`
(`src/slime_rules.h:190-216`: ground/air accel and decel, turn
boost, stop snap) is the tuned "tight platformer feel" controller,
called by both the sim (`src/systems/MoveSlimes.h:116`) and client
prediction (`src/net_session.cpp:146`). Without the shared helper
there would be two drifting implementations of movement feel.

Suggested fix: move this shape into afterhours (2D movement or a
small `physics2d` math header) with its unit tests. Platformers,
sports games, and top-down games all want eased accel with a hard
stop.

### W. No projectile-intercept or braking-arrival steering helpers

Severity: low. Effort: small.

Evidence: the CPU solves time-to-height ballistics analytically
(`seconds_until_ball_height`, `src/systems/CpuControls.h:9-28`) and
stops on its target with a velocity-scaled deadband
(`src/systems/CpuControls.h:90-95`) instead of oscillating. Both
are generic ball-sport AI, currently inlined in one game.

Suggested fix: `time_until_height(pos, vel, target_y, gravity)` and
an arrive-with-braking helper (`arrive_velocity` or a deadband
function of speed) in a steering/physics header. Pong-likes,
sports games, and platformer enemies chasing arcs all reuse them.

### X. No ready-up lobby helper for local multi-select

Severity: low. Effort: small.

Evidence: per-slot `select_ready` flags, `toggle_ready`, and
`select_all_ready` leading to match start (`src/menu.cpp:150-174`,
`src/menu.cpp:366-371`, flags in `src/components.h`) implement the
standard ready-up pattern: browse while unready, lock while ready,
start when all ready.

Suggested fix: a header-only `ReadyLobby<N>` (slots, ready flags,
all-ready query, on-complete callback). Fits local party select
and doubles as the shape network lobbies need.

### Y. No shared juice helpers: squash, altitude shadow, offscreen marker

Severity: low. Effort: small.

Evidence: each is ~10 lines this game wrote by hand and every
sports/platformer game rewrites: squash-and-stretch on land and on
hit (`src/systems/RenderPumpkins.h`, `src/systems/MoveSlimes.h`,
`src/systems/MoveBall.h`), altitude-faded ground shadows
(`src/systems/RenderPumpkins.h`, `src/systems/RenderBall.h:15-20`),
and the top-edge offscreen indicator triangle
(`rules::ball_offscreen_top`, `src/slime_rules.h:553-559`;
`src/systems/RenderBall.h:25-31`).

Suggested fix: one small header with `impact_squash`,
`altitude_shadow`, and `offscreen_edge_marker` style helpers over
the raw-draw API.

### Z. No swept-ellipse or ellipse-vs-ellipse collision math

Severity: medium. Effort: medium-large.

Evidence: the swept ball-vs-pumpkin test
(`bounce_ball_off_slime_swept`, `src/slime_rules.h:375-449`) and
the ellipse-vs-ellipse character resolve with normal-velocity
exchange (`resolve_character_collision`,
`src/slime_rules.h:475-536`) are the subtlest code in the game,
with tunneling and ratchet bugs already found and fixed in them.
The existing `plugins/collision.h` is an ECS circle-overlap plugin
with no ellipse math, so there is nowhere to put this today.

Suggested fix: a `physics2d` math header with the swept ellipse
query plus ellipse separation, moving the golden tests with the
code. Ball sports and oval-bodied platformer characters are the
direct consumers.

### ON-1. (afterhours-online) No typed-packet codec over zpp::bits

Severity: medium. Effort: small.

Evidence: `src/slime_network.h` hand-rolls `peek_type`,
`encode_packet`/`decode_packet` (type byte + exact-length check,
`slime_network.h:41-77`), plus per-message encode/decode for
Hello, Welcome, Ping/Pong, Bye, Input, and Snapshot — about 150
lines around `make_wire_out`/`make_wire_in`. `zpp_serialize.h`
pins endianness but leaves the type-byte discipline per game.

Suggested fix: a `TypedCodec<MsgEnum>` template: archive the type
byte first, validate it on decode, require exhaustion. Each game
message becomes its struct plus `serialize()`.

### ON-2. (afterhours-online) No handshake-server helper

Severity: medium. Effort: small-medium.

Evidence: the Hello branch — version check, duplicate check,
capacity check, side assignment, Welcome/Bye
(`src/net_session.cpp:386-426`) — plus the client's Welcome
handling (`src/net_session.cpp:458-478`) is ~90 lines of lobby
logic every authoritative game rewrites. Schemas stay game-side
by design (OG-04), but the slot/full/reject flow does not need to.

Suggested fix: a `HandshakeServer<Hello, Welcome>` taking
validate/assign callbacks, owning duplicate detection, capacity
rejection with Bye reasons, and slot bookkeeping.

### ON-3. (afterhours-online) No per-peer RTT tracker

Severity: low-medium. Effort: small.

Evidence: `net_send_pings` / `net_note_pong` / `ping_sent`
(`src/net_session.cpp:344-379`, fields in `src/net_session.h`)
implement one aggregate smoothed RTT that never goes stale; OG-07
already diagnoses the per-peer and expiry gaps.

Suggested fix: the library tracker OG-07 sketches — keyed by
`(peer, seq)`, smoothed RTT plus last-sample time plus expiry per
peer. Any HUD or server-health check reuses it.

### ON-4. (afterhours-online) No host:port endpoint parser

Severity: low. Effort: tiny.

Evidence: `parse_join_target` / `format_join_target`
(`src/slime_network.h:89-135`, used at `src/menu.cpp:446`) is
pure direct-IP plumbing: trim, optional `:port`, numeric range,
hostname characters. Every direct-IP game needs it, and one
library contract would force the DNS-vs-literal decision OG-06
calls for instead of each game splitting it differently.

Suggested fix: `parse_endpoint` / `format_endpoint` in
afterhours-online, with the IPv4/IPv6/DNS contract stated once.

### ON-5. (afterhours-online) No connect-attempt state machine

Severity: low-medium. Effort: small.

Evidence: hello-sent/welcomed flags, the 12 s welcome deadline
(`src/net_session.cpp:610-622`), and the terminal-disconnect
reason-to-status mapping (`src/net_session.cpp:592-608`) are
generic client UX: without them a client says "Connecting"
forever (OG-01's original complaint).

Suggested fix: a `ConnectAttempt` helper owning the deadline and
the terminal-reason mapping, so every client gets consistent
"server full / unreachable / timed out" behavior.

### ON-6. (afterhours-online) No edge-latch or input redundancy for unreliable inputs

Severity: medium for twitch games. Effort: small-medium.

Evidence: `remote_input` with OR-latched jump
(`src/net_session.h:36`, latch at `src/net_session.cpp:427-437`,
apply/clear at `src/net_session.cpp:518-533`) exists because one
dropped unreliable packet eats a jump press; OG-05 notes inputs
have no ack, redundancy, or replay.

Suggested fix: a tiny `EdgeLatch` or last-N-inputs redundancy
wrapper, transport-agnostic like the existing fault injector.
Any game sending button edges over `UnreliableNoDelay` needs it.

### Reconfirmed during this audit: already tracked, still open

- J (fitted/ellipsized text): `ellipsize_text` in `src/draw.h`
  still hand-rolled at every call site.
- K (list-menu navigation): `src/menu.cpp` still carries ~150
  lines of selection/hover/stick/shortcut state.
- L (raw-draw text edit): `handle_target_edit`
  (`src/menu.cpp:433-481`) still hand-rolls the char queue,
  backspace, replace-on-first-key, and cursor.
- O (gamepad edge + E2E injection): `menu_stick_y` /
  `select_stick_x` edge detection still duplicated
  (`src/components.h:160`, `src/components.h:204`); E2E still
  cannot inject gamepad input.
- I (FPS stats): `FpsLogger` (`src/game_loop.cpp:32-52`) still
  hand-rolls the trailing-window average.
- D update (image-size metadata): the `sprite_aspect` table in
  `src/slime_rules.h:83` still duplicates PNG dimensions for
  headless wall clamps.
- H (wall clock): `menu_bar_clock` (`src/ui_draw.cpp:52-70`)
  still carries its own day/month tables on `std::localtime`.
- F (canvas vs presented): folded into T above.
- Q / OG-05 (snapshot buffer + send pacing): `sample_snapshots`
  (`src/net_session.cpp:78-98`) and the wall-clock input,
  snapshot, and ping pacing are still game code; remains the
  top online ask.
