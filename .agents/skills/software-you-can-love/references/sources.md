# Sources

Paraphrased notes, gathered 2026-09-30 from <https://softwareyoucan.love/> and the pages and
talks it links. Talk notes come from YouTube auto-captions, so details may carry transcription
errors. No source text is reproduced beyond short phrases.

## The site

Software You Can Love is a conference series (Milan 2022 and 2024, Vancouver 2023 and 2026)
from the Zig community, plus single-day Zig Days. It frames making software as an intimate act,
like painting or music, and celebrates software made for humans: tools for music, generative
art, cross-stitching, rethinking computing's building blocks. It names the problem directly:
much software is user-hostile, and elegant solutions get discarded for extravagant business
models. It also wants people to learn to defend themselves against dark patterns.

## Loris Cro, "Playing the Open Source Game" (kristoff.it, 2021)

- Coined "software you can love" because neither open source nor free software captured the
  ideal; keep the good of both, drop the flaws.
- Source availability helps learning and right-to-repair but is not the point. A fairly priced,
  solid proprietary tool can beat janky open source with a murky business model.
- Caps on lovability: bad UX, good UX that nags for engagement, bloat, jank, short shelf life.
- Hostile examples: email wall for a driver update, subscription for a garage opener, forced
  new login after an acquisition, "Try Free" upsells on an open-source project's site.
- Root causes: startups built as acquisition moats for a quick exit, VC pressure bending
  design to cover business weaknesses, companies and clouds treating OSS as licence terms.
- Respect original design intent; don't bolt on features that contradict the core model.
- Prefers small, independent or non-profit structures that keep the creator's final say.

## Loris Cro, "My Software North Star" (kristoff.it, 2026)

Ranked: useful and lovable, then correct, then maintainable and efficient. Each lower rank is
meaningless if a higher one fails. Developer experience matters only as far as it delivers
lovable software. He admits detours; the star sets direction.

## Mindy Preston, "CLI Tools Are Not Inherently User-Hostile" (SYCL 2022)

- The terminal is a portable platform like the browser. The things developers love about CLIs
  (composable, copy-pasteable, scriptable, no hidden state, no ads) should reach everyone.
- Don't be the wizard on the hill; give users the magic.
- Copyable text for commands, instructions and errors; a text record lets anyone catch up on a
  missed step. Users report GUI error boxes by drawing a squiggle where the text was.
- Any output into any input. Maximal apps only allow relationships they anticipated.
- Prompts can be silenced. Never paywall the user's own work (cross-stitch apps charging to
  export your own pattern); never put ads in the workflow.
- Reuse off-the-shelf parts for the boring bits; build for the common need (text in a border
  wants a template, not a pixel editor).
- GUIs and CLIs share the discoverability problem.

## Jeff Fowler, "Does Technology Destroy Art?" (SYCL 2022)

- Art is a personal pursuit of perfection; a better machine cannot take the process away. But
  technology can destroy the markets and communities that keep skills alive.
- Programming is a craft with aesthetics. Good tools widen range (pianoforte, perspective).
- Changing the tool changes how the work feels, even when output is similar.
- Constraints help (four-channel Game Boy chiptunes). Don't judge tech by its first version
  (early digital cameras). Disrupt with empathy; the Luddites had a point.

## Stevie Hryciw, "A Language for Making Languages for Making Synthesizers" (SYCL)

- Build the tiny embeddable core others use to make their own tools, not another closed app.
- Library over application; one job (DSP), no UI or sequencing; tiny core model; grow only as
  real synths demand; a big library of worked examples.
- Hard constraints are features: real-time audio must never glitch, so constant-time work.
- Under 10 ms feels instantaneous.
- A platform is a curiosity safety net: go deep without losing the original goal.
- Anti-patterns: GUI-only tools with no headless runtime, DSLs built by string gluing, tools
  that do too much, architectures that rule out constrained targets.

## Stephen Gutekanst, "gkurve: simpler vector graphics" (SYCL 2022)

- Start from what the GPU understands (triangles) and build the artist interface around it,
  instead of layering tricks onto cubic Béziers.
- Pick the dumb, sufficient primitive; keep primitives local; composability gives texturing,
  gradients and animation for free; a small readable shader.
- If a tool needs a game to teach it (the Bézier Game), it is too complex.
- Old conventions such as even-odd fill are choices, not laws.

## Karsten Schmidt, "Bottom-up Design" (SYCL 2022)

- Durable creative tooling grows bottom-up from many small composable packages built over
  years for concrete needs (thi.ng, about 170 packages).
- Build for a market of one, ten or a hundred. Borrow concepts across languages; skip tribal
  fights.
- Explicit control flow and allocation over magic; teach the fundamentals.
- Tiny vocabularies generate a lot (an eight-verb shape language). Interfaces should disappear
  (tangible marker cards children used without instruction).
- Anti-patterns: beginner tools that hide how things work, manufactured demand built to exit.
