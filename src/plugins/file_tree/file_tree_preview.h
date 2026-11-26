#pragma once

// WM-local plugin: file-tree text preview loader (Batch 3 Lane I).
//
// Pure model: read a selected file as text (128 KB cap, binary
// detection, line splitting, tab expansion, status strings). No UI,
// no widgets, no afterhours/vendor edits. FileTreeShowcase keeps the
// caching wrapper (reload only when the selected path changes) and
// all rendering; this header holds the load itself so a second
// consumer can reuse the exact preview semantics.
//
// Public API lives in namespace wm::plugins::file_tree::preview.
// Header-only; see README.md in this directory.

#include "file_tree_source.h"

#include <cstddef>
#include <fstream>
#include <string>
#include <vector>

namespace wm::plugins::file_tree::preview {

inline constexpr size_t kLimitBytes = 128 * 1024;

struct Preview {
  std::string path;
  std::string status;
  std::vector<std::string> lines;
};

// Load a file path as text preview. Empty path yields an empty
// Preview (no selection). Unopenable files yield
// "Could not open this file." Binary content (embedded NUL) yields
// "Binary file / text preview unavailable" with no lines. Otherwise
// lines are split on \n, trailing \r is stripped, and tabs expand to
// 4 spaces; status is "Preview limited to first 128 KB" when the
// file hits the cap, else "Read only".
inline Preview load_text(const std::string &path) {
  Preview out;
  out.path = path;
  if (path.empty())
    return out;
  std::ifstream input(path, std::ios::binary);
  if (!input) {
    out.status = "Could not open this file.";
    return out;
  }
  constexpr size_t limit = kLimitBytes;
  std::string contents(limit, '\0');
  input.read(contents.data(), static_cast<std::streamsize>(contents.size()));
  contents.resize(static_cast<size_t>(input.gcount()));
  if (contents.find('\0') != std::string::npos) {
    out.status = "Binary file / text preview unavailable";
    return out;
  }
  std::istringstream lines(contents);
  for (std::string line; std::getline(lines, line);) {
    if (!line.empty() && line.back() == '\r')
      line.pop_back();
    for (size_t i = 0; (i = line.find('\t', i)) != std::string::npos; i += 4)
      line.replace(i, 1, "    ");
    out.lines.push_back(std::move(line));
  }
  out.status = contents.size() == limit ? "Preview limited to first 128 KB"
                                        : "Read only";
  return out;
}

// Load preview for a tree selection. Nullptr and directories yield
// an empty Preview (nothing to preview).
inline Preview load_for_selection(const FileEntry *selected) {
  if (!selected || selected->is_directory)
    return Preview{};
  return load_text(selected->path);
}

} // namespace wm::plugins::file_tree::preview
