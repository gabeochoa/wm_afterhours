#pragma once

// Compatibility shim (Batch 4 Lane K pluginization): the canonical
// screenshot validation now lives at
// src/devtools/headless_harness/screenshot_validation.h. This header keeps
// the historical include path ("testing/screenshot_validation.h")
// working unchanged. Behavior and public API unchanged.
#include "../devtools/headless_harness/screenshot_validation.h"
