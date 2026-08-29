#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleNineSliceBordersScreen : ScreenSystem<UIContext<InputAction>> {
  // Fantasy parchment aesthetic
  afterhours::Color bg_dark{35, 28, 22, 255};      // Dark wood/leather
  afterhours::Color bg_medium{55, 45, 38, 255};    // Medium brown
  afterhours::Color parchment{245, 235, 220, 255}; // Parchment background
  afterhours::Color gold_accent{230, 185, 60,
                                255}; // Gold (brightened for contrast)
  afterhours::Color text_dark{45, 35, 25, 255};     // Dark text
  afterhours::Color text_light{235, 225, 210, 255}; // Light text

  // High-contrast text colors for tinted panels (accessibility)
  afterhours::Color text_on_gold{30, 20, 10, 255};    // Dark brown on gold
  afterhours::Color text_on_blue{255, 255, 255, 255}; // White on blue
  afterhours::Color text_on_red{255, 255, 255, 255};  // White on red
  afterhours::Color text_on_green{20, 40, 20, 255};   // Dark green on green

  bool textures_loaded = false;
  std::string base_path;

  // Panel textures - different styles
  raylib::Texture2D panel_000; // Simple rounded
  raylib::Texture2D panel_005; // Ornate corners
  raylib::Texture2D panel_010; // Double border
  raylib::Texture2D panel_015; // Thick border
  raylib::Texture2D panel_020; // Decorative
  raylib::Texture2D panel_025; // Heavy ornate

  // Border-only textures (transparent center)
  raylib::Texture2D border_000;
  raylib::Texture2D border_005;
  raylib::Texture2D border_010;

  // Transparent border (center visible)
  raylib::Texture2D trans_border_000;
  raylib::Texture2D trans_border_010;

  // Double-width versions
  raylib::Texture2D double_panel_000;
  raylib::Texture2D double_panel_010;

  void load_textures_if_needed() {
    if (textures_loaded)
      return;
    textures_loaded = true;

    base_path = afterhours::files::get_resource_path(
                    "kenney", "kenney_fantasy-ui-borders/PNG")
                    .string();

    // Default panels
    panel_000 = raylib::LoadTexture(
        (base_path + "/Default/Panel/panel-000.png").c_str());
    panel_005 = raylib::LoadTexture(
        (base_path + "/Default/Panel/panel-005.png").c_str());
    panel_010 = raylib::LoadTexture(
        (base_path + "/Default/Panel/panel-010.png").c_str());
    panel_015 = raylib::LoadTexture(
        (base_path + "/Default/Panel/panel-015.png").c_str());
    panel_020 = raylib::LoadTexture(
        (base_path + "/Default/Panel/panel-020.png").c_str());
    panel_025 = raylib::LoadTexture(
        (base_path + "/Default/Panel/panel-025.png").c_str());

    // Border-only (transparent center)
    border_000 = raylib::LoadTexture(
        (base_path + "/Default/Border/panel-border-000.png").c_str());
    border_005 = raylib::LoadTexture(
        (base_path + "/Default/Border/panel-border-005.png").c_str());
    border_010 = raylib::LoadTexture(
        (base_path + "/Default/Border/panel-border-010.png").c_str());

    // Transparent border
    trans_border_000 = raylib::LoadTexture(
        (base_path +
         "/Default/Transparent border/panel-transparent-border-000.png")
            .c_str());
    trans_border_010 = raylib::LoadTexture(
        (base_path +
         "/Default/Transparent border/panel-transparent-border-010.png")
            .c_str());

    // Double-width panels (thicker borders)
    double_panel_000 = raylib::LoadTexture(
        (base_path + "/Double/Panel/panel-000.png").c_str());
    double_panel_010 = raylib::LoadTexture(
        (base_path + "/Double/Panel/panel-010.png").c_str());
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    load_textures_if_needed();

    Theme theme;
    theme.font = text_light;
    theme.darkfont = text_dark;
    theme.font_muted = afterhours::Color{180, 170, 155, 255};
    theme.background = bg_dark;
    theme.surface = bg_medium;
    theme.primary = gold_accent;
    theme.secondary = afterhours::Color{139, 90, 43, 255};
    theme.accent = afterhours::Color{200, 150, 50, 255};
    theme.error = afterhours::Color{180, 60, 60, 255};
    theme.roundness = 0.0f;
    context.theme = theme;

    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    // Background
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(
                ComponentSize{pixels(screen_width), pixels(screen_height)})
            .with_custom_background(bg_dark)
            .with_debug_name("bg"));

    float start_x = 60.0f;
    float start_y = 60.0f; // Reduced to fit more content
    float box_width = 140.0f;
    float box_height = 85.0f;   // Reduced from 100
    float gap = 15.0f;          // Reduced from 20
    float label_height = 22.0f; // Reduced from 25

    // Title - needs to be tall enough for font + 9-slice border insets
    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_label("9-SLICE BORDERS")
            .with_size(ComponentSize{pixels(screen_width - 80.0f), pixels(56)})
            .with_absolute_position(start_x, 8.0f)
            .with_nine_slice_border(panel_015, 16)
            .with_font("EqProRounded", h720(22.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center));

    // Row 1: Basic panel styles
    struct PanelDemo {
      std::string label;
      raylib::Texture2D *texture;
      int slice_size;
    };

    PanelDemo row1[] = {
        {"Simple", &panel_000, 16},      {"Ornate", &panel_005, 16},
        {"Double Line", &panel_010, 16}, {"Thick", &panel_015, 16},
        {"Decorative", &panel_020, 16},  {"Heavy", &panel_025, 16},
    };

    // Row 1 label
    div(context, mk(entity, 5),
        ComponentConfig{}
            .with_label("Panel Styles (filled background)")
            .with_size(ComponentSize{pixels(350), pixels(label_height)})
            .with_absolute_position(start_x, start_y)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(gold_accent));

    float row1_y = start_y + label_height + 5;
    for (int i = 0; i < 6; i++) {
      float x = start_x + i * (box_width + gap);
      button(
          context, mk(entity, 10 + i),
          ComponentConfig{}
              .with_label(row1[i].label)
              .with_size(ComponentSize{pixels(box_width), pixels(box_height)})
              .with_absolute_position(x, row1_y)
              .with_nine_slice_border(*row1[i].texture, row1[i].slice_size)
              .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
              .with_custom_text_color(text_dark)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("panel_" + std::to_string(i)));
    }

    // Row 2: Border-only styles (transparent centers)
    float row2_y = row1_y + box_height + gap + label_height;

    div(context, mk(entity, 25),
        ComponentConfig{}
            .with_label("Border Only (transparent center)")
            .with_size(ComponentSize{pixels(350), pixels(label_height)})
            .with_absolute_position(start_x, row2_y - label_height - 5)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(gold_accent));

    PanelDemo row2[] = {
        {"Border A", &border_000, 16},      {"Border B", &border_005, 16},
        {"Border C", &border_010, 16},      {"Trans A", &trans_border_000, 16},
        {"Trans B", &trans_border_010, 16},
    };

    // Increased font size for better label readability (was 18.0f)
    constexpr float border_label_font_size = 20.0f;
    for (int i = 0; i < 5; i++) {
      float x = start_x + i * (box_width + gap);
      // A transparent centre shows the light fill, so light text vanishes there.
      const bool transparent_centre = row2[i].texture == &trans_border_000 ||
                                      row2[i].texture == &trans_border_010;
      div(context, mk(entity, 30 + i),
          ComponentConfig{}
              .with_label(row2[i].label)
              .with_size(ComponentSize{pixels(box_width), pixels(box_height)})
              .with_absolute_position(x, row2_y)
              .with_nine_slice_border(*row2[i].texture, row2[i].slice_size)
              .with_font(UIComponent::DEFAULT_FONT, border_label_font_size)
              .with_custom_text_color(transparent_centre ? text_dark
                                                         : text_light)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("border_" + std::to_string(i)));
    }

    // Row 3: Different sizes to show stretching
    float row3_y = row2_y + box_height + gap + label_height;

    div(context, mk(entity, 45),
        ComponentConfig{}
            .with_label("Different Sizes (9-slice stretching)")
            .with_size(ComponentSize{pixels(400), pixels(label_height)})
            .with_absolute_position(start_x, row3_y - label_height - 5)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_custom_text_color(gold_accent));

    // Small
    div(context, mk(entity, 50),
        ComponentConfig{}
            .with_label("Small")
            .with_size(ComponentSize{pixels(80), pixels(55)})
            .with_absolute_position(start_x, row3_y)
            .with_nine_slice_border(panel_010, 16)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center));

    // Medium
    div(context, mk(entity, 51),
        ComponentConfig{}
            .with_label("Medium")
            .with_size(ComponentSize{pixels(130), pixels(70)})
            .with_absolute_position(start_x + 95, row3_y)
            .with_nine_slice_border(panel_010, 16)
            .with_font(UIComponent::DEFAULT_FONT, h720(15.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center));

    // Large
    div(context, mk(entity, 52),
        ComponentConfig{}
            .with_label("Large Panel")
            .with_size(ComponentSize{pixels(180), pixels(85)})
            .with_absolute_position(start_x + 240, row3_y)
            .with_nine_slice_border(panel_010, 16)
            .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center));

    // Wide
    div(context, mk(entity, 53),
        ComponentConfig{}
            .with_label("Wide Panel - Good for menus")
            .with_size(ComponentSize{pixels(350), pixels(55)})
            .with_absolute_position(start_x + 440, row3_y)
            .with_nine_slice_border(panel_015, 16)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center));

    // Tall
    div(context, mk(entity, 54),
        ComponentConfig{}
            .with_label("Tall")
            .with_size(ComponentSize{pixels(70), pixels(110)})
            .with_absolute_position(start_x + 810, row3_y - 25)
            .with_nine_slice_border(panel_005, 16)
            .with_font(UIComponent::DEFAULT_FONT, h720(13.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center));

    // Row 4: Double-width borders and tinting
    float row4_y = row3_y + 100 + gap + label_height;

    div(context, mk(entity, 60),
        ComponentConfig{}
            .with_label("Double-Width & Tinting")
            .with_size(ComponentSize{pixels(350), pixels(label_height)})
            .with_absolute_position(start_x, row4_y - label_height - 5)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(gold_accent));

    // Double-width border
    div(context, mk(entity, 65),
        ComponentConfig{}
            .with_label("Double")
            .with_size(ComponentSize{pixels(box_width), pixels(box_height)})
            .with_absolute_position(start_x, row4_y)
            .with_nine_slice_border(double_panel_000,
                                    32) // Double needs larger slice
            .with_font(UIComponent::DEFAULT_FONT, h720(13.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 66),
        ComponentConfig{}
            .with_label("Ornate")
            .with_size(ComponentSize{pixels(box_width), pixels(box_height)})
            .with_absolute_position(start_x + box_width + gap + 20.0f, row4_y)
            .with_nine_slice_border(double_panel_010, 32)
            .with_font(UIComponent::DEFAULT_FONT, h720(13.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center));

    // Tinted panels - using high-contrast text colors for accessibility
    div(context, mk(entity, 67),
        ComponentConfig{}
            .with_label("Gold Tint")
            .with_size(ComponentSize{pixels(box_width), pixels(box_height)})
            .with_absolute_position(start_x + 2 * (box_width + gap), row4_y)
            .with_nine_slice_border(
                NineSliceBorder::uniform(panel_010, 16, gold_accent))
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(text_on_gold)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 68),
        ComponentConfig{}
            .with_label("Blue Tint")
            .with_size(ComponentSize{pixels(box_width), pixels(box_height)})
            .with_absolute_position(start_x + 3 * (box_width + gap), row4_y)
            .with_nine_slice_border(NineSliceBorder::uniform(
                panel_010, 16, afterhours::Color{100, 150, 220, 255}))
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(text_on_blue)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 69),
        ComponentConfig{}
            .with_label("Red Tint")
            .with_size(ComponentSize{pixels(box_width), pixels(box_height)})
            .with_absolute_position(start_x + 4 * (box_width + gap), row4_y)
            .with_nine_slice_border(NineSliceBorder::uniform(
                panel_010, 16, afterhours::Color{200, 80, 80, 255}))
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(text_on_red)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 70),
        ComponentConfig{}
            .with_label("Green Tint")
            .with_size(ComponentSize{pixels(box_width), pixels(box_height)})
            .with_absolute_position(start_x + 5 * (box_width + gap), row4_y)
            .with_nine_slice_border(NineSliceBorder::uniform(
                panel_010, 16, afterhours::Color{80, 180, 100, 255}))
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(text_on_green)
            .with_alignment(TextAlignment::Center));

    // Interactive button example
    float row5_y = row4_y + box_height + gap + 20;

    div(context, mk(entity, 75),
        ComponentConfig{}
            .with_label("Interactive Elements")
            .with_size(ComponentSize{pixels(350), pixels(label_height)})
            .with_absolute_position(start_x, row5_y - label_height - 5)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(gold_accent));

    auto btn_result =
        button(context, mk(entity, 80),
               ComponentConfig{}
                   .with_label("Click Me!")
                   .with_size(ComponentSize{pixels(180.0f), pixels(54.0f)})
                   .with_absolute_position(start_x, row5_y)
                   .with_nine_slice_border(panel_020, 16)
                   .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
                   .with_custom_text_color(text_dark)
                   .with_alignment(TextAlignment::Center));

    if (btn_result) {
      log_info("Nine-slice button clicked!");
    }

    // Dialog box example - positioned next to button with proper spacing
    div(context, mk(entity, 85),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(300.0f), pixels(100.0f)})
            .with_absolute_position(start_x + 200, row5_y)
            .with_custom_background(parchment)
            .with_nine_slice_border(border_010, 16)
            .with_padding(Spacing::sm)
            .with_debug_name("dialog_box"));

    div(context, mk(entity, 86),
        ComponentConfig{}
            .with_label("9-slice dialog: corners stay crisp!")
            .with_size(ComponentSize{pixels(270), pixels(70)})
            .with_absolute_position(start_x + 215, row5_y + 15)
            .with_font(UIComponent::DEFAULT_FONT, h720(13.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Left));

    // Nested panels example - with visible outer panel
    div(context, mk(entity, 90),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(280.0f), pixels(100.0f)})
            .with_absolute_position(start_x + 520, row5_y)
            .with_custom_background(parchment)
            .with_nine_slice_border(border_010, 16)
            .with_debug_name("nested_outer"));

    div(context, mk(entity, 91),
        ComponentConfig{}
            .with_label("Nested Panels")
            .with_size(ComponentSize{pixels(250), pixels(25)})
            .with_absolute_position(start_x + 535, row5_y + 8)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 92),
        ComponentConfig{}
            .with_label("Inner")
            .with_size(ComponentSize{pixels(110.0f), pixels(50.0f)})
            .with_absolute_position(start_x + 535, row5_y + 40)
            .with_nine_slice_border(NineSliceBorder::uniform(
                border_005, 16, afterhours::Color{180, 140, 80, 255}))
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 93),
        ComponentConfig{}
            .with_label("Panel")
            .with_size(ComponentSize{pixels(110.0f), pixels(50.0f)})
            .with_absolute_position(start_x + 675, row5_y + 40)
            .with_nine_slice_border(NineSliceBorder::uniform(
                border_010, 16, afterhours::Color{100, 140, 180, 255}))
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center));
  }
};

REGISTER_EXAMPLE_SCREEN(nine_slice_borders, "Components",
                        "9-slice border textures",
                        ExampleNineSliceBordersScreen)
