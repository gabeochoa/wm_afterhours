#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include "DialogPresentation.h"
#include <array>
#include <afterhours/ah.h>
#include <afterhours/src/plugins/modal.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ModalShowcase : ScreenSystem<UIContext<InputAction>> {
  // Modal visibility states
  bool show_basic_modal = false;
  bool show_composable_modal = false;
  bool show_info_modal = false;
  bool show_confirm_modal = false;
  bool show_fyi_modal = false;
  bool show_stacked_settings = false;
  bool show_stacked_confirm = false;

  // Results tracking
  afterhours::DialogResult last_confirm_result =
      afterhours::DialogResult::Pending;
  afterhours::DialogResult last_fyi_result = afterhours::DialogResult::Pending;
  int confirm_count = 0;
  int cancel_count = 0;
  int background_click_count = 0; // For testing input blocking

  // Entity IDs for modals
  static constexpr int MODAL_BASIC = 100;
  static constexpr int MODAL_COMPOSABLE = 101;
  static constexpr int MODAL_INFO = 102;
  static constexpr int MODAL_CONFIRM = 103;
  static constexpr int MODAL_FYI = 104;
  static constexpr int MODAL_SETTINGS = 105;
  static constexpr int MODAL_NESTED_CONFIRM = 106;

  struct FormOptions { bool a = false; bool b = true; };
  FormOptions saved_options, draft_options;
  bool form_reset_pending = false;
  bool settings_reset_pending = false;
  bool notifications = true;
  bool dark_mode = false;
  bool confirm_opened = false;
  bool notice_opened = false;
  std::string info_status = "Not opened";
  std::string form_status = "Not opened";

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    Theme theme = afterhours::ui::theme_presets::neon_dark();
    theme.background = {14, 22, 34, 255};
    theme.surface = {27, 39, 56, 255};
    theme.primary = {44, 92, 125, 255};
    theme.secondary = {53, 68, 90, 255};
    theme.accent = {50, 110, 139, 255};
    theme.font = {236, 242, 248, 255};
    theme.font_muted = {181, 199, 217, 255};
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_grid_snapping(false);
    UIStylingDefaults::get().set_default_font("AtkinsonMock", pixels(22 * s));
    const auto at = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(x * s, y * s).with_corner_radius(0)
          .with_background(Theme::Usage::None);
    };
    div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_custom_background(theme.background).with_corner_radius(0)
        .with_debug_name("modal_bg"));
    auto main = div(context, mk(entity, 1), at(0, 0, 1280, 720)
        .with_absolute_position((context.screen_width - 1280 * s) / 2,
                                (context.screen_height - 720 * s) / 2)
        .with_debug_name("modal_main"));
    const auto text = [&](int id, const std::string &label, float x, float y,
                          float w, float h, float font_size, const std::string &name = "") {
      return div(context, mk(main.ent(), id), at(x, y, w, h).with_label(label)
          .with_font("AtkinsonMock", pixels(font_size * s)).with_letter_spacing(0)
          .with_custom_text_color(theme.font).with_text_overflow(TextOverflow::Wrap)
          .with_ignore_pointer_events().with_debug_name(name));
    };
    const auto launch = [&](int id, const std::string &label, float x, float y,
                            float w, const std::string &name) {
      return button(context, mk(main.ent(), id), at(x, y, w, 44).with_label(label)
          .with_font("AtkinsonMock", pixels(22 * s)).with_letter_spacing(0)
          .with_custom_background(theme.primary).with_custom_text_color(theme.font)
          .with_corner_radius(6 * s).with_alignment(TextAlignment::Center)
          .with_click_activation(ClickActivationMode::Release).with_debug_name(name));
    };
    const auto row = [&](int id, float y, float h, const std::string &name) {
      div(context, mk(main.ent(), id), at(48, y, 1184, h)
          .with_custom_background(theme.surface).with_corner_radius(10 * s)
          .with_debug_name(name));
    };

    // Title
    text(0, "Modal Dialogs", 48, 24, 1184, 44, 34, "modals_title");
    text(1, "Choose a dialog example and inspect its result.", 48, 74, 1184, 30, 21);

    // =========================================================================
    // Row 1: Basic Modals
    // =========================================================================
    row(10, 134, 116, "row1_basic");
    text(11, "Basic", 68, 150, 176, 40, 23);
    if (launch(12, "Simple Modal", 264, 150, 240, "btn_simple")) show_basic_modal = true;
    if (launch(13, "Composable Modal", 532, 150, 280, "btn_composable")) {
      draft_options = saved_options;
      form_reset_pending = true;
      form_status = "Open";
      show_composable_modal = true;
    }
    text(14, "Plain message dialog", 264, 202, 240, 28, 18);
    text(15, "Form controls with Save and Cancel", 532, 202, 348, 30, 18);
    text(16, "Escape closes the front dialog.\nFocus returns to its launcher.", 904, 152, 300, 72, 18);

    // =========================================================================
    // Row 2: Helper Functions
    // =========================================================================
    row(20, 264, 136, "row2_helpers");
    text(21, "Standard\ndialog types", 68, 280, 176, 70, 23);
    if (launch(22, "Info Dialog", 264, 280, 240, "btn_info")) {
      show_info_modal = true;
      info_status = "Open";
    }
    if (launch(23, "Confirmation", 532, 280, 280, "btn_confirm")) {
      show_confirm_modal = true;
      confirm_opened = true;
      last_confirm_result = afterhours::DialogResult::Pending;
    }
    if (launch(24, "Notice", 840, 280, 360, "btn_notice")) {
      show_fyi_modal = true;
      notice_opened = true;
      last_fyi_result = afterhours::DialogResult::Pending;
    }
    text(25, "Acknowledge information", 264, 331, 240, 26, 18);
    text(26, "Proceed or cancel an action", 532, 331, 280, 26, 18);
    text(27, "Choose now, later or more detail", 840, 331, 360, 26, 18);

    // =========================================================================
    // Row 3: Modal Stacking
    // =========================================================================
    row(30, 414, 98, "row3_stacking");
    text(31, "Stacking", 68, 430, 176, 36, 23);
    if (launch(32, "Open settings", 264, 428, 240, "btn_stacked")) show_stacked_settings = true;
    text(33, "Then open reset confirmation", 264, 477, 348, 27, 18);
    div(context, mk(main.ent(), 34), at(636, 432, 104, 58)
        .with_ignore_pointer_events().with_debug_name("modals_stack_preview")
        .with_on_draw_fg([=](RectangleType r) {
          const float unit = r.width / 104;
          raylib::DrawRectangleRec({r.x, r.y, 84 * unit, 44 * unit}, {58, 92, 119, 255});
          raylib::DrawRectangleLinesEx({r.x, r.y, 84 * unit, 44 * unit}, unit, {160, 187, 211, 255});
          raylib::DrawRectangleRec({r.x + 20 * unit, r.y + 14 * unit, 84 * unit, 44 * unit}, {83, 119, 148, 255});
          raylib::DrawRectangleLinesEx({r.x + 20 * unit, r.y + 14 * unit, 84 * unit, 44 * unit}, unit, {220, 234, 244, 255});
        }));
    text(35, "Two layers: the front dialog blocks input\nto settings; closing it reveals settings again.",
         772, 426, 428, 72, 18);

    // =========================================================================
    // Row 4: Input Blocking Test
    // =========================================================================
    row(40, 526, 98, "row4_blocking");
    text(41, "Background input\nblocking test", 68, 538, 176, 66, 21);
    if (launch(42, "Background Button", 264, 538, 240, "btn_background")) ++background_click_count;
    text(43, "Background clicks: " + std::to_string(background_click_count),
         532, 543, 300, 36, 22, "modals_background_count");
    text(44, "Open Simple Modal, then try this button. Its count must stay unchanged while blocked.",
         264, 587, 936, 28, 18);

    // =========================================================================
    // Row 5: Results Display
    // =========================================================================
    if (launch(50, "Reset results", 1048, 654, 184, "modals_reset")) {
      last_confirm_result = afterhours::DialogResult::Pending;
      last_fyi_result = afterhours::DialogResult::Pending;
      confirm_opened = false;
      notice_opened = false;
      confirm_count = 0;
      cancel_count = 0;
      background_click_count = 0;
      info_status = "Not opened";
      form_status = "Not opened";
    }

    // =========================================================================
    // Modal implementations
    // =========================================================================
    const auto style_dialog = [&](afterhours::Entity &panel, float width, float body_height,
                                  std::span<const float> action_widths, const std::string &name) {
      dialog_presentation::style(context, panel, width, body_height, action_widths);
      panel.get<UIComponentDebug>().set(name + "_panel");
      for (const auto id : panel.get<UIComponent>().children) {
        auto &child = UICollectionHolder::getEntityForIDEnforce(id);
        if (!child.has<UIComponentDebug>() || child.get<UIComponentDebug>().name() != "dialog_buttons") continue;
        int index = 0;
        for (const auto button_id : child.get<UIComponent>().children) {
          auto &action = UICollectionHolder::getEntityForIDEnforce(button_id);
          action.get<UIComponentDebug>().set(name + "_action_" + std::to_string(index++));
        }
      }
    };
    const auto content_config = [&](float width, float height) {
      return ComponentConfig{}.with_size({pixels(width * s), pixels(height * s)})
          .with_font("AtkinsonMock", pixels(22 * s)).with_custom_text_color(theme.font)
          .with_background(Theme::Usage::None).with_corner_radius(0)
          .with_text_overflow(TextOverflow::Wrap).with_render_layer(1001);
    };
    const auto action_config = [&](const std::string &label, float width, const std::string &name) {
      return content_config(width, 44).with_label(label).with_corner_radius(6 * s)
          .with_custom_background(theme.primary).with_alignment(TextAlignment::Center)
          .with_debug_name(name);
    };

    const auto checkbox_field = [&](afterhours::Entity &parent, int id, bool &value,
                                    const std::string &label, const std::string &debug,
                                    float width, float height, bool reset = false) {
      auto ep = mk(parent, id);
      auto [control, control_parent] = deref(ep);
      if (reset && control.has<HasCheckboxState>()) control.get<HasCheckboxState>().on = value;
      auto result = checkbox(context, ep, value,
          content_config(width, height).with_label(label).with_debug_name(debug).with_checkbox_indicators("On", "Off"));
      const auto &children = result.cmp().children;
      if (children.size() != 2) return;
      auto &label_entity = UICollectionHolder::getEntityForIDEnforce(children[0]);
      label_entity.get<UIComponent>().set_desired_width(pixels((width - 64) * s));
      auto &toggle = UICollectionHolder::getEntityForIDEnforce(children[1]);
      toggle.get<UIComponent>().set_desired_width(pixels(64 * s));
      toggle.get<UIComponentDebug>().set(debug + "_toggle");
    };

    // Basic modal - uses ClosedBy::Any so it closes on backdrop click
    if (auto m = afterhours::modal(context, mk(entity, MODAL_BASIC), show_basic_modal,
          afterhours::ModalConfig{}.with_size(pixels(520 * s), pixels(260 * s))
              .with_title("Basic Modal").with_closed_by(afterhours::ClosedBy::Any))) {
      div(context, mk(m.ent(), 0), content_config(472, 72)
          .with_label("This is a simple modal dialog.\nClose it, press Escape or click the backdrop."));
      if (button(context, mk(m.ent(), 1), action_config("Close", 132, "modals_basic_close")))
        show_basic_modal = false;
      style_dialog(m.ent(), 520, 100, {}, "modals_basic");
    }

    // Composable modal with custom content
    const bool form_was_open = show_composable_modal;
    if (auto m = afterhours::modal(context, mk(entity, MODAL_COMPOSABLE), show_composable_modal,
          afterhours::ModalConfig{}.with_size(pixels(580 * s), pixels(336 * s))
              .with_title("Composable Modal"))) {
      div(context, mk(m.ent(), 0), content_config(532, 52)
          .with_label("Edit these options. Save keeps them; Cancel discards edits."));
      // Checkboxes
      checkbox_field(m.ent(), 1, draft_options.a,
          draft_options.a ? "Option A: On" : "Option A: Off", "modals_option_a", 532, 36, form_reset_pending);
      checkbox_field(m.ent(), 2, draft_options.b,
          draft_options.b ? "Option B: On" : "Option B: Off", "modals_option_b", 532, 36, form_reset_pending);
      form_reset_pending = false;
      // Progress indicator
      div(context, mk(m.ent(), 3), content_config(532, 36).with_label("Progress: 65% / static example"));
      // Buttons row
      auto buttons = hstack(context, mk(m.ent(), 4), content_config(532, 48)
          .with_no_wrap().with_gap(pixels(12 * s)).with_justify_content(JustifyContent::FlexEnd));
      if (button(context, mk(buttons.ent(), 0), action_config("Cancel", 132, "modals_form_cancel"))) {
        form_status = "Cancelled";
        show_composable_modal = false;
      }
      if (button(context, mk(buttons.ent(), 1), action_config("Save", 132, "modals_form_save"))) {
        saved_options = draft_options;
        form_status = "Saved";
        show_composable_modal = false;
      }
      style_dialog(m.ent(), 580, 176, {}, "modals_form");
    }
    if (form_was_open && !show_composable_modal && form_status == "Open") form_status = "Dismissed";

    // modal::info helper
    const bool info_was_open = show_info_modal;
    auto info = afterhours::modal::info(context, mk(entity, MODAL_INFO), show_info_modal,
        "Information", "This is an informational message. Choose Got it to acknowledge it.", "Got it");
    if (info) style_dialog(info.ent(), 600, 84, std::array<float, 1>{132}, "modals_info");
    if (info_was_open && !show_info_modal)
      info_status = info.ent().get<afterhours::modal::Modal>().result == afterhours::DialogResult::Dismissed
          ? "Dismissed" : "Acknowledged";

    // modal::confirm helper
    const bool confirm_was_open = show_confirm_modal;
    auto confirm_result = afterhours::modal::confirm(context, mk(entity, MODAL_CONFIRM),
        show_confirm_modal, "Confirm Action", "Are you sure you want to proceed with this demo action?",
        "Yes, proceed", "Cancel");
    if (confirm_result) style_dialog(confirm_result.ent(), 640, 84,
        std::array<float, 2>{132, 180}, "modals_confirm");
    // Track confirm results
    if (confirm_was_open && !show_confirm_modal) {
      last_confirm_result = confirm_result.result();
      if (confirm_result.confirmed()) ++confirm_count;
      if (confirm_result.cancelled()) ++cancel_count;
    }

    // modal::fyi helper
    const bool notice_was_open = show_fyi_modal;
    auto fyi_result = afterhours::modal::fyi(context, mk(entity, MODAL_FYI), show_fyi_modal,
        "Update Available", "Version 2.0 is an example notice. These buttons report a choice; no download starts.",
        "Install Now", "Later", "Release Notes");
    if (fyi_result) style_dialog(fyi_result.ent(), 740, 84,
        std::array<float, 3>{192, 120, 164}, "modals_notice");
    if (notice_was_open && !show_fyi_modal) last_fyi_result = fyi_result.result();

    // Stacked modals - Settings with nested confirm
    if (auto settings = afterhours::modal(context, mk(entity, MODAL_SETTINGS), show_stacked_settings,
          afterhours::ModalConfig{}.with_size(pixels(600 * s), pixels(308 * s)).with_title("Settings"))) {
      div(context, mk(settings.ent(), 0), content_config(552, 48)
          .with_label("Preferences update immediately in this example."));
      checkbox_field(settings.ent(), 1, notifications,
          notifications ? "Enable notifications: On" : "Enable notifications: Off",
          "modals_notifications", 552, 40, settings_reset_pending);
      checkbox_field(settings.ent(), 2, dark_mode,
          dark_mode ? "Dark mode: On" : "Dark mode: Off", "modals_dark_mode", 552, 40, settings_reset_pending);
      settings_reset_pending = false;
      auto buttons = hstack(context, mk(settings.ent(), 3), content_config(552, 48)
          .with_no_wrap().with_justify_content(JustifyContent::SpaceBetween));
      if (button(context, mk(buttons.ent(), 0),
          action_config("Reset to Defaults", 240, "modals_settings_reset"))) {
        show_stacked_confirm = true; // Open nested modal
      }
      if (button(context, mk(buttons.ent(), 1), action_config("Done", 132, "modals_settings_done")))
        show_stacked_settings = false;
      style_dialog(settings.ent(), 600, 148, {}, "modals_settings");
    }

    // Nested confirm modal (stacked on top of settings)
    const bool nested_was_open = show_stacked_confirm;
    auto nested = afterhours::modal::confirm(context, mk(entity, MODAL_NESTED_CONFIRM),
        show_stacked_confirm, "Reset Settings?", "Reset notifications to On and dark mode to Off?", "Reset", "Cancel");
    if (nested) style_dialog(nested.ent(), 660, 84,
        std::array<float, 2>{132, 132}, "modals_nested");
    if (nested_was_open && !show_stacked_confirm && nested.confirmed()) {
      notifications = true;
      dark_mode = false;
      settings_reset_pending = true;
    }
    const auto result_name = [](afterhours::DialogResult result, bool opened, bool open) {
      if (open) return std::string("Open");
      if (!opened) return std::string("Not opened");
      if (result == afterhours::DialogResult::Confirmed) return std::string("Confirmed");
      if (result == afterhours::DialogResult::Cancelled) return std::string("Cancelled");
      if (result == afterhours::DialogResult::Custom) return std::string("Release Notes");
      return std::string("Dismissed");
    };
    const std::string confirm_name = result_name(last_confirm_result, confirm_opened, show_confirm_modal);
    std::string notice_name = result_name(last_fyi_result, notice_opened, show_fyi_modal);
    if (last_fyi_result == afterhours::DialogResult::Confirmed && !show_fyi_modal) notice_name = "Install Now";
    if (last_fyi_result == afterhours::DialogResult::Cancelled && !show_fyi_modal) notice_name = "Later";
    text(60, "Info: " + info_status, 264, 364, 240, 27, 18, "modals_info_result");
    text(61, "Confirmation: " + confirm_name, 532, 364, 280, 27, 18, "modals_confirm_result");
    text(62, "Notice: " + notice_name, 840, 364, 360, 27, 18, "modals_notice_result");
    text(63, "Confirmed: " + std::to_string(confirm_count), 48, 663, 240, 28, 18, "modals_confirm_count");
    text(64, "Cancelled: " + std::to_string(cancel_count), 308, 663, 240, 28, 18, "modals_cancel_count");
    text(65, "Form: " + form_status, 568, 663, 440, 28, 18, "modals_form_result");
  }
};

REGISTER_EXAMPLE_SCREEN(modals, "Component Galleries",
                        "Modal dialog system demo", ModalShowcase)
