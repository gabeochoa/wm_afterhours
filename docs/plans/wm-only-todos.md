# WM-only todo pass

## Completion checks

Every WM-owned item in `todo.md` gets a current disposition: implemented and verified, already covered by current code/tests, or blocked on a named library capability. Library and other-project changes stay out of this pass. Completed entries leave the active todo list. No vendor changes or pushes.

## Workflow

1. Read the Poteto principles and ground each historical entry against current code.
2. Capture the current screens and UI geometry before editing.
3. Make independently reviewable screen, infrastructure, and research changes. Use isolated worktrees for these three independent groups.
4. Batch C++ edits before compiling, as requested for the shared laptop. Run cheap source/script checks before committing each unit; compile once with `nice -n 10 make -j2`, then run affected screens individually.
5. Compare changed screens and interactive outcomes with captures and assertions. Investigate actual warnings and preserve intentional overflow examples.
6. Record measured performance and distinguish library blockers from WM fixes. Remove only confirmed completed todos.
7. Review the combined diff and evidence, confirm afterhours is unchanged, and remove temporary worktrees and captures.

## Work groups

- Screen behavior: sample/live File Tree, horizontal drag preview control, language layout, mockup copy, Entity Index presentation, closed popovers, current screen-polish reports.
- Infrastructure: generated includes isolated by build output, assertion coverage, deterministic capture settling, startup and CPU/RAM measurements.
- WM rendering and assets: pixel-intended corner radii, remaining real layout warnings, shared artwork atlases where texture loading/draw evidence supports them.
- Design research and mock comparisons: supplied design guidance and sf-windows review, current CSS-versus-native disagreements, scope of typography/animation/plugin proposals.

## Boundaries

New native animation effects, component shaders/masking, chart-library extraction, text preparation, and native control/configuration fixes need afterhours ownership or extension decisions. Preserve those todos for the later library pass instead of making local substitutes.

Broad older requests such as more juice and game interactivity are checked against the current demos and their tests; any concrete remaining WM defects are fixed. A passing build alone does not close visual or behavioral work.

## Disposition of the WM backlog

| Previous item | WM result / check |
|---|---|
| Dannaway articles and vertical rhythm | Supplied articles distilled in `docs/design-review/dannaway-wm.md`; examples use one family and deliberate spacing. External linked design systems were not independently audited. |
| Expressive typography | New `typography_composition` screen; static default, replay and reduced motion. Native curved/rotated/grapheme effects remain deferred. |
| Transparent controls | New `transparent_controls` screen on light/dark/patterned surfaces, observable button state and disabled controls; composited contrast calculations in the design-review note. |
| Chart indicators | Current Chart Lab already has distinct marker shapes, threshold lines and application drawing. Tests 153/176 passed; fresh charts show both marker types and threshold visibility. Native chart options/extraction remain open. |
| Podcast / theme swatches | New `podcast_library` has simulated playback, seeking, speed and episode selection. Existing theme gallery covers swatches. No audio playback is claimed. |
| Pixel-intended roundness | Nine remaining caller files now use explicit radii. Remaining fractions describe circles/pills or the roundness gallery. |
| Artwork atlases | Cafe and image examples each use one padded atlas for five source images. Generator checks source pixels and padding. Follow-through driver measurements found unchanged draw/bind counts; see runtime report. |
| Launch and CPU/RAM | Deferred CJK fonts, shared loading and a bounded measurement script. Fonts remain loaded after first use; headless and verified-vsync windowed measurements are in the runtime report. Controlled cold-start/isolated idle checks remain open. |
| Generated include collision | Includes and objects live under each output/screen variant. Independent build directories no longer overwrite a shared source header. |
| Assertion-free E2E scripts | Inventory distinguishes interactive scripts from capture-only fixtures; seven scripts gained behavioral assertions. New word-selection assertion exposed redundant WM Shift+Arrow mappings, now removed. |
| Headless settling | Screenshot frames/dt configurable, with default 120 frames and optional instant animations. Non-screenshot test modes retain their prior policy. |
| File Tree live data | Current screen already defaults to a stable sample. New test 292 exercises sample/live switching. |
| Navigator categories/coverage | Current Screen Tree already normalizes historical categories and has headless interaction coverage; test 155 passed. |
| Popover initial state | All popovers start closed; test 233 exercises open/dismiss behavior. |
| Horizontal ghost toggle | Explicit Translucent preview on/off control; test 290 drags the same tag to the same rectangle. 6,004 of 6,052 crop pixels change, with lower RGB values in the translucent state. |
| Drag sibling consolidation | Board/Lanes mode switch in `drag_drop`, preserving both data sets and preview preference. Standalone horizontal entry retained for direct tests. |
| Language preview | Compact/wide menu, measured icon placement, bold Japanese font and test 291. |
| Self-align focus | Fresh forward/backward traversal and focus captures show all four complete outlines; no caller patch needed. |
| Old mockup debug copy / Entity Index | Current backup/VPN/media screens already have application copy; Entity Index is the Patch Bay media browser. Existing interaction tests pass. |
| Old config/layout/checkbox/game polish | Replacement screens have current interaction coverage. Fixed actual text-box height warnings in layout, Casual Settings and Empire Tycoon; broad old requests are not treated as unbounded redesign work. |
| Layout warnings in decorators, nine-slice, parcels, cards | Source/capture review separates existing intentional boundary specimens from caller defects. New library behavior is deferred. |
| Layout run-to-run report | Standalone and all-screen native captures are pixel-identical. Historical E2E variation is not reproduced by that comparison; the repeated same-phase E2E capture is also pixel-identical. The historical variation was not reproduced. |
| Mock diagonal offsets / unreachable text | Fixed duplicated CSS absolute margins. Historical missing AIM messages are present; the follow-through adds font metadata and intrinsic text participation to the mock. Fresh browser comparison remains blocked by the unavailable required browser setup. |
| sf-windows | The external source loader rejected the URL under its input-filtering policy. No source audit or adoption is claimed; todo retained. |

## Review findings corrected during verification

The first combined run passed 48 of 50 scripts. The transparency state label queried
current-frame hover before hit testing; it now reads the previous completed frame.
The Language baseline described the old screen and required a reviewed replacement.
Screenshot review also caught defects invisible to text-existence assertions:

- Typography called `with_translate()` after `with_absolute_position()`, assuming
  translation composed with placement. Both setters write the same fields. The WM
  screen now computes one final position; tests assert the words' actual rectangles.
- Language icons were children of styled buttons and inherited their content origin.
  Icons now use the menu's coordinate space, with explicit vertical assertions.
- Font loading skipped prepopulated invalid default aliases in headless mode. Alias
  assignment now replaces those placeholders, verified by fresh focus capture.
- Atlas destruction originally checked only window lifetime, missing live headless
  contexts. Cleanup now recognizes either graphics context.

No afterhours implementation changed. The assumptions above are recorded for the
later library/API review rather than hidden behind screen-specific patches.

## Verification results

- 56 distinct affected E2E scripts passed after corrections. The initial run's two
  failures were fixed and rerun; tests 193/214/291/293/294/296 also passed together.
- All 123 screens captured successfully. The only intentional large-radius
  specimen is in Example Borders; the transparent Separators root was made square
  after the full capture exposed its inherited radius. Its default 120-frame capture
  then passed without validation warnings.
- Internal checks: 7 passed. Five invalid screenshot frame/dt inputs rejected.
- Atlas generator and mock absolute-margin fixture passed. E2E inventory found
  259 scripts with expectations, 31 without interactions, and no interactive
  scripts lacking expectations.
- Self-align audit found four focus targets, complete forward/backward cycles and
  no flagged screen. Repeated Layout E2E captures were pixel-identical.
- A no-change `make -j2` performed no compilation or linking. Builds used nice 10
  and at most two jobs. See the runtime report for measured startup/memory limits.

The local decision log is `output/wm-only-todos/decisions.tsv`. Review captures and
machine-readable results are under `output/wm-only-todos`; temporary worktrees and
unneeded test-generated files are removed after verification. No push was made.

## Follow-through after the incomplete handoff

The original handoff left executable WM work among the next-five suggestions.
That was incomplete scope handling. Commits `49746363` and `b4c87b31` finish the
font-aware mock input work and add WM-only OpenGL/runtime measurements. Afterhours
remains unchanged at `c5cfd359967c6bb165fd40b6a2bc14d5009adc2f`.

The final build passed without warnings. Mock Node checks pass, including hidden
labels retaining intrinsic size and report limitations. Eight fresh native
captures include 325 labels with resolvable font assets. The viewer and hashed
inputs are in `output/wm-followthrough/mock-review/`. The atlas experiment completed
eight runs; the windowed workload and three startup runs passed with verified swap
interval 1. The temporary 261 MiB comparison worktree and failed probe trials were
removed. Reports retain the evidence and reproduction commands.

Remaining WM verification has concrete external prerequisites: a working local
browser for CSS/native comparison, permitted external source access for sf-windows,
and an idle machine for controlled cold-start/isolated idle CPU measurement.
Those are still unchecked in `todo.md`. The font measurement mismatch and the
batching investigation belong in `docs/AFTERHOURS_GAPS.md`; no library implementation
is part of this pass.

## Final WM ownership check

The rolling-number entry was incorrectly grouped with native animation API work.
Its requested demo fits WM's existing custom-draw hook, so it is now implemented
as `rolling_number` in System Demos. `src/rolling_number.h` holds reusable numeric
transition state independently of drawing. It supports six decimal reels for
integers 0 through 999999, directional carry/borrow, bounded jumps and retargeting
from the current fractional positions. It is a counter specimen, not a localized
currency or signed/decimal formatting API.

Changed reels use five weighted vertical glyph samples for motion blur, then draw
sharply at rest. The screen includes blur on/off, pause/resume and reduced motion.
Reduced motion settles immediately, including when enabled while paused. Reset
and increments use the same transition path. Unchanged digits stay sharp. The
screen uses one Atkinson family throughout and existing high-resolution fonts.
No shader, font-layout or animation-library changes are required.

Validation: the standalone numeric test covers carry, borrow, an interrupted
reversal, bounds and negative/large dt. E2E 297 covers pointer/keyboard controls,
reduced motion, pause/resume and 1024px resizing. Captures were reviewed at rest
and mid-transition. Pixel comparison confirmed the digit region is unchanged
when retargeted while paused with blur off. Build emitted no warnings. Run the
numeric check with `nice -n 10 clang++ -std=c++23 tests/rolling_number_test.cpp -o output/rolling_number_test` followed by `nice -n 10 ./output/rolling_number_test`.

The CPU sampler now also measures differences in the app's own cumulative CPU
time during settled idle/active intervals. It excludes startup/audit boundaries
and the sampler's CPU. The runtime report records the actual shared-machine run.
Controlled cold caches and an otherwise idle laptop remain external conditions.

Remaining implementation entries are library work: the theme serializer is in
`ui/theme_io.h`; shared effects, glyph segmentation and component shaders require
native animation/rendering contracts; chart extraction moves existing library
code; prepared-text layout changes native measurement/layout. The rest name
specific native component, validation, input or backend fixes. None were replaced
with WM copies. Browser comparisons and sf-windows remain blocked as previously
recorded. Other repositories remain outside this WM-only pass.
