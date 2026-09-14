# UI review prompt

Use [WM's design guidelines](../docs/DESIGN_GUIDELINES.md) and
[verification workflow](../docs/AGENTS_UI.md).

Compare the native screen with its mock at the same viewport/state. Check composition,
type, spacing, artwork, clipping, borders, focus and interaction. Exercise pointer and
keyboard paths, open overlays, empty/error/disabled states and resize. Use contact
sheets for sequences and full-size crops for details.

For each defect, record the screen/state, visible symptom, likely owner and a concrete
fix/check. Keep findings short. Distinguish observation from hypothesis, passing tests
from visual acceptance, and historical reports from current reproduction. Check existing
APIs before proposing an afterhours gap. Preserve intentional imitation styles.

Reference traditions: Apple HIG for consistency/direct manipulation; Atlassian for
semantic tokens/elevation; Material for states/adaptivity; Win95/Sun JLF for predictable
menus and desktop focus; USWDS for clarity and accessible workflows. Apply the relevant
pattern without claiming certification or mixing unrelated styles on one screen.
