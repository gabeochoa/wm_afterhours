# decide: the disable family (from #145/#147/#158 comments)

**Problem.** Five mechanisms all read as "disabled" in plain English, and a
reader cannot tell from the name what each one stops:

| Mechanism | Clicks | Focus | Tab | Edits | State source |
|---|---|---|---|---|---|
| `with_disabled(true)` (#124) | blocked and swallowed (#169) | no | no | n/a | `IsDisabled` / `HasLabel::is_disabled` |
| `with_inert()` (#145) | subtree passes nothing | subtree skipped | subtree skipped | n/a | `HasInert` on the subtree root |
| `with_takes_focus(false)` (#147) | fires | never by click | skipped (fixed) | n/a | `DoesNotTakeFocus` |
| text `readonly` (#158) | focusable, selectable | yes | yes | no | `HasTextInputState::readonly` |
| text `disabled` (#158) | no | no | no | no | `HasTextInputState::disabled` |
| `with_skip_tabbing(true)` | fires and focuses | yes by click | skipped | n/a | `SkipWhenTabbing` |

**Options**
1. **Keep all five, document the table** (this file, plus one WM lab row per
   mechanism). No renames, no behaviour change. The confusion is naming, and
   each name is already in consumer code.
2. **Rename for a system** (e.g. `disabled` -> `inert` everywhere). Breaking
   for every consumer, and the concepts genuinely differ (a toolbar button
   that must not steal focus is not a disabled button).
3. **Collapse inert into disabled-with-children.** Removes one name but makes
   `disabled` mean two scopes (self vs subtree), which is the current
   confusion in a new shape.

**Recommendation:** option 1. Land #145, #147 and #158 as they are, add the
table to the UI docs and one lab row each in PointerParity/FocusRing labs
during the WM coverage pass, and revisit only if a consumer mixes two of
them up in a bug report.
