// Compatibility shim (WM pluginization Batch 4 Lane L): the canonical
// E2E integration wiring now lives at
// src/testing/screen_command_pack/e2e_integration.h. This header keeps
// the historical include path working unchanged, so src/game.cpp,
// src/main.cpp, and src/devtools/wm_devtools.h compile unmodified.
// Spelling and semantics are identical (same e2e:: namespace, same
// register_e2e_systems / configure_runner / re-exported arg parsing).
#include "screen_command_pack/e2e_integration.h"
