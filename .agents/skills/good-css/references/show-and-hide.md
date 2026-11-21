# Show and hide
_Source: good-css by Vojta Holík, https://good-css.com (github.com/vojtaholik/good-css). Copied 2026-10-07 for Gabe's agents; credits per entry are the original authors'._

## 1. Enter and exit transitions from `display: none`

**When:** Use it for dialogs, popovers and anything else toggled with `display`. It replaces a script that adds classes and waits for `transitionend`.

```css
dialog,
[popover] {
  opacity: 0;
  transition:
    opacity 0.2s var(--ease-out, ease-out),
    translate 0.2s var(--ease-out, ease-out),
    display 0.2s allow-discrete,
    overlay 0.2s allow-discrete;
}

dialog::backdrop {
  opacity: 0;
  transition:
    opacity 0.2s var(--ease-out, ease-out),
    display 0.2s allow-discrete,
    overlay 0.2s allow-discrete;
}

dialog[open],
dialog[open]::backdrop { opacity: 1; }

[popover]:popover-open { opacity: 1; }

@starting-style {
  dialog[open],
  [popover]:popover-open,
  dialog[open]::backdrop { opacity: 0; }
}

@media (prefers-reduced-motion: no-preference) {
  dialog,
  [popover] { translate: 0 0.5rem; }

  dialog[open] { translate: 0 0; }

  [popover]:popover-open { translate: 0 0; }

  @starting-style {
    dialog[open],
    [popover]:popover-open { translate: 0 0.5rem; }
  }
}
```

**Why it works**
- `@starting-style` gives the browser a "before open" state to transition from when the element leaves `display: none`. That is the entry.
- `allow-discrete` on `display` delays the switch to `display: none` until the other transitions finish. That is the exit. `overlay` keeps the element in the top layer meanwhile.
- The fade is not motion, so it runs for everyone. Only the slide sits inside the reduced-motion query.

**Rules**
- Put `@starting-style` after the open-state rule. It has no extra weight in the cascade.
- Never start from `scale(0)`. If the element scales in, start at `scale(0.95)` with `opacity: 0`.
- The dialog's `::backdrop` needs the same three states and its own transition, as in the block. Without them it snaps while the dialog fades.
- Write no fallback. Where the exit is not supported the element closes at once.
- Keep `[popover]:popover-open` out of the selector list that opens the dialog. A browser that does not know one selector in a list drops the whole rule, and the dialog would open at `opacity: 0`. Inside `@starting-style` the shared list is safe, because every browser that reads the block knows the selector.

**Support**
- Both directions in Chrome 117. In Safari 27 the entry animates and `<dialog>` and popovers close at once, tested in real Safari on 2026-10-01. An element toggled with a class animates both ways there. Firefox 129 animates the entry only.

**Credits**
- Borrowed from: Una Kravets and Joey Arhar, Four new CSS features for smooth entry and exit animations · Kevin Powell, Transition to and from display: none, 2026-07-22 · Chris Coyier, In-N-Out Animations: Dialogs, 2026-06-01 · Adam Argyle, Steal this popover code, 2024-03-15 · Ahmad Shadeed, Grouping vendor selectors, Defensive CSS, for the split rules

## 2. Popover anchored to its trigger

**When:** Use it for dropdown menus and other popovers opened by a click. It replaces a positioning library or a script that reads `getBoundingClientRect()`.

```css
<button popovertarget="menu">Options</button>
<div id="menu" popover>
  <button>Rename</button>
  <button>Delete</button>
</div>
```

**Why it works**
- `popovertarget` makes the button the popover's anchor, so no anchor names are needed.
- `position-area` places the popover on a 3×3 grid around the anchor. The fallbacks flip it when it would overflow the viewport.

**Rules**
- Use `span-*` values, not `center`. Adam Argyle found that `center` stops the flips when the popover is wider than its grid cell.
- `margin-block-start` is the gap between trigger and popover. The flip mirrors it, so the gap stays on the trigger's side.
- A popover opened by a script has no anchor and lands in the top-left corner. Pass the trigger, as in `menu.showPopover({ source: button })`.
- Write no fallback. Without support the `@supports` block is skipped and the popover opens centered in the viewport.

**Support**
- Chrome133Firefox147Safari26

**Credits**
- Borrowed from: Una Kravets, Introducing the CSS anchor positioning API, 2024-05-10 · Saron Yitbarek, A gentle introduction to anchor positioning, 2025-08-12 · Adam Argyle, Why isn't my position-try-fallback working in small spaces?, 2024-10-12

## 3. Accordion that animates its height

**When:** Use it for FAQs and any other disclosure. `<details>` already gives you the toggle, keyboard support and find-in-page. These rules animate the open and the close with no JavaScript.

```css
:root { interpolate-size: allow-keywords; }

details::details-content {
  height: 0;
  overflow: clip;
}

details[open]::details-content { height: auto; }

@media (prefers-reduced-motion: no-preference) {
  details::details-content {
    transition:
      height 0.3s var(--ease-out, ease-out),
      content-visibility 0.3s allow-discrete;
  }
}
```

**Why it works**
- `::details-content` is the box the browser wraps around everything in `<details>` except the `<summary>`.
- `interpolate-size: allow-keywords` lets a transition run between a length and `auto`. It inherits, so one declaration on `:root` covers the page.
- A closed `<details>` hides its content with `content-visibility`. Transitioning that property with `allow-discrete` keeps the content rendered until the height reaches zero. Leave it out and the close snaps.

**Rules**
- Put padding on an element inside, never on `::details-content`. Padding there stays visible when the accordion is closed.
- Give sibling `<details>` the same `name` attribute to keep one open at a time.
- Hide the marker with `summary { list-style: none; }` plus `summary::-webkit-details-marker { display: none; }` for Safari.
- Write no fallback. Safari and Firefox do not support `interpolate-size` yet, so they open and close without the animation.

**Support**
- `::details-content` in Chrome 131, Safari 18.4, Firefox 143. `interpolate-size` in Chrome 129 only.

**Credits**
- Borrowed from: Bramus Van Damme, Animate to height: auto
- Docs: MDN: ::details-content · MDN: interpolate-size · MDN: transition-behavior

## 4. Reveal with `clip-path`

**When:** Use it for dropdowns, menus and panels that open over the page and whose height is not known. It unrolls the panel without animating `height`, and it works in every browser.

```css
.menu {
  clip-path: inset(0 -3rem 100%);
  visibility: hidden;
}

.menu.is-open {
  clip-path: inset(0 -3rem -3rem);
  visibility: visible;
}

@media (prefers-reduced-motion: no-preference) {
  .menu {
    transition:
      clip-path 0.25s var(--ease-out, ease-out),
      visibility 0.25s;
  }
}
```

**Why it works**
- `inset()` clips from each edge, in the order top, sides, bottom. A bottom value of `100%` hides the whole box. The percentage refers to the element's own height, so nothing has to measure it.
- A negative inset widens the clip beyond the box. That leaves room for the shadow at the sides and the bottom. The top stays at `0`, so no shadow shows where the menu meets its trigger.
- Layout never changes. The panel has its full size the whole time and nothing around it moves.
- The clipped part takes no pointer events, so a closed menu cannot be clicked.
- `visibility` in the transition switches to `hidden` only when the close has finished. That takes the links out of the tab order.

**Rules**
- The negative inset must be larger than the shadow's blur plus its offset.
- Use it for panels that overlap the page. A closed panel in the normal flow leaves a gap, so an accordion uses Accordion that animates its height.
- Add `round` with a radius inside `inset()` only when the clip itself should have round corners. The element's own `border-radius` still applies.

**Support**
- Every browser.

**Credits**
- Borrowed from: Stripe, in the navigation menu on stripe.com. The `visibility` part was added for this list.
- Docs: MDN: inset()
