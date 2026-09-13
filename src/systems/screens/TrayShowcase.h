#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct TrayShowcase : ScreenSystem<UIContext<InputAction>> {
  std::array<int,3> h_clicks{},v_clicks{};
  std::string last="none";

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context, float) override {
    context.theme=afterhours::ui::theme_presets::neon_dark();
    context.theme.focus={255,255,100,255};context.theme.focus_ring_thickness=4;context.theme.focus_ring_offset=5;
    context.scaling_mode=ScalingMode::Proportional;
    const float s=std::min(context.screen_width/1280.f,context.screen_height/720.f);
    const afterhours::Color ink{235,241,250,255},muted{176,190,211,255};
    const auto box=[s](float x,float y,float w,float h) {
      return ComponentConfig{}.with_size({pixels(w*s),pixels(h*s)})
          .with_absolute_position(x*s,y*s).with_corner_radius(0);
    };
    div(context,mk(entity,0),ComponentConfig{}.with_size({pixels(context.screen_width),pixels(context.screen_height)})
        .with_background(Theme::Usage::Background).with_corner_radius(0).with_debug_name("tray_bg"));
    auto root=div(context,mk(entity,1),ComponentConfig{}.with_size({pixels(1000*s),pixels(596*s)})
        .with_absolute_position((context.screen_width-1000*s)/2,(context.screen_height-596*s)/2)
        .with_background(Theme::Usage::Surface).with_corner_radius(12*s).with_debug_name("card"));
    const auto label=[&](int id,const std::string &text,float x,float y,float w,float h,float size,bool dim=false) {
      return div(context,mk(root.ent(),id),box(x,y,w,h).with_label(text)
          .with_font("AtkinsonMock",pixels(size*s)).with_custom_text_color(dim?muted:ink)
          .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None).with_ignore_pointer_events());
    };
    label(0,"Tray Navigation",32,20,936,44,32);
    label(1,"One Tab stop per group. Arrow keys choose an item; Enter activates it.",32,74,936,32,21,true);
    label(2,"Horizontal group / Left + Right",32,126,570,32,23);
    label(3,"Vertical group / Up + Down",644,126,324,32,23);
    div(context,mk(root.ent(),4),box(20,168,600,102).with_border({72,88,109,255},1).with_custom_background({0,0,0,0}).with_corner_radius(8*s));
    div(context,mk(root.ent(),5),box(632,168,348,234).with_border({72,88,109,255},1).with_custom_background({0,0,0,0}).with_corner_radius(8*s));
    auto horizontal=tray(context,mk(root.ent(),6),box(32,184,576,70)
        .with_flex_direction(FlexDirection::Row).with_no_wrap().with_justify_content(JustifyContent::SpaceAround)
        .with_align_items(AlignItems::Center).with_gap(pixels(10*s)).with_debug_name("h_tray"));
    auto vertical=tray(context,mk(root.ent(),7),box(644,184,324,202)
        .with_flex_direction(FlexDirection::Column).with_no_wrap().with_gap(pixels(14*s)).with_debug_name("v_tray"));
    const std::array<std::string,3> names{"Alpha","Beta","Gamma"};
    const std::array<afterhours::Color,3> colors{{{165,55,140,255},{55,195,195,255},{145,215,65,255}}};
    for(int i=0;i<3;++i) {
      const auto config=[&](int count,const std::string &debug,bool h) {
        return ComponentConfig{}.with_size({percent(h?0.3f:1.f),pixels(58*s)})
            .with_label(names[i]+(count>0?" ("+std::to_string(count)+")":""))
            .with_custom_background(colors[i]).with_auto_text_color(true)
            .with_font("AtkinsonMock",pixels(21*s)).with_corner_radius(8*s).with_debug_name(debug);
      };
      if(button(context,mk(horizontal.ent(),i),config(h_clicks[i],"h_btn_"+std::to_string(i),true))) {
        ++h_clicks[i];last="Horizontal / "+names[i];
      }
      if(button(context,mk(vertical.ent(),i),config(v_clicks[i],"v_btn_"+std::to_string(i),false))) {
        ++v_clicks[i];last="Vertical / "+names[i];
      }
    }
    label(10,"The same colors identify the same choices.",32,292,576,30,19,true);
    label(11,"Arrows wrap from Gamma to Alpha and back.",32,334,576,30,19,true);
    label(12,"Tab order: Horizontal > Vertical > Reset > Horizontal",32,376,576,48,19,true);
    const std::string active=context.has_focus(horizontal.ent().id)?"Horizontal":context.has_focus(vertical.ent().id)?"Vertical":"Outside the trays";
    label(13,"Active tray: "+active,32,450,580,32,22);
    label(14,"Last activated: "+last,32,492,580,32,22);
    if(button(context,mk(root.ent(),15),box(680,460,288,44).with_label("Reset demo")
        .with_font("AtkinsonMock",pixels(21*s)).with_background(Theme::Usage::Secondary)
        .with_auto_text_color(true).with_corner_radius(8*s).with_debug_name("tray_reset"))) {
      h_clicks.fill(0);v_clicks.fill(0);last="none";
      horizontal.ent().get<HasTray>().selection_index=0;
      vertical.ent().get<HasTray>().selection_index=0;
    }
    label(16,"Shift + Tab reverses the group order. Pointer clicks activate a choice directly.",32,546,936,30,19,true);
  }
};

REGISTER_EXAMPLE_SCREEN(tray, "System Demos",
                        "Tray navigation — single tab stop with arrow key selection among children", TrayShowcase)
