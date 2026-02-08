# Feature Request: Modal Children Auto-Inherit Render Layer

**Type:** Afterhours change required
**Decision:** YES -- approved for implementation
**File:** `vendor/afterhours/src/plugins/ui/modal.h` (or similar)
**Impact:** ~15 lines saved per modal screen

## Problem

Every child element inside a modal must explicitly specify `.with_render_layer(MODAL_CONTENT_LAYER)`. Forgetting this causes the element to render behind the modal backdrop, which is a confusing and common bug.

In `ModalShowcase.h`, this appears on nearly every element inside modal content callbacks:

```cpp
div(ctx, mk(content.ent(), 1),
    ComponentConfig{}
        .with_label("Are you sure?")
        .with_render_layer(MODAL_CONTENT_LAYER)  // <-- required on EVERY child
        ...);
```

## Proposed Solution

When the modal system creates the content container, it should set a flag or context that causes all descendant elements to automatically inherit the modal's render layer. This could be:

1. **Inherited render layer**: If a parent has a render layer set, children without an explicit layer inherit it
2. **Modal context**: The modal content callback receives a context that auto-applies the layer
3. **Content container flag**: The modal content container has a `propagate_render_layer` flag

## Notes

- This requires changes to afterhours (not solvable with project-level helpers)
- Affects ModalShowcase and any screen that uses modals
- Low priority since modals are relatively rare compared to other patterns
- Approved during vendor feature review session
