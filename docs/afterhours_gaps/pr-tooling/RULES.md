# Rules for afterhours gap agents (read fully)

Repo: /Users/gabeochoa/p/afterhours (header-only C++20 game library, ~20 projects use it; backends raylib, sokol (Metal on macOS, Vulkan on Windows), none).
Gap sources: /Users/gabeochoa/p/wm_afterhours/docs/afterhours_gaps/PRIORITIZED.md (ranked list; your gaps are named there) and the per-project source docs in that same dir (+ /Users/gabeochoa/p/wm_afterhours/docs/AFTERHOURS_GAPS.md). Grep them for each gap to see what each project actually needed and the workaround they wrote. Check current afterhours first: if a gap is already solved, report it as "already fixed: <symbol>" and move on.

Per gap:
- One branch `gap/<short-name>` off origin/main, in its own worktree: `git -C /Users/gabeochoa/p/afterhours worktree add /tmp/ah-gap-<short-name> -b gap/<short-name> origin/main`. Work only in your own worktrees. Never touch /Users/gabeochoa/p/afterhours's checkout, any vendor/afterhours submodule, or other agents' worktrees. NEVER push; I push and open PRs.
- If a gap genuinely builds on another of your gaps, branch off that branch instead and say so in the PR body's first line: `**Depends on gap/<other>** (merge that first).`
- Smallest change that solves the projects' real need. No speculative extras. Follow CORRECT.md / CORRECT-RULES.md / PLUGIN_API.md. Run check_plugin_boundaries.sh if you touch plugins.
- No new code comments at all. No trailing-underscore member names. Fail loud: log_error + crash on broken assumptions; log_warn only when recoverable.
- Tests: a focused test in tests/ following existing patterns, registered in tests/Makefile like the others; show it can fail (revert the src change once and confirm the test fails). Also run existing tests that cover the code you touched.
- Example: Gabe wants every PR to include a runnable usage example. Add examples/catalog/<category>/<name>/main.cpp + makefile (leaf pattern: set NAME/EXTRA_FLAGS/LDFLAGS then `include ../../common.mk`; use `-DAFTER_HOURS_USE_NONE` unless the feature needs raylib, see examples/catalog/utility/cpu_image for a headless raylib one; run.sh auto-discovers dirs). Short, reads like documentation, real calls, builds with no warnings from your code, runs headless to exit 0. For docs-only gaps the example is a compiling program demonstrating the documented contract.
- CHANGELOG.md entry matching existing style.
- NEVER use `git stash`: the stash is shared across all afterhours worktrees and agents have swapped stashes. Use cp backups or a WIP commit on your own branch.
- Commits: feature commit(s) then `examples: <name> shows <feature>`. Message style like history (`area: summary`), ~10 lines max, says why.
- Build/run ONLY your own test binaries and examples, always `nice -n 10 make -j1 ...`. Never run the full `make test` suite (other agents compile concurrently; suites collide on shared temp paths).
- Write the PR body to /tmp/prs/gap-<short-name>.md in this shape (see /tmp/prs/jobs.md for a finished one): one-line summary; **Why:** (which projects, Tier); **API** or **What changed** bullets; **Example:** path + `cd ... && make`; **Tests:** what ran, results, the can-fail check. Plain writing, no filler.
- After EACH branch is ready, SendMessage the lead (to: "team-lead") one line: `ready: gap/<short-name> (base: main|gap/<x>) body: /tmp/prs/gap-<short-name>.md`. Then continue to your next gap.
- If a gap needs a product/API decision from Gabe or is too large to do well in one small PR, don't guess: write /tmp/prs/decide-<short-name>.md (problem, 2-3 options, your recommendation, rough size) and message the lead `decide: <short-name> /tmp/prs/decide-<short-name>.md`. Then move on.
- Final reply when all your gaps are done: one line per gap: branch | ready/decide/already-fixed. Nothing else.
