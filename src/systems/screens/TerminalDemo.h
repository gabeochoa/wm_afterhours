#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <afterhours/src/plugins/terminal/terminal.h>
#include <limits>

struct TerminalDemo : ScreenSystem<afterhours::ui::UIContext<InputAction>> {
  struct CountCommand : afterhours::terminal::CommandBase {
    int &counter;
    explicit CountCommand(int &value) : counter(value) {}
    std::string_view name() const override { return "count"; }
    std::string_view help() const override { return "Add to the counter: count [amount], default 1"; }
    afterhours::terminal::Result run(afterhours::terminal::Arguments args) override {
      if (args.size() > 1) return {"Usage: count [amount]", false};
      const auto amount = args.get<int>(0, 1);
      if (!amount) return {"Amount must be a whole number", false};
      if ((*amount > 0 && counter > std::numeric_limits<int>::max() - *amount) ||
          (*amount < 0 && counter < std::numeric_limits<int>::min() - *amount))
        return {"Counter would overflow", false};
      counter += *amount;
      return {"Counter: " + std::to_string(counter)};
    }
  };

  int counter = 0;
  std::string accent = "blue";
  std::vector<std::string> palette{"blue", "green", "purple"};
  afterhours::terminal::Console console;

  TerminalDemo() {
    using namespace afterhours::terminal;
    console.add_command({"echo", "Print your text", [](Arguments args) {
      std::string text;
      for (const auto &arg : args) {
        if (!text.empty()) text += ' ';
        text += arg;
      }
      return Result{std::move(text)};
    }});
    console.add_command(std::make_unique<CountCommand>(counter));
    console.add_command({"accent", "Change color using the current palette", [this](Arguments args) {
      if (args.size() != 1 || std::find(palette.begin(), palette.end(), args[0]) == palette.end())
        return Result{"Choose an available accent color", false};
      accent = args[0];
      return Result{"Accent: " + accent};
    }, {}, [this](const CompletionRequest &request) {
      return request.argument_index == 0 ? palette : std::vector<std::string>{};
    }});
    console.add_command({"palette", "Edit available colors: palette add|remove <color>", [this](Arguments args) {
      if (args.size() != 2 || (args[0] != "add" && args[0] != "remove"))
        return Result{"Usage: palette add|remove <color>", false};
      const std::vector<std::string> colors{"amber", "blue", "green", "purple"};
      if (std::find(colors.begin(), colors.end(), args[1]) == colors.end())
        return Result{"Unknown color: " + args[1], false};
      auto found = std::find(palette.begin(), palette.end(), args[1]);
      if (args[0] == "add") {
        if (found != palette.end()) return Result{"Color already available", false};
        palette.push_back(args[1]);
      } else {
        if (args[1] == accent) return Result{"Choose another accent before removing this color", false};
        if (found == palette.end()) return Result{"Color is not in the palette", false};
        palette.erase(found);
      }
      console.invalidate_completions();
      return Result{"Palette updated"};
    }, {}, [this](const CompletionRequest &request) -> std::vector<std::string> {
      if (request.argument_index == 0) return {"add", "remove"};
      if (request.argument_index != 1) return {};
      if (request.arguments[0] == "remove") {
        auto colors = palette;
        std::erase(colors, accent);
        return colors;
      }
      if (request.arguments[0] != "add") return {};
      std::vector<std::string> colors{"amber", "blue", "green", "purple"};
      std::erase_if(colors, [this](const auto &color) {
        return std::find(palette.begin(), palette.end(), color) != palette.end();
      });
      return colors;
    }});
    console.add_command({"history", "List recent commands", [this](Arguments args) {
      if (!args.empty()) return Result{"Usage: history", false};
      std::string text;
      for (const auto &line : console.history()) {
        if (!text.empty()) text += '\n';
        text += line;
      }
      return Result{std::move(text)};
    }});
    console.print({"Welcome. Commands here are registered by the WM screen."});
    console.execute("help");
  }

  void for_each_with(afterhours::Entity &entity,
                     afterhours::ui::UIContext<InputAction> &ctx, float) override {
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    const float scale = std::min(ctx.screen_width / 1280.f, ctx.screen_height / 720.f);
    const float left = (ctx.screen_width - 1184.f * scale) / 2.f;
    const afterhours::Color background{15, 22, 34, 255};
    const afterhours::Color foreground{233, 239, 248, 255};
    const afterhours::Color muted{171, 185, 207, 255};
    const afterhours::Color highlight = accent == "amber" ? afterhours::Color{244, 195, 105, 255} :
        accent == "green" ? afterhours::Color{118, 224, 179, 255} :
        accent == "purple" ? afterhours::Color{198, 172, 255, 255} : afterhours::Color{143, 193, 255, 255};
    ctx.theme = Theme{};
    ctx.theme.background = background;
    ctx.theme.surface = {25, 35, 51, 255};
    ctx.theme.secondary = {36, 48, 67, 255};
    ctx.theme.primary = {44, 65, 92, 255};
    ctx.theme.font = foreground;
    ctx.theme.font_muted = muted;
    ctx.theme.accent = highlight;
    ctx.theme.error = {255, 152, 160, 255};
    ctx.scaling_mode = ScalingMode::Proportional;
    div(ctx, mk(entity, 0), ComponentConfig{}
        .with_size({pixels(ctx.screen_width), pixels(ctx.screen_height)})
        .with_absolute_position(0, 0).with_custom_background(background).with_corner_radius(0));
    auto label = [&](int id, std::string value, float y, float size, afterhours::Color color) {
      return div(ctx, mk(entity, id), ComponentConfig{}
          .with_size({pixels(1184.f * scale), pixels(42.f * scale)})
          .with_absolute_position(left, y * scale)
          .with_font("AtkinsonMock", pixels(size * scale))
          .with_label(value).with_alignment(TextAlignment::Left)
          .with_custom_text_color(color).with_transparent_bg());
    };
    label(1, "Developer terminal", 32, 34, foreground);
    label(2, "Custom commands, native text editing, and a console you can embed in your game.", 84, 20, muted);
    label(3, "Counter: " + std::to_string(counter) + "     Accent: " + accent, 134, 22, highlight);
    afterhours::terminal::AutocompleteStyle autocomplete_style;
    autocomplete_style.list.with_border_top({58, 73, 93, 255}, pixels(1.f));
    autocomplete_style.row.with_custom_text_color({200, 210, 225, 255})
        .with_custom_hover_bg({30, 43, 61, 255});
    autocomplete_style.selected_row.with_custom_text_color(highlight)
        .with_border_left(highlight, pixels(2.f * scale));
    afterhours::terminal::panel(ctx, mk(entity, 4), console, ComponentConfig{}
        .with_size({pixels(1184.f * scale), pixels(436.f * scale)})
        .with_absolute_position(left, 196.f * scale)
        .with_font("AtkinsonMock", pixels(20.f * scale))
        .with_custom_background({25, 35, 51, 255}).with_corner_radius(12.f * scale),
        autocomplete_style);
    label(5, "Enter: accept / run   |   Up / Down: suggestions / history   |   Tab: complete   |   Esc: dismiss / leave", 652, 18, muted);
  }
};

REGISTER_EXAMPLE_SCREEN(terminal, "Tools", "Register and run custom developer commands", TerminalDemo)
