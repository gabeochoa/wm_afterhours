#include "../src/settings.h"
#include <afterhours/src/plugins/files.h>
#include <afterhours/src/plugins/reduced_motion.h>
#include <nlohmann/json.hpp>
#include <cassert>
#include <chrono>
#include <filesystem>

int main() {
  namespace fs = std::filesystem;
  const auto previous = fs::current_path();
  const auto temporary = fs::temp_directory_path() / ("wm-settings-" +
      std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
  fs::create_directory(temporary);
  fs::current_path(temporary);
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
  fs::remove("settings.json");
  settings.reset();
  assert(!settings.load_save_file(1280, 720));
  assert(settings.get_reduced_motion_enabled() == os_reduced);

  fs::current_path(previous);
  fs::remove_all(temporary);
}
