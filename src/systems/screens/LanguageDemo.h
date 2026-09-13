#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <array>
#include <afterhours/src/plugins/translation.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;
using namespace afterhours::translation;

struct LanguageDemoScreen : ScreenSystem<UIContext<InputAction>> {
  Language current_language = Language::English;

  std::optional<size_t> last_action;
  bool compact_preview = false;

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
    if (afterhours::input::is_key_pressed(raylib::KEY_ONE) || afterhours::input::is_key_pressed(raylib::KEY_KP_1)) current_language = Language::English;
    if (afterhours::input::is_key_pressed(raylib::KEY_TWO) || afterhours::input::is_key_pressed(raylib::KEY_KP_2)) current_language = Language::Korean;
    if (afterhours::input::is_key_pressed(raylib::KEY_THREE) || afterhours::input::is_key_pressed(raylib::KEY_KP_3)) current_language = Language::Japanese;
    auto theme = afterhours::ui::theme_presets::ocean_navy();
    theme.language_fonts[Language::English] = FontConfig("AtkinsonMock", 1.0f);
    theme.language_fonts[Language::Korean] = FontConfig("NotoSansKR", 1.35f);
    theme.language_fonts[Language::Japanese] = FontConfig("NotoSansJP", 1.15f);
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const std::array<Language, 3> languages{Language::English, Language::Korean, Language::Japanese};
    const std::array<const char *, 3> names{"English", "한국어", "日本語"};
    const std::array<const char *, 3> english_names{"English", "Korean", "Japanese"};
    const std::array<const char *, 3> paragraphs{
        "Choose a language.\nChanges apply only to this demo.",
        "언어를 선택하세요.\n설정은 이 데모에만 적용됩니다.",
        "言語を選んでください。\n設定はこのデモにのみ適用されます。"};
    const auto box = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_corner_radius(0);
    };
    div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_absolute_position(0, 0).with_background(Theme::Usage::Background).with_corner_radius(0));
    auto root = div(context, mk(entity, 1), ComponentConfig{}
        .with_size({pixels(1160 * s), pixels(672 * s)})
        .with_absolute_position((context.screen_width - 1160 * s) / 2,
                                (context.screen_height - 672 * s) / 2)
        .with_background(Theme::Usage::None).with_debug_name("main_bg"));
    const auto label = [&](int id, const std::string &value, float x, float y,
                           float w, float h, float size, const std::string &font = "AtkinsonMock") {
      return div(context, mk(root.ent(), id), box(x, y, w, h).with_label(value)
          .with_font(font, pixels(size * s)).with_custom_text_color({224, 235, 245, 255})
          .with_background(Theme::Usage::None).with_alignment(TextAlignment::Left)
          .with_ignore_pointer_events());
    };
    const auto &heading_font = theme.get_font_config(current_language);
    label(0, get_sample(current_language).title, 0, 0, 1160, 42, 28 * heading_font.size_scale, heading_font.font_name);
    label(1, "Resize the menu preview and compare translated labels with their action icons.", 0, 46, 1160, 28, 20);
    if (button(context, mk(root.ent(), 90), box(928, 0, 232, 40)
        .with_label(compact_preview ? "Widen preview" : "Narrow preview")
        .with_font("AtkinsonMock", pixels(18 * s))
        .with_background(Theme::Usage::Secondary)
        .with_custom_text_color({235, 242, 250, 255}).with_corner_radius(6 * s)
        .with_debug_name("language_preview_width")))
      compact_preview = !compact_preview;
    for (size_t i = 0; i < languages.size(); ++i) {
      const auto lang = languages[i];
      const auto &font = theme.get_font_config(lang);
      const bool active = lang == current_language;
      auto picker = button(context, mk(root.ent(), 10 + static_cast<int>(i)), box(static_cast<float>(i) * 392, 86, 376, 50)
          .with_padding(Padding{}).with_label(names[i]).with_font(font.font_name, pixels(22 * font.size_scale * s))
          .with_background(active ? Theme::Usage::Primary : Theme::Usage::Secondary)
          .with_custom_text_color({235, 242, 250, 255}).with_corner_radius(8 * s)
          .with_border(active ? afterhours::Color{132, 220, 224, 255} : afterhours::Color{62, 91, 114, 255}, 1)
          .with_debug_name("language_" + std::to_string(i)));
      if (picker) current_language = lang;
      div(context, mk(root.ent(), 70 + static_cast<int>(i)), box(static_cast<float>(i) * 392 + 12, 96, 30, 30)
          .with_label(std::to_string(i + 1)).with_font("AtkinsonMock", pixels(18 * s))
          .with_alignment(TextAlignment::Center).with_custom_background({19, 37, 55, 255})
          .with_custom_text_color({224, 235, 245, 255}).with_corner_radius(4 * s)
          .with_ignore_pointer_events());
      if (active) div(context, mk(root.ent(), 80 + static_cast<int>(i)), box(static_cast<float>(i) * 392 + 300, 96, 64, 30)
          .with_label("Active").with_font("AtkinsonMock", pixels(15 * s))
          .with_background(Theme::Usage::None).with_custom_text_color({151, 235, 218, 255})
          .with_ignore_pointer_events());
    }
    const size_t selected = current_language == Language::English ? 0 : current_language == Language::Korean ? 1 : 2;
    const auto &font = theme.get_font_config(current_language);
    const auto sample = get_sample(current_language);
    const float preview_width = compact_preview ? 300.f : 444.f;
    const float control_width = preview_width - 40;
    div(context, mk(root.ent(), 20), box(0, 158, preview_width, 422)
        .with_background(Theme::Usage::Surface).with_corner_radius(12 * s)
        .with_debug_name("language_preview"));
    div(context, mk(root.ent(), 21), box(464, 158, 696, 422).with_background(Theme::Usage::Surface).with_corner_radius(12 * s));
    label(22, std::string("Current language: ") + english_names[selected], 20, 176, control_width, 28, 19);
    label(23, sample.greeting, 20, 210, control_width, 54, 32 * font.size_scale, font.font_name);
    label(24, "Localized menu controls", 20, 276, control_width, 24, 17);
    auto *fonts = afterhours::EntityHelper::get_singleton_cmp<FontManager>();
    const auto action = [&](int id, size_t index, const std::string &value,
                            float y, float height, const std::string &name) {
      auto control = button(context, mk(root.ent(), id), box(20, y, control_width, height)
          .with_label(value).with_font(font.font_name, pixels(22 * font.size_scale * s))
          .with_background(index == 3 ? Theme::Usage::Secondary : Theme::Usage::Primary)
          .with_custom_text_color({235, 242, 250, 255}).with_corner_radius(6 * s)
          .with_debug_name(name));
      control.ent().get<HasLabel>().text_x_offset = 14 * s;
      const float text_width = afterhours::measure_text(fonts->get_font(font.font_name),
          value.c_str(), 22 * font.size_scale * s, 1.f).x;
      div(context, mk(root.ent(), id + 100), box(20 + (control_width - text_width / s - 28) / 2,
          y + (height - 20) / 2, 20, 20)
          .with_background(Theme::Usage::None).with_ignore_pointer_events()
          .with_debug_name("language_icon_" + std::to_string(index))
          .with_on_draw_fg([index, s](RectangleType r) {
            const auto ink = afterhours::Color{235, 242, 250, 255};
            const float x = r.x, y = r.y;
            if (index == 0) {
              raylib::DrawTriangle({x + 4 * s, y + 2 * s}, {x + 4 * s, y + 18 * s},
                                   {x + 17 * s, y + 10 * s}, ink);
              return;
            }
            if (index == 1) {
              for (int row = 0; row < 3; ++row) {
                const float cy = y + (4 + row * 6) * s;
                raylib::DrawLineEx({x + 2 * s, cy}, {x + 18 * s, cy}, 2 * s, ink);
                raylib::DrawCircleV({x + (row == 1 ? 13 : 7) * s, cy}, 3 * s, ink);
              }
              return;
            }
            if (index == 2) {
              raylib::DrawRing({x + 10 * s, y + 10 * s}, 8 * s, 9.5f * s, 0, 360, 24, ink);
              raylib::DrawCircleV({x + 10 * s, y + 6 * s}, 1.3f * s, ink);
              raylib::DrawLineEx({x + 10 * s, y + 9 * s}, {x + 10 * s, y + 15 * s}, 2 * s, ink);
              return;
            }
            if (index == 3)
              raylib::DrawRectangleLinesEx({x + s, y + 2 * s, 10 * s, 16 * s}, 1.5f * s, ink);
            raylib::DrawLineEx({x + 5 * s, y + 10 * s}, {x + 18 * s, y + 10 * s}, 2 * s, ink);
            raylib::DrawLineEx({x + 13 * s, y + 5 * s}, {x + 18 * s, y + 10 * s}, 2 * s, ink);
            raylib::DrawLineEx({x + 13 * s, y + 15 * s}, {x + 18 * s, y + 10 * s}, 2 * s, ink);
          }));
      if (control) last_action = index;
    };
    for (size_t i = 0; i < 4; ++i)
      action(30 + static_cast<int>(i), i, sample.menu_items[i],
             i == 3 ? 518.f : 310 + static_cast<float>(i) * 46, 40,
             "language_action_" + std::to_string(i));
    action(35, 4, sample.button_text, 462, 44, "language_continue");
    label(40, "All Languages", 484, 176, 656, 30, 25);
    label(41, "Shared greeting and Start / 22px with script-specific scaling", 484, 210, 656, 26, 17);
    label(42, "Language / font", 484, 246, 224, 24, 17);
    label(43, "Greeting", 714, 246, 264, 24, 17);
    label(44, "Start", 998, 246, 140, 24, 17);
    for (size_t i = 0; i < languages.size(); ++i) {
      const auto &row_font = theme.get_font_config(languages[i]);
      const auto row_sample = get_sample(languages[i]);
      const float top = 282 + static_cast<float>(i) * 66;
      const int id = 50 + static_cast<int>(i) * 4;
      label(id, english_names[i], 484, top, 224, 28, 20);
      label(id + 1, row_font.font_name, 484, top + 28, 224, 22, 16);
      label(id + 2, row_sample.greeting, 714, top, 264, 40, 22 * row_font.size_scale, row_font.font_name);
      label(id + 3, row_sample.menu_items[0], 998, top, 140, 40, 22 * row_font.size_scale, row_font.font_name);
    }
    label(64, paragraphs[selected], 484, 490, 656, 72, 20 * font.size_scale, font.font_name)
        .ent().get<HasLabel>().text_overflow = TextOverflow::Wrap;
    const std::string feedback = last_action
        ? "Demo action: " + (*last_action == 4 ? sample.button_text : sample.menu_items[*last_action])
        : "Choose a language, then try a localized control.";
    label(65, feedback, 0, 594, 1160, 32, 21 * font.size_scale, font.font_name)
        .ent().addComponentIfMissing<UIComponentDebug>("language_feedback").set("language_feedback");
    label(66, "Language: " + std::string(english_names[selected]) + " / Active font: " + font.font_name,
          0, 632, 660, 28, 18);
    label(67, "1 English / 2 Korean / 3 Japanese", 700, 632, 460, 28, 18);
  }

};

REGISTER_EXAMPLE_SCREEN(language_demo, "System Demos",
                        "Multi-language font support demo", LanguageDemoScreen)
