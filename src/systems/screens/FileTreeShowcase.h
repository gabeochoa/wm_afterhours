#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include "../../theme_presets.h"
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

  FileTreeShowcase() {
    current_root = std::filesystem::current_path().string();
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

  void lazy_load_children(TreeNode<FileEntry> &node,
                          HasTreeViewState &state) {
    if (!node.data.is_directory)
      return;
    if (!state.is_expanded(node.data.path))
      return;
    if (!node.children.empty())
      return;

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
                       static_cast<double>(bytes) /
                           (1024.0 * 1024.0 * 1024.0));
  }

  void for_each_with(afterhours::Entity &entity,
                     UIContext<InputAction> &context, float) override {
    Theme theme;
    theme.background = afterhours::Color{30, 30, 30, 255};
    theme.surface = afterhours::Color{45, 45, 45, 255};
    theme.primary = afterhours::Color{60, 120, 200, 255};
    theme.secondary = afterhours::Color{55, 55, 55, 255};
    theme.accent = afterhours::Color{80, 160, 240, 255};
    theme.font = afterhours::Color{210, 210, 210, 255};
    theme.darkfont = afterhours::Color{40, 40, 40, 255};
    theme.font_muted = afterhours::Color{140, 140, 140, 255};
    context.theme = theme;

    int screen_width = Settings::get().get_screen_width();
    int screen_height = Settings::get().get_screen_height();

    // Main background
    div(context, mk(entity, 0),
        ComponentConfig{}
            .with_size(
                ComponentSize{pixels(screen_width), pixels(screen_height)})
            .with_custom_background(theme.background)
            .with_debug_name("bg"));

    // Main container
    auto root =
        div(context, mk(entity, 1),
            ComponentConfig{}
                .with_size(ComponentSize{screen_pct(0.8f), screen_pct(0.85f)})
                .with_self_align(SelfAlign::Center)
                .with_custom_background(theme.surface)
                .with_border(theme.font_muted, 1.0f)
                .with_rounded_corners(RoundedCorners().all_round())
                .with_roundness(0.02f)
                .with_padding(Spacing::md)
                .with_flex_direction(FlexDirection::Column)
                .with_debug_name("main_container"));

    // Title bar
    div(context, mk(root.ent(), 0),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(36)})
            .with_label("File Tree")
            .with_custom_background(afterhours::Color{50, 50, 50, 255})
            .with_custom_text_color(theme.font)
            .with_font(UIComponent::DEFAULT_FONT, h720(22.0f))
            .with_rounded_corners(RoundedCorners().bottom_sharp())
            .with_roundness(0.1f)
            .with_debug_name("title"));

    // Toolbar
    auto toolbar = div(
        context, mk(root.ent(), 1),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(34)})
            .with_flex_direction(FlexDirection::Row)
            .with_align_items(AlignItems::Center)
            .with_no_wrap()
            .with_padding(Padding::horizontal(pixels(4)))
            .with_custom_background(afterhours::Color{40, 40, 40, 255})
            .with_debug_name("toolbar"));

    // Home button
    if (button(context, mk(toolbar.ent(), 0),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(60), pixels(26)})
                   .with_label("Home")
                   .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
                   .with_background(Theme::Usage::Secondary)
                   .with_margin(Margin{.right = pixels(4)})
                   .with_debug_name("home_btn"))) {
      current_root = std::filesystem::current_path().string();
      cached_roots.clear();
      needs_refresh = true;
    }

    // Up button
    if (button(context, mk(toolbar.ent(), 1),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(40), pixels(26)})
                   .with_label("Up")
                   .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
                   .with_background(Theme::Usage::Secondary)
                   .with_margin(Margin{.right = pixels(4)})
                   .with_debug_name("up_btn"))) {
      auto parent_path =
          std::filesystem::path(current_root).parent_path().string();
      if (!parent_path.empty() && parent_path != current_root) {
        current_root = parent_path;
        cached_roots.clear();
        needs_refresh = true;
      }
    }

    // Refresh button
    if (button(context, mk(toolbar.ent(), 2),
               ComponentConfig{}
                   .with_size(ComponentSize{pixels(70), pixels(26)})
                   .with_label("Refresh")
                   .with_font(UIComponent::DEFAULT_FONT, h720(16.0f))
                   .with_background(Theme::Usage::Secondary)
                   .with_debug_name("refresh_btn"))) {
      cached_roots.clear();
      needs_refresh = true;
    }

    // Current path label
    div(context, mk(root.ent(), 2),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(24)})
            .with_label(current_root)
            .with_font(UIComponent::DEFAULT_FONT, h720(14.0f))
            .with_custom_text_color(theme.font_muted)
            .with_alignment(TextAlignment::Left)
            .with_padding(Padding::horizontal(pixels(8)))
            .with_custom_background(afterhours::Color{35, 35, 35, 255})
            .with_debug_name("path_label"));

    // Load directory contents
    if (needs_refresh) {
      cached_roots = scan_directory(current_root);
      needs_refresh = false;
    }

    // Lazy-load expanded directories
    // Get state from the tree_view entity if it exists
    auto tree_pair = mk(root.ent(), 3);
    auto [tree_entity, tree_parent] = deref(tree_pair);
    if (tree_entity.has<HasTreeViewState>()) {
      auto &tv_state = tree_entity.get<HasTreeViewState>();
      for (auto &node : cached_roots) {
        lazy_load_children(node, tv_state);
      }
    }

    // Tree view config
    TreeViewConfig<FileEntry> view_config;
    view_config.indent_width = 20.0f;
    view_config.row_height = 24.0f;
    view_config.get_label = [](const FileEntry &entry) -> std::string {
      if (entry.is_directory)
        return entry.name + "/";
      return entry.name;
    };
    view_config.get_id = [](const FileEntry &entry) -> std::string {
      return entry.path;
    };
    view_config.is_expandable = [](const FileEntry &entry) -> bool {
      return entry.is_directory;
    };

    // Tree view
    auto tree_result = tree_view(
        context, tree_pair, cached_roots, view_config,
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), expand()})
            .with_custom_background(theme.background)
            .with_debug_name("file_tree"));

    // Invalidate children cache when a directory is expanded/collapsed
    if (tree_result) {
      auto &tv_state = tree_entity.get<HasTreeViewState>();
      for (auto &node : cached_roots) {
        lazy_load_children(node, tv_state);
      }
    }

    // Status bar
    std::string status_text = "No selection";
    if (tree_entity.has<HasTreeViewState>()) {
      auto &tv_state = tree_entity.get<HasTreeViewState>();
      if (!tv_state.selected_node_id.empty()) {
        std::error_code ec;
        auto path = std::filesystem::path(tv_state.selected_node_id);
        if (std::filesystem::is_regular_file(path, ec)) {
          auto sz = std::filesystem::file_size(path, ec);
          status_text =
              tv_state.selected_node_id + "  (" + format_size(sz) + ")";
        } else {
          status_text = tv_state.selected_node_id;
        }
      }
    }

    div(context, mk(root.ent(), 4),
        ComponentConfig{}
            .with_size(ComponentSize{percent(1.0f), pixels(24)})
            .with_label(status_text)
            .with_font(UIComponent::DEFAULT_FONT, h720(13.0f))
            .with_custom_text_color(theme.font_muted)
            .with_alignment(TextAlignment::Left)
            .with_padding(Padding::horizontal(pixels(8)))
            .with_custom_background(afterhours::Color{40, 40, 40, 255})
            .with_rounded_corners(RoundedCorners().top_sharp())
            .with_roundness(0.1f)
            .with_debug_name("status_bar"));
  }
};

REGISTER_EXAMPLE_SCREEN(file_tree, "Component Galleries",
                        "File tree browser using tree_view component",
                        FileTreeShowcase)
