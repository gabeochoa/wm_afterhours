#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <array>

struct ActiveTabStyles : ScreenSystem<afterhours::ui::UIContext<InputAction>> {
  size_t selected = 1;

  void for_each_with(afterhours::Entity &entity, afterhours::ui::UIContext<InputAction> &ctx, float) override {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    const afterhours::Color paper{249, 247, 238, 255}, ink{32, 51, 51, 255},
        muted{68, 88, 85, 255}, green{25, 96, 78, 255}, wash{225, 231, 219, 255};
    ctx.theme.background = wash;
    ctx.theme.surface = paper;
    ctx.theme.font = ink;
    ctx.theme.font_muted = muted;
    ctx.theme.accent = green;
    ctx.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(ctx.screen_width / 1280.f, ctx.screen_height / 720.f);
    auto page = div(ctx, mk(entity), ComponentConfig{}.with_size({pixels(1280*s), pixels(720*s)})
        .with_absolute_position((ctx.screen_width-1280*s)/2, (ctx.screen_height-720*s)/2)
        .with_custom_background(paper).with_corner_radius(0));
    auto box = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_absolute_position(x*s,y*s).with_size({pixels(w*s),pixels(h*s)}).with_corner_radius(0);
    };
    auto text = [&](int id, float x, float y, float w, float h, float pt, const std::string &value, afterhours::Color color, bool bold=false) {
      div(ctx, mk(page.ent(),id), box(x,y,w,h).with_label(value).with_alignment(TextAlignment::Left)
          .with_font(bold ? "AtkinsonMockBold" : "AtkinsonMock",pixels(pt*s)).with_custom_text_color(color)
          .with_background(Theme::Usage::None).with_ignore_pointer_events());
    };
    text(1,72,42,950,70,52,"Active tab styles",ink,true);
    text(2,72,112,1120,36,24,"Select a tab in either row to compare its selected state.",muted);
    const std::array<std::string_view,3> tabs{"Headland", "Estuary", "Marsh"};
    auto bar = [&](float y, const char *name) {
      return box(72,y,1136,60).with_font("AtkinsonMock",pixels(25*s)).with_debug_name(name);
    };
    text(3,72,176,1100,36,24,"01   Default underline",ink,true);
    tab_container(ctx,mk(page.ent(),10),tabs,selected,bar(222,"default_tabs"));
    text(4,72,324,1100,36,24,"02   Field notebook",ink,true);
    tab_container(ctx,mk(page.ent(),11),tabs,selected,bar(370,"notebook_tabs"),
        ComponentConfig{}.with_custom_background(green).with_custom_text_color(paper)
          .with_corner_radius(12*s).with_rounded_corners(RoundedCorners().top_round())
          .with_border_top(ink,pixels(4*s)));
    div(ctx,mk(page.ent(),12),box(72,430,1136,188).with_custom_background(wash).with_ignore_pointer_events());
    const std::array<std::string_view,3> titles{"Wind above the cliffs", "Where the tides meet", "Reeds at low water"};
    const std::array<std::string_view,3> notes{"Fulmars circle the exposed face of the headland.","Fresh water meets the incoming tide along the salt line.","A narrow channel winds between the reed beds."};
    text(5,104,454,1050,28,18,"COASTAL SURVEY   /   FIELD NOTE 0" + std::to_string(selected+1),green,true);
    text(6,104,491,1060,54,39,std::string(titles[selected]),ink,true);
    text(7,104,554,1060,38,23,std::string(notes[selected]),muted);
  }
};

REGISTER_EXAMPLE_SCREEN(active_tab_styles, "Component Galleries", "Default and caller-styled active tabs", ActiveTabStyles)
