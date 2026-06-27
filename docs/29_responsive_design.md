# Responsive Design Helpers

**Status:** Not implemented  
**Priority:** Medium  
**Source:** break_ross migration experience

---

## Problem

Making responsive UIs requires manual calculation of screen percentages. There's no built-in support for different screen sizes, aspect ratios, or resolution scaling.

---

## Reference Resolution Scaling

### Problem

Using `screen_pct()` means sizes change with screen size, but game UIs often want "this should be 200px on a 1080p screen, and scale proportionally".

### Suggested Implementation

```cpp
// Set reference resolution for the project
namespace ui::responsive {
    void set_reference_resolution(int width, int height);  // e.g., 1920x1080
    
    // Responsive pixel values that scale to actual screen
    float px(float pixels);      // Scales based on screen width
    float px_y(float pixels);    // Scales based on screen height
    float px_min(float pixels);  // Scales based on min(width, height)
    
    // Component size helpers
    ComponentSize size_px(float w, float h);
}

// Usage:
ui::responsive::set_reference_resolution(1920, 1080);

// On a 1920x1080 screen: exactly 200px
// On a 3840x2160 screen: 400px (scaled 2x)
// On a 1280x720 screen: ~133px (scaled 0.67x)
.with_size({ui::responsive::px(200), ui::responsive::px(50)})
```

---

## Aspect Ratio Handling

```cpp
namespace ui::responsive {
    // Get current aspect ratio
    float get_aspect_ratio();
    
    // Common aspect ratio checks
    bool is_widescreen();     // > 16:10
    bool is_ultrawide();      // > 21:9
    bool is_portrait();       // height > width
    bool is_square_ish();     // between 4:3 and 16:9
    
    // Aspect-ratio aware positioning
    float safe_area_left();   // Left edge of 16:9 area on ultrawide
    float safe_area_right();  // Right edge
    float letterbox_top();    // Top of letterbox on 4:3
    float letterbox_bottom();
}

// Usage: Center content in 16:9 safe area
imm::div(ctx, mk(parent, 0),
    ComponentConfig{}
        .with_absolute_position()
        .with_translate({ui::responsive::safe_area_left(), 0})
        .with_size({ui::responsive::px(1920), fill_parent()})
);
```

---

## Breakpoint System

### Problem

Different layouts for different screen sizes (mobile vs desktop, small TV vs large monitor).

### Suggested Implementation

```cpp
namespace ui::responsive {
    enum class Breakpoint { XS, SM, MD, LG, XL };
    
    // Get current breakpoint based on screen width
    Breakpoint current_breakpoint();
    
    // Custom breakpoint thresholds
    void set_breakpoints(int xs, int sm, int md, int lg, int xl);
    // Default: 480, 768, 1024, 1280, 1920
    
    // Conditional values
    template<typename T>
    T at_breakpoint(std::initializer_list<std::pair<Breakpoint, T>> values);
}

// Usage:
float padding = ui::responsive::at_breakpoint<float>({
    {Breakpoint::XS, 4.0f},
    {Breakpoint::SM, 8.0f},
    {Breakpoint::MD, 12.0f},
    {Breakpoint::LG, 16.0f},
    {Breakpoint::XL, 24.0f}
});

int columns = ui::responsive::at_breakpoint<int>({
    {Breakpoint::XS, 1},
    {Breakpoint::SM, 2},
    {Breakpoint::MD, 3},
    {Breakpoint::LG, 4}
});

// In rendering:
if (ui::responsive::current_breakpoint() <= Breakpoint::SM) {
    render_mobile_layout();
} else {
    render_desktop_layout();
}
```

---

## Responsive Font Sizing

```cpp
namespace ui::responsive {
    // Font size that scales with screen
    float font_px(float base_pixels);
    
    // Font presets
    float font_xs();   // Extra small (captions)
    float font_sm();   // Small (secondary text)
    float font_md();   // Medium (body text) - default
    float font_lg();   // Large (headings)
    float font_xl();   // Extra large (titles)
    float font_2xl();  // Display text
}

// Usage:
.with_font_size(ui::responsive::font_lg())

// Or with helpers in ComponentConfig:
ComponentConfig{}
    .with_responsive_font_size(24)  // 24px at reference resolution
```

---

## Layout Helpers

```cpp
namespace ui::responsive {
    // Responsive spacing
    float spacing_xs();
    float spacing_sm();
    float spacing_md();
    float spacing_lg();
    float spacing_xl();
    
    // Responsive padding/margin presets
    Padding padding_responsive(PaddingSize size);
    Margin margin_responsive(MarginSize size);
}

// Usage:
.with_padding(ui::responsive::padding_responsive(PaddingSize::MD))
.with_gap(ui::responsive::spacing_md())
```

---

## Dynamic Layout Switching

```cpp
// Change layout based on available space
namespace ui::responsive {
    // Check if children would overflow
    bool would_overflow(Entity container, Direction direction);
    
    // Auto-switch between row and column
    Direction auto_direction(Entity container);
}

// Usage: Auto-wrap to column on narrow screens
imm::div(ctx, mk(parent, 0),
    ComponentConfig{}
        .with_direction(ui::responsive::auto_direction(parent))
        .with_wrap(true)
);
```

---

## Screen Change Detection

```cpp
namespace ui::responsive {
    // Callbacks for screen changes
    using ScreenChangeCallback = std::function<void(int width, int height)>;
    void on_screen_resize(ScreenChangeCallback callback);
    
    // Check if screen changed this frame
    bool screen_resized_this_frame();
    
    // Force layout recalculation on resize
    void invalidate_layout_on_resize(bool enabled = true);
}

// Usage:
ui::responsive::on_screen_resize([](int w, int h) {
    log_info("Screen resized to {}x{}", w, h);
    // Recalculate layouts, update breakpoint-dependent state
});
```

---

## Example Screen: ResponsiveDesignShowcase

**File:** `src/systems/screens/ResponsiveDesignShowcase.h`
**CLI:** `--screen=responsive_design`
**Category:** DX / Developer Experience

### Layout

A screen that adapts to different window sizes:

1. **Breakpoint Indicator** — A label at the top showing the current breakpoint: "XS (480)", "SM (768)", "MD (1024)", "LG (1280)", "XL (1920)". Updates in real time as window resizes.

2. **Responsive Grid** — A card grid that changes column count: 1 column at XS, 2 at SM, 3 at MD, 4 at LG. Cards reflow smoothly as the window resizes.

3. **Sidebar Toggle** — At MD and above: a left sidebar (250px) + main content. Below MD: sidebar hidden, full-width content with a hamburger button to toggle sidebar as an overlay.

4. **Font Scaling** — Text rendered with `font_xs()` through `font_2xl()`. At each breakpoint, all font sizes scale proportionally. Labels show the resolved pixel sizes.

5. **Spacing Demo** — A column of items with `spacing_md()` between them. On XS, the spacing is compact; on XL, it's generous. A label shows the resolved spacing value.

6. **Reference Resolution** — Two boxes: one sized with `px(200)` (scales with screen), one sized with `pixels(200)` (fixed). On a 1920px screen they're equal; on a 3840px screen, the responsive one doubles.

### Features Exercised

- `set_reference_resolution()` and `px()` responsive sizing
- `current_breakpoint()` and `at_breakpoint<T>()`
- `font_xs()` through `font_2xl()` responsive font presets
- `spacing_xs()` through `spacing_xl()` responsive spacing
- `is_widescreen()`, `safe_area_left()` for aspect ratio handling
- `screen_resized_this_frame()` for change detection

### Verification

- Resizing window updates the breakpoint indicator
- Grid column count changes at each breakpoint threshold
- Sidebar visibility toggles at MD breakpoint
- `px(200)` at 2x screen resolution renders at 400px
- Font sizes scale proportionally with screen size

### E2E Test Plan

**Test file:** `src/testing/tests/ResponsiveDesignTest.h`

#### New Custom Commands Needed

- `simulate_window_resize(w, h)` — change window/reference resolution. Core command for responsive testing. Adjusts the reference resolution so breakpoint calculations update.
- `expect_element_size(label, w, h, tolerance)` — check element dimensions. Needed to verify px() scaling and responsive sizes.

#### Screenshots

1. `responsive_1920x1080` — full layout at 1920x1080 (4-column grid, sidebar visible)
2. `responsive_1280x720` — layout at 1280x720 (3-column grid, sidebar still visible)
3. `responsive_800x600` — layout at 800x600 (2-column grid, sidebar hidden)
4. `responsive_480x320` — layout at 480x320 (1-column grid, mobile layout)
5. `responsive_breakpoint_indicator` — readout showing current breakpoint and resolved values

#### Test Script

```cpp
TEST(responsive_breakpoint_switch) {
  co_await TestApp::wait_for_frames(5);

  // Start at 1920x1080
  simulate_window_resize(1920, 1080);
  co_await TestApp::wait_for_frames(10);

  TestApp::expect_ui_exists("XL");  // breakpoint indicator
  auto snap1 = TestApp::capture_snapshot("responsive_1920x1080");

  // Resize to 800x600
  simulate_window_resize(800, 600);
  co_await TestApp::wait_for_frames(10);

  TestApp::expect_ui_exists("SM");  // breakpoint should change
  auto snap2 = TestApp::capture_snapshot("responsive_800x600");

  // Resize to 480x320
  simulate_window_resize(480, 320);
  co_await TestApp::wait_for_frames(10);

  TestApp::expect_ui_exists("XS");
  auto snap3 = TestApp::capture_snapshot("responsive_480x320");
}

TEST(responsive_sidebar_toggle) {
  co_await TestApp::wait_for_frames(5);

  // At large resolution, sidebar should be visible
  simulate_window_resize(1920, 1080);
  co_await TestApp::wait_for_frames(10);
  TestApp::expect_ui_exists("Sidebar");

  // At small resolution, sidebar should be hidden
  simulate_window_resize(600, 400);
  co_await TestApp::wait_for_frames(10);
  TestApp::expect_ui_not_exists("Sidebar");
}

TEST(responsive_grid_columns) {
  co_await TestApp::wait_for_frames(5);

  simulate_window_resize(1920, 1080);
  co_await TestApp::wait_for_frames(10);
  TestApp::expect_ui_exists("Columns: 4");

  simulate_window_resize(1280, 720);
  co_await TestApp::wait_for_frames(10);
  TestApp::expect_ui_exists("Columns: 3");

  simulate_window_resize(800, 600);
  co_await TestApp::wait_for_frames(10);
  TestApp::expect_ui_exists("Columns: 2");
}
```

---

## Complete Example

```cpp
void render_responsive_ui(UIContext& ctx, Entity root) {
    using namespace ui::responsive;
    
    // Set up responsive sizing
    set_reference_resolution(1920, 1080);
    
    // Main container - adapts to screen
    imm::div(ctx, mk(root, 0),
        ComponentConfig{}
            .with_size(fill_parent())
            .with_padding(padding_responsive(PaddingSize::LG))
            .with_direction(current_breakpoint() >= Breakpoint::MD 
                          ? Direction::Row : Direction::Column)
    ) {
        // Sidebar - hidden on small screens
        if (current_breakpoint() >= Breakpoint::MD) {
            imm::div(ctx, mk(current, 0),
                ComponentConfig{}
                    .with_size({px(250), fill_parent()})
                    .with_color_usage(Theme::Usage::Surface)
            );
        }
        
        // Main content - takes remaining space
        imm::div(ctx, mk(current, 1),
            ComponentConfig{}
                .with_size(fill_parent())
                .with_padding(padding_responsive(PaddingSize::MD))
        ) {
            // Title
            imm::text(ctx, mk(current, 0),
                ComponentConfig{}
                    .with_label("Dashboard")
                    .with_font_size(font_xl())
            );
            
            // Responsive grid
            int cols = at_breakpoint<int>({
                {Breakpoint::XS, 1},
                {Breakpoint::SM, 2},
                {Breakpoint::LG, 3}
            });
            
            render_grid(ctx, current, cols);
        }
    }
}
```

