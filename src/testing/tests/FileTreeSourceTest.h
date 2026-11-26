#pragma once
// Unit tests for the FileTree plugin model
// (src/plugins/file_tree/file_tree_source.h + file_tree_preview.h).
//
// Pure model checks over a temp directory tree: scan ordering/dotfile
// handling/sizes, expand/collapse visibility, counts, selection,
// formatting, kind classification, and preview loading. No UIContext,
// no ECS, no raylib rendering. Registered via testing/tests/all_tests.h;
// runs headlessly under --run-all-tests.

#include "../../plugins/file_tree/file_tree_preview.h"
#include "../../plugins/file_tree/file_tree_source.h"
#include "../test_macros.h"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

namespace file_tree_source_test {
using namespace wm::plugins::file_tree;

inline void ft_assert(bool condition, const std::string &message) {
  if (!condition) {
    throw std::runtime_error(message);
  }
}

inline std::filesystem::path make_temp_tree() {
  const auto base = std::filesystem::temp_directory_path() / "wm_filetree_test";
  std::error_code ec;
  std::filesystem::remove_all(base, ec);
  std::filesystem::create_directories(base / "sub" / "deep");
  const auto write = [](const std::filesystem::path &p, const std::string &s) {
    std::ofstream out(p, std::ios::binary);
    out << s;
  };
  write(base / "a.txt", "hello\tworld\r\nsecond line\n");
  write(base / "B.txt", std::string("x", 2048));
  write(base / ".hidden", "secret");
  write(base / "sub" / "c.cpp", "int main(){}\n");
  write(base / "sub" / "deep" / "d.h", "pragma once\n");
  std::filesystem::create_directories(base / ".hiddendir");
  write(base / ".hiddendir" / "e.txt", "hidden");
  return base;
}

inline void remove_temp_tree(const std::filesystem::path &base) {
  std::error_code ec;
  std::filesystem::remove_all(base, ec);
}

} // namespace file_tree_source_test

TEST(file_tree_scan_and_counts) {
  using namespace file_tree_source_test;

  const auto base = make_temp_tree();
  const auto nodes = scan_directory(base.string());
  // Top level: sub (dir) first, then a.txt, B.txt; dotfiles skipped.
  ft_assert(nodes.size() == 3, "scan sees 3 top-level entries");
  ft_assert(nodes[0].data.is_directory && nodes[0].data.name == "sub",
            "directories sort first");
  ft_assert(nodes[1].data.name == "B.txt" && nodes[2].data.name == "a.txt",
            "files sort alphabetically (ASCII order)");
  ft_assert(nodes[1].data.file_size == 2048, "B.txt size is recorded");
  ft_assert(nodes[2].data.file_size == 25, "a.txt size is recorded");
  ft_assert(nodes[0].is_leaf == false && nodes[1].is_leaf == true,
            "leaf flags track kind");

  // Classify the scan result.
  ft_assert(classify(nodes[0].data) == EntryKind::Directory &&
                classify(nodes[1].data) == EntryKind::File,
            "classify tracks is_directory");
  ft_assert(is_expandable(nodes[0].data) && !is_expandable(nodes[1].data),
            "expandable == directory");
  ft_assert(should_show_size(nodes[1].data) &&
                !should_show_size(nodes[0].data),
            "size shown for files only");
  ft_assert(extension_of(nodes[1].data) == ".txt" &&
                extension_of(nodes[0].data).empty(),
            "extension_of on files vs dirs");

  // Selection lookup over a hand-built tree.
  const auto sub_path = (base / "sub").string();
  const auto deep_path = (base / "sub" / "deep").string();
  const auto file_path = (base / "sub" / "deep" / "d.h").string();
  const std::vector<Node> tree = {
      make_dir(base.string(), "sub",
               {make_dir(sub_path, "deep",
                         {make_file(deep_path, "d.h", 11)})})};
  const auto *found = find_by_path(tree, file_path);
  ft_assert(found && found->name == "d.h" && found->file_size == 11,
            "find_by_path descends to nested files");
  ft_assert(find_by_path(tree, base.string() + "/nope") == nullptr,
            "find_by_path returns nullptr for unknown paths");

  const Counts c = count_nodes(tree);
  ft_assert(c.folders == 2 && c.files == 1, "count_nodes counts nested tree");

  remove_temp_tree(base);
  co_return;
}

TEST(file_tree_expand_collapse_visibility) {
  using namespace file_tree_source_test;

  const auto base = make_temp_tree();
  const auto sub_path = (base / "sub").string();
  const auto deep_path = (base / "sub" / "deep").string();
  const std::vector<Node> tree = {
      make_dir(base.string(), "sub",
               {make_dir(sub_path, "deep",
                         {make_file(deep_path, "d.h", 11)}),
                make_file(sub_path, "c.cpp", 13)}),
      make_file(base.string(), "a.txt", 20)};

  ExpandedSet expanded;
  // Collapsed: only roots visible.
  auto rows = visible_rows(tree, expanded);
  ft_assert(rows.size() == 2 && rows[0].entry->name == "sub" &&
                rows[1].entry->name == "a.txt",
            "collapsed shows roots only");
  ft_assert(rows[0].depth == 0, "roots are depth 0");

  // Expand sub only: its children appear at depth 1, deep's child hidden.
  expanded.insert(sub_path);
  rows = visible_rows(tree, expanded);
  ft_assert(rows.size() == 4, "expanding sub reveals its children");
  ft_assert(rows[1].entry->name == "deep" && rows[1].depth == 1,
            "child depth is 1");
  bool saw_deep_child = false;
  for (const auto &r : rows)
    if (r.entry->name == "d.h")
      saw_deep_child = true;
  ft_assert(!saw_deep_child, "collapsed deep hides its child");

  // expand_all + toggle round-trip.
  expand_all(tree, expanded);
  ft_assert(expanded.contains(sub_path) && expanded.contains(deep_path),
            "expand_all inserts every directory");
  rows = visible_rows(tree, expanded);
  ft_assert(rows.size() == 5, "fully expanded shows every node");
  toggle_expanded(expanded, sub_path);
  rows = visible_rows(tree, expanded);
  ft_assert(rows.size() == 2, "collapsing sub hides the subtree");
  ft_assert(!is_expanded(expanded, sub_path), "toggle collapses");

  // Lazy load: curated tree (use_real_fs=false) never scans; real-fs
  // scans expanded empty dirs.
  std::vector<Node> lazy = {make_dir(base.string(), "sub", {})};
  lazy_load_children(lazy[0], expanded, false);
  ft_assert(lazy[0].children.empty(),
            "curated tree does not lazy-load");
  expanded.insert(sub_path);
  lazy_load_children(lazy[0], expanded, true);
  ft_assert(!lazy[0].children.empty(),
            "real-fs lazy-load scans expanded dir");

  remove_temp_tree(base);
  co_return;
}

TEST(file_tree_format_and_preview) {
  using namespace file_tree_source_test;

  ft_assert(format_size(0) == "0 B" && format_size(1023) == "1023 B",
            "byte formatting");
  ft_assert(format_size(1024) == "1.0 KB" &&
                format_size(1536) == "1.5 KB",
            "KB formatting");
  ft_assert(format_size(1024 * 1024) == "1.0 MB",
            "MB formatting");
  ft_assert(format_size(1024LL * 1024 * 1024) == "1.0 GB",
            "GB formatting");

  ft_assert(relative_label("/repo/sub/x", "/repo") == "wm_afterhours/sub/x" &&
                relative_label("/repo", "/repo") == "wm_afterhours/" &&
                relative_label("/elsewhere", "/repo") == "/elsewhere",
            "relative_label prefixing");

  const auto base = make_temp_tree();
  const auto a_path = (base / "a.txt").string();

  // Text preview: lines split, \r stripped, tabs expanded.
  const auto prev = preview::load_text(a_path);
  ft_assert(prev.status == "Read only", "text preview status");
  ft_assert(prev.lines.size() == 2 &&
                prev.lines[0].find("hello    world") == 0,
            "preview splits lines and expands tabs");
  ft_assert(prev.lines[0].empty() == false &&
                prev.lines[0].find('\r') == std::string::npos,
            "preview strips trailing carriage returns");

  // Selection preview: files load, dirs/null do not.
  FileEntry file{"a.txt", a_path, false, 20};
  FileEntry dir{"sub", (base / "sub").string(), true, 0};
  ft_assert(preview::load_for_selection(&file).status == "Read only",
            "file selection loads preview");
  ft_assert(preview::load_for_selection(&dir).lines.empty() &&
                preview::load_for_selection(nullptr).lines.empty(),
            "dirs and null select empty preview");

  // Binary + missing handling.
  const auto bin_path = (base / "bin.dat").string();
  {
    std::ofstream out(bin_path, std::ios::binary);
    out.write("a\0b", 3);
  }
  ft_assert(preview::load_text(bin_path).status ==
                "Binary file / text preview unavailable",
            "binary content is rejected");
  ft_assert(preview::load_text((base / "missing.txt").string()).status ==
                "Could not open this file.",
            "missing file reports open failure");

  // Synthetic tree shape (counts 6 folders / 12 files over the
  // fixed root, sizes from disk or 0).
  const auto synth = synthetic_tree("/repo");
  const Counts c = count_nodes(synth);
  ft_assert(c.folders == 6 && c.files == 12,
            "synthetic tree is 6 folders / 12 files");
  ft_assert(find_by_path(synth, "/repo/src/systems/screens/FileTreeShowcase.h") != nullptr,
            "synthetic tree contains the showcase file");
  ft_assert(extension_of(FileEntry{"archive.tar.gz", "/x", false, 1}) == ".gz" &&
                extension_of(FileEntry{"makefile", "/x", false, 1}).empty(),
            "extension_of handles multi-dot and dotfile-less names");

  remove_temp_tree(base);
  co_return;
}
