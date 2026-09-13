#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <string>
#include <vector>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct LocalizedInputPrompts : ScreenSystem<UIContext<InputAction>> {
  bool compact = false;
  int jumps = 0;
  float jump_time = 0;

  struct Translation {
    const char *language;
    const char *sentence;
    const char *name;
  };

  const std::array<Translation, 2> translations{{
      {"English", "Press {space} to jump over the gap and reach the next platform.", "en"},
      {"Korean / 한국어", "{space} 키를 누르면 점프합니다. 틈을 넘어 다음 발판에 도착하세요.", "ko"},
  }};

  static std::vector<std::string> words(const std::string &sentence) {
    std::vector<std::string> result;
    size_t begin = 0;
    while (begin < sentence.size()) {
      const size_t end = sentence.find(' ', begin);
      result.push_back(sentence.substr(begin, end - begin));
      if (end == std::string::npos) break;
      begin = end + 1;
    }
    return result;
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float dt) override {
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const auto ink = afterhours::Color{230, 239, 249, 255};
    const auto muted = afterhours::Color{168, 187, 210, 255};
    const auto accent = afterhours::Color{135, 212, 197, 255};
    Theme theme;
    theme.background = {15, 22, 33, 255};
    theme.font = ink;
    theme.font_muted = muted;
    theme.primary = accent;
    context.set_theme(theme);
    UIStylingDefaults::get().set_grid_snapping(false);
    UIStylingDefaults::get().set_default_font("NotoSansKRInline", pixels(20 * s));
    jump_time = std::max(0.f, jump_time - dt);
    if (afterhours::input::is_key_pressed(afterhours::keys::SPACE)) {
      ++jumps;
      jump_time = .6f;
    }
    auto canvas = div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background(theme.background).with_corner_radius(0));
    const auto box = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_corner_radius(0)
          .with_transparent_bg().with_skip_grid_snap(true);
    };
    auto root = div(context, mk(canvas.ent(), 0), box(0, 0, 1280, 720)
        .with_absolute_position((context.screen_width - 1280 * s) / 2,
                                (context.screen_height - 720 * s) / 2)
        .with_debug_name("lp_root"));
    const auto label = [&](int id, const std::string &value, float x, float y,
                           float w, float h, float size, afterhours::Color color,
                           const std::string &name = "") {
      return div(context, mk(root.ent(), id), box(x, y, w, h).with_label(value)
          .with_font("NotoSansKRInline", pixels(size * s)).with_custom_text_color(color)
          .with_text_inset(0, 0).with_alignment(TextAlignment::Left)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    label(1, "Words move. Actions stay the same.", 56, 30, 1150, 52, 34, ink);
    label(2, "Keyboard prompts in English and Korean", 56, 90, 920, 34, 22, muted);
    label(3, "One Space key, two natural sentence orders. Narrow the cards to see both prompts wrap.",
          56, 135, 1150, 30, 18, muted);
    if (button(context, mk(root.ent(), 4), box(988, 88, 236, 42)
        .with_label(compact ? "Use wide cards" : "Use narrow cards")
        .with_font("NotoSansKRInline", pixels(18 * s)).with_custom_background({39, 63, 80, 255})
        .with_custom_text_color(ink).with_corner_radius(8 * s).with_debug_name("lp_width"))) compact = !compact;
    auto *fonts = afterhours::EntityHelper::get_singleton_cmp<FontManager>();
    const auto font = fonts->get_font("NotoSansKRInline");
    const float width = compact ? 280.f : 512.f;
    for (size_t i = 0; i < translations.size(); ++i) {
      const auto &translation = translations[i];
      const float x = 56 + static_cast<float>(i) * 600;
      const int id = 10 + static_cast<int>(i) * 10;
      div(context, mk(root.ent(), id), box(x, 202, 568, 290)
          .with_custom_background({25, 36, 52, 255}).with_corner_radius(12 * s));
      label(id + 1, translation.language, x + 28, 222, 512, 36, 24, accent);
      auto sentence = hstack(context, mk(root.ent(), id + 2), box(x + 28, 280, width, 196)
          .with_wrap().with_align_items(AlignItems::FlexStart).with_gap(pixels(8 * s))
          .with_debug_name(std::string("lp_sentence_") + translation.name));
      const auto tokens = words(translation.sentence);
      for (size_t token = 0; token < tokens.size(); ++token) {
        const bool key = tokens[token] == "{space}";
        const std::string value = key ? "Space" : tokens[token];
        const float token_width = key ? 104 * s : std::ceil(afterhours::measure_text(font, value.c_str(), 24 * s, 1.f).x);
        auto cfg = ComponentConfig{}.with_size({pixels(token_width), pixels(44 * s)})
            .with_label(value).with_font("NotoSansKRInline", pixels(24 * s))
            .with_letter_spacing(0).with_text_inset(0, 0)
            .with_custom_text_color(key ? theme.background : ink)
            .with_alignment(key ? TextAlignment::Center : TextAlignment::Left)
            .with_ignore_pointer_events().with_skip_grid_snap(true)
            .with_debug_name(std::string("lp_") + translation.name + (key ? "_key" : "_word_" + std::to_string(token)));
        if (key) cfg.with_custom_background(accent).with_corner_radius(6 * s);
        else cfg.with_transparent_bg().with_corner_radius(0);
        div(context, mk(sentence.ent(), static_cast<int>(token)), cfg);
      }
    }
    label(40, "Try the action", 56, 524, 500, 36, 24, ink);
    label(41, "Press Space to jump.", 56, 572, 500, 32, 20, muted);
    label(42, "Jumps: " + std::to_string(jumps), 56, 622, 500, 36, 22, accent, "lp_jumps");
    div(context, mk(root.ent(), 43), box(656, 536, 568, 130)
        .with_custom_background({25, 36, 52, 255}).with_corner_radius(12 * s)
        .with_on_draw_fg([jump = jump_time, s, accent](RectangleType r) {
          const float phase = 1.f - jump / .6f;
          const float rise = jump > 0 ? 4 * phase * (1 - phase) * 60 * s : 0;
          afterhours::draw_rectangle({r.x + 32 * s, r.y + 100 * s, r.width - 64 * s, 3 * s}, {111, 139, 166, 255});
          afterhours::draw_rectangle({r.x + r.width / 2 - 16 * s, r.y + 68 * s - rise, 32 * s, 32 * s}, accent);
        }).with_debug_name("lp_action_preview"));
  }
};

REGISTER_EXAMPLE_SCREEN(localized_input_prompts, "System Demos",
                        "Inline keyboard prompts with translated word order", LocalizedInputPrompts)
