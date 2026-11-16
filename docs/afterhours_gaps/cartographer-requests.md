# Afterhours Library Requests

## Input System

- **Mouse delta as an action**: `GetMouseDelta()` for camera look can't go through the action mapping system since it's a continuous 2D delta, not a button/axis-with-direction. Would be nice to have a way to map mouse movement to actions so mouse sensitivity and rebinding could go through the same system as everything else.
- **`get_input_collector()` uses EntityQuery instead of singleton lookup**: There's a TODO in the code itself. Using the singleton registry would be faster and more correct.

## E2E Testing

- **E2E command handlers must be registered per SystemManager**: When using multiple SystemManagers (e.g. game_systems, pause_systems, transition_systems), command handlers like `HandlePressKeyCommand` and `HandleKeyDownCommand` need to be manually registered in each one. If you forget one, commands silently fail when that SystemManager is active. A global command registry or automatic propagation would prevent this class of bug.
- **`synthetic_press_delay` makes tests fragile**: The 1-frame delay in `consume_press` before injected key presses register means you need `wait` commands between `key_down` and any logic that depends on the press. The delay isn't documented and makes it hard to reason about when injected input actually takes effect.
