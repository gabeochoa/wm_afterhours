#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>
#include <algorithm>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// The widgets that shipped and nothing here called: button_group, pagination,
// spacer, image and icon_row. A library feature with no consumer is one nobody
// finds out is broken.
struct WidgetGapGallery : ScreenSystem<UIContext<InputAction>> {
  bool loaded = false;
  raylib::Texture2D gear{}, star{}, trophy{}, sheet{};
  size_t page = 0;
  std::vector<std::string> pages = {"1", "2", "3", "4", "5"};
  int last_group = -1;
  std::vector<std::string> tools = {"Select", "Move", "Rotate", "Scale"};
  size_t configure_count = 0;
  std::string status = "Use either Configure button to compare icon placement.";

  ~WidgetGapGallery() override {
    if (!raylib::IsWindowReady()) return;
    for (auto texture : {gear, star, trophy, sheet}) {
      if (texture.id != 0) raylib::UnloadTexture(texture);
    }
  }

  void load() {
    if (loaded) return;
    loaded = true;
    const auto dir = afterhours::files::get_resource_path("kenney/kenney_game-icons/PNG/White/2x/", "").string();
    gear = raylib::LoadTexture((dir + "gear.png").c_str());
    star = raylib::LoadTexture((dir + "star.png").c_str());
    trophy = raylib::LoadTexture((dir + "trophy.png").c_str());
    auto atlas = raylib::GenImageColor(400, 100, raylib::Color{0, 0, 0, 0});
    const std::array<const char *, 4> names{"home", "star", "gear", "trophy"};
    for (size_t i = 0; i < names.size(); ++i) {
      auto source = raylib::LoadImage((dir + names[i] + ".png").c_str());
      if (!source.data) continue;
      raylib::ImageDraw(&atlas, source, {0, 0, static_cast<float>(source.width), static_cast<float>(source.height)},
          {static_cast<float>(i) * 100, 0, 100, 100}, raylib::Color{255, 255, 255, 255});
      raylib::UnloadImage(source);
    }
    sheet = raylib::LoadTextureFromImage(atlas);
    raylib::UnloadImage(atlas);
  }

  static afterhours::texture_manager::Rectangle whole(const raylib::Texture2D &t) {
    return {0.f, 0.f, static_cast<float>(t.width), static_cast<float>(t.height)};
  }

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context, float) override {
    load();
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float left = (context.screen_width - 1280 * s) / 2;
    const float top = (context.screen_height - 720 * s) / 2;
    const afterhours::Color white{235, 240, 250, 255}, muted{180, 197, 220, 255};
    const afterhours::Color cyan{117, 219, 223, 255}, ink{14, 22, 35, 255}, panel{27, 39, 57, 255};
    const afterhours::Color control{47, 69, 99, 255};
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(20 * s));
    UIStylingDefaults::get().set_grid_snapping(false);
    div(context, mk(entity, 900), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_absolute_position(0, 0).with_custom_background(ink).with_corner_radius(0));
    auto root = div(context, mk(entity, 901), ComponentConfig{}
        .with_size({pixels(1280 * s), pixels(720 * s)})
        .with_absolute_position(left, top).with_transparent_bg().with_skip_grid_snap(true)
        .with_debug_name("wg_root"));
    const auto box = [&](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_transparent_bg()
          .with_corner_radius(0).with_skip_grid_snap(true);
    };
    const auto text = [&](int id, std::string value, float x, float y, float w,
                          float h, float size, afterhours::Color color, const char *debug = "", bool bold = false) {
      div(context, mk(root.ent(), id), box(x, y, w, h).with_label(value)
          .with_font(bold ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s))
          .with_alignment(TextAlignment::Left).with_custom_text_color(color)
          .with_ignore_pointer_events().with_debug_name(debug));
    };
    text(0, "Widget integration gallery", 48, 20, 1184, 44, 36, white, "wg_title", true);
    text(1, "button_group · pagination · spacer · image · icon_row · with_icon", 48, 70, 1184, 28, 21, muted);
    div(context, mk(root.ent(), 2), box(48, 110, 568, 550).with_custom_background(panel).with_corner_radius(12 * s).with_debug_name("wg_left"));
    div(context, mk(root.ent(), 3), box(644, 110, 588, 550).with_custom_background(panel).with_corner_radius(12 * s).with_debug_name("wg_right"));
    const auto style_child = [&](afterhours::Entity &child, float width, bool selected, const std::string &debug) {
      auto &cmp = child.get<UIComponent>();
      cmp.desired[Axis::X] = pixels(width * s);
      cmp.skip_grid_snap = true;
      cmp.enable_font("AtkinsonMock", pixels(20 * s), true);
      for (auto axis : {Axis::left, Axis::right, Axis::top, Axis::bottom}) {
        cmp.desired_padding[axis] = pixels(0);
        cmp.desired_margin[axis] = pixels(0);
      }
      child.addComponentIfMissing<afterhours::HasColor>(control).set(selected ? cyan : control);
      child.addComponentIfMissing<HasRoundedCorners>().set_radius_px(5 * s);
      child.addComponentIfMissing<UIComponentDebug>(debug).set(debug);
      if (child.has<HasLabel>()) {
        auto &label = child.get<HasLabel>();
        label.font_name = "AtkinsonMock";
        label.explicit_text_color = selected ? ink : white;
      }
    };
    text(10, "Editing tools / button_group", 68, 128, 528, 32, 25, white, "", true);
    auto group = button_group(context, mk(root.ent(), 11), tools, box(68, 174, 528, 44)
        .with_flex_direction(FlexDirection::Row).with_font("AtkinsonMock", pixels(20 * s))
        .with_custom_background(control).with_debug_name("wg_group"));
    if (group) last_group = group.as<int>();
    group.ent().get<afterhours::HasColor>().set({0, 0, 0, 0});
    group.cmp().desired_gap = pixels(6 * s);
    for (size_t i = 0; i < group.cmp().children.size(); ++i) {
      auto child = UICollectionHolder::getEntityForID(group.cmp().children[i]);
      if (!child) continue;
      style_child(child.asE(), 127.5f, static_cast<int>(i) == last_group, fmt::format("wg_tool_{}", i));
    }
    text(12, last_group < 0 ? "Selected tool: none" : "Selected tool: " + tools[static_cast<size_t>(last_group)],
         68, 230, 528, 28, 21, cyan, "wg_group_out");
    text(13, "Choose an editing tool. Cyan shows the selection.", 68, 264, 528, 26, 18, muted);
    text(20, "Pagination / five pages", 68, 302, 528, 32, 25, white, "", true);
    const size_t page_before = page;
    auto page_buttons = pagination(context, mk(root.ent(), 21), pages, page, box(68, 348, 528, 44)
        .with_font("AtkinsonMock", pixels(20 * s)).with_custom_background(control).with_debug_name("wg_pages"));
    page_buttons.ent().get<afterhours::HasColor>().set({0, 0, 0, 0});
    page_buttons.cmp().desired_gap = pixels(6 * s);
    const auto page_children = page_buttons.cmp().children;
    for (size_t i = 0; i < page_children.size(); ++i) {
      auto child = UICollectionHolder::getEntityForID(page_children[i]);
      if (!child || !child.asE().has<HasClickListener>() || !child.asE().get<HasClickListener>().down) continue;
      page = i == 0 ? (page_before + pages.size() - 1) % pages.size() : i == pages.size() + 1 ? (page_before + 1) % pages.size() : i - 1;
      page_buttons.ent().get<HasDropdownState>().last_option_clicked = page;
      context.set_focus(child.asE().id);
      break;
    }
    for (size_t i = 0; i < page_children.size(); ++i) {
      auto child = UICollectionHolder::getEntityForID(page_children[i]);
      if (!child) continue;
      style_child(child.asE(), 492.f / 7.f, i == page + 1, fmt::format("wg_page_{}", i));
    }
    text(22, fmt::format("Page {} of 5", page + 1), 68, 402, 528, 28, 21, cyan, "wg_page_out");
    text(23, "Arrows wrap. Cyan is selected; white is focus.", 68, 436, 528, 26, 18, muted);
    text(30, "Flexible space / spacer", 68, 478, 528, 32, 25, white, "", true);
    auto srow = hstack(context, mk(root.ent(), 31), box(68, 524, 528, 40).with_no_wrap().with_debug_name("wg_srow"));
    auto head = div(context, mk(srow.ent(), 0), ComponentConfig{}.with_size({pixels(90 * s), pixels(40 * s)})
        .with_label("head").with_text_inset(0, 0).with_alignment(TextAlignment::Left)
        .with_font("AtkinsonMock", pixels(20 * s)).with_custom_text_color(white)
        .with_custom_background({59, 91, 135, 255}).with_corner_radius(5 * s).with_skip_grid_snap(true).with_debug_name("wg_head"));
    head.ent().get<HasLabel>().text_x_offset = 10 * s;
    auto flexible = spacer(context, mk(srow.ent(), 1));
    flexible.ent().get<UIComponentDebug>().set("wg_space");
    flexible.cmp().skip_grid_snap = true;
    auto tail = div(context, mk(srow.ent(), 2), ComponentConfig{}.with_size({pixels(90 * s), pixels(40 * s)})
        .with_label("tail").with_text_inset(0, 0).with_alignment(TextAlignment::Left)
        .with_font("AtkinsonMock", pixels(20 * s)).with_custom_text_color(white)
        .with_custom_background({131, 88, 61, 255}).with_corner_radius(5 * s).with_skip_grid_snap(true).with_debug_name("wg_tail"));
    tail.ent().get<HasLabel>().text_x_offset = 10 * s;
    div(context, mk(root.ent(), 32), box(158, 574, 348, 8).with_ignore_pointer_events()
        .with_on_draw_fg([muted](RectangleType r) {
          afterhours::draw_rectangle({r.x, r.y + 3, r.width, 1}, muted);
          afterhours::draw_rectangle({r.x, r.y, 1, r.height}, muted);
          afterhours::draw_rectangle({r.x + r.width - 1, r.y, 1, r.height}, muted);
        }));
    text(33, fmt::format("Head {:.0f} px + flexible {:.0f} px + tail {:.0f} px", 90 * s, flexible.cmp().rect().width, 90 * s),
         68, 600, 528, 26, 18, muted, "wg_space_out");
    text(40, "Image tiles / image", 668, 128, 540, 32, 25, white, "", true);
    const std::array<const char *, 3> names{"Gear", "Star", "Trophy"};
    const std::array<raylib::Texture2D, 3> textures{gear, star, trophy};
    for (size_t i = 0; i < textures.size(); ++i) {
      const float x = 668 + static_cast<float>(i) * 112;
      image(context, mk(root.ent(), 41 + static_cast<int>(i)), box(x, 174, 80, 80)
          .with_texture(textures[i], afterhours::texture_manager::HasTexture::Alignment::Center)
          .with_custom_background({43, 57, 77, 255}).with_corner_radius(6 * s)
          .with_debug_name(fmt::format("wg_image_{}", i)));
      text(45 + static_cast<int>(i), names[i], x, 262, 104, 25, 19, white);
    }
    text(48, fmt::format("100 × 100 px sources / {:.0f} × {:.0f} px tiles", 80 * s, 80 * s), 668, 290, 540, 25, 17, muted, "wg_image_size");
    text(50, "One atlas / icon_row", 668, 324, 540, 32, 25, white, "", true);
    const std::array<afterhours::texture_manager::Rectangle, 4> frames{{{0, 0, 100, 100}, {100, 0, 100, 100}, {200, 0, 100, 100}, {300, 0, 100, 100}}};
    auto icon_region = div(context, mk(root.ent(), 51), box(668, 370, 360, 50));
    auto icons = icon_row(context, mk(icon_region.ent(), 0), sheet, frames, 0.5f * s,
        ComponentConfig{}.with_size({percent(1.f), pixels(50 * s)}).with_transparent_bg().with_debug_name("wg_iconrow"));
    icons.cmp().desired_gap = pixels(18 * s);
    for (const auto id : icons.cmp().children) {
      auto child = UICollectionHolder::getEntityForID(id);
      if (child) child.asE().get<UIComponent>().skip_grid_snap = true;
    }
    const std::array<const char *, 4> icon_names{"Home", "Star", "Gear", "Trophy"};
    for (size_t i = 0; i < icon_names.size(); ++i) text(52 + static_cast<int>(i), icon_names[i], 664 + static_cast<float>(i) * 68, 425, 76, 25, 17, white);
    text(56, fmt::format("400 × 100 atlas / {:.0f} px icons / {:.0f} px gaps", 50 * s, 18 * s), 668, 457, 540, 25, 17, muted, "wg_atlas_size");
    text(60, "Icon placement / with_icon", 668, 492, 540, 32, 25, white, "", true);
    for (int i = 0; i < 2; ++i) {
      if (button(context, mk(root.ent(), 61 + i), box(668 + static_cast<float>(i) * 278, 538, 262, 46)
          .with_label("Configure").with_font("AtkinsonMock", pixels(20 * s))
          .with_custom_background(control).with_custom_text_color(white)
          .with_icon(gear, whole(gear)).with_icon_position(i == 0 ? IconPosition::Left : IconPosition::Right)
          .with_padding(Padding{.top = pixels(6 * s), .bottom = pixels(6 * s),
                                .left = pixels(14 * s), .right = pixels(14 * s)})
          .with_corner_radius(6 * s).with_debug_name(i == 0 ? "wg_btn_icon_left" : "wg_btn_icon_right"))) {
        ++configure_count;
        status = fmt::format("Configure {}: {} activation{}", i == 0 ? "left" : "right", configure_count, configure_count == 1 ? "" : "s");
      }
    }
    text(64, "Same gear and label; only icon side changes.", 668, 597, 540, 25, 18, muted);
    text(65, status, 668, 627, 540, 25, 17, cyan, "wg_action");
    text(70, "Kenney game-icons / White / 2x. Image tiles and atlas use the same unmodified source art.", 48, 681, 1184, 26, 18, muted);
  }
};

REGISTER_EXAMPLE_SCREEN(widget_gap_gallery, "Component Galleries",
                        "button_group, pagination, spacer, image, icon_row",
                        WidgetGapGallery)
