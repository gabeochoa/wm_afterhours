#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../plugins/file_tree/file_tree_preview.h"
#include "../../plugins/file_tree/file_tree_source.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct FileTreeShowcase : ScreenSystem<UIContext<InputAction>> {
  // Data model lives in the WM-local file_tree plugin
  // (src/plugins/file_tree/); presentation stays here.
  using FileEntry = wm::plugins::file_tree::FileEntry;
  using Node = wm::plugins::file_tree::Node;

  std::string current_root;
  std::vector<Node> cached_roots;
  bool needs_refresh = true;
  // Off by default: scanning the real cwd made this screen's baseline change
  // whenever anything appeared in the repo root, including our own build dirs.
  bool use_real_fs = false;
  bool seeded_expansion = false;

  std::string preview_path;
  std::string preview_status;
  std::vector<std::string> preview_lines;

  static std::string project_root() {
    return wm::plugins::file_tree::project_root();
  }

  FileTreeShowcase() { current_root = project_root(); }

  static Node make_dir(const std::string &parent,
                       const std::string &name,
                       std::vector<Node> kids) {
    return wm::plugins::file_tree::make_dir(parent, name, std::move(kids));
  }

  static Node make_file(const std::string &parent,
                        const std::string &name,
                        uintmax_t size) {
    return wm::plugins::file_tree::make_file(parent, name, size);
  }

  static std::vector<Node> synthetic_tree() {
    return wm::plugins::file_tree::synthetic_tree();
  }

  void read_preview(const FileEntry *selected) {
    const auto path = selected && !selected->is_directory ? selected->path : "";
    if (path == preview_path) return;
    auto preview = wm::plugins::file_tree::preview::load_for_selection(selected);
    preview_path = preview.path;
    preview_status = preview.status;
    preview_lines = preview.lines;
  }

  std::vector<Node> scan_directory(const std::string &dir_path) {
    return wm::plugins::file_tree::scan_directory(dir_path);
  }

  void lazy_load_children(Node &node, HasTreeViewState &state) {
    wm::plugins::file_tree::lazy_load_children(node, state.expanded_nodes,
                                               use_real_fs);
  }

  static std::string format_size(uintmax_t bytes) {
    return wm::plugins::file_tree::format_size(bytes);
  }

  static std::string relative_label(const std::string &path) {
    return wm::plugins::file_tree::relative_label(path);
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.background = {13, 19, 29, 255};
    theme.surface = {27, 36, 51, 255};
    theme.primary = {43, 83, 130, 255};
    theme.secondary = {43, 55, 74, 255};
    theme.accent = {102, 172, 224, 255};
    theme.font = {229, 237, 247, 255};
    theme.font_muted = {168, 185, 207, 255};
    context.theme = theme;
    context.scaling_mode = ScalingMode::Proportional;
    UIStylingDefaults::get().set_grid_snapping(false);
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    context.theme.focus_ring_offset = 2.5f * s;
    context.theme.focus_ring_thickness = 1.5f * s;
    const float x = (context.screen_width - 1160 * s) / 2;
    const float y = (context.screen_height - 664 * s) / 2;
    div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_absolute_position(0, 0).with_custom_background(theme.background)
        .with_corner_radius(0).with_debug_name("bg"));
    auto root = div(context, mk(entity), ComponentConfig{}
        .with_size({pixels(1160 * s), pixels(664 * s)}).with_absolute_position(x, y)
        .with_custom_background(theme.surface).with_border({78, 97, 121, 255}, 1)
        .with_corner_radius(12 * s).with_debug_name("main_container"));
    const auto box = [s](float px, float py, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w * s), pixels(h * s)})
          .with_absolute_position(px * s, py * s).with_corner_radius(0);
    };
    const auto label = [&](int id, const std::string &text, float px, float py,
                           float w, float h, float size, bool muted = false) {
      return div(context, mk(root.ent(), id), box(px, py, w, h)
          .with_label(text).with_font("AtkinsonMock", pixels(size * s))
          .with_custom_text_color(muted ? theme.font_muted : theme.font)
          .with_background(Theme::Usage::None).with_text_overflow(TextOverflow::Ellipsis)
          .with_alignment(TextAlignment::Left).with_ignore_pointer_events());
    };
    label(0, "File Tree", 24, 12, 580, 42, 32);
    label(40, "Project browser / read-only", 810, 18, 326, 30, 18, true);
    const auto control = [&](int id, const std::string &text, float px, float w,
                             const std::string &name, bool disabled = false) {
      auto result = button(context, mk(root.ent(), id), box(px, 64, w, 40)
          .with_label(text).with_font("AtkinsonMock", pixels(20 * s))
          .with_background(Theme::Usage::Secondary).with_custom_text_color(theme.font)
          .with_corner_radius(6 * s).with_disabled(disabled).with_debug_name(name));
      if (id != 10 && id != 12 && id != 13) return result;
      result.ent().get<HasLabel>().text_x_offset = 5 * s; // halved: centered labels now shift by the full offset (#471)
      result.ent().addComponentIfMissing<HasOnDraw>().fg = [id, s, theme](RectangleType r) {
        const float py = r.y + r.height / 2;
        if (id == 10) {
          const float px = r.x + 12 * s;
          raylib::DrawLineEx({px, py}, {px + 7 * s, py - 7 * s}, 1.5f * s, theme.font);
          raylib::DrawLineEx({px + 7 * s, py - 7 * s}, {px + 14 * s, py}, 1.5f * s, theme.font);
          raylib::DrawRectangleLinesEx({px + 3 * s, py, 8 * s, 8 * s}, s, theme.font);
          return;
        }
        if (id == 12) {
          const float px = r.x + 12 * s;
          raylib::DrawRing({px + 7 * s, py}, 5 * s, 6.5f * s, 40, 320, 24, theme.font);
          raylib::DrawLineEx({px + 12 * s, py - 5 * s}, {px + 13 * s, py + 1 * s}, 2 * s, theme.font);
          return;
        }
        const float px = r.x + r.width - 14 * s;
        raylib::DrawLineEx({px, py - 5 * s}, {px + 5 * s, py}, 1.5f * s, theme.font);
        raylib::DrawLineEx({px + 5 * s, py}, {px, py + 5 * s}, 1.5f * s, theme.font);
      };
      return result;
    };
    if (control(10, "Home", 24, 104, "home_btn")) {
      current_root = use_real_fs ? std::filesystem::current_path().string() : project_root();
      needs_refresh = true;
      seeded_expansion = false;
    }
    if (control(11, "Up", 136, 76, "up_btn", !use_real_fs || current_root == "/")) {
      const auto parent = std::filesystem::path(current_root).parent_path().string();
      if (!parent.empty()) current_root = parent;
      needs_refresh = true;
    }
    if (control(12, "Refresh", 220, 128, "refresh_btn")) needs_refresh = true;
    if (control(13, use_real_fs ? "All files" : "Project sources", 958, 178, "source_btn")) {
      use_real_fs = !use_real_fs;
      current_root = use_real_fs ? std::filesystem::current_path().string() : project_root();
      needs_refresh = true;
      seeded_expansion = false;
    }
    label(2, relative_label(current_root), 24, 116, 1112, 30, 21, true)
        .ent().addComponentIfMissing<UIComponentDebug>("path_label").set("path_label");
    label(42, "Name", 24, 156, 400, 26, 18, true);
    label(43, "Size", 410, 156, 104, 26, 18, true).ent().get<HasLabel>().alignment = TextAlignment::Right;
    label(44, "Source preview", 548, 156, 560, 26, 18, true);
    if (needs_refresh) {
      cached_roots = use_real_fs ? scan_directory(current_root) : synthetic_tree();
      needs_refresh = false;
      preview_path.clear();
    }
    auto tree_pair = mk(root.ent());
    auto [tree_entity, tree_parent] = deref(tree_pair);
    auto &state = tree_entity.addComponentIfMissing<HasTreeViewState>();
    if (!seeded_expansion && !use_real_fs) {
      seeded_expansion = true;
      state.selected_node_id.clear();
      state.expanded_nodes.clear();
      wm::plugins::file_tree::expand_all(cached_roots, state.expanded_nodes);
    }
    for (auto &node : cached_roots) lazy_load_children(node, state);
    const auto counts = wm::plugins::file_tree::count_nodes(cached_roots);
    const size_t folders = counts.folders;
    const size_t files = counts.files;
    const FileEntry *selected = wm::plugins::file_tree::find_by_path(
        cached_roots, state.selected_node_id);
    const auto plugin_visible = wm::plugins::file_tree::visible_rows(
        cached_roots, state.expanded_nodes);
    std::vector<std::pair<const FileEntry *, int>> visible;
    visible.reserve(plugin_visible.size());
    for (const auto &row : plugin_visible)
      visible.emplace_back(row.entry, row.depth);
    TreeViewConfig<FileEntry> config;
    config.indent_width = 20 * s;
    config.row_height = 23 * s;
    config.get_label = [](const FileEntry &entry) { return entry.name; };
    config.get_id = [](const FileEntry &entry) { return entry.path; };
    config.is_expandable = [](const FileEntry &entry) {
      return wm::plugins::file_tree::is_expandable(entry);
    };
    auto result = tree_view(context, tree_pair, cached_roots, config,
        box(24, 186, 500, 430).with_font("AtkinsonMock", pixels(17 * s))
          .with_custom_text_color(theme.font).with_custom_background(theme.surface)
          .with_debug_name("file_tree"));
    if (!result.cmp().children.empty()) {
      auto scroll = UICollectionHolder::getEntityForID(result.cmp().children.front());
      if (scroll.valid()) {
        const auto rows = scroll.asE().get<UIComponent>().children;
        const auto count = std::min(rows.size(), visible.size());
        for (size_t i = 0; i < count; ++i) {
          auto item = UICollectionHolder::getEntityForID(rows[i]);
          if (!item.valid()) continue;
          auto &row = item.asE();
          auto &cmp = row.get<UIComponent>();
          const auto [entry, depth] = visible[i];
          row.addComponentIfMissing<UIComponentDebug>("file_row_" + entry->name).set("file_row_" + entry->name);
          if (depth == 0 && i > 0) cmp.desired_margin[Axis::top] = pixels(3 * s);
          if (cmp.children.empty()) continue;
          auto text = UICollectionHolder::getEntityForID(cmp.children.front());
          if (!text.valid()) continue;
          auto &caption = text.asE().get<HasLabel>();
          caption.label = entry->name;
          caption.text_overflow = TextOverflow::Ellipsis;
          auto &text_cmp = text.asE().get<UIComponent>();
          text_cmp.desired_margin[Axis::left] = pixels(46 * s);
          text_cmp.desired_margin[Axis::right] = pixels(88 * s);
          const bool expanded = state.is_expanded(entry->path);
          div(context, mk(row, 300), box(0, 0, 42, 23)
              .with_background(Theme::Usage::None).with_ignore_pointer_events()
              .with_on_draw_fg([s, directory = entry->is_directory, expanded, depth, theme](RectangleType r) {
                const float middle = r.y + r.height / 2;
                for (int level = 1; level <= depth; ++level)
                  raylib::DrawLineEx({r.x - 20 * s * static_cast<float>(level) + 8 * s, r.y},
                                    {r.x - 20 * s * static_cast<float>(level) + 8 * s, r.y + r.height}, 1, {56, 73, 94, 255});
                if (directory) {
                  if (expanded) {
                    raylib::DrawLineEx({r.x + 2 * s, middle - 2 * s}, {r.x + 7 * s, middle + 3 * s}, 1.5f * s, theme.font_muted);
                    raylib::DrawLineEx({r.x + 7 * s, middle + 3 * s}, {r.x + 12 * s, middle - 2 * s}, 1.5f * s, theme.font_muted);
                  } else {
                    raylib::DrawLineEx({r.x + 4 * s, middle - 5 * s}, {r.x + 9 * s, middle}, 1.5f * s, theme.font_muted);
                    raylib::DrawLineEx({r.x + 9 * s, middle}, {r.x + 4 * s, middle + 5 * s}, 1.5f * s, theme.font_muted);
                  }
                  raylib::DrawRectangleRec({r.x + 22 * s, middle - 5 * s, 17 * s, 12 * s}, {196, 161, 98, 255});
                  raylib::DrawRectangleRec({r.x + 22 * s, middle - 8 * s, 8 * s, 4 * s}, {196, 161, 98, 255});
                  return;
                }
                raylib::DrawRectangleRoundedLinesEx({r.x + 25 * s, middle - 8 * s, 12 * s, 16 * s}, .25f, 8, 1 * s, theme.font_muted);
              }));
          if (entry->is_directory) continue;
          div(context, mk(row, 301), box(388 - 20 * static_cast<float>(depth), 0, 102, 23)
              .with_label(format_size(entry->file_size)).with_font("AtkinsonMock", pixels(17 * s))
              .with_alignment(TextAlignment::Right).with_custom_text_color(theme.font_muted)
              .with_background(Theme::Usage::None).with_ignore_pointer_events());
        }
      }
    }
    const bool changed_preview = preview_path != (selected && !selected->is_directory ? selected->path : "");
    read_preview(selected);
    div(context, mk(root.ent(), 50), box(540, 186, 596, 430)
        .with_custom_background({17, 24, 36, 255}).with_corner_radius(8 * s));
    label(51, selected ? selected->name : "No selection", 552, 194, 572, 34, 23);
    label(52, selected ? (selected->is_directory ? "Expand or collapse with Enter" : format_size(selected->file_size))
                       : "No file selected", 552, 230, 572, 26, 17, true);
    label(54, selected ? relative_label(selected->path) : relative_label(current_root), 552, 260, 572, 28, 16, true)
        .ent().addComponentIfMissing<UIComponentDebug>("file_preview_path").set("file_preview_path");
    auto preview = virtual_list(context, mk(root.ent(), 56), preview_lines.size(), 24 * s,
        [&](size_t index, afterhours::Entity &row) {
          const auto &line = preview_lines[index];
          const float width = std::max(548 * s, (76 + static_cast<float>(line.size()) * 18) * s);
          row.get<UIComponent>().set_desired_width(pixels(width));
          div(context, mk(row, 0), ComponentConfig{}
              .with_size({pixels(width), pixels(24 * s)})
              .with_label(fmt::format("{:4}  {}", index + 1, line))
              .with_font("AtkinsonMock", pixels(18 * s)).with_text_inset(0)
              .with_custom_text_color(theme.font).with_background(Theme::Usage::None)
              .with_corner_radius(0).with_ignore_pointer_events()
              .with_debug_name("file_preview_line_" + std::to_string(index + 1)));
        }, box(552, 298, 572, 274).with_overflow(Overflow::Scroll, Axis::X)
              .with_custom_background({17, 24, 36, 255}).with_debug_name("file_preview"));
    auto &preview_scroll = preview.ent().get<HasScrollView>();
    preview_scroll.scroll_speed = 36 * s;
    if (changed_preview) {
      preview_scroll.scroll_offset = {};
      preview_scroll.scroll_target = {};
      preview_scroll.last_eased_offset = {};
    }
    label(55, preview_status.empty() ? "Select a source file on the left" : preview_status +
          fmt::format(" / {} lines / scroll to read", preview_lines.size()),
          552, 580, 572, 28, 16, true);
    label(4, fmt::format("{} folders / {} files{}", folders, files, use_real_fs ? " loaded" : " in project"),
          24, 626, 676, 26, 18, true).ent().addComponentIfMissing<UIComponentDebug>("status_bar").set("status_bar");
  }

};

REGISTER_EXAMPLE_SCREEN(file_tree, "Component Galleries",
                        "File tree browser using tree_view component",
                        FileTreeShowcase)
