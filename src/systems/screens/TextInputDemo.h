#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct TextInputDemo : ScreenSystem<UIContext<InputAction>> {
  // Text input values
  std::string username = "";
  std::string email = "";
  std::string password = "";
  std::string search_query = "";
  std::string notes = "Type something here...";

  // Status message
  std::string status_message = "Enter your details above";

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Apply a clean theme
    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;

    // Main container - centered on screen
    auto root =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.90f), screen_pct(0.90f)})
                .with_self_align(SelfAlign::Center)
                .with_custom_background(theme.background)
                .with_roundness(0.08f)
                .with_padding(Spacing::lg)  // Move padding to root
                .with_debug_name("text_input_demo_bg"));

    // Content container - use percent for proper sizing
    auto main_container =
        div(context, mk(root.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                .with_flex_direction(FlexDirection::Column)
                .with_no_wrap()  // Prevent children from wrapping
                .with_debug_name("text_input_main"));

    // Title
    div(context, mk(main_container.ent(), 0),
        ComponentConfig{}
            .with_label("Text Input Demo")
            .with_size(ComponentSize{percent(1.0f), pixels(45)})
            .with_custom_background(theme.surface)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 24.0f)
            .with_margin(Margin{.bottom = DefaultSpacing::small()})
            .with_debug_name("title"));

    // Form container - use percent height for proper containment
    auto form_container =
        div(context, mk(main_container.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.95f), percent(0.75f)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::md)
                .with_flex_direction(FlexDirection::Column)
                .with_justify_content(JustifyContent::FlexStart)
                .with_align_items(AlignItems::FlexStart)  // Align children left
                .with_no_wrap()  // Prevent flex wrapping
                .with_debug_name("form_container"));

    // Helper to create labeled text input with stacked layout (label above input)
    auto make_input_field = [&](int idx, const std::string& label_text,
                                std::string& value, Theme::Usage bg,
                                std::optional<char> mask = std::nullopt) -> bool {
      // Label above the input
      div(context, mk(form_container.ent(), idx * 2),
          ComponentConfig{}
              .with_label(label_text + ":")
              .with_size(ComponentSize{pixels(396), pixels(24)})
              .with_background(Theme::Usage::None)
              .with_font(UIComponent::DEFAULT_FONT, 16.0f)
              .with_skip_tabbing(true)
              .with_debug_name(label_text + "_label"));

      // Text input without internal label
      auto input_config = ComponentConfig{}
          .with_size(ComponentSize{pixels(396), pixels(44)})
          .with_background(bg)
          .with_font(UIComponent::DEFAULT_FONT, 18.0f)
          .with_margin(Margin{.bottom = DefaultSpacing::small()})
          .with_debug_name(label_text + "_input");

      if (mask) {
        input_config.with_mask_char(*mask);
      }

      return text_input(context, mk(form_container.ent(), idx * 2 + 1), value, input_config);
    };

    // Username input
    if (make_input_field(0, "Username", username, Theme::Usage::Primary)) {
      status_message = "Username: " + username;
    }

    // Email input
    if (make_input_field(1, "Email", email, Theme::Usage::Accent)) {
      status_message = "Email: " + email;
    }

    // Password input with masking
    if (make_input_field(2, "Password", password, Theme::Usage::Secondary, '*')) {
      status_message = "Password changed";
    }

    // Separator - indices 0-5 used by 3 label/input pairs above
    separator(context, mk(form_container.ent(), 6),
              SeparatorOrientation::Horizontal,
              ComponentConfig{}.with_margin(Spacing::sm));

    // Search input (no label)
    div(context, mk(form_container.ent(), 7),
        ComponentConfig{}
            .with_label("Search (no label version):")
            .with_size(ComponentSize{pixels(396), pixels(24)})
            .with_skip_tabbing(true)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
            .with_debug_name("search_label"));

    // Search input - no label so field uses full width
    if (text_input(context, mk(form_container.ent(), 8), search_query,
                   ComponentConfig{}
                       .with_size(ComponentSize{pixels(396), pixels(44)})
                       .with_background(Theme::Usage::Primary)
                       .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                       .with_margin(Margin{.bottom = DefaultSpacing::small()})
                       .with_debug_name("search_input"))) {
      status_message = "Searching for: " + search_query;
    }

    // Submit button
    if (button(context, mk(form_container.ent(), 9),
               ComponentConfig{}
                   .with_label("Submit")
                   .with_size(ComponentSize{pixels(200), pixels(50)})
                   .with_background(Theme::Usage::Accent)
                   .with_font(UIComponent::DEFAULT_FONT, 20.0f)
                   .with_margin(Margin{.top = DefaultSpacing::medium()})
                   .with_debug_name("submit_btn"))) {
      status_message = "Submitted! User: " + username + ", Email: " + email;
    }

    // Status display - reduced margin to fit
    div(context, mk(main_container.ent(), 2),
        ComponentConfig{}
            .with_label(status_message)
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
            .with_custom_background(theme.surface)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 16.0f)
            .with_margin(Margin{.top = DefaultSpacing::small()})
            .with_debug_name("status"));

    // Instructions - reduced size to fit
    div(context, mk(main_container.ent(), 3),
        ComponentConfig{}
            .with_label("Click to focus, type to input. Backspace/arrows/Home/End for navigation.")
            .with_size(ComponentSize{percent(1.0f), pixels(35)})
            .with_custom_background(
                afterhours::colors::darken(theme.surface, 0.8f))
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_skip_tabbing(true)
            .with_debug_name("instructions"));
  }
};

REGISTER_EXAMPLE_SCREEN(text_input, "Component Galleries",
                        "Text input fields with cursor and keyboard support",
                        TextInputDemo)
