# Loading States

**Status:** Not implemented  
**Priority:** Medium  
**Source:** Component Gallery Analysis

---

## Spinner / Loader

### Description

A visual indicator that a process is happening in the background but the interface is not yet ready for interaction.

**Also known as:** Loader, Loading

### Use Cases in Game UI
- Loading screens
- Async operation in progress
- Connecting to server
- Saving game
- Loading assets
- Button loading state

### Suggested Implementation

```cpp
enum class SpinnerSize { SM, MD, LG };
enum class SpinnerVariant { Circular, Dots, Bars, Pulse };

struct SpinnerConfig {
    SpinnerSize size = SpinnerSize::MD;
    SpinnerVariant variant = SpinnerVariant::Circular;
    Color color = Theme::Usage::Accent;
    float speed = 1.0f;  // Animation speed multiplier
};

ElementResult spinner(HasUIContext auto &ctx, EntityParent ep_pair,
                      SpinnerConfig config = SpinnerConfig());

// Spinner with label
ElementResult spinner_with_label(HasUIContext auto &ctx, EntityParent ep_pair,
                                 const std::string &label,
                                 SpinnerConfig config = SpinnerConfig());

// Usage:
spinner(ctx, mk(parent, 0), SpinnerConfig{.size = SpinnerSize::LG});

spinner_with_label(ctx, mk(parent, 1), "Loading...");
```

### Features Needed
- Continuous animation
- Multiple visual styles
- Size variants
- Color theming
- Speed control
- Optional label

### Animation Variants

1. **Circular**: Rotating arc (most common)
2. **Dots**: Bouncing/pulsing dots
3. **Bars**: Loading bars
4. **Pulse**: Pulsing circle

---

## Skeleton / Placeholder

### Description

A placeholder layout for content which hasn't yet loaded, built from grey shapes that mimic the expected content structure.

**Also known as:** Skeleton loader, Content placeholder

### Use Cases in Game UI
- Leaderboard loading
- Player profile loading
- Inventory loading
- Shop items loading
- Any async content

### Suggested Implementation

```cpp
enum class SkeletonShape { Rectangle, Circle, Text };

struct SkeletonConfig {
    SkeletonShape shape = SkeletonShape::Rectangle;
    bool animate = true;  // Shimmer animation
};

ElementResult skeleton(HasUIContext auto &ctx, EntityParent ep_pair,
                       ComponentSize size,
                       SkeletonConfig config = SkeletonConfig());

// Text skeleton (multiple lines)
ElementResult skeleton_text(HasUIContext auto &ctx, EntityParent ep_pair,
                            int num_lines,
                            float last_line_width = 0.6f,  // As percentage
                            ComponentConfig config = ComponentConfig());

// Preset skeleton layouts
ElementResult skeleton_card(HasUIContext auto &ctx, EntityParent ep_pair);
ElementResult skeleton_list_item(HasUIContext auto &ctx, EntityParent ep_pair);
ElementResult skeleton_avatar(HasUIContext auto &ctx, EntityParent ep_pair,
                              AvatarSize size = AvatarSize::MD);

// Usage:
if (is_loading) {
    skeleton_card(ctx, mk(parent, 0));
    skeleton_card(ctx, mk(parent, 1));
    skeleton_card(ctx, mk(parent, 2));
} else {
    for (auto& item : items) {
        item_card(ctx, mk(parent, item.id), item);
    }
}
```

### Features Needed
- Rectangle, circle, text shapes
- Shimmer/pulse animation
- Composable for complex layouts
- Match actual content dimensions
- Preset layouts for common patterns

---

## Loading Overlay

### Description

Full-screen or container overlay with loading indicator, blocking interaction.

### Suggested Implementation

```cpp
struct LoadingOverlayConfig {
    std::string message = "Loading...";
    SpinnerVariant spinner_variant = SpinnerVariant::Circular;
    float backdrop_opacity = 0.7f;
    bool show_progress = false;
    float progress = 0.0f;  // 0-1 if show_progress
};

ElementResult loading_overlay(HasUIContext auto &ctx, EntityParent ep_pair,
                              bool is_loading,
                              LoadingOverlayConfig config = LoadingOverlayConfig());

// Usage:
loading_overlay(ctx, mk(parent, 0), is_saving,
                LoadingOverlayConfig{.message = "Saving game..."});
```

---

## Progress with Loading

### Description

Determinate progress indicator for operations with known completion percentage.

### Note

See `28_meters_gauges.md` for basic progress bars.

### Suggested Implementation

```cpp
struct LoadingProgressConfig {
    std::string label;
    std::string detail;  // "3 of 10 files"
    bool show_percentage = true;
    bool cancelable = false;
};

ElementResult loading_progress(HasUIContext auto &ctx, EntityParent ep_pair,
                               float progress,  // 0-1
                               LoadingProgressConfig config = LoadingProgressConfig());

// Usage:
loading_progress(ctx, mk(parent, 0), download_progress,
                 LoadingProgressConfig{
                     .label = "Downloading update...",
                     .detail = fmt::format("{} MB / {} MB", downloaded, total)
                 });
```

---

## Example Screen: LoadingStatesShowcase

**File:** `src/systems/screens/LoadingStatesShowcase.h`
**CLI:** `--screen=loading_states`
**Category:** Widgets

### Layout

A screen demonstrating all loading state components:

1. **Spinners** — A row of 4 spinner variants: Circular (rotating arc), Dots (bouncing dots), Bars (loading bars), Pulse (pulsing circle). Each labeled with its variant. Speed can be adjusted with a slider.

2. **Spinner Sizes** — Circular spinners at SM, MD, LG sizes side by side.

3. **Skeleton Loaders** — A mock "loading" state for a profile card: skeleton avatar (circle), skeleton text (3 lines, last line 60% width), skeleton button. After 3 seconds, the actual content replaces the skeletons with a fade transition.

4. **Skeleton List** — A list of 5 `skeleton_list_item()` placeholders simulating a leaderboard loading. Shimmer animation sweeps across each skeleton.

5. **Loading Overlay** — A button "Start Operation" that triggers a `loading_overlay()` covering the screen. Shows a spinner, message "Processing...", and a progress bar filling from 0% to 100% over 3 seconds. Overlay blocks all interaction until complete.

6. **Button Loading State** — A button that shows a spinner inside it when clicked (replacing the label text with a small spinner + "Saving..."). After 2 seconds, reverts to "Save".

### Features Exercised

- `spinner()` with all `SpinnerVariant` options and sizes
- `skeleton()`, `skeleton_text()`, `skeleton_card()`, `skeleton_list_item()`
- Shimmer animation on skeleton loaders
- `loading_overlay()` with message, spinner, progress bar
- Button loading state with inline spinner

### Verification

- All spinners animate continuously (not static)
- Skeleton shimmer sweeps left-to-right in a repeating pattern
- After 3 seconds, skeletons are replaced by actual content
- Loading overlay blocks all mouse clicks and keyboard input
- Progress bar fills smoothly from 0% to 100%

### E2E Test Plan

**Test file:** `src/testing/tests/LoadingStatesTest.h`

#### New Custom Commands Needed

None — uses existing `click_button`, `expect_ui_exists`, `wait_for_frames`, `capture_snapshot`. Animation is verified via snapshot comparison across frames.

#### Screenshots

1. `loading_spinners` — row of spinner variants (circular, dots, bars, pulse)
2. `loading_spinner_sizes` — SM/MD/LG spinners
3. `loading_skeleton_loading` — skeleton placeholders before content loads
4. `loading_skeleton_loaded` — actual content after skeleton transition (3s later)
5. `loading_overlay_active` — full-screen loading overlay with progress bar
6. `loading_overlay_complete` — after loading completes, overlay gone
7. `loading_button_state` — button showing inline spinner during save

#### Test Script

```cpp
TEST(loading_spinners_animate) {
  co_await TestApp::wait_for_frames(5);

  auto snap_a = TestApp::capture_snapshot("loading_spinners");
  co_await TestApp::wait_for_frames(15);
  auto snap_b = TestApp::capture_snapshot("loading_spinners_frame_b");

  // Spinners should be animated (frames differ)
}

TEST(loading_skeleton_transition) {
  co_await TestApp::wait_for_frames(5);

  auto snap_loading = TestApp::capture_snapshot("loading_skeleton_loading");

  // Wait for skeleton-to-content transition (3s = ~180 frames)
  co_await TestApp::wait_for_frames(200);

  auto snap_loaded = TestApp::capture_snapshot("loading_skeleton_loaded");
  // Content should now be visible, skeletons gone
}

TEST(loading_overlay) {
  co_await TestApp::wait_for_frames(5);

  TestApp::click_button("Start Operation");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("Processing...");
  auto snap = TestApp::capture_snapshot("loading_overlay_active");

  // Overlay should block clicks (try clicking a background button)
  TestApp::click_button("Start Operation");  // should be blocked
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  // Wait for loading to complete (3s = ~180 frames)
  co_await TestApp::wait_for_frames(200);

  auto snap2 = TestApp::capture_snapshot("loading_overlay_complete");
}

TEST(loading_button_inline) {
  co_await TestApp::wait_for_frames(5);

  TestApp::click_button("Save");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("Saving...");
  auto snap = TestApp::capture_snapshot("loading_button_state");

  // Wait for save to complete (2s = ~120 frames)
  co_await TestApp::wait_for_frames(130);

  TestApp::expect_ui_exists("Save");  // reverted to normal
}
```

