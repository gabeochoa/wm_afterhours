#include "layout_summary.h"

#include "../input_mapping.h"
#include <afterhours/ah.h>
#include <afterhours/src/plugins/e2e_testing/ui_commands.h>
#include <cmath>
#include <filesystem>
#include <fstream>

namespace layout_summary {

bool write(std::ostream &out) {
  using afterhours::testing::ui_commands::get_screen_rect;

  for (afterhours::Entity &entity :
       afterhours::EntityQuery()
           .whereHasComponent<afterhours::ui::UIComponent>()
           .whereHasComponent<afterhours::ui::UIComponentDebug>()
           .gen()) {
    afterhours::ui::UIComponent &cmp = entity.get<afterhours::ui::UIComponent>();
    if (!cmp.was_rendered_to_screen) {
      continue;
    }

    const afterhours::ui::UIComponentDebug &dbg =
        entity.get<afterhours::ui::UIComponentDebug>();
    if (dbg.type != afterhours::ui::UIComponentDebug::Type::custom ||
        dbg.name().empty()) {
      continue;
    }

    auto rect = get_screen_rect(entity);
    out << dbg.name() << ' ' << std::lround(rect.x) << ' '
        << std::lround(rect.y) << ' ' << std::lround(rect.width) << ' '
        << std::lround(rect.height) << '\n';
  }

  return static_cast<bool>(out);
}

bool write_to_file(const std::string &path) {
  std::filesystem::path out_path(path);
  if (out_path.has_parent_path()) {
    std::filesystem::create_directories(out_path.parent_path());
  }

  std::ofstream out(path);
  if (!out.is_open()) {
    return false;
  }

  return write(out);
}

} // namespace layout_summary
