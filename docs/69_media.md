# Media Components

**Status:** Not implemented  
**Priority:** Low  
**Source:** Component Gallery Analysis

---

## Image

### Description

An element for displaying images with proper sizing, aspect ratio, and fallback handling.

**Also known as:** Picture

### Use Cases in Game UI
- Item icons
- Character portraits
- Screenshots
- Background images
- Achievement icons
- Map previews

### Suggested Implementation

```cpp
enum class ImageFit { Contain, Cover, Fill, ScaleDown, None };

struct ImageConfig {
    ImageFit fit = ImageFit::Contain;
    float aspect_ratio = 0.0f;  // 0 = natural ratio
    std::optional<TextureConfig> fallback;
    bool lazy_load = false;
    float border_radius = 0.0f;
};

ElementResult image(HasUIContext auto &ctx, EntityParent ep_pair,
                    TextureConfig texture,
                    ComponentSize size,
                    ImageConfig config = ImageConfig());

// With fallback
ElementResult image(HasUIContext auto &ctx, EntityParent ep_pair,
                    std::optional<TextureConfig> texture,
                    ComponentSize size,
                    ImageConfig config = ImageConfig());

// Usage:
image(ctx, mk(parent, 0), item.icon, {pixels(64), pixels(64)},
      ImageConfig{.fit = ImageFit::Cover, .border_radius = 8.0f});
```

### Features Needed
- Multiple fit modes (contain, cover, fill)
- Aspect ratio control
- Fallback for missing images
- Border radius for rounded corners
- Lazy loading (for lists)
- Loading placeholder

### Fit Modes

- **Contain**: Scale to fit, maintain ratio, may have letterboxing
- **Cover**: Scale to fill, maintain ratio, may crop
- **Fill**: Stretch to fill (may distort)
- **ScaleDown**: Like contain but never scale up
- **None**: Natural size

---

## Video

### Description

Video players for displaying video content with playback controls.

**Also known as:** Video player

### Use Cases in Game UI
- Tutorial videos
- Cutscene player
- Trailer playback
- Recording playback

### Note

Video playback requires significant backend support. This is a wishlist item.

### Suggested Implementation

```cpp
struct VideoConfig {
    bool autoplay = false;
    bool loop = false;
    bool muted = false;
    bool show_controls = true;
    float start_time = 0.0f;
};

struct VideoState {
    float current_time;
    float duration;
    bool is_playing;
    bool is_ended;
    float volume;
};

ElementResult video(HasUIContext auto &ctx, EntityParent ep_pair,
                    const std::string &source_path,
                    VideoState &state,
                    VideoConfig config = VideoConfig());

// Usage:
video(ctx, mk(parent, 0), "videos/tutorial_01.mp4", tutorial_state,
      VideoConfig{.show_controls = true});
```

---

## Icon (Extended)

### Description

Extended icon system with multiple sources and fallbacks.

### Note

See `13_icon_registry.md` for the icon registry system. This extends with UI integration.

### Suggested Implementation

```cpp
enum class IconSize { XS, SM, MD, LG, XL };

struct IconConfig {
    IconSize size = IconSize::MD;
    Color color = Theme::Usage::Foreground;
    float rotation = 0.0f;  // Degrees
    bool spin = false;  // For loading icons
};

// Icon by name (uses IconRegistry)
ElementResult icon(HasUIContext auto &ctx, EntityParent ep_pair,
                   const std::string &icon_name,
                   IconConfig config = IconConfig());

// Icon from texture
ElementResult icon(HasUIContext auto &ctx, EntityParent ep_pair,
                   TextureConfig texture,
                   IconConfig config = IconConfig());

// Icon from symbol font
ElementResult icon_symbol(HasUIContext auto &ctx, EntityParent ep_pair,
                          char32_t codepoint,
                          IconConfig config = IconConfig());

// Usage:
icon(ctx, mk(parent, 0), "settings");
icon(ctx, mk(parent, 1), "loading", IconConfig{.spin = true});
icon_symbol(ctx, mk(parent, 2), U'★', IconConfig{.color = colors::gold()});
```

### Features Needed
- Multiple size presets
- Color theming
- Rotation
- Spin animation (for loading)
- Mirroring (for RTL)
- Fallback to text symbol

---

## Carousel

### Description

A means of displaying multiple slides of content, navigated via swiping, scrolling, or buttons.

**Also known as:** Content slider, Slideshow

### Use Cases in Game UI
- Screenshot gallery
- Character selection
- Item showcase
- Tutorial slides
- News/announcements

### Suggested Implementation

```cpp
struct CarouselConfig {
    bool show_navigation = true;  // Prev/next buttons
    bool show_indicators = true;  // Dots
    bool auto_play = false;
    float auto_play_interval = 5.0f;
    bool loop = true;
    bool swipeable = true;
    int slides_visible = 1;  // For multi-item carousels
};

ElementResult carousel(HasUIContext auto &ctx, EntityParent ep_pair,
                       int num_slides,
                       int &current_slide,
                       CarouselConfig config = CarouselConfig());

// Usage:
if (carousel(ctx, mk(parent, 0), screenshots.size(), current_index)) {
    // Render current slide content
    image(ctx, mk(current, 0), screenshots[current_index], fill_parent());
}
```

### Features Needed
- Previous/next navigation buttons
- Dot indicators
- Slide animation
- Touch/swipe support
- Auto-play with pause on hover
- Loop or stop at ends
- Multi-slide view
- Keyboard navigation (arrows)

