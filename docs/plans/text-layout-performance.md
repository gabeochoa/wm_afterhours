# Faster text layout using prepared text

Status: proposed, awaiting code review before implementation. This document adds no runtime changes. No benchmarks or tests were run for this plan.

Prepare text when its content or typography changes. Reflow that prepared text when its available width changes. Use the resulting lines and positions for sizing, drawing, caret placement, selection, and hit testing. This follows the useful part of Pretext's design while keeping afterhours' current rendering behavior as the first correctness target.

The first implementation should remove repeated work in existing text paths. Unicode shaping and browser parity are separate capabilities, with an optional plugin boundary. A fast line counter alone would not fix the current duplication between sizing, drawing, and editing.

## Evidence and scope

Inspected afterhours revision `3a5f2444dd63f0ab9badfadbb5cb3984112d410f`, wm's working tree on 2026-09-13, and Pretext revision [`37d4b741a67e55ce9d1172c82fc41cffa22d18bf`](https://github.com/chenglou/pretext/tree/37d4b741a67e55ce9d1172c82fc41cffa22d18bf). Screen files were being updated during inspection; implementation should record its own wm revision when collecting the baseline.

These are source observations, not measured performance results:

| Existing path | What it currently does | What to investigate |
| --- | --- | --- |
| [`TextMeasureCache`](../../vendor/afterhours/src/core/text_cache.h) | Keeps up to 4,096 whole-string measurements, keyed by a hash of text, font name, exact float size, and spacing. Prunes by generation. | Cache hits still scan text to hash it. Font replacement under the same name is not represented in the key. |
| [`measure_memo`](../../vendor/afterhours/src/measure_memo.h) and [raylib measurement](../../vendor/afterhours/src/backends/raylib/font_helper.h) | A second 4,096-entry cache wraps `MeasureTextEx`. Its key uses text, size and spacing quantized to 1/64, and texture ID/base size/glyph count. | Measure duplicate hashing and cache administration. Same-shaped reloaded atlases can reuse that identity. Quantized measurements can alter a break near a width boundary. |
| [`wrap_runs_to_width` and `wrap_memo`](../../vendor/afterhours/src/plugins/ui/text_selection.h) | Tokenizes spaces and hard newlines; constructs chunks and candidate strings; remeasures growing same-weight stretches. The 512-entry wrap cache stores vectors of copied text runs. | On new widths, a warm measurement cache still leaves tokenization, concatenation, hashing, and layout allocations. Large candidate prefixes can produce quadratic work within long lines. |
| [Wrap cache consumers](../../vendor/afterhours/src/plugins/ui/rendering.h) | Both buffered and immediate styled rendering use a key containing runs, colors, weights, and width. It omits resolved font identity, font size, and spacing. | Reproduce stale wrapping by rendering identical runs at equal widths with different typography. This is a correctness problem before it is a speed problem. |
| [`text_area()`](../../vendor/afterhours/src/plugins/ui/text_input/text_area.h) | Unconditionally calls `state.layout_cache.rebuild()` during widget construction; rebuilds again after content changes. | A class named cache is still rebuilt for idle text areas. Count rebuilds on unchanged frames before changing its lifecycle. |
| [`TextLayoutCache`](../../vendor/afterhours/src/plugins/ui/text_input/text_layout.h) | Wraps into copied strings, then uses `source.find()` to recover source offsets. | Carry source ranges through layout directly. Repeated empty lines and consumed break whitespace deserve explicit mapping cases. |
| [Selection helpers](../../vendor/afterhours/src/plugins/ui/text_selection.h) and [single-line input](../../vendor/afterhours/src/plugins/ui/text_input/component.h) | Hit testing walks UTF-8 codepoints and measures successively longer prefixes. Selection geometry also measures prefixes. | Reuse cached positions. Current codepoint boundaries are not grapheme boundaries. |
| [Autolayout](../../vendor/afterhours/src/plugins/autolayout.h), [`measure_config`](../../vendor/afterhours/src/plugins/ui/measure_config.h), [public measurement](../../vendor/afterhours/src/plugins/ui/text_measure.h), and rendering | Multiple callers ask for related text sizes, wrap the same content, or calculate line geometry. | Count unique text/typography/width questions and make consumers share one answer. |

`FontManager::load_font()` currently replaces map entries without a revision or automatic cache invalidation. No calls to `wrap_memo::clear()` or `measure_memo::clear()` were found in the inspected afterhours `src/` or wm `src/` trees. Directly exposed `FontManager::fonts` also allows replacement outside those methods. Fixing only one load method would leave that route open.

A comment in `measure_memo.h` records an earlier wm observation of 995 measurement calls for 101 distinct questions in a frame. It does not identify this run's screen, build, or timing. Treat it as a reason to instrument, not a current benchmark or proof of where frame time goes.

The upstream [raylib 5.5 implementation](https://github.com/raysan5/raylib/blob/5.5/src/rtext.c#L1148) decodes codepoints and searches the font's glyph array during drawing and measurement. `GetGlyphIndex()` linearly scans that array. wm loads large Japanese and Korean ranges in [`font_config.h`](../../src/font_config.h). This makes glyph lookup a plausible cost in those screens. Confirm the installed raylib implementation and profile it before introducing a lookup table. Faster wrapping will not remove repeated glyph searches inside `DrawTextEx()`.

## What to borrow from Pretext

Pretext's [public API and limits](https://github.com/chenglou/pretext/blob/37d4b741a67e55ce9d1172c82fc41cffa22d18bf/README.md), [prepared data and layout entry points](https://github.com/chenglou/pretext/blob/37d4b741a67e55ce9d1172c82fc41cffa22d18bf/src/layout.ts), [analysis](https://github.com/chenglou/pretext/blob/37d4b741a67e55ce9d1172c82fc41cffa22d18bf/src/analysis.ts), and [measurement](https://github.com/chenglou/pretext/blob/37d4b741a67e55ce9d1172c82fc41cffa22d18bf/src/measurement.ts) support the split. Preparation performs analysis and measurement. The height-only `layout()` walks prepared numeric data without string materialization; richer range APIs expose lines when callers need them.

The actual implementation is more involved than independently measuring words and adding their widths. It includes grapheme fit advances, discretionary hyphens, tabs, line-end adjustments, language rules, and browser-specific measurement policies. Its [line breaker](https://github.com/chenglou/pretext/blob/37d4b741a67e55ce9d1172c82fc41cffa22d18bf/src/line-break.ts) shares break behavior across count and range operations. This is the pattern to preserve.

| Browser behavior | afterhours decision |
| --- | --- |
| Canvas calls the browser's font machinery without DOM layout. | Use the active native backend's metrics. wm already has no DOM reflow to avoid. |
| `Intl.Segmenter` supplies word and grapheme segmentation. | C++ has no equivalent standard facility. Keep existing break behavior initially; an optional Unicode provider must supply tested segmentation and line-break rules. Word segmentation alone is not UAX #14 line breaking. |
| CSS `normal` collapses whitespace; `pre-wrap` preserves it. | Preserve existing whitespace and hard breaks by default. Do not import unconditional normalization into editors, code, or diff views. |
| Pretext assumes browser font shaping and has browser-specific adjustments. | Do not copy its Blink/WebKit/Gecko detection or emoji corrections into raylib. Native glyph advances and spacing are a different contract. |
| The compact API returns height without drawing. | Keep a cheap metrics operation, but derive metrics and renderable line ranges from the same break walker. |
| Pretext includes approximate bidi levels. | These are not a replacement for a complete bidi/shaping implementation. Its own README says widths cannot position Arabic or mixed-bidi characters. |

Do not repeat the explainer's `~0.0002ms` as an afterhours target. Workload, clock resolution, runtime, preparation cost, and output needs differ. Compare the same native workload before and after.

## Proposed caller experience

Normal UI callers keep using `with_label`, wrapped labels, styled spans, and text inputs. The UI plugin owns prepared state and notices changes. A screen should not need to call `prepare()` every frame or manually invalidate fonts.

An optional direct API supports games, custom controls, and variable-height lists. The following is an API sketch, not code to add yet:

```cpp
auto prepared = text::prepare(source, resolved_style, prepare_options);
text::LayoutBuffer lines;
auto metrics = text::layout(prepared, constraints, lines);
text::draw(prepared, lines, origin, paint);
auto caret = text::hit_test(prepared, lines, point);
```

A metrics-only overload should avoid storing lines. `LayoutBuffer` retains capacity across width changes. Prepared storage owns the text or has explicit shared ownership; it must not retain a view into temporary immediate-mode labels. Line ranges refer to that storage, not into a global cache entry that eviction can destroy.

Minimum useful types:

- `PreparedText` is an opaque immutable value containing source storage, resolved font dependencies, break opportunities, source mappings, and advances. Its representation stays private.
- `ResolvedTextStyle` identifies actual font resources and metric settings. Requested family names alone are insufficient after weight resolution or fallback.
- `PrepareOptions` names whitespace, break policy, locale, and direction where supported. Start with an explicit legacy-compatible policy. Do not advertise Unicode features before a provider implements them.
- `LayoutConstraints` contains available width and vertical line policy. An explicit no-wrap choice replaces internal magic huge widths. Validate non-finite and invalid values at the API boundary.
- `LayoutBuffer` contains source ranges, consumed break ranges, line advances, baseline positions, and the mapping needed by editing. Wrapped lines never become replacement source text.
- `SourceByteOffset`, cluster positions, and visual positions are distinct concepts. Carets include leading/trailing affinity where a soft wrap or bidi boundary has two visual positions.

Start with resolved pixel size. Width changes reuse preparation; font-size changes initially prepare again. Reusing normalized font-unit advances across size changes is a later backend-specific optimization, after proving parity for hinting, spacing, and atlas behavior.

For the current raylib backend, decode and resolve glyph metrics once per preparation. Keep cumulative advances and the current spacing rule. Adding isolated segment widths loses spacing at joins; the existing wrapper deliberately measures contiguous same-weight text together to avoid that. Preserve which spans are metric boundaries and which only change paint. Color changes should not force shaping or wrapping.

For a future shaping backend, prepare context-valid runs rather than independently measured words. Kerning, ligatures, combining marks, Arabic joining, and line-boundary shaping can cross the proposed segment boundary. A provider must return cluster mappings and safe break information. If choosing a break requires reshaping, account for that work explicitly; do not promise a universally measurement-free hot path.

## Cache ownership and invalidation

Use one owner for prepared text in the UI plugin. Stable widget state can retain its handle. Shared text reuse may use a bounded cache, but should not introduce a fourth permanent cache above the existing three. Benchmark a simple per-widget retained value first; shared segment caching is worthwhile only if cross-widget reuse pays for its lookup and memory costs.

| Input change | Required action |
| --- | --- |
| Source bytes or metric span boundaries change | Reprepare affected content. Start with the entire paragraph; incremental editing comes later if measured. |
| Resolved font face, weight variant, size, spacing, variation, shaping features, locale, direction, or break policy changes | Reprepare relevant runs. Include only supported settings, but extend identity when support is added. |
| Font reload/unload, fallback registration, missing-glyph availability, or backend change | Invalidate dependent prepared handles through a resource generation. Reused texture IDs must not revive old metrics. |
| Available width changes | Reflow prepared content. No segmentation or font lookups for the proven raylib path. |
| Line height changes | Recompute vertical geometry without horizontal remeasurement when metrics permit. Mixed-size runs may need per-line ascent/descent maxima. |
| Text color, hover tint, selection color, shadow color, or draw origin changes | Repaint. Preserve prepared metrics and break positions. Effects that intentionally contribute to layout bounds require their own explicit policy. |
| DPI/UI scale changes | Reprepare if resolved size or backend metrics change; otherwise update placement. Do not assume every scale change is width-only. |

Use collision-checked keys or stable content identity with verified equality. Existing hash-only caches do not compare the original key on a hit. Do not quantize width, size, or spacing without making the corresponding rendering quantization identical and tested at break boundaries.

Budget caches in bytes as well as entries. Store source bytes once per prepared object, ranges into it, and reusable arrays. Track current bytes, peak bytes, entry count, hits, misses, and evictions. Include memory held by live widgets after shared-cache eviction. Expose a clear/release path for closing a document or screen. Never use an unbounded global segment map for changing profiler numbers or user-entered text.

Begin on the UI thread. Pure reflow over immutable preparation can later run elsewhere with caller-owned output. Font lookup, atlas creation, mutation, and cache eviction retain an explicit owner; no global cache lock belongs in the hot layout loop.

## Keep the core small

Put the prepared layout implementation in an optional `text_layout` plugin, with UI depending on it. The ECS core should not own Unicode tables, font fallback policies, paragraph layout, or measurement caches. `core/text_cache.h` is already UI-specific despite its location; migrate its callers into plugin ownership once the replacement proves itself.

Keep the backend adapter narrow. It resolves a font resource and prepares the metrics or shaped runs that both drawing and layout understand. Color remains a paint value. Animation changes the ordinary style or width values; the text plugin compares resolved inputs and invalidates the right layer. Neither the color plugin nor animation needs to know text-cache internals.

Unicode segmentation and shaping can be separate optional providers. For a later macOS implementation, CoreText can supply shaping and font fallback behind the same public text API. Cross-platform candidates include HarfBuzz for shaping and a maintained Unicode segmentation/bidi library. HarfBuzz alone does not provide font fallback or a complete paragraph bidi and line-break service. Keep those dependencies out of core and evaluate their size, license, portability, and correctness before selecting them.

A new shaping provider must also draw its shaped glyph positions. Measuring with CoreText and then passing original strings to `DrawTextEx()` would measure one renderer and draw another. The initial provider therefore stays raylib-compatible. Do not claim Arabic support from a faster wrapper over the current codepoint renderer.

## Implementation sequence

Each phase is a separate reviewable change with evidence attached. No afterhours changes belong in the current screen cleanup work.

1. **Collect a baseline and expose repeated work.** Add opt-in text counters and scopes using the existing profiling system. Count preparation/rebuilds, wraps, candidate bytes copied, measurement calls and misses, glyph lookup time, layout time, draw time, allocations, and retained bytes. Measure idle, resize, edit, scroll, font switch, and dynamic-value workloads separately. Reproduce the omitted wrap-cache inputs and same-name font reload cases before fixing them. Record toolchain, actual raylib version, build flags, font file hashes, atlas sizes, resolution, and profiler state.
2. **Fix cache correctness and idle rebuilds.** Introduce font resource generations and complete typography keys. Give text-area layout a dirty check that includes text, width, typography, and vertical metrics. Keep the existing wrapping algorithm so its effect is measurable independently. Migrate direct font-map mutation where necessary. Verify that changing color does not invalidate metrics, while changing an actual face does.
3. **Add prepared ranges for the current backend.** Replace copied tokens and candidate concatenation with source ranges and prepared advances. Carry hard breaks and consumed spaces through layout. Implement one break walker for metrics and line output. Preserve current overflow, indentation, trailing-space, blank-line, inset, and auto-fit behavior. Benchmark a per-font codepoint lookup table only if profiling identifies glyph search as material.
4. **Share results across sizing and rendering.** Route autolayout, `measure_config`, public wrap measurement, buffered rendering, and immediate rendering through the prepared result. Resolve typography through the same path. Retain shaped/metric runs across paint-only span changes. Preserve text effects, clipping, alignment, and rotation; track geometry mismatches separately from visual pixel differences.
5. **Use prepared positions for editing.** Migrate single-line input, text areas, read-only selection, and clipboard mapping. Hit testing uses stored positions and source offsets. Binary search is valid only for monotonic LTR advances; negative tracking or visual bidi runs need a suitable search. Keep source text byte-identical across soft wraps. Test empty hard lines and selections crossing consumed spaces explicitly.
6. **Consolidate and decide the next scope.** Remove replaced caches and duplicate wrapping paths after all callers migrate. Set default memory budgets from the measured working sets. If latency is still dominated by glyph drawing, address that separately. If multilingual product requirements justify it, propose the Unicode/shaping provider as its own reviewed feature with shared API and explicit supported behavior.

## Native benchmark corpus

Use the actual current source strings and typography from these screens, with revisions recorded. Preserve their existing e2e coverage; add focused performance workloads rather than timing the whole screenshot runner.

| Screen or workload | Exercise |
| --- | --- |
| [`MultiLineTextLab`](../../src/systems/screens/MultiLineTextLab.h) and `FitContentLab` | Identical text measured and rendered; hard newlines; whitespace; resize-driven wrapping and height changes. |
| [`StyledTextLab`](../../src/systems/screens/StyledTextLab.h), [`FontWeightShowcase`](../../src/systems/screens/FontWeightShowcase.h) | Color-only boundaries, contiguous metric runs, actual DGOne and Archivo weight faces, missing-weight fallback. |
| [`LanguageDemo`](../../src/systems/screens/LanguageDemo.h) | English, Korean, Japanese, named fonts, fullwidth punctuation, large glyph arrays, repeated language switches. |
| `text`, `text_overflow`, `text_shadow`, `text_stroke` | Negative/positive tracking, auto-fit, exact wrap and ellipsis thresholds, multiple paint passes sharing layout. |
| `text_input`, `text_input_lab`, `text_input_sizes`, `ComposerLab` | Idle fields, typing, undo/redo, selection drag, caret scrolling, password/display text, long single-line input, wrapped editor content. |
| `SyncScrollLab` | Code and diff text with indentation, hard lines, source offsets, and selection that must preserve clipboard bytes. |
| [`VirtualListLab`](../../src/systems/screens/VirtualListLab.h) | 10,000 logical rows, only visible and overscan rows built. Scroll through unique row strings without preparing all rows or growing the cache indefinitely. |
| `AIMChatDemo`, `system_profile_lab`, and live profiler | Repeated messages versus continuously changing numeric labels. Compare profiler hidden/visible and recording stopped/running. |

Add constructed correctness cases that actual screens do not cover: empty text, repeated blank lines, tabs, CRLF, nonbreaking spaces, zero-width space, word joiner, soft hyphen, very long tokens, combining accents, emoji ZWJ sequences, flags, Thai, Arabic, and mixed bidi. Classify each as legacy-compatible, supported by the Unicode provider, or explicitly unsupported. Missing glyphs must be visible in the test result rather than counted as a successful Unicode layout.

Measure cold preparation, repeat preparation, same-width idle reuse, changing-width reflow, single edits, and continuously changing content separately. Report time per operation and frame contribution, p50/p95/p99, calls, copied bytes, allocations, retained memory, and peak memory. Sweep lengths and widths to reveal scaling; do not measure only one short paragraph.

Run with `nice -n 10`, controlled `make -j2` builds, no concurrent screen capture/build jobs, and fixed input sequences. Use optimized builds for performance decisions and the normal development build to report what `make run` feels like. Warm fonts separately from layout so startup rasterization is not attributed to wrapping. Batch tiny operations long enough to exceed timer noise. Alternate baseline/candidate order over repeated runs on the same machine. Measure profiling overhead independently and keep detailed text tracing off during final timing.

The first baseline report sets numeric CPU, memory, and tail-latency budgets. Structural goals can be specified now: unchanged text should not rebuild; proven warm raylib reflow should not call the font backend or materialize strings; metrics-only layout should not allocate; line output should reuse capacity; and cache memory must remain bounded during unique-text churn. These are acceptance checks, not claims about current results.

## Correctness and acceptance

The initial oracle is the actual native renderer with the same loaded font resources, size, spacing, wrapping policy, and inset. Existing screenshot baselines catch visual regressions; they do not alone establish source mapping or line-break correctness.

- Capture line source ranges, line counts, advances, baselines, block bounds, and glyph positions where available. Compare the old and new path at widths just below, at, and above every observed break. Include fractional sizes and widths, 720p, 1080p, and the narrow window fixtures.
- Render both versions with the real raylib backend and actual font atlases. The no-graphics backend and its estimated text metrics are insufficient for pixel parity. Check both buffered and immediate draw paths.
- Assert identical source bytes after selection/copy/paste across soft wraps and blank hard lines. Check hit-test/caret round trips at line ends, before/after spaces, and at each supported cluster boundary. Keep pointer and keyboard behavior consistent with drawn positions.
- Change font size, tracking, weight, registered fallback, and the font loaded under an existing name without restarting. Old prepared handles must either remain tied to valid immutable resources or be invalidated safely. Recycle texture IDs in a controlled test.
- Exercise whitespace preservation and long-word overflow before enabling any new break policy. Current wrapping leaves an oversized word on its own line; Pretext's default can split it at grapheme boundaries. That difference requires an explicit option, not a silent optimization.
- Use a shared walker for height-only and line-producing APIs and compare their counts. A faster measurement path that disagrees with drawing fails even when screenshots happen to look close.
- Characterize existing disagreements instead of preserving them blindly. In raylib 5.5, zero-advance glyph handling differs between `MeasureTextEx()` and `DrawTextEx()`. Keep any intended correction in a separate reviewed change with a reproduction.

For future Unicode behavior, add UAX #29 grapheme, UAX #14 line-break, and UAX #9 bidi conformance cases for the chosen provider/version. Shaping tests need real fonts with ligatures, joining, and fallback, plus visible glyph-position comparisons. Browser DOM comparisons are useful for the HTML mocks only when font files and policies match; browser output cannot be the native oracle while the two renderers support different shaping behavior.

Pretext's [validation record](https://github.com/chenglou/pretext/blob/37d4b741a67e55ce9d1172c82fc41cffa22d18bf/tests/wrapping/VALIDATION.md) is a useful model for preserving known mismatches and recording browser/font/environment changes. If any upstream implementation or generated tables are reused, retain its [MIT license](https://github.com/chenglou/pretext/blob/37d4b741a67e55ce9d1172c82fc41cffa22d18bf/LICENSE) and record provenance; an architectural adaptation does not require copying its browser compatibility code.
