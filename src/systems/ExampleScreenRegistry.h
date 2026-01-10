#pragma once

#include <afterhours/ah.h>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

struct ExampleScreen {
  std::string name;
  std::string category;
  std::string description;
  std::function<std::unique_ptr<afterhours::SystemBase>()> create_system;
};

struct ExampleScreenRegistry {
  static ExampleScreenRegistry &get() {
    static ExampleScreenRegistry instance;
    return instance;
  }

  void register_screen(
      const std::string &flag_name, const std::string &category,
      const std::string &description,
      std::function<std::unique_ptr<afterhours::SystemBase>()> create_func) {
    screens[flag_name] = {flag_name, category, description, create_func};
  }

  bool has_screen(const std::string &flag_name) const {
    return screens.find(flag_name) != screens.end();
  }

  std::unique_ptr<afterhours::SystemBase>
  create_screen(const std::string &flag_name) {
    auto it = screens.find(flag_name);
    if (it == screens.end()) {
      return nullptr;
    }
    return it->second.create_system();
  }

  std::string get_screen_description(const std::string &flag_name) const {
    auto it = screens.find(flag_name);
    if (it == screens.end()) {
      return "";
    }
    return it->second.description;
  }

  void list_screens() const {
    // Group screens by category
    std::map<std::string, std::vector<const ExampleScreen *>> by_category;
    for (const auto &[name, screen] : screens) {
      by_category[screen.category].push_back(&screen);
    }

    // Define category order
    std::vector<std::string> category_order = {
        "Game Mockups", "Component Galleries", "System Demos", "Tools"};

    std::cout << "Available example screens:\n\n";

    for (const auto &category : category_order) {
      auto it = by_category.find(category);
      if (it == by_category.end())
        continue;

      std::cout << "  " << category << ":\n";
      for (const auto *screen : it->second) {
        std::cout << "    --screen=" << screen->name;
        // Pad to align descriptions
        int padding = 20 - static_cast<int>(screen->name.length());
        if (padding > 0) {
          std::cout << std::string(padding, ' ');
        } else {
          std::cout << "  ";
        }
        std::cout << screen->description << "\n";
      }
      std::cout << "\n";
    }

    // Print any screens in unknown categories
    for (const auto &[category, screen_list] : by_category) {
      bool found = false;
      for (const auto &c : category_order) {
        if (c == category) {
          found = true;
          break;
        }
      }
      if (!found) {
        std::cout << "  " << category << ":\n";
        for (const auto *screen : screen_list) {
          std::cout << "    --screen=" << screen->name;
          int padding = 20 - static_cast<int>(screen->name.length());
          if (padding > 0) {
            std::cout << std::string(padding, ' ');
          } else {
            std::cout << "  ";
          }
          std::cout << screen->description << "\n";
        }
        std::cout << "\n";
      }
    }
  }

  std::vector<std::string> get_screen_names() const {
    std::vector<std::string> names;
    for (const auto &[name, screen] : screens) {
      names.push_back(name);
    }
    return names;
  }

private:
  std::map<std::string, ExampleScreen> screens;
};

#define REGISTER_EXAMPLE_SCREEN(flag_name, category, description, system_type) \
  static struct ExampleScreenRegistrar_##flag_name {                           \
    ExampleScreenRegistrar_##flag_name() {                                     \
      ExampleScreenRegistry::get().register_screen(                            \
          #flag_name, category, description,                                   \
          []() -> std::unique_ptr<afterhours::SystemBase> {                    \
            auto ptr = std::make_unique<system_type>();                        \
            return std::unique_ptr<afterhours::SystemBase>(ptr.release());     \
          });                                                                  \
    }                                                                          \
  } example_screen_registrar_##flag_name;
