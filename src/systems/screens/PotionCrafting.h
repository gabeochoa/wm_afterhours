#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>
#include <array>

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
  float brew_progress = .65f;
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
    const afterhours::Color ink{77, 67, 45, 255}, muted{131, 113, 75, 255};
    const afterhours::Color rule{161, 139, 97, 255}, paper{232, 217, 175, 255};
    const afterhours::Color olive{119, 116, 72, 255};
    const float scale = context.screen_height / 720.f;
    UIStylingDefaults::get().set_default_font("Garamond", h720(20.f));
    Theme theme;
    theme.font = ink;
    theme.font_muted = muted;
    theme.background = paper;
    theme.surface = paper;
    theme.primary = olive;
    theme.accent = afterhours::Color{109, 82, 39, 255};
    theme.corner_radius = 0;
    context.set_theme(theme);
    auto box = [scale](float x, float y, float w, float h) {
      return ComponentConfig{}
          .with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position(x * scale, y * scale)
          .with_corner_radius(0.f)
          .with_background(Theme::Usage::None);
    };
    auto text = [&](int id, float x, float y, float w, float h,
                    const std::string &label, float size,
                    afterhours::Color color, const std::string &name = "",
                    TextAlignment align = TextAlignment::Left) {
      div(context, mk(entity, id),
          box(x, y, w, h)
              .with_label(label)
              .with_font("Garamond", h720(size * 1.25f))
              .with_text_overflow(TextOverflow::Wrap)
              .with_custom_text_color(color)
              .with_letter_spacing(0)
              .with_alignment(align)
              .with_debug_name(name)
              .with_ignore_pointer_events());
    };
    auto line = [&](int id, float x, float y, float w, float h) {
      div(context, mk(entity, id),
          box(x, y, w, h)
              .with_custom_background(rule)
              .with_ignore_pointer_events());
    };
    auto picture = [&](int id, size_t index, float x, float y, float w,
                       float h) {
      const auto texture = art[index];
      sprite(context, mk(entity, id), texture,
             {0, 0, static_cast<float>(texture.width),
              static_cast<float>(texture.height)},
             box(x, y, w, h).with_ignore_pointer_events());
    };
    div(context, mk(entity, 0),
        box(0, 0, 1280, 720).with_on_draw_bg([](RectangleType r) {
          afterhours::draw_rectangle_gradient_v(r, {215, 199, 153, 255},
                                                {226, 209, 163, 255});
          for (float y = 0; y < r.height; y += 4 * r.height / 720.f)
            afterhours::draw_rectangle(
                {r.x, r.y + y, r.width, r.height / 720.f}, {155, 131, 81, 35});
        }));
    div(context, mk(entity, 1),
        box(36, 47, 36, 36)
            .with_ignore_pointer_events()
            .with_on_draw_fg([ink](RectangleType r) {
              const float s = r.width / 36;
              afterhours::draw_line_ex({r.x + 3 * s, r.y + 15 * s},
                                       {r.x + 15 * s, r.y + 4 * s}, 5 * s, ink);
              afterhours::draw_ellipse(static_cast<int>(r.x + 21 * s),
                                       static_cast<int>(r.y + 9 * s), 9 * s,
                                       7 * s, ink);
              afterhours::draw_line_ex({r.x + 12 * s, r.y + 34 * s},
                                       {r.x + 15 * s, r.y + 22 * s}, 3 * s,
                                       ink);
              afterhours::draw_line_ex({r.x + 30 * s, r.y + 34 * s},
                                       {r.x + 27 * s, r.y + 22 * s}, 3 * s,
                                       ink);
              afterhours::draw_line_ex({r.x + 15 * s, r.y + 22 * s},
                                       {r.x + 27 * s, r.y + 22 * s}, 3 * s,
                                       ink);
              afterhours::draw_line_ex({r.x + 21 * s, r.y + 22 * s},
                                       {r.x + 21 * s, r.y + 34 * s}, 3 * s,
                                       ink);
            }));
    text(2, 90, 27, 450, 20, "T H E  A R T  O F  A L C H E M Y", 11, ink);
    text(3, 90, 45, 620, 45, "Alchemist's Workshop", 37, ink, "potion_title");
    div(context, mk(entity, 4),
        box(1098, 32, 146, 56)
            .with_custom_background(paper)
            .with_border(rule, 1));
    text(5, 1138, 41, 100, 38, "2,450", 28, ink, "potion_gold",
         TextAlignment::Center);
    div(context, mk(entity, 8),
        box(1121, 46, 27, 27)
            .with_ignore_pointer_events()
            .with_on_draw_fg([ink](RectangleType r) {
              const raylib::Vector2 c{r.x + r.width * .5f,
                                      r.y + r.height * .5f};
              raylib::DrawCircleLinesV(c, r.width * .45f, ink);
              raylib::DrawCircleV(c, r.width * .34f, ink);
            }));
    line(6, 36, 96, 1208, 1);
    line(7, 36, 152, 1208, 1);
    const std::array<std::string, 3> tabs{"Recipes", "Inventory", "Journal"};
    for (size_t i = 0; i < tabs.size(); ++i) {
      auto config = box(36 + 120.f * i, 99, 108, 49)
                        .with_label(tabs[i])
                        .with_font("Garamond", h720(25))
                        .with_custom_text_color(ink)
                        .with_alignment(TextAlignment::Center)
                        .with_debug_name("potion_tab_" + std::to_string(i));
      if (i == selected_tab)
        config.with_custom_background({210, 191, 144, 255});
      if (button(context, mk(entity, 10 + static_cast<int>(i)), config))
        selected_tab = i;
      if (i == selected_tab)
        line(20 + static_cast<int>(i), 36 + 120.f * i, 145, 108, 3);
    }
    text(30, 884, 106, 360, 36, "Wednesday, the 12th day of Autumn", 15, ink,
         "", TextAlignment::Right);
    line(31, 36, 655, 1208, 1);
    text(32, 36, 660, 150, 32, "Alchemy Level: 12", 13, muted);
    text(33, 195, 660, 130, 32, "4 recipes known", 13, muted);
    text(34, 339, 660, 220, 32,
         std::to_string(potions_brewed) + " potions brewed", 13, muted,
         "potion_total");
    text(35, 913, 660, 331, 32, "Tab: move   Enter: choose", 13, muted, "",
         TextAlignment::Right);

    if (selected_tab == 2) {
      text(40, 60, 182, 700, 48, "The alchemist's journal", 34, ink,
           "potion_journal");
      text(41, 60, 246, 1120, 44,
           last_brew.empty()
               ? "No potions bottled today. The workbench is ready."
               : last_brew,
           25, ink, "potion_journal_entry");
      text(42, 60, 308, 1120, 40,
           "A steady hand. A patient heart. One more drop.", 22, muted);
      return;
    }

    const bool inventory = selected_tab == 1;
    if (inventory) {
      text(40, 36, 175, 700, 45, "Ingredient inventory", 32, ink,
           "potion_inventory");
      text(41, 36, 225, 740, 32,
           "Select an ingredient to read your field notes.", 20, muted);
    } else {
      line(50, 321, 167, 1, 478);
      line(51, 342, 167, 1, 478);
      line(52, 869, 167, 1, 478);
      text(53, 36, 175, 276, 40, "Recipe book", 27, ink);
      text(54, 36, 215, 276, 26, "Notes from a practiced hand", 14, muted);
      for (size_t i = 0; i < recipes.size(); ++i) {
        float y = 250 + 83.f * i;
        auto config =
            box(36, y, 267, 82)
                .with_debug_name("potion_recipe_" + std::to_string(i));
        if (i == selected_recipe)
          config.with_custom_background({234, 219, 178, 255})
              .with_border(rule, 1);
        const int id = 100 + static_cast<int>(i) * 10;
        if (button(context, mk(entity, id), config)) {
          selected_recipe = i;
          brew_progress = 0;
          message.clear();
        }
        picture(id + 1, i, 43, y + 9, 52, 58);
        text(id + 2, 100, y + 7, 200, 27, recipes[i].name, 20, ink);
        text(id + 3, 100, y + 34, 200, 19, recipes[i].effect, 13, muted);
        text(id + 4, 100, y + 53, 200, 20,
             std::to_string(recipes[i].seconds) + "s to brew", 13, muted);
      }
      text(150, 72, 587, 210, 63,
           "A steady hand.\nA patient heart.\nOne more drop.", 17, muted, "",
           TextAlignment::Center);
      const auto &recipe = recipes[selected_recipe];
      text(200, 350, 175, 510, 45, recipe.name, 34, ink, "potion_recipe_title",
           TextAlignment::Center);
      text(201, 350, 215, 510, 28, recipe.description, 15, muted, "",
           TextAlignment::Center);
      div(context, mk(entity, 202),
          box(478, 236, 256, 256)
              .with_ignore_pointer_events()
              .with_on_draw_fg([](RectangleType r) {
                const raylib::Color faint{116, 101, 66, 35};
                const raylib::Vector2 c{r.x + r.width / 2, r.y + r.height / 2};
                raylib::DrawCircleLinesV(c, r.width * .49f, faint);
                raylib::DrawCircleLinesV(c, r.width * .43f, faint);
                raylib::DrawPolyLines(c, 3, r.width * .44f, -90, faint);
                raylib::DrawPolyLines(c, 3, r.width * .44f, 90, faint);
                raylib::DrawCircleLinesV(c, r.width * .245f, faint);
              }));
      picture(203, selected_recipe, 502, 254, 208, 208);
      div(context, mk(entity, 204),
          box(416, 456, 380, 17).with_on_draw_bg([](RectangleType r) {
            afterhours::draw_rectangle_gradient_v(r, {175, 151, 107, 255},
                                                  {102, 84, 58, 255});
            for (int i = 0; i < 3; ++i)
              afterhours::draw_rectangle(
                  {r.x, r.y + r.height * i / 3, r.width, r.height / 6},
                  {155, 128, 88, 255});
          }));
      div(context, mk(entity, 205),
          box(465, 485, 282, 8)
              .with_custom_background({187, 170, 128, 255})
              .with_border(rule, 1));
      div(context, mk(entity, 206),
          box(465, 485, 282 * brew_progress, 8)
              .with_custom_background({118, 129, 77, 255}));
      text(207, 420, 496, 370, 25,
           brew_progress == 1
               ? "Brew complete"
               : std::to_string(static_cast<int>(brew_progress * 100)) +
                     "% brewed",
           13, ink, "potion_progress", TextAlignment::Center);
      for (size_t i = 0; i < recipe.required.size(); ++i) {
        auto [index, count] = recipe.required[i];
        float x = 471 + 172.f * i;
        picture(210 + static_cast<int>(i) * 4, index + 4, x, 544, 34, 40);
        text(211 + static_cast<int>(i) * 4, x + 36, 542, 123, 23,
             std::string(ingredients[index].name) + " x " +
                 std::to_string(count),
             13, ink);
        text(212 + static_cast<int>(i) * 4, x + 36, 565, 123, 20,
             ingredients[index].count >= count ? "Available" : "Not enough", 12,
             ingredients[index].count >= count
                 ? afterhours::Color{106, 121, 70, 255}
                 : afterhours::Color{158, 56, 36, 255},
             "potion_required_" + std::to_string(i));
      }
      text(220, 596, 544, 30, 40, "+", 25, ink);
      const bool available = can_brew(recipe);
      if (button(
              context, mk(entity, 230),
              box(496, 605, 220, 54)
                  .with_label(available ? "Brew potion" : "Missing ingredients")
                  .with_font("Garamond", h720(available ? 30.f : 23.f))
                  .with_custom_background(olive)
                  .with_border(ink, 2)
                  .with_custom_text_color(paper)
                  .with_alignment(TextAlignment::Center)
                  .with_debug_name("potion_brew"))) {
        if (available) {
          for (auto [index, count] : recipe.required)
            ingredients[index].count -= count;
          ++potions_brewed;
          brew_progress = 1;
          last_brew =
              std::string(recipe.name) + " brewed. " + recipe.effect + ".";
          message = last_brew;
        } else
          message = "Gather the missing ingredients before brewing.";
      }
      div(context, mk(entity, 231),
          box(500, 609, 212, 46)
              .with_border({160, 155, 104, 255}, 1)
              .with_ignore_pointer_events());
      text(232, 377, 675, 457, 18,
           std::to_string(recipe.seconds) + " seconds / Produces 1 potion", 12,
           muted, "", TextAlignment::Center);
      if (!message.empty())
        text(233, 360, 523, 492, 20, message, 13, ink, "potion_message",
             TextAlignment::Center);
    }
    const float gx = inventory ? 36.f : 898.f;
    const float gy = inventory ? 278.f : 250.f;
    if (!inventory) {
      text(300, 898, 175, 340, 40, "Ingredients", 27, ink);
      text(301, 898, 215, 340, 26, "Gathered from near and far", 14, muted);
    }
    for (size_t i = 0; i < ingredients.size(); ++i) {
      const float x = gx + static_cast<float>(inventory ? i : i % 3) * 116.f;
      const float y =
          gy + (inventory ? 0.f : static_cast<float>(i / 3) * 143.f);
      const int id = 310 + static_cast<int>(i) * 10;
      if (button(
              context, mk(entity, id),
              box(x, y, 108, 135)
                  .with_custom_background(
                      selected_ingredient == static_cast<int>(i)
                          ? afterhours::Color{249, 236, 199, 255}
                          : paper)
                  .with_border(
                      selected_ingredient == static_cast<int>(i) ? ink : rule,
                      1)
                  .with_debug_name("potion_ingredient_" + std::to_string(i))))
        selected_ingredient = static_cast<int>(i);
      picture(id + 1, i + 4, x + 15, y + 17, 77, 85);
      text(id + 2, x + 3, y + 99, 102, 30, ingredients[i].name, 14, ink, "",
           TextAlignment::Center);
      text(id + 3, x + 65, y + 3, 36, 22,
           "x " + std::to_string(ingredients[i].count), 13, ink,
           "potion_stock_" + std::to_string(i), TextAlignment::Right);
    }
    const float note_y = inventory ? 438.f : 553.f;
    line(380, gx, note_y, inventory ? 1120.f : 340.f, 1);
    text(381, gx, note_y + 10, inventory ? 1120.f : 340.f, 40,
         "Alchemist's note", 21, ink, "", TextAlignment::Center);
    const auto &note =
        ingredients[selected_ingredient < 0
                        ? 2
                        : static_cast<size_t>(selected_ingredient)];
    text(382, gx + 8, note_y + 54, inventory ? 1104.f : 324.f, 44, note.note,
         14, muted, "potion_field_note", TextAlignment::Center);
  }
};

REGISTER_EXAMPLE_SCREEN(
    potion_crafting, "Game Mockups",
    "Alchemy crafting workshop with recipes and ingredients",
    PotionCraftingScreen)
