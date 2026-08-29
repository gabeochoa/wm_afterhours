#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// A tunnel client, where one toggle drives every other colour on the screen.
// The right column renders the same status block in all three states at once,
// so the theming is legible without having to trigger each one.
struct SecureTunnelMockup : ScreenSystem<UIContext<InputAction>> {
  enum struct Link { Off, Dialing, Up };

  struct Server {
    const char *place;
    const char *tag;
    int ms;
  };

  bool wants_up = false;
  Link link = Link::Off;
  float dialing_for = 0.f;
  size_t server_idx = 1;
  afterhours::EntityID row_id[6] = {-1, -1, -1, -1, -1, -1};

  // Dialing settles into Up after this long, so the live screen animates but
  // never rests mid-transition.
  static constexpr float DIAL_SECONDS = 1.2f;

  static constexpr Server SERVERS[6] = {
      {"Reykjavik", "is-01", 14},  {"Lisbon", "pt-04", 27},
      {"Montreal", "ca-02", 41},   {"Fortaleza", "br-01", 96},
      {"Wellington", "nz-03", 178}, {"Cape Town", "za-01", 204},
  };

  static const char *state_word(Link l) {
    switch (l) {
    case Link::Off:
      return "Not connected";
    case Link::Dialing:
      return "Negotiating";
    case Link::Up:
      return "Tunnel up";
    }
    return "";
  }

  static afterhours::Color state_fill(Link l) {
    switch (l) {
    case Link::Off:
      return afterhours::Color{58, 62, 74, 255};
    case Link::Dialing:
      return afterhours::Color{122, 88, 24, 255};
    case Link::Up:
      return afterhours::Color{28, 92, 58, 255};
    }
    return afterhours::Color{58, 62, 74, 255};
  }

  static afterhours::Color state_ink(Link l) {
    switch (l) {
    case Link::Off:
      return afterhours::Color{198, 204, 218, 255};
    case Link::Dialing:
      return afterhours::Color{255, 224, 150, 255};
    case Link::Up:
      return afterhours::Color{168, 240, 196, 255};
    }
    return afterhours::Color{198, 204, 218, 255};
  }

  void advance(float dt) {
    if (!wants_up) {
      link = Link::Off;
      dialing_for = 0.f;
      return;
    }
    if (link == Link::Off) {
      link = Link::Dialing;
      dialing_for = 0.f;
    }
    if (link == Link::Dialing) {
      dialing_for += dt;
      if (dialing_for >= DIAL_SECONDS)
        link = Link::Up;
    }
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float dt) override {
    advance(dt);

    context.theme = afterhours::ui::theme_presets::midnight();
    context.scaling_mode = ScalingMode::Adaptive;

    const auto ink = afterhours::Color{232, 237, 246, 255};
    const auto muted = afterhours::Color{158, 168, 190, 255};
    const auto panel = afterhours::Color{30, 34, 46, 255};
    const auto row_fill = afterhours::Color{42, 47, 62, 255};
    const auto row_pick = afterhours::Color{58, 78, 112, 255};

    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(1280), pixels(720)})
            .with_absolute_position(0.f, 0.f)
            .with_custom_background(afterhours::Color{18, 20, 28, 255})
            .with_debug_name("tn_bg"));

    // ---- left: the live client -------------------------------------------
    div(context, mk(entity, 1),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(520), pixels(608)})
            .with_absolute_position(64.f, 56.f)
            .with_custom_background(panel)
            .with_roundness(0.04f)
            .with_debug_name("tn_panel"));

    div(context, mk(entity, 2),
        ComponentConfig{}
            .with_label("Secure Tunnel")
            .with_size(ComponentSize{pixels(460), pixels(34)})
            .with_absolute_position(96.f, 80.f)
            .with_alignment(TextAlignment::Left)
            .with_font_size(26.f)
            .with_custom_text_color(ink)
            .with_debug_name("tn_title"));

    // Status block, the thing the toggle actually drives.
    div(context, mk(entity, 3),
        ComponentConfig{}
            .with_size(ComponentSize{pixels(456), pixels(96)})
            .with_absolute_position(96.f, 128.f)
            .with_custom_background(state_fill(link))
            .with_roundness(0.06f)
            .with_debug_name("tn_status"));

    div(context, mk(entity, 4),
        ComponentConfig{}
            .with_label(state_word(link))
            .with_size(ComponentSize{pixels(420), pixels(34)})
            .with_absolute_position(120.f, 146.f)
            .with_alignment(TextAlignment::Left)
            .with_font_size(24.f)
            .with_custom_text_color(state_ink(link))
            .with_debug_name("tn_state"));

    div(context, mk(entity, 5),
        ComponentConfig{}
            .with_label(link == Link::Up
                            ? fmt::format("{}  {}  {} ms",
                                          SERVERS[server_idx].place,
                                          SERVERS[server_idx].tag,
                                          SERVERS[server_idx].ms)
                            : std::string("no route"))
            .with_size(ComponentSize{pixels(420), pixels(26)})
            .with_absolute_position(120.f, 182.f)
            .with_alignment(TextAlignment::Left)
            .with_font_size(17.f)
            .with_custom_text_color(state_ink(link))
            .with_debug_name("tn_route"));

    toggle_switch(context, mk(entity, 6), wants_up,
                  ComponentConfig{}
                      .with_label("Connect")
                      .with_size(ComponentSize{pixels(456), pixels(56)})
                      .with_absolute_position(96.f, 244.f)
                      .with_custom_background(row_fill)
                      .with_font_size(19.f)
                      .with_padding(Padding{.left = pixels(16),
                                            .right = pixels(16)})
                      .with_roundness(0.08f)
                      .with_debug_name("tn_toggle"));

    div(context, mk(entity, 7),
        ComponentConfig{}
            .with_label("Exit node")
            .with_size(ComponentSize{pixels(456), pixels(24)})
            .with_absolute_position(96.f, 316.f)
            .with_alignment(TextAlignment::Left)
            .with_font_size(16.f)
            .with_custom_text_color(muted)
            .with_debug_name("tn_listhead"));

    // Server rows: place on the left, latency right-aligned so the digits line
    // up down the column.
    for (int i = 0; i < 6; i++) {
      const bool picked = (size_t)i == server_idx;
      auto row = button(
          context, mk(entity, 20 + i),
          ComponentConfig{}
              .with_label(SERVERS[i].place)
              .with_size(ComponentSize{pixels(456), pixels(40)})
              .with_absolute_position(96.f, 348.f + (float)i * 44.f)
              .with_padding(Padding{.left = pixels(14), .right = pixels(14)})
              .with_custom_background(picked ? row_pick : row_fill)
              .with_auto_text_color(false)
              .with_custom_text_color(ink)
              .with_alignment(TextAlignment::Left)
              .with_font_size(17.f)
              .with_roundness(0.06f)
              .with_debug_name(fmt::format("tn_row_{}", i)));
      row_id[i] = row.ent().id;
      if (row)
        server_idx = (size_t)i;

      div(context, mk(row.ent(), 0),
          ComponentConfig{}
              .with_label(fmt::format("{} ms", SERVERS[i].ms))
              .with_size(ComponentSize{pixels(120), pixels(40)})
              .with_absolute_position(308.f, 0.f)
              .with_alignment(TextAlignment::Right)
              .with_font_size(17.f)
              .with_custom_text_color(muted)
              .with_ignore_pointer_events(true)
              .with_debug_name(fmt::format("tn_ms_{}", i)));
    }

    // ---- right: the same block in every state ----------------------------
    div(context, mk(entity, 8),
        ComponentConfig{}
            .with_label("Every state, so the theming is visible at rest")
            .with_size(ComponentSize{pixels(592), pixels(28)})
            .with_absolute_position(624.f, 62.f)
            .with_alignment(TextAlignment::Left)
            .with_font_size(17.f)
            .with_custom_text_color(muted)
            .with_debug_name("tn_matrix_head"));

    const Link all[3] = {Link::Off, Link::Dialing, Link::Up};
    const char *why[3] = {
        "toggle off, no route held",
        "toggle on, still negotiating",
        "toggle on, route established",
    };
    for (int i = 0; i < 3; i++) {
      const float y = 100.f + (float)i * 128.f;
      div(context, mk(entity, 40 + i),
          ComponentConfig{}
              .with_size(ComponentSize{pixels(592), pixels(78)})
              .with_absolute_position(624.f, y)
              .with_custom_background(state_fill(all[i]))
              .with_roundness(0.06f)
              .with_debug_name(fmt::format("tn_demo_{}", i)));
      div(context, mk(entity, 50 + i),
          ComponentConfig{}
              .with_label(state_word(all[i]))
              .with_size(ComponentSize{pixels(544), pixels(30)})
              .with_absolute_position(648.f, y + 14.f)
              .with_alignment(TextAlignment::Left)
              .with_font_size(21.f)
              .with_custom_text_color(state_ink(all[i]))
              .with_debug_name(fmt::format("tn_demo_word_{}", i)));
      div(context, mk(entity, 60 + i),
          ComponentConfig{}
              .with_label(why[i])
              .with_size(ComponentSize{pixels(544), pixels(24)})
              .with_absolute_position(648.f, y + 44.f)
              .with_alignment(TextAlignment::Left)
              .with_font_size(16.f)
              .with_custom_text_color(state_ink(all[i]))
              .with_debug_name(fmt::format("tn_demo_why_{}", i)));
    }

    // A tray of secondary actions, grouped away from the primary toggle.
    // The tray sits in a positioned wrapper: given an absolute position of its
    // own it lays out nothing.
    auto tray_slot = div(context, mk(entity, 9),
                         ComponentConfig{}
                             .with_size(ComponentSize{pixels(592), pixels(56)})
                             .with_absolute_position(624.f, 496.f)
                             .with_debug_name("tn_tray_slot"));
    auto actions = tray(context, mk(tray_slot.ent(), 0),
                        ComponentConfig{}
                            .with_size(ComponentSize{percent(1.0f), pixels(56)})
                            .with_flex_direction(FlexDirection::Row)
                            .with_flex_wrap(FlexWrap::NoWrap)
                            .with_justify_content(JustifyContent::SpaceAround)
                            .with_align_items(AlignItems::Center)
                            .with_debug_name("tn_tray"));
    const char *acts[3] = {"Protocol", "Split routing", "Diagnostics"};
    for (int i = 0; i < 3; i++) {
      button(context, mk(actions.ent(), i),
             ComponentConfig{}
                 .with_label(acts[i])
                 .with_size(ComponentSize{percent(0.30f), pixels(44)})
                 .with_custom_background(afterhours::Color{46, 52, 68, 255})
                 .with_auto_text_color(false)
                 .with_custom_text_color(afterhours::Color{214, 222, 238, 255})
                 .with_font_size(16.f)
                 .with_roundness(0.16f)
                 .with_debug_name(fmt::format("tn_act_{}", i)));
    }

    // Reads as a real client's status line rather than a variable dump.
    div(context, mk(entity, 10),
        ComponentConfig{}
            // Pipe, not a middot: the font has no glyph for it and draws "?".
            .with_label(fmt::format("{}  |  exit node {}", state_word(link),
                                    SERVERS[server_idx].place))
            .with_size(ComponentSize{pixels(592), pixels(26)})
            .with_absolute_position(624.f, 588.f)
            .with_alignment(TextAlignment::Left)
            .with_font_size(16.f)
            .with_custom_text_color(muted)
            .with_debug_name("tn_readout"));
  }
};

REGISTER_EXAMPLE_SCREEN(secure_tunnel, "App Mockups",
                        "Tunnel client: one toggle drives the whole screen",
                        SecureTunnelMockup)
