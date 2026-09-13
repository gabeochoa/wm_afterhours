#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

struct ExampleText : ScreenSystem<UIContext<InputAction>> {
  bool colors_page=false, guides=false;

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context, float) override {
    const afterhours::Color bg{28,26,24,255}, panel{42,38,35,255}, gold{205,175,110,255};
    const afterhours::Color cream{250,245,235,255}, silver{210,210,215,255}, muted{210,205,200,255};
    Theme theme;theme.background=bg;theme.surface=panel;theme.font=cream;theme.darkfont=bg;theme.font_muted=muted;
    theme.primary=gold;context.theme=theme;context.scaling_mode=ScalingMode::Proportional;
    const float s=std::min(context.screen_width/1280.f,context.screen_height/720.f);
    const auto box=[s](float x,float y,float w,float h) {
      return ComponentConfig{}.with_size({pixels(w*s),pixels(h*s)})
          .with_absolute_position(x*s,y*s).with_corner_radius(0);
    };
    div(context,mk(entity,0),ComponentConfig{}.with_size({pixels(context.screen_width),pixels(context.screen_height)})
        .with_custom_background(bg).with_corner_radius(0));
    auto root=div(context,mk(entity,1),ComponentConfig{}.with_size({pixels(1160*s),pixels(656*s)})
        .with_absolute_position((context.screen_width-1160*s)/2,(context.screen_height-656*s)/2)
        .with_custom_background(panel).with_corner_radius(12*s).with_debug_name("main_panel"));
    const auto label=[&](int id,const std::string &text,float x,float y,float w,float h,float size,
                         const std::string &font,afterhours::Color color) {
      return div(context,mk(root.ent(),id),box(x,y,w,h).with_label(text).with_font(font,pixels(size*s))
          .with_custom_text_color(color).with_alignment(TextAlignment::Left)
          .with_background(Theme::Usage::None).with_ignore_pointer_events());
    };
    const auto meta=[&](int id,const std::string &text,float x,float y,float w,float h=28) {
      return label(id,text,x,y,w,h,19,"AtkinsonMock",muted);
    };
    label(0,"Typography Showcase",32,16,660,46,36,"Garamond",gold);
    for(int i=0;i<2;++i) {
      if(button(context,mk(root.ent(),1+i),box(32+static_cast<float>(i)*278,76,262,40)
          .with_label(i==0?"Size hierarchy":"Spacing and color").with_font("AtkinsonMock",pixels(20*s))
          .with_custom_background(colors_page==(i==1)?gold:afterhours::Color{64,57,49,255})
          .with_custom_text_color(colors_page==(i==1)?bg:cream).with_corner_radius(6*s)
          .with_debug_name("text_page_"+std::to_string(i)))) colors_page=i==1;
    }
    if(button(context,mk(root.ent(),3),box(850,76,278,40).with_label(guides?"Bounds guides: on":"Bounds guides: off")
        .with_font("AtkinsonMock",pixels(19*s)).with_custom_background({64,57,49,255})
        .with_custom_text_color(cream).with_debug_name("text_guides"))) guides=!guides;
    if(!colors_page) {
      meta(10,"Text Size Hierarchy",32,142,560);
      meta(11,"Family / size at 720p / weight / purpose",678,142,450);
      struct Sample {const char *text;const char *font;float size,y,h;const char *metadata;afterhours::Color color;};
      const std::array<Sample,5> samples{{
        {"Display Text","Garamond",48,182,62,"Garamond / 48 px / regular / display",cream},
        {"Headline Style","Garamond",36,262,50,"Garamond / 36 px / regular / headline",silver},
        {"Subheading Text","Garamond",28,330,42,"Garamond / 28 px / regular / subheading",silver},
        {"Aa Bb Cc 0123456789.,!? Mixed-case text wraps within this sample width.",UIComponent::DEFAULT_FONT,20,392,88,"Gaegu-Bold / 20 px / bold / body",muted},
        {"Caption & metadata - smaller supporting text",UIComponent::DEFAULT_FONT,14,504,30,"Gaegu-Bold / 14 px / bold / caption",muted}}};
      for(size_t i=0;i<samples.size();++i) {
        const auto &sample=samples[i];
        auto text=label(20+static_cast<int>(i)*3,sample.text,32,sample.y,600,sample.h,sample.size,sample.font,sample.color);
        if(i==3) text.ent().get<HasLabel>().text_overflow=TextOverflow::Wrap;
        meta(21+static_cast<int>(i)*3,sample.metadata,678,sample.y,450,38);
        if(guides) div(context,mk(root.ent(),22+static_cast<int>(i)*3),box(32,sample.y+sample.h,600,1)
            .with_custom_background({92,82,69,255}).with_ignore_pointer_events());
      }
      meta(40,"Body box: 600 x 88 px / native line spacing",678,442,450,38);
      meta(41,"Caption remains small; these notes stay readable.",678,546,450,46);
    } else {
      meta(50,"Letter spacing / same text, face and size",32,142,520);
      const std::array<float,3> tracking{-1,2,3};
      for(int i=0;i<3;++i) {
        const float y=196+static_cast<float>(i)*104;
        auto sample=label(60+i*3,"TRACKING SAMPLE",32,y,510,40,22,"Garamond",cream);
        sample.ent().get<HasLabel>().letter_spacing=tracking[i]*s;
        meta(61+i*3,fmt::format("Garamond 22 px / spacing {:+.0f} px",tracking[i]),32,y+44,510);
        if(guides) div(context,mk(root.ent(),62+i*3),box(32,y+40,510,1).with_custom_background({92,82,69,255}));
      }
      meta(70,"Secondary metadata context",32,526,510);
      label(71,"Last updated 12 September",32,564,510,32,20,"AtkinsonMock",muted);
      meta(72,"Color treatments / Garamond 24 px",606,142,522);
      const auto backing=afterhours::colors::darken(panel,0.85f);
      const std::array<afterhours::Color,5> colors{gold,cream,silver,muted,afterhours::Color{210,140,140,255}};
      const std::array<std::string,5> roles{"Primary / gold","Font / cream","Custom / silver","FontMuted / metadata","Custom / highlight"};
      for(int i=0;i<5;++i) {
        const float y=184+static_cast<float>(i)*82;
        div(context,mk(root.ent(),80+i*4),box(606,y,522,38).with_custom_background(backing).with_corner_radius(6*s));
        label(81+i*4,"Sample text Aa 123",620,y,490,38,24,"Garamond",colors[i]);
        const auto c=colors[i];
        meta(82+i*4,fmt::format("{} #{:02X}{:02X}{:02X} / {:.2f}:1",roles[i],c.r,c.g,c.b,afterhours::colors::contrast_ratio(c,backing)),606,y+42,522,30);
        div(context,mk(root.ent(),83+i*4),box(1098,y+10,20,20).with_custom_background(c));
      }
    }
    meta(110,fmt::format("Garamond + Gaegu-Bold specimens / Atkinson labels / {:.0f} x {:.0f} / scale {:.2f}",context.screen_width,context.screen_height,s),32,614,1096,28);
  }
};

REGISTER_EXAMPLE_SCREEN(text, "System Demos",
                        "Text rendering examples with different sizes", ExampleText)
