#pragma once

// WM-local plugin: filesystem file-tree data source (Batch 3 Lane I).
//
// Non-UI data/model layer extracted from
// src/systems/screens/FileTreeShowcase.h (behavior-preserving).
// Everything here is pure model code: directory scan / synthetic tree,
// tree nodes, expand/collapse state helpers, file sizes, selection
// lookup, kind classification, and size/path formatting. No UIContext,
// no widgets, no ECS, no rendering, no afterhours/vendor edits.
// FileTreeShowcase is the sole consumer and keeps every visual,
// behavior, label, and debug name exactly as before.
//
// The node type is afterhours::ui::imm::TreeNode<FileEntry> (the same
// node tree_view consumes), so a showcase can pass roots straight to
// afterhours::ui::imm::tree_view without an adapter copy. Callers must
// include <afterhours/ah.h> (or src/external.h) before this header,
// matching the AH prelude convention of the other local plugins
// (rolling_number, theme_pack).
//
// Public API lives in namespace wm::plugins::file_tree. Header-only;
// there is no .cpp to link. See README.md in this directory for the
// plugin card, consumer, and promotion gate.

#include <afterhours/src/plugins/ui/tree_view.h>

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <fmt/format.h>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace wm::plugins::file_tree {

// ------------------------------------------------------------------
// Data model
// ------------------------------------------------------------------

struct FileEntry {
  std::string name;
  std::string path;
  bool is_directory = false;
  uintmax_t file_size = 0;
};

// Node type: the exact container afterhours tree_view consumes.
// Alias (not a copy) so roots pass through with no adapter.
using Node = afterhours::ui::imm::TreeNode<FileEntry>;

using ExpandedSet = std::unordered_set<std::string>;

// Separable icon/kind classification (presentation stays in showcase).
enum class EntryKind { Directory, File };

inline EntryKind classify(const FileEntry &entry) {
  return entry.is_directory ? EntryKind::Directory : EntryKind::File;
}

inline bool is_directory(const FileEntry &entry) {
  return entry.is_directory;
}

inline bool is_file(const FileEntry &entry) {
  return !entry.is_directory;
}

inline bool is_expandable(const FileEntry &entry) {
  return entry.is_directory;
}

inline bool should_show_size(const FileEntry &entry) {
  return !entry.is_directory;
}

// "archive.tar.gz" -> ".gz"; "makefile" -> "".
inline std::string extension_of(const FileEntry &entry) {
  const auto pos = entry.name.rfind('.');
  if (pos == std::string::npos || pos == 0 || pos + 1 == entry.name.size())
    return "";
  return entry.name.substr(pos);
}

// ------------------------------------------------------------------
// Root helpers
// ------------------------------------------------------------------

inline std::string project_root() {
  return std::filesystem::current_path().string();
}

// ------------------------------------------------------------------
// Node builders (exact path-join semantics of the showcase)
// ------------------------------------------------------------------

inline Node make_dir(const std::string &parent, const std::string &name,
                     std::vector<Node> kids) {
  Node n;
  n.data.name = name;
  n.data.path = parent + "/" + name;
  n.data.is_directory = true;
  n.data.file_size = 0;
  n.is_leaf = false;
  n.children = std::move(kids);
  return n;
}

inline Node make_file(const std::string &parent, const std::string &name,
                      uintmax_t size) {
  Node n;
  n.data.name = name;
  n.data.path = parent + "/" + name;
  n.data.is_directory = false;
  n.data.file_size = size;
  n.is_leaf = true;
  return n;
}

// ------------------------------------------------------------------
// Curated tree (the "Project sources" view)
// ------------------------------------------------------------------

inline std::vector<Node> synthetic_tree(const std::string &root) {
  const auto source = [](const std::string &parent, const std::string &name) {
    std::error_code ec;
    const auto bytes = std::filesystem::file_size(parent + "/" + name, ec);
    return make_file(parent, name, ec ? 0 : bytes);
  };
  return {
      make_dir(root, "src",
               {make_dir(root + "/src", "engine",
                         {source(root + "/src/engine", "input_injector.cpp"),
                          source(root + "/src/engine", "input_injector.h")}),
                make_dir(root + "/src", "systems",
                         {make_dir(root + "/src/systems", "screens",
                                   {source(root + "/src/systems/screens",
                                            "FileTreeShowcase.h"),
                                    source(root + "/src/systems/screens",
                                            "Forms.h"),
                                    source(root + "/src/systems/screens",
                                            "DividerShowcase.h"),
                                    source(root + "/src/systems/screens",
                                            "KirbyOptions.h")})}),
                source(root + "/src", "main.cpp"),
                source(root + "/src", "settings.cpp")}),
      make_dir(root, "tests",
               {make_dir(root + "/tests", "e2e_scripts",
                         {source(root + "/tests/e2e_scripts",
                                  "199_file_tree_design.e2e"),
                          source(root + "/tests/e2e_scripts",
                                 "204_forms_design.e2e")})}),
      source(root, "makefile"),
      source(root, "todo.md")};
}

inline std::vector<Node> synthetic_tree() {
  return synthetic_tree(project_root());
}

// ------------------------------------------------------------------
// Real filesystem scan (the "All files" view)
// ------------------------------------------------------------------

inline std::vector<Node> scan_directory(const std::string &dir_path) {
  std::vector<Node> nodes;
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

    Node node;
    node.data = fe;
    node.is_leaf = !fe.is_directory;
    nodes.push_back(std::move(node));
  }

  // Sort: directories first, then alphabetically
  std::sort(nodes.begin(), nodes.end(),
            [](const Node &a, const Node &b) {
              if (a.data.is_directory != b.data.is_directory)
                return a.data.is_directory > b.data.is_directory;
              return a.data.name < b.data.name;
            });

  return nodes;
}

// ------------------------------------------------------------------
// Expand/collapse + lazy load
// ------------------------------------------------------------------

inline bool is_expanded(const ExpandedSet &expanded,
                        const std::string &id) {
  return expanded.contains(id);
}

inline void toggle_expanded(ExpandedSet &expanded, const std::string &id) {
  if (expanded.contains(id)) {
    expanded.erase(id);
  } else {
    expanded.insert(id);
  }
}

// Insert every directory path in the tree into `expanded`.
inline void expand_all(const std::vector<Node> &nodes, ExpandedSet &expanded) {
  for (const auto &node : nodes) {
    if (!node.data.is_directory)
      continue;
    expanded.insert(node.data.path);
    expand_all(node.children, expanded);
  }
}

// Lazy-load expanded children for the real-fs view. No-op for the
// curated tree (use_real_fs == false), for files, and for collapsed
// dirs. Only scans when the node has no children yet, then recurses
// into children that are themselves expanded.
inline void lazy_load_children(Node &node, const ExpandedSet &expanded,
                               bool use_real_fs) {
  // The curated tree already contains the source files selected for this demo.
  if (!use_real_fs)
    return;
  if (!node.data.is_directory)
    return;
  if (!expanded.contains(node.data.path))
    return;
  if (node.children.empty())
    node.children = scan_directory(node.data.path);
  // Recursively load expanded children
  for (auto &child : node.children) {
    lazy_load_children(child, expanded, use_real_fs);
  }
}

// ------------------------------------------------------------------
// Selection / visibility / counts
// ------------------------------------------------------------------

inline const FileEntry *find_by_path(const std::vector<Node> &nodes,
                                     const std::string &path) {
  for (const auto &node : nodes) {
    if (node.data.path == path)
      return &node.data;
    if (const auto *found = find_by_path(node.children, path))
      return found;
  }
  return nullptr;
}

struct Counts {
  size_t folders = 0;
  size_t files = 0;
};

// Count every node in the tree (visible or not).
inline Counts count_nodes(const std::vector<Node> &nodes) {
  Counts c;
  for (const auto &node : nodes) {
    node.data.is_directory ? ++c.folders : ++c.files;
    const Counts child = count_nodes(node.children);
    c.folders += child.folders;
    c.files += child.files;
  }
  return c;
}

struct VisibleRow {
  const FileEntry *entry = nullptr;
  int depth = 0;
};

// Pre-order DFS: a node is visible if all its ancestors are expanded
// (roots are always shown). Mirrors the showcase walk exactly.
inline std::vector<VisibleRow> visible_rows(const std::vector<Node> &nodes,
                                            const ExpandedSet &expanded) {
  std::vector<VisibleRow> out;
  const std::function<void(const std::vector<Node> &, int, bool)> walk =
      [&](const std::vector<Node> &ns, int depth, bool shown) {
        for (const auto &node : ns) {
          if (shown)
            out.push_back({&node.data, depth});
          walk(node.children, depth + 1,
               shown && expanded.contains(node.data.path));
        }
      };
  walk(nodes, 0, true);
  return out;
}

// ------------------------------------------------------------------
// Size / path formatting (exact strings surfaced in the showcase)
// ------------------------------------------------------------------

inline std::string format_size(uintmax_t bytes) {
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

inline std::string relative_label(const std::string &path,
                                  const std::string &root) {
  if (path == root)
    return "wm_afterhours/";
  if (path.rfind(root + "/", 0) == 0)
    return "wm_afterhours/" + path.substr(root.size() + 1);
  return path;
}

inline std::string relative_label(const std::string &path) {
  return relative_label(path, project_root());
}

// ------------------------------------------------------------------
// Plugin facade: named entry point for future consumers
// ------------------------------------------------------------------
namespace plugin {

struct Descriptor {
  const char *name = "file_tree";
  const char *version = "1.0.0";
  // True: scan_directory skips dotfiles and sorts dirs-first alpha.
  bool dirs_first = true;
  bool skip_dotfiles = true;
  size_t preview_limit_bytes = 128 * 1024;
};

inline constexpr Descriptor descriptor{};

} // namespace plugin

} // namespace wm::plugins::file_tree
