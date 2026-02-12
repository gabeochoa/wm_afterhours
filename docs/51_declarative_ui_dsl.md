# Declarative DSL for UI Layout

TODO: Investigate whether the UI could be driven by a declarative scripting language embedded in multiline strings, with references to real C++ code.

## Concept

Instead of building UI trees procedurally in C++, define layouts in a DSL:

```
div {
    button {
        background-color: red,
        onClick: my_cpp_function()
    }
}
```

The DSL would be parsed at runtime (or compile time via constexpr) and bind to actual C++ functions, styles, and state.

## Open Questions

- **Ergonomics**: Does wrapping everything in string literals feel natural, or does it fight the type system? Is the loss of autocomplete/refactoring tooling worth it?
- **Function binding**: How do C++ functions get registered and referenced by name? A macro-based registry? Manual registration?
- **Type safety**: Can we catch mismatches (wrong argument types, missing handlers) at compile time, or only at runtime?
- **State and reactivity**: How does the DSL reference and react to changing state? Inline expressions? Bindings?
- **Styling**: Flat properties on elements (CSS-like) vs. a separate style system?
- **Nesting and composition**: Can DSL fragments be composed or included from other fragments?
- **Debugging**: How do you step through or set breakpoints in DSL-defined UI?
- **Performance**: Parsing overhead vs. procedural construction. Can the DSL be compiled to a static tree?

## Tradeoffs to Evaluate

- **Pro**: Concise, visual layout definition; easy to tweak without recompiling logic; familiar to anyone who knows HTML/CSS/JSX.
- **Con**: Another language to learn; potential loss of compile-time guarantees; tooling gap (no LSP, no autocomplete in string literals); every callback needs to be a named, registered function which could be annoying for one-off handlers.

## Prior Art to Look At

- Qt QML
- Dear ImGui markup proposals
- Flutter's widget tree (declarative but in Dart, not a DSL)
- SwiftUI (declarative but compiler-integrated)
- XAML (WPF/UWP)
- Slint (Rust/C++ declarative UI)
