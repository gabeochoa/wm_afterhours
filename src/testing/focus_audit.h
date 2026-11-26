#pragma once

// Compatibility shim (Batch 3 Lane G pluginization): the canonical focus
// audit now lives at src/devtools/layout_audit/focus_audit.h.
// Makefile only globs src/testing/*.cpp, so the implementation translation
// unit stays in this directory (see focus_audit.cpp transport TU below)
// while the headers are canonicalized. This header keeps the historical
// include path ("testing/focus_audit.h", "focus_audit.h") working
// unchanged. Behavior and public API unchanged.
#include "../devtools/layout_audit/focus_audit.h"
