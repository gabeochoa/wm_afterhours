#pragma once

// Compatibility shim (WM pluginization Batch 2 Lane E): the canonical
// ScreenTree implementation now lives at
// src/plugins/gallery_shell/ScreenTree.h (named WM-local
// plugin "gallery_shell"). This header keeps the historical include
// path working unchanged, so tests/screen_tree_test.cpp and
// ScreenNavigator compile unmodified. Behavior is identical; new code
// should include the canonical path.
#include "../plugins/gallery_shell/ScreenTree.h"
