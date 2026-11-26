#pragma once

// GalleryPage.h -- shared gallery / specimen template (layout audit #8 + #10).
//
// Most component-gallery screens used to hand-place every card with absolute
// coordinates: a title band, then a grid of specimen cards computed from
// `i % columns` arithmetic in each screen. That made every gallery a bespoke
// layout that could drift from its neighbours and could not be restyled in
// one place. This header is the one template those screens share:
//
//   gallery::gallery_page(...)  -- full-screen background, a centred page
//                                  column (modelled on the vstack root in
//                                  LayoutPatterns.h and the header in
//                                  DefaultTheme.h), and a title/subtitle
//                                  header band. Returns the page so callers
//                                  can append sections below the header.
//   gallery::specimen_grid(...) -- a card grid built from hstack rows inside
//                                  a vstack, spaced with the caller's gap
//                                  scale (DefaultSpacing by default). Each
//                                  card gets its specimen title, then the
//                                  caller's builder fills the card body.
//
// The template deliberately builds only from plain imm primitives (div /
// vstack / hstack). It does not depend on any screen scaffold: sibling
// refactors can land in any order and compose on top of this.
//
// Geometry is the caller's contract. Sizes arrive in design units and are
// multiplied by `scale` (the screens' usual min(w/1280, h/720) factor), so a
// converted screen can reproduce its previous absolute layout exactly while
// gaining real parent/child flow.

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>
#include <functional>
#include <span>
#include <string>

namespace gallery {

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// One specimen card in a grid: the heading drawn by the template and the
// debug name the card is anchored by (e2e scripts address cards by name).
struct Specimen {
  const char *title;
  const char *debug_name;
};

// Layout and palette for a gallery page, in design units (pre-scale).
// Defaults reproduce the geometry the hand-placed galleries already used:
// a 1144-wide centred column on a 1280x720 canvas, a 92-tall header band,
// and 368x188 specimen cards on a 20/22 column/row gap.
struct GalleryOptions {
  float scale = 1.f;
  float design_width = 1280.f;
  float design_height = 720.f;
  float content_width = 1144.f;
  float top_inset = 24.f;    // page top padding above the header band
  float page_gap = 24.f;     // vertical gap between page sections
  float header_height = 92.f;
  float card_width = 368.f;
  float card_height = 188.f;
  float column_gap = 20.f;
  float row_gap = 22.f;
  int columns = 3;
  afterhours::Color background{17, 23, 34, 255};
  afterhours::Color header_background{31, 42, 59, 255};
  afterhours::Color card_background{27, 36, 51, 255};
  afterhours::Color ink{236, 241, 250, 255};
  afterhours::Color muted{171, 188, 212, 255};
  const char *background_debug_name = "gallery_bg";
  const char *title_debug_name = "gallery_title";
};

// The built page: `root` is the full-screen background, `page` is the
// centred column sections should be appended to, and `header` is the
// title band (already filled with title + subtitle).
struct GalleryPage {
  ElementResult root;
  ElementResult page;
  ElementResult header;
};

// Build the page scaffold: background, centred page column, header band.
//
// The page column is centred by deriving its horizontal padding from the
// live screen size, exactly like the hand-placed screens derived their
// `left` offset; the vertical padding adds the same centring offset above
// the caller's top inset. Sections appended to `page` flow below the
// header separated by `page_gap`.
inline GalleryPage gallery_page(UIContext<InputAction> &context,
                                afterhours::Entity &entity,
                                const std::string &title,
                                const std::string &subtitle,
                                const GalleryOptions &opts) {
  const float s = opts.scale;
  auto root = div(context, mk(entity),
                  ComponentConfig{}
                      .with_size({pixels(context.screen_width),
                                  pixels(context.screen_height)})
                      .with_custom_background(opts.background)
                      .with_corner_radius(0)
                      .with_debug_name(opts.background_debug_name));

  const float pad_x =
      (context.screen_width - opts.content_width * s) / 2.f;
  const float pad_top =
      (context.screen_height - opts.design_height * s) / 2.f +
      opts.top_inset * s;
  auto page = vstack(context, mk(root.ent(), 0),
                     ComponentConfig{}
                         .with_size({pixels(context.screen_width),
                                     pixels(context.screen_height)})
                         .with_padding(Padding{.top = pixels(pad_top),
                                               .left = pixels(pad_x),
                                               .bottom = pixels(0),
                                               .right = pixels(pad_x)})
                         .with_gap(pixels(opts.page_gap * s))
                         .with_skip_grid_snap()
                         .with_background(Theme::Usage::None)
                         .with_corner_radius(0)
                         .with_debug_name("gallery_page"));

  auto header = vstack(context, mk(page.ent(), 0),
                       ComponentConfig{}
                           .with_size({pixels(opts.content_width * s),
                                       pixels(opts.header_height * s)})
                           .with_custom_background(opts.header_background)
                           .with_corner_radius(12.f * s)
                           .with_padding(Padding{.top = pixels(10.f * s),
                                                 .left = pixels(20.f * s),
                                                 .bottom = pixels(0),
                                                 .right = pixels(20.f * s)})
                           .with_gap(pixels(3.f * s))
                           .with_skip_grid_snap()
                           .with_debug_name("gallery_header"));
  div(context, mk(header.ent(), 0),
      ComponentConfig{}
          .with_skip_grid_snap()
          .with_size({pixels((opts.content_width - 40.f) * s),
                      pixels(42.f * s)})
          .with_label(title)
          .with_font("AtkinsonMock", pixels(34.f * s))
          .with_custom_text_color(opts.ink)
          .with_alignment(TextAlignment::Left)
          .with_text_overflow(TextOverflow::Ellipsis)
          .with_ignore_pointer_events()
          .with_debug_name(opts.title_debug_name));
  div(context, mk(header.ent(), 1),
      ComponentConfig{}
          .with_skip_grid_snap()
          .with_size({pixels((opts.content_width - 40.f) * s),
                      pixels(28.f * s)})
          .with_label(subtitle)
          .with_font("AtkinsonMock", pixels(21.f * s))
          .with_custom_text_color(opts.muted)
          .with_alignment(TextAlignment::Left)
          .with_text_overflow(TextOverflow::Ellipsis)
          .with_ignore_pointer_events());

  return GalleryPage{root, page, header};
}

// Build a grid of specimen cards under `parent` and let `builder` fill
// each card body below its title.
//
// Builder beware: imm entity identity hashes the mk() call site, so
// elements built in a loop from one call site collide into a single
// entity. The template passes explicit sibling ids for everything it
// builds in a loop; builders must do the same (mk(card, 1), mk(body, 2),
// ...) for the elements they add.
//
// Cards flow in hstack rows of `opts.columns` inside a vstack, spaced by
// column_gap / row_gap -- the flow equivalent of the old
// `x = (i % 3) * 388, y = i < 3 ? 140 : 350` arithmetic, with the same
// resulting geometry. The builder receives the card entity and the
// specimen index; anything it adds becomes a card child, so containment
// checks (assert_within_parents) see the real hierarchy.
template <typename Builder>
inline void specimen_grid(UIContext<InputAction> &context,
                          afterhours::Entity &parent,
                          std::span<const Specimen> specimens,
                          Builder &&builder, const GalleryOptions &opts) {
  const float s = opts.scale;
  const int columns = opts.columns > 0 ? opts.columns : 1;
  const size_t rows =
      (specimens.size() + static_cast<size_t>(columns) - 1) /
      static_cast<size_t>(columns);
  const float grid_height =
      rows == 0 ? 0.f
                : static_cast<float>(rows) * opts.card_height +
                      static_cast<float>(rows - 1) * opts.row_gap;
  auto grid = vstack(context, mk(parent, 900),
                     ComponentConfig{}
                         .with_size({pixels(opts.content_width * s),
                                     pixels(grid_height * s)})
                         .with_gap(pixels(opts.row_gap * s))
                         .with_skip_grid_snap()
                         .with_background(Theme::Usage::None)
                         .with_corner_radius(0)
                         .with_debug_name("specimen_grid"));

  size_t index = 0;
  for (size_t r = 0; r < rows; ++r) {
    auto row = hstack(context, mk(grid.ent(), static_cast<int>(r)),
                      ComponentConfig{}
                          .with_size({pixels(opts.content_width * s),
                                      pixels(opts.card_height * s)})
                          .with_gap(pixels(opts.column_gap * s))
                          .with_background(Theme::Usage::None)
                          .with_corner_radius(0)
                          .with_no_wrap()
                          .with_skip_grid_snap()
                          .with_debug_name("specimen_row_" +
                                           std::to_string(r)));
    for (int c = 0; c < columns && index < specimens.size(); ++c, ++index) {
      const Specimen &specimen = specimens[index];
      auto card = vstack(context, mk(row.ent(), c),
                         ComponentConfig{}
                             .with_size({pixels(opts.card_width * s),
                                         pixels(opts.card_height * s)})
                             .with_custom_background(opts.card_background)
                             .with_corner_radius(10.f * s)
                             // Bottom padding is 0 on purpose: cards are
                             // fixed-height and each body sizes its own
                             // bottom inset, so containment checks should
                             // measure bodies against the full card height.
                             .with_padding(Padding{.top = pixels(12.f * s),
                                                   .left = pixels(16.f * s),
                                                   .bottom = pixels(0),
                                                   .right = pixels(16.f * s)})
                             .with_gap(pixels(14.f * s))
                             .with_skip_grid_snap()
                             .with_debug_name(specimen.debug_name));
      div(context, mk(card.ent(), 0),
          ComponentConfig{}
              .with_skip_grid_snap()
              .with_size({pixels((opts.card_width - 32.f) * s),
                          pixels(32.f * s)})
              .with_label(specimen.title)
              .with_font("AtkinsonMock", pixels(24.f * s))
              .with_custom_text_color(opts.ink)
              .with_alignment(TextAlignment::Left)
              .with_text_overflow(TextOverflow::Ellipsis)
              .with_ignore_pointer_events());
      builder(context, card.ent(), index);
    }
  }
}

} // namespace gallery
