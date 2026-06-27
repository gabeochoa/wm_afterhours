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

---

## Example Screen: AvatarsBadgesShowcase

**File:** `src/systems/screens/AvatarsBadgesShowcase.h`
**CLI:** `--screen=avatars_badges`
**Category:** Widgets

### Layout

A profile/social screen demonstrating avatars and badges:

1. **Avatar Sizes** — A row of avatars at sizes XS, SM, MD, LG, XL. All using the same placeholder image. Labels show the size name below each.

2. **Avatar Shapes** — Three avatars: Circle, Square, Rounded. Same image, different `AvatarShape`.

3. **Fallback Initials** — Avatars with no image, showing colored circles with initials: "JD" (blue), "AB" (green), "XY" (red). Each with a different `fallback_color`.

4. **Status Indicators** — Avatars with online (green dot), away (yellow dot), and offline (gray dot) status indicators in the bottom-right corner.

5. **Avatar Group** — An `avatar_group()` showing 7 overlapping avatars, but `max_visible = 4`. The 5th position shows "+3" indicating hidden avatars.

6. **Badge Gallery** — Badges in all variants: Default, Primary, Secondary, Success, Warning, Error. Both filled and outlined styles. Pill-shaped and squared.

7. **Notification Badge** — A bell icon with a red `notification_badge(count=5)` positioned in the top-right corner. Count of 0 hides the badge. Count > 99 shows "99+".

### Features Exercised

- `avatar()` with size, shape, image, fallback, status
- `avatar_group()` with `max_visible` and overflow count
- `badge()` with all `BadgeVariant` options
- `notification_badge()` with count display
- Pill vs squared badge shapes
- Filled vs outlined badge styles

### Verification

- XS avatar is visually smaller than XL avatar
- Circle avatar clips image to circle shape
- Initials render centered in the fallback circle
- Avatar group shows overlap with "+N" count
- Notification badge with count=0 is hidden
- Notification badge with count=150 shows "99+"

### E2E Test Plan

**Test file:** `src/testing/tests/AvatarsBadgesTest.h`

#### New Custom Commands Needed

None — avatars and badges are display-only. Snapshot comparison is the primary verification method.

#### Screenshots

1. `avatars_sizes` — XS through XL avatars side by side
2. `avatars_shapes` — circle, rounded square, and square avatars
3. `avatars_fallback_initials` — colored circles with initials
4. `avatars_status` — avatars with online/away/offline indicators
5. `avatars_group` — overlapping avatar group with "+3" overflow
6. `badges_variants` — all badge variants and styles
7. `badges_notification` — bell icon with notification count badge

#### Test Script

```cpp
TEST(avatars_badges_render) {
  co_await TestApp::wait_for_frames(5);

  // Verify key elements exist
  TestApp::expect_ui_exists("JD");  // initials fallback
  TestApp::expect_ui_exists("+3");  // avatar group overflow
  TestApp::expect_ui_exists("LEGENDARY");  // badge text
  TestApp::expect_ui_exists("99+");  // capped notification count

  auto snap1 = TestApp::capture_snapshot("avatars_sizes");
  auto snap2 = TestApp::capture_snapshot("avatars_shapes");
  auto snap3 = TestApp::capture_snapshot("avatars_fallback_initials");
  auto snap4 = TestApp::capture_snapshot("avatars_status");
  auto snap5 = TestApp::capture_snapshot("avatars_group");
  auto snap6 = TestApp::capture_snapshot("badges_variants");
  auto snap7 = TestApp::capture_snapshot("badges_notification");
}

TEST(avatars_badges_regression) {
  co_await TestApp::wait_for_frames(5);

  auto snap = TestApp::capture_snapshot("avatars_badges_full");
  auto compare = TestApp::compare_snapshot("avatars_badges_full");
  if (!compare.success) {
    throw std::runtime_error("Avatars/badges regression: " + compare.error_message);
  }
}
```

