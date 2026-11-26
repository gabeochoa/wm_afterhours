#pragma once

// Compatibility shim (Batch 3 Lane G pluginization): the canonical layout
// summary now lives at src/devtools/layout_audit/layout_summary.h.
// Makefile only globs src/testing/*.cpp, so the implementation translation
// unit stays in this directory (see layout_summary.cpp transport TU below)
// while the headers are canonicalized. This header keeps the historical
// include path ("testing/layout_summary.h", "layout_summary.h") working
// unchanged. Behavior and public API unchanged.
#include "../devtools/layout_audit/layout_summary.h"
