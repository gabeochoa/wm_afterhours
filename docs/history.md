# Completed work and historical audits

These are past verification results, not a fresh run or a current acceptance checklist.
Original reports remain in Git at WM `072a602a`. Current gaps live in
[AFTERHOURS_GAPS.md](AFTERHOURS_GAPS.md); pending work lives in [todo.md](../todo.md).

## Approved cross-project batch

| Item | Local change | Validation |
|---|---|---|
| UP-01 | afterhours `1fce0d9`, included by wm's submodule pin | Regression failed before the fix with 7/34 checks passing. Afterward all 39 checks pass, including the new master getter. Full afterhours `make -C tests -j2 test` exits 0; no-backend compile check passes. |
| UP-02 | afterhours `325caee` | JSON/raw failure regression failed before and passes after; atomic-file tests 29/29. Raw-only compilation passes. Bitsery 5.2.4 save/load round-trip passes. |
| UP-03 | afterhours `b81173b` | Clipboard tests 15/15, including fresh writes, nested scope restoration after exceptions, reset, and single/multiline widget copy/paste. |
| UP-04 | afterhours `544ea06` | Portable queue checks cover single consumption, Unicode paths, cancellation, errors and reentrant processing. Real macOS open/save/folder cancellation and off-main-thread rejection pass. |
| UP-05 | afterhours `00a6745` | All binding alternatives, explicit modifiers, malformed records and atomic list decoding pass with no backend and raylib. |
| UP-06 | afterhours `bcefe22` | Tests cover full modifiers, axis directions, drift/mouse-motion rejection, held-axis behavior, pinned preference, remapping and layer changes. |
| UP-09 | afterhours `f4de4d3` | Real filesystem tests cover create/modify/rename/delete, Unicode paths, multiple roots, invalid roots, immediate destruction, restart, stop and bounded-buffer rescan. |
| UP-10 | afterhours `6eea5bd` | Raylib/Metal nonsquare translucent captures match decoded PNG byte-for-byte. Legacy PNG, failed captures, Metal blend and HiDPI tests pass. |
| UP-11 | afterhours `5a8e3be` | Bounded recording, runtime controls, independent legacy hooks, compile-out checks and the interactive panel pass. Measurements and limits are in `vendor/afterhours/docs/profiling-measurements.md`. |
| UP-12 | afterhours `dff9c96` | Chart bounds and sample lookup tests, pointer and keyboard controls, hover inspection, live updates and 720p/1080p wm checks pass. |

UP-07/08 were deferred; sound feedback and periodic timers were skipped. Chart scope
was narrowed to profiler needs. The later UP-13/14/15/16 fixes covered queued draws,
config-owned texture references, logical/physical virtual rows and Metal resize:
39 checks per collection mode, 13 texture checks, 16,373 list checks and 1,356 Metal
checks at each of 1×/2×/3×. Consumer apps were not migrated by those tests.

## Focus, modal and popup audit

September audit: WM E2E 280/280, baselines 117/117, 1,091 forward focus targets across
117 screens, and 73 popup cases with 2,636 snapshots. Both traversal directions,
ring crops, containment, opener restoration and dismissal were checked. The full
library suite passed; later affected suites passed separately. Windowed Escape
handling was inspected in code, while input probes ran headlessly.

| Gap | What was wrong and why | Assumption that missed it | Fix |
| --- | --- | --- | --- |
| Focus paint order | A ring followed its own background but preceded opaque children, images, and custom foreground drawing. Dropdowns, text fields, checkbox labels, and poster tiles covered it. | Checking the ring rectangle, or testing a childless button, was enough to prove visibility. | Both renderers paint the ring after the focused subtree, while preserving the order of later overlays. Tests cover opaque children and an overlapping higher layer. |
| Incomplete recording backend | The batched fill primitive was a no-op in the recording backend, so tests could observe a ring without the opaque fill that would cover it. | Testing either renderer through that backend captured equivalent drawing operations. | Record batched rounded/rotated fill calls as well; the new paint-order test requires a recorded child fill, then the ring, then a higher overlay. Actual screenshots remain necessary for pixel appearance. |
| Thin focus targets | A fixed 4px inset produced negative dimensions on 4px split handles and a 1px decorative rule. | Every focusable element was larger than twice the theme inset. | Clamp each axis independently; omit an inner contrast outline when it cannot fit. The decorative rule also explicitly opts out of Tab. Pointer passthrough alone is not a keyboard policy. |
| Concentric corners | The fill's pixel radius was reused after insetting the ring, so their corner centers differed. | A fixed radius was correct for every rectangle associated with the component. | Subtract the inset before deriving the ring radius; retain concentric expansion for its contrast edges. |
| Rotated focus targets | Batched rounded outlines had no rotation field, although fills and text could rotate. | Applying translation and scale to the ring rectangle also covered rotation. | Carry rotation through the outline primitive and rotate around the ring center in both renderers. |
| Scroll extent disagreed between update and render | Neither path included container padding; only the update path included flex gaps. Rendering could clamp a correctly revealed last row back out of view. | Two copies of similar size arithmetic would stay equivalent. | Share the measurement routine, including padding, gaps, margins, and unbuilt virtual-list content. |
| Keyboard scroll visibility | Tab could reach controls below a scroll viewport without revealing them. Forms and the scroll-click fixture visibly lost focus. | Being built and enabled meant the control was visible to the user. | Reveal a changed keyboard/programmatic focus target through its scroll ancestors. An unchanged focus does not undo manual wheel scrolling. |
| Custom modal styling | AIM, media playback/settings, the race pause sheet, and the motorway tutorial used drawn panels instead of the modal plugin. Background controls remained keyboard reachable. | A scrim plus disabled background buttons was equivalent to a modal. Disabled buttons intentionally remain discoverable with Tab. | `ModalConfig::with_panel` accepts normal component styling while retaining the shared modal stack, dismissal, input gate, and focus restoration. The screens use that path. |
| Modal viewport anchoring | A viewport-sized backdrop and centered panel were positioned relative to whichever parent called the modal. An offset/padded parent shifted the backdrop and left part of the window uncovered. | Every caller was the full-screen root at (0, 0). | Attach backdrops and default panels to the layout root; explicitly styled custom panels retain their caller-relative layout. Test an offset, padded parent. |
| Modal transition timing | The input gate was installed by a system on a later frame. Closing a covered modal could also steal focus from the top dialog. | A one-frame delay was harmless, and every closing dialog owned focus. | Synchronize the gate at open/close transitions; only restore focus owned by the closing dialog. Retire omitted modal panels so conditional rendering cannot leave an invisible input gate. |
| Backdrop press ownership | The watcher returned early for an empty modal stack without clearing its remembered press. A later dialog could immediately interpret the opening release as an outside click; a release could also cross between stacked dialogs. | A boolean saying that some modal was active was sufficient to authorize dismissal of whichever modal was on top later. | Bind a press to the modal already present before it began; reset ownership when the stack empties and after release. Tests cover reopening, stacking, opening presses, and a valid later backdrop click. |
| Escape in modal text fields | A text field consumed Escape to blur, then the focus trap put focus back inside. A rename dialog could never close from its input field. | Local blur was the final handler of Escape, independent of an enclosing dialog. | Blur without consuming the enclosing dismissal action. Dropdowns still consume Escape locally, so their open list closes before its dialog. |
| Stale actions | An unconsumed Escape survived indefinitely in `last_action`; a later popup could consume it. Screen resets did not clear it either. | A press was safe to retain until some future component handled it. | Initialize and expire the action at the next UI frame, clear input bits on reset, and deliver synthetic E2E actions through the existing next-frame queue. |
| Popup dismissal | Menus, popovers, and dropdowns closed only when focus moved elsewhere. Clicking blank space did not move focus, and Escape was not handled. | Every outside click would focus another control. | Explicit outside-press and Escape dismissal, with focus restoration when the popup still owns focus. Cozy Cafe now uses the same popover behavior. |
| Pending menu activation | Checking focus-loss dismissal before rebuilding menu rows discarded a click queued by the previous frame. | Closing and selecting were independent, so their order did not matter. | Read pending item selection before applying generic dismissal; cover selection after focus moves and retain real pointer-routing E2Es. |
| E2E cancellation | Skipping a failed script left other pending assertions alive. They timed out against later screens and falsely blamed those scripts. | Moving the script cursor also cancelled already-dispatched work. | Consume pending commands without creating new failures before finalizing a skipped script, and clear its waits. Test cancellation and the following script. |
| Disabled menu options | Tray traversal included unavailable menu actions. | A click listener meant an item was navigable. | Exclude disabled labeled items from tray traversal; ordinary disabled controls retain their existing Tab behavior. |
| Dropdown trigger width | A dropdown copied its percentage width into its trigger, applying the percentage twice. The visible field was narrower than the focus-cluster rectangle. | Reusing a size configuration preserved its resolved size when moved under a new parent. | Size the trigger and optional label against the resolved holder; test both labeled and unlabeled percentage-width controls. |
| Long dropdowns | A twelve-option list exceeded the available space around its trigger, and used unresolved style units to estimate height. | Flipping/clamping the origin could make an arbitrarily tall list fit. | Resolve row height from the trigger and limit the tray to available space, with scrolling and keyboard reveal. |
| Corner-unit setter precedence | Calling `with_roundness(1)` after a base style with `with_corner_radius(0)` silently kept square corners. This occurred again while correcting the motorway rings. | The last explicit builder call would override the base style, as other style setters do. | Each setter clears the other unit; tests verify the rendered radius for both call orders. |
| Custom painted shapes | Some circular or pill-shaped controls advertised square geometry, so a geometrically correct ring still looked wrong. | A custom draw callback communicated its shape to the renderer. | Supply matching corner geometry in the bird, motorway, and Kirby screens. File-tree rows also reserve the ring's full thickness and contrast edge inside their clip. |

The important failed assumption was that valid focus geometry proved visible paint.
Opaque children covered rings; recording-backend fills were missing too. Tests now
check draw order and actual crops. Closing dialogs must track press ownership and
retained parent links, not child vectors temporarily cleared during UI construction.

## September live review

50 E2E scripts, four performance scenarios and 13 focused library suites passed.
Reviewed captures covered 720p and each script's additional sizes. Forms, Modals and
text-stroke baselines were refreshed after review. This did not certify Metal text-stroke pixels.

| Finding | Cause and mistaken assumption | Change and scope |
|---|---|---|
| Virtual List Lab rendering became expensive despite virtualization | The list already builds only visible rows plus overscan. Assuming 10,000 live widgets would target the wrong code. A macOS stack sample instead showed repeated scissor ends flushing Raylib batches through Metal submission. | Coalesce adjacent identical clip scopes without reordering draws. Tests cover equal/different clips, intervening unclipped work and final closure. |
| Dragged cards lost their contents and styling | The overlay copied a few root properties and assumed that represented the card. Nested labels, badges, drawing callbacks and inherited opacity were missing. | Render the source subtree at the preview position while preserving its internal clipping and effective opacity. Restore source geometry afterward; the overlay remains noninteractive. Both renderers have parity tests. |
| Selection highlights and carets were rounded | Internal text-editing decorations inherited the ordinary component radius. A default appropriate for controls is not appropriate for a selection rectangle. | Set selection and caret radii explicitly to zero in single- and multiline inputs. |
| Stepper text acquired an outline | The outer configuration, including its border, was copied into arrow and value children. | Strip borders from internal stepper children while retaining the outer control border. |
| Center/End stacks inserted gaps around 30px children | Final position/cursor snapping still ran after a widget opted out of grid snapping. | Respect parent and child opt-outs during final positioning, including the running layout cursor. Tests assert exact adjacency. |
| Text strokes showed offset copies instead of a continuous outline | Eight displaced text draws approximate only eight directions and leave holes around thin glyphs or large radii. | Expand actual glyph coverage with a Euclidean distance transform; cache outlined textures and preserve advances. Retire cached textures at frame boundaries and clear them before graphics shutdown. Raylib has pixel/metrics tests; Metal uses the same mask operation but requires separate runtime verification. |
| A stepper ignored keyboard changes | The widget reset its index before consuming the previous frame's left/right result. Focus eligibility also excluded widgets with only left/right handlers. Pointer arrows masked both bugs. | Consume pending keyboard state before syncing to the caller, report each change once, and normalize against the current option count. Steppers use configured key-repeat events; continuous sliders still use held input. |
| Reused clipping stayed enabled | Applying visible overflow did not remove `HasClipChildren` installed by an earlier hidden configuration. The Acid test exposed this because its pink probes stayed hidden in the intentional failure mode. | Clear config-owned clipping on a normal rebuild; preserve it for partial restyles. Tests cover both transitions. `HasScrollView` ownership during Scroll→Visible remains a separate open question because callers also install it directly. |
| Toasts could only stack at bottom center | Positioning was embedded in the renderer rather than an explicit placement choice. | Add top/bottom left/right anchors and preserve bottom-center as the default. Stack and entrance directions follow the chosen anchor. |
| Profiler could not inspect a frame interval | Per-system history existed in the collector but snapshots exposed only aggregate statistics. | Copy histories only for a frozen view, select an inclusive range and calculate selected average, peak and last-frame timings. Live snapshots retain the cheaper path. |
| Kirby Options loaded slowly and leaked artwork across visits | Individual textures were loaded by each screen instance without matching ownership/cleanup. | Pack the 22 used images into one atlas and retain it in a resource-owning UI component. This is wm asset ownership work, not a new core atlas API. |
| Dropdown-looking controls, progress focus targets and inactive tabs | Screen styling implied behaviors that those particular implementations did not provide. | Use actual dropdowns, passive progress segments and distinct tab data. These are wm implementation defects. |
| Menus, popups and decorative frames looked inconsistent | Local choices overused accent fills, nested outlines and mismatched radii. Library availability did not establish good defaults for each composition. | Simplify those screen styles while preserving the separate Dialog examples the user accepted. |
| Localized inline key prompts were missing | Treating an icon as fixed before/after a sentence assumes English word order. | Demonstrate translation-owned key placeholders with measured wrapping atoms in English and Korean. This demo does not claim general bidi, Unicode line-breaking or a reusable rich-text API. |

## Transitions Lab second review

The September 21, 2026 feedback is complete in `transitions_lab` (commit `1e1c2a14`). Confetti settles into a pile, the duplicate like burst was replaced, smoke rings fade smallest first, Get Pro shows its gradient, the image placeholder fades in pixel by pixel, the smoky dissolve dissolves, the gooey plus menu and organic shimmer render, 3D tilt increased, and drag and drop plops down, slides into place and fades out. Captures at each animation stage were inspected against the requested behavior.

Get Pro, gooey and organic shimmer were blank only under `make run`. That path draws straight to the window, where `gl_FragCoord` is in retina pixels and the shader regions were in logical pixels, and blur requests were dropped. Both paths now match.

## Default-theme follow-through

Native modal/toast font inheritance, text insets in both renderers, tooltip measurement,
slider geometry, persistent spacing and no-fill reconciliation were fixed. The final
WM selection passed 15/15 E2Es; multiline rendering passed 62/62. Earlier default-theme,
spacing, modal, slider and list suites passed. Nine preexisting designated-initializer
warnings remained in the autolayout test source; the WM build had no warnings.

## Visual acceptance

The September screen pass updated 117 baselines in separate commits. Earlier native
baseline and interaction passes had been overstated as web parity. They prove stability
and asserted behavior, not matching artwork, type, depth, transforms or control states.
The old 26-screen residual comparisons remain historical; fresh paired browser/native
review is still required where tracked in the root todo. Avoid reopening replacement
screens from August counts alone.

## WM-only pass

Typography composition, transparent controls, chart indicators and rolling-number
examples use existing APIs. The rolling counter supports unsigned 0–999999, carry/borrow,
retargeting, pause and reduced motion with a five-tap blur. E2E 297 passed. This is a
WM specimen, not native cross-backend animation support.

The screen browser, lower-priority launch, lazy fonts, atlas resources, source viewer,
font-aware mock model and measurement tools are implemented. Controlled cold launch,
idle-machine CPU, live CSS comparison and sf-windows review remain blocked or pending.
[Performance](performance.md) records measured results and their limits.

## Older reports retired

The February coroutine/snapshot failures and August screen/a11y audits described older
code. Their API inventories, numerical scores, task-by-task patches and duplicate source
copies were removed. Keep their practical lessons: inspect pixels, distinguish caller
constraints from solver errors, test both update and render paths, and verify negative
cases. No old report is evidence that a current feature is missing.
