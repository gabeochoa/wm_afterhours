# decide: debug-name lookup is ambiguous (hanabi #?, #337)

## Problem
Every name- or text-addressed e2e command takes the first entity the query reaches: `click_ui`, `right_click_ui`, `click_text`, `assert_ui`, `assert_ui_text`, `focus_ui`, `double_click_ui`, `expect_input_text` and others. Each of about 15 handlers in `ui_commands.h` runs its own query. When two panes build the same widget from the same `mk` call, or every row in a list shares one debug name, which entity wins depends on entity-id order, and that changes as ids get reused. Scripts pass while testing the wrong widget. hanabi gave its pane scroll views per-pane names just to defeat this, and its sidebar tests can only assert on strings that one panel happens to produce.

## Options
A. **Fail on ambiguity everywhere.** Route all name and text lookups through one resolver that fails with `3 elements match 'row'; scope it with under=NAME`. It's correct and loud, but it breaks every downstream script that relies on "first row wins". Lists that share one debug name are common, so I'd expect breakage across several of the ~20 projects.

B. **Scope argument plus a warning.** One resolver accepts `under=ANCESTOR`, which walks the `UIComponent::parent` chain to a debug-named ancestor, as hanabi proposed. An ambiguous match without a scope logs `log_warn` once per name and still takes the first, as today. Nothing breaks, and the ambiguity becomes visible and fixable.

C. **B now, then A later.** Ship B, give projects a release to add `under=` or distinct names, then turn the warning into a failure.

## Recommendation
C. B is about 120 lines: one `resolve_ui(name_or_text, scope)` used by every handler, an `under=` key parsed from the args the way `assert_ui` already parses `key=value`, plus one test per command family. Its first job is to show how many downstream scripts are ambiguous today. That number decides when to flip to A. Going straight to A risks a silent mass failure on the next submodule bump.

## Rough size
B: about 120 lines in ui_commands.h, 1 test file, 1 example. A on top: about 10 lines plus fixing the downstream scripts it trips.
