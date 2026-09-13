#pragma once

#include "../../external.h"
#include "../../input_mapping.h"
#include "../ExampleScreenRegistry.h"
#include <array>
#include <afterhours/src/plugins/modal.h>
#include <afterhours/src/plugins/toast.h>

struct DefaultTheme : ScreenSystem<afterhours::ui::UIContext<InputAction>> {
  std::string project_name = "Weekend workshop";
  bool notifications = true;
  bool confirm_open = false;
  bool about_open = false;
  bool name_required = false;
  float volume = .6f;
  size_t visibility = 0;
  int section = 0;
  int saves = 0;
  std::vector<std::string> visibility_options{"Only me", "My team", "Anyone with the link"};

  void for_each_with(afterhours::Entity &entity,
                     afterhours::ui::UIContext<InputAction> &ctx, float) override {
    using namespace afterhours;
    using namespace afterhours::ui;
    using namespace afterhours::ui::imm;
    ctx.set_theme(Theme{});
    ctx.scaling_mode = ScalingMode::Proportional;
    const auto layout = [](float width, float height) {
      return ComponentConfig{}.with_size({w1280(width), h720(height)});
    };
    div(ctx, mk(entity, 0), ComponentConfig{}
        .with_size({screen_pct(1.f), screen_pct(1.f)})
        .with_absolute_position(0, 0).with_color_usage(Theme::Usage::Background)
        .with_corner_radius(0));
    auto page = vstack(ctx, mk(entity, 1), layout(1184, 624)
        .with_margin(Margin{.top = h720(48), .left = w1280(48)})
        .with_gap(DefaultSpacing::medium()).with_debug_name("default_page"));
    auto header = vstack(ctx, mk(page.ent(), 0), layout(1184, 80)
        .with_gap(DefaultSpacing::micro()));
    div(ctx, mk(header.ent(), 0), layout(1184, 44).with_label("Your workspace")
        .with_font_size(FontSize::Large)
        .with_font_weight(colors::FontWeight::Bold));
    div(ctx, mk(header.ent(), 1), layout(1184, 32)
        .with_label("A complete page using the library's default colors, type and controls.")
        .with_text_color(Theme::Usage::FontMuted));
    auto body = hstack(ctx, mk(page.ent(), 1), layout(1184, 448)
        .with_gap(DefaultSpacing::medium()).with_no_wrap());
    auto nav = vstack(ctx, mk(body.ent(), 0), layout(200, 448)
        .with_gap(DefaultSpacing::tiny()));
    const std::array<std::string, 3> sections{"General", "Notifications", "Members"};
    for (size_t i = 0; i < sections.size(); ++i) {
      if (button(ctx, mk(nav.ent(), static_cast<int>(i)), layout(200, 44)
          .with_label(sections[i]).with_button_variant(section == static_cast<int>(i)
              ? ButtonVariant::Filled : ButtonVariant::Ghost)
          .with_debug_name("default_nav_" + std::to_string(i))))
        section = static_cast<int>(i);
    }
    auto card = vstack(ctx, mk(body.ent(), 1), layout(648, 448)
        .with_color_usage(Theme::Usage::Surface)
        .with_corner_radius(ctx.theme.panel_corner_radius)
        .with_padding(Padding::all(DefaultSpacing::medium())).with_gap(DefaultSpacing::tiny())
        .with_debug_name("default_card"));
    div(ctx, mk(card.ent(), 0), layout(600, 32).with_label(sections[section])
        .with_font_weight(colors::FontWeight::Bold));
    if (section == 0) {
      div(ctx, mk(card.ent(), 1), layout(600, 28).with_label("Project name"));
      ui::imm::text_input(ctx, mk(card.ent(), 2), project_name, layout(600, 44)
          .with_placeholder("Enter a project name").with_debug_name("default_name"));
      if (!project_name.empty()) name_required = false;
      div(ctx, mk(card.ent(), 3), layout(600, 28).with_label("Who can see this project?"));
      dropdown(ctx, mk(card.ent(), 4), visibility_options, visibility,
          layout(600, 44).with_debug_name("default_visibility"));
      div(ctx, mk(card.ent(), 5), layout(600, 28)
          .with_label("Notification volume").with_tooltip("Controls notification sounds for this workspace."));
      slider(ctx, mk(card.ent(), 6), volume, layout(600, 44)
          .with_debug_name("default_volume"));
      div(ctx, mk(card.ent(), 7), layout(600, 28)
          .with_label("Volume: " + std::to_string(static_cast<int>(volume * 100)) + "%")
          .with_text_color(Theme::Usage::FontMuted).with_debug_name("default_volume_value"));
    }
    if (section == 1) {
      toggle_switch(ctx, mk(card.ent(), 8), notifications, layout(600, 48)
          .with_label("Email notifications").with_debug_name("default_notifications"));
      div(ctx, mk(card.ent(), 9), layout(600, 64)
          .with_label(notifications ? "Email notifications are on." : "Email notifications are off.")
          .with_text_color(Theme::Usage::FontMuted));
    }
    if (section == 2) {
      div(ctx, mk(card.ent(), 10), layout(600, 40).with_label("You are the workspace owner."));
      button(ctx, mk(card.ent(), 11), layout(200, 44)
          .with_label("Invite members").with_disabled(true)
          .with_debug_name("default_disabled"));
      div(ctx, mk(card.ent(), 12), layout(600, 48)
          .with_label("Member invitations are unavailable on this plan.")
          .with_text_color(Theme::Usage::FontMuted));
    }
    auto summary = vstack(ctx, mk(body.ent(), 2), layout(288, 448)
        .with_gap(DefaultSpacing::small()));
    div(ctx, mk(summary.ent(), 0), layout(288, 32).with_label("Workspace details")
        .with_font_weight(colors::FontWeight::Bold));
    div(ctx, mk(summary.ent(), 1), layout(288, 72)
        .with_label("Changes stay here until you save them.")
        .with_text_overflow(TextOverflow::Wrap).with_text_color(Theme::Usage::FontMuted));
    if (button(ctx, mk(summary.ent(), 2), layout(200, 44)
        .with_label("About defaults").with_button_variant(ButtonVariant::Outline)
        .with_tooltip("One theme supplies every native control and overlay.", .15f)
        .with_debug_name("default_help"))) about_open = true;
    auto footer = hstack(ctx, mk(page.ent(), 2), layout(1184, 48)
        .with_gap(DefaultSpacing::small()).with_no_wrap());
    if (button(ctx, mk(footer.ent(), 0), layout(184, 44).with_label("Save changes")
        .with_debug_name("default_save"))) {
      name_required = project_name.empty();
      confirm_open = !name_required;
    }
    div(ctx, mk(footer.ent(), 1), layout(720, 44)
        .with_label(name_required ? "Enter a project name before saving." :
            saves ? "Saved " + std::to_string(saves) + " time(s)." : "No changes saved yet.")
        .with_color_usage(name_required ? Theme::Usage::Error : Theme::Usage::None)
        .with_debug_name("default_status"));
    modal::info(ctx, mk(entity, 3), about_open, "About this theme",
        "Native controls share colors, spacing and one readable font family.", "Got it");
    auto confirm = modal::confirm(ctx, mk(entity, 2), confirm_open,
        "Save workspace?", "Your name, visibility and notification preferences will be saved.",
        "Confirm save", "Keep editing");
    if (confirm.confirmed()) {
      ++saves;
      toast::send_success(ctx, "Workspace settings saved.");
    }
  }
};

REGISTER_EXAMPLE_SCREEN(default_theme, "Component Galleries",
                        "A workspace with the default theme", DefaultTheme)
