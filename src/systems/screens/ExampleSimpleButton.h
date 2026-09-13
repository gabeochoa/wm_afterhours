#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleSimpleButton : ScreenSystem<UIContext<InputAction>> {
  int button_click_count = 0;
  std::string last = "None";

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context, float) override {
    context.theme = afterhours::ui::theme_presets::cozy_kraft();
    context.scaling_mode = ScalingMode::Proportional;
    const float s=std::min(context.screen_width/1280.f,context.screen_height/720.f);
    const afterhours::Color ink{55,50,60,255}, muted{99,87,105,255}, coral{255,115,105,255};
    const auto box=[s](float x,float y,float w,float h) {
      return ComponentConfig{}.with_size({pixels(w*s),pixels(h*s)})
          .with_absolute_position(x*s,y*s).with_corner_radius(0);
    };
    div(context,mk(entity,0),ComponentConfig{}.with_size({pixels(context.screen_width),pixels(context.screen_height)})
        .with_custom_background({255,245,235,255}).with_corner_radius(0).with_debug_name("bg"));
    auto card=div(context,mk(entity,1),ComponentConfig{}.with_size({pixels(720*s),pixels(620*s)})
        .with_absolute_position((context.screen_width-720*s)/2,(context.screen_height-620*s)/2)
        .with_custom_background({255,255,255,255}).with_corner_radius(12*s)
        .with_soft_shadow(4*s,8*s,20*s,{80,60,100,30}).with_debug_name("card"));
    const auto label=[&](int id,const std::string &text,float x,float y,float w,float h,float size,bool dim=false,const std::string &debug="") {
      return div(context,mk(card.ent(),id),box(x,y,w,h).with_label(text).with_debug_name(debug)
          .with_font("AtkinsonMock",pixels(size*s)).with_custom_text_color(dim?muted:ink)
          .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None).with_ignore_pointer_events());
    };
    label(0,"Simple Button Demo",32,20,656,44,32);
    label(1,"Every button adds one click. Reset clears the shared result.",32,70,656,30,20,true);
    const auto action=[&](int id,const std::string &name,const std::string &debug,float x,float y,float w,float h,
                          afterhours::Color color,float radius,bool outlined=false) {
      auto config=box(x,y,w,h).with_label(name).with_custom_background(color)
          .with_font("AtkinsonMock",pixels(22*s)).with_custom_text_color(ink)
          .with_corner_radius(radius*s).with_debug_name(debug);
      if(outlined) config.with_border({106,91,119,255},1);
      if(button(context,mk(card.ent(),id),config)) {++button_click_count;last=name;}
    };
    action(10,"Add a click","simple_increment",32,116,656,56,coral,14);
    label(11,"Primary / coral #FF7369 / height 56 px / radius 14 px",32,180,656,28,17,true);
    label(12,"Result",32,224,140,26,18,true);
    label(13,"Click count: "+std::to_string(button_click_count),32,254,340,36,27,false,"counter_bg");
    label(14,"Last activated: "+last,32,298,480,26,18,true);
    if(button(context,mk(card.ent(),15),box(512,252,176,40).with_label("Reset count")
        .with_font("AtkinsonMock",pixels(19*s)).with_custom_background({244,240,246,255})
        .with_custom_text_color(ink).with_corner_radius(8*s).with_debug_name("simple_reset"))) {
      button_click_count=0;last="None";
    }
    div(context,mk(card.ent(),16),box(32,338,656,1).with_debug_name("divider").with_custom_background({218,208,221,255}));
    label(17,"More styles",32,352,656,34,25);
    action(20,"Secondary","simple_secondary",32,398,316,44,{100,180,200,255},9);
    action(21,"Outline","simple_outline",372,398,316,44,{255,255,255,255},9,true);
    label(22,"Filled / #64B4C8 / 44 px",32,450,316,26,17,true);
    label(23,"Outline / #6A5B77 / 44 px",372,450,316,26,17,true);
    action(24,"Small","simple_small",32,492,200,36,{170,235,200,255},9);
    action(25,"Pill","simple_pill",260,492,200,36,{255,180,190,255},18);
    action(26,"Buttons","simple_gold",488,492,200,36,{255,215,140,255},9);
    label(27,"Mint / #AAEBC8",32,536,200,26,17,true);
    label(28,"Pink / radius 18 px",260,536,200,26,17,true);
    label(29,"Gold / #FFD78C",488,536,200,26,17,true);
    label(30,"Small controls: 36 px high. Tab + Enter activates the focused button.",32,578,656,26,18,true);
  }
};

REGISTER_EXAMPLE_SCREEN(simple_button, "Tools",
                        "Simple button example with click counter",
                        ExampleSimpleButton)
