#pragma once

// Compatibility shim (WM pluginization Batch 2 Lane E): the canonical
// RenderScreenHUD implementation now lives at
// src/plugins/gallery_shell/RenderScreenHUD.h (named WM-local
// plugin "gallery_shell"). This header keeps the historical include
// path working unchanged. Behavior is identical; new code should
// include the canonical path.
#include "../plugins/gallery_shell/RenderScreenHUD.h"
