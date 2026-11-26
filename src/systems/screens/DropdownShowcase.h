#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include "GalleryPage.h"
#include <afterhours/ah.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct DropdownShowcase : ScreenSystem<UIContext<InputAction>> {
  // Selection indices for each dropdown
  size_t basic_idx = 0;
  size_t labeled_idx = 1;
  size_t many_idx = 0;
  size_t custom_indicator_idx = 0;
  size_t disabled_idx = 2;
  size_t styled_idx = 0;

  // Option data
  std::vector<std::string> basic_options = {"Alpha", "Beta", "Gamma"};
  std::vector<std::string> labeled_options = {"Dog", "Cat", "Bird", "Fish"};
  std::vector<std::string> many_options = {
      "January", "February", "March",     "April",   "May",      "June",
      "July",    "August",   "September", "October", "November", "December"};
  std::vector<std::string> indicator_options = {"Option A", "Option B",
                                                "Option C"};
  std::vector<std::string> disabled_options = {"Locked", "Sealed", "Frozen"};
  std::vector<std::string> styled_options = {"Ruby", "Emerald", "Sapphire",
                                             "Topaz"};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.theme.primary = {47, 74, 109, 255};
    context.theme.surface = {27, 36, 51, 255};
    context.theme.font = {236, 241, 250, 255};
    context.theme.font_muted = {171, 188, 212, 255};
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const auto ink = context.theme.font;
    const auto muted = context.theme.font_muted;

    // Page scaffold + header come from the shared gallery template
    // (GalleryPage.h); this screen only fills specimen cards and the
    // status bar below the grid. The template reproduces the geometry
    // this screen used to hand-place: 1144-wide centred column, header
    // band at the top, 368x188 cards on a 20/22 gap grid.
    gallery::GalleryOptions opts;
    opts.scale = s;
    opts.ink = ink;
    opts.muted = muted;
    opts.background = {17, 23, 34, 255};
    opts.header_background = {31, 42, 59, 255};
    opts.card_background = {27, 36, 51, 255};
    opts.background_debug_name = "dropdown_bg";
    opts.title_debug_name = "dropdown_title";
    auto page = gallery::gallery_page(
        context, entity, "Dropdown Showcase",
        "Choose a value. Compare labels, markers, disabled state, and custom styling.",
        opts);

    // One dropdown specimen inside a card body. The dropdown is a flow
    // child now (its parent card positions it); the post-build walk below
    // still restyles the dropdown's internal label/trigger because the
    // library exposes no config for label width, text offset, or the
    // hand-drawn chevron -- that walk is dropdown-specific and stays with
    // the specimen that needs it.
    const auto specimen = [&](afterhours::Entity &parent, int id, const auto &options, size_t &selected, float w,
                              const std::string &name, bool disabled = false, bool custom = false,
                              bool labeled = false, bool markers = false) {
      auto cfg = ComponentConfig{}.with_size({pixels(w * s), pixels(44 * s)})
          .with_font("AtkinsonMock", pixels(22 * s))
          .with_custom_background(custom ? afterhours::Color{218, 180, 117, 255}
                                        : afterhours::Color{47, 74, 109, 255})
          .with_custom_text_color(custom ? afterhours::Color{30, 35, 44, 255} : ink)
          .with_corner_radius((custom ? 12.f : 6.f) * s).with_disabled(disabled)
          .with_dropdown_indicators(markers ? " [+]" : "", markers ? " [-]" : "")
          .with_debug_name(name).with_render_layer(10).with_skip_grid_snap();
      if (custom) cfg.with_border({240, 223, 192, 255}, 2 * s);
      if (labeled) cfg.with_label("Pet");
      ElementResult result = dropdown(context, mk(parent, id), options, selected, cfg);
      auto &outer = result.ent();
      const bool expanded = outer.get<HasDropdownState>().on;
      for (const auto child_id : result.cmp().children) {
        auto child = UICollectionHolder::getEntityForID(child_id);
        if (!child.valid() || !child.asE().has<UIComponent>()) continue;
        auto &node = child.asE();
        auto &cmp = node.get<UIComponent>();
        const auto debug = node.has<UIComponentDebug>() ? node.get<UIComponentDebug>().name() : "";
        if (debug == "dropdown_label") {
          cmp.desired[Axis::X] = pixels(84 * s);
          node.get<HasLabel>().alignment = TextAlignment::Left;
          node.get<HasLabel>().text_x_offset = 10 * s;
          node.get<afterhours::HasColor>().set({36, 47, 65, 255});
          continue;
        }
        if (debug != "option 1") continue;
        node.get<HasLabel>().alignment = TextAlignment::Left;
        node.get<HasLabel>().text_x_offset = 12 * s;
        if (labeled) cmp.desired[Axis::X] = pixels((w - 84) * s);
        node.addComponentIfMissing<UIComponentDebug>(name + "_trigger").set(name + "_trigger");
        if (disabled) node.removeComponentIfExists<HasClickListener>();
        if (markers) continue;
        node.addComponentIfMissing<HasOnDraw>().fg = [s, expanded, disabled, custom, ink, muted](RectangleType r) {
          const float center_x = r.x + r.width - 20 * s;
          const float center_y = r.y + r.height / 2;
          const float direction = expanded ? -1.f : 1.f;
          const auto color = disabled ? muted : custom ? afterhours::Color{30, 35, 44, 255} : ink;
          afterhours::draw_line_ex({center_x - 5 * s, center_y - direction * 2 * s}, {center_x, center_y + direction * 3 * s}, 2 * s, color);
          afterhours::draw_line_ex({center_x, center_y + direction * 3 * s}, {center_x + 5 * s, center_y - direction * 2 * s}, 2 * s, color);
        };
      }
    };

    // Caption line inside a card body or the status bar.
    const auto caption = [&](afterhours::Entity &parent, int id, const std::string &text, float w, float h,
                             float size, afterhours::Color color, float margin_top = 0.f,
                             const std::string &name = "") {
      div(context, mk(parent, id), ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_skip_grid_snap()
          .with_label(text).with_font("AtkinsonMock", pixels(size * s))
          .with_custom_text_color(color).with_alignment(TextAlignment::Left)
          .with_text_overflow(TextOverflow::Ellipsis).with_ignore_pointer_events()
          .with_margin(Margin{.top = pixels(margin_top * s)})
          .with_background(Theme::Usage::None).with_corner_radius(0).with_debug_name(name));
    };

    // Specimen cards. The builder runs per card with the card entity;
    // each body is a vstack whose margins reproduce the caption offsets
    // the absolute layout used (dropdown, then two caption lines).
    const std::array<gallery::Specimen, 6> specimens{{
        {"Greek letter", "col_basic"},
        {"Pet / built-in label", "col_labeled"},
        {"Month / 12 options", "col_many"},
        {"Preset / custom markers", "col_indicators"},
        {"Disabled example", "col_disabled"},
        {"Gemstone / style comparison", "col_styled"},
    }};
    const auto build_card = [&](UIContext<InputAction> &ctx, afterhours::Entity &card, size_t i) {
      (void)ctx;
      if (i == 5) {
        // Styled comparison: two half-width dropdowns side by side, their
        // Default/Custom labels underneath, then two full-width captions.
        auto body = vstack(context, mk(card, 1), ComponentConfig{}
            .with_size({pixels(336 * s), pixels(126 * s)})
            .with_skip_grid_snap()
            .with_background(Theme::Usage::None).with_corner_radius(0));
        auto drops = hstack(context, mk(body.ent(), 0), ComponentConfig{}
            .with_size({pixels(336 * s), pixels(44 * s)}).with_gap(pixels(12 * s)).with_skip_grid_snap()
            .with_no_wrap().with_background(Theme::Usage::None).with_corner_radius(0));
        specimen(drops.ent(), 46, styled_options, styled_idx, 162, "default_gem_dropdown");
        specimen(drops.ent(), 47, styled_options, styled_idx, 162, "styled_dropdown", false, true);
        auto kinds = hstack(context, mk(body.ent(), 1), ComponentConfig{}
            .with_size({pixels(336 * s), pixels(24 * s)}).with_gap(pixels(12 * s)).with_skip_grid_snap()
            .with_no_wrap().with_background(Theme::Usage::None).with_corner_radius(0)
            .with_margin(Margin{.top = pixels(3 * s)}));
        caption(kinds.ent(), 0, "Default", 162, 24, 18, muted);
        caption(kinds.ent(), 1, "Custom", 162, 24, 18, muted);
        caption(body.ent(), 2, "#DAB475 fill / #F0DFC0 border", 336, 24, 18, ink, 5);
        caption(body.ent(), 3, "2px requested border / 12px corners", 336, 24, 17, muted, 2);
        return;
      }
      auto body = vstack(context, mk(card, 1), ComponentConfig{}
          .with_size({pixels(336 * s), pixels(114 * s)})
          .with_skip_grid_snap()
          .with_background(Theme::Usage::None).with_corner_radius(0));
      switch (i) {
        case 0:
          specimen(body.ent(), 40, basic_options, basic_idx, 336, "basic_dropdown");
          caption(body.ent(), 60, "Basic selection / 3 options", 336, 28, 20, muted, 11);
          caption(body.ent(), 61, "Value: " + basic_options[basic_idx], 336, 28, 20, ink, 3);
          break;
        case 1:
          specimen(body.ent(), 41, labeled_options, labeled_idx, 336, "labeled_dropdown", false, false, true);
          caption(body.ent(), 60, "Separate label and selectable value", 336, 28, 19, muted, 11);
          caption(body.ent(), 61, "Value: " + labeled_options[labeled_idx], 336, 28, 20, ink, 3);
          break;
        case 2:
          specimen(body.ent(), 42, many_options, many_idx, 336, "many_dropdown");
          caption(body.ent(), 60, "12 months / scroll to choose", 336, 28, 20, muted, 11);
          caption(body.ent(), 61, "Open to inspect screen-edge clamping", 336, 28, 18, ink, 3);
          break;
        case 3:
          specimen(body.ent(), 43, indicator_options, custom_indicator_idx, 336, "indicator_dropdown", false, false, false, true);
          caption(body.ent(), 60, "[+] closed / [-] open", 336, 28, 20, muted, 11);
          caption(body.ent(), 61, "Three selectable preset slots", 336, 28, 20, ink, 3);
          break;
        default:
          specimen(body.ent(), 44, disabled_options, disabled_idx, 336, "disabled_dropdown", true);
          caption(body.ent(), 60, "Frozen / unavailable", 336, 28, 21, ink, 11);
          caption(body.ent(), 61, "Read-only example; selection is fixed", 336, 28, 19, muted, 3);
          break;
      }
    };
    gallery::specimen_grid(context, page.page.ent(), specimens, build_card, opts);

    // Status bar: current selections on the left, a placement diagram on
    // the right. A page section like the grid, pulled up 4 units so its
    // top matches the old absolute placement (the page gap is 24, the
    // status gap was 20).
    auto status = hstack(context, mk(page.page.ent(), 901), ComponentConfig{}
        .with_skip_grid_snap()
        .with_size({pixels(1144 * s), pixels(138 * s)})
        .with_custom_background({30, 40, 57, 255}).with_corner_radius(10 * s)
        .with_no_wrap().with_margin(Margin{.top = pixels(-4 * s)})
        .with_debug_name("dropdown_status"));
    auto selections = vstack(context, mk(status.ent(), 0), ComponentConfig{}
        .with_skip_grid_snap()
        .with_size({pixels(808 * s), pixels(138 * s)})
        .with_padding(Padding{.top = pixels(10 * s), .left = pixels(20 * s),
                               .bottom = pixels(0), .right = pixels(0)})
        .with_background(Theme::Usage::None).with_corner_radius(0));
    caption(selections.ent(), 0, "Current selections", 720, 30, 22, ink);
    auto row1 = hstack(context, mk(selections.ent(), 1), ComponentConfig{}
        .with_size({pixels(756 * s), pixels(28 * s)}).with_gap(pixels(12 * s)).with_skip_grid_snap()
        .with_no_wrap().with_background(Theme::Usage::None).with_corner_radius(0)
        .with_margin(Margin{.top = pixels(11 * s)}));
    caption(row1.ent(), 0, "Basic: " + basic_options[basic_idx], 244, 28, 21, ink);
    caption(row1.ent(), 1, "Pet: " + labeled_options[labeled_idx], 244, 28, 21, ink);
    caption(row1.ent(), 2, "Month: " + many_options[many_idx], 244, 28, 21, ink);
    auto row2 = hstack(context, mk(selections.ent(), 2), ComponentConfig{}
        .with_size({pixels(756 * s), pixels(28 * s)}).with_gap(pixels(12 * s)).with_skip_grid_snap()
        .with_no_wrap().with_background(Theme::Usage::None).with_corner_radius(0)
        .with_margin(Margin{.top = pixels(14 * s)}));
    caption(row2.ent(), 0, "Preset: " + indicator_options[custom_indicator_idx], 244, 28, 21, ink);
    caption(row2.ent(), 1, "Disabled: " + disabled_options[disabled_idx], 244, 28, 21, ink);
    caption(row2.ent(), 2, "Gemstone: " + styled_options[styled_idx], 244, 28, 21, ink);
    auto placement = vstack(context, mk(status.ent(), 1), ComponentConfig{}
        .with_skip_grid_snap()
        .with_size({pixels(320 * s), pixels(138 * s)})
        .with_padding(Padding{.top = pixels(10 * s), .left = pixels(0),
                               .bottom = pixels(0), .right = pixels(0)})
        .with_background(Theme::Usage::None).with_corner_radius(0));
    caption(placement.ent(), 0, "Placement: below, flip, then clamp", 320, 28, 18, muted);
    div(context, mk(placement.ent(), 1), ComponentConfig{}
        .with_size({pixels(286 * s), pixels(63 * s)})
        .with_margin(Margin{.top = pixels(12 * s), .left = pixels(16 * s)})
        .with_ignore_pointer_events().with_on_draw_fg([s](RectangleType r) {
          const afterhours::Color line{138, 169, 210, 255};
          for (int i = 0; i < 2; ++i) {
            const float x = r.x + i * 160 * s;
            const float y = r.y + (i == 0 ? 0 : 42 * s);
            afterhours::draw_rectangle_outline({x, y, 105 * s, 18 * s}, line, s);
            const float popup_y = i == 0 ? y + 24 * s : y - 39 * s;
            afterhours::draw_rectangle_outline({x, popup_y, 105 * s, 33 * s}, line, s);
          }
        }).with_debug_name("dropdown_placement"));
  }
};

REGISTER_EXAMPLE_SCREEN(dropdowns, "Component Galleries",
                        "Dropdown variations: basic, labeled, many options, "
                        "custom indicators, disabled, styled",
                        DropdownShowcase)
