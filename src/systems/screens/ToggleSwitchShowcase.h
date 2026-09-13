#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ToggleSwitchShowcase : ScreenSystem<UIContext<InputAction>> {
  // Pill style toggles
  bool enable_notifications = true;
  bool enable_sound = false;
  bool enable_vibration = true;
  // Circle style toggles
  bool dark_mode = true;
  bool auto_save = false;
  bool cloud_sync = true;
  // Disabled toggles (non-interactive)
  bool disabled_on = true;
  bool disabled_off = false;
  bool reset_pending = false;

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::neon_dark();
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_grid_snapping(false);
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(20 * s));
    auto at = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_background(Theme::Usage::None).with_corner_radius(0);
    };
    // Full-screen dark background
    auto background = div(context, mk(entity, 0), ComponentConfig{}
        .with_size({screen_pct(1), screen_pct(1)}).with_background(Theme::Usage::Background)
        .with_corner_radius(0).with_debug_name("toggle_bg"));
    auto root = div(context, mk(background.ent(), 0), at(0, 0, 1280, 720)
        .with_absolute_position((context.screen_width - 1280 * s) / 2, (context.screen_height - 720 * s) / 2));
    // Centered card (settings panel style)
    div(context, mk(root.ent(), 0), at(304, 24, 672, 672)
        .with_background(Theme::Usage::Surface).with_corner_radius(12 * s).with_debug_name("toggle_card"));
    const auto label = [&](int id, const std::string &caption, float x, float y,
                           float w, float h, float size, const std::string &name = "") {
      return div(context, mk(root.ent(), id), at(x, y, w, h).with_label(caption)
          .with_font("AtkinsonMock", pixels(size * s)).with_custom_text_color(theme.font)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    // Title
    label(1, "Toggle and checkbox settings demo", 328, 42, 624, 44, 29);
    const int enabled = static_cast<int>(enable_notifications) + static_cast<int>(enable_sound) +
        static_cast<int>(enable_vibration) + static_cast<int>(dark_mode) +
        static_cast<int>(auto_save) + static_cast<int>(cloud_sync);
    label(2, "Editable states: " + std::to_string(enabled) + " On / " + std::to_string(6 - enabled) + " Off",
          328, 91, 624, 28, 19, "toggle_summary");
    auto row = [&](int id, const std::string &caption, bool &value, float y,
                   bool circle, bool disabled, const std::string &name) {
      div(context, mk(root.ent(), 100 + id), at(328, y, 624, 38)
          .with_custom_background(afterhours::colors::lighten(theme.surface, .06f)).with_corner_radius(5 * s));
      label(120 + id, caption, 344, y, 340, 38, 21);
      auto ep = mk(root.ent(), 140 + id);
      auto [control, parent] = deref(ep);
      if (reset_pending && circle && control.has<HasCheckboxState>())
        control.get<HasCheckboxState>().on = value;
      if (reset_pending && !circle && control.has<HasToggleSwitchState>()) {
        auto &state = control.get<HasToggleSwitchState>();
        state.on = value;
        state.animation_progress = value ? 1.f : 0.f;
      }
      if (circle) {
        checkbox(context, ep, value, at(862, y + 1, 36, 36)
            .with_custom_background(theme.primary).with_border(theme.font_muted, s)
            .with_rounded_corners(RoundedCorners().all_round()).with_corner_radius(18 * s)
            .with_checkbox_indicators("", "").with_disabled(disabled).with_debug_name(name));
        if (value)
          div(context, mk(root.ent(), 160 + id), at(862, y + 1, 36, 36)
              .with_ignore_pointer_events().with_on_draw_fg([theme, s](RectangleType r) {
                afterhours::draw_line_ex({r.x + 8 * s, r.y + 18 * s}, {r.x + 15 * s, r.y + 25 * s}, 3 * s, theme.font);
                afterhours::draw_line_ex({r.x + 15 * s, r.y + 25 * s}, {r.x + 28 * s, r.y + 10 * s}, 3 * s, theme.font);
              }));
      } else {
        auto config = at(854, y, 64, 38).with_size({pixels(std::max(64 * s, 52.f)), pixels(std::max(38 * s, 28.f))})
            .with_disabled(disabled).with_debug_name(name);
        config.corner_radius.reset();
        toggle_switch(context, ep, value, config);
      }
      label(180 + id, value ? "On" : "Off", 718, y, 92, 38, 20, name + "_state");
    };
    // ── Pill Style Section ──
    label(3, "General", 328, 131, 260, 28, 23);
    label(4, "Pill switches", 666, 133, 286, 26, 18);
    row(0, "Notifications", enable_notifications, 170, false, false, "toggle_notifications");
    row(1, "Sound Effects", enable_sound, 214, false, false, "toggle_sound");
    row(2, "Vibration", enable_vibration, 258, false, false, "toggle_vibration");
    // Separator
    div(context, mk(root.ent(), 5), at(328, 305, 624, 1).with_custom_background(theme.font_muted));
    // ── Circle Style Section ──
    label(6, "Preferences", 328, 313, 260, 28, 23);
    label(7, "Checkbox variants / circle", 666, 315, 286, 26, 18);
    row(3, "Dark Mode", dark_mode, 351, true, false, "toggle_dark_mode");
    row(4, "Auto-Save", auto_save, 395, true, false, "toggle_auto_save");
    row(5, "Cloud Sync", cloud_sync, 439, true, false, "toggle_cloud_sync");
    // Separator
    div(context, mk(root.ent(), 8), at(328, 486, 624, 1).with_custom_background(theme.font_muted));
    // ── Disabled Section ──
    label(9, "Disabled", 328, 496, 260, 28, 23);
    label(10, "Track On / thumb On", 666, 498, 286, 26, 18);
    // Disabled pill toggle (ON state, non-interactive)
    row(6, "Locked Setting", disabled_on, 535, false, true, "toggle_locked");
    label(11, "Managed by administrator", 344, 576, 400, 22, 16);
    // Disabled circle checkbox (OFF state, non-interactive)
    row(7, "Unavailable Option", disabled_off, 603, true, true, "toggle_unavailable");
    label(12, "Not available in this demo", 344, 644, 330, 22, 16);
    reset_pending = false;
    if (button(context, mk(root.ent(), 13), at(706, 649, 246, 32)
        .with_label("Reset demo defaults").with_font("AtkinsonMock", pixels(19 * s))
        .with_background(Theme::Usage::Primary).with_corner_radius(5 * s).with_debug_name("toggle_reset"))) {
      enable_notifications = true;
      enable_sound = false;
      enable_vibration = true;
      dark_mode = true;
      auto_save = false;
      cloud_sync = true;
      reset_pending = true;
    }
  }
};

REGISTER_EXAMPLE_SCREEN(toggle_switches, "Component Galleries",
                        "Toggle switch (Pill) and round checkbox styles",
                        ToggleSwitchShowcase)
