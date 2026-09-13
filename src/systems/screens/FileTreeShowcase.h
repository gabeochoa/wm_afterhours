#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <filesystem>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct FileTreeShowcase : ScreenSystem<UIContext<InputAction>> {
  struct FileEntry {
    std::string name;
    std::string path;
    bool is_directory = false;
    uintmax_t file_size = 0;
  };

  std::string current_root;
  std::vector<TreeNode<FileEntry>> cached_roots;
  bool needs_refresh = true;
  // Off by default: scanning the real cwd made this screen's baseline change
  // whenever anything appeared in the repo root, including our own build dirs.
  bool use_real_fs = false;
  bool seeded_expansion = false;

  static constexpr const char *SYNTHETIC_ROOT = "~/projects/harbour";

  FileTreeShowcase() { current_root = SYNTHETIC_ROOT; }

  static TreeNode<FileEntry> make_dir(const std::string &parent,
                                      const std::string &name,
                                      std::vector<TreeNode<FileEntry>> kids) {
    TreeNode<FileEntry> n;
    n.data.name = name;
    n.data.path = parent + "/" + name;
    n.data.is_directory = true;
    n.is_leaf = false;
    n.children = std::move(kids);
    return n;
  }

  static TreeNode<FileEntry> make_file(const std::string &parent,
                                       const std::string &name,
                                       uintmax_t size) {
    TreeNode<FileEntry> n;
    n.data.name = name;
    n.data.path = parent + "/" + name;
    n.data.is_directory = false;
    n.data.file_size = size;
    n.is_leaf = true;
    return n;
  }

  // A believable project, fixed so the baseline is stable.
  static std::vector<TreeNode<FileEntry>> synthetic_tree() {
    const std::string r = SYNTHETIC_ROOT;
    const std::string src = r + "/src";
    const std::string assets = r + "/assets";
    return {
        make_dir(r, "src",
                 {
                     make_dir(src, "engine",
                              {
                                  make_file(src + "/engine", "renderer.cpp",
                                            48213),
                                  make_file(src + "/engine", "renderer.h", 6122),
                                  make_file(src + "/engine", "scheduler.cpp",
                                            21504),
                              }),
                     make_dir(src, "ui",
                              {
                                  make_file(src + "/ui", "layout.cpp", 73940),
                                  make_file(src + "/ui", "theme.cpp", 12880),
                              }),
                     make_file(src, "main.cpp", 3271),
                 }),
        make_dir(r, "assets",
                 {
                     make_dir(assets, "fonts",
                              {
                                  make_file(assets + "/fonts", "Archivo.ttf",
                                            184320),
                              }),
                     make_file(assets, "atlas.png", 2411724),
                 }),
        make_dir(r, "tests",
                 {
                     make_file(r + "/tests", "layout_test.cpp", 15890),
                     make_file(r + "/tests", "theme_test.cpp", 8044),
                 }),
        make_file(r, "README.md", 4180),
        make_file(r, "makefile", 2965),
    };
  }

  std::vector<TreeNode<FileEntry>> scan_directory(const std::string &dir_path) {
    std::vector<TreeNode<FileEntry>> nodes;
    std::error_code ec;

    for (const auto &entry :
         std::filesystem::directory_iterator(dir_path, ec)) {
      if (ec)
        break;

      std::string name = entry.path().filename().string();
      // Skip hidden files/dirs
      if (!name.empty() && name[0] == '.')
        continue;

      FileEntry fe;
      fe.name = name;
      fe.path = entry.path().string();
      fe.is_directory = entry.is_directory(ec);

      if (!fe.is_directory) {
        fe.file_size = entry.file_size(ec);
        if (ec)
          fe.file_size = 0;
      }

      TreeNode<FileEntry> node;
      node.data = fe;
      node.is_leaf = !fe.is_directory;
      nodes.push_back(std::move(node));
    }

    // Sort: directories first, then alphabetically
    std::sort(nodes.begin(), nodes.end(),
              [](const TreeNode<FileEntry> &a, const TreeNode<FileEntry> &b) {
                if (a.data.is_directory != b.data.is_directory)
                  return a.data.is_directory > b.data.is_directory;
                return a.data.name < b.data.name;
              });

    return nodes;
  }

  void lazy_load_children(TreeNode<FileEntry> &node, HasTreeViewState &state) {
    // The synthetic tree ships its children already, and scanning a made-up
    // path would only empty them.
    if (!use_real_fs)
      return;
    if (!node.data.is_directory)
      return;
    if (!state.is_expanded(node.data.path))
      return;
    if (node.children.empty())
      node.children = scan_directory(node.data.path);
    // Recursively load expanded children
    for (auto &child : node.children) {
      lazy_load_children(child, state);
    }
  }

  static std::string format_size(uintmax_t bytes) {
    if (bytes < 1024)
      return fmt::format("{} B", bytes);
    if (bytes < 1024 * 1024)
      return fmt::format("{:.1f} KB", static_cast<double>(bytes) / 1024.0);
    if (bytes < 1024 * 1024 * 1024)
      return fmt::format("{:.1f} MB",
                         static_cast<double>(bytes) / (1024.0 * 1024.0));
    return fmt::format("{:.1f} GB",
                       static_cast<double>(bytes) / (1024.0 * 1024.0 * 1024.0));
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
    context.theme.focus_ring_offset = s;
    context.theme.focus_ring_thickness = 1.5f * s;
    const float x = (context.screen_width - 1160 * s) / 2;
    const float y = (context.screen_height - 664 * s) / 2;
    div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_absolute_position(0, 0).with_custom_background(theme.background)
        .with_corner_radius(0).with_debug_name("bg"));
    auto root = div(context, mk(entity, 1), ComponentConfig{}
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
      if (id != 10 && id != 12) return result;
      result.ent().get<HasLabel>().text_x_offset = 10 * s;
      result.ent().addComponentIfMissing<HasOnDraw>().fg = [id, s, theme](RectangleType r) {
        const float px = r.x + 12 * s;
        const float py = r.y + r.height / 2;
        if (id == 10) {
          raylib::DrawLineEx({px, py}, {px + 7 * s, py - 7 * s}, 1.5f * s, theme.font);
          raylib::DrawLineEx({px + 7 * s, py - 7 * s}, {px + 14 * s, py}, 1.5f * s, theme.font);
          raylib::DrawRectangleLinesEx({px + 3 * s, py, 8 * s, 8 * s}, s, theme.font);
          return;
        }
        raylib::DrawRing({px + 7 * s, py}, 5 * s, 6.5f * s, 40, 320, 24, theme.font);
        raylib::DrawLineEx({px + 12 * s, py - 5 * s}, {px + 13 * s, py + 1 * s}, 2 * s, theme.font);
      };
      return result;
    };
    if (control(10, "Home", 24, 104, "home_btn")) {
      current_root = use_real_fs ? std::filesystem::current_path().string() : SYNTHETIC_ROOT;
      needs_refresh = true;
      seeded_expansion = false;
    }
    if (control(11, "Up", 136, 76, "up_btn", !use_real_fs || current_root == "/")) {
      const auto parent = std::filesystem::path(current_root).parent_path().string();
      if (!parent.empty()) current_root = parent;
      needs_refresh = true;
    }
    if (control(12, "Refresh", 248, 128, "refresh_btn")) needs_refresh = true;
    if (control(13, use_real_fs ? "Real files" : "Sample data", 958, 178, "source_btn")) {
      use_real_fs = !use_real_fs;
      current_root = use_real_fs ? std::filesystem::current_path().string() : SYNTHETIC_ROOT;
      needs_refresh = true;
      seeded_expansion = false;
    }
    label(41, "Switch data source at right", 656, 68, 290, 30, 17, true);
    label(2, current_root, 24, 116, 1112, 30, 21, true)
        .ent().addComponentIfMissing<UIComponentDebug>("path_label").set("path_label");
    label(42, "Name", 24, 156, 400, 26, 18, true);
    label(43, "Size", 596, 156, 104, 26, 18, true);
    label(44, "Selection details", 760, 156, 348, 26, 18, true);
    if (needs_refresh) {
      cached_roots = use_real_fs ? scan_directory(current_root) : synthetic_tree();
      needs_refresh = false;
    }
    auto tree_pair = mk(root.ent(), 3);
    auto [tree_entity, tree_parent] = deref(tree_pair);
    auto &state = tree_entity.addComponentIfMissing<HasTreeViewState>();
    if (!seeded_expansion && !use_real_fs) {
      seeded_expansion = true;
      state.selected_node_id.clear();
      state.expanded_nodes.clear();
      const std::function<void(const std::vector<TreeNode<FileEntry>> &)> open =
          [&](const std::vector<TreeNode<FileEntry>> &nodes) {
        for (const auto &node : nodes) {
          if (!node.data.is_directory) continue;
          state.expanded_nodes.insert(node.data.path);
          open(node.children);
        }
      };
      open(cached_roots);
    }
    for (auto &node : cached_roots) lazy_load_children(node, state);
    std::vector<std::pair<const FileEntry *, int>> visible;
    size_t folders = 0;
    size_t files = 0;
    const FileEntry *selected = nullptr;
    const std::function<void(const std::vector<TreeNode<FileEntry>> &, int, bool)> walk =
        [&](const std::vector<TreeNode<FileEntry>> &nodes, int depth, bool shown) {
      for (const auto &node : nodes) {
        node.data.is_directory ? ++folders : ++files;
        if (node.data.path == state.selected_node_id) selected = &node.data;
        if (shown) visible.emplace_back(&node.data, depth);
        walk(node.children, depth + 1, shown && state.is_expanded(node.data.path));
      }
    };
    walk(cached_roots, 0, true);
    TreeViewConfig<FileEntry> config;
    config.indent_width = 20 * s;
    config.row_height = 23 * s;
    config.get_label = [](const FileEntry &entry) { return entry.name; };
    config.get_id = [](const FileEntry &entry) { return entry.path; };
    config.is_expandable = [](const FileEntry &entry) { return entry.is_directory; };
    auto result = tree_view(context, tree_pair, cached_roots, config,
        box(24, 186, 676, 430).with_font("AtkinsonMock", pixels(19 * s))
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
          caption.font_name = entry->is_directory ? "AtkinsonMockBold" : "AtkinsonMock";
          auto &text_cmp = text.asE().get<UIComponent>();
          text_cmp.desired_margin[Axis::left] = pixels(46 * s);
          text_cmp.desired_margin[Axis::right] = pixels(108 * s);
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
                raylib::DrawRectangleLinesEx({r.x + 25 * s, middle - 8 * s, 12 * s, 16 * s}, 1 * s, theme.font_muted);
              }));
          if (entry->is_directory) continue;
          div(context, mk(row, 301), box(552 - 20 * static_cast<float>(depth), 0, 114, 23)
              .with_label(format_size(entry->file_size)).with_font("AtkinsonMock", pixels(17 * s))
              .with_alignment(TextAlignment::Right).with_custom_text_color(theme.font_muted)
              .with_background(Theme::Usage::None).with_ignore_pointer_events());
        }
      }
    }
    div(context, mk(root.ent(), 50), box(736, 186, 400, 430)
        .with_custom_background({21, 29, 43, 255}).with_corner_radius(8 * s));
    label(51, selected ? selected->name : "No selection", 760, 210, 352, 42, 25);
    label(52, selected ? (selected->is_directory ? "Folder" : "File") : "Select a file to view details",
          760, 264, 352, 32, 20, true);
    label(53, selected ? (selected->is_directory ? "Expand or collapse with Enter" : format_size(selected->file_size))
                       : "Click a row or use Tab and Enter.", 760, 306, 352, 32, 18, true);
    label(54, selected ? selected->path : "Sample data never changes your files.", 760, 356, 352, 88, 18, true)
        .ent().get<HasLabel>().text_overflow = TextOverflow::Wrap;
    label(55, use_real_fs ? "Read-only local directory" : "harbour / sample project", 760, 560, 352, 30, 17, true);
    label(4, fmt::format("{} folders / {} files{}", folders, files, use_real_fs ? " loaded" : " in sample"),
          24, 626, 676, 26, 18, true).ent().addComponentIfMissing<UIComponentDebug>("status_bar").set("status_bar");
  }

};

REGISTER_EXAMPLE_SCREEN(file_tree, "Component Galleries",
                        "File tree browser using tree_view component",
                        FileTreeShowcase)
