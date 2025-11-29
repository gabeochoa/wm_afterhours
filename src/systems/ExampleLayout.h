#pragma once

#include "../external.h"
#include "../input_mapping.h"
#include "ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleLayout : afterhours::System<UIContext<InputAction>> {
  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {

    auto container =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.8f), screen_pct(0.6f)})
                .with_custom_color(afterhours::Color{40, 40, 40, 255})
                .with_padding(Padding{screen_pct(0.02f), screen_pct(0.02f),
                                      screen_pct(0.02f), screen_pct(0.02f)})
                .with_debug_name("main_container"));

    div(context, mk(container.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(0.3f), screen_pct(0.2f)})
            .with_custom_color(afterhours::Color{100, 50, 50, 255})
            .with_margin(Margin{.top = pixels(0),
                                .right = screen_pct(0.01f),
                                .bottom = pixels(0),
                                .left = pixels(0)})
            .with_debug_name("left_box"));

    div(context, mk(container.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(0.3f), screen_pct(0.2f)})
            .with_custom_color(afterhours::Color{50, 100, 50, 255})
            .with_margin(Margin{.top = pixels(0),
                                .right = screen_pct(0.01f),
                                .bottom = pixels(0),
                                .left = pixels(0)})
            .with_debug_name("middle_box"));

    div(context, mk(container.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(0.3f), screen_pct(0.2f)})
            .with_custom_color(afterhours::Color{50, 50, 100, 255})
            .with_debug_name("right_box"));

    div(context, mk(container.ent(), 3),
        ComponentConfig{}
            .with_label("Layout Example")
            .with_size(ComponentSize{screen_pct(0.7f), screen_pct(0.15f)})
            .with_custom_color(afterhours::Color{60, 60, 60, 255})
            .with_margin(Margin{.top = screen_pct(0.01f),
                                .right = pixels(0),
                                .bottom = pixels(0),
                                .left = pixels(0)})
            .with_font(UIComponent::DEFAULT_FONT, 24.0f)
            .with_debug_name("title"));
  }
};

REGISTER_EXAMPLE_SCREEN(layout, "Layout system example with containers",
                        ExampleLayout)
