---
name: software-you-can-love
description: Builds software people can love rather than merely tolerate, following the Software You Can Love movement (Loris Cro, SYCL talks). Ranks end-user value over correctness over maintainability and efficiency, rejects user-hostile patterns (nags, lock-in, paywalled user work, engagement bait), gives users power instead of black boxes, and grows small composable cores from real needs. Use when scoping a product or tool, reviewing a feature for user hostility, designing a CLI, library or creative tool, or deciding what a user should be able to do with their own work. Skip for fact lookups and mechanical edits with no user-facing outcome.
---

# Software you can love

Software is made for humans, and making it is a craft. The goal is software a person is glad
to have in their life, not software they are stuck with. Sources and per-talk notes:
`references/sources.md`.

## 1. The north star (ranked)

1. **Useful to the end user**, aiming to be loved.
2. **Correct.** Bugs subtract from what the user gets.
3. **Maintainable and efficient.** Waste neither people nor machines getting more value out.

A lower rank is worthless if a higher one fails. No bugs means nothing if the product is a
rugpull. A memory-safe language means nothing without designing for correctness and a process
that drives bugs to zero. A beautiful abstraction stack means nothing if it is slow or nobody
can maintain it. Developer experience counts only as far as it ships more lovable software.

## 2. What caps how much software can be loved

Reject or flag these on sight:

- Bad UX, bloat, jank.
- Good UX that keeps nagging: re-asking for notifications, ratings, sign-ups after a no.
- Gating: email for a driver download, an account or subscription for something that works
  locally, a forced login after an acquisition.
- Paywalling the user's own work: free to make, pay to export.
- Ads or upsells inside the workflow or the core product.
- Hidden state that works against what the user is trying to do.
- Short shelf life from poor engineering choices.
- Features bent to serve the business model at the cost of the design.

The test: does this exist mainly for engagement, lock-in or funnelling to a purchase? If yes,
it does not ship without the user getting something real in return.

## 3. Give users power, not a wizard on a hill

- Show how things work. Don't hand out opaque results and fetch quests.
- Make output copyable text: commands, instructions and especially errors, so they can be
  pasted into a bug report or a search.
- Let any output feed any input. Don't restrict connections to the ones you anticipated.
- Every prompt can be silenced (flag, config, pipe). "Don't ask again" means it.
- Each operation is a well-behaved part of a larger system (see clig.dev for CLIs). How the
  pieces compose is the UX.
- Hold GUIs and CLIs to the same discoverability bar. A blank canvas is not friendlier than a
  blank prompt.
- Build for the common need first. The popular case wants a template, not a pixel editor.

## 4. Build small cores that compose

- Ship a library before an application, so people drive it from the tools they already use.
- Do one job. Leave UI, I/O and adjacent domains to others.
- Keep the core model tiny: a few primitives, everything else built on top. A small vocabulary
  should generate a lot.
- Grow the core only when real use demands it. Build the actual examples first and let them
  show which primitive is missing.
- Build for a real market of one, ten or a hundred, not manufactured demand.
- Start from what the machine natively does, then shape the human interface around it. Old
  conventions are choices, not laws; question them when the hardware has moved.
- Pick the dumb, sufficient primitive. Add one simple piece before adopting a complex standard.
- Be explicit about control flow and allocation. Zero-config magic is undebuggable when it
  breaks.
- If using the tool needs a game to teach it, the tool is too complex.

## 5. Respect the craft

- Feel is a product property. Latency under about 10 ms feels instant and makes iteration
  possible; a tool that drifts or stutters changes the whole experience.
- Hard constraints are features. If it must never glitch, design for bounded, constant-time
  work.
- Expand what users can do; don't replace the process they enjoy.
- Constraints can make better work. Not every limit is friction.
- Don't judge a technology by its first version, and don't dismiss the people it displaces.
- Serve the curious without swallowing them: a good platform lets people go deep and still
  finish the thing they set out to make.

## 6. Checklist for a review

- [ ] What does the end user gain? Name it.
- [ ] Anything here for engagement, lock-in or upsell rather than the user?
- [ ] Can the user get their own work out, free, in an open format?
- [ ] Can every prompt or nag be turned off permanently?
- [ ] Are errors copyable and do they say what to do next?
- [ ] Can outputs feed other tools, and can the tool be scripted?
- [ ] Is the core small enough to explain in a few primitives?
- [ ] Is it correct by design, with a path to zero known bugs?
- [ ] Is it fast enough to feel instant where it matters?
- [ ] Will it still work in five years without us?

## 7. Tensions

- **Money vs. integrity.** Projects need revenue. Charge fairly and honestly for real value; a
  well-priced, solid proprietary tool beats a janky open one with a murky business model.
  Refuse revenue that comes from degrading the user's experience.
- **Openness vs. quality.** Source availability helps learning and repair, but quality and an
  honest model matter more.
- **Power vs. first run.** Exposing how things work must not mean abandoning newcomers. Good
  defaults and templates first, full power one step away.
- **Explicit intent wins.** If the customer asks for the pattern this skill flags, say once what
  it costs the user, then build what was asked.

## 8. Relation to other skills

- `customer-obsession`: names whose outcome this is for; this skill defines what lovable means
  for them.
- `suckless`: the simplicity half of §4, stated harder.
- `principle-experience-first`, `principle-prove-it-works`: the outcome bar and its evidence.
