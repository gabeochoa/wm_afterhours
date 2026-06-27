# Layout QA Pipeline Design

**Date:** 2026-02-22
**Status:** Approved

## Goal

Automated visual QA pipeline that captures screenshots of every screen (idle + interaction states), then fans them out to multi-audit with a layout-specific focus: corners, container bounds, overflow, and overlap.

Must be portable across UI projects and support both Cursor subagents and Claude CLI as audit backends.

## Architecture

Two Python scripts + one Cursor skill wrapper:

```
collect_qa_screenshots.py    Phase 1: capture & organize screenshots
run_layout_audit.py          Phase 2: fan out to AI auditors
.cursor/skills/layout-qa/    Thin Cursor skill wrapper
```

---

## Phase 1: `collect_qa_screenshots.py`

### What it does

1. **Discover screens** -- runs `{executable} --list-screens` to get the canonical screen list
2. **Capture idle screenshots** -- runs `screenshot_all_screens.py --quick` for 720p idle shots of every screen
3. **Build screen-to-e2e map** -- parses all `.e2e` files for `goto_screen` and `screenshot` commands
4. **Run e2e tests** -- for each screen with matching e2e tests, runs them and collects interaction screenshots
5. **Organize** -- copies all screenshots into per-screen directories
6. **Write manifest** -- outputs `manifest.json` mapping screen names to screenshot paths

### Discovery (project-agnostic)

| What | Discovery order |
|------|----------------|
| Executable | `--exe` arg > `./output/ui_tester.exe` > any `.exe` in `./output/` or `./build/` |
| E2E tests | `--e2e-dir` arg > recursive `*.e2e` search from project root |
| E2E runner | `--runner` arg > `scripts/run_e2e.sh` > direct `{exe} --test-script` |
| Screens | `{exe} --list-screens` (always dynamic) |

### Screen-to-E2E mapping

Parses each `.e2e` file for `goto_screen {name}` lines. A test maps to a screen if it contains `goto_screen {screen_name}`. Tests that visit multiple screens map to all of them, but screenshots are attributed to the screen that was active when `screenshot` was called.

For screens with no matching e2e test, only the idle screenshot is collected.

### Output structure

```
/tmp/ui_qa_audit/
  manifest.json
  forms/
    idle_720p.png
    responsive_forms_720p.png
    forms_scroll_end.png
  advanced_modals/
    idle_720p.png
    adv_modals_login_open.png
    adv_modals_wizard_step1.png
    adv_modals_settings_open.png
    adv_modals_feedback_open.png
  accessibility/
    idle_720p.png
  ...
```

### CLI

```bash
# Auto-discover everything in current project
python3 collect_qa_screenshots.py

# Explicit paths for a different project
python3 collect_qa_screenshots.py --exe ./build/my_game.exe --e2e-dir ./testing/scripts/

# Single screen
python3 collect_qa_screenshots.py --screen forms
```

---

## Phase 2: `run_layout_audit.py`

### What it does

1. Reads `manifest.json` from Phase 1
2. Splits screens into N batches (default 10)
3. Spawns N parallel audit processes, one per batch
4. Each process sends screenshots + focus prompt to the AI backend
5. Collects per-screen findings into `docs/layout_qa/{screen}_layout_qa.md`
6. Consolidates into `docs/layout_qa/consolidated_layout_qa.md`

### Backends

| Backend | Flag | How it works |
|---------|------|-------------|
| Claude CLI | `--backend claude` | Spawns `claude` processes with `--image` flags |
| Cursor | `--backend cursor` | Writes prompt files for Cursor subagents |
| Manual | `--backend manual` | Writes prompts + paths for manual paste |

### Audit focus prompt

```
Focus exclusively on LAYOUT correctness. For each screenshot:

1. CORNERS: Are container corners clean? No content bleeding past rounded
   corners or border edges.
2. CONTAINER BOUNDS: Are containers sized correctly? No unexpected expansion
   pushing elements off-screen.
3. OVERFLOW: Does any content overflow its parent container? Check text,
   images, and child elements.
4. OVERLAP: Are any elements overlapping unintentionally? Check for z-order
   issues and misaligned siblings.

Do NOT flag color, contrast, font choice, or accessibility concerns.
Only report layout/spatial issues.

For each issue found, provide:
- Type (corners | container_bounds | overflow | overlap)
- Which screenshot shows it
- Specific location in the screenshot
- Suggested fix
```

The focus is overridable via `--focus "custom prompt text"` for other audit types.

### Output format

**Per-screen** (`docs/layout_qa/{screen}_layout_qa.md`):

```markdown
# Layout QA: {screen_name}

**Screenshots analyzed:** N

## Issues Found

### 1. {Title}
**Type:** overflow | overlap | corners | container_bounds
**Screenshot:** {filename}
**Detail:** {Specific observation}
**Suggested fix:** {Actionable recommendation}
```

**Consolidated** (`docs/layout_qa/consolidated_layout_qa.md`):

```markdown
# Layout QA Consolidated Report

| Type              | Screens affected | Severity |
|-------------------|-----------------|----------|
| Overflow          | N               | High     |
| Corner bleed      | N               | Medium   |
| Overlap           | N               | Medium   |
| Container expand  | N               | Low      |

## Overflow Issues (N screens)
### {screen}: {title}
...
```

### CLI

```bash
# Full pipeline
python3 collect_qa_screenshots.py
python3 run_layout_audit.py --backend claude --parallel 10

# Single screen
python3 run_layout_audit.py --backend claude --screen forms

# Custom focus
python3 run_layout_audit.py --backend claude --focus "Check only text overflow and truncation"
```

---

## Cursor Skill: `/layout-qa`

Thin wrapper that calls both scripts in sequence:

```
/layout-qa                    # Full audit, auto-detect backend
/layout-qa:screen forms       # Single screen
/layout-qa:collect            # Phase 1 only (just collect screenshots)
/layout-qa:audit              # Phase 2 only (manifest must exist)
```

Defaults to `--backend cursor` when run inside Cursor.

---

## Batching strategy

With ~76 screens and 10 agents:

| Batch | Screens (alphabetical) |
|-------|----------------------|
| 1 | accessibility through buttons (~8) |
| 2 | cards through colors (~8) |
| ... | ... |
| 10 | toggle_switches through vstack_showcase (~6) |

Alphabetical split is deterministic and reproducible.

---

## Future: E2E framework integration

Move screenshot collection into the e2e framework itself so other games get it for free:

| New e2e command | What it does |
|----------------|-------------|
| `screenshot_all` | Visit every screen and screenshot it |
| `assert_no_overlap` | Check sibling elements don't intersect |
| `assert_corners_clean` | Check no child bleeds past parent border-radius |
| `assert_contained` | Check all children fit within parent bounds |

With these, a single `layout_audit.e2e` script replaces `collect_qa_screenshots.py`:

```e2e
screenshot_all
assert_no_overflow
assert_no_overlap
assert_corners_clean
```

This is a separate effort tracked independently.

---

## Rejected alternatives

**Pure Cursor skill (no Python):** Too token-heavy for a single session, harder to debug, not usable from terminal/CI.

**E2E-only approach:** Would require significant e2e framework changes before being usable. Python bridge is faster to ship now.

**Per-screen subagents (76 agents):** Too many parallel processes. Batching into 10 is a better balance of parallelism vs. resource usage.
