---
name: good-css
description: Opinionated modern CSS techniques (good-css, ~48 entries) for building or reviewing any web UI — use when writing or fixing CSS, styling a page or dashboard (OW/Deck, Nest apps, internal tools), when text overflows or gets cut off, cards or grids misalign, colors/dark mode/tokens are messy, focus/hover/press states are missing, modals/popovers/accordions need animation, or scroll areas misbehave.
---

# good-css — modern CSS practices

Source: good-css by Vojta Holík, https://good-css.com (github.com/vojtaholik/good-css). Copied 2026-10-07 for Gabe's agents; credits per entry are the original authors'.

Plain CSS, no packages or class names to adopt. Each entry is: when to use it, the code, why it works, the rules, browser support and credits. Read the reference file for the category you need; don't load all eight.

## How to apply (house workflow)

1. **Audit first** on an existing page: count hardcoded colors, distinct font sizes, `outline:none`, missing `:focus-visible`, `:hover` outside `@media (hover:hover)`, numbers without `tabular-nums`, and duplicate selectors. Check the **served** stylesheet, not the source you think is deployed.
2. **Biggest wins first**, usually: the reset (`*{min-width:0}`, `text-wrap:pretty`, `overflow-wrap:break-word`, `scrollbar-gutter:stable`) → color tokens in OKLCH + `color-mix` → one type scale → `tabular-nums` → focus/hover/press → scroll behavior.
3. **One change per deploy** when a page is live, so a regression points at one cause. Screenshot before and after.
4. In an existing project, add reset rules **one at a time**: `min-width:0` and the font rules change existing rendering.

## Lessons from applying it (OW, 2026-10-07)

- `*{min-width:0}` was the real root cause of titles clipping in grid/flex cards (children refused to shrink below their content). Clamps and ingest truncation had only treated the symptom.
- When bulk-replacing hex colors, don't match `#` preceded by `&`: `&#128164;` is an HTML emoji entity. Assert the entities survive.
- A duplicate later selector silently overrides your fix; grep the served CSS for the selector after deploying.

## Index

| Category | Entry | When to use | Reference |
|---|---|---|---|
| Foundations | The reset | Start every new project with it. In an existing project add one rule at a time and look at the result,… | `references/foundations.md` |
| Foundations | Logical properties | Use them in place of every left, right, top and bottom, in spacing, borders, offsets and alignment. | `references/foundations.md` |
| Foundations | OKLCH color | Write colors in `oklch()` and derive every related color from a base with `color-mix(in oklch, …)`. Never… | `references/foundations.md` |
| Foundations | One set of color tokens for light and dark | Use it on any site with a dark mode. Each token holds both values, so there is no second block of… | `references/foundations.md` |
| Foundations | Fluid sizes with `clamp()` | Use it for anything that should grow with the screen, starting with font sizes and section spacing. One… | `references/foundations.md` |
| Foundations | One fluid scale for type and space | Use it when a design has more than two or three font sizes. Six numbers describe every size, and the… | `references/foundations.md` |
| Layout | Content grid with breakouts | Use it wherever you would reach for a centered max-width container. One grid on the section replaces the… | `references/layout.md` |
| Layout | Intrinsic grid | Use it for any set of equal cards or tiles. It replaces a column count per breakpoint. | `references/layout.md` |
| Layout | Subgrid rows shared across cards | Use it when cards in a row have parts that should line up, such as title, body and footer, whatever the… | `references/layout.md` |
| Layout | Sidebar that wraps on its own | Use it for any pair where one side has an ideal width and the other takes the rest, such as a media… | `references/layout.md` |
| Layout | Container queries with container units | Use it for any component that appears in slots of different widths. The component responds to the space it… | `references/layout.md` |
| Layout | Stack layers with grid | Use it whenever things sit on top of each other, such as text over an image, a badge on a card, or two… | `references/layout.md` |
| Layout | Safe alignment | Use it wherever content is centered or end-aligned in a container that can become too small, such as a tab… | `references/layout.md` |
| Layout | `overflow: clip` over `hidden` | Use it whenever you want to cut off overflow and do not need scrolling. | `references/layout.md` |
| Spacing and shape | Section spacing that depends on its neighbors | Use it on any page built from reorderable sections, above all in a CMS where an editor decides the order.… | `references/spacing-and-shape.md` |
| Spacing and shape | Space between siblings set by the parent | Use it wherever elements sit one above another, such as form fields, the parts of a card or the blocks of… | `references/spacing-and-shape.md` |
| Spacing and shape | Push one item away with an auto margin | Use it when one item in a flex row or column sits apart from the rest, such as the actions at the bottom… | `references/spacing-and-shape.md` |
| Spacing and shape | Concentric nested radius | Use it wherever a rounded element sits inside a padded rounded parent, such as an image in a card or a… | `references/spacing-and-shape.md` |
| Text and media | Long text that wraps, truncates or clamps | Use it on any text that comes from a user or a CMS, such as names, titles, URLs and excerpts. Decide for… | `references/text-and-media.md` |
| Text and media | Image box that holds any upload | Use it on every image, video or embed whose file you do not control, such as thumbnails, cover photos and… | `references/text-and-media.md` |
| Text and media | Tabular numbers | Use it on any number that changes or sits in a column, such as prices, tables, timers and counters. | `references/text-and-media.md` |
| Text and media | Label centered on its letters with `text-box` | Use it on single-line labels in buttons, badges and chips, so equal padding looks equal in any font. | `references/text-and-media.md` |
| Text and media | Icon sized by the text beside it | Use it on every icon that sits next to a label, in buttons, links, list rows and notices. The icon takes… | `references/text-and-media.md` |
| Interaction | One focus ring with `:focus-visible` | Use it as the focus style for every interactive element. Never write `outline: none`. | `references/interaction.md` |
| Interaction | Hover styles only where hover exists | Use it on every `:hover` rule, so a tap on a touch screen does not leave the hover state stuck. | `references/interaction.md` |
| Interaction | Press feedback | Use it on every button and anything else that can be pressed. | `references/interaction.md` |
| Interaction | Hit area larger than the visual | Use it on icon buttons, close buttons and any target that looks smaller than 44px. | `references/interaction.md` |
| Interaction | Whole card clickable from one link | Use it for cards, list rows and tiles where the whole area should navigate. It replaces an `<a>` wrapped… | `references/interaction.md` |
| Interaction | `:has()` for parent and page state | Use it wherever a script adds a class to a parent because of what it contains or what state a child is in. | `references/interaction.md` |
| Interaction | Form feedback with `:user-invalid` | Use it for inline form validation, in place of blur listeners and a "touched" class. | `references/interaction.md` |
| Interaction | Textarea that grows with its content | Use it for any multi-line input, such as a chat composer or a comment box. `field-sizing: content` sizes… | `references/interaction.md` |
| Motion | Opt-in motion | Use it for every transition or animation that moves or scales something. | `references/motion.md` |
| Motion | Motion tokens | Use them in every transition and animation. Two easing curves, defined once, and a rule for durations. | `references/motion.md` |
| Motion | Transition a custom property with `@property` | Use it when one state change should drive several values together, or when a script feeds a number such as… | `references/motion.md` |
| Motion | Shadow change that fades and does not repaint | Use it on cards that lift on hover, most of all in a grid of many cards or with a large blur. | `references/motion.md` |
| Motion | Cross-document view transitions | Use it on any multi-page site to crossfade between pages. It replaces a client-side router or a… | `references/motion.md` |
| Motion | Indicator that slides to the active item | Use it for the underline on a tab row or the bar beside a side nav. It replaces a script that measures the… | `references/motion.md` |
| Show and hide | Enter and exit transitions from `display: none` | Use it for dialogs, popovers and anything else toggled with `display`. It replaces a script that adds… | `references/show-and-hide.md` |
| Show and hide | Popover anchored to its trigger | Use it for dropdown menus and other popovers opened by a click. It replaces a positioning library or a… | `references/show-and-hide.md` |
| Show and hide | Accordion that animates its height | Use it for FAQs and any other disclosure. `<details>` already gives you the toggle, keyboard support and… | `references/show-and-hide.md` |
| Show and hide | Reveal with `clip-path` | Use it for dropdowns, menus and panels that open over the page and whose height is not known. It unrolls… | `references/show-and-hide.md` |
| Scroll and viewport | Carousel on native scroll | Use it for any horizontal row of cards. The carousel is a real scroll container, so swipe, trackpad,… | `references/scroll-and-viewport.md` |
| Scroll and viewport | Scroll area between a fixed header and footer | Use it for the body of a modal, a chat list, a drawer or a sidebar. The panel grows with its content up to… | `references/scroll-and-viewport.md` |
| Scroll and viewport | Styles that apply only when a scroller overflows | Use it on any row that may or may not fit, such as tabs, a toolbar or a table wrapper. It fades the edges… | `references/scroll-and-viewport.md` |
| Scroll and viewport | Anchor targets that clear a sticky header | Use it on any page with in-page links and a fixed or sticky header. | `references/scroll-and-viewport.md` |
| Scroll and viewport | No rubber-band bounce on desktop | Use it on app-like pages with fixed UI, such as a sidebar or a sticky header. On macOS a trackpad scroll… | `references/scroll-and-viewport.md` |
| Scroll and viewport | Content clear of the notch | Use it on fixed headers, bottom bars, toasts and sheets in anything that should feel like an app on a… | `references/scroll-and-viewport.md` |
| Scroll and viewport |  |  | `references/scroll-and-viewport.md` |
