#pragma once

// Compatibility shim (WM pluginization Batch 3 Lane H): the canonical
// ApplyBlurPass implementation now lives at
// src/plugins/render_passes/ApplyBlurPass.h (named WM-local plugin
// "render_passes"). This header keeps the historical include path
// working unchanged, so game.cpp and wm_devtools.h compile unmodified.
// Behavior is identical; new code should include the canonical path.
#include "../plugins/render_passes/ApplyBlurPass.h"
