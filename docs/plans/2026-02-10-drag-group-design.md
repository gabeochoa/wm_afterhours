# drag_group() Design

Drop-in replacement for `div()` that enables drag-and-drop reordering within
a container and moving items between containers.

## API

```cpp
// Screen renders drag_group just like a div:
auto group = drag_group(ctx, mk(parent, 0), config);
for (int i = 0; i < items.size(); i++) {
    render_card(ctx, group.ent(), i, items[i], theme);
}
```

The screen consumes completed drag events each frame:

```cpp
auto *state = EntityHelper::get_singleton_cmp<DragGroupState>();
for (auto &evt : state->events) {
    // move items[evt.source_index] from source group to target group at target index
}
state->events.clear();
```

## Data Structures

All in `vendor/afterhours/src/plugins/ui/components.h`.

```cpp
// Bare marker component attached by drag_group() to its div entity.
struct HasDragGroup : BaseComponent {};

// Singleton component on ui_root.
struct DragGroupState : BaseComponent {
  struct Event {
    EntityID source_group;
    int source_index;
    EntityID target_group;
    int target_index;
  };

  bool dragging = false;
  EntityID drag_source_group = -1;
  int drag_source_index = -1;
  EntityID dragged_entity_id = -1;

  EntityID hover_group = -1;
  int hover_index = -1;

  // Original size of dragged item (for spacer + overlay).
  float dragged_width = 0;
  float dragged_height = 0;

  std::vector<Event> events;
};
```

## Widget Function

In `vendor/afterhours/src/plugins/ui/imm_components.h`.

`drag_group()` creates a `div()` and attaches the `HasDragGroup` marker. Returns
`ElementResult` so the screen can parent children to it, identical to `scroll_view()`.

```cpp
ElementResult drag_group(HasUIContext auto &ctx, EntityParent ep_pair,
                         ComponentConfig config = ComponentConfig()) {
  auto [entity, parent] = deref(ep_pair);
  if (config.size.is_default)
    config.with_size(ComponentSize{percent(1.0f), children()});
  if (config.flex_direction == FlexDirection::None)
    config.with_flex_direction(FlexDirection::Column);

  _init_component(ctx, ep_pair, config, ComponentType::Div, false, "drag_group");
  entity.template addComponentIfMissing<HasDragGroup>();
  return {true, entity};
}
```

## System

`HandleDragGroups` in `vendor/afterhours/src/plugins/ui/systems.h`. Registered
in `UIPluginPostUpdateBridge` after `HandleDrags`.

Queries all entities with `HasDragGroup` + `UIComponent`. Gets `DragGroupState`
from the singleton.

### Per-frame logic

**Not dragging — detect drag start:**
- For each drag group, iterate `UIComponent::children`.
- If `mouse.just_pressed` and mouse is inside a child rect, begin drag:
  - Set `dragging = true`, record source group entity ID, child index, child size.
  - Set `dragged_entity_id` to the child.

**Dragging — update hover and visuals:**
- Hide the dragged entity (`ShouldHide` component).
- Insert/move a spacer entity in the source group at the drag source index
  (same size as the original item).
- Check mouse position against all drag groups and their children to determine
  `hover_group` and `hover_index`.
- If hovering a different index or group, move the spacer to the hover position.
- Render a floating overlay: a top-level div parented to the UI root, sized to
  the original item dimensions, positioned at the mouse cursor, on a high layer.
  The dragged entity's visual content is cloned or re-rendered inside this overlay.

**Mouse released — emit event and clean up:**
- Emit `Event{source_group, source_index, hover_group, hover_index}`.
- Remove the spacer entity.
- Remove `ShouldHide` from the dragged entity.
- Reset all drag state fields.

### Visual behavior

- **Reordering within a group:** the spacer (gap) moves through the children
  list as the mouse moves over different positions. Items shift to accommodate.
- **Dragging between groups:** the source group keeps a gap where the item was.
  The target group shows a gap at the hover position.

## Registration

In `vendor/afterhours/src/plugins/ui/utilities.h`:

- Add `DragGroupState` to `init_ui_plugin()` as a singleton on `ui_root`.
- Add `HandleDragGroups` to `UIPluginPostUpdateBridge` after `HandleDrags`.

## Screen-Side Usage (DragDropShowcase)

The screen stores items in per-column vectors. Each frame:

1. Poll `DragGroupState::events` and apply moves to the vectors.
2. Render each column using `drag_group()` + a loop over the vector.

No drag-aware rendering logic needed in the screen code.

## Files Changed

| File | Change |
|------|--------|
| `vendor/.../ui/components.h` | Add `HasDragGroup`, `DragGroupState` |
| `vendor/.../ui/imm_components.h` | Add `drag_group()` function |
| `vendor/.../ui/systems.h` | Add `HandleDragGroups` system |
| `vendor/.../ui/utilities.h` | Register singleton + system |
| `src/.../DragDropShowcase.h` | Use `drag_group()`, consume events |
