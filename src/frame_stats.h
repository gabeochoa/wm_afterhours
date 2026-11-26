#pragma once

// Compatibility shim (WM pluginization Batch 3 Lane H): the canonical
// FrameStats implementation now lives at
// src/devtools/profiling/frame_stats.h (named WM-local plugin
// "profiling"). This header keeps the historical include path working
// unchanged, so AnimationStressLab, CurvedTextStressLab, and
// DevtoolsManifestTest compile unmodified. Behavior is identical; new
// code should include the canonical path.
#include "devtools/profiling/frame_stats.h"
