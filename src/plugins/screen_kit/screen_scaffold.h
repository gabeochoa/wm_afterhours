#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../gallery_shell/ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/files.h>
#include <algorithm>
#include <array>
#include <optional>
#include <string>

// The preamble ~140 screens repeat: set the theme, fix the scaling mode,
// derive the 720p scale, paint a full-screen background, and build on a
// centred 1280x720 stage. begin_screen does that once; screens keep only
// what is actually theirs (inner cards, hand-tuned colors).
//
// Internals follow DefaultTheme.h (Theme::Usage background, screen_pct
// stage, set_theme). Sizes stay pixels(v * s) -- see layout() for why
// with_720p_size cannot replace the box lambdas on letterboxed screens.

struct ScreenOptions {
  // Background painted as a flat color instead of Theme::Usage::Background.
  std::optional<afterhours::Color> background;
  // Text colors instead of theme.font / theme.font_muted.
  std::optional<afterhours::Color> ink;
  std::optional<afterhours::Color> muted;
  const char *bg_name = "bg";
};

struct ScreenFrame {
  float s = 1.f; // min(screen_width / 1280, screen_height / 720)
  afterhours::ui::imm::ElementResult root;
  afterhours::Color ink{};
  afterhours::Color muted{};

  // Replaces the per-screen box lambda: size + absolute position in 720p
  // design units, sharp corners, no background (call sites chain styling).
  // Deliberately pixels(v * s), not with_720p_size: w1280/h720 scale each
  // axis independently, while these screens letterbox by the min() scale --
  // at 4:3 the 720p sizes outgrow their parents and the e2e containment
  // assertions fail. pixels() is also the one dim autolayout never snaps
  // to its 4px grid (850 -> 852), so glyphs stay where the screens put them.
  afterhours::ui::imm::ComponentConfig layout(float x, float y, float w,
                                         float h) const {
    return afterhours::ui::imm::ComponentConfig{}
        .with_size({afterhours::ui::pixels(w * s),
                    afterhours::ui::pixels(h * s)})
        .with_absolute_position(x * s, y * s)
        .with_background(afterhours::ui::Theme::Usage::None)
        .with_corner_radius(0)
        .with_skip_grid_snap();
  }
};

inline ScreenFrame begin_screen(afterhours::ui::UIContext<InputAction> &ctx,
                                afterhours::Entity &entity,
                                const afterhours::ui::Theme &theme,
                                const ScreenOptions &opts = {}) {
  using namespace afterhours::ui;
  // set_theme, not assignment: sizing reads metrics through ThemeDefaults.
  ctx.set_theme(theme);
  ctx.scaling_mode = ScalingMode::Proportional;
  const float s =
      std::min(ctx.screen_width / 1280.f, ctx.screen_height / 720.f);
  auto bg_config = imm::ComponentConfig{}
                       .with_size({screen_pct(1.f), screen_pct(1.f)})
                       .with_absolute_position(0, 0)
                       .with_corner_radius(0)
                       .with_skip_grid_snap()
                       .with_debug_name(opts.bg_name);
  if (opts.background)
    bg_config.with_custom_background(*opts.background);
  else
    bg_config.with_background(Theme::Usage::Background);
  imm::div(ctx, mk(entity), bg_config);
  auto root = imm::div(ctx, mk(entity),
                       imm::ComponentConfig{}
                           .with_size({screen_pct(1.f), screen_pct(1.f)})
                           .with_absolute_position(0, 0)
                           .with_background(Theme::Usage::None)
                           .with_corner_radius(0)
                           .with_skip_grid_snap()
                           .with_debug_name("screen_root"));
  return ScreenFrame{.s = s,
                     .root = root,
                     .ink = opts.ink.value_or(theme.font),
                     .muted = opts.muted.value_or(theme.font_muted)};
}

// The art loop screens repeat (AngryBirdsSettings::load_art): one texture
// per name, folder + name + ".png", bilinear. Per-screen subfolder quirks
// stay at the call site.
template <size_t N>
void load_art(std::array<raylib::Texture2D, N> &art,
              const std::array<const char *, N> &names,
              const std::string &folder) {
  for (size_t i = 0; i < N; ++i) {
    const std::string path =
        afterhours::files::get_resource_path("images", folder + names[i] +
                                                           ".png")
            .string();
    art[i] = raylib::LoadTexture(path.c_str());
    raylib::SetTextureFilter(art[i], raylib::TEXTURE_FILTER_BILINEAR);
  }
}
