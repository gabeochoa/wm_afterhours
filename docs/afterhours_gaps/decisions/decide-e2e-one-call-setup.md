# decide: one-call E2E setup (pong-afterhours)

## Problem
On main, an app wires e2e by hand: `parse_e2e_args`, `configure_runner`, register the handlers, tick the runner each frame, take screenshots after render, and turn `has_failed()` into an exit code. The one-call version already exists, but only on Gabe's unmerged `origin/pong-gaps` branch: `E2EApp` (7581934), `E2EApp::set_seed` (c93b178) and `E2EApp::attach(cfg, argc, argv, ...)` (d898e7c), which wraps `graphics::run`. pumpkin-beachball and pong vendor that branch and use it.

I tried cherry-picking the three E2EApp commits onto main. `app.h` needs `graphics::canvas()` from `c39f0d7` (`RunConfig::canvas_width/height`), and `attach` relies on the `run()` changes in `65ce69b`. Those in turn sit on the none-backend work (`ab04b63`, `ad36f4a`, ...). So it's not a small port; it means landing most of pong-gaps.

## Options
A. **Merge (or rebase and PR) `pong-gaps` as is.** It has 17 commits: E2EApp, hold_key, expect, register_command, the none platform backend, canvas, rect::bounds and more. Several of my open PRs (#98 register_command/expect, gap/e2e-hold-key) carry the same commits or signatures, so merging pong-gaps first and rebasing those is cleanest.
B. **Split pong-gaps into themed PRs:** e2e (E2EApp, hold_key, expect, register_command), the none backend, and canvas/run. That's reviewable, and it follows the keep-PRs-small rule.
C. **Write a smaller `testing::run_e2e(...)` on main** that doesn't depend on canvas or the none backend. That duplicates E2EApp and conflicts with what two projects already use.

## Recommendation
B. pong-gaps is already the answer for this gap and for parts of #98 and gap/e2e-hold-key. Splitting it lets the e2e piece land without the backend work. I didn't open a PR, because which commits go into which split is Gabe's call on his own branch.

## Rough size
B, e2e split: E2EApp needs canvas, so that split is about 6 commits once c39f0d7/65ce69b come along. None-backend split: about 6 commits.
