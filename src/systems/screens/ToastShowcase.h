#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/toast.h>
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ToastShowcase : ScreenSystem<UIContext<InputAction>> {
  int toast_counter = 0;
  int notifications_sent = 0;
  std::string last_notification = "None yet";

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    using toast = afterhours::toast;
    const auto notices = afterhours::EntityQuery().whereHasComponent<toast::Toast>().gen();
    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_grid_snapping(false);
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(20 * s));
    const auto style_toast = [s](ElementResult notice) {
      notice.cmp().font_size = pixels(18 * s);
      auto &label = notice.ent().get<HasLabel>();
      label.letter_spacing = -1.f;
      label.text_x_offset = 8 * s;
    };
    auto at = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_background(Theme::Usage::None).with_corner_radius(0);
    };
    div(context, mk(entity, 0), ComponentConfig{}.with_size({screen_pct(1), screen_pct(1)})
        .with_background(Theme::Usage::Background).with_corner_radius(0).with_debug_name("toast_bg"));
    auto root = div(context, mk(entity, 1), at(0, 0, 1280, 720)
        .with_absolute_position((context.screen_width - 1280 * s) / 2, (context.screen_height - 720 * s) / 2)
        .with_debug_name("toast_main"));
    auto text = [&](int id, const std::string &caption, float x, float y, float w,
                    float h, float size, const std::string &name = "") {
      return div(context, mk(root.ent(), id), at(x, y, w, h).with_label(caption)
          .with_font("AtkinsonMock", pixels(size * s)).with_custom_text_color(theme.font)
          .with_text_overflow(TextOverflow::Wrap).with_ignore_pointer_events().with_debug_name(name));
    };
    auto action = [&](int id, const std::string &caption, float x, float y,
                      afterhours::Color background, const std::string &name) {
      return button(context, mk(root.ent(), id), at(x, y, 188, 44)
          .with_label(caption).with_font("AtkinsonMock", pixels(20 * s))
          .with_custom_background(background).with_auto_text_color(true)
          .with_corner_radius(6 * s).with_debug_name(name));
    };
    text(0, "Toast Notifications", 48, 24, 900, 44, 34);
    text(1, "Inspect native severity, duration and stacking behavior.", 48, 77, 900, 30, 21);
    if (action(2, "Clear toasts", 1044, 45, theme.secondary, "toast_clear")) {
      for (afterhours::Entity &notice : notices)
        notice.get<toast::Toast>().dismiss();
      last_notification = "All toasts cleared";
    }
    const auto section = [&](int id, float y, float h, const std::string &name) {
      div(context, mk(root.ent(), id), at(48, y, 860, h).with_background(Theme::Usage::Surface)
          .with_corner_radius(10 * s).with_debug_name(name));
    };
    // =========================================================================
    // Section 1: Simple toasts
    // =========================================================================
    section(10, 128, 176, "section1");
    // Section header
    text(11, "Simple toasts / native severity colors", 64, 140, 812, 30, 23);
    const std::array<const char *, 4> names{"[i] Info Toast", "[+] Success Toast", "[!] Warning Toast", "[x] Error Toast"};
    const std::array<const char *, 4> messages{"This is an info message", "Operation completed successfully!", "Warning: Check your settings", "Error: Something went wrong!"};
    const std::array<const char *, 4> debug{"btn_info", "btn_success", "btn_warning", "btn_error"};
    const std::array<afterhours::Color, 4> colors{theme.primary, theme.secondary, theme.accent, theme.error};
    const std::array<const char *, 4> lifetime{"Default: 3 s", "Default: 3 s", "Default: 5 s", "Default: 7 s"};
    for (size_t i = 0; i < names.size(); ++i) {
      const float x = 64 + static_cast<float>(i) * 208;
      if (action(20 + static_cast<int>(i), names[i], x, 184, colors[i], debug[i])) {
        last_notification = messages[i];
        ++notifications_sent;
        if (i == 0) style_toast(toast::send_info(context, "[i] This is an info message #" + std::to_string(++toast_counter)));
        if (i == 1) style_toast(toast::send_success(context, "[+] Operation completed successfully!"));
        if (i == 2) style_toast(toast::send_warning(context, messages[i]));
        if (i == 3) style_toast(toast::send_error(context, messages[i]));
      }
      text(30 + static_cast<int>(i), lifetime[i], x, 236, 188, 24, 17);
      text(40 + static_cast<int>(i), messages[i], x, 264, 188, 36, 14);
    }
    // =========================================================================
    // Section 2: Duration and spam
    // =========================================================================
    section(50, 324, 154, "section2");
    text(51, "Duration and stacking tests", 64, 338, 812, 30, 23);
    const afterhours::Color coral{255, 127, 80, 255};
    if (action(52, "Quick, 1 s", 64, 383, theme.primary, "btn_quick")) {
      style_toast(toast::send_info(context, "This disappears fast!", 1));
      ++notifications_sent;
      last_notification = "Quick notification / 1 s";
    }
    if (action(53, "Long, 10 s", 272, 383, theme.primary, "btn_long")) {
      style_toast(toast::send_info(context, "This sticks around for a while...", 10));
      ++notifications_sent;
      last_notification = "Long notification / 10 s";
    }
    if (action(54, "Show five toasts", 480, 383, theme.accent, "btn_spam")) {
      for (int i = 0; i < 5; ++i)
        style_toast(toast::send_warning(context, "Stacking toast #" + std::to_string(i + 1), 4));
      notifications_sent += 5;
      last_notification = "Five warnings / 4 s each";
    }
    if (action(55, "Custom Color", 688, 383, coral, "btn_custom")) {
      style_toast(toast::send_custom(context, "Custom colored toast!", coral, 4));
      ++notifications_sent;
      last_notification = "Coral notification / 4 s";
    }
    text(56, "Native stack: new notifications accumulate upward.", 64, 437, 592, 28, 18);
    text(57, "Coral / #FF7F50", 688, 437, 188, 28, 17);
    section(60, 498, 178, "section3");
    text(61, "Simulated action feedback", 64, 511, 812, 30, 23);
    if (action(62, "Sample deletion", 64, 558, theme.secondary, "btn_undo")) {
      style_toast(toast::send_success(context, "Demo: deletion feedback only", 5));
      ++notifications_sent;
      last_notification = "Deletion feedback / no undo action";
    }
    text(63, "Sends a five-second message. No item is deleted and no undo action is attached.",
         272, 554, 600, 64, 19);
    text(64, "Notifications sent: " + std::to_string(notifications_sent), 64, 631, 812, 28, 20, "toast_sent");
    div(context, mk(root.ent(), 70), at(932, 128, 300, 548)
        .with_background(Theme::Usage::Surface).with_corner_radius(10 * s));
    text(71, "Preview location", 948, 140, 268, 32, 23);
    div(context, mk(root.ent(), 72), at(948, 187, 268, 143).with_border(theme.font_muted, s)
        .with_ignore_pointer_events().with_on_draw_fg([theme, s](RectangleType r) {
          for (int i = 0; i < 3; ++i)
            afterhours::draw_rectangle({r.x + (r.width - 120 * s) / 2, r.y + r.height - (26 + i * 25) * s, 120 * s, 18 * s}, theme.primary);
        }));
    text(73, "Bottom center / stack grows upward", 948, 344, 268, 52, 19);
    int active_count = 0;
    for (afterhours::Entity &notice : notices)
      if (!notice.get<toast::Toast>().is_expired()) ++active_count;
    text(74, "Active count: " + std::to_string(active_count), 948, 411, 268, 32, 21, "toast_active");
    text(75, "Last notification", 948, 466, 268, 28, 19);
    text(76, last_notification, 948, 505, 268, 118, 20, "toast_last");
  }
};

REGISTER_EXAMPLE_SCREEN(toasts, "Component Galleries",
                        "Toast notification system demo", ToastShowcase)
