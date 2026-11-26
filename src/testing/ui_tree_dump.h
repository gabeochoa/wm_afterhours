#pragma once

// Compatibility shim (Batch 3 Lane G pluginization): the canonical UI tree
// dump now lives at src/devtools/layout_audit/ui_tree_dump.h (header-only).
// This header keeps the historical include path ("testing/ui_tree_dump.h",
// "ui_tree_dump.h") working unchanged. Behavior, JSON output format, and
// public API unchanged.
#include "../devtools/layout_audit/ui_tree_dump.h"
