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

    std::string icon_path =
        afterhours::files::get_resource_path("kenney/kenney_game-icons/PNG/White/2x/", "").string();

    gear_tex = raylib::LoadTexture((icon_path + "gear.png").c_str());
    star_tex = raylib::LoadTexture((icon_path + "star.png").c_str());
    trophy_tex = raylib::LoadTexture((icon_path + "trophy.png").c_str());
    home_tex = raylib::LoadTexture((icon_path + "home.png").c_str());
    play_tex = raylib::LoadTexture((icon_path + "forward.png").c_str());
  }

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context,
                     float) override {
    load_textures();

    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;

    // Main container background - centered on screen with padding
    auto root =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.90f), screen_pct(0.90f)})
                .with_self_align(SelfAlign::Center)
                .with_custom_background(theme.background)
                .with_roundness(0.08f)
                .with_padding(Spacing::lg)
                .with_debug_name("image_bg"));

    // Content container
    auto main_container =
        div(context, mk(root.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                .with_flex_direction(FlexDirection::Column)
                .with_no_wrap()
                .with_debug_name("image_main"));

    // Title
    div(context, mk(main_container.ent(), 0),
        ComponentConfig{}
            .with_label("Image & Sprite Components")
            .with_size(ComponentSize{percent(1.0f), pixels(50)})
            .with_custom_background(theme.surface)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 28.0f)
            .with_margin(Margin{.bottom = DefaultSpacing::medium()})
            .with_debug_name("title"));

    raylib::Rectangle full_src{0, 0, 128, 128};

    // Row 1: sprite() demo
    auto row1 = div(context, mk(main_container.ent(), 1),
                    ComponentConfig{}
                        .with_size(ComponentSize{percent(1.0f), pixels(140)})
                        .with_custom_background(theme.surface)
                        .with_padding(Spacing::sm)
                        .with_flex_direction(FlexDirection::Row)
                        .with_align_items(AlignItems::Center)
                        .with_debug_name("row1_sprites"));

    div(context, mk(row1.ent(), 0),
        ComponentConfig{}
            .with_label("Sprites")
            .with_size(ComponentSize{pixels(120), pixels(48)})
            .with_custom_background(theme.primary)
            .with_auto_text_color(true)
            .with_skip_tabbing(true)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_debug_name("sprite_label"));

    // Helper lambda to create sprite with label for row1
    auto create_labeled_sprite = [&](int idx, const raylib::Texture2D& tex,
                                      const std::string& label_text) {
      auto sprite_wrapper = div(context, mk(row1.ent(), idx),
                                ComponentConfig{}
                                    .with_size(ComponentSize{pixels(90), pixels(120)})
                                    .with_flex_direction(FlexDirection::Column)
                                    .with_align_items(AlignItems::Center)
                                    .with_margin(Spacing::xs)
                                    .with_debug_name("sprite_wrapper_" + std::to_string(idx)));

      sprite(context, mk(sprite_wrapper.ent(), 0), tex, full_src,
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(72), pixels(72)})
                 .with_debug_name("sprite_icon_" + std::to_string(idx)));

      div(context, mk(sprite_wrapper.ent(), 1),
          ComponentConfig{}
              .with_label(label_text)
              .with_size(ComponentSize{pixels(80), pixels(24)})
              .with_custom_text_color(theme.font)
              .with_font(UIComponent::DEFAULT_FONT, 14.0f)
              .with_skip_tabbing(true)
              .with_debug_name("sprite_label_" + std::to_string(idx)));
    };

    create_labeled_sprite(1, gear_tex, "Gear");
    create_labeled_sprite(2, star_tex, "Star");
    create_labeled_sprite(3, trophy_tex, "Trophy");
    create_labeled_sprite(4, home_tex, "Home");
    create_labeled_sprite(5, play_tex, "Play");

    // Row 2: image_button() demo
    auto row2 = div(context, mk(main_container.ent(), 2),
                    ComponentConfig{}
                        .with_size(ComponentSize{percent(1.0f), pixels(110)})
                        .with_custom_background(theme.surface)
                        .with_padding(Spacing::sm)
                        .with_flex_direction(FlexDirection::Row)
                        .with_align_items(AlignItems::Center)
                        .with_margin(Margin{.top = DefaultSpacing::small()})
                        .with_debug_name("row2_buttons"));

    div(context, mk(row2.ent(), 0),
        ComponentConfig{}
            .with_label("Image Buttons")
            .with_size(ComponentSize{pixels(160), pixels(48)})
            .with_custom_background(theme.accent)
            .with_auto_text_color(true)
            .with_skip_tabbing(true)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_debug_name("imgbtn_label"));

    if (image_button(context, mk(row2.ent(), 1), gear_tex, full_src,
                     ComponentConfig{}
                         .with_size(ComponentSize{pixels(80), pixels(80)})
                         .with_custom_background(theme.primary)
                         .with_rounded_corners(RoundedCorners().all_round())
                         .with_roundness(0.2f)
                         .with_margin(Spacing::sm)
                         .with_debug_name("imgbtn_gear"))) {
      button_clicks++;
    }

    if (image_button(context, mk(row2.ent(), 2), play_tex, full_src,
                     ComponentConfig{}
                         .with_size(ComponentSize{pixels(80), pixels(80)})
                         .with_custom_background(theme.accent)
                         .with_rounded_corners(RoundedCorners().all_round())
                         .with_roundness(0.2f)
                         .with_margin(Spacing::sm)
                         .with_debug_name("imgbtn_play"))) {
      button_clicks++;
    }

    if (image_button(context, mk(row2.ent(), 3), star_tex, full_src,
                     ComponentConfig{}
                         .with_size(ComponentSize{pixels(80), pixels(80)})
                         .with_custom_background(theme.secondary)
                         .with_rounded_corners(RoundedCorners().all_round())
                         .with_roundness(0.2f)
                         .with_margin(Spacing::sm)
                         .with_debug_name("imgbtn_star"))) {
      button_clicks++;
    }

    div(context, mk(row2.ent(), 4),
        ComponentConfig{}
            .with_label("Clicks: " + std::to_string(button_clicks))
            .with_size(ComponentSize{pixels(120), pixels(48)})
            .with_custom_background(theme.surface)
            .with_custom_text_color(theme.font)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_margin(Margin{.left = DefaultSpacing::medium()})
            .with_skip_tabbing(true)
            .with_debug_name("click_counter"));

    // Row 3: Image with background demo
    auto row3 = div(context, mk(main_container.ent(), 3),
                    ComponentConfig{}
                        .with_size(ComponentSize{percent(1.0f), pixels(100)})
                        .with_custom_background(theme.surface)
                        .with_padding(Spacing::sm)
                        .with_flex_direction(FlexDirection::Row)
                        .with_align_items(AlignItems::Center)
                        .with_margin(Margin{.top = DefaultSpacing::small()})
                        .with_debug_name("row3_image"));

    div(context, mk(row3.ent(), 0),
        ComponentConfig{}
            .with_label("With Background")
            .with_size(ComponentSize{pixels(160), pixels(48)})
            .with_custom_background(theme.primary)
            .with_auto_text_color(true)
            .with_skip_tabbing(true)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_debug_name("withbg_label"));

    auto img_container =
        div(context, mk(row3.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(200), pixels(70)})
                .with_custom_background(theme.secondary)
                .with_rounded_corners(RoundedCorners().all_round())
                .with_roundness(0.1f)
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Row)
                .with_align_items(AlignItems::Center)
                .with_margin(Spacing::sm)
                .with_debug_name("image_container"));

    sprite(context, mk(img_container.ent(), 0), gear_tex, full_src,
           ComponentConfig{}
               .with_size(ComponentSize{pixels(64), pixels(64)})
               .with_debug_name("container_gear"));

    div(context, mk(img_container.ent(), 1),
        ComponentConfig{}
            .with_label("Settings")
            .with_size(ComponentSize{pixels(100), pixels(36)})
            .with_custom_text_color(theme.font)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_margin(Margin{.left = DefaultSpacing::small()})
            .with_skip_tabbing(true)
            .with_debug_name("settings_label"));

    // Row 4: Icon row demo with labels
    auto row4 = div(context, mk(main_container.ent(), 4),
                    ComponentConfig{}
                        .with_size(ComponentSize{percent(1.0f), pixels(150)})
                        .with_custom_background(theme.surface)
                        .with_padding(Spacing::sm)
                        .with_flex_direction(FlexDirection::Row)
                        .with_align_items(AlignItems::Center)
                        .with_margin(Margin{.top = DefaultSpacing::small()})
                        .with_debug_name("row4_iconrow"));

    div(context, mk(row4.ent(), 0),
        ComponentConfig{}
            .with_label("Icon Row")
            .with_size(ComponentSize{pixels(120), pixels(48)})
            .with_custom_background(theme.accent)
            .with_auto_text_color(true)
            .with_skip_tabbing(true)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_debug_name("iconrow_label"));

    auto icon_row_container =
        div(context, mk(row4.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(520), pixels(120)})
                .with_flex_direction(FlexDirection::Row)
                .with_align_items(AlignItems::Center)
                .with_justify_content(JustifyContent::SpaceAround)
                .with_custom_background(
                    afterhours::colors::darken(theme.surface, 0.9f))
                .with_padding(Spacing::sm)
                .with_rounded_corners(RoundedCorners().all_round())
                .with_roundness(0.08f)
                .with_no_wrap()
                .with_debug_name("icon_row_container"));

    // Helper lambda to create icon with label
    auto create_labeled_icon = [&](int idx, const raylib::Texture2D& tex,
                                    const std::string& label_text) {
      auto icon_wrapper = div(context, mk(icon_row_container.ent(), idx),
                              ComponentConfig{}
                                  .with_size(ComponentSize{pixels(90), pixels(110)})
                                  .with_flex_direction(FlexDirection::Column)
                                  .with_align_items(AlignItems::Center)
                                  .with_debug_name("icon_wrapper_" + std::to_string(idx)));

      sprite(context, mk(icon_wrapper.ent(), 0), tex, full_src,
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(72), pixels(72)})
                 .with_debug_name("icon_" + std::to_string(idx)));

      div(context, mk(icon_wrapper.ent(), 1),
          ComponentConfig{}
              .with_label(label_text)
              .with_size(ComponentSize{pixels(80), pixels(24)})
              .with_custom_text_color(theme.font)
              .with_font(UIComponent::DEFAULT_FONT, 14.0f)
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
