#pragma once

// Compatibility shim (WM pluginization Batch 2 Lane E): the canonical
// ExampleScreenRegistry implementation now lives at
// src/plugins/gallery_shell/ExampleScreenRegistry.h (named WM-local
// plugin "gallery_shell"). This header keeps the historical include
// path working unchanged, so all screens and game/main compile
// unmodified. Behavior is identical; new code should include the
// canonical path.
#include "../plugins/gallery_shell/ExampleScreenRegistry.h"
