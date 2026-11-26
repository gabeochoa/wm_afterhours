#pragma once

// Compatibility shim (Batch 4 Lane K pluginization): the canonical
// headless screenshots harness now lives at
// src/devtools/headless_harness/headless_screenshots.h. This header keeps
// the historical include path ("headless_screenshots.h") working
// unchanged. Behavior and public API unchanged.
#include "devtools/headless_harness/headless_screenshots.h"
