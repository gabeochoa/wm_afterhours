# Spacing and shape
_Source: good-css by Vojta Holík, https://good-css.com (github.com/vojtaholik/good-css). Copied 2026-10-07 for Gabe's agents; credits per entry are the original authors'._

## 1. Section spacing that depends on its neighbors

**When:** Use it on any page built from reorderable sections, above all in a CMS where an editor decides the order. When two particular sections meet, the spacing between them changes on its own.

```css
section { padding-block: 6rem; }

.logos:has(+ .features) { padding-block-end: 2rem; }
.features:has(+ .cta) { padding-block-end: 0; }

.hero + .logos { padding-block-start: 2rem; }
.features + .faq { padding-block-start: 3rem; }

.logos:where(.hero + *) { border-block-start: 1px solid; }
```

**Why it works**
- `A:has(+ B)` looks ahead. It matches A when B follows it, so A adjusts its own bottom edge.
- `A + B` looks back. It matches B when A precedes it, so B adjusts its own top edge.
- The rules describe pairs, not pages. Whatever order an editor picks, each pair that meets gets its spacing and every other section keeps its default.
- `:where()` adds no specificity, so a default that depends on the neighbor stays easy to override.

**Rules**
- A section owns its default padding. Write a pair rule only for a meeting that looks wrong with the defaults.
- Never fix spacing with a spacer element, a per-page override, or a modifier class the template has to work out.
- Put the rule on the section whose edge changes. Look ahead to change a bottom edge and look back to change a top edge.
- Name sections by what they are, such as `.hero` and `.faq`, never by the page they sit on. The pair rules depend on those names.
- Keep the list short. When many pairs share a reason, such as two sections with the same background, write one rule for the reason.
- Where the authoring system has no sibling selectors, keep these few rules in a plain stylesheet.

**Support**
- `:has()` in Chrome 105, Firefox 121, Safari 15.4. The `+` combinator works everywhere.

**Credits**
- Borrowed from: house pattern, used on production CMS sites · Kevin Powell, The adjacent sibling combinator · Jen Simmons, Using :has() as a CSS Parent Selector and much more, 2022-08-18

## 2. Space between siblings set by the parent

**When:** Use it wherever elements sit one above another, such as form fields, the parts of a card or the blocks of an article. The parent sets one space between its children, and no child carries a block margin of its own.

```css
.fields {
  display: flex;
  flex-direction: column;
  gap: 1rem;
}

.prose > * { margin-block: 0; }
.prose > * + * { margin-block-start: var(--flow-space, 1em); }

.prose > :is(h2, h3) { --flow-space: 2em; }
.prose > :is(h2, h3) + * { --flow-space: 0.5em; }
```

**Why it works**
- A margin describes the relation between two neighbors, so the element itself is the wrong place for it. A paragraph with `margin-block-end` leaves that margin behind when it is the last child of a padded box.
- `gap` puts space between flex items only. Nothing is left above the first child or below the last.
- `* + *` matches every child that has a sibling before it, so the first child gets no margin. Heydon Pickering named the selector the lobotomized owl.
- The fallback in `var(--flow-space, 1em)` is in `em`, so the space above each element follows that element's font size.
- A custom property on a child changes the space above that child alone. A heading asks for more room above itself, and whatever follows a heading asks for less.

**Rules**
- Use the flex form in a component whose children you know. Use the margin form for content you do not control, such as CMS or Markdown output, because flex turns every child into a flex item.
- A flex column stretches its children, so a button or link that is a direct child becomes full width. Set `align-self: start` on it.
- In the margin form, zero the children's block margins first. Without that line the browser's default margins stay above the first child and below the last.
- Keep the `>`. Without it the rule reaches every nested element, list items included.
- The margin form counts a hidden child, so a hidden first child leaves a space at the top. `gap` ignores hidden children.

**Support**
- `gap` in flex layout in Chrome 84, Firefox 63, Safari 14.1. `:is()` in Chrome 88, Firefox 78, Safari 14.

**Credits**
- Borrowed from: Heydon Pickering and Andy Bell, The Stack, Every Layout · Heydon Pickering, Axiomatic CSS and Lobotomized Owls, 2014-10-21 · Andy Bell, My favourite 3 lines of CSS, 2023-02-06. Both authors keep the margin form everywhere. This version uses `gap` in components.
- Docs: MDN: gap · MDN: next-sibling combinator

## 3. Push one item away with an auto margin

**When:** Use it when one item in a flex row or column sits apart from the rest, such as the actions at the bottom of a card, the account link at the end of a toolbar, or a title centered in a full-height section between a header and a footer.

```css
.card {
  display: flex;
  flex-direction: column;
  gap: 0.5rem;
}

.card > .actions { margin-block-start: auto; }

.toolbar {
  display: flex;
  gap: 1rem;
}

.toolbar > .account { margin-inline-start: auto; }

.hero {
  display: flex;
  flex-direction: column;
  gap: 1rem;
  min-block-size: 100svh;
}

.hero > h1 { margin-block: auto; }
```

**Why it works**
- In a flex container an auto margin takes all the free space on its side. One auto margin pushes the item, and everything after it, to the far end.
- Two opposite auto margins split the free space, so the item sits centered between its neighbors. The header stays at the top and the footer at the bottom, and either can be missing.
- `gap` still applies. It is the least space the pushed item keeps when there is no free space left.

**Rules**
- The container needs free space. A card has it when a grid row stretches it to match a taller neighbor. A column that is the only child of a taller box needs `block-size: 100%`.
- The centered item is centered in the space left over, not in the section. A footer with no header pulls the title up by half the footer's height.
- Write `min-block-size`, never `block-size`. A fixed height cuts off long content.
- This is a flex technique. In a grid column the spare height goes to the rows first and every child grows.
- It replaces a spacer element, `space-between` on a container with more than two children, and a wrapper around the group that should stay together.

**Support**
- Auto margins work wherever flexbox does. `margin-block` in Chrome 87, Firefox 66, Safari 14.1. `svh` in Chrome 108, Firefox 101, Safari 15.4.

**Credits**
- Borrowed from: Heydon Pickering and Andy Bell, The Stack, section "Splitting the stack", and The Cover, Every Layout. The Cover gives every child a margin and removes it at both ends. `gap` does that in one declaration.
- Docs: MDN: Aligning items in a flex container

## 4. Concentric nested radius

**When:** Use it wherever a rounded element sits inside a padded rounded parent, such as an image in a card or a button in an input.

```css
.card {
  --radius: 0.75rem;
  --pad: 0.5rem;
  padding: var(--pad);
  border-radius: calc(var(--radius) + var(--pad));
}

.card > * { border-radius: var(--radius); }
```

**Why it works**
- Two curves look parallel only when they share a center. That means outer radius = inner radius + gap. Equal radii make the corner band look pinched.

**Rules**
- Derive the outer radius from the inner one. The other way round reaches zero once the padding exceeds the radius.

**Support**
- Every browser.

**Credits**
- Borrowed from: Chris Coyier, The Classic Border Radius Advice, Plus an Unusual Trick, 2024-05-13, which quotes Jhey Tompkins's version
