#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct EmpireTycoonScreen : afterhours::System<UIContext<InputAction>> {
  int64_t cash = 2847500;
  int64_t revenue = 485000;
  int employees = 156;
  float stock_change = 2.3f;

  struct Department {
    std::string name;
    int staff;
    int efficiency;
  };

  std::vector<Department> departments = {
    {"Engineering", 45, 92},
    {"Sales", 32, 78},
    {"Marketing", 18, 85},
  };

  struct Project {
    std::string name;
    int progress;
  };

  std::vector<Project> projects = {
    {"Product Launch 2.0", 75},
    {"Market Expansion", 45},
  };

  std::string format_money(int64_t amt) {
    if (amt >= 1000000) return "$" + std::to_string(amt / 1000000) + "." + std::to_string((amt % 1000000) / 100000) + "M";
    if (amt >= 1000) return "$" + std::to_string(amt / 1000) + "K";
    return "$" + std::to_string(amt);
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::ocean_navy();
    context.theme = theme;

    // Full screen
    auto main = div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
            .with_custom_color(theme.background)
            .with_padding(Spacing::lg)
            .with_flex_direction(FlexDirection::Column)
            .with_debug_name("main"));

    // ========== HEADER - Company & Metrics ==========
    auto header = div(context, mk(main.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(0.92f), pixels(100)})
            .with_custom_color(theme.surface)
            .with_padding(Spacing::md)
            .with_flex_direction(FlexDirection::Row)
            .with_debug_name("header"));

    div(context, mk(header.ent(), 0),
        ComponentConfig{}
            .with_label("NEXUS INDUSTRIES")
            .with_size(ComponentSize{pixels(380), pixels(70)})
            .with_color_usage(Theme::Usage::Primary)
            .with_font(UIComponent::DEFAULT_FONT, 36.0f)
            .with_padding(Spacing::sm)
            .with_debug_name("company_name"));

    // Metrics row
    auto metrics = div(context, mk(header.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(600), pixels(70)})
            .with_custom_color(theme.surface)
            .with_flex_direction(FlexDirection::Row)
            .with_debug_name("metrics"));

    // Cash
    auto cash_card = div(context, mk(metrics.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(140), pixels(55)})
            .with_color_usage(Theme::Usage::Secondary)
            .with_flex_direction(FlexDirection::Column)
            .with_padding(Spacing::xs)
            .with_margin(Spacing::xs)
            .with_debug_name("cash_card"));
    div(context, mk(cash_card.ent(), 0),
        ComponentConfig{}
            .with_label("CASH")
            .with_size(ComponentSize{pixels(120), pixels(20)})
            .with_color_usage(Theme::Usage::Secondary)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_debug_name("cash_label"));
    div(context, mk(cash_card.ent(), 1),
        ComponentConfig{}
            .with_label(format_money(cash))
            .with_size(ComponentSize{pixels(120), pixels(30)})
            .with_color_usage(Theme::Usage::Secondary)
            .with_font(UIComponent::DEFAULT_FONT, 22.0f)
            .with_debug_name("cash_value"));

    // Revenue
    auto rev_card = div(context, mk(metrics.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(150), pixels(55)})
            .with_color_usage(Theme::Usage::Accent)
            .with_flex_direction(FlexDirection::Column)
            .with_padding(Spacing::xs)
            .with_margin(Spacing::xs)
            .with_debug_name("rev_card"));
    div(context, mk(rev_card.ent(), 0),
        ComponentConfig{}
            .with_label("REVENUE")
            .with_size(ComponentSize{pixels(130), pixels(20)})
            .with_color_usage(Theme::Usage::Accent)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_debug_name("rev_label"));
    div(context, mk(rev_card.ent(), 1),
        ComponentConfig{}
            .with_label(format_money(revenue) + "/mo")
            .with_size(ComponentSize{pixels(130), pixels(30)})
            .with_color_usage(Theme::Usage::Accent)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_debug_name("rev_value"));

    // Employees
    auto emp_card = div(context, mk(metrics.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(120), pixels(55)})
            .with_color_usage(Theme::Usage::Primary)
            .with_flex_direction(FlexDirection::Column)
            .with_padding(Spacing::xs)
            .with_margin(Spacing::xs)
            .with_debug_name("emp_card"));
    div(context, mk(emp_card.ent(), 0),
        ComponentConfig{}
            .with_label("STAFF")
            .with_size(ComponentSize{pixels(100), pixels(20)})
            .with_color_usage(Theme::Usage::Primary)
            .with_font(UIComponent::DEFAULT_FONT, 14.0f)
            .with_debug_name("emp_label"));
    div(context, mk(emp_card.ent(), 1),
        ComponentConfig{}
            .with_label(std::to_string(employees))
            .with_size(ComponentSize{pixels(100), pixels(30)})
            .with_color_usage(Theme::Usage::Primary)
            .with_font(UIComponent::DEFAULT_FONT, 24.0f)
            .with_debug_name("emp_value"));

    // ========== MAIN CONTENT ==========
    auto content = div(context, mk(main.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(0.92f), pixels(340)})
            .with_custom_color(theme.background)
            .with_flex_direction(FlexDirection::Row)
            .with_margin(Margin{.top = DefaultSpacing::small()})
            .with_debug_name("content"));

    // LEFT - Departments
    auto dept_panel = div(context, mk(content.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(420), pixels(320)})
            .with_custom_color(theme.surface)
            .with_padding(Spacing::md)
            .with_flex_direction(FlexDirection::Column)
            .with_margin(Spacing::sm)
            .with_debug_name("dept_panel"));

    div(context, mk(dept_panel.ent(), 0),
        ComponentConfig{}
            .with_label("DEPARTMENTS")
            .with_size(ComponentSize{pixels(360), pixels(40)})
            .with_color_usage(Theme::Usage::Primary)
            .with_font(UIComponent::DEFAULT_FONT, 24.0f)
            .with_padding(Spacing::xs)
            .with_debug_name("dept_header"));

    for (size_t i = 0; i < departments.size(); i++) {
      auto& d = departments[i];
      auto row = div(context, mk(dept_panel.ent(), 1 + static_cast<int>(i)),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(360), pixels(65)})
              .with_custom_color(afterhours::colors::darken(theme.surface, 0.85f))
              .with_padding(Spacing::sm)
              .with_flex_direction(FlexDirection::Row)
              .with_margin(Spacing::sm)
              .with_debug_name("dept_" + std::to_string(i)));

      div(context, mk(row.ent(), 0),
          ComponentConfig{}
              .with_label(d.name)
              .with_size(ComponentSize{pixels(140), pixels(45)})
              .with_color_usage(Theme::Usage::Primary)
              .with_font(UIComponent::DEFAULT_FONT, 18.0f)
              .with_debug_name("dept_name"));

      div(context, mk(row.ent(), 1),
          ComponentConfig{}
              .with_label(std::to_string(d.staff) + " staff")
              .with_size(ComponentSize{pixels(90), pixels(45)})
              .with_color_usage(Theme::Usage::Secondary)
              .with_font(UIComponent::DEFAULT_FONT, 16.0f)
              .with_debug_name("dept_staff"));

      div(context, mk(row.ent(), 2),
          ComponentConfig{}
              .with_label(std::to_string(d.efficiency) + "%")
              .with_size(ComponentSize{pixels(70), pixels(45)})
              .with_color_usage(d.efficiency >= 85 ? Theme::Usage::Secondary : Theme::Usage::Accent)
              .with_font(UIComponent::DEFAULT_FONT, 16.0f)
              .with_debug_name("dept_eff"));
    }

    // RIGHT - Projects
    auto proj_panel = div(context, mk(content.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(380), pixels(320)})
            .with_custom_color(theme.surface)
            .with_padding(Spacing::md)
            .with_flex_direction(FlexDirection::Column)
            .with_margin(Spacing::sm)
            .with_debug_name("proj_panel"));

    div(context, mk(proj_panel.ent(), 0),
        ComponentConfig{}
            .with_label("ACTIVE PROJECTS")
            .with_size(ComponentSize{pixels(320), pixels(40)})
            .with_color_usage(Theme::Usage::Primary)
            .with_font(UIComponent::DEFAULT_FONT, 22.0f)
            .with_padding(Spacing::xs)
            .with_debug_name("proj_header"));

    for (size_t i = 0; i < projects.size(); i++) {
      auto& p = projects[i];
      auto card = div(context, mk(proj_panel.ent(), 1 + static_cast<int>(i)),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(320), pixels(95)})
              .with_custom_color(afterhours::colors::darken(theme.surface, 0.85f))
              .with_padding(Spacing::md)
              .with_flex_direction(FlexDirection::Column)
              .with_margin(Spacing::sm)
              .with_debug_name("proj_" + std::to_string(i)));

      div(context, mk(card.ent(), 0),
          ComponentConfig{}
              .with_label(p.name)
              .with_size(ComponentSize{pixels(280), pixels(30)})
              .with_color_usage(Theme::Usage::Primary)
              .with_font(UIComponent::DEFAULT_FONT, 18.0f)
              .with_debug_name("proj_name"));

      // Progress bar bg
      div(context, mk(card.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(280), pixels(22)})
              .with_custom_color(afterhours::Color{30, 50, 80, 255})
              .with_debug_name("prog_bg"));

      // Progress bar fill
      div(context, mk(card.ent(), 2),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(p.progress * 2.8f), pixels(22)})
              .with_color_usage(p.progress >= 70 ? Theme::Usage::Secondary : Theme::Usage::Accent)
              .with_margin(Margin{.top = pixels(-22)})
              .with_debug_name("prog_fill"));

      div(context, mk(card.ent(), 3),
          ComponentConfig{}
              .with_label(std::to_string(p.progress) + "% complete")
              .with_size(ComponentSize{pixels(280), pixels(22)})
              .with_color_usage(Theme::Usage::Font)
              .with_font(UIComponent::DEFAULT_FONT, 14.0f)
              .with_debug_name("prog_text"));
    }

    // ========== FOOTER ==========
    auto footer = div(context, mk(main.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(0.7f), pixels(70)})
            .with_custom_color(theme.surface)
            .with_padding(Spacing::md)
            .with_flex_direction(FlexDirection::Row)
            .with_margin(Margin{.top = DefaultSpacing::small()})
            .with_debug_name("footer"));

    button(context, mk(footer.ent(), 0),
           ComponentConfig{}
               .with_label("HIRE")
               .with_size(ComponentSize{pixels(100), pixels(45)})
               .with_color_usage(Theme::Usage::Secondary)
               .with_font(UIComponent::DEFAULT_FONT, 18.0f)
               .with_margin(Spacing::sm)
               .with_debug_name("hire"));

    button(context, mk(footer.ent(), 1),
           ComponentConfig{}
               .with_label("BUILD")
               .with_size(ComponentSize{pixels(100), pixels(45)})
               .with_color_usage(Theme::Usage::Primary)
               .with_font(UIComponent::DEFAULT_FONT, 18.0f)
               .with_margin(Spacing::sm)
               .with_debug_name("build"));

    button(context, mk(footer.ent(), 2),
           ComponentConfig{}
               .with_label("RESEARCH")
               .with_size(ComponentSize{pixels(130), pixels(45)})
               .with_color_usage(Theme::Usage::Accent)
               .with_font(UIComponent::DEFAULT_FONT, 18.0f)
               .with_margin(Spacing::sm)
               .with_debug_name("research"));

    button(context, mk(footer.ent(), 3),
           ComponentConfig{}
               .with_label("NEXT QUARTER >>")
               .with_size(ComponentSize{pixels(180), pixels(50)})
               .with_color_usage(Theme::Usage::Secondary)
               .with_font(UIComponent::DEFAULT_FONT, 20.0f)
               .with_margin(Spacing::sm)
               .with_debug_name("next_quarter"));
  }
};

REGISTER_EXAMPLE_SCREEN(empire_tycoon, "Game Mockups", "Business tycoon dashboard", EmpireTycoonScreen)
