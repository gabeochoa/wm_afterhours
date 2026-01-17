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

    // Main container
    auto root =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.92f), screen_pct(0.88f)})
                .with_custom_background(theme.background)
                .with_roundness(0.08f)
                .with_debug_name("text_input_demo_bg"));

    // Content container with padding
    auto main_container =
        div(context, mk(root.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                .with_padding(Spacing::lg)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("text_input_main"));

    // Title
    div(context, mk(main_container.ent(), 0),
        ComponentConfig{}
            .with_label("Text Input Component Demo")
            .with_size(ComponentSize{percent(1.0f), pixels(50)})
            .with_custom_background(theme.surface)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 28.0f)
            .with_margin(Margin{.top = pixels(0),
                                .bottom = DefaultSpacing::medium(),
                                .left = pixels(0),
                                .right = pixels(0)})
            .with_debug_name("title"));

    // Form container
    auto form_container =
        div(context, mk(main_container.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.9f), pixels(400)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::md)
                .with_flex_direction(FlexDirection::Column)
                .with_justify_content(JustifyContent::FlexStart)
                .with_debug_name("form_container"));

    // Username input
    if (text_input(context, mk(form_container.ent(), 0), username,
                   ComponentConfig{}
                       .with_label("Username")
                       .with_size(ComponentSize{pixels(400), pixels(45)})
                       .with_background(Theme::Usage::Primary)
                       .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                       .with_margin(Spacing::sm)
                       .with_debug_name("username_input"))) {
      status_message = "Username: " + username;
    }

    // Email input
    if (text_input(context, mk(form_container.ent(), 1), email,
                   ComponentConfig{}
                       .with_label("Email")
                       .with_size(ComponentSize{pixels(400), pixels(45)})
                       .with_background(Theme::Usage::Accent)
                       .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                       .with_margin(Spacing::sm)
                       .with_debug_name("email_input"))) {
      status_message = "Email: " + email;
    }

    // Password input with masking
    if (text_input(context, mk(form_container.ent(), 2), password,
                   ComponentConfig{}
                       .with_label("Password")
                       .with_size(ComponentSize{pixels(400), pixels(45)})
                       .with_background(Theme::Usage::Secondary)
                       .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                       .with_margin(Spacing::sm)
                       .with_mask_char('*')
                       .with_debug_name("password_input"))) {
      status_message = "Password changed";
    }

    // Separator
    separator(context, mk(form_container.ent(), 3),
              SeparatorOrientation::Horizontal,
              ComponentConfig{}.with_margin(Spacing::md));

    // Search input (no label)
    div(context, mk(form_container.ent(), 4),
        ComponentConfig{}
            .with_label("Search (no label version):")
            .with_size(ComponentSize{pixels(400), pixels(30)})
            .with_skip_tabbing(true)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
            .with_debug_name("search_label"));

    if (text_input(context, mk(form_container.ent(), 5), search_query,
                   ComponentConfig{}
                       .with_size(ComponentSize{pixels(400), pixels(45)})
                       .with_background(Theme::Usage::Primary)
                       .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                       .with_margin(Spacing::sm)
                       .with_debug_name("search_input"))) {
      status_message = "Searching for: " + search_query;
    }

    // Submit button
    if (button(context, mk(form_container.ent(), 6),
               ComponentConfig{}
                   .with_label("Submit")
                   .with_size(ComponentSize{pixels(200), pixels(50)})
                   .with_background(Theme::Usage::Accent)
                   .with_font(UIComponent::DEFAULT_FONT, 20.0f)
                   .with_margin(Margin{.top = DefaultSpacing::medium()})
                   .with_debug_name("submit_btn"))) {
      status_message = "Submitted! User: " + username + ", Email: " + email;
    }

    // Status display
    div(context, mk(main_container.ent(), 2),
        ComponentConfig{}
            .with_label(status_message)
            .with_size(ComponentSize{percent(1.0f), pixels(50)})
            .with_custom_background(theme.surface)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_margin(Margin{.top = DefaultSpacing::medium(),
                                .bottom = pixels(0),
                                .left = pixels(0),
                                .right = pixels(0)})
            .with_debug_name("status"));

    // Instructions
    div(context, mk(main_container.ent(), 3),
        ComponentConfig{}
            .with_label("Click a field to focus, type to input. Backspace to "
                        "delete. Arrow keys to move cursor.")
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
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
