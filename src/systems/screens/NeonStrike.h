#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct NeonStrikeScreen : ScreenSystem<UIContext<InputAction>> {
  struct Weapon {
    std::string name;
    int damage;
    int accuracy;
    int fire_rate;
  };

  std::vector<Weapon> weapons = {
      {"VIPER AR-7", 45, 78, 65},
      {"NOVA REVOLVER", 85, 92, 25},
      {"PHANTOM SMG", 32, 60, 95},
  };

  size_t selected_weapon = 0;
  size_t selected_secondary = 0;
  size_t selected_perk = 0;

  std::vector<std::string> perks = {"QUICK HANDS", "IRON SKIN", "GHOST STEP"};
  std::vector<std::string> gear = {"SMOKE", "EMP", "DRONE", "MEDKIT"};
  std::vector<std::string> secondary_weapons = {"PISTOL", "KNIFE", "LAUNCHER"};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.font = afterhours::Color{240, 240, 235, 255};
    theme.darkfont = afterhours::Color{15, 15, 12, 255};
    theme.font_muted = afterhours::Color{140, 140, 130, 255};
    theme.background = afterhours::Color{12, 14, 18, 255};
    theme.surface = afterhours::Color{24, 28, 36, 255};
    theme.primary = afterhours::Color{38, 44, 56, 255};
    theme.secondary = afterhours::Color{52, 60, 72, 255};
    theme.accent = afterhours::Color{175, 105, 20, 255};  // Dark amber (contrast 4.5+ with dark text)
    theme.error = afterhours::Color{170, 45, 45, 255};
    context.theme = theme;

    // Layout constants - all elements contained in card
    constexpr float CARD_WIDTH = 1000.0f;
    constexpr float HEADER_H = 50.0f;
    constexpr float CONTENT_H = 400.0f;
    constexpr float FOOTER_H = 50.0f;
    constexpr float INNER_PAD = 20.0f;
    constexpr float SECTION_GAP = 12.0f;
    constexpr float CARD_HEIGHT = HEADER_H + CONTENT_H + FOOTER_H + INNER_PAD * 2 + SECTION_GAP * 2;
    constexpr float LEFT_COL = 560.0f;
    constexpr float RIGHT_COL = 380.0f;
    constexpr float COL_GAP = 20.0f;

    // Full screen background
    auto main = div(context, mk(entity, 0),
                    ComponentConfig{}
                        .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
                        .with_custom_background(theme.background)
                        .with_flex_direction(FlexDirection::Column)
                        .with_padding(Spacing::lg)
                        .with_debug_name("main"));

    // Single unified card container
    auto card = div(context, mk(main.ent(), 0),
                    ComponentConfig{}
                        .with_size(ComponentSize{pixels(CARD_WIDTH), pixels(CARD_HEIGHT)})
                        .with_custom_background(theme.surface)
                        .with_padding(Padding{.top = pixels(INNER_PAD), .right = pixels(INNER_PAD),
                                               .bottom = pixels(INNER_PAD), .left = pixels(INNER_PAD)})
                        .with_flex_direction(FlexDirection::Column)
                        .with_debug_name("card"));

    // ========== HEADER ROW ==========
    auto header = div(context, mk(card.ent(), 0),
                      ComponentConfig{}
                          .with_size(ComponentSize{pixels(CARD_WIDTH - INNER_PAD * 2), pixels(50)})
                          .with_custom_background(theme.primary)
                          .with_padding(Spacing::sm)
                          .with_flex_direction(FlexDirection::Row)
                          .with_debug_name("header"));

    div(context, mk(header.ent(), 0),
        ComponentConfig{}
            .with_label("OPERATION: BLACKOUT")
            .with_size(ComponentSize{pixels(280), pixels(38)})
            .with_background(Theme::Usage::Accent)
            .with_text_color(Theme::Usage::DarkFont)
            .with_font(UIComponent::DEFAULT_FONT, 22.0f)
            .with_padding(Spacing::sm)
            .with_debug_name("mission_name"));

    div(context, mk(header.ent(), 1),
        ComponentConfig{}
            .with_label("EXTRACTION")
            .with_size(ComponentSize{pixels(130), pixels(38)})
            .with_custom_background(afterhours::Color{140, 40, 40, 255})
            .with_custom_text_color(theme.font)
            .with_font(UIComponent::DEFAULT_FONT, 19.0f)
            .with_margin(Margin{.left = pixels(12)})
            .with_padding(Spacing::sm)
            .with_debug_name("mission_type"));

    div(context, mk(header.ent(), 2),
        ComponentConfig{}
            .with_label("SQUAD: 4/4")
            .with_size(ComponentSize{pixels(120), pixels(38)})
            .with_background(Theme::Usage::Secondary)
            .with_custom_text_color(theme.font)
            .with_font(UIComponent::DEFAULT_FONT, 19.0f)
            .with_margin(Margin{.left = pixels(12)})
            .with_padding(Spacing::sm)
            .with_debug_name("squad"));

    // ========== CONTENT ROW (Two columns) ==========
    auto content = div(context, mk(card.ent(), 1),
                       ComponentConfig{}
                           .with_size(ComponentSize{pixels(CARD_WIDTH - INNER_PAD * 2), pixels(CONTENT_H)})
                           .with_flex_direction(FlexDirection::Row)
                           .with_margin(Margin{.top = pixels(SECTION_GAP)})
                           .with_debug_name("content"));

    // LEFT COLUMN - Weapons
    auto left_col = div(context, mk(content.ent(), 0),
                        ComponentConfig{}
                            .with_size(ComponentSize{pixels(LEFT_COL), pixels(CONTENT_H)})
                            .with_custom_background(theme.primary)
                            .with_padding(Spacing::md)
                            .with_flex_direction(FlexDirection::Column)
                            .with_debug_name("left_col"));

    constexpr float LEFT_INNER = LEFT_COL - 32.0f;

    div(context, mk(left_col.ent(), 0),
        ComponentConfig{}
            .with_label("PRIMARY WEAPON")
            .with_size(ComponentSize{pixels(LEFT_INNER), pixels(32)})
            .with_background(Theme::Usage::Secondary)
            .with_custom_text_color(theme.accent)
            .with_font(UIComponent::DEFAULT_FONT, 19.0f)
            .with_padding(Spacing::sm)
            .with_skip_tabbing(true)
            .with_debug_name("weapon_lbl"));

    // Weapon buttons
    for (size_t i = 0; i < weapons.size(); i++) {
      bool sel = (i == selected_weapon);
      if (button(context, mk(left_col.ent(), 1 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(weapons[i].name)
                     .with_size(ComponentSize{pixels(LEFT_INNER), pixels(40)})
                     .with_custom_background(sel ? theme.accent : theme.secondary)
                     .with_custom_text_color(sel ? theme.darkfont : theme.font)
                     .with_font(UIComponent::DEFAULT_FONT, 19.0f)
                     .with_margin(Margin{.top = pixels(8)})
                     .with_debug_name("weapon_" + std::to_string(i)))) {
        selected_weapon = i;
      }
    }

    // Stats section
    auto &w = weapons[selected_weapon];
    auto stats = div(context, mk(left_col.ent(), 10),
                     ComponentConfig{}
                         .with_size(ComponentSize{pixels(LEFT_INNER), pixels(100)})
                         .with_custom_background(theme.background)
                         .with_padding(Spacing::md)
                         .with_flex_direction(FlexDirection::Column)
                         .with_margin(Margin{.top = pixels(16)})
                         .with_debug_name("stats"));

    constexpr float STAT_BAR_W = 320.0f;

    auto draw_stat = [&](int idx, const std::string &name, int value) {
      auto row = div(context, mk(stats.ent(), idx),
                     ComponentConfig{}
                         .with_size(ComponentSize{pixels(LEFT_INNER - 32), pixels(24)})
                         .with_flex_direction(FlexDirection::Row)
                         .with_margin(idx > 0 ? Margin{.top = pixels(6)} : Margin{})
                         .with_debug_name(name + "_row"));

      // Label
      div(context, mk(row.ent(), 0),
          ComponentConfig{}
              .with_label(name)
              .with_size(ComponentSize{pixels(48), pixels(20)})
              .with_custom_text_color(theme.font_muted)
              .with_font(UIComponent::DEFAULT_FONT, 19.0f)
              .with_debug_name(name + "_lbl"));

      // Bar background
      auto bar_bg = div(context, mk(row.ent(), 1),
                        ComponentConfig{}
                            .with_size(ComponentSize{pixels(STAT_BAR_W), pixels(14)})
                            .with_custom_background(theme.primary)
                            .with_margin(Margin{.left = pixels(8), .top = pixels(3)})
                            .with_debug_name(name + "_bg"));

      // Bar fill (contained inside bar_bg)
      float fill_pct = static_cast<float>(value) / 100.0f;
      float fill_w = STAT_BAR_W * fill_pct;
      div(context, mk(bar_bg.ent(), 0),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(fill_w), pixels(14)})
              .with_background(Theme::Usage::Accent)
              .with_debug_name(name + "_fill"));

      // Value text
      div(context, mk(row.ent(), 2),
          ComponentConfig{}
              .with_label(std::to_string(value))
              .with_size(ComponentSize{pixels(40), pixels(20)})
              .with_custom_text_color(theme.font)
              .with_font(UIComponent::DEFAULT_FONT, 19.0f)
              .with_margin(Margin{.left = pixels(12)})
              .with_debug_name(name + "_val"));
    };

    draw_stat(0, "DMG", w.damage);
    draw_stat(1, "ACC", w.accuracy);
    draw_stat(2, "ROF", w.fire_rate);

    // RIGHT COLUMN - Gear & Perks
    auto right_col = div(context, mk(content.ent(), 1),
                         ComponentConfig{}
                             .with_size(ComponentSize{pixels(RIGHT_COL), pixels(CONTENT_H)})
                             .with_custom_background(theme.primary)
                             .with_padding(Spacing::md)
                             .with_flex_direction(FlexDirection::Column)
                             .with_margin(Margin{.left = pixels(COL_GAP)})
                             .with_debug_name("right_col"));

    constexpr float RIGHT_INNER = RIGHT_COL - 32.0f;

    // SECONDARY weapon
    div(context, mk(right_col.ent(), 0),
        ComponentConfig{}
            .with_label("SECONDARY")
            .with_size(ComponentSize{pixels(RIGHT_INNER), pixels(28)})
            .with_background(Theme::Usage::Secondary)
            .with_custom_text_color(theme.accent)
            .with_font(UIComponent::DEFAULT_FONT, 19.0f)
            .with_padding(Spacing::sm)
            .with_skip_tabbing(true)
            .with_debug_name("secondary_lbl"));

    navigation_bar(context, mk(right_col.ent(), 1), secondary_weapons,
                   selected_secondary,
                   ComponentConfig{}
                       .with_size(ComponentSize{pixels(RIGHT_INNER), pixels(36)})
                       .with_background(Theme::Usage::Secondary)
                       .with_text_color(Theme::Usage::Font)
                       .with_font(UIComponent::DEFAULT_FONT, 19.0f)
                       .with_margin(Margin{.top = pixels(8)})
                       .with_debug_name("secondary_nav"));

    // TACTICAL GEAR - visible header
    div(context, mk(right_col.ent(), 5),
        ComponentConfig{}
            .with_label("TACTICAL GEAR")
            .with_size(ComponentSize{pixels(RIGHT_INNER), pixels(32)})
            .with_background(Theme::Usage::Accent)
            .with_text_color(Theme::Usage::DarkFont)
            .with_font(UIComponent::DEFAULT_FONT, 19.0f)
            .with_margin(Margin{.top = pixels(12)})
            .with_skip_tabbing(true)
            .with_debug_name("gear_lbl"));

    constexpr float GEAR_BTN_W = (RIGHT_INNER - 12.0f) / 2.0f;

    auto gr1 = div(context, mk(right_col.ent(), 6),
                   ComponentConfig{}
                       .with_size(ComponentSize{pixels(RIGHT_INNER), pixels(36)})
                       .with_flex_direction(FlexDirection::Row)
                       .with_margin(Margin{.top = pixels(8)})
                       .with_debug_name("gr1"));

    for (size_t i = 0; i < 2; i++) {
      button(context, mk(gr1.ent(), static_cast<int>(i)),
             ComponentConfig{}
                 .with_label(gear[i])
                 .with_size(ComponentSize{pixels(GEAR_BTN_W), pixels(32)})
                 .with_background(Theme::Usage::Secondary)
                 .with_custom_text_color(theme.font)
                 .with_font(UIComponent::DEFAULT_FONT, 19.0f)
                 .with_margin(i == 0 ? Margin{} : Margin{.left = pixels(12)})
                 .with_debug_name("g_" + std::to_string(i)));
    }

    auto gr2 = div(context, mk(right_col.ent(), 7),
                   ComponentConfig{}
                       .with_size(ComponentSize{pixels(RIGHT_INNER), pixels(36)})
                       .with_flex_direction(FlexDirection::Row)
                       .with_margin(Margin{.top = pixels(8)})
                       .with_debug_name("gr2"));

    for (size_t i = 2; i < 4; i++) {
      button(context, mk(gr2.ent(), static_cast<int>(i)),
             ComponentConfig{}
                 .with_label(gear[i])
                 .with_size(ComponentSize{pixels(GEAR_BTN_W), pixels(32)})
                 .with_background(Theme::Usage::Secondary)
                 .with_custom_text_color(theme.font)
                 .with_font(UIComponent::DEFAULT_FONT, 19.0f)
                 .with_margin(i == 2 ? Margin{} : Margin{.left = pixels(12)})
                 .with_debug_name("g_" + std::to_string(i)));
    }

    // PERK - visible header
    div(context, mk(right_col.ent(), 10),
        ComponentConfig{}
            .with_label("PERK")
            .with_size(ComponentSize{pixels(RIGHT_INNER), pixels(32)})
            .with_background(Theme::Usage::Accent)
            .with_text_color(Theme::Usage::DarkFont)
            .with_font(UIComponent::DEFAULT_FONT, 19.0f)
            .with_margin(Margin{.top = pixels(12)})
            .with_skip_tabbing(true)
            .with_debug_name("perk_lbl"));

    dropdown(context, mk(right_col.ent(), 11), perks, selected_perk,
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(RIGHT_INNER), pixels(36)})
                 .with_background(Theme::Usage::Accent)
                 .with_text_color(Theme::Usage::DarkFont)
                 .with_font(UIComponent::DEFAULT_FONT, 19.0f)
                 .with_margin(Margin{.top = pixels(8)})
                 .with_debug_name("perk_dd"));

    // ========== FOOTER ROW ==========
    auto footer = div(context, mk(card.ent(), 2),
                      ComponentConfig{}
                          .with_size(ComponentSize{pixels(CARD_WIDTH - INNER_PAD * 2), pixels(FOOTER_H)})
                          .with_custom_background(theme.secondary)
                          .with_padding(Spacing::sm)
                          .with_flex_direction(FlexDirection::Row)
                          .with_margin(Margin{.top = pixels(SECTION_GAP)})
                          .with_debug_name("footer"));

    button(context, mk(footer.ent(), 0),
           ComponentConfig{}
               .with_label("BACK")
               .with_size(ComponentSize{pixels(100), pixels(38)})
               .with_background(Theme::Usage::Secondary)
               .with_custom_text_color(theme.font)
               .with_font(UIComponent::DEFAULT_FONT, 19.0f)
               .with_debug_name("back"));

    button(context, mk(footer.ent(), 1),
           ComponentConfig{}
               .with_label("SAVE LOADOUT")
               .with_size(ComponentSize{pixels(150), pixels(38)})
               .with_background(Theme::Usage::Secondary)
               .with_custom_text_color(theme.font)
               .with_font(UIComponent::DEFAULT_FONT, 19.0f)
               .with_margin(Margin{.left = pixels(12)})
               .with_debug_name("save"));

    button(context, mk(footer.ent(), 2),
           ComponentConfig{}
               .with_label("DEPLOY >>")
               .with_size(ComponentSize{pixels(150), pixels(38)})
               .with_background(Theme::Usage::Accent)
               .with_text_color(Theme::Usage::DarkFont)
               .with_font(UIComponent::DEFAULT_FONT, 19.0f)
               .with_margin(Margin{.left = pixels(12)})
               .with_debug_name("deploy"));
  }
};

REGISTER_EXAMPLE_SCREEN(neon_strike, "Game Mockups",
                        "Tactical shooter loadout screen", NeonStrikeScreen)
