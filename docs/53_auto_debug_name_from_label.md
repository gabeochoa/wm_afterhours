# Feature Request: Auto-Derive Debug Name from Label

**Type:** Afterhours vendor change
**File:** `vendor/afterhours/src/plugins/ui/imm_components.h` (at element creation)
**Decision:** YES -- approved for implementation
**Impact:** ~1,000 `.with_debug_name()` calls could be removed across 64 screens

## Problem

`.with_debug_name()` appears ~1,400 times across all screens. In the vast majority of cases, the debug name is the label text or something very similar. This is pure boilerplate:

```cpp
div(ctx, mk(e, 10),
    ComponentConfig{}
        .with_label("Score")
        .with_debug_name("score_label"));  // redundant -- we already know it's "Score"
```

Forgetting `.with_debug_name()` makes debugging harder, but requiring it on every element is tedious.

## Proposed Solution

When an element is created (in `_init_component` or similar), if no explicit debug name was set but a label exists, auto-generate the debug name as `"<label> <component_type>"`:

```cpp
// In _init_component() or element creation path:
if (!config.has_debug_name() && config.has_label()) {
    config.debug_name = fmt::format("{} {}", config.label, component_type_name);
}
```

Format: `"<label> <component_type>"`

Examples:
- `with_label("Score")` on a `div` → debug name `"Score div"`
- `with_label("Start Game")` on a `button` → debug name `"Start Game button"`
- `with_label("Volume")` on a `slider` → debug name `"Volume slider"`

## Rules

1. If `.with_debug_name()` is explicitly called, it always wins (no override)
2. If no label and no debug name, keep current behavior (empty or auto-generated)
3. The auto name is set at element creation time, not in `ComponentConfig` builder

## Usage

```cpp
// Before:
div(ctx, mk(e, 10),
    ComponentConfig{}.with_label("Score").with_debug_name("score"));

// After (debug name auto-set to "Score div"):
div(ctx, mk(e, 10),
    ComponentConfig{}.with_label("Score"));

// Explicit override still works:
div(ctx, mk(e, 10),
    ComponentConfig{}.with_label("Score").with_debug_name("hud_score"));
```

## Notes

- Elements without labels (backgrounds, separators, decorative) still need explicit `.with_debug_name()` for debuggability
- Could potentially remove ~1,000 `.with_debug_name()` calls across the codebase
- No breaking changes -- existing explicit debug names are preserved
