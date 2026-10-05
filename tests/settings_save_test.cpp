#include "../src/settings.h"
#include <afterhours/src/plugins/files.h>
#include <afterhours/src/plugins/reduced_motion.h>
#include <nlohmann/json.hpp>
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <string>

int main() {
  namespace fs = std::filesystem;
  const auto previous = fs::current_path();
  const auto temporary = fs::temp_directory_path() / ("wm-settings-" +
      std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
  fs::create_directory(temporary);
  fs::current_path(temporary);
  // Pin settings to this temp dir so executable-relative candidates (the repo
  // settings next to a built test binary) cannot leak into the test.
  const std::string settings_override = (temporary / "settings.json").string();
  assert(::setenv("WM_SETTINGS_PATH", settings_override.c_str(), 1) == 0);
  auto &settings = Settings::get();
  settings.update_music_volume(.7f);
  assert(settings.write_save_file());
  const auto original = afterhours::files::read_string("settings.json");
  assert(original);
  settings.update_music_volume(.9f);
  const auto blocker = std::string("settings.json") + afterhours::files::TEMP_SUFFIX;
  fs::create_directory(blocker);
  assert(!settings.write_save_file());
  assert(afterhours::files::read_string("settings.json") == original);
  fs::remove(blocker);
  assert(settings.write_save_file());
  const auto saved = nlohmann::json::parse(*afterhours::files::read_string("settings.json"));
  assert(saved["audio"]["music_volume"].get<float>() == .9f);
  assert(saved["project"].get<std::string>() == "wm_afterhours");
  assert(!fs::exists(blocker));

  const bool os_reduced = afterhours::os::reduced_motion_enabled();
  auto without_key = saved;
  without_key.erase("reduced_motion_enabled");
  assert(afterhours::files::write_string_atomic("settings.json", without_key.dump(4)));
  settings.reset();
  settings.set_reduced_motion_enabled(!os_reduced);
  assert(settings.load_save_file(1280, 720));
  assert(settings.get_reduced_motion_enabled() == os_reduced);
  auto with_key = saved;
  with_key["reduced_motion_enabled"] = !os_reduced;
  assert(afterhours::files::write_string_atomic("settings.json", with_key.dump(4)));
  settings.reset();
  assert(settings.load_save_file(1280, 720));
  assert(settings.get_reduced_motion_enabled() == !os_reduced);
  auto legacy = saved;
  legacy.erase("project");
  legacy.erase("reduced_motion_enabled");
  assert(afterhours::files::write_string_atomic("settings.json", legacy.dump(4)));
  settings.reset();
  settings.set_reduced_motion_enabled(!os_reduced);
  assert(settings.load_save_file(1280, 720));
  assert(settings.get_reduced_motion_enabled() == os_reduced);
  fs::remove("settings.json");
  settings.reset();
  assert(!settings.load_save_file(1280, 720));
  assert(settings.get_reduced_motion_enabled() == os_reduced);

  // A same-named file from another app must be refused, not parsed as WM
  // settings and never overwritten by a later save.
  const std::string foreign =
      R"({"commit_log_ratio":0.5,"window_width":1440})";
  assert(afterhours::files::write_string_atomic("settings.json", foreign));
  settings.reset();
  assert(!settings.load_save_file(1280, 720));
  assert(settings.get_reduced_motion_enabled() == os_reduced);
  assert(!settings.write_save_file());
  assert(afterhours::files::read_string("settings.json").value() == foreign);

  // A valid WM-shaped file for another project must still be refused by the
  // project marker, so identity does not depend on schema coincidence.
  auto wrong_project = saved;
  wrong_project["project"] = "cartographer";
  const std::string wrong_project_text = wrong_project.dump(4);
  assert(afterhours::files::write_string_atomic("settings.json", wrong_project_text));
  settings.reset();
  assert(!settings.load_save_file(1280, 720));
  assert(settings.get_reduced_motion_enabled() == os_reduced);
  assert(!settings.write_save_file());
  assert(afterhours::files::read_string("settings.json").value() == wrong_project_text);

  assert(::unsetenv("WM_SETTINGS_PATH") == 0);
  fs::current_path(previous);
  fs::remove_all(temporary);
}
