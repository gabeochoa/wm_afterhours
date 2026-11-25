#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>

struct DisclosureShowcase : ScreenSystem<afterhours::ui::UIContext<InputAction>> {
  int toggle_count = 0;
  std::string last_toggled = "None";

  void for_each_with(afterhours::Entity &entity,
                     afterhours::ui::UIContext<InputAction> &ctx,
                     float) override {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;

    const afterhours::Color bg{244, 246, 248, 255};
    const afterhours::Color card{255, 255, 255, 255};
    const afterhours::Color row_bg{236, 240, 244, 255};
    const afterhours::Color border{206, 214, 222, 255};
    const afterhours::Color ink{26, 36, 48, 255};
    const afterhours::Color muted{84, 98, 114, 255};
    const afterhours::Color accent{28, 100, 168, 255};

    ctx.theme = Theme{};
    ctx.theme.background = bg;
    ctx.theme.surface = card;
    ctx.theme.font = ink;
    ctx.theme.font_muted = muted;
    ctx.theme.accent = accent;
    ctx.scaling_mode = ScalingMode::Proportional;

    const float s =
        std::min(ctx.screen_width / 1280.f, ctx.screen_height / 720.f);
    auto page = div(
        ctx, mk(entity),
        ComponentConfig{}
            .with_size({pixels(1280.f * s), pixels(720.f * s)})
            .with_absolute_position((ctx.screen_width - 1280.f * s) / 2.f,
                                    (ctx.screen_height - 720.f * s) / 2.f)
            .with_custom_background(bg)
            .with_padding(Padding{.top = pixels(40.f * s),
                                  .left = pixels(72.f * s),
                                  .bottom = pixels(40.f * s),
                                  .right = pixels(72.f * s)})
            .with_flex_direction(FlexDirection::Column)
            .with_gap(pixels(12.f * s))
            .with_corner_radius(0)
            .with_debug_name("disclosure_page"));

    div(ctx, mk(page.ent(), 1),
        ComponentConfig{}
            .with_size({percent(1.f), pixels(54.f * s)})
            .with_label("Disclosure sections")
            .with_alignment(TextAlignment::Left)
            .with_font("AtkinsonMockBold", pixels(40.f * s))
            .with_custom_text_color(ink)
            .with_transparent_bg()
            .with_ignore_pointer_events());

    div(ctx, mk(page.ent(), 2),
        ComponentConfig{}
            .with_size({percent(1.f), pixels(30.f * s)})
            .with_label("Click a header to expand or collapse its section.")
            .with_alignment(TextAlignment::Left)
            .with_font("AtkinsonMock", pixels(20.f * s))
            .with_custom_text_color(muted)
            .with_transparent_bg()
            .with_ignore_pointer_events());

    auto stack = div(ctx, mk(page.ent(), 3),
                     ComponentConfig{}
                         .with_size({pixels(760.f * s), children()})
                         .with_flex_direction(FlexDirection::Column)
                         .with_gap(pixels(10.f * s))
                         .with_transparent_bg()
                         .with_debug_name("disclosure_stack"));

    auto add_rows = [&](afterhours::Entity &body,
                        const std::array<std::pair<const char *, const char *>, 3>
                            &rows) {
      for (size_t i = 0; i < rows.size(); ++i) {
        auto r = div(ctx, mk(body, static_cast<int>(i)),
                     ComponentConfig{}
                         .with_size({percent(1.f), pixels(34.f * s)})
                         .with_flex_direction(FlexDirection::Row)
                         .with_align_items(AlignItems::Center)
                         .with_padding(Padding{.top = pixels(0),
                                               .left = pixels(28.f * s),
                                               .bottom = pixels(0),
                                               .right = pixels(16.f * s)})
                         .with_custom_background(i % 2 == 0 ? card : row_bg)
                         .with_corner_radius(0)
                         .with_ignore_pointer_events());
        div(ctx, mk(r.ent(), 0),
            ComponentConfig{}
                .with_size({pixels(220.f * s), percent(1.f)})
                .with_label(rows[i].first)
                .with_alignment(TextAlignment::Left)
                .with_font("AtkinsonMock", pixels(18.f * s))
                .with_custom_text_color(muted)
                .with_transparent_bg()
                .with_ignore_pointer_events());
        div(ctx, mk(r.ent(), 1),
            ComponentConfig{}
                .with_size({expand(), percent(1.f)})
                .with_label(rows[i].second)
                .with_alignment(TextAlignment::Left)
                .with_font("AtkinsonMockBold", pixels(18.f * s))
                .with_custom_text_color(ink)
                .with_transparent_bg()
                .with_ignore_pointer_events());
      }
    };

    auto section_cfg = [&](const char *name, bool disabled = false) {
      return ComponentConfig{}
          .with_size({percent(1.f), children()})
          .with_custom_background(card)
          .with_custom_text_color(disabled ? muted : ink)
          .with_font("AtkinsonMockBold", pixels(20.f * s))
          .with_border(border, pixels(1.f))
          .with_corner_radius(8.f * s)
          .with_disabled(disabled)
          .with_debug_name(name);
    };

    DisclosureConfig open_cfg{.default_expanded = true,
                              .header_height = 46.f * s,
                              .chevron_size = 20.f * s};
    DisclosureConfig closed_cfg{.default_expanded = false,
                                .header_height = 46.f * s,
                                .chevron_size = 20.f * s};

    if (disclosure(
            ctx, mk(stack.ent(), 10), "Display & graphics",
            [&](afterhours::Entity &body) {
              add_rows(body, {{{"Resolution", "1280 x 720"},
                               {"VSync", "Enabled"},
                               {"UI scale", "100%"}}});
            },
            open_cfg, section_cfg("disc_display"))) {
      ++toggle_count;
      last_toggled = "Display & graphics";
    }

    if (disclosure(
            ctx, mk(stack.ent(), 11), "Audio buses",
            [&](afterhours::Entity &body) {
              add_rows(body, {{{"Master", "80%"},
                               {"Music", "65%"},
                               {"Effects", "90%"}}});
            },
            closed_cfg, section_cfg("disc_audio"))) {
      ++toggle_count;
      last_toggled = "Audio buses";
    }

    if (disclosure(
            ctx, mk(stack.ent(), 12), "Locked diagnostics",
            [&](afterhours::Entity &body) {
              add_rows(body, {{{"Build", "Debug"},
                               {"Backend", "Raylib"},
                               {"Profiler", "Off"}}});
            },
            closed_cfg, section_cfg("disc_locked", true))) {
      ++toggle_count;
      last_toggled = "Locked diagnostics";
    }

    div(ctx, mk(page.ent(), 4),
        ComponentConfig{}
            .with_size({percent(1.f), pixels(28.f * s)})
            .with_label("Toggles: " + std::to_string(toggle_count) +
                        "   |   Last: " + last_toggled)
            .with_alignment(TextAlignment::Left)
            .with_font("AtkinsonMock", pixels(18.f * s))
            .with_custom_text_color(muted)
            .with_transparent_bg()
            .with_ignore_pointer_events()
            .with_debug_name("disc_status"));
  }
};

REGISTER_EXAMPLE_SCREEN(disclosure_showcase, "Component Galleries",
                        "Expand/collapse disclosure sections",
                        DisclosureShowcase)
