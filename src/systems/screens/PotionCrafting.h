#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>
#include <array>
#include <cmath>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct PotionCraftingScreen : ScreenSystem<UIContext<InputAction>> {
  struct Ingredient {
    const char *name;
    const char *art;
    int count;
    const char *note;
  };
  struct Recipe {
    const char *name;
    const char *effect;
    const char *description;
    const char *art;
    std::array<std::pair<size_t, int>, 2> required;
    int seconds;
  };
  std::array<Ingredient, 6> ingredients{{
      {"Moonpetal", "moonpetal", 5,
       "Moonpetal restores balance. Gather it under a clear night sky."},
      {"Fire Root", "fire_root", 3,
       "Fire Root warms the mixture. Handle its glowing roots with care."},
      {"Sea Salt", "sea_salt", 12,
       "Sea Salt steadies the mixture. Add Star Dust only after the water "
       "turns clear."},
      {"Honey Dew", "honey_dew", 8,
       "Honey Dew softens bitter remedies. One drop goes a long way."},
      {"Shadow Moss", "shadow_moss", 2,
       "Shadow Moss binds volatile mixtures. Keep the jar away from sunlight."},
      {"Star Dust", "star_dust", 1,
       "Star Dust awakens the final brew. Sprinkle gently and stir clockwise."},
  }};
  const std::array<Recipe, 4> recipes{{
      {"Healing Potion",
       "Restores 50 HP",
       "A little comfort for a weary traveler.",
       "healing",
       {{{0, 2}, {3, 1}}},
       30},
      {"Mana Elixir",
       "Restores 30 MP",
       "The remedy for an exhausted mind.",
       "mana",
       {{{2, 3}, {5, 1}}},
       45},
      {"Fire Bomb",
       "Deals 25 fire damage",
       "A spark best kept safely in its bottle.",
       "fire",
       {{{1, 2}, {4, 1}}},
       60},
      {"Antidote",
       "Cures poison",
       "A fresh start, one careful drop at a time.",
       "antidote",
       {{{0, 1}, {2, 2}}},
       20},
  }};
  size_t selected_recipe = 1;
  size_t selected_tab = 0;
  int selected_ingredient = -1;
  int potions_brewed = 47;
  float brew_progress = 0;
  std::string last_brew;
  std::string message;
  std::array<raylib::Texture2D, 10> art{};
  bool art_loaded = false;

  bool can_brew(const Recipe &recipe) const {
    for (auto [index, count] : recipe.required)
      if (ingredients[index].count < count)
        return false;
    return true;
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    if (!art_loaded) {
      for (size_t i = 0; i < art.size(); ++i) {
        const auto name = i < recipes.size()
                              ? recipes[i].art
                              : ingredients[i - recipes.size()].art;
        art[i] = raylib::LoadTexture(
            afterhours::files::get_resource_path(
                "images", std::string("potion_crafting/") + name + ".png")
                .string()
                .c_str());
      }
      art_loaded = true;
    }
    const afterhours::Color ink{66, 51, 29, 255}, muted{92, 72, 43, 255};
    const afterhours::Color rule{154, 129, 84, 255}, paper{238, 224, 185, 255};
    const afterhours::Color olive{64, 78, 35, 255}, unavailable{137, 44, 28, 255};
    const float scale = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float left = (context.screen_width / scale - 1280) / 2;
    const float top = (context.screen_height / scale - 720) / 2;
    context.scaling_mode = ScalingMode::Proportional;
    Theme theme;
    theme.font = ink;
    theme.font_muted = muted;
    theme.background = paper;
    theme.surface = paper;
    theme.primary = olive;
    theme.accent = afterhours::Color{109, 82, 39, 255};
    theme.corner_radius = 0;
    context.set_theme(theme);
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_corner_radius(0).with_custom_background(paper).with_debug_name("potion_bg")
        .with_on_draw_bg([](RectangleType r) {
          afterhours::draw_rectangle_gradient_v(r, {224, 207, 162, 255}, {241, 226, 188, 255});
          for (float y = 0; y < r.height; y += 4)
            afterhours::draw_rectangle({r.x, r.y + y, r.width, 1}, {120, 91, 43, 8});
        }));
    const auto box = [scale, left, top](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position((left + x) * scale, (top + y) * scale)
          .with_corner_radius(0).with_background(Theme::Usage::None);
    };
    const int section = static_cast<int>(selected_tab) * 1000;
    const auto eid = [section](int id) { return id < 40 ? id : id + section; };
    const auto text = [&](int id, float x, float y, float w, float h, const std::string &value,
                          float size, afterhours::Color color, const std::string &name = "",
                          TextAlignment align = TextAlignment::Left) {
      return div(context, mk(root.ent(), eid(id)), box(x, y, w, h).with_label(value)
          .with_font("Garamond", pixels(size * scale)).with_custom_text_color(color)
          .with_text_overflow(TextOverflow::Wrap).with_alignment(align)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    const auto line = [&](int id, float x, float y, float w, float h, afterhours::Color color) {
      div(context, mk(root.ent(), eid(id)), box(x, y, w, h).with_custom_background(color)
          .with_ignore_pointer_events());
    };
    const auto picture = [&](int id, size_t index, float x, float y, float w, float h) {
      const auto texture = art[index];
      const RectangleType source = index < 4 ? RectangleType{64, 27, 272, 351}
                                             : RectangleType{0, 0, 400, 400};
      const float fit = std::min(w / source.width, h / source.height);
      const float width = source.width * fit;
      const float height = source.height * fit;
      auto image = sprite(context, mk(root.ent(), eid(id)), texture, source,
          box(x + (w - width) / 2, y + (h - height) / 2, width, height).with_ignore_pointer_events());
      if (index < 4 || index == 6 || index == 9) return;
      image.ent().addComponentIfMissing<HasOnDraw>().fg = [index](RectangleType r) {
        const auto point = [r](float x, float y) { return raylib::Vector2{r.x + x * r.width, r.y + y * r.height}; };
        if (index == 4) {
          for (int i = 0; i < 5; ++i) {
            const float angle = static_cast<float>(i) * 1.256637f;
            raylib::DrawCircleV(point(.48f + .12f * std::cos(angle), .28f + .12f * std::sin(angle)), r.width * .085f, {250, 241, 217, 255});
          }
          raylib::DrawCircleV(point(.48f, .28f), r.width * .065f, {178, 131, 42, 255});
          return;
        }
        if (index == 5) {
          for (int i = 0; i < 4; ++i)
            afterhours::draw_line_ex(point(.57f, .59f), point(.32f + static_cast<float>(i) * .13f, .94f), r.width * .045f, {134, 60, 25, 255});
          return;
        }
        if (index == 7) {
          raylib::DrawTriangle(point(.70f, .57f), point(.62f, .76f), point(.78f, .76f), {189, 111, 14, 255});
          raylib::DrawCircleV(point(.70f, .76f), r.width * .08f, {189, 111, 14, 255});
          return;
        }
        for (int i = 0; i < 5; ++i) {
          const float y = .35f + static_cast<float>(i) * .10f;
          afterhours::draw_line_ex(point(.53f, y), point(.30f, y - .13f), r.width * .035f, {44, 66, 43, 255});
          afterhours::draw_line_ex(point(.53f, y), point(.77f, y - .13f), r.width * .035f, {44, 66, 43, 255});
        }
      };
    };
    text(2, 32, 22, 700, 24, "THE ART OF ALCHEMY", 18, muted);
    text(3, 32, 47, 850, 49, "Alchemist's Workshop", 43, ink, "potion_title");
    div(context, mk(root.ent(), 4), box(1052, 27, 196, 63).with_custom_background(paper).with_border(rule, 1));
    text(5, 1064, 39, 66, 37, "Gold", 22, muted);
    text(8, 1130, 37, 106, 41, "2,450", 32, ink, "potion_gold", TextAlignment::Right);
    line(6, 24, 103, 1232, 1, rule);
    line(7, 24, 157, 1232, 1, rule);
    const std::array<std::string, 3> tabs{"Recipes", "Inventory", "Journal"};
    for (size_t i = 0; i < tabs.size(); ++i) {
      const float x = 24 + 140.f * static_cast<float>(i);
      auto config = box(x, 109, 132, 42).with_label(tabs[i])
          .with_font("Garamond", pixels(27 * scale)).with_custom_text_color(ink)
          .with_alignment(TextAlignment::Center).with_debug_name("potion_tab_" + std::to_string(i));
      if (i == selected_tab) config.with_custom_background({213, 192, 143, 255});
      if (button(context, mk(root.ent(), 10 + static_cast<int>(i)), config)) selected_tab = i;
      if (i == selected_tab) line(20 + static_cast<int>(i), x, 150, 132, 3, ink);
    }
    text(30, 848, 113, 400, 33, "Wednesday, the 12th day of Autumn", 21, muted, "", TextAlignment::Right);
    line(31, 24, 682, 1232, 1, rule);
    text(32, 24, 687, 188, 29, "Alchemy level 12", 21, ink);
    text(33, 223, 687, 171, 29, "4 recipes known", 21, ink);
    text(34, 405, 687, 263, 29, std::to_string(potions_brewed) + " potions brewed", 21, ink, "potion_total");
    for (int i = 0; i < 2; ++i) {
      const float x = 878 + static_cast<float>(i) * 173;
      div(context, mk(root.ent(), 36 + i), box(x, 688, i == 0 ? 49 : 67, 27)
          .with_custom_background(paper).with_border(rule, 1));
      text(38 + i, x, 687, i == 0 ? 49 : 67, 29, i == 0 ? "Tab" : "Enter", 20, ink, "", TextAlignment::Center);
    }
    text(35, 933, 687, 112, 29, "Move", 20, muted);
    text(9, 1124, 687, 124, 29, "Choose", 20, muted);
    if (selected_tab == 2) {
      text(40, 48, 190, 1120, 49, "The alchemist's journal", 36, ink, "potion_journal");
      text(41, 48, 266, 1120, 70, last_brew.empty() ? "No potions bottled today. The workbench is ready." : last_brew,
           30, ink, "potion_journal_entry");
      text(42, 48, 355, 1120, 38, "Each completed brew uses ingredients from your inventory.", 25, muted);
      return;
    }
    const bool inventory = selected_tab == 1;
    if (inventory) {
      text(40, 32, 183, 1100, 48, "Ingredient inventory", 36, ink, "potion_inventory");
      text(41, 32, 233, 1100, 35, "Select an ingredient for field notes. The leading mark identifies ingredients for " + std::string(recipes[selected_recipe].name) + ".", 24, muted);
    } else {
      line(50, 310, 174, 1, 494, rule);
      line(51, 880, 174, 1, 494, rule);
      text(53, 24, 176, 276, 40, "Recipe book", 30, ink);
      for (size_t i = 0; i < recipes.size(); ++i) {
        const float y = 231 + 101.f * static_cast<float>(i);
        const int id = 100 + static_cast<int>(i) * 10;
        auto config = box(24, y, 274, 92).with_debug_name("potion_recipe_" + std::to_string(i));
        if (i == selected_recipe) config.with_custom_background(paper).with_border(rule, 1);
        if (button(context, mk(root.ent(), eid(id)), config)) {
          selected_recipe = i;
          brew_progress = 0;
          message.clear();
        }
        if (i == selected_recipe) line(id + 5, 24, y, 4, 92, olive);
        picture(id + 1, i, 34, y + 18, 44, 57);
        text(id + 2, 84, y + 5, 207, 30, recipes[i].name, 25, ink);
        text(id + 3, 84, y + 35, 207, 25, recipes[i].effect, 21, muted);
        text(id + 4, 84, y + 63, 207, 24, std::to_string(recipes[i].seconds) + " s recipe", 20, muted);
      }
      text(150, 24, 644, 274, 28, "Four recipes, one workbench.", 21, muted);
      const auto &active = recipes[selected_recipe];
      const bool available = can_brew(active);
      text(200, 328, 175, 536, 43, active.name, 36, ink, "potion_recipe_title", TextAlignment::Center);
      text(201, 340, 220, 512, 36, active.description, 23, muted, "", TextAlignment::Center);
      div(context, mk(root.ent(), eid(202)), box(503, 260, 186, 154).with_ignore_pointer_events()
          .with_on_draw_fg([](RectangleType r) {
            const raylib::Vector2 center{r.x + r.width / 2, r.y + r.height / 2};
            const raylib::Color faint{116, 101, 66, 65};
            raylib::DrawCircleLinesV(center, r.height * .48f, faint);
            raylib::DrawPolyLines(center, 3, r.height * .46f, -90, faint);
            raylib::DrawPolyLines(center, 3, r.height * .46f, 90, faint);
          }));
      picture(203, selected_recipe, 523, 261, 146, 148);
      line(204, 461, 411, 270, 3, rule);
      text(207, 340, 423, 512, 31, brew_progress == 1 ? "Brew complete / 100% bottled" : available ? "Ready to brew" : "Missing ingredients",
           25, brew_progress == 1 || available ? olive : unavailable, "potion_progress", TextAlignment::Center);
      for (size_t i = 0; i < active.required.size(); ++i) {
        const auto [index, count] = active.required[i];
        const auto &ingredient = ingredients[index];
        const float x = 328 + 276.f * static_cast<float>(i);
        const int id = 210 + static_cast<int>(i) * 10;
        const bool enough = ingredient.count >= count;
        div(context, mk(root.ent(), eid(id)), box(x, 464, 260, 82).with_custom_background(paper).with_border(rule, 1));
        text(id + 1, x + 10, 469, 240, 28, std::string(ingredient.name) + " × " + std::to_string(count), 24, ink);
        text(id + 2, x + 10, 497, 240, 23, "Required " + std::to_string(count) + " / Owned " + std::to_string(ingredient.count), 20, muted,
             "potion_required_" + std::to_string(i));
        text(id + 3, x + 31, 520, 219, 22, enough ? "After brew: " + std::to_string(ingredient.count - count) + " remaining" : "Not enough in stock",
             19, enough ? olive : unavailable);
        div(context, mk(root.ent(), eid(id + 4)), box(x + 12, 525, 12, 12).with_ignore_pointer_events()
            .with_on_draw_fg([enough, olive, unavailable](RectangleType r) {
              const auto color = enough ? olive : unavailable;
              afterhours::draw_line_ex({r.x, r.y + r.height * .5f}, {r.x + r.width * .4f, r.y + r.height}, 2, color);
              afterhours::draw_line_ex({r.x + r.width * .4f, r.y + r.height}, {r.x + r.width, r.y}, 2, color);
              if (!enough) afterhours::draw_line_ex({r.x, r.y}, {r.x + r.width, r.y + r.height}, 2, color);
            }));
      }
      text(232, 328, 552, 536, 27, std::to_string(active.seconds) + " s recipe / Instant demo / Makes 1 potion", 21, muted, "", TextAlignment::Center);
      if (button(context, mk(root.ent(), eid(230)), box(448, 586, 296, 49)
          .with_label(available ? "Brew potion" : "Missing ingredients").with_font("Garamond", pixels(29 * scale))
          .with_custom_background(available ? olive : afterhours::Color{105, 82, 48, 255})
          .with_border(ink, 1).with_custom_text_color({255, 246, 216, 255})
          .with_alignment(TextAlignment::Center).with_debug_name("potion_brew"))) {
        if (available) {
          for (auto [index, count] : active.required) ingredients[index].count -= count;
          ++potions_brewed;
          brew_progress = 1;
          last_brew = std::string(active.name) + " brewed. " + active.effect + ".";
          message = last_brew;
        } else {
          message = "Gather the missing ingredients before brewing.";
        }
      }
      text(233, 328, 643, 536, 33, message.empty() ? "Combine ingredients, then bottle the potion." : message,
           21, ink, "potion_message", TextAlignment::Center);
    }
    const float gx = inventory ? 32.f : 900.f;
    const float gy = inventory ? 293.f : 228.f;
    if (!inventory) {
      text(300, 900, 176, 348, 40, "Ingredients", 30, ink);
      text(301, 900, 209, 348, 22, "Marked tiles are used by this recipe.", 18, muted);
    }
    for (size_t i = 0; i < ingredients.size(); ++i) {
      const float x = gx + static_cast<float>(inventory ? i : i % 3) * (inventory ? 202.f : 116.f);
      const float y = gy + (inventory ? 0 : static_cast<float>(i / 3) * 140);
      const float width = inventory ? 190.f : 108.f;
      const float height = inventory ? 176.f : 132.f;
      const int id = 310 + static_cast<int>(i) * 10;
      bool required = false;
      for (const auto &[index, count] : recipes[selected_recipe].required) {
        if (index == i && count > 0) required = true;
      }
      const bool selected = selected_ingredient == static_cast<int>(i);
      if (button(context, mk(root.ent(), eid(id)), box(x, y, width, height)
          .with_custom_background(selected ? afterhours::Color{251, 241, 211, 255} : paper)
          .with_border(selected ? ink : rule, selected ? 2 : 1)
          .with_debug_name("potion_ingredient_" + std::to_string(i)))) selected_ingredient = static_cast<int>(i);
      if (required) line(id + 5, x + 3, y + 3, 3, height - 6, olive);
      picture(id + 1, i + 4, x + width / 2 - (inventory ? 52 : 33), y + 22,
              inventory ? 104 : 66, inventory ? 104 : 66);
      text(id + 2, x + 4, y + height - 48, width - 8, 44, ingredients[i].name, inventory ? 26 : 20, ink, "", TextAlignment::Center);
      div(context, mk(root.ent(), eid(id + 4)), box(x + width - 58, y + 5, 51, 24).with_custom_background({216, 195, 147, 255}));
      text(id + 3, x + width - 58, y + 3, 51, 28, "× " + std::to_string(ingredients[i].count), 20, ink,
           "potion_stock_" + std::to_string(i), TextAlignment::Center);
    }
    const float note_y = inventory ? 508.f : 517.f;
    const float note_width = inventory ? 1216.f : 348.f;
    line(380, gx, note_y, note_width, 1, rule);
    const auto &note = ingredients[selected_ingredient < 0 ? 2 : static_cast<size_t>(selected_ingredient)];
    text(381, gx, note_y + 8, note_width, 31, std::string("Recipe notes / ") + note.name, 25, ink);
    text(382, gx, note_y + 43, note_width, inventory ? 80 : 107, note.note, inventory ? 27 : 23, muted, "potion_field_note");
  }
};

REGISTER_EXAMPLE_SCREEN(
    potion_crafting, "Game Mockups",
    "Alchemy crafting workshop with recipes and ingredients",
    PotionCraftingScreen)
