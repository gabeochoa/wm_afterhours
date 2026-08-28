#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ImageShowcase : ScreenSystem<UIContext<InputAction>> {
  // State for tracking interactions
  int button_clicks = 0;
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
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    load_textures();

    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;

    // Main container background - centered on screen with padding
    auto root =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.90f), screen_pct(0.95f)})
                .with_self_align(SelfAlign::Center)
                .with_background(Theme::Usage::Background)
                .with_roundness(0.08f)
                .with_padding(Spacing::xl)
                .with_debug_name("image_bg"));

    // Content container
    auto main_container =
        vstack(context, mk(root.ent(), 0),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                   .with_no_wrap()
                   .with_debug_name("image_main"));

    // Title
    div(context, mk(main_container.ent(), 0),
        ComponentConfig{}
            .with_label("Image & Sprite Components")
            .with_size(ComponentSize{percent(1.0f), pixels(44)})
            .with_background(Theme::Usage::Surface)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, pixels(24.0f))
            .with_margin(Margin{.bottom = DefaultSpacing::small()}));

    // Source rect must match each texture's real size — sampling a larger
    // region than the texture (these icons are 100x100, not 128x128) reads past
    // the edge and shows adjacent/garbage pixels as bleed.
    auto tex_src = [](const raylib::Texture2D &t) {
      return raylib::Rectangle{0, 0, static_cast<float>(t.width),
                               static_cast<float>(t.height)};
    };

    // Row 1: sprite() demo
    auto row1 = hstack(context, mk(main_container.ent(), 1),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.0f), pixels(110)})
                           .with_background(Theme::Usage::Surface)
                           .with_padding(Spacing::xs)
                           .with_align_items(AlignItems::Center)
                           .with_debug_name("row1_sprites"));

    div(context, mk(row1.ent(), 0),
        ComponentConfig{}
            .with_label("Sprites")
            .with_size(ComponentSize{pixels(100), pixels(40)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_skip_tabbing(true)
            .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f)));

    // Helper lambda to create sprite with label for row1
    auto create_labeled_sprite = [&](int idx, const raylib::Texture2D &tex,
                                     const std::string &label_text) {
      auto sprite_wrapper =
          vstack(context, mk(row1.ent(), idx),
                 ComponentConfig{}
                     .with_size(ComponentSize{pixels(88), pixels(90)})
                     .with_align_items(AlignItems::Center)
                     .with_margin(Spacing::xs)
                     .with_debug_name("sprite_wrapper_" + std::to_string(idx)));

      sprite(context, mk(sprite_wrapper.ent(), 0), tex, tex_src(tex),
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(56), pixels(56)})
                 .with_debug_name("sprite_icon_" + std::to_string(idx)));

      div(context, mk(sprite_wrapper.ent(), 1),
          ComponentConfig{}
              .with_label(label_text)
              .with_size(ComponentSize{pixels(80), pixels(22)})
              .with_custom_text_color(theme.font)
              .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
              .with_skip_tabbing(true)
              .with_debug_name("sprite_label_" + std::to_string(idx)));
    };

    create_labeled_sprite(1, gear_tex, "Gear");
    create_labeled_sprite(2, star_tex, "Star");
    create_labeled_sprite(3, trophy_tex, "Trophy");
    create_labeled_sprite(4, home_tex, "Home");
    create_labeled_sprite(5, play_tex, "Play");

    // Row 2: image_button() demo
    auto row2 = hstack(context, mk(main_container.ent(), 2),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.0f), pixels(110)})
                           .with_background(Theme::Usage::Surface)
                           .with_padding(Spacing::xs)
                           .with_align_items(AlignItems::Center)
                           .with_margin(Margin{.top = DefaultSpacing::tiny()})
                           .with_debug_name("row2_buttons"));

    div(context, mk(row2.ent(), 0),
        ComponentConfig{}
            .with_label("Image Buttons")
            .with_size(ComponentSize{pixels(140), pixels(40)})
            .with_background(Theme::Usage::Accent)
            .with_auto_text_color(true)
            .with_skip_tabbing(true)
            .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f)));

    // Helper lambda to create image button with label
    auto create_labeled_button = [&](int idx, const raylib::Texture2D &tex,
                                     const std::string &label_text,
                                     Theme::Usage bg_usage) {
      auto btn_wrapper =
          vstack(context, mk(row2.ent(), idx),
                 ComponentConfig{}
                     .with_size(ComponentSize{pixels(88), pixels(90)})
                     .with_align_items(AlignItems::Center)
                     .with_margin(Spacing::xs)
                     .with_debug_name("btn_wrapper_" + std::to_string(idx)));

      if (image_button(context, mk(btn_wrapper.ent(), 0), tex, tex_src(tex),
                       ComponentConfig{}
                           .with_size(ComponentSize{pixels(64), pixels(64)})
                           .with_background(bg_usage)
                           .with_rounded_corners(RoundedCorners().all_round())
                           .with_roundness(0.2f)
                           .with_debug_name("imgbtn_" + std::to_string(idx)))) {
        button_clicks++;
      }

      div(context, mk(btn_wrapper.ent(), 1),
          ComponentConfig{}
              .with_label(label_text)
              .with_size(ComponentSize{pixels(80), pixels(22)})
              .with_custom_text_color(theme.font)
              .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
              .with_skip_tabbing(true)
              .with_debug_name("btn_label_" + std::to_string(idx)));
    };

    create_labeled_button(1, gear_tex, "Settings", Theme::Usage::Primary);
    create_labeled_button(2, play_tex, "Play", Theme::Usage::Accent);
    create_labeled_button(3, star_tex, "Favorite", Theme::Usage::Secondary);

    div(context, mk(row2.ent(), 4),
        ComponentConfig{}
            .with_label("Clicks: " + std::to_string(button_clicks))
            .with_size(ComponentSize{pixels(110), pixels(40)})
            .with_background(Theme::Usage::Surface)
            .with_custom_text_color(theme.font)
            .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
            .with_margin(Margin{.left = DefaultSpacing::small()})
            .with_skip_tabbing(true));

    // Row 3: Image with background demo
    auto row3 = hstack(context, mk(main_container.ent(), 3),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.0f), pixels(80)})
                           .with_background(Theme::Usage::Surface)
                           .with_padding(Spacing::xs)
                           .with_align_items(AlignItems::Center)
                           .with_margin(Margin{.top = DefaultSpacing::tiny()})
                           .with_debug_name("row3_image"));

    div(context, mk(row3.ent(), 0),
        ComponentConfig{}
            .with_label("With Background")
            .with_size(ComponentSize{pixels(140), pixels(40)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_skip_tabbing(true)
            .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f)));

    auto img_container =
        hstack(context, mk(row3.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(200), pixels(56)})
                   .with_background(Theme::Usage::Secondary)
                   .with_rounded_corners(RoundedCorners().all_round())
                   .with_roundness(0.1f)
                   .with_padding(Spacing::xs)
                   .with_align_items(AlignItems::Center)
                   .with_margin(Spacing::xs)
                   .with_debug_name("image_container"));

    sprite(context, mk(img_container.ent(), 0), gear_tex, tex_src(gear_tex),
           ComponentConfig{}
               .with_size(ComponentSize{pixels(48), pixels(48)})
               .with_debug_name("container_gear"));

    div(context, mk(img_container.ent(), 1),
        ComponentConfig{}
            .with_label("Settings")
            .with_size(ComponentSize{pixels(100), pixels(32)})
            .with_custom_text_color(theme.font)
            .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
            .with_margin(Margin{.left = DefaultSpacing::tiny()})
            .with_skip_tabbing(true));

    // Row 4: Icon row demo with labels
    auto row4 = hstack(context, mk(main_container.ent(), 4),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.0f), pixels(115)})
                           .with_background(Theme::Usage::Surface)
                           .with_padding(Spacing::xs)
                           .with_align_items(AlignItems::Center)
                           .with_margin(Margin{.top = DefaultSpacing::tiny()})
                           .with_debug_name("row4_iconrow"));

    div(context, mk(row4.ent(), 0),
        ComponentConfig{}
            .with_label("Icon Row")
            .with_size(ComponentSize{pixels(100), pixels(40)})
            .with_background(Theme::Usage::Accent)
            .with_auto_text_color(true)
            .with_skip_tabbing(true)
            .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f)));

    auto icon_row_container =
        hstack(context, mk(row4.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(600), pixels(100)})
                   .with_align_items(AlignItems::Center)
                   .with_justify_content(JustifyContent::SpaceAround)
                   .with_custom_background(
                       afterhours::colors::darken(theme.surface, 0.9f))
                   .with_padding(Spacing::xs)
                   .with_rounded_corners(RoundedCorners().all_round())
                   .with_roundness(0.08f)
                   .with_no_wrap()
                   .with_debug_name("icon_row_container"));

    // Helper lambda to create icon with label
    auto create_labeled_icon = [&](int idx, const raylib::Texture2D &tex,
                                   const std::string &label_text) {
      auto icon_wrapper =
          vstack(context, mk(icon_row_container.ent(), idx),
                 ComponentConfig{}
                     .with_size(ComponentSize{pixels(90), pixels(82)})
                     .with_align_items(AlignItems::Center)
                     .with_debug_name("icon_wrapper_" + std::to_string(idx)));

      sprite(context, mk(icon_wrapper.ent(), 0), tex, tex_src(tex),
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(56), pixels(56)})
                 .with_debug_name("icon_" + std::to_string(idx)));

      div(context, mk(icon_wrapper.ent(), 1),
          ComponentConfig{}
              .with_label(label_text)
              .with_size(ComponentSize{pixels(84), pixels(22)})
              .with_custom_text_color(theme.font)
              .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
              .with_skip_tabbing(true)
              .with_debug_name("label_" + std::to_string(idx)));
    };

    create_labeled_icon(0, gear_tex, "Settings");
    create_labeled_icon(1, star_tex, "Favorites");
    create_labeled_icon(2, trophy_tex, "Awards");
    create_labeled_icon(3, home_tex, "Home");
    create_labeled_icon(4, play_tex, "Play");
  }
};

REGISTER_EXAMPLE_SCREEN(images, "Component Galleries",
                        "Image, sprite, and image_button demo", ImageShowcase)
