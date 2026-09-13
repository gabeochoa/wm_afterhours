#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <algorithm>
#include <array>
#include <cmath>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct PodcastLibrary : ScreenSystem<UIContext<InputAction>> {
  struct Episode {
    const char *title;
    const char *description;
    const char *date;
    int seconds;
  };
  static constexpr std::array<Episode, 4> episodes{{
      {"The city before sunrise", "A walk with the people who wake a city.", "Sep 12", 1104},
      {"Objects worth keeping", "The stories we carry in ordinary things.", "Sep 5", 1448},
      {"A room of your own", "Making space for a little quiet.", "Aug 29", 1902},
      {"Taking the long way home", "What changes when we stop rushing.", "Aug 22", 975},
  }};
  static constexpr std::array<float, 3> speeds{1.f, 1.5f, 2.f};
  size_t selected = 0;
  size_t speed_index = 0;
  float position = 0;
  bool playing = false;

  static std::string timestamp(float seconds) {
    const int total = static_cast<int>(seconds);
    return fmt::format("{:02}:{:02}", total / 60, total % 60);
  }

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context,
                     float dt) override {
    if (playing) {
      position = std::min(static_cast<float>(episodes[selected].seconds),
                          position + std::max(0.f, dt) * speeds[speed_index]);
      if (position >= episodes[selected].seconds) playing = false;
    }
    Theme theme;
    theme.background = {17, 25, 29, 255};
    theme.surface = {27, 38, 43, 255};
    theme.primary = {209, 174, 112, 255};
    theme.secondary = {46, 61, 65, 255};
    theme.font = {241, 238, 229, 255};
    theme.font_muted = {175, 192, 192, 255};
    theme.accent = {209, 174, 112, 255};
    theme.focus = {157, 221, 214, 255};
    context.set_theme(theme);
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const auto box = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_corner_radius(0)
          .with_background(Theme::Usage::None);
    };
    div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_absolute_position(0, 0).with_background(Theme::Usage::Background)
        .with_corner_radius(0));
    auto root = div(context, mk(entity, 1), box(0, 0, 1280, 720)
        .with_absolute_position((context.screen_width - 1280 * s) / 2,
                                (context.screen_height - 720 * s) / 2)
        .with_debug_name("podcast_root"));
    const auto text = [&](int id, const std::string &value, float x, float y,
                          float w, float h, float size, afterhours::Color color,
                          const std::string &name = "", bool bold = false) {
      return div(context, mk(root.ent(), id), box(x, y, w, h).with_label(value)
          .with_font(bold ? "AtkinsonMockBold" : "AtkinsonMock", pixels(size * s))
          .with_custom_text_color(color).with_alignment(TextAlignment::Left)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    const auto action = [&](int id, const std::string &value, float x, float y,
                            float w, float h, const std::string &name, bool primary = false) {
      return button(context, mk(root.ent(), id), box(x, y, w, h).with_label(value)
          .with_font("AtkinsonMock", pixels(20 * s)).with_corner_radius(9 * s)
          .with_custom_background(primary ? theme.primary : theme.secondary)
          .with_custom_text_color(primary ? theme.background : theme.font)
          .with_debug_name(name));
    };
    text(2, "Signal", 48, 24, 520, 46, 36, theme.font, "podcast_title", true);
    text(3, "Your podcast library", 48, 77, 750, 30, 22, theme.font_muted);
    text(4, "A little room to listen.", 892, 40, 340, 30, 21, theme.primary);
    div(context, mk(root.ent(), 5), box(48, 132, 330, 406)
        .with_background(Theme::Usage::Surface).with_corner_radius(14 * s));
    div(context, mk(root.ent(), 6), box(68, 152, 290, 222)
        .with_custom_background(theme.primary).with_corner_radius(8 * s)
        .with_on_draw_fg([s](RectangleType r) {
          const afterhours::Color dark{36, 61, 64, 255};
          raylib::DrawCircleV({r.x + 214 * s, r.y + 70 * s}, 43 * s, dark);
          for (int line = 0; line < 7; ++line) {
            const float y = r.y + (143 + line * 10) * s;
            raylib::DrawLineEx({r.x + 24 * s, y}, {r.x + 265 * s, y}, 2 * s, dark);
          }
        }).with_debug_name("podcast_cover"));
    text(7, "Small", 86, 172, 154, 53, 43, theme.background, "", true);
    text(8, "Hours", 86, 218, 154, 53, 43, theme.background, "", true);
    text(9, "Small Hours", 68, 394, 290, 36, 27, theme.font, "", true);
    text(10, "With Maya Chen", 68, 435, 290, 27, 20, theme.font_muted);
    text(11, "Stories for the spaces\nbetween everything else.", 68, 472, 290, 52,
         19, theme.font_muted).ent().get<HasLabel>().text_overflow = TextOverflow::Wrap;
    div(context, mk(root.ent(), 12), box(402, 132, 830, 406)
        .with_background(Theme::Usage::Surface).with_corner_radius(14 * s));
    text(13, "Latest episodes", 426, 148, 560, 34, 26, theme.font, "", true);
    text(14, "4 episodes", 1094, 152, 116, 26, 18, theme.font_muted);
    for (size_t i = 0; i < episodes.size(); ++i) {
      const auto &episode = episodes[i];
      const float y = 198 + static_cast<float>(i) * 80;
      const bool active = i == selected;
      if (button(context, mk(root.ent(), 20 + static_cast<int>(i)), box(418, y, 798, 72)
          .with_custom_background(active ? afterhours::Color{49, 67, 69, 255} : theme.surface)
          .with_corner_radius(9 * s).with_debug_name("podcast_episode_" + std::to_string(i)))) {
        selected = i;
        position = 0;
        playing = false;
      }
      text(30 + static_cast<int>(i), fmt::format("{:02}", episodes.size() - i),
           434, y + 15, 42, 32, 23, active ? theme.primary : theme.font_muted);
      text(40 + static_cast<int>(i), episode.title, 488, y + 7, 544, 30,
           23, theme.font, "", true);
      text(50 + static_cast<int>(i), episode.description, 488, y + 39, 544, 24,
           17, theme.font_muted);
      text(60 + static_cast<int>(i), timestamp(episode.seconds), 1110, y + 8,
           90, 28, 20, active ? theme.primary : theme.font_muted);
      text(70 + static_cast<int>(i), episode.date, 1110, y + 40,
           90, 24, 17, theme.font_muted);
    }
    div(context, mk(root.ent(), 80), box(48, 562, 1184, 120)
        .with_background(Theme::Usage::Surface).with_corner_radius(14 * s)
        .with_debug_name("podcast_player"));
    text(81, episodes[selected].title, 72, 578, 710, 32, 25,
         theme.font, "podcast_current", true);
    if (action(82, "-15 sec", 842, 580, 100, 44, "podcast_back"))
      position = std::max(0.f, position - 15);
    if (action(83, playing ? "Pause" : "Play", 954, 580, 142, 44, "podcast_play", true)) {
      if (!playing && position >= episodes[selected].seconds) position = 0;
      playing = !playing;
    }
    if (action(84, "+15 sec", 1108, 580, 100, 44, "podcast_forward"))
      position = std::min(static_cast<float>(episodes[selected].seconds), position + 15);
    if (action(85, fmt::format("Speed: {}x", speeds[speed_index]), 1034, 637,
               174, 30, "podcast_speed"))
      speed_index = (speed_index + 1) % speeds.size();
    float progress = position / static_cast<float>(episodes[selected].seconds);
    if (slider(context, mk(root.ent(), 86), progress,
        box(72, 643, 710, 22).with_background(Theme::Usage::Secondary)
            .with_debug_name("podcast_timeline")))
      position = progress * episodes[selected].seconds;
    if (position >= episodes[selected].seconds) playing = false;
    text(87, timestamp(position) + " / " + timestamp(episodes[selected].seconds),
         72, 613, 360, 27, 18, theme.font_muted, "podcast_time");
    text(88, playing ? "Playing preview" : "Paused", 842, 639, 180, 27,
         18, playing ? theme.primary : theme.font_muted, "podcast_state");
    text(89, "Playback preview. No audio is played.", 48, 689, 1184, 24,
         17, theme.font_muted, "podcast_demo_notice");
  }
};

REGISTER_EXAMPLE_SCREEN(podcast_library, "App Mockups",
                        "Podcast library with local playback preview", PodcastLibrary)
