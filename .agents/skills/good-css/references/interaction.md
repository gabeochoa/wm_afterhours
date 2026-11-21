# Interaction
_Source: good-css by Vojta Holík, https://good-css.com (github.com/vojtaholik/good-css). Copied 2026-10-07 for Gabe's agents; credits per entry are the original authors'._

## 1. One focus ring with `:focus-visible`

**When:** Use it as the focus style for every interactive element. Never write `outline: none`.

```css
:focus-visible {
  outline: max(2px, 0.08em) solid currentColor;
  outline-offset: 0.25em;
}
```

**Why it works**
- The browser decides when a ring helps. Keyboard focus gets one, a mouse click on a button does not, and text inputs always do.
- `currentColor` and `em` make the ring follow each component's color and size.
- `outline` takes no layout space and survives forced-colors mode, which removes `box-shadow`.

**Rules**
- On a filled button `currentColor` can fail contrast. Set the outline to the button's background color there.
- A component that draws its ring with `box-shadow` keeps `outline-color: transparent`, never `outline: none`, so forced-colors mode can repaint it.

**Support**
- Chrome86Firefox85Safari15.4

**Credits**
- Borrowed from: Stephanie Eckles, Modern CSS Upgrades To Improve Accessibility, 2021-04-09, and Smol Focus Styles · Kevin Powell, 3 super small changes to improve your CSS, 2023-03-07

## 2. Hover styles only where hover exists

**When:** Use it on every `:hover` rule, so a tap on a touch screen does not leave the hover state stuck.

```css
@media (hover: hover) and (pointer: fine) {
  .button:hover { background: var(--primary-hover); }
}
```

**Why it works**
- Touch browsers apply `:hover` on tap and keep it until the next tap elsewhere. `hover: hover` asks whether the primary input can hover.
- `pointer: fine` asks whether it is precise, like a mouse. It rules out styluses and the Android devices that claim hover support.

**Rules**
- Tailwind v4's `hover:` variant wraps itself in `(hover: hover)` only. Write the full query by hand everywhere else.
- Touch users still need feedback on press. Give it with `:active`, which works for every input.

**Support**
- Chrome38Firefox64Safari9

**Credits**
- Borrowed from: Emil Kowalski, mobile-native skill and emil-design-eng skill · Rauno Freiberg, Web Interface Guidelines

## 3. Press feedback

**When:** Use it on every button and anything else that can be pressed.

```css
.button:active { transform: scale(0.97); }

@media (prefers-reduced-motion: no-preference) {
  .button { transition: transform 160ms var(--ease-out, ease-out); }
}
```

**Why it works**
- A native button responds the instant a finger lands. `:active` does the same on every kind of input. Feedback that waits for `click` arrives when the finger leaves, and that reads as lag.
- `scale()` shrinks the label and icon along with the button, so the whole control reads as pressed.

**Rules**
- Keep the scale between 0.95 and 0.98.
- The reset removes the browser's tap highlight. Without this entry a tap gives no feedback at all.
- When feedback needs a script, listen for `pointerdown`, not `click`.

**Support**
- Every browser.

**Credits**
- Borrowed from: Emil Kowalski, emil-design-eng skill and mobile-native skill

## 4. Hit area larger than the visual

**When:** Use it on icon buttons, close buttons and any target that looks smaller than 44px.

```css
.icon-button { position: relative; }

.icon-button::after {
  content: "";
  position: absolute;
  inset: min(0px, (100% - 44px) / 2);
}
```

**Why it works**
- The pseudo-element belongs to the button, so a click on it hits the button.
- The negative inset grows the area to exactly 44px. `min(0px, …)` leaves larger targets alone.

**Rules**
- `overflow: hidden` or `clip` on the button cuts the area off.
- It does not work on `<input>`, which has no pseudo-elements. Wrap it in a `<label>`.

**Support**
- Chrome87Firefox66Safari14.1

**Credits**
- Borrowed from: Emil Kowalski, Agents with Taste, for the rule · Vercel Web Interface Guidelines. The `inset` formula was written for this list.

## 5. Whole card clickable from one link

**When:** Use it for cards, list rows and tiles where the whole area should navigate. It replaces an `<a>` wrapped around the card, which makes a screen reader read every word as the link text, and it replaces a click handler on a `div`.

```css
<article class="card">
  <h3><a class="card-link" href="/reports/q3">Quarterly report</a></h3>
  <p>Revenue grew in every region.</p>
  <button type="button">Save</button>
</article>
```

**Why it works**
- The pseudo-element belongs to the link and covers the card, so a click anywhere activates the link. It is a real link, so open in new tab and the status bar URL still work.
- The link text stays short. A screen reader announces "Quarterly report" and not the whole card.
- `:has()` moves the focus ring from the small link to the card it activates.
- Other controls sit above the overlay and keep their own clicks.

**Rules**
- No element between the card and the link may be positioned. The overlay would size itself to that element.
- The link's own ring is made transparent, never removed, as One focus ring with `:focus-visible` requires. Write the full `2px solid transparent`. Safari ignores `outline-color` on its default ring.
- Text under the overlay cannot be selected. Accept it.
- Put hover styles on `.card:hover`, inside the query from Hover styles only where hover exists.

**Support**
- Chrome105Firefox121Safari15.4

**Credits**
- Borrowed from: Heydon Pickering, Cards, for the stretched `::after` · Stripe, for moving the focus ring to the card with `:has()`

## 6. `:has()` for parent and page state

**When:** Use it wherever a script adds a class to a parent because of what it contains or what state a child is in.

```css
article:has(img) { grid-column: span 2; }
h2:has(+ p) { margin-block-end: 0; }
form:has(:focus-visible) { background: var(--surface-raised); }

html { scrollbar-gutter: stable; }
html:has(dialog:modal) { overflow: hidden; }
```

**Why it works**
- `:has()` matches an element by its descendants or following siblings, and it accepts state pseudo-classes. Content and form state can style any ancestor.
- On the root it works as a page-wide listener. The last rule locks page scroll while a modal dialog is open and lifts the lock when it closes, with nothing to clean up.

**Rules**
- `:modal` matches `showModal()` only, so a non-modal dialog does not lock the page.
- Keep `scrollbar-gutter: stable` with the scroll lock. Without it the page shifts sideways when the scrollbar disappears. The reset sets it.
- `:has()` cannot be nested inside `:has()`.

**Support**
- Chrome105Firefox121Safari15.4

**Credits**
- Borrowed from: Jen Simmons, Using :has() as a CSS Parent Selector and much more, 2022-08-18 · Josh W. Comeau, The Undeniable Utility Of CSS :has, 2024-09-09 · Chris Coyier, Scroll-Locked Dialogs, 2024-02-19

## 7. Form feedback with `:user-invalid`

**When:** Use it for inline form validation, in place of blur listeners and a "touched" class.

```css
input:user-invalid { border-color: var(--danger); }
input:user-valid { border-color: var(--success); }
```

**Why it works**
- `:invalid` matches an empty required field on page load. The `:user-` versions match only after the user has edited the field or tried to submit.
- Validity comes from HTML attributes: `required`, `minlength`, `type`, `pattern`.

**Rules**
- Color alone is not enough feedback. Pair it with text or an icon.
- The server still validates.

**Support**
- Chrome119Firefox88Safari16.5

**Credits**
- Borrowed from: Kevin Powell, Improve forms with :user-valid and :user-invalid, 2025-02-20, and Use these CSS features instead of JavaScript, 2026-09-23

## 8. Textarea that grows with its content

**When:** Use it for any multi-line input, such as a chat composer or a comment box. `field-sizing: content` sizes the textarea to its text, so there is no auto-grow script and no hidden mirror element.

```css
textarea {
  field-sizing: content;
  min-height: 3lh;
  max-height: 12lh;
  resize: none;
}
```

**Why it works**
- The browser measures the text itself, so the size is right on first paint, on paste and on window resize.
- The textarea reaches its final height at once. Only the wrapper animates, and it clips the textarea until it catches up.
- `lh` is one line at the element's own line-height. `min-height: 3lh` means three lines at any font size.

**Rules**
- Bound it with `min-height` and `max-height`. A fixed `height` brings the fixed size back. Past `max-height` the textarea scrolls.
- Put the border and background on the wrapper and leave the textarea bare. The box the user sees must be the one that animates.
- Keep the wrapper `content-box`. The observer writes the textarea's full height, and a `border-box` wrapper would subtract its own border from it.
- Write no fallback. A browser without `field-sizing` shows a fixed textarea that scrolls.

**Support**
- Baseline newly available since June 2026.

**Credits**
- Borrowed from: Jhey Tompkins, @jh3yy on X, 2026-10-01
- Docs: MDN: field-sizing · Adam Argyle, CSS field-sizing · MDN: ResizeObserver
