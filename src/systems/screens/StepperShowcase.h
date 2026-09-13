#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct StepperShowcase : ScreenSystem<UIContext<InputAction>> {
  // Left column: various steppers
  size_t resolution_idx = 2;
  std::vector<std::string> resolutions = {"1280 × 720", "1920 × 1080", "2560 × 1440",
                                          "3840 × 2160"};

  size_t quality_idx = 1;
  std::vector<std::string> qualities = {"Low", "Medium", "High", "Ultra"};

  size_t language_idx = 0;
  std::vector<std::string> languages = {"English", "Deutsch", "Francais",
                                        "Espanol", "Japanese"};

  size_t difficulty_idx = 1;
  std::vector<std::string> difficulties = {"Easy", "Normal", "Hard",
                                           "Nightmare"};

  size_t framerate_idx = 1;
  std::vector<std::string> framerates = {"30 FPS", "60 FPS", "120 FPS", "Unlimited"};

  // Right column: stepper controlling a card display
  size_t card_idx = 0;

  struct CardData {
    std::string title;
    std::string description;
    afterhours::Color color;
  };

  std::vector<CardData> cards = {
      {"Warrior", "Heavy armor, melee attacks, high defense.",
       afterhours::Color{170, 75, 65, 255}},
      {"Mage", "Ranged spells, low defense, high damage.",
       afterhours::Color{60, 80, 180, 255}},
      {"Rogue", "Fast attacks, stealth, critical strikes.",
       afterhours::Color{60, 160, 80, 255}},
      {"Healer", "Support class, restoration, shields.",
       afterhours::Color{180, 160, 50, 255}},
  };

  std::vector<std::string> card_names() {
    std::vector<std::string> names;
    for (auto &c : cards)
      names.push_back(c.title);
    return names;
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.scaling_mode = ScalingMode::Proportional;
    const float scale = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float left = (context.screen_width / scale - 1144) / 2;
    const float top = (context.screen_height / scale - 720) / 2;
    const afterhours::Color ink{238, 244, 253, 255}, muted{182, 197, 218, 255};
    const afterhours::Color panel{25, 37, 56, 255}, field{35, 51, 74, 255};
    const auto box = [scale, left, top](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position((left + x) * scale, (top + y) * scale)
          .with_background(Theme::Usage::None).with_corner_radius(0);
    };
    auto root = div(context, mk(entity), ComponentConfig{}.with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background({13, 22, 36, 255}).with_corner_radius(0).with_debug_name("root"));
    const auto label = [&](int id, float x, float y, float w, float h, const std::string &value,
                           float size, afterhours::Color color, const std::string &name = "") {
      return div(context, mk(root.ent(), id), box(x, y, w, h).with_label(value)
          .with_font("AtkinsonMock", pixels(size * scale)).with_custom_text_color(color)
          .with_alignment(TextAlignment::Left).with_text_overflow(TextOverflow::Wrap)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    label(0, 0, 24, 1144, 44, "Stepper Showcase", 36, ink);
    label(1, 0, 78, 865, 56, "Both variants cycle through a finite list. Every arrow wraps from the last option to the first.", 23, muted);
    if (button(context, mk(root.ent(), 2), box(904, 83, 240, 44).with_label("Restore defaults")
        .with_font("AtkinsonMock", pixels(23 * scale)).with_custom_background(field)
        .with_custom_text_color(ink).with_corner_radius(6 * scale).with_debug_name("step_reset"))) {
      resolution_idx = 2;
      quality_idx = difficulty_idx = framerate_idx = 1;
      language_idx = card_idx = 0;
    }
    for (int i = 0; i < 2; ++i)
      div(context, mk(root.ent(), 10 + i), box(static_cast<float>(i) * 592, 155, 552, 513)
          .with_custom_background(panel).with_corner_radius(10 * scale).with_debug_name(i == 0 ? "left_col" : "right_col"));
    label(20, 20, 171, 512, 37, "Stepper Variants", 28, ink);
    label(21, 612, 171, 512, 37, "Card Selector", 28, ink);
    const auto style_stepper = [&](ElementResult &result, const std::string &name, bool neighbors) {
      int arrow_index = 0;
      for (const auto child_id : result.cmp().children) {
        auto &child = UICollectionHolder::getEntityForIDEnforce(child_id);
        auto &cmp = child.get<UIComponent>();
        if (child.has<HasClickListener>()) {
          const bool previous = arrow_index++ == 0;
          cmp.set_desired_width(pixels(44 * scale));
          child.addComponentIfMissing<UIComponentDebug>(name).set(name + (previous ? "_previous" : "_next"));
          child.get<HasLabel>().label.clear();
          child.addComponentIfMissing<afterhours::HasColor>(field).set(field);
          child.addComponentIfMissing<HasOnDraw>().fg = [previous, scale, ink](RectangleType r) {
            const float dir = previous ? -1.f : 1.f;
            const float x = r.x + r.width / 2;
            const float y = r.y + r.height / 2;
            afterhours::draw_line_ex({x - dir * 4 * scale, y - 9 * scale}, {x + dir * 5 * scale, y}, 2.5f * scale, ink);
            afterhours::draw_line_ex({x + dir * 5 * scale, y}, {x - dir * 4 * scale, y + 9 * scale}, 2.5f * scale, ink);
          };
          continue;
        }
        if (!neighbors) continue;
        size_t index = 0;
        for (const auto label_id : cmp.children) {
          auto &choice = UICollectionHolder::getEntityForIDEnforce(label_id);
          choice.get<UIComponent>().set_desired_width(pixels(118 * scale));
          choice.addComponentIfMissing<afterhours::HasColor>(field).set(field);
          const bool center = index++ == 1;
          choice.addComponentIfMissing<HasOnDraw>().fg = [center, scale, ink](RectangleType r) {
            if (!center) return;
            afterhours::draw_rectangle({r.x + 10 * scale, r.y + r.height - 4 * scale, r.width - 20 * scale, 3 * scale}, ink);
          };
        }
      }
    };
    const auto specimen = [&](int id, int row, const std::string &title, const std::vector<std::string> &options,
                               size_t &index, size_t default_index, const std::string &name) {
      const float y = 229 + static_cast<float>(row) * 79;
      label(30 + id, 20, y + 4, 162, 35, title, 23, ink);
      ElementResult result = stepper(context, mk(root.ent(), 100 + id), options, index,
          box(190, y, 342, 45).with_custom_background(field).with_border({92, 112, 140, 255}, 1)
              .with_font("AtkinsonMock", pixels(23 * scale)).with_custom_text_color(ink).with_debug_name(name));
      style_stepper(result, name, false);
      label(50 + id, 190, y + 49, 342, 25, fmt::format("{} / {}  |  {}", index + 1, options.size(),
            index == default_index ? "Default" : "Changed"), 19, muted, name + "_state");
    };
    specimen(0, 0, "Resolution", resolutions, resolution_idx, 2, "step_resolution");
    specimen(1, 1, "Quality", qualities, quality_idx, 1, "step_quality");
    specimen(2, 2, "Language", languages, language_idx, 0, "step_language");
    specimen(3, 3, "Difficulty", difficulties, difficulty_idx, 1, "step_difficulty");
    specimen(4, 4, "Framerate", framerates, framerate_idx, 1, "step_framerate");
    const auto names = card_names();
    ElementResult selector = stepper(context, mk(root.ent(), 110), names, card_idx,
        box(612, 229, 512, 53).with_font("AtkinsonMock", pixels(23 * scale))
            .with_custom_background(field).with_custom_text_color(ink).with_debug_name("step_class"), 3);
    style_stepper(selector, "step_class", true);
    const auto &card = cards[card_idx];
    const afterhours::Color card_ink = card_idx >= 2 ? afterhours::Color{12, 27, 18, 255} : afterhours::Color{255, 255, 255, 255};
    div(context, mk(root.ent(), 120), box(612, 313, 512, 242).with_custom_background(card.color)
        .with_corner_radius(9 * scale).with_debug_name("card"));
    div(context, mk(root.ent(), 121), box(837, 285, 62, 28).with_custom_background(card.color).with_ignore_pointer_events());
    div(context, mk(root.ent(), 122), box(640, 343, 64, 64).with_ignore_pointer_events()
        .with_on_draw_fg([selected = card_idx, card_ink](RectangleType r) {
          const auto point = [r](float x, float y) { return raylib::Vector2{r.x + x * r.width, r.y + y * r.height}; };
          const float stroke = r.width / 16;
          if (selected == 0) {
            afterhours::draw_line_ex(point(.12f, .1f), point(.88f, .1f), stroke, card_ink);
            afterhours::draw_line_ex(point(.12f, .1f), point(.18f, .6f), stroke, card_ink);
            afterhours::draw_line_ex(point(.88f, .1f), point(.82f, .6f), stroke, card_ink);
            afterhours::draw_line_ex(point(.18f, .6f), point(.5f, .92f), stroke, card_ink);
            afterhours::draw_line_ex(point(.82f, .6f), point(.5f, .92f), stroke, card_ink);
            return;
          }
          if (selected == 1) {
            afterhours::draw_line_ex(point(.25f, .9f), point(.65f, .25f), stroke, card_ink);
            raylib::DrawCircleLinesV(point(.7f, .2f), r.width * .15f, card_ink);
            return;
          }
          if (selected == 2) {
            afterhours::draw_triangle(point(.8f, .05f), point(.35f, .62f), point(.55f, .72f), card_ink);
            afterhours::draw_line_ex(point(.45f, .64f), point(.2f, .95f), stroke * 1.4f, card_ink);
            afterhours::draw_line_ex(point(.28f, .53f), point(.64f, .78f), stroke, card_ink);
            return;
          }
          afterhours::draw_rectangle({r.x + r.width * .38f, r.y + r.height * .08f, r.width * .24f, r.height * .84f}, card_ink);
          afterhours::draw_rectangle({r.x + r.width * .08f, r.y + r.height * .38f, r.width * .84f, r.height * .24f}, card_ink);
        }));
    label(123, 728, 340, 368, 47, card.title, 35, card_ink, "step_card_title");
    label(124, 728, 395, 368, 108, card.description, 25, card_ink, "step_card_description");
    label(125, 632, 514, 472, 30, "Selected class / " + std::to_string(card_idx + 1) + " of 4", 22, card_ink);
    label(126, 612, 572, 512, 34, fmt::format("{} accent / #{:02X}{:02X}{:02X}", card.title, card.color.r, card.color.g, card.color.b), 22, ink);
    label(127, 612, 618, 512, 40, "Center segment shows the selected class. Neighbors show the previous and next class.", 20, muted);
    label(130, 0, 686, 1144, 29, "Click an arrow, or Tab to an arrow and press Enter. Every selector wraps; Restore defaults resets all six.", 21, muted);
  }
};

REGISTER_EXAMPLE_SCREEN(
    stepper_showcase, "Component Galleries",
    "Stepper component with various styles and card selector", StepperShowcase)
