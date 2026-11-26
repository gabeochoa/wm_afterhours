// Transport TU (Batch 3 Lane G pluginization): the canonical implementation
// lives at src/devtools/layout_audit/focus_audit.cpp. The Makefile only
// globs src/testing/*.cpp (MAIN_SRC), so .cpp files under src/devtools/**
// are not compiled; this file exists to keep the link working while the
// headers/impl are canonicalized under src/devtools/layout_audit/.
// #including the canonical header + impl here means the canonical source
// is the single source of truth and is compiled exactly once through this
// translation unit. Do not duplicate the implementation logic here.
#include "../devtools/layout_audit/focus_audit.h"
#include "../devtools/layout_audit/focus_audit.cpp"
