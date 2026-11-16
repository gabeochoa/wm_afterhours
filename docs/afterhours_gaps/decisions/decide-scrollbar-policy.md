# decide: scrollbar space policy (Tier 2: hanabi, floatinghotel)

**Problem.** `HasScrollView` has `show_scrollbar` (a bool) plus thickness, min thumb and colours. Projects asked for two different things:

- **hanabi #94, overlay/auto-hide.** It wants macOS overlay scrollers: nothing at rest, a bar while scrolling or hovering, then it fades. hanabi currently sets `show_scrollbar = mouse_in_subtree || mouse_was_in_subtree` every frame. That misses wheel momentum after the pointer leaves, and keyboard Page Down never shows a bar. Separately, the bar paints over the panel's own trailing edge, covering hanabi's 1px column rule.
- **floatinghotel, reserved gutter.** A focus border on a scroll child was drawn under the bar, so the app reserves 8px in each affected row by hand. It asks for a scroll-container content inset so children don't need to know the bar's width.

These are two separate features, and each has a semantic choice. Together they're more than one small PR.

## Options

1. **Gutter only, now.** Add `HasScrollView::scrollbar_gutter` (bool, default false). Autolayout adds the resolved `scrollbar_thickness` to the trailing padding on each enabled axis, always, like CSS `scrollbar-gutter: stable`. "Only while needed" would make the content width depend on whether the content overflows, which is a layout feedback loop. The bar is drawn in that space. About 40 lines; padding is resolved in two autolayout passes and both need the addition. Pixels don't change unless a view opts in. Overlay mode stays open.
2. **Gutter + auto-hide in one PR.** Option 1, plus `ScrollbarVisibility { Always, WhileActive, Never }` replacing the bool (Always = today's behaviour; `show_scrollbar` becomes a deprecated alias or a compile break), plus `float last_activity` on `HasScrollView`, bumped wherever `scroll_offset` moves (wheel, momentum glide, keyboard reveal, thumb drag) and while hovered. The bar fades out over `scrollbar_fade` seconds after activity stops. Needs a time source in the render pass and a fade-out that also requests frames in on-demand mode. About 150 lines across components.h, systems.h and rendering.h, plus tests at 60/120fps.
3. **Docs only for the gutter.** `with_padding(Padding{.right = h720(6)})` on the scroll view already insets its children while the bar draws over the panel's full rect. So the gutter exists today if the caller matches padding to `scrollbar_thickness` by hand. Document it and do auto-hide as its own PR.

## Recommendation

Option 1 now, then auto-hide (the visibility half of option 2) as a follow-up PR. The gutter is small and opt-in. Auto-hide needs Gabe to decide whether to replace the `show_scrollbar` bool used across about 20 projects or keep it next to a new mode field, and what counts as "activity" (hover? keyboard focus inside?).

**Rough size:** gutter S (about 40 lines + test + example); auto-hide M (about 150 lines).
