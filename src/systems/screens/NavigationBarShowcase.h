#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct NavigationBarShowcase : ScreenSystem<UIContext<InputAction>> {
  size_t category_idx = 0;
  size_t mode_idx = 1;
  size_t level_idx = 0;
  const std::vector<std::string> categories{"Graphics", "Audio", "Controls", "Gameplay"};
  const std::vector<std::string> modes{"Story", "Arcade", "Multiplayer"};
  const std::vector<std::string> levels{"Level 1 of 6", "Level 2 of 6", "Level 3 of 6", "Level 4 of 6", "Level 5 of 6", "Level 6 of 6"};

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context,
                     float) override {
    context.theme = afterhours::ui::theme_presets::ocean_navy();
    context.theme.primary = {43, 74, 106, 255};
    context.theme.secondary = {33, 51, 72, 255};
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const afterhours::Color ink{229, 239, 248, 255}, muted{166, 192, 214, 255};
    const auto box = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_corner_radius(0);
    };
    div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_absolute_position(0, 0).with_background(Theme::Usage::Background).with_corner_radius(0));
    auto root = div(context, mk(entity, 1), ComponentConfig{}
        .with_size({pixels(880 * s), pixels(636 * s)})
        .with_absolute_position((context.screen_width - 880 * s) / 2,
                                (context.screen_height - 636 * s) / 2)
        .with_background(Theme::Usage::None).with_debug_name("navbar_bg"));
    const auto label = [&](int id, const std::string &value, float x, float y,
                           float w, float h, float size, bool subdued = false) {
      return div(context, mk(root.ent(), id), box(x, y, w, h).with_label(value)
          .with_font("AtkinsonMock", pixels(size * s)).with_custom_text_color(subdued ? muted : ink)
          .with_background(Theme::Usage::None).with_alignment(TextAlignment::Left)
          .with_ignore_pointer_events());
    };
    label(0, "Cycle selectors", 0, 0, 880, 44, 32);
    label(1, "Navigation bar / use the arrows to cycle through each example.", 0, 50, 880, 30, 21, true);
    const auto selector = [&](int id, float y, const std::vector<std::string> &options,
                              size_t &selected, const std::string &name) {
      auto result = navigation_bar(context, mk(root.ent(), id), options, selected,
          box(60, y, 760, 54).with_padding(Padding{})
              .with_font("AtkinsonMock", pixels(26 * s)).with_custom_text_color(ink)
              .with_border({79, 106, 132, 255}, 1).with_corner_radius(6 * s)
              .with_debug_name(name));
      const auto &children = result.cmp().children;
      for (size_t i = 0; i < children.size(); ++i) {
        auto child = UICollectionHolder::getEntityForID(children[i]);
        if (!child.valid()) continue;
        auto &item = child.asE();
        item.get<UIComponent>().desired[Axis::X] = pixels((i == 1 ? 640.f : 60.f) * s);
        item.addComponentIfMissing<UIComponentDebug>(name + "_" + std::to_string(i)).set(name + "_" + std::to_string(i));
      }
    };
    label(2, "Settings category", 60, 112, 600, 30, 24);
    label(3, fmt::format("{} of {} / Wraps", category_idx + 1, categories.size()), 610, 112, 210, 30, 19, true);
    selector(10, 150, categories, category_idx, "navbar_category");
    label(4, "Selected category: " + categories[category_idx], 60, 212, 760, 30, 20, true);
    label(5, "Game mode", 60, 274, 600, 30, 24);
    label(6, fmt::format("{} of {} / Wraps", mode_idx + 1, modes.size()), 610, 274, 210, 30, 19, true);
    selector(11, 312, modes, mode_idx, "navbar_mode");
    const std::array<const char *, 3> descriptions{"Follow a sequence of story chapters.", "Quick rounds with a score to beat.", "Play together with other players."};
    label(7, descriptions[mode_idx], 60, 374, 760, 30, 20, true);
    label(8, "Level", 60, 436, 600, 30, 24);
    label(9, "6 options / Wraps", 610, 436, 210, 30, 19, true);
    selector(12, 474, levels, level_idx, "navbar_level");
    const std::array<const char *, 6> names{"Harbour approach", "Forest trail", "Cliffside road", "Moonlit valley", "Highland crossing", "Summit finish"};
    label(13, std::string("Sample level: ") + names[level_idx], 60, 536, 760, 30, 20, true);
    label(14, "Tab to an arrow, then Enter / each selector keeps its own value", 0, 602, 880, 28, 18, true);
  }
};

REGISTER_EXAMPLE_SCREEN(navigation_bar_demo, "Component Galleries",
                        "Navigation bar widget demo", NavigationBarShowcase)
