#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleNineSliceBordersScreen : ScreenSystem<UIContext<InputAction>> {
  // Fantasy parchment aesthetic
  afterhours::Color bg_dark{35, 28, 22, 255};           // Dark wood/leather
  afterhours::Color bg_medium{55, 45, 38, 255};         // Medium brown
  afterhours::Color parchment{245, 235, 220, 255};      // Parchment background
  afterhours::Color gold_accent{218, 165, 32, 255};     // Gold
  afterhours::Color text_dark{45, 35, 25, 255};         // Dark text
  afterhours::Color text_light{235, 225, 210, 255};     // Light text

  bool textures_loaded = false;
  std::string base_path;

  // Panel textures - different styles
  raylib::Texture2D panel_000;  // Simple rounded
  raylib::Texture2D panel_005;  // Ornate corners
  raylib::Texture2D panel_010;  // Double border
  raylib::Texture2D panel_015;  // Thick border
  raylib::Texture2D panel_020;  // Decorative
  raylib::Texture2D panel_025;  // Heavy ornate

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

    base_path = afterhours::files::get_resource_path("kenney", "kenney_fantasy-ui-borders/PNG").string();

    // Default panels
    panel_000 = raylib::LoadTexture((base_path + "/Default/Panel/panel-000.png").c_str());
    panel_005 = raylib::LoadTexture((base_path + "/Default/Panel/panel-005.png").c_str());
    panel_010 = raylib::LoadTexture((base_path + "/Default/Panel/panel-010.png").c_str());
    panel_015 = raylib::LoadTexture((base_path + "/Default/Panel/panel-015.png").c_str());
    panel_020 = raylib::LoadTexture((base_path + "/Default/Panel/panel-020.png").c_str());
    panel_025 = raylib::LoadTexture((base_path + "/Default/Panel/panel-025.png").c_str());

    // Border-only (transparent center)
    border_000 = raylib::LoadTexture((base_path + "/Default/Border/panel-border-000.png").c_str());
    border_005 = raylib::LoadTexture((base_path + "/Default/Border/panel-border-005.png").c_str());
    border_010 = raylib::LoadTexture((base_path + "/Default/Border/panel-border-010.png").c_str());

    // Transparent border
    trans_border_000 = raylib::LoadTexture((base_path + "/Default/Transparent border/panel-transparent-border-000.png").c_str());
    trans_border_010 = raylib::LoadTexture((base_path + "/Default/Transparent border/panel-transparent-border-010.png").c_str());

    // Double-width panels (thicker borders)
    double_panel_000 = raylib::LoadTexture((base_path + "/Double/Panel/panel-000.png").c_str());
    double_panel_010 = raylib::LoadTexture((base_path + "/Double/Panel/panel-010.png").c_str());
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
    theme.roundness = 0.0f;  // Nine-slice handles corners
    context.theme = theme;

    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    // Background
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_width), pixels(screen_height)})
            .with_custom_background(bg_dark)
            .with_debug_name("bg"));

    float start_x = 40.0f;
    float start_y = 70.0f;  // Account for taller title
    float box_width = 140.0f;
    float box_height = 100.0f;
    float gap = 20.0f;
    float label_height = 25.0f;

    // Title - needs to be tall enough for font + 9-slice border insets
    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_label("9-SLICE BORDERS")
            .with_size(ComponentSize{pixels(screen_width - 80.0f), pixels(56)})
            .with_absolute_position()
            .with_translate(start_x, 8.0f)
            .with_nine_slice_border(panel_015, 16)
            .with_font("EqProRounded", 22.0f)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("title"));

    // Row 1: Basic panel styles
    struct PanelDemo {
      std::string label;
      raylib::Texture2D *texture;
      int slice_size;
    };

    PanelDemo row1[] = {
        {"Simple", &panel_000, 16},
        {"Ornate", &panel_005, 16},
        {"Double Line", &panel_010, 16},
        {"Thick", &panel_015, 16},
        {"Decorative", &panel_020, 16},
        {"Heavy", &panel_025, 16},
    };

    // Row 1 label
    div(context, mk(entity, 5),
        ComponentConfig{}
            .with_label("Panel Styles (filled background)")
            .with_size(ComponentSize{pixels(350), pixels(label_height)})
            .with_absolute_position()
            .with_translate(start_x, start_y)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(gold_accent)
            .with_debug_name("row1_label"));

    float row1_y = start_y + label_height + 5;
    for (int i = 0; i < 6; i++) {
      float x = start_x + i * (box_width + gap);
      div(context, mk(entity, 10 + i),
          ComponentConfig{}
              .with_label(row1[i].label)
              .with_size(ComponentSize{pixels(box_width), pixels(box_height)})
              .with_absolute_position()
              .with_translate(x, row1_y)
              .with_nine_slice_border(*row1[i].texture, row1[i].slice_size)
              .with_font(UIComponent::DEFAULT_FONT, 16.0f)
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
            .with_absolute_position()
            .with_translate(start_x, row2_y - label_height - 5)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(gold_accent)
            .with_debug_name("row2_label"));

    PanelDemo row2[] = {
        {"Border A", &border_000, 16},
        {"Border B", &border_005, 16},
        {"Border C", &border_010, 16},
        {"Trans A", &trans_border_000, 16},
        {"Trans B", &trans_border_010, 16},
    };

    for (int i = 0; i < 5; i++) {
      float x = start_x + i * (box_width + gap);
      div(context, mk(entity, 30 + i),
          ComponentConfig{}
              .with_label(row2[i].label)
              .with_size(ComponentSize{pixels(box_width), pixels(box_height)})
              .with_absolute_position()
              .with_translate(x, row2_y)
              .with_nine_slice_border(*row2[i].texture, row2[i].slice_size)
              .with_font(UIComponent::DEFAULT_FONT, 16.0f)
              .with_custom_text_color(text_light)
              .with_alignment(TextAlignment::Center)
              .with_debug_name("border_" + std::to_string(i)));
    }

    // Row 3: Different sizes to show stretching
    float row3_y = row2_y + box_height + gap + label_height;

    div(context, mk(entity, 45),
        ComponentConfig{}
            .with_label("Different Sizes (9-slice stretching)")
            .with_size(ComponentSize{pixels(400), pixels(label_height)})
            .with_absolute_position()
            .with_translate(start_x, row3_y - label_height - 5)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(gold_accent)
            .with_debug_name("row3_label"));

    // Small
    div(context, mk(entity, 50),
        ComponentConfig{}
            .with_label("Small")
            .with_size(ComponentSize{pixels(80), pixels(60)})
            .with_absolute_position()
            .with_translate(start_x, row3_y)
            .with_nine_slice_border(panel_010, 16)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("size_small"));

    // Medium
    div(context, mk(entity, 51),
        ComponentConfig{}
            .with_label("Medium")
            .with_size(ComponentSize{pixels(150), pixels(80)})
            .with_absolute_position()
            .with_translate(start_x + 100, row3_y)
            .with_nine_slice_border(panel_010, 16)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("size_medium"));

    // Large
    div(context, mk(entity, 52),
        ComponentConfig{}
            .with_label("Large Panel")
            .with_size(ComponentSize{pixels(220), pixels(100)})
            .with_absolute_position()
            .with_translate(start_x + 270, row3_y)
            .with_nine_slice_border(panel_010, 16)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("size_large"));

    // Wide
    div(context, mk(entity, 53),
        ComponentConfig{}
            .with_label("Wide Panel - Good for menus and headers")
            .with_size(ComponentSize{pixels(400), pixels(60)})
            .with_absolute_position()
            .with_translate(start_x + 510, row3_y)
            .with_nine_slice_border(panel_015, 16)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("size_wide"));

    // Tall
    div(context, mk(entity, 54),
        ComponentConfig{}
            .with_label("Tall")
            .with_size(ComponentSize{pixels(80), pixels(140)})
            .with_absolute_position()
            .with_translate(start_x + 930, row3_y - 40)
            .with_nine_slice_border(panel_005, 16)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("size_tall"));

    // Row 4: Double-width borders and tinting
    float row4_y = row3_y + 120 + gap + label_height;

    div(context, mk(entity, 60),
        ComponentConfig{}
            .with_label("Double-Width & Tinting")
            .with_size(ComponentSize{pixels(350), pixels(label_height)})
            .with_absolute_position()
            .with_translate(start_x, row4_y - label_height - 5)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(gold_accent)
            .with_debug_name("row4_label"));

    // Double-width border
    div(context, mk(entity, 65),
        ComponentConfig{}
            .with_label("Double Width")
            .with_size(ComponentSize{pixels(box_width), pixels(box_height)})
            .with_absolute_position()
            .with_translate(start_x, row4_y)
            .with_nine_slice_border(double_panel_000, 32)  // Double needs larger slice
            .with_font(UIComponent::DEFAULT_FONT, 15.0f)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("double_a"));

    div(context, mk(entity, 66),
        ComponentConfig{}
            .with_label("Double Ornate")
            .with_size(ComponentSize{pixels(box_width), pixels(box_height)})
            .with_absolute_position()
            .with_translate(start_x + box_width + gap, row4_y)
            .with_nine_slice_border(double_panel_010, 32)
            .with_font(UIComponent::DEFAULT_FONT, 15.0f)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("double_b"));

    // Tinted panels
    div(context, mk(entity, 67),
        ComponentConfig{}
            .with_label("Gold Tint")
            .with_size(ComponentSize{pixels(box_width), pixels(box_height)})
            .with_absolute_position()
            .with_translate(start_x + 2 * (box_width + gap), row4_y)
            .with_nine_slice_border(NineSliceBorder::uniform(panel_010, 16, gold_accent))
            .with_font(UIComponent::DEFAULT_FONT, 15.0f)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("tint_gold"));

    div(context, mk(entity, 68),
        ComponentConfig{}
            .with_label("Blue Tint")
            .with_size(ComponentSize{pixels(box_width), pixels(box_height)})
            .with_absolute_position()
            .with_translate(start_x + 3 * (box_width + gap), row4_y)
            .with_nine_slice_border(NineSliceBorder::uniform(panel_010, 16, 
                                afterhours::Color{100, 150, 220, 255}))
            .with_font(UIComponent::DEFAULT_FONT, 15.0f)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("tint_blue"));

    div(context, mk(entity, 69),
        ComponentConfig{}
            .with_label("Red Tint")
            .with_size(ComponentSize{pixels(box_width), pixels(box_height)})
            .with_absolute_position()
            .with_translate(start_x + 4 * (box_width + gap), row4_y)
            .with_nine_slice_border(NineSliceBorder::uniform(panel_010, 16,
                                afterhours::Color{200, 80, 80, 255}))
            .with_font(UIComponent::DEFAULT_FONT, 15.0f)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("tint_red"));

    div(context, mk(entity, 70),
        ComponentConfig{}
            .with_label("Green Tint")
            .with_size(ComponentSize{pixels(box_width), pixels(box_height)})
            .with_absolute_position()
            .with_translate(start_x + 5 * (box_width + gap), row4_y)
            .with_nine_slice_border(NineSliceBorder::uniform(panel_010, 16,
                                afterhours::Color{80, 180, 100, 255}))
            .with_font(UIComponent::DEFAULT_FONT, 15.0f)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("tint_green"));

    // Interactive button example
    float row5_y = row4_y + box_height + gap * 2;

    div(context, mk(entity, 75),
        ComponentConfig{}
            .with_label("Interactive Elements")
            .with_size(ComponentSize{pixels(350), pixels(label_height)})
            .with_absolute_position()
            .with_translate(start_x, row5_y - label_height - 5)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(gold_accent)
            .with_debug_name("row5_label"));

    auto btn_result = button(
        context, mk(entity, 80),
        ComponentConfig{}
            .with_label("Click Me!")
            .with_size(ComponentSize{pixels(180.0f), pixels(50.0f)})
            .with_absolute_position()
            .with_translate(start_x, row5_y)
            .with_nine_slice_border(panel_020, 16)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("nine_slice_button"));

    if (btn_result) {
      log_info("Nine-slice button clicked!");
    }

    // Dialog box example
    div(context, mk(entity, 85),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(350.0f), pixels(120.0f)})
            .with_absolute_position()
            .with_translate(start_x + 200, row5_y)
            .with_nine_slice_border(panel_025, 16)
            .with_padding(Spacing::sm)
            .with_debug_name("dialog_box"));

    div(context, mk(entity, 86),
        ComponentConfig{}
            .with_label("This is a dialog box using 9-slice borders. "
                       "The corners stay crisp while the edges stretch!")
            .with_size(ComponentSize{pixels(330), pixels(100)})
            .with_absolute_position()
            .with_translate(start_x + 210, row5_y + 10)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("dialog_text"));

    // Nested panels example
    div(context, mk(entity, 90),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(280.0f), pixels(120.0f)})
            .with_absolute_position()
            .with_translate(start_x + 570, row5_y)
            .with_nine_slice_border(panel_015, 16)
            .with_debug_name("nested_outer"));

    div(context, mk(entity, 91),
        ComponentConfig{}
            .with_label("Nested Panels")
            .with_size(ComponentSize{pixels(250), pixels(30)})
            .with_absolute_position()
            .with_translate(start_x + 585, row5_y + 10)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(text_dark)
            .with_debug_name("nested_title"));

    div(context, mk(entity, 92),
        ComponentConfig{}
            .with_label("Inner")
            .with_size(ComponentSize{pixels(120.0f), pixels(55.0f)})
            .with_absolute_position()
            .with_translate(start_x + 585, row5_y + 50)
            .with_nine_slice_border(NineSliceBorder::uniform(border_005, 16, 
                                afterhours::Color{180, 140, 80, 255}))
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("nested_inner_a"));

    div(context, mk(entity, 93),
        ComponentConfig{}
            .with_label("Panel")
            .with_size(ComponentSize{pixels(120.0f), pixels(55.0f)})
            .with_absolute_position()
            .with_translate(start_x + 715, row5_y + 50)
            .with_nine_slice_border(NineSliceBorder::uniform(border_010, 16,
                                afterhours::Color{100, 140, 180, 255}))
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_custom_text_color(text_light)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("nested_inner_b"));
  }
};

REGISTER_EXAMPLE_SCREEN(nine_slice_borders, "Components",
                        "9-slice border textures", ExampleNineSliceBordersScreen)

