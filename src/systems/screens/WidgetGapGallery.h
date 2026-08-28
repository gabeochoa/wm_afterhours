#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// The widgets that shipped and nothing here called: button_group, pagination,
// spacer, image and icon_row. A library feature with no consumer is one nobody
// finds out is broken.
struct WidgetGapGallery : ScreenSystem<UIContext<InputAction>> {
  bool loaded = false;
  raylib::Texture2D gear{}, star{}, trophy{}, home{}, heart{};

  size_t page = 0;
  std::vector<std::string> pages = {"1", "2", "3", "4", "5"};
  int last_group = -1;
  std::vector<std::string> tools = {"Select", "Move", "Rotate", "Scale"};

  void load() {
    if (loaded)
      return;
    loaded = true;
    const std::string dir =
        afterhours::files::get_resource_path(
            "kenney/kenney_game-icons/PNG/White/2x/", "")
            .string();
    gear = raylib::LoadTexture((dir + "gear.png").c_str());
    star = raylib::LoadTexture((dir + "star.png").c_str());
    trophy = raylib::LoadTexture((dir + "trophy.png").c_str());
    home = raylib::LoadTexture((dir + "home.png").c_str());
    heart = raylib::LoadTexture((dir + "heart.png").c_str());
  }

  static afterhours::texture_manager::Rectangle whole(
      const raylib::Texture2D &t) {
    return {0.f, 0.f, (float)t.width, (float)t.height};
  }

  void section(UIContext<InputAction> &context, afterhours::Entity &parent,
               int idx, const char *title) {
    div(context, mk(parent, idx),
        ComponentConfig{}
            .with_label(title)
            .with_size(ComponentSize{percent(1.f), pixels(30)})
            .with_background(Theme::Usage::None)
            .with_custom_text_color(afterhours::Color{140, 200, 255, 255})
            .with_font_size(pixels(19.f))
            .with_alignment(TextAlignment::Left)
            .with_text_inset(6.f, 0.f));
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    load();
    auto theme = afterhours::ui::theme_presets::neon_dark();
    theme.roundness = 0.12f;
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;
    UIStylingDefaults::get().set_default_font(UIComponent::DEFAULT_FONT,
                                              pixels(18.0f));

    auto root = vstack(context, mk(entity),
                       ComponentConfig{}
                           .with_size(ComponentSize{screen_pct(0.94f),
                                                    screen_pct(0.95f)})
                           .with_self_align(SelfAlign::Center)
                           .with_background(Theme::Usage::Background)
                           .with_padding(Spacing::md)
                           .with_no_wrap()
                           .with_debug_name("wg_root"));

    div(context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_label("Widgets that had no caller")
            .with_size(ComponentSize{percent(1.f), pixels(44)})
            .with_background(Theme::Usage::Surface)
            .with_auto_text_color(true)
            .with_font_size(pixels(28.f))
            .with_alignment(TextAlignment::Center)
            .with_corner_radius(10.f)
            .with_margin(Margin{.bottom = DefaultSpacing::small()}));

    auto cols = hstack(context, mk(root.ent(), 1),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.f), expand()})
                           .with_no_wrap()
                           .with_debug_name("wg_cols"));

    // ---- left: button_group + pagination + spacer ---------------------
    auto left = vstack(context, mk(cols.ent(), 0),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(0.49f), percent(1.f)})
                           .with_background(Theme::Usage::Surface)
                           .with_padding(Spacing::md)
                           .with_corner_radius(10.f)
                           .with_margin(Margin{.right = DefaultSpacing::small()})
                           .with_no_wrap()
                           .with_debug_name("wg_left"));

    section(context, left.ent(), 0, "button_group");

    auto grp = button_group(context, mk(left.ent(), 1), tools,
                            ComponentConfig{}
                                .with_size(ComponentSize{expand(), pixels(44)})
                                // Without this the group inherits the Column
                                // default and stacks its four buttons in 44px.
                                .with_flex_direction(FlexDirection::Row)
                                .with_font_size(pixels(17.f))
                                .with_debug_name("wg_group"));
    if (grp)
      last_group = grp.as<int>();

    div(context, mk(left.ent(), 2),
        ComponentConfig{}
            .with_label(last_group < 0
                            ? std::string("no tool picked yet")
                            : fmt::format("picked: {}", tools[(size_t)last_group]))
            .with_size(ComponentSize{percent(1.f), pixels(30)})
            .with_background(Theme::Usage::None)
            .with_custom_text_color(afterhours::Color{200, 210, 230, 255})
            .with_font_size(pixels(16.f))
            .with_text_inset(6.f, 0.f)
            .with_margin(Margin{.bottom = DefaultSpacing::medium()})
            .with_debug_name("wg_group_out"));

    section(context, left.ent(), 3, "pagination");

    pagination(context, mk(left.ent(), 4), pages, page,
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.f), pixels(44)})
                   .with_font_size(pixels(17.f))
                   .with_debug_name("wg_pages"));

    div(context, mk(left.ent(), 5),
        ComponentConfig{}
            .with_label(fmt::format("page {} of {}", pages[page], pages.size()))
            .with_size(ComponentSize{percent(1.f), pixels(30)})
            .with_background(Theme::Usage::None)
            .with_custom_text_color(afterhours::Color{200, 210, 230, 255})
            .with_font_size(pixels(16.f))
            .with_text_inset(6.f, 0.f)
            .with_margin(Margin{.bottom = DefaultSpacing::medium()})
            .with_debug_name("wg_page_out"));

    section(context, left.ent(), 6, "spacer pushes the tail to the edge");

    auto srow = hstack(context, mk(left.ent(), 7),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.f), pixels(44)})
                           .with_no_wrap()
                           .with_debug_name("wg_srow"));
    div(context, mk(srow.ent(), 0),
        ComponentConfig{}
            .with_label("head")
            .with_size(ComponentSize{pixels(90), pixels(40)})
            .with_custom_background(afterhours::Color{60, 90, 140, 255})
            .with_corner_radius(6.f));
    spacer(context, mk(srow.ent(), 1));
    div(context, mk(srow.ent(), 2),
        ComponentConfig{}
            .with_label("tail")
            .with_size(ComponentSize{pixels(90), pixels(40)})
            .with_custom_background(afterhours::Color{140, 90, 60, 255})
            .with_corner_radius(6.f));

    // ---- right: image + icon_row + icon buttons -----------------------
    auto right = vstack(context, mk(cols.ent(), 1),
                        ComponentConfig{}
                            .with_size(ComponentSize{percent(0.49f), percent(1.f)})
                            .with_background(Theme::Usage::Surface)
                            .with_padding(Spacing::md)
                            .with_corner_radius(10.f)
                            .with_no_wrap()
                            .with_debug_name("wg_right"));

    section(context, right.ent(), 0, "image");

    auto irow = hstack(context, mk(right.ent(), 1),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.f), pixels(88)})
                           .with_no_wrap()
                           .with_margin(Margin{.bottom = DefaultSpacing::medium()})
                           .with_debug_name("wg_images"));
    int slot = 0;
    for (const raylib::Texture2D *t : {&gear, &star, &trophy}) {
      image(context, mk(irow.ent(), slot++),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(80), pixels(80)})
                .with_texture(*t,
                              afterhours::texture_manager::HasTexture::Alignment::Center)
                .with_custom_background(afterhours::Color{40, 46, 62, 255})
                .with_corner_radius(8.f)
                .with_margin(Margin{.right = pixels(8)}));
    }

    section(context, right.ent(), 2, "icon_row from one spritesheet");

    std::vector<afterhours::texture_manager::Rectangle> frames = {
        whole(home), whole(heart), whole(star), whole(gear)};
    icon_row(context, mk(right.ent(), 3), home, frames, 0.5f,
             ComponentConfig{}
                 .with_size(ComponentSize{percent(1.f), pixels(72)})
                 .with_margin(Margin{.bottom = DefaultSpacing::medium()})
                 .with_debug_name("wg_iconrow"));

    section(context, right.ent(), 4, "with_icon on a button, either side");

    auto brow = hstack(context, mk(right.ent(), 5),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.f), pixels(52)})
                           .with_no_wrap()
                           .with_debug_name("wg_iconbtns"));

    button(context, mk(brow.ent(), 0),
           ComponentConfig{}
               .with_label("Save")
               .with_size(ComponentSize{pixels(150), pixels(46)})
               .with_icon(gear, whole(gear))
               .with_icon_position(IconPosition::Left)
               .with_corner_radius(8.f)
               .with_margin(Margin{.right = pixels(10)})
               .with_debug_name("wg_btn_icon_left"));

    button(context, mk(brow.ent(), 1),
           ComponentConfig{}
               .with_label("Next")
               .with_size(ComponentSize{pixels(150), pixels(46)})
               .with_icon(star, whole(star))
               .with_icon_position(IconPosition::Right)
               .with_corner_radius(8.f)
               .with_debug_name("wg_btn_icon_right"));
  }
};

REGISTER_EXAMPLE_SCREEN(widget_gap_gallery, "Component Galleries",
                        "button_group, pagination, spacer, image, icon_row",
                        WidgetGapGallery)
