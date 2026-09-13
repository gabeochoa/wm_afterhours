#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/modal.h>
#include <array>

#include "DialogBackdrop.h"
#include "DialogPresentation.h"

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// modal::confirm_danger shown open: destructive confirm (Error-red action,
// Keep save as the calm default on the left).
struct DialogDangerShowcase : ScreenSystem<UIContext<InputAction>> {
  bool open = true;
  bool save_present = true;
  std::string status = "3 demo saves. Chapter 3 autosave is selected.";

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context,
                     float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.theme.surface = {30, 40, 57, 255};
    context.theme.primary = {44, 93, 155, 255};
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    dialog_backdrop::draw_context(context, entity, "Saves",
        {{"Chapter 1 - Manual", "Slot 01 / 04 Sep 2026 / 0h 48m"},
         {"Chapter 2 - Camp", "Slot 02 / 05 Sep 2026 / 2h 16m"},
         {save_present ? "Chapter 3 - Autosave" : "Empty save slot",
          save_present ? "Slot 03 / 06 Sep 2026, 21:14 / 4h 32m" : "Slot 03 / empty", save_present},
         {"Collection", save_present ? "3 demo saves" : "2 demo saves"},
         {"Storage", "In-memory demo data"}}, status);
    if (button(context, mk(entity, 20), ComponentConfig{}
        .with_size({pixels(270 * s), pixels(44 * s)}).with_absolute_position(942 * s, 92 * s)
        .with_label("Delete selected save").with_font("AtkinsonMock", pixels(21 * s))
        .with_background(Theme::Usage::Error).with_auto_text_color(true)
        .with_disabled(!save_present).with_corner_radius(8 * s).with_debug_name("dd_review"))) {
      open = true;
      status = "3 demo saves. Chapter 3 autosave is selected.";
    }
    if (button(context, mk(entity, 21), ComponentConfig{}
        .with_size({pixels(270 * s), pixels(44 * s)}).with_absolute_position(942 * s, 560 * s)
        .with_label("Reset save demo").with_font("AtkinsonMock", pixels(21 * s))
        .with_background(Theme::Usage::Primary).with_auto_text_color(true)
        .with_corner_radius(8 * s).with_debug_name("dd_reset"))) {
      save_present = true;
      status = "Demo reset. All 3 saves restored.";
    }
    const bool confirming = open;
    auto confirmation = afterhours::modal::confirm_danger(context, mk(entity, 1), open,
        "Delete Chapter 3 autosave?",
        "Chapter 3 - Autosave\n"
        "Slot 03 / saved 06 Sep 2026, 21:14\n"
        "Playtime: 4h 32m\n"
        "This permanently deletes the selected save.\n"
        "Demo data only. Reset save demo restores this example.",
        "Delete save", "Keep save");
    if (confirming && confirmation.confirmed()) {
      save_present = false;
      status = "Deleted Chapter 3 autosave. 2 demo saves remain.";
    }
    if (confirming && (confirmation.cancelled() || confirmation.dismissed()))
      status = "Kept Chapter 3 autosave. All 3 demo saves remain.";
    if (!confirmation) return;
    constexpr std::array<float, 2> action_widths{220, 220};
    dialog_presentation::style(context, confirmation.ent(), 760, 156, action_widths);
    afterhours::Entity *header = nullptr;
    for (const auto id : confirmation.cmp().children) {
      auto child = UICollectionHolder::getEntityForID(id);
      if (!child.valid() || !child.asE().has<UIComponentDebug>()) continue;
      auto &node = child.asE();
      const auto name = node.get<UIComponentDebug>().name();
      if (name == "modal_header") header = &node;
      if (name == "dialog_message") {
        auto &cmp = node.get<UIComponent>();
        cmp.desired[Axis::X] = pixels(616 * s);
        cmp.desired_margin[Axis::left] = pixels(96 * s);
      }
      if (name != "dialog_buttons") continue;
      size_t action = 0;
      for (const auto button_id : node.get<UIComponent>().children) {
        auto button_entity = UICollectionHolder::getEntityForID(button_id);
        if (!button_entity.valid()) continue;
        button_entity.asE().addComponentIfMissing<UIComponentDebug>("dd_keep")
            .set(action++ == 0 ? "dd_keep" : "dd_delete");
      }
    }
    div(context, mk(confirmation.ent(), 200), ComponentConfig{}
        .with_size({pixels(72 * s), pixels(84 * s)}).with_absolute_position(0, 50 * s)
        .with_label("03").with_font("AtkinsonMockBold", pixels(40 * s))
        .with_alignment(TextAlignment::Center).with_custom_text_color({220, 230, 246, 255})
        .with_custom_background({48, 66, 92, 255}).with_corner_radius(8 * s)
        .with_ignore_pointer_events().with_debug_name("dd_chapter_emblem"));
    div(context, mk(confirmation.ent(), 201), ComponentConfig{}
        .with_size({pixels(82 * s), pixels(24 * s)}).with_absolute_position(-5 * s, 139 * s)
        .with_label("CHAPTER").with_font("AtkinsonMock", pixels(15 * s))
        .with_alignment(TextAlignment::Center).with_custom_text_color({171, 191, 219, 255})
        .with_background(Theme::Usage::None).with_ignore_pointer_events());
    if (!header) return;
    for (const auto id : header->get<UIComponent>().children) {
      auto child = UICollectionHolder::getEntityForID(id);
      if (!child.valid() || !child.asE().has<UIComponentDebug>()) continue;
      if (child.asE().get<UIComponentDebug>().name() != "modal_title") continue;
      child.asE().get<UIComponent>().desired_margin[Axis::left] = pixels(36 * s);
    }
    div(context, mk(*header, 200), ComponentConfig{}
        .with_size({pixels(24 * s), pixels(28 * s)}).with_absolute_position(0, 6 * s)
        .with_background(Theme::Usage::None).with_ignore_pointer_events()
        .with_debug_name("dd_warning_icon").with_on_draw_fg([s](RectangleType r) {
          const afterhours::Color warning{238, 173, 79, 255};
          const raylib::Vector2 a{r.x + r.width / 2, r.y};
          const raylib::Vector2 b{r.x + r.width, r.y + r.height};
          const raylib::Vector2 c{r.x, r.y + r.height};
          raylib::DrawLineEx(a, b, 2 * s, warning);
          raylib::DrawLineEx(b, c, 2 * s, warning);
          raylib::DrawLineEx(c, a, 2 * s, warning);
          raylib::DrawLineEx({a.x, r.y + 10 * s}, {a.x, r.y + 17 * s}, 2 * s, warning);
          raylib::DrawCircleV({a.x, r.y + 23 * s}, 1.5f * s, warning);
        }));
  }
};

REGISTER_EXAMPLE_SCREEN(dialog_danger, "Component Galleries",
                        "Danger confirm dialog (shown open)",
                        DialogDangerShowcase)
