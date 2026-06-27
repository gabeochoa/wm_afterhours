# Design Verifications

Consolidated reference for all design verification, testing, and validation systems in the afterhours framework. This document covers runtime validators, automated testing, screenshot regression, accessibility audits, and design audit processes.

---

## 1. Runtime Validation System

The `ValidationConfig` system runs automated checks against the UI at runtime, catching design violations early.

### 1.1 Existing Validators

| Flag | What It Checks | Severity |
|------|---------------|----------|
| `enforce_contrast_ratio` | Text meets 4.5:1 contrast against background | High |
| `enforce_min_font_size` | Text is at least minimum readable size | High |
| `enforce_screen_bounds` | Elements stay within screen edges | High |
| `enforce_child_containment` | Children render inside their parent containers | High |
| `enforce_resolution_independence` | Layout scales correctly across resolutions | Medium |
| `enforce_spacing_rhythm` | Margins/padding align to 4px grid | Low |
| `enforce_pixel_alignment` | No fractional positions causing blurry rendering | Low |
| `enforce_zero_size_detection` | Elements don't resolve to 0 width/height | High |
| `enforce_absolute_margin_conflict` | Absolute-positioned elements don't have unintended margins | Medium |
| `enforce_label_has_font` | Elements with labels have a font set | High |

### 1.2 Validation Modes

| Mode | Behavior |
|------|----------|
| Development | Enables config-conflict validators (ZeroSize, AbsoluteMargin, LabelHasFont) |
| Strict | Enables all validators including spacing rhythm and pixel alignment |
| A11y | Enables accessibility-focused validators (contrast, font size, touch targets, focus rings, labels) |

### 1.3 Proposed Accessibility Validators

#### High Priority

| Flag | Category | WCAG | What It Checks |
|------|----------|------|---------------|
| `enforce_min_touch_target` | Touch | 2.5.8 | Interactive elements >= 44x44px |
| `enforce_focus_ring_visibility` | Keyboard | 2.4.7/13 | Focus ring >= 2px thick, >= 3:1 contrast |
| `enforce_inputs_have_labels` | Forms | 1.3.1 | All inputs have visible text labels |
| `enforce_large_text_contrast` | Contrast | 1.4.3 | Large text (18pt+) meets 3:1, normal text 4.5:1 |
| `enforce_no_hidden_focusables` | Keyboard | 2.4.3 | No invisible elements in the tab order |
| `enforce_unique_interactive_labels` | Content | 2.4.6 | No duplicate labels on interactive elements |
| `enforce_modal_has_dismiss` | Dialogs | 2.1.2 | Every modal has a dismiss mechanism (button or Escape) |
| `enforce_focus_restoration` | Dialogs | 2.4.3 | Focus returns to trigger when modal closes |

#### Medium Priority

| Flag | Category | WCAG | What It Checks |
|------|----------|------|---------------|
| `enforce_focus_order_matches_layout` | Keyboard | 2.4.3 | Tab order follows visual reading order |
| `enforce_touch_target_spacing` | Touch | 2.5.8 | >= 8px gap between adjacent interactive elements |
| `enforce_input_border_contrast` | Contrast | 1.4.11 | Input borders >= 3:1 contrast against background |
| `enforce_icon_contrast` | Contrast | 1.4.11 | Icons >= 3:1 contrast against background |
| `enforce_status_not_color_only` | Feedback | 1.4.1 | Status indicators use text/icons in addition to color |
| `enforce_composite_keyboard_contract` | Keyboard | APG | Composite widgets use roving focus (arrows within, Tab exits) |

#### Low Priority

| Flag | Category | WCAG | What It Checks |
|------|----------|------|---------------|
| `enforce_flash_rate_limit` | Animation | 2.3.1 | No flashing > 3 times/second |
| `enforce_reduced_motion_support` | Animation | 2.3.3 | Reduced-motion preference exists and is respected |
| `enforce_images_have_description` | Images | 1.1.1 | Non-decorative images have labels |
| `enforce_disabled_remain_focusable` | Keyboard | Best practice | Disabled elements stay in tab order |

### 1.4 APCA Contrast Validators

APCA (Accessible Perceptual Contrast Algorithm) provides more accurate contrast measurement than WCAG 2 ratios. Proposed flags:

```
enforce_apca_contrast = false
apca_body_text_min = 75.0    // Lc 75 for body text
apca_large_text_min = 60.0   // Lc 60 for large text (18px+)
apca_headline_min = 45.0     // Lc 45 for headlines
apca_placeholder_min = 30.0  // Lc 30 for placeholder/disabled
apca_nontext_min = 30.0      // Lc 30 for non-text elements
apca_dark_mode_max = 90.0    // Max Lc for large dark-mode text (prevent halation)
```

---

## 2. UI Testing Framework

### 2.1 Test Structure

Tests are coroutines using the `TEST` macro:

```cpp
TEST(my_test) {
    co_await TestApp::wait_for_frames(5);
    TestApp::expect_ui_exists("Button Label");
    TestApp::click_button("Button Label");
    co_await TestApp::wait_for_frames(2);
    TestApp::expect_ui_exists("Expected Result");
}
```

### 2.2 Test Helpers

**Waiting:**
- `wait_for_frames(n)` -- wait N frames
- `wait_for_ui_exists(label, max_frames)` -- wait until element appears
- `wait_for_condition(fn, max_frames)` -- wait for custom condition

**Assertions:**
- `expect_ui_exists(label)` -- element with label exists
- `expect_ui_not_exists(label)` -- element does not exist
- `expect_focus(label)` -- element has keyboard focus

**Input Simulation:**
- `click_button(label)` -- click by label
- `simulate_tab()` / `simulate_shift_tab()` -- Tab navigation
- `simulate_arrow_key(key)` -- arrow key press
- `simulate_enter()` / `simulate_escape()` -- Enter/Escape
- `simulate_typing(text)` -- type text

**Planned Custom Commands:**
- `hover_element(label)` -- move mouse to element without clicking
- `right_click_element(label)` -- right-click by label
- `long_press_button(label, frames)` -- click and hold
- `simulate_window_resize(w, h)` -- change resolution
- `expect_element_size(label, w, h, tolerance)` -- check dimensions

### 2.3 Snapshot Testing

Captures visual screenshots + UI state JSON for regression testing:

```cpp
auto result = TestApp::capture_snapshot("my_snapshot");
auto result = TestApp::compare_snapshot("my_snapshot", 0.01f);
```

Files saved to `test_snapshots/`:
- `{name}.png` -- screenshot
- `{name}_state.json` -- UI state (positions, sizes, visibility, focus)
- `{name}_diff.png` -- visual diff on failure

### 2.4 Running Tests

```bash
./output/ui_tester.exe --list-tests          # List all tests
./output/ui_tester.exe --run-test test_name   # Run one test
./output/ui_tester.exe --run-test test_name --slow-test  # Visible mode
```

### 2.5 Best Practices

- Always wait a few frames after UI creation before interacting
- Use unique, descriptive labels for all testable elements
- Break complex tests into smaller, focused tests
- Use snapshots sparingly -- they're useful but can be brittle
- Handle timing: UI updates span multiple frames

---

## 3. Screenshot Regression Testing

### 3.1 Baseline Directory Structure

```
screenshot-baselines/
+-- screens/                    # One PNG per showcase screen
|   +-- simple_button.png
|   +-- modal_showcase.png
|   +-- ...
+-- tests/                      # One PNG per TestApp snapshot
|   +-- tabbing_forward.png
|   +-- button_click_initial.png
|   +-- ...
+-- manifest.json               # Resolution, tolerance, git hash
```

### 3.2 Workflow

**Update baselines** (after intentional UI changes):
```bash
make update-baselines
git add screenshot-baselines/
```

**Validate current build** (CI / pre-commit):
```bash
make validate-screenshots
```

**Quick test-only check:**
```bash
make validate-tests
```

### 3.3 Headless Rendering

The system supports headless rendering (no window) for CI:

```bash
./output/ui_tester.exe --run-all-tests --headless
./output/ui_tester.exe --headless-screenshots --image-output screenshot-baselines/screens/
```

### 3.4 Multi-Resolution Testing

Screenshots are captured at multiple resolutions to catch resolution-dependent bugs:

| Resolution | Name |
|-----------|------|
| 1280x720 | 720p |
| 1920x1080 | 1080p |

```bash
python3 screenshot_all_screens.py --validate --resolution=1280x720,1920x1080
```

Validation runs at each resolution, reporting which resolution triggered which warnings.

### 3.5 Tolerance

Default threshold: 1% pixel difference. Per-screen overrides supported via `manifest.json`:

```json
{
    "default_tolerance": 1.0,
    "overrides": {
        "language_demo": 3.0,
        "text_input_demo": 2.0
    }
}
```

### 3.6 Failure Artifacts

On mismatch, artifacts are saved to `test-failures/`:
- `{name}_baseline.png` -- expected
- `{name}_current.png` -- actual
- `{name}_diff.png` -- highlighted differences
- `summary.json` -- machine-readable failure list

---

## 4. Similarity Testing (Inspiration Comparison)

Measures how closely rendered UI screens match inspiration mockups.

### 4.1 Score Interpretation

| Range | Meaning |
|-------|---------|
| 90%+ | Excellent match, minor differences only |
| 70-90% | Good match, some layout/styling differences |
| 50-70% | Recognizable but significant differences |
| <50% | Major layout/style mismatches |

### 4.2 Workflow

```bash
make                                    # Build
python3 screenshot_all_screens.py       # Capture all screens
cp /tmp/ui_showcase_screenshots/*.png output/
python3 scripts/image_diff.py           # Compare against inspiration
```

### 4.3 Common Score Reducers

- **Aspect ratio mismatch:** Inspiration at 1024x1024 vs app at 1280x720
- **Missing background images:** Gradients/solids vs illustrated backgrounds
- **Font differences:** Similar fonts render differently
- **Missing advanced text effects:** 3D text, complex outlines

---

## 5. Design Audit Process

### 5.1 Multi-Audit System

The multi-audit runs 4 parallel design audits against different guidelines, then consolidates findings by frequency:

1. **Apple HIG (1987)** -- desktop metaphor, usability
2. **Google Material Design 3** -- surfaces, expression, accessibility
3. **Sun Java Look and Feel (1999)** -- cross-platform consistency
4. **Microsoft Windows 95** -- data-centered, 3D borders, menus

### 5.2 USWDS Audit

The USWDS (U.S. Web Design System) audit provides the most thorough accessibility coverage:

**10 Categories:**
1. Design Principles (user needs, trust, accessibility, continuity, listening)
2. Accessibility (WCAG 2.1 AA)
3. Color System (semantic tokens, grade system, magic number)
4. Typography (type scale, readability)
5. Spacing and Layout (8px grid, responsive breakpoints)
6. Components (buttons, forms, navigation, alerts, cards)
7. Plain Language (active voice, short sentences, consistent terms)
8. Visual Design (hierarchy, iconography, states)
9. Responsive and Mobile (touch targets, thumb zone)
10. Trust and Credibility (polish, transparency)

### 5.3 Audit Depth Levels

| Level | Scope |
|-------|-------|
| Quick (5 min) | 5 principles + spot-check contrast + verify focus + top 3-5 issues |
| Standard (20 min) | All 10 categories + accessibility deep check + component spot-checks |
| Deep (60+ min) | Full checklist + every component + full WCAG 2.1 AA + responsive + content |

### 5.4 Evidence-Based Feedback

All audit findings must reference:
- A specific screenshot or interaction state
- Which guideline or principle it violates
- A concrete recommendation with specific values

Format:
1. **What's wrong** -- specific observation with location
2. **Why it matters** -- which principle/guideline is violated
3. **How to fix it** -- concrete recommendation with specific values

---

## 6. Accessibility Checklist (Quick Reference)

### Critical (Must Fix)

- [ ] All text meets 4.5:1 contrast (normal) or 3:1 (large text)
- [ ] All interactive elements reachable via keyboard
- [ ] Color is never the sole means of conveying information
- [ ] All form inputs have visible labels (not placeholder-only)
- [ ] All interactive elements >= 44x44px touch target
- [ ] Focus indicators visible on all focusable elements

### Major (Should Fix)

- [ ] Visual language consistent throughout (same buttons, spacing, typography)
- [ ] Error messages specific and helpful (not generic "Invalid input")
- [ ] Focus ring has 3:1 contrast change between states
- [ ] Tab order follows visual reading order
- [ ] Modals trap focus and return it on close
- [ ] No invisible elements in the tab order

### Minor (Nice to Have)

- [ ] Spacing follows consistent scale (4px/8px rhythm)
- [ ] Typography hierarchy clear and consistent
- [ ] Icons paired with text labels (not icon-only)
- [ ] Disabled elements visually muted but still focusable
- [ ] Loading/empty states provide helpful guidance

---

## 7. Validation Coverage Summary

Of 64 A11y Project checklist items:
- **38** are web-specific (HTML, ARIA, CSS) -- N/A for native UI
- **26** are applicable to afterhours
- **6** are covered by existing validators
- **20** are proposed as new validator flags
- **After full implementation:** 24 of 29 applicable criteria automated

### Current vs Planned Coverage

| Category | Existing | Proposed | Total |
|----------|---------|---------|-------|
| Contrast | 1 | 4 | 5 |
| Keyboard/Focus | 1 (partial) | 7 | 8 |
| Touch Targets | 0 | 2 | 2 |
| Labels/Content | 1 | 3 | 4 |
| Layout/Spacing | 4 | 0 | 4 |
| Config Conflicts | 3 | 0 | 3 |
| Animation Safety | 0 | 2 | 2 |
| Dialogs | 0 | 2 | 2 |
| **Total** | **10** | **+20** | **30** |

---

## 8. CI Integration

### 8.1 Pre-Commit Hook

Only runs on source file changes (.cpp, .h, .hpp):

```bash
# Install
cp scripts/pre-commit-screenshots .git/hooks/pre-commit
chmod +x .git/hooks/pre-commit
```

Blocks commits when screenshots drift beyond threshold.

### 8.2 CI Target

```bash
make ci    # Build + validate all screenshots
```

### 8.3 Recommended Workflow

1. Make code changes
2. Run `make validate-screenshots` locally
3. If intentional UI changes: run `make update-baselines` and commit baselines
4. If unintentional drift: fix the regression
5. Pre-commit hook catches any missed regressions

---

## 9. Common Violations (Ranked by Frequency)

From multi-audit analysis across all screens, these issues appear most often:

### Top 10 Issues

1. **Color as sole state indicator** -- checkboxes, toggles, disabled states using only color
2. **Small/low-contrast text** -- section headers, labels, metadata text
3. **Disabled states too subtle** -- inconsistent or barely visible dimming
4. **Inconsistent element sizing** -- buttons and controls at varying sizes
5. **Section headers too small** -- especially in gallery/showcase screens
6. **Missing hover/focus demonstrations** -- interactive states not shown
7. **Weak tab indication** -- tab strip active state not distinct enough
8. **Small touch targets** -- interactive elements below 44px
9. **Debug-like status text** -- raw state dumps instead of user-friendly text
10. **Cryptic icons** -- icons without text labels
