#pragma once

// Compatibility shim (WM pluginization Batch 3 Lane J): the canonical
// screen scaffold now lives at src/plugins/screen_kit/screen_scaffold.h
// (named WM-local plugin "screen_kit"). This header keeps the historical
// include path working unchanged, so all existing consumers compile
// unmodified. Behavior is identical; new code should include the
// canonical path.
#include "../plugins/screen_kit/screen_scaffold.h"
