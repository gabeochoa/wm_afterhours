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

  struct LanguageSample {
    std::string title;
    std::string greeting;
    std::string menu_items[4];
    std::string button_text;
  };

  LanguageSample get_sample(Language lang) {
    switch (lang) {
    case Language::Korean:
      return LanguageSample{
          "언어 데모",
          "안녕하세요!",
          {"시작", "설정", "정보", "종료"},
          "계속하기"};
    case Language::Japanese:
      return LanguageSample{
          "言語デモ",
          "こんにちは！",
          {"開始", "設定", "情報", "終了"},
          "続ける"};
    case Language::English:
    default:
      return LanguageSample{
          "Language Demo",
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

    // Full screen background (no padding so it stays in bounds)
    auto background =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
                .with_custom_background(theme.background)
                .with_debug_name("main_bg"));

    // Content container with padding
    auto main =
        div(context, mk(background.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("main"));

    // ===== HEADER ROW (80px) =====
    auto header =
        div(context, mk(main.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(1000), pixels(80)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Row)
                .with_margin(Spacing::xs)
                .with_debug_name("header"));

    // Title
    div(context, mk(header.ent(), 0),
        ComponentConfig{}
            .with_label(sample.title)
            .with_size(ComponentSize{pixels(280), pixels(60)})
            .with_background(Theme::Usage::Primary)
            .with_font(font_config.font_name, 32.0f * font_config.size_scale)
            .with_debug_name("title"));

    // Spacer
    div(context, mk(header.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(340), pixels(10)})
            .with_debug_name("spacer"));

    // Language buttons
    if (button(context, mk(header.ent(), 2),
               ComponentConfig{}
                   .with_label("EN")
                   .with_size(ComponentSize{pixels(65), pixels(45)})
                   .with_margin(Spacing::xs)
                   .with_font(UIComponent::DEFAULT_FONT, 20.0f)
                   .with_background(current_language == Language::English
                                         ? Theme::Usage::Primary
                                         : Theme::Usage::Secondary))) {
      current_language = Language::English;
    }

    if (button(context, mk(header.ent(), 3),
               ComponentConfig{}
                   .with_label("KO")
                   .with_size(ComponentSize{pixels(65), pixels(45)})
                   .with_margin(Spacing::xs)
                   .with_font(UIComponent::DEFAULT_FONT, 20.0f)
                   .with_background(current_language == Language::Korean
                                         ? Theme::Usage::Primary
                                         : Theme::Usage::Secondary))) {
      current_language = Language::Korean;
    }

    if (button(context, mk(header.ent(), 4),
               ComponentConfig{}
                   .with_label("JA")
                   .with_size(ComponentSize{pixels(65), pixels(45)})
                   .with_margin(Spacing::xs)
                   .with_font(UIComponent::DEFAULT_FONT, 20.0f)
                   .with_background(current_language == Language::Japanese
                                         ? Theme::Usage::Primary
                                         : Theme::Usage::Secondary))) {
      current_language = Language::Japanese;
    }

    // ===== CONTENT ROW (480px) =====
    auto content =
        div(context, mk(main.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{percent(1.0f), pixels(480)})
                .with_flex_direction(FlexDirection::Row)
                .with_justify_content(JustifyContent::Center)
                .with_margin(Spacing::xs)
                .with_debug_name("content"));

    // Left panel - current language demo
    auto left_panel =
        div(context, mk(content.ent(), 0),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(400), pixels(450)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Column)
                .with_margin(Spacing::sm)
                .with_debug_name("left_panel"));

    // Greeting
    div(context, mk(left_panel.ent(), 0),
        ComponentConfig{}
            .with_label(sample.greeting)
            .with_size(ComponentSize{pixels(350), pixels(50)})
            .with_background(Theme::Usage::Accent)
            .with_font(font_config.font_name, 32.0f * font_config.size_scale)
            .with_margin(Spacing::xs)
            .with_debug_name("greeting"));

    // Menu items
    for (int i = 0; i < 4; i++) {
      button(context, mk(left_panel.ent(), i + 1),
             ComponentConfig{}
                 .with_label(sample.menu_items[i])
                 .with_size(ComponentSize{pixels(200), pixels(40)})
                 .with_margin(Spacing::xs)
                 .with_font(font_config.font_name, scaled_size)
                 .with_background(Theme::Usage::Primary));
    }

    // Continue button
    button(context, mk(left_panel.ent(), 5),
           ComponentConfig{}
               .with_label(sample.button_text)
               .with_size(ComponentSize{pixels(200), pixels(42)})
               .with_margin(Spacing::sm)
               .with_font(font_config.font_name, 20.0f * font_config.size_scale)
               .with_background(Theme::Usage::Accent));

    // Right panel - all languages comparison
    auto right_panel =
        div(context, mk(content.ent(), 1),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(400), pixels(450)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::sm)
                .with_flex_direction(FlexDirection::Column)
                .with_margin(Spacing::sm)
                .with_debug_name("right_panel"));

    // Title for right panel
    div(context, mk(right_panel.ent(), 0),
        ComponentConfig{}
            .with_label("All Languages")
            .with_size(ComponentSize{pixels(350), pixels(42)})
            .with_custom_background(theme.surface)
            .with_font(UIComponent::DEFAULT_FONT, 24.0f)
            .with_margin(Spacing::xs)
            .with_debug_name("all_lang_title"));

    // English sample
    auto en = get_sample(Language::English);
    div(context, mk(right_panel.ent(), 1),
        ComponentConfig{}
            .with_label("EN: " + en.greeting + " - " + en.menu_items[0])
            .with_size(ComponentSize{pixels(350), pixels(42)})
            .with_custom_background(theme.surface)
            .with_font("Gaegu-Bold", 20.0f)
            .with_margin(Spacing::xs)
            .with_debug_name("en_sample"));

    // Korean sample
    auto ko = get_sample(Language::Korean);
    div(context, mk(right_panel.ent(), 2),
        ComponentConfig{}
            .with_label("KO: " + ko.greeting + " - " + ko.menu_items[0])
            .with_size(ComponentSize{pixels(350), pixels(42)})
            .with_custom_background(theme.surface)
            .with_font("NotoSansKR", 20.0f)
            .with_margin(Spacing::xs)
            .with_debug_name("ko_sample"));

    // Japanese sample
    auto ja = get_sample(Language::Japanese);
    div(context, mk(right_panel.ent(), 3),
        ComponentConfig{}
            .with_label("JA: " + ja.greeting + " - " + ja.menu_items[0])
            .with_size(ComponentSize{pixels(350), pixels(42)})
            .with_custom_background(theme.surface)
            .with_font("Sazanami", 20.0f)
            .with_margin(Spacing::xs)
            .with_debug_name("ja_sample"));

    // ===== FOOTER ROW (60px) =====
    auto footer =
        div(context, mk(main.ent(), 2),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(1000), pixels(60)})
                .with_custom_background(theme.surface)
                .with_padding(Spacing::xs)
                .with_flex_direction(FlexDirection::Column)
                .with_margin(Spacing::xs)
                .with_debug_name("footer"));

    std::string lang_name = current_language == Language::English  ? "ENGLISH"
                            : current_language == Language::Korean ? "KOREAN"
                                                                   : "JAPANESE";

    div(context, mk(footer.ent(), 0),
        ComponentConfig{}
            .with_label("Active: " + lang_name + " | Font: " +
                        font_config.font_name)
            .with_size(ComponentSize{pixels(500), pixels(26)})
            .with_custom_background(theme.surface)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_debug_name("lang_indicator"));

    div(context, mk(footer.ent(), 1),
        ComponentConfig{}
            .with_label("Press 1/2/3 to switch languages")
            .with_size(ComponentSize{pixels(350), pixels(20)})
            .with_custom_background(theme.surface)
            .with_font(UIComponent::DEFAULT_FONT, 20.0f)
            .with_debug_name("instructions"));
  }
};

REGISTER_EXAMPLE_SCREEN(language_demo, "System Demos",
                        "Multi-language font support demo", LanguageDemoScreen)
