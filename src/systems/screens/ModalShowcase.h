#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/modal.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ModalShowcase : ScreenSystem<UIContext<InputAction>> {
  // Modal visibility states
  bool show_basic_modal = false;
  bool show_composable_modal = false;
  bool show_info_modal = false;
  bool show_confirm_modal = false;
  bool show_fyi_modal = false;
  bool show_stacked_settings = false;
  bool show_stacked_confirm = false;

  // Results tracking
  afterhours::DialogResult last_confirm_result = afterhours::DialogResult::Pending;
  afterhours::DialogResult last_fyi_result = afterhours::DialogResult::Pending;
  int confirm_count = 0;
  int cancel_count = 0;
  int background_click_count = 0; // For testing input blocking

  // Entity IDs for modals (avoid string-based mk which doesn't exist)
  static constexpr int MODAL_BASIC = 100;
  static constexpr int MODAL_COMPOSABLE = 101;
  static constexpr int MODAL_INFO = 102;
  static constexpr int MODAL_CONFIRM = 103;
  static constexpr int MODAL_FYI = 104;
  static constexpr int MODAL_SETTINGS = 105;
  static constexpr int MODAL_NESTED_CONFIRM = 106;

  // Configurable design parameters
  static constexpr float SECTION_ROUNDNESS = 0.08f;
  static constexpr float BUTTON_ROUNDNESS = 0.08f;
  static constexpr float SECTION_HEIGHT = 95.0f;
  static constexpr float ROW_HEIGHT = 44.0f;
  static constexpr float BUTTON_HEIGHT = 40.0f;
  static constexpr float HEADER_HEIGHT = 30.0f;
  static constexpr float TITLE_HEIGHT = 50.0f;
  static constexpr float HELPER_BUTTON_WIDTH = 170.0f;  // Uniform width for helper buttons

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {

    Theme theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;

    auto root = div(
        context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(0.95f), screen_pct(0.88f)})
            .with_background(Theme::Usage::Background)
            .with_roundness(SECTION_ROUNDNESS)
            .with_self_align(SelfAlign::Center)
            .with_debug_name("modal_bg"));

    auto main_container =
        div(context, mk(root.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                .with_flex_direction(FlexDirection::Column)
                .with_padding(Spacing::md)
                .with_debug_name("modal_main"));

    // Title - use theme font sizes
    div(context, mk(main_container.ent(), 0),
        ComponentConfig{}
            .with_label("Modal Dialogs")
            .with_size(ComponentSize{percent(1.0f), pixels(TITLE_HEIGHT)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, theme.font_size_lg())
            .with_roundness(SECTION_ROUNDNESS)
            .with_margin(Margin{.top = pixels(0),
                                .bottom = DefaultSpacing::small(),
                                .left = pixels(0),
                                .right = pixels(0)}));

    // Helper to create a section with header and content row
    auto make_section = [&](int section_id, const char *header_label,
                            const char *section_name, const char *row_name) {
      auto section =
          div(context, mk(main_container.ent(), section_id),
              ComponentConfig{}
                  .with_size(ComponentSize{percent(1.0f), pixels(SECTION_HEIGHT)})
                  .with_background(Theme::Usage::Surface)
                  .with_padding(Spacing::md)
                  .with_roundness(SECTION_ROUNDNESS)
                  .with_flex_direction(FlexDirection::Column)
                  .with_margin(Margin{.bottom = DefaultSpacing::small()})
                  .with_debug_name(section_name));

      div(context, mk(section.ent(), 0),
          ComponentConfig{}
              .with_label(header_label)
              .with_size(ComponentSize{percent(1.0f), pixels(HEADER_HEIGHT)})
              .with_background(Theme::Usage::Primary)
              .with_auto_text_color(true)
              .with_padding(Spacing::sm)
              .with_font(UIComponent::DEFAULT_FONT, theme.font_size_sm())
              .with_alignment(TextAlignment::Left)
              .with_margin(Margin{.bottom = DefaultSpacing::tiny()}));

      auto row =
          div(context, mk(section.ent(), 1),
              ComponentConfig{}
                  .with_size(ComponentSize{percent(1.0f), pixels(ROW_HEIGHT)})
                  .with_flex_direction(FlexDirection::Row)
                  .with_align_items(AlignItems::Center)
                  .with_justify_content(JustifyContent::FlexStart)
                  .with_debug_name(row_name));

      return row;
    };

    // =========================================================================
    // Section 1: Basic Modals
    // =========================================================================
    auto row1 = make_section(1, "BASIC MODALS", "section1", "row1");

    if (button(context, mk(row1.ent(), 0),
               ComponentConfig{}
                   .with_label("Simple Modal")
                   .with_size(ComponentSize{pixels(180), pixels(BUTTON_HEIGHT)})
                   .with_background(Theme::Usage::Primary)
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, theme.font_size_sm())
                   .with_margin(Margin{.right = DefaultSpacing::medium()})
                   .with_roundness(BUTTON_ROUNDNESS)
                   .with_debug_name("btn_simple"))) {
      show_basic_modal = true;
    }

    if (button(context, mk(row1.ent(), 1),
               ComponentConfig{}
                   .with_label("Composable Modal")
                   .with_size(ComponentSize{pixels(220), pixels(BUTTON_HEIGHT)})
                   .with_background(Theme::Usage::Secondary)
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, theme.font_size_sm())
                   .with_roundness(BUTTON_ROUNDNESS)
                   .with_debug_name("btn_composable"))) {
      show_composable_modal = true;
    }

    // =========================================================================
    // Section 2: Helper Functions
    // =========================================================================
    auto row2 = make_section(2, "HELPER FUNCTIONS", "section2", "row2");

    if (button(context, mk(row2.ent(), 0),
               ComponentConfig{}
                   .with_label("Info Dialog")
                   .with_size(ComponentSize{pixels(HELPER_BUTTON_WIDTH), pixels(BUTTON_HEIGHT)})
                   .with_background(Theme::Usage::Primary)
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, theme.font_size_sm())
                   .with_margin(Margin{.right = DefaultSpacing::medium()})
                   .with_roundness(BUTTON_ROUNDNESS)
                   .with_debug_name("btn_info"))) {
      show_info_modal = true;
    }

    if (button(context, mk(row2.ent(), 1),
               ComponentConfig{}
                   .with_label("Confirmation")
                   .with_size(ComponentSize{pixels(HELPER_BUTTON_WIDTH), pixels(BUTTON_HEIGHT)})
                   .with_background(Theme::Usage::Accent)
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, theme.font_size_sm())
                   .with_margin(Margin{.right = DefaultSpacing::medium()})
                   .with_roundness(BUTTON_ROUNDNESS)
                   .with_debug_name("btn_confirm"))) {
      show_confirm_modal = true;
      last_confirm_result = afterhours::DialogResult::Pending;
    }

    if (button(context, mk(row2.ent(), 2),
               ComponentConfig{}
                   .with_label("Notice")
                   .with_size(ComponentSize{pixels(HELPER_BUTTON_WIDTH), pixels(BUTTON_HEIGHT)})
                   .with_background(Theme::Usage::Secondary)
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, theme.font_size_sm())
                   .with_roundness(BUTTON_ROUNDNESS)
                   .with_debug_name("btn_fyi"))) {
      show_fyi_modal = true;
      last_fyi_result = afterhours::DialogResult::Pending;
    }

    // =========================================================================
    // Section 3: Modal Stacking
    // =========================================================================
    auto row3 = make_section(3, "MODAL STACKING", "section3", "row3");

    if (button(context, mk(row3.ent(), 0),
               ComponentConfig{}
                   .with_label("Open Settings (with nested confirm)")
                   .with_size(ComponentSize{pixels(380), pixels(BUTTON_HEIGHT)})
                   .with_background(Theme::Usage::Primary)
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, theme.font_size_sm())
                   .with_roundness(BUTTON_ROUNDNESS)
                   .with_debug_name("btn_stacked"))) {
      show_stacked_settings = true;
    }

    // =========================================================================
    // Section 4: Input Blocking Test
    // =========================================================================
    auto row4 = make_section(4, "INPUT BLOCKING TEST", "section4", "row4");

    // This button is used to test input blocking - when a modal is open,
    // clicking this button should NOT increment the counter
    if (button(context, mk(row4.ent(), 0),
               ComponentConfig{}
                   .with_label("Background Button")
                   .with_size(ComponentSize{pixels(220), pixels(BUTTON_HEIGHT)})
                   .with_background(Theme::Usage::Primary)
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, theme.font_size_sm())
                   .with_margin(Margin{.right = DefaultSpacing::large()})
                   .with_roundness(BUTTON_ROUNDNESS)
                   .with_debug_name("btn_background"))) {
      background_click_count++;
    }

    // Counter display with visual containment - use lightened surface for distinction
    div(context, mk(row4.ent(), 1),
        ComponentConfig{}
            .with_label("Background Clicks: " + std::to_string(background_click_count))
            .with_size(ComponentSize{pixels(180), pixels(BUTTON_HEIGHT)})
            .with_custom_background(afterhours::colors::lighten(theme.background, 0.15f))
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, theme.font_size_sm())
            .with_padding(Spacing::xs)
            .with_roundness(BUTTON_ROUNDNESS));

    // =========================================================================
    // Section 5: Results Display
    // =========================================================================
    // Note: section5 omits bottom margin, and row5 has extra styling
    auto section5 =
        div(context, mk(main_container.ent(), 5),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(SECTION_HEIGHT)})
                .with_background(Theme::Usage::Surface)
                .with_padding(Spacing::md)
                .with_roundness(SECTION_ROUNDNESS)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("section5"));

    div(context, mk(section5.ent(), 0),
        ComponentConfig{}
            .with_label("RESULTS")
            .with_size(ComponentSize{percent(1.0f), pixels(HEADER_HEIGHT)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::sm)
            .with_font(UIComponent::DEFAULT_FONT, theme.font_size_sm())
            .with_alignment(TextAlignment::Left)
            .with_margin(Margin{.bottom = DefaultSpacing::tiny()}));

    auto row5 =
        div(context, mk(section5.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(ROW_HEIGHT)})
                .with_flex_direction(FlexDirection::Row)
                .with_align_items(AlignItems::Center)
                .with_justify_content(JustifyContent::FlexStart)
                .with_background(Theme::Usage::Background)
                .with_roundness(SECTION_ROUNDNESS)
                .with_padding(Spacing::sm)
                .with_debug_name("row5"));

    std::string confirm_result_str = "Pending";
    if (last_confirm_result == afterhours::DialogResult::Confirmed)
      confirm_result_str = "Confirmed";
    else if (last_confirm_result == afterhours::DialogResult::Cancelled)
      confirm_result_str = "Cancelled";
    else if (last_confirm_result == afterhours::DialogResult::Dismissed)
      confirm_result_str = "Dismissed";

    std::string fyi_result_str = "Pending";
    if (last_fyi_result == afterhours::DialogResult::Confirmed)
      fyi_result_str = "Primary";
    else if (last_fyi_result == afterhours::DialogResult::Cancelled)
      fyi_result_str = "Dismissed";
    else if (last_fyi_result == afterhours::DialogResult::Custom)
      fyi_result_str = "Tertiary";

    div(context, mk(row5.ent(), 0),
        ComponentConfig{}
            .with_label("Confirm: " + confirm_result_str + "  |  FYI: " +
                        fyi_result_str + "  |  Confirms: " +
                        std::to_string(confirm_count) +
                        "  Cancels: " + std::to_string(cancel_count))
            .with_size(ComponentSize{percent(1.0f), pixels(BUTTON_HEIGHT)})
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, theme.font_size_sm())
            .with_alignment(TextAlignment::Left));

    // =========================================================================
    // Modal implementations
    // =========================================================================

    // Basic modal - uses ClosedBy::Any so it closes on backdrop click
    constexpr int MODAL_CONTENT_LAYER = 1001;
    if (auto m = afterhours::modal(context, mk(entity, MODAL_BASIC),
                                   show_basic_modal,
                                   afterhours::ModalConfig{}
                                       .with_size(pixels(400), pixels(200))
                                       .with_title("Basic Modal")
                                       .with_closed_by(afterhours::ClosedBy::Any))) {
      div(context, mk(m.ent(), 0),
          ComponentConfig{}
              .with_label("This is a simple modal dialog.")
              .with_size(ComponentSize{percent(1.0f), pixels(60)})
              .with_auto_text_color(true)
              .with_padding(Spacing::md)
              .with_render_layer(MODAL_CONTENT_LAYER));

      auto btn_row = div(context, mk(m.ent(), 1),
                         ComponentConfig{}
                             .with_size(ComponentSize{percent(1.0f), pixels(44)})
                             .with_flex_direction(FlexDirection::Row)
                             .with_justify_content(JustifyContent::Center)
                             .with_align_items(AlignItems::Center)
                             .with_flex_wrap(FlexWrap::NoWrap)
                             .with_render_layer(MODAL_CONTENT_LAYER));

      if (button(context, mk(btn_row.ent(), 0),
                 ComponentConfig{}
                     .with_label("Close")
                     .with_size(ComponentSize{pixels(100), pixels(36)})
                     .with_render_layer(MODAL_CONTENT_LAYER))) {
        show_basic_modal = false;
      }
    }

    // Composable modal with custom content
    if (auto m = afterhours::modal(context, mk(entity, MODAL_COMPOSABLE),
                                   show_composable_modal,
                                   afterhours::ModalConfig{}
                                       .with_size(pixels(500), pixels(350))
                                       .with_title("Composable Modal"))) {
      div(context, mk(m.ent(), 0),
          ComponentConfig{}
              .with_label("This modal contains composable UI components:")
              .with_size(ComponentSize{percent(1.0f), pixels(40)})
              .with_auto_text_color(true)
              .with_render_layer(MODAL_CONTENT_LAYER));

      // Checkboxes
      auto checkbox_row = div(context, mk(m.ent(), 1),
                              ComponentConfig{}
                                  .with_size(ComponentSize{percent(1.0f), children()})
                                  .with_flex_direction(FlexDirection::Row)
                                  .with_align_items(AlignItems::Center)
                                  .with_render_layer(MODAL_CONTENT_LAYER));

      static bool check1 = false, check2 = true;
      checkbox(context, mk(checkbox_row.ent(), 0), check1,
               ComponentConfig{}
                   .with_label("Option A")
                   .with_size(ComponentSize{pixels(120), pixels(36)})
                   .with_render_layer(MODAL_CONTENT_LAYER));
      checkbox(context, mk(checkbox_row.ent(), 1), check2,
               ComponentConfig{}
                   .with_label("Option B")
                   .with_size(ComponentSize{pixels(120), pixels(36)})
                   .with_margin(Margin{.left = DefaultSpacing::medium()})
                   .with_render_layer(MODAL_CONTENT_LAYER));

      // Progress indicator
      div(context, mk(m.ent(), 2),
          ComponentConfig{}
              .with_label("Progress: 65%")
              .with_size(ComponentSize{percent(1.0f), pixels(30)})
              .with_auto_text_color(true)
              .with_margin(Margin{.top = DefaultSpacing::medium()})
              .with_render_layer(MODAL_CONTENT_LAYER));

      // Buttons row
      auto btn_row = div(context, mk(m.ent(), 3),
                         ComponentConfig{}
                             .with_size(ComponentSize{percent(1.0f), pixels(44)})
                             .with_flex_direction(FlexDirection::Row)
                             .with_justify_content(JustifyContent::FlexEnd)
                             .with_align_items(AlignItems::Center)
                             .with_flex_wrap(FlexWrap::NoWrap)
                             .with_render_layer(MODAL_CONTENT_LAYER));

      if (button(context, mk(btn_row.ent(), 0),
                 ComponentConfig{}
                     .with_label("Cancel")
                     .with_size(ComponentSize{pixels(100), pixels(36)})
                     .with_margin(Margin{.right = DefaultSpacing::small()})
                     .with_render_layer(MODAL_CONTENT_LAYER))) {
        show_composable_modal = false;
      }

      if (button(context, mk(btn_row.ent(), 1),
                 ComponentConfig{}
                     .with_label("Save")
                     .with_size(ComponentSize{pixels(100), pixels(36)})
                     .with_background(Theme::Usage::Primary)
                     .with_render_layer(MODAL_CONTENT_LAYER))) {
        show_composable_modal = false;
      }
    }

    // modal::info helper
    afterhours::modal::info(context, mk(entity, MODAL_INFO), show_info_modal,
                "Information",
                "This is an informational message. Click OK to dismiss.",
                "Got it");

    // modal::confirm helper
    auto confirm_result = afterhours::modal::confirm(
        context, mk(entity, MODAL_CONFIRM), show_confirm_modal, "Confirm Action",
        "Are you sure you want to proceed with this action?", "Yes, proceed",
        "Cancel");

    // Track confirm results
    if (confirm_result.confirmed()) {
      confirm_count++;
    } else if (confirm_result.cancelled()) {
      cancel_count++;
    }

    // modal::fyi helper
    auto fyi_result = afterhours::modal::fyi(
        context, mk(entity, MODAL_FYI), show_fyi_modal, "Update Available",
        "A new version (2.0) is available for download.", "Install Now", "Later",
        "Release Notes");
    last_fyi_result = fyi_result.result();

    // Stacked modals - Settings with nested confirm
    if (auto settings = afterhours::modal(
            context, mk(entity, MODAL_SETTINGS), show_stacked_settings,
            afterhours::ModalConfig{}
                .with_size(pixels(450), pixels(300))
                .with_title("Settings"))) {

      div(context, mk(settings.ent(), 0),
          ComponentConfig{}
              .with_label("Configure your preferences below:")
              .with_size(ComponentSize{percent(1.0f), pixels(40)})
              .with_auto_text_color(true)
              .with_render_layer(MODAL_CONTENT_LAYER));

      static bool setting1 = true, setting2 = false;
      checkbox(context, mk(settings.ent(), 1), setting1,
               ComponentConfig{}
                   .with_label("Enable notifications")
                   .with_size(ComponentSize{percent(1.0f), pixels(40)})
                   .with_render_layer(MODAL_CONTENT_LAYER));
      checkbox(context, mk(settings.ent(), 2), setting2,
               ComponentConfig{}
                   .with_label("Dark mode")
                   .with_size(ComponentSize{percent(1.0f), pixels(40)})
                   .with_render_layer(MODAL_CONTENT_LAYER));

      auto btn_row = div(context, mk(settings.ent(), 3),
                         ComponentConfig{}
                             .with_size(ComponentSize{percent(1.0f), pixels(44)})
                             .with_flex_direction(FlexDirection::Row)
                             .with_justify_content(JustifyContent::SpaceBetween)
                             .with_align_items(AlignItems::Center)
                             .with_flex_wrap(FlexWrap::NoWrap)
                             .with_render_layer(MODAL_CONTENT_LAYER));

      if (button(context, mk(btn_row.ent(), 0),
                 ComponentConfig{}
                     .with_label("Reset to Defaults")
                     .with_size(ComponentSize{pixels(160), pixels(36)})
                     .with_background(Theme::Usage::Error)
                     .with_render_layer(MODAL_CONTENT_LAYER))) {
        show_stacked_confirm = true; // Open nested modal
      }

      if (button(context, mk(btn_row.ent(), 1),
                 ComponentConfig{}
                     .with_label("Done")
                     .with_size(ComponentSize{pixels(100), pixels(36)})
                     .with_background(Theme::Usage::Primary)
                     .with_render_layer(MODAL_CONTENT_LAYER))) {
        show_stacked_settings = false;
      }
    }

    // Nested confirm modal (stacked on top of settings)
    if (afterhours::modal::confirm(
            context, mk(entity, MODAL_NESTED_CONFIRM), show_stacked_confirm,
            "Reset Settings?",
            "This will reset all settings to their default values.", "Reset",
            "Cancel")
            .confirmed()) {
      // Reset settings would happen here
    }
  }
};

REGISTER_EXAMPLE_SCREEN(modals, "Component Galleries",
                        "Modal dialog system demo", ModalShowcase)

