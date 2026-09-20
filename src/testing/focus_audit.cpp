#include "focus_audit.h"
#include "../game.h"
#include "../input_mapping.h"
#include "../settings.h"
#include "screenshot_validation.h"
#include <afterhours/src/plugins/e2e_testing/e2e_testing.h>
#include <afterhours/src/plugins/modal.h>
#include <afterhours/src/plugins/ui/menu.h>
#include <afterhours/src/plugins/ui/rendering.h>
#include <afterhours/src/plugins/ui/tooltip.h>
#include <cmath>
#include <fstream>

namespace focus_audit {
namespace {
using namespace afterhours;
using namespace afterhours::ui;
using Json = nlohmann::json;

Json rect_json(RectangleType r) { return {r.x, r.y, r.width, r.height}; }

RectangleType rotated_bounds(RectangleType rect, float degrees) {
  const float radians = degrees * 3.14159265358979323846f / 180.f;
  const float width = std::abs(std::cos(radians)) * rect.width +
                      std::abs(std::sin(radians)) * rect.height;
  const float height = std::abs(std::sin(radians)) * rect.width +
                       std::abs(std::cos(radians)) * rect.height;
  return {rect.x + (rect.width - width) * .5f,
          rect.y + (rect.height - height) * .5f, width, height};
}

Json describe(Entity &entity, UIContext<InputAction> &context) {
  const auto &cmp = entity.get<UIComponent>();
  auto rect = cmp.rect();
  const auto scroll = afterhours::ui::detail::accumulated_scroll_offset(entity);
  rect.x -= scroll.x;
  rect.y -= scroll.y;
  if (entity.has<HasUIModifiers>())
    rect = entity.get<HasUIModifiers>().apply_modifier(rect);
  const auto [clips, clip] =
      afterhours::ui::detail::compute_intersected_clip_rect(entity);
  Json data = {{"id", entity.id},
               {"parent", cmp.parent},
               {"rect", rect_json(rect)},
               {"layout_rect", rect_json(cmp.rect())},
               {"visible", cmp.was_rendered_to_screen},
               {"hidden", cmp.should_hide || entity.has<ShouldHide>()},
               {"focusable", can_be_focused(context, entity)},
               {"input_allowed", context.is_input_allowed(entity.id)},
               {"skip_tab", cmp.skip_when_tabbing || entity.has<SkipWhenTabbing>()},
               {"clickable", entity.has<HasClickListener>()},
               {"draggable", entity.has<HasDragListener>()},
               {"cluster", entity.has<FocusClusterRoot>()},
               {"tray", entity.has<HasTray>()},
               {"scroll", {scroll.x, scroll.y}},
               {"clip", clips && !entity.has<HasScrollView>() ? rect_json(clip)
                                                              : Json(nullptr)},
               {"font", cmp.font_name},
               {"font_size", cmp.font_size.value}};
  if (entity.has<UIComponentDebug>())
    data["name"] = entity.get<UIComponentDebug>().name();
  if (entity.has<HasLabel>())
    data["label"] = entity.get<HasLabel>().label;
  if (entity.has<HasUIModifiers>())
    data["rotation"] = entity.get<HasUIModifiers>().rotation;
  if (entity.has<HasRoundedCorners>()) {
    const auto &corners = entity.get<HasRoundedCorners>();
    data["radius"] =
        resolve_roundness(corners.radius_px, corners.roundness, rect) *
        std::min(rect.width, rect.height) * .5f;
  }
  return data;
}

std::filesystem::path pending_path;
bool pending_crop = false;

struct HandleAudit : System<testing::PendingE2ECommand> {
  void for_each_with(Entity &, testing::PendingE2ECommand &cmd,
                     float) override {
    if (cmd.is_consumed() || !cmd.is("audit_ui"))
      return;
    if (!cmd.has_args(1)) {
      cmd.fail("audit_ui requires an output path without an extension");
      return;
    }
    pending_path = cmd.arg(0);
    pending_crop = cmd.has_args(2) && cmd.arg(1) == "crop";
    cmd.consume();
  }
};

struct RenderAudit : System<> {
  void once(float) override {
    if (pending_path.empty())
      return;
    auto path = std::exchange(pending_path, {});
    const auto data = snapshot(true);
    write(path.string() + ".json", data);
    if (pending_crop) {
      capture_crop(path.string() + ".png", data);
      return;
    }
    screenshot_validation::save_screenshot_to(path.string() + ".png");
  }
};
}

nlohmann::json snapshot(bool include_elements) {
  auto *context = EntityHelper::get_singleton_cmp<UIContext<InputAction>>();
  if (!context)
    throw std::runtime_error("Focus audit requires UIContext");
  Json data = {{"focus_id", context->focus_id},
               {"visual_focus_id", context->visual_focus_id},
               {"origin", context->focus_origin()},
               {"interacted", context->has_interacted},
               {"offset", context->theme.focus_ring_offset},
               {"thickness", context->theme.focus_ring_thickness},
               {"viewport",
                {Settings::get().get_screen_width(),
                 Settings::get().get_screen_height()}},
               {"candidates", Json::array()},
               {"modals", Json::array()},
               {"popups", Json::array()}};
  auto &collection = UICollectionHolder::get().collection;
  collection.merge_entity_arrays();
  for (auto &ref : EntityQuery(collection, {.ignore_temp_warning = true})
                       .whereHasComponent<UIComponent>()
                       .gen()) {
    auto &entity = ref.get();
    const auto &cmp = entity.get<UIComponent>();
    if (can_be_focused(*context, entity))
      data["candidates"].push_back(describe(entity, *context));
    if (include_elements && cmp.was_rendered_to_screen && !cmp.should_hide)
      data["elements"].push_back(describe(entity, *context));
    if (cmp.was_rendered_to_screen && entity.has<imm::HasMenuState>()) {
      const auto &state = entity.get<imm::HasMenuState>();
      auto panel = UICollectionHolder::getEntityForID(state.panel);
      if (state.was_open_last_frame && panel.valid() &&
          panel->has<UIComponent>()) {
        auto entry = describe(panel.asE(), *context);
        entry["restore_focus"] = state.restore_focus;
        entry["kind"] = "menu_or_popover";
        data["popups"].push_back(entry);
      }
    }
    if (cmp.was_rendered_to_screen && entity.has<HasDropdownState>() &&
        entity.get<HasDropdownState>().on) {
      for (auto child_id : cmp.children) {
        auto child = UICollectionHolder::getEntityForID(child_id);
        if (!child.valid() || !child->has<HasTray>())
          continue;
        auto entry = describe(child.asE(), *context);
        entry["kind"] = "dropdown";
        data["popups"].push_back(entry);
      }
    }
    if (context->focus_id == entity.id)
      data["focused"] = describe(entity, *context);
    if (context->visual_focus_id != entity.id)
      continue;
    data["visual"] = describe(entity, *context);
    auto ring = afterhours::ui::detail::focus_ring_for(
        *context, entity, cmp,
        afterhours::ui::detail::accumulated_scroll_offset(entity));
    if (!ring)
      continue;
    const float rotation = entity.has<HasUIModifiers>()
                               ? entity.get<HasUIModifiers>().rotation
                               : 0.f;
    data["ring"] = {
        {"inner", rect_json(ring->rect)},
        {"outer", rect_json(ring->expanded(ring->thickness + 1.f))},
        {"bounds", rect_json(rotated_bounds(
                       ring->expanded(ring->thickness + 1.f), rotation))},
        {"rotation", rotation},
        {"color", {ring->color.r, ring->color.g, ring->color.b, ring->color.a}},
        {"radius",
         ring->roundness * std::min(ring->rect.width, ring->rect.height) * .5f},
        {"corners", ring->corners.to_string()}};
  }
  if (EntityHelper::has_singleton<modal::ModalRoot>()) {
    for (auto id :
         EntityHelper::get_singleton_cmp<modal::ModalRoot>()->modal_stack) {
      auto opt = UICollectionHolder::getEntityForID(id);
      if (!opt.valid() || !opt->has<modal::Modal>() || !opt->has<UIComponent>())
        continue;
      auto entry = describe(opt.asE(), *context);
      const auto &m = opt->get<modal::Modal>();
      entry["title"] = m.title;
      entry["restore_focus"] = m.previously_focused_element;
      data["modals"].push_back(entry);
    }
  }
  if (EntityHelper::has_singleton<TooltipState>()) {
    const auto *tip = EntityHelper::get_singleton_cmp<TooltipState>();
    if (tip->is_showing())
      data["tooltip"] = {{"text", tip->text},
                         {"anchor", rect_json(tip->anchor)}};
  }
  return data;
}

void write(const std::filesystem::path &path, const nlohmann::json &data) {
  if (path.has_parent_path())
    std::filesystem::create_directories(path.parent_path());
  std::ofstream out(path);
  out << data.dump(2) << '\n';
  if (!out)
    throw std::runtime_error("Could not write focus audit: " + path.string());
}

bool capture_crop(const std::filesystem::path &path,
                  const nlohmann::json &sample) {
  if (!sample.contains("visual"))
    return false;
  const auto &visual = sample.at("visual");
  const auto &source_rect = visual.at("rect");
  const auto r = rect_json(
      rotated_bounds({source_rect[0].get<float>(), source_rect[1].get<float>(),
                      source_rect[2].get<float>(), source_rect[3].get<float>()},
                     visual.value("rotation", 0.f)));
  const float left = std::max(0.f, std::floor(r[0].get<float>() - 12.f));
  const float top = std::max(0.f, std::floor(r[1].get<float>() - 12.f));
  const float right =
      std::min(static_cast<float>(mainRT.texture.width),
               std::ceil(r[0].get<float>() + r[2].get<float>() + 12.f));
  const float bottom =
      std::min(static_cast<float>(mainRT.texture.height),
               std::ceil(r[1].get<float>() + r[3].get<float>() + 12.f));
  if (right <= left || bottom <= top)
    return false;
  auto image = raylib::LoadImageFromTexture(mainRT.texture);
  if (!image.data)
    return false;
  raylib::ImageFlipVertical(&image);
  raylib::ImageCrop(&image, {left, top, right - left, bottom - top});
  const bool saved = raylib::ExportImage(image, path.string().c_str());
  raylib::UnloadImage(image);
  return saved;
}

void register_systems(afterhours::SystemManager &systems) {
  systems.register_update_system(std::make_unique<HandleAudit>());
  systems.register_render_system(std::make_unique<RenderAudit>());
}
}
