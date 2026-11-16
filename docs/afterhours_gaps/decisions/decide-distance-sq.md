# decide: `distance_sq` on the raylib path (Tier 3: wm_afterhours / tetr)

**Problem.** `developer.h` defines a global `distance_sq(Vector2Type, Vector2Type)`, but only inside the `#ifndef Vector2Type` fallback (`MyVec2`). raylib and sokol consumers get nothing, so tetr keeps its own copy. The catch is that other raylib projects already define the same function at global scope with the same signature:
- kart-afterhours `src/math_util.h:13`: `constexpr float distance_sq(const vec2, const vec2)` (vec2 = raylib Vector2)
- endless-dance-chaos `src/vec_util.h:99`, the same
- afterhours-template `src/math_util.h:13`, the same
- cartographer has a `static distance_sq(vec3, vec3)`, a different type, so no clash

Adding a global one on the raylib path is a redefinition error in those three. Adding `afterhours::distance_sq` makes every unqualified call ambiguous in any of them that does `using namespace afterhours;`. So the 3-line helper is a breaking change for three projects.

## Options

1. **One `afterhours::distance_sq(Vector2Type, Vector2Type)` for every backend**, replacing the fallback-only global, with a global re-export like the type aliases have. kart, endless-dance-chaos and afterhours-template get a compile error on their next bump and delete their copy (the fix is deleting 3 lines). This consolidates four copies into one. It goes in the CHANGELOG under Breaking.
2. **A name that can't collide**, for example `afterhours::vec::distance_sq`, available on every backend, with the fallback global left as is. Nothing breaks; tetr calls it qualified. The existing copies stay.
3. **Leave it.** It's a 3-line function.

## Recommendation

Option 1 if Gabe is fine with three projects deleting their copies on bump (the error is immediate and the fix is obvious). Otherwise option 2. I didn't write it because the choice is whether to break those builds.

**Rough size:** XS either way: about 10 lines in `developer.h`, one test and one example.
