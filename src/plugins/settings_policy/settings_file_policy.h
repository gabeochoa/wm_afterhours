#pragma once

// WM-local plugin: settings foreign/corrupt-file refusal policy
// (Batch 6 Lane S).
//
// Pure classification layer extracted from src/settings.cpp
// (behavior-preserving, zero behavior change). This header owns only
// the decisions; src/settings.cpp keeps ALL filesystem IO, candidate
// discovery (launch dir / executable dir / save path / WM_SETTINGS_PATH
// override), parsing, logging text/order, and save/write fallbacks.
//
// Previously embedded in Settings::load_save_file / write_save_file:
//   - the project marker check (kSettingsProject = "wm_afterhours"),
//   - legacy unmarked-schema acceptance (an unmarked file is accepted
//     only when the full WM schema parses; a foreign/corrupt unmarked
//     file is refused with the same "not WM settings" log),
//   - the write-fallback rule (never overwrite a refused file; save to
//     the first nonexistent app-owned candidate from index 1 on, else
//     block the save).
//
// No logging, no filesystem, no executable-path discovery here: callers
// pass parsed JSON + parse success, and receive a decision. The header
// is header-only; there is no .cpp to link. Do not weaken refusal
// behavior: every Refuse* class maps to the existing skip-and-continue
// path in settings.cpp. This plugin never changes the settings.json
// format.
//
// Public API lives in namespace wm::plugins::settings_policy.

#include <nlohmann/json.hpp>

#include <cstddef>
#include <string>
#include <vector>

namespace wm::plugins::settings_policy {

// Project marker written by Settings' to_json and required on load.
// Identical bytes to the former kSettingsProject in settings.cpp.
inline constexpr const char *kProjectMarker = "wm_afterhours";

// Marker-level classification (no schema consideration). settings.cpp
// uses this to decide whether to even attempt the schema parse, in the
// same order as before: marker refusal wins over any parse outcome.
enum class Marker {
  // project present, string, equal to kProjectMarker.
  Match,
  // project present, string, different value.
  Foreign,
  // project present, not a string.
  Invalid,
  // project absent: legacy unmarked file, pending schema check.
  Absent,
};

inline Marker classify_marker(const nlohmann::json &j) {
  if (!j.contains("project"))
    return Marker::Absent;
  const auto &project = j.at("project");
  if (!project.is_string())
    return Marker::Invalid;
  return project.get<std::string>() == kProjectMarker ? Marker::Match
                                                       : Marker::Foreign;
}

// Full file-level decision. `schema_ok` is whether the file parsed as
// the full WM schema (from_json succeeded). Marker refusal wins
// regardless of schema_ok, mirroring the load loop's check order.
enum class FileClass {
  // Marked with our project and the full schema parsed: accept.
  AcceptMarked,
  // Unmarked legacy file whose full schema parsed: accept.
  AcceptLegacyUnmarked,
  // Marked for another project: refuse, never parse or overwrite.
  RefuseForeignProject,
  // Marker present but not a string: refuse, never parse or overwrite.
  RefuseInvalidProjectMarker,
  // Marker allowed (or absent) but the full schema did not parse:
  // foreign/corrupt file refused as "not WM settings".
  RefuseSchemaMismatch,
};

inline FileClass classify(const nlohmann::json &j, bool schema_ok) {
  switch (classify_marker(j)) {
  case Marker::Foreign:
    return FileClass::RefuseForeignProject;
  case Marker::Invalid:
    return FileClass::RefuseInvalidProjectMarker;
  case Marker::Match:
    return schema_ok ? FileClass::AcceptMarked : FileClass::RefuseSchemaMismatch;
  case Marker::Absent:
    return schema_ok ? FileClass::AcceptLegacyUnmarked
                     : FileClass::RefuseSchemaMismatch;
  }
  return FileClass::RefuseSchemaMismatch;
}

// True for the two accept classes; false for every refusal.
inline bool accepts_file(FileClass cls) {
  return cls == FileClass::AcceptMarked ||
         cls == FileClass::AcceptLegacyUnmarked;
}

// Result of the write-fallback scan after a refused load: the index
// into the candidate list to save to, when one is usable.
struct FallbackTarget {
  bool has_target = false;
  std::size_t index = 0;
};

// First nonexistent app-owned candidate at index >= 1, mirroring the
// load_save_file fallback loop exactly (index 0 is the launch-directory
// candidate whose foreign/corrupt file must never be overwritten).
// `exists` flags must cover every candidate; a short list is treated
// as "does not exist" only for indices it actually covers, so the
// caller must pass one flag per candidate.
inline FallbackTarget first_missing_candidate(
    std::size_t candidate_count, const std::vector<bool> &exists) {
  for (std::size_t i = 1; i < candidate_count; ++i) {
    const bool present = i < exists.size() && exists[i];
    if (!present)
      return FallbackTarget{.has_target = true, .index = i};
  }
  return FallbackTarget{};
}

// Whether a refused load blocks the later save: only when something
// was refused and no fallback candidate is available.
inline bool blocks_save(bool saw_unusable, FallbackTarget target) {
  return saw_unusable && !target.has_target;
}

// Stable, self-describing plugin facade (convention shared with the
// other WM-local plugins).
namespace plugin {
struct Descriptor {
  const char *name = "settings_policy";
  const char *version = "1.0.0";
  const char *project_marker = kProjectMarker;
};
inline constexpr Descriptor descriptor{};
} // namespace plugin

} // namespace wm::plugins::settings_policy
