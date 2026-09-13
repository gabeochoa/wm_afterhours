#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/modal.h>
#include <afterhours/src/plugins/ui/text_input/text_input.h>

#include <algorithm>
#include <array>
#include <cctype>
#include <string>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Real-world modal patterns that go beyond the basic showcase: an
// input-gated destructive confirm, a searchable command palette, and a
// scrollable long-content dialog. These prove the modal plugin can carry
// production UI flows (validation gating, live filtering, scroll-in-modal).
struct RealWorldModals : ScreenSystem<UIContext<InputAction>> {
  bool show_delete = false;
  bool show_palette = false;
  bool show_tos = false;
  bool show_drawer = false;
  bool show_sheet = false;
  bool show_cookie = false;

  std::string delete_confirm = "";
  std::string palette_query = "";
  std::string status = "";

  static constexpr int MODAL_DELETE = 300;
  static constexpr int MODAL_PALETTE = 301;
  static constexpr int MODAL_TOS = 302;
  static constexpr int MODAL_DRAWER = 303;
  static constexpr int MODAL_SHEET = 304;
  static constexpr int MODAL_COOKIE = 305;
  static constexpr int CL = 1001; // content layer (above the modal panel)
  static constexpr float FOCUS_SAFE_WIDTH = 0.92f;

  static std::string lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return s;
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    auto theme = afterhours::ui::theme_presets::ocean_navy();
    theme.background = {19, 24, 32, 255};
    theme.surface = {32, 40, 52, 255};
    theme.primary = {48, 91, 151, 255};
    theme.secondary = {48, 59, 76, 255};
    theme.font = {237, 244, 251, 255};
    theme.font_muted = {183, 205, 224, 255};
    theme.focus = {165, 201, 248, 255};
    theme.corner_radius = 6;
    theme.roundness = 0;
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_grid_snapping(false);
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(20 * s));
    const auto at = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_corner_radius(0)
          .with_background(Theme::Usage::None);
    };
    div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background(theme.background).with_corner_radius(0).with_debug_name("rw_modal_bg"));
    auto main = div(context, mk(entity, 1), at(0, 0, 1280, 720)
        .with_absolute_position((context.screen_width - 1280 * s) / 2,
                                (context.screen_height - 720 * s) / 2)
        .with_debug_name("rw_modal_main"));
    const auto text = [&](int id, const std::string &label, float x, float y,
                          float w, float h, float size, const std::string &debug = "") {
      div(context, mk(main.ent(), id), at(x, y, w, h).with_label(label)
          .with_font("AtkinsonMock", pixels(size * s)).with_custom_text_color(theme.font)
          .with_ignore_pointer_events().with_debug_name(debug));
    };
    text(0, "Real-World Modals", 48, 22, 1184, 44, 34, "rw_title");
    text(1, "Choose a pattern to preview its dialog.", 48, 71, 1184, 28, 22);
    text(2, "Escape closes a preview. Click outside to dismiss all except the cookie banner.",
         48, 106, 1184, 28, 18);
    text(3, "Centered dialogs", 48, 139, 1184, 30, 24);
    text(4, "Edge overlays", 48, 359, 1184, 30, 24);
    const std::array<const char *, 6> names{"Preview delete confirmation", "Command Palette", "Terms of Service", "Side Drawer", "Bottom Sheet", "Cookie Banner"};
    const std::array<const char *, 6> captions{"Type DELETE to enable the action", "Search commands", "Scrollable sample agreement", "Navigation from the right edge", "Actions from the bottom edge", "A short consent strip"};
    const std::array<const char *, 6> debug{"btn_delete", "btn_palette", "btn_tos", "btn_drawer", "btn_sheet", "btn_cookie"};
    for (size_t i = 0; i < names.size(); ++i) {
      const float x = 48 + static_cast<float>(i % 3) * 408;
      const float y = i < 3 ? 180 : 400;
      div(context, mk(main.ent(), 10 + static_cast<int>(i)), at(x, y, 368, 170)
          .with_custom_background(theme.surface).with_corner_radius(10 * s));
      auto cfg = at(x + 16, y + 14, 336, 44).with_label(names[i])
          .with_font("AtkinsonMock", pixels(21 * s)).with_letter_spacing(0)
          .with_custom_text_color(theme.font).with_custom_background(theme.primary)
          .with_corner_radius(6 * s).with_alignment(TextAlignment::Center)
          .with_click_activation(ClickActivationMode::Release).with_debug_name(debug[i]);
      if (i == 0) cfg.with_custom_background(theme.surface).with_border({223, 126, 131, 255}, s);
      if (button(context, mk(main.ent(), 20 + static_cast<int>(i)), cfg)) {
        if (i == 0) { show_delete = true; delete_confirm.clear(); }
        if (i == 1) { show_palette = true; palette_query.clear(); }
        if (i == 2) show_tos = true;
        if (i == 3) show_drawer = true;
        if (i == 4) show_sheet = true;
        if (i == 5) show_cookie = true;
      }
      text(30 + static_cast<int>(i), captions[i], x + 16, y + 69, 336, 27, 18);
      div(context, mk(main.ent(), 40 + static_cast<int>(i)), at(x + 16, y + 108, 100, 48)
          .with_border({107, 135, 162, 255}, s).with_ignore_pointer_events()
          .with_on_draw_fg([i](RectangleType r) {
            const float unit = r.width / 100;
            RectangleType sample{r.x + 25 * unit, r.y + 11 * unit, 50 * unit, 27 * unit};
            if (i == 3) sample = {r.x + 70 * unit, r.y, 30 * unit, r.height};
            if (i == 4) sample = {r.x, r.y + 28 * unit, r.width, 20 * unit};
            if (i == 5) sample = {r.x, r.y + 37 * unit, r.width, 11 * unit};
            raylib::DrawRectangleRec(sample, {75, 142, 181, 255});
            raylib::DrawRectangleLinesEx(sample, unit, {210, 233, 246, 255});
          }).with_debug_name("rw_preview_" + std::to_string(i)));
      text(50 + static_cast<int>(i), "Placement preview", x + 132, y + 118, 220, 27, 17);
    }
    div(context, mk(main.ent(), 60), at(48, 607, 1184, 88)
        .with_custom_background({25, 45, 64, 255}).with_corner_radius(10 * s)
        .with_debug_name("rw_result_panel"));
    text(61, "Action result", 68, 616, 1144, 28, 22);
    text(62, status.empty() ? "No action yet" : status, 68, 652, 1144, 28, 20, "rw_action_result");

    const auto style_title = [s](afterhours::Entity &panel) {
      panel.addComponentIfMissing<HasRoundedCorners>().set_radius_px(12 * s);
      panel.addComponentIfMissing<HasBorder>().border =
          Border::all(afterhours::Color{81, 96, 119, 255}, pixels(s));
      for (const auto id : panel.get<UIComponent>().children) {
        auto &header = UICollectionHolder::getEntityForIDEnforce(id);
        if (!header.has<UIComponentDebug>() || header.get<UIComponentDebug>().name() != "modal_header") continue;
        for (const auto child_id : header.get<UIComponent>().children) {
          auto &title = UICollectionHolder::getEntityForIDEnforce(child_id);
          if (!title.has<UIComponentDebug>() || title.get<UIComponentDebug>().name() != "modal_title") continue;
          title.get<UIComponent>().enable_font("AtkinsonMockBold", pixels(28 * s), true);
          title.get<HasLabel>().font_name = "AtkinsonMockBold";
        }
      }
    };

    // ===================================================================
    // 1. Type-to-confirm destructive delete
    // ===================================================================
    if (auto m = afterhours::modal(
            context, mk(entity, MODAL_DELETE), show_delete,
            afterhours::ModalConfig{}
                .with_size(pixels(620.f * s), pixels(320.f * s))
                .with_title("Delete Project?")
                .with_closed_by(afterhours::ClosedBy::Any))) {
      m.ent().get<UIComponentDebug>().set("rw_delete_panel");
      style_title(m.ent());
      div(context, mk(m.ent(), 0),
          ComponentConfig{}
              .with_label("Preview only: type DELETE to confirm this sample "
                          "project deletion.")
              .with_text_overflow(TextOverflow::Wrap)
              .with_size(ComponentSize{percent(1.0f), pixels(48.f * s)})
              .with_auto_text_color(true)
              .with_font("AtkinsonMock", pixels(20.f * s))
              .with_render_layer(CL));

      text_input(context, mk(m.ent(), 1), delete_confirm,
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(FOCUS_SAFE_WIDTH), pixels(38.f * s)})
                     .with_background(Theme::Usage::Primary)
                     .with_corner_radius(8.f * s)
                     .with_margin(Margin{.top = DefaultSpacing::small(),
                                         .bottom = DefaultSpacing::small()})
                     .with_render_layer(CL)
                     .with_debug_name("delete_confirm_input"));

      bool can_delete = (delete_confirm == "DELETE");
      auto row = hstack(context, mk(m.ent(), 2),
                        ComponentConfig{}
                            .with_size(ComponentSize{percent(1.0f), pixels(44.f * s)})
                            .with_justify_content(JustifyContent::SpaceBetween)
                            .with_no_wrap()
                            .with_render_layer(CL));

      if (button(context, mk(row.ent(), 0),
                 ComponentConfig{}
                     .with_label("Delete")
                     .with_size(ComponentSize{pixels(180.f * s), pixels(38.f * s)})
                     .with_background(Theme::Usage::Error)
                     .with_disabled(!can_delete)
                     .with_debug_name("rw_delete_confirm")
                     .with_render_layer(CL))) {
        if (can_delete) {
          status = "Delete confirmed (demo).";
          show_delete = false;
          delete_confirm = "";
        }
      }
      if (button(context, mk(row.ent(), 1),
                 ComponentConfig{}
                     .with_label("Cancel").with_debug_name("rw_delete_cancel")
                     .with_background(Theme::Usage::Secondary)
                     .with_size(ComponentSize{pixels(180.f * s), pixels(38.f * s)})
                     .with_render_layer(CL))) {
        show_delete = false;
        delete_confirm = "";
      }
    }

    // ===================================================================
    // 2. Command palette (search + live-filtered list)
    // ===================================================================
    if (auto m = afterhours::modal(
            context, mk(entity, MODAL_PALETTE), show_palette,
            afterhours::ModalConfig{}
                .with_size(pixels(660.f * s), pixels(540.f * s))
                .with_title("Command Palette")
                .with_closed_by(afterhours::ClosedBy::Any))) {
      m.ent().get<UIComponentDebug>().set("rw_palette_panel");
      style_title(m.ent());
      text_input(context, mk(m.ent(), 0), palette_query,
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(FOCUS_SAFE_WIDTH), pixels(38.f * s)})
                     .with_background(Theme::Usage::Primary)
                     .with_corner_radius(8.f * s)
                     .with_margin(Margin{.bottom = DefaultSpacing::small()})
                     .with_render_layer(CL)
                     .with_debug_name("palette_query_input"));

      static constexpr std::array<const char *, 8> commands = {
          "New File",       "Open Folder",  "Save All",    "Toggle Theme",
          "Find in Files",  "Git: Commit",  "Run Build",   "Open Settings"};

      std::string q = lower(palette_query);
      int idx = 1;
      for (const char *cmd : commands) {
        std::string c = cmd;
        if (!q.empty() && lower(c).find(q) == std::string::npos)
          continue;
        const int command_index = idx++;
        auto command = button(context, mk(m.ent(), command_index),
                   ComponentConfig{}
                       .with_label(c).with_debug_name("rw_command_" + std::to_string(command_index))
                       .with_custom_background(theme.surface)
                       .with_custom_hover_bg({56, 73, 96, 255})
                       .with_size(ComponentSize{percent(1.0f), pixels(38.f * s)})
                       .with_alignment(TextAlignment::Left)
                       .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
                       .with_render_layer(CL));
        command.ent().get<HasLabel>().text_x_offset = 12 * s;
        if (command) {
          status = "Selected command: " + c;
          show_palette = false;
          palette_query = "";
        }
      }
      if (idx == 1) div(context, mk(m.ent(), 100), ComponentConfig{}
          .with_label("No matching commands").with_size({percent(1), pixels(44 * s)})
          .with_font("AtkinsonMock", pixels(20 * s)).with_custom_text_color(theme.font)
          .with_render_layer(CL));
    }

    // ===================================================================
    // 3. Scrollable Terms of Service
    // ===================================================================
    if (auto m = afterhours::modal(
            context, mk(entity, MODAL_TOS), show_tos,
            afterhours::ModalConfig{}
                .with_size(pixels(680.f * s), pixels(560.f * s))
                .with_title("Terms of Service")
                .with_closed_by(afterhours::ClosedBy::Any))) {
      m.ent().get<UIComponentDebug>().set("rw_tos_panel");
      style_title(m.ent());
      auto scroll = div(context, mk(m.ent(), 0),
                        ComponentConfig{}
                            .with_size(ComponentSize{percent(1.0f), pixels(350.f * s)})
                            .with_overflow(Overflow::Scroll, Axis::Y)
                            .with_render_layer(CL)
                            .with_debug_name("tos_scroll"));

      static constexpr std::array<const char *, 8> paragraphs = {
          "1. Acceptance. By using this software you agree to these terms in "
          "full. If you disagree with any part, do not use the software.",
          "2. License. You are granted a non-exclusive, non-transferable "
          "license to use the software for its intended purpose.",
          "3. Restrictions. You may not reverse engineer, resell, or "
          "redistribute the software without written permission.",
          "4. Data. The software may collect anonymous usage metrics to "
          "improve reliability. No personal data is sold to third parties.",
          "5. Warranty. The software is provided \"as is\" without warranty of "
          "any kind, express or implied.",
          "6. Liability. In no event shall the authors be liable for any claim "
          "or damages arising from the use of the software.",
          "7. Termination. This license terminates automatically if you breach "
          "any of these terms.",
          "8. Changes. These terms may be updated at any time; continued use "
          "constitutes acceptance of the revised terms."};

      int i = 0;
      for (const char *p : paragraphs) {
        div(context, mk(scroll.ent(), i++),
            ComponentConfig{}
                .with_label(p)
                .with_size(ComponentSize{percent(FOCUS_SAFE_WIDTH), pixels(80.f * s)})
                .with_auto_text_color(true)
                .with_text_overflow(TextOverflow::Wrap)
                .with_font("AtkinsonMock", pixels(20.f * s))
                .with_alignment(TextAlignment::Left)
                .with_margin(Margin{.bottom = DefaultSpacing::small()})
                .with_render_layer(CL));
      }

      auto row = hstack(context, mk(m.ent(), 1),
                        ComponentConfig{}
                            .with_size(ComponentSize{percent(1.0f), pixels(44.f * s)})
                            .with_justify_content(JustifyContent::SpaceBetween)
                            .with_no_wrap()
                            .with_margin(Margin{.top = DefaultSpacing::small()})
                            .with_render_layer(CL));
      if (button(context, mk(row.ent(), 0),
                 ComponentConfig{}
                     .with_label("Accept").with_debug_name("rw_terms_accept")
                     .with_size(ComponentSize{pixels(200.f * s), pixels(38.f * s)})
                     .with_background(Theme::Usage::Primary)
                     .with_render_layer(CL))) {
        status = "Terms accepted.";
        show_tos = false;
      }
      if (button(context, mk(row.ent(), 1),
                 ComponentConfig{}
                     .with_label("Decline").with_debug_name("rw_terms_decline")
                     .with_background(Theme::Usage::Secondary)
                     .with_size(ComponentSize{pixels(200.f * s), pixels(38.f * s)})
                     .with_render_layer(CL))) {
        status = "Terms declined.";
        show_tos = false;
      }
    }

    // ===================================================================
    // 4. Side drawer (right edge, full height)
    // ===================================================================
    if (auto m = afterhours::modal(
            context, mk(entity, MODAL_DRAWER), show_drawer,
            afterhours::ModalConfig{}
                .with_size(pixels(360.f * s), screen_pct(1.0f))
                .with_title("Menu")
                .with_anchor(afterhours::ModalAnchor::Right)
                .with_closed_by(afterhours::ClosedBy::Any))) {
      m.ent().get<UIComponentDebug>().set("rw_drawer_panel");
      style_title(m.ent());
      static constexpr std::array<const char *, 5> items = {
          "Dashboard", "Projects", "Team", "Billing", "Settings"};
      int idx = 0;
      for (const char *it : items) {
        const int destination_index = idx++;
        auto destination = button(context, mk(m.ent(), destination_index),
                   ComponentConfig{}
                       .with_label(it).with_debug_name("rw_destination_" + std::to_string(destination_index))
                       .with_custom_background(theme.surface)
                       .with_custom_hover_bg({56, 73, 96, 255})
                       .with_size(ComponentSize{percent(1.0f), pixels(40.f * s)})
                       .with_alignment(TextAlignment::Left)
                       .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
                       .with_render_layer(CL));
        destination.ent().get<HasLabel>().text_x_offset = 12 * s;
        if (destination) {
          status = std::string("Selected destination: ") + it;
          show_drawer = false;
        }
      }
    }

    // ===================================================================
    // 5. Bottom sheet (bottom edge, full width)
    // ===================================================================
    if (auto m = afterhours::modal(
            context, mk(entity, MODAL_SHEET), show_sheet,
            afterhours::ModalConfig{}
                .with_size(percent(1.0f), pixels(240.f * s))
                .with_title("Share")
                .with_anchor(afterhours::ModalAnchor::Bottom)
                .with_closed_by(afterhours::ClosedBy::Any))) {
      m.ent().get<UIComponentDebug>().set("rw_sheet_panel");
      style_title(m.ent());
      auto row = hstack(context, mk(m.ent(), 0),
                        ComponentConfig{}
                            .with_size(ComponentSize{percent(1.0f), pixels(90.f * s)})
                            .with_justify_content(JustifyContent::SpaceAround)
                            .with_align_items(AlignItems::Center)
                            .with_no_wrap()
                            .with_render_layer(CL));
      static constexpr std::array<const char *, 4> actions = {
          "Copy Link", "Email", "Message", "More"};
      int idx = 0;
      for (const char *a : actions) {
        const int action_index = idx++;
        if (button(context, mk(row.ent(), action_index),
                   ComponentConfig{}
                       .with_label(a).with_debug_name("rw_share_" + std::to_string(action_index))
                       .with_size(ComponentSize{pixels(160.f * s), pixels(64.f * s)})
                       .with_render_layer(CL))) {
          status = std::string("Share choice: ") + a;
          show_sheet = false;
        }
      }
    }

    // ===================================================================
    // 6. Cookie consent banner (bottom edge, short strip)
    // ===================================================================
    if (auto m = afterhours::modal(
            context, mk(entity, MODAL_COOKIE), show_cookie,
            afterhours::ModalConfig{}
                .with_size(percent(1.0f), pixels(120.f * s))
                .with_anchor(afterhours::ModalAnchor::Bottom)
                .with_show_close_button(false)
                .with_backdrop_color({0, 0, 0, 60})
                .with_closed_by(afterhours::ClosedBy::CloseRequest))) {
      m.ent().get<UIComponentDebug>().set("rw_cookie_panel");
      auto row = hstack(context, mk(m.ent(), 0),
                        ComponentConfig{}
                            .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                            .with_justify_content(JustifyContent::SpaceBetween)
                            .with_align_items(AlignItems::Center)
                            .with_no_wrap()
                            .with_render_layer(CL));
      div(context, mk(row.ent(), 0),
          ComponentConfig{}
              .with_label("We use cookies to improve your experience.")
              .with_size(ComponentSize{percent(0.6f), pixels(40.f * s)})
              .with_auto_text_color(true)
              .with_text_overflow(TextOverflow::Wrap)
              .with_font("AtkinsonMock", pixels(20.f * s))
              .with_render_layer(CL));
      if (button(context, mk(row.ent(), 1),
                 ComponentConfig{}
                     .with_label("Reject").with_debug_name("rw_cookie_reject")
                     .with_background(Theme::Usage::Secondary)
                     .with_size(ComponentSize{pixels(140.f * s), pixels(40.f * s)})
                     .with_margin(Margin{.right = DefaultSpacing::small()})
                     .with_render_layer(CL))) {
        status = "Cookies rejected.";
        show_cookie = false;
      }
      if (button(context, mk(row.ent(), 2),
                 ComponentConfig{}
                     .with_label("Accept").with_debug_name("rw_cookie_accept")
                     .with_size(ComponentSize{pixels(140.f * s), pixels(40.f * s)})
                     .with_background(Theme::Usage::Primary)
                     .with_render_layer(CL))) {
        status = "Cookies accepted.";
        show_cookie = false;
      }
    }
  }
};

REGISTER_EXAMPLE_SCREEN(real_world_modals, "Component Galleries",
                        "Real-world modal patterns (destructive confirm, "
                        "command palette, scrollable content)",
                        RealWorldModals)
