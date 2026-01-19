#pragma once

#include "../../external.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include <afterhours/src/plugins/toast.h>
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ToastShowcase : ScreenSystem<UIContext<InputAction>> {
  int toast_counter = 0;
  int undo_counter = 0;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    using toast = afterhours::toast;

    Theme theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;

    auto root = div(
        context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(0.92f), screen_pct(0.90f)})
            .with_custom_background(theme.background)
            .with_roundness(0.08f)
            .with_debug_name("toast_bg"));

    auto main_container =
        div(context, mk(root.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                .with_flex_direction(FlexDirection::Column)
                .with_padding(Spacing::md)
                .with_debug_name("toast_main"));

    div(context, mk(main_container.ent(), 0),
        ComponentConfig{}
            .with_label("Toast Notifications")
            .with_size(ComponentSize{percent(1.0f), pixels(60)})
            .with_custom_background(theme.surface)
            .with_auto_text_color(true)
            .with_padding(Spacing::md)
            .with_font(UIComponent::DEFAULT_FONT, 32.0f)
            .with_roundness(0.1f)
            .with_margin(Margin{.top = pixels(0),
                                .bottom = DefaultSpacing::medium(),
                                .left = pixels(0),
                                .right = pixels(0)})
            .with_debug_name("title"));

    // =========================================================================
    // Row 1: Simple toasts
    // =========================================================================
    auto button_row =
        div(context, mk(main_container.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(90)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::md)
                .with_roundness(0.1f)
                .with_flex_direction(FlexDirection::Row)
                .with_align_items(AlignItems::Center)
                .with_debug_name("button_row"));

    if (button(context, mk(button_row.ent(), 0),
               ComponentConfig{}
                   .with_label("Info Toast")
                   .with_size(ComponentSize{pixels(150), pixels(55)})
                   .with_background(Theme::Usage::Primary)
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, 20.0f)
                   .with_margin(Spacing::sm)
                   .with_debug_name("btn_info"))) {
      toast::send_info(context, "This is an info message #" +
                  std::to_string(++toast_counter));
    }

    if (button(context, mk(button_row.ent(), 1),
               ComponentConfig{}
                   .with_label("Success Toast")
                   .with_size(ComponentSize{pixels(160), pixels(55)})
                   .with_background(Theme::Usage::Secondary)
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, 20.0f)
                   .with_margin(Spacing::sm)
                   .with_debug_name("btn_success"))) {
      toast::send_success(context, "Operation completed successfully!");
    }

    afterhours::Color warningBg = theme.accent;
    if (button(context, mk(button_row.ent(), 2),
               ComponentConfig{}
                   .with_label("Warning Toast")
                   .with_size(ComponentSize{pixels(170), pixels(55)})
                   .with_custom_background(warningBg)
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, 20.0f)
                   .with_margin(Spacing::sm)
                   .with_debug_name("btn_warning"))) {
      toast::send_warning(context, "Warning: Check your settings");
    }

    if (button(context, mk(button_row.ent(), 3),
               ComponentConfig{}
                   .with_label("Error Toast")
                   .with_size(ComponentSize{pixels(150), pixels(55)})
                   .with_background(Theme::Usage::Error)
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, 20.0f)
                   .with_margin(Spacing::sm)
                   .with_debug_name("btn_error"))) {
      toast::send_error(context, "Error: Something went wrong!");
    }

    // =========================================================================
    // Row 2: Duration and spam
    // =========================================================================
    auto second_row =
        div(context, mk(main_container.ent(), 2),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(90)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::md)
                .with_roundness(0.1f)
                .with_flex_direction(FlexDirection::Row)
                .with_align_items(AlignItems::Center)
                .with_margin(Margin{.top = DefaultSpacing::medium(),
                                    .bottom = pixels(0),
                                    .left = pixels(0),
                                    .right = pixels(0)})
                .with_debug_name("second_row"));

    if (button(context, mk(second_row.ent(), 0),
               ComponentConfig{}
                   .with_label("Quick (1s)")
                   .with_size(ComponentSize{pixels(140), pixels(55)})
                   .with_background(Theme::Usage::Primary)
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                   .with_margin(Spacing::sm)
                   .with_debug_name("btn_quick"))) {
      toast::send_info(context, "This disappears fast!", 1.0f);
    }

    if (button(context, mk(second_row.ent(), 1),
               ComponentConfig{}
                   .with_label("Long (10s)")
                   .with_size(ComponentSize{pixels(140), pixels(55)})
                   .with_background(Theme::Usage::Primary)
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                   .with_margin(Spacing::sm)
                   .with_debug_name("btn_long"))) {
      toast::send_info(context, "This sticks around for a while...", 10.0f);
    }

    if (button(context, mk(second_row.ent(), 2),
               ComponentConfig{}
                   .with_label("Spam x5")
                   .with_size(ComponentSize{pixels(130), pixels(55)})
                   .with_background(Theme::Usage::Accent)
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                   .with_margin(Spacing::sm)
                   .with_debug_name("btn_spam"))) {
      for (int i = 0; i < 5; i++) {
        toast::send_info(context, "Spam toast #" + std::to_string(i + 1), 4.0f);
      }
    }

    afterhours::Color coral = {255, 127, 80, 255};
    if (button(context, mk(second_row.ent(), 3),
               ComponentConfig{}
                   .with_label("Custom Color")
                   .with_size(ComponentSize{pixels(160), pixels(55)})
                   .with_custom_background(coral)
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                   .with_margin(Spacing::sm)
                   .with_debug_name("btn_custom"))) {
      toast::send_custom(context, "Custom colored toast!", coral, 4.0f);
    }

    // =========================================================================
    // Row 3: Interactive toasts with buttons
    // =========================================================================
    auto third_row =
        div(context, mk(main_container.ent(), 3),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(90)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::md)
                .with_roundness(0.1f)
                .with_flex_direction(FlexDirection::Row)
                .with_align_items(AlignItems::Center)
                .with_margin(Margin{.top = DefaultSpacing::medium(),
                                    .bottom = pixels(0),
                                    .left = pixels(0),
                                    .right = pixels(0)})
                .with_debug_name("third_row"));

    if (button(context, mk(third_row.ent(), 0),
               ComponentConfig{}
                   .with_label("With Undo Action")
                   .with_size(ComponentSize{pixels(180), pixels(55)})
                   .with_background(Theme::Usage::Secondary)
                   .with_auto_text_color(true)
                   .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                   .with_margin(Spacing::sm)
                   .with_debug_name("btn_undo"))) {
      // Create a toast that shows undo was triggered
      // Note: For truly interactive toasts with buttons, a different approach
      // would be needed (modal or persistent UI element)
      undo_counter++;
      toast::send_success(context,
                          "Item deleted (undo #" + std::to_string(undo_counter) +
                              ")",
                          5.0f);
    }

    // Show undo count
    div(context, mk(third_row.ent(), 1),
        ComponentConfig{}
            .with_label("Undos: " + std::to_string(undo_counter))
            .with_size(ComponentSize{pixels(120), pixels(55)})
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_margin(Spacing::sm));
  }
};

REGISTER_EXAMPLE_SCREEN(toasts, "Component Galleries",
                        "Toast notification system demo", ToastShowcase)
