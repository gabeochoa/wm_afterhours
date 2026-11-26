// Batch 6 Lane S: settings foreign/corrupt-file refusal policy tests.
// Covers only the pure classification layer in
// src/plugins/settings_policy/settings_file_policy.h; the IO/refusal
// behavior suite stays in tests/settings_save_test.cpp. No makefile
// target (mirrors settings_save_test); build from the repo root:
//
//   clang++ -std=c++23 -I vendor -Isrc tests/settings_policy_test.cpp \
//     -o /tmp/settings_policy_test && /tmp/settings_policy_test
#include "../src/plugins/settings_policy/settings_file_policy.h"
#include <nlohmann/json.hpp>
#include <cassert>
#include <string_view>
#include <vector>

int main() {
  using namespace wm::plugins::settings_policy;
  using nlohmann::json;

  // Plugin descriptor is stable and self-describing.
  assert(std::string_view(plugin::descriptor.name) == "settings_policy");
  assert(std::string_view(plugin::descriptor.version) == "1.0.0");
  assert(std::string_view(plugin::descriptor.project_marker) == "wm_afterhours");
  assert(std::string_view(kProjectMarker) == "wm_afterhours");

  const auto marked = json::parse(
      R"({"project":"wm_afterhours","resolution":{"width":1280,"height":720},)"
      R"("audio":{"master_volume":0.5,"music_volume":0.5,"sfx_volume":0.5},)"
      R"("fullscreen_enabled":false,"post_processing_enabled":true})");
  const auto unmarked = json::parse(
      R"({"resolution":{"width":1280,"height":720},)"
      R"("audio":{"master_volume":0.5,"music_volume":0.5,"sfx_volume":0.5},)"
      R"("fullscreen_enabled":false,"post_processing_enabled":true})");

  // Marked match: accepted when the full schema parses.
  assert(classify_marker(marked) == Marker::Match);
  assert(classify(marked, true) == FileClass::AcceptMarked);
  assert(accepts_file(classify(marked, true)));
  // Marked match but corrupt schema: still refused (schema mismatch).
  assert(classify(marked, false) == FileClass::RefuseSchemaMismatch);
  assert(!accepts_file(classify(marked, false)));

  // Marked foreign: refused regardless of schema coincidence.
  auto foreign = marked;
  foreign["project"] = "cartographer";
  assert(classify_marker(foreign) == Marker::Foreign);
  assert(classify(foreign, true) == FileClass::RefuseForeignProject);
  assert(classify(foreign, false) == FileClass::RefuseForeignProject);
  assert(!accepts_file(classify(foreign, true)));

  // Non-string project: refused as invalid, never accepted.
  for (const auto &bad : {json(42), json(true), json(nullptr),
                          json::array(), json::object()}) {
    auto doc = unmarked;
    doc["project"] = bad;
    assert(classify_marker(doc) == Marker::Invalid);
    assert(classify(doc, true) == FileClass::RefuseInvalidProjectMarker);
    assert(!accepts_file(classify(doc, true)));
  }

  // Unmarked full-schema (legacy): accepted only when the schema parses.
  assert(classify_marker(unmarked) == Marker::Absent);
  assert(classify(unmarked, true) == FileClass::AcceptLegacyUnmarked);
  assert(accepts_file(classify(unmarked, true)));
  // Unmarked partial/corrupt: refused as schema mismatch.
  const auto partial = json::parse(R"({"resolution":{"width":1280}})");
  assert(classify(partial, false) == FileClass::RefuseSchemaMismatch);
  assert(classify(unmarked, false) == FileClass::RefuseSchemaMismatch);
  assert(!accepts_file(classify(partial, false)));

  // Write-fallback decision: first nonexistent app-owned candidate
  // from index 1 on; index 0 (launch dir) is never chosen.
  assert(blocks_save(true, first_missing_candidate(4, {true, true, true, true})));
  const auto target = first_missing_candidate(4, {true, true, false, false});
  assert(target.has_target && target.index == 2);
  assert(!blocks_save(true, target));
  const auto early = first_missing_candidate(3, {true, false, false});
  assert(early.has_target && early.index == 1);
  // No refusal seen: never blocked.
  assert(!blocks_save(false, first_missing_candidate(2, {true, true})));
  // Single candidate (override): no index-1 candidate exists.
  assert(blocks_save(true, first_missing_candidate(1, {true})));
  assert(!first_missing_candidate(1, {true}).has_target);
}
