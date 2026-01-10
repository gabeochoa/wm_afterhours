#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct NeonStrikeScreen : afterhours::System<UIContext<InputAction>> {
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
  size_t selected_perk = 0;
  
  std::vector<std::string> perks = {"QUICK HANDS", "IRON SKIN", "GHOST STEP"};
  std::vector<std::string> gear = {"SMOKE", "EMP", "DRONE", "MEDKIT"};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;

    // Full screen dark background
    auto main = div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
            .with_custom_color(theme.background)
            .with_padding(Spacing::lg)
            .with_flex_direction(FlexDirection::Column)
            .with_debug_name("main"));

    // ========== HEADER - Mission Brief ==========
    auto header = div(context, mk(main.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(0.92f), pixels(90)})
            .with_custom_color(theme.surface)
            .with_padding(Spacing::md)
            .with_flex_direction(FlexDirection::Row)
            .disable_rounded_corners()
            .with_debug_name("header"));

    div(context, mk(header.ent(), 0),
        ComponentConfig{}
            .with_label("OPERATION: BLACKOUT")
            .with_size(ComponentSize{pixels(450), pixels(60)})
            .with_color_usage(Theme::Usage::Primary)
            .with_font(UIComponent::DEFAULT_FONT, 36.0f)
            .with_padding(Spacing::sm)
            .disable_rounded_corners()
            .with_debug_name("mission_name"));

    div(context, mk(header.ent(), 1),
        ComponentConfig{}
            .with_label("EXTRACTION")
            .with_size(ComponentSize{pixels(180), pixels(50)})
            .with_color_usage(Theme::Usage::Accent)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_margin(Spacing::md)
            .with_debug_name("mission_type"));

    div(context, mk(header.ent(), 2),
        ComponentConfig{}
            .with_label("SQUAD: 4/4")
            .with_size(ComponentSize{pixels(150), pixels(50)})
            .with_color_usage(Theme::Usage::Secondary)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_margin(Spacing::md)
            .with_debug_name("squad"));

    // ========== MAIN CONTENT ==========
    auto content = div(context, mk(main.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(0.92f), pixels(380)})
            .with_custom_color(theme.background)
            .with_flex_direction(FlexDirection::Row)
            .with_margin(Margin{.top = DefaultSpacing::small()})
            .with_debug_name("content"));

    // LEFT - Weapon Selection
    auto weapon_panel = div(context, mk(content.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(480), pixels(360)})
            .with_custom_color(theme.surface)
            .with_padding(Spacing::md)
            .with_flex_direction(FlexDirection::Column)
            .with_margin(Spacing::sm)
            .disable_rounded_corners()
            .with_debug_name("weapon_panel"));

    div(context, mk(weapon_panel.ent(), 0),
        ComponentConfig{}
            .with_label("< PRIMARY WEAPON >")
            .with_size(ComponentSize{pixels(420), pixels(45)})
            .with_color_usage(Theme::Usage::Primary)
            .with_font(UIComponent::DEFAULT_FONT, 22.0f)
            .with_padding(Spacing::xs)
            .disable_rounded_corners()
            .with_debug_name("weapon_header"));

    // Weapon buttons
    for (size_t i = 0; i < weapons.size(); i++) {
      bool sel = (i == selected_weapon);
      if (button(context, mk(weapon_panel.ent(), 1 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(weapons[i].name)
                     .with_size(ComponentSize{pixels(420), pixels(50)})
                     .with_color_usage(sel ? Theme::Usage::Accent : Theme::Usage::Primary)
                     .with_font(UIComponent::DEFAULT_FONT, 20.0f)
                     .with_margin(Spacing::sm)
                     .disable_rounded_corners()
                     .with_debug_name("weapon_" + std::to_string(i)))) {
        selected_weapon = i;
      }
    }

    // Stats for selected weapon - inline with weapon buttons
    auto& w = weapons[selected_weapon];
    auto stats = div(context, mk(weapon_panel.ent(), 10),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(400), pixels(100)})
            .with_custom_color(afterhours::colors::darken(theme.surface, 0.8f))
            .with_padding(Spacing::sm)
            .with_flex_direction(FlexDirection::Column)
            .with_margin(Spacing::sm)
            .disable_rounded_corners()
            .with_debug_name("stats"));

    // Stat bars
    auto draw_stat = [&](int idx, const std::string& name, int value) {
      auto row = div(context, mk(stats.ent(), idx),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(370), pixels(28)})
              .with_custom_color(afterhours::colors::darken(theme.surface, 0.8f))
              .with_flex_direction(FlexDirection::Row)
              .with_debug_name(name + "_row"));

      div(context, mk(row.ent(), 0),
          ComponentConfig{}
              .with_label(name)
              .with_size(ComponentSize{pixels(90), pixels(24)})
              .with_color_usage(Theme::Usage::Font)
              .with_font(UIComponent::DEFAULT_FONT, 13.0f)
              .with_debug_name(name + "_label"));

      // Bar background
      div(context, mk(row.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(180), pixels(16)})
              .with_custom_color(afterhours::Color{30, 30, 40, 255})
              .disable_rounded_corners()
              .with_debug_name(name + "_bg"));

      // Bar fill
      div(context, mk(row.ent(), 2),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(value * 1.8f), pixels(16)})
              .with_color_usage(Theme::Usage::Accent)
              .with_margin(Margin{.left = pixels(-180)})
              .disable_rounded_corners()
              .with_debug_name(name + "_fill"));

      div(context, mk(row.ent(), 3),
          ComponentConfig{}
              .with_label(std::to_string(value))
              .with_size(ComponentSize{pixels(40), pixels(24)})
              .with_color_usage(Theme::Usage::Font)
              .with_font(UIComponent::DEFAULT_FONT, 13.0f)
              .with_debug_name(name + "_val"));
    };

    draw_stat(0, "DAMAGE", w.damage);
    draw_stat(1, "ACCURACY", w.accuracy);
    draw_stat(2, "FIRE RATE", w.fire_rate);

    // RIGHT - Gear & Perks
    auto gear_panel = div(context, mk(content.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(360), pixels(360)})
            .with_custom_color(theme.surface)
            .with_padding(Spacing::md)
            .with_flex_direction(FlexDirection::Column)
            .with_margin(Spacing::sm)
            .disable_rounded_corners()
            .with_debug_name("gear_panel"));

    div(context, mk(gear_panel.ent(), 0),
        ComponentConfig{}
            .with_label("< TACTICAL GEAR >")
            .with_size(ComponentSize{pixels(300), pixels(40)})
            .with_color_usage(Theme::Usage::Primary)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_padding(Spacing::xs)
            .disable_rounded_corners()
            .with_debug_name("gear_header"));

    // Gear buttons
    for (size_t i = 0; i < gear.size(); i++) {
      button(context, mk(gear_panel.ent(), 1 + static_cast<int>(i)),
             ComponentConfig{}
                 .with_label(gear[i])
                 .with_size(ComponentSize{pixels(300), pixels(36)})
                 .with_color_usage(Theme::Usage::Secondary)
                 .with_font(UIComponent::DEFAULT_FONT, 16.0f)
                 .with_margin(Spacing::xs)
                 .disable_rounded_corners()
                 .with_debug_name("gear_" + std::to_string(i)));
    }

    div(context, mk(gear_panel.ent(), 10),
        ComponentConfig{}
            .with_label("< PERK >")
            .with_size(ComponentSize{pixels(300), pixels(36)})
            .with_color_usage(Theme::Usage::Primary)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_margin(Margin{.top = DefaultSpacing::medium()})
            .disable_rounded_corners()
            .with_debug_name("perk_header"));

    // Perk dropdown
    dropdown(context, mk(gear_panel.ent(), 11), perks, selected_perk,
             ComponentConfig{}
                 .with_size(ComponentSize{pixels(300), pixels(45)})
                 .with_color_usage(Theme::Usage::Accent)
                 .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                 .with_margin(Spacing::sm)
                 .disable_rounded_corners()
                 .with_debug_name("perk_dropdown"));

    // ========== FOOTER - Action buttons ==========
    auto footer = div(context, mk(main.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(0.7f), pixels(70)})
            .with_custom_color(theme.surface)
            .with_padding(Spacing::md)
            .with_flex_direction(FlexDirection::Row)
            .with_margin(Margin{.top = DefaultSpacing::small()})
            .disable_rounded_corners()
            .with_debug_name("footer"));

    button(context, mk(footer.ent(), 0),
           ComponentConfig{}
               .with_label("< BACK")
               .with_size(ComponentSize{pixels(120), pixels(45)})
               .with_color_usage(Theme::Usage::Secondary)
               .with_font(UIComponent::DEFAULT_FONT, 18.0f)
               .with_margin(Spacing::sm)
               .disable_rounded_corners()
               .with_debug_name("back"));

    button(context, mk(footer.ent(), 1),
           ComponentConfig{}
               .with_label("SAVE LOADOUT")
               .with_size(ComponentSize{pixels(160), pixels(45)})
               .with_color_usage(Theme::Usage::Primary)
               .with_font(UIComponent::DEFAULT_FONT, 18.0f)
               .with_margin(Spacing::sm)
               .disable_rounded_corners()
               .with_debug_name("save"));

    button(context, mk(footer.ent(), 2),
           ComponentConfig{}
               .with_label(">> DEPLOY <<")
               .with_size(ComponentSize{pixels(200), pixels(50)})
               .with_color_usage(Theme::Usage::Accent)
               .with_font(UIComponent::DEFAULT_FONT, 22.0f)
               .with_margin(Spacing::sm)
               .disable_rounded_corners()
               .with_debug_name("deploy"));
  }
};

REGISTER_EXAMPLE_SCREEN(neon_strike, "Game Mockups", "Tactical shooter loadout screen", NeonStrikeScreen)
