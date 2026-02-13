# Riot.js Analysis for afterhours

## Overview

[Riot.js](https://riot.js.org) is a minimalist component-based UI library (~6KB gzipped)
that emphasizes simplicity, small API surface, and closeness to web standards. Unlike
libraries like Radix or Base UI that provide pre-built widget primitives, Riot is a
**component framework** — it defines *how* you build and compose components, not *what*
components you get.

Riot's value for afterhours is not in its component catalog (it has none) but in its
**architectural patterns**: its component lifecycle, plugin system, props/state
separation, slot mechanism, and update optimization. These are directly applicable to
afterhours' immediate-mode ECS component system.

---

## 1. Core Architecture

### 1.1 Component Model

A Riot component is defined by three parts:
- **Template** (HTML layout)
- **Script** (JavaScript logic)
- **Style** (scoped CSS)

The component exports an object with state, methods, and lifecycle callbacks:

```javascript
export default {
  // initial state
  onBeforeMount(props, state) {
    this.state = { count: props.start || 0 }
  },
  // methods
  increment() {
    this.update({ count: this.state.count + 1 })
  },
  // lifecycle
  onMounted(props, state) { /* DOM ready */ },
  onUpdated(props, state) { /* after re-render */ },
  onUnmounted(props, state) { /* cleanup */ }
}
```

### 1.2 Props vs State Separation

Riot enforces a strict separation:

| Aspect | Props | State |
|--------|-------|-------|
| Mutability | **Immutable** (`Object.freeze()`) | Mutable via `this.update()` |
| Source | Parent component or mount call | Internal to the component |
| Update flow | Parent → child (unidirectional) | Component calls `this.update(newState)` |
| Reactivity | Frozen; changes require parent re-render | Triggers re-render on `update()` |

**Key insight:** Props are frozen with `Object.freeze()`. The component cannot modify
its own props — only the parent can change them. This enforces one-way data flow and
prevents accidental mutation bugs.

### 1.3 Update Mechanism

Riot uses **explicit updates** rather than automatic reactivity:

```javascript
// Does NOT trigger re-render:
this.state.name = 'Jack'

// DOES trigger re-render:
this.update({ name: 'Jack' })
```

Updates propagate **downward only**: parent → children. There is no upward bubbling of
state changes. A parent updating causes all its children to update as well.

Riot also provides a `shouldUpdate(newProps, currentProps)` method that components can
override to skip unnecessary re-renders:

```javascript
export default {
  shouldUpdate(newProps, currentProps) {
    // Only re-render if the title actually changed
    return newProps.title !== currentProps.title
  }
}
```

---

## 2. Component Lifecycle

Riot defines 6 lifecycle callbacks, called in this order:

```
Creation:
  1. onBeforeMount(props, state)  — before first render
  2. onMounted(props, state)      — after first render, DOM accessible

Updates:
  3. onBeforeUpdate(props, state) — before re-render
  4. onUpdated(props, state)      — after re-render

Destruction:
  5. onBeforeUnmount(props, state) — before removal from DOM
  6. onUnmounted(props, state)     — after removal, cleanup
```

Each callback receives the current `props` and `state` as arguments.

### Lifecycle Sequence

```
mount(element) →
  freeze(props) →
  onBeforeMount() →
  render template →
  onMounted()

update(newState) →
  shouldUpdate()? →
  merge state →
  onBeforeUpdate() →
  re-render template →
  onUpdated()

unmount() →
  onBeforeUnmount() →
  remove DOM →
  onUnmounted()
```

---

## 3. Plugin System

Riot's plugin system is one of its most elegant features. A plugin is a function that
receives every component instance at creation time and can enhance it:

```javascript
// Install a plugin that adds a unique ID to every component
let id = 0
riot.install(function(component) {
  component.uid = id++
  return component
})

// Now every component has a .uid property
```

Plugins can:
- Add properties/methods to all components
- Wrap lifecycle callbacks
- Add observable patterns
- Inject shared utilities (classNames, styleAttribute, etc.)
- Add logging/profiling

**The plugin runs at instantiation time**, not at definition time. This means plugins
can access the component's props and initial state.

### Plugin Examples from Riot Ecosystem

```javascript
// 1. Observable pattern plugin (event emitter on components)
riot.install(function(component) {
  observable(component)  // adds .on(), .off(), .trigger()
  return component
})

// 2. Class names helper plugin
riot.install(function(component) {
  component.classNames = (obj) =>
    Object.entries(obj)
      .filter(([_, v]) => v)
      .map(([k]) => k)
      .join(' ')
  return component
})

// 3. Logging plugin
riot.install(function(component) {
  const origMount = component.onMounted
  component.onMounted = function(...args) {
    console.log(`[${component.name}] mounted`)
    return origMount?.apply(this, args)
  }
  return component
})
```

---

## 4. Slots (Content Projection)

Riot supports **slots** — named placeholders where parent components inject content
into child component templates:

```html
<!-- Child component: greeting.riot -->
<greeting>
  <p>Hello <slot/></p>
</greeting>

<!-- Parent component: uses greeting -->
<user>
  <greeting>
    <b>{ text }</b>
  </greeting>
</user>

<!-- Renders: <p>Hello <b>World</b></p> -->
```

Slots allow a component to define its **structure** while letting the consumer define
specific **content** areas. This is a form of inversion of control for UI composition.

---

## 5. Conditional and Loop Directives

### Conditionals

```html
<div if={ isPremium }>
  <p>Premium content</p>
</div>
```

The `if` directive **mounts/unmounts** the element and its children — not just
show/hide. This means nested components get their full lifecycle (mount/unmount)
when the condition changes.

### Loops with Keys

```html
<li each={ item in items } key={ item.id }>
  { item.title }
</li>
```

The `key` attribute enables efficient list reconciliation: Riot can track which items
moved, were added, or were removed, and minimize DOM operations. Without `key`, Riot
must recreate all loop items on every update.

### Fragment Rendering

```html
<template if={ isReady }>
  <header></header>
  <main></main>
  <footer></footer>
</template>
```

The `<template>` tag acts as an invisible wrapper — it renders its children without
adding an extra DOM node. This avoids unnecessary wrapper elements.

---

## 6. Pure Components

Riot's `pure()` function provides an escape hatch for full manual control over
rendering. Pure components bypass Riot's template engine entirely:

```javascript
export default pure(() => ({
  mount(el, context) {
    this.el = el
    this.el.innerHTML = 'Hello There'
  },
  update(context) {
    this.el.innerHTML = 'I got updated!'
  },
  unmount() {
    this.el.parentNode.removeChild(this.el)
  }
}))
```

Pure components implement only 3 methods: `mount`, `update`, `unmount`. They receive
no automatic template binding, no expression evaluation, no lifecycle hooks beyond
these three. This is useful for:
- Integrating third-party rendering libraries
- Performance-critical components
- Custom rendering strategies (canvas, WebGL, etc.)

---

## 7. Component Registration and Composition

### Global Registry

```javascript
riot.register('my-component', MyComponent)  // register globally
riot.mount('my-component')                   // mount all instances

riot.unregister('my-component')              // remove from registry
```

### Local Composition

```html
<parent-component>
  <child-component/>

  <script>
    import ChildComponent from './child-component.riot'
    export default {
      components: { ChildComponent }
    }
  </script>
</parent-component>
```

Components can be imported locally (only available to the importing component) or
registered globally (available everywhere). Local imports are preferred for
encapsulation.

### Dynamic Components

```html
<div is={ currentComponent }></div>

<script>
  export default {
    currentComponent: 'dog',
    switchComponent() {
      this.currentComponent = 'cat'
      this.update()
    }
  }
</script>
```

The `is` attribute dynamically swaps which component is rendered on a DOM node. Riot
handles unmounting the old component and mounting the new one.

---

## 8. Ecosystem Modules

Riot's ecosystem is fully modular — each piece is a separate package:

| Package | Purpose | afterhours Relevance |
|---------|---------|---------------------|
| `@riotjs/route` | Client-side routing with path matching | Screen/page navigation |
| `@riotjs/lazy` | Lazy-load components on demand | Deferred component loading |
| `@riotjs/ssr` | Server-side rendering | Headless screenshot rendering |
| `@riotjs/hydrate` | Hydrate SSR markup with interactivity | N/A (web-specific) |
| `@riotjs/dom-bindings` | Expression-based template engine | Template/expression system |
| `@riotjs/compiler` | Compile .riot files to JavaScript | N/A (web-specific) |
| `@riotjs/custom-elements` | Native Web Components wrapper | N/A (web-specific) |

---

## 9. Comparison with afterhours

### Architecture Comparison

| Aspect | Riot.js | afterhours |
|--------|---------|------------|
| Paradigm | Retained-mode, reactive updates | Immediate-mode, rebuild each frame |
| Component definition | `.riot` file (HTML + JS + CSS) | C++ function returning `ElementResult` |
| State management | `this.state` + `this.update()` | ECS components on entities |
| Props | Frozen object from parent | `ComponentConfig` struct |
| Rendering | DOM diffing/patching | RenderCommandBuffer per frame |
| Lifecycle | 6 explicit callbacks | Implicit (frame-by-frame) |
| Styling | Scoped CSS per component | `Theme` + `ComponentConfig` colors |
| Composition | Nesting + slots | Entity parent/child + `mk()` |
| Plugin system | `riot.install()` global enhancer | **None** |
| Update optimization | `shouldUpdate()` skip | **None** (always rebuilds) |
| Component registry | Global name → factory map | **None** (direct function calls) |

### What afterhours Already Has

| Riot Feature | afterhours Equivalent |
|-------------|----------------------|
| Props (frozen config from parent) | `ComponentConfig` (mutable, but passed by value) |
| State (internal mutable data) | ECS components (`HasColor`, `HasLabel`, etc.) |
| Template expressions | C++ expressions in immediate-mode calls |
| Conditional rendering | `if` statements in C++ |
| Loop rendering | `for` loops in C++ |
| Scoped styling | `Theme` system with per-component overrides |
| DOM queries (`$`, `$$`) | `EntityQuery` |
| Event handling | ECS input systems (`HasClickedOn`, etc.) |
| Nested components | `mk(parent, index)` for parent/child relationships |

---

## 10. Recommendations for afterhours

### 10.1 Plugin System (High Priority)

**Riot pattern:** `riot.install(fn)` — a function that enhances every component at
creation time.

**afterhours adoption:** A plugin/middleware system that can intercept component
creation and add behavior. This would formalize what currently requires manual
boilerplate:

```cpp
namespace afterhours::ui::imm {

using ComponentPlugin = std::function<void(Entity&, const ComponentConfig&)>;

// Global plugin registry
inline std::vector<ComponentPlugin>& get_plugins() {
    static std::vector<ComponentPlugin> plugins;
    return plugins;
}

// Install a plugin that runs on every component creation
inline void install(ComponentPlugin plugin) {
    get_plugins().push_back(std::move(plugin));
}

// Uninstall a plugin
inline void uninstall(size_t index) {
    auto& plugins = get_plugins();
    if (index < plugins.size()) plugins.erase(plugins.begin() + index);
}

// Called internally during component_init:
inline void run_plugins(Entity& entity, const ComponentConfig& config) {
    for (auto& plugin : get_plugins()) {
        plugin(entity, config);
    }
}

} // namespace afterhours::ui::imm
```

**Use cases:**
- Auto-add accessibility labels to all interactive components
- Auto-add debug borders/overlays
- Auto-log component creation for profiling
- Auto-apply animation presets to all components
- Auto-enforce minimum touch target sizes

```cpp
// Example: plugin that auto-adds focus ring to all interactive components
afterhours::ui::imm::install([](Entity& e, const ComponentConfig& cfg) {
    if (e.has<HasClickHandler>()) {
        e.addComponent<FocusRingStyle>(colors::info(), 2.0f);
    }
});
```

### 10.2 shouldUpdate Optimization (High Priority)

**Riot pattern:** Components can return `false` from `shouldUpdate()` to skip
re-rendering when nothing relevant changed.

**afterhours adoption:** In immediate-mode UI, the entire tree is rebuilt every frame.
This is simple but wasteful for static or rarely-changing content. A `shouldUpdate`
mechanism could cache previous frame's render output and skip re-rendering unchanged
subtrees:

```cpp
struct CachedUI : BaseComponent {
    size_t config_hash = 0;  // Hash of the ComponentConfig that produced this
    size_t state_hash = 0;   // Hash of relevant state
    bool dirty = true;       // Force update flag

    bool should_update(size_t new_config_hash, size_t new_state_hash) const {
        return dirty || config_hash != new_config_hash ||
               state_hash != new_state_hash;
    }
};
```

This is particularly valuable for:
- Large lists with many items (only re-render changed items)
- Complex panels that rarely change (settings, inventory grids)
- Text-heavy UIs where text measurement is expensive

### 10.3 Slots / Content Projection (High Priority)

**Riot pattern:** `<slot/>` tags let parent components inject content into
predefined areas of child components.

**afterhours adoption:** A slot mechanism would enable reusable container components
that define layout/styling while letting consumers define content:

```cpp
// Define a card component with header, body, and footer slots
auto card_begin(HasUIContext auto& ctx, EntityParent ep,
                ComponentConfig config = {}) -> ElementResult;

// Usage:
auto card = card_begin(ctx, mk(parent, 0), card_config);
{
    // "header" slot content
    slot(ctx, card, "header", [&](auto& ctx, auto parent) {
        text(ctx, mk(parent, 0), "Card Title");
    });

    // "body" slot content
    slot(ctx, card, "body", [&](auto& ctx, auto parent) {
        text(ctx, mk(parent, 0), "Card body content...");
        button(ctx, mk(parent, 1), btn_config);
    });
}
```

Currently afterhours achieves this through raw nesting, but a formal slot mechanism
would:
- Document which content areas a component expects
- Enforce slot constraints (e.g., "header" must be a single element)
- Allow default slot content that the consumer can override

### 10.4 Props Immutability / Frozen Config (Medium Priority)

**Riot pattern:** `Object.freeze(props)` prevents components from modifying their
own props.

**afterhours adoption:** `ComponentConfig` is currently a plain struct that can be
modified anywhere. Making it conceptually immutable after component creation would
prevent bugs where a component modifies its config and causes inconsistent rendering:

```cpp
// ComponentConfig could be stored as const after initial setup:
struct UIComponent : BaseComponent {
    const ComponentConfig initial_config;  // Frozen at creation time
    // ... mutable runtime state (position, size, etc.)
};
```

This doesn't need to be enforced at the type level (C++ `const` is sufficient), but
the pattern of "config in, don't modify" should be documented and encouraged.

### 10.5 Component Registry (Medium Priority)

**Riot pattern:** `riot.register('name', Component)` creates a global name → factory
mapping. Components can be looked up and instantiated by name.

**afterhours adoption:** A component registry would enable:
- Data-driven UI construction (build UI from JSON/config files)
- Dynamic component swapping (like Riot's `is` attribute)
- Theme-level component overrides (register a custom button factory)

```cpp
namespace afterhours::ui::registry {

using ComponentFactory = std::function<ElementResult(
    HasUIContext auto&, EntityParent, ComponentConfig)>;

inline std::map<std::string, ComponentFactory>& get_registry() {
    static std::map<std::string, ComponentFactory> reg;
    return reg;
}

inline void register_component(const std::string& name,
                               ComponentFactory factory) {
    get_registry()[name] = std::move(factory);
}

// Mount by name:
inline ElementResult mount(const std::string& name,
                           HasUIContext auto& ctx, EntityParent ep,
                           ComponentConfig config) {
    auto& reg = get_registry();
    auto it = reg.find(name);
    if (it != reg.end()) return it->second(ctx, ep, config);
    // fallback: render error placeholder
}

} // namespace
```

**Use cases:**
- Build screens from data (JSON screen definitions)
- Replace default components with themed variants
- Plugin-provided custom components
- Dynamic content that varies by game state

### 10.6 Keyed List Reconciliation (Medium Priority)

**Riot pattern:** `each={ item in items } key={ item.id }` tracks items by key for
efficient add/remove/reorder operations.

**afterhours adoption:** When rendering lists of ECS entities (inventory items,
player lists, chat messages), afterhours currently uses positional indexing via
`mk(parent, index)`. If items are reordered or removed from the middle, all subsequent
entities get new IDs, losing any associated state (animation progress, scroll position,
selection state).

A key-based approach would map stable keys to entity IDs:

```cpp
// Current: positional indexing (fragile for dynamic lists)
for (int i = 0; i < items.size(); i++) {
    button(ctx, mk(parent, i), config.with_label(items[i].name));
}

// Proposed: keyed indexing (stable across reorders)
for (auto& item : items) {
    button(ctx, mk_keyed(parent, item.id), config.with_label(item.name));
}
```

`mk_keyed` would maintain a stable entity-ID ↔ key mapping across frames, so
reordering items doesn't destroy and recreate entities.

### 10.7 Lifecycle Hooks (Lower Priority)

**Riot pattern:** 6 lifecycle callbacks (onBeforeMount, onMounted, onBeforeUpdate,
onUpdated, onBeforeUnmount, onUnmounted).

**afterhours context:** Immediate-mode UI rebuilds every frame, so traditional mount/
unmount lifecycles don't apply the same way. However, afterhours *does* have
entity creation and cleanup, which map to mount/unmount:

```cpp
// Entity creation hooks (run once when entity first appears):
struct OnFirstFrame : BaseComponent {
    std::function<void(Entity&)> callback;
    bool has_run = false;
};

// Entity cleanup hooks (run when entity is about to be removed):
struct OnCleanup : BaseComponent {
    std::function<void(Entity&)> callback;
};
```

These are most useful for:
- Starting animations on component appearance
- Loading resources on first render
- Cleaning up timers/subscriptions on removal
- Fade-in/fade-out transitions

### 10.8 Dynamic Component Swapping (Lower Priority)

**Riot pattern:** `<div is={ componentName }>` dynamically renders different components
on the same DOM node based on a runtime value.

**afterhours adoption:** Combined with a component registry, this enables data-driven
UIs:

```cpp
// Render whatever component "widget_type" specifies
auto widget_type = game_state.current_widget; // "inventory" or "shop" or "stats"
registry::mount(widget_type, ctx, mk(parent, 0), config);
```

This is useful for:
- Tab panels where each tab shows a different component
- Game screens that swap based on state
- Configurable dashboards

---

## 11. What NOT to Adopt

| Riot Feature | Reason to Skip |
|-------------|----------------|
| Reactive template expressions | afterhours uses C++ expressions directly; no need for a template language |
| Virtual DOM diffing | afterhours is immediate-mode; no retained DOM to diff |
| `.riot` file format / compiler | afterhours components are C++ functions |
| Scoped CSS injection | afterhours uses `Theme` + `ComponentConfig` for styling |
| HTML-based component definition | Not applicable to native UI |
| SSR / Hydration | Web-specific concepts |
| Web Components interop | Web-specific |
| `$()` / `$$()` DOM selectors | afterhours uses `EntityQuery` |

---

## 12. Key Architectural Lessons

### Lesson 1: Minimal API Surface

Riot's entire core API is ~10 functions: mount, unmount, component, install, uninstall,
register, unregister, pure, version. Everything else is built on top via plugins.

**Takeaway:** afterhours should resist adding special-case features to the core. Instead,
provide a plugin mechanism (recommendation 10.1) that lets features be added externally.
Validation rules, debug overlays, accessibility enhancements — these should all be
plugins, not core features.

### Lesson 2: Explicit Over Magic

Riot v4 removed "magic" reactive properties in favor of explicit `this.update()` calls.
Their philosophy: *"In the face of ambiguity, refuse the temptation to guess."*

**Takeaway:** afterhours' immediate-mode approach already embodies this — every frame is
explicitly rebuilt. Don't add implicit reactivity or automatic dirty-checking. If caching
is added (shouldUpdate), make it opt-in per component, not a global default.

### Lesson 3: Props Immutability Prevents Bugs

Freezing props ensures components can't accidentally modify their configuration, which
would create hard-to-debug rendering inconsistencies.

**Takeaway:** afterhours' `ComponentConfig` should be treated as immutable after it's
passed to a component. Consider documenting this as a convention, or storing configs as
`const` in the ECS.

### Lesson 4: Plugins Beat Inheritance

Riot's plugin system (`riot.install`) is more flexible than class inheritance for adding
cross-cutting concerns. A single plugin can add behavior to every component type without
modifying any component definition.

**Takeaway:** Instead of adding more optional fields to `ComponentConfig` for every new
feature, provide a plugin hook that runs at component initialization. This keeps
`ComponentConfig` focused and avoids bloat.

### Lesson 5: Composition Over Configuration

Riot's slots + nesting model encourages building complex UIs by composing simple
components, rather than configuring a single complex component.

**Takeaway:** afterhours should prefer many small composable components over fewer large
ones with many config options. A `card()` with named slots is more flexible than a
`card()` with `header_text`, `body_text`, `footer_text`, `header_icon`, etc. parameters.

---

## 13. Priority Summary

| # | Feature | Effort | Impact | Description |
|---|---------|--------|--------|-------------|
| 1 | **Plugin system** | Medium | High | `install(fn)` to enhance all components |
| 2 | **shouldUpdate cache** | Medium | High | Skip re-rendering unchanged subtrees |
| 3 | **Slots / content projection** | Medium | High | Named content areas in container components |
| 4 | **Keyed list reconciliation** | Medium | Medium | Stable entity IDs across list reorders |
| 5 | **Component registry** | Low | Medium | Name → factory mapping for dynamic UIs |
| 6 | **Props immutability** | Low | Low | Convention: don't modify config after creation |
| 7 | **Lifecycle hooks** | Low | Low | OnFirstFrame / OnCleanup callbacks |
| 8 | **Dynamic component swap** | Low | Low | Registry-based runtime component selection |
