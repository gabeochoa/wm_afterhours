#pragma once

#include "../../external.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/src/plugins/modal.h>

struct ScrollbarLayerRepro : ScreenSystem<afterhours::ui::UIContext<InputAction>> {
  bool open = false;

  void for_each_with(afterhours::Entity &entity,
                     afterhours::ui::UIContext<InputAction> &ctx, float) override {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    const float scale = std::min(ctx.screen_width / 1280.f, ctx.screen_height / 720.f);
    const float left = (ctx.screen_width - 1280.f * scale) / 2.f;
    ctx.theme = Theme{};
    ctx.theme.background = {15, 22, 34, 255};
    ctx.theme.surface = {25, 35, 51, 255};
    ctx.theme.secondary = {36, 48, 67, 255};
    ctx.theme.font = {233, 239, 248, 255};
    ctx.theme.font_muted = {171, 185, 207, 255};
    ctx.theme.accent = {143, 193, 255, 255};
    ctx.scaling_mode = ScalingMode::Proportional;
    const auto base = ComponentConfig{}.with_font("AtkinsonMock", pixels(22.f * scale))
        .with_custom_text_color(ctx.theme.font);
    div(ctx, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(ctx.screen_width), pixels(ctx.screen_height)})
        .with_absolute_position(0, 0).with_custom_background(ctx.theme.background).with_corner_radius(0));
    auto title = base;
    div(ctx, mk(entity, 1), title.with_size({pixels(1184.f * scale), pixels(48.f * scale)})
        .with_absolute_position(left + 48.f * scale, 32.f * scale)
        .with_font_size(pixels(34.f * scale)).with_label("Scrollbar layer reproduction")
        .with_alignment(TextAlignment::Left).with_transparent_bg());
    auto description = base;
    div(ctx, mk(entity, 2), description.with_size({pixels(1184.f * scale), pixels(40.f * scale)})
        .with_absolute_position(left + 48.f * scale, 88.f * scale)
        .with_label("Open the dialog. The list's scrollbar should disappear behind it.")
        .with_alignment(TextAlignment::Left).with_transparent_bg());
    auto launcher = base;
    if (button(ctx, mk(entity, 3), launcher.with_size({pixels(224.f * scale), pixels(44.f * scale)})
        .with_absolute_position(left + 48.f * scale, 144.f * scale)
        .with_label("Open dialog").with_debug_name("scrollbar_repro_open"))) open = true;

    auto list_config = base;
    auto list = div(ctx, mk(entity, 4), list_config
        .with_size({pixels(520.f * scale), pixels(360.f * scale)})
        .with_absolute_position(left + 200.f * scale, 224.f * scale)
        .with_custom_background(ctx.theme.surface).with_corner_radius(8.f * scale)
        .with_flex_direction(FlexDirection::Column).with_overflow(Overflow::Scroll, Axis::Y)
        .with_debug_name("scrollbar_repro_list"));
    for (int i = 0; i < 30; ++i) {
      auto row = base;
      div(ctx, mk(list.ent(), i), row.with_size({percent(1.f), pixels(40.f * scale)})
          .with_debug_name("scrollbar_repro_row_" + std::to_string(i))
          .with_label("Background row " + std::to_string(i + 1))
          .with_alignment(TextAlignment::Left).with_transparent_bg());
    }

    auto dialog = afterhours::modal(ctx, mk(entity, 5), open,
        afterhours::ModalConfig{}.with_size(pixels(640.f * scale), pixels(320.f * scale))
            .with_closed_by(afterhours::ClosedBy::Any));
    if (!dialog) return;
    const int layer = dialog.ent().get<afterhours::modal::Modal>().render_layer;
    auto content = base;
    content.with_render_layer(layer).with_transparent_bg().with_alignment(TextAlignment::Left);
    auto heading = content;
    div(ctx, mk(dialog.ent(), 0), heading.with_size({percent(1.f), pixels(48.f * scale)})
        .with_label("Foreground dialog").with_debug_name("scrollbar_repro_dialog_title"));
    auto explanation = content;
    div(ctx, mk(dialog.ent(), 1), explanation.with_size({percent(1.f), pixels(64.f * scale)})
        .with_label("Try scrolling here. The background list should stay still.")
        .with_text_overflow(TextOverflow::Wrap));
    auto space = content;
    div(ctx, mk(dialog.ent(), 2), space.with_size({percent(1.f), expand()}));
    auto close = base;
    if (button(ctx, mk(dialog.ent(), 3), close.with_render_layer(layer)
        .with_size({pixels(132.f * scale), pixels(44.f * scale)})
        .with_label("Close").with_debug_name("scrollbar_repro_close"))) open = false;
  }
};

REGISTER_EXAMPLE_SCREEN(scrollbar_layer_repro, "Tools", "Modal layering and background scroll isolation", ScrollbarLayerRepro)
