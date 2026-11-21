# Scroll and viewport
_Source: good-css by Vojta Holík, https://good-css.com (github.com/vojtaholik/good-css). Copied 2026-10-07 for Gabe's agents; credits per entry are the original authors'._

## 1. Carousel on native scroll

**When:** Use it for any horizontal row of cards. The carousel is a real scroll container, so swipe, trackpad, keyboard and momentum all work before any script loads.

```css
.carousel {
  display: flex;
  gap: 1rem;
  overflow-x: auto;
  scroll-snap-type: x mandatory;
  scroll-padding-inline: 1.5rem;
  padding-inline: 1.5rem;
  overscroll-behavior-x: contain;
  scrollbar-width: none;
}

.carousel > * {
  flex: none;
  scroll-snap-align: start;
}

@media (prefers-reduced-motion: no-preference) {
  .carousel { scroll-behavior: smooth; }
}
```

**Why it works**
- Scroll snap makes the browser do the scrolling and the landing. Every input method gets the same result.
- `scrollBy` moves one card width and snapping picks the final position. The script never calculates an offset.
- `scroll-behavior: smooth` in CSS animates the script's scroll. The media query turns the animation off for people who ask for reduced motion.
- `overscroll-behavior-x: contain` stops a swipe at the end of the row from triggering the browser's back gesture.

**Rules**
- Never move slides with `transform`, and never handle wheel or touch events yourself.
- Match `scroll-padding-inline` to `padding-inline` so a snapped card lines up with the page content.
- Chrome 135 can draw the arrows and dots in CSS with `::scroll-button()` and `::scroll-marker`. Firefox and Safari have neither, so use the script.

**Credits**
- Borrowed from: Steve Sewell, Build buttery smooth carousels with pure CSS like Nike. This version swaps his `scrollIntoView()` for `scrollBy()`, which can only scroll the carousel and never the page.
- Docs: Adam Argyle, Carousels with CSS · MDN: scroll snap · MDN: scrollBy()

## 2. Scroll area between a fixed header and footer

**When:** Use it for the body of a modal, a chat list, a drawer or a sidebar. The panel grows with its content up to a limit, then the middle scrolls and the header and footer stay put.

```css
.panel {
  display: flex;
  flex-direction: column;
  max-block-size: 80dvh;
}

.panel-body {
  flex: 1;
  overflow-y: auto;
  overscroll-behavior: contain;
  scrollbar-gutter: stable;
}

.panel-wrap {
  flex: 1;
  min-block-size: 0;
  display: flex;
  flex-direction: column;
}
```

**Why it works**
- A flex item will not shrink below its content. A scroll container is the exception, with a minimum of zero, so the body shrinks to the space left and scrolls.
- An element between the panel and the body, such as a form, is not a scroll container. It keeps the height of its content and pushes the footer out of the panel. `min-block-size: 0` gives it the same zero minimum.
- `auto` shows a scrollbar only when there is overflow.
- `scrollbar-gutter` does not inherit. The reset's declaration on `:root` covers the page and no inner scroller.
- `overscroll-behavior: contain` stops the scroll from passing to the page at the end of the list.

**Rules**
- The panel needs a limit, either `max-block-size` or a height. With no limit nothing overflows.
- `.panel-wrap` is only for an element that sits between the panel and the body. Leave it out when the body is a direct child.
- A header or footer with a set height shrinks to its content when the body overflows. Give it `flex: none`, or size it with padding.
- Keep `min-height: 0` out of the reset. On every element it lets each item of a fixed-height column shrink under its own text.
- The gutter takes the scrollbar's width on a short list as well. That is the price of no shift.

**Support**
- `scrollbar-gutter` in Chrome 94, Firefox 97, Safari 18.2. `overscroll-behavior` in Chrome 63, Firefox 59, Safari 16. `dvh` in Chrome 108, Firefox 101, Safari 15.4.

**Credits**
- Borrowed from: Ahmad Shadeed, Defensive CSS, the tips Scrollbars on demand, Scrollbar gutter, Scroll chaining and Minimum content size in flexbox. He gives the four declarations one by one, and the panel around them is added here.
- Docs: MDN: scrollbar-gutter · MDN: overscroll-behavior · MDN: min-height

## 3. Styles that apply only when a scroller overflows

**When:** Use it on any row that may or may not fit, such as tabs, a toolbar or a table wrapper. It fades the edges only when there is something to scroll to. It replaces a `ResizeObserver` that compares `scrollWidth` with `clientWidth`.

```css
@keyframes overflowing {
  from, to { --fade: 2rem; }
}

.scroller {
  --fade: 0px;
  overflow-x: auto;
  animation: overflowing linear;
  animation-timeline: scroll(self inline);
  mask-image: linear-gradient(
    to right,
    transparent,
    #000 var(--fade),
    #000 calc(100% - var(--fade)),
    transparent
  );
}
```

**Why it works**
- A scroll timeline is inactive while its scroller has nothing to scroll. An animation on an inactive timeline has no effect, so the keyframes apply only when the content overflows.
- In the first block `from` and `to` hold the same value. Scroll position changes nothing and the animation is an on/off switch.
- The keyframes set a custom property, and custom properties inherit. The scroller and everything inside it can read the result.
- A mask fades the content itself, so it works over any background. An overlay gradient has to match the color behind it.

**Rules**
- Write `animation-timeline` after `animation`. The shorthand resets the timeline.
- Declare the default outside the keyframes, as `--fade: 0px` does here.
- Any value can go in the keyframes, including keywords. Stripe flips its navigation to `row-reverse` this way when a translation is too long to fit.
- Write no fallback. Firefox has no scroll timelines, so the row scrolls there without the fade.

**Support**
- `animation-timeline` in Chrome 115 and Safari 26. Firefox has it in Nightly only.

**Credits**
- Borrowed from: Stripe, in the navigation on stripe.com · Bramus Van Damme, Detect if an element can scroll or not with only CSS
- Docs: MDN: scroll()

## 4. Anchor targets that clear a sticky header

**When:** Use it on any page with in-page links and a fixed or sticky header.

```css
html { scroll-padding-block-start: 5rem; }

@media (prefers-reduced-motion: no-preference) {
  html { scroll-behavior: smooth; }
}
```

**Why it works**
- The browser offsets every scroll to an anchor by the padding, so the target lands below the header. No script calculates offsets.

**Rules**
- Keep the offset outside the motion query, so people who ask for reduced motion still get it.
- Tie the value to the header's height. Share one custom property between the two.

**Support**
- Chrome69Firefox68Safari14.1

**Credits**
- Borrowed from: Kevin Powell, Stop fixed navigations covering content on scroll, 2022-06-02, and Use these CSS features instead of JavaScript, 2026-09-23

## 5. No rubber-band bounce on desktop

**When:** Use it on app-like pages with fixed UI, such as a sidebar or a sticky header. On macOS a trackpad scroll stretches the whole page past its edge and drags the fixed UI along.

```css
html { overscroll-behavior-y: none; }

@media (any-pointer: coarse) {
  html { overscroll-behavior-y: auto; }
}
```

**Why it works**
- `overscroll-behavior-y: none` on the root turns off the bounce.
- Touch devices get the default back, because pull to refresh depends on it.
- `any-pointer` checks every input, where `pointer` checks only the primary one. A device with a touchscreen plus a mouse or stylus keeps pull to refresh. Jhey's example is a foldable.

**Rules**
- Set it on `html`. The viewport takes its overscroll behavior from the root element.
- Use the `-y` longhand. `overscroll-behavior-x: none` also turns off swipe to go back.
- Give inner scroll containers, such as a sheet, a chat list or a sidebar, `overscroll-behavior: contain`. They keep their own bounce and stop the page behind them from moving. Never use a `touchmove` listener with `preventDefault()` for this.

**Credits**
- Borrowed from: wits, @witsdev on X, 2026-09-30, describing how Vercel gates it behind `pointer: fine` · Jhey Tompkins, @jh3yy on X, for the `any-pointer` version · Emil Kowalski, mobile-native skill, for the inner containers
- Docs: MDN: overscroll-behavior · MDN: any-pointer

## 6. Content clear of the notch

**When:** Use it on fixed headers, bottom bars, toasts and sheets in anything that should feel like an app on a phone. Add `viewport-fit=cover` to the viewport meta tag, then pad those elements by the safe-area insets.

```css
.app-header { padding-block-start: env(safe-area-inset-top, 0px); }

.bottom-bar { padding-block-end: env(safe-area-inset-bottom, 0px); }

.sheet { padding-block-end: calc(1rem + env(safe-area-inset-bottom, 0px)); }
```

**Why it works**
- By default the browser keeps the page inside the safe area and leaves the notch and home-indicator zones as bare background. `viewport-fit=cover` lets the page paint edge to edge.
- `env(safe-area-inset-*)` gives the size of each zone, so the content moves clear while the background still fills the screen.

**Rules**
- Without `viewport-fit=cover` every inset is `0px` and the padding does nothing.
- Give `env()` a fallback of `0px` wherever it sits inside `calc()`.
- Normal page content needs none of this. The header's padding covers it.

**Support**
- Chrome69Firefox65Safari11.1

**Credits**
- Borrowed from: Emil Kowalski, mobile-native skill

## 7. 

**When:** 
