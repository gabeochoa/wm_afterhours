#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/ui/color_swatch.h>
#include <afterhours/src/plugins/ui/theme_io.h>
#include <optional>
#include <vector>

// Theme builder lab: token table + live preview + inspector + links.
// Undo/redo over edits; export/import as a .theme file.
struct ThemeBuilderLab : ScreenSystem<UIContext<InputAction>> {
  struct TokenRow {
    Theme::Usage usage;
    const char *name;
    const char *debug;
    const char *category;
  };
  // Order is the table order; the category header prints on change.
  static constexpr TokenRow kTokens[] = {
      {Theme::Usage::Font, "Text", "text", "Text"},
      {Theme::Usage::DarkFont, "Dark font", "darkfont", "Text"},
      {Theme::Usage::FontMuted, "Muted text", "muted", "Text"},
      {Theme::Usage::Background, "Background", "background", "Surfaces"},
      {Theme::Usage::Surface, "Surface", "surface", "Surfaces"},
      {Theme::Usage::Primary, "Primary", "primary", "Brand"},
      {Theme::Usage::Secondary, "Secondary", "secondary", "Brand"},
      {Theme::Usage::Accent, "Accent", "accent", "Brand"},
      {Theme::Usage::Error, "Error", "error", "Status"},
      {Theme::Usage::Success, "Success", "success", "Status"},
      {Theme::Usage::Warning, "Warning", "warning", "Status"},
      {Theme::Usage::Focus, "Focus ring", "focus", "Focus"},
  };
  static constexpr int kTokenCount = 12;

  Theme edited;
  bool open[kTokenCount] = {};
  // Token index this token follows, or nothing for an independent color.
  // The swatch edits the independent color; the resolved color below is
  // what the preview shows, so unlinking restores the token's own color.
  std::optional<int> link[kTokenCount] = {};
  // One shared link picker (a second popover panel breaks later clicks):
  // the row whose picker is open, and its anchor rect (stored per frame).
  int link_picker_for = -1;
  bool link_picker_open = false;
  RectangleType link_anchor[kTokenCount];
  bool link_picker_press_began_while_open = false;
  // Token index the inspector last jumped to, or -1 before any click.
  int inspected = -1;
  // Sliders are 0..1 fractions; derived into the theme below each frame,
  // so the readouts and the theme can never disagree.
  float radius01 = 0.5f; // -> panel_corner_radius 0..24px, default 12px
  float ring01 = 0.5f;   // -> focus_ring_thickness 0..6px, default 3px
  std::string preview_name = "Weekend workshop";
  bool preview_notifications = true;
  float preview_volume = 0.6f;

  std::string export_path;
  bool export_path_set = false;
  std::string file_status = "Export or import a .theme file.";

  struct ThemeSnapshot {
    afterhours::Color color[kTokenCount];
    float radius01;
    float ring01;
    std::optional<int> link[kTokenCount];
  };
  std::vector<ThemeSnapshot> undo_stack;
  std::vector<ThemeSnapshot> redo_stack;
  bool history_gesture_open = false;

  ThemeSnapshot theme_snapshot() {
    ThemeSnapshot s{};
    s.radius01 = radius01;
    s.ring01 = ring01;
    for (int i = 0; i < kTokenCount; i++) {
      s.color[i] = edited.color_ref(kTokens[i].usage);
      s.link[i] = link[i];
    }
    return s;
  }
  void apply_snapshot(const ThemeSnapshot &s) {
    for (int i = 0; i < kTokenCount; i++) {
      edited.color_ref(kTokens[i].usage) = s.color[i];
      link[i] = s.link[i];
    }
    radius01 = s.radius01;
    ring01 = s.ring01;
  }
  static bool snapshots_equal(const ThemeSnapshot &a,
                              const ThemeSnapshot &b) {
    if (a.radius01 != b.radius01 || a.ring01 != b.ring01)
      return false;
    for (int i = 0; i < kTokenCount; i++) {
      const afterhours::Color &x = a.color[i];
      const afterhours::Color &y = b.color[i];
      if (x.r != y.r || x.g != y.g || x.b != y.b || x.a != y.a)
        return false;
      if (a.link[i] != b.link[i])
        return false;
    }
    return true;
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &ctx, float) override {
    using namespace afterhours;
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;

    const ThemeSnapshot frame_start = theme_snapshot();
    bool history_suppressed = false;
    if (!export_path_set) {
      export_path_set = true;
      const auto dir = files::get_save_path();
      export_path = dir.empty()
                        ? "theme_builder_export.theme"
                        : (dir / "theme_builder_export.theme").string();
    }

    Theme resolved = edited;
    resolved.panel_corner_radius = radius01 * 24.f;
    resolved.focus_ring_thickness = ring01 * 6.f;
    for (int i = 0; i < kTokenCount; i++) {
      if (link[i]) {
        int cur = i;
        bool seen[kTokenCount] = {};
        while (link[cur] && !seen[cur]) {
          seen[cur] = true;
          cur = *link[cur];
        }
        resolved.color_ref(kTokens[i].usage) =
            edited.color_ref(kTokens[cur].usage);
      }
    }
    ctx.set_theme(resolved);
    ctx.scaling_mode = ScalingMode::Proportional;

    const auto layout = [](float w, float h) {
      return ComponentConfig{}.with_size({w1280(w), h720(h)});
    };
    const Color chrome_text{240, 245, 253, 255};
    const Color chrome_muted{181, 195, 216, 255};
    const Color chrome_highlight{255, 213, 79, 255};
    const Color chrome_bg{14, 19, 28, 255};
    const Color swatch_bg{42, 54, 71, 255};

    div(ctx, mk(entity),
        ComponentConfig{}
            .with_size({pixels(ctx.screen_width), pixels(ctx.screen_height)})
            .with_absolute_position(0, 0)
            .with_custom_background(chrome_bg)
            .with_corner_radius(0)
            .with_debug_name("tb_root"));
    auto page = vstack(ctx, mk(entity),
                       layout(1232, 688)
                           .with_margin(Margin{.top = h720(16),
                                              .left = w1280(24)})
                           .with_gap(h720(8))
                           .with_transparent_bg()
                           .with_debug_name("tb_page"));
    auto header = vstack(ctx, mk(page.ent()),
                         layout(1232, 56)
                             .with_gap(h720(4))
                             .with_transparent_bg());
    auto title_row = hstack(ctx, mk(header.ent()),
                              layout(1232, 32)
                                  .with_gap(w1280(8))
                                  .with_no_wrap()
                                  .with_transparent_bg());
    div(ctx, mk(title_row.ent()),
        layout(996, 32)
            .with_label("Theme builder")
            .with_font_size(FontSize::Large)
            .with_font_weight(colors::FontWeight::Bold)
            .with_custom_text_color(chrome_text)
            .with_transparent_bg());
    if (button(ctx, mk(title_row.ent()),
               ComponentConfig{}
                   .with_size({w1280(110), h720(32)})
                   .with_label("Undo")
                   .with_font("AtkinsonMock", FontSize::Small)
                   .with_custom_background(swatch_bg)
                   .with_custom_text_color(chrome_text)
                   .with_disabled(undo_stack.empty())
                   .with_debug_name("tb_undo"))) {
      redo_stack.push_back(theme_snapshot());
      apply_snapshot(undo_stack.back());
      undo_stack.pop_back();
      history_suppressed = true;
      history_gesture_open = false;
    }
    if (button(ctx, mk(title_row.ent()),
               ComponentConfig{}
                   .with_size({w1280(110), h720(32)})
                   .with_label("Redo")
                   .with_font("AtkinsonMock", FontSize::Small)
                   .with_custom_background(swatch_bg)
                   .with_custom_text_color(chrome_text)
                   .with_disabled(redo_stack.empty())
                   .with_debug_name("tb_redo"))) {
      undo_stack.push_back(theme_snapshot());
      apply_snapshot(redo_stack.back());
      redo_stack.pop_back();
      history_suppressed = true;
      history_gesture_open = false;
    }
    div(ctx, mk(header.ent()),
        layout(1232, 20)
            .with_label("Edit every theme token and watch the preview follow.")
            .with_custom_text_color(chrome_muted)
            .with_transparent_bg());

    auto main = hstack(ctx, mk(page.ent()),
                       layout(1232, 624)
                           .with_gap(w1280(16))
                           .with_no_wrap()
                           .with_transparent_bg());

    // TODO: reevaluate — swatch container is 52 tall (28 button + 24 gap).
    // Gap is not inheritable, so rows size to children().
    auto table = hstack(ctx, mk(main.ent()),
                        layout(816, 624)
                            .with_gap(w1280(16))
                            .with_no_wrap()
                            .with_transparent_bg()
                            .with_debug_name("tb_table"));
    bool was_open[kTokenCount] = {};
    bool was_link_picker_open = link_picker_open;
    for (int i = 0; i < kTokenCount; i++)
      was_open[i] = open[i];
    // Linking from -> to cycles if to's chain reaches from.
    const auto can_link = [&](int from, int to) {
      bool seen[kTokenCount] = {};
      int cur = to;
      while (cur >= 0 && !seen[cur]) {
        if (cur == from)
          return false;
        seen[cur] = true;
        cur = link[cur].value_or(-1);
      }
      return true;
    };
    const auto token_column = [&](afterhours::Entity &col, int begin,
                                  int end) {
      int child = 0;
      const char *current_category = "";
      for (int i = begin; i < end; i++) {
        const TokenRow &token = kTokens[i];
        if (std::string(token.category) != current_category) {
          current_category = token.category;
          div(ctx, mk(col, child++),
              layout(400, 20)
                  .with_label(token.category)
                  .with_font_weight(colors::FontWeight::Bold)
                  .with_custom_text_color(chrome_text)
                  .with_transparent_bg()
                  .with_debug_name(std::string("tb_category_") +
                                   token.category));
        }
        auto token_row = hstack(ctx, mk(col, child++),
                                ComponentConfig{}
                                    .with_size({w1280(400), children()})
                                    .with_gap(w1280(8))
                                    .with_no_wrap()
                                    .with_transparent_bg());
        auto left = vstack(ctx, mk(token_row.ent(), 0),
                           ComponentConfig{}
                               .with_size({w1280(110), children()})
                               .with_gap(h720(2))
                               .with_transparent_bg());
        div(ctx, mk(left.ent(), 0),
            layout(110, 28)
                .with_label(token.name)
                .with_font_size(FontSize::Small)
                .with_custom_text_color(inspected == i ? chrome_highlight
                                                       : chrome_text)
                .with_transparent_bg());
        const std::string link_label =
            link[i] ? fmt::format("-> {}", kTokens[*link[i]].name) : "Link...";
        auto link_button = button(ctx, mk(left.ent(), 1),
                                  ComponentConfig{}
                                      .with_size({w1280(110), h720(24)})
                                      .with_label(link_label)
                                      .with_font("AtkinsonMock",
                                                 FontSize::Small)
                                      .with_custom_background(swatch_bg)
                                      .with_custom_text_color(chrome_text)
                                      .with_debug_name(std::string("tb_link_") +
                                                       token.debug));
        link_anchor[i] =
            link_button.ent().template get<UIComponent>().rect();
        if (ctx.mouse.just_pressed && link_picker_open &&
            link_picker_for == i &&
            is_mouse_inside(ctx.mouse.pos, link_anchor[i]))
          link_picker_press_began_while_open = true;
        const bool toggle_link =
            link_button && !link_picker_press_began_while_open;
        if (toggle_link) {
          if (link_picker_open && link_picker_for == i) {
            link_picker_open = false;
            link_picker_for = -1;
          } else {
            link_picker_for = i;
            link_picker_open = true;
          }
        }
        if (link[i]) {
          const Color c = resolved.color_ref(token.usage);
          div(ctx, mk(token_row.ent(), 1),
              ComponentConfig{}
                  .with_size({expand(), h720(28)})
                  .with_label(fmt::format("#{}",
                                          colors::to_hex_string(c, c.a < 255)))
                  .with_font("AtkinsonMock", FontSize::Small)
                  .with_custom_text_color(chrome_text)
                  .with_transparent_bg()
                  .with_debug_name(std::string("tb_resolved_") + token.debug));
        } else {
          color_swatch_presets(
              ctx, mk(token_row.ent(), 1), edited.color_ref(token.usage),
              open[i], default_color_presets(),
              ComponentConfig{}
                  .with_size({expand(), h720(28)})
                  // TODO: reevaluate — the button inset is charged on
                  // both sides, so only a narrow font fits the label.
                  .with_font("AtkinsonMock", FontSize::Small)
                  .with_custom_background(swatch_bg)
                  .with_custom_text_color(chrome_text)
                  .with_debug_name(std::string("tb_swatch_") + token.debug));
        }
      }
      return child;
    };
    auto col0 = vstack(ctx, mk(table.ent()),
                       layout(400, 624)
                           .with_gap(h720(3))
                           .with_transparent_bg());
    token_column(col0.ent(), 0, 8);
    auto col1 = vstack(ctx, mk(table.ent()),
                       layout(400, 624)
                           .with_gap(h720(3))
                           .with_transparent_bg());
    int col1_child = token_column(col1.ent(), 8, kTokenCount);
    // One palette or link picker open at a time, like the colour-swatch
    // lab: a newly opened row closes the rest on the next frame.
    if (link_picker_open && !was_link_picker_open) {
      for (int j = 0; j < kTokenCount; j++)
        open[j] = false;
    } else {
      for (int i = 0; i < kTokenCount; i++) {
        if (open[i] && !was_open[i]) {
          link_picker_open = false;
          link_picker_for = -1;
          for (int j = 0; j < kTokenCount; j++)
            if (j != i)
              open[j] = false;
          break;
        }
      }
    }
    // Clear after every row has checked it: an earlier row would clear
    // the flag before the active row's toggle check runs.
    if (ctx.mouse.just_released)
      link_picker_press_began_while_open = false;
    const int picker_for = link_picker_for;
    int picker_option_count = 1;
    if (picker_for >= 0) {
      picker_option_count = link[picker_for] ? 1 : 0;
      for (int j = 0; j < kTokenCount; j++)
        if (can_link(picker_for, j))
          picker_option_count++;
      if (picker_option_count == 0)
        picker_option_count = 1;
    }
    auto picker_config = ComponentConfig{}
                             .with_size({pixels(220.f),
                                         pixels(picker_option_count * 24.f)})
                             .with_border(ctx.theme.from_usage(
                                              Theme::Usage::Secondary),
                                          1.f)
                             .with_corner_radius(4.f)
                             .with_render_layer(1);
    if (auto pop = popover(ctx, mk(table.ent(), 2),
                           picker_for >= 0 ? link_anchor[picker_for]
                                           : RectangleType{},
                           link_picker_open, overlay::Placement::Below,
                           picker_config)) {
      auto body = vstack(ctx, mk(pop.ent(), 0),
                         ComponentConfig{}
                             .with_size({pixels(218.f), children()})
                             .with_transparent_bg());
      const TokenRow &picker_token = kTokens[picker_for];
      int option = 0;
      if (link[picker_for]) {
        if (button(ctx, mk(body.ent(), option++),
                   ComponentConfig{}
                       .with_size({pixels(218.f), h720(24)})
                       .with_label("Unlink")
                       .with_font_size(FontSize::Small)
                       .with_debug_name(std::string("tb_link_unlink_") +
                                        picker_token.debug))) {
          link[picker_for].reset();
          link_picker_open = false;
          link_picker_for = -1;
        }
      }
      for (int j = 0; j < kTokenCount; j++) {
        if (!can_link(picker_for, j))
          continue;
        if (button(ctx, mk(body.ent(), option++),
                   ComponentConfig{}
                       .with_size({pixels(218.f), h720(24)})
                       .with_label(fmt::format("Follow {}", kTokens[j].name))
                       .with_font_size(FontSize::Small)
                       .with_debug_name(std::string("tb_link_target_") +
                                        kTokens[j].debug))) {
          link[picker_for] = j;
          link_picker_open = false;
          link_picker_for = -1;
        }
      }
      if (option == 0) {
        div(ctx, mk(body.ent(), option++),
            ComponentConfig{}
                .with_size({pixels(218.f), h720(24)})
                .with_label("No link targets")
                .with_font_size(FontSize::Small)
                .with_custom_text_color(chrome_muted)
                .with_transparent_bg());
      }
    }

    // ---- shape scalars the preview shows ----
    div(ctx, mk(col1.ent()),
        layout(400, 20)
            .with_label("Shape")
            .with_font_weight(colors::FontWeight::Bold)
            .with_custom_text_color(chrome_text)
            .with_transparent_bg()
            .with_debug_name("tb_category_shape"));
    const auto shape_row = [&](afterhours::Entity &parent, int child,
                               const char *label,
                               const std::string &readout, float &value,
                               const char *slider_debug,
                               const char *readout_debug) {
      auto shape = hstack(ctx, mk(parent, child),
                          ComponentConfig{}
                              .with_size({w1280(400), children()})
                              .with_gap(w1280(8))
                              .with_no_wrap()
                              .with_transparent_bg());
      div(ctx, mk(shape.ent(), 0),
          layout(110, 28)
              .with_label(label)
              .with_font_size(FontSize::Small)
              .with_custom_text_color(chrome_text)
              .with_transparent_bg());
      // A slider holds its own state and overwrites the caller's value
      // every frame, so an outside edit (undo, import) must be pushed
      // back into the held state or the next frame undoes it. Same
      // pattern as the swatch channel sliders: only when the slider
      // itself did not change this frame.
      const float desired = value;
      auto slider_result = slider(ctx, mk(shape.ent(), 1), value,
                                  ComponentConfig{}
                                      .with_size({expand(), h720(28)})
                                      .with_debug_name(slider_debug));
      if (!static_cast<bool>(slider_result)) {
        value = desired;
        if (slider_result.ent().template has<UIComponent>()) {
          for (const EntityID child_id :
               slider_result.ent().template get<UIComponent>().children) {
            Entity &child =
                UICollectionHolder::getEntityForIDEnforce(child_id);
            if (child.template has<HasSliderState>())
              child.template get<HasSliderState>().value =
                  std::clamp(desired, 0.f, 1.f);
          }
        }
      }
      div(ctx, mk(shape.ent(), 2),
          layout(80, 28)
              .with_label(readout)
              .with_custom_text_color(chrome_text)
              .with_transparent_bg()
              .with_debug_name(readout_debug));
    };
    shape_row(col1.ent(), col1_child++, "Panel radius",
              fmt::format("{}px",
                          static_cast<int>(std::round(radius01 * 24.f))),
              radius01, "tb_radius_slider", "tb_radius_readout");
    // Integer readouts: an E2E slider drag lands within a percent of
    // its target, so a decimal readout would flake.
    shape_row(col1.ent(), col1_child++, "Focus ring",
              fmt::format("{}px",
                          static_cast<int>(std::round(ring01 * 6.f))),
              ring01, "tb_ring_slider", "tb_ring_readout");
    div(ctx, mk(col1.ent()),
        layout(400, 20)
            .with_label("Edits apply to this screen only.")
            .with_custom_text_color(chrome_muted)
            .with_transparent_bg());

    // ---- inspector readout: the jump lands here, in the table ----
    div(ctx, mk(col1.ent()),
        layout(400, 20)
            .with_label("Inspector")
            .with_font_weight(colors::FontWeight::Bold)
            .with_custom_text_color(chrome_text)
            .with_transparent_bg()
            .with_debug_name("tb_category_inspector"));
    div(ctx, mk(col1.ent()),
        layout(400, 20)
            .with_label("Click a preview element.")
            .with_custom_text_color(chrome_muted)
            .with_transparent_bg());
    std::string inspected_text = "Inspected: none.";
    if (inspected >= 0 && inspected < kTokenCount) {
      const TokenRow &token = kTokens[inspected];
      const Color c = resolved.color_ref(token.usage);
      if (link[inspected])
        inspected_text = fmt::format("Inspected: {} -> {} #{}", token.name,
                                     kTokens[*link[inspected]].name,
                                     colors::to_hex_string(c, c.a < 255));
      else
        inspected_text = fmt::format("Inspected: {} #{}", token.name,
                                     colors::to_hex_string(c, c.a < 255));
    }
    div(ctx, mk(col1.ent()),
        layout(400, 20)
            .with_label(inspected_text)
            .with_custom_text_color(chrome_text)
            .with_transparent_bg()
            .with_debug_name("tb_inspected"));

    // ---- theme file: export flattens links into a plain theme ----
    div(ctx, mk(col1.ent()),
        layout(400, 20)
            .with_label("Theme file")
            .with_font_weight(colors::FontWeight::Bold)
            .with_custom_text_color(chrome_text)
            .with_transparent_bg()
            .with_debug_name("tb_category_file"));
    ui::imm::text_input(ctx, mk(col1.ent()), export_path,
                        layout(400, 32)
                            .with_debug_name("tb_file_path"));
    auto file_row = hstack(ctx, mk(col1.ent()),
                           layout(400, 32)
                               .with_gap(w1280(8))
                               .with_no_wrap()
                               .with_transparent_bg());
    if (button(ctx, mk(file_row.ent()),
               ComponentConfig{}
                   .with_size({w1280(196), h720(32)})
                   .with_label("Export")
                   .with_font("AtkinsonMock", FontSize::Small)
                   .with_custom_background(swatch_bg)
                   .with_custom_text_color(chrome_text)
                   .with_debug_name("tb_export"))) {
      Theme to_save = edited;
      for (int i = 0; i < kTokenCount; i++)
        to_save.color_ref(kTokens[i].usage) =
            resolved.color_ref(kTokens[i].usage);
      to_save.panel_corner_radius = radius01 * 24.f;
      to_save.focus_ring_thickness = ring01 * 6.f;
      const std::string name =
          std::filesystem::path(export_path).filename().string();
      file_status =
          theme_io::save(to_save, export_path)
              ? fmt::format("Exported {}", name)
              : fmt::format("Export failed: {}", name);
    }
    if (button(ctx, mk(file_row.ent()),
               ComponentConfig{}
                   .with_size({w1280(196), h720(32)})
                   .with_label("Import")
                   .with_font("AtkinsonMock", FontSize::Small)
                   .with_custom_background(swatch_bg)
                   .with_custom_text_color(chrome_text)
                   .with_debug_name("tb_import"))) {
      const std::string name =
          std::filesystem::path(export_path).filename().string();
      if (const auto text = files::read_string(export_path)) {
        Theme incoming = edited;
        const auto result = theme_io::read_into(*text, incoming);
        edited = incoming;
        radius01 = std::clamp(incoming.panel_corner_radius / 24.f, 0.f, 1.f);
        ring01 = std::clamp(incoming.focus_ring_thickness / 6.f, 0.f, 1.f);
        for (int i = 0; i < kTokenCount; i++)
          link[i].reset();
        file_status =
            result.errors == 0
                ? fmt::format("Imported {}", name)
                : fmt::format("Imported {} with {} bad lines", name,
                              result.errors);
      } else {
        file_status = fmt::format("Import failed: could not open {}", name);
      }
    }
    div(ctx, mk(col1.ent()),
        layout(400, 20)
            .with_label(file_status)
            .with_custom_text_color(chrome_text)
            .with_transparent_bg()
            .with_debug_name("tb_file_status"));

    // ---- live preview: a typical settings page, usages only ----
    auto preview_col = vstack(ctx, mk(main.ent()),
                              layout(400, 624)
                                  .with_gap(h720(8))
                                  .with_transparent_bg());
    div(ctx, mk(preview_col.ent()),
        layout(400, 20)
            .with_label("Live preview")
            .with_font_weight(colors::FontWeight::Bold)
            .with_custom_text_color(chrome_text)
            .with_transparent_bg());
    auto preview = vstack(ctx, mk(preview_col.ent()),
                          layout(400, 592)
                              .with_color_usage(Theme::Usage::Background)
                              .with_corner_radius(
                                  ctx.theme.panel_corner_radius)
                              // TODO: reevaluate — Padding::all(h720)
                              // resolves horizontal against the width.
                              .with_padding(Padding{.top = h720(16),
                                                   .left = w1280(16),
                                                   .bottom = h720(16),
                                                   .right = w1280(16)})
                              .with_gap(h720(10))
                              .with_debug_name("tb_preview"));
    auto preview_title = div(ctx, mk(preview.ent()),
                             layout(368, 32)
                                 .with_label("Workspace")
                                 .with_font_size(FontSize::Large)
                                 .with_font_weight(
                                     colors::FontWeight::Bold)
                                 .with_transparent_bg());
    auto preview_subtitle = div(ctx, mk(preview.ent()),
                                layout(368, 28)
                                    .with_label(
                                        "A page using the edited theme.")
                                    .with_text_color(
                                        Theme::Usage::FontMuted)
                                    .with_text_overflow(TextOverflow::Wrap)
                                    .with_transparent_bg());
    auto card = vstack(ctx, mk(preview.ent()),
                       layout(368, 240)
                           .with_color_usage(Theme::Usage::Surface)
                           .with_corner_radius(
                               ctx.theme.panel_corner_radius)
                           .with_padding(Padding{.top = h720(12),
                                                .left = w1280(12),
                                                .bottom = h720(12),
                                                .right = w1280(12)})
                           .with_gap(h720(8))
                           .with_debug_name("tb_preview_card"));
    auto preview_project = div(ctx, mk(card.ent()),
                                 layout(344, 24)
                                     .with_label("Project name")
                                     .with_transparent_bg());
    ui::imm::text_input(ctx, mk(card.ent()), preview_name,
                        layout(344, 40)
                            .with_placeholder("Enter a project name")
                            .with_debug_name("tb_preview_name"));
    auto card_buttons = hstack(ctx, mk(card.ent()),
                               layout(344, 40)
                                   .with_gap(w1280(8))
                                   .with_no_wrap()
                                   .with_transparent_bg());
    auto save_btn = button(ctx, mk(card_buttons.ent()),
                           layout(180, 40)
                               .with_label("Save changes")
                               .with_button_variant(ButtonVariant::Filled)
                               .with_debug_name("tb_preview_save"));
    button(ctx, mk(card_buttons.ent()),
           layout(120, 40)
               .with_label("Cancel")
               .with_button_variant(ButtonVariant::Outline)
               .with_debug_name("tb_preview_cancel"));
    auto toggle_el = toggle_switch(ctx, mk(card.ent()),
                                     preview_notifications,
                                     layout(344, 36)
                                         .with_label("Email notifications")
                                         .with_debug_name(
                                             "tb_preview_toggle"));
    button(ctx, mk(card.ent()),
           layout(200, 40)
               .with_label("Invite members")
               .with_disabled(true)
               .with_debug_name("tb_preview_disabled"));
    auto chips = hstack(ctx, mk(preview.ent()),
                        layout(368, 32)
                            .with_gap(w1280(8))
                            .with_no_wrap()
                            .with_transparent_bg());
    // TODO: reevaluate — ElementResult cannot be stored (reference member).
    // Chips are unrolled so each keeps its own handle.
    auto chip_error_el = div(ctx, mk(chips.ent()),
                             layout(117, 32)
                                 .with_label("Error")
                                 .with_color_usage(Theme::Usage::Error)
                                 .with_text_color(Theme::Usage::Font)
                                 .with_corner_radius(6.f)
                                 .with_debug_name("tb_chip_error"));
    auto chip_success_el = div(ctx, mk(chips.ent()),
                               layout(117, 32)
                                   .with_label("Success")
                                   .with_color_usage(Theme::Usage::Success)
                                   .with_text_color(Theme::Usage::Font)
                                   .with_corner_radius(6.f)
                                   .with_debug_name("tb_chip_success"));
    auto chip_warning_el = div(ctx, mk(chips.ent()),
                               layout(117, 32)
                                   .with_label("Warning")
                                   .with_color_usage(Theme::Usage::Warning)
                                   .with_text_color(Theme::Usage::Font)
                                   .with_corner_radius(6.f)
                                   .with_debug_name("tb_chip_warning"));
    auto volume_row = hstack(ctx, mk(preview.ent()),
                             layout(368, 36)
                                 .with_gap(w1280(8))
                                 .with_no_wrap()
                                 .with_transparent_bg());
    div(ctx, mk(volume_row.ent()),
        layout(80, 36)
            .with_label("Volume")
            .with_transparent_bg());
    auto volume_el = slider(ctx, mk(volume_row.ent()), preview_volume,
                              ComponentConfig{}
                                  .with_size({expand(), h720(36)})
                                  .with_debug_name("tb_preview_volume"));
    div(ctx, mk(volume_row.ent()),
        layout(64, 36)
            .with_label(fmt::format("{}%",
                                    static_cast<int>(std::round(
                                        preview_volume * 100.f))))
            .with_transparent_bg()
            .with_debug_name("tb_preview_volume_value"));
    auto focus_row = hstack(ctx, mk(preview.ent()),
                            layout(368, 40)
                                .with_gap(w1280(8))
                                .with_no_wrap()
                                .with_transparent_bg());
    auto focus_btn = button(ctx, mk(focus_row.ent()),
                            layout(180, 40)
                                .with_label("Focus me")
                                .with_button_variant(ButtonVariant::Outline)
                                .with_debug_name("tb_preview_focus"));
    div(ctx, mk(focus_row.ent()),
        layout(180, 40)
            .with_label("Tab here to see the focus ring.")
            .with_font_size(FontSize::Small)
            .with_text_color(Theme::Usage::FontMuted)
            .with_text_overflow(TextOverflow::Wrap)
            .with_transparent_bg());
    auto status_el = div(ctx, mk(preview.ent()),
                         layout(368, 28)
                             .with_label("Enter a project name first.")
                             .with_text_color(Theme::Usage::Error)
                             .with_transparent_bg());
    div(ctx, mk(preview.ent()),
        layout(368, 28)
            .with_font_size(FontSize::Small)
            .with_label(fmt::format(
                "Accent #{} · Focus #{} · Primary #{}",
                colors::to_hex_string(resolved.accent,
                                       resolved.accent.a < 255),
                colors::to_hex_string(resolved.focus,
                                       resolved.focus.a < 255),
                colors::to_hex_string(resolved.primary,
                                      resolved.primary.a < 255)))
            .with_text_color(Theme::Usage::FontMuted)
            .with_transparent_bg()
            .with_debug_name("tb_preview_tokens"));

    // TODO: reevaluate — divs have no click callback; test press vs rects.
    // Innermost first; inspecting never consumes the click.
    const auto inspect = [&](int idx) {
      inspected = idx;
      link_picker_open = false;
      link_picker_for = -1;
      for (int j = 0; j < kTokenCount; j++)
        open[j] = (j == idx);
    };
    const auto inspect_usage = [&](Theme::Usage usage) {
      for (int i = 0; i < kTokenCount; i++)
        if (kTokens[i].usage == usage)
          inspect(i);
    };
    if (ctx.mouse.just_pressed) {
      const Vector2Type pos = ctx.mouse.pos;
      const auto inside = [&](const ElementResult &el) {
        return el.ent().template has<UIComponent>() &&
               is_mouse_inside(
                   pos, el.ent().template get<UIComponent>().rect());
      };
      if (inside(save_btn))
        inspect_usage(Theme::Usage::Primary);
      else if (inside(focus_btn))
        inspect_usage(Theme::Usage::Focus);
      else if (inside(toggle_el))
        inspect_usage(Theme::Usage::Accent);
      else if (inside(volume_el))
        inspect_usage(Theme::Usage::Secondary);
      else if (inside(chip_error_el))
        inspect_usage(Theme::Usage::Error);
      else if (inside(chip_success_el))
        inspect_usage(Theme::Usage::Success);
      else if (inside(chip_warning_el))
        inspect_usage(Theme::Usage::Warning);
      else if (inside(status_el))
        inspect_usage(Theme::Usage::Error);
      else if (inside(preview_title))
        inspect_usage(Theme::Usage::Font);
      else if (inside(preview_subtitle))
        inspect_usage(Theme::Usage::FontMuted);
      else if (inside(preview_project))
        inspect_usage(Theme::Usage::Font);
      else if (inside(card))
        inspect_usage(Theme::Usage::Surface);
      else if (inside(preview))
        inspect_usage(Theme::Usage::Background);
    }
    // History: edits on consecutive frames are one gesture (a slider
    // drag, a typed hex), so one undo step reverts the whole gesture.
    if (!history_suppressed) {
      const ThemeSnapshot frame_end = theme_snapshot();
      if (!snapshots_equal(frame_start, frame_end)) {
        if (!history_gesture_open) {
          undo_stack.push_back(frame_start);
          if (undo_stack.size() > 100)
            undo_stack.erase(undo_stack.begin());
          redo_stack.clear();
          history_gesture_open = true;
        }
      } else {
        history_gesture_open = false;
      }
    }
  }
};

REGISTER_EXAMPLE_SCREEN(theme_builder, "System Demos",
                        "theme builder: token table driving a live preview",
                        ThemeBuilderLab)
