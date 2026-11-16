# decide: standard transform and velocity components (Tier 3: pong-afterhours)

**Problem.** afterhours has no shared position-with-`prev` or velocity component, so each game writes its own. pong's entry (`pong-afterhours.md`, "Standard `Transform` / `HasVelocity`") asks for one that keeps `prev`, because its swept collision uses it. But pong itself says "Wait for a second consumer", and lists collision response and the like as "explicitly not gaps": the library shouldn't turn pong into configuration.

Only one project asked. Other games have their own shapes: cartographer and kart are 3D, pharmasea uses vec2 plus facing, and the catalog examples define a dozen different `Transform`s with different fields. A shared type would have to choose 2D vs 3D, whether rotation and scale are included, and whether `prev` is updated automatically (and by which system, in which phase).

## Options

1. **Do nothing yet** (pong's own recommendation). Revisit when a second 2D game wants the same `{pos, prev}` + `{vel}` shape.
2. **Add a minimal 2D pair.** `afterhours::Position2D { vec2 pos, prev; }` and `Velocity2D { vec2 vel; }`, plus an opt-in `IntegrateVelocity` fixed-update system that sets `prev = pos` and then `pos += vel * dt`. About 40 lines plus a test and an example. It's a standing commitment to one game-shape in a library that keeps game rules out.
3. **A documented recipe.** A catalog example showing the `prev`-keeping pattern with a fixed-update system (which now has a settable rate, see gap/fixed-timestep). No API to maintain.

## Recommendation

Option 1, with option 3 if Gabe wants something visible. The single requester asked us to wait, and the existing shapes across projects don't agree.

**Rough size:** option 2 is S. Option 3 is one example, about 60 lines.
