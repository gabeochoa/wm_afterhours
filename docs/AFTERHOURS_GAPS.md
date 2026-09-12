# Afterhours Library Gaps & Workarounds

This file tracks afterhours library limitations and wm-owned implementation
and visual parity gaps. Each finding identifies its owner where known;
resolved findings remain as evidence. Library changes and the submodule pin
are frozen pending user review during this audit.

The cross-project library inventory below was rechecked against `main` on
2026-09-12. That sweep is
worth repeating before trusting any of it, because the consumer docs these were
collected from are all months stale: of everything collected this cycle, hanabi
had 6 of their top 10 already fixed, cartographer's component asks were almost
entirely in, floatinghotel's footgun list was half done, and three of their four
sokol items had landed. The most valuable output of a collection pass is now
telling the projects what they can delete, not finding new work.

See also: `docs/vendor_ui_sizing_issues.md`

---

## Screen mock audit

This section also records wm-owned findings from the 26-screen mock audit.
It does not classify a visual mismatch as a library defect. Library changes
and the submodule pin are frozen for this pass; proven upstream blockers
remain open for user review. Delivery status: `docs/screen-audit.md`.

### Visual acceptance reopened

The user rejected the earlier assessment that the remaining differences were
only minor or cosmetic. The C++ screens still fall short of the web mocks.
The earlier reviews established functional progress and readable layouts;
they did not establish visual parity. All 26 screens retain open visual gaps.

The web mock remains the acceptance target. Typography, composition, depth,
artwork and control states must be reviewed together. Passing E2E tests proves
the asserted behavior. Passing screenshot validation proves stability against
native baselines. Neither proves a match to `mocks.html`.

### Shared visual gaps

These entries collect the implementation shortcuts behind the per-screen
findings below. They describe missing results in wm, not confirmed missing
APIs in afterhours. Check the existing library support before assigning an
upstream change; library changes still require user review.

| ID | Open gap and evidence | Ownership / next work | Closure evidence |
|---|---|---|---|
| VP-01 | Typography does not match the browser: font weight, italics, tracking, line height, wrapping and mixed-size text. Fighter, Flight, Race Results and Kart Select have different display lettering; Potion and Shop lack the target italic treatment; Neon uses one ammo font size. | wm must match actual font faces and text metrics. Investigate library support only where a specific text treatment cannot be expressed. Synthetic outlines or a larger atlas alone do not reproduce the intended weight or slant. | Compare headings, body copy, values and hints at 720p/1080p. Match hierarchy, baseline placement, line breaks and emphasis without clipping. |
| VP-02 | Rotation, skew and perspective were flattened. Fighter's title/hologram and Dead Space's rows lack the target transforms; Parcel's phone and Empire's logo remain upright. These change the composition substantially. | wm implementation gap. Determine whether existing drawing and transform support can reproduce the composition; any missing reusable transform API needs a separate library reproduction. | Match angles, silhouettes and overlap to the mock. Transformed controls must retain correctly placed pointer targets and keyboard navigation. |
| VP-03 | Gradients, shadows, glow and inset highlights are simplified or absent. Cozy loses radial lighting and paper depth; Empire loses plate/button shading; Dead Space and Flight lose glow; PowerWash, Kart, Offsite and Secure Tunnel have flatter shadows. | wm implementation gap; library capability and rendering cost need investigation. Flat fills are not an accepted substitute for the mock's depth. | Compare light direction, gradient falloff, shadow spread, glow and foreground/background separation in matched captures. |
| VP-04 | Borders, corner shapes, clipping and layered decoration differ. Cozy lacks inset arcs and its paper fold; Minesweeper has square bevel joins; Marlo cards differ in rounding; Guess Who outlines are heavier; Potion's inset border is faint. | wm owns the visible result. The uniform-border thickness bug is separately proven below; do not assign every shape mismatch to that bug. | Match border thickness, corner silhouettes, inset edges and overlaps at both resolutions. Verify content and shadows clip where the mock clips them. |
| VP-05 | Artwork, icons and small controls are approximated with simpler shapes or text glyphs. PowerWash status/gear icons, Secure Tunnel flags/shield, AIM and Media toolbar icons, Shop items and Offsite folders differ; mobile currency/lives decoration is missing or simplified. | wm art and rendering work. Use faithful isolated artwork or native vector drawing while keeping labels, values and controls live. | Compare silhouettes, stroke weights, detail, color and scale. No missing or substitute glyphs where the mock has a distinct icon. |
| VP-06 | Spacing and proportions remain wrong even where the main panels align. Parcel's objective badge crowds text; PowerWash help wraps differently; Guess Who arrows sit too close to labels; Race headers and Flight keycaps are smaller; several screens have different rail, footer or label spacing. | wm layout and text measurement. Absence of overflow is only a minimum correctness check. | Match padding, alignment, line lengths, density and relative sizes to the mock in the same viewport and state. |
| VP-07 | Decorative backgrounds lost material and scene detail. Cozy lacks grain; Islands has approximate paper/background contours; Parcel's street/rider and PowerWash's wall/pool scene are simplified. These affect the overall resemblance, not just individual widgets. | wm artwork/composition work. Keep decorative textures separate from changing content. | Compare the full canvas as well as control crops. Match texture, scene geometry, contrast and the balance between decoration and controls. |
| VP-08 | Control-state appearance has not been accepted against the web mock. Sports selected-row shading differs; Media's selected poster border differs; Guess Who dropdown chevrons differ. Existing interaction tests do not establish matching hover, focus, pressed, disabled, expanded or transition visuals across all screens. | wm review and implementation. Audit these states explicitly; record any missing transition or state treatment with a reproduction instead of assuming it matches because the control works. | Capture matching pointer and keyboard states, open menus/dialogs, selection, disabled controls and transitions. Compare appearance and hit areas against the web version while preserving existing behavior. |
| VP-09 | Visual review used an inadequate acceptance standard. Substantial changes to perspective, type and depth were labeled cosmetic; passing native baselines and E2E checks were used to support overly broad completion claims. | wm audit process. Reopen visual acceptance for all 26 screens and keep functional verification separate from appearance. | Each screen needs direct web-versus-C++ review in matched states and resolutions. Close concrete visual findings only when the result matches or the user accepts a specific deviation. |
| VP-10 | Browser styling is easy to compose, while the native implementations used repeated custom drawing and approximations. The audit has not established which treatments need better library APIs and which merely need correct wm code. | wm investigation first. For a proposed library gap, record the desired effect, existing API attempted, minimal reproduction and limitation. Existing border/position/slider findings below remain separate proven cases. | Each suspected API gap has evidence and a clear owner. Library availability alone does not close the corresponding visual gap; the screen must render the intended result. |

### Per-screen findings

The following rows preserve implemented fixes and known residual differences.
References to passing tests or previous reviews are evidence of those checks,
not visual acceptance. The shared gaps above and these screen-specific visual
gaps remain open.

| Screen / state | Finding | Owner | Disposition / evidence |
|---|---|---|---|
| Hosted gallery / comparison views | Relative image and font URLs break outside the repository. | wm tooling | Standalone export embeds all 48 assets; 26 canvases and all comparison images verified outside the repo. Pixelcloud revision 5 embeds all 26 audited native baselines and opens directly in a draggable Original vs Mock/Current comparison; pointer and keyboard checks passed. |
| potion_crafting / recipe view | Dark panels, text initials for ingredients, and a rectangular flask did not match the parchment composition. | wm | Replaced with parchment layout, botanical/bottle artwork, scaled geometry and Garamond text; reviewed at 720p and 1080p. |
| potion_crafting / crafting | Brew ignored clicks; tabs did not change content; recipe row IDs overlapped. | wm | Unique IDs, live stock consumption, shortage handling, recipe-aware ingredient requirements, inventory and journal views. E2E 40 exercises pointer, real Tab traversal, Enter, repeated shortage and resize. |
| potion_crafting / visual parity | Secondary copy lacks the target's italic face; brew button's inset border is subtler; footer gives keyboard guidance instead of the decorative Close workshop hint. | wm | Open visual parity gap. No library blocker claimed; gameplay and layout checks pass. |
| angry_birds_settings / layout | Flat board, mismatched icons and uneven button caps did not resemble the mobile settings mock. | wm | Shared label-free forest, board and button artwork, high-resolution Fredoka atlas, and explicit scaled geometry. Fresh 720p/1080p captures reviewed independently. |
| angry_birds_settings / interactions | Language, help and progress actions lacked useful views. | wm | Real language selection, separate campaign save/load state, credits/support/privacy views, notification/audio settings, apply/cancel, close/reopen and Escape. E2E 128 and responsive/containment regressions pass. |
| angry_birds_settings / visual parity | Native font outlines are heavier; hint baseline is about 7px higher; faint background currency pill is absent. | wm | Open visual parity gap; primary layout and control geometry match. |
| cozy_cafe / layout | Mis-sized panels, substitute icons, faint borders and clipped footer/music labels differed from the cafe mock. | wm | Matched panel geometry, isolated flower/star/tool artwork, scaled outlines and improved type. Fresh 720p/1080p captures; independent reviews. |
| cozy_cafe / service and selection | Serving incremented counters without removing orders; selected specials lacked a visible state. | wm | Serve consumes the next order and disables when empty, promotion uses the selected special, visible selection outline and live music control. E2E 129 checks pointer, Tab/Enter, depleted queue and resize. |
| cozy_cafe / visual parity | Paper lacks the mock's grain, radial glow, inset arcs and corner fold; native text outlines are stronger and title wider; customer separators use hyphens. | wm | Open visual parity gap. Correctness, overflow and parent containment checks pass. |
| casual_settings / layout and dialogs | Generic colors, substitute icons and inert information actions differed from the mobile mock. | wm | Shared decorative board/icons, matched geometry, high-resolution rounded type, language apply/cancel, campaign save/load, credits, support FAQ, terms and about views. E2E 130 passes at both resolutions, including dialog containment and keyboard use. |
| casual_settings / visual parity | Faint lives/currency pills are simplified to a currency label; native label outlines differ. | wm | Open visual parity gap. Fresh 720p/1080p captures independently reviewed; text-overflow warnings resolved. |
| fighter_menu / rendering and navigation | Missing lobby, invisible slanted triangles, blurred display type and letter substitutes for icons weakened the fighter layout. | wm | Fixed triangle winding and background layering, added isolated lobby/icons and a 96px display font. Live tabs and option descriptions; E2E 131 passes pointer, Enter/arrows and 1080 resize. Fresh images independently reviewed. |
| fighter_menu / visual parity | Title/hologram lack the mock's rotation and perspective; serif weight is thinner; lobby and panel shading are approximate. | wm | Open visual parity gap. No clipping or interaction blocker found in the final 720p/1080p review. |
| flight_options / layout and settings | Generic options layout and shallow interactions did not match the flight-system mock. | wm | Matched the airspace grid, selection geometry, nine categories and setting editors. Apply/cancel retains or discards values; keyboard and pointer coverage in E2E 134. |
| flight_options / visual parity | Native lettering is thinner and lacks the browser glow; keycap labels are smaller. Vibration is explicitly marked unavailable. | wm | Open visual parity gap and a stated hardware limitation; final 720p/1080p review found no clipping or interaction blocker. |
| deadspace_settings / hologram and controls | Rectangular slabs, invisible corner triangles and inert categories missed the hologram design. | wm | Winding-correct clipped polygons, scan lines, layered menus and live category detail settings; E2E 132 passes pointer/keyboard/back and resize. |
| deadspace_settings / visual parity | Rows lack the mock's perspective skew and stronger glow; rear panel contrast and keycaps differ. | wm | Open visual parity gap. Final 720p/1080p captures independently reviewed with no clipping or unreadable selection. |
| kirby_options / notebook and preferences | Plain panels and decorative-only options missed the notebook composition and useful interactions. | wm | Isolated paper, tab and icon artwork with native labels; editable name, per-category preferences, favorite mode, tab views and confirmed profile reset. E2E 137 covers pointer, Tab/Enter and resize. |
| kirby_options / visual parity | Native type spacing/weight and pencil artwork differ; bumper hints use purple rather than dark keycaps. Online view is explicitly local. | wm | Open visual parity gap and stated simulation scope. Final 720p/1080p images independently reviewed. |
| minesweeper_lab / desktop and game | Generic chrome, unreadable counters and overlapping taskbar text missed the classic desktop mock. | wm | Native desktop/window/taskbar, high-resolution font aliases, regular counter face to distinguish zero from eight, separate labels for icon buttons, real game reset and window state controls. Existing play test 120 and new 139 pass. |
| minesweeper_lab / visual parity | Bevel corners are square instead of diagonal, menu underlines are absent and window text spacing differs. | wm | Open visual parity gap; board gameplay preserved and 720p/1080p reviewed. Access-key underlines remain an existing library gap below. |
| empire_tycoon / dashboard and state | Hidden park art, substitute icons, misplaced controls and faint borders weakened the tycoon composition. | wm | Isolated park/icons, winding/layering fixes, scaled outlines and dashboard geometry. Live production, cash, projects, gauges and milestone react to actions; E2E 133 passes. |
| empire_tycoon / visual parity and scope | Logo remains upright; plates/buttons lack target gradients and inset highlights; body text uses a heavier rounded face; gauges and chat tail are simplified. Tool buttons report selection without full destination screens, as in the mock. | wm | Open visual parity gap and bounded demo navigation. 720p/1080p reviewed; no library changes. |
| mini_motorways_settings / layout and categories | Typography, detached controls and road bend artifacts missed the map-like settings mock. | wm | Native grid/road geometry, scaled controls and real per-tab content; ring-based bend eliminates draw-segment artifacts. |
| mini_motorways_settings / visual parity | Font tracking, symbol edges and exact tutorial/version text dimensions differ. | wm | Open visual parity gap; E2E 140 covers pointer, keyboard, values and containment at both resolutions. |
| neon_strike / HUD and actions | Generic HUD placement, missing artwork and inert controls differed from the mock. | wm | Matched map/compass/equipment geometry with isolated text-free art and native high-resolution labels. Reload transfers reserve ammunition; abilities and equipment have visible selection. |
| neon_strike / visual parity | Ammo uses one font size instead of mixed sizes; objective chevrons and subtle background glow differ. | wm | Open visual parity gap. E2E 141 and final 720p/1080p reviews pass. |
| marlo_kart / six-phase presentation | Driver/kart/cup screens and racing presentation did not match the mock; custom roads/maps were accidentally covered by panel fills in the first audit draft. | wm | Isolated driver/kart/trophy art, live text and HUD, perspective road with moving stripes, visible shared-path minimap/cup previews, pause above countdown. Existing fixed-step racing, items, drift, eight racers, four cups and points remain intact. E2E 126/127/138 pass. |
| marlo_kart / visual parity | Card rounding, label proportions and key hints differ from the mock; native race view follows the real simulation rather than the gallery's phase shortcuts. | wm | Open visual parity gap; all six phases visually reviewed, including actual cup completion and trophy standings. High-resolution regular font removes enlarged-text blur. |
| aim_chat / desktop and messaging | Simplified chrome and shared/inert conversation actions did not match the classic messenger mock. | wm | Two native windows, desktop/taskbar, isolated buddy history/drafts, multiline composer, warn/block, menus and window controls. Existing text-editing test and E2E 149 pass. |
| aim_chat / modal hit testing | The nested pasted-log scrollbar intercepted Clear unsent draft in the Edit dialog, leaving the dialog open and blocking minimize. | wm workaround; deeper ownership unresolved | Disable both underlying scroll axes while a modal is open; restore on close. Reproduced before fix; E2E 149 now clears the draft and minimizes/restores correctly. No afterhours edit. |
| aim_chat / visual parity and scope | Fonts are lighter; some toolbar/taskbar icons are text substitutes; italic/underline report format availability instead of formatting text. Buddy List is a fixed-size utility window. | wm | Open visual parity and formatting gaps. Chat maximize works; messaging is explicitly a local demo. |
| islands_trains_settings / layout and actions | Centered paper/row proportions differed and Keyboard/Tutorial had no visible effect; Close only displayed a status. | wm | Matched paper/controls and added bindings help, local pan/place/run tutorial, real close/reopen with preserved preferences. E2E 136 covers these flows. |
| islands_trains_settings / visual parity | Typography and close glyph are lighter; paper/background contours and gradient are approximate. | wm | Open visual parity gap; all controls and tutorial views remain contained at both resolutions. |
| media_library / layout and browsing | Generic grid and card padding displaced posters; tiny thumbnail labels overflowed in list mode. | wm | Matched library/sidebar/detail composition, six isolated poster templates with native titles, real filtering/sorting/paging, list view and per-item watchlist. Removed duplicate tiny list-poster titles; adjacent live titles remain. |
| media_library / visual parity and playback | Text is lighter and some navigation icons are text substitutes; selected poster border is thinner. Playback is a labeled local preview rather than a media backend. | wm | Open visual parity gap and explicit demo scope. E2E 150 covers browsing and pause/seek with parent containment. |
| shop_interface / shop and basket | Generic rounded rows, mismatched proportions and invisible flask liquid weakened the store mock. | wm | Square table rows, paper/wood frames, visible liquid, native item art, live basket purchase and gold/stock changes. Keyboard add/remove and real close/reopen verified in E2E 135. |
| shop_interface / visual parity and scope | Emboldened serif headings are softer/heavier than target; item drawings and italic text remain approximate. Basket previews only its first three distinct items; Sell remains a preview tab. | wm | Open visual parity and demo-scope gaps. Full basket totals still include every item; primary purchase flow and 720p/1080p containment pass. |
| rubber_bandits_menu / layout and roster | Logo font, promo overflow, squared selection and invisible bolt differed from the mock. | wm | Correct isolated logo/cast, rounded selection, bounded promo text, native bolt and live character/menu state. All four names checked for overflow. |
| rubber_bandits_menu / visual parity | Promo border/inset, character-label weight and small footer details differ. Supporter action is local demo feedback. | wm | Open visual parity gap; final 720p/1080p images and long-name state reviewed. |
| secure_tunnel / client and connection | Dark state-matrix demo lacked the mock map/sidebar and location search. | wm | Native client layout, isolated map art with live markers, typed search and filtered rows, selected-server details and protocol selection. Existing Off/Dialing/Up transition preserved; E2E 152 passes. |
| secure_tunnel / visual parity and scope | Condensed type, flag/icon drawings, rail spacing and shadows differ; shield artwork is approximate. Protocol, diagnostics and account actions are local simulations. | wm | Open visual parity and demo-scope gaps. The screen states network simulation; no real tunnel is created. |
| parcel_corps_settings / phone settings | Controls extended beyond the phone, selector arrows hid values and volume controls only incremented on clicks. | wm | Contained native controls with visible values; real draggable sliders and predictable keyboard steps. Status moved into free space above audio rows. E2E 142 verifies exact values and drag at both resolutions. |
| parcel_corps_settings / visual parity | Phone remains upright, rider/street art and icons are simplified, and the objective badge crowds its final line. | wm | Open visual parity gap. Primary settings are readable and unobstructed; previous review checked readability, not visual parity. |
| race_results / results layout | Tiny text, invisible header triangles, rounded rows and missing footer contrast weakened the results mock. | wm | Matched skewed row fills/backing, readable standings, corrected triangle winding, isolated shared racing scenery/portraits and restored light footer. |
| race_results / visual parity and scope | Native type lacks italic slant and some headers are smaller; result action buttons report navigation choices in this standalone screen. | wm | Open visual parity gap and demo navigation scope. E2E 144 covers selection/action feedback and resize. |
| sports_settings / graphics settings | Labels displayed raw slider percentages instead of FPS/gamma; controls reused cached values across tabs and Back only changed a status. | wm | Domain value formatting, per-tab slider identity, internal state synchronization after external edits/reset, real close/reopen and initial-value reset. Exact scene artwork and native controls match the layout. |
| sports_settings / visual parity | Text weight, selected-row shading and tiny bumper keycap glyphs differ; scrollbar lacks diagonal stripes. | wm | Open visual parity gap. E2E 145 and independent audio/default captures confirm values and knob positions agree. |
| offsite_backup / backup layout | Small type and overlapping old controls obscured the new backup layout. | wm | Matched desktop/window/panels, removed duplicate rows/switches, separated checkbox labels and completion text. Existing progress/rescan model and local backup controls retained. |
| offsite_backup / visual parity and scope | Native typography/ring differ, folder icons are faint, and borders/shadows are flatter than the mock. Backup, restore and account actions are local demonstrations. | wm | Open visual parity gap and demo scope; final labels and controls are readable without overlap. |
| powerwash_settings / tablet and actions | Generic panel geometry, clipped help/keycaps and status-only close/reset differed from the mock. | wm | Matched tablet, tabs and setting rows; native values, initial-value reset and real close/reopen/Escape. Fresh captures and E2E 143 pass. |
| powerwash_settings / visual parity | Status and gear icons are approximate; help wraps differently; background wall/pool shapes and tablet shadow are simplified. | wm | Open visual parity gap. Both resolutions remain readable and contained. |
| kart_select / racer selection | Generic cards, missing preview art and misplaced controls differed from the mock. | wm | Isolated eight-racer portrait/kart atlases, native labels and stats, checker header, platform and keyboard/pointer selection. E2E 42/147 pass. |
| kart_select / visual parity and scope | Display type lacks italic slant, labels have different weight/spacing and preview shadows are flatter. Ready reports the chosen driver/vehicle in this standalone demo. | wm | Open visual parity gap and demo scope; all art is present and both resolutions independently reviewed. |
| guess_who_lab / board and questions | Generic board lacked the mock portraits and complete question/note controls. | wm | Isolated logo and 24-portrait atlas, native cards/labels, two question dropdowns and 24 note dropdowns; filtering, flips and reset work. E2E 121/148 pass without overflow. |
| guess_who_lab / visual parity | Native typography, title-case names and dropdown chevrons differ; outlines are heavier and reset/ask arrows sit closer to labels. | wm | Open visual parity gap. All 24 cards and controls remain readable and contained at both resolutions. |

---


### Final regression audit

The full suite exposed stale Sports/Shop expectations and an Empire status
label whose text needed 181px in a 170px box. The legacy tests now exercise
the approved controls; Empire's box is 190px wide with its visual center
unchanged. Fresh default captures are byte-identical after that containment
fix. The Tooltip baseline included a keyboard focus ring inherited from an
earlier script. Its test now sends Tab and explicitly focuses the first button;
both existing tooltip baselines match exactly without replacement.


### Capture runner: focus state leaked between screens

`UIContext::reset()` leaves `has_interacted` set. A screen that explicitly took
focus therefore enabled a ring in every later batch capture, while the same
screen captured alone had no ring. wm's screenshot reset now clears that flag
once per screen; explicit focus inside a screen still works. This changes 45
legacy baselines only by removing focus outlines (88,315 pixels total,
independently reviewed); the other 45 legacy images are identical. No legacy
screen source or library code changed. The 26 audited screens pass the same
comparison threshold against their isolated captures; 25 match pixel for pixel.

AIM retains a separate small capture-order difference: the disabled Buddy List
maximize button has a blue fill in the batch and grey in an isolated capture
(17×17px region). This is below the screenshot threshold, does not change
interaction, and remains open for investigation rather than being baselined
away. The gallery uses the isolated capture.

### Capture runner: multiple resolutions in one process

`--headless-screenshots --screen example_borders --resolution 720p,1080p`
crashes at the second resolution in `EntityCollection::invalidate_entity_slot_if_any`
with an out-of-range slot. Reproduced on unchanged `example_borders`, independently
of the new screen layout. Separate 720p and 1080p invocations work. Owner is not
established between wm reset sequencing and the library collection lifetime;
afterhours remains frozen. This is not an implementation of the unrelated
cascade-delete plan.

### Absolute horizontal positions resolve against screen height

At the frozen pin, `component_init.h` resolves both `translate_x` and
`translate_y` using `screen_height`. Passing `w1280(1047)` as an absolute x
therefore produces 589px at 1280×720 instead of 1047px. The first Angry Birds
audit draft reproduced the misplaced controls. wm now supplies explicit pixel
positions scaled from the current UI context. Library behavior remains open
for review; no vendor changes were made.

### Uniform solid borders ignore requested thickness

`rendering.h`'s uniform-border branch calls `draw_rectangle_rounded_lines`
without passing `BorderSide::thickness`. The rounded helper renders a 1px
outline; the sharp helper uses a fixed 3px. Cozy Cafe's requested 4px paper
border and 3px panel borders consequently appeared as faint 1px lines.
The wm screen draws its outlines with the existing thickness-aware helper.
A library fix is deferred for user review. This also explains the subtler
Potion Crafting inset border noted above.


### Slider state does not follow external model changes

At the frozen pin, `imm_components.h` initializes `HasSliderState` only when
creating the slider, then writes its cached value back through the supplied
reference on subsequent frames. Sports settings reproduced stale knob positions
after tab switches and Reset, even when the displayed domain values changed.
wm now uses per-tab slider IDs and synchronizes the cached state when the model
changes externally. E2E 145 checks reset values and subsequent keyboard/drag
edits; independent captures confirm labels and knob positions agree. Whether
this should be a controlled-value API is deferred for library review.

### Slider keyboard steps repeat within one keypress

Parcel Corps reproduced 25 → 23 from one injected Left keypress. The native
slider listener increments by 0.01 for each callback while the key is held;
it does not use the UI context's pressed/repeat pacing. wm wraps the listener
with `pressed_or_repeat` so keyboard adjustment follows the existing repeat
schedule. E2E 142 verifies exact single steps in both directions, dragging,
and resized input. Library input pacing remains open for review.

## Cross-project upstream review, 2026-09-12

Reviewed the 38 other top-level project directories under `~/p` with three
parallel source reviewers, plus the owned nested `armchair_coach/puzzle` project. Coverage, revisions, exclusions and adoption opportunities
are recorded in [afterhours-upstream-review.md](afterhours-upstream-review.md).
This was a source review, not an exhaustive line-by-line or runtime audit.
No applications were run and no consumer or library code was changed.

Availability was checked against both local library copies: standalone
`afterhours` at `19d6c97` and wm's newer frozen vendor at `e3f13a7`.
The former is an ancestor of the latter. A feature present in the newer copy
is already implemented; publishing and consumer pin updates are separate work.
These checks do not establish the current state of any remote branch.

The source review initially proposed the requests below. The decision table
records the user's subsequent selections; none are marked implemented here.
P1 identifies shared correctness or test-isolation problems; P2 identifies
reusable capabilities; P3 requires more evidence before extraction. Paths in
source citations are relative to `~/p`. Library citations refer to wm's vendor
unless stated otherwise. Line numbers describe the reviewed local snapshots.

| ID | Priority | Request | Evidence class |
|---|---|---|---|
| UP-01 | P1 | Independent master/music/effects gain and correct late sound loading | Defect confirmed by source and independent review |
| UP-02 | P1 | Settings writes preserve the last good file and report failure | Defect confirmed by source and independent review |
| UP-03 | P1 | Scoped clipboard provider shared by apps and built-in widget tests | Missing test boundary; two consumer implementations |
| UP-04 | P2 | Optional native open/save/directory dialogs | Repeated platform integration in two apps |
| UP-05 | P2 | Lossless input-binding encoding and decoding | Repeated persistence; incomplete migration adapter |
| UP-06 | P2 | Device-aware display of current action bindings | Live consumer formatter; mapping lookup already exists |
| UP-07 | P2 | Accessible control semantics and announcements | Working browser pattern; native bridge absent in inspected source |
| UP-08 | P2 | Mutable RGBA textures through a common backend API | Extension of the existing image-wrapper gap |
| UP-09 | P3 | Optional filesystem watcher with explicit availability | One substantial implementation; confirm another consumer first |
| UP-10 | P1 | Consistent in-memory render capture format | PNG on raylib, raw RGBA on sokol/Metal under the same API |
| UP-11 | P2 | Useful default profiling UI with customization | User-requested addition; collection hooks and a wm demonstration already exist |
| UP-12 | P2 | Chart set with an interactive wm test screen | User-expanded sparkline proposal; profiling supplies a concrete consumer |

### User decisions

Follow-up design choices and implementation readiness are tracked in
[gap-design-decisions.md](gap-design-decisions.md). Pending recommendations there
are not accepted decisions.

| Item | Decision | Scope / condition |
|---|---|---|
| UP-01 Audio gains | Approved | Master scales music/effects while preserving their relative preferences. |
| UP-02 Settings saves | Approved | Preserve the previous file on failure and report failures accurately. |
| UP-03 Test clipboard | Approved | Isolate both app and built-in widget clipboard operations. |
| UP-04 Native dialogs | Approved | Shared open/save/folder dialogs with safe completion and test responses. |
| UP-05 Binding persistence | Approved | Save/load complete bindings, including modifiers and axes. |
| UP-06 Binding prompts | Approved | Current binding labels and keyboard/controller prompts. |
| UP-07 Screen-reader support | Deferred | Wait for a library consumer to need it, implement it and propose upstreaming. |
| UP-08 Mutable RGBA textures | Deferred | Wait for an afterhours consumer implementation, then consider upstreaming. |
| UP-09 Filesystem watcher | Approved | Upstream the existing watcher. Wordproc is a possible second consumer, not a confirmed one. |
| UP-10 Capture formats | Approved | Make encoded and raw capture results explicit and consistent. |
| UP-11 Default profiling UI | Requested addition | Useful defaults with customization; reuse existing profiling hooks. |
| UP-12 Chart set and test screen | Approved, expanded | Replace the small sparkline proposal with a chart set and an interactive wm test screen. |
| UI sound-feedback hooks | Skip for now | Use existing click callbacks. Revisit only if a consumer needs a missing focus-change notification. |
| Periodic timer helper | Skip | No shared remainder-preserving timer requested. |

### UP-01: Independent audio gains

`pharmasea/src/engine/settings.cpp:110` sets music and effects levels before
calling `sound_system::set_master_volume`. The shared implementation at
`src/plugins/sound_system.h:344` overwrites both category levels. The setters
at `:91` and `:152` replace stored preferences as well as backend gain. For
example, effects 0.2, music 0.3, then master 0.5 produces 0.5 for both categories,
rather than effective gains 0.1 and 0.15. Changing a category after muting
master can also bypass that mute.

`MyNameChef/src/settings.cpp:158`, `kart-afterhours/src/settings.cpp:122` and
`afterhours-template/src/settings.cpp:150` bypass the helper with raylib master
volume. `endless-dance-chaos/src/audio.h:198` and `:223` multiply gains locally.
Those are demand/workaround evidence, not affected calls to the shared helper.
A related inconsistency exists at `SoundLibrary::load`, line 39: newly loaded
sounds do not inherit the saved category volume. Music loading already
reapplies its volume at line 136. Both inspected revisions have these behaviors.

Retain independent master and category preferences, apply their combined effect
consistently, and apply the current gain to newly loaded sounds. Existing
libraries and playback requests are sufficient; this does not need a new mixer
or game cue system. Validate category getters, effective gain after changes in
either order, mute/unmute, and loading after a volume change using an
instrumented backend. The source defect is confirmed; no audio runtime test was
performed in this review.

### UP-02: Atomic settings saves and truthful results

`kart-afterhours/src/settings.cpp:75` uses the shared JSON settings save path.
In both revisions, `src/plugins/settings.h:178` opens and truncates the final
file at line 181 before serializing at line 187. A serialization exception
therefore destroys the previous file even though save returns false. The JSON,
raw-string and conditionally compiled Bitsery writers also return success
without checking write/close failure. Raw and Bitsery paths begin at lines 226
and 201. The Bitsery configuration was not compiled during this review.

Serialize completely before touching the destination, then reuse
`src/plugins/files.h:133`, `files::write_string_atomic`, and propagate its
result. Atomic-write support is already implemented; the gap is its integration
into settings. `pharmasea/src/save_game/save_game.cpp:39` separately implements
checked temporary-file replacement, while its engine settings and
`floatinghotel/src/settings.cpp:103` still write settings directly. These
callers can adopt the existing helper independently.

Validate that a throwing serializer, failed write, or failed rename preserves
a known good file and returns failure, and that a successful save reloads
exactly. This requests atomic replacement, not power-loss durability or
concurrent-writer coordination. Kart also ignores the result and updates
`last_written_json` at line 76, suppressing retries; that is a separate
consumer fix and must not be mistaken for an afterhours change.

### UP-03: A scoped clipboard provider for tests

`wordproc/src/util/clipboard.h:11` stores a test clipboard and switches local
get/set calls to it; `wordproc/src/main.cpp:602` enables it for tests.
`hanabi/src/util/clipboard.h:11` instead counts writes and still uses the host
clipboard. Its assertion at `hanabi/src/ecs/e2e_commands.h:238` can accept a
matching old value without proving a fresh write. Meanwhile
`hanabi/src/ui/text_select.h:383` and the built-in text widgets call afterhours
clipboard directly, bypassing app-only replacements.

Both copies of `src/plugins/clipboard.h` call the platform directly, or return
empty/no-op results without a backend. Place an optional scoped provider under
the existing get/set/has operations, with a resettable in-memory implementation
for tests. This must cover calls from `src/plugins/ui/text_input/component.h:574` and
`src/plugins/ui/text_input/utils.h:547` as well as applications. Restore the prior provider
on scope exit. A write generation can distinguish a fresh copy from stale text.

Validate paste and copy through built-in single-line and multiline widgets,
per-test reset, fresh-copy assertions, and restoration of normal behavior.
Tests using the provider must not write the host clipboard. Editors, chat,
consoles and game naming fields all benefit. Clipboard content should not be
dumped as incidental test failure output.

### UP-04: Optional native file dialogs

`wordproc/src/util/file_dialog.mm:26` wraps NSOpenPanel/NSSavePanel;
`wordproc/src/util/file_dialog.h:8` supplies filters and queued test results.
`hanabi/src/native_extras.mm:961` independently implements directory selection,
used at `hanabi/src/ecs/settings_system.h:902`. Wordproc defers these calls
outside ECS execution at `wordproc/src/main.cpp:403` because a native modal loop can
reenter frames and live queries. Its fallback also conflates cancellation with
unsupported platforms by returning an empty string.

Neither inspected library has a native picker. `src/plugins/files.h` supplies
filesystem operations, and in-app modal widgets do not cover this boundary.
Provide an optional file/save/directory picker with owned paths, filter/default
name options, and distinct chosen/cancelled/unsupported/error outcomes. Define
a safe completion boundary outside ECS iteration and queued responses for E2E.
Document import/export and format handling stay with the application.

Validate Unicode paths and spaces, cancellation without state changes, explicit
unsupported outcomes, one-time consumption of queued results, and absence of
nested ECS execution. Editors, attachment pickers, level tools and export flows
are beneficiaries. Platform behavior still needs a later runtime prototype.

### UP-05: Lossless input-binding persistence

`pharmasea/src/engine/keymap.cpp:183` and `:222` encode/decode legacy inputs,
with live loading/saving through `pharmasea/src/preload.cpp:240` and `:769`.
`supermarket-engine/engine/keycodes.h:198` and `:216` independently persist
name/key pairs. Pharmasea's newer afterhours adapter at
`pharmasea/src/input_mapping_persistence.h:14` serializes only the key from a KeyChord,
drops modifier fields, and branches on only three numeric variant positions.
It is included by the app, but no active save/load call to this newer adapter
was found; user-visible corruption through that path is not established.

Both libraries define typed bindings but no persistence codec. The newer
`src/plugins/input_system.h:857` defines chord modifiers and line 900 includes
mouse axes in AnyInput. Provide an optional tagged codec owned with these
binding types, preserving modifiers, explicit no-modifier chords, buttons and
axis directions. Reject malformed/unknown values instead of producing key zero.
Keep action/layer naming, file paths and file format integration caller-owned.
JSON support need not become a mandatory input-plugin dependency.

Validate round-trips for every binding alternative, plain versus explicitly
modifier-free keys, combined modifiers, signed axes and remapping. Invalid data
must not partially replace live mappings. This is a reusable migration aid,
not a claim that input mapping itself is missing.

### UP-06: Binding display and active-device prompts

`cartographer/src/input_action.h:472` tracks the last input device; lines
507-618 format keys/buttons and inspect mapping internals. The formatter at
line 614 discards chord modifiers. It feeds live prompts in
`cartographer/src/map_systems.cpp:1430` and `cartographer/src/gallery_systems.cpp:154`.
Wordproc's menus at `wordproc/src/ui/menu_setup.h:17` and toolbar at
`wordproc/src/ecs/toolbar_system.h:199` instead hard-code shortcut text. Its
`wordproc/src/extracted/action_binding.h` is a draft, not proof of a live shared helper.

`ProvidesLayeredInputMapping::get_bindings` already exists at
`src/plugins/input_system.h:1248`; cartographer can stop walking maps today.
The remaining request is a small typed binding formatter and optional
last-meaningful-device preference. Preserve full modifiers and axis direction,
return an explicit unbound result, and let apps choose text or artwork.
Controller icon packs and game-specific wording stay out of the library.

Validate Ctrl+Shift and Super chords, signed axes, layer/remap changes and
switching between keyboard and gamepad prompts. Sub-deadzone noise must not
switch prompts. Keep this separate from UP-05 so formatting does not require
persistence, or vice versa.

### UP-07: Accessible semantics and announcements

`scrubdaddy/src/components/GameCanvas.jsx:29` updates a polite live region when
the rescued count increases. Lines 47-65 label the canvas and expose that
region. `scrubdaddy/src/App.jsx:681` gives upgrade controls names, roles,
keyboard activation and disabled state. The browser supplies the connection to
assistive technology; this review did not test those controls with a reader.

The inspected afterhours implementations have visual labels, contrast checks
and minimum hit targets, but no semantic control tree or platform announcement
bridge. See `src/plugins/ui/ui_core_components.h:483`, `src/plugins/ui/theme.h:391`, and the design-only
`docs/plans/2026-07-24-accessible-settings-rfc.md`. Access-key underlines alone
do not provide this capability.

Add opt-in role/name/value/state/action metadata and an announcement queue;
prove one native platform adapter before expanding platform coverage. Ordinary
widgets should derive semantics, and custom drawn controls should supply them.
Keep announcement text and event timing with apps. Validate focus descriptions,
state changes, one announcement per event, disabled behavior, retired controls,
and modal/resize behavior with an actual screen reader. This benefits both
forms and game HUDs; adapter scope remains a design question.

### UP-08: Mutable RGBA textures

This extends the existing image-wrapper gap with local source evidence.
`last_mile/src/Grid.tsx:1061` switches dense maps to a reused RGBA pixel buffer;
lines 1104-1108 upload and scale it with nearest filtering.
`gabeochoa.github.com/sand.html:1066` and `:1082` likewise read/write a pixel
buffer. These are rendering techniques, not simulation rules to upstream.

Render targets already exist. The newer sokol helper
`src/backends/sokol/drawing_helpers.h:1431` uploads pixels into an immutable
image at line 1440; neither library has a common mutable-texture update API.
Provide backend-neutral RGBA8 creation and full-buffer update with explicit
size, buffer-length/stride and filtering contracts. Preserve resource identity
across updates. Subrectangle updates and a general image editor are unnecessary
for the demonstrated uses.

Validate repeated updates to a nonsquare image, transparency, changed edge
pixels, orientation, nearest scaling, invalid lengths and cleanup on raylib
and sokol. Verify visual results before making performance claims; this review
did not benchmark the web or native paths. This is separate from the texture
atlas TODO, since dynamic pixels and static sprite packing solve different needs.

### UP-09: Optional filesystem change notifications

`floatinghotel/src/platform/file_watcher.h:40` owns FSEvents and a background
run loop. Lines 105-140 handle lifecycle races; lines 165-187 preserve rescan
requirements after dropped events. Its fallback at line 209 silently reports
no changes. `floatinghotel/src/ecs/file_watcher_system.h:27` watches roots and lines 48-59
poll events before applying Git-specific refresh policy.

Neither inspected library provides a watcher. An optional watch/replace-roots,
stop and drain-events API could expose changed paths, rescan-required state,
and explicit unsupported/error results. Keep Git filtering, debounce and
refresh decisions in the app. This is a candidate for editors and asset reload,
but only one substantial consumer was confirmed, so establish a second use
before choosing a public interface. The subsequent user decision approves
upstreaming now, with wordproc as a possible consumer to investigate.

Validate multiple roots, create/modify/delete or rescan events, root replacement,
immediate destruction after start, and no callbacks after stop. A portable
polling fallback or an explicit unavailable result must replace silent success.
Concurrent floatinghotel changes were observed; the cited watcher files were
not among those changing during this review.

### UP-10: Consistent in-memory capture format

Puzzle's export and pixel-sampling code needs raw image bytes, as documented in
the existing image-wrapper gap below. The shared
`capture_render_texture_to_memory` operation already exists, but the backends
return different representations. Raylib encodes PNG at
`src/backends/raylib/drawing_helpers.h:565`; sokol forwards a byte buffer at
`src/backends/sokol/drawing_helpers.h:1345`, and its Metal implementation at
`src/backends/sokol/capture_impl.h:224` returns width × height × 4 raw RGBA
bytes. This difference is confirmed by direct implementation reads, not a
runtime reproduction. No affected portable caller was established.

Define a consistent capture result across backends, distinguishing encoded PNG
from owned raw RGBA pixels with dimensions and orientation. Review existing
callers before changing either representation. Validate the same nonsquare,
partly transparent render target on both backends: encoded results decode to
the expected image, raw results have the expected length and pixel positions,
and failed readback returns failure. Pixel inspection, export and visual tests
benefit. Keep this contract correction separate from UP-08's mutable upload API.

### UP-11: Useful default profiling UI

The user requested a shared profiling UI because consumers build their own.
Timing infrastructure already exists: `src/core/system.h:518` defines
`SystemProfileHook`, and `src/plugins/e2e_testing/perf_commands.h:44` supplies
built-in collection. wm's `src/systems/screens/SystemProfileLab.h:17` has a
start/stop demonstration and a text table. The missing product is a useful,
easy-to-enable default panel that apps can customize.

Provide frame-time history, FPS and frame-time percentiles, a sortable list of
expensive systems, and available process CPU/memory counters. Include pause,
resume and reset with clear sampling windows and units. Missing counters must
show as unavailable. The default setup should need no custom data provider for
metrics already collected by the library. Apps can add named counters with
units, select sections and history duration, and use their theme in an embedded
panel or overlay. Reuse the chart set in UP-12.

Keep measurement meanings accurate. The current built-in collector sums elapsed
wall time across calls and puts call counts in `PerfEntry::entity_count`; its
dump labels the totals as averages and counts as entities. The default panel
must distinguish total/mean/per-frame timings and invocation/entity counts.
Elapsed system time must not be labeled CPU utilization or GPU time. Reuse or
extract the existing collection path so normal apps do not need to register
E2E commands merely to show diagnostics. Define how an existing app profiler
can supply data without silently replacing its hooks.

Validate the panel in wm with a repeatable workload and custom counters. Show
that an intentionally slower system appears, history stays bounded, pause
preserves the displayed snapshot, reset has clear behavior, and unavailable
metrics stay honest. Check keyboard/mouse use and resizing, then measure the
panel's enabled and disabled overhead. This is requested scope, not a measured
performance improvement or an implemented feature.

### UP-12: Chart set with an interactive test screen

The user approved the sparkline idea and expanded it to a chart set with a wm
test screen. Proposed initial coverage is line, area, bar, scatter and sparkline
charts, with hover values and changing datasets. Share axes, scales, labels,
legends and theme styling where useful; sparklines can omit the full chart
chrome. The profiling panel is a concrete consumer for time-series charts.

Keep data ownership and application-specific aggregation with callers. The wm
test screen should exercise empty, single-value and constant datasets, negative
values, multiple series, live updates and resizing. Validate finite coordinate
mapping, clipping, readable labels, hover selection, and keyboard access to
interactive controls. The user requested the chart set and test screen; the
initial chart types and interactions are proposed implementation scope.

### Smaller opportunities and their decisions

| Opportunity | Evidence | Why deferred / what would justify extraction |
|---|---|---|
| Sparkline helper, expanded to UP-12 | `scrubdaddy/src/components/StockSparkline.jsx:1`, used by `scrubdaddy/src/components/StockMarket.jsx:44`; independent chart code in `watching-a-movie-a-day-presentation/templates/reveal/js/charts.js:307` and `:443` | Originally deferred. The user subsequently requested a chart set and wm test screen; see UP-12. Existing line drawing remains the rendering foundation. |
| UI activation/navigation feedback | `MyNameChef/src/sound_systems.cpp:18`, `afterhours-template/src/sound_systems.cpp:18`, `kart-afterhours/src/systems/sound_systems.cpp:19` scan listeners to request sounds | Click callbacks and playback already exist; these are closely related copied implementations. Consider a small optional activation/focus callback only if it removes scans and produces exactly one cue for nested controls and keyboard activation. |
| Remainder-preserving periodic timer | `endless-dance-chaos/src/crowd_systems.cpp:153` and `endless-dance-chaos/src/schedule_systems.cpp:152` retain elapsed overshoot; `src/plugins/timer.h:51` and `:74` reset accumulated time | Timers already exist and cooldown semantics differ. A separate periodic helper could return elapsed ticks/remainder with an explicit catch-up policy; do not change every timer's semantics. |

## Open, asked for by other projects

### From floatinghotel's footguns list

Their July list, rechecked against `main`. **All of it has now landed** --
label word-wrap, `Dim::Text` as a measured-width unit, `with_corner_radius`,
row-flex `expand()`, index-based tick iteration (their heap-use-after-free),
the button-inside-a-clickable-row hit priority, the sokol group, and the
absolute-stacking one via `with_overlay`.

### From hanabi's triage

hanabi keeps a 14,000-line gap file and an index ranking their top ten by pain
per line of upstream change. Rechecked against `main`, not their pin
(`428047e`): **all ten are now closed** -- six were already in when first
collected, and the rest landed this cycle. Nothing outstanding from that list.
Worth telling them: they can delete `src/util/atlas_guard.h` (~50 lines) and
`src/ui/focus_visible.h`, and drop three hand-rolled virtualization windows for
the `height_of` overload.

### Components

- **access-key underlines** (wordproc) — per-character decoration, so `&File`
  can underline the F. Needs the renderer to decorate one glyph in a run, which
  the run machinery from the wrap work could carry. Filed as accessibility.

### E2E

- **command handlers are registered per SystemManager** (cartographer) — each
  manager needs `register_all_handlers`. Not silent, as their doc says: the
  runner blocks on an unconsumed command and times out, and the timeout now
  names what stalled and points at an unregistered pack. The boilerplate
  remains. Both automatic fixes were costed and rejected -- a registry of live
  managers needs hand-written move members (`SystemManager` is returned by
  value, wm builds ~92 a run), and lazy first-tick registration inverts the
  documented order.

### Harness

- **`wait` resolution is quantised by the substep batch**
  (endless-dance-chaos, minor) — a `wait` cannot land inside a batch of
  substeps, so its resolution is the batch. They rated it low and documented it
  on their side; keeping `sim_steps` small is the whole workaround.

### Wrappers

- **raw `raylib::` calls with no wrapper** (puzzle gap 18) — they count 79 raw
  sites, but the count overstates it: the top entries are *types*
  (`raylib::Color` x57, `Image` x18, `Vector` x10, `Font`, `Rectangle`,
  `Camera`) and enum constants, which afterhours already aliases and they could
  swap today. Rechecked across `backends/` and `plugins/`, what is genuinely
  unwrapped is: the image pixel API (`ImageDrawRectangle`, `GenImageColor`,
  `ImageFormat`, `GetPixelDataSize`, `GetImageColor`), plus
  `ToggleBorderlessWindowed`, `IsAudioDeviceReady` and `GetKeyPressed`.
  `LoadImageFromTexture` and `ImageFlipVertical` exist inside afterhours but
  are not exposed.
  Only the image group is a coherent abstraction; the rest are one-function
  wrappers in unrelated areas. UP-08 above now supplies local consumer evidence
  from last_mile and sand for mutable RGBA uploads. It narrows one part of this
  request. A follow-up read of the nested puzzle project confirms native
  consumers too: `armchair_coach/puzzle/src/gif_export.cpp:108` reads back,
  flips, resizes and converts frames to RGBA for export;
  `armchair_coach/puzzle/src/systems/node_feed.cpp:531` reads back and samples
  pixels; `armchair_coach/puzzle/src/systems/systems_internal.h:445` generates
  image pixels for a texture. Both libraries already expose
  `capture_render_texture_to_memory`, but its format differs between raylib
  and sokol/Metal, as recorded in UP-10. The narrower missing portable operation
  is owned raw RGBA readback with defined dimensions and orientation. Validate export orientation, alpha, resize and pixel sampling
  before broadening the CPU image API. GIF encoding and node logic stay local.
  (Blend mode is *not* among them -- `set_blend_mode`/`blend_scope` are wrapped
  in all three backends and raylib's reaches `rlSetBlendMode`.)

### Diagnostics

- **no lint for custom colours bypassing the theme** (cartographer) — theme
  usage is easy to skip by accident and nothing catches it in shared UI.

### Core

- **relationship integrity depends on enumerating child types** (puzzle).
  Current `armchair_coach/puzzle/src/systems/canvas_systems.cpp:666` explicitly
  reaps ports, dropdowns, knobs, sliders and attached wires. The earlier orphan
  bug is fixed in that source;
  `armchair_coach/puzzle/src/e2e/e2e_commands.h:1101` provides an orphan assertion. Adding another child type still requires updating the deletion
  list. Consider a declared ownership relationship that makes cleanup follow
  that relationship, with separate treatment for non-owning links and cycles.
  Validate all child types, unrelated-node survival, recycled handles after
  undo/recreation, and unfinished wires before choosing a library contract.
  This is a data-model candidate, not evidence that queries need to run faster.
  The separate cascade-delete plan remains unstarted; this review only updates
  the evidence and does not authorize or implement that plan.

- **EntityQuery allocation and sorting** (MyNameChef) — mostly already done and
  their doc predates it: `run_query` has a `stop_on_first` path that allocates
  nothing, reserves upfront, and sorts only when `orderby` is set with more
  than one result. Their remaining items are the smaller ones -- consolidating
  the tag query methods, and the `permanant_ids` typo, which is fixed. They
  also ask to remove the `OptEntity` wrapper, which is the opposite of the
  direction this repo went.

- **kart's remaining component extractions** — `HasLabels`, `CanWrapAround`,
  `TeamID`, `ManagesAvailableColors`. Arguably game-specific; listed for
  completeness, not recommended.

---

## Known limitations (open, low priority)

- **tab order is allocation order, not tree order** — `process_tabbing` moves
  focus by setting it to `ROOT` and letting the next entity the iteration
  reaches grab it, so Tab follows the entity collection's storage order rather
  than the widget tree. Two widgets side by side tab in whatever order they
  were allocated, which is build order today and need not stay that way; there
  is no tab index and no way for a caller to state the order it wants.
  Order-preserving `cleanup()` (`58e1613`) stops widget retirement from
  reshuffling it, which **hides this rather than fixing it** — the order is
  stable now, but it is still the wrong order to be deriving from. A real fix
  sorts focusables by tree position, with an explicit override.

- **a virtualized list still builds a real div for the rows above the fold** —
  `virtual_list` folds the rows *below* the window straight into
  `content_size` via `HasScrollView::unbuilt_content_size`, but the rows above
  it are a `vlist_skipped_above` div of the right height. It cannot be folded
  the same way because the scroll offset is applied when drawing rather than
  when laying out: with nothing in front of it the first built row lays out at
  the top of the content and is then drawn off screen. Folding it needs
  children offset during the layout pass. One div, so the cost is negligible;
  it is the asymmetry that will confuse the next reader.

- **slider handle 0.75 compression** — the knob's center never quite reaches the
  value position at 100% (cosmetic). `imm_components.h` `slider`. Revisit the
  handle width/position model only if it becomes visible.
- **crowded tab bars still need a smaller font at the call site** — `tab_container`
  content-fit (`expand()` + `min_width = Dim::Text`) removes truncation only when
  the labels *can* fit. A bar with many long labels (e.g. `flight_options`' 9 tabs
  at the default font) still overflows; set a smaller font on the tab_container
  config for those.
- **word-wrap has no hard character break** — with `TextOverflow::Wrap`, a single
  word wider than its box goes on its own line (not split), and wrap requires an
  explicit font size (auto-fit + wrap is ambiguous).

Batch/headless screenshot determinism is a wm-side tooling concern, not an
afterhours gap — tracked in `docs/LAYOUT_AUDIT.md`.

---

## Resolved & upstreamed (merged into afterhours main, pinned `e348efb`)

Six of hanabi's top ten are already in and they do not know it -- their pin is
`428047e`. Worth telling them rather than waiting for the next bump:

- **font atlas exhaustion** (their #1, #351/#350/#352/#353) —
  `fonsSetErrorCallback` is registered and `FONS_ATLAS_FULL` is reported. Their
  fifty-line `src/util/atlas_guard.h` can go.
- **widget retirement** (#115) — the frame stamp and end-of-frame sweep landed
- **GPU pool sizes** (#210) — `AFTERHOURS_SG_{PIPELINE,IMAGE,SAMPLER,BUFFER}_POOL_SIZE`
- **parent containment** (#275) — `assert_within_parents`, which is the
  assertion they asked for: `assert_no_overflow` measured against the viewport
- **measure without building** (#224) — `plugins/ui/measure_config.h`
- **diagnostics** (#192/#161/#113) — landed in `2caf525`

- **`--headless` was parsed and then dropped** (endless-dance-chaos) —
  `RunConfig::display` plus `E2EArgs::display_mode()`, and raylib's `run()`
  skips window setup and teardown when headless. `Config::display` already
  existed and `raylib_init` already honoured it, so backend selection worked
  all along -- the `run()` path was the whole gap, which is why the flag looked
  wired up. Worth 11.7x to them. Sokol is untouched: it has no headless backend
  to select.
- **no fixed timestep** (endless-dance-chaos) — `RunConfig::fixed_dt` and
  `sim_steps`, with `frame_steps()` returning the schedule. The consumer still
  writes the loop, because `RunConfig::frame` takes no dt and calling it N
  times would render N times rather than simulate N times. `time_scale`
  deliberately does not stretch a fixed step; doing so would silently put back
  the frame-rate dependence the fixed step exists to remove.
- **`synthetic_press_delay` was undocumented and load-bearing** (cartographer)
  — an injected press is not readable on the frame it was injected, which
  matches a real keyboard but read as a broken feature twice. Documented at the
  field and at `set_key_down`, with the timing pinned by test.
- **text editing opted into by enumerator name** (hanabi #255) —
  `has_editing_action<InputAction>()` to static_assert on, and a run-once
  warning naming whatever resolved to nothing.
- **the label origin was an unnamed 5px literal** (hanabi #85) — `kTextInset`
  and `text_inset_for()`. Padding on a label with no children still does
  nothing -- measured, `pixels(12)` and `pixels(40)` give the same 75px box --
  but it now says so once per run instead of never.
- **nothing sized a box to its own text** (hanabi #136) —
  `with_fit_content(max_w, font_size)`. `Dim::Text` and `max_width` already did
  most of it; the gap was that it takes four settings that must agree and three
  of four caps the width while silently not wrapping.

- **an ordered `gen_first` sorted the whole list** (MyNameChef) — it scanned
  and then `std::sort`ed to return one entity. Takes the minimum now, except
  where a stateful mod like `take()` sits after the orderby and needs the real
  order.
- **absolute children stacking** (floatinghotel) — `with_overlay(levels)`:
  absolute plus a layer relative to the parent, so a nested overlay clears the
  one it sits in. Opt-in, because absolute is also how underlays are built --
  a blanket default blanked `file_tree` and `islands_trains_settings`.
- **card preset** — `with_card(pad)`: Surface, rounded corners, padding.
- **font sizes off the type scale** — `ValidationConfig::enforce_font_size_tiers`,
  opt-in.

- **sokol headless, wheel injection, GPU sync** (floatinghotel) — all three
  landed. `g_headless_rt` and the headless branches exist; `consume_wheel()`
  does not consume (it flags `wheel_read`, every reader in a frame sees the
  same value, cleared at the next `reset_frame`); `capture_impl.h` does the
  blit-to-resolve. The wheel one keeps a misleading name, which is likely why
  their doc still lists it.
- **virtualization with variable row heights** (hanabi #326/#170/#224) —
  `virtual_list` has a `height_of(index)` overload that binary-searches a
  running total. They hand-rolled the same window three times.
- **mouse delta through the action mapping** (cartographer) — `MouseAxisWithDir`,
  normalised by `MOUSE_DELTA_SCALE`.
- **advance vs ink measure** (hanabi #137) — sokol already returned the advance;
  the raylib odd-one-out, `measure_text_internal`, is deleted.
- **focus ring painted at rest** (hanabi #83) — off until first interaction.
  Their hand-rolled `focus_visible.h` can go.

- **the two floatinghotel blockers** — the sokol include-order break and the
  flex solver budgeting raw child sizes. Both fixed, with a third snapping site
  they had not found; `sokol_include_order_test` compiles `window_manager.h`
  under `SOKOL_METAL`, which nothing did before.
- **tooltip** — `plugins/ui/tooltip.h`, with `TooltipLab` and a baseline
- **table / grid layout** — `plugins/ui/grid.h`; `RaceResults` and
  `MinesweeperLab` converted to it, plus `GridLab`
- **scrollbar colour and style** — track/thumb usages and explicit colour
  overrides on `HasScrollView`, with `ScrollbarStyleLab`
- **singleton-only systems skip the entity scan** — a system whose components
  are all registered singletons resolves through the singleton map instead of
  walking every entity (puzzle profiled that scan at 37% of a frame)
- tab_container tab strip bounds under `with_absolute_position`
- **render-command sort recycled-id tiebreak** (SEVERE — fixed 76/79 screens;
  root cause behind the modals breakage, first-child-missing-control, and
  cross-rebuild screenshot non-determinism)
- progress_bar fill/label percent compounding, and track percent compounding
- slider handle position on percent-sized tracks
- stepper multi-visible label separation
- tab_container equal-width → long-label ellipsize (now `expand()` + `Dim::Text`
  min width; added `Dim::Text` to `resolve_constraint`)
- static-label word-wrap — new `TextOverflow::Wrap` feature
- `children()` sizing now includes `flex_gap`
- checkbox/toggle focus ring — confirmed **handled**, not a bug (checkbox/toggle
  are focus clusters via `FocusClusterRoot`/`InFocusCluster`; the ring render is
  type-agnostic with an explicit cluster branch)
- toggle_switch "sibling entities consume layout space" — **not a bug** (internal
  entities are children, not siblings)

Regression tests for these live in the afterhours `tests/` suite: `autolayout_test`,
`progress_bar_test`, `slider_test`, `stepper_test`, `tab_container_test`,
`text_wrap_test`, `render_order_test`.
