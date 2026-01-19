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
      toast::send_info("This is an info message #" +
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
      toast::send_success("Operation completed successfully!");
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
      toast::send_warning("Warning: Check your settings");
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
      toast::send_error("Error: Something went wrong!");
    }

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
      toast::send_info("This disappears fast!", 1.0f);
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
      toast::send_info("This sticks around for a while...", 10.0f);
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
        toast::send_info("Spam toast #" + std::to_string(i + 1), 4.0f);
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
      toast::send_custom("Custom colored toast!", coral, 4.0f);
    }
    // Toast rendering is handled automatically by ToastRenderSystem
  }
};

REGISTER_EXAMPLE_SCREEN(toasts, "Component Galleries",
                        "Toast notification system demo", ToastShowcase)
