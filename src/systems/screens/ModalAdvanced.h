#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/modal.h>
#include <afterhours/src/plugins/ui/text_input/text_input.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ModalAdvanced : ScreenSystem<UIContext<InputAction>> {
  // Modal visibility
  bool show_login = false;
  bool show_wizard = false;
  bool show_settings = false;
  bool show_feedback = false;

  // Login form state
  std::string login_username = "";
  std::string login_password = "";
  bool login_remember = false;
  std::string login_error = "";
  std::string login_status = "";

  // Wizard state
  int wizard_step = 0;
  std::string wizard_name = "";
  std::string wizard_email = "";
  size_t wizard_theme_idx = 0;
  bool wizard_notifications = true;
  std::string wizard_status = "";

  // Settings state
  float settings_volume = 0.7f;
  size_t settings_lang_idx = 0;
  bool settings_dark_mode = true;
  bool settings_notifs = true;
  // Saved values for reset
  float saved_volume = 0.7f;
  size_t saved_lang_idx = 0;
  bool saved_dark_mode = true;
  bool saved_notifs = true;
  std::string settings_status = "";

  // Feedback form state
  std::string feedback_subject = "";
  size_t feedback_category_idx = 0;
  float feedback_severity = 0.5f;
  std::string feedback_message = "";
  std::string feedback_error = "";
  std::string feedback_status = "";

  // Modal entity IDs
  static constexpr int MODAL_LOGIN = 200;
  static constexpr int MODAL_WIZARD = 201;
  static constexpr int MODAL_SETTINGS = 202;
  static constexpr int MODAL_FEEDBACK = 203;
  static constexpr int CL = 1001; // Content layer

  // Inset from 100% to leave room for focus ring (~10px total: 4px offset + 3px ring + 3px outline)
  static constexpr float FOCUS_SAFE_WIDTH = 0.92f;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::ocean_navy();
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;
    UIStylingDefaults::get().set_default_font(UIComponent::DEFAULT_FONT,
                                              pixels(16.0f));

    auto root =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.90f), screen_pct(0.90f)})
                .with_background(Theme::Usage::Background)
                .with_roundness(0.08f)
                .with_self_align(SelfAlign::Center)
                .with_padding(Spacing::lg)
                .with_debug_name("adv_modal_bg"));

    auto main = vstack(
        context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
            .with_justify_content(JustifyContent::SpaceAround)
            .with_no_wrap()
            .with_debug_name("adv_modal_main"));

    // Title
    div(context, mk(main.ent(), 0),
        ComponentConfig{}
            .with_label("Advanced Modals")
            .with_size(ComponentSize{percent(1.0f), pixels(50)})
            .with_background(Theme::Usage::Surface)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, pixels(28.0f)));

    // 2x2 grid of trigger buttons
    auto grid_top = hstack(
        context, mk(main.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(80)})
            .with_background(Theme::Usage::Surface)
            .with_padding(Spacing::sm)
            .with_align_items(AlignItems::Center)
            .with_justify_content(JustifyContent::SpaceAround)
            .with_margin(Margin{.top = DefaultSpacing::small()})
            .with_debug_name("grid_top"));

    if (button(context, mk(grid_top.ent(), 0),
               ComponentConfig{}
                   .with_label("Login Form")
                   .with_size(ComponentSize{pixels(220), pixels(50)})
                   .with_background(Theme::Usage::Primary)
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
                   .with_roundness(0.08f)
                   .with_cursor(CursorType::Pointer)
                   .with_debug_name("btn_login"))) {
      show_login = true;
      login_error = "";
    }

    if (button(context, mk(grid_top.ent(), 1),
               ComponentConfig{}
                   .with_label("Multi-Step Wizard")
                   .with_size(ComponentSize{pixels(220), pixels(50)})
                   .with_background(Theme::Usage::Accent)
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
                   .with_roundness(0.08f)
                   .with_cursor(CursorType::Pointer)
                   .with_debug_name("btn_wizard"))) {
      show_wizard = true;
      wizard_step = 0;
    }

    auto grid_bot = hstack(
        context, mk(main.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(80)})
            .with_background(Theme::Usage::Surface)
            .with_padding(Spacing::sm)
            .with_align_items(AlignItems::Center)
            .with_justify_content(JustifyContent::SpaceAround)
            .with_margin(Margin{.top = DefaultSpacing::small()})
            .with_debug_name("grid_bot"));

    if (button(context, mk(grid_bot.ent(), 0),
               ComponentConfig{}
                   .with_label("Settings Panel")
                   .with_size(ComponentSize{pixels(220), pixels(50)})
                   .with_background(Theme::Usage::Secondary)
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
                   .with_roundness(0.08f)
                   .with_cursor(CursorType::Pointer)
                   .with_debug_name("btn_settings"))) {
      show_settings = true;
    }

    if (button(context, mk(grid_bot.ent(), 1),
               ComponentConfig{}
                   .with_label("Feedback Form")
                   .with_size(ComponentSize{pixels(220), pixels(50)})
                   .with_custom_background(afterhours::Color{140, 70, 70, 255})
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, pixels(18.0f))
                   .with_roundness(0.08f)
                   .with_cursor(CursorType::Pointer)
                   .with_debug_name("btn_feedback"))) {
      show_feedback = true;
      feedback_error = "";
    }

    // Status line
    std::string status = login_status;
    if (!wizard_status.empty())
      status += (status.empty() ? "" : " | ") + wizard_status;
    if (!settings_status.empty())
      status += (status.empty() ? "" : " | ") + settings_status;
    if (!feedback_status.empty())
      status += (status.empty() ? "" : " | ") + feedback_status;
    if (status.empty())
      status = "Open a modal above to get started";

    div(context, mk(main.ent(), 3),
        ComponentConfig{}
            .with_label(status)
            .with_size(ComponentSize{percent(1.0f), pixels(40)})
            .with_custom_background(
                afterhours::colors::lighten(theme.background, 0.08f))
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_roundness(0.1f)
            .with_font(UIComponent::DEFAULT_FONT, pixels(14.0f))
            .with_margin(Margin{.top = DefaultSpacing::small()}));

    // =====================================================================
    // Modal 1: Login Form
    // =====================================================================
    if (auto m = afterhours::modal(
            context, mk(entity, MODAL_LOGIN), show_login,
            afterhours::ModalConfig{}
                .with_size(pixels(420), pixels(400))
                .with_title("Login")
                .with_closed_by(afterhours::ClosedBy::Any)
                .with_backdrop_color({0, 0, 0, 180}))) {

      // Username label + input
      div(context, mk(m.ent(), 0),
          ComponentConfig{}
              .with_label("Username")
              .with_size(ComponentSize{percent(1.0f), pixels(24)})
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
              .with_alignment(TextAlignment::Left)
              .with_skip_tabbing(true)
              .with_render_layer(CL));

      text_input(context, mk(m.ent(), 1), login_username,
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(FOCUS_SAFE_WIDTH), pixels(36)})
                     .with_background(Theme::Usage::Primary)
                     .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
                     .with_roundness(0.1f)
                     .with_margin(Margin{.bottom = DefaultSpacing::small()})
                     .with_render_layer(CL)
                     .with_debug_name("login_username_input"));

      // Password label + input
      div(context, mk(m.ent(), 2),
          ComponentConfig{}
              .with_label("Password")
              .with_size(ComponentSize{percent(1.0f), pixels(24)})
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
              .with_alignment(TextAlignment::Left)
              .with_skip_tabbing(true)
              .with_render_layer(CL));

      text_input(context, mk(m.ent(), 3), login_password,
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(FOCUS_SAFE_WIDTH), pixels(36)})
                     .with_background(Theme::Usage::Primary)
                     .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
                     .with_roundness(0.1f)
                     .with_mask_char('*')
                     .with_margin(Margin{.bottom = DefaultSpacing::small()})
                     .with_render_layer(CL)
                     .with_debug_name("login_password_input"));

      // Remember me toggle
      toggle_switch(context, mk(m.ent(), 4), login_remember,
                    ComponentConfig{}
                        .with_label("Remember me")
                        .with_size(ComponentSize{pixels(300), pixels(36)})
                        .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
                        .with_margin(Margin{.bottom = DefaultSpacing::small()})
                        .with_render_layer(CL)
                        .with_debug_name("login_remember_toggle"));

      // Error message
      if (!login_error.empty()) {
        div(context, mk(m.ent(), 5),
            ComponentConfig{}
                .with_label(login_error)
                .with_size(ComponentSize{percent(1.0f), pixels(24)})
                .with_custom_text_color(afterhours::Color{255, 100, 100, 255})
                .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
                .with_alignment(TextAlignment::Left)
                .with_render_layer(CL));
      }

      // Buttons
      auto btn_row =
          hstack(context, mk(m.ent(), 6),
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(1.0f), pixels(44)})
                     .with_justify_content(JustifyContent::FlexEnd)
                     .with_align_items(AlignItems::Center)
                     .with_render_layer(CL));

      if (button(context, mk(btn_row.ent(), 0),
                 ComponentConfig{}
                     .with_label("Cancel")
                     .with_size(ComponentSize{pixels(100), pixels(36)})
                     .with_margin(Margin{.right = DefaultSpacing::small()})
                     .with_render_layer(CL)
                     .with_debug_name("login_cancel_btn"))) {
        show_login = false;
      }

      if (button(context, mk(btn_row.ent(), 1),
                 ComponentConfig{}
                     .with_label("Login")
                     .with_size(ComponentSize{pixels(100), pixels(36)})
                     .with_background(Theme::Usage::Primary)
                     .with_auto_text_color(true)
                     .with_render_layer(CL)
                     .with_debug_name("login_submit_btn"))) {
        if (login_username.empty() || login_password.empty()) {
          login_error = "Username and password are required";
        } else {
          login_status = "Logged in as: " + login_username;
          login_error = "";
          show_login = false;
        }
      }
    }

    // =====================================================================
    // Modal 2: Multi-Step Wizard
    // =====================================================================
    if (auto m = afterhours::modal(
            context, mk(entity, MODAL_WIZARD), show_wizard,
            afterhours::ModalConfig{}
                .with_size(pixels(480), pixels(500))
                .with_title("Setup Wizard")
                .with_closed_by(afterhours::ClosedBy::Any)
                .with_backdrop_color({0, 0, 0, 180}))) {

      // Progress bar
      float progress = static_cast<float>(wizard_step + 1) / 3.0f;
      progress_bar(context, mk(m.ent(), 0), progress,
                   ComponentConfig{}
                       .with_size(ComponentSize{percent(1.0f), pixels(24)})
                       .with_margin(Margin{.bottom = DefaultSpacing::small()})
                       .with_render_layer(CL));

      // Step indicator
      std::string step_label =
          "Step " + std::to_string(wizard_step + 1) + " of 3";
      div(context, mk(m.ent(), 1),
          ComponentConfig{}
              .with_label(step_label)
              .with_size(ComponentSize{percent(1.0f), pixels(24)})
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, pixels(14.0f))
              .with_alignment(TextAlignment::Left)
              .with_skip_tabbing(true)
              .with_margin(Margin{.bottom = DefaultSpacing::small()})
              .with_render_layer(CL));

      if (wizard_step == 0) {
        // Step 1: Account info
        div(context, mk(m.ent(), 2),
            ComponentConfig{}
                .with_label("Name")
                .with_size(ComponentSize{percent(1.0f), pixels(24)})
                .with_auto_text_color(true)
                .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
                .with_alignment(TextAlignment::Left)
                .with_skip_tabbing(true)
                .with_render_layer(CL));

        text_input(context, mk(m.ent(), 3), wizard_name,
                   ComponentConfig{}
                       .with_size(ComponentSize{percent(FOCUS_SAFE_WIDTH), pixels(36)})
                       .with_background(Theme::Usage::Primary)
                       .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
                       .with_roundness(0.1f)
                       .with_margin(Margin{.bottom = DefaultSpacing::small()})
                       .with_render_layer(CL)
                       .with_debug_name("wizard_name_input"));

        div(context, mk(m.ent(), 4),
            ComponentConfig{}
                .with_label("Email")
                .with_size(ComponentSize{percent(1.0f), pixels(24)})
                .with_auto_text_color(true)
                .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
                .with_alignment(TextAlignment::Left)
                .with_skip_tabbing(true)
                .with_render_layer(CL));

        text_input(context, mk(m.ent(), 5), wizard_email,
                   ComponentConfig{}
                       .with_size(ComponentSize{percent(FOCUS_SAFE_WIDTH), pixels(36)})
                       .with_background(Theme::Usage::Primary)
                       .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
                       .with_roundness(0.1f)
                       .with_margin(Margin{.bottom = DefaultSpacing::small()})
                       .with_render_layer(CL)
                       .with_debug_name("wizard_email_input"));

      } else if (wizard_step == 1) {
        // Step 2: Preferences
        div(context, mk(m.ent(), 2),
            ComponentConfig{}
                .with_label("Theme")
                .with_size(ComponentSize{percent(1.0f), pixels(24)})
                .with_auto_text_color(true)
                .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
                .with_alignment(TextAlignment::Left)
                .with_skip_tabbing(true)
                .with_render_layer(CL));

        std::array<std::string_view, 3> theme_options = {"Light", "Dark",
                                                          "Auto"};
        radio_group(context, mk(m.ent(), 3), theme_options,
                    wizard_theme_idx,
                    ComponentConfig{}
                        .with_size(ComponentSize{percent(FOCUS_SAFE_WIDTH), pixels(36)})
                        .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
                        .with_margin(Margin{.bottom = DefaultSpacing::small()})
                        .with_render_layer(CL)
                        .with_debug_name("wizard_theme_radio"));

        toggle_switch(
            context, mk(m.ent(), 4), wizard_notifications,
            ComponentConfig{}
                .with_label("Enable notifications")
                .with_size(ComponentSize{pixels(340), pixels(40)})
                .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
                .with_margin(Margin{.bottom = DefaultSpacing::small()})
                .with_render_layer(CL)
                .with_debug_name("wizard_notif_toggle"));

      } else {
        // Step 3: Summary
        div(context, mk(m.ent(), 2),
            ComponentConfig{}
                .with_label("Review your settings:")
                .with_size(ComponentSize{percent(1.0f), pixels(24)})
                .with_auto_text_color(true)
                .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
                .with_alignment(TextAlignment::Left)
                .with_skip_tabbing(true)
                .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
                .with_render_layer(CL));

        std::string summary = "Name: " + (wizard_name.empty() ? "(empty)" : wizard_name) +
                              "\nEmail: " + (wizard_email.empty() ? "(empty)" : wizard_email) +
                              "\nTheme: " + std::string(wizard_theme_idx == 0 ? "Light" : wizard_theme_idx == 1 ? "Dark" : "Auto") +
                              "\nNotifications: " + (wizard_notifications ? "On" : "Off");

        div(context, mk(m.ent(), 3),
            ComponentConfig{}
                .with_label(summary)
                .with_size(ComponentSize{percent(1.0f), pixels(100)})
                .with_custom_background(
                    afterhours::colors::lighten(theme.surface, 0.05f))
                .with_auto_text_color(true)
                .with_font(UIComponent::DEFAULT_FONT, pixels(14.0f))
                .with_alignment(TextAlignment::Left)
                .with_padding(Spacing::sm)
                .with_roundness(0.08f)
                .with_render_layer(CL));
      }

      // Navigation buttons
      auto nav_row =
          hstack(context, mk(m.ent(), 10),
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(1.0f), pixels(44)})
                     .with_justify_content(JustifyContent::SpaceBetween)
                     .with_align_items(AlignItems::Center)
                     .with_render_layer(CL));

      if (wizard_step > 0) {
        if (button(context, mk(nav_row.ent(), 0),
                   ComponentConfig{}
                       .with_label("Back")
                       .with_size(ComponentSize{pixels(100), pixels(36)})
                       .with_render_layer(CL)
                       .with_debug_name("wizard_back_btn"))) {
          wizard_step--;
        }
      } else {
        // Spacer for alignment
        div(context, mk(nav_row.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(100), pixels(36)})
                .with_render_layer(CL));
      }

      if (wizard_step < 2) {
        if (button(context, mk(nav_row.ent(), 1),
                   ComponentConfig{}
                       .with_label("Next")
                       .with_size(ComponentSize{pixels(100), pixels(36)})
                       .with_background(Theme::Usage::Primary)
                       .with_auto_text_color(true)
                       .with_render_layer(CL)
                       .with_debug_name("wizard_next_btn"))) {
          wizard_step++;
        }
      } else {
        if (button(context, mk(nav_row.ent(), 1),
                   ComponentConfig{}
                       .with_label("Finish")
                       .with_size(ComponentSize{pixels(100), pixels(36)})
                       .with_background(Theme::Usage::Accent)
                       .with_auto_text_color(true)
                       .with_render_layer(CL)
                       .with_debug_name("wizard_finish_btn"))) {
          wizard_status = "Wizard complete: " + wizard_name;
          show_wizard = false;
        }
      }
    }

    // =====================================================================
    // Modal 3: Settings Panel
    // =====================================================================
    if (auto m = afterhours::modal(
            context, mk(entity, MODAL_SETTINGS), show_settings,
            afterhours::ModalConfig{}
                .with_size(pixels(460), pixels(440))
                .with_title("Settings")
                .with_closed_by(afterhours::ClosedBy::Any)
                .with_backdrop_color({0, 0, 0, 180}))) {

      // Volume slider
      div(context, mk(m.ent(), 0),
          ComponentConfig{}
              .with_label("Volume")
              .with_size(ComponentSize{percent(1.0f), pixels(24)})
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
              .with_alignment(TextAlignment::Left)
              .with_skip_tabbing(true)
              .with_render_layer(CL));

      slider(context, mk(m.ent(), 1), settings_volume,
             ComponentConfig{}
                 .with_size(ComponentSize{percent(FOCUS_SAFE_WIDTH), pixels(40)})
                 .with_background(Theme::Usage::Primary)
                 .with_margin(Margin{.bottom = DefaultSpacing::small()})
                 .with_render_layer(CL)
                 .with_debug_name("settings_volume_slider"),
             SliderHandleValueLabelPosition::OnHandle);

      // Language dropdown
      div(context, mk(m.ent(), 2),
          ComponentConfig{}
              .with_label("Language")
              .with_size(ComponentSize{percent(1.0f), pixels(24)})
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
              .with_alignment(TextAlignment::Left)
              .with_skip_tabbing(true)
              .with_render_layer(CL));

      std::array<std::string_view, 4> languages = {"English", "Spanish",
                                                     "French", "German"};
      dropdown(context, mk(m.ent(), 3), languages, settings_lang_idx,
               ComponentConfig{}
                   .with_size(ComponentSize{percent(FOCUS_SAFE_WIDTH), pixels(40)})
                   .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
                   .with_margin(Margin{.bottom = DefaultSpacing::small()})
                   .with_render_layer(CL)
                   .with_debug_name("settings_lang_dropdown"));

      // Toggle switches - use pixel widths so labels aren't clipped
      toggle_switch(context, mk(m.ent(), 4), settings_dark_mode,
                    ComponentConfig{}
                        .with_label("Dark Mode")
                        .with_size(ComponentSize{pixels(340), pixels(40)})
                        .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
                        .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
                        .with_render_layer(CL)
                        .with_debug_name("settings_dark_toggle"));

      toggle_switch(context, mk(m.ent(), 5), settings_notifs,
                    ComponentConfig{}
                        .with_label("Notifications")
                        .with_size(ComponentSize{pixels(340), pixels(40)})
                        .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
                        .with_margin(Margin{.bottom = DefaultSpacing::small()})
                        .with_render_layer(CL)
                        .with_debug_name("settings_notif_toggle"));

      // Action buttons - Reset on far left, Cancel+Apply on right
      auto btn_row =
          hstack(context, mk(m.ent(), 6),
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(1.0f), pixels(44)})
                     .with_justify_content(JustifyContent::SpaceBetween)
                     .with_align_items(AlignItems::Center)
                     .with_render_layer(CL));

      if (button(context, mk(btn_row.ent(), 0),
                 ComponentConfig{}
                     .with_label("Reset")
                     .with_size(ComponentSize{pixels(80), pixels(36)})
                     .with_background(Theme::Usage::Error)
                     .with_auto_text_color(true)
                     .with_render_layer(CL)
                     .with_debug_name("settings_reset_btn"))) {
        settings_volume = saved_volume;
        settings_lang_idx = saved_lang_idx;
        settings_dark_mode = saved_dark_mode;
        settings_notifs = saved_notifs;
      }

      auto right_btns =
          hstack(context, mk(btn_row.ent(), 1),
                 ComponentConfig{}
                     .with_size(ComponentSize{pixels(200), pixels(44)})
                     .with_justify_content(JustifyContent::FlexEnd)
                     .with_align_items(AlignItems::Center)
                     .with_no_wrap()
                     .with_render_layer(CL));

      if (button(context, mk(right_btns.ent(), 0),
                 ComponentConfig{}
                     .with_label("Cancel")
                     .with_size(ComponentSize{pixels(80), pixels(36)})
                     .with_margin(Margin{.right = DefaultSpacing::small()})
                     .with_render_layer(CL)
                     .with_debug_name("settings_cancel_btn"))) {
        settings_volume = saved_volume;
        settings_lang_idx = saved_lang_idx;
        settings_dark_mode = saved_dark_mode;
        settings_notifs = saved_notifs;
        show_settings = false;
      }

      if (button(context, mk(right_btns.ent(), 1),
                 ComponentConfig{}
                     .with_label("Apply")
                     .with_size(ComponentSize{pixels(80), pixels(36)})
                     .with_background(Theme::Usage::Primary)
                     .with_auto_text_color(true)
                     .with_render_layer(CL)
                     .with_debug_name("settings_apply_btn"))) {
        saved_volume = settings_volume;
        saved_lang_idx = settings_lang_idx;
        saved_dark_mode = settings_dark_mode;
        saved_notifs = settings_notifs;
        settings_status = "Settings applied";
        show_settings = false;
      }
    }

    // =====================================================================
    // Modal 4: Feedback Form
    // =====================================================================
    if (auto m = afterhours::modal(
            context, mk(entity, MODAL_FEEDBACK), show_feedback,
            afterhours::ModalConfig{}
                .with_size(pixels(480), pixels(580))
                .with_title("Submit Feedback")
                .with_closed_by(afterhours::ClosedBy::Any)
                .with_backdrop_color({0, 0, 0, 180}))) {

      // Subject
      div(context, mk(m.ent(), 0),
          ComponentConfig{}
              .with_label("Subject")
              .with_size(ComponentSize{percent(1.0f), pixels(24)})
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
              .with_alignment(TextAlignment::Left)
              .with_skip_tabbing(true)
              .with_render_layer(CL));

      text_input(context, mk(m.ent(), 1), feedback_subject,
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(FOCUS_SAFE_WIDTH), pixels(36)})
                     .with_background(Theme::Usage::Primary)
                     .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
                     .with_roundness(0.1f)
                     .with_margin(Margin{.bottom = DefaultSpacing::small()})
                     .with_render_layer(CL)
                     .with_debug_name("feedback_subject_input"));

      // Category dropdown
      div(context, mk(m.ent(), 2),
          ComponentConfig{}
              .with_label("Category")
              .with_size(ComponentSize{percent(1.0f), pixels(24)})
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
              .with_alignment(TextAlignment::Left)
              .with_skip_tabbing(true)
              .with_render_layer(CL));

      std::array<std::string_view, 4> categories = {"Bug", "Feature",
                                                      "Question", "Other"};
      dropdown(context, mk(m.ent(), 3), categories,
               feedback_category_idx,
               ComponentConfig{}
                   .with_size(ComponentSize{percent(FOCUS_SAFE_WIDTH), pixels(36)})
                   .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
                   .with_margin(Margin{.bottom = DefaultSpacing::small()})
                   .with_render_layer(CL)
                   .with_debug_name("feedback_cat_dropdown"));

      // Severity slider
      div(context, mk(m.ent(), 4),
          ComponentConfig{}
              .with_label("Severity")
              .with_size(ComponentSize{percent(1.0f), pixels(24)})
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
              .with_alignment(TextAlignment::Left)
              .with_skip_tabbing(true)
              .with_render_layer(CL));

      slider(context, mk(m.ent(), 5), feedback_severity,
             ComponentConfig{}
                 .with_size(ComponentSize{percent(FOCUS_SAFE_WIDTH), pixels(36)})
                 .with_background(Theme::Usage::Primary)
                 .with_margin(Margin{.bottom = DefaultSpacing::small()})
                 .with_render_layer(CL)
                 .with_debug_name("feedback_severity_slider"),
             SliderHandleValueLabelPosition::OnHandle);

      // Message text area
      div(context, mk(m.ent(), 6),
          ComponentConfig{}
              .with_label("Message")
              .with_size(ComponentSize{percent(1.0f), pixels(24)})
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
              .with_alignment(TextAlignment::Left)
              .with_skip_tabbing(true)
              .with_render_layer(CL));

      text_area(context, mk(m.ent(), 7), feedback_message,
                ComponentConfig{}
                    .with_size(ComponentSize{percent(FOCUS_SAFE_WIDTH), pixels(100)})
                    .with_background(Theme::Usage::Primary)
                    .with_font(UIComponent::DEFAULT_FONT, pixels(14.0f))
                    .with_roundness(0.08f)
                    .with_max_lines(5)
                    .with_line_height(pixels(18))
                    .with_margin(Margin{.bottom = DefaultSpacing::small()})
                    .with_render_layer(CL)
                    .with_debug_name("feedback_message_input"));

      // Error message
      if (!feedback_error.empty()) {
        div(context, mk(m.ent(), 8),
            ComponentConfig{}
                .with_label(feedback_error)
                .with_size(ComponentSize{percent(1.0f), pixels(24)})
                .with_custom_text_color(afterhours::Color{255, 100, 100, 255})
                .with_font(UIComponent::DEFAULT_FONT, pixels(16.0f))
                .with_alignment(TextAlignment::Left)
                .with_render_layer(CL));
      }

      // Buttons
      auto btn_row =
          hstack(context, mk(m.ent(), 9),
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(1.0f), pixels(44)})
                     .with_justify_content(JustifyContent::FlexEnd)
                     .with_align_items(AlignItems::Center)
                     .with_render_layer(CL));

      if (button(context, mk(btn_row.ent(), 0),
                 ComponentConfig{}
                     .with_label("Cancel")
                     .with_size(ComponentSize{pixels(100), pixels(36)})
                     .with_margin(Margin{.right = DefaultSpacing::small()})
                     .with_render_layer(CL)
                     .with_debug_name("feedback_cancel_btn"))) {
        show_feedback = false;
      }

      if (button(context, mk(btn_row.ent(), 1),
                 ComponentConfig{}
                     .with_label("Submit")
                     .with_size(ComponentSize{pixels(100), pixels(36)})
                     .with_background(Theme::Usage::Primary)
                     .with_auto_text_color(true)
                     .with_render_layer(CL)
                     .with_debug_name("feedback_submit_btn"))) {
        if (feedback_subject.empty() || feedback_message.empty()) {
          feedback_error = "Subject and message are required";
        } else {
          feedback_status =
              "Feedback sent: " + std::string(categories[feedback_category_idx]);
          feedback_error = "";
          show_feedback = false;
        }
      }
    }
  }
};

REGISTER_EXAMPLE_SCREEN(advanced_modals, "Component Galleries",
                        "Advanced modals with forms, wizard, settings",
                        ModalAdvanced)
