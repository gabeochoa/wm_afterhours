#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/setting_row.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// An offsite backup client: one ring for the whole job, a bar per folder, and a
// settings column. Built for the setting_row variants that had no caller
// anywhere (slider, dropdown, display) and for their icon parameter, which
// nothing passed either.
struct OffsiteBackupApp : ScreenSystem<UIContext<InputAction>> {
  struct Folder {
    const char *name;
    const char *size;
    float target;
  };

  static constexpr Folder folders[5] = {
      {"Documents", "12.4 GB", 1.00f}, {"Photos", "184.2 GB", 0.71f},
      {"Projects", "46.8 GB", 1.00f},  {"Music", "31.0 GB", 0.34f},
      {"Archive", "512.6 GB", 0.08f},
  };

  // Seeded AT the target rather than easing up from zero: headless renders two
  // frames, so anything still in flight would be caught mid-animation and the
  // baseline would move. Rescan drops them to zero, which is when it animates.
  float overall = 0.f;
  float folder_now[5] = {0.f, 0.f, 0.f, 0.f, 0.f};
  bool seeded = false;

  float bandwidth = 0.62f;
  size_t schedule_idx = 1;
  size_t retention_idx = 2;
  bool pause_on_battery = true;
  bool backup_externals = false;

  std::vector<std::string> schedules = {"Hourly", "4 hours", "Daily", "Weekly"};
  std::vector<std::string> retentions = {"7 days", "30 days", "1 year",
                                         "Forever"};

  // Converge, then stop exactly, or the resting frame drifts a pixel forever.
  static void ease(float &now, float target, float dt) {
    now += (target - now) * std::min(1.f, dt * 3.f);
    if (std::fabs(target - now) < 0.001f)
      now = target;
  }

  int complete_count() const {
    int n = 0;
    for (int i = 0; i < 5; i++)
      if (folder_now[i] >= 1.f)
        n++;
    return n;
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float dt) override {
    auto theme = afterhours::ui::theme_presets::ocean_navy();
    theme.roundness = 0.10f;
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;

    const auto ink = afterhours::Color{232, 238, 248, 255};
    const auto muted = afterhours::Color{168, 182, 206, 255};
    const auto panel = afterhours::Color{30, 42, 64, 255};
    const auto track = afterhours::Color{22, 31, 48, 255};
    const auto done_col = afterhours::Color{120, 210, 155, 255};
    const auto busy_col = afterhours::Color{95, 165, 235, 255};

    if (!seeded) {
      seeded = true;
      for (int i = 0; i < 5; i++)
        folder_now[i] = folders[i].target;
      overall = 0.626f;
    }

    float sum = 0.f;
    for (int i = 0; i < 5; i++) {
      ease(folder_now[i], folders[i].target, dt);
      sum += folder_now[i];
    }
    ease(overall, sum / 5.f, dt);

    const float total_gb = 787.0f;

    auto root =
        hstack(context, mk(entity, 0),
               ComponentConfig{}
                   .with_size(ComponentSize{screen_pct(0.94f), screen_pct(0.92f)})
                   .with_self_align(SelfAlign::Center)
                   .with_background(Theme::Usage::Background)
                   .with_padding(Spacing::md)
                   .with_no_wrap()
                   .with_debug_name("bk_root"));

    // ---- left: the job -------------------------------------------------
    auto left =
        vstack(context, mk(root.ent(), 0),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(0.56f), percent(1.f)})
                   .with_custom_background(panel)
                   .with_padding(Spacing::md)
                   .with_corner_radius(12.f)
                   .with_margin(Margin{.right = DefaultSpacing::small()})
                   .with_no_wrap()
                   .with_debug_name("bk_left"));

    div(context, mk(left.ent(), 0),
        ComponentConfig{}
            .with_label("Backing up to offsite storage")
            .with_size(ComponentSize{percent(1.f), pixels(32)})
            .with_custom_text_color(ink)
            .with_font_size(pixels(21.f))
            .with_debug_name("bk_title"));

    auto ring_row =
        hstack(context, mk(left.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(1.f), pixels(150)})
                   .with_align_items(AlignItems::Center)
                   .with_no_wrap()
                   .with_debug_name("bk_ring_row"));

    circular_progress(context, mk(ring_row.ent(), 0), overall,
                      ComponentConfig{}
                          .with_size(ComponentSize{pixels(120), pixels(120)})
                          .with_custom_background(busy_col)
                          .with_border(track, pixels(12.f))
                          .with_debug_name("bk_ring"));

    auto ring_txt =
        vstack(context, mk(ring_row.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{expand(), pixels(140)})
                   .with_justify_content(JustifyContent::Center)
                   .with_margin(Margin{.left = DefaultSpacing::medium()})
                   .with_debug_name("bk_ring_txt"));

    // Real byte counts, so the ring can be checked rather than trusted.
    div(context, mk(ring_txt.ent(), 0),
        ComponentConfig{}
            .with_label(fmt::format("{:.0f} of {:.0f} GB uploaded",
                                    overall * total_gb, total_gb))
            .with_size(ComponentSize{percent(1.f), pixels(28)})
            .with_custom_text_color(ink)
            .with_font_size(pixels(19.f))
            .with_debug_name("bk_bytes"));

    div(context, mk(ring_txt.ent(), 1),
        ComponentConfig{}
            .with_label(
                fmt::format("{} of 5 folders complete", complete_count()))
            .with_size(ComponentSize{percent(1.f), pixels(26)})
            .with_custom_text_color(muted)
            .with_font_size(pixels(17.f))
            .with_debug_name("bk_folders_done"));

    // The only way to watch the easing: drop to zero and let it climb back.
    if (button(context, mk(ring_txt.ent(), 2),
               ComponentConfig{}
                   .with_label("Rescan")
                   .with_size(ComponentSize{pixels(120), pixels(40)})
                   .with_margin(Margin{.top = DefaultSpacing::tiny()})
                   .with_debug_name("bk_rescan"))) {
      overall = 0.f;
      for (int i = 0; i < 5; i++)
        folder_now[i] = 0.f;
    }

    div(context, mk(left.ent(), 2),
        ComponentConfig{}
            .with_label("PER FOLDER")
            .with_size(ComponentSize{percent(1.f), pixels(28)})
            .with_custom_text_color(muted)
            .with_font_size(pixels(16.f))
            .with_margin(Margin{.top = DefaultSpacing::small()})
            .with_debug_name("bk_folders_hdr"));

    for (int i = 0; i < 5; i++) {
      auto row = hstack(context, mk(left.ent(), 10 + i),
                        ComponentConfig{}
                            .with_size(ComponentSize{percent(1.f), pixels(40)})
                            .with_align_items(AlignItems::Center)
                            .with_no_wrap()
                            .with_debug_name(fmt::format("bk_row_{}", i)));

      div(context, mk(row.ent(), 0),
          ComponentConfig{}
              .with_label(folders[i].name)
              .with_size(ComponentSize{pixels(110), pixels(26)})
              .with_custom_text_color(ink)
              .with_font_size(pixels(17.f))
              .with_debug_name(fmt::format("bk_name_{}", i)));

      // Fixed width, not expand(): under expand() the fill resolves to zero
      // and every bar renders as a full track. Colour comes from the theme, so
      // per-bar state reads out of the trailing label instead.
      progress_bar(context, mk(row.ent(), 1), folder_now[i],
                   ComponentConfig{}
                       .with_size(ComponentSize{pixels(220), pixels(18)})
                       .with_debug_name(fmt::format("bk_bar_{}", i)),
                   ProgressBarLabelStyle::None);

      div(context, mk(row.ent(), 2),
          ComponentConfig{}
              .with_label(folder_now[i] >= 1.f
                              ? std::string("done")
                              : fmt::format("{:.0f}%", folder_now[i] * 100.f))
              .with_size(ComponentSize{pixels(56), pixels(26)})
              .with_alignment(TextAlignment::Right)
              .with_custom_text_color(folder_now[i] >= 1.f ? done_col : ink)
              .with_font_size(pixels(16.f))
              .with_margin(Margin{.left = DefaultSpacing::small()})
              .with_debug_name(fmt::format("bk_pct_{}", i)));

      div(context, mk(row.ent(), 3),
          ComponentConfig{}
              .with_label(folders[i].size)
              .with_size(ComponentSize{pixels(92), pixels(26)})
              .with_alignment(TextAlignment::Right)
              .with_custom_text_color(muted)
              .with_font_size(pixels(16.f))
              .with_margin(Margin{.left = DefaultSpacing::small()})
              .with_debug_name(fmt::format("bk_size_{}", i)));
    }

    // ---- right: settings ------------------------------------------------
    auto right =
        vstack(context, mk(root.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{percent(0.42f), percent(1.f)})
                   .with_custom_background(panel)
                   .with_padding(Spacing::md)
                   .with_corner_radius(12.f)
                   .with_no_wrap()
                   .with_debug_name("bk_right"));

    const auto section = [&](int id, const char *text, bool first) {
      div(context, mk(right.ent(), id),
          ComponentConfig{}
              .with_label(text)
              .with_size(ComponentSize{percent(1.f), pixels(28)})
              .with_custom_text_color(muted)
              .with_font_size(pixels(16.f))
              .with_margin(Margin{.top = first ? Size{}
                                               : DefaultSpacing::small()})
              .with_debug_name(fmt::format("bk_sec_{}", id)));
    };

    section(0, "SCHEDULE", true);
    setting_row_dropdown(context, mk(right.ent(), 1), "Run backup",
                         schedule_idx, schedules, "T", busy_col);
    setting_row_dropdown(context, mk(right.ent(), 2), "Keep versions",
                         retention_idx, retentions, "V",
                         afterhours::Color{150, 130, 220, 255});

    section(3, "TRANSFER", false);
    setting_row_slider(context, mk(right.ent(), 4), "Bandwidth cap", bandwidth,
                       "B", done_col);
    setting_row_toggle(context, mk(right.ent(), 5), "Pause on battery",
                       pause_on_battery);
    setting_row_toggle(context, mk(right.ent(), 6), "Include externals",
                       backup_externals);

    section(7, "ACCOUNT", false);
    setting_row_display(context, mk(right.ent(), 8), "Last completed",
                        "2 hours ago", "C", done_col);
    setting_row_display(context, mk(right.ent(), 9), "Plan", "787 GB of 2 TB",
                        "P", afterhours::Color{225, 165, 45, 255});

    // Bandwidth is the one continuous setting, so it gets the live readout.
    div(context, mk(right.ent(), 10),
        ComponentConfig{}
            .with_label(fmt::format("cap {:.0f}% is {:.1f} MB/s",
                                    bandwidth * 100.f, bandwidth * 40.f))
            .with_size(ComponentSize{percent(1.f), pixels(26)})
            .with_custom_text_color(muted)
            .with_font_size(pixels(16.f))
            .with_margin(Margin{.top = DefaultSpacing::small()})
            .with_debug_name("bk_readout"));
  }
};

REGISTER_EXAMPLE_SCREEN(offsite_backup, "App Mockups",
                        "Backup client: ring, per-folder bars, setting rows",
                        OffsiteBackupApp)
