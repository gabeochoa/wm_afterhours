# Foundations
_Source: good-css by Vojta Holík, https://good-css.com (github.com/vojtaholik/good-css). Copied 2026-10-07 for Gabe's agents; credits per entry are the original authors'._

## 1. The reset

**When:** Start every new project with it. In an existing project add one rule at a time and look at the result, because `min-width: 0` and the font rules change how things already render.

```css
*,
*::before,
*::after {
  box-sizing: border-box;
  min-width: 0;
}

:root {
  interpolate-size: allow-keywords;
  scrollbar-gutter: stable;
  text-wrap: pretty;
  overflow-wrap: break-word;
  font-synthesis: none;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  -webkit-text-size-adjust: 100%;
  -webkit-tap-highlight-color: transparent;
}

body {
  margin: 0;
  min-height: 100svh;
}

h1, h2, h3, h4 { text-wrap: balance; }

img, svg, video {
  display: block;
  max-width: 100%;
  height: auto;
}

input, textarea, select { font-size: max(16px, 1rem); }

button, a, [role="button"] { touch-action: manipulation; }

button, [role="button"] {
  user-select: none;
  -webkit-user-select: none;
}
```

**Why it works**
- `min-width: 0` lets flex and grid items shrink below their content. Their default is `min-width: auto`, which is why a long word or a wide child overflows a grid.
- `interpolate-size: allow-keywords` turns on transitions to and from `auto` for the whole page. Accordion that animates its height depends on it.
- `text-wrap: pretty` tells the browser to spend more effort on line breaks. It avoids a last line that holds one short word, and Safari also evens out the ragged edge. The property inherits, so `:root` reaches every element.
- `text-wrap: balance` gives headings lines of similar length.
- `font-synthesis: none` stops the browser from faking a bold or italic that the font file lacks.
- The two smoothing rules switch macOS from subpixel to grayscale antialiasing. Text renders thinner and matches the weight shown in Figma. Other systems ignore them.
- `100svh` is the viewport height with the mobile browser toolbars showing, so the page never sits behind them. `100dvh` changes as the toolbar moves and re-lays out the page each time.
- `scrollbar-gutter: stable` reserves the scrollbar's space. The page does not shift sideways when a modal locks scrolling or when content grows past one screen.
- `overflow-wrap: break-word` breaks a word that cannot fit on a line, such as a URL, where it would otherwise run out of its box. Long text that wraps, truncates or clamps has the rest.
- `-webkit-text-size-adjust: 100%` stops iOS from inflating text in landscape.
- `-webkit-tap-highlight-color: transparent` removes the gray flash iOS and Android paint over a tapped element.
- Inputs at 16px or more stop iOS Safari from zooming the page on focus. It does not zoom back out.
- `touch-action: manipulation` tells the browser the element never double-tap zooms, so `click` fires without the wait.
- `user-select: none` on controls stops a long press from selecting a button's label.

**Rules**
- Load every weight and style the design uses. With `font-synthesis: none` a missing bold renders as regular.
- With `min-width: 0` on everything, a sized image, icon or avatar in a flex row shrinks to make room for the text beside it. Give it `flex: none`.
- The reset removes the tap highlight, so every pressable element needs its own `:active` state. Press feedback has it.
- Never fix input zoom with `user-scalable=no` or `maximum-scale=1`. That takes zoom away from people who need it.
- Keep `user-select: none` to controls. Never set it on `body` or on links, because people copy text.
- Text the user types inherits `pretty`, in `textarea` and `contenteditable`. If lines shift while typing, set `text-wrap: stable` on that element.
- The reserved gutter puts centered content slightly off-center on a page too short to scroll. Chris Coyier documents this. Accept it.
- `svh` is for documents and heroes. An app shell with UI pinned to the bottom uses `height: 100dvh`, so it tracks the toolbar. Emil Kowalski draws this line in his mobile-native skill.

**Support**
- `text-wrap: pretty` in Chrome 117 and Safari 26, ignored by Firefox. `scrollbar-gutter` in Chrome 94, Firefox 97, Safari 18.2. `interpolate-size` in Chrome 129 only. The rest works everywhere.

**Credits**
- Borrowed from: Travis Arnold, @souporserious on X, 2025-04-02, "2025 CSS Reset" · Ayomidé Daniel, @aydahnizzy on X, 2025-10-10, for the font rules · `text-wrap: pretty` on everything is a house rule · Kevin Powell, 3 modern CSS properties to add to your reset, 2026-04-02, for `svh` and the gutter · Emil Kowalski, mobile-native skill, for the touch rules · Chris Coyier, The Downsides of scrollbar-gutter: stable, 2025-12-03 · Ahmad Shadeed, Defensive CSS, for `overflow-wrap` and the `flex: none` rule
- Background: Jen Simmons, Better typography with text-wrap pretty · Adam Argyle, CSS text-wrap: pretty
- Docs: MDN: text-wrap · MDN: font-synthesis · MDN: font-smooth

## 2. Logical properties

**When:** Use them in place of every left, right, top and bottom, in spacing, borders, offsets and alignment.

```css
.card {
  position: relative;
  padding-inline: 1.5rem;
  margin-block-end: 2rem;
  border-inline-start: 4px solid;
  text-align: start;
}

.card .close {
  position: absolute;
  inset-block-start: 1rem;
  inset-inline-end: 1rem;
}
```

**Why it works**
- `inline` is the direction text runs and `block` the direction blocks stack. Start and end follow `dir` and `writing-mode`, so one set of declarations renders mirrored in a right-to-left page.

**Rules**
- The four-value `margin`, `padding` and `inset` shorthands stay physical. Use the `-inline` and `-block` shorthands.

**Support**
- Chrome87Firefox66Safari14.1

**Credits**
- Borrowed from: Ahmad Shadeed, Digging Into CSS Logical Properties, 2021-03-10

## 3. OKLCH color

**When:** Write colors in `oklch()` and derive every related color from a base with `color-mix(in oklch, …)`. Never hardcode a second hex or an `rgba()` for a hover, tint or transparent version.

```css
:root {
  --primary: oklch(55% 0.15 250);
  --primary-hover: color-mix(in oklch, var(--primary), black 15%);
  --primary-subtle: color-mix(in oklch, var(--primary) 12%, transparent);
}
```

**Why it works**
- The three channels are lightness, chroma and hue. Lightness is perceptual, so two colors with the same L look equally light whatever their hue. Scales and contrast become predictable.
- Mixing in OKLCH keeps the hue of the base color, where sRGB mixing drifts toward gray.
- One base value drives the whole family. Change the base and the hover and subtle versions follow.

**Rules**
- Write grays, white and black with `none` as the hue, as in `oklch(98% 0 none)`. A written hue of `0` is red, and it pulls every mix toward red. Half white `oklch(1 0 0)` and half a blue at hue 264 came out at hue 312 in Chrome and Safari. With `none`, or with the keyword `white`, it stayed at 264.

**Credits**
- Borrowed from: Andrey Sitnik and Travis Turner (Evil Martians), OKLCH in CSS: why we moved from RGB and HSL · Björn Ottosson, A perceptual color space for image processing
- Docs: MDN: oklch() · MDN: color-mix() · oklch.com picker

## 4. One set of color tokens for light and dark

**When:** Use it on any site with a dark mode. Each token holds both values, so there is no second block of variables under a media query or a `.dark` class.

```css
:root {
  color-scheme: light dark;
  --background: light-dark(oklch(1 0 none), oklch(0.145 0 none));
  --foreground: light-dark(oklch(0.145 0 none), oklch(0.985 0 none));
}

[data-theme="light"] { color-scheme: light; }
[data-theme="dark"] { color-scheme: dark; }
```

**Why it works**
- `color-scheme: light dark` says the page supports both and follows the system setting. It also switches the browser's own UI, such as form controls, scrollbars and the default page background.
- `light-dark(a, b)` returns `a` in a light scheme and `b` in a dark one.
- It resolves per element. Set `color-scheme` on any subtree and every token used inside it flips. The same tokens serve a manual toggle and a section that is always dark.

**Rules**
- Switch themes by setting `color-scheme`. Never redefine the tokens.
- The grays use `none` for the hue, so they can be mixed with a color later without drifting toward red. OKLCH color has the measurement.
- `light-dark()` takes two colors. Anything else that differs by theme, such as an image, needs its own rule.
- Add `<meta name="color-scheme" content="light dark">` so the browser paints the right background before the CSS loads.

**Support**
- Baseline since May 2024.

**Credits**
- Borrowed from: IP, @ipwanciu on X, 2025-10-30
- Docs: MDN: light-dark() · MDN: color-scheme · Bramus Van Damme, CSS color-scheme-dependent colors with light-dark()

## 5. Fluid sizes with `clamp()`

**When:** Use it for anything that should grow with the screen, starting with font sizes and section spacing. One declaration replaces a ladder of breakpoint overrides.

```css
:root {
  --text-body: clamp(1rem, 0.75rem + 1vw, 1.75rem);
  --space-section: clamp(3rem, 1rem + 6vw, 8rem);
}
```

**Why it works**
- `clamp(min, preferred, max)` uses the preferred value and stops at the two bounds.
- The `vw` part of the preferred value grows with the viewport. The `rem` part sets where the growth starts.
- `--text-body` above is 16px up to a 400px viewport, 28px from 1600px, and a straight line between.

**Rules**
- Write the preferred value as `rem + vw`, never `vw` alone. `clamp(1rem, 1vw, 1.75rem)` stays at 16px on every screen narrower than 1600px, because 1vw is smaller than 1rem until then. A bare `vw` value also ignores the reader's font size setting.
- Work the preferred value out from two points. The slope is `(max - min) / (wide - narrow)`, in px over px, and times 100 it is the `vw` number. The `rem` part is `min - slope × narrow`.
- Keep both bounds in `rem` so they follow the reader's font size.
- For a font size, keep the maximum at or below 2.5 times the minimum. The `vw` part does not grow when the reader zooms, so fluid text grows less than the page. Past 2.5 times, 500% zoom cannot double the text at some widths, and that fails WCAG 1.4.4.
- Put fluid values in tokens. Components use the token and never repeat the math.
- For more than two or three sizes, derive a whole scale as One fluid scale for type and space does.

**Credits**
- Borrowed from: Kyrylo Silin, @kyrylo on X, 2026-05-10 · James Gilyead and Trys Mudford, Utopia · Adrian Roselli, Responsive Type and Zoom · Maxwell Barvian, Addressing Accessibility Concerns With Using Fluid Type, 2023-11-07, for the 2.5 limit
- Docs: MDN: clamp() · Adrian Bece, Modern Fluid Typography Using CSS Clamp

## 6. One fluid scale for type and space

**When:** Use it when a design has more than two or three font sizes. Six numbers describe every size, and the browser derives the steps. It replaces a table of sizes per breakpoint, and it replaces a generator that someone has to find and run again.

```css
:root {
  --narrow: 20;
  --wide: 77.5;
  --size-narrow: 1.125;
  --size-wide: 1.25;
  --ratio-narrow: 1.2;
  --ratio-wide: 1.25;

  --fluid: clamp(0rem, (100vw - var(--narrow) * 1rem) / (var(--wide) - var(--narrow)), 1rem);
  --at-narrow: calc(var(--size-narrow) * (1rem - var(--fluid)));
  --at-wide: calc(var(--size-wide) * var(--fluid));

  --step--1: calc(var(--step-0) / var(--ratio-narrow));
  --step-0: calc(var(--at-narrow) + var(--at-wide));
  --step-1: calc(var(--at-narrow) * var(--ratio-narrow) + var(--at-wide) * var(--ratio-wide));
  --step-2: calc(var(--at-narrow) * pow(var(--ratio-narrow), 2) + var(--at-wide) * pow(var(--ratio-wide), 2));
  --step-3: calc(var(--at-narrow) * pow(var(--ratio-narrow), 3) + var(--at-wide) * pow(var(--ratio-wide), 3));
  --step-4: calc(var(--at-narrow) * pow(var(--ratio-narrow), 4) + var(--at-wide) * pow(var(--ratio-wide), 4));

  --space-s: var(--step-0);
  --space-m: calc(1.5 * var(--step-0));
  --space-l: calc(2 * var(--step-0));
  --space-s-l: calc(var(--at-narrow) + 2 * var(--at-wide));
}

body { font-size: var(--step-0); }
h1 { font-size: var(--step-4); }
h2 { font-size: var(--step-2); }
small { font-size: var(--step--1); }

.card {
  display: grid;
  gap: var(--space-s);
  padding: var(--space-s-l);
}
```

**Why it works**
- The first six lines are the whole design decision. Widths and sizes are in rem, written as plain numbers, so 20 is 320px and 1.125 is 18px. Body text runs from 18px on a 320px screen to 20px on a 1240px one, and each heading step is 1.2 times the last on the phone and 1.25 times on the desktop.
- `--fluid` is a length that is 0 at the narrow width, 1rem at the wide width and a straight line between. It holds the only `clamp()`, and that one bounds every token.
- `--at-narrow` and `--at-wide` are the two base sizes, each weighted by how close the viewport is to its width. Their sum is step 0.
- `pow()` raises each ratio to the step number. A step is the narrow scale's size times one weight plus the wide scale's size times the other, which is the line Fluid sizes with `clamp()` draws between two points.
- A space token is a multiple of step 0, so it grows as much as body text does. A pair such as `--space-s-l` gives the two terms different multipliers. It is the small size on the phone and the large size on the desktop, 18px to 40px here.

**Rules**
- Write the inputs as plain numbers. With `20rem` and `77.5rem` the `--fluid` line divides a length by a length, and Firefox does not support that.
- Change an input in the rule that declares the steps. A step takes its value where it is declared, so a ratio set on a descendant changes nothing below it.
- Derive sizes below step 0 from step 0 and the narrow ratio, as `--step--1` does. With both ratios a small size shrinks as the screen grows.
- Check the top step against the limit in Fluid sizes with `clamp()`. Step `n` passes when `(size-wide / size-narrow) × (ratio-wide / ratio-narrow)ⁿ` is 2.5 or less. The example gives 1.31 at step 4.
- Write a pair only where a layout uses it, and never use one for a font size. A pair is steep on purpose and breaks that limit.
- For a scale that follows the container, write `100cqi` in place of `100vw`. Container queries with container units has the rules.
- Components use the tokens and never repeat the math.

**Support**
- Set by `pow()`. Tested in Chrome 150 and real Safari 27 on 2026-10-01, where every step came out within 0.001px of the formula.

**Credits**
- Borrowed from: James Gilyead, Designing with fluid type scales, 2020-02-01, for the two scales · Trys Mudford, CSS-only fluid modular type scales, 2020-02-02, and Fluid custom properties, 2020-02-03 · James Gilyead, Designing with a fluid space palette, 2021-03-16, for space pairs. Utopia prints one finished `clamp()` per step from a calculator, because CSS had no powers in 2020. With `pow()` the browser does that math.
- Background: Miriam Suzanne, Responsive and Fluid Typography with Baseline CSS Features, 2025-12-16
- Docs: MDN: pow() · Utopia type calculator
