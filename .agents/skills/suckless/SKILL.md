---
name: suckless
description: Applies the suckless.org philosophy (simplicity, clarity, frugality) to product and code decisions. Treats removed lines as progress, designs for experienced users instead of an imagined average user, refuses features that blur the concept, and calls for a rewrite when accreted complexity has broken the design. Use when scoping a feature, reviewing a design or diff for bloat, choosing between a simple tool and a configurable one, or deciding whether to patch or rewrite. Skip for fact lookups and for work where the customer explicitly wants the heavier option.
---

# Suckless

Source: <https://suckless.org/philosophy/>, paraphrased. Simple, minimal, usable software is
the goal; complex, slow, error-prone software is the industry default to push against.

## Core claims

1. **Simplicity, clarity, frugality.** These three are the quality bar, not nice-to-haves.
2. **Lines of code are not progress.** More code is not more skill. Removing code is progress;
   a shrinking codebase is the sign of a better programmer and software that sucks less.
3. **Simple is harder than bloated.** Letting ad-hoc and over-ambitious features pile up is the
   easy path. Paying for a simple, elegant design up front is what buys reliability and
   maintainability.
4. **Minimalism makes goals finishable.** A small scope is attainable; a clear scope gets
   driven to completion instead of sprawling forever.
5. **Complexity is the root defect.** It produces bloated, hard-to-use, inconsistent software:
   suboptimal solutions, resources tied up indefinitely, slow performance, security holes.
6. **"It works, keep it" compounds.** Accepting whatever first seems to solve the problem,
   repeated over a codebase's life, leaves a broken structure and a flawed design. The cause is
   missing conceptual clarity and integrity, not missing effort.
7. **Rewrite when the design is gone.** Once complexity has taken over, patching does not fix
   it; a from-scratch rewrite does. Pride in large code and in "understanding" it is what stops
   rewrites from happening.
8. **Serve experienced users.** They are the neglected market. Build tools that fit how skilled
   people actually work rather than flattening everything for a hypothetical novice.
9. **Ingenious means simple.** Simplicity is the heart of the Unix philosophy.

## Applying it

Before building:
- State the concept in one sentence. A feature that does not fit that sentence is out.
- Ask what can be removed to meet the need, before asking what to add.
- Prefer one sharp tool that composes with others over one tool with many modes.
- Configuration is code the user has to carry. Pick a good default; add a knob only when a real
  user needs it.

While building:
- Measure a change by what it lets you delete as well as what it adds.
- Resist the first thing that works when it muddies the structure. Clarity is part of done.
- Keep the whole thing small enough that one person can hold it in their head.

When reviewing:
- Flag features that exist because they were easy to add, not because the concept needs them.
- Flag abstractions, layers and options with a single user.
- Flag slowness and resource use as defects, not polish.
- When every fix fights the structure, say so and propose a rewrite with its scope and cost.
  Do not keep patching a design that is already gone.

## Tension with other defaults

- **Experienced users vs. first-run ease.** Suckless optimises for the skilled, repeat user.
  When the primary customer (see `customer-obsession`) is a novice, keep the simplicity but do
  not import the hostility to hand-holding. Name the customer first, then decide.
- **Rewrite vs. incremental.** A rewrite is irreversible-ish and expensive. Propose it with
  evidence (how many fixes fought the structure, what the rewrite deletes); do not start one
  unasked.
- **Explicit intent wins.** If the customer asks for the heavier option, say once what it costs
  in complexity, then build it.

## Relation to other skills

- `customer-obsession`: decides whose outcome matters; this skill keeps the solution small.
- `principle-subtract-before-you-add`, `ponytail-lazy-coding`: the code-level version of
  claim 2.
- `principle-redesign-from-first-principles`: the method for claim 7.
- `principle-minimize-reader-load`: claim 6's clarity at the line level.
