#pragma once

// SettingsPage kit: one data-driven tabs+rows+dirty-state page built on the
// vendored imm::setting_row, replacing the hand-drawn clones in the ten
// *Settings/*Options screens (4,463 lines of the same tab header, row layout
// and saved-snapshot bookkeeping, with setting_row itself used by one
// showcase). Callers keep their bespoke chrome (artwork, preview panes,
// footers); the kit owns the tab header, the row stack, selection, dirty
// tracking (current value vs saved snapshot) and the status line.
//
// There is deliberately no Action/Button control kind: setting_row has none,
// and adding one here would fork the widget. A screen with a command row
// (e.g. "Restore Defaults") draws that one button itself, outside the kit's
// row list. Likewise an unavailable row renders through setting_row's
// read-only Display control rather than a disabled stepper, because the
// stepper would still report arrow clicks as changes.

#include "../../external.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/setting_row.h>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace settings_kit {

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

enum struct Control { Stepper, Toggle, Slider, Display };

struct Row {
  std::string label;
  Control control = Control::Stepper;
  std::vector<std::string> options; // Stepper choices; index into this.
  size_t index = 0, saved_index = 0;
  bool toggle = false, saved_toggle = false;
  float slider = 0.f, saved_slider = 0.f;
  std::string display; // Display rows only; never dirty.
  bool unavailable = false;
  std::string help;

  bool dirty() const {
    if (unavailable || control == Control::Display) return false;
    if (control == Control::Toggle) return toggle != saved_toggle;
    if (control == Control::Slider) return slider != saved_slider;
    return index != saved_index;
  }
  void apply() {
    saved_index = index;
    saved_toggle = toggle;
    saved_slider = slider;
  }
  void revert() {
    index = saved_index;
    toggle = saved_toggle;
    slider = saved_slider;
  }
  // Returns false for rows that can never change (unavailable / no choices),
  // so callers do not announce a "change" that did not happen.
  bool step(int direction) {
    if (unavailable || control != Control::Stepper || options.empty())
      return false;
    const size_t offset = direction > 0 ? 1 : options.size() - 1;
    index = (index + offset) % options.size();
    return true;
  }
  std::string current_text() const {
    if (unavailable) return "Unavailable";
    if (control == Control::Display) return display;
    if (control == Control::Toggle) return toggle ? "On" : "Off";
    if (control == Control::Slider)
      return std::to_string(static_cast<int>(slider * 100.f)) + "%";
    return options.empty() ? std::string{} : options[index % options.size()];
  }
  std::string saved_text() const {
    Row saved = *this;
    saved.revert();
    saved.unavailable = false;
    return saved.current_text();
  }
};

inline Row stepper_row(std::string label, std::vector<std::string> options,
                       size_t initial = 0, std::string help = "") {
  Row row;
  row.label = std::move(label);
  row.options = std::move(options);
  row.index = row.saved_index = initial;
  row.help = std::move(help);
  return row;
}
inline Row toggle_row(std::string label, bool initial, std::string help = "") {
  Row row;
  row.label = std::move(label);
  row.control = Control::Toggle;
  row.toggle = row.saved_toggle = initial;
  row.help = std::move(help);
  return row;
}
inline Row slider_row(std::string label, float initial, std::string help = "") {
  Row row;
  row.label = std::move(label);
  row.control = Control::Slider;
  row.slider = row.saved_slider = initial;
  row.help = std::move(help);
  return row;
}
inline Row unavailable_row(std::string label, std::string help = "") {
  Row row = stepper_row(std::move(label), {"Unavailable"}, 0, std::move(help));
  row.unavailable = true;
  return row;
}

struct Tab {
  std::string name;
  std::vector<Row> rows;
};

struct Page {
  std::vector<Tab> tabs;
  size_t active_tab = 0, selected_row = 0;
  std::string status;

  Row &selected() { return tabs[active_tab].rows[selected_row]; }
  const Row &selected() const { return tabs[active_tab].rows[selected_row]; }
  bool dirty() const {
    for (const auto &tab : tabs)
      for (const auto &row : tab.rows)
        if (row.dirty()) return true;
    return false;
  }
  void apply() {
    for (auto &tab : tabs)
      for (auto &row : tab.rows) row.apply();
  }
  void revert() {
    for (auto &tab : tabs)
      for (auto &row : tab.rows) row.revert();
  }
  void revert_tab(size_t tab) {
    for (auto &row : tabs[tab].rows) row.revert();
  }
  void select_tab(size_t tab) {
    active_tab = tab;
    selected_row = 0;
  }
  // Keyboard row navigation skips unavailable rows, matching the hand-drawn
  // screens; wraps at both ends.
  void move_selection(int direction) {
    const auto &rows = tabs[active_tab].rows;
    if (rows.empty()) return;
    const size_t offset = direction > 0 ? 1 : rows.size() - 1;
    size_t next = selected_row;
    do {
      next = (next + offset) % rows.size();
    } while (rows[next].unavailable && next != selected_row);
    selected_row = next;
  }
  bool step_selected(int direction) { return selected().step(direction); }
};

// Frame-scoped styling: pilots compute scaled sizes from their own scale
// factor each frame and hand the kit plain values, so the kit never has to
// know which resolution a screen designs against.
struct Style {
  float row_height = 44.f, row_spacing = 6.f;
  float tab_width = 180.f, tab_height = 36.f, tab_gap = 4.f;
  std::string font_name = "ArchivoMock";
  float font_size = 22.f;
  std::string tab_debug_prefix, row_debug_prefix;
  int focus_tab = -1, focus_row = -1; // entity focus request, consumed by kit
  afterhours::Color row_bg{0, 0, 0, 0}, row_bg_selected{0, 0, 0, 0};
  afterhours::Color tab_bg{0, 0, 0, 0}, tab_bg_selected{0, 0, 0, 0};
  std::optional<afterhours::Color> label_color, label_color_selected;
  std::optional<afterhours::Color> tab_text, tab_text_selected;
  std::optional<afterhours::Color> control_bg;
  TextAlignment tab_alignment = TextAlignment::Center;
  float slider_width = 200.f;
  float stepper_value_width = 160.f; // setting_row defaults to 80, which truncates long option names
};

struct Result {
  bool tab_changed = false, row_changed = false, selection_changed = false;
  bool row_selected = false; // wrapper clicked, even if it was already selected
  size_t changed_row = 0, selected_row = 0;
};

namespace detail {
// setting_row names its stepper arrows generically, so every row's arrows
// share debug names; rename them per row (same traversal SettingRowShowcase
// already does) so callers and e2e can address one row's controls.
inline void name_stepper(afterhours::Entity &stepper, const std::string &base) {
  if (!stepper.has<UIComponent>()) return;
  int direction = 0;
  for (auto arrow_id : stepper.get<UIComponent>().children) {
    auto &arrow = UICollectionHolder::getEntityForIDEnforce(arrow_id);
    if (!arrow.has<HasClickListener>()) continue;
    const std::string name = base + (direction++ == 0 ? "_previous" : "_next");
    arrow.addComponentIfMissing<UIComponentDebug>(name).set(name);
  }
}

inline void name_row_controls(afterhours::Entity &row_entity,
                              const std::string &base) {
  if (!row_entity.has<UIComponent>()) return;
  // Wrapper -> setting_row -> stepper: the stepper is a grandchild here,
  // one level deeper than in SettingRowShowcase's direct call.
  for (auto child_id : row_entity.get<UIComponent>().children) {
    auto &child = UICollectionHolder::getEntityForIDEnforce(child_id);
    if (!child.has<UIComponent>()) continue;
    if (child.has<HasStepperState>()) name_stepper(child, base);
    for (auto grandchild_id : child.get<UIComponent>().children) {
      auto &grandchild = UICollectionHolder::getEntityForIDEnforce(grandchild_id);
      if (grandchild.has<HasStepperState>()) name_stepper(grandchild, base);
    }
  }
}
} // namespace detail

// Tab header: a row (or, when vertical, a column) of tab buttons. Selecting
// a tab resets the row selection; status text policy stays with the caller.
template <typename Ctx>
Result render_tabs(Ctx &ctx, EntityParent parent, Page &page,
                   ComponentConfig container, const Style &style,
                   bool vertical = false) {
  Result result;
  container.with_gap(pixels(vertical ? style.tab_gap : style.tab_gap));
  auto tabs = vertical ? vstack(ctx, parent, container)
                       : hstack(ctx, parent, container);
  for (size_t i = 0; i < page.tabs.size(); ++i) {
    const bool active = i == page.active_tab;
    auto cfg = ComponentConfig{}
                   .with_size({pixels(style.tab_width), pixels(style.tab_height)})
                   .with_label(page.tabs[i].name)
                   .with_font(style.font_name, pixels(style.font_size))
                   .with_custom_background(active ? style.tab_bg_selected
                                                  : style.tab_bg)
                   .with_alignment(style.tab_alignment)
                   .with_debug_name(style.tab_debug_prefix + std::to_string(i));
    if (style.tab_text)
      cfg.with_custom_text_color(active && style.tab_text_selected
                                     ? *style.tab_text_selected
                                     : *style.tab_text);
    auto tab_button = button(ctx, mk(tabs.ent(), static_cast<int>(i)), cfg);
    if (style.focus_tab == static_cast<int>(i))
      ctx.set_focus(tab_button.ent().id);
    if (tab_button) {
      page.select_tab(i);
      result.tab_changed = true;
    }
  }
  return result;
}

// Row stack for the active tab. Each row is a selectable wrapper div
// (click selects without changing) around a vendored setting_row (arrows /
// switch / slider change the value). Availability maps to Display, per the
// header note.
template <typename Ctx>
Result render_rows(Ctx &ctx, EntityParent parent, Page &page,
                   ComponentConfig container, const Style &style) {
  Result result;
  container.with_gap(pixels(style.row_spacing));
  auto rows_box = vstack(ctx, parent, container);
  auto &rows = page.tabs[page.active_tab].rows;
  for (size_t i = 0; i < rows.size(); ++i) {
    Row &row = rows[i];
    const bool is_selected = i == page.selected_row;
    const std::string base = style.row_debug_prefix + std::to_string(i);
    auto wrapper = div(
        ctx, mk(rows_box.ent(), static_cast<int>(i)),
        ComponentConfig{}
            .with_size({percent(1.f), pixels(style.row_height)})
            .with_label("")
            .with_padding(Padding{})
            .without_border()
            .with_custom_background(is_selected ? style.row_bg_selected
                                                : style.row_bg)
            .with_debug_name(base));
    // A plain div with button's click listener, not a button: a button
    // wrapper offsets setting_row's label (its content box is inset for its
    // own label), while this reports clicks exactly like button() does.
    wrapper.ent().template addComponentIfMissing<HasClickListener>([](afterhours::Entity &) {});
    const bool row_clicked = wrapper.ent().template get<HasClickListener>().down;
    if (style.focus_row == static_cast<int>(i))
      ctx.set_focus(wrapper.ent().id);
    if (row_clicked) {
      result.row_selected = true;
      result.selected_row = i;
      if (!is_selected) {
        page.selected_row = i;
        result.selection_changed = true;
        result.changed_row = i;
      }
    }

    SettingRowConfig row_config;
    row_config.label = row.label;
    row_config.row_height = style.row_height;
    row_config.row_spacing = 0.f; // wrapper stack owns the gap
    row_config.slider_width = style.slider_width;
    row_config.stepper_value_width = style.stepper_value_width;
    row_config.control_type =
        row.unavailable ? SettingRowControlType::Display
        : row.control == Control::Toggle  ? SettingRowControlType::Toggle
        : row.control == Control::Slider  ? SettingRowControlType::Slider
        : row.control == Control::Display ? SettingRowControlType::Display
                                          : SettingRowControlType::Stepper;
    if (style.label_color)
      row_config.slot_label_config =
          ComponentConfig{}.with_custom_text_color(
              is_selected && style.label_color_selected ? *style.label_color_selected
                                                        : *style.label_color);
    if (style.control_bg)
      row_config.slot_control_config =
          ComponentConfig{}.with_custom_background(*style.control_bg);

    auto row_cfg = ComponentConfig{}
                       .with_size({percent(1.f), pixels(style.row_height)})
                       .with_font(style.font_name, pixels(style.font_size))
                       .with_background(Theme::Usage::None)
                       .with_debug_name(base + "_setting");
    // ElementResult holds an Entity&, so collect the bool branch by branch.
    bool changed = false;
    if (row_config.control_type == SettingRowControlType::Toggle)
      changed = setting_row(ctx, mk(wrapper.ent()), row_config, &row.toggle, row_cfg);
    else if (row_config.control_type == SettingRowControlType::Slider)
      changed = setting_row(ctx, mk(wrapper.ent()), row_config, &row.slider, row_cfg);
    else if (row_config.control_type == SettingRowControlType::Display)
      changed = setting_row(ctx, mk(wrapper.ent()), row_config,
                            row.unavailable ? std::string{"Unavailable"} : row.display,
                            row_cfg);
    else
      changed = setting_row(ctx, mk(wrapper.ent()), row_config,
                            std::make_pair(&row.index, row.options), row_cfg);
    detail::name_row_controls(wrapper.ent(), base);
    if (changed) {
      if (!is_selected) result.selection_changed = true;
      page.selected_row = i;
      result.row_changed = true;
      result.changed_row = i;
    }
  }
  return result;
}

// Status line: one label for page.status; renders nothing when empty, so a
// caller can keep it mounted permanently and drive it from state only.
template <typename Ctx>
void render_status(Ctx &ctx, EntityParent parent, const Page &page,
                   ComponentConfig config) {
  if (page.status.empty()) return;
  div(ctx, parent, config.with_label(page.status));
}

// Whole page (tab bar over rows over status) for screens without bespoke
// chrome around the settings area; pilots with their own layout call the
// three pieces above into their own panels instead.
template <typename Ctx>
Result render_page(Ctx &ctx, EntityParent parent, Page &page,
                   ComponentConfig container, const Style &style) {
  auto page_box = vstack(ctx, parent, container);
  Result result = render_tabs(ctx, mk(page_box.ent(), 0), page,
                              ComponentConfig{}.with_size(
                                  {percent(1.f), pixels(style.tab_height)}),
                              style);
  auto rows = render_rows(
      ctx, mk(page_box.ent(), 1), page,
      ComponentConfig{}.with_size({percent(1.f), children()}), style);
  render_status(ctx, mk(page_box.ent(), 2), page,
                ComponentConfig{}.with_size({percent(1.f), pixels(style.font_size + 8.f)})
                    .with_font(style.font_name, pixels(style.font_size)));
  result.row_changed = rows.row_changed;
  result.selection_changed = rows.selection_changed;
  result.row_selected = rows.row_selected;
  result.changed_row = rows.changed_row;
  result.selected_row = rows.selected_row;
  return result;
}

} // namespace settings_kit
