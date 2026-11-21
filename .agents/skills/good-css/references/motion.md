# Motion
_Source: good-css by Vojta Holík, https://good-css.com (github.com/vojtaholik/good-css). Copied 2026-10-07 for Gabe's agents; credits per entry are the original authors'._

## 1. Opt-in motion

**When:** Use it for every transition or animation that moves or scales something.

```css
@media (hover: hover) and (pointer: fine) {
  .box:hover { transform: scale(1.2); }
}

@media (prefers-reduced-motion: no-preference) {
  .box { transition: transform 300ms var(--ease-out, ease-out); }
}
```

**Why it works**
- Motion exists only inside the query, so the default is none. People who never touched the setting match `no-preference` and see it.

**Rules**
- Build the state change first and add the motion inside the query. Do not strip motion afterwards with a global rule that sets every duration to `0.01ms`.
- Fades of opacity and color are not motion and can stay outside the query.
- Animation driven by a script checks the same query with `matchMedia`.

**Support**
- Chrome74Firefox63Safari10.1

**Credits**
- Borrowed from: Josh W. Comeau, Accessible Animations in React with "prefers-reduced-motion" · Kevin Powell, 3 super small changes to improve your CSS, 2023-03-07

## 2. Motion tokens

**When:** Use them in every transition and animation. Two easing curves, defined once, and a rule for durations.

```css
:root {
  --ease-out: cubic-bezier(0.23, 1, 0.32, 1);
  --ease-in-out: cubic-bezier(0.77, 0, 0.175, 1);
}

.dropdown { transition: opacity 180ms var(--ease-out); }
```

**Why it works**
- An ease-out curve starts fast, so the interface moves at the moment the user is watching. Use `--ease-out` for anything entering or leaving.
- An ease-in-out curve suits something that is already on screen and moves to a new place. Use `--ease-in-out` there.
- The built-in `ease-out` and `ease-in-out` keywords are weak versions of the same shapes. These two are stronger.

**Rules**
- Never use `ease-in` on UI. It starts slow and reads as lag.
- Keep a UI transition at 300ms or less. Press feedback takes 100 to 160ms, tooltips and small popovers 125 to 200ms, dropdowns 150 to 250ms. The one exception is a modal or drawer, which takes 200 to 500ms.
- Name the properties in a transition. Never write `transition: all`.
- Other entries write `var(--ease-out, ease-out)`, so their snippets work before these tokens exist.
- Whether something should animate at all is not a CSS question. Emil Kowalski's `animate` and `review-animations` skills answer it.

**Support**
- Every browser.

**Credits**
- Borrowed from: Emil Kowalski, emil-design-eng skill and 7 Practical Animation Tips

## 3. Transition a custom property with `@property`

**When:** Use it when one state change should drive several values together, or when a script feeds a number such as pointer position or progress into CSS and the result should ease.

```css
@property --progress {
  syntax: "<number>";
  inherits: true;
  initial-value: 0;
}

.card:focus-within { --progress: 1; }

@media (hover: hover) and (pointer: fine) {
  .card:hover { --progress: 1; }
}

.card .icon {
  display: inline-block;
  rotate: calc(var(--progress) * 90deg);
}

.card .bar {
  transform-origin: left;
  scale: calc(0.25 + var(--progress) * 0.75) 1;
}

.card .more { opacity: var(--progress); }

@media (prefers-reduced-motion: no-preference) {
  .card { transition: --progress 0.3s var(--ease-out, ease-out); }
}
```

**Why it works**
- An unregistered custom property is a string, and a string cannot be interpolated. Registering it with a `syntax` gives it a type, and the browser then transitions it like any number.
- One transition with one easing drives every dependent value. The children derive theirs with `calc()`, so the parts stay in step.
- `inherits: true` passes the number down to the children that use it.

**Rules**
- `initial-value` is required for every syntax except `"*"`.
- `rotate`, `scale` and `translate` do nothing on an inline element. Give the element `display: inline-block` or another box type first.
- A registration is global. Register each name once and keep the names specific.
- A script only writes the variable, as in `el.style.setProperty("--x", "40%")`. The easing and duration stay in CSS.
- Other useful types are `<length-percentage>` for a gradient position and `<angle>` for a conic gradient.

**Support**
- Chrome85Firefox128Safari16.4

**Credits**
- Borrowed from: Stripe, on the stripe.com home page and its sales contact page
- Docs: MDN: @property

## 4. Shadow change that fades and does not repaint

**When:** Use it on cards that lift on hover, most of all in a grid of many cards or with a large blur.

```css
.card {
  position: relative;
  isolation: isolate;
}

.card::before,
.card::after {
  content: "";
  position: absolute;
  inset: 0;
  z-index: -1;
  border-radius: inherit;
  pointer-events: none;
}

.card::before {
  box-shadow:
    0 1px 4px oklch(0.3 0.05 250 / 0.1),
    0 2px 10px oklch(0.3 0.05 250 / 0.08);
}

.card::after {
  box-shadow:
    0 4px 8px oklch(0.3 0.05 250 / 0.06),
    0 12px 32px oklch(0.3 0.05 250 / 0.16);
  opacity: 0;
}

@media (hover: hover) and (pointer: fine) {
  .card:hover::before { opacity: 0; }
  .card:hover::after { opacity: 1; }
}

@media (prefers-reduced-motion: no-preference) {
  .card::before,
  .card::after { transition: opacity 0.3s var(--ease-out, ease-out); }
}
```

**Why it works**
- A `box-shadow` transition repaints the shadow on every frame. Each shadow here is painted once, and the transition changes only `opacity`, which the compositor handles. In a Chrome trace of one card it took 12 paint events, against about 78 for a plain `box-shadow` transition.
- The two shadows cross-fade, so the end state is exactly the raised shadow and not the sum of both.
- `isolation: isolate` gives the card its own stacking context. `z-index: -1` then puts the pseudo-elements above the card's background and below its content.

**Rules**
- The card needs its own background. The pseudo-elements have none.
- It uses both pseudo-elements of the card. Hit area larger than the visual uses `::after` for the hit area and Whole card clickable from one link uses it for the link, so do not combine them on one element.
- For a single small element, transition `box-shadow` directly. The saving is real but small per card.

**Support**
- Every browser.

**Credits**
- Borrowed from: Stripe, on its resource cards · Tobias Ahlin, How to animate box-shadow, for the older form of the idea

## 5. Cross-document view transitions

**When:** Use it on any multi-page site to crossfade between pages. It replaces a client-side router or a page-transition script.

```css
@view-transition { navigation: auto; }
```

**Why it works**
- On a same-origin navigation the browser snapshots the old and new page and crossfades them. Parts that do not change, such as the header, appear to stay in place.

**Rules**
- Both pages must include the rule.
- The default crossfade adds no movement and needs no reduced-motion guard. If you add slides, put the rule inside `@media (prefers-reduced-motion: no-preference)`.

**Support**
- Firefox navigates as normal.

**Credits**
- Borrowed from: Jen Simmons and James Craig, Two lines of Cross-Document View Transitions code you can use on every website today, 2025-05-21 · Bramus Van Damme, Cross-document view transitions for multi-page applications

## 6. Indicator that slides to the active item

**When:** Use it for the underline on a tab row or the bar beside a side nav. It replaces a script that measures the active item and writes a `transform` and a width.

```css
<nav class="tabs">
  <ul>
    <li><a href="/overview" aria-current="page">Overview</a></li>
    <li><a href="/pricing">Pricing and billing</a></li>
    <li><a href="/docs">Docs</a></li>
  </ul>
</nav>
```

**Why it works**
- The anchor name sits on whichever item carries `aria-current`. Move the attribute and the anchor moves with it. The same attribute tells a screen reader which item is current, so there is no separate "active" class to keep in sync.
- `anchor()` and `anchor-size()` resolve to the item's edge and size. A longer label, a larger font or a wrapped line needs no new measurement.
- The bar is one element whose inset and size change, so a plain transition slides it.

**Rules**
- Keep `anchor-scope` on the component root. Without it an anchor name is global, and two navs on one page both follow the last active item.
- The list needs `position: relative`, so the bar is positioned against the box that holds the items.
- For a vertical list swap the axes. Use `inset-block-start: anchor(top)` and `block-size: anchor-size(height)`, and transition those two.
- Give the active item a second cue such as weight or color. A browser without anchor positioning drops the `anchor()` declarations and the bar has no width.

**Support**
- Firefox places the bar and does not slide it.

**Credits**
- Borrowed from: Stripe, which ships the vertical form for a side nav in its home page stylesheet. The research found the rules and no page that renders the component, so this is from the stylesheet alone.
- Docs: MDN: anchor-name · MDN: anchor-scope · MDN: anchor-size()
