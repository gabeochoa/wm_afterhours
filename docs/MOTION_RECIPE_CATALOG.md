# WM Motion Recipe Catalog

Local WM motion cookbook. It inventories every distinct recipe/checkpoint in
the Transitions Lab and the two motion checkpoints, maps each one to the
vendored Afterhours (AH) primitive that already covers it, and names the WM
recipes worth keeping plus a short ranked list of narrow AH gap candidates.

This is documentation only. It proposes no new runtime. The deferred motion
recipe catalog todo's next step is this file: a reference a screen author
reads before writing new motion, not a new abstraction layer.

Sources (all read at `origin/main` `fdf5d6b`):

- `src/systems/screens/TransitionsLab.h` — 1,925 lines, `struct TransitionsLab`,
  `enum struct Key` at line 129, six groups at lines 35–42.
- `src/systems/screens/MotionButtonCheckpoint.h` — 112 lines,
  `struct MotionButtonCheckpoint`, debug names `motion_btn`, `origin_btn`,
  `slab`, `scaled_slab` / `child_btn`, `scaled_child_btn`.
- `src/systems/screens/MotionModalCheckpoint.h` — 92 lines,
  `struct MotionModalCheckpoint`, debug names `open_btn`, `bg_btn`,
  `inner_btn`, `close_btn`, `close_reopen_btn`.
- Vendored AH (read-only): `vendor/afterhours/src/plugins/ui_motion.h` (472
  lines), `animation_presets.h` + `animation_presets/{basic_presets,effect_presets}.h`,
  `effects.h` (402 lines), `particles.h` (112 lines), `modal.h` (1,155 lines),
  `animation/{spring,timeline}.h`, `reduced_motion.h`, `toast.h`.
- Reference: `docs/animation.md` ("Animation in afterhours").

## How each entry was verified

Every catalog row's slug or debug name was verified by counting its quoted
occurrences in the claimed file (Python `str.count`, equivalent to
`grep -c`). Method and counts, run on Boulder in `~/w/wm-motion-recipes`:

- Transitions Lab: each of the 47 primary slugs appears 2× in
  `TransitionsLab.h` (once in the group registry at lines 35–41, once in the
  `slug == ...` branch), except `icon_swap`, `learn_more`, `toast`,
  `matrix`, `pro_text`, `dissolve`, `bend` (3–4× from branch + state labels),
  `blur` (3×: effects group, effect-presets group, branch), and the 14
  effect-preset-only slugs `unblur`, `wipe`, `curtain`, `sweep`, `iris`,
  `spotlight`, `blinds`, `unroll`, `shear`, `stretch`, `flip`, `tumble`,
  `swing`, `recede`, `emerge` (1×: registry only — they share the single
  `effect_presets` branch at lines 342–382, driven by
  `afterhours::presets::effect_presets[example[group]]`).
- Checkpoints: `motion_btn` 1×, `origin_btn` 1×, `scaled_slab` 1× in
  `MotionButtonCheckpoint.h`; `open_btn` 6×, `bg_btn` 5×, `close_reopen_btn`
  1× in `MotionModalCheckpoint.h`.
- Motion tracks: `Key::CheckDraw`, `Key::ToastHold`, `Key::StarPreview`,
  `Key::GenLoad`, `Key::DissolveP`, `Key::OrganicP`, `Key::BendP`,
  `Key::TiltRX`, `Key::EffectPresetP` each 2× (reset + use); `Key::Reel0`,
  `Key::GooA0` 3× (reset loop + address arithmetic + use); `Key::PillX`,
  `Key::MorphW`, `Key::SpinAngle`, `Key::CardW`, `Key::DragX` 1×.

No recipe below was invented. Where a lab section is just a demo of an AH
preset, the entry says so.

## AH primitives referenced

- `afterhours::motion` core (`animation/{spring,timeline,track}.h`):
  `Spring{.response, .bounce}`, presets `smooth()` `0.3/0`, `snappy()`
  `0.22/0.15`, `bouncy()` `0.35/0.35`, `gentle()` `0.5/0`; `Timeline` keys +
  `Repeat{Once,Loop,PingPong}` + `curves::{ease_in_quad, ease_out_quad,
  ease_in_out_quad, ease_out_cubic}`; `Track<T>::from/to/then/delay/repeat/
  essential/on_complete`; `set_instant` lands non-essential tracks on their
  final target; extras use keys ≥ 100 (0–5 are the `MotionProperty` slots).
- `ui_motion` bridge (`ui_motion.h`): `MotionProperty{Scale, TranslateX,
  TranslateY, Rotation, Opacity, CornerRadius, Blur}` + `background` colour;
  `MotionTrigger{Appear, State, Focus, Hover, Press, Change}`;
  `with(um::on_appear/on_hover/on_press/on_focus/on_state/on_change(...))`,
  `with_origin(x,y)` pins scale to a corner/edge; parent transforms compose
  to children. Each frame triggers collapse to one target per property
  (press > hover > focus > state/rest); falling back to rest reuses the last
  winning spring, so hover-out is a reversal.
- `presets::basic_presets.h`: `fade_up`, `fade_in`, `pop_in`, `hover_lift`,
  `press_squash`, `slide_in`, `shake(0.28s)`, `spin(0.9s)`,
  `pulse(0.5s)` — with the known wiring defect below.
- `presets::effect_presets.h`: `EffectId` × 17, `effect_presets` table,
  `effect_timeline(0.9s)`, `effect_blur_radius`, `effect_quad`, plus
  `Shimmer` / `ShimmerStrip`.
- `effects`: `Effect::load("name")` (reads `resources/shaders/name.fs`),
  `Effect::Scope`, `effects::reload_all()`, `BlurPass`; `with_blur(px)` /
  `blur` property (capped at 8).
- `particles`: `Emitter<Capacity>` pool (gravity, drag, floor, restitution),
  `spawn()/update()/each()/count()`, `hash01(n)`; typically drawn from
  `with_on_draw_fg`.
- `modal`: `ModalConfig::with_motion(enter, exit)` drives
  `Hidden → Entering → Visible → Exiting → Hidden`; input is blocked until
  the exit settles, focus is restored once, reopening mid-exit reverses;
  without `with_motion` nothing changes.
- `with_unit_motion(TextUnitMotion{unit, from_y, from_opacity, duration,
  stagger})` — per-grapheme/per-word staging; `text_units_active()`.
- `draw_quad` / `draw_texture_quad` — four corners for projected or sheared
  shapes.
- Reduced motion: `Settings::set_reduced_motion_enabled(bool)` persists and
  drives `animation::set_instant`; loops that must keep moving are marked
  `.essential()` (the lab spinner does this).

Known AH defect that matters for "covered": the built-in presets return
`MotionRule`, but `motion_rules()` collects only `MotionExt`, so
`.with(presets::fade_up())` silently never animates (see
`docs/AFTERHOURS_GAPS.md`, "Built-in motion presets return `MotionRule` and
are silently dropped"; zero `presets::` callers in WM `src/` today). Where
this catalog says "AH-covered via `presets::X`", the recipe exists and the
numbers are right, but a screen author today must hand-write the same
`on_appear/on_hover/...` rule (as the lab does) until that one-place AH fix
lands. No lab entry uses `.with(presets::...)` directly.

## Catalog

Legend: **[AH]** = AH primitive already covers it (lab may still hand-write
the values) · **[WM]** = WM-local-only recipe worth keeping, assembled from
AH primitives · gap candidates are flagged and ranked at the end.

### Group `controls` — "Controls" (registry line 36)

1. **Checkbox check** — slug `checkbox`, title "Checkbox check".
   Demonstrates: the check mark drawing in and retracting. Timing (lines
   411–418): `Key::CheckDraw` timeline `{{0,0},{0.35,1}}` `ease_out_quad` to
   draw in (350 ms), `{{0,0},{0.15,1}}` to retract (150 ms); background via
   `on_state` + `Spring::snappy()`. **[WM]** No AH check-draw preset exists;
   `ui_motion` covers the background/scale side. Keep the lab recipe: drive
   a polyline stroke progress from a 0→1 track with asymmetric in/out
   timelines. No AH gap (generic track + draw).

2. **Toggle** — slug `toggle`. Track `.with(um::on_state(toggled, ...,
   Spring::smooth()))`, thumb `.with(um::on_state(toggled, {.translate_x =
   {0, travel}}, Spring::bouncy()))` — the thumb overshoots and settles
   (line 436). **[AH]** `on_state` + spring; same shape as `hover_lift` /
   `press_squash` values. Reuse: toggle = background `smooth()` + thumb
   `bouncy()` on one state.

3. **Like button** — slug `like`, debug `like_btn`. `on_change(likes,
   {.scale = {0.82, 1}}, Spring{.35, .6})` pops on like; fill colour
   `Key::LikeFill` timeline `{{0,0},{0.15,1}}` (line 465); unliking only
   drains the fill. **[AH]** `on_change` + colour track. Reuse as-is.

4. **Icon swap** — slug `icon_swap`. Glyph shrinks/fades while the other
   grows in, `Timeline{{0,0},{0.25,1}}` `ease_in_out_quad` (line 473).
   **[AH]** `on_state` cross-fade + scale. Reuse as-is.

5. **Learn more hover** — slug `learn_more`, debug `learn_more`. Chevron
   slides 10 px right, arms open 30°, underline draws in; `Key::LearnShift` /
   `Key::LearnSpread` with `Spring{.35}` (line 518). **[AH]** hover
   translate/rotation pattern. The chevron-arm geometry is WM-local
   composition; the motion is `on_hover`. Reuse the hover values; the
   drawn chevron stays WM-local.

6. **Error state shake** — slug `error_shake`, debug `error_field`.
   `Key::Shake` timeline `{{0,0},{0.08,1},{0.16,-1},{0.22,.667},{0.28,0}}`
   × 6 px over 280 ms `ease_out_quad` (lines 549–550) + `Key::ErrorHold`
   3.28 s auto-clear via `on_complete`. **[AH]** These are exactly the keys
   of `presets::shake(0.28f)` — the lab hand-types them because of the
   wiring defect above. Reuse `presets::shake` once the defect is fixed;
   today copy the lab keys.

7. **Input clear** — slug `input_clear`, debug `clear_field`. Old text
   drops 12 px while fading, placeholder settles from above; `Timeline`
   `{{0,0},{0.4,1}}` `ease_out_quad` (line 559). Fade only, no glow yet.
   **[AH]** `on_state` translate + opacity. Reuse as-is.

8. **Text states swap** — slug `text_swap`, debug `swap_cell`. Old label
   lifts 4 px as it fades, new one rises from 4 px below; `Timeline`
   `{{0,0},{0.22,1}}` `ease_in_out_quad` (line 586). **[AH]** `on_state`
   translate + opacity, 220 ms. Reuse as-is (state-machine labels:
   Saved / Saving changes / Could not save).

### Group `navigation` — "Navigation and overlays" (registry line 37)

9. **Menu dropdown** — slug `menu`, debug `menu_panel`, `menu_trigger`.
   `on_appear({translate_y -16s, scale 0.97, opacity 0})` + `on_state(menu_open,
   ...)` `Timeline{{0,0},{0.25,1}}` `ease_out_quad` (lines 620–622); items
   `Spring::snappy()` hover background; `with_origin(0,0)`. **[AH]**
   `on_state` menu reveal. Reuse as-is.

10. **Tooltip** — slug `tooltip`, debug `tooltip_bubble`. `Key::TooltipX`
    retargets `{{0,0},{0.16,1}}` `ease_out_quad` (line 657); bubble
    `on_appear({scale 0.98, opacity 0})` + 0.15 s fade. **[AH]**
    `on_appear`/`on_state` + one float track. Reuse as-is.

11. **Modal open/close** (lab) — slug `modal`. `afterhours::modal(...)`
    with `ModalConfig.with_motion(enter Timeline{{0,0},{0.25,1}} ease_out_quad,
    exit Timeline{{0,0},{0.15,1}})` (lines 679–680); panel and backdrop fade
    together; input behind stays blocked until the exit settles. **[AH]**
    This is the `modal.h` contract directly. Reuse `with_motion`; see also
    the modal checkpoint (#65).

12. **Panel reveal** — slug `panel`, debug `panel_body`. `on_state` 400 ms
    `ease_out_quad` (line 705); the panel rises half its height into a
    clipping frame while fading in. **[AH]** `on_state` translate + opacity
    into a clip. Reuse as-is.

13. **Page side-by-side** — slug `page`, debug `page_frame`. `Timeline`
    `{{0,0},{0.32,1}}` `ease_out_cubic` (line 713); `on_appear({translate_x
    420s})` + `on_state(page_two, {translate_x ...})`. **[AH]** `slide_in`
    pattern. Reuse as-is.

14. **Tabs sliding** — slug `tabs`, debug `tabs_pill`, `tabs_bar`.
    `Key::PillX` / `Key::PillW` retargeted on `{{0,0},{0.25,1}}`
    `ease_out_quad` (lines 741–744); the pill is measured, not the content.
    **[WM]** AH has no tabs-pill preset; the recipe (measured x/w tracks fed
    into `with_size`) is WM-local. Keep it. Not an AH gap: `motion::anim`
    float tracks + `on_state` already express it.

15. **Accordion** — slug `accordion`, debug `acc_column`, `acc_header`,
    `acc_content`. `Key::AccHeight` `{{0,0},{0.25,1}}` `ease_out_quad`
    (lines 767–769) fed into `with_size` so the row below reflows; chevron
    folds up. **[WM]** The height-track → `with_size` reflow pattern is
    WM-local (layout follows a track). Keep it. Primitives are AH.

16. **Dropdown menu morph** — slug `morph`, debug `morph_surface`,
    `morph_plus`, `morph_row_N`. `Key::MorphW` / `Key::MorphH` grow 0.35 s /
    shrink 0.25 s `ease_out_quad` (lines 823–824); rows stagger in with a
    `{{0,0},{0.12,0},{0.35,1}}` delayed fade (line 848); plus rotates 45°
    `Spring::snappy()`. **[WM]** Surface-morph + staggered rows is a WM
    recipe on AH primitives. Keep it.

### Group `status` — "Status and loading" (registry line 38)

17. **Toast open/close** — slug `toast`, debug `toast`. Enter from 16 px
    below at scale 0.97 over 350 ms `ease_out_quad` (line 869); `Key::ToastHold`
    3 s auto-dismiss via `on_complete` (line 861). **[AH]** AH ships
    `toast.h` (`Toast` / `ToastRoot` / update + layout systems); the lab
    shape matches. Reuse `toast.h` or this `on_state` + hold pattern.

18. **Notification badge** — slug `badge`, debug `badge_dot`, `badge_wrap`.
    `on_state(badge_shown, {.scale = {0,1}, .opacity = {0,1}}, Spring{.5,
    .35})` (line 889) — pops about its own centre, collapses the same way.
    **[AH]** `pop_in` pattern. Reuse as-is.

19. **Success check** — slug `success`, debug `success_icon`. `Key::SuccessDraw`
    `{{0,0},{0.5,1}}` `ease_out_quad` delayed 0.08 s (line 904); icon
    `Spring{.5, .3}` rotation/translate/opacity (line 932). **[WM]** Stroke
    draw + spring icon is a WM recipe (same polyline-progress technique as
    #1). Keep it.

20. **Skeleton loader** — slug `skeleton`, debug `skeleton_card`,
    `skeleton_bar_N`. `Key::SkeletonLoad` `{{0,0},{1,1}}` then reveal via
    `on_complete` (line 952); bars pulse `Timeline{{0,0},{0.5,1}}`
    `Repeat::PingPong` `ease_in_out_quad` + reveal 0.4 s (lines 956–957).
    **[WM]** Skeleton pulse → content cross-fade recipe. Keep it. AH has
    `pulse()` but no skeleton composite.

21. **Spinner to check** — slug `spinner`, debug `spinner_badge`. `Key::SpinAngle`
    `{{0,0},{0.9,1}}` `Repeat::Loop` `.essential()` (line 993);
    `Key::SpinCheck` 0.6 s delayed 0.23 s; `Key::SpinHold` 2 s. **[AH]**
    `presets::spin(0.9f)` is the same loop; `.essential()` keeps it under
    reduced motion. Reuse `spin` + check hold.

22. **Banner stacking** — slug `banners`, debug `banner_stack`, `banner_N`.
    New banner rises into the front (`{{0,0},{0.35,1}}` line 1062); older
    step back and dim (`Spring{.35, .5}` on_change, line 1033); hover
    spreads, scroll for the oldest. **[WM]** Stack-depth + spread + scroll
    recipe. Keep it.

23. **Streaming text** — slug `streaming`, debug `stream_text`. `Key::StreamCount`
    `{{0,0},{0.06 × words,1}}` (line 1252) revealing word by word; runs
    counted. **[WM]** Streaming-word recipe. AH's `with_unit_motion` covers
    per-word staging but not the streaming controller; keep the lab recipe.

24. **Thinking states** — slug `thinking`, debug `thinking_text`.
    `Key::ThinkHold` 2.4 s cycle (line 1271); incoming words rise 8 px and
    fade in 0.2 s with 0.07 s gap. **[WM]** Cycling-status recipe. Keep it.

25. **Reasoning stream** — slug `reasoning`, debug `reasoning_viewport`,
    `reason_line_N`. `Key::ReasonOffset` 0.5 s `ease_out_cubic` + `Key::ReasonHold`
    0.84 s/step (lines 1296–1301): viewport slides one line per step. **[WM]**
    Auto-scrolling clipped reasoning stream. Keep it.

26. **Shimmer text** — slug `shimmer`, debug `shimmer_base` /
    `shimmer_highlight`. `Key::ShimmerX` loop `{{0,0},{0.8,1}}`
    `Repeat::Loop` (line 1324) sweeping a highlight across "Generating your
    summary". **[AH]** `effect_presets.h` ships `Shimmer` / `ShimmerStrip`
    structs for this; the lab sweep is the same pattern. Reuse the sweep.

### Group `cards` — "Cards and text" (registry line 39)

27. **Card resize** — slug `card_resize`, debug `resize_card`. `Key::CardW` /
    `Key::CardH` `{{0,0},{0.3,1}}` `ease_out_quad` (line 1109) fed into
    `with_size`; text reflows inside the changing box. **[WM]** Size-tracks →
    `with_size` recipe (same technique as the accordion, #15). Keep it.

28. **Card stack hover** — slug `card_stack`, debug `stack_zone`,
    `stack_card_N`. `Spring` open `{.41, .5}` / close `{.36, .35}` (line
    1135); fan-out translate/rotation/opacity on `on_state`; hover
    `Spring{.61}` 1.04×. **[WM]** Fan-out stack recipe on AH springs. Keep
    it.

29. **Avatar group hover** — slug `avatars`, debug `avatar_N`. Hover
    `Spring{.32, .6}` 1.05× (line 1172); hovering one lifts its neighbours
    via `Spring{.32}` in / `Spring{.32, .75}` out on track 100 (lines
    1176–1179). **[WM]** + gap candidate G3 below. Keep the recipe; the
    group-hover propagation is the gap.

30. **Texts reveal** — slug `texts_reveal`, debug `reveal_line_1/2`. Two
    lines rise 12 px while fading; first 0.5 s `ease_out_quad`, second
    delayed 0.04 s / 0.54 s (lines 1189–1190); hide 0.2 s. **[AH]**
    `on_appear`/`on_state` with per-rule delay = stagger. Reuse as-is.

31. **Matrix dot loader** — slug `matrix`, debug `dot_N`, patterns Scan /
    Twinkle / Orbit / Pulse. Loop timelines `{{0,0},{delay,0},{delay+0.18,1},
    {delay+0.54,0},{cycle+delay,0}}` `Repeat::Loop` `ease_in_out_quad`
    (lines 1233–1234). **[WM]** + gap candidate G5 below. Keep the lab
    pattern table.

32. **Spinning counter** — slug `counter`, debug `reel_N`, `reel_digit_N`.
    `Key::Reel0+col` `{{0,0},{1.4,1}}` `ease_out_cubic` staggered
    0.09 s/column (lines 1347–1351); vertical strip slides. **[WM]** + gap
    candidate G2 below. Keep it.

33. **Number pop-in** — slug `popin`, debug `popin_number`.
    `.with_unit_motion({.unit = TextUnit::Char, .from_y = 8s, .from_opacity
    = 0, .duration = 0.5, .stagger = 0.07})` (line 1379). **[AH]** Direct
    use of AH `with_unit_motion`. Reuse as-is.

### Group `effects` — "Visual effects" (registry line 40)

34. **Shader effect** — slug `effect`, debug `effect_canvas`. `Effect::load("tint")`
    + `Effect::Scope` + `effects::reload_all()` (lines 1384–1386, 1394).
    **[AH]** Direct demo of the AH effects mechanism. Reuse as-is.

35. **Blur** (Visual-effects group) — slug `blur`, debug `blur_card`.
    `on_state(blurred, {.blur = {0, 6}}, Timeline{{0,0},{0.4,1}} ease_out_quad)`
    (line 1409); "Frosted over 400 ms". **[AH]** `blur` property +
    `BlurPass`. Reuse as-is. (Distinct from the effect-presets `Blur`, #48:
    this one tweens the `blur` property; #48 drives the preset timeline.)

36. **Confetti burst** — slug `confetti`, debug `confetti_stage`. `Emitter<512>`
    `confetti`, 12-stars-per-burst spawn with `hash01` spread (lines
    1430–1431). **[AH]** `particles::Emitter` directly. Reuse as-is.

37. **Star rating** — slug `stars`, debug `star_N`, `star_sparks`. Half-star
    preview `Key::StarPreview` 0.08 s in / 0.3 s out (line 1481); per-star
    pop `{{0,0},{0.12,1}}` then `Spring{.35, .55}` staggered 0.05 s/star
    (lines 1518–1519); `Emitter<64>` spark burst on commit (line 1524).
    **[WM]** Half-star rating composite (preview + pop + sparks). Keep it;
    primitives are AH.

38. **Smoke ring** — slug `smoke`, debug `smoke_stage`. `puff()` spawns a
    30-particle ring on `Emitter<64>` (`hash01` speed/scale jitter, lines
    143–144). **[AH]** `particles::Emitter` directly. Reuse as-is.

39. **Pro gradient text** — slug `pro_text`, debug `pro_text`. `Effect::load("gradient")`
    with `origin`/`extent`/`radius`/`softness` uniforms (lines 1574–1584).
    **[WM]** Custom `gradient.fs` shader recipe; the load/scope mechanism
    is AH. Keep the shader + recipe.

40. **Get Pro button** — slug `get_pro`, debug `get_pro_btn`. Gradient pill
    under the button (`gradient.fs`, same as #39) + `on_hover({scale 1.03})`
    (line 1599); "colour glows while the label stays fixed". **[WM]** Glow-
    under-button pattern — per `docs/animation.md`, a glow is a shader with
    a soft rim, not a blur. Keep it.

41. **Image placeholder** — slug `imagegen`, debug `gen_frame`, `gen_dot_N`,
    `gen_image`. `Key::GenLoad` loop 1.4 s (line 1608); dots grow
    `{{0,0},{phase,0},{phase+0.7,1},{1.4+phase,0}}` Loop, then reveal
    scale 3.4× + corner-radius close, image fades in 0.6–0.9 s (lines
    1630–1645). **[WM]** Staged generate → pixel-reveal → image recipe.
    Keep it.

42. **Smoky dissolve** (Visual-effects group) — slug `dissolve`, debug
    `dissolve_card`. Custom `dissolve.fs` via `Effect::load("dissolve")`,
    `Key::DissolveP` 0.7 s + `Key::DissolveHold` 0.8 s (lines 1652–1659).
    **[WM]** Custom shader dissolve variant. (Distinct from the
    effect-presets `Dissolve`, #50, which is the AH preset; the lab's custom
    `dissolve.fs` adds the smoky hold behaviour.) Keep it.

43. **Gooey plus menu** — slug `gooey`, debug `goo_field`, `goo_plus`,
    `goo_action_N`. Custom `goo.fs` four-metaball field; `Key::GooA0+i`
    `Spring::bouncy()` staggered 0.04 s (line 1695); `Key::GooBob` wobble
    `{{0,0},{0.35,1},{0.7,0}}` on close (line 1698). **[WM]** Metaball
    shader + spring actions. Keep it. Not an AH gap shortlist entry: the
    mechanism (`Effect::load` + a custom shader) is AH-generic; only the
    shader is WM-authored.

44. **Organic shimmer** — slug `organic`, debug `organic_square`. Custom
    `shimmer.fs` three pastel layers; `Key::OrganicP` loop 3 s
    `ease_out_quad` (line 1746) sweeping a noisy highlight. **[WM]** Custom
    shader recipe. Keep it.

45. **Image bend** — slug `bend`, debug `bend_image`. Custom `bend.fs` with
    a `sin(πp)` envelope; `Key::BendP` 0.35 s + scale 0.3→1 over 0.45 s
    `ease_out_cubic` (lines 1766–1781). **[WM]** Bend-shader + open-scale
    recipe. Keep it.

46. **3D tilt** — slug `tilt`, debug `tilt_card`. Pointer-steered
    `Key::TiltRX/RY` follow 0.4 s / back 1.0 s `ease_out_cubic` (lines
    1800–1801), `Key::TiltGlare` glare; card rotates up to 28°, 650 px
    perspective via `draw_quad` corners (in-code label, line 1834).
    **[WM]** + gap candidate G4 below. `draw_quad` is AH; the tilt
    steering + glare recipe is WM-local. Keep it.

47. **Drag and drop** — slug `dragdrop`, debug `drop_target`,
    `drag_picture`. `Key::DragX/Y` follow pointer; `Key::DragTilt` 0.15 s
    while dragging, `Spring::snappy()` back (line 1889); drop slide 0.35 s
    `ease_out_cubic` delayed 0.2 s (line 1896), `Key::DropFade` hold-apart
    0.75–1.15 s (line 1898), miss springs home `Spring::bouncy()`
    (lines 1906–1907). **[WM]** + gap candidate G1 below. Keep it.

### Group `effect_presets` — "Effect presets" (registry line 41)

One progress track `Key::EffectPresetP` over `effect_timeline(0.9f)` drives
every entry (line 348): shaders take mode + progress, blur takes
`effect_blur_radius`, shear/stretch/flip/tumble/swing/recede/emerge take
quad corners from `effect_quad` (in-code label, line 376). All 17 entries
below are pure demos of AH `presets::effect_presets` — **covered by AH**,
no WM-local recipe beyond the single shared driver. Reuse: read
`effect_presets` / `effect_timeline` / `effect_quad` / `effect_blur_radius`
and load `effect_presets.fs` once.

48. **Blur** (effect-presets group) — slug `blur`, `EffectId::Blur`,
    `EffectShader::None`, radius driver.
49. **Unblur** — slug `unblur`, `EffectId::Unblur`, radius driver.
50. **Dissolve** (effect-presets group) — slug `dissolve`,
    `EffectId::Dissolve`, shader `Dissolve`.
51. **Wipe** — slug `wipe`, `EffectId::Wipe`, shader `Wipe`.
52. **Curtain** — slug `curtain`, `EffectId::Curtain`, shader `Curtain`.
53. **Sweep** — slug `sweep`, `EffectId::Sweep`, shader `Sweep`.
54. **Iris** — slug `iris`, `EffectId::Iris`, shader `Iris`.
55. **Spotlight** — slug `spotlight`, `EffectId::Spotlight`, shader
    `Spotlight`.
56. **Blinds** — slug `blinds`, `EffectId::Blinds`, shader `Blinds`.
57. **Unroll** — slug `unroll`, `EffectId::Unroll`, shader `Unroll`.
58. **Shear** — slug `shear`, `EffectId::Shear`, quad corners.
59. **Stretch** — slug `stretch`, `EffectId::Stretch`, quad corners.
60. **Flip** — slug `flip`, `EffectId::Flip`, quad corners.
61. **Tumble** — slug `tumble`, `EffectId::Tumble`, quad corners.
62. **Swing** — slug `swing`, `EffectId::Swing`, quad corners.
63. **Recede** — slug `recede`, `EffectId::Recede`, quad corners.
64. **Emerge** — slug `emerge`, `EffectId::Emerge`, quad corners.

### Motion checkpoints

65. **MotionButtonCheckpoint** ("First checkpoint for the motion rewrite: one
    button", `motion_button`). Debug `motion_btn`: appear
    `on_appear({translate_y 24s → 0, opacity 0 → 1}, Spring::gentle(), 0.1s
    delay)`, focus 1.03, hover 1.05 + −4 px, press 0.92 — i.e. the
    `hover_lift(4, 1.05)` / `press_squash(0.92)` values hand-written.
    `origin_btn`: `with_origin(0,0)` corner-pinned scale 1.1 + corner-radius
    + background. `slab` / `scaled_slab`: ancestor translate/scale composing
    to clickable children (`child_btn`, `scaled_child_btn`). Labels read
    live `HasTracks` scale/y/opacity + `was_hot` / `was_active` / focus.
    Reduced-motion checkbox wired to `Settings`. **[AH]** This is the
    `ui_motion` core demo; every value matches an AH preset. Reuse as the
    canonical button recipe.

66. **MotionModalCheckpoint** ("Modal overlay contract: enter, visible, exit,
    hidden", `motion_modal`). `afterhours::modal` with
    `ModalConfig.with_motion(Spring::gentle(), Timeline{{0,0},{1.5,1}})`:
    enter spring, 1.5 s exit; phases `hidden / entering / visible /
    exiting` read off `modal::Modal::Phase`; background clicks counted
    while input is blocked; "Close then reopen" reopens 3 frames after
    close. **[AH]** The `modal.h` contract directly. Reuse as the canonical
    modal recipe.

## Covered-by-AH list (41)

Direct AH primitive/demo — the lab entry exists to show the AH capability;
reuse the AH API (hand-writing the same values until the presets wiring
defect at the top is fixed):

- Controls: `toggle`, `like`, `icon_swap`, `learn_more`, `error_shake`,
  `input_clear`, `text_swap` (7)
- Navigation: `menu`, `tooltip`, `modal` (lab), `panel`, `page` (5)
- Status: `toast`, `badge`, `spinner`, `shimmer` (4)
- Cards: `texts_reveal`, `popin` (2)
- Visual effects: `effect` (shader tint), `blur` (Visual-effects group),
  `confetti`, `smoke` (4)
- Effect presets: `blur`, `unblur`, `dissolve`, `wipe`, `curtain`, `sweep`,
  `iris`, `spotlight`, `blinds`, `unroll`, `shear`, `stretch`, `flip`,
  `tumble`, `swing`, `recede`, `emerge` (17)
- Checkpoints: `MotionButtonCheckpoint`, `MotionModalCheckpoint` (2)

## WM-local-only recipes worth keeping (25)

Assembled in WM from AH primitives; no single AH preset expresses them;
keep the lab implementation as the recipe:

- `checkbox` (check-mark stroke draw), `tabs` (measured pill), `accordion`
  (height track → `with_size` reflow), `morph` (surface morph + staggered
  rows), `success` (stroke draw + spring icon), `skeleton` (pulse → reveal),
  `banners` (stack + spread + scroll), `streaming`, `thinking`, `reasoning`
  (auto-scrolling clipped stream), `card_resize` (size tracks → `with_size`),
  `card_stack` (fan-out), `avatars` (group-hover lift), `matrix` (dot
  patterns), `counter` (digit reel), `stars` (half-star preview + pop +
  sparks), `pro_text` / `get_pro` (`gradient.fs` glow), `imagegen` (staged
  pixel reveal), `dissolve` (Visual-effects group, custom `dissolve.fs`
  hold), `gooey` (`goo.fs` metaballs), `organic` (`shimmer.fs`), `bend`
  (`bend.fs` + open scale), `tilt` (pointer tilt + glare), `dragdrop`
  (drag + spring-home).

## Ranked AH gap shortlist (5)

Narrow candidates only where AH truly lacks the primitive/recipe. All five
exist today as WM-local lab recipes; anything filed against AH should
reference the lab section as the working prototype. Ranked by
reuse-frequency × blast radius:

1. **Drag-and-drop helper** (from #47 `dragdrop`): AH has no drag state —
   no "follow pointer / tilt while dragging / spring home on release /
   drop settle" composite. Lab uses five coordinated tracks
   (`DragX/Y/Tilt/Fade/Hold`) plus `bouncy()` home. Highest reuse: every
   screen with draggable media wants this.
2. **Digit reel / odometer** (from #32 `counter`): AH has springs and
   timelines but no vertical digit-strip primitive; the lab hand-builds
   four `Reel` tracks with per-column stagger and strip positioning.
3. **Group-hover propagation** (from #29 `avatars`): `on_hover` targets one
   entity; the lab manually lifts neighbours on track 100. A group-scope
   hover that fans out to siblings (and releases with a different spring)
   would replace the manual track.
4. **Pointer tilt** (from #46 `tilt`): AH has `draw_quad` for the
   projection but no tilt helper — no follow/back timelines + glare track.
5. **Dot-matrix loader patterns** (from #31 `matrix`): AH `pulse()` is a
   single value; Scan / Twinkle / Orbit / Pulse over a dot grid is a lab
   pattern table with no preset.

Deliberately not gap candidates: `gooey`/`organic`/`bend`/`pro_text`
(custom shaders — AH's `Effect::load` is already generic), `tabs` /
`accordion` / `card_resize` (expressible with `motion::anim` tracks fed
into `with_size`), `banners` / `streaming` / `thinking` / `reasoning` /
`stars` / `imagegen` (screen-level composites, not primitives).

## Notes for screen authors

- Start in the lab: `transitions_lab`, scripts 300–346 (see
  `docs/animation.md`). Every entry above is one click away in the group's
  example selector.
- Motion properties are exactly `scale`, `translate_x`, `translate_y`,
  `rotation`, `opacity`, `corner_radius`, `blur` (+ `background` colour).
  Anything outside that list (stroke progress, pill x/w, stack depth) is a
  `motion::anim` float track — keys ≥ 100 for widget-owned extras — fed
  into `with_size` / `with_absolute_position` or read back from
  `HasTracks` for custom draws.
- Hold-and-release is a timeline with `on_complete`, never a timer
  (toast 3 s, error 3.28 s, spinner 2 s all do this). Replay an entrance
  by removing `HasMotionState` + `HasTracks`.
- Reduced motion: `Settings` drives `set_instant`; only `.essential()`
  loops (spinner) keep animating. The checkpoint checkbox is the manual
  override.
