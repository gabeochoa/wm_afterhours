#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/text_input/text_input.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct TextInputSizes : ScreenSystem<UIContext<InputAction>> {
  static constexpr int NUM_SIZES = 7;
  std::string values[NUM_SIZES] = {"", "", "", "", "", "", ""};

  struct SizeSpec {
    const char *label;
    float height;
  };

  static constexpr SizeSpec specs[NUM_SIZES] = {
      {"24px", 24},  {"32px", 32},  {"40px", 40},  {"48px", 48},
      {"60px", 60},  {"80px", 80},  {"100px", 100},
  };

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::midnight();
    theme.secondary = {32, 34, 46, 255};
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;

    afterhours::Color field_border = {105, 112, 138, 255};
    afterhours::Color card_bg = {16, 17, 24, 255};
    afterhours::Color label_color = {120, 130, 158, 255};

    auto root = div(
        context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
            .with_background(Theme::Usage::Background)
            .with_justify_content(JustifyContent::Center)
            .with_align_items(AlignItems::Center)
            .with_debug_name("sizes_root"));

    auto card = vstack(
        context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(560), screen_pct(0.95f)})
            .with_custom_background(card_bg)
            .with_rounded_corners(RoundedCorners().all_round())
            .with_roundness(0.04f)
            .with_padding(Padding{.top = pixels(20), .bottom = pixels(12),
                                  .left = pixels(32), .right = pixels(32)})
            .with_no_wrap()
            .with_debug_name("sizes_card"));

    div(context, mk(card.ent(), 0),
        ComponentConfig{}
            .with_label("Text Input Sizes")
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_background(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, pixels(22.0f))
            .with_alignment(TextAlignment::Left)
            .with_skip_tabbing(true)
            .with_margin(Margin{.bottom = pixels(2)}));

    div(context, mk(card.ent(), 1),
        ComponentConfig{}
            .with_label("Cursor and selection scale with field height")
            .with_size(ComponentSize{percent(1.0f), pixels(18)})
            .with_background(Theme::Usage::None)
            .with_custom_text_color(label_color)
            .with_font(UIComponent::DEFAULT_FONT, pixels(12.0f))
            .with_alignment(TextAlignment::Left)
            .with_skip_tabbing(true)
            .with_margin(Margin{.bottom = pixels(12)}));

    auto body = vstack(
        context, mk(card.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(460)})
            .with_no_wrap()
            .with_justify_content(JustifyContent::FlexStart)
            .with_debug_name("sizes_body"));

    for (int i = 0; i < NUM_SIZES; i++) {
      float h = specs[i].height;

      auto row = hstack(
          context, mk(body.ent(), i * 2),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), pixels(h)})
              .with_align_items(AlignItems::Center)
              .with_margin(Margin{.top = pixels(i == 0 ? 0 : 6)})
              .with_debug_name(std::string("row_") + specs[i].label));

      div(context, mk(row.ent(), 0),
          ComponentConfig{}
              .with_label(specs[i].label)
              .with_size(ComponentSize{pixels(56), pixels(h)})
              .with_background(Theme::Usage::None)
              .with_custom_text_color(label_color)
              .with_font(UIComponent::DEFAULT_FONT, pixels(11.0f))
              .with_alignment(TextAlignment::Right)
              .with_skip_tabbing(true)
              .with_margin(Margin{.right = pixels(8)})
              .with_debug_name(std::string("label_") + specs[i].label));

      auto cfg = ComponentConfig{}
                     .with_size(ComponentSize{expand(), pixels(h)})
                     .with_placeholder("Type here...")
                     .with_rounded_corners(RoundedCorners().all_round())
                     .with_roundness(0.10f)
                     .with_border(field_border, 1.0f)
                     .with_debug_name(std::string("input_") + specs[i].label);

      text_input(context, mk(row.ent(), 1), values[i], cfg);
    }
  }
};

REGISTER_EXAMPLE_SCREEN(text_input_sizes, "Component Galleries",
                        "Text inputs at various heights showing cursor scaling",
                        TextInputSizes)
