# WM coverage for new afterhours UI changes

Checked 2026-11-17 against wm_afterhours screens (`src/systems/screens/`) and
E2E scripts (`tests/e2e_scripts/`). "Screen" means a registered example screen
(screenshot baseline), "E2E" a script asserting behaviour.

| Change | PR | WM screen | WM E2E | Status |
|---|---|---|---|---|
| Controlled slider/toggle | #90 | slider/checkbox showcases | 08, 10, 11 | covered (external-value reset not asserted) |
| Menu rows from config | #93 | MenuShowcase | 220 | covered |
| Focus ring contrast | #96 | FocusRingLab (pair added) | baseline only | screen done, no E2E (visual) |
| set_focus on returned entity | #97 | TextInputLab (field exists) | none for wrapper focus | MISSING E2E |
| Drag hit in scrolled strip | #100 | drag showcases | 207, 346 | probably covered |
| Grid snap extents | #101 | layout screens | 101_layout_patterns | probably covered |
| Tree render_row | #102 | FileTreeShowcase (no render_row) | none | MISSING screen use |
| Measure with weight/spacing | #118 | text screens | design baselines | visual only |
| Disabled (+ blocks fix) | #124, #169 | Checkbox disabled specimens, MenuShowcase | 08, 220 | covered |
| Initial focus / modality | #128 | FocusRingLab readout | 103_tabbing_focus | probably covered |
| Wheel + Shift+wheel | #137 | scroll screens | 12, 241-244 | probably covered |
| Middle click | #142 | PointerParityLab | 354 | covered |
| Input remap | #150 | none | none | library-only, example in afterhours |
| Click count / double click | #151 | none | none | MISSING screen + E2E |
| Toast fade | #156 | ToastShowcase | baseline only | visual only (fade is timed) |
| Pagination indices | #157 | PaginationShowcase | 17_pagination | covered |
| ease_scroll under instant | #159 | none for scroll | none | MISSING screen use |
| Dropdown right-edge clamp | #160 | DropdownShowcase | 194 | screen exists, edge case not shown |
| Virtual list overscan | #161 | VirtualListLab (built count shown) | 271 | partial: overscan value not varied |
| Text-area readonly/disabled | #158 | TextAreaStatesLab (new) | 370 (new, passes) | covered |
| Inert subtree | #145 | none | none | MISSING screen + E2E |
| takes_focus(false) | #147 | none | none | MISSING screen + E2E |
| Percent sizing + margin | #162 | none explicit | none | MISSING screen use |
| Hover tint setters | #99 | HoverLab | baseline only | probably covered |
| Styled ellipsis / monospace / translation | #165-168 | text screens | design baselines | visual only |

Build order: inert + takes-focus (PointerParity/FocusRing labs), double-click
(PointerParity), text-area readonly (TextInputLab + extend 80), overscan vary
(VirtualListLab), percent-margin row (layout lab), tree render_row
(FileTreeShowcase), set_focus wrapper E2E (TextInputLab), ease-scroll instant
(scroll lab), dropdown edge (DropdownShowcase).
