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
  std::string readonly_text = "readonly text here";
  std::string disabled_text = "disabled text here";

  bool show_password = false;
  std::string status_message = "";

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::midnight();
    theme.secondary = {32, 34, 46, 255};
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;

    afterhours::Color field_bg = {32, 34, 46, 255};
    afterhours::Color field_border = {105, 112, 138, 255};
    afterhours::Color card_bg = {16, 17, 24, 255};
    afterhours::Color muted_text = {120, 130, 158, 255};
    afterhours::Color toggle_bg = {101, 107, 141, 255};

    auto root = div(
        context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
            .with_background(Theme::Usage::Background)
            .with_justify_content(JustifyContent::Center)
            .with_align_items(AlignItems::Center)
            .with_debug_name("text_input_root"));

    auto card = vstack(
        context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(480), screen_pct(0.95f)})
            .with_custom_background(card_bg)
            .with_rounded_corners(RoundedCorners().all_round())
            .with_roundness(0.04f)
            .with_padding(Padding{.top = pixels(20), .bottom = pixels(12),
                                  .left = pixels(32), .right = pixels(32)})
            .with_no_wrap()
            .with_debug_name("card"));

    // Title
    div(context, mk(card.ent(), 0),
        ComponentConfig{}
            .with_label("Edit Profile")
            .with_size(ComponentSize{percent(1.0f), pixels(32)})
            .with_background(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, pixels(22.0f))
            .with_alignment(TextAlignment::Left)
            .with_skip_tabbing(true)
            .with_margin(Margin{.bottom = pixels(2)}));

    // Subtitle
    div(context, mk(card.ent(), 1),
        ComponentConfig{}
            .with_label("Update your account information")
            .with_size(ComponentSize{percent(1.0f), pixels(18)})
            .with_background(Theme::Usage::None)
            .with_custom_text_color(muted_text)
            .with_font(UIComponent::DEFAULT_FONT, pixels(12.0f))
            .with_alignment(TextAlignment::Left)
            .with_skip_tabbing(true)
            .with_margin(Margin{.bottom = pixels(12)}));

    // Height fits the fields (children()) rather than a hardcoded pixels(400)
    // that was smaller than the 7 fields (~478px) — the overflow used to spill
    // into the footer. See docs/LAYOUT_AUDIT.md.
    auto body = vstack(
        context, mk(card.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), children()})
            .with_no_wrap()
            .with_justify_content(JustifyContent::FlexStart)
            .with_debug_name("body"));

    auto make_field = [&](int idx, const std::string &label_text,
                          const std::string &placeholder, std::string &value,
                          std::optional<char> mask = std::nullopt) {
      div(context, mk(body.ent(), idx * 3),
          ComponentConfig{}
              .with_label(label_text)
              .with_size(ComponentSize{percent(1.0f), pixels(18)})
              .with_background(Theme::Usage::None)
              .with_font(UIComponent::DEFAULT_FONT, pixels(12.0f))
              .with_alignment(TextAlignment::Left)
              .with_skip_tabbing(true)
              .with_margin(Margin{.top = pixels(idx == 0 ? 0 : 12),
                                  .bottom = pixels(4)}));

      auto cfg = ComponentConfig{}
                     .with_size(ComponentSize{percent(1.0f), pixels(36)})
                     .with_custom_background(field_bg)
                     .with_font(UIComponent::DEFAULT_FONT, pixels(15.0f))
                     .with_rounded_corners(RoundedCorners().all_round())
                     .with_roundness(0.10f)
                     .with_border(field_border, 1.0f)
                     .with_debug_name(label_text + "_input");

      if (mask) cfg.with_mask_char(*mask);

      auto result =
          text_input(context, mk(body.ent(), idx * 3 + 1), value, cfg);

      if (value.empty()) {
        div(context, mk(result.ent(), 10),
            ComponentConfig{}
                .with_label(placeholder)
                .with_size(ComponentSize{percent(1.0f), pixels(15)})
                .with_background(Theme::Usage::None)
                .with_custom_text_color(afterhours::Color{105, 112, 138, 200})
                .with_font(UIComponent::DEFAULT_FONT, pixels(13.0f))
                .with_alignment(TextAlignment::Left)
                .with_absolute_position(14.f, 5.f)
                .with_skip_tabbing(true)
                .with_render_layer(5)
                .with_debug_name(label_text + "_placeholder"));
      }

      return result;
    };

    if (make_field(0, "Username", "Enter username", username)) {
      status_message = "Username: " + username;
    }

    if (make_field(1, "Email", "user@example.com", email)) {
      status_message = "Email: " + email;
    }

    div(context, mk(body.ent(), 6),
        ComponentConfig{}
            .with_label("Password")
            .with_size(ComponentSize{percent(1.0f), pixels(18)})
            .with_background(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, pixels(12.0f))
            .with_alignment(TextAlignment::Left)
            .with_skip_tabbing(true)
            .with_margin(Margin{.top = pixels(12), .bottom = pixels(4)}));

    auto pw_row = hstack(
        context, mk(body.ent(), 7),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(36)})
            .with_custom_background(field_bg)
            .with_rounded_corners(RoundedCorners().all_round())
            .with_roundness(0.10f)
            .with_border(field_border, 1.0f)
            .with_debug_name("pw_row"));

    auto pw_cfg = ComponentConfig{}
                      .with_size(ComponentSize{expand(), pixels(36)})
                      .with_background(Theme::Usage::None)
                      .with_font(UIComponent::DEFAULT_FONT, pixels(15.0f))
                      .with_debug_name("Password_input");

    if (!show_password) pw_cfg.with_mask_char('*');

    auto pw_result = text_input(context, mk(pw_row.ent(), 0), password, pw_cfg);
    if (pw_result) {
      status_message = "Password changed";
    }

    if (password.empty()) {
      div(context, mk(pw_result.ent(), 10),
          ComponentConfig{}
              .with_label("Enter password")
              .with_size(ComponentSize{percent(1.0f), pixels(15)})
              .with_background(Theme::Usage::None)
              .with_custom_text_color(afterhours::Color{105, 112, 138, 200})
              .with_font(UIComponent::DEFAULT_FONT, pixels(13.0f))
              .with_alignment(TextAlignment::Left)
              .with_absolute_position(14.f, 5.f)
              .with_skip_tabbing(true)
              .with_render_layer(5)
              .with_debug_name("Password_placeholder"));
    }

    if (button(context, mk(pw_row.ent(), 1),
               ComponentConfig{}
                   .with_label(show_password ? "Hide" : "Show")
                   .with_size(ComponentSize{pixels(52), pixels(36)})
                   .with_custom_background(toggle_bg)
                   .with_font(UIComponent::DEFAULT_FONT, pixels(12.0f))
                   .with_rounded_corners(
                       RoundedCorners().top_right(ROUND).bottom_right(ROUND))
                   .with_roundness(0.10f)
                   .with_debug_name("pw_toggle"))) {
      show_password = !show_password;
    }

    // Bio
    if (make_field(3, "Bio", "Tell us about yourself", bio)) {
      status_message = "Bio: " + bio;
    }

    // Search
    if (make_field(4, "Search", "Type to search...", search_query)) {
      status_message = "Search: " + search_query;
    }

    // Readonly field
    {
      div(context, mk(body.ent(), 15),
          ComponentConfig{}
              .with_label("Readonly Field")
              .with_size(ComponentSize{percent(1.0f), pixels(18)})
              .with_background(Theme::Usage::None)
              .with_font(UIComponent::DEFAULT_FONT, pixels(12.0f))
              .with_alignment(TextAlignment::Left)
              .with_skip_tabbing(true)
              .with_margin(Margin{.top = pixels(12), .bottom = pixels(4)}));

      text_input(context, mk(body.ent(), 16), readonly_text,
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(1.0f), pixels(36)})
                     .with_custom_background(field_bg)
                     .with_font(UIComponent::DEFAULT_FONT, pixels(15.0f))
                     .with_rounded_corners(RoundedCorners().all_round())
                     .with_roundness(0.10f)
                     .with_border(field_border, 1.0f)
                     .with_readonly()
                     .with_debug_name("Readonly_input"));
    }

    // Disabled field
    {
      div(context, mk(body.ent(), 17),
          ComponentConfig{}
              .with_label("Disabled Field")
              .with_size(ComponentSize{percent(1.0f), pixels(18)})
              .with_background(Theme::Usage::None)
              .with_font(UIComponent::DEFAULT_FONT, pixels(12.0f))
              .with_alignment(TextAlignment::Left)
              .with_skip_tabbing(true)
              .with_margin(Margin{.top = pixels(12), .bottom = pixels(4)}));

      text_input(context, mk(body.ent(), 18), disabled_text,
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(1.0f), pixels(36)})
                     .with_custom_background(field_bg)
                     .with_font(UIComponent::DEFAULT_FONT, pixels(15.0f))
                     .with_rounded_corners(RoundedCorners().all_round())
                     .with_roundness(0.10f)
                     .with_border(field_border, 1.0f)
                     .with_disabled(true)
                     .with_debug_name("Disabled_input"));
    }

    if (!status_message.empty()) {
      div(context, mk(body.ent(), 20),
          ComponentConfig{}
              .with_label(status_message)
              .with_size(ComponentSize{percent(1.0f), pixels(20)})
              .with_background(Theme::Usage::None)
              .with_font(UIComponent::DEFAULT_FONT, pixels(11.0f))
              .with_custom_text_color(afterhours::Color{130, 135, 158, 200})
              .with_margin(Margin{.top = pixels(12)})
              .with_alignment(TextAlignment::Left)
              .with_skip_tabbing(true)
              .with_debug_name("status_bar"));
    }

    auto footer = hstack(
        context, mk(card.ent(), 3),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
            .with_justify_content(JustifyContent::SpaceBetween)
            .with_align_items(AlignItems::Center)
            .with_margin(Margin{.top = pixels(8)})
            .with_debug_name("footer"));

    div(context, mk(footer.ent(), 0),
        ComponentConfig{}
            .with_label("Tab | Ctrl+A/C/V/X")
            .with_size(ComponentSize{expand(), pixels(20)})
            .with_background(Theme::Usage::None)
            .with_custom_text_color(afterhours::Color{120, 130, 158, 255})
            .with_font(UIComponent::DEFAULT_FONT, pixels(10.0f))
            .with_alignment(TextAlignment::Left)
            .with_skip_tabbing(true)
            .with_debug_name("kbd_hints"));

    auto btn_row = hstack(
        context, mk(footer.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(176), pixels(32)})
            .with_justify_content(JustifyContent::FlexEnd)
            .with_align_items(AlignItems::Center)
            .with_debug_name("btn_row"));

    if (button(context, mk(btn_row.ent(), 0),
               ComponentConfig{}
                   .with_label("Cancel")
                   .with_size(ComponentSize{pixels(80), pixels(32)})
                   .with_custom_background(card_bg)
                   .with_border(field_border, 1.0f)
                   .with_font(UIComponent::DEFAULT_FONT, pixels(13.0f))
                   .with_rounded_corners(RoundedCorners().all_round())
                   .with_roundness(0.10f)
                   .with_margin(Margin{.right = pixels(12)})
                   .with_debug_name("btn_cancel"))) {
      username = "";
      email = "";
      password = "";
      bio = "";
      search_query = "";
      status_message = "";
    }

    if (button(context, mk(btn_row.ent(), 1),
               ComponentConfig{}
                   .with_label("Save")
                   .with_size(ComponentSize{pixels(80), pixels(32)})
                   .with_custom_background(theme.accent)
                   .with_custom_text_color(theme.darkfont)
                   .with_font(UIComponent::DEFAULT_FONT, pixels(13.0f))
                   .with_rounded_corners(RoundedCorners().all_round())
                   .with_roundness(0.10f)
                   .with_debug_name("btn_save"))) {
      status_message = "Saved: " + username + " / " + email;
    }
  }
};

REGISTER_EXAMPLE_SCREEN(text_input, "Component Galleries",
                        "Text input fields with cursor and keyboard support",
                        TextInputDemo)
