#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct StepperShowcase : ScreenSystem<UIContext<InputAction>> {
  // Left column: various steppers
  size_t resolution_idx = 2;
  std::vector<std::string> resolutions = {"1280x720", "1920x1080", "2560x1440",
                                          "3840x2160"};

  size_t quality_idx = 1;
  std::vector<std::string> qualities = {"Low", "Medium", "High", "Ultra"};

  size_t language_idx = 0;
  std::vector<std::string> languages = {"English", "Deutsch", "Francais",
                                        "Espanol", "Japanese"};

  size_t difficulty_idx = 1;
  std::vector<std::string> difficulties = {"Easy", "Normal", "Hard",
                                           "Nightmare"};

  size_t framerate_idx = 1;
  std::vector<std::string> framerates = {"30", "60", "120", "Unlimited"};

  // Right column: stepper controlling a card display
  size_t card_idx = 0;

  struct CardData {
    std::string title;
    std::string description;
    afterhours::Color color;
  };

  std::vector<CardData> cards = {
      {"Warrior", "Heavy armor, melee attacks, high defense.",
       afterhours::Color{170, 75, 65, 255}},
      {"Mage", "Ranged spells, low defense, high damage.",
       afterhours::Color{60, 80, 180, 255}},
      {"Rogue", "Fast attacks, stealth, critical strikes.",
       afterhours::Color{60, 160, 80, 255}},
      {"Healer", "Support class, restoration, shields.",
       afterhours::Color{180, 160, 50, 255}},
  };

  std::vector<std::string> card_names() {
    std::vector<std::string> names;
    for (auto &c : cards)
      names.push_back(c.title);
    return names;
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::neon_dark();
    theme.roundness = 0.10f;
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;
    UIStylingDefaults::get().set_default_font(UIComponent::DEFAULT_FONT,
                                              pixels(24.0f));

    // Root container
    auto root =
        vstack(context, mk(entity),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.90f), screen_pct(0.90f)})
                .with_self_align(SelfAlign::Center)
                .with_background(Theme::Usage::Background)
                .with_padding(Spacing::sm)
                .with_no_wrap()
                .with_debug_name("root"));

    // Title
    div(context, mk(root.ent()),
        ComponentConfig{}
            .with_label("Stepper Showcase")
            .with_size(ComponentSize{percent(1.0f), pixels(50)})
            .with_background(Theme::Usage::Surface)
            .with_auto_text_color(true)
            .with_font_size(pixels(36.0f))
            .with_alignment(TextAlignment::Center)
            .with_roundness(0.08f)
            .with_margin(Margin{.bottom = DefaultSpacing::small()}));

    // Two-column layout
    auto columns =
        hstack(context, mk(root.ent()),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), percent(0.88f)})
                .with_justify_content(JustifyContent::SpaceBetween)
                .with_no_wrap()
                .with_debug_name("columns"));

    // ========== LEFT COLUMN: Various steppers ==========
    auto left_col =
        vstack(context, mk(columns.ent()),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.47f), percent(1.0f)})
                .with_background(Theme::Usage::Surface)
                .with_padding(Spacing::md)
                .with_roundness(0.06f)
                .with_debug_name("left_col"));

    div(context, mk(left_col.ent()),
        ComponentConfig{}
            .with_label("Stepper Variants")
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
            .with_custom_text_color(afterhours::Color{180, 220, 255, 255})
            .with_font_size(pixels(28.0f))
            .with_margin(Margin{.bottom = DefaultSpacing::small()}));

    // Each stepper with a label
    auto labeled_stepper = [&](int index, const char *label, auto &options,
                               size_t &idx) {
      auto row = hstack(context, mk(left_col.ent(), index),
                     ComponentConfig{}
                         .with_size(ComponentSize{percent(1.0f), pixels(56)})
                         .with_align_items(AlignItems::Center)
                         .with_justify_content(JustifyContent::SpaceBetween)
                         .with_margin(Margin{.bottom = pixels(8)}));

      div(context, mk(row.ent()),
          ComponentConfig{}
              .with_label(label)
              .with_size(ComponentSize{children(), pixels(56)})
              .with_background(Theme::Usage::None)
              .with_custom_text_color(context.theme.font)
              .with_font_size(pixels(24.0f))
              .with_alignment(TextAlignment::Left));

      stepper(context, mk(row.ent()), options, idx,
              ComponentConfig{}
                  .with_size(ComponentSize{pixels(240), pixels(48)})
                  .with_font_size(pixels(24.0f)));
    };

    labeled_stepper(0, "Resolution", resolutions, resolution_idx);
    labeled_stepper(1, "Quality", qualities, quality_idx);
    labeled_stepper(2, "Language", languages, language_idx);
    labeled_stepper(3, "Difficulty", difficulties, difficulty_idx);
    labeled_stepper(4, "Framerate", framerates, framerate_idx);

    // ========== RIGHT COLUMN: Stepper controlling a card ==========
    auto right_col =
        vstack(context, mk(columns.ent()),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.47f), percent(1.0f)})
                .with_background(Theme::Usage::Surface)
                .with_padding(Spacing::md)
                .with_align_items(AlignItems::Center)
                .with_roundness(0.06f)
                .with_debug_name("right_col"));

    div(context, mk(right_col.ent()),
        ComponentConfig{}
            .with_label("Card Selector")
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
            .with_custom_text_color(afterhours::Color{180, 220, 255, 255})
            .with_font_size(pixels(28.0f))
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.bottom = DefaultSpacing::medium()}));

    // Stepper to select card
    auto names = card_names();
    stepper(context, mk(right_col.ent()), names, card_idx,
            ComponentConfig{}
                .with_size(ComponentSize{pixels(300), pixels(56)})
                .with_font_size(pixels(26.0f))
                .with_margin(Margin{.bottom = DefaultSpacing::large()}),
            3);

    // Card display
    auto &card = cards[card_idx % cards.size()];

    auto card_container =
        vstack(context, mk(right_col.ent()),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(300), pixels(220)})
                .with_custom_background(card.color)
                .with_padding(Spacing::lg)
                .with_roundness(0.10f)
                .with_debug_name("card"));

    div(context, mk(card_container.ent()),
        ComponentConfig{}
            .with_label(card.title)
            .with_size(ComponentSize{percent(1.0f), pixels(48)})
            .with_custom_text_color(afterhours::Color{255, 255, 255, 255})
            .with_font_size(pixels(32.0f))
            .with_alignment(TextAlignment::Center)
            .with_background(Theme::Usage::None)
            .with_margin(Margin{.bottom = DefaultSpacing::medium()}));

    div(context, mk(card_container.ent()),
        ComponentConfig{}
            .with_label(card.description)
            .with_size(ComponentSize{percent(1.0f), children()})
            .with_custom_text_color(afterhours::Color{230, 230, 230, 255})
            .with_font_size(pixels(24.0f))
            .with_alignment(TextAlignment::Left)
            .with_background(Theme::Usage::None));
  }
};

REGISTER_EXAMPLE_SCREEN(stepper_showcase, "Component Galleries",
                        "Stepper component with various styles and card selector",
                        StepperShowcase)
