#pragma once

#include <afterhours/ah.h>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

struct ExampleScreen {
  std::string name;
  std::string description;
  std::function<std::unique_ptr<afterhours::SystemBase>()> create_system;
};

struct ExampleScreenRegistry {
  static ExampleScreenRegistry &get() {
    static ExampleScreenRegistry instance;
    return instance;
  }

  void register_screen(
      const std::string &flag_name, const std::string &description,
      std::function<std::unique_ptr<afterhours::SystemBase>()> create_func) {
    screens[flag_name] = {flag_name, description, create_func};
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

  void list_screens() const {
    std::cout << "Available example screens:\n";
    for (const auto &[name, screen] : screens) {
      std::cout << "  --screen=" << name << "    " << screen.description << "\n";
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

#define REGISTER_EXAMPLE_SCREEN(flag_name, description, system_type)           \
  static struct ExampleScreenRegistrar_##flag_name {                           \
    ExampleScreenRegistrar_##flag_name() {                                     \
      ExampleScreenRegistry::get().register_screen(                            \
          #flag_name, description,                                             \
          []() -> std::unique_ptr<afterhours::SystemBase> {                    \
            auto ptr = std::make_unique<system_type>();                        \
            return std::unique_ptr<afterhours::SystemBase>(ptr.release());     \
          });                                                                  \
    }                                                                          \
  } example_screen_registrar_##flag_name;
