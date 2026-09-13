#pragma once

#include <afterhours/ah.h>
#include <filesystem>
#include <nlohmann/json.hpp>

namespace focus_audit {
nlohmann::json snapshot(bool include_elements = false);
void write(const std::filesystem::path &path, const nlohmann::json &data);
bool capture_crop(const std::filesystem::path &path,
                  const nlohmann::json &sample);
void register_systems(afterhours::SystemManager &systems);
}
