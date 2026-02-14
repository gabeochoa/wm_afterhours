# Headless Screenshot Testing & CI Validation

**Status:** Mostly implemented  
**Priority:** High

---

## What Already Exists

The building blocks are in place but aren't wired together end-to-end:

| Piece | Status | Location |
|-------|--------|----------|
| Headless rendering (no window) | Done | `graphics/raylib/raylib_headless.h`, `DisplayMode::Headless` |
| Screenshot capture from render texture | Done | `screenshot_validation.cpp` → `save_screenshot_to()` |
| Pixel-diff comparison (percentage) | Done | `screenshot_validation.cpp` → `calculate_image_diff_percentage()` |
| Baseline comparison with 1% threshold | Done | `screenshot_validation.cpp` → `validate_screen_against_baseline()` |
| `--update-baselines` flag | Done | `e2e_integration.h` → `E2EArgs::update_baselines` |
| Headless screenshot generation for all screens | Done | `headless_screenshots.cpp`, `make screenshots` |
| Coroutine-based test framework (`TestApp`) | Done | `testing/test_app.h` |
| Script-based E2E runner (`E2ERunner`) | Done | `e2e_testing/runner.h` |
| `--headless` flag for E2E | Done | `main.cpp` line 213 |
| Snapshot capture + UI state JSON | Done | `testing/test_snapshot.h/cpp` |
| Committed baseline directory | **Done** | `screenshot-baselines/screens/` (70 screens at 720p) |
| Baseline comparison script | **Done** | `scripts/compare_baselines.py` — PIL diff with per-screen overrides |
| Makefile targets | **Done** | `make update-baselines`, `make validate-screenshots`, `make ci` |
| `.gitattributes` for PNG binary | **Done** | `.gitattributes` |
| Run all `TestApp` tests headlessly | **Missing** | — |
| Pre-commit hook | **Missing** | — |
| Diff artifact output on failure | **Done** | `scripts/compare_baselines.py --save-diffs` writes to `test-failures/` |

## Goal

1. All tests run headlessly with zero human interaction
2. A `screenshot-baselines/` directory is committed to the repo with golden images
3. A single command validates the current build against baselines
4. A pre-commit hook (or CI step) blocks merging if any screenshot drifts beyond threshold

---

## 1. Committed Baseline Directory

### Structure

```
screenshot-baselines/
├── screens/                    # One PNG per showcase screen (from --headless-screenshots)
│   ├── simple_button.png
│   ├── modal_showcase.png
│   ├── checkbox_showcase.png
│   └── ...
├── tests/                      # One PNG per TestApp snapshot
│   ├── tabbing_forward.png
│   ├── button_click_initial.png
│   └── ...
└── manifest.json               # Metadata: resolution, tolerance, git hash that generated them
```

### Manifest

```json
{
  "resolution": { "width": 1280, "height": 720 },
  "tolerance_percent": 1.0,
  "generated_at": "2026-02-07T12:00:00Z",
  "generated_by": "make update-baselines",
  "git_hash": "abc1234",
  "screen_count": 52,
  "test_count": 12
}
```

### .gitattributes

Mark PNGs as binary / LFS-tracked so diffs are manageable:

```gitattributes
screenshot-baselines/**/*.png binary
# Or if using Git LFS:
# screenshot-baselines/**/*.png filter=lfs diff=lfs merge=lfs -text
```

---

## 2. Headless Test Runner for TestApp Tests

Currently `--run-test <name>` requires a window. The coroutine-based `TestApp` tests need a headless path similar to the E2E script runner.

### New CLI flags

```
--run-all-tests              Run ALL registered TestApp tests
--run-all-tests --headless   Run all tests headlessly (no window)
--run-test <name> --headless Run a single test headlessly
```

### Implementation sketch

```cpp
int run_all_tests_headless() {
  // 1. Init headless graphics
  afterhours::graphics::Config cfg;
  cfg.display = afterhours::graphics::DisplayMode::Headless;
  cfg.width = 1280;
  cfg.height = 720;
  cfg.title = "Headless Tests";
  cfg.target_fps = 60;
  afterhours::graphics::init(cfg);

  // 2. Set up ECS singletons (same as headless_screenshots.cpp)
  setup_ecs_singletons();

  // 3. Run each registered test
  TestRegistry &registry = TestRegistry::get();
  int passed = 0, failed = 0;

  for (const auto &[name, factory] : registry.tests) {
    // Reset state between tests
    reset_test_state();

    // Create screen + systems for the test's target screen
    auto systems = create_test_systems(name);

    // Create coroutine
    TestApp test = factory();

    // Tick until done or timeout
    int frame = 0;
    constexpr int MAX_FRAMES = 600; // 10 seconds at 60fps
    while (!test.is_done() && frame < MAX_FRAMES) {
      auto &entities = EntityHelper::get_entities_for_mod();
      systems.tick_all(entities, 0.016f);
      systems.render(entities, 0.016f);
      EntityHelper::cleanup();

      if (test.handle.promise().should_continue()) {
        test.resume();
      }
      frame++;
    }

    if (test.is_done() && test.get_error().empty()) {
      std::cout << "[PASS] " << name << " (" << frame << " frames)\n";
      passed++;
    } else {
      std::string err = test.is_done() ? test.get_error() : "TIMEOUT";
      std::cout << "[FAIL] " << name << ": " << err << "\n";
      failed++;
    }
  }

  afterhours::graphics::shutdown();

  std::cout << "\n" << passed << " passed, " << failed << " failed\n";
  return failed > 0 ? 1 : 0;
}
```

### Snapshot mode during headless tests

When running headlessly, `TestApp::capture_snapshot()` should save to `screenshot-baselines/tests/` (update mode) or compare against it (validate mode):

```cpp
static test_snapshot::SnapshotResult capture_snapshot(const std::string &name) {
  if (screenshot_validation::is_update_baselines()) {
    // Save to baselines directory
    std::string path = "screenshot-baselines/tests/" + name + ".png";
    screenshot_validation::save_screenshot_to(path);
    return {.success = true, .snapshot_path = path};
  } else {
    // Compare against baseline
    std::string baseline = "screenshot-baselines/tests/" + name + ".png";
    std::string current = "/tmp/test_current_" + name + ".png";
    screenshot_validation::save_screenshot_to(current);
    float diff = screenshot_validation::calculate_image_diff_percentage(baseline, current);
    if (diff > 1.0f) {
      // Save diff image for debugging
      std::string diff_path = "test-failures/" + name + "_diff.png";
      // ... generate diff image ...
      return {.success = false,
              .error_message = fmt::format("{}% pixel diff (threshold 1%)", diff),
              .diff_path = diff_path};
    }
    return {.success = true};
  }
}
```

---

## 3. Makefile Targets

```makefile
# Generate/update ALL baselines (screens + tests)
update-baselines: $(MAIN_EXE)
	@echo "Updating screen baselines..."
	./$(MAIN_EXE) --headless-screenshots --image-output screenshot-baselines/screens/
	@echo "Updating test baselines..."
	./$(MAIN_EXE) --run-all-tests --headless --update-baselines
	@echo "Generating manifest..."
	@python3 scripts/generate_baseline_manifest.py
	@echo "Baselines updated. Review changes with: git diff screenshot-baselines/"

# Validate current build against committed baselines (CI / pre-commit)
validate-screenshots: $(MAIN_EXE)
	@echo "Validating screen screenshots..."
	./$(MAIN_EXE) --headless-screenshots --image-output /tmp/screenshot-validate/
	@python3 scripts/compare_baselines.py screenshot-baselines/screens/ /tmp/screenshot-validate/
	@echo "Validating test screenshots..."
	./$(MAIN_EXE) --run-all-tests --headless
	@echo "All screenshots match baselines."

# Quick check: only run tests, skip full screen re-render
validate-tests: $(MAIN_EXE)
	./$(MAIN_EXE) --run-all-tests --headless

# CI target: build + validate
ci: $(MAIN_EXE) validate-screenshots
	@echo "CI passed."

.PHONY: update-baselines validate-screenshots validate-tests ci
```

---

## 4. Comparison Script

A small Python script to compare directories of PNGs and report failures with a summary.

**File:** `scripts/compare_baselines.py`

```python
#!/usr/bin/env python3
"""Compare current screenshots against committed baselines."""
import sys, os, subprocess
from pathlib import Path

def compare_images(baseline: Path, current: Path, threshold: float = 1.0) -> tuple[bool, float]:
    """Compare two PNGs using ImageMagick or Pillow. Returns (passed, diff_pct)."""
    try:
        from PIL import Image
        import numpy as np
        img1 = np.array(Image.open(baseline).convert("RGB"))
        img2 = np.array(Image.open(current).convert("RGB"))
        if img1.shape != img2.shape:
            return False, 100.0
        diff = np.abs(img1.astype(float) - img2.astype(float))
        diff_pct = (diff.sum() / (img1.size * 255)) * 100
        return diff_pct <= threshold, diff_pct
    except ImportError:
        # Fallback: use the C++ binary's built-in comparison
        # or just check file hashes
        import hashlib
        h1 = hashlib.md5(baseline.read_bytes()).hexdigest()
        h2 = hashlib.md5(current.read_bytes()).hexdigest()
        return h1 == h2, 0.0 if h1 == h2 else 100.0

def main():
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <baseline_dir> <current_dir> [threshold]")
        sys.exit(1)

    baseline_dir = Path(sys.argv[1])
    current_dir = Path(sys.argv[2])
    threshold = float(sys.argv[3]) if len(sys.argv) > 3 else 1.0

    baselines = sorted(baseline_dir.glob("*.png"))
    if not baselines:
        print(f"No baselines found in {baseline_dir}")
        sys.exit(1)

    passed, failed, missing = 0, 0, 0
    failures = []

    for baseline in baselines:
        current = current_dir / baseline.name
        if not current.exists():
            print(f"  MISSING  {baseline.name}")
            missing += 1
            failures.append((baseline.name, "missing"))
            continue

        ok, diff_pct = compare_images(baseline, current, threshold)
        if ok:
            print(f"  PASS     {baseline.name} ({diff_pct:.4f}%)")
            passed += 1
        else:
            print(f"  FAIL     {baseline.name} ({diff_pct:.4f}% > {threshold}%)")
            failed += 1
            failures.append((baseline.name, f"{diff_pct:.4f}%"))

    # Check for new screenshots not in baselines
    current_names = {p.name for p in current_dir.glob("*.png")}
    baseline_names = {p.name for p in baselines}
    new_screens = current_names - baseline_names
    if new_screens:
        print(f"\n  NEW (no baseline): {', '.join(sorted(new_screens))}")
        print(f"  Run 'make update-baselines' to add them.")

    print(f"\n{passed} passed, {failed} failed, {missing} missing")
    if failures:
        print("\nFailed screenshots:")
        for name, reason in failures:
            print(f"  - {name}: {reason}")
        sys.exit(1)

if __name__ == "__main__":
    main()
```

---

## 5. Pre-Commit Hook

**File:** `scripts/pre-commit-screenshots`

```bash
#!/bin/bash
set -e

# Only run if source files changed (skip doc-only commits)
CHANGED_SRC=$(git diff --cached --name-only --diff-filter=ACMR | grep -E '\.(cpp|h|hpp)$' || true)
if [ -z "$CHANGED_SRC" ]; then
  echo "[screenshots] No source changes, skipping validation."
  exit 0
fi

echo "[screenshots] Source files changed, validating screenshots..."

# Build if needed
make -q output/ui_tester.exe 2>/dev/null || make

# Run headless validation
if ! make validate-screenshots; then
  echo ""
  echo "=========================================="
  echo "  Screenshot validation FAILED!"
  echo "=========================================="
  echo ""
  echo "  Screenshots have drifted from baselines."
  echo ""
  echo "  If this is intentional, update baselines:"
  echo "    make update-baselines"
  echo "    git add screenshot-baselines/"
  echo ""
  echo "  Then commit again."
  echo ""
  exit 1
fi

echo "[screenshots] All screenshots match baselines."
```

### Installation

```bash
# Install the hook
cp scripts/pre-commit-screenshots .git/hooks/pre-commit
chmod +x .git/hooks/pre-commit

# Or append to existing pre-commit hook
echo 'scripts/pre-commit-screenshots' >> .git/hooks/pre-commit
```

---

## 6. Failure Artifacts

When a screenshot doesn't match, save artifacts for debugging:

```
test-failures/
├── simple_button_baseline.png     # What we expected
├── simple_button_current.png      # What we got
├── simple_button_diff.png         # Red pixels = differences
└── summary.json                   # Machine-readable failure list
```

The diff image is already generated by `test_snapshot.cpp` (line 133–161). Just need to route it to the `test-failures/` directory and include the baseline + current side-by-side.

### Diff summary JSON

```json
{
  "failures": [
    {
      "name": "simple_button",
      "diff_percent": 3.45,
      "threshold": 1.0,
      "baseline": "screenshot-baselines/screens/simple_button.png",
      "current": "test-failures/simple_button_current.png",
      "diff": "test-failures/simple_button_diff.png"
    }
  ],
  "passed": 51,
  "failed": 1,
  "total": 52
}
```

---

## 7. Tolerance Tuning

Some screens will have minor rendering differences across platforms (font hinting, anti-aliasing). Support per-screen tolerance overrides:

```json
// screenshot-baselines/manifest.json
{
  "default_tolerance": 1.0,
  "overrides": {
    "language_demo": 3.0,
    "text_input_demo": 2.0
  }
}
```

The comparison script reads these overrides and applies them per-screen.

---

## Implementation Order

1. **Add `--run-all-tests --headless`** — Wire TestApp tests into headless runner
2. **Route snapshots to `screenshot-baselines/`** — Update `capture_snapshot()` and `validate_screen_against_baseline()` to use committed baseline paths
3. **Add `update-baselines` / `validate-screenshots` makefile targets**
4. **Generate initial baselines** — Run `make update-baselines`, commit the `screenshot-baselines/` directory
5. **Add `scripts/compare_baselines.py`**
6. **Add pre-commit hook**
7. **Add `.gitattributes` for PNG binary tracking**
8. **Add `test-failures/` to `.gitignore`**

---

## Example Screen: HeadlessTestShowcase

**File:** `src/systems/screens/HeadlessTestShowcase.h`
**CLI:** `--screen=headless_test`
**Category:** Testing

### Layout

A screen specifically designed to validate that headless rendering matches windowed rendering:

1. **Pixel-Perfect Panel** — A grid of colored squares (red, green, blue, white, black) at exact pixel positions. Any rendering difference will show up immediately in a diff.

2. **Font Rendering Check** — Text at sizes 12, 16, 20, 24, 32px in the project's default font. Catches font hinting / atlas differences between headless and windowed.

3. **Layout Stress Test** — Nested flex containers with percent sizing, absolute positioning, and z-index overlap. Validates that layout math is identical in both modes.

4. **Theme Color Swatches** — Every `Theme::Usage` color as a labeled swatch. Catches any theme initialization differences.

### E2E Test Plan

**Test file:** `src/testing/tests/HeadlessValidationTest.h`

#### Screenshots

1. `headless_pixel_grid` — colored squares at exact positions
2. `headless_font_sizes` — text at all sizes
3. `headless_layout_stress` — nested flex layout
4. `headless_theme_swatches` — all theme colors

#### Test Script

```cpp
TEST(headless_rendering_matches) {
  co_await TestApp::wait_for_frames(5);

  // Capture snapshots at known-good state
  auto snap1 = TestApp::capture_snapshot("headless_pixel_grid");
  auto snap2 = TestApp::capture_snapshot("headless_font_sizes");
  auto snap3 = TestApp::capture_snapshot("headless_layout_stress");
  auto snap4 = TestApp::capture_snapshot("headless_theme_swatches");

  // In validate mode, these automatically compare against baselines
  // and throw on mismatch
}
```
