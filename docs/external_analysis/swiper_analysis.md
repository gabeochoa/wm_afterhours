# Swiper Analysis

What Swiper provides, how it architects a highly customizable slide/carousel
system, and what patterns afterhours can adopt for its own carousel and
scrollable content components.

Repository: https://github.com/nolimits4web/swiper
Docs: https://swiperjs.com/swiper-api

---

## 1. What Swiper Is

Swiper is the most popular touch slider library, focused on mobile-first
swiping and sliding interactions with hardware-accelerated transitions.  It's
**not** a general UI component library — it does one thing (sliding between
content panels) and does it exceptionally well.

Why study it:
- The **Carousel** component was identified as a gap in all prior analyses
  (Bootstrap, Base UI, Radix)
- Swiper's module architecture is a textbook example of **composable
  behaviors** — exactly the pattern afterhours needs
- Touch/pointer handling, momentum physics, and snapping are directly
  applicable to a native game UI

---

## 2. Core Architecture

### 2.1 Single Class + Mixin Prototypes

The core `Swiper` class has its methods composed from separate modules via
prototype assignment:

```javascript
const prototypes = {
    eventsEmitter,  // on/off/emit/once
    update,         // recalculate sizes
    translate,      // get/set translation offset
    transition,     // set duration, start/end callbacks
    slide,          // slideTo, slideNext, slidePrev
    loop,           // loop duplication logic
    grabCursor,     // cursor: grab/grabbing
    events,         // pointer/touch event attachment
    breakpoints,    // responsive config switching
    checkOverflow,  // disable when content fits
    classes,        // CSS class management
};
// Each group's methods are assigned directly to Swiper.prototype
```

**afterhours parallel:** This is essentially a manual version of ECS — behavior
groups (translate, transition, slide) are independent modules mixed into one
entity.  In afterhours, these would be separate systems operating on shared
component data.

### 2.2 Plugin Module System

On top of the core prototypes, Swiper has 23 **opt-in modules** that register
via a function receiving `{ swiper, extendParams, on, emit }`:

```javascript
function Navigation({ swiper, extendParams, on, emit }) {
    extendParams({
        navigation: { nextEl: null, prevEl: null, ... }
    });
    on('init', () => { /* setup */ });
    on('destroy', () => { /* cleanup */ });
}
```

Each module:
1. **Declares its config** via `extendParams()` — merges into the main options
2. **Hooks into lifecycle events** via `on()` — init, destroy, slideChange, etc.
3. **Emits its own events** via `emit()` — navigationNext, paginationUpdate, etc.
4. **Attaches state** to `swiper.*` — `swiper.navigation`, `swiper.pagination`

This is tree-shakeable: unused modules are never imported.

**afterhours parallel:** This maps directly to how afterhours systems could
work.  A "carousel" entity could have optional NavigationSystem,
PaginationSystem, AutoplaySystem, etc. that subscribe to shared events.

### 2.3 Event Emitter

Swiper has its own event emitter built in:

```javascript
swiper.on('slideChange', (s) => { ... });
swiper.on('transitionStart', (s) => { ... });
swiper.once('init', (s) => { ... });
swiper.onAny((eventName, ...data) => { ... });  // catch-all
swiper.off('slideChange', handler);
```

Notable: `onAny()` provides a catch-all listener — useful for debugging
or logging.  All events pass the swiper instance as the first argument.

Full event list (core):

| Event | When |
|---|---|
| `beforeInit` | Before initialization |
| `init` | After initialization |
| `beforeDestroy` / `destroy` | Cleanup |
| `slideChange` | Active slide changes |
| `slideChangeTransitionStart` / `End` | Transition animation |
| `transitionStart` / `transitionEnd` | Any translation animation |
| `setTranslate` | Translation value changes |
| `setTransition` | Transition duration changes |
| `touchStart` / `touchMove` / `touchEnd` | Pointer interactions |
| `click` / `tap` / `doubleTap` | Click gestures |
| `reachBeginning` / `reachEnd` | Edge reached |
| `progress` | Progress value updates (0 to 1) |
| `beforeResize` / `resize` | Container size changes |
| `lock` / `unlock` | Overflow changes |
| `update` | After any recalculation |
| `enable` / `disable` | Swiper enabled/disabled |

---

## 3. Module Inventory

### 3.1 Navigation Modules

| Module | Purpose | afterhours Relevance |
|---|---|---|
| **Navigation** | Prev/next arrow buttons | Direct: arrow buttons on carousel |
| **Pagination** | Bullet dots, progress bar, fraction (e.g. "3/10") | Direct: slide indicators |
| **Scrollbar** | Draggable scrollbar track | Already have: `scrollbar()` |
| **Keyboard** | Arrow/PgUp/PgDown key control | Direct: keyboard nav for carousel |
| **Mousewheel** | Scroll wheel to advance slides | Direct: mousewheel carousel control |

### 3.2 Behavior Modules

| Module | Purpose | afterhours Relevance |
|---|---|---|
| **Autoplay** | Timer-based auto-advance with pause-on-hover | Direct: auto-rotating content |
| **Controller** | Two-way sync between multiple swipers | Medium: linked scroll views |
| **Free Mode** | No snapping, momentum scrolling with velocity | High: momentum physics for scroll |
| **Loop** | Infinite loop by cloning edge slides | Direct: infinite carousel |
| **Virtual** | Only renders visible slides + buffer | High: virtual scrolling pattern |
| **Thumbs** | Thumbnail gallery synced to main slider | Medium: gallery selection UI |

### 3.3 Visual Effect Modules

| Module | Purpose | afterhours Relevance |
|---|---|---|
| **Fade** | Crossfade between slides | High: transition effect |
| **Cube** | 3D cube rotation between slides | Medium: 3D transform support |
| **Coverflow** | 3D coverflow (like album art) | High: great for item showcases |
| **Flip** | 3D card flip transition | Medium: card flip animation |
| **Cards** | Stacked card swipe (like Tinder) | Medium: card-stack UI |
| **Creative** | Custom per-slide translate/rotate/scale/opacity | High: fully programmable transitions |

### 3.4 Layout Modules

| Module | Purpose | afterhours Relevance |
|---|---|---|
| **Grid** | Multi-row slide grid (e.g. 3x2) | High: grid carousel layout |
| **Parallax** | Per-element parallax on scroll progress | High: depth/polish effects |
| **Zoom** | Pinch-to-zoom on slide content | Medium: image/map viewers |

### 3.5 Web-Specific Modules (Skip)

| Module | Purpose | Why Skip |
|---|---|---|
| **A11y** | ARIA attributes, live regions | Web-specific (ARIA) |
| **Hash Navigation** | URL hash synced to active slide | Web-specific |
| **History** | Browser history API integration | Web-specific |
| **Manipulation** | DOM slide add/remove/prepend/append | DOM-specific |

---

## 4. Key Concepts for afterhours

### 4.1 Translate + Snap Grid

Swiper's core model is simple:

```
translate: float  — the current scroll offset (pixels)
slidesGrid: []    — positions where each slide starts
snapGrid: []      — positions where the view can snap to
progress: float   — 0.0 (start) to 1.0 (end)
velocity: float   — current swipe velocity
```

Sliding is just an animated `translate` value that snaps to the nearest
`snapGrid` position.  This is directly implementable in afterhours:

```cpp
struct CarouselState {
    float translate = 0;        // Current scroll offset
    float velocity = 0;         // Momentum velocity
    float progress = 0;         // 0..1 normalized position
    bool animating = false;

    std::vector<float> slides_grid;   // Slide start positions
    std::vector<float> snap_grid;     // Snap points

    int active_index = 0;
    int real_index = 0;        // For loop mode
    bool is_beginning = true;
    bool is_end = false;
};
```

### 4.2 Momentum / Free Mode Physics

Swiper's free mode calculates momentum from touch velocity:

```javascript
// Simplified from free-mode.mjs
const velocity = (currentPos - previousPos) / (currentTime - previousTime);
const momentumDistance = velocity * velocity / (2 * deceleration);
const newPosition = currentPos + momentumDistance * sign(velocity);
// Animate to newPosition with easeOut
```

Parameters:
- `momentumRatio: 1` — multiplier on momentum distance
- `momentumBounce: true` — rubber-band at edges
- `momentumBounceRatio: 1` — bounce elasticity
- `minimumVelocity: 0.02` — ignore tiny swipes

**afterhours adoption:** This momentum model is identical to what scroll views
need.  The same physics can power:
- Carousel free-scroll mode
- `scroll_area()` momentum scrolling
- List flick-scroll behavior
- Map/canvas panning

### 4.3 Resistance (Rubber-Banding)

When swiping past the edge:

```javascript
resistanceRatio: 0.85  // 0 = no resistance, 1 = no overscroll
// Actual resistance: 1 - (overscroll / containerSize)^resistanceRatio
```

This is the iOS-style rubber-band effect.  Essential for polished touch UIs.

### 4.4 Direction Abstraction

Swiper handles horizontal and vertical with a single abstraction:

```javascript
getDirectionLabel(property) {
    if (this.isHorizontal()) return property;
    return {
        'width': 'height',
        'margin-left': 'margin-top',
        'padding-left': 'padding-top',
        ...
    }[property];
}
```

All internal code uses the horizontal terminology (`width`, `translateX`),
and this mapper converts for vertical mode.  This means every feature
automatically works in both orientations.

**afterhours adoption:** For carousel/slider, accept a `Direction` enum
and internally always work in the primary axis, converting via helper.

### 4.5 Virtual Slides

For large datasets, Swiper only renders slides in a window around the
active index:

```
[buffer] [visible viewport] [buffer]
  ^                                ^
  addSlidesBefore              addSlidesAfter
```

Slides outside this window are not in the render tree.  A `renderSlide`
callback creates slide content on demand.  Cached by index.

**afterhours adoption:** This is the same virtual scrolling pattern needed
for large lists.  Shared implementation between carousel and scroll view:

```cpp
struct VirtualContent {
    int total_items;
    int buffer_before = 1;
    int buffer_after = 1;
    int visible_start = 0;  // First rendered index
    int visible_end = 0;    // Last rendered index
    std::unordered_map<int, CachedItem> cache;

    // Only items in [visible_start - buffer, visible_end + buffer]
    // are actually rendered
};
```

### 4.6 Transition Effects

Each effect module provides two functions:

```javascript
function EffectFade({ swiper, on }) {
    const setTranslate = () => {
        // For each slide: compute opacity/transform from slide.progress
        targetEl.style.opacity = 1 + Math.min(Math.max(slide.progress, -1), 0);
        targetEl.style.transform = `translate3d(${tx}px, ${ty}px, 0px)`;
    };
    const setTransition = (duration) => {
        // Set CSS transition duration on each slide
        el.style.transitionDuration = `${duration}ms`;
    };
    effectInit({ effect: 'fade', swiper, on, setTranslate, setTransition });
}
```

The key insight: **every effect is just a function that maps `slide.progress`
(-1..0..1) to visual properties** (translate, rotate, scale, opacity).

| Effect | Transform per slide.progress |
|---|---|
| **Slide** (default) | translateX = progress * slideWidth |
| **Fade** | opacity = 1 - |progress| |
| **Cube** | rotateY = 90 * progress, translateZ = -half |
| **Coverflow** | rotateY = angle * progress, translateZ = -depth * |progress|, scale |
| **Flip** | rotateY = 180 * progress |
| **Cards** | translateY = |progress| * gap, scale = 1 - |progress| * 0.1 |
| **Creative** | User-defined translate/rotate/scale/opacity for prev/next |

**afterhours adoption:** Define a `CarouselEffect` interface:

```cpp
struct CarouselEffect {
    // Given slide progress (-1 to 1), return visual transform
    virtual Transform compute(float progress, vec2 slide_size) = 0;

    struct Transform {
        vec3 translate = {0, 0, 0};
        vec3 rotate = {0, 0, 0};    // degrees
        float scale = 1.0f;
        float opacity = 1.0f;
    };
};

// Built-in effects
struct SlideEffect : CarouselEffect { ... };
struct FadeEffect : CarouselEffect { ... };
struct CoverflowEffect : CarouselEffect { ... };
struct CreativeEffect : CarouselEffect {
    Transform prev_config;  // Target transform for previous slide
    Transform next_config;  // Target transform for next slide
    // Lerp between them based on progress
};
```

### 4.7 Parallax

Swiper's parallax module adds depth by moving child elements at different
rates relative to slide progress:

```
data-swiper-parallax="-300"       // translateX by -300px * progress
data-swiper-parallax-opacity="0"  // Fade from 1 to 0 across progress
data-swiper-parallax-scale="0.5"  // Scale from 1 to 0.5 across progress
data-swiper-parallax-rotate="45"  // Rotate by 45deg * progress
```

Each element gets its own `duration` that can differ from the main slide
transition.

**afterhours adoption:** Could be a decorator or a system that applies
progress-dependent transforms to children of a carousel slide:

```cpp
// Example usage
carousel_slide([&]() {
    // Background moves slower (parallax -50%)
    with_parallax(-0.5f, [&]() { background_image(bg); });
    // Title moves faster (parallax +20%)
    with_parallax(0.2f, [&]() { text("Level 3: The Cave"); });
    // Subtitle fades in
    with_parallax_opacity(0.0f, 1.0f, [&]() { text("Unlock at rank 10"); });
});
```

### 4.8 Two-Way Controller

The Controller module syncs multiple Swiper instances using linear
interpolation:

```javascript
// When main swiper moves, controller updates controlled swipers
function LinearSpline(x, y) {
    // Binary search + interpolation to map position
    // from one swiper's snap grid to another's
}
```

Modes:
- `by: 'slide'` — sync by slide index
- `by: 'container'` — sync by percentage of total scroll

Use case: thumbnail strip + main view (like a photo gallery), or linked
scroll views.

**afterhours adoption:** For a gallery component where a thumbnail strip
and main view are linked:

```cpp
struct CarouselController {
    // List of linked carousels
    std::vector<EntityID> controlled;
    SyncMode mode = SyncMode::BySlide;  // or ByProgress
    bool inverse = false;
};
```

### 4.9 Autoplay with Per-Slide Timing

Autoplay supports per-slide delay via data attributes and precise time
tracking:

```javascript
autoplay: {
    delay: 3000,
    disableOnInteraction: false,  // Keep going after user touches
    pauseOnMouseEnter: false,
    stopOnLastSlide: false,
    reverseDirection: false,
}
```

Notable: `disableOnInteraction: false` + `pauseOnMouseEnter: true` is
the standard pattern for auto-advancing content that pauses when the user
shows interest.

The module tracks `timeLeft` in real-time for progress indicators.

### 4.10 Breakpoints / Responsive Configuration

Swiper allows completely different configurations at different viewport
widths:

```javascript
breakpoints: {
    640: { slidesPerView: 2, spaceBetween: 20 },
    768: { slidesPerView: 3, spaceBetween: 30 },
    1024: { slidesPerView: 4, spaceBetween: 40 },
}
```

Any parameter can change at any breakpoint.  afterhours uses continuous
resolution scaling instead, but the concept of responsive config overrides
could apply to responsive layout switching.

---

## 5. What afterhours Should Adopt

### 5.1 Carousel Component (High Priority)

A full carousel/slider component built on Swiper's concepts:

```cpp
// Proposed API
carousel({
    .direction = Direction::Horizontal,
    .slides_per_view = 3,
    .space_between = 16.0f,
    .centered = true,
    .loop = true,
    .effect = CarouselEffect::Coverflow,
    .autoplay = { .delay = 3.0f, .pause_on_hover = true },
    .pagination = PaginationType::Bullets,
    .navigation = true,
    .on_slide_change = [](int index) { ... },
}, [&]() {
    carousel_slide([&]() { render_item(items[0]); });
    carousel_slide([&]() { render_item(items[1]); });
    carousel_slide([&]() { render_item(items[2]); });
});
```

### 5.2 Momentum Physics (High Priority)

Extract Swiper's momentum model as a reusable utility:

```cpp
struct MomentumState {
    float position = 0;
    float velocity = 0;
    float deceleration = 0.006f;  // Swiper default
    float bounce_ratio = 1.0f;
    float resistance_ratio = 0.85f;

    void apply_touch_velocity(float v);
    void update(float dt, float min, float max);
    bool is_moving() const;
};
```

Use for: carousel, scroll areas, list views, map panning.

### 5.3 Slide Transition Effects (High Priority)

The `progress → transform` mapping pattern:

```cpp
struct SlideTransform {
    vec3 translate = {0, 0, 0};
    vec3 rotate = {0, 0, 0};
    float scale = 1.0f;
    float opacity = 1.0f;
};

using TransitionEffect = std::function<SlideTransform(float progress, vec2 size)>;

// Built-in presets
TransitionEffect slide_effect();
TransitionEffect fade_effect(bool crossfade = false);
TransitionEffect coverflow_effect(float rotate = 50, float depth = 100);
TransitionEffect cube_effect();
TransitionEffect cards_effect();
TransitionEffect creative_effect(SlideTransform prev, SlideTransform next);
```

### 5.4 Virtual Content Rendering (High Priority)

The pattern of only rendering visible items + a buffer is needed for both
carousel and list views:

```cpp
struct VirtualWindow {
    int total_count;
    int visible_start;
    int visible_end;
    int buffer = 1;

    // Returns indices that should be rendered
    std::vector<int> get_render_range() const;

    // Call when scroll position changes
    void update(float scroll_offset, float item_size, float viewport_size);
};
```

### 5.5 Parallax System (Medium Priority)

A progress-based parallax decorator for carousel slides and scroll areas:

```cpp
// Within a carousel slide or scroll area child:
with_parallax({
    .translate_x = -200.0f,  // Move 200px opposite to scroll
    .opacity = {0.0f, 1.0f}, // Fade from 0% to 100%
    .scale = {0.8f, 1.0f},   // Scale from 80% to 100%
}, [&]() {
    // Child content
});
```

### 5.6 Controller / Linked Scrolling (Low Priority)

For synced scroll views (main + thumbnail gallery):

```cpp
struct LinkedScroll {
    EntityID primary;
    std::vector<EntityID> followers;
    SyncMode mode;  // ByIndex or ByProgress
};
```

---

## 6. Module Architecture Lessons

### 6.1 The Module Function Pattern

Swiper's module pattern is worth adopting directly:

```javascript
// Every module receives the same 4 hooks:
function MyModule({ swiper, extendParams, on, emit }) {
    extendParams({ myModule: { optionA: true } });  // Declare config
    on('init', () => { ... });                        // Subscribe to events
    emit('myModuleReady');                            // Publish events
    swiper.myModule = { ... };                        // Attach public API
}
```

**afterhours equivalent:**

```cpp
// A carousel "module" as an ECS system
struct CarouselAutoplaySystem {
    static void register_config(CarouselConfig& config) {
        config.autoplay = { .delay = 3.0f };
    }
    static void on_init(Entity carousel) { ... }
    static void on_update(Entity carousel, float dt) { ... }
    static void on_destroy(Entity carousel) { ... }
};
```

### 6.2 Progress as Universal Coordinate

The single most important idea from Swiper: **everything is expressed in
terms of `progress` (a float from -1 to 1 relative to each slide's center
position)**.

- Effects use progress to compute transforms
- Parallax uses progress for displacement
- Pagination uses progress for indicator positioning
- Autoplay uses progress for time display

This means adding a new effect or behavior never requires understanding
the underlying scroll math — you just consume `progress`.

### 6.3 Direction Abstraction

Writing all logic in one axis and mapping to the other via a helper
function halves the code for supporting both horizontal and vertical
orientations.  Apply this to carousel, scroll areas, and sliders.

---

## 7. Comparison with Bootstrap Carousel

| Feature | Bootstrap Carousel | Swiper |
|---|---|---|
| **Effects** | Slide, Fade only | 7 effects + Creative |
| **Touch** | Basic touch | Full momentum, resistance |
| **Slides per view** | 1 only | Any number, auto |
| **Loop** | Basic | True infinite loop |
| **Virtual** | No | Yes (only render visible) |
| **Grid** | No | Multi-row grid |
| **Parallax** | No | Per-element parallax |
| **Zoom** | No | Pinch-to-zoom |
| **Keyboard** | Yes (basic) | Full keyboard + mousewheel |
| **Controller** | No | Two-way sync |
| **Thumbs** | No | Thumbnail gallery |
| **Pagination types** | Indicators only | Bullets, progress, fraction, custom |
| **Autoplay** | Basic interval | Per-slide delay, pause-on-hover, time tracking |
| **Modularity** | Monolithic | Tree-shakeable modules |

Swiper is vastly more capable.  For afterhours' carousel implementation,
use Swiper as the reference architecture rather than Bootstrap's simpler
version.

---

## 8. Updated Component Gap Impact

This analysis refines the Carousel gap identified in `bootstrap_analysis.md`:

| Gap Component | Source | Swiper Contribution |
|---|---|---|
| **Carousel** | Bootstrap, Swiper | Full architecture: translate model, snap grid, effects, loop, virtual, autoplay, pagination, navigation |
| **Momentum Scrolling** | Swiper | Free mode physics: velocity tracking, deceleration, bounce, resistance |
| **Virtual Scrolling** | Swiper, Base UI | Only-render-visible pattern for both lists and carousels |
| **Parallax** | Swiper | Progress-based per-element transform offset system |
| **Image Gallery** | Swiper (Thumbs + Zoom) | Linked carousel + pinch zoom for media viewing |

---

## Summary

Swiper's main contributions to the afterhours analysis:

1. **Carousel architecture** — The translate + snap grid + progress model is
   the definitive way to build a carousel.  Simple math, powerful results.

2. **Module composition pattern** — `{ extendParams, on, emit }` provides
   clean separation of optional behaviors.  Maps naturally to ECS systems.

3. **Progress as universal coordinate** — All effects, parallax, and
   pagination are functions of a single `float progress`.  This is the key
   abstraction that makes the system extensible.

4. **Momentum physics** — Velocity tracking, deceleration, bounce, and
   resistance are reusable across carousel, scroll view, and list.

5. **Virtual rendering** — Only rendering visible slides + buffer is the
   pattern for both large carousels and large lists.

6. **Transition effects** — The `progress → (translate, rotate, scale,
   opacity)` mapping is a clean, pluggable interface for slide transitions.
   Coverflow and Creative effects are particularly useful for game UIs
   (character select, item showcase, level browser).

7. **Direction abstraction** — Write once in horizontal, auto-map to vertical.
   Apply to carousel, slider, scroll areas.
