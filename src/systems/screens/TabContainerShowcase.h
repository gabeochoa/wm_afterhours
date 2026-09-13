#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/clipboard.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct TabContainerShowcase : ScreenSystem<UIContext<InputAction>> {
  size_t active_tab = 0;
  bool open = true;
  bool copied = false;
  std::array<std::string_view, 3> tab_labels = {"Profile", "Audio", "Display"};

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    const afterhours::Color ink{28, 42, 61, 255}, muted{73, 91, 110, 255};
    const afterhours::Color accent{0, 105, 177, 255}, white{255, 255, 255, 255};
    const afterhours::Color field{235, 241, 247, 255};
    context.theme = theme_presets::neon_dark();
    context.theme.background = {232, 238, 244, 255};
    context.theme.surface = white;
    context.theme.font = ink;
    context.theme.font_muted = muted;
    context.theme.accent = accent;
    context.scaling_mode = ScalingMode::Proportional;
    const float scale = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const float left = (context.screen_width / scale - 1144) / 2;
    const float top = (context.screen_height / scale - 720) / 2;
    const auto box = [scale, left, top](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * scale), pixels(h * scale)})
          .with_absolute_position((left + x) * scale, (top + y) * scale)
          .with_background(Theme::Usage::None).with_corner_radius(0);
    };
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background(context.theme.background).with_corner_radius(0).with_debug_name("root"));
    const auto label = [&](int id, float x, float y, float w, float h, const std::string &text, float size,
                           afterhours::Color color, const std::string &name = "") {
      return div(context, mk(root.ent(), id), box(x, y, w, h).with_label(text)
          .with_font("AtkinsonMock", pixels(size * scale)).with_custom_text_color(color)
          .with_alignment(TextAlignment::Left).with_text_overflow(TextOverflow::Wrap)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    label(0, 0, 25, 1144, 46, "Tab container demo", 35, ink);
    label(1, 0, 78, 1144, 34, "Choose a tab to swap its content. Tab and Enter activate the native controls.", 22, muted);
    if (!open) {
      label(2, 24, 194, 1096, 44, "Example closed. No account or device settings were changed.", 27, ink, "tc_closed");
      if (button(context, mk(root.ent(), 3), box(24, 258, 280, 46).with_label("Reopen example")
          .with_font("AtkinsonMock", pixels(23 * scale)).with_custom_background(accent)
          .with_custom_text_color(white).with_corner_radius(7 * scale).with_debug_name("tc_reopen"))) open = true;
      return;
    }
    div(context, mk(root.ent(), 10), box(0, 141, 1144, 550).with_custom_background(white).with_corner_radius(10 * scale));
    auto tabs = tab_container(context, mk(root.ent(), 11), tab_labels, active_tab,
        box(0, 141, 1144, 58).with_font("AtkinsonMock", pixels(27 * scale))
            .with_no_wrap().with_debug_name("tabs"));
    size_t tab_index = 0;
    for (const auto child_id : tabs.ent().get<UIComponent>().children) {
      auto &tab = UICollectionHolder::getEntityForIDEnforce(child_id);
      const auto font = tab_index == active_tab ? "AtkinsonMockBold" : "AtkinsonMock";
      tab.get<UIComponent>().enable_font(font, pixels(27 * scale), true);
      tab.get<HasLabel>().font_name = font;
      ++tab_index;
    }
    const std::array<const char *, 3> titles{"Profile details", "Audio sample", "Display sample"};
    label(12, 24, 219, 720, 42, titles[active_tab], 31, ink, "tc_title");
    label(13, 24, 265, 1096, 31, active_tab == 0 ? "Example profile / read-only fixture data" : "Read-only fixture values / this demo does not change device settings", 20, muted);
    const auto row = [&](int id, float y, const std::string &name, const std::string &value) {
      label(id, 24, y, 170, 37, name, 21, muted);
      label(id + 1, 204, y, 362, 37, value, 23, ink);
    };
    if (active_tab == 0) {
      row(20, 326, "Username", "player_one");
      label(22, 24, 380, 170, 40, "Email", 21, muted);
      div(context, mk(root.ent(), 23), box(204, 379, 374, 42).with_custom_background(field).with_corner_radius(5 * scale));
      label(24, 216, 381, 350, 38, "player@example.com", 23, ink, "tc_email");
      row(25, 434, "Created", "January 2024");
      label(27, 24, 488, 170, 40, "Subscription", 21, muted);
      div(context, mk(root.ent(), 28), box(204, 490, 142, 35).with_custom_background({222, 237, 255, 255}).with_corner_radius(6 * scale));
      label(29, 216, 491, 118, 33, "Premium", 23, accent);
      row(30, 542, "Sample last login", "2024-01-15 10:30 UTC");
      div(context, mk(root.ent(), 32), box(634, 321, 486, 268).with_custom_background(field).with_corner_radius(8 * scale));
      label(33, 658, 341, 438, 37, "Example profile", 27, ink);
      label(34, 658, 390, 438, 90, "These are fixed sample values. Closing and reopening the example keeps your selected tab.", 22, muted);
      if (button(context, mk(root.ent(), 35), box(658, 514, 230, 44).with_label(copied ? "Email copied" : "Copy email")
          .with_font("AtkinsonMock", pixels(22 * scale)).with_custom_background(white)
          .with_custom_text_color(accent).with_corner_radius(6 * scale).with_debug_name("tc_copy"))) {
        afterhours::clipboard::set_text("player@example.com");
        copied = true;
      }
    }
    if (active_tab == 1) {
      row(40, 326, "Master volume", "80%");
      row(42, 380, "Music volume", "60%");
      row(44, 434, "Sound effects", "75%");
      label(46, 24, 502, 510, 72, "Static percentages, shown independently. No audio is played or adjusted here.", 22, muted);
      label(47, 602, 312, 518, 34, "Bar width specimens", 25, ink);
      for (int i = 0; i < 5; ++i) {
        const float width = 120 + static_cast<float>(i) * 90;
        const float y = 359 + static_cast<float>(i) * 48;
        div(context, mk(root.ent(), 50 + i), box(602, y, width, 24).with_custom_background(accent)
            .with_corner_radius(1.8f * scale).with_debug_name("bar_" + std::to_string(i)));
        label(60 + i, 602, y + 24, 518, 23, fmt::format("{:.0f} × {:.0f} px", width * scale, 24 * scale), 17, muted);
      }
    }
    if (active_tab == 2) {
      row(70, 326, "Resolution", "1920 × 1080");
      row(72, 380, "Fullscreen", "Enabled");
      row(74, 434, "VSync", "On");
      label(76, 24, 502, 510, 72, "The six swatches preserve the fixture colors. They are samples, not a monitor calibration.", 22, muted);
      const std::array<afterhours::Color, 6> swatches{{{255, 87, 87, 255}, {255, 189, 87, 255}, {255, 255, 87, 255}, {87, 255, 87, 255}, {87, 189, 255, 255}, {189, 87, 255, 255}}};
      label(77, 602, 312, 518, 34, fmt::format("Color swatches / {:.0f} × {:.0f} px", 60 * scale, 60 * scale), 25, ink);
      for (size_t i = 0; i < swatches.size(); ++i) {
        const float x = 614 + static_cast<float>(i % 3) * 176;
        const float y = 364 + static_cast<float>(i / 3) * 121;
        div(context, mk(root.ent(), 80 + static_cast<int>(i)), box(x, y, 60, 60)
            .with_custom_background(swatches[i]).with_corner_radius(6 * scale).with_debug_name("swatch_" + std::to_string(i)));
        label(90 + static_cast<int>(i), x, y + 68, 152, 28,
              fmt::format("#{:02X}{:02X}{:02X}", swatches[i].r, swatches[i].g, swatches[i].b), 22, ink);
      }
    }
    div(context, mk(root.ent(), 100), box(24, 609, 1096, 1).with_custom_background({203, 214, 226, 255}));
    label(101, 24, 633, 840, 33, "Read-only example / No pending changes", 21, muted);
    if (button(context, mk(root.ent(), 102), box(940, 627, 180, 46).with_label("Close")
        .with_font("AtkinsonMock", pixels(23 * scale)).with_custom_background(accent)
        .with_custom_text_color(white).with_corner_radius(7 * scale).with_debug_name("tc_close"))) open = false;
  }
};

REGISTER_EXAMPLE_SCREEN(tab_container, "Component Galleries",
                        "Tab container component demo", TabContainerShowcase)
