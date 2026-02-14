#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/text_input/text_input.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct TextInputDemo : ScreenSystem<UIContext<InputAction>> {
  // Text input values with placeholder text
  std::string username = "";
  std::string email = "";
  std::string password = "";
  std::string search_query = "Type to search...";
  std::string notes = "Type something here...";

  // Password visibility toggle
  bool show_password = false;

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
                .with_size(ComponentSize{screen_pct(0.90f), screen_pct(0.98f)})
                .with_self_align(SelfAlign::Center)
                .with_background(Theme::Usage::Background)
                .with_roundness(0.08f)
                .with_padding(Spacing::sm)  // Smaller padding for more content space
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
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
            .with_background(Theme::Usage::Surface)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, h720(24.0f)));

    // Form container - uses flex_grow to fill available space, leaving room for
    // status and instructions at the bottom of main_container
    auto form_container =
        div(context, mk(main_container.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.95f), expand()})
                .with_background(Theme::Usage::Surface)
                .with_padding(Spacing::xs)
                .with_flex_direction(FlexDirection::Column)
                .with_justify_content(JustifyContent::FlexStart)
                .with_align_items(AlignItems::FlexStart)  // Align children left
                .with_self_align(SelfAlign::Center)  // Center the form container
                .with_no_wrap()  // Prevent flex wrapping
                .with_debug_name("form_container"));

    // Helper to create labeled text input with stacked layout (label above input)
    auto make_input_field = [&](int idx, const std::string& label_text,
                                std::string& value, Theme::Usage bg,
                                std::optional<char> mask = std::nullopt) -> bool {
      // Label above the input - prominent font for clear field identification
      div(context, mk(form_container.ent(), idx * 2),
          ComponentConfig{}
              .with_label(label_text + ":")
              .with_size(ComponentSize{pixels(396), pixels(34)})
              .with_background(Theme::Usage::None)
              .with_font(UIComponent::DEFAULT_FONT, h720(28.0f))
              .with_skip_tabbing(true)
              .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
              .with_debug_name(label_text + "_label"));

      // Text input without internal label
      // Note: with_rounded_corners and with_roundness ensure the focus ring matches the rounded input field
      auto input_config = ComponentConfig{}
          .with_size(ComponentSize{pixels(396), pixels(38)})
          .with_background(bg)
          .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
          .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
          .with_rounded_corners(RoundedCorners().all_round())
          .with_roundness(0.15f)
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

    // Password input with masking and show/hide toggle
    // Password label
    div(context, mk(form_container.ent(), 4),
        ComponentConfig{}
            .with_label("Password:")
            .with_size(ComponentSize{pixels(396), pixels(34)})
            .with_background(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, h720(28.0f))
            .with_skip_tabbing(true)
            .with_margin(Margin{.bottom = DefaultSpacing::tiny()}));

    // Password row container (input + toggle button)
    auto password_row =
        hstack(context, mk(form_container.ent(), 5),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(396), pixels(38)})
                .with_justify_content(JustifyContent::SpaceBetween)
                .with_align_items(AlignItems::Center)
                .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
                .with_debug_name("password_row"));

    // Password text input (narrower to make room for toggle)
    auto password_config = ComponentConfig{}
        .with_size(ComponentSize{pixels(310), pixels(38)})
        .with_background(Theme::Usage::Secondary)
        .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
        .with_rounded_corners(RoundedCorners().all_round())
        .with_roundness(0.15f)
        .with_debug_name("password_input");

    // Apply mask only when not showing password
    if (!show_password) {
      password_config.with_mask_char('*');
    }

    if (text_input(context, mk(password_row.ent(), 0), password, password_config)) {
      status_message = "Password changed";
    }

    // Show/Hide toggle button
    if (button(context, mk(password_row.ent(), 1),
               ComponentConfig{}
                   .with_label(show_password ? "Hide" : "Show")
                   .with_size(ComponentSize{pixels(76), pixels(38)})
                   .with_background(Theme::Usage::Accent)
                   .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
                   .with_rounded_corners(RoundedCorners().all_round())
                   .with_roundness(0.15f)
                   .with_debug_name("password_toggle"))) {
      show_password = !show_password;
      status_message = show_password ? "Password visible" : "Password hidden";
    }

    // Separator - indices 0-5 used by 3 label/input pairs above
    separator(context, mk(form_container.ent(), 6),
              SeparatorOrientation::Horizontal);

    // Search input (no label)
    div(context, mk(form_container.ent(), 7),
        ComponentConfig{}
            .with_label("Search (no label version):")
            .with_size(ComponentSize{pixels(396), pixels(34)})
            .with_skip_tabbing(true)
            .with_font(UIComponent::DEFAULT_FONT, h720(28.0f))
            .with_margin(Margin{.bottom = DefaultSpacing::tiny()}));

    // Search input - no label so field uses full width
    if (text_input(context, mk(form_container.ent(), 8), search_query,
                   ComponentConfig{}
                       .with_size(ComponentSize{pixels(396), pixels(38)})
                       .with_background(Theme::Usage::Primary)
                       .with_font(UIComponent::DEFAULT_FONT, h720(18.0f))
                       .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
                       .with_rounded_corners(RoundedCorners().all_round())
                       .with_roundness(0.15f)
                       .with_debug_name("search_input"))) {
      status_message = "Searching for: " + search_query;
    }

    // Submit button
    if (button(context, mk(form_container.ent(), 9),
               ComponentConfig{}
                   .with_label("Submit")
                   .with_size(ComponentSize{pixels(200), pixels(44)})
                   .with_background(Theme::Usage::Accent)
                   .with_font(UIComponent::DEFAULT_FONT, h720(20.0f))
                   .with_margin(Margin{.top = DefaultSpacing::tiny()})
                   .with_debug_name("submit_btn"))) {
      status_message = "Submitted! User: " + username + ", Email: " + email;
    }

    // Status display - compact margin to fit within container
    div(context, mk(main_container.ent(), 2),
        ComponentConfig{}
            .with_label(status_message)
            .with_size(ComponentSize{percent(1.0f), pixels(36)})
            .with_background(Theme::Usage::Surface)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f)));

    // Instructions - brief contextual navigation hint
    div(context, mk(main_container.ent(), 3),
        ComponentConfig{}
            .with_label("Tab / Enter")
            .with_size(ComponentSize{percent(1.0f), pixels(24)})
            .with_custom_background(
                afterhours::colors::darken(theme.surface, 0.8f))
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_skip_tabbing(true));

    // ========== FOOTER: OK / Cancel / Apply ==========
    auto footer = hstack(context, mk(main_container.ent(), 4),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(36)})
            .with_justify_content(JustifyContent::FlexEnd)
            .with_align_items(AlignItems::Center)
            .with_padding(Padding{.right = DefaultSpacing::medium()})
            .with_background(Theme::Usage::None)
            .with_debug_name("dialog_footer"));

    button(context, mk(footer.ent(), 0),
        ComponentConfig{}
            .with_label("OK")
            .with_size(ComponentSize{pixels(80), pixels(30)})
            .with_background(Theme::Usage::Primary)
            .with_margin(Margin{.right = DefaultSpacing::small()})
            .with_debug_name("btn_ok"));

    button(context, mk(footer.ent(), 1),
        ComponentConfig{}
            .with_label("Cancel")
            .with_size(ComponentSize{pixels(80), pixels(30)})
            .with_background(Theme::Usage::Surface)
            .with_margin(Margin{.right = DefaultSpacing::small()})
            .with_debug_name("btn_cancel"));

    button(context, mk(footer.ent(), 2),
        ComponentConfig{}
            .with_label("Apply")
            .with_size(ComponentSize{pixels(80), pixels(30)})
            .with_background(Theme::Usage::Surface)
            .with_debug_name("btn_apply"));
  }
};

REGISTER_EXAMPLE_SCREEN(text_input, "Component Galleries",
                        "Text input fields with cursor and keyboard support",
                        TextInputDemo)
