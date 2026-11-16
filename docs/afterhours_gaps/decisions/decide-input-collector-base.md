# decide: input-collector system base (Tier 3; pong-afterhours)

**Problem.** pong's `ImpulseBall` and `ImpulsePaddle` each declare `input::PossibleInputCollector inpc;`, fill it in `once()` and check `has_value()`. That is about 4 lines per system. pong's own gap file says: "Saves about 8 lines across 2 systems. Revisit when a third game wants it."

**Options**
1. **Do nothing here. Recommended.** gap/enum-actions (`input::is_pressed(Action)` / `is_held(Action)`) already removes the collector from `ImpulseBall`: its `any_of` over `inputs_pressed()` becomes `input::is_pressed(InputAction::Launch)`. Only `ImpulsePaddle` still reads the collector, because it filters by pad id.
2. **Pad-filtered queries (about 15 lines).** `input::is_held(Action, GamepadID pad)` returns true for that pad's gamepad input or any keyboard input, which is exactly pong's per-paddle rule. With it, neither pong system touches the collector. Do this only if a second game wants per-pad filtering.
3. **`input::System<Components...>` base (about 20 lines).** A base that fetches the collector in `once()` and hands it to `for_each_with`. It adds a second System type to learn, and every input query a game needs can live in free functions instead. I don't recommend it.

**Recommendation:** option 1 now. gap/enum-actions makes the base unnecessary for pong's launch path, and pong's own note says to wait for a third game. If Gabe wants pong to drop the collector entirely, option 2 is the smaller follow-up. Size: 0 lines for option 1, about 15 lines plus a test for option 2.
