#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleTabbing : ScreenSystem<UIContext<InputAction>> {
  std::array<int,4> button_clicks{};
  int last = -1;

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context, float) override {
    const afterhours::Color bg{18,12,28,255}, surface{35,28,52,255}, ink{240,235,250,255}, muted{195,181,215,255};
    const std::array<afterhours::Color,4> colors{{{165,55,140,255},{55,195,195,255},{145,215,65,255},{235,175,55,255}}};
    Theme theme;
    theme.background=bg; theme.surface=surface; theme.font=ink; theme.darkfont=bg;
    theme.primary=colors[0]; theme.secondary=colors[1]; theme.accent=colors[2]; theme.font_muted=muted;
    theme.focus={255,255,100,255}; theme.focus_ring_thickness=4; theme.focus_ring_offset=5;
    context.theme=theme;
    context.scaling_mode=ScalingMode::Proportional;
    const float s=std::min(context.screen_width/1280.f,context.screen_height/720.f);
    const auto box=[s](float x,float y,float w,float h) {
      return ComponentConfig{}.with_size({pixels(w*s),pixels(h*s)})
          .with_absolute_position(x*s,y*s).with_corner_radius(0);
    };
    div(context,mk(entity,0),ComponentConfig{}.with_size({pixels(context.screen_width),pixels(context.screen_height)})
        .with_custom_background(bg).with_corner_radius(0).with_debug_name("background"));
    auto panel=div(context,mk(entity,1),ComponentConfig{}.with_size({pixels(920*s),pixels(596*s)})
        .with_absolute_position((context.screen_width-920*s)/2,(context.screen_height-596*s)/2)
        .with_custom_background(surface).with_corner_radius(12*s).with_debug_name("main_panel"));
    const auto label=[&](int id,const std::string &text,float x,float y,float w,float h,float size,bool dim=false) {
      return div(context,mk(panel.ent(),id),box(x,y,w,h).with_label(text)
          .with_font("AtkinsonMock",pixels(size*s)).with_custom_text_color(dim?muted:ink)
          .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None).with_ignore_pointer_events());
    };
    label(0,"Keyboard navigation",32,20,856,44,32);
    label(1,"Tab moves forward. Shift + Tab moves back. Enter activates.",32,74,856,32,21,true);
    const std::array<std::string,4> names{"OPTION A","OPTION B","OPTION C","OPTION D"};
    for(int i=0;i<4;++i) {
      const float y=140+static_cast<float>(i)*78;
      label(10+i,std::to_string(i+1),32,y,32,55,23,true);
      if(button(context,mk(panel.ent(),20+i),box(80,y,340,55)
          .with_label(names[i]+(button_clicks[i]>0 ? " ("+std::to_string(button_clicks[i])+")" : ""))
          .with_custom_background(colors[i]).with_auto_text_color(true)
          .with_font("AtkinsonMock",pixels(23*s)).with_corner_radius(11*s)
          .with_debug_name("tab_button_"+std::to_string(i)))) {++button_clicks[i];last=i;}
    }
    label(30,"Follow the focus ring",476,132,412,36,25);
    label(31,"It appears after keyboard navigation begins.",476,180,412,56,20,true);
    label(32,"Four colors test the same yellow ring",476,246,412,32,19,true);
    label(33,"against different button backgrounds.",476,276,412,32,19,true);
    div(context,mk(panel.ent(),34),box(486,334,74,40).with_custom_background({0,0,0,0})
        .with_border(theme.focus,1).with_corner_radius(11*s).with_ignore_pointer_events());
    label(35,"Ring color / #FFFF64",584,326,304,30,19);
    label(36,"Requested stroke 4 px / offset 5 px",476,390,412,30,18,true);
    label(37,"Each activation increments the shared total.",476,432,412,32,18,true);
    const int total=button_clicks[0]+button_clicks[1]+button_clicks[2]+button_clicks[3];
    label(38,"Total activations: "+std::to_string(total),32,484,420,32,24);
    label(39,"Last activated: "+(last<0 ? std::string("none") : names[last]),32,524,420,30,20,true);
    if(button(context,mk(panel.ent(),40),box(660,486,228,42).with_label("Reset count")
        .with_font("AtkinsonMock",pixels(20*s)).with_background(Theme::Usage::Secondary)
        .with_auto_text_color(true).with_corner_radius(8*s).with_debug_name("tab_reset"))) {
      button_clicks.fill(0);last=-1;
    }
    label(41,"Tab order: A > B > C > D > Reset > A",476,550,412,28,18,true);
  }
};

REGISTER_EXAMPLE_SCREEN(tabbing, "System Demos",
                        "Keyboard tabbing navigation example", ExampleTabbing)
