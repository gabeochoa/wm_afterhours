#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct CozyCafeScreen : afterhours::System<UIContext<InputAction>> {
  // Game state
  float music_volume = 0.7f;
  bool notifications_on = true;
  size_t selected_special = 0;
  int gold_coins = 1247;
  int customers_today = 23;
  int reputation = 4;

  std::vector<std::string> daily_specials = {"Lavender Latte", "Honey Toast",
                                             "Matcha Cake"};

  struct Customer {
    std::string name;
    std::string order;
    int wait_time;
  };

  std::vector<Customer> waiting_customers = {
      {"Emma", "Cappuccino", 2},
      {"Oliver", "Croissant", 5},
  };

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::cozy_kraft();
    context.theme = theme;

    // Full screen warm background
    auto main =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
                .with_custom_color(theme.background)
                .with_padding(Spacing::lg)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("main"));

    // ========== HEADER - Big beautiful title ==========
    auto header =
        div(context, mk(main.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.9f), pixels(100)})
                .with_custom_color(theme.surface)
                .with_padding(Spacing::md)
                .with_flex_direction(FlexDirection::Row)
                .with_debug_name("header"));

    // Big cafe name
    div(context, mk(header.ent(), 0),
        ComponentConfig{}
            .with_label("Blossom Cafe")
            .with_size(ComponentSize{pixels(400), pixels(70)})
            .with_color_usage(Theme::Usage::Primary)
            .with_font(UIComponent::DEFAULT_FONT, 48.0f)
            .with_padding(Spacing::sm)
            .with_debug_name("title"));

    // Stats badges
    auto stats = div(context, mk(header.ent(), 1),
                     ComponentConfig{}
                         .with_size(ComponentSize{pixels(450), pixels(70)})
                         .with_custom_color(theme.surface)
                         .with_flex_direction(FlexDirection::Row)
                         .with_debug_name("stats"));

    div(context, mk(stats.ent(), 0),
        ComponentConfig{}
            .with_label("$ " + std::to_string(gold_coins))
            .with_size(ComponentSize{pixels(130), pixels(50)})
            .with_color_usage(Theme::Usage::Accent)
            .with_font(UIComponent::DEFAULT_FONT, 28.0f)
            .with_margin(Spacing::sm)
            .with_debug_name("gold"));

    std::string stars = "";
    for (int i = 0; i < reputation; i++)
      stars += "*";
    div(context, mk(stats.ent(), 1),
        ComponentConfig{}
            .with_label(stars)
            .with_size(ComponentSize{pixels(120), pixels(50)})
            .with_color_usage(Theme::Usage::Secondary)
            .with_font(UIComponent::DEFAULT_FONT, 32.0f)
            .with_margin(Spacing::sm)
            .with_debug_name("stars"));

    div(context, mk(stats.ent(), 2),
        ComponentConfig{}
            .with_label(std::to_string(customers_today) + " served")
            .with_size(ComponentSize{pixels(150), pixels(50)})
            .with_color_usage(Theme::Usage::Primary)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_margin(Spacing::sm)
            .with_debug_name("customers"));

    // ========== MAIN CONTENT - Two columns ==========
    auto content =
        div(context, mk(main.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.9f), pixels(340)})
                .with_custom_color(theme.background)
                .with_flex_direction(FlexDirection::Row)
                .with_margin(Margin{.top = DefaultSpacing::small()})
                .with_debug_name("content"));

    // LEFT - Today's Menu
    auto menu_panel =
        div(context, mk(content.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(420), pixels(320)})
                .with_custom_color(theme.surface)
                .with_padding(Spacing::md)
                .with_flex_direction(FlexDirection::Column)
                .with_margin(Spacing::sm)
                .with_debug_name("menu_panel"));

    div(context, mk(menu_panel.ent(), 0),
        ComponentConfig{}
            .with_label("Today's Specials")
            .with_size(ComponentSize{pixels(360), pixels(45)})
            .with_color_usage(Theme::Usage::Primary)
            .with_font(UIComponent::DEFAULT_FONT, 26.0f)
            .with_padding(Spacing::xs)
            .with_debug_name("menu_title"));

    // Menu items - BIG buttons
    for (size_t i = 0; i < daily_specials.size(); i++) {
      bool selected = (i == selected_special);
      if (button(context, mk(menu_panel.ent(), 1 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(daily_specials[i])
                     .with_size(ComponentSize{pixels(360), pixels(55)})
                     .with_color_usage(selected ? Theme::Usage::Accent
                                                : Theme::Usage::Secondary)
                     .with_font(UIComponent::DEFAULT_FONT, 22.0f)
                     .with_margin(Spacing::sm)
                     .with_debug_name("special_" + std::to_string(i)))) {
        selected_special = i;
      }
    }

    // Promote button
    button(context, mk(menu_panel.ent(), 10),
           ComponentConfig{}
               .with_label("Promote Special!")
               .with_size(ComponentSize{pixels(360), pixels(45)})
               .with_color_usage(Theme::Usage::Primary)
               .with_font(UIComponent::DEFAULT_FONT, 18.0f)
               .with_margin(Margin{.top = DefaultSpacing::medium()})
               .with_debug_name("promote"));

    // RIGHT - Customers waiting
    auto customer_panel =
        div(context, mk(content.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(380), pixels(320)})
                .with_custom_color(theme.surface)
                .with_padding(Spacing::md)
                .with_flex_direction(FlexDirection::Column)
                .with_margin(Spacing::sm)
                .with_debug_name("customer_panel"));

    div(context, mk(customer_panel.ent(), 0),
        ComponentConfig{}
            .with_label("Customers")
            .with_size(ComponentSize{pixels(320), pixels(45)})
            .with_color_usage(Theme::Usage::Primary)
            .with_font(UIComponent::DEFAULT_FONT, 26.0f)
            .with_padding(Spacing::xs)
            .with_debug_name("customer_title"));

    // Customer list
    for (size_t i = 0; i < waiting_customers.size(); i++) {
      auto &c = waiting_customers[i];
      auto row = div(context, mk(customer_panel.ent(), 1 + static_cast<int>(i)),
                     ComponentConfig{}
                         .with_size(ComponentSize{pixels(320), pixels(60)})
                         .with_custom_color(
                             afterhours::colors::darken(theme.surface, 0.95f))
                         .with_padding(Spacing::sm)
                         .with_flex_direction(FlexDirection::Row)
                         .with_margin(Spacing::sm)
                         .with_debug_name("customer_" + std::to_string(i)));

      div(context, mk(row.ent(), 0),
          ComponentConfig{}
              .with_label(c.name)
              .with_size(ComponentSize{pixels(90), pixels(45)})
              .with_color_usage(Theme::Usage::Secondary)
              .with_font(UIComponent::DEFAULT_FONT, 20.0f)
              .with_debug_name("name"));

      div(context, mk(row.ent(), 1),
          ComponentConfig{}
              .with_label(c.order)
              .with_size(ComponentSize{pixels(130), pixels(45)})
              .with_color_usage(Theme::Usage::Primary)
              .with_font(UIComponent::DEFAULT_FONT, 18.0f)
              .with_debug_name("order"));

      div(context, mk(row.ent(), 2),
          ComponentConfig{}
              .with_label(std::to_string(c.wait_time) + "m")
              .with_size(ComponentSize{pixels(60), pixels(45)})
              .with_color_usage(Theme::Usage::Accent)
              .with_font(UIComponent::DEFAULT_FONT, 18.0f)
              .with_debug_name("wait"));
    }

    // Serve button
    button(context, mk(customer_panel.ent(), 10),
           ComponentConfig{}
               .with_label("Serve Next")
               .with_size(ComponentSize{pixels(320), pixels(55)})
               .with_color_usage(Theme::Usage::Accent)
               .with_font(UIComponent::DEFAULT_FONT, 22.0f)
               .with_margin(Margin{.top = DefaultSpacing::medium()})
               .with_debug_name("serve"));

    // ========== FOOTER - Settings ==========
    auto footer =
        div(context, mk(main.ent(), 2),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.7f), pixels(70)})
                .with_custom_color(theme.surface)
                .with_padding(Spacing::md)
                .with_flex_direction(FlexDirection::Row)
                .with_margin(Margin{.top = DefaultSpacing::small()})
                .with_debug_name("footer"));

    // Volume slider
    slider(context, mk(footer.ent(), 0), music_volume,
           ComponentConfig{}
               .with_label("Music")
               .with_size(ComponentSize{pixels(250), pixels(45)})
               .with_color_usage(Theme::Usage::Secondary)
               .with_font(UIComponent::DEFAULT_FONT, 16.0f)
               .with_margin(Spacing::sm)
               .with_debug_name("music"),
           SliderHandleValueLabelPosition::WithLabel);

    // Settings button
    button(context, mk(footer.ent(), 1),
           ComponentConfig{}
               .with_label("Settings")
               .with_size(ComponentSize{pixels(120), pixels(45)})
               .with_color_usage(Theme::Usage::Primary)
               .with_font(UIComponent::DEFAULT_FONT, 18.0f)
               .with_margin(Spacing::sm)
               .with_debug_name("settings"));

    // Save button
    button(context, mk(footer.ent(), 2),
           ComponentConfig{}
               .with_label("Save & Exit")
               .with_size(ComponentSize{pixels(140), pixels(45)})
               .with_color_usage(Theme::Usage::Accent)
               .with_font(UIComponent::DEFAULT_FONT, 18.0f)
               .with_margin(Spacing::sm)
               .with_debug_name("save"));
  }
};

REGISTER_EXAMPLE_SCREEN(cozy_cafe, "Game Mockups",
                        "Cozy cafe simulation game menu", CozyCafeScreen)
