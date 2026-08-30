#pragma once

#include <memory>

#include <afterhours/src/library.h>
#include <afterhours/src/singleton.h>

SINGLETON_FWD(Preload)
struct Preload {
  SINGLETON(Preload)

  Preload();
  ~Preload();

  Preload(const Preload &) = delete;
  void operator=(const Preload &) = delete;

  Preload &init(const char *title);
  Preload &make_singleton();
};

// Theme and styling the whole app assumes. Headless capture builds its own
// singletons rather than going through Preload, so it has to call this too or
// the baselines are rendered with different defaults than the app runs.
void apply_ui_styling_defaults();

// Call before building a screen. ThemeDefaults and UIStylingDefaults are
// process globals that screens write to and never put back, so without this a
// screen renders differently depending on which screens ran before it.
void restore_ui_styling_defaults();
