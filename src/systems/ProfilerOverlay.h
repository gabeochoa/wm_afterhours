#pragma once

// Compatibility shim (WM pluginization Batch 3 Lane H): the canonical
// ProfilerOverlay implementation now lives at
// src/devtools/profiling/ProfilerOverlay.h (named WM-local plugin
// "profiling"). This header keeps the historical include path working
// unchanged, so game.cpp and wm_devtools.h compile unmodified. Behavior
// is identical; new code should include the canonical path.
#include "../devtools/profiling/ProfilerOverlay.h"
