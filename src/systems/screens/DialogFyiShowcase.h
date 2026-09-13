#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/modal.h>
#include <afterhours/src/plugins/ui/text_input/text_area.h>
#include <array>

#include "DialogBackdrop.h"
#include "DialogPresentation.h"

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// modal::fyi shown open: three-button dialog (e.g. Save / Don't Save / Cancel).
struct DialogFyiShowcase : ScreenSystem<UIContext<InputAction>> {
  static constexpr const char *document_name = "North meadow field notes.txt";
  static constexpr const char *initial_saved =
      "North meadow field notes\n\nSurvey the eastern path before noon.\nRecord the plant samples.";
  static constexpr const char *initial_draft =
      "North meadow field notes\n\nSurvey the eastern path before noon.\nRecord the plant samples.\nBring the waterproof notebook.";
  bool open = true;
  bool document_open = true;
  bool focus_dialog = true;
  bool return_to_editor = false;
  std::string saved_text = initial_saved;
  std::string draft_text = initial_draft;
  std::string status = "Closing North meadow field notes.txt with unsaved edits.";

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context,
                     float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.theme.surface = {30, 40, 57, 255};
    context.theme.primary = {44, 93, 155, 255};
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const auto box = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_background(Theme::Usage::None).with_corner_radius(0);
    };
    const bool dirty = draft_text != saved_text;
    dialog_backdrop::draw_context(context, entity, "Documents",
        {{"Document", document_name, document_open},
         {"Status", document_open ? dirty ? "Unsaved changes" : "All changes saved" : "Closed", dirty}}, status);
    if (button(context, mk(entity, 20), box(942, 92, 270, 44)
        .with_label(document_open ? "Close document" : "Reopen saved document")
        .with_font("AtkinsonMock", pixels(21 * s)).with_background(Theme::Usage::Primary)
        .with_auto_text_color(true).with_corner_radius(8 * s).with_debug_name("df_close_or_reopen"))) {
      if (!document_open) {
        document_open = true;
        draft_text = saved_text;
        return_to_editor = true;
        status = "Opened the saved document.";
      } else if (dirty) {
        open = true;
        focus_dialog = true;
        status = "Closing North meadow field notes.txt with unsaved edits.";
      } else {
        document_open = false;
        status = "Closed the document. No unsaved changes.";
      }
    }
    if (button(context, mk(entity, 21), box(926, 586, 286, 40)
        .with_label("Reset unsaved demo").with_font("AtkinsonMock", pixels(21 * s))
        .with_background(Theme::Usage::Primary).with_auto_text_color(true).with_corner_radius(8 * s)
        .with_debug_name("df_reset"))) {
      saved_text = initial_saved;
      draft_text = initial_draft;
      document_open = true;
      open = true;
      focus_dialog = true;
      return_to_editor = false;
      status = "Demo reset. The document has unsaved edits.";
    }
    div(context, mk(entity, 22), box(264, 301, 948, 34)
        .with_label(document_open ? "Editor preview" : "Saved version / document closed")
        .with_font("AtkinsonMockBold", pixels(27 * s)).with_custom_text_color({213, 226, 243, 255})
        .with_alignment(TextAlignment::Left).with_ignore_pointer_events());
    afterhours::EntityID editor_id = -1;
    if (document_open) {
      auto editor = text_area(context, mk(entity, 23), draft_text, box(264, 342, 948, 232)
          .with_font("AtkinsonMock", pixels(22 * s)).with_line_height(pixels(30 * s))
          .with_custom_text_color({224, 233, 248, 255}).with_custom_background({29, 38, 53, 255})
          .with_word_wrap(true).with_corner_radius(8 * s).with_debug_name("df_editor"));
      for (const auto child_id : editor.cmp().children) {
        const auto child = UICollectionHolder::getEntityForID(child_id);
        if (!child || !child.asE().has<InFocusCluster>()) continue;
        editor_id = child_id;
        break;
      }
    } else {
      div(context, mk(entity, 24), box(264, 342, 948, 232).with_label(saved_text)
          .with_font("AtkinsonMock", pixels(22 * s)).with_custom_text_color({199, 212, 231, 255})
          .with_custom_background({29, 38, 53, 255}).with_corner_radius(8 * s)
          .with_text_overflow(TextOverflow::Wrap).with_alignment(TextAlignment::Left)
          .with_ignore_pointer_events().with_debug_name("df_saved_preview"));
    }
    div(context, mk(entity, 25), box(264, 590, 634, 31)
        .with_label("Editable demo / save data lasts for this session")
        .with_font("AtkinsonMock", pixels(19 * s)).with_custom_text_color({164, 185, 213, 255})
        .with_alignment(TextAlignment::Left).with_ignore_pointer_events());

    const bool confirming = open;
    auto confirmation = afterhours::modal::fyi(context, mk(entity, 1), open,
        "Save changes before leaving?",
        "North meadow field notes.txt\n"
        "You are closing this document with unsaved edits.\n"
        "Save and close keeps your edits. Discard closes without them.\n"
        "Keep editing returns to the document.\n"
        "Demo changes stay in this session.",
        "Save and close", "Keep editing", "Discard unsaved changes");
    if (confirming && confirmation.confirmed()) {
      saved_text = draft_text;
      document_open = false;
      status = "Saved and closed North meadow field notes.txt.";
    }
    if (confirming && confirmation.result() == afterhours::DialogResult::Custom) {
      draft_text = saved_text;
      document_open = false;
      status = "Discarded unsaved edits and closed the document.";
    }
    if (confirming && (confirmation.cancelled() || confirmation.dismissed())) {
      status = "Returned to editor. Unsaved changes kept.";
      return_to_editor = true;
    }
    if (!confirmation) {
      if (return_to_editor && document_open && editor_id >= 0 && context.is_input_allowed(editor_id)) {
        context.set_focus(editor_id);
        return_to_editor = false;
      }
      return;
    }
    constexpr std::array<float, 3> action_widths{272, 176, 196};
    dialog_presentation::style(context, confirmation.ent(), 800, 144, action_widths);
    for (const auto id : confirmation.cmp().children) {
      auto child = UICollectionHolder::getEntityForID(id);
      if (!child.valid() || !child.asE().has<UIComponentDebug>()) continue;
      auto &row = child.asE();
      if (row.get<UIComponentDebug>().name() != "dialog_buttons") continue;
      size_t action = 0;
      for (const auto button_id : row.get<UIComponent>().children) {
        auto button_entity = UICollectionHolder::getEntityForID(button_id);
        if (!button_entity.valid()) continue;
        auto &button = button_entity.asE();
        button.addComponentIfMissing<UIComponentDebug>("df_discard")
            .set(action == 0 ? "df_discard" : action == 1 ? "df_keep" : "df_save");
        if (action == 0) {
          button.addComponentIfMissing<HasBorder>().border = Border::all({177, 88, 94, 255}, pixels(1));
          button.get<HasLabel>().explicit_text_color = afterhours::Color{241, 170, 172, 255};
          button.get<UIComponent>().desired_margin[Axis::right] = pixels(24 * s);
        }
        if (action == 1 && focus_dialog && open) {
          context.set_focus(button.id);
          focus_dialog = false;
        }
        ++action;
      }
    }
  }
};

REGISTER_EXAMPLE_SCREEN(dialog_fyi, "Component Galleries",
                        "Fyi (three-button) dialog (shown open)",
                        DialogFyiShowcase)
