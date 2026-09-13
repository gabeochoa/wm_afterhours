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
