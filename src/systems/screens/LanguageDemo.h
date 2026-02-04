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
    bool show_full_language_names = true;  // Display "English", "Korean", "Japanese" below codes

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
                .with_custom_background(theme.background)
                .with_roundness(0.05f)
                .with_padding(Spacing::md)
                .with_debug_name("main_bg"));

    // Content container
    auto main = div(context, mk(root.ent(), 0),
                    ComponentConfig{}
                        .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                        .with_flex_direction(FlexDirection::Column)
                        .with_no_wrap()
                        .with_debug_name("main"));

    // ===== HEADER ROW =====
    auto header = div(context, mk(main.ent(), 0),
                      ComponentConfig{}
                          .with_size(ComponentSize{percent(1.0f), pixels(70)})
                          .with_custom_background(theme.surface)
                          .with_padding(Padding{.left = pixels(16), .right = pixels(16),
                                                .top = pixels(8), .bottom = pixels(8)})
                          .with_flex_direction(FlexDirection::Row)
                          .with_no_wrap()
                          .with_justify_content(JustifyContent::SpaceBetween)
                          .with_align_items(AlignItems::Center)
                          .with_debug_name("header"));

    // Title container
    auto title_container =
        div(context, mk(header.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(280), pixels(50)})
                .with_background(Theme::Usage::Primary)
                .with_padding(Spacing::sm)
                .with_debug_name("title_container"));

    // Title text
    div(context, mk(title_container.ent(), 0),
        ComponentConfig{}
            .with_label(sample.title)
            .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
            .with_font(font_config.font_name, 24.0f * font_config.size_scale)
            .with_alignment(TextAlignment::Left)
            .with_debug_name("title_text"));

    // Button row container - needs enough width for 3 buttons
    // Height increased to accommodate full language names when enabled
    auto button_row =
        div(context, mk(header.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(300), pixels(54)})
                .with_flex_direction(FlexDirection::Row)
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
                   .with_size(ComponentSize{pixels(80), pixels(48)})
                   .with_margin(Margin{.left = pixels(4), .right = pixels(4)})
                   .with_font(UIComponent::DEFAULT_FONT, config.show_full_language_names ? 14.0f : 18.0f)
                   .with_background(current_language == Language::English
                                        ? Theme::Usage::Primary
                                        : Theme::Usage::Secondary))) {
      current_language = Language::English;
    }

    // Language buttons - KO
    if (button(context, mk(button_row.ent(), 1),
               ComponentConfig{}
                   .with_label(make_lang_label("KO", "Korean", "2"))
                   .with_size(ComponentSize{pixels(80), pixels(48)})
                   .with_margin(Margin{.left = pixels(4), .right = pixels(4)})
                   .with_font(UIComponent::DEFAULT_FONT, config.show_full_language_names ? 14.0f : 18.0f)
                   .with_background(current_language == Language::Korean
                                        ? Theme::Usage::Primary
                                        : Theme::Usage::Secondary))) {
      current_language = Language::Korean;
    }

    // Language buttons - JA
    if (button(context, mk(button_row.ent(), 2),
               ComponentConfig{}
                   .with_label(make_lang_label("JA", "Japanese", "3"))
                   .with_size(ComponentSize{pixels(80), pixels(48)})
                   .with_margin(Margin{.left = pixels(4), .right = pixels(4)})
                   .with_font(UIComponent::DEFAULT_FONT, config.show_full_language_names ? 14.0f : 18.0f)
                   .with_background(current_language == Language::Japanese
                                        ? Theme::Usage::Primary
                                        : Theme::Usage::Secondary))) {
      current_language = Language::Japanese;
    }

    // ===== CONTENT ROW ===== - adjusted height to fit within parent
    auto content = div(context, mk(main.ent(), 1),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.0f), pixels(480)})
                           .with_flex_direction(FlexDirection::Row)
                           .with_no_wrap()
                           .with_justify_content(JustifyContent::Center)
                           .with_debug_name("content"));

    // Left panel - current language demo - expanded width
    auto left_panel =
        div(context, mk(content.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.50f), percent(1.0f)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::md)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("left_panel"));

    // Greeting
    div(context, mk(left_panel.ent(), 0),
        ComponentConfig{}
            .with_label(sample.greeting)
            .with_size(ComponentSize{percent(0.9f), pixels(42)})
            .with_background(Theme::Usage::Accent)
            .with_font(font_config.font_name, 26.0f * font_config.size_scale)
            .with_margin(Spacing::xs)
            .with_debug_name("greeting"));

    // Menu items - 44px touch targets
    for (int i = 0; i < 4; i++) {
      button(context, mk(left_panel.ent(), i + 1),
             ComponentConfig{}
                 .with_label(sample.menu_items[i])
                 .with_size(ComponentSize{pixels(180), pixels(44)})
                 .with_margin(Spacing::xs)
                 .with_font(font_config.font_name, scaled_size)
                 .with_background(Theme::Usage::Primary));
    }

    // Continue button - 44px touch target
    // Color is configurable: consistent (Primary) or distinct (Accent)
    button(context, mk(left_panel.ent(), 5),
           ComponentConfig{}
               .with_label(sample.button_text)
               .with_size(ComponentSize{pixels(180), pixels(44)})
               .with_font(font_config.font_name, 18.0f * font_config.size_scale)
               .with_background(config.use_consistent_button_colors
                                    ? Theme::Usage::Primary
                                    : Theme::Usage::Accent));

    // Right panel - all languages comparison - expanded width
    auto right_panel =
        div(context, mk(content.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(0.48f), percent(1.0f)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::md)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("right_panel"));

    // Title for right panel
    div(context, mk(right_panel.ent(), 0),
        ComponentConfig{}
            .with_label("All Languages")
            .with_size(ComponentSize{percent(0.9f), pixels(38)})
            .with_background(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, 22.0f)
            .with_margin(Spacing::xs)
            .with_debug_name("all_lang_title"));

    // English sample
    auto en = get_sample(Language::English);
    div(context, mk(right_panel.ent(), 1),
        ComponentConfig{}
            .with_label("EN: " + en.greeting + " - " + en.menu_items[0])
            .with_size(ComponentSize{percent(0.9f), pixels(38)})
            .with_background(Theme::Usage::None)
            .with_font("Gaegu-Bold", 18.0f)
            .with_margin(Spacing::xs)
            .with_debug_name("en_sample"));

    // Korean sample
    auto ko = get_sample(Language::Korean);
    div(context, mk(right_panel.ent(), 2),
        ComponentConfig{}
            .with_label("KO: " + ko.greeting + " - " + ko.menu_items[0])
            .with_size(ComponentSize{percent(0.9f), pixels(38)})
            .with_background(Theme::Usage::None)
            .with_font("NotoSansKR", 18.0f)
            .with_margin(Spacing::xs)
            .with_debug_name("ko_sample"));

    // Japanese sample
    auto ja = get_sample(Language::Japanese);
    div(context, mk(right_panel.ent(), 3),
        ComponentConfig{}
            .with_label("JA: " + ja.greeting + " - " + ja.menu_items[0])
            .with_size(ComponentSize{percent(0.9f), pixels(38)})
            .with_background(Theme::Usage::None)
            .with_font("Sazanami", 18.0f)
            .with_margin(Spacing::xs)
            .with_debug_name("ja_sample"));

    // ===== FOOTER ROW =====
    auto footer = div(context, mk(main.ent(), 2),
                      ComponentConfig{}
                          .with_size(ComponentSize{percent(1.0f), pixels(55)})
                          .with_custom_background(theme.surface)
                          .with_padding(Spacing::xs)
                          .with_flex_direction(FlexDirection::Column)
                          .with_debug_name("footer"));

    std::string lang_name = current_language == Language::English  ? "ENGLISH"
                            : current_language == Language::Korean ? "KOREAN"
                                                                   : "JAPANESE";

    div(context, mk(footer.ent(), 0),
        ComponentConfig{}
            .with_label("Active: " + lang_name +
                        " | Font: " + font_config.font_name)
            .with_size(ComponentSize{percent(1.0f), pixels(28)})
            .with_background(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_debug_name("lang_indicator"));

    div(context, mk(footer.ent(), 1),
        ComponentConfig{}
            .with_label("Press 1/2/3 to switch languages")
            .with_size(ComponentSize{percent(1.0f), pixels(24)})
            .with_background(Theme::Usage::None)
            .with_font(UIComponent::DEFAULT_FONT, 18.0f)
            .with_debug_name("instructions"));
  }
};

REGISTER_EXAMPLE_SCREEN(language_demo, "System Demos",
                        "Multi-language font support demo", LanguageDemoScreen)
