#pragma once

#include <afterhours/src/plugins/ui/component_config.h>
#include <afterhours/src/plugins/ui/theme.h>

// Semantic starting points for the shapes screens actually repeat.
//
// Every screen used to spell these out as three to eight chained calls, and
// most of that spelling was noise: `.with_corner_radius(0)` alone appeared
// ~300 times because a config's radius is only sharp once someone says so.
// These helpers bake the boring parts in -- page surfaces are sharp, cards
// sit on the raised surface with the theme's own border, dividers are the
// muted border at 1px, header bars carry the primary fill -- so a call site
// only states what makes it different (size, position, label, an override).
//
// They are plain ComponentConfigs, not components: chain any with_* call on
// the result and the last write wins, which is how the pilot in Cards.h can
// adopt them without changing a pixel. Colours come from the theme passed
// in (its pinned surface_raised / border / border_muted once set, the
// theme.h fallbacks until then), never from literals here.

namespace shapes {

using afterhours::ui::imm::ComponentConfig;
using afterhours::ui::Theme;

// Full-page backdrop: the theme background, sharp corners. Sharp is stated
// explicitly because the config default would otherwise round the page.
inline ComponentConfig page_bg(const Theme &theme) {
  (void)theme; // background is a Usage, resolved against the live theme
  return ComponentConfig{}
      .with_background(Theme::Usage::Background)
      .with_corner_radius(0);
}

// A card/panel: raised surface over the page, the theme's control border at
// 1px, and the panel radius. Override padding, radius or border at the call
// site when a design needs something else.
inline ComponentConfig card(const Theme &theme) {
  return ComponentConfig{}
      .with_custom_background(theme.raised_surface())
      .with_border(theme.control_border(), 1.f)
      .with_corner_radius(theme.panel_corner_radius);
}

// A 1px horizontal rule in the muted border colour. Width defaults to the
// parent's full width; height is fixed so callers do not restate it.
inline ComponentConfig divider(const Theme &theme) {
  return ComponentConfig{}
      .with_size({afterhours::ui::percent(1.f), afterhours::ui::pixels(1.f)})
      .with_custom_background(theme.subtle_border())
      .with_corner_radius(0);
}

// A header/title bar: primary fill with the panel radius. Text colour is
// left to the labels placed inside it (auto text colour is on by default).
inline ComponentConfig header_bar(const Theme &theme) {
  return ComponentConfig{}
      .with_background(Theme::Usage::Primary)
      .with_corner_radius(theme.panel_corner_radius);
}

} // namespace shapes
