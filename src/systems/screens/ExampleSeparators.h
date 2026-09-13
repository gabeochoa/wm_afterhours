#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleSeparators : ScreenSystem<UIContext<InputAction>> {
  bool guides = true;

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context, float) override {
    context.theme=afterhours::ui::theme_presets::cozy_kraft();
    context.scaling_mode=ScalingMode::Proportional;
    const float s=std::min(context.screen_width/1280.f,context.screen_height/720.f);
    const afterhours::Color ink{30,41,59,255}, muted{82,99,122,255}, line{180,190,210,255};
    const std::array<afterhours::Color,4> colors{{{59,130,246,255},{34,197,94,255},{139,92,246,255},{249,115,22,255}}};
    const auto box=[s](float x,float y,float w,float h) {
      return ComponentConfig{}.with_size({pixels(w*s),pixels(h*s)})
          .with_absolute_position(x*s,y*s).with_corner_radius(0);
    };
    div(context,mk(entity,0),ComponentConfig{}.with_size({pixels(context.screen_width),pixels(context.screen_height)})
        .with_custom_background({248,250,252,255}).with_corner_radius(0));
    auto root=div(context,mk(entity,1),ComponentConfig{}.with_size({pixels(1080*s),pixels(672*s)})
        .with_absolute_position((context.screen_width-1080*s)/2,(context.screen_height-672*s)/2)
        .with_background(Theme::Usage::None).with_debug_name("separators_root"));
    const auto label=[&](int id,const std::string &text,float x,float y,float w,float h,float size,bool dim=false) {
      return div(context,mk(root.ent(),id),box(x,y,w,h).with_label(text)
          .with_font("AtkinsonMock",pixels(size*s)).with_custom_text_color(dim?muted:ink)
          .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None).with_ignore_pointer_events());
    };
    const auto stroke=[&](int id,float x,float y,float w,float h,afterhours::Color color,const std::string &name="") {
      return div(context,mk(root.ent(),id),box(x,y,w,h).with_custom_background(color).with_debug_name(name).with_ignore_pointer_events());
    };
    label(0,"Separators",0,0,800,40,32);
    label(1,"Compare thickness, width, labels and color. Dimensions are shown at 720p.",0,44,1080,28,19,true);
    for(int i=0;i<2;++i) div(context,mk(root.ent(),2+i),box(static_cast<float>(i)*560,88,520,526)
        .with_custom_background({255,255,255,255}).with_corner_radius(12*s)
        .with_debug_name(i==0?"left_card_bg":"right_card_bg"));
    label(10,"Horizontal specimens",32,106,456,34,25);
    label(11,"Basic / 456 x 3 px / #B4BED2",32,150,456,28,19,true);
    stroke(12,32,188,456,3,line,"separator_basic");
    label(13,"Thick / 456 x 6 px",32,216,456,28,19,true);
    stroke(14,32,254,456,6,line,"separator_thick");
    if(guides) stroke(15,498,254,2,6,ink);
    label(16,"Blue / 456 x 3 px / #3B82F6",32,282,456,28,19,true);
    stroke(17,32,320,456,3,colors[0]);
    label(18,"Purple / 60% of 456 px / centered",32,348,456,28,19,true);
    if(guides) stroke(19,32,386,456,1,{220,226,235,255});
    stroke(20,123.2f,386,273.6f,3,colors[2],"separator_partial");
    label(21,"Labeled specimens",32,412,456,32,25);
    stroke(22,32,468,198,3,line);
    label(23,"OR",235,454,50,28,19);
    stroke(24,290,468,198,3,line);
    label(25,"Fixed 60 px label gap",32,486,456,26,17,true);
    stroke(26,32,540,158,3,colors[0],"settings_line1");
    label(27,"Settings",205,526,110,30,21);
    stroke(28,330,540,158,3,colors[0]);
    label(29,"Long label / fixed 140 px gap / blue accent",32,568,456,26,17,true);
    label(30,"Mixed separator examples",592,106,456,34,25);
    label(31,"Static navigation / 3 x 40 px dividers",592,150,456,28,19,true);
    div(context,mk(root.ent(),32),box(592,188,456,64).with_custom_background({248,250,252,255})
        .with_corner_radius(8*s).with_debug_name("nav_bg"));
    const std::array<std::string,4> names{"Home","Settings","Profile","Help"};
    const std::array<afterhours::Color,3> nav_colors{muted,colors[1],colors[3]};
    for(int i=0;i<4;++i) {
      label(40+i,names[i],604+static_cast<float>(i)*110,204,86,30,19);
      if(i<3) stroke(50+i,697+static_cast<float>(i)*110,200,3,40,nav_colors[i],"vsep_"+std::to_string(i));
    }
    label(54,"Slate / Green / Orange",592,258,456,26,17,true);
    label(55,"Color gallery",592,292,456,32,25);
    label(56,"Same full-width line / 346 x 4 px",592,326,456,26,17,true);
    const std::array<std::string,4> captions{"Blue #3B82F6","Green #22C55E","Purple #8B5CF6","Orange #F97316"};
    for(int i=0;i<4;++i) {
      const float y=362+static_cast<float>(i)*56;
      label(60+i,captions[i],592,y,456,26,18,true);
      stroke(70+i,592,y+34,346,4,colors[i],"colorbar_"+std::array<std::string,4>{"Blue","Green","Purple","Orange"}[i]);
    }
    label(80,"Grouping example: Contact details   |   Delivery address",0,632,790,28,19,true);
    if(button(context,mk(root.ent(),81),box(816,630,264,36).with_label(guides?"Width guides: on":"Width guides: off")
        .with_font("AtkinsonMock",pixels(18*s)).with_custom_background({227,235,246,255})
        .with_custom_text_color(ink).with_corner_radius(8*s).with_debug_name("separator_guides"))) guides=!guides;
  }
};

REGISTER_EXAMPLE_SCREEN(separators, "Component Galleries",
                        "Separator widget examples (horizontal, vertical, labeled)", ExampleSeparators)
