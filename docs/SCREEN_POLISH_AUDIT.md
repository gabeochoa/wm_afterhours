# Screen Polish Audit — All 156 Screens

Audited through two lenses:
- **Software You Can Love:** every screen should show *how* the feature works, respond immediately, avoid dead/fake controls or meta-disclaimers inside realistic mockups, and let curious developers copy/inspect/tweak live parameters.
- **Interface Design:** subtle surface layering over harsh borders, whisper-quiet elevation, disciplined typography hierarchy, purposeful color (gray builds structure; color communicates), symmetrical padding, and complete interaction states (hover, active, focus, disabled).

See also `docs/DUPLICATE_SCREENS_AUDIT.md` for screen consolidation recommendations (156 → ~115 screens).

---

## Systemic Polish Opportunities (High Leverage Across Many Screens)

1. **Replace full-row saturated fills on list/control specimens with quiet surfaces + semantic accents.** Screens like `checkboxes`, `themes`, and `toggle_switches` paint entire rows in bright primary/secondary/accent fills, hurting contrast on secondary labels (`Checked`/`Unchecked`) and creating visual noise. Use subtle surface elevation for structure and reserve brand/accent color for the active mark or selection indicator.
2. **Strip meta-disclaimers out of realistic mockups and dialogs.** Copy like `"Demo preview only; your actual display stays unchanged"` (`dialog_confirm`), `"Network simulation only"` repeated 4× (`secure_tunnel`), `"Read-only example / No pending changes"` (`tab_container`), and `"DevTeam_Support: Check out the new update!"` (`cozy_cafe`, `empire_tycoon`) breaks the spell. Keep realistic UIs realistic; if a demo note is needed, put one quiet caption in the outer chrome.
3. **Add live parameter knobs & copyable snippets on technical/component labs.** Most system demos (`animation_spring`, `text_shadow`, `text_stroke`, `themes`, `grid_lab`, `cards`) show static specimens. Giving the user one live slider/toggle + a `"Copy config"` button turns a passive gallery into a tool developers actually love using.
4. **Normalize typography, ASCII glyphs, and keycap alignment.** Replace ASCII `v`, `>`, `->` with proper chevrons/arrows (`▾`, `›`, `→`), center keycap labels inside snug badges (`deadspace_settings` has left-aligned `[Enter   ]`), and use tabular monospace alignment for numeric readouts and counters.
5. **Eliminate pure-white-on-tinted-canvas jumps and harsh 1px outlines.** Soften card fills toward their background's color world (`layout`, `text_shadow`) and commit to one depth strategy per screen (subtle shadow *or* hairline border, not mixed).

---

## Animations (13 screens)

### 1. `animation_basic` (`src/systems/screens/AnimationBasicDemo.h`)
1. Add an easing-curve selector (Linear / Ease-Out / Ease-In-Out) so viewers can feel the difference on the same fade, slide, and scale tracks.
2. Replace the heavy card borders with whisper-quiet surface elevation so the animated shapes are the visual hero.
3. Show a live normalized progress bar (`0.0 → 1.0`) and elapsed ms readout under each tile so users see *how* the property interpolates.
4. Add a `0.25×` slow-motion toggle (matching `animation_interactive`) for inspecting frames.
5. Dim `"Done / Idle"` status text and highlight `"Running"` in the track's accent color so state transitions read at a glance.

### 2. `animation_declarative` (`src/systems/screens/AnimationDeclarativeDemo.h`)
1. Group the 11 trigger buttons into two clear columns — **Entry triggers** vs **Interaction triggers** (`Hover scale`, `Click to scale`, `Hover + click`) — instead of one flat grid.
2. Show the actual declarative trigger expression (`on_hover → scale(1.08)`) in monospace below each control so developers can copy the pattern.
3. Add a live `"Last triggered"` event log strip (last 3 events with timestamps) so trigger firing order is visible.
4. Give each tile a resting-bounds hairline ghost outline (like `animation_spring`) so scale/slide deltas are measurable.
5. Add a `"Reduced motion"` toggle that demonstrates how declarative blocks gracefully degrade.

### 3. `animation_interactive` (`src/systems/screens/AnimationInteractiveDemo.h`)
1. Show a live press-scale curve readout (`1.00× → 0.94×`) beneath each button while pressed so independent instances are visually provable.
2. Add keyboard activation (`1`, `2`, `3` or `Space`/`Enter`) with visible focus rings to prove press-scale works identically for keyboard and mouse.
3. Differentiate the 3 buttons subtly by label/icon (e.g., `Save`, `Share`, `Archive`) instead of three identical `"Press"` labels, making independent state obvious.
4. Move `"Preview all at 1/4 speed"` and `"Reset counts"` into a clean bottom toolbar separated by a hairline rule.
5. Give the click counter badge tabular monospace numerals so numbers don't jitter as counts increment.

### 4. `animation_looping` (`src/systems/screens/AnimationLoopingDemo.h`)
1. Add a Pause / Play toggle (and `Space` shortcut) so viewers can freeze a loop mid-phase and inspect it.
2. Add a speed multiplier pill (`0.5× / 1× / 2×`) and a global `"Reduced motion"` switch that cleanly halts loops.
3. Draw a subtle phase indicator (`0°–360°` or sine-wave dot) under Pulse, Bounce, and Spin so users see the underlying math.
4. Standardize card padding and vertical alignment of the animated targets across all columns so the row feels cohesive.
5. Soften the dark card borders and tint the background surface slightly per loop phase instead of static harsh boxes.

### 5. `animation_spring` (`src/systems/screens/AnimationSpringDemo.h`)
1. Fix the `"start"` ghost outline on `04 Drop` so it sits cleanly above the resting box with inner padding instead of clipping into the green square.
2. Dim `"Settled"` to muted foreground and light up `"Running"` in each card's accent color so active motion stands out immediately.
3. Tone down the 5 neon rainbow borders to whisper-quiet surface elevation so the spring physics—not the outlines—command attention.
4. Add interactive Frequency / Decay sliders (plus `"Copy spring params"`) on the selected sample so developers can tune springs live.
5. Replace the heavy boxed top header with clean typographic hierarchy directly on the canvas.

### 6. `animation_stress_lab` (`src/systems/screens/AnimationStressLab.h`)
1. Pin the live frame-time / FPS / active-tracks readout in a fixed monospace HUD strip so layout never shifts while dragging the stress slider.
2. Add quick preset buttons (`50`, `250`, `1,000`, `Max`) alongside the slider for instant benchmarking.
3. Color-code the frame-time readout semantically (green `<8ms`, amber `8–16ms`, red `>16ms`) instead of plain white text.
4. Label the animated `"Ab"` cells with a concise legend explaining which animation types are running concurrently.
5. Add a `"Pause simulation"` button so developers can inspect individual animated entities mid-flight.

### 7. `colour_track_lab` (`src/systems/screens/ColourTrackLab.h`)
1. Show a side-by-side **OKLab vs sRGB** gradient strip for the exact same start/end swatches so the perceptual advantage of OKLab is immediately obvious.
2. Display live hex + OKLab `(L, a, b)` coordinates during interpolation in tabular monospace.
3. Add a scrubbable `t = 0.0 → 1.0` slider next to `"Replay"` so users can inspect the midpoint without re-triggering.
4. Put a subtle checkerboard behind the alpha-track specimen so transparency changes read clearly on Cozy Kraft.
5. Add a `"Copy OKLab stops"` button so developers can grab the color values directly.

### 8. `curved_text_lab` (`src/systems/screens/CurvedTextLab.h`)
1. Draw an optional faint baseline arc / circle guide toggle so users can verify exact glyph tangency along the curve.
2. Add a live editable text field (with emoji/grapheme cluster presets like `"Café ✨ 東京"`) to prove grapheme-aware curvature interactively.
3. Group the radius, arc-angle, and font controls into a neat side inspector panel instead of crowding the canvas.
4. Show per-glyph bounding-box debug overlay as a toggle for developers verifying kerning/rotation.
5. Soften the neon-dark control borders and align slider value labels right-justified in monospace.

### 9. `curved_text_stress_lab` (`src/systems/screens/CurvedTextStressLab.h`)
1. Render stage timings (`layout`, `shape`, `draw`) as a compact stacked horizontal bar rather than raw text numbers alone.
2. Add a `"Copy timing CSV"` button so benchmark runs can be pasted straight into PRs or spreadsheets.
3. Highlight the bottleneck stage in the accent color and keep the other stages neutral gray.
4. Add quick ring-count presets (`1`, `5`, `15`, `30`) alongside the slider.
5. Keep the timing HUD anchored at top-right so changing density never pushes the metrics down.

### 10. `motion_button` (`src/systems/screens/MotionButtonCheckpoint.h`)
1. Display the button's live state machine (`Idle → Hover → Press → Focus`) as a horizontal 4-pill indicator that lights up in real time.
2. Show the active spring/tween scale and offset values numerically in monospace next to `"Hover, press, focus me"`.
3. Align `"Corner pinned"`, `"Child"`, and `"Scaled"` into a structured 3-card comparison row with clear 1-line captions explaining what each verifies.
4. Make the `"Reduced motion"` checkbox visually show *what* changed (e.g., `"instant swap"` badge).
5. Give the primary specimen button a clearer focus ring contrast on Cozy Kraft so keyboard focus is unmistakable.

### 11. `motion_modal` (`src/systems/screens/MotionModalCheckpoint.h`)
1. Visualize the 4-phase lifecycle (`hidden → enter → visible → exit`) as an always-visible state ribbon at the top of the screen so users watch transitions settle.
2. Add a `0.25×` slow-motion toggle so the enter/exit backdrop and scale choreography are easy to inspect.
3. Make `"Close then reopen"` visually log the interruption event (`interrupted exit at t=0.42`) to prove interruptibility.
4. Add subtle soft shadow elevation to the modal card so it lifts cleanly off the Cozy Kraft backdrop.
5. Show `"Esc"` and `"Click backdrop"` keyboard/pointer affordance hints inside the modal footer.

### 12. `text_motion_lab` (`src/systems/screens/TextMotionLab.h`)
1. Add per-row `"Replay"` icon buttons next to each of the 9 animation styles so users don't have to hit `"Reset all"` every time.
2. Label each row with its exact `text_unit_motion` preset name and unit granularity (`char` / `word` / `line`) in muted monospace.
3. Add a `"Stagger delay"` slider (`0ms–120ms`) to let users feel batching and rhythm live.
4. Add a `"Reduced motion"` toggle that switches all 9 rows to subtle opacity fades.
5. Increase vertical row spacing slightly so animated ascenders/descenders never visually crowd adjacent rows.

### 13. `transitions_lab` (`src/systems/screens/TransitionsLab.h`)
1. Add a sticky category filter bar (e.g., `All (43)`, `Modals`, `Menus`, `Controls`, `Feedback`) so users aren't scrolling blindly through 43 transitions.
2. Show a live `"Currently playing"` counter and duration/easing badge on each card when triggered.
3. Ensure `"Search the transitions"` focuses with `/` and shows a `"Clear"` button + match count (`12 / 43`).
4. Make `"Reduced motion"` a prominent top-bar pill with a visual indicator explaining which transitions swap to cross-fade.
5. Standardize card height, internal padding, and subtle hairline borders across all 43 specimens so the catalog feels like an authored reference book.

---

## App Mockups (5 screens)

### 14. `aim_chat` (`src/systems/screens/AIMChatDemo.h`)
1. Fix the low-contrast white smiley on bright yellow in the chat header (use dark ink on yellow or a crisp 1px dark border like classic AIM icons).
2. Make the formatting toolbar (`B`, `I`, `U`, color) visibly toggle active beveled-down states and apply to the preview or show live feedback instead of appearing static.
3. Replace `"Demo log..."` floating on the toolbar right edge with a subtle clickable `"Chat Log"` button or remove the placeholder text.
4. Make clicking buddies in the Buddy List (`sk8ergirl88`, `xXguitarguyXx`) switch the active IM conversation thread (even with 2–3 lines of flavor text) so the buddy list feels alive.
5. Add subtle padding inside the Windows 98 `Start` button and classic clock tray so text never crowds the 3D bevel edge.

### 15. `media_library` (`src/systems/screens/MediaLibraryGrid.h`)
1. Add a genre/sort filter pill bar (`All`, `Recent`, `A–Z`) beside `"Search your library"` to show paged rebuilding under live filtering.
2. Give poster cards a crisp hover lift/border highlight and a subtle focus ring for keyboard browsing.
3. Display a quiet `"Built 12 of 240 items (Page 1/20)"` telemetry pill in the footer so the virtualization/paging win is immediately visible without breaking the app aesthetic.
4. Add a clean empty-search state (`"No titles match '…'"` + `"Clear search"` button) when filtering returns zero items.
5. Ensure the detail modal uses consistent spacing, Esc-to-close hint, and muted backdrop dimming.

### 16. `offsite_backup` (`src/systems/screens/OffsiteBackupApp.h`)
1. Distinguish **Uploading** (active coral) from **Queued** (neutral muted slate) in the folder progress bars so queued items don't look like warnings or active transfers.
2. Promote `"Pause backup"` to primary visual weight (matching its importance) and demote `"Rescan"` to a quiet secondary button.
3. Add rounded line caps and slightly more breathing room between the `31% COMPLETE` ring and `"Backing up your files"`.
4. Replace ASCII `>` in `"Manage account >"` with a crisp chevron (`›`) and add a hover/click breakdown popover on the `Account storage` bar.
5. Wire the `?` help button (and top menu items) to a lightweight diagnostics/shortcuts popover so no visible chrome feels dead.

### 17. `podcast_library` (`src/systems/screens/PodcastLibrary.h`)
1. Add a real-time playhead scrubbing bar with elapsed/remaining tabular timestamps (`12:34 / 45:00`) in the mini-player.
2. Show a clear `"Now Playing"` indicator (subtle accent bar + animated equalizer bars or `"Playing"` badge) on the active episode row.
3. Add playback speed (`1× / 1.25× / 1.5× / 2×`) and `±15s` skip buttons to make the playback preview feel genuinely usable.
4. Give episode rows consistent thumbnail aspect ratios, subtle hover shading, and truncated 2-line descriptions.
5. Add a `"Played / Unplayed"` filter or bookmark toggle so the library feels like software you'd actually open daily.

### 18. `secure_tunnel` (`src/systems/screens/SecureTunnelMockup.h`)
1. Replace the heavy flat-gray `"Privacy, wherever you go."` overlay box covering South America with a translucent, whisper-quiet floating card tucked into the bottom-left of the map.
2. Deduplicate the simulation disclaimer — it currently appears in 4 places (`Demo mode`, `Network simulation only`, `Demo connection`, and inside the map banner). Keep ONE quiet `"Simulated"` pill in the top-right.
3. Color-code latency numbers subtly (green `<50ms`, amber `50–120ms`, muted coral `>120ms`) and add a `"Fastest"` badge on Reykjavik (`14 ms`).
4. Replace ASCII `>` chevrons on every location row and `"View diagnostics >"` with proper `›` glyphs and align them cleanly.
5. Animate a subtle pulse ring on the selected/connected map pin and show a live simulated throughput/uptime counter when connected.

---

## Game Mockups (20 screens)

### 19. `angry_birds_settings` (`src/systems/screens/AngryBirdsSettings.h`)
1. Brighten the top-left `5 / 5 lives` and top-right `1000 coins` HUD pills (or give them translucent dark badges) — right now they are barely readable against the dark forest backdrop.
2. Visually desaturate/dim the big round icon buttons when their toggle is OFF (`VIBRATION` is bright blue while OFF, which reads as active at a glance).
3. Center the active-tab underline cleanly beneath `"AUDIO"` and give inactive tabs a softer saturation so the active tab owns the green pop.
4. Disable `"APPLY"` visually when `"All changes saved"` (and enable with a warm highlight when dirty) so dirty-state feedback is unmistakable.
5. Add a subtle sound/haptic visual pulse ripple when tapping an icon toggle so it feels chunky and responsive like a mobile game.

### 20. `casual_settings` (`src/systems/screens/CasualSettings.h`)
1. Unify `"TERMS & PRIVACY"` and `"ABOUT / OPEN DETAILS"` button styling with clear right-aligned chevrons instead of slash-separated labels.
2. Add subtle press-scale feedback and clear focus rings on every settings row so keyboard/gamepad navigation feels intentional.
3. Group local progress actions (Reset Progress, Export/Backup) separately from preferences with a hairline divider and distinct destructive styling.
4. Show a clean confirmation toast (`"Preferences saved"`) when toggling options so changes feel persistent.
5. Tighten vertical rhythm and symmetrical card padding across all sections so the mobile frame feels balanced.

### 21. `cozy_cafe` (`src/systems/screens/CozyCafe.h`)
1. Give each of `"Today's Specials"` distinct descriptions and coin rewards instead of repeating `"promotion earns 25 coins"` three times.
2. Let clicking a customer card (like urgent red `LOW` Oliver) target `"Serve Oliver"` directly — or show a clear queue-order badge (`#1`, `#2`) explaining why Emma is next.
3. Replace `"DevTeam_Support: Check out the new update!"` in Guild chat with cozy in-world chatter (`"BakerMomo: fresh matcha batch ready!"`) and give the chat box a warmer, higher-contrast parchment-brown tone.
4. Remove the redundant `"No notices"` sub-caption under `Crafting` (and tighten `"2 notices"` / `"1 recipe note"` since the red corner badges already say `2` and `1`).
5. Wrap the `Music: 70%` slider in a quiet rounded pill with a mute icon button so it doesn't float loose above Guild chat.

### 22. `deadspace_settings` (`src/systems/screens/DeadSpaceSettings.h`)
1. Center `"Enter"` and `"Esc"` inside snug, proportional keycap badges — currently they sit flush-left inside overly wide white rectangles.
2. Increase horizontal spacing between the ghosted background `"INITIAL SETTINGS"` panel and the foreground `"MORE SETTINGS"` panel so `"More Settings"` doesn't overlap `"Policies and Licenses"`.
3. Add a subtle holographic scanline or vertical cyan glow bar on the selected row (`> Gameplay`) to heighten the diegetic RIG feel.
4. Right-align `"4 entries / Enter to open"` at the bottom of the right-hand info pane with consistent inner margins.
5. Dim `"INFORMATION"` section header with a thin cyan hairline rule so the divider between gameplay settings and legal/credits reads cleanly.

### 23. `empire_tycoon` (`src/systems/screens/EmpireTycoon.h`)
1. Add breathing room between the `0` / `100` gauge ticks and the `"Happiness 85%"` / `"Resources 60%"` pill badges so the numerals don't collide with the pill border.
2. Clarify the `"Speed Up / +5% work"` button subtext (`"Free / +$25,000 cash"` reads backwards — either make it cost cash or label it `"Boost +5% & Collect $25k"`).
3. Replace `"DEVTEAM Update v2.0 is live"` in the bottom-left ticker with in-world park events (`"Park News: Ferris Wheel queue is booming!"`).
4. Highlight the active left-rail tab (`Rides`) with a bold shadow/border offset and dim unselected tabs slightly so navigation state is instant.
5. Animate the `+$` delta numbers (`+0` → `+120/min`) in bright emerald when rates increase so production feedback feels rewarding.

### 24. `fighter_menu` (`src/systems/screens/FighterMenu.h`)
1. Fix `"System Options"` overlapping the bottom edge of the translucent teal gear card — anchor it cleanly inside or below the preview frame with consistent margins.
2. Hide or contextualize the floating `"Offline Mode"` badge when the `"Options"` tab is active (it currently contradicts the active tab).
3. Add inner horizontal padding to the lime-green `"OPTIONS / SYSTEM"` strip so text isn't flush against the left edge.
4. Give the angled menu bars a crisp hover/focus slide-in offset (`+6px` right) so switching rows feels snappy like an arcade fighter.
5. Center the bottom keycap legends (`Select`, `Choose`, `Reset`, `Change tab`) with consistent keycap widths and subtle dark backing.

### 25. `flight_options` (`src/systems/screens/FlightOptions.h`)
1. Add a crisp tactical radar/horizon reticle accent and monospace tabular readouts (`FOV 90°`, `DEADZONE 0.12`) to sell the Ace Combat cockpit feel.
2. Show a clear `"Modified"` indicator dot next to any changed preference plus a `"Revert unsaved"` button.
3. Align value steppers (`< Value >`) on a strict right column so every row scans like an avionics checklist.
4. Add a compact description footer that updates as you focus each setting (explaining pitch inversion, HUD opacity, etc.).
5. Ensure disabled rows use distinct muted slate contrast without looking broken.

### 26. `islands_trains_settings` (`src/systems/screens/IslandsTrainsSettings.h`)
1. Soften borders and use warm pastel elevation shifts (sand, seafoam, terracotta) to match the calm diorama aesthetic.
2. Add a confirmation step or undo toast on `"RESET TO DEFAULTS"` so accidental clicks can't wipe preferences silently.
3. Group `"Keyboard controls"` and `"PLAY TUTORIAL"` in a dedicated Help/Guides section separated from Audio/Video toggles.
4. Give sliders and toggles gentle rounded pill geometry with clear active/focus rings.
5. Add a subtle visual preview swatch (e.g., daylight / dusk / night tile) when toggling visual ambiance settings.

### 27. `kart_select` (`src/systems/screens/KartSelect.h`)
1. Display driver/kart stat bars (Speed, Acceleration, Handling, Weight) with delta highlights (`+2` green / `-1` red) when hovering another kart so comparison is instant.
2. Make the `P1` / `SELECTED` badge sit cleanly inside the portrait frame without clipping borders.
3. Add distinct keyboard/gamepad prompts (`←/→ Character`, `↑/↓ Kart`, `Enter Confirm`) anchored cleanly in a bottom bar.
4. Give unselected portraits a subtle desaturated overlay so the active driver pops immediately.
5. Add a `"Randomize"` button (`R`) with a fun brief roulette tick for party-game delight.

### 28. `kirby_options` (`src/systems/screens/KirbyOptions.h`)
1. Give the player profile `text_input` a cheerful rounded pill container with a live character counter (`8 / 16`) and clear focus glow.
2. Add soft warm drop shadows (one consistent depth strategy) to the option tiles instead of mixing flat borders and heavy boxes.
3. Highlight the selected tile with a bouncy star/arrow cursor and warm pastel tint.
4. Add a `"Reset to Defaults"` confirmation banner or undo toast so profile edits feel safe.
5. Align all preference values on a neat right column so the board reads cleanly at a glance.

### 29. `marlo_kart` (`src/systems/screens/MarloKart.h`)
1. Add a persistent minimal top-right phase indicator / quick-jump bar (`Title · Select · Race · Podium`) so demo viewers can also jump straight to any state without replaying a full lap.
2. Tighten the Race HUD (lap counter, position badge, minimap/progress bar, speed) with high-contrast outlines and tabular numerals so numbers never jitter mid-race.
3. Show live keybind reminders (`WASD / Arrows`, `Space Drift`, `Esc Pause`) in a subtle translucent pill during the first 3 seconds of a race.
4. Polish the Podium screen with medal color accents (gold/silver/bronze), point deltas (`+15 pts`), and a clear primary `"Next Race"` vs secondary `"Title"` hierarchy.
5. Ensure the Pause modal dims the race cleanly and restores focus to `"Resume"` on open.

### 30. `mini_motorways_settings` (`src/systems/screens/MiniMotorwaysSettings.h`)
1. Remove the verbose `"Local account connection demo. No account is contacted"` sentence from the main flow; replace with a crisp `"Offline Demo"` status chip.
2. Give the `"Tutorial guide"` modal crisp numbered step badges (`1`, `2`, `3`) and generous vertical line-height to match Mini Motorways' iconic Scandinavian minimalism.
3. Use a single restrained accent color (coral-red or motorway-blue) across active toggles, sliders, and `"Got it"`, keeping everything else warm cream and charcoal.
4. Make `"Open Settings"` when closed show a subtle miniature road-grid backdrop so the closed state still feels in-world.
5. Align slider thumbs, labels, and percentage readouts on a strict 8px spacing grid.

### 31. `neon_strike` (`src/systems/screens/NeonStrike.h`)
1. Add subtle dark translucent backing plates behind HUD clusters (ammo, health, minimap, killfeed) so text remains legible over bright scene areas.
2. Use tabular monospace digits for ammo (`30 / 120`), HP (`100`), and round timer (`01:42`) with low-ammo amber/red threshold color shifts.
3. Add interactive demo triggers (`Fire`, `Reload`, `Take Damage`, `Toggle Low HP`) in a discreet debug dock so viewers can test HUD states live.
4. Tone down competing neon accents to ONE primary tactical cyan + semantic red for damage/low ammo.
5. Respect safe-area margins (24–32px inset from screen edges) so corner HUD elements never hug the viewport border.

### 32. `parcel_corps_settings` (`src/systems/screens/ParcelCorpsSettings.h`)
1. Frame the phone mockup with a clean status bar (time, battery, signal) and consistent inner padding so it reads unmistakably as a handheld device.
2. Make `"Lv 0"` / progression badge respond to a quick interactive test toggle or clarify its meaning in the phone header.
3. Use chunky, thumb-friendly 48px+ row heights and rounded pill toggles fitting a mobile courier phone UI.
4. Add a clear back-arrow header inside sub-panels and a subtle drop shadow behind the phone chassis.
5. Ensure the `"Open settings"` closed state centers cleanly with a visual phone-icon cue.

### 33. `potion_crafting` (`src/systems/screens/PotionCrafting.h`)
1. Show ingredient stock counts as crisp `have / need` badges (`3 / 2` green, `1 / 3` red) right on each recipe row so craftability is obvious before clicking.
2. Add a visual brewing result feedback toast/flash (`"Brewed: Elixir of Swiftness +1"`) and decrement/update inventory counts live.
3. Differentiate selected recipe rows with a warm parchment glow + left accent bar (similar to `shop_interface`) rather than harsh borders.
4. Add a `"Craftable only"` filter toggle and `"Restock ingredients"` reset button so users can experiment endlessly.
5. Give the cauldron/result preview panel a dedicated focal frame with potion effect stats and rarity badge.

### 34. `powerwash_settings` (`src/systems/screens/PowerWashSettings.h`)
1. Style the `[A]` / `[D]` tab-switching keycaps with crisp centered labels and subtle hover/click support so mouse users can click them too.
2. Use crisp water-blue + clean-white surface contrast with subtle borders so the UI feels fresh and pressurized like PowerWash Simulator.
3. Add a right-hand setting preview/description card that updates as each row is hovered or focused.
4. Show a clear `"Unsaved changes"` banner with `Apply` / `Revert` when any option is edited.
5. Improve the `"Settings closed"` state with a cleaner centered card and `[Esc]` shortcut hint to reopen.

### 35. `race_results` (`src/systems/screens/RaceResults.h`)
1. Right-align `Race time` and `Race points` in tabular monospace so decimal points (`1:24.38`) and point deltas (`+15`) line up vertically.
2. Highlight `Player 1`'s row with a distinct warm gold/blue accent strip and subtle glow so the player finds themselves in <100ms.
3. Add medal badges (Gold `1st`, Silver `2nd`, Bronze `3rd`) in the `POS.` column instead of plain text.
4. Visually separate `RACE RESULTS` and `CUP STANDINGS` with clear section hierarchy or a clean tabbed/split toggle.
5. Add an interactive `"Simulate Next Race"` button that updates times and re-sorts Cup Standings live!

### 36. `rubber_bandits_menu` (`src/systems/screens/RubberBanditsMenu.h`)
1. Give the bold cartoony menu buttons thick, consistent comic-style offset shadows (single depth style) and energetic hover tilt/scale.
2. Ensure high contrast on every label (dark outline or crisp ink on bright yellow/coral fills) so party-game energy stays 100% legible.
3. Add clear gamepad/keyboard hints (`↑↓ Navigate`, `Enter Pick`, `Esc Close`) in a playful bottom pill bar.
4. Make `"OPEN MENU"` closed state show a lively title badge rather than a lone bare button.
5. Align button heights, corner radii, and spacing rhythm so the playful layout still feels engineered, not chaotic.

### 37. `shop_interface` (`src/systems/screens/ShopInterface.h`)
1. Align the two decorative diamond ornaments symmetrically around `"General Store"` (currently the right diamond floats mid-air).
2. Increase contrast and legibility on the tiny red `"Low stock"` sub-label under stock counts (e.g., a crisp tinted pill `3 left`).
3. Unify `"Add to basket"` and `"Purchase / 180 G"` button border treatments so primary vs secondary CTA hierarchy is effortless.
4. Show an inline `"Bought! -180 G"` feedback toast and disable `"Purchase"` with an `"Insufficient gold"` tooltip if basket exceeds `YOUR GOLD`.
5. Make the `"Sell"` tab feel equally rich with player-inventory item badges and resale-value comparison (`50% value`).

### 38. `sports_settings` (`src/systems/screens/SportsSettings.h`)
1. Give `[LB]` / `[RB]` shoulder-bumper pills centered text and clickability, plus an animated underline sliding beneath the active category tab.
2. Remove the meta-caption `"Settings preview / changes stay local"` from the main panel; show a clean `"Modified"` badge only when values differ from default.
3. Show `"Default: …"` comparison subtly on the right-hand info panel with a subtle tick mark on each slider indicating the default value.
4. Style `[Y] Reset` and `[B] Back` in a crisp dark broadcast-style footer bar with generous spacing.
5. Ensure disabled rows and active focus rows have unmistakable contrast separation.

---

## Layout (3 screens)

### 39. `baseline_align` (`src/systems/screens/BaselineAlignShowcase.h`)
1. Draw a crisp 1px horizontal accent laser line across the shared text baseline in the `Baseline` row so alignment is immediately verifiable by eye.
2. Add an interactive toggle to switch sample text (e.g., descenders `"gjpqy"` vs caps `"HXM"` vs CJK/digits) to prove baseline math on tricky glyphs.
3. Label each row (`FlexStart`, `Center`, `FlexEnd`, `Baseline`) in a fixed-width left gutter column so the text boxes align vertically across rows.
4. Show the computed pixel Y-offset of each mixed-size run (`16 / 48 / 24 / 15 px`) in tiny muted monospace below the box.
5. Soften container borders so the baseline guide line is the strongest visual element on screen.

### 40. `rotated_controls` (`src/systems/screens/RotatedControlsShowcase.h`)
1. Add a live rotation angle slider (`-180°` to `+180°`) + quick preset pills (`-15°`, `0°`, `30°`, `45°`, `90°`) so users can test arbitrary angles.
2. Add a `"Show hit-box & AABB"` overlay toggle that draws the true rotated hit quad vs axis-aligned bounding box in real time.
3. Show a live click/hover indicator (`Hovered: Button B · Clicks: 3`) so hit-area accuracy is provable without guessing.
4. Highlight the rotated focus ring clearly when tabbing between rotated buttons.
5. Give each rotated button enough outer margin so corners never clip or touch neighboring cards at `45°`.

### 41. `rotated_subtree` (`src/systems/screens/RotatedSubtreeShowcase.h`)
1. Add a container rotation slider and `"Reset 0°"` button so users can rotate the entire parent subtree dynamically.
2. Draw a subtle pivot crosshair at the rotation origin so it's obvious *around which point* the subtree rotates.
3. Show live click counters directly on `"Inner A"` and `"Inner B"` (and parent container) to prove nested rotated hit-testing.
4. Add a second nested control type (e.g., a small checkbox or slider inside the rotated card) to demonstrate subtree generality.
5. Soften the parent card border and add a subtle corner badge showing the current angle (`12°`).

---

## Stress Tests (2 screens)

### 42. `guess_who_lab` (`src/systems/screens/GuessWhoLab.h`)
1. Dim/flip eliminated character cards with a crisp translucent `"X"` or folded-down state so remaining suspects pop out immediately.
2. Show a live `"Remaining: 14 / 24"` counter and turn history log (`"Has glasses? → Yes"`) in a clean side or top strip.
3. Highlight the selected dropdown trait cleanly and ensure dropdown popups never clip off-screen near the bottom/right edges.
4. Add a `"Make Guess"` mode toggle with a celebratory win/loss banner so the game is genuinely fun to play end-to-end.
5. Keep the 26-dropdown stress readout (open count, layout ms) in a compact monospace corner badge so it doesn't clutter the board.

### 43. `minesweeper_lab` (`src/systems/screens/MinesweeperLab.h`)
1. Give classic Minesweeper number colors (`1` blue, `2` green, `3` red, `4` navy, `5` maroon) bold, crisp typography centered cleanly in each of the 256 cells.
2. Add difficulty presets (`Beginner 9×9`, `Intermediate 16×16`) and a live chord-click hint in `"Help"` so players enjoy testing it.
3. Style the mine counter and timer as classic red 7-segment-feel tabular monospace LCD readouts with consistent bevels.
4. Give the smiley reset button expressive states (neutral, `:O` on mouse-down, sunglasses on win, `X_X` on mine hit).
5. Ensure right-click flagging and first-click safety work smoothly, and show a subtle `"256 live entities · X.X ms"` perf pill in the taskbar.

---

## Text (1 screen)

### 44. `text_scale` (`src/systems/screens/TextScaleShowcase.h`)
1. Add a smooth continuous slider (`0.75×` – `2.00×`) alongside discrete preset pills (`100%`, `125%`, `150%`, `200%`) for instant scaling previews.
2. Show a realistic mini UI card (heading, body paragraph, button, badge, input) next to the specimen ladder so users see layout reflow under `Theme::text_scale`.
3. Display both base px and resolved scaled px (`16px → 24px`) in muted monospace next to each specimen line.
4. Highlight any clipping/overflow risk automatically if a fixed-height container gets tight at `200%`.
5. Add a `"Reset 1.0×"` shortcut button that stays fixed in size/position so you never lose the reset control at large scales.

---

## Component Galleries (46 screens)

### 45. `active_tab_styles` (`src/systems/screens/ActiveTabStyles.h`)
1. Place Default vs Caller-Styled tab bars side by side with identical tab content so styling deltas are immediately comparable.
2. Add an interactive underline/pill/filled active-indicator switcher so developers can compare tab treatments live.
3. Show the exact style overrides (`active_bg`, `active_border`, `indicator_height`) in a compact muted caption beneath each bar.
4. Add keyboard focus ring visibility (`Tab` / `←` / `→`) on both default and styled tabs.
5. Soften the outer container borders so the active tab highlight is the primary visual signal.

### 46. `button_variants` (`src/systems/screens/ButtonVariantsShowcase.h`)
1. Arrange Filled, Outline, and Ghost as crisp matrix columns against sizes (`SM`, `MD`, `LG`) and states (`Default`, `Hover`, `Disabled`) so the grid reads like a design-system spec sheet.
2. Add a live theme switcher pill bar (`Cozy Kraft`, `Neon Dark`, `Ocean Navy`, `Sage Natural`) right in the header since this screen already imports all 4 presets.
3. Show a live `"Last clicked: Outline / MD"` status chip so every specimen feels responsive, not decorative.
4. Ensure Outline and Ghost variants have crisp, consistent 1px borders and subtle hover tint fills (no sudden opaque jumps).
5. Include an icon+label button row to demonstrate alignment and gap rhythm across all three variants.

### 47. `buttons` (`src/systems/screens/Buttons.h`)
1. Replace raw hardcoded `Color{}` overrides with theme-derived semantic tokens so button hierarchy stays cohesive on Ocean Navy.
2. Separate **Theme Roles** (Primary/Secondary/Accent/Danger), **Sizes**, **States** (including Loading/Disabled), and **Button Groups** into distinct quiet surface cards.
3. Add a live press/focus state indicator strip so keyboard activation (`Tab` + `Space`/`Enter`) is visibly demonstrated.
4. Keep button group inner dividers whisper-quiet (`1px` subtle alpha) with outer corners rounded and inner corners sharp (`0px`).
5. Add a `"Copy button config"` or compact spec label (`height / padding / radius`) beneath each size row.

### 48. `cards` (`src/systems/screens/Cards.h`)
1. Fix the low-contrast muted-brown text on the `Primary` card, `Primary` swatch, and `"Hide guides"` / `"Compare opaque fill"` buttons — boost foreground contrast to WCAG AA+.
2. Soften the checkerboard contrast behind `"Semi-Transparent"` (or add a subtle translucent text backing) so the dark copy remains effortlessly readable while still proving alpha blending.
3. Right-align the `140 x 100 px / requested 1 px border #968273` metadata cleanly with the right edge of the 4-card grid instead of floating mid-row.
4. Replace the heavy solid brown header banner with clean typographic hierarchy + a subtle hairline divider on the Cozy Kraft canvas.
5. Add a live corner-radius / alpha slider so viewers can scrub `0px → 16px` roundness and `0% → 100%` fill opacity interactively.

### 49. `checkboxes` (`src/systems/screens/CheckboxShowcase.h`)
1. Remove the full-row saturated teal/purple/crimson background fills on checkbox rows — use a quiet dark surface and let the checkbox mark itself carry the Primary/Secondary/Accent color.
2. Fix the very low-contrast dark-gray `"Checked"` / `"Unchecked"` status labels on the right of each row (use crisp muted foreground for Unchecked and bright accent/foreground for Checked).
3. Visually highlight the `"Min 1, max 2"` constraint on `Delivery channels` with a clean pill badge (`1 / 2 selected`) and a gentle disabled lock icon on the last remaining item.
4. Align `"Box Only / 44 x 44 design px"` boxes evenly with crisp centered captions and subtle hover rings.
5. Add an indeterminate (`—`) checkbox state example (e.g., `"Select all news"` parent checkbox) to complete the real-world checklist pattern.

### 50. `circular_progress` (`src/systems/screens/CircularProgressShowcase.h`)
1. Add a live progress scrub slider (`0%–100%`) plus an `"Auto-animate"` toggle so every radial style updates smoothly in unison.
2. Standardize ring sizes, stroke widths, and centered tabular monospace percentage labels (`65%`) across all style cards.
3. Show both determinate (exact `%`) and indeterminate (spinning arc) circular indicators side by side.
4. Use semantic color progression (neutral → accent → green checkmark at `100%`) on one specimen to demonstrate real-world completion craft.
5. Soften the surrounding card borders so the circular arcs read cleanly against the surface.

### 51. `config_gap_gallery` (`src/systems/screens/ConfigGapGallery.h`)
1. Group the 5 config capabilities (`shadow`, `clip`, `insets`, `on_draw hooks`, `click activation`) into equally proportioned cards with a clear 1-line API signature header on each.
2. Make the `Inset probe` visually draw shaded padding/margin bands on toggle (like DevTools box model) so insets are self-explanatory.
3. Add a live `"Click activation: Press vs Release"` counter badge that flashes on mouse-down vs mouse-up so the difference is tactile.
4. Ensure `"Narrow text wraps onto two lines."` sits inside a clearly dimensioned box with a subtle width pill (`140px`).
5. Add a subtle light/dark surface toggle for the `shadow` card so soft vs hard shadows are easy to judge.

### 52. `decorative_frame` (`src/systems/screens/DecorativeFrameShowcase.h`)
1. Give each decorative frame style a distinct realistic mini-content inside (title crest, item card, quote) instead of identical placeholder blocks so viewers see how each frame *feels* in use.
2. Add a frame-thickness / corner-accent toggle alongside `"Same neutral colors"` so users can compare geometry vs palette independently.
3. Replace raw hardcoded `Color{}` accents with cohesive warm/neutral surface tokens so the frames don't clash.
4. Label each frame with its exact enum/helper name in quiet monospace beneath the specimen.
5. Ensure symmetrical outer margins and equal card heights across the showcase grid.

### 53. `decorators` (`src/systems/screens/DecoratorShowcase.h`)
1. Show a clear `"Before → After"` or toggleable decorator pill (`+ Badge`, `+ Shadow`, `+ Border`, `+ Padding`) so users immediately see what each higher-order decorator wraps.
2. Replace the generic placeholder quote (`"The only way to do great work..."`) with crisp UI-relevant copy showing real layout behavior.
3. Reduce raw `Color{}` noise to one quiet surface hierarchy + one highlight color marking the decorated layer.
4. Show a live `"Composition stack"` breadcrumb (`card → with_padding → with_border → with_badge`) under the composed example.
5. Standardize card spacing and alignment across all decorator examples.

### 54. `default_theme` (`src/systems/screens/DefaultTheme.h`)
1. Tighten the visual rhythm of `"Your workspace"` so it looks like a flagship SaaS settings page out of the box (generous 24px card padding, crisp 1px subtle borders, aligned form labels).
2. Group `"Project name"`, `"Who can see this project?"`, and `"Notification volume"` into a structured 2-column form (label + helper text on left, control on right).
3. Add a live `"Unsaved changes"` save bar at the bottom that triggers a real toast (`"Workspace saved"`) when clicking Save.
4. Add a quiet `"Show resolved default tokens"` drawer/toggle so developers can inspect the exact default palette without leaving the page.
5. Ensure disabled controls and tooltips demonstrate default contrast cleanly without any custom overrides.

### 55. `dialog_confirm` (`src/systems/screens/DialogConfirmShowcase.h`)
1. Move `"Demo preview only; your actual display stays unchanged."` out of the modal body into a quiet outer footer — keeping the dialog itself realistic.
2. Format `Resolution` and `Refresh rate` as a crisp 2-row diff card with proper `→` arrows (`1920 × 1080 → 2560 × 1440`) instead of plain ASCII `->` text lines.
3. Add subtle soft shadow elevation on the modal over the dimmed backdrop so it lifts off the page cleanly instead of relying solely on a crisp 1px gray border.
4. Right-align `"Keep current settings"` and `"Apply display settings"` with subtle `[Esc]` / `[Enter]` keycap hints.
5. When `"Apply display settings"` is clicked, trigger an actual 15-second revert countdown banner matching the dialog copy.

### 56. `dialog_danger` (`src/systems/screens/DialogDangerShowcase.h`)
1. Style the destructive action button in restrained semantic red with a subtle warning icon, keeping the cancel button as the safe default focus target.
2. Present the target save slot (`Chapter 03`, timestamp, playtime) in a subtle inset summary pill inside the dialog so the user knows *exactly* what is being destroyed.
3. Add soft shadow depth to the dialog card and ensure the dimmed backdrop blocks background hover states cleanly.
4. Show an undo/restore toast (`"Save 03 deleted — Reset save demo to restore"`) after confirming so the action feels tangible.
5. Keep `"Reset save demo"` neatly anchored in the background toolbar once the dialog closes.

### 57. `dialog_fyi` (`src/systems/screens/DialogFyiShowcase.h`)
1. Clearly separate the 3 button weights: **Primary** (`Save changes`), **Secondary** (`Cancel`), and **Ghost/Destructive** (`Discard`) pushed to the left so users never mis-click Discard.
2. Remove `"Editable demo / save data lasts for this session"` from inside the editor header or fold it into a quiet status bar indicator (`"Unsaved · local"`).
3. Highlight the dirty document title (`notes.txt •`) in the backdrop editor so the 3-button unsaved-changes prompt makes immediate contextual sense.
4. Add `[Esc]` (Cancel) and `[⌘S / Enter]` (Save) visual hints and verify keyboard focus starts on Save.
5. Soften the modal border and add a subtle drop shadow to lift the dialog above the editor backdrop.

### 58. `dialog_info` (`src/systems/screens/DialogInfoShowcase.h`)
1. Give the info dialog a crisp info-circle accent badge aligned with the title for instant visual scannability.
2. Tighten body copy to 2–3 scannable lines with generous line-height (`1.45×`) and subtle muted metadata below.
3. Right-align the single acknowledge button (`"Got it"` / `"OK"`) with an `[Enter]` affordance and clean soft shadow on the card.
4. Add a `"Reopen info dialog"` trigger in the backdrop workspace with a subtle `"Dismissed at …"` log line.
5. Ensure backdrop click and `Esc` both dismiss cleanly and restore focus to the trigger button.

### 59. `dialog_prompt` (`src/systems/screens/DialogPromptShowcase.h`)
1. Auto-focus the prompt `text_input` immediately on open and select its initial text so typing works on frame 1.
2. Show inline real-time validation feedback directly below the input (e.g., green check `"Valid name"` vs red hint `"Must be 3–24 characters"`) and disable the Confirm CTA while invalid.
3. Replace the 5 hardcoded `Color{}` values with semantic theme tokens (error, success, border, surface).
4. Show the submitted value immediately in the background workspace card with a subtle highlight flash so the prompt visibly *does* something.
5. Add soft shadow lift and right-aligned `Cancel` / `Create` actions.

### 60. `disclosure_showcase` (`src/systems/screens/DisclosureShowcase.h`)
1. Add a crisp rotating chevron (`▸` → `▾`) on each disclosure header so open/closed state is unmistakable before reading text.
2. Add `"Expand all"` / `"Collapse all"` utility buttons next to the `"Toggles:"` counter in the top bar.
3. Group disclosure rows inside a single bordered card separated by whisper-quiet `1px` dividers (accordion aesthetic) rather than separate heavy boxes.
4. Give the disabled disclosure row a subtle lock/muted chevron and a brief tooltip explaining why it's locked.
5. Add keyboard `Enter`/`Space` toggling with a clean inset focus ring on each header row.

### 61. `drag_drop` (`src/systems/screens/DragDropShowcase.h`)
1. Show a crisp dashed drop-target placeholder card inside the hovered lane *before* release so insertion position is 100% predictable.
2. Elevate the actively dragged card with a subtle soft shadow, slight scale (`1.02×`), and muted ghost at its origin slot.
3. Add lane item-count pills (`To Do 3`, `In Progress 2`, `Done 1`) in each column header that update live on drop.
4. Support keyboard reordering (`Space` to pick up, `←/→/↑/↓` to move, `Enter` to drop) with a subtle hint in the footer.
5. Add an `"Undo last move"` button next to Reset so experimenting feels effortless.

### 62. `dropdowns` (`src/systems/screens/DropdownShowcase.h`)
1. Replace any ASCII `v` chevrons with crisp downward arrows that flip upward (`▴`) when the menu is open.
2. Add soft shadow lift and subtle border contrast to open dropdown menus so they float cleanly above underlying cards.
3. Show a checkmark (`✓`) next to the currently selected option inside the open popup list.
4. Demonstrate a searchable/filterable `"Many options"` dropdown or crisp scroll indicator when the list overflows.
5. Ensure the `"Disabled"` dropdown clearly dims its chevron and border without becoming unreadable.

### 63. `bevel_borders` (`src/systems/screens/ExampleBevelBorders.h`)
1. Organize bevel styles (`Raised`, `Sunken`, `Ridge`, `Groove`) in a clean 2×2 matrix paired with a live thickness stepper (`1px`–`6px`) so users can scrub bevel depth.
2. Add a zoomed `4×` corner loupe diagram showing the exact highlight/shadow pixel edges of the selected bevel.
3. Show interactive press inversion (`Raised` → `Sunken` on mouse-down) on a live classic button specimen.
4. Replace arbitrary raw `Color{}` fills with a cohesive retro-desktop neutral palette (`#C0C0C0` / warm stone) so light/dark bevel edges pop naturally.
5. Label each specimen with its exact API flag and light/shadow contrast ratio.

### 64. `example_borders` (`src/systems/screens/ExampleBorders.h`)
1. Align the border gallery into clear horizontal bands: **Weights** (`1px–4px`), **Corners** (sharp/rounded/per-corner), **Shadows + Borders**, and **Interactive** (`Try bordered button`).
2. Avoid mixing hard shadows, soft shadows, and thick borders in the same visual row without explicit comparison labels.
3. Show hover/focus border-color transitions on `"Try bordered button"`, `"Filled child"`, and `"Outline child"` with a live state badge.
4. Add a `"Background tint"` toggle (dark / light / blueprint) to test border subtlety on different surfaces.
5. Label each card with concise monospace tokens (`1px · r=6px`) instead of verbose prose.

### 65. `nine_slice_borders` (`src/systems/screens/ExampleNineSliceBorders.h`)
1. Add a `"Show 9-slice grid lines"` toggle that overlays the exact corner/edge/center cut guides right on top of the parchment/gold/blue panels.
2. Add a live width/height drag handle (or preset size pills: `Compact`, `Wide`, `Tall`) on one 9-slice panel to prove corners never distort while edges stretch cleanly.
3. Give `"Click Me!"` a visible pressed 9-slice texture/tint state so interactive 9-slice buttons feel tactile.
4. Align `"Nested panels"` (`Gold child`, `Blue child`) with symmetrical inner padding so the nesting hierarchy looks crafted.
5. Display the active slice insets (`top / right / bottom / left px`) in a subtle monospace caption under each texture.

### 66. `separators` (`src/systems/screens/ExampleSeparators.h`)
1. Showcase horizontal, vertical, and labeled separators inside realistic mini-contexts (a settings card, a toolbar, an `"OR"` auth divider) rather than isolated lines in empty space.
2. Add a live thickness (`1px / 2px`) and opacity (`subtle / medium / strong`) switcher so viewers can see whisper-quiet vs structural dividers.
3. Show vertical separator centering inside a flex row of icons/buttons with symmetrical horizontal gaps.
4. Display the resolved separator color token (`Theme::border` / `alpha`) in quiet monospace.
5. Keep Cozy Kraft card padding symmetrical and remove any redundant outer borders that compete with the separators themselves.

### 67. `file_tree` (`src/systems/screens/FileTreeShowcase.h`)
1. Add crisp folder/file type icons (open vs closed folder, `.h`, `.cpp`, `.md`, `.json`) and subtle vertical indent guide lines (`1px` low-alpha) to make deep nesting effortless to scan.
2. Add a live filter input (`"Filter files..."`) at the top of the tree that highlights matching nodes and auto-expands parent folders.
3. Show a right-hand inspector preview card for the selected file (path, size, type, `"Copy path"` button) so clicking rows gives rich feedback.
4. Add keyboard navigation hints (`↑↓ Move`, `←→ Collapse/Expand`, `Enter Open`) in a subtle footer bar.
5. Give hovered and selected rows subtle surface fills with a crisp left accent indicator on the active selection.

### 68. `font_weight` (`src/systems/screens/FontWeightShowcase.h`)
1. Display every weight (`100 Thin` through `900 Black`) in a clean aligned table with weight number, name, pangram, and fallback status badge (`Loaded` vs `Synthesized`).
2. Add a font-size stepper (`14px / 18px / 24px / 32px`) so users can check thin and black weight legibility at both small UI and display sizes.
3. Show a realistic mixed-weight UI row (bold filename `theme.h` + regular path `src/ui` + muted timestamp) with baseline alignment guides toggleable.
4. Add a Light/Dark surface toggle — thin weights look dramatically different on light vs dark backgrounds.
5. Align the `0123456789` numerals row in tabular spacing so glyph width consistency across weights is easy to judge.

### 69. `forms` (`src/systems/screens/Forms.h`)
1. Fix the clipped `"Show FPS"` checkbox at the bottom of the Checkboxes scroll panel — give the scroll viewport clean bottom padding or increase container height slightly so rows never slice in half.
2. Indent `"Synchronize frames with the display."` to align flush with `"V-Sync"`'s label text rather than under the checkbox square.
3. Add a section title (e.g., `"Display & Export"`) to the bottom-right card, replace ASCII `v` in `"1920x1080 v"` / `"Medium v"` with proper chevrons, and give `"Saved 0s"` a subtle check icon + pill.
4. Replace `"More options below / scroll this list"` with a subtle bottom gradient fade or compact `↓ 3 more` pill that disappears once scrolled to the bottom.
5. Add a `"Reset form"` button and show a brief confirmation flash on `"Copy Status"` (`"Copied!"`) when clicked.

### 70. `hstack_showcase` (`src/systems/screens/HStackShowcase.h`)
1. Add interactive Gap (`0 / 8 / 16 / 24px`) and Alignment (`Start / Center / End / Stretch`) pill toggles so users see `hstack()` reflow live.
2. Replace the 8 hardcoded rainbow `Color{}` item boxes with a restrained tonal palette (3 cohesive shades) so layout geometry is the focus.
3. Show a subtle gap-dimension overlay toggle (`12px` ticks between items) so spacing math is transparent.
4. Include one real-world `hstack()` example (e.g., a card header with avatar, title, spacer, and `"Settings"` button) alongside abstract boxes.
5. Keep card padding and row heights uniform across every example band.

### 71. `horizontal_drag` (`src/systems/screens/HorizontalDragShowcase.h`)
1. Add a crisp vertical insertion-bar indicator between horizontal items while dragging so drop targets are unmistakable.
2. Elevate the dragged tile with a soft shadow and slight opacity dim on its vacated slot.
3. Show priority rank badges (`#1`, `#2`, `#3`) that dynamically renumber as cards move between priority lanes.
4. Add keyboard left/right reorder support (`←/→`) with a subtle status readout of the last move.
5. Style `"Reset example"` as a quiet secondary button in the top-right header instead of competing with the lanes.

### 72. `images` (`src/systems/screens/ImageShowcase.h`)
1. Clearly label aspect-ratio / fit modes (`Contain`, `Cover`, `Stretch`, `Pixel-crisp`) with a subtle dotted frame showing the container bounds vs rendered image bounds.
2. Give `image_button` unmistakable hover/press/focus states (subtle brightness lift + border ring) and a live click counter badge.
3. Add a sprite-sheet frame scrubber or animation toggle on the sprite specimen so frame slicing is visibly proven.
4. Display natural dimensions vs rendered dimensions (`128×128 → 96×96`) in quiet monospace below each image.
5. Add a checkerboard backdrop toggle behind transparent PNGs/sprites to verify alpha edges cleanly.

### 73. `meters_gauges` (`src/systems/screens/MetersGaugesShowcase.h`)
1. Add a single master `"Simulate load"` toggle (or scrubbable slider) so all inline meters, paired bars, and gauges respond together smoothly.
2. Use semantic threshold coloring (teal normal, amber `>75%`, coral `>90%`) with crisp threshold tick marks on the tracks.
3. Align numeric readouts (`72%`, `4.2 / 8.0 GB`) right-justified in tabular monospace so columns stay rock-steady during animation.
4. Give paired comparison bars (e.g., Download vs Upload or Team A vs Team B) balanced labels on both ends and a subtle center zero-rule.
5. Replace `"Restart"` with a clean Play/Pause + Reset pair in the header toolbar.

### 74. `advanced_modals` (`src/systems/screens/ModalAdvanced.h`)
1. Give the launchpad grid 4 distinct, cleanly styled preview cards (`Login Form`, `Setup Wizard`, `Settings`, `Feedback`) with a brief 1-line summary of the modal pattern each demonstrates.
2. In the Setup Wizard modal, render a crisp numbered step progress bar (`1 Account → 2 Workspace → 3 Confirm`) with completed checkmarks.
3. Show inline validation errors directly beneath `"Username"` / `"Password"` fields with a gentle shake or red focus border instead of only a top banner.
4. Replace the 16 hardcoded `Color{}` values with Ocean Navy semantic tokens so all 4 modals share one coherent depth and border language.
5. Display the last submitted form payload in a copyable summary card on the main screen after closing a modal so user input feels real.

### 75. `modals` (`src/systems/screens/ModalShowcase.h`)
1. Replace the static `"Progress: 65% / static example"` bar with a real animated progress modal (with `"Cancel"` and `"Complete"` states) so it actually feels alive.
2. Add a soft drop shadow and subtle scale-in entrance on every modal card so depth separation from the backdrop is effortless.
3. Show a live `"Modal stack depth: 2"` indicator when testing stacked modals, and ensure `Esc` dismisses only the topmost layer.
4. Keep the 6 modal trigger cards in a clean 3×2 grid with consistent height, icon badges, and `"Last result"` readouts.
5. Restore keyboard focus back to the exact trigger button that opened the modal upon dismissal.

### 76. `navigation_bar_demo` (`src/systems/screens/NavigationBarShowcase.h`)
1. Pair each navigation bar specimen with a live reactive content stub below it (`"Viewing: Reports"`) so clicking nav items visibly changes state.
2. Show active-item styling clearly (subtle pill fill or crisp bottom indicator bar) plus a hover tint on inactive items.
3. Include a notification badge example (`Inbox 3`) and a disabled nav item to cover real-world navigation states.
4. Add a `"Compact / Wide"` container width toggle to show how `navigation_bar` handles narrow vs wide layouts.
5. Ensure keyboard `←/→` arrow navigation and focus rings work smoothly across all nav items.

### 77. `pagination` (`src/systems/screens/PaginationShowcase.h`)
1. Connect the pagination widget to a live compact data table (e.g., 5 rows per page showing `Showing 11–15 of 120 items`) so paging has visible meaning.
2. Add a `"Total pages"` / `"Page size"` switcher (`5 / 10 / 25`) to demonstrate ellipsis (`1 … 4 5 6 … 20`) dynamically adapting.
3. Give Prev/Next buttons crisp `‹` / `›` icons and clear disabled dimming on page 1 and the last page.
4. Ensure the active page pill has unmistakable contrast and tabular monospace numerals so buttons don't shift width between `9` and `10`.
5. Add keyboard `←` / `→` page-stepping support when the paginator is focused.

### 78. `radio_buttons` (`src/systems/screens/RadioGroupShowcase.h`)
1. Replace any heavy row background fills with quiet surface cards so the circular radio dot is the clear focal signal.
2. Add a horizontal radio-group variant and a rich card-radio variant (title + short description + price/badge) alongside the vertical list.
3. Include a disabled radio option and a live `"Selected value"` summary pill at the top of each group.
4. Ensure inner dot centering is pixel-crisp at `with_roundness(1.0)` and has a subtle focus ring for arrow-key selection.
5. Add a `"Reset selection"` action in the top-right header with consistent Neon Dark styling.

### 79. `real_world_modals` (`src/systems/screens/RealWorldModals.h`)
1. Move `"Preview only: type DELETE to confirm this sample"` into clean in-world copy (`"Type DELETE to permanently remove 'project-alpha'."`) so the destructive gate feels authentic.
2. In the Command Palette modal, highlight matched substrings, show keyboard shortcut pills (`⌘K`, `↑↓`, `↵`), and make `"No matching commands"` include a `"Clear filter"` link.
3. In the Terms of Service / long-content modal, enable `"Accept"` only after scrolling to the bottom (with a live `"Scroll to bottom to accept"` hint).
4. Give the Drawer and Bottom Sheet modals distinct slide-in edge anchoring with subtle elevation shadows and drag/close handles.
5. Log every modal outcome (`"Deleted"`, `"Ran: Toggle Dark Mode"`, `"Accepted TOS"`) in a clean activity feed on the main screen.

### 80. `scroll_view` (`src/systems/screens/ScrollViewShowcase.h`)
1. Show live scroll telemetry (`y = 142 / 680 px · 21%`) in a subtle header badge so scrolling mechanics are observable.
2. Add `"Scroll to Top"`, `"Scroll to Middle"`, and `"Scroll to Bottom"` programmatic jump buttons to showcase scroll API control.
3. Include both a vertical list and a horizontal card strip so both scroll axes are demonstrated clearly.
4. Give rows alternating subtle surface striping and crisp hover states so scroll velocity is visually legible.
5. Ensure the scrollbar thumb has rounded caps and subtle hover thickening.

### 81. `setting_row_showcase` (`src/systems/screens/SettingRowShowcase.h`)
1. Present all `setting_row` variants (toggle, slider, dropdown, stepper, action button) inside one cohesive macOS/Linear-style settings card with `1px` inner hairlines.
2. Align every right-hand control to a consistent right column width so toggles, sliders, and value pills form a crisp vertical edge.
3. Add a subtle 1-line helper description beneath complex rows, aligned cleanly under the row title.
4. Show a live `"Dirty / Reset to defaults"` bar at the bottom that appears only after a setting changes.
5. Include a disabled setting row with muted typography and a brief reason (`"Requires Pro plan"` or `"Managed by policy"`).

### 82. `stepper_showcase` (`src/systems/screens/StepperShowcase.h`)
1. Disable the `−` button at min bound and `+` button at max bound with clear visual dimming and a subtle `"Min"` / `"Max"` bound hint.
2. Use crisp tabular monospace numerals in the center readout so single- to triple-digit transitions (`9 → 10 → 100`) never shift the buttons.
3. Support press-and-hold auto-repeat (or `Shift+Click` for `±10` steps) and mention it in a subtle caption.
4. Reduce the 6 raw `Color{}` accents on the card selector to one cohesive selection border + subtle surface tint.
5. Place `"Restore defaults"` in the top-right header with a quiet feedback readout showing how many steppers differ from default.

### 83. `tab_container` (`src/systems/screens/TabContainerShowcase.h`)
1. Remove the duplicate `"Read-only example / No pending changes"` and `"Example profile / read-only fixture data"` disclaimers — one quiet caption is plenty (or make the Profile fields actually editable!).
2. Size the `"Premium"` badge to hug its text (`fit_content`) instead of stretching wide with trailing empty blue space.
3. Give the `"Copy email"` white button a subtle `1px` border on the light gray card so its clickable bounds read clearly, and flash `"Copied!"` on click.
4. Make `"Long labels"` tab visibly demonstrate truncation/scrolling behavior with a crisp tooltip showing the full tab title.
5. Style the closed `"Reopen example"` state with a friendly centered card explaining that your last active tab (`Audio`, `Display`, etc.) was preserved.

### 84. `text_input` (`src/systems/screens/TextInputDemo.h`)
1. Add a password visibility toggle (`Show` / `Hide`) on the password field and a live strength/character-count indicator.
2. Highlight focused fields with a clean Midnight accent ring and show clear visual differentiation between `Read-only` (copyable, subtle fill) and `Disabled` (muted, non-interactive).
3. Wire `"Save demo"` and `"Cancel"` to real dirty-state tracking with an inline `"Saved!"` confirmation toast.
4. Add a `"Clear (×)"` button inside the search input that appears only when text is non-empty.
5. Align all field labels, inputs, and helper text on a strict vertical rhythm (`8px` label-to-input, `16px` between fields).

### 85. `text_input_sizes` (`src/systems/screens/TextInputSizes.h`)
1. Annotate each of the 7 input heights (`24px` → `100px`) with a clean left-aligned monospace column showing `height`, `font_size`, and `caret_height`.
2. Add a `"Sync text across all inputs"` toggle so typing in one field mirrors into all 7 sizes simultaneously for instant cursor/vertical-centering comparison.
3. Add a subtle horizontal centerline guide toggle to visually verify exact vertical centering of text and caret at every height.
4. Give each input realistic placeholder text matching its scale (`Compact tag`, `Standard field`, `Hero title`).
5. Keep outer card padding generous so the `100px` input never crowds the bottom edge.

### 86. `toasts` (`src/systems/screens/ToastShowcase.h`)
1. Group toast triggers by semantic type (`Info`, `Success`, `Warning`, `Error`, `Action/Undo`) with matching subtle icon badges and restrained left-border accents.
2. Show a thin animated countdown progress bar along the bottom edge of each toast that pauses on hover (`Software You Can Love`: never vanish a message the user is reading!).
3. Add a corner-placement selector (`Top-Right`, `Bottom-Right`, `Bottom-Center`, `Top-Center`) so viewers can test stacking direction live.
4. Wire an interactive `"Undo"` button inside an action toast that visibly reverts a sample counter or deleted item.
5. Add a `"Dismiss all"` button and a subtle `"Active toasts: N"` readout in the header.

### 87. `toggle_switches` (`src/systems/screens/ToggleSwitchShowcase.h`)
1. Use quiet dark surface rows instead of saturated row fills so the pill switch thumb and track carry the visual signal.
2. Pair every pill toggle with clear `ON` / `OFF` (or subtle colored track) contrast and crisp disabled dimming.
3. Compare Pill Toggle vs Round Checkbox side by side with a 1-line UX guidance caption (`"Use toggles for instant effects; checkboxes for forms"`).
4. Add keyboard `Space`/`Enter` focus rings and a smooth thumb-slide animation check.
5. Anchor `"Reset demo defaults"` neatly in the top-right header with a subtle count of modified switches.

### 88. `transparent_controls` (`src/systems/screens/TransparentControlsLab.h`)
1. Present the 3 backgrounds (`Light`, `Dark`, `Patterned`) as equal-width vertical columns with identical control stacks so contrast legibility compares straight across.
2. Add a live background-alpha / frosted-tint slider (`0% → 40%`) so users can see the exact point where transparent controls gain comfortable contrast.
3. Show live hover, active, focus, and disabled states clearly on all 3 surfaces (especially ensuring focus rings stay visible on patterned backgrounds).
4. Display a small WCAG contrast badge (`4.8:1 AA`) computed for the foreground text against each surface.
5. Tidy `"Add to list"` and `"Reset count"` into a clean control bar with tabular count readout.

### 89. `vstack_showcase` (`src/systems/screens/VStackShowcase.h`)
1. Add interactive Gap (`0 / 8 / 16px`) and Cross-Axis Alignment (`Start / Center / End / Stretch`) pills so `vstack()` reflows live.
2. Show `"Content width"` vs `"Full width"` side by side with a subtle dashed container outline so width-hugging vs stretching is unmistakable.
3. Include one realistic `vstack()` card (notification item or form group) alongside the abstract stack specimens.
4. Show a live spacing-tick overlay toggle (`8px` gap markers) between stacked items.
5. Mirror the visual structure of `hstack_showcase` so the pair feels like one unified layout guide.

### 90. `widget_gap_gallery` (`src/systems/screens/WidgetGapGallery.h`)
1. Organize the 5 widgets (`button_group`, `pagination`, `spacer`, `image`, `icon_row`) into clean titled cards with a 1-line API snippet under each heading.
2. Visualize `spacer` with a subtle hatched/dotted flex-fill band (`flex: 1`) between `"head"` and `"tail"` so invisible spacing becomes visible on toggle.
3. Make `"Configure"` and every `button_group` / `icon_row` item visibly interactive with active state pills and a `"Last action"` strip.
4. Align `icon_row` icons and labels on a crisp horizontal baseline with consistent icon sizing (`16px` or `20px`).
5. Soften the outer borders and standardize padding across all 5 cards so the gallery feels cohesive, not piecemeal.

---

## System Demos (53 screens)

### 91. `absolute_positioning` (`src/systems/screens/AbsolutePositioningShowcase.h`)
1. Draw a subtle coordinate grid (`50px` / `100px` faint lines + axis ticks) inside the positioning stage so `(100,10)` and `(300,10)` are immediately verifiable by eye.
2. Replace the 18 competing hardcoded `Color{}` fills with a disciplined technical blueprint palette (slate surfaces + 1 cyan highlight for pinned corners `TL/TR/BL/BR`).
3. Add a draggable or nudgable sample box (`←↑↓→` ±10px) with live `(x, y)` coordinate readout so users can *feel* absolute vs relative placement.
4. Clearly label the 4 positioning modes (`Translate`, `Corner-Pinned`, `Mixed`, `Screen %`) as distinct titled panels or tabs.
5. Add a `"Show parent bounds"` toggle that outlines each reference container in a crisp dashed line.

### 92. `alpha_blend_repro` (`src/systems/screens/AlphaBlendRepro.h`)
1. Place `"Expected (reference)"` vs `"Actual (live render)"` side by side with an explicit alpha percentage label (`20%`, `50%`, `80%`) on each overlay strip.
2. Add a live opacity slider (`0%–100%`) so developers can sweep alpha continuously and spot premultiplied-alpha fringing immediately.
3. Put a subtle status badge (`Open gap` / `Verified`) and short 1-line diagnosis explaining what artifact to look for (e.g., dark halo around text).
4. Include a `"Light / Dark text"` toggle beneath the translucent overlays to test both foreground polarities.
5. Soften the specimen container borders so the blended edges themselves are cleanly inspectable.

### 93. `auto_text_color` (`src/systems/screens/AutoTextColorShowcase.h`)
1. Display the computed WCAG contrast ratio (`7.4:1 AAA`, `4.6:1 AA`) directly on every swatch alongside whether Auto chose Light, Dark, or Theme Candidate.
2. Add a live RGB/Hue scrub bar so users can watch the text color flip instantaneously at the luminance threshold.
3. Group **Auto-chosen**, **Theme Candidates**, and **Explicit Overrides** into 3 clear horizontal bands with aligned column headers.
4. Highlight the exact luminance crossover point (`L ≈ 0.179`) with a clean visual divider line across a gradient strip.
5. Add a `"Copy contrast report"` button so developers can audit theme pairs easily.

### 94. `blend_mode_lab` (`src/systems/screens/BlendModeLab.h`)
1. Render overlapping geometric color discs (or a crisp photo/gradient texture) inside each `blend::Mode` card so `Add`, `Multiply`, `Alpha`, etc. look unmistakable.
2. Pin the `blend_scope` transition counter in a crisp top-right monospace telemetry pill with a `"Reset counter"` button.
3. Add a `"Background polarity"` toggle (dark / mid-gray / light) since Additive vs Multiply behave dramatically differently per backdrop.
4. Replace raw `Color{}` box frames with quiet neutral surfaces so the blended shapes are the only saturated color on screen.
5. Label each card with its exact `blend::Mode::...` enum name and expected blend equation in muted monospace.

### 95. `chart_lab` (`src/systems/screens/ChartLab.h`)
1. Add interactive hover crosshair + tooltip showing exact timestamp and millisecond value for any hovered bar/point.
2. Draw clean horizontal reference rules (`16.6ms 60fps`, `8.3ms 120fps`) with right-aligned monospace labels so frame-budget spikes jump out immediately.
3. Add a Pause/Resume capture button and `"Copy CSV"` export (`Software You Can Love`: let outputs feed other tools).
4. Use one calm primary data color + semantic amber/red only for budget-exceeding spikes.
5. Format Y-axis labels right-aligned in tabular monospace with consistent tick intervals.

### 96. `composer_lab` (`src/systems/screens/ComposerLab.h`)
1. Style the message composer like a crisp chat input bar (Slack/Linear feel) with a live line-count & pixel-height pill (`3 lines · 76px / max 140px`) beside it.
2. Show an unmistakable keycap hint below the composer (`Enter to send · Shift+Enter for newline`) that highlights whichever key you just pressed.
3. Append submitted messages into a real mini transcript above the composer so pressing `Enter` feels immediately rewarding.
4. Put the `"Wrap"` vs `"No-wrap clipping"` specimens side by side with a subtle right-edge clip ruler so horizontal overflow is obvious.
5. Add a `"Load long paragraph"` quick-fill button to test auto-grow and scroll handoff in one click.

### 97. `context_menu_lab` (`src/systems/screens/ContextMenuLab.h`)
1. Draw faint corner/edge flip-zone guides (`"Right-click near edge to test flip"`) as a toggleable overlay so users know where to click to trigger edge flipping.
2. Add keyboard shortcut hints (`⌘C`, `⌘V`, `⌫`) right-aligned inside menu rows, plus crisp hairline dividers between menu groups.
3. Give the open context menu soft shadow lift, crisp `1px` subtle border, and clear hover highlight + disabled row dimming.
4. Show a live `"Last menu event: Copied @ (1180, 640) [flipped left+up]"` telemetry strip in the footer.
5. Support keyboard `↑↓` / `Enter` / `Esc` inside the open context menu and mention it in a quiet header caption.

### 98. `divider_lab` (`src/systems/screens/DividerShowcase.h`)
1. Give draggable dividers a subtle centered grip pill that brightens on hover and turns accent-colored while dragging so affordance is unmistakable.
2. Display live pane split percentages (`38% | 62%`) in tabular monospace while dragging.
3. Clearly separate **Draggable `divider()`** from **Static rule `divider()`** into two distinct titled cards so users immediately grasp both modes.
4. Show min/max clamp indicators (subtle flash or `"Min 120px"` pill) when dragging into a pane's minimum bound.
5. Keep `"Reset dividers"` in the top-right header and show a subtle double-click-to-reset hint on the handle.

### 99. `entity_index_lab` (`src/systems/screens/EntityIndexLab.h`)
1. Visualize the parent → children hierarchy as a clean indented node tree with live entity ID badges (`#42`) and edge connectors.
2. Add interactive `"Add child"`, `"Reparent"`, and `"Delete node"` buttons so users can watch the secondary index update in real time.
3. Show a live lookup-timing & consistency badge (`Index hits: 14 · 0 stale · 0.02ms`) in crisp monospace.
4. Tone down the 8 hardcoded `Color{}` node fills to a quiet 2-tone parent/child palette + 1 accent for the currently queried node.
5. Add a `"Copy index dump"` button for quick debugging.

### 100. `accessibility` (`src/systems/screens/ExampleAccessibility.h`)
1. Pair every `"Manual foreground colors"` vs `"Automatic foreground colors"` card side by side with live WCAG contrast pills (`2.9:1 Fail` vs `7.2:1 AAA`).
2. Add interactive toggles for High Contrast mode, Reduced Motion, and Focus Ring visibility so users experience every Garnish accessibility feature live.
3. Replace the 7 raw `Color{}` accents with clearly named Slate demo tokens and show the resolved hex values in monospace.
4. Add a keyboard-only navigation check strip (`"Try navigating this card with Tab + Space"`) with a live focus-target readout.
5. Organize the screen into 3 balanced columns (Contrast, Focus & Motion, Typography/Scale) instead of a long vertical wall.

### 101. `color_swatch_lab` (`src/systems/screens/ExampleColorSwatch.h`)
1. Add a one-click `"Copy Hex / RGB"` button next to every swatch picker that flashes `"Copied!"` on click.
2. Show the live selected color applied to a realistic mini UI badge/button preview so users see their picked color in context.
3. Add a subtle checkerboard behind swatches when alpha `< 255` so translucency is clearly visible.
4. Give popover color editors soft shadow elevation, crisp saturation/lightness cursor rings, and `Esc` to cancel/revert.
5. Align Inline, Presets, and Popover variants in clean titled cards with symmetrical padding.

### 102. `colors` (`src/systems/screens/ExampleColors.h`)
1. Display each theme swatch with its semantic role name, hex code, luminance, and click-to-copy feedback (`"Copied #194691"`).
2. Add a clean contrast matrix mini-grid showing which foreground/background pairs pass AA/AAA.
3. Remove arbitrary raw `Color{}` accents outside the swatch specimens so the Midnight canvas stays quiet and lets the palette shine.
4. Show a tint/shade ramp (`-20%`, `Base`, `+20%`) for Primary and Accent to demonstrate color derivation.
5. Align all swatch cards on a strict grid with consistent `6px` radius and subtle `1px` inner border so dark swatches don't vanish into the Midnight background.

### 103. `flex_alignment` (`src/systems/screens/ExampleFlexAlignment.h`)
1. Add interactive item-count (`2 / 3 / 5`) and container-direction (`Row / Column`) toggles inside each tab so users can stress-test `justify` and `align` live.
2. Draw subtle dashed main-axis and cross-axis arrows (`→ Main`, `↓ Cross`) inside each specimen frame so flexbox terminology is self-teaching.
3. Keep tab bar styling crisp with a clear active underline and smooth keyboard switching.
4. Tone down the 4 raw `Color{}` child blocks into harmonious numbered chips (`1`, `2`, `3`) with subtle width/height labels.
5. Show the exact C++ config call (`with_justify_content(JustifyContent::SpaceBetween)`) in quiet monospace beneath each active specimen.

### 104. `layout` (`src/systems/screens/ExampleLayout.h`)
1. Replace the solid gray margin tabs glued to `Item 1` with crisp architectural dimension lines + `28px` tick labels.
2. Tint the pure-white (`#FFFFFF`) cards to warm sage-white (`#F7FAF6`) so they belong to the Sage Natural color world instead of jumping starkly off the canvas.
3. Tighten the `"60% of the parent content"` and `"3 px gap"` callouts so they sit snugly beside the stack with clean leader lines instead of floating in empty space.
4. Add an interactive toggle for gap size (`4px / 12px / 28px`) or item count (`3 / 5`) so users can watch the layout engine respond.
5. Condense the two competing footer text lines into one crisp monospace status strip (`Sage Natural · 1280×720 · Row/Col 1136px`).

### 105. `tabbing` (`src/systems/screens/ExampleTabbing.h`)
1. Display a live `"Focus trail"` breadcrumb (`Button 1 → Button 2 → Button 3`) and prominent `[Tab]` / `[Shift+Tab]` / `[Enter]` keycaps that light up as pressed.
2. Add a `"Tab index"` badge (`#1`, `#2`, `#3`, `Skip`) on each control so focus order is visually verifiable before pressing a key.
3. Include one disabled control and one non-focusable label in the sequence to prove tabbing skips them cleanly.
4. Make the active focus ring high-contrast and unmistakable with a subtle outer glow/offset.
5. Group the controls in a clean centered card with `"Reset count"` aligned quietly in the header.

### 106. `text` (`src/systems/screens/ExampleText.h`)
1. Format the type-size ladder as a real typographic specimen sheet (size `12px–48px`, line-height, role name `Caption / Body / H3 / H2 / H1`, and sample phrase).
2. Add a `"Show baseline & bounding box"` toggle so developers can inspect ascenders, descenders, and cap-height padding directly.
3. Add a Light/Dark surface toggle so text legibility and hinting can be inspected on both polarities.
4. Replace generic `"Hello World"` / placeholder lines with realistic UI copy (headings, numbers, punctuation, wrapped body).
5. Align size labels in a fixed-width left monospace gutter so all specimen baselines line up cleanly.

### 107. `text_shadow` (`src/systems/screens/ExampleTextShadow.h`)
1. Soften the jarring jump between the bright ice-blue canvas and the dark charcoal specimen cards (use either a cohesive dark canvas or subtle elevated cards matching the page world).
2. Replace the vibrating cyan-on-red (`#50FFFF + #C80000`) `S5` specimen with a tasteful colored-glow pair (e.g., warm amber with deep rust shadow) that still demonstrates colored shadows cleanly.
3. Add live interactive **Offset X/Y** and **Shadow Alpha** sliders on a sandbox specimen at the bottom so users can dial in values live.
4. Format the `Offset ladder` on the right with single-line aligned labels (`0.0 px`, `1.0 px`, …) instead of wrapping `x / y` onto a second line.
5. Condense the two dense footer lines into a crisp monospace spec bar with a `"Copy shadow config"` button.

### 108. `text_stroke` (`src/systems/screens/ExampleTextStroke.h`)
1. Match the canvas and specimen card color world (avoid stark dark boxes on bright light backgrounds unless explicitly testing Light vs Dark contrast).
2. Add a live **Stroke Width** (`0.5px–6px`) slider and **Stroke Color** preset pills so users can inspect outline crispness continuously.
3. Show a `2×` or `4×` zoomed detail comparison of `1px`, `2px`, and `4px` stroke joins (miter/round corners on `A`, `M`, `W`).
4. Include a realistic game-HUD badge specimen (`+250 XP` or `LEVEL UP`) showing stroked text over a busy patterned backdrop.
5. Add a `"Box guides"` toggle (matching `text_shadow`) to prove stroke doesn't distort layout bounds.

### 109. `fit_content_lab` (`src/systems/screens/FitContentLab.h`)
1. Add an editable input box or `"Cycle message length"` button (`Short` / `Medium` / `Long` / `Emoji`) so users watch the chat bubbles hug their text dynamically.
2. Toggle subtle bounding-box guides with pixel dimensions (`148 × 36 px`) stamped on each bubble to prove exact content hugging.
3. Style the chat bubbles like a polished modern messenger (asymmetric corner radii, subtle sender vs receiver tint, timestamps) instead of flat colored rectangles.
4. Show a side-by-side comparison of `fit_content()` vs fixed-width `percent(1.0)` so the difference is unmistakable.
5. Keep max-width wrapping clearly marked with a faint vertical dashed line (`max: 360px`).

### 110. `focus_ring_lab` (`src/systems/screens/FocusRingLab.h`)
1. Display a live `"Input modality: Mouse / Keyboard"` badge that flips immediately when you click vs press `Tab`, proving `:focus-visible` behavior.
2. Add interactive sliders/pills for **Ring Offset** (`0px / 2px / 4px`) and **Ring Width** (`1px / 2px / 3px`) applied live to the `"Save"` specimen.
3. Arrange all highlight modes (`Outline`, `Glow`, `Inset`, `High-Contrast`) in an aligned 2×2 grid with consistent card padding.
4. Add a `"Focus next (Tab)"` helper button and subtle keycap hint so mouse-only reviewers can also trigger keyboard-focus mode.
5. Show the exact `FocusRingStyle` snippet beneath the active mode.

### 111. `grid_lab` (`src/systems/screens/GridLab.h`)
1. Add clean vertical breathing room between the bottom track ruler (`60 | 240 | 120 | 120`) and `"Columns stay aligned..."`, or fold the track ruler inside the Example 2 card.
2. Label the `"8x requested-border diagram"` with explicit pixel math (`16px preview = 8× 2px border`) so the empty thick-bordered box is immediately self-explanatory.
3. Add a `"Gap: 0 / 4 / 12 px"` toggle alongside `"Swap text lengths"` to demonstrate grid track gutters live.
4. Highlight the spanning cell (`one cell, spanning three tracks`) with a subtle accent tint so column spanning pops at a glance.
5. Give the striped body rows hover highlighting across the full row to show table-like grid interactivity.

### 112. `host_loop_lab` (`src/systems/screens/HostLoopLab.h`)
1. Visualize frame-wake events as a live scrolling sparkline / pulse ticker (`Idle` flatline vs `Wake` spikes) so demand-driven rendering is unmistakable.
2. Add target-FPS preset pills (`30 / 60 / 120 / Uncapped`) and a `"Request single frame"` pulse button with a visible frame-counter flash.
3. Display live CPU-sleep vs active-render percentage in crisp tabular monospace.
4. Show a clear green `"Sleeping (0 FPS)"` vs cyan `"Awake (60 FPS)"` status pill in the header.
5. Group controls into a clean 2-column dashboard (Controls on left, Live Loop Telemetry on right).

### 113. `hover_lab` (`src/systems/screens/HoverLab.h`)
1. Show a live hover-stack breadcrumb (`Hovered: Outer Card → Inner Badge (pointer-events: auto)`) updating on every mouse move.
2. Visually distinguish `pointer-events: none` overlays with a subtle diagonal hatch or `"Pass-through"` pill so users immediately see *why* clicks fall through.
3. Reduce the 11 raw `Color{}` fills to a calm neutral hierarchy where ONLY the actively hovered subtree lights up in accent cyan.
4. Show live click counts inside crisp corner pills on both parent and child targets, with `"Reset click counts"` tucked in the top-right header.
5. Add a `"Toggle child pointer-events"` switch so users can flip pass-through on and off on the exact same box.

### 114. `language_demo` (`src/systems/screens/LanguageDemo.h`)
1. Group language buttons (EN, JP, KR, ZH, AR, etc.) into a crisp pill selector with native script labels + ISO codes (`日本語 · JA`) and a clear `"Active"` accent bar.
2. Show a realistic mini UI card (dialog title, button, notification, date/number) rendered in the selected language alongside the font-coverage specimen.
3. Display the active font fallback chain (`Inter → Noto Sans JP → Emoji`) and missing-glyph count (`0 missing`) in a subtle monospace footer.
4. Add a `"Toggle RTL preview"` pill for Arabic/Hebrew specimens so layout mirroring is inspectable too.
5. Standardize card spacing and soft borders on Ocean Navy so every script has generous line-height (`1.5×`) for tall diacritics and CJK characters.

### 115. `localized_input_prompts` (`src/systems/screens/LocalizedInputPrompts.h`)
1. Highlight the inline keycap badges (`[E]`, `[Space]`, `[Shift]`) with crisp beveled styling so the translated word-order swap (`"Press [E] to open"` vs `"[E] 키를 눌러 열기"`) jumps out visually.
2. Add a live prompt-device switcher (`Keyboard / Xbox / PlayStation`) so users see inline prompt tokens swap cleanly across input glyphs.
3. Place EN, KO, JA, and DE translations in aligned rows with subtle grammar-order annotations (`Verb · Key · Object` vs `Key · Object · Verb`).
4. Add a `"Test press [E]"` interactive highlight so pressing the actual key flashes the inline prompt across all languages!
5. Give each language row a quiet surface card with symmetrical padding and generous vertical rhythm.

### 116. `menu_showcase` (`src/systems/screens/MenuShowcase.h`)
1. Style anchored menus with soft elevation shadows, crisp `1px` subtle borders, and right-aligned keyboard shortcut hints (`⌘N`, `⌘S`).
2. Add corner-anchor buttons (`TL`, `TR`, `BL`, `BR`) with a subtle `"Flipped: ↑"` telemetry badge when edge-flipping triggers.
3. Show checkmark states for toggleable menu items (e.g., `✓ Show Grid`) and right chevrons (`›`) for submenus.
4. Dim disabled menu entries gracefully without losing legibility and skip them during `↑↓` keyboard navigation.
5. Log the last invoked menu command in a quiet bottom status pill so every menu item feels functional.

### 117. `multiline_text_lab` (`src/systems/screens/MultiLineTextLab.h`)
1. Add a draggable or stepped container-width slider (`180px → 520px`) so users watch line wrapping and self-sizing height update in real time.
2. Show a live `"Layout lines: 4 · Render lines: 4 [MATCH]"` green verification pill (turning red on any mismatch).
3. Add a `"Show newline markers (↵)"` toggle to make hard breaks vs soft wraps immediately distinguishable.
4. Replace the 5 raw `Color{}` fills with quiet Neon Dark cards and a crisp vertical dashed right-wrap guide.
5. Include a test button for edge-case strings (`Long unbreakable URL`, `Double newlines`, `CJK wrap`).

### 118. `pinch_zoom_lab` (`src/systems/screens/PinchZoomLab.h`)
1. Add `+` / `−` buttons and scroll-wheel fallback (`Ctrl+Wheel`) alongside trackpad pinch so desktop mouse users can also test zoom smoothly.
2. Display a crisp zoom HUD pill (`1.00×` · focal point `(640, 360)`) with `"Reset to 1.00×"` right next to it.
3. Draw a subtle crosshair at the active zoom focal point while pinching so focal-point stability is visually verifiable.
4. Put a rich, crisp vector/grid canvas (map tiles or schematic diagram) inside the zoom viewport instead of 10 raw colored boxes so magnification feels rewarding.
5. Add min/max zoom rail indicator (`0.25×` — `4.00×`) showing current zoom position at a glance.

### 119. `pointer_parity_lab` (`src/systems/screens/PointerParityLab.h`)
1. Draw a clean minimalist top-down mouse diagram whose Left, Middle, Right, Back, Forward buttons and Scroll Wheel light up live as you click/scroll.
2. Render horizontal and vertical wheel deltas (`dx`, `dy`) as live spring-back directional bars plus a scrolling event log of the last 8 events.
3. Display raw cursor coordinates `(x, y)` and modifier keys (`Shift`, `Ctrl`, `Alt`, `Cmd`) as crisp live keycaps in tabular monospace.
4. Add a `"Clear log"` button and `"Copy event log"` button for bug reports (`Software You Can Love`).
5. Give the test pad area a generous subtle surface with clear borders so users know exactly where to scroll and click.

### 120. `popover_lab` (`src/systems/screens/PopoverLab.h`)
1. Give popovers a soft drop shadow, subtle `1px` border, and a small directional arrow/caret pointing cleanly back to the anchor button.
2. Add a live `"Anchor / Placement"` status badge (`Anchored bottom-left → flipped top-left`) whenever edge-flip occurs.
3. Make the controls inside `"Commit options"`, `"Commit preferences"`, and `"Switch branch"` update a real summary badge on the main canvas so popover state feels alive.
4. Replace the 8 hardcoded `Color{}` accents with a clean neutral surface hierarchy + one primary accent.
5. Support `Esc` and outside-click dismissal with a visible `"Dismissed via: Esc"` readout.

### 121. `pseudo_locale_lab` (`src/systems/screens/PseudoLocaleLab.h`)
1. Add a prominent top-bar mode switcher (`English`, `Accented [Ṕśéúḍö]`, `Double-Length 2×`, `RTL Mirror`) that transforms the entire `friendface` mockup instantaneously.
2. Highlight any overflowing or clipped text containers automatically with a toggleable `"Show overflow warnings"` red outline overlay!
3. Soften the 16 raw `Color{}` fills in `friendface` into a crisp, believable social-feed palette (clean header bar, subtle card shadows, balanced avatar badges).
4. Replace the space-padded `"Live video      Photo or video      Feeling or activity"` single label with 3 real evenly-spaced flex buttons so they reflow properly under 2× pseudo-locale expansion.
5. Show a live `"Expansion: +100% · Overflowing nodes: 0"` audit counter in the top-right developer strip.

### 122. `rect_algebra_lab` (`src/systems/screens/RectAlgebraLab.h`)
1. Add interactive sliders for `cut` amount, `pad` inset, and `expand` margin so users watch the rectangle algebra transform live.
2. Label each resulting sub-rect directly inside its region (`top(48)`, `left(120)`, `remainder`) with crisp pixel dimensions (`420 × 180`).
3. Use a disciplined translucent blueprint color palette (instead of 8 harsh raw colors) so overlapping `pad`/`expand` regions remain legible.
4. Show the exact 2-line C++ `rect::cut / pad / expand / align` call beneath each diagram with a `"Copy snippet"` button.
5. Add a `"Show original rect"` dashed ghost outline toggle on every specimen.

### 123. `rolling_number` (`src/systems/screens/RollingNumberLab.h`)
1. Add quick interactive delta buttons (`+1`, `+10`, `+1,234`, `−500`, `Random`) plus a formatted currency/score/timer showcase (`$12,450.00`, `04:20`) so viewers can trigger dramatic multi-digit rolls on demand.
2. Use strict tabular monospace numerals with a crisp vertical clip mask and subtle top/bottom fade gradient so rolling drums look like a mechanical split-flap / odometer.
3. Add a side-by-side `"Motion blur: ON"` vs `"Reduced motion"` comparison toggle.
4. Add a `"Roll duration"` pill selector (`150ms` / `350ms` / `800ms`) so users can feel snappy vs dramatic pacing.
5. Center the hero rolling counter in an elevated Ocean Navy showcase card with quiet metadata below.

### 124. `scrollbar_style_lab` (`src/systems/screens/ScrollbarStyleLab.h`)
1. Put all 4 styled scroll panes in a symmetrical 2×2 grid with a crisp header badge on each showing `track`, `thumb`, and `thickness` (`6px`, `10px`, `14px`, `Pill`).
2. Add a `"Sync scroll all 4 panes"` toggle so scrolling one pane lets you compare all 4 scrollbar styles in motion simultaneously.
3. Populate each pane with clean structured list rows (not bare numbered lines) so scrollbar contrast against real content is obvious.
4. Show subtle hover/active thumb state shifts on each pane and display the resolved colors in muted monospace below.
5. Add a `"Reset scroll"` button in the top-right header.

### 125. `self_align` (`src/systems/screens/SelfAlignShowcase.h`)
1. Draw faint horizontal/vertical alignment guide lines inside each Sage Natural container so `Start`, `Center`, `End`, and `Auto / inherited` are visually verifiable at a glance.
2. Add an interactive `"Parent align"` toggle (`Start / Center / End`) so `"Auto / inherited"` visibly follows the parent while explicit `SelfAlign` children stay put!
3. Tint card surfaces warm sage-white instead of stark pure white so they harmonize with the Sage Natural backdrop.
4. Label each box concisely with its enum (`SelfAlign::Center`) in quiet monospace.
5. Add a `"Row / Column"` axis switch to prove `SelfAlign` works identically on both cross-axes.

### 126. `small_components_lab` (`src/systems/screens/SmallComponentsLab.h`)
1. Arrange `settings_list`, `value_pill`, and `divider_line` inside a cohesive two-pane inspector (`Left pane` settings list, `Right pane` live value-pill readout) so the components work together.
2. Add semantic `value_pill` variants (`Neutral`, `Success`, `Warning`, `Accent`) that update live as you drag the sliders or flip the toggle.
3. Show subtle hover highlights on `settings_list` rows and align all right-hand controls cleanly.
4. Keep `divider_line` whisper-quiet (`1px` subtle alpha) with symmetrical vertical spacing above and below.
5. Add a `"Reset"` button and concise API caption under each pane.

### 127. `split_pane_lab` (`src/systems/screens/SplitPaneLab.h`)
1. Give the draggable splitter a clear centered grip indicator and hover/drag highlight so users immediately spot where to grab.
2. Show live pixel widths and split ratios (`320px · 32%` | `680px · 68%`) in quiet monospace inside each pane header.
3. Include a nested vertical-inside-horizontal split (IDE layout: Sidebar | Editor / Terminal) with realistic mini-content so the usefulness of `split_pane` shines.
4. Add `"Collapse left"`, `"50 / 50"`, and `"Reset"` quick-action buttons in the top bar.
5. Reduce the 6 raw `Color{}` pane backgrounds to subtle layered slate surfaces (`surface-1`, `surface-2`, `surface-3`).

### 128. `stale_index_lab` (`src/systems/screens/StaleIndexLab.h`)
1. Display the current `selected_index` and `options.size()` in a bold live pill (`idx: 4 / len: 5` → clamps to `idx: 2 / len: 3`) with a brief amber flash whenever clamping occurs.
2. Add clear `"Shrink list (−2)"`, `"Clear list (0)"`, `"Grow list (+3)"`, and `"Select last"` action buttons in one tidy toolbar.
3. Align all 4 affected widgets (`dropdown`, `navigation_bar`, `radio_group`, `tab_container`) in a clean 2×2 grid so you watch all four clamp safely in unison.
4. Show a green `"In bounds"` badge next to every widget to prove zero out-of-range states.
5. Add an event log strip (`"Shrank 5 → 3 · Clamped idx 4 → 2"`) so state transitions are transparent.

### 129. `styled_text_lab` (`src/systems/screens/StyledTextLab.h`)
1. Showcase realistic inline styled-text patterns (a commit log line, a pricing callout, a keyboard shortcut sentence, a warning notice) alongside raw weight-span specimens.
2. Add a `"Show span boundaries"` toggle that draws subtle dotted underlines/pills around each individual text span so per-span runs are inspectable.
3. Replace the 5 raw `Color{}` highlights with cohesive semantic theme accents (code, emphasis, warning, link).
4. Add a font-size stepper (`14px / 18px / 24px`) to verify baseline alignment across mixed-weight spans at multiple sizes.
5. Display the compact span-definition syntax beneath one hero example so developers can copy the idiom.

### 130. `sync_scroll_lab` (`src/systems/screens/SyncScrollLab.h`)
1. Give the `"Horizontal header/table group"` sticky-looking crisp column borders and alternating row striping so synchronized horizontal scrolling feels like a real spreadsheet/IDE diff view.
2. Display a live `sync_group` offset telemetry pill (`scrollX: 140px · scrollY: 320px · Synced ✓`) updating in real time.
3. Add a `"Toggle sync_group ON/OFF"` switch so users can deliberately unsync the panes and feel the difference immediately.
4. Tone down the 10 hardcoded `Color{}` accents to a clean dark-table surface hierarchy with one cyan sync-link indicator between paired panes.
5. Add `"Jump to End"` and `"Reset Scroll"` buttons to test programmatic sync across all linked views.

### 131. `system_profile_lab` (`src/systems/screens/SystemProfileLab.h`)
1. Render per-system frame budgets as sorted horizontal proportional bars (`Layout 1.2ms`, `Render 2.8ms`, `Input 0.3ms`) so the slowest system is obvious in <1 second.
2. Color-code frame-time totals semantically (green `<8ms`, amber `8–16ms`, coral `>16ms`) with tabular monospace alignment.
3. Add `"Pause snapshot"`, `"Sort by avg / max"`, and `"Copy profile report"` buttons (`Software You Can Love`).
4. Show a rolling 60-frame sparkline at the top of the panel so spikes don't disappear before you can read them.
5. Soften table borders and give rows subtle hover highlighting with min/avg/p95/max breakdown.

### 132. `text_area_lab` (`src/systems/screens/TextAreaLab.h`)
1. Organize the 4 parity features (`Placeholder`, `Backgrounds`, `Focus Ring`, `Auto-Grow`) into a crisp 2×2 grid with explicit 1-line captions and live line/char counters.
2. Add a `"Clear all"` / `"Fill sample text"` toolbar button so reviewers can see placeholder states and multi-line auto-grow in one click.
3. Ensure the focus ring clearly wraps the entire `text_area` frame with consistent radius and offset matching `text_input`.
4. Display current height (`88px / 4 lines`) live inside the Auto-Grow card header.
5. Keep consistent internal text padding (`10px 12px`) across all 4 `text_area` specimens.

### 133. `text_area_states_lab` (`src/systems/screens/TextAreaStatesLab.h`)
1. Present `Editable`, `Read-only`, and `Disabled` as 3 equal-height side-by-side columns with matching sample text so visual state differences compare directly.
2. Add a subtle lock icon or `"Selectable / Copyable"` badge on `Read-only` to clarify how it differs from `Disabled` (`"Non-interactive"`).
3. Ensure `Disabled` text maintains comfortable muted readability (not invisible) and visibly rejects focus/caret on click.
4. Show a live `"Last interaction: Blocked (readonly)"` / `"Edited (42 chars)"` status line at the bottom.
5. Replace the 3 raw `Color{}` state fills with semantic theme tokens.

### 134. `text_cache_lab` (`src/systems/screens/TextCacheLab.h`)
1. Display live cache hit/miss counters and hit-rate percentage (`99.4% hit · 128 entries`) in a crisp monospace header strip.
2. Add `"Invalidate cache"` and `"Resize width"` trigger buttons so developers can watch cache misses spike once and immediately settle back to 100% hits.
3. Separate the 4 diagnostic probes (`wrap memo key`, `styled-run columns`, `glyph coverage`, `label inset`) into 4 cleanly labeled cards.
4. Visualize `label inset` with subtle shaded padding bands around the text box so the inset delta is obvious.
5. Add a `"Copy cache stats"` button for perf regression notes.

### 135. `text_input_lab` (`src/systems/screens/TextInputLab.h`)
1. Format the live state readout (`caret`, `selection [start..end]`, `scroll_x`, `byte_len`, `grapheme_len`) as a clean structured key-value HUD card in tabular monospace.
2. Highlight the active selection range and scroll viewport visually on a miniature schematic bar beneath the input so internal scrolling is self-explanatory.
3. Add quick fixture buttons alongside `"Primary: load café"` (e.g., `"Load emoji 👨‍👩‍👧‍👦"`, `"Load 80-char overflow"`, `"Select middle"`).
4. Keep `"Reset fixture"` cleanly placed in the header toolbar.
5. Ensure the primary input has crisp focus-ring contrast and generous padding on Neon Dark.

### 136. `theme_builder` (`src/systems/screens/ThemeBuilderLab.h`)
1. Group the token table into collapsible semantic sections (`Surfaces`, `Text`, `Brand & Accent`, `Borders & Radius`) so the editor feels calm rather than overwhelming.
2. Show live WCAG contrast badges (`AA ✓` / `Fail !`) next to foreground/background token pairs right inside the token table.
3. Add a prominent `"Export C++ / JSON"` button with instant clipboard copy + `"Copied!"` toast (`Software You Can Love`: let users take their work out free).
4. Highlight `"Undo"` / `"Redo"` with step counts (`Undo (3)`) and keyboard shortcut tooltips (`⌘Z` / `⇧⌘Z`), dimming them cleanly when empty.
5. Make the live preview pane include a realistic mini card, form row, alert, and button bar so every token change proves itself in context.

### 137. `tooltip_lab` (`src/systems/screens/TooltipLab.h`)
1. Give tooltips crisp soft shadows, subtle borders, generous `6px 10px` padding, and a small directional arrow pointing to the hovered target.
2. Add a live `"Hover delay (0ms / 250ms / 500ms)"` pill selector so users can feel immediate vs delayed tooltip ergonomics.
3. Mark the 4 corner edge-flip test buttons cleanly (`TL`, `TR`, `BL`, `BR`) and show a live `"Placement: top → flipped bottom"` badge when a tooltip flips.
4. Include a tooltip on a disabled button or icon-only button to demonstrate accessible explanations.
5. Ensure tooltip text wraps gracefully at a clean max width (`240px`) without ever clipping off-screen.

### 138. `tray` (`src/systems/screens/TrayShowcase.h`)
1. Display a prominent keyboard legend (`[Tab] Enter/Exit Tray · [←/→] Move Inside · [Enter] Activate`) that lights up each key as pressed.
2. Draw a crisp outer focus halo around the active tray container while focused so `"single tab stop"` grouping is immediately legible.
3. Show a live `"Tab stops in this form: 3 (not 8)"` comparison badge explaining *why* trays matter for keyboard UX.
4. Give the selected child inside the tray a distinct active pill fill and subtle checkmark/dot.
5. Align `"Reset demo"` cleanly in the top-right header.

### 139. `typography_composition` (`src/systems/screens/TypographyCompositionLab.h`)
1. Give `"MAKE"` / `"WAVES"` a true editorial poster composition with generous negative space, crisp typographic scale contrast, and aligned baselines.
2. Add a preset switcher (`Editorial Poster`, `Game Title Card`, `Kinetic Quote`) so users see whole-word typography across 3 distinct moods.
3. Add a `"Motion: On / Reduced"` toggle next to `"Replay"` so static vs animated composition can be compared cleanly.
4. Show subtle typographic spec tags (`tracking`, `leading`, `weight`) in a toggleable `"Show grid"` overlay.
5. Frame the composition on an uncluttered Ocean Navy stage without heavy enclosing borders.

### 140. `validation_throttle_lab` (`src/systems/screens/ValidationThrottleLab.h`)
1. Show a bold side-by-side live counter: **`Frames elapsed: 420`** vs **`Warnings emitted: 1 (throttled)`** so the core guarantee proves itself at a glance.
2. Add a `"Trigger new warning"` button and `"Reset throttle"` button so developers can see a distinct warning fire once while duplicate warnings stay silenced.
3. Render the warning log in a clean monospace console box with timestamps, call-site key, and suppressed-repeat count (`×419 suppressed`).
4. Add a `"Copy log"` button for pasting straight into issue trackers.
5. Soften the outer container borders so the console and counters are the clear focal hierarchy.

### 141. `virtual_list_heights_lab` (`src/systems/screens/VirtualListHeightsLab.h`)
1. Show each row's variable height (`48px`, `96px`, `140px`) as a subtle right-aligned monospace pill so height diversity is unmistakable.
2. Make `"Prepend 10 items"` flash a brief `"Anchor preserved · scroll shifted +820px"` badge so the prepend-anchor guarantee is visibly celebrated.
3. Give `"scroll_to_bottom"` and `"Jump to retained index"` crisp toolbar buttons with a live `"Visible: #42–#54 · Pool: 18 entities"` telemetry strip.
4. Style variable-height rows like realistic chat/feed cards (1-line note, 3-line comment, code/quote block) instead of bare height bars.
5. Add subtle alternating surface elevation and crisp row dividers so scrolling feels buttery smooth.

### 142. `virtual_list_lab` (`src/systems/screens/VirtualListLab.h`)
1. Highlight the virtualization win in a clean header telemetry bar: **`10,000 logical rows · 22 pooled entities · 0.4ms`**.
2. Pair `"Jump"` with quick preset chips (`#1`, `#2,500`, `#5,000`, `#9,999`) and validate out-of-range jump inputs gracefully.
3. Add a `"Show overscan rows"` subtle badge/tint toggle so developers can see the exact overscan buffer above and below the viewport.
4. Give rows clean structured columns (Index `#0421`, Title, Category pill, Timestamp) in tabular monospace so fast scrolling feels like a real pro table.
5. Ensure the scrollbar thumb has a clean minimum height (`24px`) even across 10,000 rows so it's always easy to grab.

### 143. `wm_defaults_lab` (`src/systems/screens/WmDefaultsLab.h`)
1. Organize the 3 WM defaults (**Release clicks**, **Outset focus ring**, **Font-scaled text-area rows**) into 3 balanced comparison cards with `"Default"` vs `"Legacy / Opt-in"` side by side.
2. Make `"WM default (Release)"` vs `"Explicit Press opt-in"` flash a crisp `"Fired on mouse-UP!"` / `"Fired on mouse-DOWN!"` badge with timestamp so the feel difference is unmistakable.
3. Add a live `font_size` stepper on the text-area card so users watch 3 font-scaled rows grow cleanly without clipping.
4. Show a magnified focus-ring comparison (`Outset +2px` vs `Flush 0px`) with keyboard `Tab` hint.
5. Add a `"Reset defaults demo"` button in the top-right header.

---

## Tools (13 screens)

### 144. `adaptive_scaling` (`src/systems/screens/AdaptiveScalingDemo.h`)
1. Place **Adaptive** vs **Proportional** viewports side by side with a crisp resolution switcher (`720p`, `1080p`, `1440p`, `Ultrawide`, `4:3`) and `−` / `+` scale stepper in one unified top bar.
2. Display exact computed pixel sizes (`48px → 64px` vs `48px → 48px`) directly on the `"Equal squares"` specimens so scaling math is transparent.
3. Tone down the 16 raw `Color{}` fills into a clean Ocean Navy technical palette where ONLY the measured reference squares carry color.
4. Add a `"1:1 Physical ruler"` or aspect-ratio badge showing how padding and typography hold proportions at non-16:9 ratios.
5. Include a realistic mini-button + label pair inside both modes so users see how real controls scale, not just squares.

### 145. `command_picker` (`src/systems/screens/CommandPickerDemo.h`)
1. Highlight fuzzy-matched characters in bold/accent color inside every command title so search filtering feels instant and smart (`Raycast / VS Code` craft).
2. Group results under quiet uppercase category headers (`Navigation`, `Theme`, `Debug`) with right-aligned keyboard shortcut keycaps.
3. Show a live `"Executed: …"` toast + recent-commands history strip when running a command so picking feels genuinely useful.
4. Auto-focus the search input on screen entry and show a clean `"No commands match"` empty state with `"Clear"` button.
5. Add soft shadow depth and subtle `1px` border to the floating palette modal frame.

### 146. `simple_button` (`src/systems/screens/ExampleSimpleButton.h`)
1. Give the hero button a clean centered showcase card on Cozy Kraft with a subtle press-state readout (`Idle / Hover / Pressed / Focused`).
2. Display the click counter in bold tabular monospace with a subtle scale pop on increment.
3. Style `"Reset count"` as a quiet secondary ghost/outline button so the primary demo button owns the focal hierarchy.
4. Show the minimal 5-line C++ `button()` snippet in a quiet card below — making the simplest screen in the repo the best "Hello World" onboarding page!
5. Add a `"Copy snippet"` button next to the code block.

### 147. `text_overflow` (`src/systems/screens/ExampleTextOverflow.h`)
1. Put `Clip`, `Ellipsis (…)`, `Wrap`, and `Overflow Debug` side by side with identical strings and a live box-width slider (`80px → 320px`) so users can drag right across the overflow threshold.
2. Show a clear `"Overflowing: +42px"` red pill vs `"Fits"` green pill beneath each box.
3. Add a `"Toggle debug indicators"` switch that turns the red/amber overflow stripes on and off cleanly.
4. Include an unbreakable-word stress test (`supercalifragilisticexpialidocious`) alongside normal sentences.
5. Keep Neon Dark container borders subtle so the bright overflow debug overlays are immediately obvious.

### 148. `layout_acid_test` (`src/systems/screens/LayoutAcidTest.h`)
1. Add a crisp `"Expected Pass Criteria"` checklist strip (e.g., `No red bleed · Crisp clip corners · Aligned inner grid`) so anyone opening the acid test knows instantly whether it passed.
2. Add a `"Toggle pass/fail reference overlay"` hairline grid showing exact expected pixel edges.
3. Replace arbitrary raw `Color{}` fills with deliberate Acid-Test semantics (neutral gray structure, green pass zones, red only where bleed/bug would appear).
4. Display viewport dimensions and scale factor (`1280×720 · 1.00×`) in a quiet corner badge.
5. Keep margins symmetrical so edge-clipping cases are never confused with viewport edges.

### 149. `layout_bug_repros` (`src/systems/screens/LayoutBugRepros.h`)
1. Give each of the 7 regression fixture tabs a concise human-readable name + status pill (`Fixed ✓` / `Open`) and a 1-line `"What broke"` / `"Expected"` header.
2. Highlight the exact target node (`percent(1.0) child`, `Child 1`, `Child 2`) with a subtle dashed expected-bounds overlay toggle.
3. Show live computed rect `(x, y, w, h)` in monospace on each fixture's key child so regressions are verifiable numerically, not just by eye.
4. Add a `"Copy fixture report"` button dumping all 7 rects for bug reports.
5. Keep tab navigation crisp with consistent Midnight card padding and clear active tab underline.

### 150. `layout_patterns` (`src/systems/screens/LayoutPatterns.h`)
1. Label each minimal E2E layout pattern (`Header + Content`, `Sidebar + Main`, `3-Col Equal`, `Centered Card`) with a crisp title and pixel dimensions badge.
2. Add a `"Show bounds & gaps"` debug overlay toggle that renders container padding/gap guides without altering E2E geometry.
3. Use a restrained tonal surface palette so structural nesting reads cleanly at a glance.
4. Keep consistent `8px` spacing multiples and aligned outer card frames across all pattern tiles.
5. Add a quiet footer note noting the fixed E2E geometry contract so future contributors don't accidentally break test selectors.

### 151. `scroll_click_bug` (`src/systems/screens/ScrollClickBug.h`)
1. Show a bold live verification readout: **`Clicked row: #14 · Expected: #14 [PASS]`** alongside current `scroll_y` offset in tabular monospace.
2. Flash the clicked row in emerald green (or red on mismatch) so hit-test accuracy after scrolling is unmistakable.
3. Add quick `"Scroll to 0px"`, `"Scroll to 200px"`, `"Scroll to Bottom"` buttons next to `"Reset scroll and result"` for 1-click reproduction.
4. Tone down the 10 raw `Color{}` row fills into alternating quiet surfaces with crisp row index badges (`#01`–`#30`).
5. Display the raw pointer `(x, y)` vs scroll-adjusted `(x, y + scroll_y)` coordinates in a tiny diagnostic footer.

### 152. `scroll_clip_bug` (`src/systems/screens/ScrollClipBugRepro.h`)
1. Align the 4 regression panels (`1: Control`, `2: ClipChild rows`, `3: Nested wrapper`, `4: ...`) into 4 clean equal-width columns with a concise 1-line pass/fail criterion under each title.
2. Add a `"Sync scroll all 4 panels"` button so you can compare clipping behavior at the exact same scroll offset across all 4 panels simultaneously.
3. Highlight the clip boundary edge with a crisp 1px dashed cyan rule so any child bleeding past the scissor rect is immediately caught.
4. Condense the dense geometry subtitle (`Original geometry: 40px rows / outer height = ...`) into neat key-value chips (`Row: 40px`, `Inner: 1.5×`).
5. Add a `"Scroll all to 50%"` test trigger button in the header.

### 153. `scrollbar_layer_repro` (`src/systems/screens/ScrollbarLayerRepro.h`)
1. Add a clear visual status banner (`Modal: CLOSED — Background scroll active` → `Modal: OPEN — Scrollbar & wheel must stay hidden/blocked`).
2. Show a live background `scroll_y` counter inside the `"Foreground dialog"` itself so you can wheel-scroll over the modal and prove background `scroll_y` stays frozen at `0`.
3. Give the `"Foreground dialog"` a proper dimmed backdrop and soft shadow elevation so z-layering over the scrollbar is unmistakably clear.
4. Add a `"Scroll background to mid"` button before opening so the scrollbar thumb is positioned right behind the dialog center where bleed-through is easiest to spot.
5. Keep row spacing and padding clean on `"Background row N"` items.

### 154. `terminal` (`src/systems/screens/TerminalDemo.h`)
1. Style the command overlay like a crisp Quake/VS Code developer console (monospace font, subtle translucent dark backing, prompt `>` in accent color, `F2` / `Esc` keycap hint).
2. Add command autocomplete hints (`Tab` to complete) and `↑` / `↓` command history recall with a subtle placeholder (`Type 'help' to list commands...`).
3. Color-code output lines semantically (command echo in muted cyan, success in green, errors in soft coral, help table aligned in columns).
4. Add a `"Copy output"` button and `"Clear"` command so developers can grab terminal output cleanly (`Software You Can Love`).
5. Make the background page behind `"Open overlay (F2)"` show a neat clickable cheatsheet of registered demo commands so first-time users know what to try immediately.

### 155. `themes` (`src/systems/screens/Themes.h`)
1. **Replace the bright Secondary Green (`#0C5E2A`) fills on every unselected theme button in the left sidebar** with quiet surface rows + tiny 3-dot palette swatch previews — let ONLY the active theme carry the accent bar!
2. Refine the chunky `32px`-tall green-and-blue slider bar into a sleek track with rounded caps and a crisp high-contrast thumb so it reads as a slider, not a progress bar.
3. Place the `"Shadow comparison"` cards on a lighter preview strip (or add a Light/Dark comparison backdrop) so dark drop shadows are actually visible on Ocean Navy / Neon Dark.
4. Soften the stark `1px` white borders around `"Primary Text / Secondary Text / Surface Text"` specimen boxes into whisper-quiet surface elevation.
5. Add a `"Copy theme tokens"` button at the bottom of the `"Resolved color tokens"` panel so developers can export any preset in one click.

### 156. `touch_targets` (`src/systems/screens/TouchTargetDemo.h`)
1. Draw a crisp `44 × 44 px` (or `48 × 48 px`) dotted minimum-target halo around every button specimen so undersized targets (`24px`, `32px`) visibly fail the threshold by eye.
2. Stamp each control with a semantic pass/fail pill (`28×28 · Too small` in coral vs `48×48 · Pass` in emerald).
3. Add a `"Standard: Apple HIG (44px) / Material (48px)"` toggle pill in the header.
4. Include an interactive `"Hit accuracy test"` mini-counter showing clicks landed vs missed near the edge of small vs large targets.
5. Keep Ocean Navy card padding symmetrical and remove any competing borders around the target halos.


