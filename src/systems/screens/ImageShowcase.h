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

    // Main container background - centered with padding
    auto root = div(context, mk(entity, 0),
                    ComponentConfig{}
                        .with_size(ComponentSize{screen_pct(0.90f), screen_pct(0.90f)})
                        .with_self_align(SelfAlign::Center)
                        .with_custom_background(theme.background)
                        .with_roundness(0.08f)
                        .with_padding(Spacing::lg)  // Padding on root
                        .with_debug_name("image_bg"));

    // Content container - no padding since root has it
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
            .with_label("Image & Sprite Components Showcase")
            .with_size(ComponentSize{percent(1.0f), pixels(50)})
            .with_custom_background(theme.surface)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 28.0f)
            .with_margin(Margin{.top = pixels(0),
                                .bottom = DefaultSpacing::medium(),
                                .left = pixels(0),
                                .right = pixels(0)})
            .with_debug_name("title"));

    // Content area - two columns
    auto content =
        div(context, mk(main_container.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(420)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::md)
                .with_flex_direction(FlexDirection::Row)
                .with_justify_content(JustifyContent::SpaceBetween)
                .with_debug_name("content"));

    // Left column - Static images
    auto left_col =
        div(context, mk(content.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.48f), percent(1.0f)})
                .with_custom_background(
                    afterhours::colors::darken(theme.surface, 0.95f))
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("left_column"));

    // Section: sprite()
    div(context, mk(left_col.ent(), 0),
        ComponentConfig{}
            .with_label("sprite() - Display icons from texture")
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_custom_background(theme.primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_margin(Margin{.bottom = DefaultSpacing::small()})
            .with_debug_name("sprite_label"));

    // Row of sprites
    auto sprite_row =
        div(context, mk(left_col.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(80)})
                .with_flex_direction(FlexDirection::Row)
                .with_align_items(AlignItems::Center)
                .with_margin(Margin{.bottom = DefaultSpacing::medium()})
                .with_debug_name("sprite_row"));

    // Display individual sprites
    raylib::Rectangle full_src{0, 0, 128, 128};

    sprite(context, mk(sprite_row.ent(), 0), gear_tex, full_src,
           ComponentConfig{}
               .with_size(ComponentSize{pixels(64), pixels(64)})
               .with_margin(Margin{.right = DefaultSpacing::small()})
               .with_debug_name("sprite_gear"));

    sprite(context, mk(sprite_row.ent(), 1), star_tex, full_src,
           ComponentConfig{}
               .with_size(ComponentSize{pixels(64), pixels(64)})
               .with_margin(Margin{.right = DefaultSpacing::small()})
               .with_debug_name("sprite_star"));

    sprite(context, mk(sprite_row.ent(), 2), trophy_tex, full_src,
           ComponentConfig{}
               .with_size(ComponentSize{pixels(64), pixels(64)})
               .with_margin(Margin{.right = DefaultSpacing::small()})
               .with_debug_name("sprite_trophy"));

    sprite(context, mk(sprite_row.ent(), 3), home_tex, full_src,
           ComponentConfig{}
               .with_size(ComponentSize{pixels(64), pixels(64)})
               .with_debug_name("sprite_home"));

    // Section: image() with texture
    div(context, mk(left_col.ent(), 2),
        ComponentConfig{}
            .with_label("image() - Background texture")
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_custom_background(theme.primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_margin(Margin{.bottom = DefaultSpacing::small()})
            .with_debug_name("image_label"));

    // Simple box with texture as image
    auto img_container =
        div(context, mk(left_col.ent(), 3),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(200), pixels(100)})
                .with_custom_background(theme.secondary)
                .with_rounded_corners(RoundedCorners().all_round())
                .with_roundness(0.1f)
                .with_margin(Margin{.bottom = DefaultSpacing::medium()})
                .with_debug_name("image_with_tex"));

    sprite(context, mk(img_container.ent(), 0), gear_tex, full_src,
           ComponentConfig{}
               .with_size(ComponentSize{pixels(80), pixels(80)})
               .with_debug_name("image_gear"));

    // Right column - Interactive elements
    auto right_col =
        div(context, mk(content.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.48f), percent(1.0f)})
                .with_custom_background(
                    afterhours::colors::darken(theme.surface, 0.95f))
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("right_column"));

    // Section: image_button()
    div(context, mk(right_col.ent(), 0),
        ComponentConfig{}
            .with_label("image_button() - Clickable icons")
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_custom_background(theme.accent)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_margin(Margin{.bottom = DefaultSpacing::small()})
            .with_debug_name("image_button_label"));

    // Row of image buttons
    auto button_row =
        div(context, mk(right_col.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(80)})
                .with_flex_direction(FlexDirection::Row)
                .with_align_items(AlignItems::Center)
                .with_margin(Margin{.bottom = DefaultSpacing::small()})
                .with_debug_name("button_row"));

    if (image_button(context, mk(button_row.ent(), 0), gear_tex, full_src,
                     ComponentConfig{}
                         .with_size(ComponentSize{pixels(64), pixels(64)})
                         .with_custom_background(theme.primary)
                         .with_rounded_corners(RoundedCorners().all_round())
                         .with_roundness(0.2f)
                         .with_margin(Margin{.right = DefaultSpacing::small()})
                         .with_debug_name("imgbtn_gear"))) {
      button_clicks++;
    }

    if (image_button(context, mk(button_row.ent(), 1), play_tex, full_src,
                     ComponentConfig{}
                         .with_size(ComponentSize{pixels(64), pixels(64)})
                         .with_custom_background(theme.accent)
                         .with_rounded_corners(RoundedCorners().all_round())
                         .with_roundness(0.2f)
                         .with_margin(Margin{.right = DefaultSpacing::small()})
                         .with_debug_name("imgbtn_play"))) {
      button_clicks++;
    }

    if (image_button(context, mk(button_row.ent(), 2), star_tex, full_src,
                     ComponentConfig{}
                         .with_size(ComponentSize{pixels(64), pixels(64)})
                         .with_custom_background(theme.secondary)
                         .with_rounded_corners(RoundedCorners().all_round())
                         .with_roundness(0.2f)
                         .with_debug_name("imgbtn_star"))) {
      button_clicks++;
    }

    // Click counter
    div(context, mk(right_col.ent(), 2),
        ComponentConfig{}
            .with_label("Button clicks: " + std::to_string(button_clicks))
            .with_size(ComponentSize{pixels(200), pixels(30)})
            .with_custom_text_color(theme.font)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_margin(Margin{.bottom = DefaultSpacing::large()})
            .with_debug_name("click_counter"));

    // Section: icon_row() - multiple frames
    div(context, mk(right_col.ent(), 3),
        ComponentConfig{}
            .with_label("icon_row() - Multiple icons in row")
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_custom_background(theme.accent)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_margin(Margin{.bottom = DefaultSpacing::small()})
            .with_debug_name("icon_row_label"));

    // icon_row with multiple frames from the same spritesheet
    std::vector<raylib::Rectangle> icon_frames = {
        {0, 0, 128, 128}, // Using same rect since we have separate textures
    };

    // For icon_row demo, we'll just show a simpler display
    auto icon_row_container =
        div(context, mk(right_col.ent(), 4),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(60)})
                .with_flex_direction(FlexDirection::Row)
                .with_align_items(AlignItems::Center)
                .with_custom_background(theme.surface)
                .with_padding(Spacing::sm)
                .with_rounded_corners(RoundedCorners().all_round())
                .with_roundness(0.08f)
                .with_debug_name("icon_row_container"));

    // Manually create an icon row effect
    sprite(context, mk(icon_row_container.ent(), 0), gear_tex, full_src,
           ComponentConfig{}
               .with_size(ComponentSize{pixels(40), pixels(40)})
               .with_margin(Margin{.right = pixels(8)})
               .with_debug_name("icon_row_1"));

    sprite(context, mk(icon_row_container.ent(), 1), star_tex, full_src,
           ComponentConfig{}
               .with_size(ComponentSize{pixels(40), pixels(40)})
               .with_margin(Margin{.right = pixels(8)})
               .with_debug_name("icon_row_2"));

    sprite(context, mk(icon_row_container.ent(), 2), trophy_tex, full_src,
           ComponentConfig{}
               .with_size(ComponentSize{pixels(40), pixels(40)})
               .with_margin(Margin{.right = pixels(8)})
               .with_debug_name("icon_row_3"));

    sprite(context, mk(icon_row_container.ent(), 3), home_tex, full_src,
           ComponentConfig{}
               .with_size(ComponentSize{pixels(40), pixels(40)})
               .with_margin(Margin{.right = pixels(8)})
               .with_debug_name("icon_row_4"));

    sprite(context, mk(icon_row_container.ent(), 4), play_tex, full_src,
           ComponentConfig{}
               .with_size(ComponentSize{pixels(40), pixels(40)})
               .with_debug_name("icon_row_5"));
  }
};

REGISTER_EXAMPLE_SCREEN(images, "Component Galleries",
                        "Image, sprite, and image_button demo", ImageShowcase)

