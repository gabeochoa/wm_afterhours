# Feature Request: sprite() Full-Texture Overload

**Type:** Afterhours vendor change
**File:** `vendor/afterhours/src/plugins/ui/imm_components.h`
**Decision:** YES -- approved for implementation
**Impact:** ~40 call sites across 3+ screens

## Problem

Every `sprite()` call requires manually constructing a `Rectangle` source rect, even when you just want the full texture (which is ~90% of cases):

```cpp
afterhours::texture_manager::Rectangle src{0, 0, (float)tex.width, (float)tex.height};
sprite(context, mk(entity, 6), tex, src, config);
```

This 2-line boilerplate is error-prone (easy to get width/height wrong) and adds visual noise.

## Proposed Solution

Add an overload of `sprite()` that omits `source_rect` and defaults to the full texture:

```cpp
// New overload in imm_components.h, alongside existing sprite():
ElementResult sprite(HasUIContext auto &ctx, EntityParent ep_pair,
                     afterhours::texture_manager::Texture texture,
                     ComponentConfig config = ComponentConfig()) {
    afterhours::texture_manager::Rectangle full_rect{
        0, 0, (float)texture.width, (float)texture.height};
    return sprite(ctx, ep_pair, texture, full_rect, config);
}
```

## Usage

```cpp
// Before (2 lines):
afterhours::texture_manager::Rectangle src{0, 0, (float)tex.width, (float)tex.height};
sprite(context, mk(entity, 6), tex, src, config);

// After (1 line):
sprite(context, mk(entity, 6), tex, config);
```

## Screens that benefit

- EmpireTycoon (4 full-texture sprite calls)
- CozyCafe (9 full-texture sprite calls)
- NeonStrike (11 full-texture sprite calls)
- ImageShowcase (5 full-texture sprite calls)

## Notes

- The existing 4-arg `sprite()` remains for partial-texture/spritesheet use cases
- No breaking changes
- Trivial implementation -- just delegates to the existing function
