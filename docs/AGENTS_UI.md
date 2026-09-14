# Build and test

Use the headless CLI and E2E scripts for native checks. Do not use the old MCP
capture workflow. Run builds/tests at `nice -n 10`, with at most two build jobs
on the shared laptop. Batch edits before rebuilding, then test affected screens
one at a time. Keep each screen change in its own commit.

```sh
nice -n 10 make -j2
nice -n 10 make test
nice -n 10 make test-layout
nice -n 10 make validate-screenshots
nice -n 10 ./output/ui_tester.exe --test-script tests/e2e_scripts/101_layout_patterns.e2e --headless --quiet
nice -n 10 ./output/ui_tester.exe --layout-summary layout_patterns
```

`make test-visible` opens a window for debugging. `make run-all-tests` runs the
remaining FontConfig coroutine tests; new UI tests belong in E2E scripts.
`make ci` combines the project checks. Headless batches reuse one process to
avoid accumulating WindowServer resources.

## Screen changes

1. Add stable `with_debug_name(...)` names to controls under test.
2. Start scripts with `goto_screen <name>` and `wait 0.5`. Assert geometry with
   `assert_ui` / `assert_ui_text`, and behavior through observable state.
3. Exercise pointer and keyboard input, disabled/empty states, and resizing.
4. Capture and inspect the rendered result. Use labeled contact sheets for
   sequences, then full-size crops for uncertain details.
5. Use `validate_screen` for existing baselines. Update only reviewed baselines;
   a passing native baseline does not establish parity with `mocks.html`.

Use compact assertion failures and layout summaries first. Reserve `dump_ui`
for focused regressions. `--time-scale` can shorten E2E waits; capture animation
keyframes rather than dumping every frame. Remove only your temporary scripts,
images and audit directories after review.

## Focus and popups

```sh
python3 scripts/audit_focus.py --output output/focus-results.json
python3 scripts/audit_popups.py --output output/popup-results.json
```

These scripts run the app with `nice` and clean temporary screenshots. Focus
supports `--screen NAME`; popups support `--capture-dir PATH` and `--existing PATH`.
For retained focus crops, run the app with `--focus-test --focus-audit --max-tabs 512
--image-output PATH`, then analyze with `audit_focus.py --existing PATH`.

Check ring paint order as well as rectangle bounds. Check Tab/Shift+Tab cycles,
clipped targets, modal containment, Escape/outside dismissal and opener restoration.
Keep screen capture state independent of preceding tests.

## Interpreting failures

Account for requested margins, gaps, negative offsets, strict sizing and scroll
clipping before reporting a solver defect. Synthetic key presses become visible
on a later frame; `wait` advances simulation batches. Prove a new assertion fails
on the broken behavior. A visible-text registry or geometry dump alone cannot
prove that pixels were drawn.

[Mock comparison limits](../mocks/README.md) explain which CSS differences are
independent evidence. [Gaps](AFTERHOURS_GAPS.md) track library blockers.
