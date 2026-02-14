#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct SportsSettingsScreen : ScreenSystem<UIContext<InputAction>> {
  size_t active_tab = 1; // VIDEO tab
  size_t selected_row = 6; // Anti-Aliasing method selected to show TSR tooltip

  // Colors matching Rematch/FIFA style - dark with bright green accents
  afterhours::Color bg_dark{18, 22, 28, 255};        // Very dark background
  afterhours::Color overlay_dark{25, 32, 42, 200};   // Semi-transparent overlay
  afterhours::Color panel_dark{32, 42, 55, 255};     // Panel background
  afterhours::Color accent_green{85, 195, 145, 255}; // Bright green accent
  afterhours::Color accent_green_dark{55, 155, 105, 255}; // Darker green
  afterhours::Color text_white{235, 240, 245, 255};       // White text
  afterhours::Color text_muted{125, 140, 155, 255};       // Muted text
  afterhours::Color highlight_row{
      45, 75, 95, 160}; // Selected row highlight (semi-transparent)
  afterhours::Color slider_bg{55, 65, 80, 255};    // Slider background
  afterhours::Color slider_track{22, 28, 38, 255}; // Slider track - much darker for strong contrast against green fill
  afterhours::Color slider_empty_border{55, 65, 80, 255}; // Border for empty portion - subtle but visible

  std::vector<std::string> tabs = {"GAMEPLAY", "VIDEO", "AUDIO", "CONTROLS"};

  struct SettingRow {
    std::string label;
    bool is_slider;
    float slider_pct;
    int min_val; // For slider value mapping
    int max_val;
    std::vector<std::string> options; // For non-sliders
    size_t option_idx;                // Current option index
  };

  std::vector<SettingRow> graphics_settings = {
      {"Window mode",
       false,
       0.0f,
       0,
       0,
       {"Fullscreen", "Borderless window", "Windowed"},
       1},
      {"Resolution",
       false,
       0.0f,
       0,
       0,
       {"1920x1080", "2560x1440", "3840x2160"},
       1},
      {"Screen percentage", true, 1.0f, 50, 100, {}, 0},
      {"V Sync", false, 0.0f, 0, 0, {"Disabled", "Enabled"}, 1},
      {"Max FPS", true, 0.5f, 30, 240, {}, 0},
      {"Gamma", true, 0.45f, 10, 30, {}, 0}, // 1.0-3.0 scaled by 10
      {"Anti-Aliasing method",
       false,
       0.0f,
       0,
       0,
       {"Off", "FXAA", "TAA", "TSR (Temporal Super Resolution)", "DLSS"},
       3},
      {"Dynamic resolution", false, 0.0f, 0, 0, {"Disabled", "Enabled"}, 1},
      {"Framerate target", true, 0.4f, 30, 120, {}, 0},
      {"Motion blur", true, 0.7f, 0, 10, {}, 0},
      {"Graphics quality",
       false,
       0.0f,
       0,
       0,
       {"Low", "Medium", "High", "Ultra"},
       3},
      {"Texture quality",
       false,
       0.0f,
       0,
       0,
       {"Low", "Medium", "High", "Ultra"},
       3},
  };

  std::vector<SettingRow> gameplay_settings = {
      {"Difficulty",
       false,
       0.0f,
       0,
       0,
       {"Beginner", "Amateur", "Semi-Pro", "Professional", "World Class",
        "Legendary"},
       3},
      {"Game Speed",
       false,
       0.0f,
       0,
       0,
       {"Slow", "Normal", "Fast"},
       1},
      {"Match Length",
       false,
       0.0f,
       0,
       0,
       {"4 min", "6 min", "8 min", "10 min", "15 min", "20 min"},
       2},
      {"Camera Type",
       false,
       0.0f,
       0,
       0,
       {"Broadcast", "Co-op", "Dynamic", "End to End", "Pro"},
       0},
      {"Camera Height", true, 0.5f, 0, 20, {}, 0},
      {"Camera Zoom", true, 0.5f, 0, 20, {}, 0},
      {"Ball Indicator", false, 0.0f, 0, 0, {"Off", "On"}, 1},
      {"Player Names",
       false,
       0.0f,
       0,
       0,
       {"Off", "Always", "When Controlled"},
       2},
  };

  std::vector<SettingRow> audio_settings = {
      {"Master Volume", true, 0.8f, 0, 100, {}, 0},
      {"Music Volume", true, 0.7f, 0, 100, {}, 0},
      {"Commentary Volume", true, 0.9f, 0, 100, {}, 0},
      {"Crowd Volume", true, 0.85f, 0, 100, {}, 0},
      {"SFX Volume", true, 0.75f, 0, 100, {}, 0},
      {"Commentary Language",
       false,
       0.0f,
       0,
       0,
       {"English", "Spanish", "French", "German", "Italian"},
       0},
      {"Crowd Chants", false, 0.0f, 0, 0, {"Off", "On"}, 1},
  };

  std::vector<SettingRow> controls_settings = {
      {"Vibration", false, 0.0f, 0, 0, {"Off", "On"}, 1},
      {"Vibration Strength", true, 0.7f, 0, 100, {}, 0},
      {"Auto Switch",
       false,
       0.0f,
       0,
       0,
       {"Off", "Ball Only", "Air Balls", "All"},
       2},
      {"Pass Assistance", false, 0.0f, 0, 0, {"Manual", "Semi", "Assisted"}, 2},
      {"Shot Assistance", false, 0.0f, 0, 0, {"Manual", "Semi", "Assisted"}, 1},
      {"Through Ball", false, 0.0f, 0, 0, {"Manual", "Semi", "Assisted"}, 1},
      {"Crossing", false, 0.0f, 0, 0, {"Manual", "Semi", "Assisted"}, 2},
  };

  struct TabInfo {
    std::vector<SettingRow> *settings;
    const char *header;
  };

  TabInfo get_tab_info() {
    TabInfo infos[] = {
        {&gameplay_settings, "Gameplay"},
        {&graphics_settings, "Graphics"},
        {&audio_settings, "Audio"},
        {&controls_settings, "Controls"},
    };
    size_t idx = active_tab < 4 ? active_tab : 1;
    return infos[idx];
  }

  std::vector<SettingRow> &get_current_settings() {
    return *get_tab_info().settings;
  }

  std::string get_section_header() {
    return get_tab_info().header;
  }

  // Returns a group header label at the start of each logical section.
  // nullptr means no header at this row index.
  const char *get_group_header(size_t tab, size_t row) {
    if (tab == 0) { // GAMEPLAY
      if (row == 0) return "Match";
      if (row == 4) return "Camera";
      if (row == 6) return "Display";
    } else if (tab == 1) { // VIDEO / Graphics
      if (row == 0) return "Display";
      if (row == 6) return "Anti-Aliasing & Performance";
      if (row == 10) return "Quality";
    } else if (tab == 2) { // AUDIO
      if (row == 0) return "Volume";
      if (row == 5) return "Options";
    } else if (tab == 3) { // CONTROLS
      if (row == 0) return "Vibration";
      if (row == 2) return "Assistance";
    }
    return nullptr;
  }

  // Get description text for a setting (tooltips for abbreviations)
  std::string get_setting_description(const std::string &label,
                                      const std::string &current_value) {
    // Anti-Aliasing method explanations
    if (label == "Anti-Aliasing method") {
      if (current_value == "TSR (Temporal Super Resolution)") {
        return "TSR (Temporal Super Resolution) uses";
      } else if (current_value == "TAA") {
        return "TAA (Temporal Anti-Aliasing) smooths";
      } else if (current_value == "FXAA") {
        return "FXAA (Fast Approximate AA) is a";
      } else if (current_value == "DLSS") {
        return "DLSS (Deep Learning Super Sampling)";
      }
    }
    return "Adjust this setting to customize your";
  }

  std::string get_setting_description_line2(const std::string &label,
                                            const std::string &current_value) {
    if (label == "Anti-Aliasing method") {
      if (current_value == "TSR (Temporal Super Resolution)") {
        return "AI to upscale lower resolution frames.";
      } else if (current_value == "TAA") {
        return "edges using temporal frame data.";
      } else if (current_value == "FXAA") {
        return "fast, low-cost edge smoothing method.";
      } else if (current_value == "DLSS") {
        return "uses NVIDIA AI for quality upscaling.";
      }
    }
    return "gaming experience.";
  }

  // Helper to format slider value for display - works with current tab's
  // settings
  std::string format_slider_value(size_t index, float pct) {
    auto &settings = get_current_settings();
    if (index >= settings.size())
      return "---";
    auto &setting = settings[index];
    int val = setting.min_val +
              static_cast<int>(pct * (setting.max_val - setting.min_val));

    // Special case for gamma in graphics tab
    if (active_tab == 1 &&
        index == 5) { // Gamma - divide by 10 for decimal display
      return fmt::format("{:.1f}", val / 10.0f);
    }
    // Add units for common slider types
    std::string label = setting.label;
    if (label == "Max FPS" || label == "Framerate target") {
      return std::to_string(val) + " FPS";
    } else if (label.find("Volume") != std::string::npos ||
               label == "Screen percentage" ||
               label == "Vibration Strength") {
      return std::to_string(val) + "%";
    }
    return std::to_string(val);
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    UIStylingDefaults::get().set_default_font("EqProRounded", h720(19.0f));
    Theme theme;
    theme.font = text_white;
    theme.darkfont = bg_dark;
    theme.font_muted = text_muted;
    theme.background = bg_dark;
    theme.surface = panel_dark;
    theme.primary = accent_green;
    theme.secondary = text_muted;
    theme.accent = accent_green;
    theme.error = afterhours::Color{180, 80, 80, 255};
    theme.roundness = 0.0f;
    theme.segments = 4;
    context.theme = theme;

    int screen_w = Settings::get().get_screen_width();
    int screen_h = Settings::get().get_screen_height();
    auto pxf = [](float v) { return pixels(static_cast<int>(v)); };

    // ========== SYNC SELECTION WITH FOCUS ==========
    // Update selected_row based on focus FIRST (before keyboard handling)
    // This ensures arrow keys work on the currently focused row
    auto &current_settings = get_current_settings();
    auto focus_id = context.focus_id;
    auto opt_focused = afterhours::ui::UICollectionHolder::getEntityForID(focus_id);
    if (opt_focused.has_value()) {
      auto &focused = opt_focused.asE();
      if (focused.has<afterhours::ui::HasLabel>()) {
        auto &label = focused.get<afterhours::ui::HasLabel>().label;
        for (size_t i = 0; i < current_settings.size(); i++) {
          if (label == current_settings[i].label) {
            selected_row = i;
            break;
          }
        }
      }
    }

    // ========== KEYBOARD INPUT HANDLING ==========
    // Handle left/right arrow keys to change value when a row is selected
    if (selected_row < current_settings.size()) {
      auto &setting = current_settings[selected_row];
      float step = 0.05f; // 5% per press for sliders

      if (context.pressed(InputAction::WidgetLeft)) {
        if (setting.is_slider) {
          setting.slider_pct = std::max(0.0f, setting.slider_pct - step);
        } else if (!setting.options.empty()) {
          setting.option_idx = (setting.option_idx == 0)
                                   ? setting.options.size() - 1
                                   : setting.option_idx - 1;
        }
      }

      if (context.pressed(InputAction::WidgetRight)) {
        if (setting.is_slider) {
          setting.slider_pct = std::min(1.0f, setting.slider_pct + step);
        } else if (!setting.options.empty()) {
          setting.option_idx =
              (setting.option_idx + 1) % setting.options.size();
        }
      }
    }

    // ========== BACKGROUND ==========
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(screen_w), pixels(screen_h)})
            .with_custom_background(bg_dark)
            .with_debug_name("bg"));

    // ========== TOP TAB BAR ==========
    float tab_y = 15.0f;
    float tab_start_x = 70.0f;

    tab_container(context, mk(entity, 10), tabs, active_tab,
        ComponentConfig{}
            .with_size(ComponentSize{pixels(520), pixels(35)})
            .with_absolute_position(tab_start_x, tab_y));

    // ========== SECTION HEADER ==========
    float header_y = tab_y + 55.0f;

    div(context, mk(entity, 30),
        ComponentConfig{}
            .with_label(get_section_header())
            .with_size(ComponentSize{pixels(150), pixels(30)})
            .with_absolute_position(50.0f, header_y)
            .with_font("EqProRounded", h720(20.0f))
            .with_custom_text_color(text_white));

    // Separator between header and settings rows
    div(context, mk(entity, 31),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(520), pixels(1)})
            .with_absolute_position(50.0f, header_y + 32.0f)
            .with_custom_background(afterhours::Color{85, 195, 145, 60})
            .with_debug_name("section_separator_header"));

    // ========== LEFT PANEL: Settings ==========
    float panel_x = 50.0f;
    float panel_y = header_y + 40.0f;
    float panel_w = 520.0f;
    float row_h = 42.0f;

    // Reset selected_row if out of bounds for current tab
    if (selected_row >= current_settings.size()) {
      selected_row = 0;
    }

    // Each row is a single tabbable element - the entire row is selectable
    float group_offset = 0.0f;
    constexpr float group_header_h = 28.0f; // Height of group header + separator
    for (size_t i = 0; i < current_settings.size(); i++) {
      // Check for group header at this row
      const char *group_label = get_group_header(active_tab, i);
      if (group_label) {
        float gh_y = panel_y + (float)i * row_h + group_offset;
        // Add extra spacing before non-first groups
        if (i > 0) {
          group_offset += 8.0f; // gap before separator
          gh_y += 8.0f;
        }
        // Group separator line
        div(context, mk(entity, 1200 + static_cast<int>(i)),
            ComponentConfig{}
                .with_size(ComponentSize{pixels((int)(panel_w - 20)), pixels(1)})
                .with_absolute_position(panel_x - 5.0f, gh_y)
                .with_custom_background(
                    i > 0 ? afterhours::Color{85, 195, 145, 40}
                           : afterhours::Color{0, 0, 0, 0}) // invisible for first group
                .with_skip_tabbing(true)
                .with_debug_name(std::string("group_sep_") + std::to_string(i)));
        // Group header label
        div(context, mk(entity, 1250 + static_cast<int>(i)),
            ComponentConfig{}
                .with_label(group_label)
                .with_size(ComponentSize{pixels(250), pixels(20)})
                .with_absolute_position(panel_x + 20.0f, gh_y + 3.0f)
                .with_font("EqProRounded", h720(14.0f))
                .with_custom_text_color(afterhours::Color{85, 195, 145, 180})
                .with_skip_tabbing(true)
                .with_debug_name(std::string("group_lbl_") + std::to_string(i)));
        group_offset += group_header_h;
      }
      float ry = panel_y + (float)i * row_h + group_offset;
      bool is_selected = (i == selected_row);
      auto &setting = current_settings[i];

      // Get display value based on type
      std::string display_value;
      if (setting.is_slider) {
        display_value = format_slider_value(i, setting.slider_pct);
      } else if (!setting.options.empty()) {
        display_value = setting.options[setting.option_idx];
      } else {
        display_value = "---";
      }

      // Colors based on selection state - always use readable colors
      afterhours::Color label_color = is_selected ? text_white : text_muted;
      afterhours::Color value_color = is_selected ? accent_green : text_muted;
      afterhours::Color arrow_color = is_selected ? text_white : text_muted;

      // Row background - render first (behind everything)
      div(context, mk(entity, 50 + static_cast<int>(i)),
          ComponentConfig{}
              .with_720p_size(panel_w, row_h - 2)
              .with_absolute_position(panel_x - 10.0f, ry)
              .with_custom_background(is_selected
                                          ? highlight_row
                                          : afterhours::Color{35, 45, 55, 255})
              .with_render_layer(-1)
              .with_debug_name("row_bg_" + std::to_string(i)));

      // Selection indicator - left border accent
      if (is_selected) {
        div(context, mk(entity, 60 + static_cast<int>(i)),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(4),
                                         pxf(row_h - 8)})
                .with_absolute_position(panel_x - 15.0f, ry + 3.0f)
                .with_custom_background(accent_green)
                .with_render_layer(1)
                .with_debug_name("row_accent_" + std::to_string(i)));
      }

      // Label button - click to select row (this is tabbable)
      if (button(context, mk(entity, 100 + static_cast<int>(i)),
                 ComponentConfig{}
                     .with_label(setting.label)
                     .with_size(ComponentSize{
                         pixels(200), pxf(row_h - 4)})
                     .with_absolute_position(panel_x + 20.0f, ry + 2.0f)
                     .with_font("EqProRounded", h720(18.0f))
                     .with_custom_text_color(label_color)
                     .with_custom_background(afterhours::Color{0, 0, 0, 0})
                     .with_alignment(TextAlignment::Left)
                     .with_padding(Padding{.left = pixels(4)})
                     .with_debug_name("label_" + std::to_string(i)))) {
        selected_row = i;
      }

      float value_x = panel_x + 220.0f;
      float arrow_size = 28.0f;  // Minimum accessible touch target
      float step = 0.05f; // 5% per click for sliders

      // Left arrow < (skip tabbing - use row's keyboard handling) - render
      // above button
      if (button(context, mk(entity, 300 + static_cast<int>(i) * 3),
                 ComponentConfig{}
                     .with_label("<")
                     .with_size(ComponentSize{
                         pixels(44), pixels(44)})
                     .with_absolute_position(value_x, ry + 0.0f)
                     .with_font("EqProRounded", h720(20.0f))
                     .with_custom_text_color(arrow_color)
                     .with_custom_background(afterhours::Color{0, 0, 0, 0})
                     .with_skip_tabbing(true)
                     .with_render_layer(2)
                     .with_debug_name("left_" + std::to_string(i)))) {
        selected_row = i;
        if (setting.is_slider) {
          setting.slider_pct = std::max(0.0f, setting.slider_pct - step);
        } else if (!setting.options.empty()) {
          setting.option_idx = (setting.option_idx == 0)
                                   ? setting.options.size() - 1
                                   : setting.option_idx - 1;
        }
      }

      // Value display text (skip tabbing) - render above button
      div(context, mk(entity, 301 + static_cast<int>(i) * 3),
          ComponentConfig{}
              .with_label(display_value)
              .with_size(ComponentSize{pixels(130), pixels(44)})
              .with_absolute_position(value_x + 44.0f + 4.0f, ry + 0.0f)
              .with_font("EqProRounded", h720(18.0f))
              .with_custom_text_color(value_color)
              .with_alignment(TextAlignment::Center)
              .with_skip_tabbing(true)
              .with_render_layer(2)
              .with_debug_name("value_" + std::to_string(i)));

      // Right arrow > (skip tabbing - use row's keyboard handling) - render
      // above button
      if (button(context, mk(entity, 302 + static_cast<int>(i) * 3),
                 ComponentConfig{}
                     .with_label(">")
                     .with_size(ComponentSize{
                         pixels(44), pixels(44)})
                     .with_absolute_position(value_x + 44.0f + 148.0f, ry + 0.0f)
                     .with_font("EqProRounded", h720(20.0f))
                     .with_custom_text_color(arrow_color)
                     .with_custom_background(afterhours::Color{0, 0, 0, 0})
                     .with_skip_tabbing(true)
                     .with_render_layer(2)
                     .with_debug_name("right_" + std::to_string(i)))) {
        selected_row = i;
        if (setting.is_slider) {
          setting.slider_pct = std::min(1.0f, setting.slider_pct + step);
        } else if (!setting.options.empty()) {
          setting.option_idx =
              (setting.option_idx + 1) % setting.options.size();
        }
      }

      // Slider visual bar (static display - matches arrow control values)
      if (setting.is_slider) {
        float bar_x = value_x + arrow_size + 170.0f;
        float bar_y = ry + 12.0f;
        float bar_w = 100.0f;
        float bar_h = 14.0f;

        // Slider track background with border for contrast
        div(context, mk(entity, 500 + static_cast<int>(i)),
            ComponentConfig{}
                .with_720p_size(bar_w, bar_h)
                .with_absolute_position(bar_x, bar_y)
                .with_custom_background(slider_track)
                .with_border(slider_empty_border, 1.0f)
                .with_skip_tabbing(true)
                .with_debug_name("slider_bg_" + std::to_string(i)));

        // Slider fill (shows current value)
        float fill_w = bar_w * setting.slider_pct;
        if (fill_w > 2.0f) {
          div(context, mk(entity, 600 + static_cast<int>(i)),
              ComponentConfig{}
                  .with_720p_size(fill_w, bar_h)
                  .with_absolute_position(bar_x, bar_y)
                  .with_custom_background(accent_green)
                  .with_render_layer(1)
                  .with_skip_tabbing(true)
                  .with_debug_name("slider_fill_" + std::to_string(i)));
        }
      } else if (setting.options.size() > 1) {
        // Segmented position indicator for non-slider settings
        // Shows dots/segments matching the slider bar position and style
        float bar_x = value_x + arrow_size + 170.0f;
        float bar_y = ry + 14.0f;
        float bar_w = 100.0f;
        float dot_h = 10.0f;
        size_t num_opts = setting.options.size();
        float total_gap = 4.0f * (float)(num_opts - 1); // gaps between segments
        float seg_w = (bar_w - total_gap) / (float)num_opts;

        for (size_t j = 0; j < num_opts; j++) {
          float sx = bar_x + (float)j * (seg_w + 4.0f);
          bool is_active = (j == setting.option_idx);
          afterhours::Color seg_color =
              is_active ? accent_green : slider_track;
          afterhours::Color seg_border =
              is_active ? accent_green : slider_empty_border;

          div(context,
              mk(entity, 800 + static_cast<int>(i) * 20 + static_cast<int>(j)),
              ComponentConfig{}
                  .with_720p_size(seg_w, dot_h)
                  .with_absolute_position(sx, bar_y)
                  .with_custom_background(seg_color)
                  .with_border(seg_border, 1.0f)
                  .with_skip_tabbing(true)
                  .with_debug_name("seg_" + std::to_string(i) + "_" +
                                   std::to_string(j)));
        }
      }
    }

    // ========== RIGHT PANEL: Help/Info ==========
    float help_x = panel_x + panel_w + 100.0f;
    float help_y = 85.0f;
    float help_w = 350.0f;

    auto &selected_setting = current_settings[selected_row];
    std::string current_val =
        selected_setting.is_slider
            ? format_slider_value(selected_row, selected_setting.slider_pct)
            : (selected_setting.options.empty()
                   ? "---"
                   : selected_setting.options[selected_setting.option_idx]);

    // Get dynamic descriptions (with tooltips for abbreviations like TSR)
    std::string desc_line1 =
        get_setting_description(selected_setting.label, current_val);
    std::string desc_line2 =
        get_setting_description_line2(selected_setting.label, current_val);

    // Help text lines
    struct HelpLine { int id; const char *text; float y_off; int h; float font; afterhours::Color color; };
    std::string current_label = "Current: " + current_val;
    HelpLine help_lines[] = {
        {700, selected_setting.label.c_str(), 0.0f, 35, 22.0f, text_white},
        {701, desc_line1.c_str(), 40.0f, 50, 0.0f, text_white},
        {702, desc_line2.c_str(), 70.0f, 30, 0.0f, text_white},
        {703, current_label.c_str(), 110.0f, 30, 0.0f, text_muted},
    };
    for (auto &hl : help_lines) {
      auto cfg = ComponentConfig{}
          .with_label(hl.text)
          .with_size(ComponentSize{pxf(help_w), pixels(hl.h)})
          .with_absolute_position(help_x, help_y + hl.y_off)
          .with_custom_text_color(hl.color);
      if (hl.font > 0.0f) cfg.with_font("EqProRounded", h720(hl.font));
      div(context, mk(entity, hl.id), cfg);
    }

    // ========== BOTTOM BUTTON PROMPTS ==========
    float prompt_y = (float)screen_h - 40.0f;
    float prompt_x = (float)screen_w - 330.0f;

    // Bottom button prompts
    struct BtnPrompt {
      const char *btn; const char *action; int base_id;
      afterhours::Color bg; afterhours::Color text; float offset; int label_w;
    };
    BtnPrompt btn_prompts[] = {
        {"Y", "Reset to default", 400, {180, 160, 60, 255}, bg_dark, 0.0f, 130},
        {"B", "Back", 402, {180, 80, 80, 255}, text_white, 175.0f, 50},
    };
    for (auto &bp : btn_prompts) {
      div(context, mk(entity, bp.base_id),
          ComponentConfig{}
              .with_label(bp.btn)
              .with_size(ComponentSize{pixels(28), pixels(28)})
              .with_absolute_position(prompt_x + bp.offset, prompt_y)
              .with_custom_background(bp.bg)
              .with_custom_text_color(bp.text)
              .with_alignment(TextAlignment::Center)
              .with_rounded_corners(RoundedCorners())
              .with_roundness(1.0f));
      div(context, mk(entity, bp.base_id + 1),
          ComponentConfig{}
              .with_label(bp.action)
              .with_size(ComponentSize{pixels(bp.label_w), pixels(25)})
              .with_absolute_position(prompt_x + bp.offset + 35.0f, prompt_y + 2.0f)
              .with_custom_text_color(text_white));
    }
  }
};

REGISTER_EXAMPLE_SCREEN(sports_settings, "Game Mockups",
                        "Sports game settings (FIFA/Rematch style)",
                        SportsSettingsScreen)
