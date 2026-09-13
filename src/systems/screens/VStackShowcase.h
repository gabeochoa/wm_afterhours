#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../../theme_presets.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/ah.h>

using namespace afterhours::ui;
using namespace afterhours::ui::imm;

// Demonstrates the vstack() primitive.
// Default: percent(1.0) width, children() height -- fills parent, shrinks to
// content.
struct VStackShowcase : ScreenSystem<UIContext<InputAction>> {
  int page=0;
  std::string selected="Home";

  void for_each_with(afterhours::Entity &entity, UIContext<InputAction> &context, float) override {
    context.theme=afterhours::ui::theme_presets::neon_dark();context.scaling_mode=ScalingMode::Proportional;
    const float s=std::min(context.screen_width/1280.f,context.screen_height/720.f);
    const afterhours::Color ink{235,241,249,255},muted{179,193,213,255},panel{40,48,70,255};
    const std::array<afterhours::Color,3> colors{{{0,180,220,255},{220,60,120,255},{50,210,130,255}}};
    const auto box=[s](float x,float y,float w,float h) {
      return ComponentConfig{}.with_size({pixels(w*s),pixels(h*s)})
          .with_absolute_position(x*s,y*s).with_corner_radius(0);
    };
    div(context,mk(entity,0),ComponentConfig{}.with_size({pixels(context.screen_width),pixels(context.screen_height)})
        .with_background(Theme::Usage::Background).with_corner_radius(0));
    auto root=div(context,mk(entity,1),ComponentConfig{}.with_size({pixels(1160*s),pixels(664*s)})
        .with_absolute_position((context.screen_width-1160*s)/2,(context.screen_height-664*s)/2)
        .with_background(Theme::Usage::None).with_debug_name("root"));
    const auto label=[&](int id,const std::string &text,float x,float y,float w,float h,float size,bool dim=false) {
      return div(context,mk(root.ent(),id),box(x,y,w,h).with_label(text)
          .with_font("AtkinsonMock",pixels(size*s)).with_custom_text_color(dim?muted:ink)
          .with_alignment(TextAlignment::Left).with_background(Theme::Usage::None).with_ignore_pointer_events());
    };
    label(0,"vstack() Showcase",12,8,1136,44,32);
    label(1,"A column stacks children vertically. Compare spacing, alignment and an applied layout.",12,60,1136,32,21,true);
    const std::array<std::string,3> tabs{"Main-axis spacing","Cross-axis alignment","Sidebar layout"};
    for(int i=0;i<3;++i) if(button(context,mk(root.ent(),2+i),box(12+static_cast<float>(i)*382,104,366,40)
        .with_label(tabs[i]).with_font("AtkinsonMock",pixels(20*s)).with_background(page==i?Theme::Usage::Primary:Theme::Usage::Surface)
        .with_auto_text_color(true).with_corner_radius(6*s).with_debug_name("vstack_page_"+std::to_string(i)))) page=i;
    if(page==0) {
      const std::array<JustifyContent,5> modes{JustifyContent::FlexStart,JustifyContent::Center,JustifyContent::FlexEnd,JustifyContent::SpaceBetween,JustifyContent::SpaceAround};
      const std::array<std::string,5> names{"Start","Center","End","Space between","Space around"};
      const std::array<std::string,5> api{"FlexStart","Center","FlexEnd","SpaceBetween","SpaceAround"};
      for(int j=0;j<5;++j) {
        const float x=24+static_cast<float>(j)*228;
        label(10+j*5,names[j],x,168,204,28,22);
        auto demo=vstack(context,mk(root.ent(),11+j*5),box(x,208,204,300)
            .with_custom_background(panel).with_padding(Padding::all(pixels(8*s)))
            .with_justify_content(modes[j]).with_align_items(AlignItems::Center).with_debug_name("vstack_jc_"+std::to_string(j)));
        const std::array<float,3> heights{0.24f,0.18f,0.22f};
        for(int k=0;k<3;++k) div(context,mk(demo.ent(),k),ComponentConfig{}.with_size({percent(0.8f),percent(heights[k])})
            .with_custom_background(colors[k]).with_corner_radius(4*s).with_debug_name(fmt::format("vstack_jc_{}_{}",j,k)));
        label(12+j*5,api[j],x,518,204,28,18,true);
        if(demo.cmp().children.size()==3) {
          const auto &a=UICollectionHolder::getEntityForIDEnforce(demo.cmp().children[0]).get<UIComponent>();
          const auto &b=UICollectionHolder::getEntityForIDEnforce(demo.cmp().children[1]).get<UIComponent>();
          label(13+j*5,fmt::format("First gap: {:.0f} px",b.rect().y-a.rect().y-a.rect().height),x,552,204,28,17,true);
        }
      }
      div(context,mk(root.ent(),40),box(12,208,1,300).with_custom_background(muted));
      label(41,"v",4,480,20,28,20,true);
      label(42,"Heights: cyan 24%, pink 18%, green 22% of content. Width: 80%. Main axis points down.",12,596,1136,28,18,true);
      label(43,"SpaceAround: edge space is half an interior gap. No extra gap is configured.",12,630,1136,28,18,true);
    } else if(page==1) {
      label(50,"Cross axis: left <----------------------------------------------------> right",12,160,1136,28,20,true);
      const std::array<AlignItems,4> modes{AlignItems::FlexStart,AlignItems::Center,AlignItems::Stretch,AlignItems::FlexEnd};
      const std::array<std::string,4> names{"Start","Center","Stretch","End"};
      for(int a=0;a<4;++a) {
        const float x=24+static_cast<float>(a)*284;
        label(60+a*3,names[a],x,200,260,30,23);
        auto demo=vstack(context,mk(root.ent(),61+a*3),box(x,244,260,246)
            .with_custom_background(panel).with_padding(Padding::all(pixels(8*s)))
            .with_justify_content(JustifyContent::SpaceAround).with_align_items(modes[a]).with_debug_name("vstack_ai_"+std::to_string(a)));
        const std::array<float,3> widths{0.85f,0.5f,0.7f};
        for(int k=0;k<3;++k) div(context,mk(demo.ent(),k),ComponentConfig{}.with_size({percent(widths[k]),percent(0.24f)})
            .with_custom_background(colors[k]).with_corner_radius(4*s));
        label(62+a*3,"Widths: 85% / 50% / 70%",x,502,260,30,18,true);
      }
      auto auto_width=vstack(context,mk(root.ent(),80),box(592,552,260,54).with_custom_background(panel)
          .with_padding(Padding::all(pixels(8*s))).with_align_items(AlignItems::Stretch));
      div(context,mk(auto_width.ent(),0),ComponentConfig{}.with_size({children(),pixels(30*s)})
          .with_label("Content width").with_font("AtkinsonMock",pixels(18*s)).with_background(Theme::Usage::Primary).with_auto_text_color(true));
      label(81,"Stretch currently positions children at Start.",12,552,556,30,19,true);
      label(82,"children() keeps content width; expand() fills.",12,588,556,30,19,true);
      label(83,"Same specimen colors across all modes. Each child is 24% of content height.",12,632,1136,28,18,true);
    } else {
      label(90,"Applied layout / sidebar + content",12,164,1136,32,25);
      auto layout=hstack(context,mk(root.ent(),91),box(24,212,1112,352));
      auto sidebar=vstack(context,mk(layout.ent(),0),ComponentConfig{}.with_size({percent(0.38f),percent(1)})
          .with_background(Theme::Usage::Surface).with_padding(Padding::all(pixels(12*s))));
      const std::array<std::string,4> names{"Home","Settings","Profile","Help"};
      for(int i=0;i<4;++i) if(button(context,mk(sidebar.ent(),i),ComponentConfig{}.with_size({percent(1),pixels(48*s)})
          .with_label(names[i]).with_font("AtkinsonMock",pixels(22*s)).with_background(selected==names[i]?Theme::Usage::Primary:Theme::Usage::Secondary)
          .with_auto_text_color(true).with_margin(Margin{.bottom=pixels(12*s)}).with_debug_name("vstack_nav_"+std::to_string(i)))) selected=names[i];
      auto content=vstack(context,mk(layout.ent(),1),ComponentConfig{}.with_size({percent(0.60f),percent(1)})
          .with_custom_background(panel).with_padding(Padding::all(pixels(12*s))).with_margin(Margin{.left=pixels(12*s)}));
      div(context,mk(content.ent(),0),ComponentConfig{}.with_size({percent(1),pixels(48*s)})
          .with_label(selected+" / Content").with_font("AtkinsonMock",pixels(24*s)).with_custom_text_color(ink));
      const std::array<std::string,3> names2{"Alerts","Activity","Stats"};
      for(int i=0;i<3;++i) {
        auto card=div(context,mk(content.ent(),i+1),ComponentConfig{}.with_size({percent(1),pixels(60*s)})
            .with_custom_background(colors[i]).with_padding(Padding{.left=pixels(12*s),.right=pixels(12*s)}).with_margin(Margin{.top=pixels(12*s)}));
        div(context,mk(card.ent(),0),ComponentConfig{}.with_size({percent(1),percent(1)})
            .with_label(names2[i]).with_font("AtkinsonMock",pixels(22*s))
            .with_custom_text_color({8,22,30,255}));
      }
      label(92,"Choose a sidebar item to update the content heading. Cards are static layout samples.",12,590,1136,28,19,true);
      label(93,"vstack defaults: full parent width; height grows from children. This fixture sets both explicitly.",12,628,1136,28,18,true);
    }
  }
};

REGISTER_EXAMPLE_SCREEN(vstack_showcase, "Component Galleries",
                        "vstack() primitive showcase", VStackShowcase)
