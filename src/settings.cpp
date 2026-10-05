#include "settings.h"

#include <afterhours/src/plugins/animation.h>

#include <algorithm>
#include <cstdlib>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <system_error>
#include <vector>

#include "rl.h"
#include <afterhours/src/plugins/files.h>
#include <afterhours/src/plugins/reduced_motion.h>
#include <afterhours/src/plugins/sound_system.h>

using namespace afterhours;

template <typename T> struct ValueHolder {
  using value_type = T;
  T data;

  ValueHolder(const T &initial) : data(initial) {}
  T &get() { return data; }
  const T &get() const { return data; }
  void set(const T &data_) { data = data_; }

  operator const T &() { return get(); }
};

struct Pct : ValueHolder<float> {
  Pct(const float &initial) : ValueHolder(0.f) { set(initial); }
  void set(const float &data_) { data = std::min(1.f, std::max(0.f, data_)); }
  float str() const { return data; }
};

struct S_Data {
  afterhours::window_manager::Resolution resolution = {
      .width = 1280,
      .height = 720,
  };

  Pct master_volume = 0.1f;
  Pct music_volume = 0.1f;
  Pct sfx_volume = 0.1f;

  bool fullscreen_enabled = false;
  bool post_processing_enabled = true;
  bool reduced_motion_enabled = false;

  std::filesystem::path loaded_from;
  std::filesystem::path save_to;
  bool save_blocked = false;
};

constexpr const char *kSettingsProject = "wm_afterhours";

void to_json(nlohmann::json &j,
             const afterhours::window_manager::Resolution &resolution) {
  j = nlohmann::json{
      {"width", resolution.width},
      {"height", resolution.height},
  };
}

void from_json(const nlohmann::json &j,
               afterhours::window_manager::Resolution &resolution) {
  j.at("width").get_to(resolution.width);
  j.at("height").get_to(resolution.height);
}

void to_json(nlohmann::json &j, const S_Data &data) {
  j["project"] = kSettingsProject;

  nlohmann::json rez_j;
  to_json(rez_j, data.resolution);
  j["resolution"] = rez_j;

  nlohmann::json audio_j;
  audio_j["master_volume"] = data.master_volume.str();
  audio_j["music_volume"] = data.music_volume.str();
  audio_j["sfx_volume"] = data.sfx_volume.str();
  j["audio"] = audio_j;

  j["fullscreen_enabled"] = data.fullscreen_enabled;
  j["post_processing_enabled"] = data.post_processing_enabled;
  j["reduced_motion_enabled"] = data.reduced_motion_enabled;
}

void from_json(const nlohmann::json &j, S_Data &data) {
  from_json(j.at("resolution"), data.resolution);

  nlohmann::json audio_j = j.at("audio");
  data.master_volume.set(audio_j.at("master_volume"));
  data.music_volume.set(audio_j.at("music_volume"));
  data.sfx_volume.set(audio_j.at("sfx_volume"));

  data.fullscreen_enabled = j.at("fullscreen_enabled");

  if (j.contains("post_processing_enabled")) {
    data.post_processing_enabled = j.at("post_processing_enabled");
  }
  if (j.contains("reduced_motion_enabled")) {
    data.reduced_motion_enabled = j.at("reduced_motion_enabled");
  }
}

namespace {
// Empty when unset. Tests and controlled run dirs set this to isolate
// settings from both the launch directory and the executable.
std::filesystem::path settings_path_override() {
  const char *override_path = std::getenv("WM_SETTINGS_PATH");
  return (override_path == nullptr || override_path[0] == '\0')
             ? std::filesystem::path{}
             : std::filesystem::path(override_path);
}

void add_settings_candidate(std::vector<std::filesystem::path> &places,
                            const std::filesystem::path &place) {
  if (place.empty())
    return;
  const auto normalized = place.lexically_normal();
  if (std::find(places.begin(), places.end(), normalized) == places.end())
    places.push_back(normalized);
}

std::vector<std::filesystem::path> settings_places() {
  const auto override_path = settings_path_override();
  if (!override_path.empty())
    return {override_path};

  std::vector<std::filesystem::path> places;
  // A settings.json in the launch directory is only a candidate, never a
  // commitment: load uses it when its project marker matches, or for legacy
  // unmarked files only when the full WM schema parses. That preserves
  // repo-root and copied-run-dir launches, while another app's same-named
  // file is skipped in favour of the executable's own settings.
  add_settings_candidate(places,
                         std::filesystem::current_path() / "settings.json");
  const auto exe_dir = files::get_executable_dir();
  if (!exe_dir.empty()) {
    add_settings_candidate(places, exe_dir / "settings.json");
    if (exe_dir.has_parent_path())
      add_settings_candidate(places,
                             exe_dir.parent_path() / "settings.json");
  }
  if (files::get_provider() != nullptr)
    add_settings_candidate(places, files::get_save_path() / "settings.json");
  return places;
}
} // namespace

Settings::Settings() { data = new S_Data(); }
Settings::~Settings() { delete data; }

void Settings::reset() {
  delete data;
  data = new S_Data();
  refresh_settings();
}

int Settings::get_screen_width() const { return data->resolution.width; }
int Settings::get_screen_height() const { return data->resolution.height; }
float Settings::get_music_volume() const { return data->music_volume; }
float Settings::get_sfx_volume() const { return data->sfx_volume; }
float Settings::get_master_volume() const { return data->master_volume; }

void Settings::update_resolution(afterhours::window_manager::Resolution rez) {
  data->resolution = rez;
}

void Settings::update_music_volume(float vol) {
  data->music_volume = vol;
  sound_system::set_music_volume(data->music_volume);
}

void Settings::update_sfx_volume(float vol) {
  data->sfx_volume = vol;
  sound_system::set_sound_volume(data->sfx_volume);
}

void Settings::update_master_volume(float vol) {
  data->master_volume = vol;
  sound_system::set_master_volume(data->master_volume);
}

void match_fullscreen_to_setting(bool fs_enabled) {
  if (raylib::IsWindowFullscreen() && fs_enabled)
    return;
  if (!raylib::IsWindowFullscreen() && !fs_enabled)
    return;
  raylib::ToggleFullscreen();
}

void Settings::refresh_settings() {
  update_music_volume(data->music_volume);
  update_sfx_volume(data->sfx_volume);
  update_master_volume(data->master_volume);
  match_fullscreen_to_setting(data->fullscreen_enabled);
  afterhours::animation::set_instant(data->reduced_motion_enabled);
}

void Settings::toggle_fullscreen() {
  data->fullscreen_enabled = !data->fullscreen_enabled;
  raylib::ToggleFullscreen();
}

bool &Settings::get_fullscreen_enabled() { return data->fullscreen_enabled; }

bool &Settings::get_post_processing_enabled() {
  return data->post_processing_enabled;
}
bool Settings::get_reduced_motion_enabled() const {
  return data->reduced_motion_enabled;
}
void Settings::set_reduced_motion_enabled(bool enabled) {
  data->reduced_motion_enabled = enabled;
  afterhours::animation::set_instant(enabled);
}

void Settings::toggle_post_processing() {
  data->post_processing_enabled = !data->post_processing_enabled;
}

bool Settings::load_save_file(int width, int height) {
  data->resolution.width = width;
  data->resolution.height = height;
  data->loaded_from.clear();
  data->save_to.clear();
  data->save_blocked = false;

  const auto places = settings_places();
  bool saw_unusable_settings = false;

  for (const auto &place : places) {
    std::ifstream ifs(place);
    if (!ifs.is_open())
      continue;
    try {
      const auto settingsJSON = nlohmann::json::parse(ifs, nullptr, true, true);
      if (settingsJSON.contains("project")) {
        const auto &project = settingsJSON.at("project");
        if (!project.is_string() ||
            project.get<std::string>() != kSettingsProject) {
          saw_unusable_settings = true;
          log_warn("Settings::load_save_file: skipping {} because it belongs "
                   "to a different project",
                   place);
          continue;
        }
      }
      const bool stored_reduced_motion =
          settingsJSON.contains("reduced_motion_enabled");
      S_Data parsed;
      parsed = settingsJSON;
      *data = parsed;
      if (!stored_reduced_motion)
        data->reduced_motion_enabled = afterhours::os::reduced_motion_enabled();
      data->loaded_from = place;
      data->save_to = place;
      log_info("opened file {}", place);
      refresh_settings();
      return true;
    } catch (const std::exception &e) {
      saw_unusable_settings = true;
      log_warn("Settings::load_save_file: skipping {} because it is not WM "
               "settings: {}",
               place, e.what());
    }
  }

  std::stringstream buffer;
  buffer << "Failed to find usable settings file (Read): \n";
  for (const auto &place : places)
    buffer << place << ", \n";
  log_warn("{}", buffer.str());
  data->reduced_motion_enabled = afterhours::os::reduced_motion_enabled();
  if (saw_unusable_settings) {
    // Never let the launch-directory write fallback replace a foreign or
    // corrupt settings.json that was just refused. Save to the first
    // app-owned candidate that does not exist yet; if every candidate
    // already exists but is unusable, run unsaved instead of overwriting it.
    for (size_t i = 1; i < places.size(); ++i) {
      std::error_code ec;
      if (!std::filesystem::exists(places[i], ec)) {
        data->save_to = places[i];
        break;
      }
    }
    data->save_blocked = data->save_to.empty();
  }
  refresh_settings();
  return false;
}

bool Settings::write_save_file() {
  if (data->save_blocked) {
    log_warn("Settings::write_save_file: not saving because every settings "
             "candidate already exists but is unusable");
    return false;
  }

  std::filesystem::path save_path = data->save_to;
  if (save_path.empty())
    save_path = settings_path_override();
  if (save_path.empty())
    save_path = data->loaded_from;
  if (save_path.empty())
    save_path = "settings.json";

  try {
    const nlohmann::json settingsJSON = *data;
    const auto content = settingsJSON.dump(4);
    if (!files::write_string_atomic(save_path.string(), content)) {
      log_warn("Settings::write_save_file: failed to save {}", save_path);
      return false;
    }
    data->loaded_from = save_path;
    data->save_to = save_path;
    data->save_blocked = false;
    log_info("Saved settings to {}", data->loaded_from);
    return true;
  } catch (const std::exception &error) {
    log_warn("Settings::write_save_file: {}: {}", save_path, error.what());
    return false;
  }
}
