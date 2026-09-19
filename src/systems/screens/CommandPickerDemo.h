#pragma once

#include "../../external.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/src/plugins/command_picker/command_picker.h>

struct CommandPickerDemo : ScreenSystem<afterhours::ui::UIContext<InputAction>> {
  int counter = 0;
  std::string accent = "Blue";
  bool grid = false;
  afterhours::terminal::Console commands;
  afterhours::command_picker::Picker picker{{
      {"increment", "Increase counter", "Counter", ""},
      {"reset", "Reset counter", "Counter", ""},
      {"color blue", "Blue accent", "Appearance", ""},
      {"color green", "Green accent", "Appearance", ""},
      {"color purple", "Purple accent", "Appearance", ""},
      {"grid", "Toggle grid", "View", "G"},
      {"zoom 50", "Zoom to 50%", "View", ""},
      {"zoom 75", "Zoom to 75%", "View", ""},
      {"zoom 100", "Actual size", "View", "1"},
      {"zoom 125", "Zoom to 125%", "View", ""},
      {"zoom 150", "Zoom to 150%", "View", ""},
      {"zoom 200", "Zoom to 200%", "View", ""}}};
  int zoom = 100;

  CommandPickerDemo() {
    using namespace afterhours::terminal;
    commands.execution = Execution::Queued;
    commands.add_command({"increment", "Add one to the counter", [this](Arguments) {
      ++counter;
      return Result{"Counter increased"};
    }});
    commands.add_command({"reset", "Start counting from zero", [this](Arguments) {
      counter = 0;
      return Result{"Counter reset"};
    }, {}, {}, "reset", [this]() -> std::optional<std::string> {
      if (counter == 0) return "Counter is already zero";
      return {};
    }});
    commands.add_command({"color", "Change the preview accent", [this](Arguments args) {
      if (args.size() != 1) return Result{"Usage: color blue|green|purple", false};
      if (args[0] == "blue") accent = "Blue";
      else if (args[0] == "green") accent = "Green";
      else if (args[0] == "purple") accent = "Purple";
      else return Result{"Choose blue, green or purple", false};
      return Result{"Accent: " + accent};
    }, {}, {}, "color blue|green|purple"});
    commands.add_command({"grid", "Show or hide the preview grid", [this](Arguments) {
      grid = !grid;
      return Result{grid ? "Grid shown" : "Grid hidden"};
    }});
    commands.add_command({"zoom", "Resize the preview tile", [this](Arguments args) {
      const auto value = args.get<int>(0);
      if (args.size() != 1 || !value || *value < 50 || *value > 200)
        return Result{"Usage: zoom 50..200", false};
      zoom = *value;
      return Result{"Zoom: " + std::to_string(zoom) + "%"};
    }, {}, {}, "zoom 50..200"});
  }

  void once(float) override { commands.drain(); }

  void for_each_with(afterhours::Entity &entity,
                     afterhours::ui::UIContext<InputAction> &ctx, float) override {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    const float scale = std::min(ctx.screen_width / 1280.f, ctx.screen_height / 720.f);
    const float left = (ctx.screen_width - 1184.f * scale) / 2.f;
    const afterhours::Color ink{15, 22, 34, 255}, surface{25, 35, 51, 255};
    const afterhours::Color text{233, 239, 248, 255}, muted{171, 185, 207, 255};
    const afterhours::Color highlight = accent == "Green" ? afterhours::Color{118, 224, 179, 255} :
        accent == "Purple" ? afterhours::Color{198, 172, 255, 255} : afterhours::Color{143, 193, 255, 255};
    ctx.theme = Theme{};
    ctx.theme.background = ink;
    ctx.theme.surface = surface;
    ctx.theme.secondary = {36, 48, 67, 255};
    ctx.theme.font = text;
    ctx.theme.font_muted = muted;
    ctx.theme.accent = highlight;
    ctx.theme.error = {255, 152, 160, 255};
    ctx.scaling_mode = ScalingMode::Proportional;
    div(ctx, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(ctx.screen_width), pixels(ctx.screen_height)})
        .with_absolute_position(0, 0).with_custom_background(ink).with_corner_radius(0));
    auto label = [&](int id, std::string value, float x, float y, float width, float size,
                     afterhours::Color color) {
      div(ctx, mk(entity, id), ComponentConfig{}
          .with_size({pixels(width * scale), pixels(40.f * scale)})
          .with_absolute_position(left + x * scale, y * scale)
          .with_font("AtkinsonMock", pixels(size * scale))
          .with_label(value).with_alignment(TextAlignment::Left)
          .with_custom_text_color(color).with_transparent_bg());
    };
    label(1, "Command picker", 0, 32, 1184, 34, text);
    label(2, "Search by name or category. Try grn, counter, or zoom.", 0, 84, 1184, 20, muted);
    afterhours::command_picker::panel(ctx, mk(entity, 3), commands, picker, ComponentConfig{}
        .with_absolute_position(left, 148.f * scale)
        .with_size({pixels(824.f * scale), pixels(488.f * scale)})
        .with_padding(Padding::all(pixels(16.f * scale)))
        .with_font("AtkinsonMock", pixels(18.f * scale))
        .with_custom_background(surface).with_corner_radius(12.f * scale));
    div(ctx, mk(entity, 4), ComponentConfig{}
        .with_absolute_position(left + 848.f * scale, 148.f * scale)
        .with_size({pixels(336.f * scale), pixels(488.f * scale)})
        .with_custom_background(surface).with_corner_radius(12.f * scale));
    label(5, "Preview", 868, 164, 292, 24, text);
    label(6, "Counter: " + std::to_string(counter), 868, 216, 292, 22, highlight);
    label(7, "Accent: " + accent, 868, 256, 292, 20, muted);
    label(8, "Zoom: " + std::to_string(zoom) + "%", 868, 296, 292, 20, muted);
    label(9, grid ? "Grid: on" : "Grid: off", 868, 336, 292, 20, muted);
    if (grid) {
      for (int i = 0; i < 9; ++i) {
        div(ctx, mk(entity, 20 + i), ComponentConfig{}
            .with_absolute_position(left + (884.f + static_cast<float>(i) * 32.f) * scale, 404.f * scale)
            .with_size({pixels(scale), pixels(176.f * scale)})
            .with_custom_background({58, 73, 93, 255}).with_corner_radius(0));
      }
      for (int i = 0; i < 6; ++i) {
        div(ctx, mk(entity, 40 + i), ComponentConfig{}
            .with_absolute_position(left + 884.f * scale, (404.f + static_cast<float>(i) * 32.f) * scale)
            .with_size({pixels(256.f * scale), pixels(scale)})
            .with_custom_background({58, 73, 93, 255}).with_corner_radius(0));
      }
    }
    const float tile = 64.f * static_cast<float>(zoom) / 100.f;
    div(ctx, mk(entity, 10), ComponentConfig{}
        .with_absolute_position(left + (1016.f - tile / 2.f) * scale, (490.f - tile / 2.f) * scale)
        .with_size({pixels(tile * scale), pixels(tile * scale)})
        .with_custom_background(highlight).with_corner_radius(8.f * scale));
    label(11, "Up / Down: select    Enter: run    Esc: clear search", 0, 652, 1184, 18, muted);
  }
};

REGISTER_EXAMPLE_SCREEN(command_picker, "Tools", "Search and run categorized commands", CommandPickerDemo)
