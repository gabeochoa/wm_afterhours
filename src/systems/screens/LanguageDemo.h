#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/translation.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;
using namespace afterhours::translation;

struct LanguageDemoScreen : ScreenSystem<UIContext<InputAction>> {
  Language current_language = Language::English;

  // Configuration options for design improvements
  struct Config {
    // Issue 1: Make keyboard shortcuts more discoverable
    bool show_prominent_keyboard_hints = true;  // Show hints on language buttons

    // Issue 2: Show full language names below ISO codes
    // NOTE: Disabled by default — multiline labels caused the button_row to
    // overflow the header's right edge at 68px button width.
    bool show_full_language_names = false;

    // Issue 3: Button color consistency
    bool use_consistent_button_colors = true;  // Use Primary instead of Accent for Continue button
  };
  Config config;

  struct LanguageSample {
    std::string title;
    std::string greeting;
    std::string menu_items[4];
    std::string button_text;
  };

  LanguageSample get_sample(Language lang) {
    switch (lang) {
    case Language::Korean:
      return LanguageSample{"언어 데모",
                            "안녕하세요!",
                            {"시작", "설정", "정보", "종료"},
                            "계속하기"};
    case Language::Japanese:
      return LanguageSample{"言語デモ",
                            "こんにちは！",
                            {"開始", "設定", "情報", "終了"},
                            "続ける"};
    case Language::English:
    default:
      return LanguageSample{"Language Demo",
                            "Hello!",
                            {"Start", "Settings", "About", "Exit"},
                            "Continue"};
    }
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Keyboard shortcuts
    if (raylib::IsKeyPressed(raylib::KEY_ONE) ||
        raylib::IsKeyPressed(raylib::KEY_KP_1)) {
      current_language = Language::English;
    }
    if (raylib::IsKeyPressed(raylib::KEY_TWO) ||
        raylib::IsKeyPressed(raylib::KEY_KP_2)) {
      current_language = Language::Korean;
    }
    if (raylib::IsKeyPressed(raylib::KEY_THREE) ||
        raylib::IsKeyPressed(raylib::KEY_KP_3)) {
      current_language = Language::Japanese;
    }

    auto theme = afterhours::ui::theme_presets::ocean_navy();
    context.theme = theme;

    const FontConfig &font_config = theme.get_font_config(current_language);
    float scaled_size = theme.get_scaled_font_size(current_language, 20.0f);
    LanguageSample sample = get_sample(current_language);

    // Main container - centered with padding
    auto root =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.92f), screen_pct(0.85f)})
                .with_self_align(SelfAlign::Center)
                .with_background(Theme::Usage::Background)
                .with_roundness(0.05f)
                .with_padding(Spacing::md)
                .with_debug_name("main_bg"));

    // Content container - use percent(1.0f) to resolve during parent phase
    auto main = div(context, mk(root.ent(), 0),
                    ComponentConfig{}
                        .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                        .with_flex_direction(FlexDirection::Column)
                        .with_no_wrap()
                        .with_debug_name("main"));

    // ===== HEADER ROW =====
    auto header = hstack(context, mk(main.ent(), 0),
                      ComponentConfig{}
                          .with_size(ComponentSize{percent(1.0f), pixels(70)})
                          .with_background(Theme::Usage::Surface)
                          .with_padding(Padding{.left = pixels(16), .right = pixels(16),
                                                .top = pixels(8), .bottom = pixels(8)})
                          .with_no_wrap()
                          .with_justify_content(JustifyContent::SpaceBetween)
                          .with_align_items(AlignItems::Center)
                          .with_debug_name("header"));

    // Title container - expand to fill remaining space after button_row
    auto title_container =
        div(context, mk(header.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{expand(), pixels(50)})
                .with_background(Theme::Usage::Primary)
                .with_padding(Spacing::sm)
                .with_debug_name("title_container"));

    // Title text - use expand() to fill title_container after it's resolved
    div(context, mk(title_container.ent(), 0),
        ComponentConfig{}
            .with_label(sample.title)
            .with_size(ComponentSize{expand(), expand()})
            .with_font(font_config.font_name, 24.0f * font_config.size_scale)
            .with_alignment(TextAlignment::Left));

    // Button row container - needs enough width for 3 buttons
    // Height increased to accommodate full language names when enabled
    // Using children() instead of fixed pixels to avoid overflow in header
    auto button_row =
        hstack(context, mk(header.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{children(), pixels(54)})
                .with_no_wrap()
                .with_align_items(AlignItems::Center)
                .with_debug_name("button_row"));

    // Helper to build language button labels with optional hints and full names
    auto make_lang_label = [this](const char* code, const char* full_name, const char* key_hint) {
      std::string label = code;
      if (config.show_full_language_names) {
        label += std::string("\n") + full_name;
      }
      if (config.show_prominent_keyboard_hints) {
        label += std::string(" [") + key_hint + "]";
      }
      return label;
    };

    // Language buttons - EN
    if (button(context, mk(button_row.ent(), 0),
               ComponentConfig{}
                   .with_label(make_lang_label("EN", "English", "1"))
                   .with_size(ComponentSize{pixels(68), pixels(48)})
                   .with_font(UIComponent::DEFAULT_FONT, h720(config.show_full_language_names ? 13.0f : 16.0f))
                   .with_background(current_language == Language::English
                                        ? Theme::Usage::Primary
                                        : Theme::Usage::Secondary))) {
      current_language = Language::English;
    }

    // Language buttons - KO
    if (button(context, mk(button_row.ent(), 1),
               ComponentConfig{}
                   .with_label(make_lang_label("KO", "Korean", "2"))
                   .with_size(ComponentSize{pixels(68), pixels(48)})
                   .with_font(UIComponent::DEFAULT_FONT, h720(config.show_full_language_names ? 13.0f : 16.0f))
                   .with_background(current_language == Language::Korean
                                        ? Theme::Usage::Primary
                                        : Theme::Usage::Secondary))) {
      current_language = Language::Korean;
    }

    // Language buttons - JA
    if (button(context, mk(button_row.ent(), 2),
               ComponentConfig{}
                   .with_label(make_lang_label("JA", "Japanese", "3"))
                   .with_size(ComponentSize{pixels(68), pixels(48)})
                   .with_font(UIComponent::DEFAULT_FONT, h720(config.show_full_language_names ? 13.0f : 16.0f))
                   .with_background(current_language == Language::Japanese
                                        ? Theme::Usage::Primary
                                        : Theme::Usage::Secondary))) {
      current_language = Language::Japanese;
    }

    // ===== CONTENT ROW ===== - fixed height to enable child percent sizing
    // Main is ~554px. Header=70, Footer=55, so content = 554-70-55 = 429
    auto content = hstack(context, mk(main.ent(), 1),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.0f), h720(425)})
                           .with_no_wrap()
                           .with_debug_name("content"));

    // Left panel - current language demo
    auto left_panel =
        div(context, mk(content.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.49f), percent(1.0f)})
                .with_background(Theme::Usage::Surface)
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Column)
                .with_no_wrap()
                .with_margin(Margin{.right = pixels(4)})
                .with_debug_name("left_panel"));

    // Greeting - fill parent width
    div(context, mk(left_panel.ent(), 0),
        ComponentConfig{}
            .with_label(sample.greeting)
            .with_size(ComponentSize{percent(1.0f), pixels(42)})
            .with_background(Theme::Usage::Accent)
            .with_font(font_config.font_name, 26.0f * font_config.size_scale)
            .with_margin(Margin{.top = screen_pct(0.01f), .bottom = screen_pct(0.01f)}));

    // Menu items - 44px touch targets
    for (int i = 0; i < 4; i++) {
      button(context, mk(left_panel.ent(), i + 1),
             ComponentConfig{}
                 .with_label(sample.menu_items[i])
                 .with_size(ComponentSize{percent(1.0f), pixels(44)})
                 .with_margin(Margin{.top = screen_pct(0.01f), .bottom = screen_pct(0.01f)})
                 .with_flex_direction(FlexDirection::Row)
                 .with_font(font_config.font_name, scaled_size)
                 .with_background(Theme::Usage::Primary));
    }

    // Continue button - 44px touch target
    // Color is configurable: consistent (Primary) or distinct (Accent)
    button(context, mk(left_panel.ent(), 5),
           ComponentConfig{}
               .with_label(sample.button_text)
               .with_size(ComponentSize{percent(1.0f), pixels(44)})
               .with_margin(Margin{.top = screen_pct(0.01f), .bottom = screen_pct(0.01f)})
               .with_flex_direction(FlexDirection::Row)
               .with_font(font_config.font_name, 18.0f * font_config.size_scale)
               .with_background(config.use_consistent_button_colors
                                    ? Theme::Usage::Primary
                                    : Theme::Usage::Accent));

    // Right panel - all languages comparison
    auto right_panel =
        div(context, mk(content.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.49f), percent(1.0f)})
                .with_background(Theme::Usage::Surface)
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Column)
                .with_no_wrap()
                .with_margin(Margin{.left = pixels(4)})
                .with_debug_name("right_panel"));

    // Title for right panel
    div(context, mk(right_panel.ent(), 0),
        ComponentConfig{}
            .with_label("All Languages")
            .with_size(ComponentSize{percent(1.0f), pixels(38)})
            .with_background(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, h720(22.0f))
            .with_margin(Margin{.top = screen_pct(0.01f), .bottom = screen_pct(0.01f)}));

    // English sample
    auto en = get_sample(Language::English);
    div(context, mk(right_panel.ent(), 1),
        ComponentConfig{}
            .with_label("EN: " + en.greeting + " - " + en.menu_items[0])
            .with_size(ComponentSize{percent(1.0f), pixels(38)})
            .with_background(Theme::Usage::None)
            .with_font("Gaegu-Bold", h720(18.0f))
            .with_margin(Margin{.top = screen_pct(0.01f), .bottom = screen_pct(0.01f)}));

    // Korean sample
    auto ko = get_sample(Language::Korean);
    div(context, mk(right_panel.ent(), 2),
        ComponentConfig{}
            .with_label("KO: " + ko.greeting + " - " + ko.menu_items[0])
            .with_size(ComponentSize{percent(1.0f), pixels(38)})
            .with_background(Theme::Usage::None)
            .with_font("NotoSansKR", h720(18.0f))
            .with_margin(Margin{.top = screen_pct(0.01f), .bottom = screen_pct(0.01f)}));

    // Japanese sample
    auto ja = get_sample(Language::Japanese);
    div(context, mk(right_panel.ent(), 3),
        ComponentConfig{}
            .with_label("JA: " + ja.greeting + " - " + ja.menu_items[0])
            .with_size(ComponentSize{percent(1.0f), pixels(38)})
            .with_background(Theme::Usage::None)
            .with_font("Sazanami", h720(18.0f))
            .with_margin(Margin{.top = screen_pct(0.01f), .bottom = screen_pct(0.01f)}));

    // ===== FOOTER ROW =====
    auto footer = div(context, mk(main.ent(), 2),
                      ComponentConfig{}
                          .with_size(ComponentSize{percent(1.0f), pixels(55)})
                          .with_background(Theme::Usage::Surface)
                          .with_padding(Spacing::xs)
                          .with_flex_direction(FlexDirection::Column));

    std::string lang_name = current_language == Language::English  ? "ENGLISH"
                            : current_language == Language::Korean ? "KOREAN"
                                                                   : "JAPANESE";

    div(context, mk(footer.ent(), 0),
        ComponentConfig{}
            .with_label("Active: " + lang_name +
                        " | Font: " + font_config.font_name)
            .with_size(ComponentSize{percent(1.0f), pixels(28)})
            .with_background(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f)));

    div(context, mk(footer.ent(), 1),
        ComponentConfig{}
            .with_label("Press 1/2/3 to switch languages")
            .with_size(ComponentSize{percent(1.0f), pixels(24)})
            .with_background(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, h720(18.0f)));
  }
};

REGISTER_EXAMPLE_SCREEN(language_demo, "System Demos",
                        "Multi-language font support demo", LanguageDemoScreen)
