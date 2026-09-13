#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../settings.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct SelfAlignShowcase : ScreenSystem<UIContext<InputAction>> {
  bool guides = true;
  std::string last = "No specimen activated";

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::sage_natural();
    context.theme.background = {35, 40, 45, 255};
    context.theme.surface = {50, 55, 60, 255};
    context.scaling_mode = ScalingMode::Proportional;
    const float s = std::min(context.screen_width / 1280.f, context.screen_height / 720.f);
    const afterhours::Color ink{236, 241, 236, 255}, muted{187, 203, 193, 255};
    const auto box = [s](float x, float y, float w, float h) {
      return ComponentConfig{}.with_size({pixels(w*s), pixels(h*s)})
          .with_absolute_position(x*s, y*s).with_corner_radius(0);
    };
    div(context, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(context.screen_width), pixels(context.screen_height)})
        .with_background(Theme::Usage::Background).with_corner_radius(0));
    auto root = div(context, mk(entity, 1), ComponentConfig{}
        .with_size({pixels(1080*s), pixels(584*s)})
        .with_absolute_position((context.screen_width-1080*s)/2, (context.screen_height-584*s)/2)
        .with_background(Theme::Usage::None).with_debug_name("self_align_root"));
    const auto label = [&](int id, const std::string &text, float x, float y, float w, float h, float size, bool dim=false) {
      return div(context, mk(root.ent(), id), box(x,y,w,h).with_label(text)
          .with_font("AtkinsonMock", pixels(size*s)).with_custom_text_color(dim ? muted : ink)
          .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None).with_ignore_pointer_events());
    };
    label(0, "SelfAlign: child alignment overrides", 0,0,1080,44,32);
    label(1, "The parent stays a column; each child chooses its horizontal alignment.", 0,52,1080,32,21,true);
    label(2, "Column parent / align-items: FlexStart", 48,104,624,30,23);
    label(3, "Cross axis  <-------------------->", 48,142,600,28,18,true);
    auto demo = vstack(context, mk(root.ent(), 4), box(48,184,600,224)
        .with_custom_background({45,50,55,255}).with_border({100,110,120,255},2)
        .with_padding(Padding{.left=pixels(8*s),.right=pixels(8*s)})
        .with_justify_content(JustifyContent::SpaceAround).with_debug_name("demo"));
    const std::array<SelfAlign,3> values{SelfAlign::FlexStart,SelfAlign::Center,SelfAlign::FlexEnd};
    const std::array<std::string,3> names{"FlexStart","Center","FlexEnd"};
    const std::array<Theme::Usage,3> colors{Theme::Usage::Primary,Theme::Usage::Secondary,Theme::Usage::Accent};
    for (int i=0;i<3;++i) {
      if (button(context,mk(demo.ent(),i),ComponentConfig{}
          .with_label(names[i]).with_size({pixels(184*s),pixels(48*s)})
          .with_self_align(values[i]).with_background(colors[i]).with_auto_text_color(true)
          .with_font("AtkinsonMock",pixels(20*s)).with_corner_radius(8*s)
          .with_debug_name("self_align_"+std::to_string(i)))) last=names[i]+" activated";
    }
    if (guides) {
      div(context,mk(root.ent(),10),box(348,184,1,224).with_custom_background({170,195,183,75}).with_ignore_pointer_events());
      div(context,mk(root.ent(),11),box(640,380,16,2).with_custom_background(muted).with_ignore_pointer_events());
      div(context,mk(root.ent(),12),box(30,184,2,224).with_custom_background(muted).with_ignore_pointer_events());
      label(13,"v",20,384,20,24,18,true);
    }
    label(14,"Main axis: top to bottom",48,420,600,28,19,true);
    label(15,"8 px left and right padding / 184 x 48 px children",48,454,600,28,18,true);
    label(16,"Parent default",712,104,360,30,23);
    label(17,"SelfAlign::Auto inherits FlexStart",712,142,360,28,18,true);
    auto comparison=vstack(context,mk(root.ent(),18),box(712,184,360,90)
        .with_background(Theme::Usage::Surface).with_padding(Padding{.left=pixels(8*s),.right=pixels(8*s)})
        .with_justify_content(JustifyContent::Center).with_debug_name("self_align_default"));
    div(context,mk(comparison.ent(),0),ComponentConfig{}.with_size({pixels(184*s),pixels(48*s)})
        .with_label("Auto / inherited").with_font("AtkinsonMock",pixels(19*s))
        .with_background(Theme::Usage::Primary).with_auto_text_color(true).with_corner_radius(8*s));
    label(19,"Value                   Position",712,302,360,30,20);
    label(20,"FlexStart              Left + inset",712,340,360,30,20,true);
    label(21,"Center                  Center",712,378,360,30,20,true);
    label(22,"FlexEnd                 Right - inset",712,416,360,30,20,true);
    label(23,last,48,506,600,32,21);
    if (button(context,mk(root.ent(),24),box(712,506,360,40).with_label(guides ? "Alignment guides: on" : "Alignment guides: off")
        .with_font("AtkinsonMock",pixels(19*s)).with_background(Theme::Usage::Secondary)
        .with_auto_text_color(true).with_debug_name("self_align_guides"))) guides=!guides;
    label(25,"Specimen dimensions are shown at 720p. Native SelfAlign computes every child position.",48,552,1024,28,18,true);
  }
};

REGISTER_EXAMPLE_SCREEN(self_align, "System Demos",
                        "SelfAlign feature for centering content",
                        SelfAlignShowcase)
