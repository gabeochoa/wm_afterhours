#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/modal.h>

#include "DialogBackdrop.h"
#include "DialogPresentation.h"
#include <array>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// modal::prompt shown open: message + text input + OK/Cancel.
struct DialogPromptShowcase : ScreenSystem<UIContext<InputAction>> {
  bool open = true;
  bool submit_requested = false;
  std::string current_name = "untitled_map";
  std::string value = current_name;
  std::string status = "3 demo files. untitled_map.map is selected.";

  std::string validation() const {
    if (value.empty()) return "Enter a name before renaming.";
    if (value.size() > 40) return "Use 40 characters or fewer.";
    std::string folded;
    for (const unsigned char character : value) {
      if (!((character >= 'a' && character <= 'z') ||
            (character >= 'A' && character <= 'Z') ||
            (character >= '0' && character <= '9') || character == '_' || character == '-'))
        return "Use letters, numbers, underscores, or hyphens.";
      folded += static_cast<char>(character >= 'A' && character <= 'Z' ? character + ('a' - 'A') : character);
    }
    if (folded == "harbour" || folded == "research_deck") return "That filename already exists in this folder.";
    return "";
  }

  void rename_file() {
    const auto previous = current_name;
    current_name = value;
    status = previous == current_name ? "Name unchanged: " + current_name + ".map"
                                     : "Renamed " + previous + ".map to " + current_name + ".map";
    open = false;
  }

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context,
                     float) override {
    context.theme = afterhours::ui::theme_presets::neon_dark();
    context.theme.surface = {30, 40, 57, 255};
    context.theme.primary = {44, 93, 155, 255};
    context.theme.secondary = {17, 25, 38, 255};
    context.theme.accent = {128, 186, 250, 255};
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);

    // A real app to dim, not one word at x=0. See DialogBackdrop.h.
    dialog_backdrop::draw_context(context, entity, "Files",
        {{current_name + ".map", "Selected / 64 KB / map file", true},
         {"harbour.map", "128 KB / map file"},
         {"research_deck.map", "96 KB / map file"},
         {"Folder", "Harbour Studio / maps"},
         {"Storage", "In-memory demo files"}}, status);
    const auto control = [s](float x, float y, const std::string &label, const std::string &name) {
      return ComponentConfig{}.with_size({pixels(240 * s), pixels(44 * s)})
          .with_absolute_position(x * s, y * s).with_label(label)
          .with_font("AtkinsonMock", pixels(22 * s)).with_background(Theme::Usage::Primary)
          .with_custom_text_color({235, 241, 250, 255}).with_corner_radius(8 * s).with_debug_name(name);
    };
    if (button(context, mk(entity, 20), control(972, 92, "Rename selected file", "dp_open"))) {
      value = current_name;
      open = true;
    }
    if (button(context, mk(entity, 21), control(972, 560, "Reset file demo", "dp_reset"))) {
      current_name = "untitled_map";
      value = current_name;
      status = "Demo reset. All 3 original filenames restored.";
    }
    auto *fonts = afterhours::EntityHelper::get_singleton_cmp<FontManager>();
    const auto previous_active_font = fonts ? fonts->active_font : std::string{};
    auto &defaults = UIStylingDefaults::get();
    const auto previous_font_name = defaults.default_font_name;
    const auto previous_font_size = defaults.default_font_size;
    defaults.set_default_font("AtkinsonMock", pixels(22 * s));
    const bool was_open = open;
    auto prompt = afterhours::modal::prompt(context, mk(entity, 1), open, value, "Rename file",
        "Current: " + current_name + ".map", "Rename", "Cancel");
    defaults.set_default_font(previous_font_name, previous_font_size);
    if (fonts) fonts->set_active(previous_active_font);
    const auto error = validation();
    if (was_open && (prompt.confirmed() || submit_requested)) {
      if (error.empty()) rename_file();
      else open = true;
    }
    submit_requested = false;
    if (was_open && (prompt.cancelled() || prompt.dismissed())) {
      value = current_name;
      status = "Rename cancelled. Kept " + current_name + ".map";
    }
    if (!prompt) return;
    constexpr std::array<float, 2> action_widths{140, 160};
    dialog_presentation::style(context, prompt.ent(), 640, 238, action_widths);
    prompt.ent().addComponentIfMissing<UIComponentDebug>("dp_panel").set("dp_panel");
    const auto place = [s](UIComponent &cmp, float x, float y, float w, float h) {
      cmp.absolute = true;
      cmp.absolute_pos_x = x * s;
      cmp.absolute_pos_y = y * s;
      cmp.desired[Axis::X] = pixels(w * s);
      cmp.desired[Axis::Y] = pixels(h * s);
    };
    for (const auto id : prompt.cmp().children) {
      auto child = UICollectionHolder::getEntityForID(id);
      if (!child.valid() || !child.asE().has<UIComponent>()) continue;
      auto &node = child.asE();
      auto &cmp = node.get<UIComponent>();
      const auto name = node.has<UIComponentDebug>() ? node.get<UIComponentDebug>().name() : "";
      if (node.has<afterhours::text_input::HasTextInputState>()) {
        place(cmp, 0, 140, 512, 44);
        node.addComponentIfMissing<UIComponentDebug>("dp_name").set("dp_name");
        node.addComponentIfMissing<afterhours::text_input::HasTextInputListener>().on_submit = [this](afterhours::Entity &) { submit_requested = true; };
        for (const auto field_id : cmp.children) {
          auto field = UICollectionHolder::getEntityForID(field_id);
          if (!field.valid() || !field.asE().has<UIComponent>()) continue;
          auto &field_node = field.asE();
          auto &field_cmp = field_node.get<UIComponent>();
          field_cmp.desired[Axis::X] = pixels(512 * s);
          field_cmp.desired[Axis::Y] = pixels(44 * s);
          field_node.addComponentIfMissing<afterhours::HasColor>(afterhours::Color{17, 25, 38, 255}).set({17, 25, 38, 255});
          field_node.addComponentIfMissing<HasBorder>().border = Border::all(
              context.has_focus(field_node.id) || context.has_focus(node.id)
                  ? afterhours::Color{128, 186, 250, 255} : afterhours::Color{112, 132, 160, 255}, pixels(s));
          for (const auto overlay_id : field_cmp.children) {
            auto overlay = UICollectionHolder::getEntityForID(overlay_id);
            if (!overlay.valid() || !overlay.asE().has<UIComponentDebug>()) continue;
            if (overlay.asE().get<UIComponentDebug>().name() != "selection") continue;
            overlay.asE().get<afterhours::HasColor>().set({54, 113, 194, 110});
          }
        }
        continue;
      }
      if (name == "dialog_buttons") {
        place(cmp, 0, 294, 592, 56);
        size_t index = 0;
        for (const auto action_id : cmp.children) {
          auto action = UICollectionHolder::getEntityForID(action_id);
          if (!action.valid()) continue;
          auto &button = action.asE();
          const bool confirm = index++ == 1;
          button.addComponentIfMissing<UIComponentDebug>("dp_cancel").set(confirm ? "dp_rename" : "dp_cancel");
          if (!confirm || error.empty()) continue;
          button.get<HasLabel>().is_disabled = true;
          button.removeComponentIfExists<HasClickListener>();
        }
        continue;
      }
      if (name == "modal_header") continue;
      place(cmp, 0, 48, 592, 28);
      if (node.has<HasLabel>()) node.get<HasLabel>().set_text_overflow(TextOverflow::Ellipsis);
      for (auto axis : {Axis::left, Axis::right, Axis::top, Axis::bottom}) cmp.desired_padding[axis] = pixels(0);
    }
    const int layer = prompt.cmp().render_layer + 2;
    const auto label = [&](int id, const std::string &text, float x, float y, float w, float h,
                           float font_size, afterhours::Color color, const std::string &name) {
      div(context, mk(prompt.ent(), id), ComponentConfig{}
          .with_size({pixels(w * s), pixels(h * s)}).with_absolute_position(x * s, y * s)
          .with_label(text).with_text_overflow(TextOverflow::Ellipsis).with_font("AtkinsonMock", pixels(font_size * s))
          .with_custom_text_color(color).with_background(Theme::Usage::None)
          .with_ignore_pointer_events().with_render_layer(layer).with_debug_name(name));
    };
    label(105, "Folder: Harbour Studio / maps", 0, 78, 592, 26, 20, {188, 203, 226, 255}, "dp_folder");
    label(100, "New name", 0, 110, 592, 26, 22, {235, 241, 250, 255}, "dp_name_label");
    label(101, ".map", 528, 140, 64, 44, 22, {202, 216, 237, 255}, "dp_extension");
    label(102, "1-40 letters, numbers, _ or -. The .map extension stays.",
          0, 194, 592, 26, 18, {188, 203, 226, 255}, "dp_guidance");
    label(103, error.empty() ? "Name available in this folder." : error,
          0, 224, 592, 26, 20, error.empty() ? afterhours::Color{152, 214, 183, 255}
                                         : afterhours::Color{255, 178, 164, 255}, "dp_validation");
    label(104, "Preview: " + (value.empty() ? std::string("(enter a name)") : value + ".map"),
          0, 254, 592, 30, 22, {235, 241, 250, 255}, "dp_preview");
  }
};

REGISTER_EXAMPLE_SCREEN(dialog_prompt, "Component Galleries",
                        "Prompt (input) dialog (shown open)",
                        DialogPromptShowcase)
