# Page Section Components

**Status:** Not implemented  
**Priority:** Low  
**Source:** Component Gallery Analysis

---

## Header

### Description

An element that appears across the top of screens, usually containing the title and main navigation.

### Use Cases in Game UI
- Main menu header
- In-game HUD top bar
- Settings screen header
- Pause menu header

### Suggested Implementation

```cpp
struct HeaderConfig {
    std::optional<TextureConfig> logo;
    bool sticky = false;  // Fixed at top when scrolling
    bool show_back_button = false;
    std::function<void()> on_back;
};

ElementResult header(HasUIContext auto &ctx, EntityParent ep_pair,
                     const std::string &title,
                     HeaderConfig config = HeaderConfig());

// Usage:
header(ctx, mk(parent, 0), "Settings",
       HeaderConfig{.show_back_button = true, .on_back = []{ go_back(); }});
```

### Features Needed
- Title display
- Optional logo/icon
- Back button
- Optional right-side actions
- Sticky positioning
- Consistent height across screens

---

## Footer

### Description

Appears at the bottom of a page or section, displaying secondary information, copyright, or navigation links.

### Use Cases in Game UI
- Copyright/version info
- Controller button hints
- Navigation shortcuts
- Status bar

### Suggested Implementation

```cpp
ElementResult footer(HasUIContext auto &ctx, EntityParent ep_pair,
                     ComponentConfig config = ComponentConfig());

// Controller button hints footer
struct ButtonHint {
    std::string button;  // "A", "B", "X", "Y", etc.
    std::string action;  // "Select", "Back", etc.
};

ElementResult button_hints_footer(HasUIContext auto &ctx, EntityParent ep_pair,
                                  const std::vector<ButtonHint> &hints,
                                  ComponentConfig config = ComponentConfig());

// Usage:
button_hints_footer(ctx, mk(parent, 0), {
    {"A", "Select"},
    {"B", "Back"},
    {"Y", "Options"}
});
```

---

## Hero / Jumbotron / Banner

### Description

A large banner, usually one of the first items on a page, often containing a full-width image, title, and call-to-action.

### Use Cases in Game UI
- Main menu hero image
- Event/update banners
- Featured content
- Welcome screens

### Suggested Implementation

```cpp
struct HeroConfig {
    std::optional<TextureConfig> background_image;
    std::optional<TextureConfig> overlay_gradient;
    float height = 0.4f;  // As screen percentage
    bool parallax = false;
};

ElementResult hero(HasUIContext auto &ctx, EntityParent ep_pair,
                   const std::string &title,
                   const std::string &subtitle,
                   HeroConfig config = HeroConfig());

// Usage:
hero(ctx, mk(parent, 0), 
     "Welcome Back, Player!",
     "Your adventure continues...",
     HeroConfig{.background_image = hero_bg, .parallax = true});
```

### Features Needed
- Full-width layout
- Background image
- Overlay gradient for text readability
- Title and subtitle
- Optional call-to-action button
- Parallax scrolling effect

---

## Heading

### Description

A title or caption used to introduce a new section, with appropriate visual hierarchy.

### Suggested Implementation

```cpp
enum class HeadingLevel { H1, H2, H3, H4, H5, H6 };

ElementResult heading(HasUIContext auto &ctx, EntityParent ep_pair,
                      const std::string &text,
                      HeadingLevel level = HeadingLevel::H1,
                      ComponentConfig config = ComponentConfig());

// Usage:
heading(ctx, mk(parent, 0), "Graphics Settings", HeadingLevel::H2);
```

### Features Needed
- Size hierarchy (H1 largest, H6 smallest)
- Semantic structure
- Consistent styling per level
- Optional icon

---

## Card

### Description

A container for content representing a single entity (contact, article, item, etc.).

**Also known as:** Tile

### Use Cases in Game UI
- Item cards
- Player profile cards
- Achievement cards
- Save game slots
- Shop items

### Suggested Implementation

```cpp
struct CardConfig {
    bool elevated = true;  // Shadow
    bool hoverable = true;  // Lift on hover
    bool clickable = true;
    float border_radius = 8.0f;
    std::optional<TextureConfig> header_image;
};

ElementResult card(HasUIContext auto &ctx, EntityParent ep_pair,
                   CardConfig config = CardConfig());

// Pre-built card sections
ElementResult card_header(HasUIContext auto &ctx, EntityParent ep_pair,
                          const std::string &title,
                          const std::string &subtitle = "");
ElementResult card_body(HasUIContext auto &ctx, EntityParent ep_pair);
ElementResult card_footer(HasUIContext auto &ctx, EntityParent ep_pair);

// Usage:
card(ctx, mk(parent, 0)) {
    if (item.image) {
        image(ctx, mk(current, 0), item.image, {fill_parent(), pixels(120)});
    }
    card_header(ctx, mk(current, 1), item.name, item.category);
    card_body(ctx, mk(current, 2)) {
        text(ctx, mk(current, 0), item.description);
    }
    card_footer(ctx, mk(current, 3)) {
        price_display(ctx, mk(current, 0), item.price);
        button(ctx, mk(current, 1), "Buy");
    }
}
```

### Features Needed
- Elevation/shadow
- Hover state
- Click handling
- Header image slot
- Header/body/footer sections
- Border radius
- Content padding

---

## Example Screen: PageSectionsShowcase

**File:** `src/systems/screens/PageSectionsShowcase.h`
**CLI:** `--screen=page_sections`
**Category:** Widgets

### Layout

A full-page layout demonstrating all page section components:

1. **Header** — A `header()` at the top with: title "Game Settings", a back button (←), and a right-side action button (gear icon). Consistent height, dark background.

2. **Hero Banner** — A `hero()` section below the header: full-width, 40% height, with a background image/color, title "Welcome Back, Player!", subtitle "Your adventure continues...", and a "Play Now" call-to-action button.

3. **Heading Hierarchy** — Below the hero: `heading("Section", H1)`, `heading("Subsection", H2)`, `heading("Detail", H3)` showing visual size hierarchy from largest to smallest.

4. **Card Grid** — Four `card()` components in a 2x2 grid. Each card has: a colored header image area, `card_header()` with title/subtitle, `card_body()` with description text, and `card_footer()` with a price display and "Buy" button. Cards have elevation shadow and lift on hover.

5. **Footer** — A `footer()` at the bottom with: version text "v1.2.3", copyright, and `button_hints_footer()` showing controller hints: "A: Select | B: Back | Y: Options".

### Features Exercised

- `header()` with title, back button, actions
- `hero()` with background, title, subtitle, CTA button
- `heading()` at levels H1, H2, H3 with visual hierarchy
- `card()` with `card_header/body/footer` sections, elevation, hover
- `footer()` and `button_hints_footer()` for controller hints

### Verification

- Header stays at top, consistent height across screens
- Hero banner fills width with readable text over background
- H1 is visually larger than H2, H2 larger than H3
- Cards have visible shadow that lifts on hover
- Footer displays at bottom with version and controller hints
- Back button in header fires callback
- Card click fires callback

### E2E Test Plan

**Test file:** `src/testing/tests/PageSectionsTest.h`

#### New Custom Commands Needed

- `hover_element(label)` — move mouse to element without clicking. Needed for card hover-lift effect verification.

#### Screenshots

1. `page_header` — header bar with title, back button, and action button
2. `page_hero` — hero banner with title, subtitle, and CTA button
3. `page_headings` — H1/H2/H3 heading hierarchy
4. `page_cards` — 2x2 card grid with shadows
5. `page_card_hovered` — card with lift/shadow effect on hover
6. `page_footer` — footer with version text and controller hints

#### Test Script

```cpp
TEST(page_sections_render) {
  co_await TestApp::wait_for_frames(5);

  // Verify all sections exist
  TestApp::expect_ui_exists("Game Settings");  // header title
  TestApp::expect_ui_exists("Welcome Back, Player!");  // hero title
  TestApp::expect_ui_exists("Section");  // H1
  TestApp::expect_ui_exists("Subsection");  // H2
  TestApp::expect_ui_exists("v1.2.3");  // footer version

  auto snap1 = TestApp::capture_snapshot("page_header");
  auto snap2 = TestApp::capture_snapshot("page_hero");
  auto snap3 = TestApp::capture_snapshot("page_headings");
  auto snap4 = TestApp::capture_snapshot("page_cards");
  auto snap5 = TestApp::capture_snapshot("page_footer");
}

TEST(page_sections_back_button) {
  co_await TestApp::wait_for_frames(5);

  // Click back button in header
  TestApp::click_button("Back");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  // Back callback should fire (logged or screen changes)
}

TEST(page_sections_hero_cta) {
  co_await TestApp::wait_for_frames(5);

  TestApp::click_button("Play Now");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  // CTA callback should fire
}

TEST(page_sections_card_hover) {
  co_await TestApp::wait_for_frames(5);

  auto snap_before = TestApp::capture_snapshot("page_cards");

  // Hover over first card
  hover_element("Card 1");
  co_await TestApp::wait_for_frames(5);

  auto snap_hovered = TestApp::capture_snapshot("page_card_hovered");
  // Card should show elevated shadow in hovered state
}

TEST(page_sections_card_click) {
  co_await TestApp::wait_for_frames(5);

  TestApp::click_button("Buy");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(3);

  // Card click callback should fire
}

TEST(page_sections_button_hints) {
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_exists("A: Select");
  TestApp::expect_ui_exists("B: Back");
}
```

