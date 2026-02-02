#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct PaginationShowcase : ScreenSystem<UIContext<InputAction>> {
  // Pagination configuration
  // When true, arrows wrap around (last->first, first->last)
  // When false, arrows are disabled at boundaries
  bool enable_wraparound = false;

  // Pagination state
  size_t page_idx = 0;
  size_t difficulty_idx = 1;
  size_t color_idx = 2;

  std::vector<std::string> pages = {"Page 1", "Page 2", "Page 3", "Page 4",
                                    "Page 5"};
  std::vector<std::string> difficulties = {"Easy", "Medium", "Hard", "Expert"};
  std::vector<std::string> color_options = {"Red", "Green", "Blue", "Yellow", "Purple"};

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context,
                     float) override {
    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;

    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    // Background
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_width), pixels(screen_height)})
            .with_custom_background(theme.background)
            .with_debug_name("bg"));

    // Main card - centered, expanded to fill more screen
    float card_width = screen_width * 0.92f;
    float card_height = screen_height * 0.88f;
    float card_x = (screen_width - card_width) / 2.0f;
    float card_y = (screen_height - card_height) / 2.0f;

    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(card_width), pixels(card_height)})
            .with_absolute_position()
            .with_translate(card_x, card_y)
            .with_custom_background(theme.surface)
            .with_roundness(0.05f)
            .with_debug_name("card"));

    float content_x = card_x + 40.0f;
    float current_y = card_y + 30.0f;

    // Title
    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label("Pagination Component Showcase")
            .with_size(ComponentSize{pixels(card_width - 80.0f), pixels(56)})
            .with_absolute_position()
            .with_translate(content_x, current_y)
            .with_font(UIComponent::DEFAULT_FONT, 32.0f)
            .with_custom_text_color(theme.font)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("title"));

    current_y += 76.0f;

    // Section 1: Basic Pagination
    div(context, mk(entity, 10),
        ComponentConfig{}
            .with_label("Basic Pagination")
            .with_size(ComponentSize{pixels(card_width - 80.0f), pixels(44)})
            .with_absolute_position()
            .with_translate(content_x, current_y)
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_skip_tabbing(true)
            .with_debug_name("section1_label"));

    current_y += 52.0f;

    // Pagination 1 - placed directly without absolute positioning
    // Use a row of individual buttons to simulate pagination
    float pag1_x = content_x;
    float btn_width = 100.0f;
    float btn_height = 48.0f;
    float arrow_width = 48.0f;
    float btn_gap = 8.0f;

    // Left arrow - 44px minimum touch target
    // Disable when at first item (unless wraparound enabled)
    bool pag1_left_disabled = !enable_wraparound && (page_idx == 0);
    button(context, mk(entity, 100),
           ComponentConfig{}
               .with_label("<")
               .with_size(ComponentSize{pixels(arrow_width), pixels(btn_height)})
               .with_absolute_position()
               .with_translate(pag1_x, current_y)
               .with_background(Theme::Usage::Primary)
               .with_font(UIComponent::SYMBOL_FONT, 22.0f)
               .with_disabled(pag1_left_disabled)
               .with_debug_name("pag1_left"));

    pag1_x += arrow_width + btn_gap;

    // Page buttons with gaps
    for (size_t i = 0; i < pages.size(); i++) {
      bool selected = (i == page_idx);
      button(context, mk(entity, 101 + i),
             ComponentConfig{}
                 .with_label(pages[i])
                 .with_size(ComponentSize{pixels(btn_width), pixels(btn_height)})
                 .with_absolute_position()
                 .with_translate(pag1_x, current_y)
                 .with_background(selected ? Theme::Usage::Accent : Theme::Usage::Primary)
                 .with_auto_text_color(true)
                 .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                 .with_debug_name("pag1_btn_" + std::to_string(i)));
      pag1_x += btn_width + btn_gap;
    }

    // Right arrow - 44px minimum touch target
    // Disable when at last item (unless wraparound enabled)
    bool pag1_right_disabled = !enable_wraparound && (page_idx == pages.size() - 1);
    button(context, mk(entity, 106),
           ComponentConfig{}
               .with_label(">")
               .with_size(ComponentSize{pixels(arrow_width), pixels(btn_height)})
               .with_absolute_position()
               .with_translate(pag1_x, current_y)
               .with_background(Theme::Usage::Primary)
               .with_font(UIComponent::SYMBOL_FONT, 22.0f)
               .with_disabled(pag1_right_disabled)
               .with_debug_name("pag1_right"));

    current_y += 58.0f;

    // Status text 1
    div(context, mk(entity, 12),
        ComponentConfig{}
            .with_label("Selected: " + pages[page_idx])
            .with_size(ComponentSize{pixels(300), pixels(36)})
            .with_absolute_position()
            .with_translate(content_x, current_y)
            .with_custom_text_color(theme.font)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_debug_name("status1"));

    current_y += 50.0f;

    // Section 2: Difficulty Selector
    div(context, mk(entity, 20),
        ComponentConfig{}
            .with_label("Difficulty Selector (4 options)")
            .with_size(ComponentSize{pixels(card_width - 80.0f), pixels(44)})
            .with_absolute_position()
            .with_translate(content_x, current_y)
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_skip_tabbing(true)
            .with_debug_name("section2_label"));

    current_y += 52.0f;

    // Pagination 2 - difficulty buttons
    float pag2_x = content_x;

    // Left arrow - disable at first item (unless wraparound enabled)
    bool pag2_left_disabled = !enable_wraparound && (difficulty_idx == 0);
    button(context, mk(entity, 200),
           ComponentConfig{}
               .with_label("<")
               .with_size(ComponentSize{pixels(arrow_width), pixels(btn_height)})
               .with_absolute_position()
               .with_translate(pag2_x, current_y)
               .with_background(Theme::Usage::Primary)
               .with_font(UIComponent::SYMBOL_FONT, 22.0f)
               .with_disabled(pag2_left_disabled)
               .with_debug_name("pag2_left"));

    pag2_x += arrow_width + btn_gap;

    for (size_t i = 0; i < difficulties.size(); i++) {
      bool selected = (i == difficulty_idx);
      button(context, mk(entity, 201 + i),
             ComponentConfig{}
                 .with_label(difficulties[i])
                 .with_size(ComponentSize{pixels(btn_width), pixels(btn_height)})
                 .with_absolute_position()
                 .with_translate(pag2_x, current_y)
                 .with_background(selected ? Theme::Usage::Accent : Theme::Usage::Primary)
                 .with_auto_text_color(true)
                 .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                 .with_debug_name("pag2_btn_" + std::to_string(i)));
      pag2_x += btn_width + btn_gap;
    }

    // Right arrow - disable at last item (unless wraparound enabled)
    bool pag2_right_disabled = !enable_wraparound && (difficulty_idx == difficulties.size() - 1);
    button(context, mk(entity, 205),
           ComponentConfig{}
               .with_label(">")
               .with_size(ComponentSize{pixels(arrow_width), pixels(btn_height)})
               .with_absolute_position()
               .with_translate(pag2_x, current_y)
               .with_background(Theme::Usage::Primary)
               .with_font(UIComponent::SYMBOL_FONT, 22.0f)
               .with_disabled(pag2_right_disabled)
               .with_debug_name("pag2_right"));

    current_y += 58.0f;

    // Status text 2
    div(context, mk(entity, 22),
        ComponentConfig{}
            .with_label("Difficulty: " + difficulties[difficulty_idx])
            .with_size(ComponentSize{pixels(300), pixels(36)})
            .with_absolute_position()
            .with_translate(content_x, current_y)
            .with_custom_text_color(theme.font)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_debug_name("status2"));

    current_y += 50.0f;

    // Section 3: Option Selector (renamed from Color Picker for clearer mental model)
    div(context, mk(entity, 30),
        ComponentConfig{}
            .with_label("Option Selector (5 options)")
            .with_size(ComponentSize{pixels(card_width - 80.0f), pixels(44)})
            .with_absolute_position()
            .with_translate(content_x, current_y)
            .with_background(Theme::Usage::Primary)
            .with_auto_text_color(true)
            .with_padding(Spacing::xs)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_skip_tabbing(true)
            .with_debug_name("section3_label"));

    current_y += 52.0f;

    // Pagination 3 - option buttons
    float pag3_x = content_x;

    // Left arrow - disable at first item (unless wraparound enabled)
    bool pag3_left_disabled = !enable_wraparound && (color_idx == 0);
    button(context, mk(entity, 300),
           ComponentConfig{}
               .with_label("<")
               .with_size(ComponentSize{pixels(arrow_width), pixels(btn_height)})
               .with_absolute_position()
               .with_translate(pag3_x, current_y)
               .with_background(Theme::Usage::Primary)
               .with_font(UIComponent::SYMBOL_FONT, 22.0f)
               .with_disabled(pag3_left_disabled)
               .with_debug_name("pag3_left"));

    pag3_x += arrow_width + btn_gap;

    for (size_t i = 0; i < color_options.size(); i++) {
      bool selected = (i == color_idx);
      button(context, mk(entity, 301 + i),
             ComponentConfig{}
                 .with_label(color_options[i])
                 .with_size(ComponentSize{pixels(btn_width), pixels(btn_height)})
                 .with_absolute_position()
                 .with_translate(pag3_x, current_y)
                 .with_background(selected ? Theme::Usage::Accent : Theme::Usage::Primary)
                 .with_auto_text_color(true)
                 .with_font(UIComponent::DEFAULT_FONT, 18.0f)
                 .with_debug_name("pag3_btn_" + std::to_string(i)));
      pag3_x += btn_width + btn_gap;
    }

    // Right arrow - disable at last item (unless wraparound enabled)
    bool pag3_right_disabled = !enable_wraparound && (color_idx == color_options.size() - 1);
    button(context, mk(entity, 306),
           ComponentConfig{}
               .with_label(">")
               .with_size(ComponentSize{pixels(arrow_width), pixels(btn_height)})
               .with_absolute_position()
               .with_translate(pag3_x, current_y)
               .with_background(Theme::Usage::Primary)
               .with_font(UIComponent::SYMBOL_FONT, 22.0f)
               .with_disabled(pag3_right_disabled)
               .with_debug_name("pag3_right"));

    current_y += 58.0f;

    // Status text 3
    div(context, mk(entity, 32),
        ComponentConfig{}
            .with_label("Selected: " + color_options[color_idx])
            .with_size(ComponentSize{pixels(300), pixels(36)})
            .with_absolute_position()
            .with_translate(content_x, current_y)
            .with_custom_text_color(theme.font)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_debug_name("status3"));
  }
};

REGISTER_EXAMPLE_SCREEN(pagination, "Component Galleries",
                        "Pagination widget demo", PaginationShowcase)

