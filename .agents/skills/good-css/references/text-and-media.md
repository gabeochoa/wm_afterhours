# Text and media
_Source: good-css by Vojta Holík, https://good-css.com (github.com/vojtaholik/good-css). Copied 2026-10-07 for Gabe's agents; credits per entry are the original authors'._

## 1. Long text that wraps, truncates or clamps

**When:** Use it on any text that comes from a user or a CMS, such as names, titles, URLs and excerpts. Decide for each one what happens when the content is longer than the design. It wraps, it is cut to one line, or it is cut to a few lines.

```css
:root { overflow-wrap: break-word; }

.name {
  white-space: nowrap;
  overflow: clip;
  text-overflow: ellipsis;
}

.excerpt {
  display: -webkit-box;
  -webkit-box-orient: vertical;
  -webkit-line-clamp: 3;
  overflow: clip;
}
```

**Why it works**
- `overflow-wrap: break-word` breaks a word only when it cannot fit on a line by itself. Ordinary text wraps as before. It inherits, so the one declaration in the reset covers the page.
- The reset's `min-width: 0` lets a flex or grid item get narrower than its longest word. It does nothing for the word, which then paints over the next column. The two rules need each other.
- `nowrap` keeps the text on one line, `overflow` cuts it and `text-overflow` draws the ellipsis at the cut.
- `-webkit-line-clamp` counts lines, so the clamp holds at any font size and any width.

**Rules**
- Put the truncation on the element that holds the text. On a flex or grid container the text is cut and no ellipsis appears.
- Every flex item between the row and the truncated text must be able to shrink. The reset's `min-width: 0` covers that. Without the reset, set `min-inline-size: 0` on each of them.
- The clamp needs all four declarations. Unprefixed `line-clamp` is in no browser yet.
- Put padding on a wrapper, never on the clamped element. The next line shows through the bottom padding.
- Use `break-word` on the root and not `anywhere`. `anywhere` also shrinks the minimum content width, so a box sized by its content collapses to one letter per line.
- A table with automatic layout ignores `break-word`. Set `overflow-wrap: anywhere` on the cell.
- Truncate only text the reader can get in full somewhere else. Never truncate text they have to read.

**Support**
- `text-overflow`, `-webkit-line-clamp` and `overflow-wrap: break-word` work everywhere. With `overflow: clip` the ellipsis and the clamp draw in Chrome 150 and Safari 27, tested on 2026-10-01. Firefox is untested, and `overflow: hidden` is the form to fall back to. `overflow-wrap: anywhere` in Chrome 80, Firefox 65, Safari 15.4.

**Credits**
- Borrowed from: Ahmad Shadeed, Defensive CSS, the tips Long content and Minimum content size in flexbox · Ahmad Shadeed, Handling Short And Long Content In CSS, 2020-12-16
- Docs: MDN: overflow-wrap · MDN: text-overflow · MDN: line-clamp

## 2. Image box that holds any upload

**When:** Use it on every image, video or embed whose file you do not control, such as thumbnails, cover photos and avatars. The box keeps its shape whatever the file's ratio, and it has that shape before the file loads.

```css
.thumb {
  inline-size: 100%;
  block-size: auto;
  aspect-ratio: 16 / 9;
  object-fit: cover;
  background-color: var(--surface-muted, oklch(0.9 0 none));
}

.avatar {
  flex: none;
  inline-size: 3.5rem;
  block-size: 3.5rem;
  border-radius: 50%;
  object-fit: cover;
  outline: 1px solid light-dark(oklch(0 0 none / 0.1), oklch(1 0 none / 0.1));
  outline-offset: -1px;
}
```

**Why it works**
- `aspect-ratio` works out the height from the width, so the space is there before the file loads.
- `object-fit: cover` scales the picture until it fills the box and crops the rest. The default stretches it.
- The background color shows until the image paints, and in its place when the load fails.
- An outline with a negative offset draws inside the image's edge. It takes no space and follows the `border-radius`, so a white avatar on a white card still has an edge.
- `flex: none` stops the text beside the avatar from squeezing it.

**Rules**
- One axis must be `auto`. A `height` attribute or a fixed height wins over the ratio. The reset sets `height: auto` on `img`, `svg` and `video`, and `block-size: auto` here does the same for an `iframe`.
- `object-fit` does nothing until the box has both sizes or a ratio.
- `cover` crops. Use `contain` for logos, product shots and anything that must stay whole. Move the crop with `object-position`.
- Leave the background color off an image that has transparent areas. It shows through them.
- A fixed-size image in a flex row needs both sizes, as the avatar has. With only a width it stretches to the height of the row.
- In Safari a failed image ignores `aspect-ratio` and draws a square box. Where a failed load must not move the layout, put the ratio on a wrapper and give the image `inline-size: 100%` and `block-size: 100%`.
- `light-dark()` needs the `color-scheme` from One set of color tokens for light and dark. Without it the outline stays black on a dark page.

**Support**
- `aspect-ratio` in Chrome 88, Firefox 89, Safari 15. An outline that follows the radius in Chrome 94, Firefox 88, Safari 16.4. `light-dark()` in Chrome 123, Firefox 120, Safari 17.5. The square box of a failed image is from real Safari 27, tested on 2026-10-01. Chrome 150 keeps the ratio.

**Credits**
- Borrowed from: Ahmad Shadeed, Defensive CSS, the tips Image distortion, Image inner border and Default flexbox stretching. This version draws the inner edge with an outline where his adds an element · Heydon Pickering and Andy Bell, The Frame, Every Layout, which wraps the media. On `img`, `video` and `iframe` the rule goes on the element.
- Docs: MDN: aspect-ratio · MDN: object-fit · MDN: outline-offset

## 3. Tabular numbers

**When:** Use it on any number that changes or sits in a column, such as prices, tables, timers and counters.

```css
.price,
td,
time { font-variant-numeric: tabular-nums; }
```

**Why it works**
- Every digit gets the same width, so columns line up and a ticking value does not shift its neighbors.

**Rules**
- Do not set it globally. Proportional digits read better in prose.
- The font must ship tabular figures, or nothing changes.

**Support**
- Chrome52Firefox34Safari9.1

**Credits**
- Borrowed from: Rauno Freiberg, Web Interface Guidelines · Vercel Web Interface Guidelines

## 4. Label centered on its letters with `text-box`

**When:** Use it on single-line labels in buttons, badges and chips, so equal padding looks equal in any font.

```css
.button {
  display: inline-block;
  padding: 0.75rem 1.25rem;
  text-box: trim-both cap alphabetic;
}

.button-with-icon {
  display: inline-flex;
  align-items: center;
  gap: 0.5rem;
  padding: 0.75rem 1.25rem;
}

.button-with-icon > span { text-box: trim-both cap alphabetic; }
```

**Why it works**
- A line of text reserves space above the capitals and below the baseline, and the two amounts differ from font to font. Equal padding around that line puts the letters off-center.
- `trim-both cap alphabetic` cuts the box down to the cap height and the baseline. The padding then starts at the letters.
- A font swap or a fallback font no longer moves the label.

**Rules**
- It does nothing on a flex or grid container. A button that is `inline-flex` because it holds an icon needs the declaration on the element that wraps the text, as in the second rule of the block.
- The button gets shorter, because the padding now starts at the letters. With `0.75rem` of padding it went from 48px tall to 35px in Chrome and Safari. Raise the padding to keep the height.
- Use it for one line. Descenders hang into the bottom padding, which is the intent.
- Scope it to labels. Never set it on `*`, because it shrinks every text block. A one-line paragraph at 16px/1.5 went from 25px tall to 11px.
- Write no fallback. Without support the label keeps its normal line box.

**Support**
- Chrome133Firefox154Safari18.2

**Credits**
- Borrowed from: Stripe, which ships it on its buttons at stripe.com. There the button is a flex container, so the declaration has no effect and uneven padding does the centering. The first rule above is the fix.
- Docs: MDN: text-box

## 5. Icon sized by the text beside it

**When:** Use it on every icon that sits next to a label, in buttons, links, list rows and notices. The icon takes its size from the font, so it follows the text and never needs a size per variant.

```css
.with-icon {
  display: inline-flex;
  align-items: baseline;
  gap: 0.5em;
}

.with-icon > svg {
  flex: none;
  block-size: 1cap;
  inline-size: auto;
}

.notice {
  display: flex;
  align-items: start;
  gap: 0.5em;
}

.notice > svg {
  flex: none;
  inline-size: 1em;
  block-size: 1lh;
}
```

**Why it works**
- `1cap` is the height of the font's capital letters. An icon that tall reads as one more capital.
- An `svg` has no baseline of its own. `align-items: baseline` rests its bottom edge on the baseline of the text, so the icon stands where a capital stands.
- `1lh` is the height of one line. The second rule makes the icon's box as tall as the first line of the label, and the drawing centers itself in that box. The icon stays beside the first line when the label wraps.
- A `gap` in `em` keeps the space in proportion.

**Rules**
- Never size an icon in `px`. A 16px icon that matches 16px text is 5px to 7px shorter than the capitals of 32px text.
- Keep `flex: none`. Without it a long label squeezes the icon.
- The `svg` needs a `viewBox`. `inline-size: auto` then follows the drawing's ratio.
- Many icon sets leave padding inside the drawing, and `1cap` looks small on those. Raise the number and keep the unit, as in `1.2cap`.
- Do not use `align-items: center` on a label that can wrap. The icon then sits beside the middle line.
- With Label centered on its letters with `text-box` the trimmed label is `1cap` tall, so `align-items: center` lines the icon up with it at both edges.

**Support**
- `cap` in Chrome 118, Firefox 97, Safari 17.2. `lh` in Chrome 109, Firefox 120, Safari 16.4. Tested in Chrome 150 and real Safari 27 on 2026-10-01, where the icon came out within 0.01px of the capital height at 16px and at 32px.

**Credits**
- Borrowed from: Andy Bell, How I build a button component, 2024-09-18, for `cap` and `flex: none`, and LH units are cool, 2023-12-13 · Heydon Pickering and Andy Bell, The Icon, Every Layout
- Docs: MDN: length units
