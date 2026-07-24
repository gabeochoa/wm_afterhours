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

  std::string delete_confirm = "";
  std::string palette_query = "";
  std::string status = "";

  static constexpr int MODAL_DELETE = 300;
  static constexpr int MODAL_PALETTE = 301;
  static constexpr int MODAL_TOS = 302;
  static constexpr int CL = 1001; // content layer (above the modal panel)
  static constexpr float FOCUS_SAFE_WIDTH = 0.92f;

  static std::string lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return s;
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    auto theme = afterhours::ui::theme_presets::ocean_navy();
    context.theme = theme;
    context.scaling_mode = ScalingMode::Adaptive;
    UIStylingDefaults::get().set_default_font(UIComponent::DEFAULT_FONT,
                                              pixels(16.0f));

    auto root =
        div(context, mk(entity, 0),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.90f), screen_pct(0.90f)})
                .with_background(Theme::Usage::Background)
                .with_roundness(0.08f)
                .with_self_align(SelfAlign::Center)
                .with_padding(Spacing::lg)
                .with_debug_name("rw_modal_bg"));

    auto main = vstack(context, mk(root.ent(), 0),
                       ComponentConfig{}
                           .with_size(ComponentSize{percent(1.0f), percent(1.0f)})
                           .with_no_wrap()
                           .with_debug_name("rw_modal_main"));

    div(context, mk(main.ent(), 0),
        ComponentConfig{}
            .with_label("Real-World Modals")
            .with_size(ComponentSize{percent(1.0f), pixels(48)})
            .with_auto_text_color(true)
            .with_font(UIComponent::DEFAULT_FONT, pixels(26.0f))
            .with_alignment(TextAlignment::Center));

    auto triggers = hstack(context, mk(main.ent(), 1),
                           ComponentConfig{}
                               .with_size(ComponentSize{percent(1.0f), pixels(52)})
                               .with_justify_content(JustifyContent::Center)
                               .with_align_items(AlignItems::Center)
                               .with_no_wrap()
                               .with_margin(Margin{.top = DefaultSpacing::medium()})
                               .with_debug_name("rw_triggers"));

    if (button(context, mk(triggers.ent(), 0),
               ComponentConfig{}
                   .with_label("Delete Item")
                   .with_size(ComponentSize{pixels(180), pixels(44)})
                   .with_background(Theme::Usage::Error)
                   .with_margin(Margin{.right = DefaultSpacing::small()})
                   .with_debug_name("btn_delete"))) {
      show_delete = true;
      delete_confirm = "";
    }

    if (button(context, mk(triggers.ent(), 1),
               ComponentConfig{}
                   .with_label("Command Palette")
                   .with_size(ComponentSize{pixels(200), pixels(44)})
                   .with_margin(Margin{.right = DefaultSpacing::small()})
                   .with_debug_name("btn_palette"))) {
      show_palette = true;
      palette_query = "";
    }

    if (button(context, mk(triggers.ent(), 2),
               ComponentConfig{}
                   .with_label("Terms of Service")
                   .with_size(ComponentSize{pixels(200), pixels(44)})
                   .with_debug_name("btn_tos"))) {
      show_tos = true;
    }

    div(context, mk(main.ent(), 2),
        ComponentConfig{}
            .with_label(status.empty() ? "No action yet" : status)
            .with_size(ComponentSize{percent(1.0f), pixels(28)})
            .with_custom_text_color(theme.secondary)
            .with_font(UIComponent::DEFAULT_FONT, pixels(14.0f))
            .with_alignment(TextAlignment::Center)
            .with_margin(Margin{.top = DefaultSpacing::medium()}));

    // ===================================================================
    // 1. Type-to-confirm destructive delete
    // ===================================================================
    if (auto m = afterhours::modal(
            context, mk(entity, MODAL_DELETE), show_delete,
            afterhours::ModalConfig{}
                .with_size(pixels(460), pixels(300))
                .with_title("Delete Project?")
                .with_closed_by(afterhours::ClosedBy::Any))) {
      div(context, mk(m.ent(), 0),
          ComponentConfig{}
              .with_label("This permanently deletes the project and all its "
                          "data. Type DELETE to confirm.")
              .with_size(ComponentSize{percent(1.0f), pixels(48)})
              .with_auto_text_color(true)
              .with_font(UIComponent::DEFAULT_FONT, pixels(15.0f))
              .with_render_layer(CL));

      text_input(context, mk(m.ent(), 1), delete_confirm,
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(FOCUS_SAFE_WIDTH), pixels(38)})
                     .with_background(Theme::Usage::Primary)
                     .with_roundness(0.1f)
                     .with_margin(Margin{.top = DefaultSpacing::small(),
                                         .bottom = DefaultSpacing::small()})
                     .with_render_layer(CL)
                     .with_debug_name("delete_confirm_input"));

      bool can_delete = (delete_confirm == "DELETE");
      auto row = hstack(context, mk(m.ent(), 2),
                        ComponentConfig{}
                            .with_size(ComponentSize{percent(1.0f), pixels(44)})
                            .with_justify_content(JustifyContent::SpaceBetween)
                            .with_no_wrap()
                            .with_render_layer(CL));

      if (button(context, mk(row.ent(), 0),
                 ComponentConfig{}
                     .with_label("Delete")
                     .with_size(ComponentSize{pixels(180), pixels(38)})
                     .with_background(Theme::Usage::Error)
                     .with_disabled(!can_delete)
                     .with_render_layer(CL))) {
        if (can_delete) {
          status = "Project deleted.";
          show_delete = false;
          delete_confirm = "";
        }
      }
      if (button(context, mk(row.ent(), 1),
                 ComponentConfig{}
                     .with_label("Cancel")
                     .with_size(ComponentSize{pixels(180), pixels(38)})
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
                .with_size(pixels(520), pixels(420))
                .with_title("Command Palette")
                .with_closed_by(afterhours::ClosedBy::Any))) {
      text_input(context, mk(m.ent(), 0), palette_query,
                 ComponentConfig{}
                     .with_size(ComponentSize{percent(FOCUS_SAFE_WIDTH), pixels(38)})
                     .with_background(Theme::Usage::Primary)
                     .with_roundness(0.1f)
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
        if (button(context, mk(m.ent(), idx++),
                   ComponentConfig{}
                       .with_label(c)
                       .with_size(ComponentSize{percent(1.0f), pixels(34)})
                       .with_alignment(TextAlignment::Left)
                       .with_margin(Margin{.bottom = DefaultSpacing::tiny()})
                       .with_render_layer(CL))) {
          status = "Ran: " + c;
          show_palette = false;
          palette_query = "";
        }
      }
    }

    // ===================================================================
    // 3. Scrollable Terms of Service
    // ===================================================================
    if (auto m = afterhours::modal(
            context, mk(entity, MODAL_TOS), show_tos,
            afterhours::ModalConfig{}
                .with_size(pixels(560), pixels(460))
                .with_title("Terms of Service")
                .with_closed_by(afterhours::ClosedBy::Any))) {
      auto scroll = div(context, mk(m.ent(), 0),
                        ComponentConfig{}
                            .with_size(ComponentSize{percent(1.0f), pixels(320)})
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
                .with_size(ComponentSize{percent(FOCUS_SAFE_WIDTH), pixels(64)})
                .with_auto_text_color(true)
                .with_text_overflow(TextOverflow::Wrap)
                .with_font(UIComponent::DEFAULT_FONT, pixels(14.0f))
                .with_alignment(TextAlignment::Left)
                .with_margin(Margin{.bottom = DefaultSpacing::small()})
                .with_render_layer(CL));
      }

      auto row = hstack(context, mk(m.ent(), 1),
                        ComponentConfig{}
                            .with_size(ComponentSize{percent(1.0f), pixels(44)})
                            .with_justify_content(JustifyContent::SpaceBetween)
                            .with_no_wrap()
                            .with_margin(Margin{.top = DefaultSpacing::small()})
                            .with_render_layer(CL));
      if (button(context, mk(row.ent(), 0),
                 ComponentConfig{}
                     .with_label("Accept")
                     .with_size(ComponentSize{pixels(200), pixels(38)})
                     .with_background(Theme::Usage::Primary)
                     .with_render_layer(CL))) {
        status = "Terms accepted.";
        show_tos = false;
      }
      if (button(context, mk(row.ent(), 1),
                 ComponentConfig{}
                     .with_label("Decline")
                     .with_size(ComponentSize{pixels(200), pixels(38)})
                     .with_render_layer(CL))) {
        status = "Terms declined.";
        show_tos = false;
      }
    }
  }
};

REGISTER_EXAMPLE_SCREEN(real_world_modals, "Component Galleries",
                        "Real-world modal patterns (destructive confirm, "
                        "command palette, scrollable content)",
                        RealWorldModals)
