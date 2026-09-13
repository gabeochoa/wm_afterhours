#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct SettingRowShowcase : ScreenSystem<UIContext<InputAction>> {
  bool music_enabled=true, sound_effects=true, vibration=false;
  size_t language_idx=0, quality_idx=2;
  std::vector<std::string> languages{"English","Spanish","French","German","Japanese"};
  std::vector<std::string> quality_options{"Low","Medium","High","Ultra"};
  float master_volume=0.8f, music_volume=0.65f;

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context, float) override {
    context.theme=afterhours::ui::theme_presets::neon_dark();
    context.theme.primary={85,175,125,255};context.theme.accent={65,155,220,255};context.theme.secondary={45,48,58,255};
    context.scaling_mode=ScalingMode::Proportional;
    const float s=std::min(context.screen_width/1280.f,context.screen_height/720.f);
    const afterhours::Color ink{234,241,249,255},muted{175,190,209,255};
    const auto box=[s](float x,float y,float w,float h) {
      return ComponentConfig{}.with_size({pixels(w*s),pixels(h*s)})
          .with_absolute_position(x*s,y*s).with_corner_radius(0);
    };
    div(context,mk(entity,0),ComponentConfig{}.with_size({pixels(context.screen_width),pixels(context.screen_height)})
        .with_background(Theme::Usage::Background).with_corner_radius(0));
    auto root=div(context,mk(entity,1),ComponentConfig{}.with_size({pixels(1080*s),pixels(680*s)})
        .with_absolute_position((context.screen_width-1080*s)/2,(context.screen_height-680*s)/2)
        .with_background(Theme::Usage::Surface).with_corner_radius(12*s).with_debug_name("root"));
    const auto label=[&](int id,const std::string &text,float x,float y,float w,float h,float size,bool dim=false) {
      return div(context,mk(root.ent(),id),box(x,y,w,h).with_label(text)
          .with_font("AtkinsonMock",pixels(size*s)).with_custom_text_color(dim?muted:ink)
          .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None).with_ignore_pointer_events());
    };
    label(0,"Setting Row Showcase",32,16,1016,44,32);
    label(1,"Three native control families / one shared label and control grid",32,68,1016,28,20,true);
    label(2,"TOGGLES",32,108,360,26,19,true);
    const auto row_config=[&](const std::string &text,SettingRowControlType type,const std::string &name,float width) {
      SettingRowConfig config;
      config.label=text;config.control_type=type;config.row_height=48*s;config.row_spacing=0;
      config.slot_label_config=box(0,0,384,48).with_font("AtkinsonMock",pixels(22*s));
      config.slot_control_config=box(400,2,width,44).with_font("AtkinsonMock",pixels(21*s))
          .with_debug_name(name).with_corner_radius(8*s);
      if(type==SettingRowControlType::Stepper)
        config.slot_control_config->with_custom_background({31,43,61,255}).with_border({76,92,110,255},1);
      return config;
    };
    const auto toggle=[&](int id,const std::string &text,bool &value,float y,const std::string &name) {
      setting_row(context,mk(root.ent(),id),row_config(text,SettingRowControlType::Toggle,name,72),&value,
          box(32,y,1016,48).with_font("AtkinsonMock",pixels(22*s)).with_background(Theme::Usage::None));
      label(id+100,value?"On":"Off",540,y,120,48,21);
      div(context,mk(root.ent(),id+200),box(32,y+50,1016,1).with_custom_background({51,65,84,255}));
    };
    toggle(10,"Music",music_enabled,140,"setting_music");
    toggle(11,"Sound Effects",sound_effects,194,"setting_sound");
    toggle(12,"Vibration",vibration,248,"setting_vibration");
    label(3,"OPTIONS / Language: 5 choices / Graphics quality: 4 choices",32,314,1016,26,19,true);
    const auto style_options=[&](ElementResult row,const std::string &name) {
      for(auto id:row.cmp().children) {
        auto &control=UICollectionHolder::getEntityForIDEnforce(id);
        if(!control.has<HasStepperState>()) continue;
        int direction=0;
        for(auto arrow_id:control.get<UIComponent>().children) {
          auto &arrow=UICollectionHolder::getEntityForIDEnforce(arrow_id);
          if(!arrow.has<HasClickListener>()) continue;
          const bool previous=direction++==0;
          arrow.get<UIComponent>().set_desired_width(pixels(44*s));
          arrow.get<HasLabel>().label.clear();
          arrow.addComponentIfMissing<UIComponentDebug>(name).set(name+(previous?"_previous":"_next"));
          arrow.addComponentIfMissing<afterhours::HasColor>(afterhours::Color{47,64,86,255}).set({47,64,86,255});
          arrow.addComponentIfMissing<HasOnDraw>().fg=[s,previous,ink](RectangleType r) {
            const float dir=previous?-1.f:1.f,cx=r.x+r.width/2,cy=r.y+r.height/2;
            afterhours::draw_line_ex({cx-dir*4*s,cy-8*s},{cx+dir*4*s,cy},2*s,ink);
            afterhours::draw_line_ex({cx+dir*4*s,cy},{cx-dir*4*s,cy+8*s},2*s,ink);
          };
        }
      }
    };
    style_options(setting_row(context,mk(root.ent(),20),row_config("Language",SettingRowControlType::Stepper,"setting_language",380),
        std::make_pair(&language_idx,languages),box(32,348,1016,48).with_font("AtkinsonMock",pixels(22*s))),"setting_language");
    style_options(setting_row(context,mk(root.ent(),21),row_config("Graphics quality",SettingRowControlType::Stepper,"setting_quality",380),
        std::make_pair(&quality_idx,quality_options),box(32,402,1016,48).with_font("AtkinsonMock",pixels(22*s))),"setting_quality");
    label(4,"VOLUME / 0% to 100%",32,466,1016,26,19,true);
    const auto volume=[&](int id,const std::string &title,float &value,float y,const std::string &name) {
      label(id,title,32,y,384,44,22);
      auto control=slider(context,mk(root.ent(),id+1),value,box(432,y,386,36)
          .with_custom_background({0,0,0,0}).with_debug_name(name),SliderHandleValueLabelPosition::None);
      for(auto child_id:control.cmp().children) {
        auto &child=UICollectionHolder::getEntityForIDEnforce(child_id);
        if(!child.has<HasSliderState>()) continue;
        child.addComponentIfMissing<HasOnDraw>().fg=[s,value](RectangleType r) {
          const float cy=r.y+r.height/2;
          afterhours::draw_rectangle_rounded({r.x,cy-3*s,r.width,6*s},1,8,{78,96,119,255},RoundedCorners());
          if(value>0) afterhours::draw_rectangle_rounded({r.x,cy-3*s,r.width*value,6*s},1,8,{85,175,125,255},RoundedCorners());
          afterhours::draw_circle(static_cast<int>(r.x+value*r.width),static_cast<int>(cy),9*s,{234,241,249,255});
        };
      }
      label(id+2,fmt::format("{:.0f}%",value*100),850,y,166,44,23);
    };
    volume(30,"Master Volume",master_volume,502,"master_vol_slider");
    volume(40,"Music Volume",music_volume,554,"music_vol_slider");
    label(5,fmt::format("Music preview: {:.0f}% = master x music; Music off mutes it.",music_enabled?master_volume*music_volume*100:0),32,610,1016,26,18,true);
    label(6,"Defaults: On / On / Off; English; High; Master 80%; Music 65%. Session-local preview.",32,644,1016,26,17,true);
  }
};

REGISTER_EXAMPLE_SCREEN(setting_row_showcase, "Component Galleries",
                        "Setting Row - labeled controls for settings screens", SettingRowShowcase)
