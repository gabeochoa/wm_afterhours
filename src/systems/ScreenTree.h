#pragma once

#include <algorithm>
#include <cctype>
#include <set>
#include <string>
#include <vector>

struct ScreenTree {
  struct Entry { std::string name, category, description; int screen_index; };
  struct Row { std::string label; int entry = -1; bool header() const { return entry < 0; } };
  std::vector<Entry> entries;
  std::vector<Row> rows;
  std::set<std::string> expanded;
  std::string query;
  int cursor = 0;

  static std::string lower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return value;
  }
  static std::string category(std::string value) {
    if (value == "Components" || value == "Widgets" || value == "Text" || value == "Text Editing")
      return "Component Galleries";
    if (value == "Bug Reports") return "System Demos";
    return value.empty() ? "Uncategorised" : value;
  }
  void rebuild() {
    const auto needle = lower(query);
    rows.clear();
    std::vector<std::string> categories{"Game Mockups", "Component Galleries", "System Demos", "Tools"};
    for (const auto &entry : entries) {
      const auto name = category(entry.category);
      if (std::find(categories.begin(), categories.end(), name) == categories.end()) categories.push_back(name);
    }
    std::set<std::string> seen;
    for (const auto &group : categories) {
      bool header = false;
      for (std::size_t i = 0; i < entries.size(); ++i) {
        const auto &entry = entries[i];
        if (category(entry.category) != group || seen.contains(entry.name)) continue;
        if (!needle.empty() && lower(entry.name + " " + group + " " + entry.description).find(needle) == std::string::npos) continue;
        seen.insert(entry.name);
        if (!header) { rows.push_back({group, -1}); header = true; }
        if (needle.empty() && !expanded.contains(group)) continue;
        rows.push_back({entry.name, static_cast<int>(i)});
      }
    }
    cursor = std::clamp(cursor, 0, std::max(0, static_cast<int>(rows.size()) - 1));
  }
  void move(int amount) { cursor = std::clamp(cursor + amount, 0, std::max(0, static_cast<int>(rows.size()) - 1)); }
  void toggle(int row) {
    if (row < 0 || row >= static_cast<int>(rows.size()) || !rows[row].header()) return;
    const auto label = rows[row].label;
    if (!expanded.erase(label)) expanded.insert(label);
    rebuild();
    for (std::size_t i = 0; i < rows.size(); ++i) {
      if (rows[i].header() && rows[i].label == label) { cursor = static_cast<int>(i); return; }
    }
  }
  void left() {
    if (rows.empty()) return;
    if (rows[cursor].header()) {
      if (expanded.contains(rows[cursor].label)) toggle(cursor);
      return;
    }
    while (cursor > 0 && !rows[cursor].header()) --cursor;
  }
  void right() {
    if (rows.empty() || !rows[cursor].header()) return;
    if (!expanded.contains(rows[cursor].label)) { toggle(cursor); return; }
    move(1);
  }
  void reveal(int screen_index) {
    query.clear();
    rebuild();
    for (const auto &entry : entries) {
      if (entry.screen_index != screen_index) continue;
      const auto group = category(entry.category);
      for (std::size_t i = 0; i < rows.size(); ++i) {
        if (rows[i].header()) {
          if (rows[i].label == group) cursor = static_cast<int>(i);
          continue;
        }
        if (entries[rows[i].entry].screen_index != screen_index) continue;
        cursor = static_cast<int>(i);
        return;
      }
      return;
    }
  }
};
