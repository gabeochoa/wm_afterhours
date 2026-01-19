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

