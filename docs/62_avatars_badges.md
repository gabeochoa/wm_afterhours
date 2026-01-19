# Avatars and Badges

**Status:** Not implemented  
**Priority:** Low  
**Source:** Component Gallery Analysis

---

## Avatar

### Description

A graphical representation of a user: usually a photo, illustration, or initial.

### Use Cases in Game UI
- Player profile pictures
- Friends list
- Leaderboards
- Chat/messaging
- Team member display
- Character portraits

### Suggested Implementation

```cpp
enum class AvatarSize { XS, SM, MD, LG, XL };
enum class AvatarShape { Circle, Square, Rounded };

struct AvatarConfig {
    AvatarSize size = AvatarSize::MD;
    AvatarShape shape = AvatarShape::Circle;
    std::optional<std::string> fallback_initials;
    std::optional<Color> fallback_color;
    std::optional<TextureConfig> status_icon;  // Online/offline indicator
    bool show_border = false;
};

ElementResult avatar(HasUIContext auto &ctx, EntityParent ep_pair,
                     std::optional<TextureConfig> image,
                     AvatarConfig config = AvatarConfig());

// Avatar group (overlapping avatars)
ElementResult avatar_group(HasUIContext auto &ctx, EntityParent ep_pair,
                           const std::vector<TextureConfig> &images,
                           int max_visible = 5,
                           ComponentConfig config = ComponentConfig());

// Usage:
avatar(ctx, mk(parent, 0), player_portrait, 
       AvatarConfig{.size = AvatarSize::LG, .show_border = true});

// Fallback to initials when no image
avatar(ctx, mk(parent, 1), std::nullopt,
       AvatarConfig{.fallback_initials = "JD", .fallback_color = colors::blue()});

// Group of players
avatar_group(ctx, mk(parent, 2), team_portraits, 4);
```

### Features Needed
- Multiple sizes
- Circle/square/rounded shapes
- Image display with fallback
- Initials fallback when no image
- Status indicator (online dot)
- Avatar groups with overlap
- Border/ring option

---

## Badge

### Description

A small label, generally appearing inside or in close proximity to another component, representing a status, property, or some other metadata.

**Also known as:** Tag, Label, Chip

### Use Cases in Game UI
- Item rarity (Common, Rare, Epic, Legendary)
- Player level/rank
- New item indicators
- Notification counts
- Status tags (Online, Away, Busy)
- Category labels

### Suggested Implementation

```cpp
enum class BadgeVariant { Default, Primary, Secondary, Success, Warning, Error };

struct BadgeConfig {
    BadgeVariant variant = BadgeVariant::Default;
    bool pill_shape = true;   // Rounded ends
    bool outlined = false;    // Outline vs filled
    std::optional<TextureConfig> icon;
    std::optional<Color> custom_color;
};

ElementResult badge(HasUIContext auto &ctx, EntityParent ep_pair,
                    const std::string &text,
                    BadgeConfig config = BadgeConfig());

// Notification badge (number in circle)
ElementResult notification_badge(HasUIContext auto &ctx, EntityParent ep_pair,
                                 int count,
                                 ComponentConfig config = ComponentConfig());

// Usage:
badge(ctx, mk(parent, 0), "LEGENDARY", 
      BadgeConfig{.custom_color = colors::hex(0xFFD700)});

badge(ctx, mk(parent, 1), "New", 
      BadgeConfig{.variant = BadgeVariant::Primary});

// Notification count on icon
notification_badge(ctx, mk(parent, 2), unread_messages);
```

### Features Needed
- Multiple color variants
- Pill/rounded shape
- Filled vs outlined styles
- Optional leading icon
- Compact sizing
- Notification count variant (circular)
- Positioning helpers for overlaying on other elements

## Existing Workaround

See `src/ui_workarounds/NotificationBadge.h` for a basic implementation.

