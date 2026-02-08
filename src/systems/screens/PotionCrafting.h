#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../../ui_workarounds/NotificationBadge.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct PotionCraftingScreen : ScreenSystem<UIContext<InputAction>> {
  size_t selected_recipe = 1;
  float brew_progress = 0.65f;
  size_t selected_tab = 0;

  struct Ingredient {
    std::string name;
    std::string icon; // fallback text
    int count;
    afterhours::Color color;
  };

  std::vector<Ingredient> ingredients = {
      {"Moonpetal", "M", 5, {180, 140, 220, 255}},
      {"Fire Root", "F", 3, {220, 100, 60, 255}},
      {"Sea Salt", "S", 12, {140, 200, 220, 255}},
      {"Honey Dew", "H", 8, {240, 200, 80, 255}},
      {"Shadow Moss", "X", 2, {80, 100, 80, 255}},
      {"Star Dust", "*", 1, {220, 220, 180, 255}},
  };

  struct Recipe {
    std::string name;
    std::string effect;
    std::vector<std::pair<std::string, int>> required; // ingredient name, count
    int brew_time; // seconds
    afterhours::Color potion_color;
  };

  std::vector<Recipe> recipes = {
      {"Healing Potion",
       "Restores 50 HP",
       {{"Moonpetal", 2}, {"Honey Dew", 1}},
       30,
       {220, 60, 80, 255}},
      {"Mana Elixir",
       "Restores 30 MP",
       {{"Sea Salt", 3}, {"Star Dust", 1}},
       45,
       {80, 120, 220, 255}},
      {"Fire Bomb",
       "Deals 25 fire damage",
       {{"Fire Root", 2}, {"Shadow Moss", 1}},
       60,
       {240, 140, 40, 255}},
      {"Antidote",
       "Cures poison",
       {{"Moonpetal", 1}, {"Sea Salt", 2}},
       20,
       {120, 200, 120, 255}},
  };

  std::vector<std::string> tabs = {"Recipes", "Inventory", "Journal"};

  // Alchemical/fantasy colors
  afterhours::Color bg_dark{25, 22, 30, 255};
  afterhours::Color panel_purple{38, 32, 48, 255};
  afterhours::Color panel_light{55, 48, 68, 255};
  afterhours::Color border_purple{80, 65, 100, 255};
  afterhours::Color accent_gold{220, 185, 80, 255};
  afterhours::Color accent_green{90, 190, 110, 255};
  afterhours::Color accent_blue{80, 140, 220, 255};
  afterhours::Color white{245, 240, 235, 255};
  afterhours::Color muted{140, 130, 155, 255};
  afterhours::Color dark_text{25, 22, 30, 255};
  afterhours::Color brew_bg{30, 25, 35, 255};
  afterhours::Color slot_empty{45, 38, 55, 255};
  afterhours::Color slot_filled{65, 55, 80, 255};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.font = white;
    theme.darkfont = dark_text;
    theme.font_muted = muted;
    theme.background = bg_dark;
    theme.surface = panel_purple;
    theme.primary = border_purple;
    theme.secondary = panel_light;
    theme.accent = accent_gold;
    theme.error = afterhours::Color{180, 60, 60, 255};
    theme.roundness = 0.1f;
    theme.segments = 8;
    context.theme = theme;

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();
    auto pxf = [](float v) { return pixels(static_cast<int>(v)); };

    // ========== BACKGROUND ==========
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(bg_dark)
            .with_debug_name("bg"));

    // ========== HEADER ==========
    div(context, mk(entity, 5),
        ComponentConfig{}
            .with_label("Alchemist's Workshop")
            .with_size(ComponentSize{pixels(380), pixels(45)})
            .with_absolute_position(25.0f, 12.0f)
            .with_font("Gaegu-Bold", h720(34.0f))
            .with_custom_text_color(accent_gold)
            .with_debug_name("title"));

    // Gold display
    div(context, mk(entity, 6),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(150), pixels(36)})
            .with_absolute_position((float)screen_w - 180.0f, 14.0f)
            .with_custom_background(panel_purple)
            .with_border(border_purple, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)
            .with_debug_name("gold_bg"));

    div(context, mk(entity, 7),
        ComponentConfig{}
            .with_label("Gold: 2,450")
            .with_size(ComponentSize{pixels(140), pixels(28)})
            .with_absolute_position((float)screen_w - 175.0f, 18.0f)
            .with_font("EqProRounded", h720(18.0f))
            .with_custom_text_color(accent_gold)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("gold_text"));

    // ========== TAB BAR ==========
    float tab_y = 58.0f;
    for (size_t i = 0; i < tabs.size(); i++) {
      bool is_sel = (i == selected_tab);
      afterhours::Color tab_bg =
          is_sel ? accent_gold : panel_purple;
      afterhours::Color tab_text = is_sel ? dark_text : muted;

      if (button(context, mk(entity, 10 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(tabs[i])
                     .with_size(ComponentSize{pixels(130), pixels(34)})
                     .with_absolute_position(25.0f + (float)i * 140.0f, tab_y)
                     .with_custom_background(tab_bg)
                     .with_border(border_purple, 2.0f)
                     .with_font("EqProRounded", h720(18.0f))
                     .with_custom_text_color(tab_text)
                     .with_alignment(TextAlignment::Center)
                     .with_rounded_corners(std::bitset<4>(0b1100))
                     .with_roundness(0.2f)
                     .with_debug_name("tab_" + std::to_string(i)))) {
        selected_tab = i;
      }
    }

    // ========== LEFT: RECIPE LIST ==========
    float list_x = 25.0f;
    float list_y = 100.0f;
    float list_w = 300.0f;
    float list_h = (float)screen_h - list_y - 56.0f;

    div(context, mk(entity, 100),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(list_w),
                                     pxf(list_h)})
            .with_absolute_position(list_x, list_y)
            .with_custom_background(panel_purple)
            .with_border(border_purple, 2.0f)
            .with_rounded_corners(std::bitset<4>(0b0011))
            .with_roundness(0.06f)
            .with_debug_name("recipe_panel"));

    // Recipe rows
    for (size_t i = 0; i < recipes.size(); i++) {
      float ry = list_y + 10.0f + (float)i * 80.0f;
      bool is_sel = (i == selected_recipe);

      afterhours::Color row_bg =
          is_sel ? panel_light : afterhours::Color{0, 0, 0, 0};
      afterhours::Color row_border =
          is_sel ? accent_gold : afterhours::Color{0, 0, 0, 0};

      if (button(
              context, mk(entity, 110 + static_cast<int>(i) * 4),
              ComponentConfig{}
                  .with_size(ComponentSize{
                      pxf(list_w - 20), pixels(72)})
                  .with_absolute_position(list_x + 10.0f, ry)
                  .with_custom_background(row_bg)
                  .with_border(row_border, is_sel ? 2.0f : 0.0f)
                  .with_rounded_corners(RoundedCorners())
                  .with_roundness(0.1f)
                  .with_debug_name("recipe_row_" + std::to_string(i)))) {
        selected_recipe = i;
      }

      // Potion color indicator
      div(context, mk(entity, 111 + static_cast<int>(i) * 4),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(40), pixels(40)})
              .with_absolute_position(list_x + 20.0f, ry + 8.0f)
              .with_custom_background(recipes[i].potion_color)
              .with_border(border_purple, 2.0f)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(1.0f)
              .with_debug_name("potion_dot_" + std::to_string(i)));

      // Recipe name
      div(context, mk(entity, 112 + static_cast<int>(i) * 4),
          ComponentConfig{}
              .with_label(recipes[i].name)
              .with_size(ComponentSize{pixels(200), pixels(28)})
              .with_absolute_position(list_x + 70.0f, ry + 8.0f)
              .with_font("Gaegu-Bold", h720(22.0f))
              .with_custom_text_color(is_sel ? accent_gold : white)
              .with_debug_name("recipe_name_" + std::to_string(i)));

      // Effect text
      div(context, mk(entity, 113 + static_cast<int>(i) * 4),
          ComponentConfig{}
              .with_label(recipes[i].effect)
              .with_size(ComponentSize{pixels(200), pixels(22)})
              .with_absolute_position(list_x + 70.0f, ry + 32.0f)
              .with_font("EqProRounded", h720(14.0f))
              .with_custom_text_color(muted)
              .with_debug_name("recipe_effect_" + std::to_string(i)));

      // Brew time
      div(context, mk(entity, 114 + static_cast<int>(i) * 4),
          ComponentConfig{}
              .with_label(std::to_string(recipes[i].brew_time) + "s")
              .with_size(ComponentSize{pixels(50), pixels(22)})
              .with_absolute_position(list_x + 70.0f, ry + 50.0f)
              .with_font("EqProRounded", h720(14.0f))
              .with_custom_text_color(muted)
              .with_debug_name("recipe_time_" + std::to_string(i)));
    }

    // ========== CENTER: BREWING STATION ==========
    auto &sel_recipe = recipes[selected_recipe];
    float brew_x = list_x + list_w + 16.0f;
    float brew_y = list_y;
    float brew_w = 380.0f;
    float brew_h = list_h;

    div(context, mk(entity, 200),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(brew_w),
                                     pxf(brew_h)})
            .with_absolute_position(brew_x, brew_y)
            .with_custom_background(panel_purple)
            .with_border(border_purple, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.06f)
            .with_soft_shadow(3.0f, 4.0f, 12.0f,
                              afterhours::Color{0, 0, 0, 60})
            .with_debug_name("brew_panel"));

    // Brewing header
    div(context, mk(entity, 201),
        ComponentConfig{}
            .with_label("Brewing: " + sel_recipe.name)
            .with_size(ComponentSize{pxf(brew_w - 30),
                                     pixels(30)})
            .with_absolute_position(brew_x + 15.0f, brew_y + 12.0f)
            .with_font("Gaegu-Bold", h720(24.0f))
            .with_custom_text_color(accent_gold)
            .with_debug_name("brew_header"));

    // Large potion flask visualization
    float flask_size = 120.0f;
    float flask_x = brew_x + (brew_w - flask_size) / 2.0f;
    float flask_y = brew_y + 55.0f;

    // Flask outline
    div(context, mk(entity, 210),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(flask_size),
                                     pxf(flask_size)})
            .with_absolute_position(flask_x, flask_y)
            .with_custom_background(brew_bg)
            .with_border(border_purple, 3.0f)
            .with_rounded_corners(std::bitset<4>(0b0011))
            .with_roundness(0.4f)
            .with_debug_name("flask_bg"));

    // Flask fill (based on brew progress)
    float fill_h = flask_size * brew_progress;
    div(context, mk(entity, 211),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(flask_size - 10),
                                     pxf(fill_h)})
            .with_absolute_position(flask_x + 5.0f,
                            flask_y + flask_size - fill_h - 3.0f)
            .with_custom_background(sel_recipe.potion_color)
            .with_rounded_corners(std::bitset<4>(0b0011))
            .with_roundness(0.3f)
            .with_debug_name("flask_fill"));

    // Flask neck
    div(context, mk(entity, 212),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(40), pixels(25)})
            .with_absolute_position(flask_x + (flask_size - 40.0f) / 2.0f,
                            flask_y - 22.0f)
            .with_custom_background(brew_bg)
            .with_border(border_purple, 2.0f)
            .with_debug_name("flask_neck"));

    // Brew progress bar
    float prog_y = flask_y + flask_size + 15.0f;
    div(context, mk(entity, 220),
        ComponentConfig{}
            .with_size(ComponentSize{
                pxf(brew_w - 60), pixels(24)})
            .with_absolute_position(brew_x + 30.0f, prog_y)
            .with_custom_background(brew_bg)
            .with_border(border_purple, 1.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)
            .with_debug_name("brew_bar_bg"));

    div(context, mk(entity, 221),
        ComponentConfig{}
            .with_size(ComponentSize{
                pixels(static_cast<int>((brew_w - 66) * brew_progress)),
                pixels(20)})
            .with_absolute_position(brew_x + 33.0f, prog_y + 2.0f)
            .with_custom_background(accent_green)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.5f)
            .with_debug_name("brew_bar_fill"));

    // Progress percentage
    int prog_pct = static_cast<int>(brew_progress * 100);
    div(context, mk(entity, 222),
        ComponentConfig{}
            .with_label(std::to_string(prog_pct) + "% Brewed")
            .with_size(ComponentSize{pxf(brew_w - 60),
                                     pixels(22)})
            .with_absolute_position(brew_x + 30.0f, prog_y + 28.0f)
            .with_font("EqProRounded", h720(15.0f))
            .with_custom_text_color(muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("brew_pct"));

    // ========== INGREDIENT SLOTS ==========
    float slots_y = prog_y + 60.0f;
    div(context, mk(entity, 230),
        ComponentConfig{}
            .with_label("Required Ingredients:")
            .with_size(ComponentSize{pxf(brew_w - 60),
                                     pixels(22)})
            .with_absolute_position(brew_x + 30.0f, slots_y)
            .with_font("EqProRounded", h720(16.0f))
            .with_custom_text_color(white)
            .with_debug_name("req_label"));

    float slot_size = 60.0f;
    float slot_gap = 15.0f;
    float slots_total_w =
        (float)sel_recipe.required.size() * (slot_size + slot_gap) - slot_gap;
    float slots_start_x = brew_x + (brew_w - slots_total_w) / 2.0f;

    for (size_t i = 0; i < sel_recipe.required.size(); i++) {
      auto &req = sel_recipe.required[i];
      float sx = slots_start_x + (float)i * (slot_size + slot_gap);
      float sy = slots_y + 40.0f;

      // Find ingredient to get its color
      afterhours::Color ing_color = muted;
      std::string ing_icon = "?";
      int available = 0;
      for (auto &ing : ingredients) {
        if (ing.name == req.first) {
          ing_color = ing.color;
          ing_icon = ing.icon;
          available = ing.count;
          break;
        }
      }

      bool has_enough = available >= req.second;

      // Slot background
      div(context, mk(entity, 240 + static_cast<int>(i) * 3),
          ComponentConfig{}
              .with_size(ComponentSize{pxf(slot_size),
                                       pxf(slot_size)})
              .with_absolute_position(sx, sy)
              .with_custom_background(slot_filled)
              .with_border(has_enough ? accent_green
                                      : afterhours::Color{180, 60, 60, 255},
                           2.0f)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.15f)
              .with_debug_name("slot_" + std::to_string(i)));

      // Ingredient icon
      div(context, mk(entity, 241 + static_cast<int>(i) * 3),
          ComponentConfig{}
              .with_label(ing_icon)
              .with_size(ComponentSize{pixels(36), pixels(36)})
              .with_absolute_position(sx + 12.0f, sy + 5.0f)
              .with_custom_background(ing_color)
              .with_font("EqProRounded", h720(20.0f))
              .with_custom_text_color(dark_text)
              .with_alignment(TextAlignment::Center)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(1.0f)
              .with_debug_name("slot_icon_" + std::to_string(i)));

      // Count label
      div(context, mk(entity, 242 + static_cast<int>(i) * 3),
          ComponentConfig{}
              .with_label(std::string(has_enough ? "v " : "x ") + "x" + std::to_string(req.second))
              .with_size(ComponentSize{pxf(slot_size),
                                       pixels(18)})
              .with_absolute_position(sx, sy + slot_size - 20.0f)
              .with_font("EqProRounded", h720(14.0f))
              .with_custom_text_color(has_enough ? accent_green : afterhours::Color{220, 80, 80, 255})
              .with_alignment(TextAlignment::Center)
              .with_debug_name("slot_count_" + std::to_string(i)));
    }

    // Brew button
    float brew_btn_y = slots_y + slot_size + 60.0f;
    button(context, mk(entity, 250),
           ComponentConfig{}
               .with_label("Brew!")
               .with_size(ComponentSize{pixels(160), pixels(48)})
               .with_absolute_position(brew_x + (brew_w - 160.0f) / 2.0f, brew_btn_y)
               .with_custom_background(accent_gold)
               .with_border(afterhours::Color{180, 150, 50, 255}, 3.0f)
               .with_font("Gaegu-Bold", h720(26.0f))
               .with_custom_text_color(dark_text)
               .with_alignment(TextAlignment::Center)
               .with_rounded_corners(RoundedCorners())
               .with_roundness(0.4f)
               .with_soft_shadow(2.0f, 3.0f, 10.0f,
                                 afterhours::Color{0, 0, 0, 60})
               .with_debug_name("brew_btn"));

    // ========== RIGHT: INGREDIENT INVENTORY ==========
    float inv_x = brew_x + brew_w + 16.0f;
    float inv_y = list_y;
    float inv_w = (float)screen_w - inv_x - 25.0f;
    float inv_h = list_h;

    div(context, mk(entity, 300),
        ComponentConfig{}
            .with_size(ComponentSize{pxf(inv_w),
                                     pxf(inv_h)})
            .with_absolute_position(inv_x, inv_y)
            .with_custom_background(panel_purple)
            .with_border(border_purple, 2.0f)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(0.06f)
            .with_debug_name("inv_panel"));

    // Inventory header
    div(context, mk(entity, 301),
        ComponentConfig{}
            .with_label("Ingredients")
            .with_size(ComponentSize{pxf(inv_w - 20),
                                     pixels(28)})
            .with_absolute_position(inv_x + 10.0f, inv_y + 10.0f)
            .with_font("Gaegu-Bold", h720(22.0f))
            .with_custom_text_color(accent_gold)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("inv_header"));

    // Ingredient grid (2 columns)
    int inv_cols = 2;
    float inv_cell_w = (inv_w - 30.0f) / (float)inv_cols;
    float inv_cell_h = 60.0f;

    for (size_t i = 0; i < ingredients.size(); i++) {
      int row = static_cast<int>(i) / inv_cols;
      int col = static_cast<int>(i) % inv_cols;
      float ix = inv_x + 10.0f + (float)col * inv_cell_w;
      float iy = inv_y + 45.0f + (float)row * inv_cell_h;

      // Cell background
      div(context, mk(entity, 310 + static_cast<int>(i) * 3),
          ComponentConfig{}
              .with_size(
                  ComponentSize{pxf(inv_cell_w - 5),
                                pxf(inv_cell_h - 5)})
              .with_absolute_position(ix, iy)
              .with_custom_background(panel_light)
              .with_border(border_purple, 1.0f)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(0.1f)
              .with_debug_name("inv_cell_" + std::to_string(i)));

      // Icon
      div(context, mk(entity, 311 + static_cast<int>(i) * 3),
          ComponentConfig{}
              .with_label(ingredients[i].icon)
              .with_size(ComponentSize{pixels(32), pixels(32)})
              .with_absolute_position(ix + 8.0f, iy + 8.0f)
              .with_custom_background(ingredients[i].color)
              .with_font("EqProRounded", h720(18.0f))
              .with_custom_text_color(dark_text)
              .with_alignment(TextAlignment::Center)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(1.0f)
              .with_debug_name("inv_icon_" + std::to_string(i)));

      // Name and count
      div(context, mk(entity, 312 + static_cast<int>(i) * 3),
          ComponentConfig{}
              .with_label(ingredients[i].name + "  x" +
                          std::to_string(ingredients[i].count))
              .with_size(ComponentSize{pxf(inv_cell_w - 55),
                                       pixels(40)})
              .with_absolute_position(ix + 46.0f, iy + 8.0f)
              .with_font("EqProRounded", h720(14.0f))
              .with_custom_text_color(white)
              .with_debug_name("inv_name_" + std::to_string(i)));
    }

    // ========== BOTTOM STATUS ==========
    float status_y = (float)screen_h - 40.0f;
    div(context, mk(entity, 400),
        ComponentConfig{}
            .with_label("Alchemy Level: 12  |  Recipes Known: " +
                        std::to_string(recipes.size()) +
                        "  |  Potions Brewed: 47")
            .with_size(ComponentSize{pixels(screen_w - 50), pixels(22)})
            .with_absolute_position(25.0f, status_y)
            .with_font("EqProRounded", h720(14.0f))
            .with_custom_text_color(muted)
            .with_alignment(TextAlignment::Center)
            .with_debug_name("status"));
  }
};

REGISTER_EXAMPLE_SCREEN(potion_crafting, "Game Mockups",
                        "Fantasy alchemy crafting interface",
                        PotionCraftingScreen)
