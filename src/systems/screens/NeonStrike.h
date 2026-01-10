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
    // Military/tactical theme - darker, more aggressive colors
    Theme theme;
    theme.font = afterhours::Color{220, 220, 210, 255};      // Off-white/tan
    theme.darkfont = afterhours::Color{10, 10, 10, 255};     // Near black
    theme.font_muted = afterhours::Color{120, 115, 100, 255};
    theme.background = afterhours::Color{15, 15, 12, 255};   // Dark olive-black
    theme.surface = afterhours::Color{28, 28, 22, 255};      // Dark olive
    theme.primary = afterhours::Color{45, 45, 35, 255};      // Military gray-green
    theme.secondary = afterhours::Color{60, 55, 45, 255};    // Tan-brown
    theme.accent = afterhours::Color{180, 120, 40, 255};     // Orange/gold accent
    theme.error = afterhours::Color{180, 50, 50, 255};
    context.theme = theme;

    // Full screen dark background - no padding to prevent shifting
    auto main =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
                .with_custom_background(theme.background)
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("main"));

    // ========== HEADER - Mission Brief ==========
    auto header =
        div(context, mk(main.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(80)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Row)
                .disable_rounded_corners()
                .with_debug_name("header"));

    div(context, mk(header.ent(), 0),
        ComponentConfig{}
            .with_label("OPERATION: BLACKOUT")
            .with_size(ComponentSize{pixels(380), pixels(55)})
            .with_background(Theme::Usage::Accent)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 28.0f)
            .with_padding(Spacing::sm)
            .disable_rounded_corners()
            .with_debug_name("mission_name"));

    div(context, mk(header.ent(), 1),
        ComponentConfig{}
            .with_label("EXTRACTION")
            .with_size(ComponentSize{pixels(160), pixels(45)})
            .with_custom_background(afterhours::Color{80, 30, 30, 255})
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 22.0f)
            .with_margin(Spacing::sm)
            .disable_rounded_corners()
            .with_debug_name("mission_type"));

    div(context, mk(header.ent(), 2),
        ComponentConfig{}
            .with_label("SQUAD: 4/4")
            .with_size(ComponentSize{pixels(140), pixels(45)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 22.0f)
            .with_margin(Spacing::sm)
            .disable_rounded_corners()
            .with_debug_name("squad"));

    // ========== MAIN CONTENT ==========
    auto content =
        div(context, mk(main.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(420)})
                .with_custom_background(theme.background)
                .with_flex_direction(FlexDirection::Row)
                .with_margin(Margin{.top = DefaultSpacing::small()})
                .with_debug_name("content"));

    // LEFT - Weapon Selection
    auto weapon_panel =
        div(context, mk(content.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(520), pixels(400)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::md)
                .with_flex_direction(FlexDirection::Column)
                .with_margin(Spacing::xs)
                .disable_rounded_corners()
                .with_debug_name("weapon_panel"));

    // PRIMARY WEAPON header
    div(context, mk(weapon_panel.ent(), 0),
        ComponentConfig{}
            .with_label("PRIMARY WEAPON")
            .with_size(ComponentSize{pixels(460), pixels(40)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 22.0f)
            .with_padding(Spacing::xs)
            .disable_rounded_corners()
            .with_skip_tabbing(true)
            .with_debug_name("weapon_section_label"));

    // Weapon buttons with better contrast
    for (size_t i = 0; i < weapons.size(); i++) {
      bool sel = (i == selected_weapon);
      if (button(context, mk(weapon_panel.ent(), 1 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(weapons[i].name)
                     .with_size(ComponentSize{pixels(460), pixels(44)})
                     .with_custom_background(sel ? theme.accent : theme.primary)
                     .with_auto_text_color(true)
                     .with_font(UIComponent::DEFAULT_FONT, 22.0f)
                     .with_margin(Spacing::xs)
                     .disable_rounded_corners()
                     .with_debug_name("weapon_" + std::to_string(i)))) {
        selected_weapon = i;
      }
    }

    // Stats for selected weapon
    auto &w = weapons[selected_weapon];
    auto stats = div(
        context, mk(weapon_panel.ent(), 10),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(460), pixels(110)})
            .with_custom_background(afterhours::colors::darken(theme.surface, 0.7f))
            .with_padding(Spacing::sm)
            .with_flex_direction(FlexDirection::Column)
            .with_margin(Margin{.top = DefaultSpacing::small()})
            .disable_rounded_corners()
            .with_debug_name("stats"));

    // Stat bars
    auto draw_stat = [&](int idx, const std::string &name, int value) {
      auto row = div(context, mk(stats.ent(), idx),
                     ComponentConfig{}
                         .with_size(ComponentSize{pixels(430), pixels(30)})
                         .with_custom_background(
                             afterhours::colors::darken(theme.surface, 0.7f))
                         .with_flex_direction(FlexDirection::Row)
                         .with_debug_name(name + "_row"));

      div(context, mk(row.ent(), 0),
          ComponentConfig{}
              .with_label(name)
              .with_size(ComponentSize{pixels(100), pixels(28)})
              .with_custom_text_color(theme.font)
              .with_font(UIComponent::DEFAULT_FONT, 20.0f)
              .with_debug_name(name + "_label"));

      // Bar background
      div(context, mk(row.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(220), pixels(18)})
              .with_custom_background(afterhours::Color{20, 20, 18, 255})
              .disable_rounded_corners()
              .with_debug_name(name + "_bg"));

      // Bar fill
      div(context, mk(row.ent(), 2),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(value * 2.2f), pixels(18)})
              .with_background(Theme::Usage::Accent)
              .with_margin(Margin{.left = pixels(-220)})
              .disable_rounded_corners()
              .with_debug_name(name + "_fill"));

      div(context, mk(row.ent(), 3),
          ComponentConfig{}
              .with_label(std::to_string(value))
              .with_size(ComponentSize{pixels(50), pixels(28)})
              .with_custom_text_color(theme.font)
              .with_font(UIComponent::DEFAULT_FONT, 20.0f)
              .with_debug_name(name + "_val"));
    };

    draw_stat(0, "DMG", w.damage);
    draw_stat(1, "ACC", w.accuracy);
    draw_stat(2, "ROF", w.fire_rate);

    // RIGHT - Gear & Perks
    auto gear_panel =
        div(context, mk(content.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(400), pixels(400)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::md)
                .with_flex_direction(FlexDirection::Column)
                .with_margin(Spacing::xs)
                .disable_rounded_corners()
                .with_debug_name("gear_panel"));

    // SECONDARY WEAPON - Navigation bar
    div(context, mk(gear_panel.ent(), 0),
        ComponentConfig{}
            .with_label("SECONDARY")
            .with_size(ComponentSize{pixels(340), pixels(35)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_padding(Spacing::xs)
            .disable_rounded_corners()
            .with_skip_tabbing(true)
            .with_debug_name("secondary_label"));

    navigation_bar(context, mk(gear_panel.ent(), 1), secondary_weapons,
                   selected_secondary,
                   ComponentConfig{}
                       .with_size(ComponentSize{pixels(340), pixels(40)})
                       .with_background(Theme::Usage::Primary)
                       .with_auto_text_color(true)
                       .with_font(UIComponent::DEFAULT_FONT, 20.0f)
                       .with_margin(Spacing::xs)
                       .disable_rounded_corners()
                       .with_debug_name("secondary_nav"));

    // TACTICAL GEAR section
    div(context, mk(gear_panel.ent(), 5),
        ComponentConfig{}
            .with_label("TACTICAL GEAR")
            .with_size(ComponentSize{pixels(340), pixels(35)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_margin(Margin{.top = DefaultSpacing::small()})
            .with_padding(Spacing::xs)
            .disable_rounded_corners()
            .with_skip_tabbing(true)
            .with_debug_name("gear_label"));

    // Gear buttons - 2x2 grid style
    auto gear_row1 = div(context, mk(gear_panel.ent(), 6),
                         ComponentConfig{}
                             .with_size(ComponentSize{pixels(340), pixels(40)})
                             .with_flex_direction(FlexDirection::Row)
                             .with_debug_name("gear_row1"));

    for (size_t i = 0; i < 2 && i < gear.size(); i++) {
      button(context, mk(gear_row1.ent(), static_cast<int>(i)),
             ComponentConfig{}
                 .with_label(gear[i])
                 .with_size(ComponentSize{pixels(165), pixels(40)})
                 .with_background(Theme::Usage::Secondary)
                 .with_auto_text_color(true)
                 .with_font(UIComponent::DEFAULT_FONT, 20.0f)
                 .with_margin(Spacing::xs)
                 .disable_rounded_corners()
                 .with_debug_name("gear_" + std::to_string(i)));
    }

    auto gear_row2 = div(context, mk(gear_panel.ent(), 7),
                         ComponentConfig{}
                             .with_size(ComponentSize{pixels(340), pixels(40)})
                             .with_flex_direction(FlexDirection::Row)
                             .with_debug_name("gear_row2"));

    for (size_t i = 2; i < 4 && i < gear.size(); i++) {
      button(context, mk(gear_row2.ent(), static_cast<int>(i)),
             ComponentConfig{}
                 .with_label(gear[i])
                 .with_size(ComponentSize{pixels(165), pixels(40)})
                 .with_background(Theme::Usage::Secondary)
                 .with_auto_text_color(true)
                 .with_font(UIComponent::DEFAULT_FONT, 20.0f)
                 .with_margin(Spacing::xs)
                 .disable_rounded_corners()
                 .with_debug_name("gear_" + std::to_string(i)));
    }

    // PERK section
    div(context, mk(gear_panel.ent(), 10),
        ComponentConfig{}
            .with_label("PERK")
            .with_size(ComponentSize{pixels(340), pixels(35)})
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_margin(Margin{.top = DefaultSpacing::small()})
            .with_padding(Spacing::xs)
            .disable_rounded_corners()
            .with_skip_tabbing(true)
            .with_debug_name("perk_label"));

    // Perk dropdown
    dropdown(context, mk(gear_panel.ent(), 11), perks, selected_perk,
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(340), pixels(40)})
                 .with_background(Theme::Usage::Accent)
                 .with_auto_text_color(true)
                 .with_font(UIComponent::DEFAULT_FONT, 20.0f)
                 .with_margin(Spacing::xs)
                 .disable_rounded_corners()
                 .with_debug_name("perk_dropdown"));

    // ========== FOOTER - Action buttons ==========
    auto footer =
        div(context, mk(main.ent(), 2),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(65)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Row)
                .with_margin(Margin{.top = DefaultSpacing::small()})
                .disable_rounded_corners()
                .with_debug_name("footer"));

    button(context, mk(footer.ent(), 0),
           ComponentConfig{}
               .with_label("BACK")
               .with_size(ComponentSize{pixels(120), pixels(45)})
               .with_background(Theme::Usage::Secondary)
               .with_auto_text_color(true)
               .with_font(UIComponent::DEFAULT_FONT, 20.0f)
               .with_margin(Spacing::sm)
               .disable_rounded_corners()
               .with_debug_name("back"));

    button(context, mk(footer.ent(), 1),
           ComponentConfig{}
               .with_label("SAVE LOADOUT")
               .with_size(ComponentSize{pixels(180), pixels(45)})
               .with_background(Theme::Usage::Primary)
               .with_auto_text_color(true)
               .with_font(UIComponent::DEFAULT_FONT, 20.0f)
               .with_margin(Spacing::sm)
               .disable_rounded_corners()
               .with_debug_name("save"));

    button(context, mk(footer.ent(), 2),
           ComponentConfig{}
               .with_label("DEPLOY >>")
               .with_size(ComponentSize{pixels(200), pixels(50)})
               .with_background(Theme::Usage::Accent)
               .with_auto_text_color(true)
               .with_font(UIComponent::DEFAULT_FONT, 24.0f)
               .with_margin(Spacing::sm)
               .disable_rounded_corners()
               .with_debug_name("deploy"));
  }
};

REGISTER_EXAMPLE_SCREEN(neon_strike, "Game Mockups",
                        "Tactical shooter loadout screen", NeonStrikeScreen)
