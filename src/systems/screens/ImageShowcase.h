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
                        .with_size(ComponentSize{percent(1.0f), pixels(90)})
                        .with_custom_background(theme.surface)
                        .with_padding(Spacing::sm)
                        .with_flex_direction(FlexDirection::Row)
                        .with_align_items(AlignItems::Center)
                        .with_debug_name("row1_sprites"));

    div(context, mk(row1.ent(), 0),
        ComponentConfig{}
            .with_label("sprite():")
            .with_size(ComponentSize{pixels(120), pixels(40)})
            .with_custom_background(theme.primary)
            .with_auto_text_color(true)
            .with_skip_tabbing(true)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("sprite_label"));

    sprite(context, mk(row1.ent(), 1), gear_tex, full_src,
           ComponentConfig{}
               .with_size(ComponentSize{pixels(50), pixels(50)})
               .with_margin(Spacing::sm)
               .with_debug_name("sprite_gear"));

    sprite(context, mk(row1.ent(), 2), star_tex, full_src,
           ComponentConfig{}
               .with_size(ComponentSize{pixels(50), pixels(50)})
               .with_margin(Spacing::sm)
               .with_debug_name("sprite_star"));

    sprite(context, mk(row1.ent(), 3), trophy_tex, full_src,
           ComponentConfig{}
               .with_size(ComponentSize{pixels(50), pixels(50)})
               .with_margin(Spacing::sm)
               .with_debug_name("sprite_trophy"));

    sprite(context, mk(row1.ent(), 4), home_tex, full_src,
           ComponentConfig{}
               .with_size(ComponentSize{pixels(50), pixels(50)})
               .with_margin(Spacing::sm)
               .with_debug_name("sprite_home"));

    sprite(context, mk(row1.ent(), 5), play_tex, full_src,
           ComponentConfig{}
               .with_size(ComponentSize{pixels(50), pixels(50)})
               .with_margin(Spacing::sm)
               .with_debug_name("sprite_play"));

    // Row 2: image_button() demo
    auto row2 = div(context, mk(main_container.ent(), 2),
                    ComponentConfig{}
                        .with_size(ComponentSize{percent(1.0f), pixels(90)})
                        .with_custom_background(theme.surface)
                        .with_padding(Spacing::sm)
                        .with_flex_direction(FlexDirection::Row)
                        .with_align_items(AlignItems::Center)
                        .with_margin(Margin{.top = DefaultSpacing::small()})
                        .with_debug_name("row2_buttons"));

    div(context, mk(row2.ent(), 0),
        ComponentConfig{}
            .with_label("image_button():")
            .with_size(ComponentSize{pixels(160), pixels(40)})
            .with_custom_background(theme.accent)
            .with_auto_text_color(true)
            .with_skip_tabbing(true)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("imgbtn_label"));

    if (image_button(context, mk(row2.ent(), 1), gear_tex, full_src,
                     ComponentConfig{}
                         .with_size(ComponentSize{pixels(60), pixels(60)})
                         .with_custom_background(theme.primary)
                         .with_rounded_corners(RoundedCorners().all_round())
                         .with_roundness(0.2f)
                         .with_margin(Spacing::sm)
                         .with_debug_name("imgbtn_gear"))) {
      button_clicks++;
    }

    if (image_button(context, mk(row2.ent(), 2), play_tex, full_src,
                     ComponentConfig{}
                         .with_size(ComponentSize{pixels(60), pixels(60)})
                         .with_custom_background(theme.accent)
                         .with_rounded_corners(RoundedCorners().all_round())
                         .with_roundness(0.2f)
                         .with_margin(Spacing::sm)
                         .with_debug_name("imgbtn_play"))) {
      button_clicks++;
    }

    if (image_button(context, mk(row2.ent(), 3), star_tex, full_src,
                     ComponentConfig{}
                         .with_size(ComponentSize{pixels(60), pixels(60)})
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
            .with_size(ComponentSize{pixels(120), pixels(40)})
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
            .with_label("With BG:")
            .with_size(ComponentSize{pixels(120), pixels(40)})
            .with_custom_background(theme.primary)
            .with_auto_text_color(true)
            .with_skip_tabbing(true)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
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
               .with_size(ComponentSize{pixels(50), pixels(50)})
               .with_debug_name("container_gear"));

    div(context, mk(img_container.ent(), 1),
        ComponentConfig{}
            .with_label("Settings")
            .with_size(ComponentSize{pixels(80), pixels(30)})
            .with_custom_text_color(theme.font)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_margin(Margin{.left = DefaultSpacing::small()})
            .with_skip_tabbing(true)
            .with_debug_name("settings_label"));

    // Row 4: Icon row demo
    auto row4 = div(context, mk(main_container.ent(), 4),
                    ComponentConfig{}
                        .with_size(ComponentSize{percent(1.0f), pixels(90)})
                        .with_custom_background(theme.surface)
                        .with_padding(Spacing::sm)
                        .with_flex_direction(FlexDirection::Row)
                        .with_align_items(AlignItems::Center)
                        .with_margin(Margin{.top = DefaultSpacing::small()})
                        .with_debug_name("row4_iconrow"));

    div(context, mk(row4.ent(), 0),
        ComponentConfig{}
            .with_label("Icon Row:")
            .with_size(ComponentSize{pixels(120), pixels(40)})
            .with_custom_background(theme.accent)
            .with_auto_text_color(true)
            .with_skip_tabbing(true)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_debug_name("iconrow_label"));

    auto icon_row_container =
        div(context, mk(row4.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(350), pixels(70)})
                .with_flex_direction(FlexDirection::Row)
                .with_align_items(AlignItems::Center)
                .with_custom_background(
                    afterhours::colors::darken(theme.surface, 0.9f))
                .with_padding(Spacing::xs)
                .with_rounded_corners(RoundedCorners().all_round())
                .with_roundness(0.08f)
                .with_no_wrap()
                .with_debug_name("icon_row_container"));

    sprite(context, mk(icon_row_container.ent(), 0), gear_tex, full_src,
           ComponentConfig{}
               .with_size(ComponentSize{pixels(32), pixels(32)})
               .with_margin(Spacing::xs)
               .with_debug_name("icon_row_1"));

    sprite(context, mk(icon_row_container.ent(), 1), star_tex, full_src,
           ComponentConfig{}
               .with_size(ComponentSize{pixels(32), pixels(32)})
               .with_margin(Spacing::xs)
               .with_debug_name("icon_row_2"));

    sprite(context, mk(icon_row_container.ent(), 2), trophy_tex, full_src,
           ComponentConfig{}
               .with_size(ComponentSize{pixels(32), pixels(32)})
               .with_margin(Spacing::xs)
               .with_debug_name("icon_row_3"));

    sprite(context, mk(icon_row_container.ent(), 3), home_tex, full_src,
           ComponentConfig{}
               .with_size(ComponentSize{pixels(32), pixels(32)})
               .with_margin(Spacing::xs)
               .with_debug_name("icon_row_4"));

    sprite(context, mk(icon_row_container.ent(), 4), play_tex, full_src,
           ComponentConfig{}
               .with_size(ComponentSize{pixels(32), pixels(32)})
               .with_margin(Spacing::xs)
               .with_debug_name("icon_row_5"));
  }
};

REGISTER_EXAMPLE_SCREEN(images, "Component Galleries",
                        "Image, sprite, and image_button demo", ImageShowcase)
