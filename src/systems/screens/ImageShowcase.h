#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>
#include <array>
#include <cmath>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ImageShowcase : ScreenSystem<UIContext<InputAction>> {
  // State for tracking interactions
  int button_clicks = 0;
  std::string last_action = "None yet";
  bool show_bounds = false;
  bool textures_loaded = false;

  // Textures
  raylib::Texture2D gear_tex{};
  raylib::Texture2D star_tex{};
  raylib::Texture2D trophy_tex{};
  raylib::Texture2D home_tex{};
  raylib::Texture2D play_tex{};

  void load_textures() {
    if (textures_loaded)
      return;
    textures_loaded = true;

    std::string icon_path = afterhours::files::get_resource_path(
                                "kenney/kenney_game-icons/PNG/White/2x/", "")
                                .string();

    gear_tex = raylib::LoadTexture((icon_path + "gear.png").c_str());
    star_tex = raylib::LoadTexture((icon_path + "star.png").c_str());
    trophy_tex = raylib::LoadTexture((icon_path + "trophy.png").c_str());
    home_tex = raylib::LoadTexture((icon_path + "home.png").c_str());
    play_tex = raylib::LoadTexture((icon_path + "fastForward.png").c_str());
    for (auto texture : {gear_tex, star_tex, trophy_tex, home_tex, play_tex})
      raylib::SetTextureFilter(texture, raylib::TEXTURE_FILTER_BILINEAR);
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    load_textures();
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Proportional;
    const float scale = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float left = (context.screen_width / scale - 1144) / 2;
    const float top = (context.screen_height / scale - 720) / 2;
    const afterhours::Color ink{236, 242, 250, 255};
    const afterhours::Color muted{175, 193, 216, 255};
    const afterhours::Color outline{94, 120, 154, 255};
    const auto box = [scale, left, top](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position((left + x) * scale, (top + y) * scale)
          .with_background(Theme::Usage::None).with_corner_radius(0);
    };
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({17, 24, 35, 255}).with_corner_radius(0).with_debug_name("image_bg"));
    const auto label = [&](int id, const std::string &text, float x, float y, float w, float h,
                           float size, afterhours::Color color, const std::string &name = "",
                           TextAlignment alignment = TextAlignment::Left) {
      return div(context, mk(root.ent(), id), box(x, y, w, h).with_label(text)
          .with_font("AtkinsonMock", pixels(size * scale)).with_custom_text_color(color)
          .with_alignment(alignment).with_ignore_pointer_events().with_debug_name(name));
    };
    // Source rect must match each texture's real size — sampling a larger
    // region than the texture (these icons are 100x100, not 128x128) reads past
    // the edge and shows adjacent/garbage pixels as bleed.
    const auto tex_src = [](const raylib::Texture2D &texture) {
      return raylib::Rectangle{0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height)};
    };
    struct Specimen {
      raylib::Texture2D texture;
      const char *glyph;
      const char *action;
      raylib::Rectangle visible_bounds;
    };
    const std::array<Specimen, 5> specimens{{
        {gear_tex, "Gear", "Settings", {20, 20, 64, 64}},
        {star_tex, "Star", "Favorite", {18, 20, 64, 60}},
        {trophy_tex, "Trophy", "Awards", {18, 28, 64, 56}},
        {home_tex, "Home", "Home", {20, 18, 64, 64}},
        {play_tex, "Fast forward", "Fast forward", {14, 20, 76, 64}},
    }};
    const auto size_text = [scale](float width, float height) {
      return fmt::format("{} x {}px", std::lround(width * scale), std::lround(height * scale));
    };
    // Title
    div(context, mk(root.ent(), 0), box(0, 24, 1144, 98)
        .with_custom_background({30, 42, 60, 255}).with_corner_radius(12 * scale));
    label(1, "Image & Sprite Components", 20, 34, 830, 44, 34, ink, "image_title");
    label(2, "One source, four uses. Inspect the image, its frame, and its behavior.", 20, 83, 1104, 28, 21, muted);
    if (button(context, mk(root.ent(), 3), box(894, 42, 230, 42)
        .with_label(show_bounds ? "Hide image bounds" : "Show image bounds")
        .with_font("AtkinsonMock", pixels(21 * scale)).with_custom_text_color(ink)
        .with_custom_background({49, 71, 100, 255}).with_corner_radius(7 * scale)
        .with_debug_name("image_bounds"))) show_bounds = !show_bounds;
    const auto row = [&](int id, float y, float height, const std::string &name) {
      div(context, mk(root.ent(), id), box(0, y, 1144, height)
          .with_custom_background({25, 34, 49, 255}).with_corner_radius(10 * scale).with_debug_name(name));
    };
    row(4, 140, 166, "row1_sprites");
    row(5, 320, 136, "row2_buttons");
    row(6, 470, 88, "row3_image");
    row(7, 572, 104, "row4_iconrow");
    // Row 1: sprite() demo
    label(10, "Sprites", 20, 153, 166, 30, 24, ink);
    label(11, "Static image", 20, 187, 166, 26, 19, muted);
    label(12, "Full texture", 20, 223, 166, 25, 19, muted);
    label(13, show_bounds ? "Bounds visible" : "Bounds hidden", 20, 255, 166, 25, 19, muted, "image_bounds_status");
    for (size_t i = 0; i < specimens.size(); ++i) {
      const auto &specimen = specimens[i];
      const float x = 196 + static_cast<float>(i) * 124;
      div(context, mk(root.ent(), 20 + static_cast<int>(i)), box(x, 151, 104, 94)
          .with_custom_background({35, 48, 66, 255}).with_corner_radius(6 * scale));
      auto config = box(x + 12, 158, 80, 80).with_ignore_pointer_events()
          .with_debug_name("sprite_icon_" + std::to_string(i + 1));
      if (show_bounds) config.with_on_draw_fg([outline](RectangleType r) {
        afterhours::draw_rectangle_outline(r, outline, 1.f);
      });
      sprite(context, mk(root.ent(), 30 + static_cast<int>(i)), specimen.texture, tex_src(specimen.texture), config);
      label(40 + static_cast<int>(i), specimen.glyph, x - 4, 249, 112, 26, 18, ink,
            "sprite_label_" + std::to_string(i + 1), TextAlignment::Center);
      label(50 + static_cast<int>(i), fmt::format("{} x {} source", specimen.texture.width, specimen.texture.height),
            x - 4, 275, 112, 22, 15, muted, "sprite_size_" + std::to_string(i + 1), TextAlignment::Center);
    }
    label(60, "Source rectangle", 844, 156, 280, 27, 22, ink);
    label(61, fmt::format("x 0 / y 0 / w {} / h {}", gear_tex.width, gear_tex.height), 844, 189, 280, 25, 19, muted);
    label(62, "Full-source frame: " + size_text(80, 80), 844, 220, 280, 25, 19, muted);
    label(63, fmt::format("Display scale: {:.2f}x", 80 * scale / static_cast<float>(gear_tex.height)), 844, 251, 280, 25, 19, muted);
    // Row 2: image_button() demo
    label(70, "Image buttons", 20, 335, 166, 30, 24, ink);
    label(71, "Clickable image", 20, 373, 166, 26, 19, muted);
    label(72, "Click / Tab + Enter", 20, 411, 166, 26, 18, muted);
    const std::array<int, 3> button_indices{0, 4, 1};
    const std::array<Theme::Usage, 3> button_usage{Theme::Usage::Primary, Theme::Usage::Accent, Theme::Usage::Secondary};
    const std::array<const char *, 3> button_variants{"Primary", "Accent", "Secondary"};
    for (size_t i = 0; i < button_indices.size(); ++i) {
      const int specimen_index = button_indices[i];
      const auto &specimen = specimens[static_cast<size_t>(specimen_index)];
      const float x = 196 + static_cast<float>(specimen_index) * 124;
      if (image_button(context, mk(root.ent(), 80 + static_cast<int>(i)), specimen.texture, tex_src(specimen.texture),
          box(x + 20, 333, 64, 64).with_background(button_usage[i]).with_corner_radius(8 * scale)
              .with_debug_name("imgbtn_" + std::to_string(i + 1)))) {
        ++button_clicks;
        last_action = specimen.action;
      }
      label(90 + static_cast<int>(i), specimen.action, x - 4, 398, 112, 26, 18, ink,
            "btn_label_" + std::to_string(i + 1), TextAlignment::Center);
      label(100 + static_cast<int>(i), button_variants[i], x - 4, 427, 112, 22, 16, muted, "", TextAlignment::Center);
    }
    div(context, mk(root.ent(), 110), box(440, 333, 244, 108)
        .with_custom_background({35, 48, 66, 255}).with_corner_radius(8 * scale));
    label(111, "Total image-button clicks", 450, 340, 224, 25, 18, muted, "image_click_scope");
    label(112, "Clicks: " + std::to_string(button_clicks), 450, 368, 224, 30, 26, ink, "image_clicks");
    label(113, "Last: " + last_action, 450, 408, 224, 25, 18, muted, "image_last_action");
    label(114, "Three theme variants", 844, 337, 280, 27, 22, ink);
    label(115, "Shared frame: " + size_text(64, 64), 844, 372, 280, 25, 19, muted);
    label(116, "All three update one result.", 844, 405, 280, 25, 19, muted);
    // Row 3: Image with background demo
    label(120, "Image + text", 20, 479, 166, 30, 24, ink);
    label(121, "Static compound", 20, 517, 166, 26, 18, muted);
    div(context, mk(root.ent(), 122), box(196, 482, 260, 64)
        .with_background(Theme::Usage::Secondary).with_corner_radius(8 * scale).with_debug_name("image_container"));
    sprite(context, mk(root.ent(), 123), gear_tex, tex_src(gear_tex), box(208, 490, 48, 48)
        .with_ignore_pointer_events().with_debug_name("container_gear"));
    label(124, "Settings", 268, 497, 176, 34, 24, ink);
    label(125, "Image with text background", 480, 481, 348, 28, 22, ink);
    label(126, size_text(260, 64) + " / " + std::to_string(std::lround(12 * scale)) + "px icon gap", 480, 515, 348, 26, 19, muted);
    div(context, mk(root.ent(), 127), box(844, 482, 64, 64)
        .with_custom_background({205, 215, 229, 255}).with_debug_name("image_light_background")
        .with_on_draw_fg([](RectangleType r) {
          for (int y = 0; y < 4; ++y)
            for (int x = 0; x < 4; ++x) {
              if ((x + y) % 2 == 0) continue;
              afterhours::draw_rectangle({r.x + x * r.width / 4, r.y + y * r.height / 4,
                                          r.width / 4, r.height / 4}, {148, 163, 183, 255});
            }
        }));
    sprite(context, mk(root.ent(), 128), gear_tex, tex_src(gear_tex), box(844, 482, 64, 64)
        .with_ignore_pointer_events().with_debug_name("image_light_sprite"));
    label(129, "White + alpha", 920, 482, 204, 27, 21, ink);
    label(130, "Light checkerboard", 920, 516, 204, 25, 18, muted);
    // Row 4: Icon row demo with labels
    label(140, "Icon row", 20, 583, 166, 30, 24, ink);
    label(141, "Static action labels", 20, 623, 166, 26, 18, muted);
    for (size_t i = 0; i < specimens.size(); ++i) {
      const auto &specimen = specimens[i];
      const float x = 196 + static_cast<float>(i) * 124;
      const float factor = 50 / std::max(specimen.visible_bounds.width, specimen.visible_bounds.height);
      const float width = specimen.visible_bounds.width * factor;
      const float height = specimen.visible_bounds.height * factor;
      div(context, mk(root.ent(), 150 + static_cast<int>(i)), box(x + 20, 580, 64, 64)
          .with_custom_background({35, 48, 66, 255}).with_corner_radius(8 * scale));
      sprite(context, mk(root.ent(), 160 + static_cast<int>(i)), specimen.texture, specimen.visible_bounds,
          box(x + 52 - width / 2, 612 - height / 2, width, height).with_ignore_pointer_events()
              .with_debug_name("icon_" + std::to_string(i)));
      label(170 + static_cast<int>(i), specimen.action, x - 4, 645, 112, 26, 18, ink,
            "label_" + std::to_string(i), TextAlignment::Center);
    }
    label(180, "Cropped to visible pixels", 844, 583, 280, 27, 22, ink);
    label(181, std::to_string(std::lround(50 * scale)) + "px longest visible side", 844, 618, 280, 25, 19, muted);
    label(182, "Same frames / varied silhouettes", 844, 647, 280, 22, 17, muted);
    label(190, "Kenney Game Icons / White 2x PNG / bilinear filtering / source aspect ratio preserved", 0, 685, 1144, 28, 19, muted);
  }
};

REGISTER_EXAMPLE_SCREEN(images, "Component Galleries",
                        "Image, sprite, and image_button demo", ImageShowcase)
