# Layout
_Source: good-css by Vojta Holík, https://good-css.com (github.com/vojtaholik/good-css). Copied 2026-10-07 for Gabe's agents; credits per entry are the original authors'._

## 1. Content grid with breakouts

**When:** Use it wherever you would reach for a centered max-width container. One grid on the section replaces the `section > .container` wrapper pair, and any child can be content width, wider, or edge to edge. Put it on `main` for a page of flowing content, or on each section. It suits CMS content well, because an editor widens a block with one class and no extra wrapper.

```css
.content-grid {
  --gutter: 1.5rem;
  --content: 64rem;
  --breakout: 80rem;

  display: grid;
  grid-template-columns:
    [full-width-start] minmax(var(--gutter), 1fr)
    [breakout-start] minmax(0, calc((var(--breakout) - var(--content)) / 2))
    [content-start] min(100% - var(--gutter) * 2, var(--content)) [content-end]
    minmax(0, calc((var(--breakout) - var(--content)) / 2)) [breakout-end]
    minmax(var(--gutter), 1fr) [full-width-end];
}
```

**Why it works**
- Two lines named `content-start` and `content-end` create a named area, `content`. That is why `grid-column: content` works with no line numbers.
- `min(100% - 2 × gutter, max)` shrinks the content column on small screens with no media query.
- `minmax(0, …)` lets the breakout tracks collapse, so a breakout falls back to content width when there is no room. With a fixed breakout size the page overflows on small screens.
- `grid-template-columns: inherit` gives a full-width band the same grid, so content inside it lines up with the page column without another wrapper. This is Kevin Powell's addition. In Ryan Mulligan's original the full-width child is a plain box.

**Rules**
- Every child must be placed. An unplaced child of the grid lands in the gutter track.
- Every direct child is a grid item. Wrap a run of inline elements in one element.
- Change a width by overriding `--content`, not by writing a new column template.

**Credits**
- Borrowed from: Kevin Powell, A new approach to container and wrapper classes, 2023-11-09 · Ryan Mulligan, Layout Breakouts with CSS Grid · Stephanie Eckles, SmolCSS breakout grid
- Docs: MDN: named grid lines · MDN: min()

## 2. Intrinsic grid

**When:** Use it for any set of equal cards or tiles. It replaces a column count per breakpoint.

```css
.grid {
  display: grid;
  gap: 1rem;
  grid-template-columns: repeat(auto-fit, minmax(min(100%, 16rem), 1fr));
}
```

**Why it works**
- `auto-fit` creates as many 16rem columns as the row holds, and `1fr` shares what is left. The column count follows the container.
- `min(100%, 16rem)` lets the minimum shrink to the container. Without it a 200px container overflows.

**Rules**
- `auto-fit` stretches a short row to fill the width. Use `auto-fill` when the item count changes, such as a filtered list, so cards keep their size.

**Support**
- Chrome79Firefox76Safari11.1

**Credits**
- Borrowed from: Kevin Powell, Breakpoint-Free CSS Grid Layouts, 2025-02-19 · Stephanie Eckles, Smol Responsive CSS Grid · Una Kravets, Ten modern layouts in one line of CSS

## 3. Subgrid rows shared across cards

**When:** Use it when cards in a row have parts that should line up, such as title, body and footer, whatever the length of their content.

```css
.cards {
  display: grid;
  gap: 1.5rem;
  grid-template-columns: repeat(auto-fit, minmax(min(100%, 16rem), 1fr));
}

.card {
  display: grid;
  grid-row: span 3;
  grid-template-rows: subgrid;
  row-gap: 0.5rem;
}
```

**Why it works**
- Each card spans three rows of the parent grid and adopts those tracks. The tallest title in a row sets the title height for every card in that row.

**Rules**
- The span must equal the number of parts in the card. Without `grid-row: span N` every part piles into one row.
- Set `row-gap` on the card, or it inherits the parent's gap between its parts.

**Support**
- Chrome117Firefox71Safari16

**Credits**
- Borrowed from: Josh W. Comeau, Brand New Layouts with CSS Subgrid, 2025-11-25 · Kevin Powell, Subgrid & Container Queries change how we can create layouts, 2023-10-26

## 4. Sidebar that wraps on its own

**When:** Use it for any pair where one side has an ideal width and the other takes the rest, such as a media object, an input with a button, or a page with an aside.

```css
.with-sidebar {
  display: flex;
  flex-wrap: wrap;
  gap: 1rem;
}

.sidebar {
  flex-basis: 20rem;
  flex-grow: 1;
}

.main {
  flex-basis: 0;
  flex-grow: 999;
  min-inline-size: 50%;
}
```

**Why it works**
- The main side's huge `flex-grow` takes all the free space, which holds the sidebar at its basis.
- Once the main side would get less than half the container it wraps below, and both sides grow to full width.

**Rules**
- The wrap point is a share of the container, not a length. Change `min-inline-size` to move it.
- Drop the sidebar's `flex-basis` for a sidebar sized by its content.
- For a sticky sidebar, add `align-self: start` beside `position: sticky` and its inset. The two sides stretch to the same height, and a stretched sidebar has no room to stick.
- A bare `img` or `video` as one of the sides stretches to the height of the row and distorts. Set `align-items: start` on the container.

**Support**
- Chrome84Firefox63Safari14.1

**Credits**
- Borrowed from: Heydon Pickering and Andy Bell, The Sidebar, Every Layout

## 5. Container queries with container units

**When:** Use it for any component that appears in slots of different widths. The component responds to the space it is given, where a media query only knows the viewport.

```css
.slot { container-type: inline-size; }

.card h2 { font-size: clamp(1.25rem, 1rem + 2cqi, 2rem); }

@container (width > 30rem) {
  .card {
    display: grid;
    grid-template-columns: 12rem 1fr;
  }
}
```

**Why it works**
- The query measures the nearest container, so the same card is right in a sidebar and in a wide column.
- `1cqi` is 1% of that container's inline size. The `clamp()` math from Fluid sizes with `clamp()` carries over, per component.

**Rules**
- The container must be an ancestor. An element cannot query itself.
- A container cannot take its width from its content. Never put `container-type` on a shrink-to-fit element, or it collapses to zero.
- With no container ancestor the query never matches. Kevin Powell makes `header`, `main` and `footer` containers in his reset, so most components need none of their own.
- A container cannot also be a subgrid. Subgrid rows shared across cards and this entry need separate elements.
- Container units are different from queries. With no container above the element, `cqi` measures the viewport, so one token written with `cqi` serves the page and every slot.
- Never register a fluid token that uses `cqi` with `@property`. A registered length computes once on `:root`, where there is no container, and every slot then gets the viewport's value.

**Support**
- Chrome105Firefox110Safari16

**Credits**
- Borrowed from: Kevin Powell, Improve your reset with these modern CSS additions, 2024-09-04 · Una Kravets, Container queries land in stable browsers, 2023-02-14 · Josh W. Comeau, A Friendly Introduction to Container Queries, 2024-11-04 · Stephanie Eckles, Container Query Units and Fluid Typography

## 6. Stack layers with grid

**When:** Use it whenever things sit on top of each other, such as text over an image, a badge on a card, or two icons that swap. It replaces `position: absolute` with its offsets, sizes and transforms.

```css
.stack { display: grid; }
.stack > * { grid-area: 1 / 1; }

.stack > .title { place-self: center; }
.stack > .badge { place-self: start end; }
```

**Why it works**
- Every child goes into the same grid cell, so they overlap.
- The layers stay in flow. The container takes the size of its largest layer, where an absolutely positioned child adds nothing to it.
- `place-self` aligns one layer inside the cell. Without it the layer stretches to fill.

**Rules**
- A layer later in the DOM paints on top. Use `z-index` only to change that order.
- Keep `position: absolute` for a layer that must not affect the container's size.

**Credits**
- Borrowed from: Ana Tudor, @anatudor on X, 2025-03-29
- Docs: MDN: grid-area · MDN: place-self

## 7. Safe alignment

**When:** Use it wherever content is centered or end-aligned in a container that can become too small, such as a tab row, a toolbar or a vertically centered modal.

```css
.tabs {
  display: flex;
  overflow-x: auto;
  justify-content: safe center;
}
```

**Why it works**
- Plain `center` pushes overflow off the start edge, where scrolling cannot reach it. `safe` falls back to `start` once the content overflows.

**Rules**
- It works on every `align-*`, `justify-*` and `place-*` property.
- Auto margins on the item center it the same way and are safe in every browser that has flexbox. Push one item away with an auto margin has them.

**Support**
- Chrome115Firefox63Safari17.6

**Credits**
- Borrowed from: Temani Afif, Safe align your content, 2025-06-10

## 8. `overflow: clip` over `hidden`

**When:** Use it whenever you want to cut off overflow and do not need scrolling.

```css
.hero { overflow-x: clip; }
```

**Why it works**
- `hidden` turns the element into a scroll container and sets the other axis to `auto`. That causes stray scrollbars and breaks `position: sticky` inside it.
- `clip` cuts the overflow and does neither.

**Rules**
- Put it on the element that overflows, not on `html` or `body`.
- Keep `hidden` or `auto` for elements a script scrolls. `clip` blocks that too.
- Keep `hidden` or `auto` on an element with `resize`. With `clip` the browser draws no resize handle.

**Support**
- Chrome90Firefox81Safari16

**Credits**
- Borrowed from: Kevin Powell, 2 better alternatives to overflow: hidden, 2023-12-14
