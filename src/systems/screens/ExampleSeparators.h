#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include "../../theme_presets.h"

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleSeparators : ScreenSystem<UIContext<InputAction>> {
  // Clean modern color scheme
  afterhours::Color bg_slate{248, 250, 252, 255};     // Soft gray-blue
  afterhours::Color card_white{255, 255, 255, 255};   // Pure white
  afterhours::Color text_dark{30, 41, 59, 255};       // Dark slate
  afterhours::Color text_muted{100, 116, 139, 255};   // Muted slate
  afterhours::Color accent_blue{59, 130, 246, 255};   // Vibrant blue
  afterhours::Color accent_green{34, 197, 94, 255};   // Fresh green
  afterhours::Color accent_purple{139, 92, 246, 255}; // Soft purple
  afterhours::Color accent_orange{249, 115, 22, 255}; // Warm orange
  afterhours::Color border_light{180, 190, 210, 255}; // More visible border

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::cozy_kraft();
    context.theme = theme;

    auto *res = afterhours::EntityHelper::get_singleton_cmp<
        afterhours::window_manager::ProvidesCurrentResolution>();
    int screen_width = res ? res->current_resolution.width : 1280;
    int screen_height = res ? res->current_resolution.height : 720;
    float col_width = 520.0f;
    float card_height = 500.0f;
    float col_gap = 40.0f;
    float total_width = col_width * 2 + col_gap;
    float start_x = (screen_width - total_width) / 2.0f;
    float content_y = 100.0f;

    // Background
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(
                ComponentSize{pixels(screen_width), pixels(screen_height)})
            .with_custom_background(bg_slate)
            .with_debug_name("bg"));

    // Title
    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_label("Separator Widget Demo")
            .with_size(ComponentSize{pixels(screen_width), pixels(50)})
            .with_absolute_position(0.0f, 20.0f)
            .with_font("Gaegu-Bold", h720(42.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center));

    // ========== LEFT CARD: Horizontal Separators ==========
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(col_width), pixels(card_height)})
            .with_absolute_position(start_x, content_y)
            .with_custom_background(card_white)
            .with_soft_shadow(6.0f, 10.0f, 25.0f,
                              afterhours::Color{0, 0, 0, 30})
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.06f)
            .with_debug_name("left_card_bg"));

    float y = content_y + 30.0f;
    float pad = 32.0f;
    float item_width = col_width - pad * 2;

    // Section title
    div(context, mk(entity, 11),
        ComponentConfig{}
            .with_label("Horizontal Separators")
            .with_size(ComponentSize{pixels(item_width), pixels(40)})
            .with_absolute_position(start_x + pad, y)
            .with_font("Gaegu-Bold", h720(30.0f))
            .with_custom_text_color(text_dark));
    y += 48.0f;

    // Basic separator
    div(context, mk(entity, 12),
        ComponentConfig{}
            .with_label("Basic separator - default style")
            .with_size(ComponentSize{pixels(item_width), pixels(28)})
            .with_absolute_position(start_x + pad, y)
            .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
            .with_custom_text_color(text_muted));
    y += 36.0f;

    div(context, mk(entity, 13),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(item_width), pixels(3)})
            .with_absolute_position(start_x + pad, y)
            .with_custom_background(border_light));
    y += 32.0f;

    // Thick separator
    div(context, mk(entity, 14),
        ComponentConfig{}
            .with_label("Thick separator - 6 pixels tall")
            .with_size(ComponentSize{pixels(item_width), pixels(28)})
            .with_absolute_position(start_x + pad, y)
            .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
            .with_custom_text_color(text_muted));
    y += 36.0f;

    div(context, mk(entity, 15),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(item_width), pixels(6)})
            .with_absolute_position(start_x + pad, y)
            .with_custom_background(border_light));
    y += 32.0f;

    // Blue separator
    div(context, mk(entity, 16),
        ComponentConfig{}
            .with_label("Custom color - blue accent")
            .with_size(ComponentSize{pixels(item_width), pixels(28)})
            .with_absolute_position(start_x + pad, y)
            .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
            .with_custom_text_color(text_muted));
    y += 36.0f;

    div(context, mk(entity, 17),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(item_width), pixels(3)})
            .with_absolute_position(start_x + pad, y)
            .with_custom_background(accent_blue));
    y += 32.0f;

    // Partial width separator
    div(context, mk(entity, 18),
        ComponentConfig{}
            .with_label("Partial width - 60% centered")
            .with_size(ComponentSize{pixels(item_width), pixels(28)})
            .with_absolute_position(start_x + pad, y)
            .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
            .with_custom_text_color(text_muted));
    y += 36.0f;

    float partial_width = item_width * 0.6f;
    div(context, mk(entity, 19),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(partial_width), pixels(3)})
            .with_absolute_position(start_x + pad + (item_width - partial_width) / 2, y)
            .with_custom_background(accent_purple));
    y += 40.0f;

    // Labeled separator section title
    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_label("Labeled Separators")
            .with_size(ComponentSize{pixels(item_width), pixels(34)})
            .with_absolute_position(start_x + pad, y)
            .with_font("Gaegu-Bold", h720(26.0f))
            .with_custom_text_color(text_dark));
    y += 40.0f;

    // --- OR --- style separator
    float line_width = (item_width - 60) / 2;
    div(context, mk(entity, 21),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(line_width), pixels(3)})
            .with_absolute_position(start_x + pad, y + 12)
            .with_custom_background(border_light));

    div(context, mk(entity, 22),
        ComponentConfig{}
            .with_label("OR")
            .with_size(ComponentSize{pixels(50), pixels(28)})
            .with_absolute_position(start_x + pad + line_width + 5, y)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 23),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(line_width), pixels(3)})
            .with_absolute_position(start_x + pad + line_width + 60, y + 12)
            .with_custom_background(border_light));
    y += 40.0f;

    // --- Settings --- style separator
    float settings_label_width = 110.0f;
    float settings_line_width = (item_width - settings_label_width - 30) / 2;
    div(context, mk(entity, 24),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(settings_line_width), pixels(3)})
            .with_absolute_position(start_x + pad, y + 14)
            .with_custom_background(accent_blue)
            .with_debug_name("settings_line1"));

    div(context, mk(entity, 25),
        ComponentConfig{}
            .with_label("Settings")
            .with_size(ComponentSize{pixels(settings_label_width), pixels(30)})
            .with_absolute_position(start_x + pad + settings_line_width + 15, y)
            .with_font("Gaegu-Bold", h720(22.0f))
            .with_custom_text_color(accent_blue)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 26),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(settings_line_width), pixels(3)})
            .with_absolute_position(start_x + pad + settings_line_width +
                                settings_label_width + 30,
                            y + 14)
            .with_custom_background(accent_blue));

    // ========== RIGHT CARD: Vertical Separators ==========
    float right_x = start_x + col_width + col_gap;
    div(context, mk(entity, 30),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(col_width), pixels(card_height)})
            .with_absolute_position(right_x, content_y)
            .with_custom_background(card_white)
            .with_soft_shadow(6.0f, 10.0f, 25.0f,
                              afterhours::Color{0, 0, 0, 30})
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.06f)
            .with_debug_name("right_card_bg"));

    y = content_y + 30.0f;

    // Section title
    div(context, mk(entity, 31),
        ComponentConfig{}
            .with_label("Vertical Separators")
            .with_size(ComponentSize{pixels(item_width), pixels(40)})
            .with_absolute_position(right_x + pad, y)
            .with_font("Gaegu-Bold", h720(30.0f))
            .with_custom_text_color(text_dark));
    y += 48.0f;

    div(context, mk(entity, 32),
        ComponentConfig{}
            .with_label("Use between navigation items:")
            .with_size(ComponentSize{pixels(item_width), pixels(28)})
            .with_absolute_position(right_x + pad, y)
            .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
            .with_custom_text_color(text_muted));
    y += 40.0f;

    // Navigation row with vertical separators
    float nav_width = item_width;
    float nav_height = 64.0f;
    float nav_item_width = 90.0f;
    float sep_spacing = 12.0f;

    // Nav background
    div(context, mk(entity, 33),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(nav_width), pixels(nav_height)})
            .with_absolute_position(right_x + pad, y)
            .with_custom_background(bg_slate)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.12f)
            .with_debug_name("nav_bg"));

    float nav_x = right_x + pad + 24;  // Added more left padding
    const char *nav_items[] = {"Home", "Settings", "Profile", "Help"};
    afterhours::Color sep_colors[] = {text_muted, accent_green, accent_orange};

    for (int i = 0; i < 4; i++) {
      div(context, mk(entity, 40 + i),
          ComponentConfig{}
              .with_label(nav_items[i])
              .with_size(
                  ComponentSize{pixels(nav_item_width), pixels(nav_height)})
              .with_absolute_position(nav_x, y)
              .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
              .with_custom_text_color(text_dark)
              .with_alignment(TextAlignment::Center)
              .with_debug_name(std::string("nav_") + nav_items[i]));
      nav_x += nav_item_width;

      if (i < 3) {
        // Vertical separator
        div(context, mk(entity, 50 + i),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(3), pixels(40)})
                .with_absolute_position(nav_x + sep_spacing, y + 12)
                .with_custom_background(sep_colors[i])
                .with_debug_name(std::string("vsep_") + std::to_string(i)));
        nav_x += sep_spacing * 2 + 3;
      }
    }
    y += nav_height + 40.0f;

    // Color gallery section
    float gallery_line_width = (item_width - 140) / 2;
    div(context, mk(entity, 60),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(gallery_line_width), pixels(3)})
            .with_absolute_position(right_x + pad, y + 14)
            .with_custom_background(border_light)
            .with_debug_name("gallery_line1"));

    div(context, mk(entity, 61),
        ComponentConfig{}
            .with_label("Color Gallery")
            .with_size(ComponentSize{pixels(130), pixels(30)})
            .with_absolute_position(right_x + pad + gallery_line_width + 5, y)
            .with_font("Gaegu-Bold", h720(22.0f))
            .with_custom_text_color(text_dark)
            .with_alignment(TextAlignment::Center));

    div(context, mk(entity, 62),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(gallery_line_width), pixels(3)})
            .with_absolute_position(right_x + pad + gallery_line_width + 140, y + 14)
            .with_custom_background(border_light));
    y += 40.0f;

    // Color rows
    afterhours::Color colors[] = {accent_blue, accent_green, accent_purple,
                                  accent_orange};
    const char *color_names[] = {"Blue", "Green", "Purple", "Orange"};

    for (int i = 0; i < 4; i++) {
      div(context, mk(entity, 70 + i),
          ComponentConfig{}
              .with_label(color_names[i])
              .with_size(ComponentSize{pixels(90), pixels(32)})
              .with_absolute_position(right_x + pad, y)
              .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
              .with_custom_text_color(text_muted)
              .with_debug_name(std::string("color_") + color_names[i]));

      div(context, mk(entity, 80 + i),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(item_width - 110), pixels(4)})
              .with_absolute_position(right_x + pad + 100, y + 14)
              .with_custom_background(colors[i])
              .with_debug_name(std::string("colorbar_") + color_names[i]));
      y += 40.0f;
    }

    // Footer
    y += 24.0f;
    div(context, mk(entity, 90),
        ComponentConfig{}
            .with_label("Separators help organize and structure UI content")
            .with_size(ComponentSize{pixels(item_width), pixels(30)})
            .with_absolute_position(right_x + pad, y)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
            .with_custom_text_color(text_muted)
            .with_alignment(TextAlignment::Center));
  }
};

REGISTER_EXAMPLE_SCREEN(
    separators, "Component Galleries",
    "Separator widget examples (horizontal, vertical, labeled)",
    ExampleSeparators)
