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
  afterhours::Color slider_track{22, 28, 38, 255}; // Slider track
  afterhours::Color slider_empty_border{55, 65, 80, 255}; // Border for empty portion

  std::vector<std::string> tabs = {"GAMEPLAY", "VIDEO", "AUDIO", "CONTROLS"};

  struct SettingRow {
    std::string label;
    bool is_slider;
    float slider_pct;
    int min_val;
    int max_val;
    std::vector<std::string> options;
    size_t option_idx;
  };

  std::vector<SettingRow> graphics_settings = {
      {"Window mode", false, 0.0f, 0, 0, {"Fullscreen", "Borderless window", "Windowed"}, 1},
      {"Resolution", false, 0.0f, 0, 0, {"1920x1080", "2560x1440", "3840x2160"}, 1},
      {"Screen percentage", true, 1.0f, 50, 100, {}, 0},
      {"V Sync", false, 0.0f, 0, 0, {"Disabled", "Enabled"}, 1},
      {"Max FPS", true, 0.5f, 30, 240, {}, 0},
      {"Gamma", true, 0.45f, 10, 30, {}, 0},
      {"Anti-Aliasing method", false, 0.0f, 0, 0,
       {"Off", "FXAA", "TAA", "TSR (Temporal Super Resolution)", "DLSS"}, 3},
      {"Dynamic resolution", false, 0.0f, 0, 0, {"Disabled", "Enabled"}, 1},
      {"Framerate target", true, 0.4f, 30, 120, {}, 0},
      {"Motion blur", true, 0.7f, 0, 10, {}, 0},
      {"Graphics quality", false, 0.0f, 0, 0, {"Low", "Medium", "High", "Ultra"}, 3},
      {"Texture quality", false, 0.0f, 0, 0, {"Low", "Medium", "High", "Ultra"}, 3},
  };

  std::vector<SettingRow> gameplay_settings = {
      {"Difficulty", false, 0.0f, 0, 0,
       {"Beginner", "Amateur", "Semi-Pro", "Professional", "World Class", "Legendary"}, 3},
      {"Game Speed", false, 0.0f, 0, 0, {"Slow", "Normal", "Fast"}, 1},
      {"Match Length", false, 0.0f, 0, 0,
       {"4 min", "6 min", "8 min", "10 min", "15 min", "20 min"}, 2},
      {"Camera Type", false, 0.0f, 0, 0,
       {"Broadcast", "Co-op", "Dynamic", "End to End", "Pro"}, 0},
      {"Camera Height", true, 0.5f, 0, 20, {}, 0},
      {"Camera Zoom", true, 0.5f, 0, 20, {}, 0},
      {"Ball Indicator", false, 0.0f, 0, 0, {"Off", "On"}, 1},
      {"Player Names", false, 0.0f, 0, 0, {"Off", "Always", "When Controlled"}, 2},
  };

  std::vector<SettingRow> audio_settings = {
      {"Master Volume", true, 0.8f, 0, 100, {}, 0},
      {"Music Volume", true, 0.7f, 0, 100, {}, 0},
      {"Commentary Volume", true, 0.9f, 0, 100, {}, 0},
      {"Crowd Volume", true, 0.85f, 0, 100, {}, 0},
      {"SFX Volume", true, 0.75f, 0, 100, {}, 0},
      {"Commentary Language", false, 0.0f, 0, 0,
       {"English", "Spanish", "French", "German", "Italian"}, 0},
      {"Crowd Chants", false, 0.0f, 0, 0, {"Off", "On"}, 1},
  };

  std::vector<SettingRow> controls_settings = {
      {"Vibration", false, 0.0f, 0, 0, {"Off", "On"}, 1},
      {"Vibration Strength", true, 0.7f, 0, 100, {}, 0},
      {"Auto Switch", false, 0.0f, 0, 0, {"Off", "Ball Only", "Air Balls", "All"}, 2},
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

  const char *get_group_header(size_t tab, size_t row) {
    if (tab == 0) {
      if (row == 0) return "Match";
      if (row == 4) return "Camera";
      if (row == 6) return "Display";
    } else if (tab == 1) {
      if (row == 0) return "Display";
      if (row == 6) return "Anti-Aliasing & Performance";
      if (row == 10) return "Quality";
    } else if (tab == 2) {
      if (row == 0) return "Volume";
      if (row == 5) return "Options";
    } else if (tab == 3) {
      if (row == 0) return "Vibration";
      if (row == 2) return "Assistance";
    }
    return nullptr;
  }

  std::string get_setting_description(const std::string &label,
                                      const std::string &current_value) {
    if (label == "Anti-Aliasing method") {
      if (current_value == "TSR (Temporal Super Resolution)")
        return "TSR (Temporal Super Resolution) uses";
      if (current_value == "TAA") return "TAA (Temporal Anti-Aliasing) smooths";
      if (current_value == "FXAA") return "FXAA (Fast Approximate AA) is a";
      if (current_value == "DLSS") return "DLSS (Deep Learning Super Sampling)";
    }
    return "Adjust this setting to customize your";
  }

  std::string get_setting_description_line2(const std::string &label,
                                            const std::string &current_value) {
    if (label == "Anti-Aliasing method") {
      if (current_value == "TSR (Temporal Super Resolution)")
        return "AI to upscale lower resolution frames.";
      if (current_value == "TAA") return "edges using temporal frame data.";
      if (current_value == "FXAA") return "fast, low-cost edge smoothing method.";
      if (current_value == "DLSS") return "uses NVIDIA AI for quality upscaling.";
    }
    return "gaming experience.";
  }

  std::string format_slider_value(size_t index, float pct) {
    auto &settings = get_current_settings();
    if (index >= settings.size()) return "---";
    auto &setting = settings[index];
    int val = setting.min_val +
              static_cast<int>(pct * (setting.max_val - setting.min_val));
    if (active_tab == 1 && index == 5) {
      return fmt::format("{:.1f}", val / 10.0f);
    }
    std::string label = setting.label;
    if (label == "Max FPS" || label == "Framerate target")
      return std::to_string(val) + " FPS";
    if (label.find("Volume") != std::string::npos ||
        label == "Screen percentage" || label == "Vibration Strength")
      return std::to_string(val) + "%";
    return std::to_string(val);
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    // Adaptive scaling: pixels() scales with ui_scale, layout reflows on resize
    context.scaling_mode = ScalingMode::Adaptive;
    UIStylingDefaults::get().set_scaling_mode(ScalingMode::Adaptive);
    UIStylingDefaults::get().set_default_font("EqProRounded", pixels(19.0f));
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

    auto *pcr = afterhours::EntityHelper::get_singleton_cmp<
        afterhours::window_manager::ProvidesCurrentResolution>();
    int screen_w = pcr ? pcr->width() : Settings::get().get_screen_width();
    int screen_h = pcr ? pcr->height() : Settings::get().get_screen_height();
    float sw = static_cast<float>(screen_w);
    float sh = static_cast<float>(screen_h);

    // Use logical width for breakpoint (respects ui_scale zoom)
    auto layout_info = LayoutInfo::make(sw, sh, theme.ui_scale,
                                        ScalingMode::Adaptive);
    bool show_help_panel = layout_info.logical_w >= 900;

    // ── Sync selection with focus ──
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

    // ── Keyboard input ──
    if (selected_row < current_settings.size()) {
      auto &setting = current_settings[selected_row];
      float step = 0.05f;
      if (context.pressed(InputAction::WidgetLeft)) {
        if (setting.is_slider)
          setting.slider_pct = std::max(0.0f, setting.slider_pct - step);
        else if (!setting.options.empty())
          setting.option_idx = (setting.option_idx == 0)
                                   ? setting.options.size() - 1
                                   : setting.option_idx - 1;
      }
      if (context.pressed(InputAction::WidgetRight)) {
        if (setting.is_slider)
          setting.slider_pct = std::min(1.0f, setting.slider_pct + step);
        else if (!setting.options.empty())
          setting.option_idx = (setting.option_idx + 1) % setting.options.size();
      }
    }

    if (selected_row >= current_settings.size()) selected_row = 0;

    // ── Layout dimensions ──
    // Top-level regions use screen_pct for proportional layout;
    // inner elements use pixels() which scales with ui_scale in Adaptive mode
    float tab_area_h = 50.f / sh;    // tab bar region
    float hdr_area_h = 45.f / sh;    // section header region
    float bottom_h = 45.f / sh;      // bottom prompt bar
    float content_h = 1.0f - tab_area_h - hdr_area_h - bottom_h;
    float pad_l = 50.f / sw;
    float pad_r = 20.f / sw;
    // Settings panel: ~50% when help visible, full width otherwise
    float settings_w_pct = show_help_panel
        ? (620.f / sw)
        : (1.0f - pad_l - pad_r);
    float help_gap_pct = 50.f / sw;
    float help_w_pct = show_help_panel
        ? (1.0f - pad_l - pad_r - settings_w_pct - help_gap_pct)
        : 0.f;

    // ═══════════════════════════════════════════════════════════════
    // ROOT
    // ═══════════════════════════════════════════════════════════════
    auto root = vstack(
        context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(1.0f), screen_pct(1.0f)})
            .with_custom_background(bg_dark)
            .with_no_wrap()
            .with_debug_name("sports_root"));

    // ═══════════════════════════════════════════════════════════════
    // TAB BAR
    // ═══════════════════════════════════════════════════════════════
    auto tab_area = hstack(
        context, mk(root.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), screen_pct(tab_area_h)})
            .with_padding(Padding{.top = pixels(15), .left = pixels(70),
                                  .bottom = {}, .right = {}})
            .with_align_items(AlignItems::FlexEnd)
            .with_no_wrap()
            .with_debug_name("tab_area"));

    tab_container(context, mk(tab_area.ent(), 0), tabs, active_tab,
        ComponentConfig{}
            .with_size(ComponentSize{pixels(520), pixels(35)}));

    // ═══════════════════════════════════════════════════════════════
    // SECTION HEADER
    // ═══════════════════════════════════════════════════════════════
    auto hdr = vstack(
        context, mk(root.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), screen_pct(hdr_area_h)})
            .with_padding(Padding{.top = pixels(10), .left = pixels(50),
                                  .bottom = {}, .right = {}})
            .with_no_wrap()
            .with_debug_name("header_section"));

    div(context, mk(hdr.ent(), 0),
        ComponentConfig{}
            .with_label(get_section_header())
            .with_size(ComponentSize{pixels(200), pixels(28)})
            .with_font("EqProRounded", pixels(20.0f))
            .with_custom_text_color(text_white));

    div(context, mk(hdr.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(520), pixels(1)})
            .with_custom_background(afterhours::Color{85, 195, 145, 60})
            .with_margin(Margin{.top = pixels(4)})
            .with_debug_name("section_sep"));

    // ═══════════════════════════════════════════════════════════════
    // CONTENT (settings + help side-by-side)
    // ═══════════════════════════════════════════════════════════════
    auto content = hstack(
        context, mk(root.ent(), 3),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), screen_pct(content_h)})
            .with_padding(Padding{.top = {}, .left = screen_pct(pad_l),
                                  .bottom = {}, .right = screen_pct(pad_r)})
            .with_align_items(AlignItems::FlexStart)
            .with_no_wrap()
            .with_debug_name("content"));

    // ── Settings panel ──────────────────────────────────────────
    auto settings_panel = vstack(
        context, mk(content.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{screen_pct(settings_w_pct),
                                     percent(1.0f)})
            .with_no_wrap()
            .with_debug_name("settings_panel"));

    int child_id = 0;
    for (size_t i = 0; i < current_settings.size(); i++) {
      // ── Group headers ──
      const char *group_label = get_group_header(active_tab, i);
      if (group_label) {
        if (i > 0) {
          // Group separator line
          div(context, mk(settings_panel.ent(), child_id++),
              ComponentConfig{}
                  .with_size(ComponentSize{percent(0.95f), pixels(1)})
                  .with_custom_background(afterhours::Color{85, 195, 145, 40})
                  .with_margin(Margin{.top = pixels(8)})
                  .with_skip_tabbing(true)
                  .with_debug_name("group_sep"));
        }
        div(context, mk(settings_panel.ent(), child_id++),
            ComponentConfig{}
                .with_label(group_label)
                .with_size(ComponentSize{pixels(300), pixels(22)})
                .with_font("EqProRounded", pixels(14.0f))
                .with_custom_text_color(afterhours::Color{85, 195, 145, 180})
                .with_margin(Margin{.left = pixels(20), .bottom = pixels(2)})
                .with_skip_tabbing(true)
                .with_debug_name("group_lbl"));
      }

      bool is_selected = (i == selected_row);
      auto &setting = current_settings[i];

      std::string display_value;
      if (setting.is_slider)
        display_value = format_slider_value(i, setting.slider_pct);
      else if (!setting.options.empty())
        display_value = setting.options[setting.option_idx];
      else
        display_value = "---";

      afterhours::Color label_color = is_selected ? text_white : text_muted;
      afterhours::Color value_color = is_selected ? accent_green : text_muted;
      afterhours::Color arrow_color = is_selected ? text_white : text_muted;
      float step = 0.05f;

      // ── Setting row (hstack) ──
      auto row = hstack(
          context, mk(settings_panel.ent(), child_id++),
          ComponentConfig{}
              .with_size(ComponentSize{percent(1.0f), pixels(40)})
              .with_custom_background(is_selected ? highlight_row
                                                  : afterhours::Color{35, 45, 55, 255})
              .with_align_items(AlignItems::Center)
              .with_no_wrap()
              .with_margin(Margin{.bottom = pixels(2)})
              .with_debug_name("row_" + std::to_string(i)));

      // Selection accent bar
      div(context, mk(row.ent(), 0),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(4), pixels(32)})
              .with_custom_background(
                  is_selected ? accent_green : afterhours::Color{0, 0, 0, 0})
              .with_margin(Margin{.right = pixels(12)}));

      // Label (tabbable button)
      if (button(context, mk(row.ent(), 1),
                 ComponentConfig{}
                     .with_label(setting.label)
                     .with_size(ComponentSize{pixels(175), percent(1.0f)})
                     .with_font("EqProRounded", pixels(18.0f))
                     .with_custom_text_color(label_color)
                     .with_custom_background(afterhours::Color{0, 0, 0, 0})
                     .with_alignment(TextAlignment::Left)
                     .with_padding(Padding{.left = pixels(4)})
                     .with_debug_name("label_" + std::to_string(i)))) {
        selected_row = i;
      }

      // Left arrow <
      if (button(context, mk(row.ent(), 2),
                 ComponentConfig{}
                     .with_label("<")
                     .with_size(ComponentSize{pixels(40), pixels(40)})
                     .with_font("EqProRounded", pixels(20.0f))
                     .with_custom_text_color(arrow_color)
                     .with_custom_background(afterhours::Color{0, 0, 0, 0})
                     .with_alignment(TextAlignment::Center)
                     .with_skip_tabbing(true)
                     .with_debug_name("left_" + std::to_string(i)))) {
        selected_row = i;
        if (setting.is_slider)
          setting.slider_pct = std::max(0.0f, setting.slider_pct - step);
        else if (!setting.options.empty())
          setting.option_idx = (setting.option_idx == 0)
                                   ? setting.options.size() - 1
                                   : setting.option_idx - 1;
      }

      // Value display
      div(context, mk(row.ent(), 3),
          ComponentConfig{}
              .with_label(display_value)
              .with_size(ComponentSize{pixels(240), percent(1.0f)})
              .with_font("EqProRounded", pixels(16.0f))
              .with_custom_text_color(value_color)
              .with_alignment(TextAlignment::Center)
              .with_skip_tabbing(true)
              .with_debug_name("value_" + std::to_string(i)));

      // Right arrow >
      if (button(context, mk(row.ent(), 4),
                 ComponentConfig{}
                     .with_label(">")
                     .with_size(ComponentSize{pixels(40), pixels(40)})
                     .with_font("EqProRounded", pixels(20.0f))
                     .with_custom_text_color(arrow_color)
                     .with_custom_background(afterhours::Color{0, 0, 0, 0})
                     .with_alignment(TextAlignment::Center)
                     .with_skip_tabbing(true)
                     .with_debug_name("right_" + std::to_string(i)))) {
        selected_row = i;
        if (setting.is_slider)
          setting.slider_pct = std::min(1.0f, setting.slider_pct + step);
        else if (!setting.options.empty())
          setting.option_idx = (setting.option_idx + 1) % setting.options.size();
      }

      // ── Bar area: slider or segments ──
      if (setting.is_slider) {
        // Slider: background container with fill child
        auto bar_bg = div(
            context, mk(row.ent(), 5),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(100), pixels(14)})
                .with_custom_background(slider_track)
                .with_border(slider_empty_border, 1.0f)
                .with_skip_tabbing(true)
                .with_debug_name("slider_bg_" + std::to_string(i)));

        float fill_pct = setting.slider_pct;
        if (fill_pct > 0.02f) {
          div(context, mk(bar_bg.ent(), 0),
              ComponentConfig{}
                  .with_size(ComponentSize{percent(fill_pct), percent(1.0f)})
                  .with_custom_background(accent_green)
                  .with_skip_tabbing(true)
                  .with_debug_name("slider_fill_" + std::to_string(i)));
        }
      } else if (setting.options.size() > 1) {
        // Segments: hstack with equal-weight children
        auto segs = hstack(
            context, mk(row.ent(), 5),
            ComponentConfig{}
                .with_size(ComponentSize{pixels(100), pixels(10)})
                .with_align_items(AlignItems::Center)
                .with_no_wrap()
                .with_skip_tabbing(true)
                .with_debug_name("segs_" + std::to_string(i)));

        for (size_t j = 0; j < setting.options.size(); j++) {
          bool is_active = (j == setting.option_idx);
          div(context,
              mk(segs.ent(), static_cast<int>(j)),
              ComponentConfig{}
                  .with_size(ComponentSize{expand(1.f), percent(1.0f)})
                  .with_custom_background(is_active ? accent_green : slider_track)
                  .with_border(is_active ? accent_green : slider_empty_border, 1.0f)
                  .with_skip_tabbing(true)
                  .with_debug_name("seg_" + std::to_string(i) + "_" +
                                   std::to_string(j)));
        }
      }
    } // end settings loop

    // ═══════════════════════════════════════════════════════════════
    // HELP PANEL (right side, conditional)
    // ═══════════════════════════════════════════════════════════════
    if (show_help_panel) {
      auto &selected_setting = current_settings[selected_row];
      std::string current_val =
          selected_setting.is_slider
              ? format_slider_value(selected_row, selected_setting.slider_pct)
              : (selected_setting.options.empty()
                     ? "---"
                     : selected_setting.options[selected_setting.option_idx]);

      auto help = vstack(
          context, mk(content.ent(), 1),
          ComponentConfig{}
              .with_size(ComponentSize{screen_pct(help_w_pct), percent(1.0f)})
              .with_margin(Margin{.left = screen_pct(help_gap_pct)})
              .with_padding(Padding{.top = pixels(5)})
              .with_no_wrap()
              .with_debug_name("help_panel"));

      std::string desc1 = get_setting_description(selected_setting.label, current_val);
      std::string desc2 = get_setting_description_line2(selected_setting.label, current_val);
      std::string current_label = "Current: " + current_val;

      div(context, mk(help.ent(), 0),
          ComponentConfig{}
              .with_label(selected_setting.label)
              .with_size(ComponentSize{percent(1.0f), pixels(35)})
              .with_font("EqProRounded", pixels(22.0f))
              .with_custom_text_color(text_white));

      div(context, mk(help.ent(), 1),
          ComponentConfig{}
              .with_label(desc1)
              .with_size(ComponentSize{percent(1.0f), pixels(30)})
              .with_custom_text_color(text_white)
              .with_margin(Margin{.top = pixels(8)}));

      div(context, mk(help.ent(), 2),
          ComponentConfig{}
              .with_label(desc2)
              .with_size(ComponentSize{percent(1.0f), pixels(25)})
              .with_custom_text_color(text_white));

      div(context, mk(help.ent(), 3),
          ComponentConfig{}
              .with_label(current_label)
              .with_size(ComponentSize{percent(1.0f), pixels(25)})
              .with_custom_text_color(text_muted)
              .with_margin(Margin{.top = pixels(10)}));
    }

    // ═══════════════════════════════════════════════════════════════
    // BOTTOM BAR (button prompts, right-aligned)
    // ═══════════════════════════════════════════════════════════════
    auto bottom = hstack(
        context, mk(root.ent(), 4),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), screen_pct(bottom_h)})
            .with_padding(Padding{.left = pixels(20), .right = pixels(20)})
            .with_align_items(AlignItems::Center)
            .with_justify_content(JustifyContent::FlexEnd)
            .with_no_wrap()
            .with_debug_name("bottom_bar"));

    // Y - Reset to default
    div(context, mk(bottom.ent(), 0),
        ComponentConfig{}
            .with_label("Y")
            .with_size(ComponentSize{pixels(28), pixels(28)})
            .with_custom_background(afterhours::Color{180, 160, 60, 255})
            .with_custom_text_color(bg_dark)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(1.0f));
    div(context, mk(bottom.ent(), 1),
        ComponentConfig{}
            .with_label("Reset to default")
            .with_size(ComponentSize{pixels(150), pixels(25)})
            .with_custom_text_color(text_white)
            .with_margin(Margin{.left = pixels(8), .right = pixels(30)}));

    // B - Back
    div(context, mk(bottom.ent(), 2),
        ComponentConfig{}
            .with_label("B")
            .with_size(ComponentSize{pixels(28), pixels(28)})
            .with_custom_background(afterhours::Color{180, 80, 80, 255})
            .with_custom_text_color(text_white)
            .with_alignment(TextAlignment::Center)
            .with_rounded_corners(RoundedCorners())
            .with_roundness(1.0f));
    div(context, mk(bottom.ent(), 3),
        ComponentConfig{}
            .with_label("Back")
            .with_size(ComponentSize{pixels(50), pixels(25)})
            .with_custom_text_color(text_white)
            .with_margin(Margin{.left = pixels(8)}));
  }
};

REGISTER_EXAMPLE_SCREEN(sports_settings, "Game Mockups",
                        "Sports game settings (FIFA/Rematch style)",
                        SportsSettingsScreen)
