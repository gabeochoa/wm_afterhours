#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct PaginationShowcase : ScreenSystem<UIContext<InputAction>> {
  // Pagination configuration
  // When true, arrows wrap around (last->first, first->last)
  // When false, arrows are disabled at boundaries
  bool enable_wraparound = false;

  // Pagination state
  size_t page_idx = 0;
  size_t difficulty_idx = 1;
  size_t color_idx = 2;

  std::vector<std::string> pages = {"Page 1", "Page 2", "Page 3", "Page 4",
                                    "Page 5"};
  std::vector<std::string> difficulties = {"Easy", "Medium", "Hard", "Expert"};
  std::vector<std::string> color_options = {"Red", "Green", "Blue", "Yellow",
                                            "Purple"};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Proportional;
    const float scale = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float left = (context.screen_width / scale - 1144) / 2;
    const float top = (context.screen_height / scale - 720) / 2;
    const afterhours::Color ink{237, 243, 251, 255};
    const afterhours::Color muted{175, 193, 216, 255};
    const afterhours::Color panel{25, 36, 53, 255};
    const auto box = [scale, left, top](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position((left + x) * scale, (top + y) * scale)
          .with_background(Theme::Usage::None).with_corner_radius(0);
    };
    // Background
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({15, 23, 36, 255}).with_corner_radius(0).with_debug_name("bg"));
    const auto label = [&](int id, const std::string &text, float x, float y, float w, float h,
                           float size, afterhours::Color color, const std::string &name = "") {
      return div(context, mk(root.ent(), id), box(x, y, w, h).with_label(text)
          .with_font("AtkinsonMock", pixels(size * scale)).with_custom_text_color(color)
          .with_alignment(TextAlignment::Left).with_text_overflow(TextOverflow::Wrap)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    // Title
    div(context, mk(root.ent(), 0), box(0, 24, 1144, 98)
        .with_custom_background({29, 43, 63, 255}).with_corner_radius(12 * scale));
    label(1, "Pagination and option selectors", 20, 33, 1104, 44, 34, ink, "pagination_title");
    label(2, "Choose a page or one option. Each row keeps its own selection.", 20, 83, 1104, 27, 21, muted);
    const auto panel_at = [&](int id, float y, const std::string &name) {
      div(context, mk(root.ent(), id), box(0, y, 1144, 166)
          .with_custom_background(panel).with_corner_radius(10 * scale).with_debug_name(name));
      div(context, mk(root.ent(), id + 1), box(704, y + 16, 420, 134)
          .with_custom_background({32, 47, 68, 255}).with_corner_radius(8 * scale));
    };
    panel_at(3, 138, "pag_pages_panel");
    panel_at(5, 320, "pag_difficulty_panel");
    panel_at(7, 502, "pag_colors_panel");
    const auto arrow = [&](int id, bool previous, bool disabled, float x, float y, const std::string &name) {
      auto result = button(context, mk(root.ent(), id), box(x, y, 48, 48)
          .with_custom_background(disabled ? afterhours::Color{32, 43, 58, 255} : afterhours::Color{48, 67, 94, 255})
          .with_corner_radius(7 * scale).with_disabled(disabled).with_skip_tabbing(disabled)
          .with_debug_name(name).with_on_draw_fg([previous, disabled, scale, ink, muted](RectangleType r) {
            const float direction = previous ? -1.f : 1.f;
            const float cx = r.x + r.width / 2;
            const float cy = r.y + r.height / 2;
            const auto color = disabled ? muted : ink;
            afterhours::draw_line_ex({cx - direction * 4 * scale, cy - 8 * scale},
                                     {cx + direction * 4 * scale, cy}, 2.5f * scale, color);
            afterhours::draw_line_ex({cx + direction * 4 * scale, cy},
                                     {cx - direction * 4 * scale, cy + 8 * scale}, 2.5f * scale, color);
          }));
      if (disabled) {
        result.ent().removeComponent<HasClickListener>();
        return false;
      }
      result.ent().removeComponentIfExists<SkipWhenTabbing>();
      return static_cast<bool>(result);
    };
    const auto choose = [&](int id, const std::string &text, bool selected, float x, float y,
                            float width, const std::string &name, afterhours::Color swatch = afterhours::Color{0, 0, 0, 0}) {
      auto result = button(context, mk(root.ent(), id), box(x, y, width, 48).with_label(text)
          .with_font("AtkinsonMock", pixels(22 * scale)).with_custom_text_color(ink)
          .with_custom_background({38, 55, 77, 255}).with_corner_radius(7 * scale)
          .with_debug_name(name).with_on_draw_fg([selected, swatch, scale, ink](RectangleType r) {
            if (swatch.a > 0)
              afterhours::draw_rectangle({r.x + 7 * scale, r.y + 18 * scale, 12 * scale, 12 * scale}, swatch);
            if (!selected) return;
            afterhours::draw_rectangle_outline({r.x + scale, r.y + scale, r.width - 2 * scale, r.height - 2 * scale}, ink, 2 * scale);
            afterhours::draw_rectangle({r.x + r.width / 2 - 12 * scale, r.y + r.height - 7 * scale, 24 * scale, 3 * scale}, ink);
          }));
      if (swatch.a > 0) result.ent().get<HasLabel>().text_x_offset = 15 * scale;
      return static_cast<bool>(result);
    };
    const auto step = [&](size_t &index, size_t count, bool previous) {
      if (previous) {
        if (index > 0) --index;
        else if (enable_wraparound) index = count - 1;
        return;
      }
      if (index + 1 < count) ++index;
      else if (enable_wraparound) index = 0;
    };
    const auto boundary = [&](size_t index, size_t count) {
      if (enable_wraparound) return std::string("Arrows wrap between the first and last option.");
      if (index == 0) return std::string("Previous unavailable / first option.");
      if (index + 1 == count) return std::string("Next unavailable / last option.");
      return std::string("Stops at the first and last option.");
    };

    // Section 1: Basic Pagination
    label(10, "Pages / 5 options", 20, 148, 662, 31, 25, ink);
    if (arrow(100, true, !enable_wraparound && page_idx == 0, 20, 192, "pag1_previous"))
      step(page_idx, pages.size(), true);
    for (size_t i = 0; i < pages.size(); ++i)
      if (choose(101 + static_cast<int>(i), std::to_string(i + 1), page_idx == i,
                 84 + static_cast<float>(i) * 80, 192, 64, "pag1_btn_" + std::to_string(i))) page_idx = i;
    if (arrow(106, false, !enable_wraparound && page_idx + 1 == pages.size(), 484, 192, "pag1_next"))
      step(page_idx, pages.size(), false);
    label(12, "Selected: Page " + std::to_string(page_idx + 1) + " of 5", 20, 249, 662, 27, 22, ink, "pag1_result");
    label(13, boundary(page_idx, pages.size()), 20, 278, 662, 22, 18, muted);
    const std::array<const char *, 5> page_titles{"Welcome", "Controls", "Layouts", "Themes", "Summary"};
    const std::array<const char *, 5> page_content{
        "Start with the gallery's interactive samples.", "Explore buttons, sliders, and selectors.",
        "Arrange content with rows, columns, and grids.", "Choose colors, type, and shape for your UI.", "You reached the final sample page."};
    label(14, page_titles[page_idx], 724, 164, 380, 34, 28, ink, "pag1_preview_title");
    label(15, page_content[page_idx], 724, 207, 380, 71, 22, muted, "pag1_preview_body");

    // Section 2: Difficulty Selector
    label(20, "Difficulty / 4 options", 20, 330, 662, 31, 25, ink);
    if (arrow(200, true, !enable_wraparound && difficulty_idx == 0, 20, 374, "pag2_previous"))
      step(difficulty_idx, difficulties.size(), true);
    for (size_t i = 0; i < difficulties.size(); ++i)
      if (choose(201 + static_cast<int>(i), difficulties[i], difficulty_idx == i,
                 84 + static_cast<float>(i) * 116, 374, 104, "pag2_btn_" + std::to_string(i))) difficulty_idx = i;
    if (arrow(205, false, !enable_wraparound && difficulty_idx + 1 == difficulties.size(), 552, 374, "pag2_next"))
      step(difficulty_idx, difficulties.size(), false);
    label(22, "Difficulty: " + difficulties[difficulty_idx] + " / " + std::to_string(difficulty_idx + 1) + " of 4",
          20, 431, 662, 27, 22, ink, "pag2_result");
    label(23, boundary(difficulty_idx, difficulties.size()), 20, 460, 662, 22, 18, muted);
    label(24, "Single-choice selector", 724, 346, 380, 34, 25, ink);
    const std::array<const char *, 4> difficulty_descriptions{"Easy / relaxed pace", "Medium / balanced challenge", "Hard / demanding play", "Expert / highest intensity"};
    label(25, difficulty_descriptions[difficulty_idx], 724, 389, 380, 58, 22, muted, "pag2_preview");
    label(26, "Same controls, independent selection.", 724, 451, 380, 24, 18, muted);

    label(30, "Named colors / 5 options", 20, 512, 662, 31, 25, ink);
    const std::array<afterhours::Color, 5> swatches{{{220, 65, 83, 255}, {73, 175, 112, 255},
        {70, 139, 225, 255}, {236, 196, 69, 255}, {158, 110, 213, 255}}};
    if (arrow(300, true, !enable_wraparound && color_idx == 0, 20, 556, "pag3_previous"))
      step(color_idx, color_options.size(), true);
    for (size_t i = 0; i < color_options.size(); ++i)
      if (choose(301 + static_cast<int>(i), color_options[i], color_idx == i,
                 84 + static_cast<float>(i) * 104, 556, 96, "pag3_btn_" + std::to_string(i), swatches[i])) color_idx = i;
    if (arrow(306, false, !enable_wraparound && color_idx + 1 == color_options.size(), 612, 556, "pag3_next"))
      step(color_idx, color_options.size(), false);
    label(32, "Selected: " + color_options[color_idx] + " / " + std::to_string(color_idx + 1) + " of 5",
          20, 613, 662, 27, 22, ink, "pag3_result");
    label(33, boundary(color_idx, color_options.size()), 20, 642, 662, 22, 18, muted);
    div(context, mk(root.ent(), 34), box(724, 535, 86, 98).with_custom_background(swatches[color_idx])
        .with_corner_radius(7 * scale).with_debug_name("pag3_swatch"));
    label(35, color_options[color_idx], 830, 533, 274, 36, 29, ink);
    const auto color = swatches[color_idx];
    label(36, fmt::format("#{:02X}{:02X}{:02X}", color.r, color.g, color.b), 830, 577, 274, 30, 22, muted);
    label(37, "White outline marks selection.", 830, 613, 274, 42, 18, muted);
    label(40, "Click a choice or use Tab + Enter. Arrows move one option; each row preserves the other selections.",
          0, 681, 1144, 29, 20, muted);
  }
};

REGISTER_EXAMPLE_SCREEN(pagination, "Component Galleries",
                        "Pagination widget demo", PaginationShowcase)
