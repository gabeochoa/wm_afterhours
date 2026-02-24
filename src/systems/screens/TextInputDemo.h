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
  std::string username = "";
  std::string email = "";
  std::string password = "";
  std::string search_query = "";
  std::string bio = "";

  bool show_password = false;
  std::string status_message = "";

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::midnight();
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;

    auto accent_dim = afterhours::colors::darken(theme.accent, 0.6f);
    auto surface_light = afterhours::colors::lighten(theme.surface, 1.5f);

    // Full-screen centered layout
    auto root = div(
        context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
            .with_background(Theme::Usage::Background)
            .with_justify_content(JustifyContent::Center)
            .with_align_items(AlignItems::Center)
            .with_debug_name("text_input_root"));

    // Card container
    auto card = vstack(
        context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(480), pixels(640)})
            .with_custom_background(surface_light)
            .with_rounded_corners(RoundedCorners().all_round())
            .with_roundness(0.06f)
            .with_padding(Padding{.top = pixels(0), .bottom = pixels(16),
                                  .left = pixels(0), .right = pixels(0)})
            .with_no_wrap()
            .with_debug_name("card"));

    // Header bar with accent gradient
    auto header = div(
        context, mk(card.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(56)})
            .with_custom_background(theme.primary)
            .with_rounded_corners(
                RoundedCorners().top_left(ROUND).top_right(ROUND))
            .with_roundness(0.06f)
            .with_padding(Padding{.left = pixels(24), .right = pixels(24)})
            .with_justify_content(JustifyContent::FlexStart)
            .with_align_items(AlignItems::Center)
            .with_debug_name("header"));

    div(context, mk(header.ent(), 0),
        ComponentConfig{}
            .with_label("Edit Profile")
            .with_size(ComponentSize{pixels(200), pixels(32)})
            .with_background(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, pixels(22.0f))
            .with_skip_tabbing(true));

    // Form body with padding
    auto body = vstack(
        context, mk(card.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), expand()})
            .with_padding(Padding{.top = pixels(20), .bottom = pixels(8),
                                  .left = pixels(28), .right = pixels(28)})
            .with_no_wrap()
            .with_justify_content(JustifyContent::FlexStart)
            .with_debug_name("body"));

    auto make_field = [&](int idx, const std::string &label_text,
                          std::string &value,
                          std::optional<char> mask = std::nullopt) -> bool {
      div(context, mk(body.ent(), idx * 2),
          ComponentConfig{}
              .with_label(label_text)
              .with_size(ComponentSize{percent(1.0f), pixels(22)})
              .with_background(Theme::Usage::None)
              .with_font(UIComponent::DEFAULT_FONT, pixels(14.0f))
              .with_skip_tabbing(true)
              .with_margin(Margin{.top = pixels(idx == 0 ? 0 : 12),
                                  .bottom = pixels(4)}));

      auto cfg = ComponentConfig{}
                     .with_size(ComponentSize{percent(1.0f), pixels(40)})
                     .with_custom_background(
                         afterhours::colors::darken(theme.surface, 0.5f))
                     .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
                     .with_rounded_corners(RoundedCorners().all_round())
                     .with_roundness(0.12f)
                     .with_debug_name(label_text + "_input");

      if (mask) cfg.with_mask_char(*mask);

      return text_input(context, mk(body.ent(), idx * 2 + 1), value, cfg);
    };

    if (make_field(0, "Username", username)) {
      status_message = "Username: " + username;
    }

    if (make_field(1, "Email", email)) {
      status_message = "Email: " + email;
    }

    // Password with toggle
    div(context, mk(body.ent(), 4),
        ComponentConfig{}
            .with_label("Password")
            .with_size(ComponentSize{percent(1.0f), pixels(22)})
            .with_background(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, pixels(14.0f))
            .with_skip_tabbing(true)
            .with_margin(Margin{.top = pixels(12), .bottom = pixels(4)}));

    auto pw_row = hstack(
        context, mk(body.ent(), 5),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
            .with_debug_name("pw_row"));

    auto pw_cfg = ComponentConfig{}
                      .with_size(ComponentSize{expand(), pixels(40)})
                      .with_custom_background(
                          afterhours::colors::darken(theme.surface, 0.5f))
                      .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
                      .with_rounded_corners(
                          RoundedCorners().top_left(ROUND).bottom_left(ROUND))
                      .with_roundness(0.12f)
                      .with_debug_name("Password_input");

    if (!show_password) pw_cfg.with_mask_char('*');

    if (text_input(context, mk(pw_row.ent(), 0), password, pw_cfg)) {
      status_message = "Password changed";
    }

    if (button(context, mk(pw_row.ent(), 1),
               ComponentConfig{}
                   .with_label(show_password ? "Hide" : "Show")
                   .with_size(ComponentSize{pixels(60), pixels(40)})
                   .with_custom_background(accent_dim)
                   .with_font(UIComponent::DEFAULT_FONT, pixels(13.0f))
                   .with_rounded_corners(
                       RoundedCorners().top_right(ROUND).bottom_right(ROUND))
                   .with_roundness(0.12f)
                   .with_debug_name("pw_toggle"))) {
      show_password = !show_password;
    }

    // Bio / notes
    if (make_field(3, "Bio", bio)) {
      status_message = "Bio: " + bio;
    }

    // Search
    if (make_field(4, "Search", search_query)) {
      status_message = "Search: " + search_query;
    }

    // Status bar
    if (!status_message.empty()) {
      div(context, mk(body.ent(), 10),
          ComponentConfig{}
              .with_label(status_message)
              .with_size(ComponentSize{percent(1.0f), pixels(24)})
              .with_custom_background(
                  afterhours::colors::darken(theme.surface, 0.3f))
              .with_font(UIComponent::DEFAULT_FONT, pixels(13.0f))
              .with_rounded_corners(RoundedCorners().all_round())
              .with_roundness(0.08f)
              .with_margin(Margin{.top = pixels(12)})
              .with_padding(Padding{.left = pixels(10)})
              .with_skip_tabbing(true)
              .with_debug_name("status_bar"));
    }

    // Footer buttons
    auto footer = hstack(
        context, mk(card.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(48)})
            .with_justify_content(JustifyContent::FlexEnd)
            .with_align_items(AlignItems::Center)
            .with_padding(Padding{.right = pixels(28)})
            .with_debug_name("footer"));

    if (button(context, mk(footer.ent(), 0),
               ComponentConfig{}
                   .with_label("Cancel")
                   .with_size(ComponentSize{pixels(80), pixels(36)})
                   .with_custom_background(surface_light)
                   .with_font(UIComponent::DEFAULT_FONT, pixels(14.0f))
                   .with_rounded_corners(RoundedCorners().all_round())
                   .with_roundness(0.10f)
                   .with_margin(Margin{.right = pixels(8)})
                   .with_debug_name("btn_cancel"))) {
      username = "";
      email = "";
      password = "";
      bio = "";
      search_query = "";
      status_message = "";
    }

    if (button(context, mk(footer.ent(), 1),
               ComponentConfig{}
                   .with_label("Save")
                   .with_size(ComponentSize{pixels(80), pixels(36)})
                   .with_background(Theme::Usage::Primary)
                   .with_font(UIComponent::DEFAULT_FONT, pixels(14.0f))
                   .with_rounded_corners(RoundedCorners().all_round())
                   .with_roundness(0.10f)
                   .with_debug_name("btn_save"))) {
      status_message = "Saved: " + username + " / " + email;
    }

    // Keyboard hints
    div(context, mk(card.ent(), 3),
        ComponentConfig{}
            .with_label("Tab to navigate  |  Ctrl+A select all  |  "
                         "Ctrl+C/V/X clipboard")
            .with_size(ComponentSize{percent(1.0f), pixels(20)})
            .with_custom_background(
                afterhours::colors::darken(theme.surface, 0.7f))
            .with_font(UIComponent::DEFAULT_FONT, pixels(11.0f))
            .with_padding(Padding{.left = pixels(12)})
            .with_skip_tabbing(true)
            .with_rounded_corners(
                RoundedCorners().bottom_left(ROUND).bottom_right(ROUND))
            .with_roundness(0.06f));
  }
};

REGISTER_EXAMPLE_SCREEN(text_input, "Component Galleries",
                        "Text input fields with cursor and keyboard support",
                        TextInputDemo)
