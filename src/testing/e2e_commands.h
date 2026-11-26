// Compatibility shim (WM pluginization Batch 4 Lane L): the canonical
// app-specific E2E screen command pack now lives at
// src/testing/screen_command_pack/e2e_commands.h. This header keeps the
// historical include path working unchanged, so src/game.cpp,
// src/devtools/wm_devtools.h, and any other includer compile unmodified.
// Spelling and semantics are identical (same namespace, same command
// names, same args, same behavior).
#include "screen_command_pack/e2e_commands.h"
