# Todo

## Baseline design audit

2,535 tracked items across 117 screens: 58 visible defects and 2,477 design proposals.
1,207 completed; 1,328 remaining.
This includes four additional findings from the older tooltip capture.

P1: obscured or unreadable content. P2: reading or comparison. P3: polish or added explanation.
Proposals are review candidates. Preserve intentional test fixtures; screenshot evidence alone does not establish a library defect.

[Visual review](design-audit.html) · [Audit evidence and coverage](docs/design-audit/GAPS.md)

Check items here as they are completed. The audit JSON and generated report retain the original review evidence.
The existing project backlog is preserved after the screen checklist.

### absolute_positioning

[Baseline](screenshot-baselines/screens/absolute_positioning_720p.png) · [Visual review](design-audit.html#screen=absolute_positioning)

- [x] `absolute_positioning-01` · P1 · defect. Translate example: Size the samples so both complete coordinate labels remain visible.
  Evidence: Pink translate(300,10) overlaps the green label's final characters.

- [x] `absolute_positioning-02` · P2 · defect. Corner-pinning right edge: Compute right anchors from the actual measured well width.
  Evidence: TR and BR extend beyond the inset demo well.

- [x] `absolute_positioning-03` · P2 · proposal. Corner-pinning labels: Add an external legend spelling out the four corners.
  Evidence: TL, TR, BL and BR require interpreting abbreviations.

- [x] `absolute_positioning-04` · P2 · proposal. Corner-pinning bounds: Outline the reference rectangle independently of its children.
  Evidence: The well's right boundary disappears behind the corner blocks.

- [x] `absolute_positioning-05` · P2 · proposal. Corner-pinning geometry: Print measured width and height beneath the reference rectangle.
  Evidence: Corner samples look wider than tall.

- [x] `absolute_positioning-06` · P2 · proposal. Corner-pinning spacing: Increase the well height to make top versus bottom anchoring clearer.
  Evidence: The two left samples nearly fill the short well vertically.

- [x] `absolute_positioning-07` · P2 · proposal. Corner-pinning origin: Draw a small crosshair at the container origin.
  Evidence: The top-left block has no marked anchor point.

- [x] `absolute_positioning-08` · P2 · proposal. Translate reference: Add reference ticks at x=0,100,300 outside the specimens.
  Evidence: The green and pink boxes have no origin or grid.

- [x] `absolute_positioning-09` · P2 · proposal. Translate y offsets: Add a 10-pixel offset bracket at the left edge.
  Evidence: Both translate labels show 10 without a visible vertical measurement.

- [x] `absolute_positioning-10` · P2 · proposal. Translate labels: Use a compact monospace face for numeric coordinates.
  Evidence: Coordinate text uses the handwritten display face.

- [x] `absolute_positioning-11` · P2 · proposal. Title: Inset the title text to align with the section headings.
  Evidence: The cyan title starts flush against its rounded panel edge.

- [x] `absolute_positioning-12` · P2 · proposal. Section headings: Increase heading weight while keeping specimen text smaller.
  Evidence: Four long technical headings are visually similar to specimen labels.

- [x] `absolute_positioning-13` · P2 · proposal. Section numbering: Use a narrow numbered gutter for faster section scanning.
  Evidence: Numbers share the same baseline and weight as full headings.

- [x] `absolute_positioning-14` · P2 · proposal. Correct-pattern caption: Add a short note that offsets are relative to the demo container.
  Evidence: The first heading calls the pattern correct without stating the rule.

- [x] `absolute_positioning-15` · P2 · proposal. Flow labels: Add consistent internal label padding.
  Evidence: Flow A, B and C sit directly on the left edges of their blocks.

- [x] `absolute_positioning-16` · P2 · proposal. Flow spacing: Annotate the space-around gaps below the row.
  Evidence: Large gaps between Flow A, B and C have no measurement cues.

- [x] `absolute_positioning-17` · P2 · proposal. Overlay badge: Center the badge label with equal horizontal inset.
  Evidence: BADGE is left aligned within a wide pink pill.

- [x] `absolute_positioning-18` · P2 · proposal. Overlay relationship: Add a leader from the badge to its container anchor.
  Evidence: The badge crosses Flow C and the well without explaining its anchor.

- [x] `absolute_positioning-19` · P2 · proposal. Overlay layer: Label the badge layer outside the colored blocks.
  Evidence: The overlay example does not identify its stacking order.

- [x] `absolute_positioning-20` · P2 · proposal. Percentage samples: Display viewport width beside the section heading.
  Evidence: 30% and 20% blocks describe screen sizing but omit reference width.

- [x] `absolute_positioning-21` · P2 · proposal. Percentage heights: Add computed height text below the specimens.
  Evidence: Both blocks show 8% without an absolute height comparison.

- [x] `absolute_positioning-22` · P2 · proposal. Percentage labels: Inset percentage labels by a consistent small padding.
  Evidence: Text begins at each block's left boundary.

- [x] `absolute_positioning-23` · P2 · proposal. Percentage reference: Add a thin 0–100% ruler outside the samples.
  Evidence: There is no visual scale for the unused half of screen width.

- [x] `absolute_positioning-24` · P2 · proposal. Bottom space: Use it for a compact coordinate-system legend.
  Evidence: A large empty band follows the fourth fixture.

- [x] `absolute_positioning-25` · P2 · proposal. Demo wells: Add subtle well borders so measured container boundaries remain visible.
  Evidence: Inset wells and outer section panels are close in tone.

### accessibility

[Baseline](screenshot-baselines/screens/accessibility_720p.png) · [Visual review](design-audit.html#screen=accessibility)

- [x] `accessibility-01` · P2 · proposal. WCAG badge: Add the number of validated theme pairs under the badge.
  Evidence: Theme Passes WCAG AA does not name the checked color pairs.

- [x] `accessibility-02` · P2 · proposal. WCAG badge scope: Rename it to Theme color pairs pass AA contrast.
  Evidence: The badge reads like approval of the entire interface.

- [x] `accessibility-03` · P2 · proposal. WCAG threshold: Show the threshold used for normal text.
  Evidence: No required contrast ratio appears beside the badge.

- [x] `accessibility-04` · P2 · proposal. Manual column heading: Add a subtitle saying text colors were chosen manually.
  Evidence: Without Automatic Contrast describes implementation only.

- [x] `accessibility-05` · P2 · proposal. Automatic column heading: Add a short dark-or-light text explanation.
  Evidence: With Automatic Contrast offers no selection rule.

- [x] `accessibility-06` · P2 · proposal. Dark manual sample: Use Manual light text for parallel labeling.
  Evidence: Works Fine is vague compared with the neighboring labels.

- [x] `accessibility-07` · P2 · proposal. Dark sample boundary: Add a neutral outline outside the test color area.
  Evidence: The dark manual tile blends into the surrounding panel.

- [x] `accessibility-08` · P2 · proposal. Dark automatic boundary: Outline its bounds with the same reference stroke.
  Evidence: The dark automatic tile also lacks a clear extent.

- [x] `accessibility-09` · P2 · proposal. Light pair: Show chosen foreground values below each tile.
  Evidence: The two light samples look identical without a result annotation.

- [x] `accessibility-10` · P2 · proposal. Manual note: Place it in the manual column footer with consistent inset.
  Evidence: Manual text color applied is detached from its two samples.

- [x] `accessibility-11` · P2 · proposal. Automatic note: Give both explanatory footers equal typographic emphasis.
  Evidence: Automatically picks best contrast uses a bright accent unlike the manual footer.

- [x] `accessibility-12` · P2 · proposal. Swatch heading: Label the rows Light backgrounds and Dark backgrounds.
  Evidence: Various Backgrounds offers no description of the two rows.

- [x] `accessibility-13` · P2 · proposal. Swatch identity: Give each tile a compact background color identifier.
  Evidence: All sixteen tiles have the same Auto label.

- [x] `accessibility-14` · P2 · proposal. Swatch foreground: Add the chosen foreground value to each swatch caption.
  Evidence: Black or white text choices are visible but unnamed.

- [x] `accessibility-15` · P2 · proposal. Swatch ratios: Show a ratio beneath each test tile.
  Evidence: No tile exposes its measured contrast.

- [x] `accessibility-16` · P2 · proposal. Swatch ordering: Group or order samples by luminance and label the order.
  Evidence: The bright row changes hue without a visible sorting principle.

- [x] `accessibility-17` · P2 · proposal. Dark row pairing: Add column labels if pairs are intended.
  Evidence: Dark swatches do not visually establish pairs with the row above.

- [x] `accessibility-18` · P2 · proposal. Swatch gutters: Use a clearer equal grid gutter around the swatch set.
  Evidence: Horizontal gaps are much smaller than the gap separating the comparison columns.

- [x] `accessibility-19` · P2 · proposal. Swatch type: Use a conventional text face inside the test swatches.
  Evidence: Small widely tracked lettering makes color comparisons harder to read.

- [x] `accessibility-20` · P2 · proposal. Title style: Use one readable family with weight differences for hierarchy.
  Evidence: The title uses a different face from the rest of the panel.
  Resolution: Atkinson throughout, with size establishing hierarchy; the bundled Atkinson face has no bold weight.

- [x] `accessibility-21` · P2 · proposal. Footer claim: Limit the wording to the tested foreground choices.
  Evidence: Text always remains readable is an absolute statement.

- [x] `accessibility-22` · P2 · proposal. Footer placement: Move the explanation closer to the grid.
  Evidence: The footer sits well below the swatches with a large blank gap.

- [x] `accessibility-23` · P2 · proposal. Separators: Align separator ends to the content grid.
  Evidence: Both divider lines are inset farther than the column content.

- [x] `accessibility-24` · P2 · proposal. Theme identity: Add the theme name alongside validation status.
  Evidence: The panel does not name the theme being validated.

- [x] `accessibility-25` · P2 · proposal. Test context: Display the text size used in the contrast specimens.
  Evidence: The demonstration gives no sample font size near its AA claim.

### adaptive_scaling

[Baseline](screenshot-baselines/screens/adaptive_scaling_720p.png) · [Visual review](design-audit.html#screen=adaptive_scaling)

- [x] `adaptive_scaling-01` · P2 · proposal. Logical dimensions: Increase diagnostic text to a readable body size.
  Evidence: Logical: 1280x720 is tiny beside the scale controls.

- [x] `adaptive_scaling-02` · P2 · proposal. Header status: Use the header slot for physical viewport dimensions.
  Evidence: Adaptive 100% repeats the mode and scale controls.

- [x] `adaptive_scaling-03` · P2 · proposal. Mode button: Add a change-mode indicator and name the alternate mode in helper text.
  Evidence: Mode: Adaptive does not signal that another mode exists.

- [x] `adaptive_scaling-04` · P2 · proposal. Scale decrement: Label the scale control group with its increment.
  Evidence: The minus button contains no scale-step hint.

- [x] `adaptive_scaling-05` · P2 · proposal. Scale readout: Give the numeric readout a distinct non-action field treatment.
  Evidence: 100% is styled much like its neighboring buttons.

- [x] `adaptive_scaling-06` · P2 · proposal. Pixel card description: Increase its text size and align it to the card heading.
  Evidence: This card is 280x160 px is extremely small.

- [x] `adaptive_scaling-07` · P2 · proposal. Pixel color squares: Add width-by-height captions outside each square.
  Evidence: Four colored blocks have no dimensions shown.

- [x] `adaptive_scaling-08` · P2 · proposal. Pixel square comparison: Label their intended comparison explicitly.
  Evidence: Color is the only distinction among identical-size samples.

- [x] `adaptive_scaling-09` · P2 · proposal. Pixel buttons: Use labels that include their fixed pixel dimensions.
  Evidence: Button A and Button B do not identify their sizing rule.
  Resolution: Labels state fill-width x 36 logical pixels; the original widths were percentages, not fixed pixels.

- [x] `adaptive_scaling-10` · P2 · proposal. Button widths: Show a measured width annotation below the pair.
  Evidence: Both pixel buttons span nearly the full card.

- [x] `adaptive_scaling-11` · P2 · proposal. Padding card: Add four small padding brackets around Inner content area.
  Evidence: Padding: 14px all sides lacks visible dimension markers.

- [x] `adaptive_scaling-12` · P2 · proposal. Padding inner text: Inset its label without changing the measured outer padding.
  Evidence: Inner content area sits against the dark rectangle's left edge.

- [x] `adaptive_scaling-13` · P2 · proposal. Expand heading: Add a brief ratio caption above the colored segments.
  Evidence: Expand & Flex does not define the 1x, 2x, 3x ratio.

- [x] `adaptive_scaling-14` · P2 · proposal. Expand segment boundaries: Add thin neutral dividers that preserve the measured flex widths.
  Evidence: Red, amber and green segments touch directly.

- [x] `adaptive_scaling-15` · P2 · proposal. Expand numeric labels: Increase their size for distance comparison.
  Evidence: 1x, 2x and 3x are small in tall blocks.

- [x] `adaptive_scaling-16` · P2 · proposal. Expand explanation: Use larger text and a shorter two-line explanation.
  Evidence: The long explanation is tiny and widely tracked.

- [x] `adaptive_scaling-17` · P2 · proposal. Pixel comparison heading: Name all three sizing systems in the heading.
  Evidence: screen_pct vs pixels omits the h720 sample shown below.

- [x] `adaptive_scaling-18` · P2 · proposal. px specimen: Label the specimen with its numeric pixel width.
  Evidence: px omits its configured magnitude.

- [x] `adaptive_scaling-19` · P2 · proposal. h720 specimen: Show its configured value and reference height in a caption.
  Evidence: h720 is unexplained technical shorthand.

- [x] `adaptive_scaling-20` · P2 · proposal. scr% specimen: Show the exact screen percentage outside the block.
  Evidence: scr% does not state its percentage.

- [x] `adaptive_scaling-21` · P2 · proposal. Comparison alignment: Add a horizontal pixel scale beneath them.
  Evidence: Three unequal specimens provide no common ruler.

- [x] `adaptive_scaling-22` · P2 · proposal. Breakpoint chips: Separate width breakpoints from height breakpoints.
  Evidence: narrow, medium, wide and short mix width and height categories.

- [x] `adaptive_scaling-23` · P2 · proposal. Active wide chip: Add a checkmark or Active suffix.
  Evidence: The wide state is communicated mainly by a blue fill.

- [x] `adaptive_scaling-24` · P2 · proposal. Breakpoint thresholds: Show the numeric threshold for each breakpoint.
  Evidence: No width or height thresholds appear beside the chips.

- [x] `adaptive_scaling-25` · P2 · proposal. Empty lower area: Add a compact before-and-after size table for the visible examples.
  Evidence: Most of the bottom fifth is unused.

### advanced_modals

[Baseline](screenshot-baselines/screens/advanced_modals_720p.png) · [Visual review](design-audit.html#screen=advanced_modals)

- [x] `advanced_modals-01` · P2 · proposal. Title bar: Add consistent text inset.
  Evidence: Advanced Modals starts directly at the blue strip's left edge.

- [x] `advanced_modals-02` · P2 · proposal. Title hierarchy: Add one sentence describing the four examples beneath it.
  Evidence: The heading occupies a full-width strip despite little supporting content.

- [x] `advanced_modals-03` · P2 · proposal. Outer canvas: Center the page within equal outer margins.
  Evidence: A black border surrounds the navy page with uneven apparent margins.

- [x] `advanced_modals-04` · P2 · proposal. Header gap: Reduce it to the normal section spacing.
  Evidence: A large empty gap separates the title from the first launcher row.

- [x] `advanced_modals-05` · P2 · proposal. Row gap: Use a compact two-column card grid.
  Evidence: The two button rows are separated by about a button-height of empty space.

- [x] `advanced_modals-06` · P2 · proposal. Login launcher: Add a caption identifying required-field validation.
  Evidence: Login Form offers no hint of the demonstrated validation.

- [x] `advanced_modals-07` · P2 · proposal. Wizard launcher: Show a Three-step setup caption.
  Evidence: Multi-Step Wizard gives no visible step count.

- [x] `advanced_modals-08` · P2 · proposal. Settings launcher: Add a short caption naming the visible settings categories.
  Evidence: Settings Panel does not identify its control types.

- [x] `advanced_modals-09` · P2 · proposal. Feedback launcher: Add a brief rating-and-comment caption.
  Evidence: Feedback Form gives no indication of its example purpose.

- [x] `advanced_modals-10` · P2 · proposal. Launcher colors: Use a shared action style with small category accents.
  Evidence: Blue, ochre, green and dusty red assign unrelated meanings.

- [x] `advanced_modals-11` · P2 · proposal. Button shape: Choose a deliberate shared corner system for launchers and panels.
  Evidence: Square buttons sit inside strongly rounded row panels.

- [x] `advanced_modals-12` · P2 · proposal. Button balance: Bring each button and its description into a single card.
  Evidence: Very wide gaps separate paired actions.

- [x] `advanced_modals-13` · P2 · proposal. Launch verbs: Prefix them with Open to clarify the immediate action.
  Evidence: All four labels are nouns.

- [x] `advanced_modals-14` · P2 · proposal. Status instruction: Increase the instruction size and align it to the grid.
  Evidence: Open a modal above to get started is small inside a wide strip.

- [x] `advanced_modals-15` · P2 · proposal. Status strip inset: Add horizontal padding equal to the title inset.
  Evidence: The instruction starts flush against its background edge.

- [x] `advanced_modals-16` · P2 · proposal. Default screenshot state: Provide a separate clearly labeled example-open baseline alongside this launcher.
  Evidence: No modal specimen appears in the baseline.
  Resolution: Four current open-form examples are available in the visual review, including [login validation](docs/design-audit/examples/advanced_modals_login_validation_720p.png).

### aim_chat

[Baseline](screenshot-baselines/screens/aim_chat_720p.png) · [Visual review](design-audit.html#screen=aim_chat)

- [x] `aim_chat-01` · P2 · proposal. Conversation history: Move the developer-log specimen into a separately labeled demo pane.
  Evidence: The mock's HTTP GET/POST example interrupts the conversation between messages and its timestamp.

- [x] `aim_chat-02` · P2 · proposal. Desktop computer icon: Replace it with a recognizable period-style computer bitmap.
  Evidence: My Computer uses an empty square outline.

- [x] `aim_chat-03` · P2 · proposal. Desktop folder icon: Use a folder icon consistent with the runner artwork.
  Evidence: My Documents uses an outlined parallelogram.

- [x] `aim_chat-04` · P2 · proposal. Window controls: Draw a small square window glyph.
  Evidence: Maximize is rendered as literal [] text.

- [x] `aim_chat-05` · P2 · proposal. Title minimize: Use a centered pixel-aligned minimize line.
  Evidence: The minimize character appears as text at the bottom of a tiny control.

- [x] `aim_chat-06` · P2 · proposal. Menu rows: Use flat menu labels until selected, matching the mock's menu bar.
  Evidence: Each menu item looks like a raised command button.

- [x] `aim_chat-07` · P2 · proposal. Active window: Give the inactive buddy window a subdued title bar.
  Evidence: Both title bars use equally vivid blue gradients.

- [x] `aim_chat-08` · P2 · proposal. Buddy groups: Use consistent small triangle glyphs aligned in one column.
  Evidence: v and > are used as disclosure markers.

- [x] `aim_chat-09` · P2 · proposal. Buddy count: Use a compact online/total caption in the group header.
  Evidence: Buddies (3/8) does not explain the denominator.

- [x] `aim_chat-10` · P2 · proposal. Offline group: Use one consistent count convention across groups.
  Evidence: Offline (12) lacks the count format used above it.

- [x] `aim_chat-11` · P2 · proposal. Buddy selection: Retain indentation but add a small status icon column.
  Evidence: SmarterChild's selected row starts farther right than group labels.

- [x] `aim_chat-12` · P2 · proposal. Buddy presence: Add compact online indicators beside each name.
  Evidence: Unselected buddy names carry no individual presence marker.

- [x] `aim_chat-13` · P2 · proposal. Buddy action labels: Rename the group action to Chat room or clarify the distinction.
  Evidence: IM and Chat appear side by side without visible distinction.

- [x] `aim_chat-14` · P2 · proposal. Availability bar: Add a small disclosure arrow to mark it as a status control.
  Evidence: I'm Available resembles a text field.

- [x] `aim_chat-15` · P2 · proposal. Buddy footer banner: Reduce its height to return emphasis to contacts.
  Evidence: The large AOL banner has more contrast than the buddy list header.

- [x] `aim_chat-16` · P2 · proposal. Chat identity: Keep the profile name but reduce its red emphasis.
  Evidence: SmarterChild is repeated in the title, profile header and first line.

- [x] `aim_chat-17` · P2 · proposal. Warning level: Align it with the presence row and reduce its visual priority.
  Evidence: Warning level: 0% occupies the far edge of the profile header.

- [x] `aim_chat-18` · P2 · proposal. Message spacing: Tighten line spacing to match a compact instant-message transcript.
  Evidence: The three short chat lines have substantial vertical spacing.

- [x] `aim_chat-19` · P2 · proposal. Timestamp: Darken the text enough to read at the existing small size.
  Evidence: Last message received at 2: 02 PM is faint gray on white.

- [x] `aim_chat-20` · P2 · proposal. Formatting toolbar: Use recognizable font, color, emoticon and link icons.
  Evidence: A, B, I, U, A, :) and / have unevenly understandable meanings.

- [x] `aim_chat-21` · P2 · proposal. Toolbar grouping: Insert narrow separators between formatting and insertion groups.
  Evidence: Text formatting and insertion actions form one undifferentiated row.

- [x] `aim_chat-22` · P2 · proposal. Composer label: Increase the label slightly and align it with composer inset.
  Evidence: Send to: SmarterChild is much smaller than message text.

- [x] `aim_chat-23` · P2 · proposal. Composer placeholder: Increase placeholder contrast while keeping it secondary.
  Evidence: Type a message is light gray within a large empty white area.

- [x] `aim_chat-24` · P2 · proposal. Send action: Give Send the default-button outline and separate moderation actions.
  Evidence: Send has the same weight as Warn and Block.

- [x] `aim_chat-25` · P2 · proposal. Connection footer: Use a readable status line with a small connection indicator.
  Evidence: Direct Connection Established (local demo) is tiny at the bottom edge.

### alpha_blend_repro

[Baseline](screenshot-baselines/screens/alpha_blend_repro_720p.png) · [Visual review](design-audit.html#screen=alpha_blend_repro)

- [x] `alpha_blend_repro-01` · P2 · proposal. Title claim: Qualify the expectation by opacity range.
  Evidence: The heading says text should stay readable through the wash, including opaque rows.

- [x] `alpha_blend_repro-02` · P2 · proposal. First section heading: Use a readable monospace caption above the fixture.
  Evidence: The full with_custom_background expression is tiny.

- [x] `alpha_blend_repro-03` · P2 · proposal. Second section heading: Use the same monospace heading treatment as the first section.
  Evidence: with_opacity appears in small handwritten lettering.

- [x] `alpha_blend_repro-04` · P2 · proposal. Alpha units: Label the right column Alpha / 255.
  Evidence: 32 through 255 have no visible alpha-unit label.

- [x] `alpha_blend_repro-05` · P2 · proposal. Opacity units: Label that column Overlay opacity.
  Evidence: 15% through 100% lack a column heading.

- [x] `alpha_blend_repro-06` · P2 · proposal. Opaque alpha row: Add an external Opaque reference annotation.
  Evidence: The final blue row has no visible specimen text, as expected.

- [x] `alpha_blend_repro-07` · P2 · proposal. Opaque opacity row: Label it Expected complete occlusion outside the overlay.
  Evidence: The 100% row is also intentionally blank.

- [x] `alpha_blend_repro-08` · P2 · proposal. Unwashed reference: Add one zero-opacity reference row.
  Evidence: Every text sample is covered by some overlay.

- [x] `alpha_blend_repro-09` · P2 · proposal. Underlying text: Repeat the reference text outside the fixtures.
  Evidence: The pangram is printed only inside the tested wash.

- [x] `alpha_blend_repro-10` · P2 · proposal. Row labels: Increase their size and use aligned tabular numerals.
  Evidence: Numeric values are very small and far from the specimen start.

- [x] `alpha_blend_repro-11` · P2 · proposal. Row padding: Add consistent text inset without changing the overlay extent.
  Evidence: Pangram text starts flush with the wash's left boundary.

- [x] `alpha_blend_repro-12` · P2 · proposal. Row gaps: Use equal narrow neutral gutters between all rows.
  Evidence: Several blue rows visually merge together.

- [x] `alpha_blend_repro-13` · P2 · proposal. Cross-method comparison: Add one matched-value pair for direct comparison.
  Evidence: The alpha and opacity sections use different sample values.

- [x] `alpha_blend_repro-14` · P2 · proposal. Overlay color: Add a small labeled RGB swatch outside the test region.
  Evidence: The blue test color is named only in code syntax.

- [x] `alpha_blend_repro-15` · P2 · proposal. Backing surface: Label the surface over which alpha is being composited.
  Evidence: The black background's role is not named.

- [x] `alpha_blend_repro-16` · P2 · proposal. Expected result: Add a one-line equivalence note.
  Evidence: The two methods have no nearby explanation of their expected relationship.

- [x] `alpha_blend_repro-17` · P2 · proposal. Text foreground: Show its foreground value beside the untouched reference.
  Evidence: The source text color is not stated.

- [x] `alpha_blend_repro-18` · P2 · proposal. Section grouping: Place each stack in an outlined test region with a shared label gutter.
  Evidence: Two long stacks run almost edge to edge.

- [x] `alpha_blend_repro-19` · P2 · proposal. Lower empty area: Use a compact expected-versus-observed comparison legend.
  Evidence: The lower quarter contains no interpretive aid.

- [x] `alpha_blend_repro-20` · P2 · proposal. Capture context: Add a small diagnostic footer with renderer and viewport.
  Evidence: The reproduction omits renderer information in the visible frame.

### angry_birds_settings

[Baseline](screenshot-baselines/screens/angry_birds_settings_720p.png) · [Visual review](design-audit.html#screen=angry_birds_settings)

- [x] `angry_birds_settings-01` · P2 · proposal. Audio selected tab: Add a small selected underline or check marker.
  Evidence: AUDIO relies on green fill to distinguish it from blue tabs.

- [x] `angry_birds_settings-02` · P2 · proposal. Vibration state: Give off-but-interactive controls an explicit toggle marker.
  Evidence: The OFF icon looks disabled while MUSIC and SOUND look enabled.

- [x] `angry_birds_settings-03` · P2 · proposal. State captions: Increase state caption size and weight.
  Evidence: ON and OFF are much smaller than MUSIC, SOUND and VIBRATION.

- [x] `angry_birds_settings-04` · P2 · proposal. Sound wording: Rename it SOUND EFFECTS.
  Evidence: SOUND can include the separate music setting.

- [x] `angry_birds_settings-05` · P2 · proposal. Vibration icon: Use a more balanced vibration glyph.
  Evidence: The phone is tilted and flanked by marks at different angles.

- [x] `angry_birds_settings-06` · P2 · proposal. Music icon: Normalize icon optical size within all three buttons.
  Evidence: The note occupies less width than the speaker icon.

- [x] `angry_birds_settings-07` · P2 · proposal. Close control: Bring it slightly inward to preserve a consistent safe margin.
  Evidence: The red close button overlaps the outer board edge.

- [x] `angry_birds_settings-08` · P2 · proposal. Close emphasis: Soften the red or reduce its size relative to confirmation.
  Evidence: Close is the strongest red object on a non-destructive screen.

- [x] `angry_birds_settings-09` · P2 · proposal. Footer action order: Place the primary confirmation at the far right.
  Evidence: OK appears before CANCEL and APPLY in a right-aligned group.

- [x] `angry_birds_settings-10` · P2 · proposal. OK wording: Use DONE or SAVE & CLOSE.
  Evidence: OK does not explain whether settings save and close.

- [x] `angry_birds_settings-11` · P2 · proposal. Apply action: Visually distinguish its no-pending-changes state when applicable.
  Evidence: APPLY looks active in the unchanged default state.

- [x] `angry_birds_settings-12` · P2 · proposal. Cancel action: Use a quieter secondary treatment.
  Evidence: CANCEL has the same glossy emphasis as APPLY.

- [x] `angry_birds_settings-13` · P2 · proposal. Footer spacing: Add a concise saved/unsaved status on the left.
  Evidence: The footer actions cluster on the right while the left half is empty.

- [x] `angry_birds_settings-14` · P2 · proposal. Audio instruction: Add a brief Tap an icon to toggle caption.
  Evidence: The tagline says little about the controls.

- [x] `angry_birds_settings-15` · P2 · proposal. Tab content association: Reduce the gap between tab navigation and audio description.
  Evidence: The audio controls sit far below the active tab.

- [x] `angry_birds_settings-16` · P2 · proposal. Info tab wording: Use ABOUT or a more descriptive label.
  Evidence: INFO is broad for credits, support and version details.

- [x] `angry_birds_settings-17` · P2 · proposal. Board size: Reduce outer ornament height to create breathing room.
  Evidence: The board nearly fills the available height.

- [x] `angry_birds_settings-18` · P2 · proposal. Outer shadow: Soften it to keep the footer visually lighter.
  Evidence: A heavy brown shadow adds a thick band under the board.

- [x] `angry_birds_settings-19` · P2 · proposal. Tab shadows: Reduce shadow depth while retaining the toy-like style.
  Evidence: Dark tab shadows compete with text outlines.

- [x] `angry_birds_settings-20` · P2 · proposal. Button border color: Use a consistent warmer outline palette.
  Evidence: Gray-green outlines sit beside warm brown panel outlines.

- [x] `angry_birds_settings-21` · P2 · proposal. Audio label color: Darken labels slightly for small-screen readability.
  Evidence: Brown labels are muted against the peach surface.

- [x] `angry_birds_settings-22` · P2 · proposal. Separator: Increase its contrast modestly.
  Evidence: The thin footer rule is easy to lose against the peach background.

- [x] `angry_birds_settings-23` · P2 · proposal. Background blocks: Blur or dim them further behind the modal.
  Evidence: Stacked dark blocks on the right remain visually prominent.

- [x] `angry_birds_settings-24` · P2 · proposal. Bottom white line: Remove it or make its purpose clear as a device indicator.
  Evidence: An isolated pale line spans much of the screen below the board.

- [x] `angry_birds_settings-25` · P2 · proposal. Behind-status context: Restore subdued game context above the panel if matching the reference remains the target.
  Evidence: The mock includes heart and currency status behind the board, absent here.

### animation_basic

[Baseline](screenshot-baselines/screens/animation_basic_720p.png) · [Visual review](design-audit.html#screen=animation_basic)

- [x] `animation_basic-01` · P2 · proposal. Header location: Move the header upward and reclaim space for diagnostics.
  Evidence: A large empty region sits above Animation Basics.

- [x] `animation_basic-02` · P2 · proposal. Title/subtitle bands: Combine them into one padded header block.
  Evidence: Title and subtitle occupy separate touching rounded strips.

- [x] `animation_basic-03` · P2 · proposal. Main specimen group: Use three equal cards with shared alignment.
  Evidence: The three examples occupy a small area amid large gaps.

- [x] `animation_basic-04` · P2 · proposal. Slide label order: Match subtitle order to the actual columns.
  Evidence: Subtitle says Fade, Slide, Scale while columns show Slide, Fade, Scale.

- [x] `animation_basic-05` · P2 · proposal. Slide measurement: Add start and end values in a caption.
  Evidence: x offset: 0px shows only the endpoint.

- [x] `animation_basic-06` · P2 · proposal. Slide reference: Draw a ghost origin outside its final bounds.
  Evidence: The green square has no start-position marker.

- [x] `animation_basic-07` · P2 · proposal. Fade reference: Add a checkerboard or reference swatch behind its test area.
  Evidence: The blue square is fully opaque in the captured state.

- [x] `animation_basic-08` · P2 · proposal. Scale reference: Draw a thin fixed bounds outline behind it.
  Evidence: The purple square has no visible original-size boundary.

- [x] `animation_basic-09` · P2 · proposal. Animation durations: Add a duration label below each animation name.
  Evidence: No example displays its duration.

- [x] `animation_basic-10` · P2 · proposal. Animation easing: Display the easing name per specimen.
  Evidence: Bounce and other timing differences are not described.

- [x] `animation_basic-11` · P2 · proposal. Metric chips: Use a shared width and aligned metric baseline.
  Evidence: The slide chip is wider than the other metric chips.

- [x] `animation_basic-12` · P2 · proposal. Metric typography: Use readable tabular numeric text.
  Evidence: The metrics use tiny handwritten numerals.

- [x] `animation_basic-13` · P2 · proposal. Animation phase: Show Ready, Running or Complete beside restart.
  Evidence: No status says the examples have completed.

- [x] `animation_basic-14` · P2 · proposal. Restart label: Increase its contrast relative to the specimen captions.
  Evidence: Restart Animations is the only action but has low dark-teal emphasis.

- [x] `animation_basic-15` · P2 · proposal. Restart position: Move it nearer the specimens while preserving separation.
  Evidence: The restart button is distant from the diagnostic row.

- [x] `animation_basic-16` · P2 · proposal. Independent playback: Add small individual replay controls for focused comparison.
  Evidence: One restart action serves all three examples.

- [x] `animation_basic-17` · P2 · proposal. Timeline: Add a shared progress track with an elapsed-time marker.
  Evidence: There is no way to compare progress in a static frame.

- [x] `animation_basic-18` · P2 · proposal. Motion comparison: Provide a clearly labeled strip of start, midpoint and endpoint thumbnails.
  Evidence: All endpoints are identical-size blocks.

### animation_declarative

[Baseline](screenshot-baselines/screens/animation_declarative_720p.png) · [Visual review](design-audit.html#screen=animation_declarative)

- [x] `animation_declarative-01` · P2 · defect. Appear label: Center the label within the specimen.
  Evidence: Appear is flush against the orange tile's left edge while adjacent labels are centered.

- [x] `animation_declarative-02` · P2 · defect. Pulse label: Apply the same centered alignment as neighboring examples.
  Evidence: Pulse is flush against its orange tile's left edge.

- [x] `animation_declarative-03` · P2 · proposal. Row grouping: Label trigger examples, motion examples and composition examples.
  Evidence: Three rows have no section labels.

- [x] `animation_declarative-04` · P2 · proposal. Hover tile: Add a caption identifying hover scale.
  Evidence: Hover names the trigger but not the resulting animation.

- [x] `animation_declarative-05` · P2 · proposal. Click tile: Use Click to scale with a compact parameter caption.
  Evidence: Click! names an input but hides its scale behavior.

- [x] `animation_declarative-06` · P2 · proposal. Both tile: Label it Hover + click.
  Evidence: Both! requires guessing which two inputs combine.

- [x] `animation_declarative-07` · P2 · proposal. Appear tile: Add an On entry caption.
  Evidence: Appear has no indication that it runs on entry.

- [x] `animation_declarative-08` · P2 · proposal. Slide tile: Show a direction arrow and travel distance.
  Evidence: Slide does not state direction or distance.

- [x] `animation_declarative-09` · P2 · proposal. Bounce tile: Add the actual upward translation and spring frequency/decay.
  Evidence: Bounce translates upward; its motion and spring parameters were not shown.

- [x] `animation_declarative-10` · P2 · proposal. Fade tile: Add the trigger beneath the tile.
  Evidence: Fade does not state that hover triggers it.

- [x] `animation_declarative-11` · P2 · proposal. Pulse tile: Add Looping pulse under the specimen.
  Evidence: Pulse does not name its repeating behavior.

- [x] `animation_declarative-12` · P2 · proposal. Slide+Rot tile: Spell out Slide + rotate.
  Evidence: Rot abbreviates a motion unlike the full names above.

- [x] `animation_declarative-13` · P2 · proposal. Rot+Slide tile: Add numbered transform-order labels.
  Evidence: Rot+Slide looks almost identical to its neighboring composition.

- [x] `animation_declarative-14` · P2 · proposal. All 3 tile: List Scale + slide + rotate.
  Evidence: All 3 does not identify the included transforms.

- [x] `animation_declarative-15` · P2 · proposal. Composition reference: Add neutral outlines to expose transform offsets.
  Evidence: The bottom row provides no fixed reference bounds.

- [x] `animation_declarative-16` · P2 · proposal. Third-row layout: Balance three composition cards and a fourth legend across the shared grid.
  Evidence: Three bottom tiles sit left of the four-column grid's center.

- [x] `animation_declarative-17` · P2 · proposal. Fourth empty cell: Use it for a small transform-order legend.
  Evidence: An unmarked gap remains at the bottom right.

- [x] `animation_declarative-18` · P2 · proposal. Click count: Place it beside the trigger section heading.
  Evidence: Total clicks: 0 is separated from the clickable specimens.

- [x] `animation_declarative-19` · P2 · proposal. Click attribution: Add a last-trigger label next to the counter.
  Evidence: The total counter does not identify which tile was activated.

- [x] `animation_declarative-20` · P2 · proposal. Footer code: Use a readable monospace code block.
  Evidence: A full API expression is extremely small across the footer.

- [x] `animation_declarative-21` · P2 · proposal. Footer claim: Replace it with the exact current example configuration.
  Evidence: Old: ~50 lines of boilerplate is not illustrated.

- [x] `animation_declarative-22` · P2 · proposal. Subtitle: State that each tile declares its own trigger and animation.
  Evidence: No boilerplate is promotional wording without explaining the concept.

- [x] `animation_declarative-23` · P2 · proposal. Color meaning: Add a legend or assign colors consistently by trigger.
  Evidence: Blue, green, purple and orange repeat with no stated mapping.

- [x] `animation_declarative-24` · P2 · proposal. Replay discoverability: Add a replay-entry-examples action.
  Evidence: Entry animations have no visible restart control.

- [x] `animation_declarative-25` · P2 · proposal. Phase evidence: Add small phase labels outside the moving specimens.
  Evidence: The static view cannot distinguish idle and completed examples.

### animation_interactive

[Baseline](screenshot-baselines/screens/animation_interactive_720p.png) · [Visual review](design-audit.html#screen=animation_interactive)

- [x] `animation_interactive-01` · P2 · proposal. Header placement: Move it toward the top of the page.
  Evidence: The title begins below a very large empty top area.

- [x] `animation_interactive-02` · P2 · proposal. Header strips: Combine them into one header card.
  Evidence: Title and instruction have touching rounded backgrounds.

- [x] `animation_interactive-03` · P2 · proposal. Instruction wording: State that all three independent buttons use the same press effect.
  Evidence: Click buttons to see press animations does not explain the difference between them.

- [x] `animation_interactive-04` · P2 · proposal. Blue button: Label it with its press-scale amount.
  Evidence: Click! gives no animation information.

- [x] `animation_interactive-05` · P2 · proposal. Green button: Label the green instance of the shared press response.
  Evidence: Press! reads like a synonym of Click!.

- [x] `animation_interactive-06` · P2 · proposal. Purple button: Label the purple instance of the shared press response.
  Evidence: Tap! implies a different device without describing motion.

- [x] `animation_interactive-07` · P2 · proposal. Specimen captions: Show duration under each button.
  Evidence: None of the buttons has a timing label.

- [x] `animation_interactive-08` · P2 · proposal. Scale comparison: Add fixed outlines to make compression visible.
  Evidence: All three buttons are identical at rest.

- [x] `animation_interactive-09` · P2 · proposal. Easing comparison: Display each easing name below its specimen.
  Evidence: The screen does not identify the timing curves.

- [x] `animation_interactive-10` · P2 · proposal. Button grouping: Create equal comparison cards with shared diagnostic rows.
  Evidence: Three isolated squares lack card or section framing.

- [x] `animation_interactive-11` · P2 · proposal. Color mapping: Associate each color with a separate instance of the shared animation.
  Evidence: Blue, green and purple have no legend.

- [x] `animation_interactive-12` · P2 · proposal. Counter label: Show individual counts alongside the total.
  Evidence: Total clicks counts all variants together.

- [x] `animation_interactive-13` · P2 · proposal. Last action: Add a Last pressed readout.
  Evidence: The counter cannot identify the most recently pressed specimen.

- [x] `animation_interactive-14` · P2 · proposal. Counter reset: Add a small Reset count control.
  Evidence: The count is displayed without a visible reset action.

- [x] `animation_interactive-15` · P2 · proposal. Footer type: Use a readable monospace API label.
  Evidence: The with_scale() note is small and widely tracked.

- [x] `animation_interactive-16` · P2 · proposal. Footer separation: Place it beneath the comparison cards.
  Evidence: The implementation note sits far below the examples.

- [x] `animation_interactive-17` · P2 · proposal. Motion metrics: Add live scale percentages outside the animated buttons.
  Evidence: No current scale value appears in the screenshot.

- [x] `animation_interactive-18` · P2 · proposal. Playback aid: Add a labeled slow-preview action for comparing the three effects.
  Evidence: Only direct pressing is represented.

### animation_looping

[Baseline](screenshot-baselines/screens/animation_looping_720p.png) · [Visual review](design-audit.html#screen=animation_looping)

- [x] `animation_looping-01` · P2 · proposal. Header placement: Move the header upward to make space for diagnostics.
  Evidence: A large blank area precedes the title.

- [x] `animation_looping-02` · P2 · proposal. Header construction: Combine them in one padded header.
  Evidence: Title and subtitle occupy touching rounded strips.

- [x] `animation_looping-03` · P2 · proposal. Breathe label: Add a Translation caption.
  Evidence: Breathe does not state that the sample translates.

- [x] `animation_looping-04` · P2 · proposal. Glow label: Add an Opacity caption.
  Evidence: Glow does not identify opacity as the animated property.

- [x] `animation_looping-05` · P2 · proposal. Bounce label: Add a vertical motion arrow outside the specimen.
  Evidence: Bounce does not identify its direction.

- [x] `animation_looping-06` · P2 · proposal. Spin label: Identify the marker as the orbiting element.
  Evidence: Spin sits above a stationary-looking brown base and orange marker.

- [x] `animation_looping-07` · P2 · proposal. Breathe reference: Add an unmoving outline at its center position.
  Evidence: The blue block has no fixed position outline.

- [x] `animation_looping-08` · P2 · proposal. Glow reference: Add a fully opaque reference chip outside the animation.
  Evidence: The green sample is dim at this capture phase.

- [x] `animation_looping-09` · P2 · proposal. Bounce bounds: Draw endpoints beside its motion area.
  Evidence: The purple block has no marked travel range.

- [x] `animation_looping-10` · P2 · proposal. Spin path: Add a faint orbit guide behind it.
  Evidence: The orange marker's circular path is invisible.

- [x] `animation_looping-11` · P2 · proposal. Spin center: Add a small center crosshair.
  Evidence: The brown base does not mark the orbit center.

- [x] `animation_looping-12` · P2 · proposal. Period labels: Print the period below each name.
  Evidence: None of the four examples shows its cycle duration.

- [x] `animation_looping-13` · P2 · proposal. Phase labels: Add a phase readout beneath each sample.
  Evidence: The captured examples provide no numeric phase.

- [x] `animation_looping-14` · P2 · proposal. Amplitude labels: Display translation range or opacity range per example.
  Evidence: The motion extent is not specified.

- [x] `animation_looping-15` · P2 · proposal. Pause control: Add a Pause all action above the specimen group.
  Evidence: Continuous motion has no visible pause control.

- [x] `animation_looping-16` · P2 · proposal. Replay control: Add Restart cycles beside Pause.
  Evidence: There is no visible common phase reset.

- [x] `animation_looping-17` · P2 · proposal. Specimen balance: Increase marker visibility without changing its orbit.
  Evidence: Spin's small marker is much less prominent than the three solid blocks.

- [x] `animation_looping-18` · P2 · proposal. Color explanation: Use captions that pair each color with its animated property.
  Evidence: Four colors do not map to a stated property legend.

- [x] `animation_looping-19` · P2 · proposal. Footer note: Place a readable formula note under the motion diagnostics.
  Evidence: The sin/cos statement is far from the samples and small.

- [x] `animation_looping-20` · P2 · proposal. Static capture aid: Offer a labeled trail overlay for screenshot comparison.
  Evidence: A single frame cannot show travel history.

### animation_spring

[Baseline](screenshot-baselines/screens/animation_spring_720p.png) · [Visual review](design-audit.html#screen=animation_spring)

- [x] `animation_spring-01` · P2 · proposal. Title wording: Add Spring animation variants as a subtitle.
  Evidence: Springy! does not identify the technical comparison.

- [x] `animation_spring-02` · P2 · proposal. Title space: Move it upward to reduce unused space.
  Evidence: The title starts beneath a large empty top region.

- [x] `animation_spring-03` · P2 · proposal. Instruction strip: Align it to the specimen group's width.
  Evidence: The instruction stretches across the screen while specimens are narrow.

- [x] `animation_spring-04` · P2 · proposal. Boing caption: Add its actual oscillation frequency and exponential decay below.
  Evidence: Boing! names a personality rather than a motion property.

- [x] `animation_spring-05` · P2 · proposal. Jelly caption: Add a short axis or scale descriptor.
  Evidence: Jelly lacks a description of the deformation.

- [x] `animation_spring-06` · P2 · proposal. Balloon caption: Add a rise/scale descriptor matching the example.
  Evidence: Balloon does not state its motion direction.

- [x] `animation_spring-07` · P2 · proposal. Drop caption: Show a ghost start marker above the square.
  Evidence: Drop has no marked starting height.

- [x] `animation_spring-08` · P2 · proposal. Rocker caption: Show its horizontal origin and displacement range; this specimen does not rotate.
  Evidence: Rocker does not show its pivot.

- [x] `animation_spring-09` · P2 · proposal. Specimen bounds: Add fixed reference bounds outside the animated fill.
  Evidence: None of the five blocks has a neutral resting outline.

- [x] `animation_spring-10` · P2 · proposal. Color-only identification: Add numbered external labels matching the parameter captions.
  Evidence: The colored blocks contain no persistent identifiers.

- [x] `animation_spring-11` · P2 · proposal. Label size: Increase specimen-name size.
  Evidence: The five names are tiny compared with Springy!.

- [x] `animation_spring-12` · P2 · proposal. Label plates: Align each label and specimen within a shared card width.
  Evidence: 120-pixel plates extend wider than their 100-pixel blocks.

- [x] `animation_spring-13` · P2 · proposal. Spring parameters: Add a compact five-column parameter table.
  Evidence: No visible numeric parameters distinguish the samples.

- [x] `animation_spring-14` · P2 · proposal. Bounce counter: Rename it Activations if it counts clicks, or state the counting rule.
  Evidence: Bounces: 0 does not define what is counted.

- [x] `animation_spring-15` · P2 · proposal. Counter placement: Place it adjacent to the restart control.
  Evidence: The counter sits alone well below the specimens.

- [x] `animation_spring-16` · P2 · proposal. Counter attribution: Add a last-sample label.
  Evidence: The count does not identify the last activated spring.

- [x] `animation_spring-17` · P2 · proposal. Restart wording: Use Restart all five.
  Evidence: Restart! omits whether it affects all samples.

- [x] `animation_spring-18` · P2 · proposal. Restart spacing: Bring them into one control row.
  Evidence: A large empty gap separates the counter and restart button.

- [x] `animation_spring-19` · P2 · proposal. Motion phase: Show each specimen's phase below its card.
  Evidence: No label identifies idle, moving or settled samples.

- [x] `animation_spring-20` · P2 · proposal. Slow comparison: Add a slow-preview toggle for inspecting overshoot.
  Evidence: The page offers no visible slower playback aid.

### auto_text_color

[Baseline](screenshot-baselines/screens/auto_text_color_720p.png) · [Visual review](design-audit.html#screen=auto_text_color)

- [x] `auto_text_color-01` · P2 · proposal. New default badge: Replace it with a smaller Default enabled annotation or version.
  Evidence: NEW DEFAULT is a large yellow badge without a version.

- [x] `auto_text_color-02` · P2 · proposal. Title balance: Center title and status as a balanced group.
  Evidence: The badge pulls the combined heading off the visual center.

- [x] `auto_text_color-03` · P2 · proposal. Subtitle claim: State that the system chooses between the configured light and dark colors.
  Evidence: Against any background reads as an unlimited contrast guarantee.
  Disposition: The chooser also tries backend fallback colors when both theme candidates miss its target. The new subtitle describes all candidates and measured ratios; upstream fallback/validation issues remain in AFTERHOURS_GAPS.

- [x] `auto_text_color-04` · P2 · proposal. Default heading: Add the default flag name in a monospace caption.
  Evidence: No config needed omits the relevant API setting.

- [x] `auto_text_color-05` · P2 · proposal. Dark/light pairs: Label columns as paired background families.
  Evidence: Near Black over Near White is the only obvious named pair.

- [x] `auto_text_color-06` · P2 · proposal. Color values: Show compact RGB or hex captions outside the tiles.
  Evidence: Near Black and Dark Blue give no actual values.

- [x] `auto_text_color-07` · P2 · proposal. Chosen foreground: Add Light text or Dark text result captions.
  Evidence: The samples show results without naming the chosen text color.

- [x] `auto_text_color-08` · P2 · proposal. Contrast ratios: Display contrast ratios for the tested foreground/background pairs.
  Evidence: Best contrast appears twice without measured ratios.

- [x] `auto_text_color-09` · P2 · proposal. Dark tile borders: Use an explicitly labeled specimen boundary separate from color output.
  Evidence: Thin outlines distinguish some dark samples from the page.

- [x] `auto_text_color-10` · P2 · proposal. Light tile row: Label the rows Dark backgrounds and Light backgrounds.
  Evidence: The lighter row has no row label.

- [x] `auto_text_color-11` · P2 · proposal. Mid-tone spelling: Spell out Dark magenta.
  Evidence: Dk Magenta abbreviates the color inconsistently.

- [x] `auto_text_color-12` · P2 · proposal. Mid-tone comparison: Order by luminance or state the intended sequence.
  Evidence: Six hues appear without a luminance ordering.

- [x] `auto_text_color-13` · P2 · proposal. Gray specimen: Add a companion light-text reference outside the primary fixture.
  Evidence: Gray's dark text choice is difficult to compare against alternatives.

- [x] `auto_text_color-14` · P2 · proposal. Teal specimen: Show its luminance in the caption.
  Evidence: Teal shares a similar brightness with nearby swatches but lacks numeric context.

- [x] `auto_text_color-15` · P2 · proposal. Explicit options heading: Label the row Configuration examples.
  Evidence: The options look like actions even though they are demonstration specimens.

- [x] `auto_text_color-16` · P2 · proposal. Disabled wording: Use Auto color off: theme font.
  Evidence: Disabled (theme font) can be mistaken for a disabled control.

- [x] `auto_text_color-17` · P2 · proposal. Auto default specimen: Add the resolved foreground below it.
  Evidence: Auto (default) does not show its selected foreground value.

- [x] `auto_text_color-18` · P2 · proposal. Custom red specimen: Add an Explicit override caption.
  Evidence: Custom Red Text does not state that it overrides automatic selection.

- [x] `auto_text_color-19` · P2 · proposal. Explicit comparison: Add a matched-background comparison row.
  Evidence: The middle example uses a different background from the two outer examples.

- [x] `auto_text_color-20` · P2 · proposal. Theme heading: Use Theme role examples.
  Evidence: Works with Theme Colors Too is longer than neighboring section names.

- [x] `auto_text_color-21` · P2 · proposal. Background role: Add an external outline marking its specimen bounds.
  Evidence: Background has no visible tile boundary because it matches the page.

- [x] `auto_text_color-22` · P2 · proposal. Theme identity: Add the active theme name above the role row.
  Evidence: Primary, Accent and Secondary do not name the active theme.

- [x] `auto_text_color-23` · P2 · proposal. Role ordering: Group Primary and Secondary before Accent.
  Evidence: Primary, Accent, Secondary uses an unusual sequence.

- [x] `auto_text_color-24` · P2 · proposal. Separators: Align dividers with the headings' content edge.
  Evidence: Rules start farther inward than section headings.

- [x] `auto_text_color-25` · P2 · proposal. Specimen typography: Use a readable body face while retaining the exact tested colors.
  Evidence: All examples use widely spaced handwritten labels.

### bevel_borders

[Baseline](screenshot-baselines/screens/bevel_borders_720p.png) · [Visual review](design-audit.html#screen=bevel_borders)

- [x] `bevel_borders-01` · P2 · proposal. Top blank area: Move the title and matrix upward.
  Evidence: The matrix begins after nearly two hundred pixels of empty space.

- [x] `bevel_borders-02` · P2 · proposal. Title strip: Match the title width to the matrix.
  Evidence: BEVEL BORDERS occupies an almost full-width bordered bar.

- [x] `bevel_borders-03` · P2 · proposal. Title casing: Use sentence case with a stronger weight.
  Evidence: All-capital handwritten title is less readable than the subtitle.

- [x] `bevel_borders-04` · P2 · proposal. Explanation: Split it into two labeled directional diagrams.
  Evidence: The raised/sunken lighting rule is written as one dense line.

- [x] `bevel_borders-05` · P2 · proposal. Row labels: Reduce the gap to the row-label gutter.
  Evidence: Raised and Sunken sit far from their first specimen.

- [x] `bevel_borders-06` · P2 · proposal. Column labels: Keep the column heading and replace inner text with a neutral sample mark.
  Evidence: Each thickness is repeated above and inside its squares.

- [x] `bevel_borders-07` · P2 · proposal. Numeric type: Use tabular numeric text for pixel measurements.
  Evidence: Thickness values use handwritten numerals.

- [x] `bevel_borders-08` · P2 · proposal. One-pixel detail: Add a magnified 1px corner inset.
  Evidence: The thinnest border is difficult to inspect at normal size.

- [x] `bevel_borders-09` · P2 · proposal. Six-pixel detail: Add a 6px bracket outside its specimen.
  Evidence: The widest border lacks a dimension bracket.

- [x] `bevel_borders-10` · P2 · proposal. Lighting direction: Add a small top-left light-direction arrow.
  Evidence: No icon indicates the common top-left light source.

- [x] `bevel_borders-11` · P2 · proposal. Raised edges: Label one reference square's highlight and shadow edges.
  Evidence: Light and dark edges are not individually identified.

- [x] `bevel_borders-12` · P2 · proposal. Sunken inversion: Add a small inversion annotation connecting one raised/sunken pair.
  Evidence: The lower row repeats the rule without showing correspondence.

- [x] `bevel_borders-13` · P2 · proposal. Base fill: Add the face-color value in a legend.
  Evidence: The matrix does not name its shared face color.

- [x] `bevel_borders-14` · P2 · proposal. Edge colors: Show labeled highlight and shadow chips.
  Evidence: Highlight and shadow colors lack a reference swatch.

- [x] `bevel_borders-15` · P2 · proposal. Baseline control: Add a zero-width border reference.
  Evidence: All displayed boxes have bevels.

- [x] `bevel_borders-16` · P2 · proposal. Outer dimensions: Show the shared specimen dimensions.
  Evidence: The matrix does not state whether outer box sizes are fixed.

- [x] `bevel_borders-17` · P2 · proposal. Interior size: Add a compact interior-dimension caption per column.
  Evidence: Thicker borders visibly reduce the usable interior.

- [x] `bevel_borders-18` · P2 · proposal. Lower blank area: Use it for a single enlarged annotated bevel cross-section.
  Evidence: The bottom quarter is empty.

### blend_mode_lab

[Baseline](screenshot-baselines/screens/blend_mode_lab_720p.png) · [Visual review](design-audit.html#screen=blend_mode_lab)

- [x] `blend_mode_lab-01` · P2 · proposal. Title: Separate the page title from the transition counter.
  Evidence: Blend modes. Real transitions last frame: 24 merges heading and live metric.

- [x] `blend_mode_lab-02` · P2 · proposal. Title inset: Add horizontal padding.
  Evidence: The title begins at the rounded strip's left edge.

- [x] `blend_mode_lab-03` · P2 · proposal. Transition definition: Label it GPU blend-state changes or the precise measured operation.
  Evidence: Real transitions is undefined in the visible frame.

- [x] `blend_mode_lab-04` · P2 · proposal. Transition scope: Show a short scope label.
  Evidence: 24 does not say whether it includes both specimen rows.

- [x] `blend_mode_lab-05` · P2 · proposal. Alpha caption: Add the input alpha value.
  Evidence: Alpha omits the configured opacity.

- [x] `blend_mode_lab-06` · P2 · proposal. Additive caption: Add a compact source-plus-destination caption.
  Evidence: Additive provides no blending equation.

- [x] `blend_mode_lab-07` · P2 · proposal. Multiply caption: Use Multiply.
  Evidence: Multiplied differs grammatically from the other mode names.

- [x] `blend_mode_lab-08` · P2 · proposal. Premultiply caption: Use Premultiplied alpha with the API name beneath.
  Evidence: AlphaPremultiply is a compressed API identifier.

- [x] `blend_mode_lab-09` · P2 · proposal. Mode title type: Increase label size while preserving specimen area.
  Evidence: The four mode labels are tiny.

- [x] `blend_mode_lab-10` · P2 · proposal. Input colors: Add a labeled RGB input legend.
  Evidence: Red, green and blue inputs are visible but unspecified.

- [x] `blend_mode_lab-11` · P2 · proposal. Circle order: Number the input circles outside their bounds.
  Evidence: Three overlaps do not state draw order.

- [x] `blend_mode_lab-12` · P2 · proposal. Alpha overlap: Add a GPU replay of the center overlap with a separately labeled rounded RGB model.
  Evidence: The center intersection has no measurement label.

- [x] `blend_mode_lab-13` · P2 · proposal. Multiply visibility: Add a labeled magnified sample beside the panel.
  Evidence: Dark output is expected but hard to compare to the background.

- [x] `blend_mode_lab-14` · P2 · proposal. Premultiply contract: State the source-color preparation in its caption.
  Evidence: The panel does not say whether colors are pre-multiplied.

- [x] `blend_mode_lab-15` · P2 · proposal. Backing color: Show the shared destination color value.
  Evidence: All modes share a dark panel but its color is unnamed.

- [x] `blend_mode_lab-16` · P2 · proposal. Circle geometry: Document shared radius and overlap distance once.
  Evidence: The specimens have no fixed geometry caption.

- [x] `blend_mode_lab-17` · P2 · proposal. Top panel alignment: Give all four captions a consistent clear gap.
  Evidence: Captions sit very close to panel tops.

- [x] `blend_mode_lab-18` · P2 · proposal. Batch row headers: Increase their size and simplify the wording.
  Evidence: One scope per blit and round the batch are very small.

- [x] `blend_mode_lab-19` · P2 · proposal. Per-blit count: Show 8 blits × 2 = 16 transitions.
  Evidence: 2 transitions each leaves the total implicit.

- [x] `blend_mode_lab-20` · P2 · proposal. Batch total: Show 8 blits, 2 transitions.
  Evidence: 2 total does not identify the number of blits.

- [x] `blend_mode_lab-21` · P2 · proposal. Batch item identity: Number them lightly outside their fills.
  Evidence: Eight blue squares per panel have no identifiers.

- [x] `blend_mode_lab-22` · P2 · proposal. Batch equivalence: State that output should match while state changes differ.
  Evidence: The two rows look identical without an explicit expected-result note.

- [x] `blend_mode_lab-23` · P2 · proposal. Batch boundaries: Draw one bracket over the batch and separate brackets over per-blit samples.
  Evidence: The shared scope is described only in text.

- [x] `blend_mode_lab-24` · P2 · proposal. Blank lower region: Add a small comparison table of mode and transition count.
  Evidence: A large lower region is unused.

- [x] `blend_mode_lab-25` · P2 · proposal. Capture metadata: Show a stable capture/frame identifier beside the metric.
  Evidence: The live count has no frame marker beyond last frame.

### button_variants

[Baseline](screenshot-baselines/screens/button_variants_720p.png) · [Visual review](design-audit.html#screen=button_variants)

- [x] `button_variants-01` · P2 · proposal. Title inset: Align it with the row-label inset.
  Evidence: Button Variants touches the header's left edge.

- [x] `button_variants-02` · P2 · proposal. Matrix headings: Add column headings above the matrix.
  Evidence: Normal, Accent, Secondary and Disabled repeat inside every row.

- [x] `button_variants-03` · P2 · proposal. Normal terminology: Rename the role Primary.
  Evidence: Normal occupies the same category position as theme color roles.

- [x] `button_variants-04` · P2 · proposal. Disabled comparison: Label the last column Disabled primary.
  Evidence: Disabled mixes availability with three color roles.

- [x] `button_variants-05` · P2 · proposal. Disabled fixture framing: Add neutral external specimen outlines.
  Evidence: Disabled boundaries blend into the panel.

- [x] `button_variants-06` · P2 · proposal. Ghost framing: Draw optional diagnostic bounds outside the test controls.
  Evidence: Ghost controls look like disconnected text labels.

- [x] `button_variants-07` · P2 · proposal. Outline comparison: Add enlarged corner samples for the outline row.
  Evidence: Thin colored outlines are hard to inspect.
  Disposition: Kept real outline controls with optional exact bounds and requested stroke/radius captions. A fabricated magnified corner would conceal the known native uniform-border thickness limitation.

- [x] `button_variants-08` · P2 · proposal. Row separation: Use a consistent narrow row gutter.
  Evidence: Filled, Outline and Ghost panels touch with scalloped seams.

- [x] `button_variants-09` · P2 · proposal. Row-label alignment: Bring them closer in a fixed label column.
  Evidence: Labels sit far left of their specimen grid.

- [x] `button_variants-10` · P2 · proposal. Specimen sizing: Add one shared width and height caption.
  Evidence: Main matrix controls have no dimensions shown.

- [x] `button_variants-11` · P2 · proposal. Small specimen: Add measured dimensions below it.
  Evidence: Small lacks its numeric size.

- [x] `button_variants-12` · P2 · proposal. Medium specimen: Add a compact token caption.
  Evidence: Medium has no padding or text-size annotation.

- [x] `button_variants-13` · P2 · proposal. Large specimen: State both font and control dimensions.
  Evidence: Large differs in typography as well as box size.

- [x] `button_variants-14` · P2 · proposal. Large outline label: Use Large outline.
  Evidence: Lg Outline abbreviates a word used fully elsewhere.

- [x] `button_variants-15` · P2 · proposal. Size row comparison: Keep the role constant or label the extra variable.
  Evidence: Large outline also changes color role.

- [x] `button_variants-16` · P2 · proposal. Theme cards: Distribute the three cards evenly across the specimen area.
  Evidence: Neon Dark, Cozy Kraft and Sage are left clustered.

- [x] `button_variants-17` · P2 · proposal. Theme titles: Give card headings a clearer weight.
  Evidence: Theme names share the small size of specimen labels.

- [x] `button_variants-18` · P2 · proposal. Theme button density: Add a small consistent vertical gap.
  Evidence: Filled and Outline buttons touch inside the cards.

- [x] `button_variants-19` · P2 · proposal. Theme ghost position: Align it in a reserved equal-height row.
  Evidence: Ghost text sits immediately below outline specimens.

- [x] `button_variants-20` · P2 · proposal. Theme dimensions: State that these are compact previews.
  Evidence: Theme samples are much smaller than the main matrix.

- [x] `button_variants-21` · P2 · proposal. Theme output: Add a concise color-pair caption per card.
  Evidence: No resolved foreground values accompany the theme examples.

- [x] `button_variants-22` · P2 · proposal. Main theme identity: Name the main theme beside the title.
  Evidence: The navy matrix does not identify its active theme.

- [x] `button_variants-23` · P2 · proposal. Footer instruction: Add matching horizontal padding.
  Evidence: Try any variant above is flush against the footer edge.

- [x] `button_variants-24` · P2 · proposal. Footer feedback: State that the footer reports the chosen variant.
  Evidence: The initial instruction does not explain the result of pressing a specimen.

- [x] `button_variants-25` · P2 · proposal. Legend: Add a compact key distinguishing those three dimensions.
  Evidence: Variant appearance, color role and size are interleaved without a guide.

### buttons

[Baseline](screenshot-baselines/screens/buttons_720p.png) · [Visual review](design-audit.html#screen=buttons)

- [x] `buttons-01` · P2 · proposal. Title padding: Add the same inset as row labels.
  Evidence: Button Gallery starts at the strip's edge.

- [x] `buttons-02` · P2 · proposal. Header size: Reduce its height or add a short purpose subtitle.
  Evidence: The title band is much taller than its text requires.

- [x] `buttons-03` · P2 · proposal. States label: Rename the row Roles and availability.
  Evidence: States contains Accent and Secondary, which are color roles.

- [x] `buttons-04` · P2 · proposal. Normal button: Label it Primary.
  Evidence: Normal does not identify the primary theme role.

- [x] `buttons-05` · P2 · proposal. Disabled specimen: Add a readable external Disabled caption.
  Evidence: The deliberately dim label is difficult to identify independently.

- [x] `buttons-06` · P2 · proposal. Disabled bounds: Add an external diagnostic boundary.
  Evidence: The disabled outline is hard to distinguish from the panel.

- [x] `buttons-07` · P2 · proposal. State dimensions: Add a dimensions caption for this row.
  Evidence: The four specimens show no shared measurement.

- [x] `buttons-08` · P2 · proposal. Size naming: Use Large for parallel labels.
  Evidence: Large Button differs from Small and Medium naming.

- [x] `buttons-09` · P2 · proposal. Size comparison: Print their exact height and padding below them.
  Evidence: Small and Medium appear similar in height.

- [x] `buttons-10` · P2 · proposal. Size width: Use a shared test label with external size captions.
  Evidence: Different label lengths influence perceived button size.

- [x] `buttons-11` · P2 · proposal. Size alignment: Add an optional baseline guide.
  Evidence: Controls are centered vertically but no baseline aid is shown.

- [x] `buttons-12` · P2 · proposal. Group spacing: Tighten spacing to show a coherent segmented group.
  Evidence: One through Four are separated widely despite group-specific corner shapes.

- [x] `buttons-13` · P2 · proposal. Group semantics: Use four related view options for the group example.
  Evidence: One, Two, Three and Four provide no shared task.

- [x] `buttons-14` · P2 · proposal. Group boundary: Add a caption identifying first, middle and last corners.
  Evidence: There is no outer label explaining the asymmetric corner shapes.

- [x] `buttons-15` · P2 · proposal. Group selection: State the group's intended behavior in a caption.
  Evidence: No visible marker explains whether this is a selection group or independent actions.

- [x] `buttons-16` · P2 · proposal. Custom heading: Split color examples from the corner example visually.
  Evidence: Custom combines color and corner experiments.

- [x] `buttons-17` · P2 · proposal. Coral sample: Add its configured hex value below it.
  Evidence: Coral shows no color value.

- [x] `buttons-18` · P2 · proposal. Teal sample: Add a foreground-selection caption.
  Evidence: Teal's text treatment differs from Coral without explanation.

- [x] `buttons-19` · P2 · proposal. Sharp sample: Add a matched rounded control for direct comparison.
  Evidence: Sharp lacks a rounded reference beside it.

- [x] `buttons-20` · P2 · proposal. Main color mapping: Add a small role-color legend.
  Evidence: Blue, ochre and green are unnamed theme colors.

- [x] `buttons-21` · P2 · proposal. Row width: Use the space for short specimen notes.
  Evidence: Most of each full-width row remains empty on the right.

- [x] `buttons-22` · P2 · proposal. Row alignment: Use a shared four-column comparison grid.
  Evidence: The largest-size specimen extends farther right than other rows.

- [x] `buttons-23` · P2 · proposal. Panel gaps: Reduce gaps to make room for useful captions.
  Evidence: Vertical spacing dominates the compact specimen content.

- [x] `buttons-24` · P2 · proposal. Footer padding: Inset footer text consistently.
  Evidence: Try any button above begins at the bar edge.

- [x] `buttons-25` · P2 · proposal. Footer meaning: Describe the selected-button feedback shown there.
  Evidence: The instruction does not say what activation reports.

### cards

[Baseline](screenshot-baselines/screens/cards_720p.png) · [Visual review](design-audit.html#screen=cards)

- [x] `cards-01` · P2 · defect. Style specimen labels: Give each label visible internal padding.
  Evidence: Surface, Primary, Sharp and Custom begin directly on their left borders.

- [x] `cards-02` · P2 · defect. Nested item labels: Add consistent inset to nested labels.
  Evidence: Item 1, Item 2 and Item 3 touch their colored blocks' left edges.

- [x] `cards-03` · P2 · proposal. Title inset: Align it to the main content inset.
  Evidence: Cards & Panels Gallery is flush with the brown header's edge.

- [x] `cards-04` · P2 · proposal. Header width: Use a consistent content width.
  Evidence: The header is narrower than the white style and nested rows.

- [x] `cards-05` · P2 · proposal. Style row label: Use a plain section heading to distinguish labels from specimens.
  Evidence: Styles is a dark button-shaped block.

- [x] `cards-06` · P2 · proposal. Surface example: Add an external labeled bounds guide.
  Evidence: The white card almost merges into the white row.

- [x] `cards-07` · P2 · proposal. Primary example: Add its role and resolved color beneath it.
  Evidence: Primary has no theme-color value shown.

- [x] `cards-08` · P2 · proposal. Sharp example: Add a matched rounded specimen or state both changed variables.
  Evidence: Sharp also changes fill and border weight.
  Disposition: Kept matched 1px requested borders and disclosed corner differences. Actual uniform-border thickness remains an upstream issue; changing the specimen request would hide it.

- [x] `cards-09` · P2 · proposal. Custom example: Caption the background, border and radius values.
  Evidence: Custom does not identify what was customized.

- [x] `cards-10` · P2 · proposal. Style dimensions: Show the shared outer dimensions once.
  Evidence: The card specimens have no size annotation.

- [x] `cards-11` · P2 · proposal. Style gaps: Use a regular matrix with room for parameter captions.
  Evidence: Wide gaps separate four small cards.

- [x] `cards-12` · P2 · proposal. Nested section label: Move it above the nested examples as a heading.
  Evidence: Nested is vertically centered in a button-like plaque.

- [x] `cards-13` · P2 · proposal. Nested card hierarchy: Increase heading weight and size.
  Evidence: Nested Cards has similar typography to its item labels.

- [x] `cards-14` · P2 · proposal. Nested boundaries: Add a clearer external outline marking the parent bounds.
  Evidence: The outer nested card border is faint.

- [x] `cards-15` · P2 · proposal. Nested padding: Annotate the parent padding.
  Evidence: Parent-child insets are visible but unmeasured.

- [x] `cards-16` · P2 · proposal. Nested item spacing: Add a shared gap caption.
  Evidence: The three inner items have no gap value shown.

- [x] `cards-17` · P2 · proposal. Translucent heading: Make the title treatment consistent with Nested Cards.
  Evidence: Semi-Transparent sits in a bright white title strip.

- [x] `cards-18` · P2 · proposal. Opacity evidence: Add a checkerboard or stripe behind the sample to expose blending.
  Evidence: The translucent panel overlays a nearly flat backing color.

- [x] `cards-19` · P2 · proposal. Opacity value: Display the configured percentage.
  Evidence: Uses opacity does not state the actual alpha.

- [x] `cards-20` · P2 · proposal. Opacity copy: Adjust text width or shorten the explanation.
  Evidence: The description wraps into an awkward short second line.

- [x] `cards-21` · P2 · proposal. Theme palette label: Use a neutral section label.
  Evidence: Theme Colors is a brown tile that resembles the primary swatch.

- [x] `cards-22` · P2 · proposal. Theme palette values: Add compact resolved values beneath the chips.
  Evidence: Five role chips show names without color values.

- [x] `cards-23` · P2 · proposal. Background swatch: Give the specimen an external boundary.
  Evidence: Background nearly matches the page around the palette.

- [x] `cards-24` · P2 · proposal. Theme identity: Name Cozy Kraft near the title.
  Evidence: The kraft palette has no visible theme name.

- [x] `cards-25` · P2 · proposal. Vertical spacing: Tighten spacing and allocate room for card measurements.
  Evidence: Large empty bands separate the three rows.

### casual_settings

[Baseline](screenshot-baselines/screens/casual_settings_720p.png) · [Visual review](design-audit.html#screen=casual_settings)

- [x] `casual_settings-01` · P2 · proposal. Music button: Add MUSIC and its current state beneath the icon.
  Evidence: The note icon has no visible label or ON text.

- [x] `casual_settings-02` · P2 · proposal. Sound button: Add SOUND EFFECTS and its current state.
  Evidence: The speaker icon lacks a separate effects label.

- [x] `casual_settings-03` · P2 · proposal. Vibration button: Add VIBRATION OFF beneath it.
  Evidence: Gray fill may read as unavailable rather than off.

- [x] `casual_settings-04` · P2 · proposal. Audio group: Add a compact AUDIO heading.
  Evidence: Three icon buttons have no group heading.

- [x] `casual_settings-05` · P2 · proposal. Save/load label: Use CLOUD SAVE with a descriptive secondary caption.
  Evidence: SAVE/LOAD PROGRESS combines two distinct operations.
  Disposition: This is a session-local progress slot, not cloud storage. The control now says Progress Save and explains save/restore for this session.

- [x] `casual_settings-06` · P2 · proposal. Save/load icon: Use a cloud-save icon.
  Evidence: A Wi-Fi symbol can imply connectivity rather than stored progress.
  Disposition: Used a save-slot icon; a cloud icon would falsely imply remote persistence.

- [x] `casual_settings-07` · P2 · proposal. Save status: Add a short saved-state line nearby.
  Evidence: The save button shows no account or last-sync status.

- [x] `casual_settings-08` · P2 · proposal. Notifications control: Add a visible switch indicator.
  Evidence: NOTIFICATIONS: OFF looks like a navigation button.

- [x] `casual_settings-09` · P2 · proposal. Notifications label: Separate the setting name from the OFF state.
  Evidence: The full label is denser than neighboring actions.

- [x] `casual_settings-10` · P2 · proposal. Language button: Show English or the selected language as a secondary value.
  Evidence: LANGUAGE does not display the current language.

- [x] `casual_settings-11` · P2 · proposal. Credits priority: Reduce its size or move it into an About group.
  Evidence: CREDITS has the same prominence as core preferences.

- [x] `casual_settings-12` · P2 · proposal. Support priority: Group it with help and legal actions.
  Evidence: SUPPORT visually competes with save/load.

- [x] `casual_settings-13` · P2 · proposal. Legal button: Use a quieter legal-link treatment.
  Evidence: TERMS AND PRIVACY occupies a large glossy action.

- [x] `casual_settings-14` · P2 · proposal. Build identifier: Prefix it with Build.
  Evidence: 15555-1-114203-20-10200-01 lacks a label.

- [x] `casual_settings-15` · P2 · proposal. Version text: Use a compact readable numeric face.
  Evidence: The version uses playful heavy lettering for technical data.

- [x] `casual_settings-16` · P2 · proposal. Player identifier: Group digits for scanning while preserving the exact value.
  Evidence: Player ID is a long uninterrupted number.

- [x] `casual_settings-17` · P2 · proposal. Metadata alignment: Left-align a compact two-column label/value block.
  Evidence: Build, Version and Player ID are centered as one dense block.

- [x] `casual_settings-18` · P2 · proposal. Metadata placement: Put them in an About footer.
  Evidence: Technical identifiers sit under Language without a section label.

- [x] `casual_settings-19` · P2 · proposal. Close control: Move it inward slightly for a cleaner safe margin.
  Evidence: The close button extends beyond the board's inner edge.

- [x] `casual_settings-20` · P2 · proposal. Close semantics: Add Changes save automatically if that is the intended behavior.
  Evidence: There is no visible saved-state reassurance near the sole exit.
  Disposition: Preferences apply immediately for this session; copy describes that lifetime without promising persistence.

- [x] `casual_settings-21` · P2 · proposal. Background currency: Restore a coherent subdued currency chip.
  Evidence: 1000 coins appears faintly at the top right without an icon.

- [x] `casual_settings-22` · P2 · proposal. Grid grouping: Group settings on one side and account/help actions on the other.
  Evidence: Preferences and support actions alternate across the two columns.
  Disposition: Grouped audio/notification/language settings separately from progress and help. No account exists in this fixture.

- [x] `casual_settings-23` · P2 · proposal. Save text wrapping: Increase line spacing and rebalance icon spacing.
  Evidence: SAVE/LOAD PROGRESS uses a tight two-line block.

- [x] `casual_settings-24` · P2 · proposal. Button shadows: Reduce shadow depth to open the layout.
  Evidence: Heavy dark shadows crowd the gaps between rows.

- [x] `casual_settings-25` · P2 · proposal. Bottom line: Remove it unless it represents a deliberate device affordance.
  Evidence: A long pale line below the board has no clear role.

### chart_lab

[Baseline](screenshot-baselines/screens/chart_lab_720p.png) · [Visual review](design-audit.html#screen=chart_lab)

- [x] `chart_lab-01` · P2 · defect. Axis labels: Increase tick-label size and reserve a proper axis gutter.
  Evidence: The Y endpoints 11 and 2 are tiny at the far left.

- [x] `chart_lab-02` · P2 · proposal. X units: Place seconds next to the X axis.
  Evidence: Seconds appears only in the bottom instruction.

- [x] `chart_lab-03` · P2 · proposal. Y units: Label the Y axis ms.
  Evidence: Milliseconds appears only in the bottom instruction.

- [x] `chart_lab-04` · P2 · proposal. X ticks: Add evenly spaced intermediate time ticks.
  Evidence: Only 0 and 1.98 are shown across a wide plot.

- [x] `chart_lab-05` · P2 · proposal. Y ticks: Label each major Y tick.
  Evidence: Several horizontal grid lines have no numeric values.

- [x] `chart_lab-06` · P2 · proposal. Legend: Increase their size and add short line swatches.
  Evidence: Update and Render labels are extremely small.

- [x] `chart_lab-07` · P2 · proposal. Series distinction: Give one a dashed stroke or different marker shape.
  Evidence: The two series differ only by color.

- [x] `chart_lab-08` · P2 · proposal. Line weight: Increase line width modestly.
  Evidence: Both series are very thin against the dark plot.
  Disposition: Deferred upstream: line_chart hardcodes its 2px series stroke. Kept native rendering and recorded configurable stroke in the project backlog.

- [x] `chart_lab-09` · P2 · proposal. Grid color: Use a neutral subdued grid color.
  Evidence: Purple grid lines introduce a third color unrelated to the series.

- [x] `chart_lab-10` · P2 · proposal. Selected sample: Draw a vertical selection line through the selected time.
  Evidence: Dots at the left mark the first sample without a shared guide.

- [x] `chart_lab-11` · P2 · proposal. Selected time: Show the corresponding timestamp next to the count.
  Evidence: Sample 1 of 120 omits the selected time.

- [x] `chart_lab-12` · P2 · proposal. Value labels: Use a compact selected-sample readout outside the plot.
  Evidence: 8.00 ms and 6.00 ms float near the first points.
  Disposition: Added grouped values outside the plot. Native floating hover values remain because line_chart has no suppression option; tracked upstream.

- [x] `chart_lab-13` · P2 · proposal. Sample counter: Group index, time and both series values.
  Evidence: The count is detached from both value labels.

- [x] `chart_lab-14` · P2 · proposal. Previous action: Show the boundary state when no prior sample exists.
  Evidence: Previous sample looks active while sample 1 is selected.

- [x] `chart_lab-15` · P2 · proposal. Preset selection: Mark the active dataset preset.
  Evidence: Empty, Single, Constant, Negative and Multiple all look identical.

- [x] `chart_lab-16` · P2 · proposal. Multiple wording: Rename it Multiple series.
  Evidence: Multiple does not identify multiple series.

- [x] `chart_lab-17` · P2 · proposal. Single wording: Rename it Single sample.
  Evidence: Single does not distinguish one sample from one series.

- [x] `chart_lab-18` · P2 · proposal. Fixture controls: Separate fixtures from playback controls.
  Evidence: Dataset buttons and Start live share one appearance.

- [x] `chart_lab-19` · P2 · proposal. Live status: Add a Live off status chip.
  Evidence: Start live has no persistent paused/live indicator beside it.

- [x] `chart_lab-20` · P2 · proposal. Chart title: Use Frame timing chart with a demo subtitle.
  Evidence: Charts / frame timings mixes gallery and metric names.

- [x] `chart_lab-21` · P2 · proposal. Plot padding: Add consistent internal plot padding.
  Evidence: The chart begins close to the legend and endpoint labels.

- [x] `chart_lab-22` · P2 · proposal. Negative fixture cue: Label it Negative-value test data.
  Evidence: Negative is available without any explanation of why timings can be negative.

- [x] `chart_lab-23` · P2 · proposal. Instruction density: Separate input help from chart metadata.
  Evidence: Hover, Tab, Enter and axis units are packed into one line.

- [x] `chart_lab-24` · P2 · proposal. Sample navigation: Use compact arrows with a clear shared Sample label.
  Evidence: Long Previous sample and Next sample buttons dominate the footer.

- [x] `chart_lab-25` · P2 · proposal. Frame budget: Add an optional labeled frame-budget reference line.
  Evidence: The chart has no contextual timing threshold.
  Disposition: Added the optional 8.33ms reference within the current data domain and explicit above/below-range status otherwise. Configurable axis domains remain upstream.

### checkboxes

[Baseline](screenshot-baselines/screens/checkboxes_720p.png) · [Visual review](design-audit.html#screen=checkboxes)

- [x] `checkboxes-01` · P2 · defect. Left labels: Add consistent label inset.
  Evidence: Primary, Secondary and Accent touch their colored row edges.

- [x] `checkboxes-02` · P2 · defect. Right labels: Inset labels to match the left column.
  Evidence: Option A through D begin directly at the row boundary.

- [x] `checkboxes-03` · P2 · proposal. Checkbox glyphs: Render clear square checkbox marks.
  Evidence: Selections use literal [x] and [ ] text.

- [x] `checkboxes-04` · P2 · proposal. Box-only geometry: Use square specimens or label the pill geometry as intentional.
  Evidence: Box Only specimens look like wide pills.

- [x] `checkboxes-05` · P2 · proposal. Box-only identity: Add numbered captions outside the controls.
  Evidence: Four unlabeled controls have no external identifiers.

- [x] `checkboxes-06` · P2 · proposal. Box-only states: Add an external on/off legend.
  Evidence: Two empty pills have no explicit off-state reference.

- [x] `checkboxes-07` · P2 · proposal. With Label heading: Give headings a neutral treatment.
  Evidence: The section heading uses the same teal fill as its controls.

- [x] `checkboxes-08` · P2 · proposal. Color role row: Add a matched-state comparison caption.
  Evidence: Primary, Secondary and Accent change row fill along with selection state.

- [x] `checkboxes-09` · P2 · proposal. Disabled heading: Use a neutral section header.
  Evidence: Disabled uses bright teal while its examples are intentionally dim.

- [x] `checkboxes-10` · P2 · proposal. Disabled ON wording: Use Checked and Unchecked.
  Evidence: ON/OFF terminology differs from checkbox checked state.

- [x] `checkboxes-11` · P2 · proposal. Disabled specimens: Add readable external Disabled captions.
  Evidence: Dim text is the only clear explanation of disabled status.

- [x] `checkboxes-12` · P2 · proposal. Multi-select heading: Separate the count into a compact badge.
  Evidence: Selected: 2 of 4 is embedded in a long header.

- [x] `checkboxes-13` · P2 · proposal. Option names: Use four related preference names.
  Evidence: Option A through D carry no meaningful choice example.

- [x] `checkboxes-14` · P2 · proposal. Count duplication: Use one prominent count near the group.
  Evidence: 2 of 4 appears in both the header and tiny footer.

- [x] `checkboxes-15` · P2 · proposal. Constrained heading: Use a concrete preference-selection example.
  Evidence: Select 1–2 options is followed by generic Choice names.

- [x] `checkboxes-16` · P2 · proposal. Choice 1 state: Add a note that one selection must remain.
  Evidence: The selected minimum-bound choice appears disabled with no explanation.

- [x] `checkboxes-17` · P2 · proposal. Selection rule: Show Min 1, max 2 beside the count.
  Evidence: The maximum of two has no visible state summary beyond the heading.

- [x] `checkboxes-18` · P2 · proposal. Choice color: Explain the category mapping or neutralize row fills.
  Evidence: Constrained choices use purple while unconstrained choices use teal.

- [x] `checkboxes-19` · P2 · proposal. Checkbox alignment: Bring marks closer to the label or align them in a clear trailing column.
  Evidence: Check marks are far from their labels in long filled rows.

- [x] `checkboxes-20` · P2 · proposal. Column titles: Add equal top-level labels for primitive and grouped examples.
  Evidence: Left column starts With Label while right starts Multi-Select.

- [x] `checkboxes-21` · P2 · proposal. Row corner seams: Add narrow regular gutters or shared group corners.
  Evidence: Touching rounded rows create scalloped edges.

- [x] `checkboxes-22` · P2 · proposal. Footer size: Increase summary typography.
  Evidence: Both selection summaries are very small.

- [x] `checkboxes-23` · P2 · proposal. Footer alignment: Align each beneath its associated column.
  Evidence: The summaries cluster near the middle instead of centering below their groups.

- [x] `checkboxes-24` · P2 · proposal. Lower blank space: Use it for the constrained-selection rule explanation.
  Evidence: The bottom portion of the panel is mostly empty.

- [x] `checkboxes-25` · P2 · proposal. Title scale: Reduce the title slightly and strengthen section hierarchy.
  Evidence: Checkbox Components dominates the small row labels.

### circular_progress

[Baseline](screenshot-baselines/screens/circular_progress_720p.png) · [Visual review](design-audit.html#screen=circular_progress)

- [x] `circular_progress-01` · P2 · proposal. Basic naming: Add the stroke width below the title.
  Evidence: Basic does not identify its ring width.

- [x] `circular_progress-02` · P2 · proposal. Animated naming: Display the cycle duration in its caption.
  Evidence: Animated shows 35% but no update interval.

- [x] `circular_progress-03` · P2 · proposal. Thick ring: Show its stroke width in pixels.
  Evidence: Thick Ring gives no numerical thickness.

- [x] `circular_progress-04` · P2 · proposal. Thin ring: Replace the repeated caption with its stroke width.
  Evidence: Thin Ring repeats its heading in the description.

- [x] `circular_progress-05` · P2 · proposal. Basic subtitle: State the configured progress and start angle.
  Evidence: Static value repeats what the absence of motion already suggests.

- [x] `circular_progress-06` · P2 · proposal. Top row comparison: Add a matched-value reference for thickness comparison.
  Evidence: All four rings use different values and colors.

- [x] `circular_progress-07` · P2 · proposal. Start direction: Add a small shared clockwise-from-top legend.
  Evidence: The arcs have no stated origin or direction.

- [x] `circular_progress-08` · P2 · proposal. Animated phase: Add a live status and elapsed-time label.
  Evidence: 35% has no timestamp or phase context.

- [x] `circular_progress-09` · P2 · proposal. Live controls: Add a compact Pause action outside the ring.
  Evidence: No pause control is visible for the animated sample.

- [x] `circular_progress-10` · P2 · proposal. Track contrast: Increase track contrast modestly while preserving example colors.
  Evidence: Unfilled tracks are close to the card background.

- [x] `circular_progress-11` · P2 · proposal. Thin track: Add a magnified thin-ring inset.
  Evidence: The thin ring's unfilled remainder is difficult to locate.

- [x] `circular_progress-12` · P2 · proposal. Percent placement: Bring each value nearer its ring or center it inside.
  Evidence: Percent values sit well below the rings.

- [x] `circular_progress-13` · P2 · proposal. Size card heights: Use equal-height containers while retaining ring sizes.
  Evidence: Five size-variation cards have different bottom edges.

- [x] `circular_progress-14` · P2 · proposal. Size ring alignment: Align centers on a shared horizontal baseline.
  Evidence: Ring centers move downward as diameter increases.

- [x] `circular_progress-15` · P2 · proposal. 28px value: Display its progress outside the ring.
  Evidence: The smallest example shows 28px but no percentage.

- [x] `circular_progress-16` · P2 · proposal. 40px value: Show its value consistently with larger examples.
  Evidence: The second example also lacks a percentage.

- [x] `circular_progress-17` · P2 · proposal. Size units: Label the row Diameter.
  Evidence: 28px through 80px do not state diameter versus radius.

- [x] `circular_progress-18` · P2 · proposal. Size fill variation: Use equal percentages for the size-only comparison.
  Evidence: The size row changes progress as well as diameter.

- [x] `circular_progress-19` · P2 · proposal. Size thickness variation: Show stroke width beneath each diameter.
  Evidence: Stroke thickness also changes without a caption.

- [x] `circular_progress-20` · P2 · proposal. Completed ring: Add a small Completed caption.
  Evidence: 100% is visually a pink ring without a completion cue.

- [x] `circular_progress-21` · P2 · proposal. Size colors: Use one color for size comparison or explain the mapping.
  Evidence: Five colors lack a stated relationship to size.

- [x] `circular_progress-22` · P2 · proposal. Section gap: Align it with the card grid and reduce surrounding gap.
  Evidence: The Size Variations heading floats between rows.

- [x] `circular_progress-23` · P2 · proposal. Subtitle type: Increase it slightly or replace it with useful parameter guidance.
  Evidence: Radial progress visualization is small and muted.

- [x] `circular_progress-24` · P2 · proposal. Footer space: Add a compact parameter table for all ring specimens.
  Evidence: The bottom fifth is unused.

- [x] `circular_progress-25` · P2 · proposal. Card padding: Normalize vertical padding within the four main cards.
  Evidence: Top specimen titles sit closer to the card edge than lower captions.

### colors

[Baseline](screenshot-baselines/screens/colors_720p.png) · [Visual review](design-audit.html#screen=colors)

- [x] `colors-01` · P2 · proposal. Theme identity: Move the theme name beside the title.
  Evidence: Midnight appears only in the bottom sentence.

- [x] `colors-02` · P2 · proposal. Title color: Label it as theme chrome or use a neutral heading background.
  Evidence: The red title strip resembles the Primary specimen.

- [x] `colors-03` · P2 · proposal. Theme values: Show resolved values below each role.
  Evidence: Primary through Error/Danger omit their hex values.

- [x] `colors-04` · P2 · proposal. Surface boundary: Add an external boundary or neutral backing.
  Evidence: The Surface swatch nearly merges with its black tray.

- [x] `colors-05` · P2 · proposal. Error role: Choose one canonical role name and explain aliases separately.
  Evidence: Error/Danger combines two semantic names.
  Disposition: Kept the canonical Error role and described its purpose; the library has no Danger alias.

- [x] `colors-06` · P2 · proposal. Primary/error comparison: Add a caption distinguishing action and error uses.
  Evidence: Primary red and error coral are close in hue.

- [x] `colors-07` · P2 · proposal. Secondary value: Add its foreground contrast ratio.
  Evidence: The gray Secondary tile has no luminance or contrast context.

- [x] `colors-08` · P2 · proposal. Accent foreground: Show the resolved foreground choice.
  Evidence: The yellow Accent uses black text unlike other roles.

- [x] `colors-09` · P2 · proposal. Role ordering: Group action roles, surfaces and status colors explicitly.
  Evidence: Surface is placed between Accent and Error.

- [x] `colors-10` · P2 · proposal. Theme row heading: Use Semantic roles.
  Evidence: Theme Colors repeats the page title's meaning.

- [x] `colors-11` · P2 · proposal. Custom heading: Use Fixed custom values.
  Evidence: Custom Colors does not explain independence from theme roles.

- [x] `colors-12` · P2 · proposal. Custom values type: Use monospace text for reliable reading.
  Evidence: Hex codes use handwritten characters.

- [x] `colors-13` · P2 · proposal. Custom text alignment: Align all blocks to equal internal insets.
  Evidence: Color names and hex values form left-aligned blocks inside centered tiles.

- [x] `colors-14` · P2 · proposal. Red naming: Use a precise descriptive name while keeping the value.
  Evidence: Red is visually coral-pink.

- [x] `colors-15` · P2 · proposal. Yellow naming: Name it Amber or Warm yellow.
  Evidence: Yellow is visibly amber compared with the theme accent.

- [x] `colors-16` · P2 · proposal. Palette correspondence: Show a small equality annotation.
  Evidence: Custom Red matches Error/Danger without explanation.

- [x] `colors-17` · P2 · proposal. Custom foreground: Add the measured foreground ratio below.
  Evidence: All custom swatches use black text without a contrast label.

- [x] `colors-18` · P2 · proposal. Tray bounds: Expand tray padding so all five swatches sit within equal margins.
  Evidence: The rightmost tiles extend beyond the black inner tray.

- [x] `colors-19` · P2 · proposal. Tile spacing: Use a five-column grid with uniform gaps.
  Evidence: The horizontal gaps are inconsistent at the right edge.
  Disposition: Five fixed custom colors use five equal columns. Semantic roles use six equal columns to include Background.

- [x] `colors-20` · P2 · proposal. Header alignment: Align the title text to the swatch content.
  Evidence: The title strip and content use different horizontal insets.

- [x] `colors-21` · P2 · proposal. Section spacing: Increase the gap before the second section label.
  Evidence: Custom Colors sits close to the upper tray.

- [x] `colors-22` · P2 · proposal. Footer density: Replace it with a concise theme identifier and legend.
  Evidence: One long footer sentence repeats the row organization.

- [x] `colors-23` · P2 · proposal. Footer gap: Move reference details directly under the specimens.
  Evidence: A large empty space separates swatches from the footer.

- [x] `colors-24` · P2 · proposal. Missing background role: Add a background swatch within the theme row.
  Evidence: The gallery shows Surface but not a labeled Background specimen.

- [x] `colors-25` · P2 · proposal. Reference context: Add a small light-backing comparison strip outside the primary palette.
  Evidence: The swatches show colors only on black backing.

### composer_lab

[Baseline](screenshot-baselines/screens/composer_lab_720p.png) · [Visual review](design-audit.html#screen=composer_lab)

- [x] `composer_lab-01` · P2 · proposal. Title width: Separate the title from a short behavior subtitle.
  Evidence: Composer Lab uses a long technical sentence as its title.

- [x] `composer_lab-02` · P2 · proposal. Section 1 heading: Use a readable heading with API flags on a second line.
  Evidence: auto-grow + submit_on_enter is tiny.

- [x] `composer_lab-03` · P2 · proposal. First field help: Move persistent guidance outside the input.
  Evidence: The editable value doubles as the full instruction.

- [x] `composer_lab-04` · P2 · proposal. Send count: Show a compact submission counter beside the field.
  Evidence: Sends: 0 is buried in the first heading.

- [x] `composer_lab-05` · P2 · proposal. Submitted output: Add a labeled last-submitted preview.
  Evidence: The first example has no visible result area for sent text.

- [x] `composer_lab-06` · P2 · proposal. Row-cap context: Draw a neutral maximum-height outline behind the field.
  Evidence: Capped at 5 rows is named but not visually marked.

- [x] `composer_lab-07` · P2 · proposal. Current rows: Place current/cap row counts immediately below the first field.
  Evidence: The rows 3 diagnostic is far to the right.

- [x] `composer_lab-08` · P2 · proposal. State table type: Increase diagnostic text size.
  Evidence: Composer state keys and values are very small.

- [x] `composer_lab-09` · P2 · proposal. State alignment: Use a compact aligned two-column table.
  Evidence: Keys and values are separated by a wide empty gutter.

- [x] `composer_lab-10` · P2 · proposal. Cursor units: Label the unit explicitly.
  Evidence: Cursor 136 does not state bytes, characters or codepoints.

- [x] `composer_lab-11` · P2 · proposal. Row/column indexing: State the indexing convention.
  Evidence: 2/32 has no zero-based or one-based explanation.

- [x] `composer_lab-12` · P2 · proposal. Scroll units: Display the unit used by the field.
  Evidence: scroll y0.0 lacks units.

- [x] `composer_lab-13` · P2 · proposal. Widest measurement: Rename it Widest visual line.
  Evidence: widest 404px does not identify what width was measured.

- [x] `composer_lab-14` · P2 · proposal. Second field heading: Split behavior label from input instruction.
  Evidence: Word wrap on competes with the long Enter explanation.

- [x] `composer_lab-15` · P2 · proposal. Second field height: Add optional baseline guides to explain its fixed capacity.
  Evidence: The empty lower area has no visible row guides.

- [x] `composer_lab-16` · P2 · proposal. No-wrap fixture: Add a small Clipped by design annotation outside the field.
  Evidence: The third line clips intentionally with no external endpoint cue.

- [x] `composer_lab-17` · P2 · proposal. No-wrap hidden extent: Add a text-length or horizontal-overflow diagnostic.
  Evidence: The visible line does not show how much content continues.

- [x] `composer_lab-18` · P2 · proposal. Scroll fixture: Display Rows 10–12 of 12 beside it.
  Evidence: The fourth field shows ten, eleven, twelve without its current position.

- [x] `composer_lab-19` · P2 · proposal. Scroll cue: Add an external scroll-range indicator without changing clipping.
  Evidence: The scroll fixture has no visible scrollbar in this capture.

- [x] `composer_lab-20` · P2 · proposal. Key reference: Increase type and group movement, editing and selection.
  Evidence: Keyboard help is tiny and densely packed.

- [x] `composer_lab-21` · P2 · proposal. Platform shortcuts: Show the active platform's keys first.
  Evidence: Alt/Ctrl and Cmd/Ctrl syntax is hard to scan.

- [x] `composer_lab-22` · P2 · proposal. Mouse help: Use a compact list below keyboard help.
  Evidence: Mouse instructions wrap into two distant short lines.

- [x] `composer_lab-23` · P2 · proposal. Two-column balance: Widen the examples and diagnostic table proportionally.
  Evidence: The content occupies only the left two-thirds of the screen.

- [x] `composer_lab-24` · P2 · proposal. Section spacing: Use consistent section spacing.
  Evidence: The first-to-second field gap is much larger than later gaps.

- [x] `composer_lab-25` · P2 · proposal. Focused example label: Show the active example number beside the state heading.
  Evidence: The state table says focused yes without naming the field.

### config_gap_gallery

[Baseline](screenshot-baselines/screens/config_gap_gallery_720p.png) · [Visual review](design-audit.html#screen=config_gap_gallery)

- [x] `config_gap_gallery-01` · P2 · proposal. Page title: Use Component configuration examples.
  Evidence: Config options that had no caller describes repository history.

- [x] `config_gap_gallery-02` · P2 · proposal. Panel headings: Use readable monospace parameter labels.
  Evidence: API-heavy headings use tiny handwritten text.

- [x] `config_gap_gallery-03` · P2 · proposal. Shadow labels: Add internal padding or external captions.
  Evidence: hard and soft touch their rectangles' left edges.

- [x] `config_gap_gallery-04` · P2 · proposal. Shadow comparison: Add matched-shape controls to isolate shadow style.
  Evidence: Hard and soft also differ in fill and radius.

- [x] `config_gap_gallery-05` · P2 · proposal. Hard shadow: Show its x/y offset in a caption.
  Evidence: The offset is visible but unspecified.

- [x] `config_gap_gallery-06` · P2 · proposal. Soft shadow: Display both configured values.
  Evidence: Blur and offset are not labeled.

- [x] `config_gap_gallery-07` · P2 · proposal. Segment example: Spell out 3 corner segments and add a matched smooth sample.
  Evidence: 3 seg uses an abbreviation and no smooth reference.
  Disposition: Raylib treats requests below four segments as automatic. The specimen now accurately says 3 requested / auto and compares it with 12 segments.

- [x] `config_gap_gallery-08` · P2 · proposal. Corner radius: Show 4px, 18px and 20px under the samples.
  Evidence: The three radii differ without numeric labels.

- [x] `config_gap_gallery-09` · P2 · proposal. Clamp heading: Display min 90px and max 200px.
  Evidence: The heading promises min/max but omits the limits.

- [x] `config_gap_gallery-10` · P2 · proposal. Clamp values: Add actual resolved pixel widths.
  Evidence: 25%, 55% and 85% show requested widths only.

- [x] `config_gap_gallery-11` · P2 · proposal. Clamp state: Label Min-clamped and Max-clamped.
  Evidence: The first and last samples do not identify which bound applies.

- [x] `config_gap_gallery-12` · P2 · proposal. Clamp reference: Draw thin reference-length lines behind the specimens.
  Evidence: No unclamped widths are visible.

- [x] `config_gap_gallery-13` · P2 · proposal. Clip boundary: Outline its 76px viewport boundary externally.
  Evidence: The clipped region ends immediately under partial content.

- [x] `config_gap_gallery-14` · P2 · proposal. Clip row count: Add 4 source rows, viewport 76px.
  Evidence: Rows 0 and 1 are visible without a total count.

- [x] `config_gap_gallery-15` · P2 · proposal. Hide target: Add a small source-row diagram beside the control.
  Evidence: Hide row 2 targets a row mostly below the clip boundary.

- [x] `config_gap_gallery-16` · P2 · proposal. Marquee text: Center it or inset it within the marquee.
  Evidence: selection begins outside the dashed rectangle's inner edge.

- [x] `config_gap_gallery-17` · P2 · proposal. Marquee parameters: Display dash 10px, gap 7px outside it.
  Evidence: The dashed rectangle gives no dash or gap measurements.

- [x] `config_gap_gallery-18` · P2 · proposal. Draw-layer example: Add small labels marking background fill and foreground stroke.
  Evidence: on_draw_bg and on_draw_fg are not visually identified.

- [x] `config_gap_gallery-19` · P2 · proposal. Explicit text color: Repeat its exact text in a readable external caption.
  Evidence: The deliberately dark red sentence is difficult to read.

- [x] `config_gap_gallery-20` · P2 · proposal. Text inset comparison: Add pixel guides and measured inset readouts to expose the comparison.
  Evidence: inset 24px and inset 2px appear almost equally flush left.

- [x] `config_gap_gallery-21` · P2 · proposal. Wrap example: Add a narrow paired specimen that visibly wraps.
  Evidence: The wrap-enabled sentence remains one line in the screenshot.

- [x] `config_gap_gallery-22` · P2 · proposal. Activation buttons: Use On pointer down and On pointer up captions.
  Evidence: on Press and on Release do not name pointer timing.

- [x] `config_gap_gallery-23` · P2 · proposal. Counter reset: Add a small shared Reset counters control.
  Evidence: Both activation counters have no visible reset action.

- [x] `config_gap_gallery-24` · P2 · proposal. Measurement output: Show the measured text specimen and expected sizing rule alongside.
  Evidence: measure_config says 0x0 has no visible probe or expectation.

- [x] `config_gap_gallery-25` · P2 · proposal. Focus-selection field: Add an external Select all on focus caption.
  Evidence: focus me, all selected is the only explanation of a behavior.
  Disposition: The native text field selects all on keyboard focus. Its caption promises Tab selection; tests expose that pointer refocus can also select all. That focus-origin bug and the conflicting generic SelectOnFocus behavior are tracked upstream.

### context_menu_lab

[Baseline](screenshot-baselines/screens/context_menu_lab_720p.png) · [Visual review](design-audit.html#screen=context_menu_lab)

- [x] `context_menu_lab-01` · P1 · defect. Stage file label: Inset menu text clear of its selection outline.
  Evidence: The open menu shows tage file with the initial S obscured by the selection border.

- [x] `context_menu_lab-02` · P2 · proposal. Menu padding: Add consistent horizontal item padding.
  Evidence: Unstage and Copy path begin at the menu's left boundary.

- [x] `context_menu_lab-03` · P2 · proposal. Menu target: Show the target filename in a small menu header.
  Evidence: The open menu obscures rows without identifying its target file.

- [x] `context_menu_lab-04` · P2 · proposal. Row selection: Highlight the context-menu target row.
  Evidence: No distinct selected-file treatment ties the menu to a row.

- [x] `context_menu_lab-05` · P2 · proposal. Menu action relationship: Make applicability clear for the target's current staged state.
  Evidence: Stage file and Unstage are both visually prominent.

- [x] `context_menu_lab-06` · P2 · proposal. Disabled action: Add an external note explaining why it is unavailable for this fixture.
  Evidence: Discard changes is intentionally dim but unexplained.

- [x] `context_menu_lab-07` · P2 · proposal. Copy shortcut: Increase shortcut contrast and right inset.
  Evidence: Cmd+C is dimmer than Copy path and crowded against the right edge.

- [x] `context_menu_lab-08` · P2 · proposal. Menu grouping: Use a clear separator between repository and utility actions.
  Evidence: Stage and Unstage are separated from Copy path only by a narrow gap.

- [x] `context_menu_lab-09` · P2 · proposal. Menu outline: Increase edge separation or add a subtle shadow.
  Evidence: The menu border is faint against the file rows.

- [x] `context_menu_lab-10` · P2 · proposal. Menu row heights: Use consistent padded menu rows.
  Evidence: The four items have little breathing room around their text.

- [x] `context_menu_lab-11` · P2 · proposal. File list headings: Add File and State headers.
  Evidence: Files and statuses have no column labels.

- [x] `context_menu_lab-12` · P2 · proposal. Status type: Increase status-label size.
  Evidence: modified, staged and untracked are tiny.

- [x] `context_menu_lab-13` · P2 · proposal. State recognition: Use small labeled status chips.
  Evidence: All statuses use similar muted text.

- [x] `context_menu_lab-14` · P2 · proposal. README state: Show Clean if the blank means unchanged.
  Evidence: README.md has no status while other rows do.

- [x] `context_menu_lab-15` · P2 · proposal. Filename type: Use a readable monospace filename face.
  Evidence: Paths use the decorative handwritten font.

- [x] `context_menu_lab-16` · P2 · proposal. File rows: Tighten the list for a clearer file-browser pattern.
  Evidence: Six rounded rectangles have large inter-row gaps.

- [x] `context_menu_lab-17` · P2 · proposal. Title wording: Use Context menus plus a short subtitle.
  Evidence: The title packs the point-anchoring rule into its name.

- [x] `context_menu_lab-18` · P2 · proposal. Top instruction: Change it to Right-click any file row.
  Evidence: right-click either row contradicts the six visible file rows.

- [x] `context_menu_lab-19` · P2 · proposal. Explanation heading: Use Point anchoring.
  Evidence: How this differs from a menu bar is longer than necessary.

- [x] `context_menu_lab-20` · P2 · proposal. Explanation body: Use a compact readable paragraph with a diagram.
  Evidence: Five small widely spaced lines spread across a large card.

- [x] `context_menu_lab-21` · P2 · proposal. Cursor anchor: Add an optional crosshair at the invocation point.
  Evidence: The shown menu has no marked anchor point.

- [x] `context_menu_lab-22` · P2 · proposal. First test button: Label it Open at pointer.
  Evidence: right-click me lacks the purpose of its test case.

- [x] `context_menu_lab-23` · P2 · proposal. Lower test button: Label it Bottom-edge flip test.
  Evidence: right-click me too includes a long parenthetical.

- [x] `context_menu_lab-24` · P2 · proposal. Lower caption: Place it directly above that button.
  Evidence: The bottom explanation is tiny and distant from its test button.

- [x] `context_menu_lab-25` · P2 · proposal. Empty center: Add a small placement diagram showing below versus flipped above.
  Evidence: A large unused middle region separates the test cases.

### cozy_cafe

[Baseline](screenshot-baselines/screens/cozy_cafe_720p.png) · [Visual review](design-audit.html#screen=cozy_cafe)

- [x] `cozy_cafe-01` · P2 · proposal. Outlined title: Strengthen the outline or fill for easier reading.
  Evidence: Blossom Cafe uses a very thin dark outline around pale letters.

- [x] `cozy_cafe-02` · P2 · proposal. Gold label: Use one currency convention and an icon.
  Evidence: Gold: $1347 mixes a fantasy currency name with a dollar symbol.

- [x] `cozy_cafe-03` · P2 · proposal. Gold number: Use solid tabular digits for the balance.
  Evidence: The outlined digits are harder to read than neighboring text.

- [x] `cozy_cafe-04` · P2 · proposal. Rating stars: Increase the distinction between earned and unearned stars.
  Evidence: Filled and empty stars differ subtly.

- [x] `cozy_cafe-05` · P2 · proposal. Rating value: Add a small 4/5 value.
  Evidence: Only stars communicate the numeric rating.

- [x] `cozy_cafe-06` · P2 · proposal. Customers served: Increase inset or put the count on its own quieter line.
  Evidence: Customers Served: 23 is tight within the rating box.

- [x] `cozy_cafe-07` · P2 · proposal. Special selection: Add a small Selected marker.
  Evidence: Lavender Latte differs mainly by border weight.

- [x] `cozy_cafe-08` · P2 · proposal. Special names: Use stronger solid brown text or a thicker outline.
  Evidence: Outlined white lettering is delicate on pastel fills.

- [x] `cozy_cafe-09` · P2 · proposal. Special economics: Add a concise value caption per item.
  Evidence: No prices or benefits accompany the three specials.
  Disposition: Promotion grants 25 coins immediately for every special. Captions state that actual value instead of inventing different prices.

- [x] `cozy_cafe-10` · P2 · proposal. Promote action: Include Lavender Latte in a small action caption.
  Evidence: Promote Special does not name the selected special.

- [x] `cozy_cafe-11` · P2 · proposal. Promote clock: Show the promotion duration beside it.
  Evidence: The clock icon does not state cost or duration.
  Disposition: The existing promotion has no timer or cost. Replaced the misleading clock with an explicit +25 coins instantly / no cost caption.

- [x] `cozy_cafe-12` · P2 · proposal. Customer title: Add a 2 waiting count.
  Evidence: Customers gives no queue length.

- [x] `cozy_cafe-13` · P2 · proposal. Order text: Separate customer name from the order description.
  Evidence: Emma - Cappuccio puts name and order on one long line.

- [x] `cozy_cafe-14` · P2 · proposal. Customer rows: Add a subtle separator or distinct row spacing.
  Evidence: Emma and Oliver have no visual divider.

- [x] `cozy_cafe-15` · P2 · proposal. Wait times: Label the badges Waited or Remaining.
  Evidence: 2m and 5m do not say whether they are elapsed or remaining.

- [x] `cozy_cafe-16` · P2 · proposal. Patience labels: Increase their text size.
  Evidence: Patience and Patience: LOW are very small.

- [x] `cozy_cafe-17` · P2 · proposal. Low patience: Add a clear low-patience icon or badge.
  Evidence: The red warning relies on tiny colored text.

- [x] `cozy_cafe-18` · P2 · proposal. Patience bars: Increase their height and show a numeric or verbal status.
  Evidence: Bars are narrow and visually weak.

- [x] `cozy_cafe-19` · P2 · proposal. Serve target: Show Serve Emma if queue order is fixed.
  Evidence: Serve Next does not reveal which customer will be served.

- [x] `cozy_cafe-20` · P2 · proposal. Urgency ordering: Use a modest urgency accent on Oliver's row.
  Evidence: Oliver is low on patience but remains visually secondary.

- [x] `cozy_cafe-21` · P2 · proposal. Music layout: Place Music 70% in the same control row.
  Evidence: The slider appears above its label rather than beside it.

- [x] `cozy_cafe-22` · P2 · proposal. Music track: Increase filled/unfilled distinction.
  Evidence: The brown unfilled track blends into the warm outline.

- [x] `cozy_cafe-23` · P2 · proposal. Chat hierarchy: Add a small Guild chat label.
  Evidence: Two chat messages share one heavy brown block without a header.

- [x] `cozy_cafe-24` · P2 · proposal. Research icon: Use a book, flask or research-related symbol.
  Evidence: A gift icon is labeled Research.

- [x] `cozy_cafe-25` · P2 · proposal. Tool badges: Enlarge badges slightly and explain their counts in tool captions.
  Evidence: Inventory 2 and Research! badges are tiny at button corners.

### deadspace_settings

[Baseline](screenshot-baselines/screens/deadspace_settings_720p.png) · [Visual review](design-audit.html#screen=deadspace_settings)

- [x] `deadspace_settings-01` · P2 · proposal. Front placement: Shift the active stack toward center or reserve the right area for category details.
  Evidence: The active menu occupies the left half with a large empty right region.

- [x] `deadspace_settings-02` · P2 · proposal. More settings title: Increase its spacing and prominence modestly.
  Evidence: MORE SETTINGS is small relative to the wide active panel.

- [x] `deadspace_settings-03` · P2 · proposal. Settings header: Use it as a breadcrumb identifying the parent menu.
  Evidence: // SETTINGS repeats the title without navigation context.

- [x] `deadspace_settings-04` · P2 · proposal. Active gameplay: Add a slim selection chevron.
  Evidence: Gameplay uses brighter fill and outline but no shape marker.

- [x] `deadspace_settings-05` · P2 · proposal. Active typography: Give the selected label a slight weight increase.
  Evidence: Gameplay text has similar weight to inactive rows.

- [x] `deadspace_settings-06` · P2 · proposal. Scanline intensity: Reduce line opacity immediately behind text while preserving the effect.
  Evidence: Horizontal lines run through every label.

- [x] `deadspace_settings-07` · P2 · proposal. Text edge treatment: Tighten the glow around letter edges.
  Evidence: Cyan fringe and thin letters reduce clarity.

- [x] `deadspace_settings-08` · P2 · proposal. Row interiors: Increase left inset beyond the clipped corners.
  Evidence: Labels sit near the left bevel transitions.

- [x] `deadspace_settings-09` · P2 · proposal. Front header gap: Use a deliberate shared header spacing.
  Evidence: The heading and Controls row have a larger gap than other rows.

- [x] `deadspace_settings-10` · P2 · proposal. Row rhythm: Separate legal/about items from gameplay preferences.
  Evidence: Eight identical rows provide no category grouping.

- [x] `deadspace_settings-11` · P2 · proposal. Policies label: Place it in a quieter secondary group.
  Evidence: Policies and Licenses is long and visually equal to core settings.

- [x] `deadspace_settings-12` · P2 · proposal. Credits placement: Add a small gap before the footer.
  Evidence: Credits sits directly above the footer with little distinction.

- [x] `deadspace_settings-13` · P2 · proposal. Rear menu prominence: Reduce rear text emphasis a little further.
  Evidence: Initial Settings remains detailed enough to compete with the active stack.

- [x] `deadspace_settings-14` · P2 · proposal. Rear active row: Use a connector or breadcrumb cue to explain the relationship.
  Evidence: More Settings extends out from behind the front stack.

- [x] `deadspace_settings-15` · P2 · proposal. Rear title: Increase its left safe margin.
  Evidence: INITIAL SETTINGS sits close to the screen's left edge.

- [x] `deadspace_settings-16` · P2 · proposal. Overlapping footer: Offset the rear panel vertically to reduce dense intersections.
  Evidence: The front footer overlays the rear active-row region.

- [x] `deadspace_settings-17` · P2 · proposal. Footer key labels: Render Enter and Esc as distinct keycaps.
  Evidence: Enter SELECT and Esc BACK are plain text with similar weight.

- [x] `deadspace_settings-18` · P2 · proposal. Footer spacing: Give the two actions consistent grouped spacing.
  Evidence: Select and Back cluster at the right end.

- [x] `deadspace_settings-19` · P2 · proposal. Footer alignment: Align its outer width with the main panel.
  Evidence: The footer extends slightly beyond the active row stack.

- [x] `deadspace_settings-20` · P2 · proposal. Menu depth: Add restrained perspective or skew matching the mock.
  Evidence: The baseline stacks are flatter than the mock's skewed holographic layers.
  Disposition: Added restrained slanted panel plates within their bounds, preserving readable flat text and the layered menu. Full perspective typography is not implemented.

- [x] `deadspace_settings-21` · P2 · proposal. Selected description: Show a one-sentence category preview.
  Evidence: Gameplay has no descriptive content in the empty right area.

- [x] `deadspace_settings-22` · P2 · proposal. Navigation context: Add Initial settings > More settings as a compact breadcrumb.
  Evidence: No visible cue states this is a submenu.

- [x] `deadspace_settings-23` · P2 · proposal. Category symbols: Add restrained monochrome category symbols if consistent with the reference.
  Evidence: Long text rows require reading every item.
  Disposition: The reference is icon-free, so category icons were omitted. A selection chevron supplies a noncolor cue.

- [x] `deadspace_settings-24` · P2 · proposal. Background grid: Fade its edges more gradually.
  Evidence: The faint grid ends abruptly behind the active panel region.

- [x] `deadspace_settings-25` · P2 · proposal. Scale balance: Increase active-menu scale slightly while retaining the layered layout.
  Evidence: Large black margins dominate the composition at 720p.

### decorative_frame

[Baseline](screenshot-baselines/screens/decorative_frame_720p.png) · [Visual review](design-audit.html#screen=decorative_frame)

- [x] `decorative_frame-01` · P2 · proposal. Subtitle claim: Use identical card content for a controlled comparison.
  Evidence: The same card is shown with three different titles and body texts.

- [x] `decorative_frame-02` · P2 · proposal. Frame labels: Use Kraft paper and place the API name below.
  Evidence: KraftPaper is a code-style identifier beside ordinary Simple and Inset labels.

- [x] `decorative_frame-03` · P2 · proposal. Caption type: Increase caption size.
  Evidence: Layered + corners and other captions are tiny.

- [x] `decorative_frame-04` · P2 · proposal. Card headings: Use the same heading to compare frames fairly.
  Evidence: Field Notes, Tide Table and Plate 14 vary in apparent line length.

- [x] `decorative_frame-05` · P2 · proposal. Body text: Increase body type slightly.
  Evidence: The small handwritten body is difficult to inspect from a distance.

- [x] `decorative_frame-06` · P2 · proposal. Body alignment: Use consistent left-aligned content inset.
  Evidence: All body text is centered despite short prose fragments.

- [x] `decorative_frame-07` · P2 · proposal. Kraft corner detail: Add a small annotated corner enlargement.
  Evidence: Four dark corner tabs have no explanation.

- [x] `decorative_frame-08` · P2 · proposal. Kraft layers: Show frame-band and inner-padding measurements.
  Evidence: The layered border has no thickness labels.

- [x] `decorative_frame-09` · P2 · proposal. Simple border: Add an enlarged reference corner outside the specimen.
  Evidence: A very thin blue outline is hard to inspect.

- [x] `decorative_frame-10` · P2 · proposal. Inset lighting: Add a subtle top-left lighting arrow.
  Evidence: The gray border's light direction is unnamed.

- [x] `decorative_frame-11` · P2 · proposal. Inset shadow: Show the configured inset thickness.
  Evidence: Sunken shadow does not state the shadow dimensions.

- [x] `decorative_frame-12` · P2 · proposal. Frame dimensions: Print shared outer dimensions above the row.
  Evidence: The cards appear similar in size but dimensions are unstated.

- [x] `decorative_frame-13` · P2 · proposal. Content padding: Add content-bound guides to compare usable space.
  Evidence: Different frame widths leave different visual insets.

- [x] `decorative_frame-14` · P2 · proposal. Frame colors: Add a neutral-color comparison strip.
  Evidence: Brown, blue and gray alter more than border treatment.

- [x] `decorative_frame-15` · P2 · proposal. Caption alignment: Use equal caption blocks with consistent gap.
  Evidence: Frame names sit close to the bottoms of large cards.

- [x] `decorative_frame-16` · P2 · proposal. Card proportion: Reduce card height or add more representative content.
  Evidence: Large empty interiors surround two-line texts.

- [x] `decorative_frame-17` · P2 · proposal. Footer language: State concrete use cases for each frame.
  Evidence: Pick by weight uses subjective guidance.

- [x] `decorative_frame-18` · P2 · proposal. Footer typography: Increase size and divide guidance by frame.
  Evidence: The recommendation is one small line across a wide bar.

- [x] `decorative_frame-19` · P2 · proposal. Footer border: Use a quieter note treatment.
  Evidence: The recommendation itself has a decorative frame that competes with samples.

- [x] `decorative_frame-20` · P2 · proposal. Lower whitespace: Use it for a compact parameter legend.
  Evidence: A large empty band follows the recommendation.

### decorators

[Baseline](screenshot-baselines/screens/decorators_720p.png) · [Visual review](design-audit.html#screen=decorators)

- [x] `decorators-01` · P2 · defect. Green status brackets: Anchor the brackets to the visible ONLINE panel bounds.
  Evidence: Green corner brackets extend well beyond the ONLINE panel and around its caption.

- [x] `decorators-02` · P2 · defect. First quote padding: Add a consistent left content inset.
  Evidence: The first quote starts almost against the thick teal accent bar.

- [x] `decorators-03` · P2 · proposal. Page title: Increase title size modestly.
  Evidence: UI Decorators is small relative to the unused screen area.

- [x] `decorators-04` · P2 · proposal. Subtitle type: Use a readable subtitle with monospace API text.
  Evidence: Composable visual effects via .decorate() is tiny.

- [x] `decorators-05` · P2 · proposal. Column headings: Add human-readable headings above the API names.
  Evidence: with_brackets(), with_grid_bg() and with_quote() are small and low-emphasis.

- [x] `decorators-06` · P2 · proposal. Bracket dimensions: Show bracket length and stroke width in captions.
  Evidence: The cyan and amber brackets have no length annotation.

- [x] `decorators-07` · P2 · proposal. Status title: Increase title size or reduce panel height.
  Evidence: SYSTEM STATUS is small inside a large panel.

- [x] `decorators-08` · P2 · proposal. Status message: Increase body text size.
  Evidence: All systems operational uses very tiny lettering.

- [x] `decorators-09` · P2 · proposal. Warning text: Enlarge it and separate the percentage.
  Evidence: Hull integrity at 47% is smaller than the available panel allows.

- [x] `decorators-10` · P2 · proposal. Warning punctuation: Use a distinct warning symbol.
  Evidence: ! WARNING ! uses text punctuation as the icon.

- [x] `decorators-11` · P2 · proposal. Online marker: Increase it slightly and align its center with the label.
  Evidence: The green dot is tiny relative to its status word.

- [x] `decorators-12` · P2 · proposal. Bracket caption: Move it outside specimen bounds after correcting the anchor.
  Evidence: The caption sits inside the overextended green brackets.

- [x] `decorators-13` · P2 · proposal. Grid labels: Label them Coarse 20px and Dense grid with actual spacing.
  Evidence: The first two grids have no individual names.

- [x] `decorators-14` · P2 · proposal. Grid dot: Label it as the center marker.
  Evidence: The teal center dot has no stated role.

- [x] `decorators-15` · P2 · proposal. Dense grid: Add a small spacing ruler outside the panel.
  Evidence: Dense lines have no scale reference.

- [x] `decorators-16` · P2 · proposal. Sector grid: Caption it Grid + brackets.
  Evidence: SECTOR 7-G has no explanation of combined decorators.

- [x] `decorators-17` · P2 · proposal. Grid contrast: Show line opacity values for comparison.
  Evidence: The three grid specimens vary in brightness without numeric captions.

- [x] `decorators-18` · P2 · proposal. Quote examples: Label it No attribution to clarify the variant.
  Evidence: The first quote lacks attribution while the others include it.

- [x] `decorators-19` · P2 · proposal. First quote wrapping: Adjust content width or text size for a balanced wrap.
  Evidence: The line breaks after you and leaves do. on a short second line.

- [x] `decorators-20` · P2 · proposal. Quote attribution: Increase attribution size while keeping it secondary.
  Evidence: Commander Shepard and GLaDOS labels are tiny.

- [x] `decorators-21` · P2 · proposal. Quote bar widths: Show their configured widths.
  Evidence: Teal, amber and green accent bars vary markedly.

- [x] `decorators-22` · P2 · proposal. Quote alignment: Use a shared content inset across examples.
  Evidence: The first quote starts farther left than the later quotes.

- [x] `decorators-23` · P2 · proposal. Column height: Use aligned example rows with separate captions.
  Evidence: The three specimen stacks end on different baselines.

- [x] `decorators-24` · P2 · proposal. Lower blank space: Add annotated decorator-parameter references.
  Evidence: Nearly the lower third is empty.

- [x] `decorators-25` · P2 · proposal. Color mapping: Add captions separating semantic color from decorator behavior.
  Evidence: Teal, amber and green denote both status and different examples.

### dialog_confirm

[Baseline](screenshot-baselines/screens/dialog_confirm_720p.png) · [Visual review](design-audit.html#screen=dialog_confirm)

- [x] `dialog_confirm-01` · P2 · proposal. Dialog title: Use Apply display changes?.
  Evidence: Apply changes? does not name the affected settings.

- [x] `dialog_confirm-02` · P2 · proposal. Change summary: Show a compact old-to-new summary.
  Evidence: The dialog describes new settings without listing changed values.

- [x] `dialog_confirm-03` · P2 · proposal. Restart wording: Describe the visible display interruption.
  Evidence: Restart the renderer uses implementation terminology.

- [x] `dialog_confirm-04` · P2 · proposal. Recovery wording: Name the Display settings path.
  Evidence: Revert from the settings menu afterwards gives no precise route.

- [x] `dialog_confirm-05` · P2 · proposal. Display recovery: Offer an explicit timed-revert design for unsupported display changes.
  Evidence: The message assumes the user can still see the screen after applying.

- [x] `dialog_confirm-06` · P2 · proposal. Body length: Use a standard body face and shorter sentences.
  Evidence: The explanation wraps across three widely tracked lines.

- [x] `dialog_confirm-07` · P2 · proposal. Body spacing: Reduce the title-to-body spacing.
  Evidence: The title and message are separated by a large gap.

- [x] `dialog_confirm-08` · P2 · proposal. Button size: Increase action-label size.
  Evidence: Cancel and Apply labels are small within their buttons.

- [x] `dialog_confirm-09` · P2 · proposal. Apply label: Use Apply display settings.
  Evidence: Apply repeats the vague title verb.

- [x] `dialog_confirm-10` · P2 · proposal. Cancel meaning: Use Keep current settings where space permits.
  Evidence: Cancel does not state that current settings remain.

- [x] `dialog_confirm-11` · P2 · proposal. Button alignment: Align its right edge with the body text column.
  Evidence: The action row is inset farther than the message text.

- [x] `dialog_confirm-12` · P2 · proposal. Button gap: Use a consistent compact action-group gap.
  Evidence: Cancel and Apply are separated by a large empty gap.

- [x] `dialog_confirm-13` · P2 · proposal. Dialog bottom: Reduce dialog height to fit the content.
  Evidence: Substantial blank space follows the buttons.

- [x] `dialog_confirm-14` · P2 · proposal. Dialog edge: Add subtle edge separation.
  Evidence: The dark panel has no clear border or shadow against the backdrop.

- [x] `dialog_confirm-15` · P2 · proposal. Current context: Mark the pending fields in the backdrop before dimming.
  Evidence: Visible 2560×1440 and 120Hz values do not identify pending versus current.

- [x] `dialog_confirm-16` · P2 · proposal. Changed-field cue: Add a small changed marker to edited settings.
  Evidence: All backdrop rows share the same appearance.

- [x] `dialog_confirm-17` · P2 · proposal. Modal heading hierarchy: Increase heading weight.
  Evidence: The dialog title is only modestly larger than body text.

- [x] `dialog_confirm-18` · P2 · proposal. Backdrop chrome: Add a Display changes caption within the dialog.
  Evidence: Harbour Studio and navigation remain visible but do not identify the confirmation source.

### dialog_danger

[Baseline](screenshot-baselines/screens/dialog_danger_720p.png) · [Visual review](design-audit.html#screen=dialog_danger)

- [x] `dialog_danger-01` · P2 · proposal. Save title: Use the title Delete Chapter 3 autosave?
  Evidence: Delete save? omits the save's identity.

- [x] `dialog_danger-02` · P2 · proposal. Save name wrapping: Place the save name on its own line.
  Evidence: Chapter 3 - Autosave is split across body lines.

- [x] `dialog_danger-03` · P2 · proposal. Save metadata: Add timestamp and playtime for verification.
  Evidence: Only the save name identifies the destructive target.

- [x] `dialog_danger-04` · P2 · proposal. Save preview: Add a small slot thumbnail or chapter emblem.
  Evidence: The dialog has no visual identifier for the save slot.

- [x] `dialog_danger-05` · P2 · proposal. Warning hierarchy: Emphasize the save name and irreversible consequence separately.
  Evidence: Permanent deletion and the object name use equal weight.

- [x] `dialog_danger-06` · P2 · proposal. Warning redundancy: Use one direct irreversible-action sentence.
  Evidence: Permanently deletes and cannot be undone repeat the same point.

- [x] `dialog_danger-07` · P2 · proposal. Delete label: Use Delete save.
  Evidence: Delete is generic.

- [x] `dialog_danger-08` · P2 · proposal. Cancel label: Use Keep save.
  Evidence: Cancel is less explicit than the destructive alternative.

- [x] `dialog_danger-09` · P2 · proposal. Danger icon: Add a restrained warning icon beside the title.
  Evidence: Only the button color marks the destructive action visually.

- [x] `dialog_danger-10` · P2 · proposal. Body font: Use a readable body font for the target identifier.
  Evidence: The save name uses handwritten characters.

- [x] `dialog_danger-11` · P2 · proposal. Title gap: Tighten the vertical rhythm.
  Evidence: A large gap separates title from the warning.

- [x] `dialog_danger-12` · P2 · proposal. Button text: Increase label size.
  Evidence: Action labels are small within 132px buttons.

- [x] `dialog_danger-13` · P2 · proposal. Action spacing: Use a standard action-row gap.
  Evidence: Cancel and Delete have a wide gap without a grouping cue.

- [x] `dialog_danger-14` · P2 · proposal. Bottom padding: Reduce panel height.
  Evidence: Large empty space follows the action row.

- [x] `dialog_danger-15` · P2 · proposal. Dialog boundary: Add a subtle border or shadow.
  Evidence: The panel blends into the dark display rows behind it.

- [x] `dialog_danger-16` · P2 · proposal. Backdrop section: Use a small save-slot list behind this fixture.
  Evidence: Save slots appears above Resolution and Refresh rate settings.

- [x] `dialog_danger-17` · P2 · proposal. Sidebar context: Use a Saves navigation item for this demonstration.
  Evidence: Display remains selected behind the save deletion dialog.

- [x] `dialog_danger-18` · P2 · proposal. Selection context: Highlight the target save slot behind the scrim.
  Evidence: No visible background item corresponds to Chapter 3 autosave.

### dialog_fyi

[Baseline](screenshot-baselines/screens/dialog_fyi_720p.png) · [Visual review](design-audit.html#screen=dialog_fyi)

- [x] `dialog_fyi-01` · P2 · proposal. Document identity: Show the document name prominently.
  Evidence: The warning refers only to this document.

- [x] `dialog_fyi-02` · P2 · proposal. Title question: Use Save changes before leaving?.
  Evidence: Unsaved changes states a condition without naming the decision.

- [x] `dialog_fyi-03` · P2 · proposal. Trigger context: Name the pending action.
  Evidence: The text does not identify whether closing, switching or quitting caused the prompt.

- [x] `dialog_fyi-04` · P2 · proposal. Body verbosity: Replace it with the consequence of leaving.
  Evidence: What would you like to do? adds length without decision information.

- [x] `dialog_fyi-05` · P2 · proposal. Save label: Use Save and close when that is the demonstrated flow.
  Evidence: Save does not say whether it also completes the pending action.

- [x] `dialog_fyi-06` · P2 · proposal. Keep editing: Keep it secondary and add a concise return-to-editor cue.
  Evidence: Keep editing clearly preserves work but is visually secondary to Save.

- [x] `dialog_fyi-07` · P2 · proposal. Discard wording: Use Discard unsaved changes or supporting text.
  Evidence: Discard changes does not mention permanent loss of unsaved edits.

- [x] `dialog_fyi-08` · P2 · proposal. Discard styling: Add a clear low-emphasis button boundary.
  Evidence: Discard changes looks like plain text rather than an action.

- [x] `dialog_fyi-09` · P2 · proposal. Destructive distinction: Use a restrained destructive accent.
  Evidence: The discard action uses the same pale text as ordinary content.

- [x] `dialog_fyi-10` · P2 · proposal. Action spacing: Use a structured row with discard separate from keep/save.
  Evidence: Three actions have uneven apparent widths and gaps.

- [x] `dialog_fyi-11` · P2 · proposal. Button labels: Increase action-label size.
  Evidence: Keep editing and Save use very small type.

- [x] `dialog_fyi-12` · P2 · proposal. Body measure: Constrain body text to a comfortable line length.
  Evidence: The sentence runs across most of a wide dialog.

- [x] `dialog_fyi-13` · P2 · proposal. Heading weight: Increase heading weight.
  Evidence: The title differs little from the body style.

- [x] `dialog_fyi-14` · P2 · proposal. Bottom space: Reduce the panel height.
  Evidence: A large blank band remains below the buttons.

- [x] `dialog_fyi-15` · P2 · proposal. Modal boundary: Add a subtle outline or shadow.
  Evidence: The dark panel has little separation from the dimmed background.

- [x] `dialog_fyi-16` · P2 · proposal. Backdrop content: Use a document-editor preview behind the fixture.
  Evidence: Editor is shown above display-setting rows.

- [x] `dialog_fyi-17` · P2 · proposal. Backdrop navigation: Use a matching Documents or Editor navigation item.
  Evidence: Display remains selected for an editor warning.

- [x] `dialog_fyi-18` · P2 · proposal. Unsaved cue: Add an unsaved marker to the previewed document.
  Evidence: No background document tab or dirty marker identifies pending edits.

### dialog_info

[Baseline](screenshot-baselines/screens/dialog_info_720p.png) · [Visual review](design-audit.html#screen=dialog_info)

- [x] `dialog_info-01` · P2 · proposal. Action label: Use Return to main menu.
  Evidence: OK does not name the destination described in the message.

- [x] `dialog_info-02` · P2 · proposal. Action timing: State whether pressing the button starts the return.
  Evidence: Returning to the main menu sounds automatic despite the acknowledgment button.

- [x] `dialog_info-03` · P2 · proposal. Connection identity: Show the session or host name when available.
  Evidence: The message names only the host.

- [x] `dialog_info-04` · P2 · proposal. Loss explanation: Use the body to explain the known consequence.
  Evidence: Disconnected from the host repeats Connection lost.

- [x] `dialog_info-05` · P2 · proposal. Progress consequence: Add a concise save/progress statement where supported.
  Evidence: The notice does not say whether match progress is retained.

- [x] `dialog_info-06` · P2 · proposal. Recovery option: If the demo supports it, add Retry beside return.
  Evidence: The visible action offers no reconnect path.
  Disposition: This offline fixture cannot reconnect. It explicitly says reconnect is unavailable and offers a working return to the demo main menu.

- [x] `dialog_info-07` · P2 · proposal. Connection symbol: Add a small disconnected-network symbol.
  Evidence: No icon distinguishes the notice from ordinary information.

- [x] `dialog_info-08` · P2 · proposal. Body spacing: Reduce title-to-body spacing.
  Evidence: Two short lines sit far below the title.

- [x] `dialog_info-09` · P2 · proposal. Action alignment: Align it consistently with the content column or center it deliberately.
  Evidence: The single button is offset toward the right of the panel.

- [x] `dialog_info-10` · P2 · proposal. Action type: Increase its label size.
  Evidence: OK is tiny inside the teal button.

- [x] `dialog_info-11` · P2 · proposal. Panel height: Fit its height to the content.
  Evidence: The panel has substantial empty space below the action.

- [x] `dialog_info-12` · P2 · proposal. Panel boundary: Add a subtle border or shadow.
  Evidence: The notice lacks strong edge separation from the dark backdrop.

- [x] `dialog_info-13` · P2 · proposal. Backdrop section: Use a lobby or session list behind the fixture.
  Evidence: Multiplayer appears above display settings.

- [x] `dialog_info-14` · P2 · proposal. Sidebar selection: Use a Multiplayer navigation item.
  Evidence: Display remains selected under a multiplayer notice.

- [x] `dialog_info-15` · P2 · proposal. Session context: Show its selected session row behind the scrim.
  Evidence: No visible lobby identifies the disconnected session.

- [x] `dialog_info-16` · P2 · proposal. Heading style: Use a clearer UI heading face.
  Evidence: Connection lost uses the same handwritten face as generic demo text.

### dialog_prompt

[Baseline](screenshot-baselines/screens/dialog_prompt_720p.png) · [Visual review](design-audit.html#screen=dialog_prompt)

- [x] `dialog_prompt-01` · P2 · proposal. Current file: Add a current-file caption.
  Evidence: Rename file does not show the existing name outside the editable field.

- [x] `dialog_prompt-02` · P2 · proposal. File location: Show a compact parent-folder path.
  Evidence: The prompt does not identify the file's folder.

- [x] `dialog_prompt-03` · P2 · proposal. Field label: Use the persistent label New name.
  Evidence: Enter a new name for this file is a long instruction.

- [x] `dialog_prompt-04` · P2 · proposal. Input alignment: Align the field to the content column.
  Evidence: The input begins farther right than the title and instruction.

- [x] `dialog_prompt-05` · P2 · proposal. Input width: Use a shared right alignment.
  Evidence: The field ends before the action group's right edge.

- [x] `dialog_prompt-06` · P2 · proposal. Filename typography: Use a clear filename text face.
  Evidence: untitled_map uses handwritten characters.

- [x] `dialog_prompt-07` · P2 · proposal. Selection fill: Increase selected-text foreground/background separation.
  Evidence: The selected text has a low-contrast blue highlight over purple.

- [x] `dialog_prompt-08` · P2 · proposal. Field boundary: Add a subtle input border.
  Evidence: The purple field has no distinct border against its surrounding panel.

- [x] `dialog_prompt-09` · P2 · proposal. Field color: Use a quieter field fill while preserving selection visibility.
  Evidence: Purple is stronger than the primary Rename action.

- [x] `dialog_prompt-10` · P2 · proposal. Extension handling: Show the preserved extension separately if the fixture represents a file with one.
  Evidence: The visible name has no extension context.

- [x] `dialog_prompt-11` · P2 · proposal. Name constraints: Add concise supported-name guidance below the input.
  Evidence: No filename rules are displayed.

- [x] `dialog_prompt-12` · P2 · proposal. Duplicate context: Reserve a compact validation line without asserting a current error.
  Evidence: The prompt has no space reserved for a duplicate-name message.

- [x] `dialog_prompt-13` · P2 · proposal. Rename preview: Add a small resulting-filename preview when extension handling matters.
  Evidence: Only the editable value communicates the result.

- [x] `dialog_prompt-14` · P2 · proposal. Rename label: Increase action-label size.
  Evidence: Rename is clear but small.

- [x] `dialog_prompt-15` · P2 · proposal. Cancel placement: Align the action group to the field's right edge with a regular gap.
  Evidence: Cancel begins well inside the input's left edge.

- [x] `dialog_prompt-16` · P2 · proposal. Bottom space: Reduce dialog height.
  Evidence: The panel leaves a large empty region below the actions.

- [x] `dialog_prompt-17` · P2 · proposal. Title spacing: Tighten the vertical spacing.
  Evidence: The heading and instruction are separated by a large gap.

- [x] `dialog_prompt-18` · P2 · proposal. Backdrop content: Use a compact file list behind the fixture.
  Evidence: Files appears above Resolution and Refresh rate rows.

- [x] `dialog_prompt-19` · P2 · proposal. Backdrop navigation: Select Files in the demo navigation.
  Evidence: Display is selected despite a file rename.

- [x] `dialog_prompt-20` · P2 · proposal. Target selection: Highlight the renamed file in the backdrop.
  Evidence: No background file row matches untitled_map.

### divider_lab

[Baseline](screenshot-baselines/screens/divider_lab_720p.png) · [Visual review](design-audit.html#screen=divider_lab)

- [x] `divider_lab-01` · P2 · defect. Pane labels: Add a consistent small text inset.
  Evidence: left 300px, right fills the rest, top 150px and bottom fills the rest touch pane edges.

- [x] `divider_lab-02` · P2 · proposal. Title: Separate Divider examples from a short instruction.
  Evidence: The title combines the component name and all instructions.

- [x] `divider_lab-03` · P2 · proposal. Vertical divider: Add a centered grip marker.
  Evidence: The narrow bar has no visible grip.

- [x] `divider_lab-04` · P2 · proposal. Horizontal divider: Add a grip marker to distinguish it.
  Evidence: The horizontal bar is a plain line like the non-moving rule.

- [x] `divider_lab-05` · P2 · proposal. Divider labels: Label Width divider and Height divider outside the panes.
  Evidence: Neither draggable bar has its own caption.

- [x] `divider_lab-06` · P2 · proposal. Left dimension: Show the measured width explicitly.
  Evidence: left 300px omits whether 300 is requested or actual width.

- [x] `divider_lab-07` · P2 · proposal. Right dimension: Display the current right-pane width.
  Evidence: right fills the rest gives no resolved measurement.

- [x] `divider_lab-08` · P2 · proposal. Top dimension: Add a vertical dimension bracket.
  Evidence: top 150px omits a measurement guide.

- [x] `divider_lab-09` · P2 · proposal. Bottom dimension: Display the current bottom-pane height.
  Evidence: bottom fills the rest gives no resulting height.

- [x] `divider_lab-10` · P2 · proposal. Pane total: Add the overall container width beside the first example.
  Evidence: The total split width is not shown.

- [x] `divider_lab-11` · P2 · proposal. Height total: Add the overall height beside the second example.
  Evidence: The total split height is not shown.

- [x] `divider_lab-12` · P2 · proposal. Minimum widths: Show its configured min/max values.
  Evidence: No boundary values are visible for the vertical split.

- [x] `divider_lab-13` · P2 · proposal. Minimum heights: Show its configured height limits.
  Evidence: No range is given for the horizontal split.

- [x] `divider_lab-14` · P2 · proposal. Ratio comparison: Add the resulting percentage split beside each pixel value.
  Evidence: Only pixel measurements appear.

- [x] `divider_lab-15` · P2 · proposal. Reset control: Add Reset dividers.
  Evidence: No visible action restores 300px and 150px.

- [x] `divider_lab-16` · P2 · proposal. Drag feedback: Add a drag-delta readout beside each specimen.
  Evidence: No numeric delta is reserved near the bars.

- [x] `divider_lab-17` · P2 · proposal. Pane content: Add a short paragraph or grid that responds to resizing.
  Evidence: The large empty panes provide little reflow evidence.

- [x] `divider_lab-18` · P2 · proposal. Vertical specimen height: Reduce its height enough to fit measurement captions.
  Evidence: The first split consumes a large area for two short labels.

- [x] `divider_lab-19` · P2 · proposal. Horizontal specimen labeling: Place labels in consistent top-left positions.
  Evidence: Top and bottom text float at vertical centers.

- [x] `divider_lab-20` · P2 · proposal. Rule caption: Increase its size and use monospace for divider().
  Evidence: The bottom API explanation is tiny.

- [x] `divider_lab-21` · P2 · proposal. Rule semantics: Label it Static 1px separator.
  Evidence: Movement ignored describes implementation rather than visual purpose.

- [x] `divider_lab-22` · P2 · proposal. Rule boundary: Add endpoint markers outside it for inspection.
  Evidence: The thin bottom rule blends into the black background.

### drag_drop

[Baseline](screenshot-baselines/screens/drag_drop_720p.png) · [Visual review](design-audit.html#screen=drag_drop)

- [x] `drag_drop-01` · P2 · defect. Card padding: Add comfortable horizontal text padding.
  Evidence: Design mockups, API endpoints and other titles begin almost on their card borders.

- [x] `drag_drop-02` · P2 · proposal. Board title: Use a clearer heading face while retaining the casual style.
  Evidence: Sprint Board uses a large handwritten heading.

- [x] `drag_drop-03` · P2 · proposal. Instruction type: Reduce it slightly and improve hierarchy.
  Evidence: The instruction is almost as large as column headings.

- [x] `drag_drop-04` · P2 · proposal. Instruction scope: Mention reorder behavior if supported by this demo.
  Evidence: Drag cards between columns omits reordering within a column.

- [x] `drag_drop-05` · P2 · proposal. Card grip: Add a small grip on each card.
  Evidence: Cards have no visible drag-handle cue.

- [x] `drag_drop-06` · P2 · proposal. Column counts: Use compact count badges beside the titles.
  Evidence: Counts are embedded in oversized headings.

- [x] `drag_drop-07` · P2 · proposal. To Do heading: Use consistent sentence casing.
  Evidence: To Do differs in capitalization from In Progress.

- [x] `drag_drop-08` · P2 · proposal. Column boundaries: Increase boundary distinction modestly.
  Evidence: Columns and cards are close gray tones.

- [x] `drag_drop-09` · P2 · proposal. Card borders: Use a slightly stronger card border or subtle shadow.
  Evidence: Very thin outlines are hard to see.

- [x] `drag_drop-10` · P2 · proposal. Card density: Reduce card height or add useful metadata.
  Evidence: Tall cards contain only one short text line.

- [x] `drag_drop-11` · P2 · proposal. Board whitespace: Add a quiet drop-area cue at each stack's end.
  Evidence: Large empty areas remain under all three stacks.

- [x] `drag_drop-12` · P2 · proposal. Done status: Add a completion mark or quieter completed treatment.
  Evidence: Done cards look identical to unfinished cards.

- [x] `drag_drop-13` · P2 · proposal. Task type: Add a small labeled task-type tag.
  Evidence: Fix login bug is visually identical to design and documentation tasks.

- [x] `drag_drop-14` · P2 · proposal. Priority: Add a concise priority marker to representative sample cards.
  Evidence: The board has no visible priority distinction.

- [x] `drag_drop-15` · P2 · proposal. Assignees: Add compact assignee initials to enrich the realistic specimen.
  Evidence: No card shows who owns a task.

- [x] `drag_drop-16` · P2 · proposal. Task identity: Add short task IDs for tracking moves.
  Evidence: Titles are the only card identifiers.

- [x] `drag_drop-17` · P2 · proposal. Column accent: Add restrained status accents to headers.
  Evidence: All columns share the same color despite distinct states.

- [x] `drag_drop-18` · P2 · proposal. Active target explanation: Add a static annotated example or separate drag-state capture.
  Evidence: The screenshot gives no preview of valid drop locations.

- [x] `drag_drop-19` · P2 · proposal. Move feedback: Add a small last-move message below the board.
  Evidence: No visible status area is available to report a completed move.

- [x] `drag_drop-20` · P2 · proposal. Alternative action: Add a visible Move to control for an accessible alternative.
  Evidence: Only dragging is advertised.

- [x] `drag_drop-21` · P2 · proposal. Reset demo: Add Reset board.
  Evidence: There is no visible way to restore the initial arrangement.

- [x] `drag_drop-22` · P2 · proposal. Sprint context: Add a compact sample sprint identifier.
  Evidence: Sprint Board has no sprint name or dates.

- [x] `drag_drop-23` · P2 · proposal. Progress overview: Add an overall 2 of 8 completed summary.
  Evidence: 3, 3, 2 counts require mental aggregation.

- [x] `drag_drop-24` · P2 · proposal. Column spacing: Reduce gutter width modestly.
  Evidence: Wide gutters consume space that could improve card padding.

- [x] `drag_drop-25` · P2 · proposal. Outer frame: Center the board vertically or use the space for instructions and move feedback.
  Evidence: The board ends above a broad black bottom band.

### dropdowns

[Baseline](screenshot-baselines/screens/dropdowns_720p.png) · [Visual review](design-audit.html#screen=dropdowns)

- [x] `dropdowns-01` · P2 · proposal. Title inset: Add internal title padding.
  Evidence: Dropdown Showcase begins at the header's left edge.

- [x] `dropdowns-02` · P2 · proposal. Section header treatment: Use neutral headings so controls are identifiable.
  Evidence: Card headings use the same teal fill as dropdown controls.

- [x] `dropdowns-03` · P2 · proposal. Basic label: Add a persistent external label.
  Evidence: Alpha names the value without the selected category.

- [x] `dropdowns-04` · P2 · proposal. Basic chevron: Use a consistent chevron glyph.
  Evidence: The closed indicator is a literal v.

- [x] `dropdowns-05` · P2 · proposal. With Label row: Separate label and control visually.
  Evidence: Pet and Cat share one teal pill with weak separation.

- [x] `dropdowns-06` · P2 · proposal. Pet alignment: Left-align the label near its control.
  Evidence: Pet is centered in a wide label region.

- [x] `dropdowns-07` · P2 · proposal. Many options: Use Month, 12 options.
  Evidence: Many Options 12 does not name the month category.

- [x] `dropdowns-08` · P2 · proposal. Month control width: Use a shared field width across comparable examples.
  Evidence: January occupies a narrower field than its card header.

- [x] `dropdowns-09` · P2 · proposal. Scrolling explanation: State the menu's visible-row count.
  Evidence: Twelve options, so the list scrolls does not give the visible-row limit.
  Disposition: Native dropdowns render all options and have no scrolling or visible-row limit. Preserved all 12 months, labeled the behavior, and recorded the missing configuration upstream.

- [x] `dropdowns-10` · P2 · proposal. Custom marker: Show a small open/closed marker legend.
  Evidence: Option A[-] uses punctuation whose meaning is unexplained.

- [x] `dropdowns-11` · P2 · proposal. Custom variant label: Use a meaningful category label.
  Evidence: Option A is generic beside the concrete Pet and Month examples.

- [x] `dropdowns-12` · P2 · proposal. Disabled specimen: Add a readable external state caption.
  Evidence: Frozen is deliberately dim but only its heading explains availability.

- [x] `dropdowns-13` · P2 · proposal. Disabled explanation: Use Disabled example with concise supporting detail.
  Evidence: Not focusable and does not open describes internal behavior.

- [x] `dropdowns-14` · P2 · proposal. Custom style caption: List the configured style values.
  Evidence: Custom border, fill and corner radius omits values.

- [x] `dropdowns-15` · P2 · proposal. Ruby naming: Add Gemstone as its external label.
  Evidence: Ruby implies a gemstone or color but has no category label.

- [x] `dropdowns-16` · P2 · proposal. Custom comparison: Add a matched default specimen for comparison.
  Evidence: Ruby has no adjacent default-styled control.

- [x] `dropdowns-17` · P2 · proposal. Description type: Increase body text size.
  Evidence: All six descriptions are very small.

- [x] `dropdowns-18` · P2 · proposal. Description placement: Tighten the control-to-description gap.
  Evidence: Descriptions sit far below controls in tall cards.

- [x] `dropdowns-19` · P2 · proposal. Card height: Reduce height or add useful parameter captions.
  Evidence: Each card contains substantial unused lower space.

- [x] `dropdowns-20` · P2 · proposal. Card outlines: Quiet the card outline and preserve specimen emphasis.
  Evidence: Bright thin outlines compete with subtle custom control borders.

- [x] `dropdowns-21` · P2 · proposal. Footer padding: Inset it consistently.
  Evidence: The selection summary begins at its bar's edge.

- [x] `dropdowns-22` · P2 · proposal. Footer coverage: Include all active example values or label the summary scope.
  Evidence: The summary mentions Alpha, Cat and January but omits other selectable specimens.

- [x] `dropdowns-23` · P2 · proposal. Footer phrasing: Use Basic: Alpha, Pet: Cat, Month: January as separate fields.
  Evidence: A long sentence is harder to scan than labeled values.

- [x] `dropdowns-24` · P2 · proposal. Open-state evidence: Add a separate baseline with one representative menu open.
  Evidence: The gallery shows no expanded menu.

- [x] `dropdowns-25` · P2 · proposal. Menu placement context: Add a small placement diagram outside the specimens.
  Evidence: Closed controls do not reveal the intended popup direction or bounds.

### empire_tycoon

[Baseline](screenshot-baselines/screens/empire_tycoon_720p.png) · [Visual review](design-audit.html#screen=empire_tycoon)

- [x] `empire_tycoon-01` · P2 · proposal. Happiness face: Use an expression consistent with high happiness.
  Evidence: The 85% happiness gauge uses a neutral or unhappy-looking face.

- [x] `empire_tycoon-02` · P2 · proposal. Resources symbol: Use a gear or resource icon matching the mock.
  Evidence: Resources repeats a face-like center symbol.

- [x] `empire_tycoon-03` · P2 · proposal. Gauge values: Use solid readable numeric labels.
  Evidence: 85% and 60% appear in very small outlined text.

- [x] `empire_tycoon-04` · P2 · proposal. Gauge scale: Mark 0 and 100 consistently.
  Evidence: The semicircles have ticks but no endpoint labels.

- [x] `empire_tycoon-05` · P2 · proposal. Gauge fill distinction: Increase filled-versus-unfilled contrast.
  Evidence: Resources fill is close in color to its track.

- [x] `empire_tycoon-06` · P2 · proposal. Cash emphasis: Reduce its scale slightly or strengthen section headings.
  Evidence: The large balance dominates the actionable dashboard.

- [x] `empire_tycoon-07` · P2 · proposal. Currency symbol: Choose one coherent currency convention.
  Evidence: A star coin accompanies a dollar amount.

- [x] `empire_tycoon-08` · P2 · proposal. Selected department: Add an inset selection outline or check marker.
  Evidence: Rides has no strong selected marker distinct from the other colored tabs.

- [x] `empire_tycoon-09` · P2 · proposal. Department icons: Increase icon weight and optical size.
  Evidence: Thin line icons are small inside large tab buttons.

- [x] `empire_tycoon-10` · P2 · proposal. Upgrade badge: Inset it or reserve a badge corner.
  Evidence: The red exclamation overlaps the tab edge.

- [x] `empire_tycoon-11` · P2 · proposal. Production units: Use items/min or an appropriate explicit unit.
  Evidence: 5,200/min etc. do not identify units produced.

- [x] `empire_tycoon-12` · P2 · proposal. Production arrows: Add a trend caption such as versus previous minute.
  Evidence: Green up arrows lack a stated comparison period.

- [x] `empire_tycoon-13` · P2 · proposal. Production status: Increase its size and move it into a clear status row.
  Evidence: All systems running smoothly is extremely small.

- [x] `empire_tycoon-14` · P2 · proposal. Project title bars: Separate titles from progress tracks more clearly.
  Evidence: Cyan title backgrounds resemble progress fills.

- [x] `empire_tycoon-15` · P2 · proposal. Project progress: Align values consistently at each track's end.
  Evidence: 80% and 10% sit far from the bars' starting edges.

- [x] `empire_tycoon-16` · P2 · proposal. Robot subtitle: Label the rate or replace it with remaining time.
  Evidence: 3,100/min under a mascot project has no named metric.

- [x] `empire_tycoon-17` · P2 · proposal. Rollercoaster subtitle: Show remaining time or expected benefit.
  Evidence: Expand the fun is promotional rather than useful status.

- [x] `empire_tycoon-18` · P2 · proposal. Project selection: Add a selected-project marker.
  Evidence: No project is visibly selected for Prioritize.

- [x] `empire_tycoon-19` · P2 · proposal. Prioritize action: Show Prioritize rollercoaster or a target caption.
  Evidence: Prioritize does not name its target.

- [x] `empire_tycoon-20` · P2 · proposal. Speed Up action: Show its resource cost and effect before activation.
  Evidence: Speed Up gives no cost or duration.

- [x] `empire_tycoon-21` · P2 · proposal. New Project action: Give New Project or the principal action a clear hierarchy.
  Evidence: All three actions share equal yellow emphasis.

- [x] `empire_tycoon-22` · P2 · proposal. Milestone identity: Show its goal and reward.
  Evidence: Next Milestone 65% does not name the milestone.

- [x] `empire_tycoon-23` · P2 · proposal. Milestone number: Increase the numeric label or place it above.
  Evidence: 65% is tiny inside the progress bar.

- [x] `empire_tycoon-24` · P2 · proposal. Chat channels: Increase text size and distinguish channel labels.
  Evidence: GlobalChat and DevTeam messages are very small in a corner panel.

- [x] `empire_tycoon-25` · P2 · proposal. Bottom navigation: Use solid text at a readable size and equal icon spacing.
  Evidence: Shop, Settings and Leaderboard labels are tiny outlined text.

### entity_index_lab

[Baseline](screenshot-baselines/screens/entity_index_lab_720p.png) · [Visual review](design-audit.html#screen=entity_index_lab)

- [x] `entity_index_lab-01` · P2 · proposal. Patch Bay title: Add a small Secondary index demo subtitle beside Patch Bay.
  Evidence: The title names an audio tool but not the indexing demonstration.

- [x] `entity_index_lab-02` · P2 · proposal. Introductory sentence: Break the instruction and implementation explanation into separate lines.
  Evidence: The single sentence stretches across most of the screen in small spaced lettering.

- [x] `entity_index_lab-03` · P2 · proposal. Metric footer: Present Scan 6,228 and Index 24 as adjacent labeled counters.
  Evidence: The green sentence places 6228 and 24 inside prose.

- [x] `entity_index_lab-04` · P2 · proposal. Collection size: Give collection size its own neutral counter beside the comparison.
  Evidence: The collection size 1030 sits at the end of the metric sentence.

- [x] `entity_index_lab-05` · P2 · proposal. Equality statement: Place a Results match: 24 indicator with the counters.
  Evidence: The result-equivalence statement is detached below the performance line.

- [x] `entity_index_lab-06` · P3 · proposal. Footer explanation: Name Scan and Indexed lookup explicitly in the explanation.
  Evidence: Both spellings return 24 requires knowledge of two code expressions.

- [x] `entity_index_lab-07` · P2 · proposal. Mixer A card: Add a selected marker at the leading edge of Mixer A.
  Evidence: The selected device is communicated only through its blue fill.

- [x] `entity_index_lab-08` · P3 · proposal. Device count: Label the section Devices · 6.
  Evidence: DEVICES does not state the number of listed devices.

- [x] `entity_index_lab-09` · P2 · proposal. Port table title: Increase the table title and pair it with a 4 ports summary.
  Evidence: Mixer A has the same small type scale as device names.

- [x] `entity_index_lab-10` · P3 · proposal. Port slot header: Label the column Slot index to explain zero-based numbering.
  Evidence: SLOT numbers start at 0 with no explanation.

- [x] `entity_index_lab-11` · P2 · proposal. Slot column: Align the heading and all slot numbers to one inset.
  Evidence: The SLOT heading begins left of the numeric cell alignment.

- [x] `entity_index_lab-12` · P2 · proposal. Port column: Align the PORT heading with the port-name cells.
  Evidence: PORT starts noticeably left of Input L and Input R.

- [x] `entity_index_lab-13` · P2 · proposal. State column: Align the STATE heading with its cell labels.
  Evidence: STATE starts left of connected and idle.

- [x] `entity_index_lab-14` · P2 · proposal. Status encoding: Add a filled status dot for connected and an outlined dot for idle.
  Evidence: Connected is green while idle is muted italic text.

- [x] `entity_index_lab-15` · P3 · proposal. Device names: Use a secondary device-type label to distinguish names from categories.
  Evidence: Mixer A and Preamp 1 mix letter and number identifiers without explanation.

- [x] `entity_index_lab-16` · P2 · proposal. Device badges: Use a compact count badge with brighter text at the same trailing inset.
  Evidence: Each 4 ports count is pale blue and distant from the device name.

- [x] `entity_index_lab-17` · P3 · proposal. List panel: Reduce its height or add a compact total of 24 ports there.
  Evidence: The device list has about 100 pixels of empty space below Monitor.

- [x] `entity_index_lab-18` · P2 · proposal. Detail panel: Fit the table panel to its content and bring comparison counters upward.
  Evidence: Four short table rows occupy less than half the tall panel.

- [x] `entity_index_lab-19` · P3 · proposal. Panel spacing: Use a consistent 24-pixel inter-panel gap.
  Evidence: The gap between device and detail panels is wider than either panel's internal padding.

- [x] `entity_index_lab-20` · P2 · proposal. Metric hierarchy: Move the scan/index comparison directly under the table.
  Evidence: The main learning result is near the bottom, below a large empty panel region.

- [x] `entity_index_lab-21` · P3 · proposal. Port relationship: Add a small Selected device label or connector between Mixer A and its table.
  Evidence: Device and port panels have no shared visual connector.

- [x] `entity_index_lab-22` · P2 · proposal. Technical typography: Use a readable tabular face for slot indices and measurements.
  Evidence: The numeric counters and device slots use the same handwritten face as titles.

- [x] `entity_index_lab-23` · P3 · proposal. Send row: Give idle status a small neutral badge while retaining the same row geometry.
  Evidence: Send is the only idle port but looks otherwise identical to connected rows.

- [x] `entity_index_lab-24` · P3 · proposal. Comparison scale: Add proportionate scan and index bars under their counters.
  Evidence: The footer states vastly different work counts without a visual scale.

- [x] `entity_index_lab-25` · P3 · proposal. Selection instruction: Place the short instruction directly under DEVICES.
  Evidence: Pick a device appears only in the page-wide sentence.

### example_borders

[Baseline](screenshot-baselines/screens/example_borders_720p.png) · [Visual review](design-audit.html#screen=example_borders)

- [x] `example_borders-01` · P2 · proposal. Thin Medium Thick cards: Add 1 px, 3 px, and 6 px captions outside the samples.
  Evidence: The width examples show names without numeric widths.
  Disposition: Preserved native requested-width specimens and labeled their actual one-screen-pixel rendering. Configurable uniform solid width remains an upstream gap.

- [x] `example_borders-02` · P2 · proposal. Blue Cyan Orange cards: Add small color-code captions below these samples.
  Evidence: Color names are visible but exact tested values are absent.

- [x] `example_borders-03` · P2 · proposal. Width and color row: Separate the row into labeled Width and Color groups.
  Evidence: Width variants and hue variants share one undivided row.

- [x] `example_borders-04` · P2 · proposal. Rounded card: Show the configured roundness in an external caption.
  Evidence: Rounded names the effect without its radius.

- [x] `example_borders-05` · P2 · proposal. Pill card: Add a wider companion pill to demonstrate the same corner treatment on an elongated shape.
  Evidence: The pill demonstration is nearly square.
  Disposition: Retained the original 0.5 square and added a wider true capsule using 1.0. A 0.5 fraction is not a pill.

- [x] `example_borders-06` · P2 · proposal. Top Only corner card: Rename this sample Top corners only.
  Evidence: Top Only is also used in the per-side row for a different property.
  Disposition: The raw 0b1100 mask rounds the bottom fill corners in this backend. Labeled Bottom corners after checking the native signature and actual pixels; partial outlines round all four corners, tracked upstream.

- [x] `example_borders-07` · P2 · proposal. Diagonal corner card: Add a miniature corner diagram or explicit corner names.
  Evidence: Diagonal does not say which opposite corners are rounded.

- [x] `example_borders-08` · P2 · proposal. Hard Shadow card: Caption the offset and spread below the sample.
  Evidence: The shadow parameter is not identified in its label.
  Disposition: The hard-shadow API has offsets but no spread parameter. Caption reports offset 5,5 and no blur.

- [x] `example_borders-09` · P2 · proposal. Soft Shadow card: Place a neutral comparison backdrop behind the shadow examples only.
  Evidence: The soft shadow blends into the dark page without a comparison reference.

- [x] `example_borders-10` · P3 · proposal. Corner and shadow grouping: Create separate subgroup captions for corners and shadows.
  Evidence: Two different properties share the Corners & Shadows label.

- [x] `example_borders-11` · P2 · proposal. Dark Mode sample: Give it an external sample-name caption so its boundary remains the test subject.
  Evidence: Dark Mode blends into the page around its thin outline.

- [x] `example_borders-12` · P3 · proposal. Neon Glow sample: Move the descriptive label below the sample to leave the glow unobscured.
  Evidence: The glow label and glow border use the same bright cyan.

- [x] `example_borders-13` · P3 · proposal. Cozy sample: Add a subtitle naming its warm fill and soft border treatment.
  Evidence: Cozy describes a mood instead of border behavior.

- [x] `example_borders-14` · P2 · proposal. Danger and Success: Use a consistent icon-and-label pattern for both examples.
  Evidence: The two semantic samples use different punctuation prefixes.

- [x] `example_borders-15` · P2 · proposal. Outline Only sample: Move the long name into an external caption at the shared caption size.
  Evidence: Outline Only uses much smaller text than neighboring themed samples.

- [x] `example_borders-16` · P2 · proposal. Interactive Button: Add labeled static Rest, Hover, and Pressed previews beside the live control.
  Evidence: Only the button's resting state is shown.

- [x] `example_borders-17` · P2 · proposal. Interactive row: Align their sample frames while preserving each component's intrinsic dimensions.
  Evidence: The large button and nested panel have different heights and top edges.

- [x] `example_borders-18` · P2 · proposal. Nested Panel: Add a parent/child key outside the outlined panel.
  Evidence: The nested example has no indication of which borders belong to parent and children.

- [x] `example_borders-19` · P3 · proposal. Inner Card and Second: Rename it Outline child to distinguish its treatment.
  Evidence: Second gives no information about the second inner border.

- [x] `example_borders-20` · P2 · proposal. Per-Side row: Add edge-width captions outside each sample.
  Evidence: Top Only and Bottom Only omit the line widths.

- [x] `example_borders-21` · P2 · proposal. Left+Right sample: Use Left and right with a tiny two-edge diagram.
  Evidence: The label uses a compact plus sign unlike the other edge names.

- [x] `example_borders-22` · P2 · proposal. Mixed sample: Add a four-edge legend naming each color and width.
  Evidence: Three colors appear without identifying their edge assignment.

- [x] `example_borders-23` · P2 · proposal. Dotted samples: Add numeric dash and gap captions beneath the dotted examples.
  Evidence: Thin, Medium, and Thick do not expose dash size or spacing.

- [x] `example_borders-24` · P2 · proposal. Dotted row placement: Increase the bottom safe margin or fit the gallery into a scrollable frame with a visible boundary.
  Evidence: The last row sits only about 12 pixels above the bottom edge.

- [x] `example_borders-25` · P3 · proposal. Gallery subtitle: Replace it with a brief explanation that samples intentionally vary one property at a time.
  Evidence: The small pipe-separated subtitle repeats the row names.

### fighter_menu

[Baseline](screenshot-baselines/screens/fighter_menu_720p.png) · [Visual review](design-audit.html#screen=fighter_menu)

- [x] `fighter_menu-01` · P2 · proposal. MainMenu heading: Set the heading as Main Menu with visible word spacing.
  Evidence: MainMenu visually joins the two words.

- [x] `fighter_menu-02` · P2 · proposal. Heading shadow: Reduce the green offset to keep the title silhouette crisp.
  Evidence: The green offset duplicates large white letterforms.

- [x] `fighter_menu-03` · P2 · proposal. Currency counter: Format the amount as 25,000 P$.
  Evidence: 25000 P$ appears without numeric grouping.

- [x] `fighter_menu-04` · P3 · proposal. Currency panel: Simplify the panel to one top rule and align the value vertically.
  Evidence: The currency banner has two faint decorative lines above the amount.

- [x] `fighter_menu-05` · P2 · proposal. Tab labels: Increase inactive-tab label contrast while preserving the active green underline.
  Evidence: Offline, Online, and Customize are low-contrast gray on black.

- [x] `fighter_menu-06` · P2 · proposal. Selected Options tab: Use a dark label and reserve neon green for the active indicator.
  Evidence: Orange letters on a pale cyan fill compete with the neon underline.

- [x] `fighter_menu-07` · P2 · proposal. Shoulder hints: Use equal inset and equal-size keycaps at both ends.
  Evidence: The L and R key boxes differ in spacing from the adjacent tabs.

- [x] `fighter_menu-08` · P3 · proposal. Tab row: Align all tab label baselines and express selection with the top edge or underline.
  Evidence: The active tab is taller than inactive tabs without a consistent baseline.

- [x] `fighter_menu-09` · P2 · proposal. Offline Mode backdrop label: Move or remove this secondary label so its full text remains visible.
  Evidence: The central Offline Mode label is partially covered by the selected menu ribbon.

- [x] `fighter_menu-10` · P2 · proposal. System Options ribbon: Shorten its point and keep a predictable menu-column width.
  Evidence: The bright green selected row is far wider than its text.

- [x] `fighter_menu-11` · P2 · proposal. Menu icon column: Normalize the visual size and center each icon in its tile.
  Evidence: Slanted dark icon tiles use different apparent icon sizes.

- [x] `fighter_menu-12` · P2 · proposal. Game Options icon: Use a distinct button-mapping icon for Button Settings.
  Evidence: Game Options and Button Settings share essentially the same controller silhouette.

- [x] `fighter_menu-13` · P2 · proposal. Unselected menu labels: Raise contrast and reduce letter spacing in the six menu labels.
  Evidence: Widely spaced gray serif labels require effort against black ribbons.

- [x] `fighter_menu-14` · P3 · proposal. Menu row spacing: Use consistent row separation across the left column.
  Evidence: Thin pale gaps between the black ribbons vary in visual weight because of angled ends.

- [x] `fighter_menu-15` · P2 · proposal. System preview heading: Keep one strong title and use the second line for category context.
  Evidence: System Options appears twice in adjacent large and small treatments.

- [x] `fighter_menu-16` · P2 · proposal. Large System Options text: Use a dark outline or solid backing so System Options stays legible over the pale preview.
  Evidence: The white title uses a bright green offset over pale green; the web mock gives it a dark outline.

- [x] `fighter_menu-17` · P2 · proposal. Gear illustration: Reduce the gear slightly and enlarge SYSTEM to a legible supporting label.
  Evidence: The gear dominates the right preview while the category caption is very small.

- [x] `fighter_menu-18` · P3 · proposal. Preview frame: Keep one primary frame and one subtle offset accent.
  Evidence: Several translucent green frames overlap at the right edge.

- [x] `fighter_menu-19` · P2 · proposal. Description text: Left-align the description with a shorter measure and normal spacing.
  Evidence: The description occupies three centered lines with large letter spacing.

- [x] `fighter_menu-20` · P3 · proposal. Description panel: Reduce the point so the reading area has a more regular shape.
  Evidence: The black description panel has a large protruding diagonal left point.

- [x] `fighter_menu-21` · P2 · proposal. Background character: Place the character within an unobstructed central background zone.
  Evidence: The small character intersects the menu ribbons and pale floor geometry.

- [x] `fighter_menu-22` · P3 · proposal. Background pattern: Lower their contrast behind interactive labels.
  Evidence: Crosshairs and diagonal patterns remain strong around the navigation row.

- [x] `fighter_menu-23` · P2 · proposal. Footer Select hint: Use a consistent directional-pad glyph with the same keycap styling.
  Evidence: A plus sign stands in for directional input while other actions use keycaps.

- [x] `fighter_menu-24` · P2 · proposal. Footer change entry hint: Use two individual L and R keycaps consistently.
  Evidence: L R share one wide keycap unlike the separate top shoulder hints.

- [x] `fighter_menu-25` · P2 · proposal. Footer spacing: Use evenly spaced icon-label groups without colon punctuation.
  Evidence: The hints run across a dark gradient with uneven gaps around colons.

### file_tree

[Baseline](screenshot-baselines/screens/file_tree_720p.png) · [Visual review](design-audit.html#screen=file_tree)

- [x] `file_tree-01` · P2 · proposal. File Tree heading: Increase its size and give the bar a clear window-title treatment.
  Evidence: The title is small within a wide gray title bar.

- [x] `file_tree-02` · P2 · proposal. Outer panel: Add a consistent top margin around the complete window.
  Evidence: The top of the rounded panel touches the viewport boundary.

- [x] `file_tree-03` · P2 · proposal. Tree body: Reduce the panel width or add a selected-file detail pane.
  Evidence: Files occupy the left quarter of a very wide empty panel.

- [x] `file_tree-04` · P2 · proposal. File sizes: Right-align sizes in a dedicated column.
  Evidence: 47.1 KB, 6.0 KB, and 21.0 KB begin at different horizontal positions.

- [x] `file_tree-05` · P2 · proposal. Column labels: Add Name and Size headings above the first tree row.
  Evidence: The filename and size values have no column headers.

- [x] `file_tree-06` · P2 · proposal. Folder indicators: Replace it with a crisp disclosure chevron at a consistent size.
  Evidence: Expanded folders use a small handwritten v.

- [x] `file_tree-07` · P2 · proposal. File versus folder rows: Add folder and file icons while retaining the slash convention if desired.
  Evidence: Folder identity relies mostly on a trailing slash.

- [x] `file_tree-08` · P2 · proposal. Nested engine files: Add subtle vertical hierarchy guides.
  Evidence: Indentation is the only guide connecting renderer files to engine.

- [x] `file_tree-09` · P2 · proposal. Filename typography: Use a compact readable face for filenames and tabular numerals for sizes.
  Evidence: Handwritten letters make renderer.cpp and renderer.h less scannable.

- [x] `file_tree-10` · P2 · proposal. Root path: Increase its text size and present the path as a breadcrumb.
  Evidence: ~/projects/harbour is tiny inside a nearly full-width dark pill.

- [x] `file_tree-11` · P2 · proposal. Sample tree control: Show Sample data as a selected mode beside the path.
  Evidence: Sample tree resembles an ordinary action with no mode status.

- [x] `file_tree-12` · P3 · proposal. Home control: Pair it with a home icon and align it with the breadcrumb start.
  Evidence: Home is detached from any visible location hierarchy.

- [x] `file_tree-13` · P3 · proposal. Up control: Group Home and Up as navigation, separated from refresh actions.
  Evidence: The short Up label has the same emphasis as Refresh.

- [x] `file_tree-14` · P3 · proposal. Refresh control: Add a compact refresh glyph to improve scanning.
  Evidence: Refresh has no icon in a toolbar of small text pills.

- [x] `file_tree-15` · P2 · proposal. Toolbar dimensions: Increase button height and maintain a common label baseline.
  Evidence: The toolbar buttons are short relative to the file row text.

- [x] `file_tree-16` · P2 · proposal. Toolbar background: Use a compact toolbar group or add a right-aligned mode indicator.
  Evidence: The dark toolbar strip spans empty space far past the last button.

- [x] `file_tree-17` · P2 · proposal. No selection footer: Increase it to the same supporting-text size as the path.
  Evidence: The status text is extremely small and low contrast.

- [x] `file_tree-18` · P3 · proposal. Selection guidance: Use Select a file to view details in the empty detail region.
  Evidence: No selection reports state without suggesting the next step.

- [x] `file_tree-19` · P3 · proposal. Tree summary: Add a footer count of folders and files.
  Evidence: The window contains no summary of the visible sample.

- [x] `file_tree-20` · P2 · proposal. src assets tests hierarchy: Give top-level folder labels a slightly stronger weight.
  Evidence: Top-level folders share the same weight as leaf files.

- [x] `file_tree-21` · P3 · proposal. Directory groups: Add a small gap before each top-level directory.
  Evidence: assets and tests follow immediately after the preceding subtree.

- [x] `file_tree-22` · P3 · proposal. README and makefile: Separate the root-file group with a small consistent gap.
  Evidence: Root-level files visually blend into the preceding tests subtree.

- [x] `file_tree-23` · P2 · proposal. Font asset size: Use a muted but legible size column to emphasize names.
  Evidence: Archivo.ttf and atlas.png sizes share the same tone as their names.

- [x] `file_tree-24` · P3 · proposal. Window border: Use a clearer surface contrast with a restrained border.
  Evidence: A thin gray outline and nearly identical surrounding grays flatten the window.

- [x] `file_tree-25` · P3 · proposal. Bottom whitespace: Center the reduced window vertically or expand useful content into that space.
  Evidence: A broad unused strip remains below the tree window.

### fit_content_lab

[Baseline](screenshot-baselines/screens/fit_content_lab_720p.png) · [Visual review](design-audit.html#screen=fit_content_lab)

- [x] `fit_content_lab-01` · P2 · proposal. Page heading: Use Fit-content bubbles as the title and a separate concise API subtitle.
  Evidence: The heading combines an API name and a long explanation at one small size.

- [x] `fit_content_lab-02` · P2 · proposal. Column labels: Increase the labels and use consistent spacing before px.
  Evidence: cap 380px and cap 200px are much smaller than the already small body text.

- [x] `fit_content_lab-03` · P2 · proposal. Column framing: Place each conversation in a subtle labeled frame.
  Evidence: The two repeated conversations have no visible panel boundary.

- [x] `fit_content_lab-04` · P2 · proposal. Column comparison: Add 380 px and 200 px measurement guides above the longest bubbles.
  Evidence: The narrow cap is only described in text.

- [x] `fit_content_lab-05` · P2 · proposal. Short hi bubbles: Add equal minimum horizontal and vertical bubble padding while preserving content sizing.
  Evidence: The hi background hugs the letters with almost no visible inset.

- [x] `fit_content_lab-06` · P2 · proposal. Long technical message: Use a readable body face and more line height for both comparisons.
  Evidence: The long message has tightly packed lines and a handwritten face.

- [x] `fit_content_lab-07` · P2 · proposal. Speaker distinction: Label the two roles outside the samples with a compact legend.
  Evidence: Left and right alignment plus fill color are the only speaker cues.

- [x] `fit_content_lab-08` · P3 · proposal. Repeated content: Add Same text, different maximum width beneath the title.
  Evidence: The columns do not explicitly say they contain identical messages.

- [x] `fit_content_lab-09` · P2 · proposal. Horizontal empty space: Use narrower comparison frames that keep message relationships easy to follow.
  Evidence: The short left and right messages sit at opposite edges of very wide columns.

- [x] `fit_content_lab-10` · P3 · proposal. Bottom empty region: Add a compact explanation of intrinsic width versus maximum width below the examples.
  Evidence: Most of the viewport below the six messages is unused.

- [x] `fit_content_lab-11` · P3 · proposal. Wide long bubble: Add an external line-count caption to support comparison with the narrow bubble.
  Evidence: Its exact three-line wrap is visible but unannotated.

- [x] `fit_content_lab-12` · P3 · proposal. Narrow long bubble: Add an external height guide beside the narrow example.
  Evidence: Its increased height is visible without a height reference.

- [x] `fit_content_lab-13` · P2 · proposal. Message ordering: Add unobtrusive message numbers beside matching bubbles.
  Evidence: The second conversation ends lower because of wrapping, obscuring one-to-one row comparison.

- [x] `fit_content_lab-14` · P3 · proposal. Code-oriented prose: Place the technical story in a sample badge and explain the sizing rule separately.
  Evidence: The example story mentions a layout bug but does not identify the tested sizing rule.

- [x] `fit_content_lab-15` · P3 · proposal. Title strip: Reduce the strip to a plain header with a divider above the frames.
  Evidence: The near-full-width title strip has rounded ends unrelated to the bubble comparison.

- [x] `fit_content_lab-16` · P2 · proposal. Final bubbles: Use consistent vertical conversation spacing after wrapped messages.
  Evidence: Anyway it is fixed appears far from the preceding incoming message.

### flex_alignment

[Baseline](screenshot-baselines/screens/flex_alignment_720p.png) · [Visual review](design-audit.html#screen=flex_alignment)

- [x] `flex_alignment-01` · P2 · proposal. Tab typography: Increase all six tab labels to a readable supporting size.
  Evidence: Vertical and Horizontal are tiny beside the large page title.

- [x] `flex_alignment-02` · P2 · proposal. All tab: Bring a brighter active indicator closer to the All label.
  Evidence: Selection uses a muted underline at the far bottom of a tall tab.

- [x] `flex_alignment-03` · P2 · proposal. Gap category: Add a compact gap comparison or rename All to Overview.
  Evidence: Gap appears in navigation but is absent from the All overview.
  Disposition: Renamed the old All tab to Overview because it shows vertical and horizontal fixtures while Gap remains separate.

- [x] `flex_alignment-04` · P2 · proposal. Section headings: Put API names in smaller secondary captions.
  Evidence: Long API names compete with the plain-language titles.

- [x] `flex_alignment-05` · P2 · proposal. Vertical examples: Add a downward main-axis arrow beside the column samples.
  Evidence: Start, End, and Center omit the main-axis direction.

- [x] `flex_alignment-06` · P2 · proposal. Horizontal examples: Add a rightward main-axis arrow over the row samples.
  Evidence: The row direction is named only in a long heading.

- [x] `flex_alignment-07` · P2 · proposal. Cross-axis examples: Add a vertical cross-axis guide outside the sample boxes.
  Evidence: Cross-Axis Align has no visual axis reference.

- [x] `flex_alignment-08` · P2 · proposal. SelfAlign panel: Caption the parent AlignItems::Center setting above the panel.
  Evidence: Auto and Center occupy the same height without explaining why.

- [x] `flex_alignment-09` · P3 · proposal. Auto item: Label it Inherits parent in an external caption.
  Evidence: Auto has a unique purple fill with no legend.

- [x] `flex_alignment-10` · P3 · proposal. SelfAlign colors: Add a small legend mapping colors to the four overrides.
  Evidence: Start, Auto, Center, and End use four colors without a key.

- [x] `flex_alignment-11` · P2 · proposal. Vertical item numerals: Center the numerals without changing the box positions.
  Evidence: Numbers sit near the left edge of teal rounded boxes.

- [x] `flex_alignment-12` · P2 · proposal. Horizontal numerals: Use a more legible numeral face at the same sample dimensions.
  Evidence: Tiny numerals are tightly packed inside narrow pink pills.

- [x] `flex_alignment-13` · P2 · proposal. Between label: Use Space between in a caption outside the narrow sample.
  Evidence: Between abbreviates the actual spacing mode.

- [x] `flex_alignment-14` · P2 · proposal. Around label: Add an edge-gap diagram beneath the sample.
  Evidence: Around leaves the relationship between edge and internal gaps implicit.
  Disposition: SpaceAround diagrams include the configured base gap: inner gaps equal twice edge space plus that base gap.

- [x] `flex_alignment-15` · P3 · proposal. Start versus End: Add subtle brackets showing the unused region for both cases.
  Evidence: Their leftover space is visible but unmeasured.

- [x] `flex_alignment-16` · P3 · proposal. Center column: Draw a faint midpoint guide behind the sample.
  Evidence: The centered group's relationship to the container midpoint is implicit.

- [x] `flex_alignment-17` · P3 · proposal. Between column: Add equal-gap markers alongside the three items.
  Evidence: The distribution is visible without identifying equal internal gaps.

- [x] `flex_alignment-18` · P3 · proposal. Cross-axis item heights: Caption the fixed test heights outside the comparison.
  Evidence: The three varied heights are unexplained.
  Disposition: The fixture uses 50%, 30%, and 40% heights, not fixed pixel heights. Captions identify those percentages.

- [x] `flex_alignment-19` · P2 · proposal. Start cross-axis sample: Add a subtle alignment guide along the tested top edge.
  Evidence: The top edges align but no reference line marks that edge.

- [x] `flex_alignment-20` · P2 · proposal. End cross-axis sample: Add a straight bottom reference guide inside the frame.
  Evidence: The common bottom edge is hard to compare against rounded framing.

- [x] `flex_alignment-21` · P3 · proposal. Horizontal sample frames: Reduce framing height while preserving the row's measured container.
  Evidence: Their large empty lower halves make the short row hard to inspect.

- [x] `flex_alignment-22` · P2 · proposal. Nested panel fills: Remove one decorative background layer around each example group.
  Evidence: Four similar blue-gray surface layers obscure grouping.

- [x] `flex_alignment-23` · P3 · proposal. Lower panel space: Fit the panels to their sections or use the spare space for a legend.
  Evidence: Both outer columns extend well below their content.

- [x] `flex_alignment-24` · P3 · proposal. Heading insets: Use consistent horizontal padding in every section header.
  Evidence: Section headings begin almost at the rounded panel edges.

- [x] `flex_alignment-25` · P2 · proposal. Overview explanation: Add a one-line comparison rule under Flexbox Alignment Demo.
  Evidence: No short sentence states what changes and what stays fixed between samples.

### flight_options

[Baseline](screenshot-baselines/screens/flight_options_720p.png) · [Visual review](design-audit.html#screen=flight_options)

- [x] `flight_options-01` · P2 · proposal. Flight System suboptions: Increase suboption text while retaining the two-level hierarchy.
  Evidence: Four setting names are much smaller than category navigation.

- [x] `flight_options-02` · P2 · proposal. Suboption spacing: Increase row height to form distinct selection targets.
  Evidence: Control Type and High-G Turn Settings sit in tightly spaced rows.

- [x] `flight_options-03` · P2 · proposal. Suboption location: Align their heading with the selected Flight System category.
  Evidence: The suboptions start beside the middle of the navigation list.

- [x] `flight_options-04` · P2 · proposal. Submenu heading: Add a restrained Flight System subheading over the controls.
  Evidence: No heading identifies the short list as Flight System settings.

- [x] `flight_options-05` · P2 · proposal. Control Type value: Add a right-aligned current setting value.
  Evidence: Control Type shows no current value.

- [x] `flight_options-06` · P2 · proposal. High-G Turn value: Expose the current mode beside the label.
  Evidence: High-G Turn Settings shows only its name.

- [x] `flight_options-07` · P2 · proposal. Collision damage value: Show a compact On or Off value with the setting.
  Evidence: Reduced Collision Damage has no visible on/off state.

- [x] `flight_options-08` · P2 · proposal. Vibration row: Raise disabled text contrast while keeping it visibly inactive.
  Evidence: Vibration is extremely dim even though its unavailable status is useful information.

- [x] `flight_options-09` · P2 · proposal. Unavailable explanation: Add a short requirement note beneath the disabled row.
  Evidence: Vibration says Unavailable without a reason.

- [x] `flight_options-10` · P3 · proposal. Selected category: Use a taller marker aligned with the row's text block.
  Evidence: A thin gold marker identifies Flight System but stops short of its full row height.

- [x] `flight_options-11` · P3 · proposal. Selection wash: Limit it to the navigation column width.
  Evidence: The faint gold wash stretches far beyond Flight System.

- [x] `flight_options-12` · P2 · proposal. Options heading: Increase its size or weight to establish a clearer page heading.
  Evidence: OPTIONS is only slightly larger than category labels.

- [x] `flight_options-13` · P2 · proposal. Main navigation width: Provide a quiet text zone behind the navigation list.
  Evidence: Category labels sit beside overlapping technical wire lines.

- [x] `flight_options-14` · P3 · proposal. Menu spacing: Add subtle groups for input, display, and audio/language settings.
  Evidence: The nine category rows form a dense uninterrupted block.

- [x] `flight_options-15` · P2 · proposal. Help sentence: Move the help closer to the settings panel.
  Evidence: Select the in-flight system is far below the related list.

- [x] `flight_options-16` · P3 · proposal. Help content: Describe the currently highlighted setting in one concise sentence.
  Evidence: The help sentence repeats the selected category without explaining it.

- [x] `flight_options-17` · P2 · proposal. Footer Enter key: Widen the keycap enough for the full label at readable size.
  Evidence: Enter is very small inside a narrow outlined keycap.

- [x] `flight_options-18` · P2 · proposal. Footer Esc key: Use a consistent readable key-label size for Enter and Esc.
  Evidence: Esc uses a tiny type size compared with BACK.

- [x] `flight_options-19` · P2 · proposal. Footer OK label: Use Select or Apply according to the action represented.
  Evidence: OK does not describe whether it opens or confirms the selected item.

- [x] `flight_options-20` · P3 · proposal. Footer grouping: Increase the gap between action groups.
  Evidence: Enter OK and Esc BACK have little separation from one another.

- [x] `flight_options-21` · P3 · proposal. Footer background lines: Clear the wire treatment from the footer's reading area.
  Evidence: Diagonal wire lines pass behind the footer keycaps.

- [x] `flight_options-22` · P3 · proposal. Right-side radar: Use the radar area for a restrained setting preview or explanatory illustration.
  Evidence: A large faint ellipse occupies much of the empty right half.

- [x] `flight_options-23` · P3 · proposal. Right angular outline: Integrate it into the preview frame or simplify it.
  Evidence: An isolated L-shaped outline has no relation to the displayed settings.

- [x] `flight_options-24` · P3 · proposal. Page balance: Give the setting detail area more width while retaining the sparse background.
  Evidence: Nearly all informative content sits within the left 500 pixels.

- [x] `flight_options-25` · P3 · proposal. Technical grid: Reduce grid contrast behind navigation and descriptions.
  Evidence: Grid lines remain equally visible under text and empty areas.

### font_weight

[Baseline](screenshot-baselines/screens/font_weight_720p.png) · [Visual review](design-audit.html#screen=font_weight)

- [x] `font_weight-01` · P2 · proposal. Title: Separate Font weight from Archivo Narrow and the comparison instruction.
  Evidence: The family name and test instruction share one long small heading.

- [x] `font_weight-02` · P2 · proposal. Weight labels: Use a neutral label face so letterform style does not distract from weight comparison.
  Evidence: Light 300 through Bold 700 use a handwritten face unlike the samples.

- [x] `font_weight-03` · P2 · proposal. Light fallback label: Separate Requested 300 from Rendered 400 in adjacent columns.
  Evidence: The fallback explanation is packed into the long row label.

- [x] `font_weight-04` · P2 · proposal. Row headers: Add Requested weight and Rendered sample column headings.
  Evidence: No headers distinguish weight metadata from sample text.

- [x] `font_weight-05` · P2 · proposal. Sample size: Add a larger companion specimen while preserving the current-size test.
  Evidence: The pangram is small relative to the available canvas.

- [x] `font_weight-06` · P3 · proposal. Baseline reference: Add optional subtle guides beneath each specimen.
  Evidence: The rows have no visible baseline guides.
  Disposition: Guides mark specimen bounds. Public font data does not expose baseline metrics, so no arbitrary edge is presented as a typographic baseline.

- [x] `font_weight-07` · P3 · proposal. Weight progression: Add a compact 300-to-700 weight scale beside the labels.
  Evidence: Rows have equal spacing but no connecting visual scale.

- [x] `font_weight-08` · P3 · proposal. Fallback comparison: Group those rows with a fallback bracket.
  Evidence: Light and Regular look alike as expected, but the connection is textual only.

- [x] `font_weight-09` · P2 · proposal. SemiBold label: Use Semibold 600 consistently with the other row names.
  Evidence: SemiBold capitalization differs from familiar sentence-style naming.

- [x] `font_weight-10` · P3 · proposal. Pangram numbers: Add a separate 0123456789 line for number-weight comparison.
  Evidence: Only 0123 represent numeric glyphs.

- [x] `font_weight-11` · P3 · proposal. Pangram punctuation: Add a short punctuation specimen in the unused right area.
  Evidence: The sample includes little punctuation.

- [x] `font_weight-12` · P2 · proposal. Mixed-weight example: Label this section Mixed weights on one baseline.
  Evidence: theme.h appears below a large gap without a section heading.

- [x] `font_weight-13` · P2 · proposal. Mixed-weight alignment: Bring the runs closer while keeping the shared baseline test.
  Evidence: theme.h and src/ui are separated by a wide blank gap.

- [x] `font_weight-14` · P2 · proposal. Mixed-weight explanation: Move it into a legible caption below the specimen.
  Evidence: Regular weight, same baseline is small and dim.

- [x] `font_weight-15` · P3 · proposal. Right empty region: Use a second specimen column for a close-up comparison of 500 and 600.
  Evidence: The entire right half is unused.

- [x] `font_weight-16` · P3 · proposal. Specimen metadata: Add the font size and loaded-face names in a compact footer.
  Evidence: The screenshot names the family but not the rendered size.

### forms

[Baseline](screenshot-baselines/screens/forms_720p.png) · [Visual review](design-audit.html#screen=forms)

- [x] `forms-01` · P2 · proposal. Page title: Add a top safe margin and consistent title padding.
  Evidence: Form Components touches the top of its cyan strip.

- [x] `forms-02` · P2 · proposal. Slider labels: Place labels above their tracks so fills remain visually measurable.
  Evidence: Volume, Brightness, and Difficulty are embedded inside the filled halves.

- [x] `forms-03` · P2 · proposal. Slider thumb contrast: Use a brighter outlined thumb that remains distinct on both track colors.
  Evidence: Teal thumbs sit on teal filled tracks.

- [x] `forms-04` · P2 · proposal. Slider values: Place values in a shared trailing column.
  Evidence: Percent values are buried inside the labels.

- [x] `forms-05` · P2 · proposal. Volume track: Reduce the track height while preserving a generous thumb.
  Evidence: The thick track resembles a segmented button.

- [x] `forms-06` · P3 · proposal. Difficulty percentage: Add Easy and Hard endpoint labels or a named level.
  Evidence: Difficulty is represented as 30% without a scale explanation.

- [x] `forms-07` · P3 · proposal. Brightness control: Add a dim-to-bright icon pair outside the track.
  Evidence: Brightness has no low/high endpoint references.

- [x] `forms-08` · P2 · proposal. Progress section header: Use a shared header style and reserve colors for the sample contents.
  Evidence: Progress Bars uses a red strip while Sliders uses cyan, without a legend.

- [x] `forms-09` · P2 · proposal. Audio progress label: Move the text outside the bar for an unobstructed progress comparison.
  Evidence: Audio Level 75% sits on the colored bar.

- [x] `forms-10` · P2 · proposal. Level progress label: Separate the label and numeric value from the progress track.
  Evidence: Level Progress 75/100 is embedded in the fill.

- [x] `forms-11` · P3 · proposal. Progress units: Caption the two demos Percentage and Custom range.
  Evidence: One bar uses percent and the other a fraction without explanation.

- [x] `forms-12` · P2 · proposal. Checkbox marks: Use a clear square checkbox and centered check glyph.
  Evidence: Checked states appear as literal [x] strings.

- [x] `forms-13` · P2 · proposal. Checkbox position: Place each mark consistently before its label or at the trailing edge.
  Evidence: Checkbox marks sit near the center of long cyan rows.

- [x] `forms-14` · P2 · proposal. Checkbox label size: Increase checkbox labels to match slider-label readability.
  Evidence: Music and Fullscreen are much smaller than section headings.

- [x] `forms-15` · P3 · proposal. Checkbox grouping: Add small Audio and Display group labels.
  Evidence: Audio and display options form one undifferentiated list.

- [x] `forms-16` · P3 · proposal. SFX label: Use Sound effects for clearer scanning.
  Evidence: SFX is abbreviated while Music is spelled out.

- [x] `forms-17` · P3 · proposal. V-Sync label: Add a short explanatory subtitle in the demo's available width.
  Evidence: V-Sync has no supporting description.

- [x] `forms-18` · P2 · proposal. Scroll boundary: Keep clipping intentional but add a fade or More options cue at the boundary.
  Evidence: A sliver of the next checkbox appears at the bottom edge.

- [x] `forms-19` · P2 · proposal. Scrollbar: Move it closer to the checkbox list and give its track a clearer boundary.
  Evidence: The thin light thumb sits far to the right of the option rows.

- [x] `forms-20` · P2 · proposal. Status strip: Use three labeled value groups at a readable size.
  Evidence: Volume, Resolution, and Quality appear as one small pipe-separated line.
  Disposition: Resolution and Quality are existing fixed demonstration values, now labeled demo preset. The clipboard result is read-only; Paste reads text without applying settings.

- [x] `forms-21` · P3 · proposal. Status relation: Label the strip Current settings to explain that it summarizes the whole form.
  Evidence: Resolution and Quality are summarized while their controls are offscreen.

- [x] `forms-22` · P2 · proposal. Clipboard explanation: Increase it and wrap the copy/paste explanation into two short clauses.
  Evidence: The clipboard help sentence is tiny and widely spaced.

- [x] `forms-23` · P2 · proposal. Copy Status action: Use a clear primary/secondary pairing without danger-like red for Paste.
  Evidence: Copy Status and Paste use unrelated cyan and red fills.

- [x] `forms-24` · P2 · proposal. Clipboard result: Place the result in a labeled read-only output field.
  Evidence: Clipboard empty is small inline text without a visible output boundary.

- [x] `forms-25` · P2 · proposal. Clipboard bottom row: Add bottom padding without reducing the scrollable test region.
  Evidence: Buttons sit flush against the bottom edge of their enclosing panel.

### grid_lab

[Baseline](screenshot-baselines/screens/grid_lab_720p.png) · [Visual review](design-audit.html#screen=grid_lab)

- [ ] `grid_lab-01` · P2 · proposal. Header strip: Use Grid layout as a heading and keep the explanation as a subtitle.
  Evidence: The API explanation is a small single-line title.

- [ ] `grid_lab-02` · P2 · proposal. Equal-track caption: Increase the caption and identify this as Example 1.
  Evidence: Equal columns, ragged text is tiny relative to the table width.

- [ ] `grid_lab-03` · P2 · proposal. Equal-track cell text: Increase the specimen text or add a larger companion specimen.
  Evidence: Cell text is very small despite broad columns.

- [ ] `grid_lab-04` · P2 · proposal. Cell insets: Add consistent horizontal cell padding.
  Evidence: id, name, role, and notes sit almost against grid lines.

- [ ] `grid_lab-05` · P2 · proposal. Column headings: Use a stronger header face while preserving track dimensions.
  Evidence: Header text has little weight distinction from the body.

- [ ] `grid_lab-06` · P3 · proposal. ID column: Add an Equal 1fr tracks annotation above the table.
  Evidence: The short numeric column occupies the same width as long role text by design.

- [ ] `grid_lab-07` · P3 · proposal. Ragged name example: Add a caption explaining that content length does not move column boundaries.
  Evidence: a and a much longer name illustrate width variation without a reference.

- [ ] `grid_lab-08` · P3 · proposal. Notes dash: Label it as an empty-value fixture in the caption.
  Evidence: The dash in the first notes row has no explanatory context.

- [ ] `grid_lab-09` · P2 · proposal. Second caption: Display the four numeric track sizes above their corresponding columns.
  Evidence: Explicit tracks does not show the actual track widths.

- [ ] `grid_lab-10` · P2 · proposal. Spanning header: Add a subtle three-column bracket above the merged cell.
  Evidence: One cell, spanning three tracks is only communicated through text.

- [ ] `grid_lab-11` · P3 · proposal. Blank corner cell: Caption it as the row-index gutter outside the table.
  Evidence: The second table's first header cell is empty.

- [ ] `grid_lab-12` · P2 · proposal. r1c1 labels: Use a compact monospaced face for the coordinate fixtures.
  Evidence: Coordinate-style content is harder to scan in the small handwritten face.

- [ ] `grid_lab-13` · P3 · proposal. Striped second table: Label the alternating-row treatment in the sample metadata.
  Evidence: The middle body row has a distinct blue fill without an explanation.

- [ ] `grid_lab-14` · P2 · proposal. Thick-border claim: Add an enlarged border detail and the requested border width.
  Evidence: The caption says thick border but the thin displayed lines are hard to judge at this scale.

- [ ] `grid_lab-15` · P3 · proposal. Table comparison: Give each a clearly bounded sample frame with its own width annotation.
  Evidence: The first table spans the viewport while the second occupies less than half.

- [ ] `grid_lab-16` · P3 · proposal. Unused lower area: Place a compact track-definition diagram below the rendered examples.
  Evidence: Most of the screen below the second table is empty.

- [ ] `grid_lab-17` · P3 · proposal. Grid-line contrast: Reduce internal line emphasis while keeping the outer test border unchanged.
  Evidence: Grid lines and small pale text compete for attention.

- [ ] `grid_lab-18` · P2 · proposal. Expected result: Add Columns stay aligned across every row beneath the examples.
  Evidence: The page describes setup without a short success criterion.

### guess_who_lab

[Baseline](screenshot-baselines/screens/guess_who_lab_720p.png) · [Visual review](design-audit.html#screen=guess_who_lab)

- [x] `guess_who_lab-01` · P2 · defect. New game label: Add a visible gap between the label and reset icon.
  Evidence: The reset glyph touches the end of New game.

- [x] `guess_who_lab-02` · P2 · defect. Ask question label: Separate the arrow with consistent button-icon spacing.
  Evidence: The arrow touches the final letter of question.

- [x] `guess_who_lab-03` · P2 · proposal. Question sentence: Add a clear gap between the prompt and trait field.
  Evidence: DOES YOUR PERSON runs directly into the first dropdown.

- [x] `guess_who_lab-04` · P2 · proposal. Answer field: Label the field Answer above the compact selector.
  Evidence: Yes has no visible label explaining whose answer it represents.

- [x] `guess_who_lab-05` · P2 · proposal. Trait selector: Replace it with a crisp down chevron aligned to the trailing inset.
  Evidence: The selector ends with a literal v.

- [x] `guess_who_lab-06` · P2 · proposal. Per-face note selectors: Increase note text and give the arrow a separate trailing slot.
  Evidence: Twenty-four Not sure v labels use very small text.

- [x] `guess_who_lab-07` · P2 · proposal. Note versus elimination: Label the note row as a note and keep the flip instruction beside the board.
  Evidence: Each face has both a portrait and note control without a local distinction.

- [x] `guess_who_lab-08` · P2 · proposal. Remaining count: Pair it with a small board-status label or progress indicator.
  Evidence: 24 faces left is isolated at the right of the question bar.

- [x] `guess_who_lab-09` · P3 · proposal. Turn indicator: Use an active-player token or arrow consistent with the board palette.
  Evidence: A red dot accompanies YOUR TURN without explaining red's meaning.

- [x] `guess_who_lab-10` · P2 · proposal. Turn heading: Add Player 1 or the active player's name beside the turn label.
  Evidence: YOUR TURN and Find the mystery person lack player identity.

- [x] `guess_who_lab-11` · P2 · proposal. Game objective: Add a short first-turn instruction near the question bar.
  Evidence: Find the mystery person does not explain the question-and-answer sequence.

- [x] `guess_who_lab-12` · P3 · proposal. Logo scale: Reduce its height slightly to give the header a calmer top margin.
  Evidence: The stacked logo fills nearly the entire header height.

- [x] `guess_who_lab-13` · P2 · proposal. Portrait details: Increase facial-detail contrast while retaining all 24 visible cards.
  Evidence: Glasses, eye color, and hair details are small within the 8-column board.

- [x] `guess_who_lab-14` · P3 · proposal. Hat cropping: Provide consistent headroom for hatted portraits.
  Evidence: Tall hats touch or disappear beneath the top card edge.

- [x] `guess_who_lab-15` · P2 · proposal. Name strips: Increase the name-strip separation from each face.
  Evidence: Names sit in narrow cream bands very close to portrait art.

- [x] `guess_who_lab-16` · P3 · proposal. Card borders: Reduce border emphasis and reserve stronger framing for a chosen candidate.
  Evidence: Thick yellow double borders compete with distinguishing facial features.

- [x] `guess_who_lab-17` · P2 · proposal. Board frame: Simplify the frame to reclaim space for portraits and notes.
  Evidence: Multiple blue bevel bands consume height around the grid.

- [x] `guess_who_lab-18` · P3 · proposal. Card row gaps: Increase vertical gap between complete card groups.
  Evidence: The note bar nearly touches the next row's top border.

- [x] `guess_who_lab-19` · P2 · proposal. Footer help: Place the instruction immediately above or below the board near its leading edge.
  Evidence: Click a face to flip it down is far from the card controls.

- [x] `guess_who_lab-20` · P3 · proposal. Feedback sentence: Use an instructional starting message until the first question is asked.
  Evidence: A little question. A big clue. occupies the feedback area without showing game state.

- [x] `guess_who_lab-21` · P2 · proposal. Question history: Reserve a compact clue-history strip near the remaining count.
  Evidence: The composition has no visible record area for previous clues.

- [x] `guess_who_lab-22` · P3 · proposal. Trait grammar: Group trait selection separately from answer confirmation through spacing.
  Evidence: Does your person plus wear glasses is clear but tightly merged with answer and action.

- [x] `guess_who_lab-23` · P2 · proposal. Reset action hierarchy: Reduce its visual emphasis so the question action remains primary.
  Evidence: New game has a large outline pill near the turn status.

- [x] `guess_who_lab-24` · P3 · proposal. Card coordinate reference: Add subtle row markers to support discussing or testing a specific card.
  Evidence: The dense grid provides only personal names for locating faces.

- [x] `guess_who_lab-25` · P2 · proposal. Board status semantics: Add a legend distinguishing note state from eliminated state.
  Evidence: All note rows say Not sure while the header says 24 faces left.

### horizontal_drag

[Baseline](screenshot-baselines/screens/horizontal_drag_720p.png) · [Visual review](design-audit.html#screen=horizontal_drag)

- [x] `horizontal_drag-01` · P2 · proposal. Page title: Reduce the title size and give the lanes more visual emphasis.
  Evidence: Horizontal Drag & Drop dominates the small amount of content.

- [x] `horizontal_drag-02` · P2 · proposal. Subtitle: Use a smaller supporting size with a clear gap below the heading.
  Evidence: Drag tags between priority lanes is almost title-sized.

- [x] `horizontal_drag-03` · P2 · proposal. Lane counts: Use compact count badges aligned consistently after each priority name.
  Evidence: Counts are embedded in handwritten heading text.

- [x] `horizontal_drag-04` · P2 · proposal. Lane labels: Add a severity icon or short High/Medium/Low marker beside each heading.
  Evidence: Red, yellow, and green headings communicate priority mainly through color and words.

- [x] `horizontal_drag-05` · P2 · proposal. Tag affordance: Add a small drag grip to each tag.
  Evidence: The colored tags look like ordinary buttons in the resting image.

- [x] `horizontal_drag-06` · P2 · proposal. Tag color meaning: Use a shared neutral tag fill or explain the tag-color categories.
  Evidence: Seven unrelated tag fills have no visible legend.

- [x] `horizontal_drag-07` · P2 · proposal. Drop-space cue: Add a faint Drop here cue within each lane's empty region.
  Evidence: Large blank areas to the right of the tags have no instruction.

- [x] `horizontal_drag-08` · P2 · proposal. Lane dimensions: Reduce lane width or use the space for a visible drop target boundary.
  Evidence: All lanes span nearly the entire viewport while holding two or three tags.

- [x] `horizontal_drag-09` · P2 · proposal. Empty lower region: Fit the panel to the three lanes and center it vertically.
  Evidence: The bottom quarter of the panel contains no content.

- [x] `horizontal_drag-10` · P3 · proposal. Row borders: Increase lane boundary contrast slightly.
  Evidence: Thin outlines are nearly invisible against the dark lane fill.

- [x] `horizontal_drag-11` · P2 · proposal. High-priority tags: Use one high-priority tag treatment or label their separate categories.
  Evidence: Security patch and Data loss fix use two different red tones without distinct meanings.

- [x] `horizontal_drag-12` · P3 · proposal. Refactor auth label: Use Refactor authentication if space permits.
  Evidence: Auth is abbreviated in an otherwise plain-language board.

- [x] `horizontal_drag-13` · P3 · proposal. Update deps label: Use Update dependencies for a self-contained sample.
  Evidence: Deps is a developer abbreviation.

- [x] `horizontal_drag-14` · P3 · proposal. Rename var label: Use Rename variable.
  Evidence: Var is abbreviated despite the wide empty lane.

- [x] `horizontal_drag-15` · P3 · proposal. Readme typo label: Use README typo to identify the document clearly.
  Evidence: Readme differs from the conventional filename styling.

- [x] `horizontal_drag-16` · P2 · proposal. Tag spacing: Use one fixed inter-tag gap across the three lanes.
  Evidence: The gap between tags varies because each group uses different widths.

- [x] `horizontal_drag-17` · P3 · proposal. Lane heading gap: Add a consistent gap between lane title and tags.
  Evidence: Headings sit very close to their first tag row.

- [x] `horizontal_drag-18` · P2 · proposal. Demo result: Reserve a small Last moved status line beneath the lanes.
  Evidence: Only lane counts report the result of moving a tag.

- [x] `horizontal_drag-19` · P3 · proposal. Starting arrangement: Add a compact Reset example action near the subtitle.
  Evidence: The screen shows no reference for restoring the initial priority distribution.

- [x] `horizontal_drag-20` · P2 · proposal. Instruction specificity: State whether the demonstration also supports horizontal reordering.
  Evidence: The subtitle says move between lanes but does not mention ordering within a lane.

### hover_lab

[Baseline](screenshot-baselines/screens/hover_lab_720p.png) · [Visual review](design-audit.html#screen=hover_lab)

- [x] `hover_lab-01` · P2 · proposal. Title: Use a short title and a separate experiment summary.
  Evidence: The title mixes Hover Lab with two technical concepts on one line.

- [x] `hover_lab-02` · P2 · proposal. Case captions: Increase caption size and wrap each explanation above its row.
  Evidence: The three long captions use tiny handwritten text.

- [x] `hover_lab-03` · P2 · proposal. Case naming: Add names Direct hover, Subtree hover, and Pointer passthrough.
  Evidence: Cases are identified by code expressions before their purpose.

- [x] `hover_lab-04` · P2 · proposal. Star glyphs: Use a recognizable star glyph while preserving the hit-test geometry.
  Evidence: The interactive stars are rendered as small asterisks.

- [x] `hover_lab-05` · P2 · proposal. Row labels: Use concise behavior names in the row labels.
  Evidence: Thread row 1 through 3 do not name the tested behavior.

- [x] `hover_lab-06` · P2 · proposal. Caption and row grouping: Frame each case with a consistent sample boundary.
  Evidence: Each caption sits close to the preceding blank gap without a enclosing group.

- [x] `hover_lab-07` · P2 · proposal. Diagnostic table: Add Case, Row lit, Star hot, and Clicks headings.
  Evidence: Three diagnostic lines have no column headings.

- [x] `hover_lab-08` · P2 · proposal. Diagnostic values: Show Yes and No badges beside the API terms.
  Evidence: lit=n and star_hot=n use terse boolean abbreviations.

- [x] `hover_lab-09` · P2 · proposal. Diagnostic alignment: Use fixed-width columns for the three state fields.
  Evidence: Variable-length expressions push values into uneven columns.

- [x] `hover_lab-10` · P2 · proposal. Diagnostics separation: Place each case's live status directly beneath its row.
  Evidence: All diagnostics sit below all samples.

- [x] `hover_lab-11` · P2 · proposal. Row 1 explanation: Explain that entering the star removes the row highlight.
  Evidence: The phrase star steals hotness requires internal terminology.

- [x] `hover_lab-12` · P2 · proposal. Row 2 explanation: State Row stays highlighted; star still receives clicks.
  Evidence: The subtree behavior is described in a long code-heavy sentence.

- [x] `hover_lab-13` · P2 · proposal. Row 3 explanation: Add an external Pointer passthrough badge to identify the test variant.
  Evidence: The star looks identical to the other two despite intentionally ignoring input.

- [x] `hover_lab-14` · P2 · proposal. Footer instruction: Move the instruction above the samples near the title.
  Evidence: Click each star is small and far below the actual stars.

- [x] `hover_lab-15` · P3 · proposal. Initial-state capture: Add labeled expected-state thumbnails beside the live examples.
  Evidence: All rows have the same resting appearance.

- [x] `hover_lab-16` · P3 · proposal. Unused right area: Use the right half for a simple row-versus-child hit-test diagram.
  Evidence: Most of the canvas is blank.

- [x] `hover_lab-17` · P3 · proposal. Comparison outcome: Add a small expected-results matrix covering hover and clicks.
  Evidence: No visual legend explains which case keeps the row wash.

- [x] `hover_lab-18` · P3 · proposal. Click counter readability: Give each click count a compact, prominent numeric badge.
  Evidence: star_clicks=0 is embedded at the end of each small diagnostic line.

### hstack_showcase

[Baseline](screenshot-baselines/screens/hstack_showcase_720p.png) · [Visual review](design-audit.html#screen=hstack_showcase)

- [ ] `hstack_showcase-01` · P2 · proposal. Heading: Add Horizontal stack layout as a subtitle.
  Evidence: hstack() Showcase has no plain-language explanation.

- [ ] `hstack_showcase-02` · P2 · proposal. Basic row: Label this row Equal-width children.
  Evidence: Item 1, Item 2, and Item 3 have no section label.

- [ ] `hstack_showcase-03` · P3 · proposal. Basic-row gaps: Add an external gap measurement caption.
  Evidence: Large gaps are visible but their configured size is not.

- [ ] `hstack_showcase-04` · P2 · proposal. Justification section: Add Main-axis distribution above the five examples.
  Evidence: Five pink headers appear without a shared section title.

- [ ] `hstack_showcase-05` · P2 · proposal. Between and Around: Use Space between and Space around in external captions.
  Evidence: The shortened names omit Space.

- [ ] `hstack_showcase-06` · P2 · proposal. Pink header text: Increase caption size and apply equal horizontal padding.
  Evidence: Small black handwritten labels sit close to the left edge.

- [ ] `hstack_showcase-07` · P2 · proposal. Unnumbered cyan boxes: Add 1, 2, and 3 labels to reveal item order.
  Evidence: The repeated boxes contain no identity markers.

- [ ] `hstack_showcase-08` · P3 · proposal. Start example: Add a faint tested-edge guide in the frame.
  Evidence: Its packed group has no left-edge reference.

- [ ] `hstack_showcase-09` · P3 · proposal. End example: Add a faint right-edge guide.
  Evidence: The right-aligned group has no right-edge reference.

- [ ] `hstack_showcase-10` · P3 · proposal. Center example: Add a subtle centerline behind the sample.
  Evidence: The centered group lacks a midpoint marker.

- [ ] `hstack_showcase-11` · P3 · proposal. Between example: Add small equal-gap brackets outside the item row.
  Evidence: Equal gaps are shown without measurement cues.

- [ ] `hstack_showcase-12` · P3 · proposal. Around example: Annotate edge versus internal spacing beneath the frame.
  Evidence: The half-size edge gaps are not explained.

- [ ] `hstack_showcase-13` · P2 · proposal. Cross-axis section: Add Cross-axis alignment above them.
  Evidence: Green headers Start, Center, and End lack a group label.

- [ ] `hstack_showcase-14` · P3 · proposal. Unequal item heights: Caption their fixed heights as part of the test setup.
  Evidence: Green, orange, and purple item heights vary without a legend.

- [ ] `hstack_showcase-15` · P2 · proposal. Alignment guides: Add straight reference guides inside the three samples.
  Evidence: Rounded frames make common top and bottom edges less obvious.

- [ ] `hstack_showcase-16` · P2 · proposal. Toolbar Pattern heading: Increase its text and align it with toolbar content.
  Evidence: The label is tiny relative to the blue title strip.

- [ ] `hstack_showcase-17` · P2 · proposal. Toolbar commands: Use a readable command size within the toolbar specimen.
  Evidence: File, Edit, and View are much smaller than the large surrounding examples.

- [ ] `hstack_showcase-18` · P3 · proposal. Settings placement: Annotate it as the trailing action using remaining width.
  Evidence: The right-aligned Settings button demonstrates spare-space use without explanation.

- [ ] `hstack_showcase-19` · P2 · proposal. Card Row title: Rename it Four equal cards with fixed gaps.
  Evidence: The title does not explain which hstack behavior it demonstrates.

- [ ] `hstack_showcase-20` · P2 · proposal. A B C D cards: Center the letters or add a consistent inset.
  Evidence: Single letters sit against the left edges of the small cards.

- [ ] `hstack_showcase-21` · P2 · proposal. Dashboard row: Label the row Six expanding cards.
  Evidence: Users, Revenue, Orders, Growth, Active, and Churn appear without a section heading.

- [ ] `hstack_showcase-22` · P2 · proposal. Dashboard text: Add a consistent text inset without changing the card widths.
  Evidence: Each metric label sits flush against its card's left edge.

- [ ] `hstack_showcase-23` · P3 · proposal. Dashboard values: Add small fixed values to make the dashboard example recognizable.
  Evidence: The metric cards have labels but no sample values.

- [ ] `hstack_showcase-24` · P3 · proposal. Dashboard color key: Caption them as identity colors or reduce unrelated semantic associations.
  Evidence: Six bright colors suggest meaning without a legend.

- [ ] `hstack_showcase-25` · P3 · proposal. Bottom empty space: Use it for the stack width and gap configuration summary.
  Evidence: A broad blank region remains below the dashboard row.

### images

[Baseline](screenshot-baselines/screens/images_720p.png) · [Visual review](design-audit.html#screen=images)

- [x] `images-01` · P2 · defect. Play sprite: Use a single play triangle or rename the specimen Fast forward.
  Evidence: The icon labeled Play shows two right-pointing triangles, conventionally fast-forward.

- [x] `images-02` · P2 · proposal. Page title: Increase the heading and shorten the bar to the content width.
  Evidence: Image & Sprite Components uses small text across a wide title bar.

- [x] `images-03` · P2 · proposal. Section label styles: Use one section-label treatment unless color has a documented meaning.
  Evidence: Sprites and With Background are cyan while Image Buttons and Icon Row are red.

- [x] `images-04` · P2 · proposal. Sprite names: Show glyph name and intended action as separate consistent labels.
  Evidence: Gear and Trophy become Settings and Awards in the icon row.

- [x] `images-05` · P2 · proposal. Favorite naming: Choose one action label across both examples.
  Evidence: Favorite in the button row becomes Favorites in the icon row.

- [x] `images-06` · P2 · proposal. Sprite row alignment: Center each caption directly beneath its sprite.
  Evidence: Captions are offset left of the apparent icon centers.

- [x] `images-07` · P2 · proposal. Icon-row captions: Increase caption size and reduce tracking.
  Evidence: Settings and Play use small widely spaced lettering.

- [x] `images-08` · P3 · proposal. Gear visual size: Normalize optical icon size in the comparison row.
  Evidence: The gear appears smaller than the home and play silhouettes.

- [x] `images-09` · P3 · proposal. Star specimen: Add consistent image bounds behind the specimens to expose size differences.
  Evidence: The star has a smaller visual footprint than the trophy.

- [x] `images-10` · P2 · proposal. Sprite metadata: Add a compact 100 × 100 source metadata caption.
  Evidence: The first row provides no source dimensions or scale.

- [x] `images-11` · P3 · proposal. Image-button frame: Use a shared frame color or label each theme variant.
  Evidence: The three buttons use unrelated fill colors.

- [x] `images-12` · P2 · proposal. Button versus sprite: Add Static image and Clickable image badges beside their headings.
  Evidence: The difference between the first and second rows is implied only by titles.

- [x] `images-13` · P2 · proposal. Click counter: Place it in a visible shared result badge adjacent to the button group.
  Evidence: Clicks: 0 sits in open space away from the three buttons.

- [x] `images-14` · P3 · proposal. Counter scope: Label it Total image-button clicks.
  Evidence: Clicks: 0 does not say whether all buttons share the counter.

- [x] `images-15` · P2 · proposal. With Background sample: Rename the section Image with text background.
  Evidence: The purple Settings rectangle is not described as a compound image-and-label control.

- [x] `images-16` · P2 · proposal. Compound Settings spacing: Use a consistent icon-to-label gap and centered vertical alignment.
  Evidence: The gear and Settings text have a large unstructured gap.

- [x] `images-17` · P3 · proposal. Third row height: Fit the sample panel to its contents or add its geometry annotation.
  Evidence: A single small sample sits in a long full-width panel.

- [x] `images-18` · P3 · proposal. Icon row interaction: Label whether these are static icons or actions.
  Evidence: The icon row looks similar to the button row but its role is unclear.

- [x] `images-19` · P2 · proposal. Section label widths: Use a fixed section-label column to align all sample starts.
  Evidence: Sprites and Icon Row use narrower labels than the other rows.

- [x] `images-20` · P2 · proposal. Sample columns: Align matching glyphs vertically across both rows.
  Evidence: The first and fourth rows place matching icons at different x positions.

- [x] `images-21` · P3 · proposal. Right empty space: Use the area for source-rectangle and scale annotations.
  Evidence: Each row leaves a broad unused area on the right.

- [x] `images-22` · P3 · proposal. Bottom empty space: Center the gallery vertically or add a concise rendering-specification footer.
  Evidence: The page leaves a large empty band beneath Icon Row.

- [x] `images-23` · P2 · proposal. Home silhouette: Use consistent visual weight in the action row while retaining raw sprites in the first row.
  Evidence: The filled house is a strong shape compared with the detailed outlined gear.

- [x] `images-24` · P3 · proposal. Sprite background contrast: Add a small light-background companion sample for transparency inspection.
  Evidence: All raw sprites are white on dark with no alternative backdrop reference.

- [x] `images-25` · P3 · proposal. Image boundaries: Offer thin optional bounds around the raw sprite samples for inspection.
  Evidence: The visible glyphs do not expose their rectangular image bounds.

### islands_trains_settings

[Baseline](screenshot-baselines/screens/islands_trains_settings_720p.png) · [Visual review](design-audit.html#screen=islands_trains_settings)

- [ ] `islands_trains_settings-01` · P2 · proposal. Settings title: Increase title contrast and size while retaining the hand-lettered style.
  Evidence: The title is small and muted at the top of the tall paper.

- [ ] `islands_trains_settings-02` · P2 · proposal. Section headings: Darken the section-heading color for clearer grouping.
  Evidence: DISPLAY, CONTROLS, AUDIO, and TUTORIAL are pale cyan on cream.

- [ ] `islands_trains_settings-03` · P2 · proposal. Setting labels: Use darker olive text on the same soft row backgrounds.
  Evidence: Olive-gray labels have little contrast against sage rows.

- [ ] `islands_trains_settings-04` · P2 · proposal. Row height: Increase row height and vertical text padding.
  Evidence: Display and audio controls occupy very short strips.

- [ ] `islands_trains_settings-05` · P2 · proposal. Mode value: Use a stronger value weight and a fixed value column.
  Evidence: Borderless is not visually distinguished from Mode.

- [ ] `islands_trains_settings-06` · P2 · proposal. Resolution formatting: Use 2560 × 1440 with readable spacing.
  Evidence: 2560x1440 uses a cramped lowercase x.

- [ ] `islands_trains_settings-07` · P2 · proposal. Cycle arrows: Use consistent chevron buttons with visible hit-area framing.
  Evidence: Small angle-bracket characters sit near the values.

- [ ] `islands_trains_settings-08` · P2 · proposal. Camera labels: Increase the label-to-control gap or shorten the label to Pan speed.
  Evidence: Cam panning speed nearly touches its decrease arrow.

- [ ] `islands_trains_settings-09` · P2 · proposal. Camera capitalization: Use Camera rotation speed in sentence case.
  Evidence: Cam rotating Speed capitalizes Speed inconsistently.

- [ ] `islands_trains_settings-10` · P2 · proposal. Segment contrast: Increase their distinction while keeping the pastel palette.
  Evidence: Active cyan and inactive tan segments are similar in lightness.

- [ ] `islands_trains_settings-11` · P2 · proposal. Camera values: Add current step numbers beside both segment groups.
  Evidence: The two camera controls display segments without numeric values.

- [ ] `islands_trains_settings-12` · P2 · proposal. Effects value: Show its percentage in a compact trailing value field.
  Evidence: Effects Volume has no numeric percentage.

- [ ] `islands_trains_settings-13` · P2 · proposal. Music value: Show its percentage aligned with the effects value.
  Evidence: Music Volume has no numeric percentage.

- [ ] `islands_trains_settings-14` · P3 · proposal. Segment scale: Add one shared Low to High legend for the segmented controls.
  Evidence: Ten short bars have no minimum or maximum annotation.

- [ ] `islands_trains_settings-15` · P2 · proposal. Keyboard action: Give it a navigation arrow and label it Keyboard controls.
  Evidence: KEYBOARD resembles another passive setting row.

- [ ] `islands_trains_settings-16` · P2 · proposal. Tutorial action: Use a distinct secondary-action style with a play icon.
  Evidence: PLAY TUTORIAL has the same subdued appearance as the setting rows.

- [ ] `islands_trains_settings-17` · P3 · proposal. Tutorial section: Add a brief note describing what the tutorial covers.
  Evidence: The section contains only an action with no explanatory text.

- [ ] `islands_trains_settings-18` · P2 · proposal. Reset placement: Move it upward into a clear footer safe area.
  Evidence: RESET TO DEFAULTS sits almost on the bottom edge.

- [ ] `islands_trains_settings-19` · P2 · proposal. Reset affordance: Give it a bordered secondary-button treatment.
  Evidence: The reset action is plain low-contrast text.

- [ ] `islands_trains_settings-20` · P3 · proposal. Reset scope: Add a short All settings caption beside the action.
  Evidence: Reset to defaults does not state which settings it affects.

- [ ] `islands_trains_settings-21` · P2 · proposal. Close control: Align it with the panel's top corner or add a clear Close label.
  Evidence: The X is isolated far from the narrow paper panel.

- [ ] `islands_trains_settings-22` · P3 · proposal. Paper width: Widen the paper enough to accommodate readable controls and labels.
  Evidence: The narrow panel leaves large blank side fields.

- [ ] `islands_trains_settings-23` · P3 · proposal. Lower paper void: Reduce that gap and group reset within the settings footer.
  Evidence: A large blank region separates Play tutorial from Reset.

- [ ] `islands_trains_settings-24` · P3 · proposal. Section spacing: Use a consistent section-spacing rhythm.
  Evidence: The gap before AUDIO is notably larger than before CONTROLS.

- [ ] `islands_trains_settings-25` · P3 · proposal. Save behavior communication: Add a small Changes apply immediately message if that matches behavior.
  Evidence: No visible note explains whether changes apply immediately.

### kart_select

[Baseline](screenshot-baselines/screens/kart_select_720p.png) · [Visual review](design-audit.html#screen=kart_select)

- [x] `kart_select-01` · P2 · proposal. Preview driver cap: Repeat the portrait cap detail in the large driver preview to strengthen character continuity.
  Evidence: The Drift portrait carries a P on its cap while the large driver cap is blank.

- [x] `kart_select-02` · P2 · proposal. Vehicle thumbnails: Render the selected racer consistently across all vehicle thumbnails.
  Evidence: Each vehicle thumbnail includes a differently colored driver, suggesting a racer change.

- [x] `kart_select-03` · P2 · proposal. Selected vehicle color: Match the thumbnail to the selected vehicle's preview appearance.
  Evidence: Standard shows a red kart while the large preview is orange.

- [x] `kart_select-04` · P2 · proposal. Selected racer badge: Place the badge within a reserved top corner of the selected card.
  Evidence: P1 floats above the card and interrupts its border.

- [x] `kart_select-05` · P2 · proposal. Player indicator duplication: Use a consistent badge shape and color in both locations.
  Evidence: P1 appears in both the header and selected portrait with different shapes.

- [x] `kart_select-06` · P2 · proposal. Racer identity: Label the preview Selected racer and vehicle.
  Evidence: Large Drift text appears below the vehicle without a clear section label.

- [x] `kart_select-07` · P2 · proposal. Vehicle identity: Add Standard beneath the racer name.
  Evidence: The preview names Drift but does not repeat Standard.

- [x] `kart_select-08` · P2 · proposal. Stats values: Add values such as 6/10 in a trailing column.
  Evidence: Four segmented bars have no numeric scores.

- [x] `kart_select-09` · P2 · proposal. Stats scale: Use a clearly stated common maximum scale.
  Evidence: The bars contain many narrow ticks without a labeled range.

- [x] `kart_select-10` · P2 · proposal. Stat labels: Increase stat labels and align their baselines with the bars.
  Evidence: Speed and Acceleration are small compared with the preview name.

- [x] `kart_select-11` · P3 · proposal. Weight interpretation: Add a short tradeoff description for heavier handling.
  Evidence: Weight uses the same positive yellow fill as speed without context.

- [x] `kart_select-12` · P2 · proposal. Portrait name strips: Increase name text or strip height slightly.
  Evidence: Racer names are small in thin pale strips.

- [x] `kart_select-13` · P3 · proposal. Portrait art: Add more distinct facial or costume features to aid recognition.
  Evidence: Eight racers share nearly identical facial silhouettes.

- [x] `kart_select-14` · P2 · proposal. Selected portrait border: Add a dark selected outline or check marker inside the card.
  Evidence: Yellow fill and a thick yellow border share little contrast.

- [x] `kart_select-15` · P2 · proposal. Vehicle selection: Add a check or Selected label to its card.
  Evidence: Standard relies largely on yellow fill to mark selection.

- [x] `kart_select-16` · P2 · proposal. Vehicle section title: Increase the label and add a clear section gap.
  Evidence: CHOOSE YOUR VEHICLE is small and close to the cards.

- [x] `kart_select-17` · P3 · proposal. Vehicle naming: Use consistent title casing across all vehicle labels.
  Evidence: Off-Road is styled differently from plain one-word names.

- [x] `kart_select-18` · P2 · proposal. Preview illustration: Bring the large driver art to the same character style as the cards.
  Evidence: The preview driver is far less detailed than the selection portraits.

- [x] `kart_select-19` · P3 · proposal. Preview platform: Reduce platform thickness to give the stat card more breathing room.
  Evidence: The oversized pale oval occupies substantial vertical space.

- [x] `kart_select-20` · P2 · proposal. Footer hints: Increase the hint size and use clearer keycaps.
  Evidence: Arrows to choose and A Confirm B Back are tiny beside the Back button.

- [x] `kart_select-21` · P2 · proposal. Back duplication: Combine them into one labeled action with its key hint.
  Evidence: A large Back action and a B Back hint repeat the same function side by side.

- [x] `kart_select-22` · P2 · proposal. Ready action: Add a small Start Grand Prix or Continue caption appropriate to behavior.
  Evidence: READY! does not state the next step.

- [x] `kart_select-23` · P3 · proposal. Ready button padding: Use a controlled icon-to-label gap.
  Evidence: The action text and trailing arrow are spread across the large skewed button.

- [x] `kart_select-24` · P3 · proposal. Header context: Group race context under the title or closer to the player indicator.
  Evidence: Grand Prix · Mushroom Cup is smaller and distant from the screen title.

- [x] `kart_select-25` · P3 · proposal. Background checks: Lower texture contrast behind cards and stat text.
  Evidence: The checker texture is strong behind the portrait grid.

### kirby_options

[Baseline](screenshot-baselines/screens/kirby_options_720p.png) · [Visual review](design-audit.html#screen=kirby_options)

- [ ] `kirby_options-01` · P2 · proposal. Name control: Display a sample profile name and a smaller Edit name cue.
  Evidence: Name is a generic placeholder in the largest yellow control.

- [ ] `kirby_options-02` · P2 · proposal. Name icon: Reduce it to make room for the actual profile value.
  Evidence: The large profile silhouette consumes much of the name pill.

- [ ] `kirby_options-03` · P2 · proposal. Delete Data action: Use a clear delete-data icon with the existing label.
  Evidence: A small pawn-like symbol does not communicate deletion.

- [ ] `kirby_options-04` · P2 · proposal. Delete action hierarchy: Use a restrained warning accent and separate it from profile editing.
  Evidence: Delete Data sits beside the primary name control without a destructive distinction.

- [ ] `kirby_options-05` · P2 · proposal. Delete label: Increase it to a readable secondary-action size.
  Evidence: The label is much smaller than all other action names.

- [ ] `kirby_options-06` · P2 · proposal. Common section tag: Increase its contrast and use a stronger section-label treatment.
  Evidence: Common is white on a pale gray strip and small.

- [ ] `kirby_options-07` · P2 · proposal. Bottom help: Move it into a padded footer region inside the board.
  Evidence: The long help sentence sits outside the board near the viewport edge.

- [ ] `kirby_options-08` · P2 · proposal. Help context: Use one specific description for the current selection.
  Evidence: Enter a new name and customize your controls describes multiple actions at once.

- [ ] `kirby_options-09` · P2 · proposal. Top tabs: Add compact text labels beneath each icon or a visible selected-tab caption.
  Evidence: Network, controller, home, mail, and star are icon-only.

- [ ] `kirby_options-10` · P2 · proposal. Options selection: Connect the banner visually to the selected gear tab.
  Evidence: Options is indicated by a purple tab and separate purple banner.

- [ ] `kirby_options-11` · P3 · proposal. Tab spacing: Use a consistent transformed baseline and gap rhythm.
  Evidence: The top icon tabs have irregular apparent gaps due to rotation.

- [ ] `kirby_options-12` · P2 · proposal. L and R hints: Use consistent keycap boxes aligned with the tab row.
  Evidence: Small purple key labels float outside the tabs.

- [ ] `kirby_options-13` · P2 · proposal. Controller tile: Use a consistent icon orientation within each tile.
  Evidence: The controller icon is tilted independently from the text and board.

- [ ] `kirby_options-14` · P2 · proposal. Display tile: Normalize icon stroke and visual weight.
  Evidence: Display uses a thin monitor outline beside heavier filled icons.

- [ ] `kirby_options-15` · P2 · proposal. Accessibility tile: Increase label room or reduce its size slightly within the shared type scale.
  Evidence: The wide Accessibility label approaches the cell edges.

- [ ] `kirby_options-16` · P2 · proposal. Sound and Online layout: Use one icon-label layout or explicitly distinguish the two groups.
  Evidence: Bottom-row labels sit beside icons while top-row labels sit below them.

- [ ] `kirby_options-17` · P3 · proposal. Tile grid proportions: Use consistent cell proportions or make the grouping meaningful with labels.
  Evidence: Two broad bottom cells differ strongly from the three upper cells.

- [ ] `kirby_options-18` · P2 · proposal. Grid lines: Increase divider contrast slightly to clarify the clickable tile boundaries.
  Evidence: Thin beige dividers are faint against off-white paper.

- [ ] `kirby_options-19` · P3 · proposal. Notebook checkboxes: Render them as clearly decorative sketches or add meaningful task labels.
  Evidence: Five empty notebook boxes resemble interactive tasks without readable labels.

- [ ] `kirby_options-20` · P3 · proposal. Notebook block text: Use believable low-emphasis handwritten lines to support the stationery theme.
  Evidence: Opaque square marks substitute for writing.

- [ ] `kirby_options-21` · P2 · proposal. Notebook star button: Add a Profile customization caption near the oval.
  Evidence: The green oval star looks interactive but has no visible label.

- [ ] `kirby_options-22` · P3 · proposal. Pencil decoration: Move them into a margin where they do not obscure the faux content.
  Evidence: Large outlined pencils cover the notebook's last two rows.

- [ ] `kirby_options-23` · P3 · proposal. Notebook footprint: Reduce it slightly to give the options board more whitespace.
  Evidence: The decorative notebook occupies almost a third of the screen.

- [ ] `kirby_options-24` · P3 · proposal. Board shadow: Soften decorative shadows around the main reading area.
  Evidence: Strong paper shadows and dark notebook shadows compete with tile boundaries.

- [ ] `kirby_options-25` · P3 · proposal. Footer underline: Align it with the inner board margins and shorten it to the help region.
  Evidence: The purple line extends nearly to the screen edge below the help text.

### language_demo

[Baseline](screenshot-baselines/screens/language_demo_720p.png) · [Visual review](design-audit.html#screen=language_demo)

- [ ] `language_demo-01` · P2 · proposal. Language selectors: Add English, 한국어, and 日本語 in a wider selector group.
  Evidence: EN, KO, and JA show ISO codes without native names.

- [ ] `language_demo-02` · P2 · proposal. Selected language: Use one control palette and a clear selected marker.
  Evidence: EN is blue while the other languages are green, without a legend.

- [ ] `language_demo-03` · P2 · proposal. Selector spacing: Add small uniform gaps between the controls.
  Evidence: The three language pills touch one another.

- [ ] `language_demo-04` · P2 · proposal. Keyboard hints: Show the digits in separate compact keycaps.
  Evidence: [1], [2], and [3] are part of each label string.

- [ ] `language_demo-05` · P2 · proposal. Title bar: Separate the heading visually from the language picker.
  Evidence: The title occupies an oversized blue region that merges into EN.

- [ ] `language_demo-06` · P2 · proposal. Active panel heading: Add Current language: English above the greeting specimen.
  Evidence: Hello! occupies a large mustard strip without saying it is the active language.

- [ ] `language_demo-07` · P2 · proposal. Greeting typography: Use a deliberate specimen alignment with consistent content insets.
  Evidence: Hello! is left-aligned while all menu labels are centered.

- [ ] `language_demo-08` · P2 · proposal. Menu comparison scope: Add aligned translations for the same set of menu labels.
  Evidence: The left panel shows five actions while the right compares only Hello and Start.

- [ ] `language_demo-09` · P2 · proposal. Comparison rows: Use Language, Greeting, and Start columns.
  Evidence: EN, KO, and JA text are embedded in unstructured sentences.

- [ ] `language_demo-10` · P2 · proposal. Script scale: Normalize optical reading size while retaining appropriate fonts per script.
  Evidence: Korean appears markedly smaller than the handwritten English example.

- [ ] `language_demo-11` · P2 · proposal. Japanese baseline: Align row baselines or document the script-specific metrics in captions.
  Evidence: The Japanese sample uses a different apparent vertical alignment.

- [ ] `language_demo-12` · P2 · proposal. Comparison heading: Increase the heading and add a short font-comparison subtitle.
  Evidence: All Languages is small above a broad empty panel.

- [ ] `language_demo-13` · P3 · proposal. Unused comparison area: Add a multiline localized specimen to use the available area.
  Evidence: Most of the right panel below the three examples is empty.

- [ ] `language_demo-14` · P2 · proposal. Continue action: Separate the primary action from the menu specimen list.
  Evidence: Continue shares the same styling as Start, Settings, About, and Exit.

- [ ] `language_demo-15` · P2 · proposal. Exit ordering: Move Exit into a distinct secondary action group.
  Evidence: Exit appears directly above Continue without grouping.

- [ ] `language_demo-16` · P2 · proposal. Font metadata: Split language and active-font metadata into labeled values.
  Evidence: Active: ENGLISH and Font: Gaegu-Bold share one footer line.

- [ ] `language_demo-17` · P3 · proposal. Font coverage: Show the font used for each script in the comparison rows.
  Evidence: Only the active English font is named.

- [ ] `language_demo-18` · P2 · proposal. Footer instructions: Show 1 English · 2 Korean · 3 Japanese in the footer.
  Evidence: Press 1/2/3 to switch languages repeats the key labels without mapping them.

- [ ] `language_demo-19` · P3 · proposal. Panel junction: Use consistent panel corner treatment at their junction.
  Evidence: The two side panels meet the footer with small triangular background gaps.

- [ ] `language_demo-20` · P3 · proposal. Viewport composition: Center the complete demo panel vertically or fit its background to the viewport.
  Evidence: The content ends well above the bottom of the black viewport.

### layout

[Baseline](screenshot-baselines/screens/layout_720p.png) · [Visual review](design-audit.html#screen=layout)

- [ ] `layout-01` · P2 · proposal. Page title: Add Row and column layouts as a concise subtitle.
  Evidence: Layout System Demo does not identify the two demonstrated arrangements.

- [ ] `layout-02` · P2 · proposal. Top sample: Label the top frame Row · centered children.
  Evidence: Left, Center, and Right panels have no local section heading.

- [ ] `layout-03` · P2 · proposal. Lower sample: Label the lower frame Column · stacked children.
  Evidence: Stacked Item 1 through 3 have no local section heading.

- [ ] `layout-04` · P2 · proposal. Guide order: Order the guide descriptions to match the examples.
  Evidence: The footer describes Column before Row while the examples show Row first.

- [ ] `layout-05` · P2 · proposal. Guide proximity: Place each explanation directly beneath its sample.
  Evidence: Row and Column descriptions are far below their corresponding samples.

- [ ] `layout-06` · P3 · proposal. Panel names: Use numbered items and keep position names in the frame caption.
  Evidence: Left Panel and Right Panel describe position rather than the layout rule.

- [ ] `layout-07` · P3 · proposal. Top row gaps: Add a fixed-gap annotation outside the samples.
  Evidence: The gaps between three fixed-width panels are visible but unmeasured.

- [ ] `layout-08` · P3 · proposal. Column gaps: Add a vertical gap bracket with the configured value.
  Evidence: The tightly stacked items have no spacing caption.

- [ ] `layout-09` · P3 · proposal. Margins statement: Draw an external margin guide around one sample frame.
  Evidence: Margins control spacing has no visual reference.

- [ ] `layout-10` · P2 · proposal. Column inner frame: Label Parent and Child stack outside their boundaries.
  Evidence: A gray rounded panel nests inside a large white panel without naming their roles.

- [ ] `layout-11` · P3 · proposal. Sample color mapping: Add a small color key linking the same three items across both layouts.
  Evidence: Green, brown, and rust are reused without an explicit item-identity key.

- [ ] `layout-12` · P2 · proposal. Sample typography: Increase sample-label size with equal insets.
  Evidence: Panel names and stacked-item text are small relative to their large colored blocks.

- [ ] `layout-13` · P3 · proposal. Divider lengths: Align divider endpoints with the shared content inset.
  Evidence: The thin separators are narrower than the white sample panels.

- [ ] `layout-14` · P2 · proposal. Theme footer: Increase footer readability and separate theme metadata from the layout lesson.
  Evidence: Using Sage Natural theme with responsive layouts is tiny at the bottom.

- [ ] `layout-15` · P3 · proposal. Responsive claim: Show current viewport and parent widths as demo metadata.
  Evidence: The footer names responsive layouts without exposing container dimensions.

- [ ] `layout-16` · P3 · proposal. Vertical composition: Center the complete composition vertically within the viewport.
  Evidence: The demo's pale root panel leaves a black band below it.

### layout_bug_repros

[Baseline](screenshot-baselines/screens/layout_bug_repros_720p.png) · [Visual review](design-audit.html#screen=layout_bug_repros)

- [ ] `layout_bug_repros-01` · P2 · defect. Z-order caption: Rewrite the expectation to state creation-order rendering and identify blue-on-top as the expected result.
  Evidence: Case 4 says NOT A BUG but its expected-result line calls the visible blue-on-top order a bug.

- [ ] `layout_bug_repros-02` · P2 · proposal. Case status labels: Use separate status badges outside the test geometry.
  Evidence: FIXED and NOT A BUG are embedded in long small captions.

- [ ] `layout_bug_repros-03` · P2 · proposal. Case headings: Increase heading readability without changing the repro coordinates.
  Evidence: All seven descriptions use small handwritten text.

- [ ] `layout_bug_repros-04` · P2 · proposal. Expected-result lines: Put expected behavior in a clear two-line caption beneath each case.
  Evidence: The expected and bug descriptions are tiny and dense.

- [ ] `layout_bug_repros-05` · P2 · proposal. Number sequence: Add clear column labels or a numbered index to establish reading order.
  Evidence: Cases 1–5 run down the left and 6–7 start again at the top right.

- [ ] `layout_bug_repros-06` · P2 · proposal. Origin marker: Add an Origin 0,0 caption nearby outside its measured location.
  Evidence: The red plus at the top-left looks like an unexplained floating control.

- [ ] `layout_bug_repros-07` · P3 · proposal. Percent sample: Add parent 300 × 80 and child 100% dimension captions.
  Evidence: Red and cyan overlap by design but their dimensions are not shown.

- [ ] `layout_bug_repros-08` · P3 · proposal. Percent expectation: Add a small expected-geometry diagram outside the live sample.
  Evidence: Red fits inside cyan is stated in prose only.

- [ ] `layout_bug_repros-09` · P3 · proposal. Row sample: Caption each child's fixed 100-pixel width.
  Evidence: A, B, and C show fixed boxes without width annotations.

- [ ] `layout_bug_repros-10` · P3 · proposal. Row leftover space: Label the parent's full width to explain the unused region.
  Evidence: The gray parent extends far beyond C without explanation.

- [ ] `layout_bug_repros-11` · P2 · proposal. Flow sample caption: Connect the heading to the yellow parent with a subtle leader outside the test bounds.
  Evidence: The flow sample is far to the right of its case heading.

- [ ] `layout_bug_repros-12` · P3 · proposal. Flow coordinates: Show x and y labels explicitly in the metadata.
  Evidence: The heading embeds 400,310 without units or a coordinate key.

- [ ] `layout_bug_repros-13` · P3 · proposal. Flow children: Add external child-height and gap labels.
  Evidence: Child 1 and Child 2 have no dimension metadata.

- [ ] `layout_bug_repros-14` · P2 · proposal. Z-order label obstruction: Repeat both entity identifiers in an unobstructed external legend.
  Evidence: The red box's identifier is covered by the blue box as part of the test.

- [ ] `layout_bug_repros-15` · P2 · proposal. Z-order question: Use a neutral Blue entity · created second label outside the overlap.
  Evidence: BLUE mk(2000) ON TOP? reads as unresolved despite the status badge.

- [ ] `layout_bug_repros-16` · P3 · proposal. Sizing comparison: Add explicit Auto width and 250 px reference captions beside them.
  Evidence: The green and blue buttons are stacked far below their explanation.

- [ ] `layout_bug_repros-17` · P3 · proposal. Sizing difference: Add aligned width guides extending from their outer edges.
  Evidence: The two button widths are close but difficult to compare vertically.

- [ ] `layout_bug_repros-18` · P2 · proposal. Tab fixture: Add Selected: Tab A in the fixture metadata.
  Evidence: Tab A's yellow underline has no textual selected-state caption.

- [ ] `layout_bug_repros-19` · P3 · proposal. Tab container bounds: Add an external labeled outline key instead of altering the tested border.
  Evidence: The green outline is thin and hard to distinguish from surrounding dark space.

- [ ] `layout_bug_repros-20` · P2 · proposal. Toggle labels: Add light row guides outside the toggle geometry to connect labels and controls.
  Evidence: Toggle A, B, and C are far from their switches across a wide panel.

- [ ] `layout_bug_repros-21` · P3 · proposal. Toggle state: Add an external On, Off, On state summary.
  Evidence: The yellow/gray switch states are not named.

- [ ] `layout_bug_repros-22` · P3 · proposal. Toggle measurement: Display the declared container height and summed child heights.
  Evidence: The vertical-fit case names the concept without showing measured height.

- [ ] `layout_bug_repros-23` · P2 · proposal. Right empty region: Use it for a compact fixture legend and expected-results summary.
  Evidence: Most of the lower-right canvas is unused.

- [ ] `layout_bug_repros-24` · P3 · proposal. Diagnostic color key: Add a per-case parent/child color key.
  Evidence: Red, cyan, green, yellow, blue, and purple carry different roles across cases.

- [ ] `layout_bug_repros-25` · P2 · proposal. Page purpose: Add Retained regression fixtures; badges describe current expectations beneath the title.
  Evidence: Layout Bug Repros alone does not distinguish current regressions from retained historical cases.

### layout_patterns

[Baseline](screenshot-baselines/screens/layout_patterns_720p.png) · [Visual review](design-audit.html#screen=layout_patterns)

- [ ] `layout_patterns-01` · P2 · proposal. Page identification: Add a caption outside the measured fixture naming the fixed horizontal layout test.
  Evidence: The screenshot contains three numbered boxes with no title.

- [ ] `layout_patterns-02` · P2 · proposal. Test purpose: Add a one-line explanation of the expected row arrangement.
  Evidence: Nothing visible explains why the three boxes exist.

- [ ] `layout_patterns-03` · P2 · proposal. Parent dimensions: Show its width and height outside the tested rectangle.
  Evidence: The gray parent has no dimension annotation.

- [ ] `layout_patterns-04` · P3 · proposal. Child dimensions: Add an external 120-pixel child-width annotation.
  Evidence: The three equal boxes have no size labels.

- [ ] `layout_patterns-05` · P3 · proposal. Gap measurement: Add a 12-pixel gap marker outside the fixture.
  Evidence: The equal spaces between boxes are visible but unnamed.

- [ ] `layout_patterns-06` · P3 · proposal. Padding measurement: Add a 16-pixel padding guide outside the test geometry.
  Evidence: The gray parent padding is visible without a value.

- [ ] `layout_patterns-07` · P2 · proposal. Fixture position: Mark its fixed origin in an external caption so the placement reads as intentional.
  Evidence: The sample sits at the upper-left of a mostly blank canvas.

- [ ] `layout_patterns-08` · P3 · proposal. Expected result: Add Three equal children fit without overflow beneath the sample.
  Evidence: No visual statement describes the successful geometry.

- [ ] `layout_patterns-09` · P3 · proposal. Number legibility: Provide matching enlarged number labels in the external legend while preserving the tested content.
  Evidence: Black handwritten numerals are small within the bright boxes.

- [ ] `layout_patterns-10` · P3 · proposal. Color meaning: Add an external legend stating that colors identify child order.
  Evidence: Cyan, pink, and green are not explained.

### marlo_kart

[Baseline](screenshot-baselines/screens/marlo_kart_720p.png) · [Visual review](design-audit.html#screen=marlo_kart)

- [ ] `marlo_kart-01` · P2 · proposal. Engine-class group: Add Engine class above the three choices.
  Evidence: 50cc, 100cc, and 150cc have no group label.

- [ ] `marlo_kart-02` · P2 · proposal. Engine explanation: Add short speed/difficulty descriptors below each option.
  Evidence: The classes provide displacement values without difficulty context.

- [ ] `marlo_kart-03` · P2 · proposal. Selected 150cc: Add a check or Selected label to 150cc.
  Evidence: The selected state relies on yellow fill.

- [ ] `marlo_kart-04` · P3 · proposal. Engine typography: Use consistent spacing and tabular numerals for the three values.
  Evidence: The labels combine number and cc tightly.

- [ ] `marlo_kart-05` · P2 · proposal. Start label: Add a concise Select racer sublabel to clarify the next step.
  Evidence: START YOUR ENGINES is broad promotional copy for entering the next phase.

- [ ] `marlo_kart-06` · P2 · proposal. Start control size: Align its outer width with the group above.
  Evidence: The start button is much wider than the engine selector group.

- [ ] `marlo_kart-07` · P2 · proposal. Start text padding: Increase horizontal padding or reduce the text size slightly.
  Evidence: The long label nearly fills the button width.

- [ ] `marlo_kart-08` · P2 · proposal. Control group spacing: Increase the gap between selection and confirmation.
  Evidence: The class buttons almost touch the large action beneath them.

- [ ] `marlo_kart-09` · P2 · proposal. Keyboard hints: Use readable keycap-and-label groups beside the controls.
  Evidence: Arrows: engine class and Enter: start are tiny near the bottom.

- [ ] `marlo_kart-10` · P3 · proposal. Keyboard alignment: Align them on one baseline with equal group spacing.
  Evidence: The two keyboard hints have uneven separation and no shared structure.

- [ ] `marlo_kart-11` · P2 · proposal. Game-format sentence: Increase supporting-copy size and contrast.
  Evidence: Four tracks. Eight racers. One trophy. is very small.

- [ ] `marlo_kart-12` · P3 · proposal. Subtitle: Give the subtitle a stronger size and stable baseline.
  Evidence: AFTERHOURS GRAND PRIX is tiny under the oversized logo.

- [ ] `marlo_kart-13` · P3 · proposal. Logo spacing: Tighten the two-line logo while retaining its playful tilt.
  Evidence: MARLO and KART have a large irregular vertical gap.

- [ ] `marlo_kart-14` · P3 · proposal. Logo outline: Use a more consistent outline and shadow treatment.
  Evidence: The heavy dark outline creates uneven visual weight around the letters.

- [ ] `marlo_kart-15` · P2 · proposal. Kart badge: Restore the red M brand emblem inside the white nose badge.
  Evidence: The red kart has a blank white nose badge; the web mock places a red M inside it.

- [ ] `marlo_kart-16` · P2 · proposal. Driver face: Bring the illustration closer to the expressive racer portrait style.
  Evidence: The large driver has a single dot and no other facial detail.

- [ ] `marlo_kart-17` · P3 · proposal. Driver hands: Clarify hand positions and the wheel silhouette.
  Evidence: Broad overlapping arm shapes obscure the steering wheel.

- [ ] `marlo_kart-18` · P3 · proposal. Kart wheel perspective: Refine their perspective to match the chassis plane.
  Evidence: Front and rear wheels use noticeably different angle and scale cues.

- [ ] `marlo_kart-19` · P3 · proposal. Exhaust: Add a clearer pipe connection and consistent shading.
  Evidence: The pale exhaust shape resembles a detached rectangular strip.

- [ ] `marlo_kart-20` · P3 · proposal. Kart shadow: Soften and reduce the shadow to ground the kart.
  Evidence: A large dark green oval competes with the wheel shapes.

- [ ] `marlo_kart-21` · P2 · proposal. Racing context: Add a restrained road or track curve behind the kart.
  Evidence: The vehicle sits on flat green ground with no visible track.

- [ ] `marlo_kart-22` · P3 · proposal. Mountain backdrop: Add depth through a second subdued ridge layer.
  Evidence: Large flat triangles create a static horizon behind the logo.

- [ ] `marlo_kart-23` · P3 · proposal. Horizon stripe: Place controls on a consistent backing or move the horizon away from them.
  Evidence: A bright horizontal green band cuts through the class controls.

- [ ] `marlo_kart-24` · P3 · proposal. Composition balance: Move the controls slightly upward to balance the logo and vehicle.
  Evidence: The kart dominates the right while controls cluster low on the left.

- [ ] `marlo_kart-25` · P2 · proposal. Secondary navigation: Add compact Settings and Quit/Back actions if available in this title flow.
  Evidence: The title state exposes only engine class and start.

### media_library

[Baseline](screenshot-baselines/screens/media_library_720p.png) · [Visual review](design-audit.html#screen=media_library)

- [ ] `media_library-01` · P2 · proposal. Search placeholder: Increase placeholder size and add a recognizable search icon.
  Evidence: Search your library is tiny inside a wide field.

- [ ] `media_library-02` · P2 · proposal. Header actions: Increase their text size and separate them with consistent spacing.
  Evidence: Home, Server, and Settings are small and low contrast.

- [ ] `media_library-03` · P2 · proposal. Profile badge: Use a recognizable avatar treatment and a visible account label when space permits.
  Evidence: The lone G appears in a square without context.

- [ ] `media_library-04` · P2 · proposal. Sidebar headings: Increase their size and contrast without competing with navigation.
  Evidence: YOUR MEDIA and SERVER are extremely small.

- [ ] `media_library-05` · P2 · proposal. Sidebar alignment: Left-align navigation labels to the same sidebar inset.
  Evidence: Navigation text is centered while server text is left-aligned.

- [ ] `media_library-06` · P2 · proposal. Library count: Present it as a compact 24 items badge beside the heading.
  Evidence: 24 is detached from Library and faint.

- [ ] `media_library-07` · P2 · proposal. Repeated Library names: Rename the inner tab All titles or another distinct view name.
  Evidence: Library appears in the sidebar, page heading, and a tab.

- [ ] `media_library-08` · P2 · proposal. Recommended selection: Increase selected label contrast and keep the underline tightly aligned.
  Evidence: The selected tab uses only a thin yellow underline.

- [ ] `media_library-09` · P2 · proposal. All media filter: Add a trailing chevron and visible filter-control boundary.
  Evidence: All media looks like static text without a dropdown indicator.

- [ ] `media_library-10` · P2 · proposal. By title sort: Show Sort: Title with a chevron or direction indicator.
  Evidence: By title appears as another loose label.

- [ ] `media_library-11` · P2 · proposal. Grid List switch: Use a clear segmented control with icons and readable labels.
  Evidence: Grid and List use tiny text with a faint selected background.

- [ ] `media_library-12` · P2 · proposal. Selected poster frame: Wrap the selection frame around the actual poster or the full card including its metadata.
  Evidence: The Harbour Lights outline spans the whole tile width while the poster is narrower.

- [ ] `media_library-13` · P2 · proposal. Poster gutters: Use consistent poster-width cards to give the grid a stronger rhythm.
  Evidence: Portrait posters sit within wide dark horizontal gutters.

- [ ] `media_library-14` · P2 · proposal. Media titles: Increase title size and weight slightly.
  Evidence: Harbour Lights and other titles are small relative to their artwork.

- [ ] `media_library-15` · P2 · proposal. Year and type rows: Group metadata with a dot separator beneath the title.
  Evidence: Years sit left while Feature and Series sit far right in each tile.

- [ ] `media_library-16` · P2 · proposal. Type distinctions: Use compact type badges to distinguish mixed-media results.
  Evidence: Feature, Series, and Album are tiny plain words.

- [ ] `media_library-17` · P3 · proposal. Grid spacing: Increase the vertical gap between complete cards.
  Evidence: The second-row poster begins close to the first row's metadata.

- [ ] `media_library-18` · P2 · proposal. Selected detail metadata: Increase size and use clear centered-dot separators.
  Evidence: 2019 - 118 min - Feature is small and dim.

- [ ] `media_library-19` · P2 · proposal. Rating stars: Add 4/5 or the intended rating value beside the stars.
  Evidence: Four yellow stars and one gray star have no numeric label.

- [ ] `media_library-20` · P2 · proposal. Description: Increase body size and allow a readable two-line description.
  Evidence: The plot sentence is small and occupies a long narrow line.

- [ ] `media_library-21` · P2 · proposal. Director credit: Use Directed by Ana Weir with legible supporting contrast.
  Evidence: dir. Ana Weir is faint and abbreviated.

- [ ] `media_library-22` · P2 · proposal. Play action: Increase its label and add a single-triangle play icon.
  Evidence: The main Play button has small text and no play glyph.

- [ ] `media_library-23` · P2 · proposal. Watchlist action: Use an outlined Add to watchlist button or labeled icon.
  Evidence: A tiny plus floats beside Play with no visible label.

- [ ] `media_library-24` · P2 · proposal. Pagination: Increase page-control size and bottom padding.
  Evidence: Page numbers 1–4 are tiny and close to the bottom edge.

- [ ] `media_library-25` · P2 · proposal. Server status: Add a clear local-library badge or status dot with text.
  Evidence: Home server and Local demo library are faint and do not expose connection state.

### menu_showcase

[Baseline](screenshot-baselines/screens/menu_showcase_720p.png) · [Visual review](design-audit.html#screen=menu_showcase)

- [ ] `menu_showcase-01` · P2 · proposal. Title: Use Anchored menus as a heading with a separate edge-flip subtitle.
  Evidence: Anchored menus and the behavior explanation share one small line.

- [ ] `menu_showcase-02` · P2 · proposal. Top menu group: Place them in a modest menu-bar frame.
  Evidence: File and Edit look like two isolated large buttons.

- [ ] `menu_showcase-03` · P2 · proposal. Trigger labels: Use Bottom edge and Bottom-right corner.
  Evidence: Bottom and Corner describe position without identifying the demonstrated behavior.

- [ ] `menu_showcase-04` · P2 · proposal. Direction cues: Add small down/up indicators beside their labels.
  Evidence: The upper and lower triggers do not show opening directions.

- [ ] `menu_showcase-05` · P2 · proposal. Status message: Move the instruction below the heading.
  Evidence: Click a menu appears near the center far from every trigger.

- [ ] `menu_showcase-06` · P2 · proposal. Open-state status: Show Bottom menu open · placed above anchor in the baseline state.
  Evidence: The status still says click a menu while Bottom is already open.

- [ ] `menu_showcase-07` · P2 · proposal. Open menu text: Increase menu-label size and reduce tracking.
  Evidence: New, Open, Save, and Revert are small and widely spaced.

- [ ] `menu_showcase-08` · P2 · proposal. New row padding: Increase internal text inset while preserving the focus treatment.
  Evidence: The highlighted New row is tightly bounded by its outline.

- [ ] `menu_showcase-09` · P2 · proposal. Shortcut contrast: Increase shortcut contrast to a readable secondary level.
  Evidence: Cmd+N, Cmd+O, and Cmd+S are very dim against teal.

- [ ] `menu_showcase-10` · P2 · proposal. Shortcut alignment: Add a consistent trailing inset and a fixed shortcut column.
  Evidence: Shortcuts sit close to the menu's right edge.

- [ ] `menu_showcase-11` · P2 · proposal. Disabled Revert: Keep it inactive but raise label contrast enough to identify the option.
  Evidence: Revert is extremely faint inside the gray disabled row.

- [ ] `menu_showcase-12` · P3 · proposal. Disabled explanation: Add an external demo caption explaining that it is intentionally disabled.
  Evidence: Revert has no visible reason for its disabled example state.

- [ ] `menu_showcase-13` · P2 · proposal. Menu and trigger boundary: Add a subtle border or separator distinguishing the popup from its anchor.
  Evidence: The open menu and Bottom trigger use the same teal fill and meet directly.

- [ ] `menu_showcase-14` · P3 · proposal. Upward-placement proof: Draw a faint viewport-edge guide and an upward placement arrow outside the popup.
  Evidence: The expected flip is visible without a boundary marker.

- [ ] `menu_showcase-15` · P3 · proposal. Corner example: Add a labeled expected-placement diagram beside it.
  Evidence: The Corner trigger is closed, so horizontal clamping is not demonstrated in the capture.

- [ ] `menu_showcase-16` · P3 · proposal. Empty canvas: Use it for a compact placement diagram connecting top, bottom, and corner cases.
  Evidence: Most of the central viewport is unused.

- [ ] `menu_showcase-17` · P3 · proposal. Trigger alignment: Add an unobtrusive baseline guide to emphasize their common edge position.
  Evidence: The bottom-left and bottom-right triggers have no shared footer frame.

- [ ] `menu_showcase-18` · P2 · proposal. Sample identification: Add a short Live menu examples label above the trigger area.
  Evidence: The page does not distinguish live triggers from explanatory text.

### meters_gauges

[Baseline](screenshot-baselines/screens/meters_gauges_720p.png) · [Visual review](design-audit.html#screen=meters_gauges)

- [ ] `meters_gauges-01` · P2 · defect. Stacked Bars heading: Rename the section Paired bars or add an actual stacked companion example.
  Evidence: Health and Shield are side by side while the heading calls them Stacked Bars.

- [ ] `meters_gauges-02` · P2 · proposal. Inline abbreviations: Add Health, Mana, and Experience captions.
  Evidence: HP, MP, and XP have no expanded names.

- [ ] `meters_gauges-03` · P2 · proposal. Inline meter colors: Give each metric a labeled identity color or explain the common theme treatment.
  Evidence: All three bars use the same blue fill and green remainder.

- [ ] `meters_gauges-04` · P2 · proposal. Track color: Use a neutral remainder track and reserve color for the filled portion.
  Evidence: Green remainder segments look like completed progress as strongly as blue fills.

- [ ] `meters_gauges-05` · P2 · proposal. Inline percentages: Move values to a fixed trailing value column.
  Evidence: 65%, 39%, and 84% sit at the left of their fills.

- [ ] `meters_gauges-06` · P2 · proposal. Inline row grouping: Increase inter-meter group spacing and tighten each label-to-bar gap.
  Evidence: HP, MP, and XP labels sit close to the preceding bar.

- [ ] `meters_gauges-07` · P2 · proposal. Health label: Place label and value outside the bar.
  Evidence: Health: 65% is embedded in the track.

- [ ] `meters_gauges-08` · P2 · proposal. Shield label: Use the same external label/value pattern as Health.
  Evidence: Shield: 29% is embedded in its smaller filled region.

- [ ] `meters_gauges-09` · P3 · proposal. Health-shield relationship: Add a short caption explaining independent health and shield values.
  Evidence: The heading joins the two metrics but gives no shared total or relation.

- [ ] `meters_gauges-10` · P2 · proposal. Animated value: Keep the animated percentage in a stable field beside the bar.
  Evidence: 1% sits in a tiny sliver of blue fill.

- [ ] `meters_gauges-11` · P2 · proposal. Animation spinner: Label it as a spinner or circular progress companion according to its role.
  Evidence: An unlabeled ring sits next to Animated Progress.

- [ ] `meters_gauges-12` · P3 · proposal. Animation timing: Add a small 0–100% looping caption with duration.
  Evidence: The screenshot does not describe the animation cycle.

- [ ] `meters_gauges-13` · P3 · proposal. Animated sample width: Align the pair within a compact sample frame.
  Evidence: The long bar and small ring leave a large blank right region.

- [ ] `meters_gauges-14` · P2 · proposal. Gauge labels: Add meaningful sample names or Low, Half, and High labels.
  Evidence: The circular gauges show only 25%, 50%, and 90%.

- [ ] `meters_gauges-15` · P2 · proposal. Gauge value placement: Move the percentages into the centers for a tighter readout.
  Evidence: Values sit below empty ring centers.

- [ ] `meters_gauges-16` · P2 · proposal. Gauge track contrast: Increase track contrast while preserving colored arcs.
  Evidence: The unfilled ring segments blend into the navy panel.

- [ ] `meters_gauges-17` · P3 · proposal. Gauge color semantics: Add a threshold legend outside the gauges.
  Evidence: Red, gold, and green suggest thresholds without naming them.

- [ ] `meters_gauges-18` · P3 · proposal. Gauge spacing: Bring them closer into a comparison group.
  Evidence: Three small rings are spread across almost the entire panel.

- [ ] `meters_gauges-19` · P2 · proposal. Interactive control label: Name the controlled metric and list linked examples.
  Evidence: Value: 65% does not say which meters the control affects.

- [ ] `meters_gauges-20` · P2 · proposal. Slider thumb: Use a contrasting thumb outline and a distinct handle shape.
  Evidence: The blue thumb sits on a blue-and-green track with weak separation.

- [ ] `meters_gauges-21` · P2 · proposal. Slider anatomy: Use a thinner track and explicit drag handle to distinguish input from output.
  Evidence: The interactive slider resembles the progress bars above it.

- [ ] `meters_gauges-22` · P3 · proposal. Slider endpoints: Add 0% and 100% endpoint captions.
  Evidence: The control has no minimum or maximum labels.

- [ ] `meters_gauges-23` · P2 · proposal. Section typography: Increase their size and use consistent left insets.
  Evidence: Section labels are small and widely spaced compared with available panel area.

- [ ] `meters_gauges-24` · P3 · proposal. Panel heights: Reduce decorative vertical padding and allocate more room to labels.
  Evidence: Large dark panels contain small low-height examples.

- [ ] `meters_gauges-25` · P3 · proposal. Demo footer: Add a compact setup summary below the interactive control.
  Evidence: No legend explains shared values or theme behavior.

### minesweeper_lab

[Baseline](screenshot-baselines/screens/minesweeper_lab_720p.png) · [Visual review](design-audit.html#screen=minesweeper_lab)

- [ ] `minesweeper_lab-01` · P2 · proposal. Mine counter: Add a small Mines remaining caption in the status area or beside the display.
  Evidence: The red 038 counter has no label.

- [ ] `minesweeper_lab-02` · P2 · proposal. Timer: Add Time · seconds beneath or above the timer.
  Evidence: The 000 display has no time unit or label.

- [ ] `minesweeper_lab-03` · P2 · proposal. Counter typography: Tune segment spacing so each digit is distinct while preserving the digital style.
  Evidence: The thick pixel digits make 038 visually dense.

- [ ] `minesweeper_lab-04` · P2 · proposal. Reset face: Add a small New game caption near the face control.
  Evidence: The smiley button has no visible reset label.

- [ ] `minesweeper_lab-05` · P2 · proposal. Difficulty identity: Show Intermediate · 16 × 16 in the game status area.
  Evidence: The 16-by-16 board is visible but the difficulty is unnamed.

- [ ] `minesweeper_lab-06` · P2 · proposal. Starting state: Mark the baseline as a prepared sample state in the demo frame.
  Evidence: Some cells are revealed and two flags are placed while the timer reads zero.

- [ ] `minesweeper_lab-07` · P2 · proposal. Status instruction: Increase status text readability within the classic status bar.
  Evidence: Left click to reveal. Right click to mark a mine uses small text at the bottom.

- [ ] `minesweeper_lab-08` · P2 · proposal. Flagging wording: Use Place or remove a flag in the instruction.
  Evidence: Mark a mine could imply certainty rather than a player annotation.

- [ ] `minesweeper_lab-09` · P2 · proposal. Board coordinates: Offer subtle external coordinate labels for demo inspection.
  Evidence: The dense grid has no row or column reference.

- [ ] `minesweeper_lab-10` · P3 · proposal. Flag glyphs: Increase flag silhouette clarity within the existing cell size.
  Evidence: Tiny red flags have little separation from the dark mast.

- [ ] `minesweeper_lab-11` · P3 · proposal. Revealed numbers: Increase numeral weight while keeping classic number colors.
  Evidence: Blue 1 and green 2 are thin compared with the surrounding bevels.

- [ ] `minesweeper_lab-12` · P2 · proposal. Revealed-cell state: Clarify the exposed-cell fill without changing covered-cell bevels.
  Evidence: Empty revealed cells and numbered cells use almost the same flat gray as recessed boundaries.

- [ ] `minesweeper_lab-13` · P3 · proposal. Cell bevel weight: Slightly reduce bevel contrast while preserving the retro grid structure.
  Evidence: Bright repeated bevels dominate the board more than the few revealed numbers.

- [ ] `minesweeper_lab-14` · P3 · proposal. Board outer frame: Reduce one redundant outer inset while retaining the classic sunken frame.
  Evidence: Several nested bevels consume space around the cells.

- [ ] `minesweeper_lab-15` · P2 · proposal. Game menu: Use consistent menu-item padding and readable text size.
  Evidence: Game and Help are small and closely packed.

- [ ] `minesweeper_lab-16` · P2 · proposal. Window title: Increase title text slightly and align it cleanly with the app icon.
  Evidence: Minesweeper is small within the blue title bar.

- [ ] `minesweeper_lab-17` · P2 · proposal. Maximize control: Render a clear maximize glyph at the pixel grid.
  Evidence: The middle window control appears as an almost blank square.

- [ ] `minesweeper_lab-18` · P3 · proposal. Close glyph: Center and enlarge the glyph within the classic control.
  Evidence: The x is tiny compared with its gray button.

- [ ] `minesweeper_lab-19` · P3 · proposal. Window-control spacing: Add one consistent pixel-grid gap between them.
  Evidence: The minimize, maximize, and close buttons nearly touch.

- [ ] `minesweeper_lab-20` · P3 · proposal. Desktop icon label: Use the same retro bitmap text style as the desktop shell.
  Evidence: My Computer uses a different handwritten-looking face from the window chrome.

- [ ] `minesweeper_lab-21` · P3 · proposal. Desktop icon scale: Balance icon scale with the centered label width.
  Evidence: The monitor icon is small compared with the label's wide footprint.

- [ ] `minesweeper_lab-22` · P3 · proposal. Taskbar app button: Use a width closer to the app label or show active-window emphasis more clearly.
  Evidence: The Minesweeper task button leaves a large unused interior span.

- [ ] `minesweeper_lab-23` · P3 · proposal. Taskbar clock: Tighten horizontal padding to match the classic clock treatment.
  Evidence: 12:00 PM sits in a wide recessed region.

- [ ] `minesweeper_lab-24` · P2 · proposal. Game progress: Add a compact safe-cells-remaining readout as optional demo metadata.
  Evidence: The status area shows input instructions but no revealed-cell progress.

- [ ] `minesweeper_lab-25` · P3 · proposal. Desktop empty space: Use a restrained external demo caption to explain the prepared game state without crowding the board.
  Evidence: The centered window leaves broad blank teal fields.

### mini_motorways_settings

[Baseline](screenshot-baselines/screens/mini_motorways_settings_720p.png) · [Visual review](design-audit.html#screen=mini_motorways_settings)

- [ ] `mini_motorways_settings-01` · P2 · proposal. Inactive navigation: Darken the text or backing to improve contrast.
  Evidence: Video, Audio, and other white labels sit on pale cyan blocks.

- [ ] `mini_motorways_settings-02` · P2 · proposal. Selected Game tab: Use dark selected-tab text while retaining the yellow accent.
  Evidence: White Game text sits on a light yellow block.

- [ ] `mini_motorways_settings-03` · P2 · proposal. Navigation widths: Give the tabs a consistent alignment edge and controlled padding.
  Evidence: Each tab block follows its text width and begins at a different x position.

- [ ] `mini_motorways_settings-04` · P2 · proposal. Language tab: Use a shared navigation width so long labels do not alter the column silhouette.
  Evidence: Language extends far left relative to Video and Audio.

- [ ] `mini_motorways_settings-05` · P2 · proposal. Cross-Save label: Use a consistent text size and add a short account-sync subtitle if needed.
  Evidence: Cross-Save is a wide technical term among shorter category names.

- [ ] `mini_motorways_settings-06` · P2 · proposal. Back icon: Add a Back label or frame it as a consistent navigation control.
  Evidence: A large bare arrow floats in the upper-left corner.

- [ ] `mini_motorways_settings-07` · P2 · proposal. Toggle semantics: Add explicit Off text so the X is not mistaken for an error or close action.
  Evidence: Night Mode and Vibration use an X inside a circle for Off.

- [ ] `mini_motorways_settings-08` · P2 · proposal. Enabled toggle: Pair the check with On using the same state layout as disabled rows.
  Evidence: Hold to Draw/Delete uses a checkmark without an On label.

- [ ] `mini_motorways_settings-09` · P2 · proposal. Toggle sizing: Reduce circles slightly and align them in a shared trailing column.
  Evidence: Large circular toggles compete with setting names.

- [ ] `mini_motorways_settings-10` · P2 · proposal. Hold label: Use Hold to draw or delete with a short explanation below.
  Evidence: Hold to Draw/Delete combines two actions in a long phrase.

- [ ] `mini_motorways_settings-11` · P2 · proposal. Night Mode description: Add a brief preview or description of the darker palette.
  Evidence: Night Mode has no indication of what changes.

- [ ] `mini_motorways_settings-12` · P2 · proposal. Vibration context: Add a compact Controller feedback subtitle.
  Evidence: Vibration has no note about controller applicability.

- [ ] `mini_motorways_settings-13` · P2 · proposal. Sensitivity typography: Use a consistent setting-label scale.
  Evidence: Controller Cursor Sensitivity is much smaller than adjacent setting labels.

- [ ] `mini_motorways_settings-14` · P2 · proposal. Sensitivity layout: Bring arrows closer to the value in a compact selector.
  Evidence: The value Default and large arrows span a very wide row.

- [ ] `mini_motorways_settings-15` · P2 · proposal. Sensitivity value meaning: Show Default · 1.0× or another meaningful scale if supported.
  Evidence: Default gives no measurable or relative value.

- [ ] `mini_motorways_settings-16` · P3 · proposal. Selector arrows: Use lighter chevrons with consistent button bounds.
  Evidence: Solid triangles dominate the sensitivity row.

- [ ] `mini_motorways_settings-17` · P2 · proposal. Separator: Align it to the full settings column width.
  Evidence: The horizontal rule under sensitivity ends before the rightmost controls.

- [ ] `mini_motorways_settings-18` · P2 · proposal. Colorblind Mode label: Add a short palette preview beside the setting.
  Evidence: A single binary label does not explain the alternative palette.

- [ ] `mini_motorways_settings-19` · P2 · proposal. Content heading: Add a small heading aligned with the first row.
  Evidence: The right-side settings have no Game settings heading.

- [ ] `mini_motorways_settings-20` · P3 · proposal. Row spacing: Use a consistent vertical rhythm while keeping a deliberate accessibility group break.
  Evidence: The gap around sensitivity is larger than between the toggles above.

- [ ] `mini_motorways_settings-21` · P2 · proposal. Tutorial action: Increase contrast and give it a clear secondary-action treatment.
  Evidence: Tutorial is large white text on pale cyan near the bottom edge.

- [ ] `mini_motorways_settings-22` · P3 · proposal. Tutorial arrow: Use equal horizontal padding around label and arrow.
  Evidence: The large arrow is close to the trailing edge.

- [ ] `mini_motorways_settings-23` · P2 · proposal. Version footer: Reduce its emphasis and shorten the default visible version string.
  Evidence: A long release identifier occupies prominent bottom-left space.

- [ ] `mini_motorways_settings-24` · P3 · proposal. Road decoration: Lower its prominence where it intersects tab backgrounds.
  Evidence: The white road bends directly behind the navigation column.

- [ ] `mini_motorways_settings-25` · P3 · proposal. Background grid: Keep grid detail quieter behind the text column.
  Evidence: Faint grid lines pass through every setting row.

### modals

[Baseline](screenshot-baselines/screens/modals_720p.png) · [Visual review](design-audit.html#screen=modals)

- [ ] `modals-01` · P2 · proposal. Page title: Add Choose a dialog example and inspect its result beneath the title.
  Evidence: Modal Dialogs is the only explanation of the page.

- [ ] `modals-02` · P2 · proposal. Basic category: Add a short description under each launcher.
  Evidence: Basic does not explain the difference between Simple and Composable.

- [ ] `modals-03` · P2 · proposal. Simple Modal label: Add Plain message dialog as its specimen subtitle.
  Evidence: The launcher names the implementation rather than the visible behavior.

- [ ] `modals-04` · P2 · proposal. Composable Modal label: Add Form controls with Save and Cancel beneath the label.
  Evidence: Composable is technical without describing its content.

- [ ] `modals-05` · P2 · proposal. Helpers category: Rename it Standard dialog types.
  Evidence: Helpers is an API term without user-visible meaning.

- [ ] `modals-06` · P2 · proposal. Info Dialog and Notice: Add brief purpose captions for informational acknowledgment and notice.
  Evidence: The two launchers have similar names without distinguishing behavior.

- [ ] `modals-07` · P2 · proposal. Confirmation color: Use a neutral confirmation style or label the destructive scenario.
  Evidence: Confirmation uses dark red although no destructive action is named.

- [ ] `modals-08` · P2 · proposal. Launcher palette: Use consistent launch-button styling and identify variants through labels.
  Evidence: Cyan, purple, and red fills have no legend.

- [ ] `modals-09` · P2 · proposal. Button labels: Increase readable label size and reduce tracking.
  Evidence: Small widely spaced text sits inside broad rectangular buttons.

- [ ] `modals-10` · P2 · proposal. Button heights: Increase vertical padding within each control.
  Evidence: Launch controls are low relative to their wide row panels.

- [ ] `modals-11` · P2 · proposal. Category column: Use a fixed left category column and align every action group.
  Evidence: Basic, Helpers, Stacking, and Blocking share space with long action names.

- [ ] `modals-12` · P2 · proposal. Stacking action: Use Open settings and add Then open reset confirmation as a caption.
  Evidence: Settings + Nested Confirm compresses two steps into one label.

- [ ] `modals-13` · P3 · proposal. Stacking visualization: Add a small two-layer thumbnail beside the launcher.
  Evidence: The row has no visual indication of two dialog layers.

- [ ] `modals-14` · P2 · proposal. Blocking category: Rename it Background input blocking test.
  Evidence: Blocking does not say what is being tested.

- [ ] `modals-15` · P2 · proposal. Background Button: Add an instruction explaining the intended click-count experiment.
  Evidence: The label does not tell the user to compare its count with a modal open.

- [ ] `modals-16` · P2 · proposal. Clicks counter: Use a compact Background clicks badge adjacent to the button.
  Evidence: Clicks: 0 is left-aligned in an isolated dark rectangle.

- [ ] `modals-17` · P2 · proposal. Results strip: Use separate labeled result fields.
  Evidence: Confirm: Pending, FYI: Pending, OK:0, and X:0 run together.

- [ ] `modals-18` · P2 · proposal. FYI result name: Use the same name in launcher and result display.
  Evidence: FYI does not match the visible Notice launcher.

- [ ] `modals-19` · P2 · proposal. OK and X counters: Label them Acknowledged and Dismissed or the actual events.
  Evidence: The counters have no description of what they count.

- [ ] `modals-20` · P2 · proposal. Pending state: Use Not opened for untouched examples if accurate.
  Evidence: Pending suggests an active request although no dialog is open.

- [ ] `modals-21` · P2 · proposal. Default capture: Add a labeled static preview alongside the launchers or capture a complementary open state.
  Evidence: The screenshot shows no actual dialog specimen.

- [ ] `modals-22` · P3 · proposal. Wide empty rows: Use the spare width for short expected-behavior captions.
  Evidence: Each row extends far beyond its controls.

- [ ] `modals-23` · P3 · proposal. Vertical whitespace: Tighten section spacing to create one coherent gallery.
  Evidence: Large black gaps separate identical launcher panels.

- [ ] `modals-24` · P3 · proposal. Results association: Add matching labels or small result badges beside each relevant launcher.
  Evidence: The status strip is detached from the launchers that populate it.

- [ ] `modals-25` · P3 · proposal. Reset experiment: Add a compact Reset results action in the result strip.
  Evidence: The page has no visible way to restore result counters.

### multiline_text_lab

[Baseline](screenshot-baselines/screens/multiline_text_lab_720p.png) · [Visual review](design-audit.html#screen=multiline_text_lab)

- [ ] `multiline_text_lab-01` · P2 · proposal. Title: Add Text and measured bounds should agree as a subtitle.
  Evidence: Multi-line Text Lab does not state the success criterion.

- [ ] `multiline_text_lab-02` · P2 · proposal. Case captions: Use a larger neutral caption face outside the test boxes.
  Evidence: Small handwritten captions are difficult to scan.

- [ ] `multiline_text_lab-03` · P2 · proposal. Hard-break terminology: Label them Explicit line breaks and retain the escape in metadata.
  Evidence: The first two captions use a literal newline escape without a plain-language label.

- [ ] `multiline_text_lab-04` · P2 · proposal. Auto-fit sample: Add the resulting font size in an external value label.
  Evidence: Auto-fit size gives no rendered font size.

- [ ] `multiline_text_lab-05` · P2 · proposal. Fixed-font sample: Show Fixed font size · 18 px.
  Evidence: font_size 18 mixes API syntax with human-facing metadata.

- [ ] `multiline_text_lab-06` · P3 · proposal. Hard-break comparison: Add Same text, different sizing modes above the pair.
  Evidence: The two top samples have no shared group title.

- [ ] `multiline_text_lab-07` · P2 · proposal. Fixed-wrap dimensions: Show Width 280 px · Height 110 px in the caption.
  Evidence: 280x110 uses cramped dimension formatting.

- [ ] `multiline_text_lab-08` · P2 · proposal. Self-sizing label: Use Content-derived height with the API name in smaller text.
  Evidence: Dim::Text appears without explanation.

- [ ] `multiline_text_lab-09` · P3 · proposal. Wrapped-line counts: Add external Rendered lines: 3 values.
  Evidence: The fixed and self-sized samples show three lines without a line-count label.

- [ ] `multiline_text_lab-10` · P3 · proposal. Self-sized bounds: Add an external measured-height guide for easier inspection of the intended fit.
  Evidence: The short right box ends close to its final line.

- [ ] `multiline_text_lab-11` · P2 · proposal. Styled-run legend: Add a small diff-color key outside the sample.
  Evidence: Red removed, green added, and gray context text have no external legend.

- [ ] `multiline_text_lab-12` · P3 · proposal. Styled-run font: Add a monospaced companion specimen while preserving the existing test.
  Evidence: The code-like diff sample uses the same handwritten face as prose.

- [ ] `multiline_text_lab-13` · P2 · proposal. Blank-line caption: Label it Two line breaks preserve one blank line.
  Evidence: The preserved blank-line explanation uses tiny escape notation.

- [ ] `multiline_text_lab-14` · P3 · proposal. Blank-line measurement: Add a faint external line-height bracket beside the blank line.
  Evidence: The gap between above and below is not quantified.

- [ ] `multiline_text_lab-15` · P2 · proposal. Resize instruction: Increase the instruction and show a small resize-arrow glyph.
  Evidence: Drag the corner handle appears in small text above the panel.

- [ ] `multiline_text_lab-16` · P2 · proposal. Resize handle: Add diagonal grip marks inside the existing handle bounds.
  Evidence: The cyan rounded square looks like a generic button.

- [ ] `multiline_text_lab-17` · P2 · proposal. Dynamic dimensions: Show Width and Height as separate live value badges.
  Evidence: 300 × 120 appears in parentheses inside the instruction.

- [ ] `multiline_text_lab-18` · P2 · proposal. Comparison distance: Bring its explanatory caption closer or connect the two with a labeled comparison line.
  Evidence: The same-width green sample sits far to the right of the draggable panel.

- [ ] `multiline_text_lab-19` · P3 · proposal. Comparison alignment: Add a faint alignment guide outside their bounds.
  Evidence: The draggable and content-height samples share a top edge without an explicit guide.

- [ ] `multiline_text_lab-20` · P3 · proposal. Green sample meaning: Label green as the content-derived-height reference.
  Evidence: Only the right sample uses green, without a color key.

- [ ] `multiline_text_lab-21` · P2 · proposal. Diagnostic footer: Move them into a readable diagnostic strip with bottom padding.
  Evidence: Mouse, handle, hot, held, and drags values are tiny at the bottom edge.

- [ ] `multiline_text_lab-22` · P2 · proposal. Boolean diagnostics: Display Hot: No and Dragging: No.
  Evidence: hot=n and held=n require decoding.

- [ ] `multiline_text_lab-23` · P2 · proposal. Handle geometry diagnostic: Separate X, Y, Width, and Height fields.
  Evidence: 320,612 28x28 combines position and size without column labels.

- [ ] `multiline_text_lab-24` · P3 · proposal. Case identification: Number the examples to support visual-regression discussions.
  Evidence: The eight specimens have no stable case numbers.

- [ ] `multiline_text_lab-25` · P3 · proposal. Right upper empty space: Place a compact expected line-count and sizing-mode summary there.
  Evidence: The upper-right region is unused.

### navigation_bar_demo

[Baseline](screenshot-baselines/screens/navigation_bar_demo_720p.png) · [Visual review](design-audit.html#screen=navigation_bar_demo)

- [ ] `navigation_bar_demo-01` · P2 · proposal. Title: Use Cycle selectors with the component name in a subtitle.
  Evidence: Navigation Bar Component Showcase is a long technical heading.

- [ ] `navigation_bar_demo-02` · P2 · proposal. Settings category count: Add a current-position value such as 1 of N.
  Evidence: Graphics appears without the total number of categories.

- [ ] `navigation_bar_demo-03` · P2 · proposal. Game mode count: Add an option count or short adjacent option preview.
  Evidence: Arcade gives no clue to the other available modes.

- [ ] `navigation_bar_demo-04` · P2 · proposal. Level position: Show Level 1 of 6 inside the selector.
  Evidence: The caption says 6 options while the bar says Level 1.

- [ ] `navigation_bar_demo-05` · P2 · proposal. Arrow grouping: Use bounded arrow buttons around a more compact value field.
  Evidence: Chevrons sit far from their centered values.

- [ ] `navigation_bar_demo-06` · P2 · proposal. Arrow affordance: Add subtle separators around the arrow buttons.
  Evidence: Chevrons share the same blue surface as the value.

- [ ] `navigation_bar_demo-07` · P2 · proposal. Section labels: Increase label contrast and use consistent naming.
  Evidence: Settings Categories and Game Mode Selector are small and muted.

- [ ] `navigation_bar_demo-08` · P3 · proposal. Graphics context: Add a Selected category: Graphics result line.
  Evidence: No selected-category result appears below Graphics.

- [ ] `navigation_bar_demo-09` · P3 · proposal. Arcade meaning: Add a short description below the selector.
  Evidence: The mode name has no description.

- [ ] `navigation_bar_demo-10` · P3 · proposal. Level identity: Add a sample level name or thumbnail.
  Evidence: Level 1 is generic despite the wide bar.

- [ ] `navigation_bar_demo-11` · P2 · proposal. Instruction: Add Use the arrows to cycle through each example.
  Evidence: No visible text explains the selectors.

- [ ] `navigation_bar_demo-12` · P3 · proposal. Boundary behavior: Add Wraps or Stops at ends matching the configuration.
  Evidence: The screenshot does not state whether options wrap.

- [ ] `navigation_bar_demo-13` · P2 · proposal. Control widths: Reduce their widths for easier visual association.
  Evidence: Wide bars contain only a short value and chevrons.

- [ ] `navigation_bar_demo-14` · P3 · proposal. Top empty area: Move the group upward or center the whole group vertically.
  Evidence: A large blank navy area precedes the title.

- [ ] `navigation_bar_demo-15` · P3 · proposal. Right empty area: Center them with equal side margins.
  Evidence: The controls are left-shifted inside the panel.

- [ ] `navigation_bar_demo-16` · P3 · proposal. Value typography: Use a clearer value face while retaining informal headings.
  Evidence: Graphics, Arcade, and Level 1 use handwritten text.

### neon_strike

[Baseline](screenshot-baselines/screens/neon_strike_720p.png) · [Visual review](design-audit.html#screen=neon_strike)

- [ ] `neon_strike-01` · P2 · proposal. Grenade count: Place both counts at the same lower-right inset inside their tiles.
  Evidence: The grenade count sits outside its tile while the knife count sits inside; the web mock repeats this asymmetry.

- [ ] `neon_strike-02` · P2 · proposal. Equipment key hints: Increase their size and use compact keycaps.
  Evidence: G and V are tiny in the tile corners.

- [ ] `neon_strike-03` · P2 · proposal. Equipment icons: Normalize optical icon weight within the HUD.
  Evidence: Grenade and knife line weights differ from the heavier streak icons.

- [ ] `neon_strike-04` · P2 · proposal. Selected equipment: Add a small selected marker or brighter bottom rule.
  Evidence: A thin gold outline is the grenade's only selection cue.

- [ ] `neon_strike-05` · P2 · proposal. Ammo readout: Emphasize 35 and reduce reserve-count weight.
  Evidence: 35/210 gives magazine and reserve counts similar emphasis.

- [ ] `neon_strike-06` · P2 · proposal. Weapon identity: Add a small current-weapon label above it.
  Evidence: The ammo readout has no weapon name.

- [ ] `neon_strike-07` · P2 · proposal. Health label: Increase health text and reduce skull emphasis.
  Evidence: 80 HEALTH is tiny beside a large skull tile.

- [ ] `neon_strike-08` · P2 · proposal. Armor label: Increase text and place it beside its own bar.
  Evidence: 45% ARMOR is extremely small below two thin bars.

- [ ] `neon_strike-09` · P2 · proposal. Health and armor bars: Give each bar a directly adjacent label or icon.
  Evidence: Both bars use similar cyan fills without direct labels.

- [ ] `neon_strike-10` · P2 · proposal. Health scale: Show 80/100 or the actual maximum.
  Evidence: 80 is shown without a maximum.

- [ ] `neon_strike-11` · P2 · proposal. Map caption: Move them into a readable map footer.
  Evidence: SECTOR 7B and GRID 042 are tiny along the bottom edge.

- [ ] `neon_strike-12` · P2 · proposal. Map markers: Add a compact ally-marker key near the map.
  Evidence: Cyan rings appear without a legend.

- [ ] `neon_strike-13` · P2 · proposal. Objective marker: Increase its contrast and label its objective role.
  Evidence: B is small inside a thin amber region.

- [ ] `neon_strike-14` · P3 · proposal. Player direction: Strengthen the triangle outline and soften the cone.
  Evidence: The white triangle overlaps a tan view cone.

- [ ] `neon_strike-15` · P2 · proposal. Killstreak header: Label them as charge/progress or use a clearer segmented meter.
  Evidence: Three squares follow KILLSTREAK without explanation.

- [ ] `neon_strike-16` · P2 · proposal. Streak hotkeys: Use readable, consistently aligned key badges.
  Evidence: 1 through 4 are tiny outside the ability tiles.

- [ ] `neon_strike-17` · P3 · proposal. Gear decorations: Remove repeated gears or label their configuration role.
  Evidence: A gear repeats beside each ability without explanation.

- [ ] `neon_strike-18` · P2 · proposal. Ability labels: Increase their size and reduce tracking.
  Evidence: UAV, RECON, SHIELD, and STRIKE are tiny.

- [ ] `neon_strike-19` · P2 · proposal. Strike icon: Use a recognizable strike symbol distinct from close or cancel.
  Evidence: A large X represents STRIKE.

- [ ] `neon_strike-20` · P2 · proposal. Objective banner: Add trailing padding inside a stable safe area.
  Evidence: SECURE OBJECTIVE B sits tight against the right edge.

- [ ] `neon_strike-21` · P2 · proposal. Score shield: Separate score from protection status or explain the symbol.
  Evidence: An unlabeled shield follows SCORE: 15,000.

- [ ] `neon_strike-22` · P2 · proposal. Kill feed: Distinguish actor, action, and target and increase line spacing.
  Evidence: Three dense text lines sit beneath the compass.

- [ ] `neon_strike-23` · P3 · proposal. Repeated alert: Differentiate radio activity from event history with labels.
  Evidence: Alpha_Six Under fire appears in both feed and voice area.

- [ ] `neon_strike-24` · P2 · proposal. Compass labels: Increase compass text while retaining thin guide lines.
  Evidence: Bearing values and direction letters are very small.

- [ ] `neon_strike-25` · P3 · proposal. Background flecks: Reduce their density behind peripheral readouts.
  Evidence: Gold flecks remain visible behind HUD text.

### nine_slice_borders

[Baseline](screenshot-baselines/screens/nine_slice_borders_720p.png) · [Visual review](design-audit.html#screen=nine_slice_borders)

- [ ] `nine_slice_borders-01` · P2 · proposal. Title frame: Use a simpler heading treatment.
  Evidence: The full-width white frame outweighs small border specimens.

- [ ] `nine_slice_borders-02` · P2 · proposal. Panel names: Add texture IDs beneath the samples.
  Evidence: Simple, Ornate, and Heavy omit source identifiers.

- [ ] `nine_slice_borders-03` · P3 · proposal. Slice parameters: Add a shared slice-size caption.
  Evidence: The first row does not state corner-slice dimensions.

- [ ] `nine_slice_borders-04` · P2 · proposal. Corner details: Add a magnified corner inset beside the row.
  Evidence: Ornate and Double Line corner details are small.

- [ ] `nine_slice_borders-05` · P3 · proposal. Thick sample: Show its border or slice width externally.
  Evidence: Thick gives no border measurement.

- [ ] `nine_slice_borders-06` · P2 · proposal. Decorative and Heavy: Add optional content-inset guides for comparison.
  Evidence: Broad gold patterns reduce the usable white center.

- [ ] `nine_slice_borders-07` · P2 · proposal. Border-only grouping: Separate the transparency modes with subgroup captions.
  Evidence: Border A–C have dark centers while Trans A–B have white backing.

- [ ] `nine_slice_borders-08` · P2 · proposal. Border A B C names: Add texture names or short corner descriptions.
  Evidence: Letter names do not explain corner differences.

- [ ] `nine_slice_borders-09` · P2 · proposal. Trans labels: Spell out Transparent border in external captions.
  Evidence: Trans abbreviates the tested behavior.

- [ ] `nine_slice_borders-10` · P3 · proposal. Transparency proof: Add checkerboard companion backgrounds.
  Evidence: Flat backgrounds make transparent regions hard to identify.

- [ ] `nine_slice_borders-11` · P2 · proposal. Size metadata: Add width × height captions.
  Evidence: Small, Medium, and Large have no dimensions.

- [ ] `nine_slice_borders-12` · P3 · proposal. Size isolation: Add a same-source comparison across every size.
  Evidence: Different styles appear within the stretching row.

- [ ] `nine_slice_borders-13` · P2 · proposal. Wide panel copy: Move usage advice below the frame.
  Evidence: Good for menus is packed inside the specimen label.

- [ ] `nine_slice_borders-14` · P2 · proposal. Tall alignment: Use a shared caption baseline or consistent sample framing.
  Evidence: Tall begins above the other size samples.

- [ ] `nine_slice_borders-15` · P3 · proposal. Tall caption: Use uniform external caption sizes.
  Evidence: Tall uses much smaller type than neighboring names.

- [ ] `nine_slice_borders-16` · P2 · proposal. Double-width grouping: Add separate Width and Tint subgroup labels.
  Evidence: Border width and tint share one heading.

- [ ] `nine_slice_borders-17` · P2 · proposal. Double-width spacing: Increase gaps around these specimens.
  Evidence: Thick decorations nearly meet adjacent samples.

- [ ] `nine_slice_borders-18` · P3 · proposal. Double metadata: Caption the 32-pixel slice parameter.
  Evidence: Double omits its actual slice size.

- [ ] `nine_slice_borders-19` · P3 · proposal. Tint values: Add exact tint values below them.
  Evidence: Gold, Blue, Red, and Green are names without values.

- [ ] `nine_slice_borders-20` · P2 · proposal. Tint text adaptation: Explain that text contrast adapts separately.
  Evidence: Text colors vary independently of border tint.

- [ ] `nine_slice_borders-21` · P2 · proposal. Interactive button: Add an external expected-response caption.
  Evidence: Click Me! does not say what response to inspect.

- [ ] `nine_slice_borders-22` · P2 · proposal. Dialog specimen: Add a clear dialog title and external border caption.
  Evidence: The small corners-stay-crisp sentence is buried in a large pale box.

- [ ] `nine_slice_borders-23` · P2 · proposal. Nested labels: Use Gold child and Blue child.
  Evidence: Inner and Panel do not distinguish the two children.

- [ ] `nine_slice_borders-24` · P3 · proposal. Nested parent border: Add a companion contrast backdrop without changing the texture.
  Evidence: The pale outer border blends with its fill.

- [ ] `nine_slice_borders-25` · P3 · proposal. Bottom margin: Add a consistent gallery bottom margin.
  Evidence: The last examples end close to the viewport edge.

### offsite_backup

[Baseline](screenshot-baselines/screens/offsite_backup_720p.png) · [Visual review](design-audit.html#screen=offsite_backup)

- [ ] `offsite_backup-01` · P2 · defect. Summary versus folders: Derive totals and ring progress from the same folder data or label differing scopes explicitly.
  Evidence: 493 GB uploaded is reported, but visible folder sizes and percentages imply about 242 GB uploaded.

- [ ] `offsite_backup-02` · P2 · proposal. Backup ring: Increase the caption and center the value group.
  Evidence: 63% has a tiny COMPLETE caption.

- [ ] `offsite_backup-03` · P2 · proposal. Pause backup: Use a readable secondary button with a pause icon.
  Evidence: Pause backup is tiny and pale.

- [ ] `offsite_backup-04` · P2 · proposal. Transfer speed: Increase contrast and emphasize the value.
  Evidence: 24.8 MB/s is faint in the transfer row.

- [ ] `offsite_backup-05` · P2 · proposal. Remaining time: Give the estimate a stronger value treatment.
  Evidence: About 3 hours remaining is low-contrast metadata.

- [ ] `offsite_backup-06` · P2 · proposal. Healthy badge: Use a recognizable status dot and readable text.
  Evidence: Healthy has tiny text and a square marker.

- [ ] `offsite_backup-07` · P2 · proposal. Plan subtitle: Increase readability and use a centered-dot separator.
  Evidence: Personal Backup - 2 TB plan is very small.

- [ ] `offsite_backup-08` · P2 · proposal. Folder table headings: Add Progress and Size headings.
  Evidence: FOLDERS is the only heading above progress, state, and size columns.

- [ ] `offsite_backup-09` · P2 · proposal. Completed state: Use Done with a check icon consistently.
  Evidence: done is plain lowercase text beside green bars.

- [ ] `offsite_backup-10` · P2 · proposal. Uploading state: Mark the active upload and identify queued folders if applicable.
  Evidence: Photos, Music, and Archive have no activity distinction.

- [ ] `offsite_backup-11` · P2 · proposal. Folder sizes: Increase their size and retain right alignment.
  Evidence: Size values are tiny and pale.

- [ ] `offsite_backup-12` · P2 · proposal. Rescan: Use a readable refresh action with an icon.
  Evidence: Rescan is tiny red text.

- [ ] `offsite_backup-13` · P2 · proposal. Schedule selectors: Add visible select affordances.
  Evidence: Every 4 hours and 1 year lack chevrons or strong boundaries.

- [ ] `offsite_backup-14` · P2 · proposal. Schedule labels: Increase contrast and align both rows.
  Evidence: Run backup and Keep file versions are faint.

- [ ] `offsite_backup-15` · P2 · proposal. Bandwidth meaning: Pair it directly with the actual 24.8 MB/s cap.
  Evidence: 62% does not identify the full-speed reference.

- [ ] `offsite_backup-16` · P2 · proposal. Bandwidth thumb: Add a contrasting border and larger grip.
  Evidence: The small coral thumb blends into the fill.

- [ ] `offsite_backup-17` · P2 · proposal. Slider endpoints: Use readable endpoint labels or measured speeds.
  Evidence: Slower and Faster are extremely small.

- [ ] `offsite_backup-18` · P2 · proposal. Battery checkbox: Render a clear checkmark inside it.
  Evidence: The checked state is a solid red square without a check.

- [ ] `offsite_backup-19` · P2 · proposal. External-drive checkbox: Add a visible outline.
  Evidence: The white unchecked square blends into the panel.

- [ ] `offsite_backup-20` · P2 · proposal. Checkbox labels: Increase label size and row spacing.
  Evidence: Both settings use tiny text.

- [ ] `offsite_backup-21` · P2 · proposal. Storage values: Strengthen the values and add percent used.
  Evidence: 787 GB of 2 TB has weak emphasis.

- [ ] `offsite_backup-22` · P2 · proposal. Manage account: Increase readability and add a navigation cue.
  Evidence: The link is tiny coral text.

- [ ] `offsite_backup-23` · P2 · proposal. Last completed backup: Increase contrast and add a checkmark.
  Evidence: The success line is faint beneath the folders.

- [ ] `offsite_backup-24` · P2 · proposal. Security footer: Use a recognizable lock and readable text.
  Evidence: A green square precedes the encryption statement.

- [ ] `offsite_backup-25` · P2 · proposal. Help button: Increase icon contrast and button size.
  Evidence: The question mark nearly disappears in its white square.

### pagination

[Baseline](screenshot-baselines/screens/pagination_720p.png) · [Visual review](design-audit.html#screen=pagination)

- [ ] `pagination-01` · P2 · proposal. Title: Use Pagination and option selectors with a short subtitle.
  Evidence: Pagination Component Showcase is long and technical.

- [ ] `pagination-02` · P2 · proposal. Page labels: Use compact numeric buttons under a shared Pages label.
  Evidence: Page repeats in five wide buttons.

- [ ] `pagination-03` · P2 · proposal. Current page: Add a current-page marker or stronger label weight.
  Evidence: Page 1 relies on red fill.

- [ ] `pagination-04` · P2 · proposal. Disabled arrow: Keep it disabled but identifiable and caption its boundary state.
  Evidence: The first previous arrow is very faint.

- [ ] `pagination-05` · P2 · proposal. Chevron style: Use consistent centered geometric chevrons.
  Evidence: Thin handwritten-looking arrows differ from geometric control shapes.

- [ ] `pagination-06` · P2 · proposal. Page result: Show Page 1 of 5.
  Evidence: Selected: Page 1 repeats the button without total context.

- [ ] `pagination-07` · P2 · proposal. Page content: Add a small preview reflecting the selected page.
  Evidence: No sample content connects to the paginator.

- [ ] `pagination-08` · P2 · proposal. Difficulty role: Label it Single-choice selector using the same control.
  Evidence: The difficulty group appears under Pagination without explaining the reuse.

- [ ] `pagination-09` · P2 · proposal. Difficulty meaning: Add short descriptions or an intensity scale.
  Evidence: Easy through Expert have no descriptions.

- [ ] `pagination-10` · P2 · proposal. Selected difficulty: Add an in-control marker matching current-page selection.
  Evidence: Medium uses only red fill plus a distant result.

- [ ] `pagination-11` · P2 · proposal. Named color options: State that they are named options or add small swatches.
  Evidence: All color names use the same cyan button fill.

- [ ] `pagination-12` · P2 · proposal. Blue selection: Use a neutral selection outline to avoid conflicting color meanings.
  Evidence: The Blue option has a red selected background.

- [ ] `pagination-13` · P2 · proposal. Header affordance: Use a quieter noninteractive heading treatment.
  Evidence: Broad cyan headings resemble cyan option buttons.

- [ ] `pagination-14` · P2 · proposal. Header widths: Fit frames to controls or use spare width for explanations.
  Evidence: Headings span the panel while controls occupy about half.

- [ ] `pagination-15` · P2 · proposal. Status spacing: Use shared control-to-result and section spacing.
  Evidence: Result captions and section gaps lack a consistent rhythm.

- [ ] `pagination-16` · P3 · proposal. Boundary behavior: Add Stops at first and last option if that matches configuration.
  Evidence: Arrow wrap behavior is unstated.

- [ ] `pagination-17` · P3 · proposal. Counts: Use consistent counts across all three sections.
  Evidence: Only difficulty and named options state option counts.

- [ ] `pagination-18` · P2 · proposal. Text tracking: Reduce tracking and use a clearer option face.
  Evidence: Widely spaced labels consume unnecessary width.

- [ ] `pagination-19` · P3 · proposal. Outer padding: Tighten decorative outer spacing.
  Evidence: The title area and bottom blank region make the panel sparse.

- [ ] `pagination-20` · P3 · proposal. Independent state: Add Each row keeps its own selection below the subtitle.
  Evidence: Three selected states are shown without explaining independence.

### parcel_corps_settings

[Baseline](screenshot-baselines/screens/parcel_corps_settings_720p.png) · [Visual review](design-audit.html#screen=parcel_corps_settings)

- [ ] `parcel_corps_settings-01` · P2 · proposal. Phone title: Tighten the gap to give the lower controls more room.
  Evidence: Settings is separated from the first row by a large blank gap.

- [ ] `parcel_corps_settings-02` · P2 · proposal. Text Language value: Increase value readability and trailing padding.
  Evidence: King's English is small and close to the right border.

- [ ] `parcel_corps_settings-03` · P2 · proposal. Category symbols: Use recognizable language, display, motion, sync, and audio symbols.
  Evidence: T, D, X, S, and asterisks form an unexplained icon column.

- [ ] `parcel_corps_settings-04` · P2 · proposal. Category color meaning: Group rows by category or add a compact category legend.
  Evidence: Green, orange, red, and blue badges have no key.

- [ ] `parcel_corps_settings-05` · P2 · proposal. Resolution value: Use a clear multiplication sign and a stable value column.
  Evidence: 2560 x 1440 is small in a dense row.

- [ ] `parcel_corps_settings-06` · P2 · proposal. Full Screen label: Use Fullscreen consistently.
  Evidence: Full Screen differs from the common single-word setting name.

- [ ] `parcel_corps_settings-07` · P2 · proposal. MSAA meaning: Add an Anti-aliasing subtitle or expand the label.
  Evidence: MSAA is shown without an explanation.

- [ ] `parcel_corps_settings-08` · P2 · proposal. MSAA arrows: Use centered geometric chevrons and equal button insets.
  Evidence: Small angle-bracket characters fill large dark buttons.

- [ ] `parcel_corps_settings-09` · P2 · proposal. Quality value: Give the value a fixed-width centered region.
  Evidence: High is squeezed between the two arrow buttons.

- [ ] `parcel_corps_settings-10` · P2 · proposal. Toggle states: Add a consistent state label where space allows.
  Evidence: Several vivid green switches have no explicit On text.

- [ ] `parcel_corps_settings-11` · P2 · proposal. Motion Blur icon: Use a motion-blur symbol rather than an error-like X.
  Evidence: The red X badge resembles an error despite the toggle being on.

- [ ] `parcel_corps_settings-12` · P2 · proposal. VSync naming: Use V-sync or Vertical sync consistently.
  Evidence: VSync lacks the spacing and casing used elsewhere in settings.

- [ ] `parcel_corps_settings-13` · P2 · proposal. Settings open text: Move this status into a footer or replace it with an Audio section heading.
  Evidence: Settings open interrupts the list between VSync and audio controls.

- [ ] `parcel_corps_settings-14` · P2 · proposal. Audio sliders: Increase track and thumb readability while keeping compact row heights.
  Evidence: Thin tracks and small white thumbs sit in short rows.

- [ ] `parcel_corps_settings-15` · P2 · proposal. Audio values: Show the current percentage beside each slider.
  Evidence: Audio Volume and Music Volume have no numeric percentages.

- [ ] `parcel_corps_settings-16` · P2 · proposal. Bottom controls: Add bottom padding or a visible scroll boundary.
  Evidence: Music Volume sits close to the phone's lower inner edge.

- [ ] `parcel_corps_settings-17` · P2 · proposal. Phone close path: Add a back chevron near the title if the settings flow supports it.
  Evidence: The phone shows Settings without a visible back or close control.

- [ ] `parcel_corps_settings-18` · P3 · proposal. Phone status bar: Use recognizable battery and signal symbols with consistent spacing.
  Evidence: A blank white pill and green battery-like shape replace the mock's recognizable signal and battery indicators.

- [ ] `parcel_corps_settings-19` · P2 · proposal. Objective copy: Combine it into one objective with a distinct completion state.
  Evidence: The business sign-up objective repeats twice in the blue panel.

- [ ] `parcel_corps_settings-20` · P2 · proposal. Objective marker: Place the level badge in a dedicated corner outside the text block.
  Evidence: The small Lv0 badge overlaps the objective's last line.

- [ ] `parcel_corps_settings-21` · P2 · proposal. Live-chat text: Reduce chat prominence while the phone is open and keep message text legible.
  Evidence: Tiny bright usernames and messages compete with settings.

- [ ] `parcel_corps_settings-22` · P3 · proposal. Live header: Add clear viewer/chat icons or labels around the counts.
  Evidence: 08, 10, and EDDCOATES form an unexplained compact cluster.

- [ ] `parcel_corps_settings-23` · P2 · proposal. Speed display: Use a conventional speed unit and a consistent safe-area inset.
  Evidence: 009 and M/h appear in a small panel at the lower-right edge.

- [ ] `parcel_corps_settings-24` · P3 · proposal. Background rider: Clarify the silhouette and face details without increasing background contrast.
  Evidence: The rider's simplified face and hands are hard to distinguish against the clothing.

- [ ] `parcel_corps_settings-25` · P3 · proposal. Overlay competition: Dim secondary HUD elements while settings has visual priority.
  Evidence: The phone, objective panel, live chat, and speed panel all remain prominent.

### pinch_zoom_lab

[Baseline](screenshot-baselines/screens/pinch_zoom_lab_720p.png) · [Visual review](design-audit.html#screen=pinch_zoom_lab)

- [ ] `pinch_zoom_lab-01` · P2 · proposal. Title: Use Pinch zoom with a separate gesture instruction.
  Evidence: Pinch Zoom - trackpad magnification mixes title and explanation.

- [ ] `pinch_zoom_lab-02` · P2 · proposal. Gesture instruction: Add Pinch two fingers on the trackpad to resize the square.
  Evidence: No visible text says how to perform the gesture.

- [ ] `pinch_zoom_lab-03` · P2 · proposal. Diagnostic row: Increase their text size and separate them into labeled fields.
  Evidence: Zoom, events, and pinching state are tiny.

- [ ] `pinch_zoom_lab-04` · P2 · proposal. Scale formatting: Use 1.00× consistently for magnification.
  Evidence: 1.00x uses a lowercase x.

- [ ] `pinch_zoom_lab-05` · P2 · proposal. Square label: Center the value inside the square or add a consistent inset.
  Evidence: 1.00x sits against the square's left edge.

- [ ] `pinch_zoom_lab-06` · P3 · proposal. Reference size: Add a faint 1.00× outline behind the scaling sample.
  Evidence: The square has no fixed-size comparison reference.

- [ ] `pinch_zoom_lab-07` · P3 · proposal. Dimension metadata: Add Width and Height readouts beside zoom.
  Evidence: Only scale is shown, not the resulting square dimensions.

- [ ] `pinch_zoom_lab-08` · P2 · proposal. Event counter meaning: Label it Pinch updates received.
  Evidence: events 0 does not explain what increments it.

- [ ] `pinch_zoom_lab-09` · P2 · proposal. Gesture state: Use a clear Idle or Pinching state badge.
  Evidence: pinching no is terse and visually indistinct from the counter.

- [ ] `pinch_zoom_lab-10` · P2 · proposal. Build requirement footer: Move requirements into a readable Demo prerequisites panel.
  Evidence: A long compiler-flags sentence is tiny at the bottom.

- [ ] `pinch_zoom_lab-11` · P2 · proposal. Unsupported ambiguity: Show a separate Gesture support available/unavailable diagnostic based on capability.
  Evidence: Reads 0 otherwise means no gesture and unavailable support can look the same.

- [ ] `pinch_zoom_lab-12` · P3 · proposal. Technical footer hierarchy: Keep the user gesture instruction primary and build details secondary.
  Evidence: AppKit and a long feature flag dominate the only supporting explanation.

- [ ] `pinch_zoom_lab-13` · P3 · proposal. Reset reference: Add Reset to 1.00× near the scale readout.
  Evidence: No visible action restores the square to the reference scale.

- [ ] `pinch_zoom_lab-14` · P3 · proposal. Scale limits: Add the configured range to the sample metadata.
  Evidence: The screenshot gives no minimum or maximum zoom range.

- [ ] `pinch_zoom_lab-15` · P3 · proposal. Empty space: Use an unobtrusive grid or ruler to make changes in scale easier to judge.
  Evidence: A small square occupies the center of a nearly empty canvas.

### popover_lab

[Baseline](screenshot-baselines/screens/popover_lab_720p.png) · [Visual review](design-audit.html#screen=popover_lab)

- [ ] `popover_lab-01` · P2 · proposal. Page title: Use Popovers with a separate Anchored panels containing controls subtitle.
  Evidence: The heading is a long sentence combining component name and purpose.

- [ ] `popover_lab-02` · P2 · proposal. Instruction: Explain that Branch is pinned open for comparison.
  Evidence: Click a trigger to open its panel appears even though Branch is already open.

- [ ] `popover_lab-03` · P2 · proposal. Commit options trigger: Add a short Amend and sign-off form caption beneath it.
  Evidence: The closed trigger gives no preview of its form contents.

- [ ] `popover_lab-04` · P2 · proposal. Branch trigger: Show Branch: main or a separate current-branch badge.
  Evidence: Branch does not identify the current branch.

- [ ] `popover_lab-05` · P2 · proposal. Branch heading: Increase its size and clarify Switch branch.
  Evidence: Switch to is tiny above the four branch buttons.

- [ ] `popover_lab-06` · P2 · proposal. Branch names: Use a readable monospaced face for branch names.
  Evidence: main and release/2.4 use small handwritten text.

- [ ] `popover_lab-07` · P2 · proposal. Current branch: Mark the current branch with a check or Current label.
  Evidence: All four branch rows have identical cyan fills.

- [ ] `popover_lab-08` · P2 · proposal. Branch row padding: Add consistent horizontal insets.
  Evidence: Branch names sit close to the left edges of their pills.

- [ ] `popover_lab-09` · P2 · proposal. Popover anchor relationship: Add a small anchor cue or consistent offset that makes ownership clear.
  Evidence: The panel begins directly below the trigger without a pointer or gap.

- [ ] `popover_lab-10` · P2 · proposal. Explanation heading: Give it a stronger heading size.
  Evidence: What a popover is for is small within the large dark panel.

- [ ] `popover_lab-11` · P2 · proposal. Explanation body: Use readable body text with paragraph spacing.
  Evidence: The body is tiny, widely spaced, and broken into isolated lines.

- [ ] `popover_lab-12` · P2 · proposal. Focus explanation: Rephrase as You can use controls inside without dismissing the panel.
  Evidence: Focus landing inside must not close it describes an internal rule.

- [ ] `popover_lab-13` · P3 · proposal. Menu comparison: Add a small static menu-versus-form illustration.
  Evidence: The note mentions menus without showing the difference.

- [ ] `popover_lab-14` · P2 · proposal. Placement heading: Use Placement and edge flipping as a clear section heading.
  Evidence: Placement, and what happens when there is no room is a long low-emphasis line.

- [ ] `popover_lab-15` · P2 · proposal. Above sample caption: Use a centered readable caption with consistent inset.
  Evidence: anchored Above is tiny and left-flush inside its panel.

- [ ] `popover_lab-16` · P2 · proposal. Right sample caption: Add internal padding without changing its anchor placement.
  Evidence: anchored Right touches the panel's left edge.

- [ ] `popover_lab-17` · P2 · proposal. Left sample caption: Inset the caption while retaining the diagnostic focus outline.
  Evidence: anchored Left sits close to the outlined boundary.

- [ ] `popover_lab-18` · P3 · proposal. Placement direction: Add small directional arrows outside each trigger-panel pair.
  Evidence: Above, Right, and Left labels require mentally tracking their panels.

- [ ] `popover_lab-19` · P3 · proposal. Placement grouping: Frame each pair within a labeled comparison region without changing its anchor geometry.
  Evidence: The three examples occupy irregular widths and alignments.

- [ ] `popover_lab-20` · P2 · proposal. Bottom trigger: Rename it Bottom-edge flip using the shared label style.
  Evidence: near the bottom is lowercase and differs from the other trigger labels.

- [ ] `popover_lab-21` · P2 · proposal. Bottom expected result: Add a small expected-placement diagram or companion open-state capture.
  Evidence: The bottom panel is closed, so its flipping behavior is not visible.

- [ ] `popover_lab-22` · P3 · proposal. Trigger versus panel colors: Add a compact Trigger / Popover color key.
  Evidence: Cyan triggers and dark panels lack a shared legend.

- [ ] `popover_lab-23` · P3 · proposal. Pinned-state identity: Add Pinned example as a small external badge.
  Evidence: The Branch panel resembles a user-opened panel without stating its fixture role.

- [ ] `popover_lab-24` · P3 · proposal. Empty central space: Use the space for anchor and fallback-placement guides.
  Evidence: Broad blank regions separate the upper and lower demonstrations.

- [ ] `popover_lab-25` · P2 · proposal. Result feedback: Reserve a labeled Last action line for branch or commit outcomes.
  Evidence: The only status text is the initial instruction.

### potion_crafting

[Baseline](screenshot-baselines/screens/potion_crafting_720p.png) · [Visual review](design-audit.html#screen=potion_crafting)

- [ ] `potion_crafting-01` · P2 · proposal. Parchment texture: Reduce texture contrast in text-heavy regions.
  Evidence: Fine horizontal lines run directly behind every label.

- [ ] `potion_crafting-02` · P2 · proposal. Recipe descriptions: Increase effect-text size and contrast.
  Evidence: Restores 30 MP and other effects are small and pale.

- [ ] `potion_crafting-03` · P2 · proposal. Recipe times: Use readable duration labels with consistent spacing before seconds.
  Evidence: 30s to brew and 45s to brew are tiny.

- [ ] `potion_crafting-04` · P2 · proposal. Selected recipe: Add a clear selected marker or stronger leading rule.
  Evidence: Mana Elixir uses a pale rectangular border with little contrast.

- [ ] `potion_crafting-05` · P2 · proposal. Recipe icons: Normalize bottle scale and outline weight across recipes.
  Evidence: Small potion icons occupy different visual weights by fill color.

- [ ] `potion_crafting-06` · P2 · proposal. Center subtitle: Increase contrast and keep it subordinate to the recipe title.
  Evidence: The remedy for an exhausted mind is faint under Mana Elixir.

- [ ] `potion_crafting-07` · P2 · proposal. Brew state: Label the active state Brewing and make the action reflect the current process.
  Evidence: 65% brewed appears above a button still labeled Brew potion.

- [ ] `potion_crafting-08` · P2 · proposal. Progress value: Increase value size and give the fill stronger separation from the track.
  Evidence: 65% brewed is tiny below a thin low-contrast bar.

- [ ] `potion_crafting-09` · P2 · proposal. Progress placement: Reduce shelf emphasis or increase progress-bar prominence.
  Evidence: A thick wooden shelf is more prominent than the progress bar beneath it.

- [ ] `potion_crafting-10` · P2 · proposal. Required ingredient counts: Use readable names and multiplication signs.
  Evidence: Sea Salt x 3 and Star Dust x 1 are small below the illustration.

- [ ] `potion_crafting-11` · P2 · proposal. Availability states: Add a check icon and stronger text contrast.
  Evidence: Available is pale green without the mock's clear checkmark.

- [ ] `potion_crafting-12` · P2 · proposal. Inventory relation: Show Required 3 / Owned 12 and Required 1 / Owned 1 beside the recipe.
  Evidence: Required counts and inventory counts are far apart without a shared notation.

- [ ] `potion_crafting-13` · P2 · proposal. Last Star Dust: Add a small After brew: 0 note to make the inventory consequence visible.
  Evidence: The recipe requires the entire visible Star Dust stock of one.

- [ ] `potion_crafting-14` · P2 · proposal. Brew action contrast: Increase text and border contrast while keeping the muted aesthetic.
  Evidence: The olive button blends with the brown parchment palette.

- [ ] `potion_crafting-15` · P2 · proposal. Brew duration footer: Move it closer to the action and use a readable size.
  Evidence: 45 seconds / Produces 1 potion is tiny near the bottom edge.

- [ ] `potion_crafting-16` · P2 · proposal. Ingredient tile names: Increase label size and maintain equal bottom padding.
  Evidence: Moonpetal and Fire Root use small centered serif labels.

- [ ] `potion_crafting-17` · P2 · proposal. Inventory quantity placement: Use consistent compact quantity badges.
  Evidence: x 5, x 3, and other quantities sit tiny in tile corners.

- [ ] `potion_crafting-18` · P3 · proposal. Herb silhouettes: Make their leaf silhouettes more distinct in addition to color.
  Evidence: Moonpetal, Fire Root, Honey Dew, and Shadow Moss share very similar stem shapes.

- [ ] `potion_crafting-19` · P2 · proposal. Ingredient selection: Highlight Sea Salt and Star Dust as recipe ingredients.
  Evidence: All six inventory tiles use the same frame with no indication of which are required.

- [ ] `potion_crafting-20` · P2 · proposal. Alchemist note: Increase body contrast and line height.
  Evidence: The instruction is small and faint beneath a decorative heading.

- [ ] `potion_crafting-21` · P3 · proposal. Note instruction sequence: Pair the note with a visible current brewing step.
  Evidence: Add Star Dust only after the water turns clear is detached from the brewing status.

- [ ] `potion_crafting-22` · P2 · proposal. Recipe-book quotation: Reduce decorative quote prominence or move it to a smaller note block.
  Evidence: Three faint lines compete for space below the recipe list.

- [ ] `potion_crafting-23` · P2 · proposal. Footer statistics: Use a consistent readable metadata scale and clearer group spacing.
  Evidence: Alchemy Level, recipes known, and potions brewed are tiny.

- [ ] `potion_crafting-24` · P2 · proposal. Keyboard footer: Add readable keycaps and a visible close/back action if available.
  Evidence: Tab: move Enter: choose is small and omits the mock's Close workshop action.

- [ ] `potion_crafting-25` · P3 · proposal. Currency identity: Add Gold as a small caption or a clearer coin symbol.
  Evidence: 2,450 has a generic circular icon without a currency name.

### powerwash_settings

[Baseline](screenshot-baselines/screens/powerwash_settings_720p.png) · [Visual review](design-audit.html#screen=powerwash_settings)

- [ ] `powerwash_settings-01` · P2 · defect. Resolution value: Inset every value 10 px from its border.
  Evidence: 2560 x 1440 touches the value box left border.

- [ ] `powerwash_settings-02` · P2 · defect. Footer Select hint: Add an 8 px keycap-to-label gap.
  Evidence: The Enter keycap nearly touches Select.

- [ ] `powerwash_settings-03` · P2 · defect. Footer Reset hint: Give each action group a 20 px trailing gap.
  Evidence: Reset and the next Enter keycap sit nearly together.

- [ ] `powerwash_settings-04` · P2 · defect. Top counters: Render recognizable resource icons and increase counter text to 14 px actual size.
  Evidence: O 10.00, O 200 and [] 1 are barely legible.

- [ ] `powerwash_settings-05` · P2 · proposal. Dropdown arrows: Use consistent 12 px chevrons aligned to a shared inset.
  Evidence: Nine values end in tiny v glyphs.

- [ ] `powerwash_settings-06` · P2 · proposal. Anti-Aliasing selection: Extend a selection outline around the complete label and value row.
  Evidence: Only the label half gets the pale selection treatment.

- [ ] `powerwash_settings-07` · P2 · proposal. Selected label: Use the dark navy text token for the selected label.
  Evidence: The Anti-Aliasing text is subdued blue on pale gray.

- [ ] `powerwash_settings-08` · P2 · proposal. Help heading: Increase the heading weight and add 8 px before the paragraph.
  Evidence: Anti-Aliasing is close in size to its body copy.

- [ ] `powerwash_settings-09` · P2 · proposal. Help explanation: Constrain paragraphs to about 48 characters per line.
  Evidence: The help text spans several long lines across the right pane.

- [ ] `powerwash_settings-10` · P2 · proposal. FPS wording: Replace with reduce graphics processing load so the performance effect is clear.
  Evidence: The explanation says reduce FPS load.

- [ ] `powerwash_settings-11` · P2 · proposal. Render Scale value: Display the actual native scale, such as 100%, with a native label.
  Evidence: Off appears in a control otherwise associated with scaling percentages.

- [ ] `powerwash_settings-12` · P3 · proposal. Resolution notation: Use the multiplication sign with consistent spacing.
  Evidence: Resolution uses a lowercase x between dimensions.

- [ ] `powerwash_settings-13` · P3 · proposal. Vsync label: Use VSync and expand it in the associated help.
  Evidence: Vsync is styled differently from the familiar VSync spelling.

- [ ] `powerwash_settings-14` · P2 · proposal. SSAO row: Expose Ambient occlusion in the label or a short adjacent qualifier.
  Evidence: SSAO is the only unexplained acronym in the list.

- [ ] `powerwash_settings-15` · P3 · proposal. Model and texture quality: Use one consistent scale across both quality controls.
  Evidence: High and Highest use different quality vocabularies.

- [ ] `powerwash_settings-16` · P2 · proposal. Settings categories: Add a small Video settings breadcrumb by SETTINGS to identify the current section immediately.
  Evidence: The category tabs sit below a tall content block.

- [ ] `powerwash_settings-17` · P2 · proposal. Header icons: Add short captions or an explicit application-navigation group label.
  Evidence: Six unlabeled symbols appear above the settings rows.

- [ ] `powerwash_settings-18` · P3 · proposal. Active gear icon: Add a small Settings caption to establish what the selected icon represents.
  Evidence: The selected gear has a brighter tile but no text.

- [ ] `powerwash_settings-19` · P3 · proposal. Tablet backdrop: Add the soft tablet shadow from mocks.html to separate the panel from the scene.
  Evidence: Large peach tiles surround a dense blue panel.

- [ ] `powerwash_settings-20` · P3 · proposal. Watermark: Increase the watermark size modestly while keeping it secondary.
  Evidence: FUTURLAB / POWERWASH SIMULATOR is very faint at the pane bottom.

- [ ] `powerwash_settings-21` · P2 · proposal. Tab controls A and D: Add a concise Change category hint alongside the footer controls.
  Evidence: A and D keycaps sit alone at opposite ends of the strip.

- [ ] `powerwash_settings-22` · P2 · proposal. Close button: Use a standard close glyph with a stable centered alignment.
  Evidence: A plain X in a blue square is separated from its footer explanation.

- [ ] `powerwash_settings-23` · P2 · proposal. Reset action: Rename to Reset video settings to match the active tab.
  Evidence: The footer says Reset without naming its scope.

- [ ] `powerwash_settings-24` · P2 · proposal. Change status: Add a compact Changes apply immediately message near the footer if that is the actual behavior.
  Evidence: No visible saved or pending state appears beside the settings.

- [ ] `powerwash_settings-25` · P3 · proposal. Row borders: Reduce inactive border weight and reserve the strongest edge for selection.
  Evidence: Every row has strong double blue divisions.

- [ ] `powerwash_settings-26` · P2 · proposal. Product typography parity: Match the target font face, weight and tracking, then recheck wrapping and value padding.
  Evidence: The current help and row labels are more widely spaced and lighter than the compact condensed type in the rendered mock.

- [ ] `powerwash_settings-27` · P3 · proposal. Scene perspective parity: Restore the target's perspective planes and tile geometry.
  Evidence: The current peach background is a flat grid while the target shows a receding tiled corner around the tablet.

- [ ] `powerwash_settings-28` · P3 · proposal. Active Video tab parity: Match the target tab treatment with a clear bottom selection rule.
  Evidence: The current Video tab has a strong four-sided outline; the target uses a blue fill and bright bottom edge.

### race_results

[Baseline](screenshot-baselines/screens/race_results_720p.png) · [Visual review](design-audit.html#screen=race_results)

- [ ] `race_results-01` · P2 · defect. Table headers: Increase headers to a readable 12 to 14 px actual size.
  Evidence: POS., RACER, TIME and PTS are tiny beside the racer names.

- [ ] `race_results-02` · P2 · defect. Points column: Give the points column a constant 20 px right inset.
  Evidence: The +4 and +3 values almost touch the sloping right edge.

- [ ] `race_results-03` · P2 · defect. Personal best caption: Increase the caption to at least 12 px and place it closer to the time.
  Evidence: New personal best! is extremely small under 0:48.220.

- [ ] `race_results-04` · P2 · defect. Controller help: Increase help text and separate key labels into readable groups.
  Evidence: Arrows to choose A Confirm B Back is tiny in the footer.

- [ ] `race_results-05` · P2 · proposal. Position headline: Label it Race finish or Cup position according to the intended metric.
  Evidence: 2nd appears above CUP STANDINGS without a race or cup qualifier.

- [ ] `race_results-06` · P2 · proposal. Player chip: Label it Player 1 so it cannot read as finishing position.
  Evidence: P1 in the top-right competes with the second-place result.

- [ ] `race_results-07` · P2 · proposal. Table shape: Keep decorative skew on a backing panel and align the data table to a rectangular grid.
  Evidence: Rows progressively shift left while column content does not follow the same edge.

- [ ] `race_results-08` · P3 · proposal. Bottom table gap: End the table after the last row or use the band for a labeled total.
  Evidence: An empty pale band remains below Pebble.

- [ ] `race_results-09` · P2 · proposal. Race progress: Give race progress a small four-step marker near the title.
  Evidence: Race 3/4 is embedded in the circuit title.

- [ ] `race_results-10` · P2 · proposal. Race context: Separate circuit name from championship progress with clear hierarchy.
  Evidence: Rainbow Circuit shares one line with progress.

- [ ] `race_results-11` · P2 · proposal. Points label: Rename the left column Race points.
  Evidence: PTS does not distinguish earned points from cumulative standings points.

- [ ] `race_results-12` · P2 · proposal. Time label: Use Race time as the column heading.
  Evidence: TIME does not say whether this is total race time.

- [ ] `race_results-13` · P2 · proposal. Player identity: Include the driver's name with a You tag for continuity with other screens.
  Evidence: The highlighted racer is only named YOU.

- [ ] `race_results-14` · P3 · proposal. Player portrait: Add a small player badge next to the green portrait.
  Evidence: The player portrait has the same visual treatment as every opponent.

- [ ] `race_results-15` · P2 · proposal. Standings subset: Label the section Top 4 of 8 or provide a compact full-standings affordance.
  Evidence: Only four of eight racers are shown in CUP STANDINGS.

- [ ] `race_results-16` · P2 · proposal. Championship gap: Add 4 points behind leader beside the player's cumulative score.
  Evidence: 38 pts and 42 pts require mental subtraction.

- [ ] `race_results-17` · P2 · proposal. Race gap: Show a +2.362 gap alongside the player time.
  Evidence: The player's 2:33.812 requires comparison with 2:31.450.

- [ ] `race_results-18` · P3 · proposal. Best lap association: Rename BEST LAP to YOUR BEST LAP.
  Evidence: The right card does not identify whose best lap is shown.

- [ ] `race_results-19` · P2 · proposal. Best lap achievement: Add the previous best or improvement delta when available.
  Evidence: Personal best is shown with no prior benchmark.

- [ ] `race_results-20` · P3 · proposal. Standings row separators: Increase separator contrast slightly and use even row padding.
  Evidence: Thin separators are weak against the landscape-tinted panel.

- [ ] `race_results-21` · P3 · proposal. Side-panel background: Use a more opaque backing behind text for consistent contrast.
  Evidence: The landscape silhouette crosses the standings and lap areas.

- [ ] `race_results-22` · P2 · proposal. Next race action: Add the upcoming track name as secondary button text or adjacent copy.
  Evidence: NEXT RACE gives no destination context.

- [ ] `race_results-23` · P3 · proposal. Back action: Use matching navigation chevrons with equal label spacing.
  Evidence: Back uses a text less-than symbol while Next uses a greater-than symbol.

- [ ] `race_results-24` · P3 · proposal. Footer balance: Align help centrally between Back and Next race.
  Evidence: The help text hugs Back while a large empty middle area remains.

- [ ] `race_results-25` · P3 · proposal. Standings emphasis: Add a small player marker to match the highlighted result row.
  Evidence: YOU is yellow but has no shape-based indicator in the side list.

- [ ] `race_results-26` · P2 · proposal. Result typography parity: Match the italic display treatment for the title, finish position and standings heading.
  Evidence: The current title and 2nd are upright, while the rendered target uses italic racing typography.

- [ ] `race_results-27` · P3 · proposal. Footer action geometry parity: Apply the target slant to button backings while keeping labels level and padded.
  Evidence: Back and Next race are rectangular in the current image; the target uses slanted button silhouettes.

- [ ] `race_results-28` · P2 · proposal. Result-row separators parity: Add the target row separators to strengthen horizontal scanning.
  Evidence: The current table relies on alternating pale fills, while the target adds visible cyan row rules.

### radio_buttons

[Baseline](screenshot-baselines/screens/radio_buttons_720p.png) · [Visual review](design-audit.html#screen=radio_buttons)

- [ ] `radio_buttons-01` · P2 · proposal. Group card height: Size cards to their options and reserve a shared footer for demo notes.
  Evidence: Fruit and Size leave over half their card empty.

- [ ] `radio_buttons-02` · P2 · proposal. Main heading: Add a consistent 16 px text inset.
  Evidence: Radio Button Groups sits flush against its teal bar left edge.

- [ ] `radio_buttons-03` · P2 · proposal. Group headings: Align headings with the option-label column or apply a clear shared inset.
  Evidence: Favorite Fruit, Size and Color hug their header left edges.

- [ ] `radio_buttons-04` · P2 · proposal. Selected markers: Add an adjacent selected badge or stronger outline while retaining the theme swatch.
  Evidence: Orange, Small and Blue use a dark magenta dot on a dark card.

- [ ] `radio_buttons-05` · P3 · proposal. Color options: Add small labeled color swatches without changing the radio selection color.
  Evidence: Red, Green, Blue and Yellow are all text-only.

- [ ] `radio_buttons-06` · P2 · proposal. Summary syntax: Use human-readable labeled values separated into three columns.
  Evidence: The footer reads Fruit=Orange, Size=Small, Color=Blue.

- [ ] `radio_buttons-07` · P2 · proposal. Summary alignment: Add 16 px inner padding consistent with the title.
  Evidence: Selected begins flush against the footer edge.

- [ ] `radio_buttons-08` · P2 · proposal. Selection instructions: Add a one-line instruction above the cards.
  Evidence: The title gives no explanation of one choice per group.

- [ ] `radio_buttons-09` · P2 · proposal. Keyboard guidance: Add a footer note describing the actual Tab and arrow behavior confirmed in source.
  Evidence: No key guide is visible beside the groups.

- [ ] `radio_buttons-10` · P3 · proposal. Group numbering: Add small independent group labels so the comparison purpose is evident.
  Evidence: Three equal cards have no explicit group identifiers beyond names.

- [ ] `radio_buttons-11` · P3 · proposal. Defaults: Mark the initial selection as the fixture default in the summary.
  Evidence: Orange, Small and Blue are selected with no default annotation.

- [ ] `radio_buttons-12` · P2 · proposal. Fruit label width: Add a visible long-label example caption that explains the width test.
  Evidence: Watermelon is much longer than Apple and Orange.

- [ ] `radio_buttons-13` · P3 · proposal. Size labels: Name the group Item size so the choices read as a coherent sample form.
  Evidence: Small, Medium and Large contain no size context.

- [ ] `radio_buttons-14` · P3 · proposal. Unequal option counts: Align card bottoms to the option region and label option counts for comparison.
  Evidence: Color has four rows while the other cards have three.

- [ ] `radio_buttons-15` · P3 · proposal. Card border semantics: Use neutral card outlines so magenta is reserved for selection.
  Evidence: All three cards use the same magenta as selected radio dots.

- [ ] `radio_buttons-16` · P3 · proposal. Unused lower screen: Vertically center the compact demo within the viewport.
  Evidence: A broad black band remains below the main panel.

- [ ] `radio_buttons-17` · P2 · proposal. Summary association: Move each selected value directly beneath its group and keep a compact combined summary.
  Evidence: The status line is separated from the cards by a large gutter.

- [ ] `radio_buttons-18` · P3 · proposal. Spec visibility: Add a small ring 28 px / dot 18 px caption outside the test controls.
  Evidence: The enlarged circles and dots are visible but their dimensions are undocumented.

### real_world_modals

[Baseline](screenshot-baselines/screens/real_world_modals_720p.png) · [Visual review](design-audit.html#screen=real_world_modals)

- [ ] `real_world_modals-01` · P2 · defect. Status line: Use a readable neutral status color until an action completes.
  Evidence: No action yet is dark green on the navy background.

- [ ] `real_world_modals-02` · P2 · proposal. Page framing: Use a compact centered launcher panel or fill the lower area with pattern descriptions.
  Evidence: The six launchers occupy only the upper third of a large panel.

- [ ] `real_world_modals-03` · P2 · proposal. Introductory copy: Add Choose a pattern to preview its dialog beneath the heading.
  Evidence: Real-World Modals gives no instruction for opening examples.

- [ ] `real_world_modals-04` · P2 · proposal. Delete Item button: Label it Preview delete confirmation.
  Evidence: Delete Item sounds like an immediate destructive action on the launcher screen.

- [ ] `real_world_modals-05` · P2 · proposal. Command Palette button: Add Search commands as secondary copy beneath it.
  Evidence: The launcher names the pattern but gives no example of its purpose.

- [ ] `real_world_modals-06` · P2 · proposal. Terms of Service button: Add a short Scrollable agreement description.
  Evidence: The label does not reveal that the demo includes scrollable legal text.

- [ ] `real_world_modals-07` · P2 · proposal. Side Drawer button: Add a miniature right-edge drawer diagram beside the caption.
  Evidence: The name gives no placement preview.

- [ ] `real_world_modals-08` · P2 · proposal. Bottom Sheet button: Add a bottom-edge preview diagram to distinguish placement.
  Evidence: The launcher has the same visual weight as centered modal examples.

- [ ] `real_world_modals-09` · P2 · proposal. Cookie Banner button: Add a narrow banner preview below its label.
  Evidence: The launcher does not identify this as a noncentral banner pattern.

- [ ] `real_world_modals-10` · P3 · proposal. Pattern grouping: Group centered dialogs separately from edge overlays with small headings.
  Evidence: All six buttons form an undifferentiated grid.

- [ ] `real_world_modals-11` · P2 · proposal. Status context: Place it in an Action result region with a visible heading.
  Evidence: No action yet sits alone without a label.

- [ ] `real_world_modals-12` · P3 · proposal. Destructive accent: Reserve solid red for the destructive action inside the preview and use a red outline here.
  Evidence: The red Delete Item launcher dominates the otherwise blue grid.

- [ ] `real_world_modals-13` · P3 · proposal. Button widths: Use equal column widths for the three-column grid.
  Evidence: Delete Item is visibly narrower than the other launcher buttons.

- [ ] `real_world_modals-14` · P2 · proposal. Page title scale: Increase title size and reduce letter spacing on launcher labels.
  Evidence: The title is only moderately larger than the letter-spaced button text.

- [ ] `real_world_modals-15` · P2 · proposal. Dismiss instructions: Add a short dismissal note matched to the actual supported controls.
  Evidence: No guidance explains how demo overlays can be closed.

- [ ] `real_world_modals-16` · P3 · proposal. Initial-state documentation: Add a labeled static pattern thumbnail region or separate open-state baselines for review.
  Evidence: The baseline contains no visual specimen of any modal.

### rect_algebra_lab

[Baseline](screenshot-baselines/screens/rect_algebra_lab_720p.png) · [Visual review](design-audit.html#screen=rect_algebra_lab)

- [ ] `rect_algebra_lab-01` · P2 · proposal. Left diagram heading: Label it Window tiling above the drawing.
  Evidence: The tiled rectangle has no local title.

- [ ] `rect_algebra_lab-02` · P2 · proposal. Right diagram heading: Label it Cut header, pad body, center box.
  Evidence: The orange box diagram has no local title.

- [ ] `rect_algebra_lab-03` · P2 · proposal. Sidebar rectangle: Place Sidebar in an external callout tied to that rectangle.
  Evidence: The tall narrow left pane has no identifier.

- [ ] `rect_algebra_lab-04` · P2 · proposal. Main canvas: Add a Canvas label outside its measured geometry.
  Evidence: The largest blue rectangle is unlabeled.

- [ ] `rect_algebra_lab-05` · P2 · proposal. Inspector pane: Add an Inspector callout with its 110 px cut width.
  Evidence: The right narrow blue pane cannot be distinguished by purpose.

- [ ] `rect_algebra_lab-06` · P2 · proposal. Bottom strip: Label it Status strip, cut_bottom 26 px.
  Evidence: The gray strip along the bottom has no description.

- [ ] `rect_algebra_lab-07` · P2 · proposal. Top strip: Label it Tabs, cut_top 24 px.
  Evidence: The short gray strip above the canvas has no description.

- [ ] `rect_algebra_lab-08` · P2 · proposal. Operation order: Add a numbered sequence in a small legend under the diagram.
  Evidence: The left result shows no sequence for the nested cuts.

- [ ] `rect_algebra_lab-09` · P2 · proposal. Right header strip: Add a callout for the 22 px header cut.
  Evidence: The blue top strip has no stated height.

- [ ] `rect_algebra_lab-10` · P2 · proposal. Inner outline: Annotate the inset distance outside the outline.
  Evidence: The purple rectangle does not explain the 6 px inset.

- [ ] `rect_algebra_lab-11` · P2 · proposal. Orange box: Label it 64 × 36 near the diagram.
  Evidence: The centered orange box has no dimensions.

- [ ] `rect_algebra_lab-12` · P2 · proposal. Orange outer outline: Label it expand 12 px hit target.
  Evidence: The dim orange outline can read as a decorative border.

- [ ] `rect_algebra_lab-13` · P2 · proposal. Top green tick: Add align_top to the legend with its green swatch.
  Evidence: The green marker is unnamed.

- [ ] `rect_algebra_lab-14` · P2 · proposal. Edge marker system: Add a compact legend naming top, bottom, left and right alignment.
  Evidence: Four green markers indicate alignment without named edges.

- [ ] `rect_algebra_lab-15` · P3 · proposal. Diagram key: Provide one legend mapping each color to its operation.
  Evidence: Blue, purple, orange and green have no semantic key.

- [ ] `rect_algebra_lab-16` · P2 · proposal. Center verification: Add faint optional center guides that intersect at the box center.
  Evidence: The orange rectangle appears centered but has no reference axes.

- [ ] `rect_algebra_lab-17` · P3 · proposal. Root title: Include expand in the title or subtitle.
  Evidence: rect algebra: cut, pad, align omits expand despite showing it.

- [ ] `rect_algebra_lab-18` · P3 · proposal. Unused lower area: Use that area for the operation legend and formulas.
  Evidence: A large dark strip remains below both diagrams.

- [ ] `rect_algebra_lab-19` · P2 · proposal. Static demo framing: Add a Static geometry demonstration caption.
  Evidence: The diagram looks like an empty application shell.

- [ ] `rect_algebra_lab-20` · P3 · proposal. Measured padding: Label the 10 px left-panel and 20 px right-panel padding in the legend.
  Evidence: The outer and inner margins are visible but undocumented.

### rubber_bandits_menu

[Baseline](screenshot-baselines/screens/rubber_bandits_menu_720p.png) · [Visual review](design-audit.html#screen=rubber_bandits_menu)

- [ ] `rubber_bandits_menu-01` · P2 · defect. Version label: Increase it to a readable 11 px actual size.
  Evidence: 1.6.0 / RUBBER BANDITS is nearly microscopic at bottom left.

- [ ] `rubber_bandits_menu-02` · P2 · defect. Character favorite label: Give it a dedicated line with at least 12 px readable text.
  Evidence: FAVOURITE is squeezed between the name and bottom strip.

- [ ] `rubber_bandits_menu-03` · P2 · proposal. Top account strip: Separate account name from a labeled connection badge.
  Evidence: The user name and Online status share one tiny line.

- [ ] `rubber_bandits_menu-04` · P2 · proposal. Top-right counters: Add level and currency captions or recognizable matching icons.
  Evidence: A circled 1 and green 20 have no meaning labels.

- [ ] `rubber_bandits_menu-05` · P2 · proposal. Selected Options item: Add equal left padding to match the other menu labels.
  Evidence: The selected word starts nearly at the turquoise pill edge.

- [ ] `rubber_bandits_menu-06` · P3 · proposal. Selection marker: Use a consistent selection pointer with the same size as the menu text cap height.
  Evidence: The diamond at the right of Options has little association with navigation.

- [ ] `rubber_bandits_menu-07` · P2 · proposal. Back spacing: Make the separation an explicit secondary navigation group with a divider.
  Evidence: Back is separated from Play Credits by a much larger gap.

- [ ] `rubber_bandits_menu-08` · P2 · proposal. Play Credits wording: Use Credits unless the action actually starts a distinct playable mode.
  Evidence: Play Credits reads like gameplay next to Armory.

- [ ] `rubber_bandits_menu-09` · P2 · proposal. Leaderboard width: Maintain a common menu text width and align the selection pill to that width.
  Evidence: LEADERBOARD is considerably longer than other entries.

- [ ] `rubber_bandits_menu-10` · P2 · proposal. Supporter promo: Use a clear Supporter pack heading and one supporting sentence.
  Evidence: Get more bandits shares three equally emphasized lines.

- [ ] `rubber_bandits_menu-11` · P2 · proposal. Promo action: Add View pack within the card.
  Evidence: The supporter card has no explicit action label.

- [ ] `rubber_bandits_menu-12` · P3 · proposal. Promo icon: Add a darker outline so its silhouette reads at a glance.
  Evidence: The lightning coin uses yellow against pale yellow.

- [ ] `rubber_bandits_menu-13` · P2 · proposal. Character name: Put the name on a solid or translucent backing panel.
  Evidence: EARL THE BIRD overlaps a busy trail of banknotes.

- [ ] `rubber_bandits_menu-14` · P2 · proposal. LB and RB hints: Align both on a quiet common baseline around the selection panel.
  Evidence: The bumper hints are separated by the character name and currency art.

- [ ] `rubber_bandits_menu-15` · P2 · proposal. Selected character: Add a subtle ground marker to identify the selected character without hiding the lineup.
  Evidence: The large bird overlaps several surrounding characters.

- [ ] `rubber_bandits_menu-16` · P3 · proposal. Character count: Show Earl the Bird with a position indicator such as 3 of 7.
  Evidence: The lineup contains several figures but no selected index.

- [ ] `rubber_bandits_menu-17` · P3 · proposal. Background crown: Refine its crown silhouette or reduce its prominence to avoid a placeholder appearance.
  Evidence: A flat yellow block shape rises behind the center figures.

- [ ] `rubber_bandits_menu-18` · P3 · proposal. Ground shadows: Align each shadow beneath its feet using the scene perspective.
  Evidence: Several characters appear to float above disconnected oval shadows.

- [ ] `rubber_bandits_menu-19` · P3 · proposal. Cash props: Reserve a clear lower control band and keep props above it.
  Evidence: Dense banknotes cross the bottom selection controls.

- [ ] `rubber_bandits_menu-20` · P3 · proposal. Rightmost prop: Connect it visually to its prop or remove that ambiguous shape.
  Evidence: A dark circle sits beside the cowboy and looks detached.

- [ ] `rubber_bandits_menu-21` · P2 · proposal. Menu help strip: Add vertical breathing room around the help text.
  Evidence: Change the game's settings sits at the extreme bottom edge.

- [ ] `rubber_bandits_menu-22` · P2 · proposal. Input guide: Add compact confirm and back hints beside the menu help.
  Evidence: Only character bumpers are shown despite the main menu being selected.

- [ ] `rubber_bandits_menu-23` · P3 · proposal. Logo prominence: Reduce its footprint slightly to give the menu more breathing room.
  Evidence: The large logo outweighs the actual selected menu action.

- [ ] `rubber_bandits_menu-24` · P2 · proposal. Online context: Label Back to main menu when that is the actual destination.
  Evidence: Online appears in the top strip while Back has no destination.

- [ ] `rubber_bandits_menu-25` · P3 · proposal. Character art density: Increase silhouette separation so hats and faces do not visually merge.
  Evidence: The right side packs six smaller figures close together.

- [ ] `rubber_bandits_menu-26` · P2 · proposal. Menu typography parity: Match the target font metrics and tracking so menu labels retain the intended compact width.
  Evidence: The current LEADERBOARD and PLAY CREDITS occupy more horizontal space than the rendered target.

- [ ] `rubber_bandits_menu-27` · P3 · proposal. Supporter card border parity: Restore the double border and inner inset to match the promotional-card treatment.
  Evidence: The current supporter card has one thin white edge; the target has a rounded double outline.

### scroll_click_bug

[Baseline](screenshot-baselines/screens/scroll_click_bug_720p.png) · [Visual review](design-audit.html#screen=scroll_click_bug)

- [ ] `scroll_click_bug-01` · P2 · proposal. Test title: Add a subtitle stating that the clicked label must match the reported index after scrolling.
  Evidence: Scroll Click Bug Test names a bug without a visible expected outcome.

- [ ] `scroll_click_bug-02` · P2 · proposal. Initial status: Prefix it with Last clicked and show a neutral empty value.
  Evidence: No button clicked yet does not identify the output field.

- [ ] `scroll_click_bug-03` · P2 · proposal. List context: Display 20 buttons above the viewport.
  Evidence: Only Button 0 through part of Button 6 are visible.

- [ ] `scroll_click_bug-04` · P2 · proposal. Scroll position: Add Scroll offset: 0 px beside the status.
  Evidence: The initial scrollbar gives no numeric offset.

- [ ] `scroll_click_bug-05` · P2 · proposal. Visible range: Show the visible range separately from the last-click result.
  Evidence: The partial Button 6 has no range summary.

- [ ] `scroll_click_bug-06` · P2 · proposal. Reproduction steps: Place a short scroll-then-click procedure there.
  Evidence: The large blank area below the list contains no procedure.

- [ ] `scroll_click_bug-07` · P2 · proposal. Expected result: Display Expected label and Actual clicked label side by side after selection.
  Evidence: The status line has no comparison target.

- [ ] `scroll_click_bug-08` · P3 · proposal. Title bar alignment: Add 12 px inner padding.
  Evidence: The title text touches the left of its white backing.

- [ ] `scroll_click_bug-09` · P3 · proposal. List inset: Reduce the gutter to a consistent 12 to 16 px if it is not part of the hit-test fixture.
  Evidence: A wide blank gutter separates button ends from the scrollbar.

- [ ] `scroll_click_bug-10` · P2 · proposal. Button identifiers: Add a small left-aligned index column while preserving the button hit area.
  Evidence: Centered Button N labels make row identity far from the left edge.

- [ ] `scroll_click_bug-11` · P3 · proposal. Status legibility: Increase actual text size and use a darker neutral color.
  Evidence: The initial status is light gray and small.

- [ ] `scroll_click_bug-12` · P2 · proposal. Reset control: Add Reset scroll and result outside the viewport.
  Evidence: The initial composition offers no visible way to return to its starting state.

- [ ] `scroll_click_bug-13` · P3 · proposal. Fixture frame: Tighten the outer frame around the list and explanatory text.
  Evidence: A large light panel surrounds a comparatively small test area.

- [ ] `scroll_click_bug-14` · P2 · proposal. Clipping boundary: Label the viewport bottom as the intentional clip boundary.
  Evidence: The partially visible final button could look accidental to a reviewer.

- [ ] `scroll_click_bug-15` · P3 · proposal. Index convention: Add a Zero-based indices note beside the count.
  Evidence: Buttons begin at 0 with no explanation.

### scroll_clip_bug

[Baseline](screenshot-baselines/screens/scroll_clip_bug_720p.png) · [Visual review](design-audit.html#screen=scroll_clip_bug)

- [ ] `scroll_clip_bug-01` · P2 · proposal. Instruction line: Increase it to a readable 13 px actual size.
  Evidence: The all-four-panels instruction is extremely small.

- [ ] `scroll_clip_bug-02` · P2 · proposal. Panel labels: Increase caption size and keep text white with a separate colored swatch.
  Evidence: Control, ClipChild rows, Nested wrapper and Scroll in Scroll are tiny colored captions.

- [ ] `scroll_clip_bug-03` · P2 · proposal. Control caption: Add normal rows as its secondary caption.
  Evidence: Control gives no structural explanation.

- [ ] `scroll_clip_bug-04` · P2 · proposal. ClipChild caption: Add each row clips its children below the caption.
  Evidence: ClipChild rows exposes an implementation term without defining it.

- [ ] `scroll_clip_bug-05` · P2 · proposal. Nested wrapper caption: Add a small structure diagram above or below the panel.
  Evidence: Nested wrapper does not show which boundary belongs to the wrapper.

- [ ] `scroll_clip_bug-06` · P2 · proposal. Nested scroll caption: Label both boundaries with matching callout markers.
  Evidence: Scroll in Scroll does not distinguish inner and outer scroll regions.

- [ ] `scroll_clip_bug-07` · P2 · proposal. Bottom nested border: Add a visible horizontal outer-boundary guide to make intended clipping reviewable.
  Evidence: The inner dark frame extends below the common outer bottom line.

- [ ] `scroll_clip_bug-08` · P2 · proposal. Scroll offsets: Add one numeric offset per panel outside the clip region.
  Evidence: No panel reports its current scroll offset.

- [ ] `scroll_clip_bug-09` · P2 · proposal. Comparison state: Add a companion preset that places all panels at the same nonzero offset.
  Evidence: All panels start at row 1, so the target scrolled state is absent.

- [ ] `scroll_clip_bug-10` · P2 · proposal. Row labels: Add matching numeric markers at a common x inset for easier alignment comparison.
  Evidence: Ctrl, Clip, Nest and Inner prefixes differ across comparison panels.

- [ ] `scroll_clip_bug-11` · P3 · proposal. Row text: Increase label size without changing the measured row dimensions.
  Evidence: Row identifiers are very small inside 40 px colored bands.

- [ ] `scroll_clip_bug-12` · P2 · proposal. Test count: Add total count to each pane caption.
  Evidence: The screenshot does not state the total rows per pane.

- [ ] `scroll_clip_bug-13` · P2 · proposal. Expected boundary rule: Add a clearly keyed boundary legend outside the fixture.
  Evidence: The top note mentions colored borders, which are thin and subtle.

- [ ] `scroll_clip_bug-14` · P2 · proposal. Nested scrolling instructions: Add a concise two-step nested-scroll procedure beneath the title.
  Evidence: The screenshot gives no way to distinguish moving inner versus outer content.

- [ ] `scroll_clip_bug-15` · P3 · proposal. Bottom margin: Reserve a small external margin for boundary labels without resizing the test viewports.
  Evidence: The test panes run nearly to the viewport edge.

- [ ] `scroll_clip_bug-16` · P2 · proposal. Reset comparison: Add Reset all and Set comparison offset controls above the four panes.
  Evidence: No reset or synchronized positioning control is visible.

### scroll_view

[Baseline](screenshot-baselines/screens/scroll_view_720p.png) · [Visual review](design-audit.html#screen=scroll_view)

- [ ] `scroll_view-01` · P2 · defect. Vertical row text: Inset the labels by 12 px.
  Evidence: Item labels touch their blue cards' left edges.

- [ ] `scroll_view-02` · P2 · defect. Horizontal item text: Center the numbers or use the same 12 px inset as vertical rows.
  Evidence: Numbers 1 through 5 sit flush against card left edges.

- [ ] `scroll_view-03` · P2 · proposal. Invert Scroll control: Render a labeled toggle with an explicit current state.
  Evidence: Invert Scroll is a gray rectangle with no visible on/off state.

- [ ] `scroll_view-04` · P2 · proposal. Horizontal progress bar: Label it visible fraction or scroll position according to what it represents.
  Evidence: A blue segment sits in a gray bar below a 0% label.

- [ ] `scroll_view-05` · P2 · proposal. Duplicate horizontal bars: Explain the second indicator or combine the redundant visualizations.
  Evidence: The horizontal view has a scrollbar and another unlabeled bar below it.

- [ ] `scroll_view-06` · P2 · proposal. Position labels: Use First visible item and include the full visible range.
  Evidence: Item 1 of 20 and Item 1 of 15 can imply one visible item.

- [ ] `scroll_view-07` · P3 · proposal. Panel heights: Align outer demo cards while retaining different internal viewport heights.
  Evidence: The vertical viewport is taller than the horizontal viewport.

- [ ] `scroll_view-08` · P2 · proposal. Horizontal blank space: Reduce the viewport to content height or label the intentional spare area.
  Evidence: A large empty white area sits below the horizontal cards.

- [ ] `scroll_view-09` · P2 · proposal. Scroll hints: Place the vertical gesture near Vertical Scroll and the horizontal gesture near Horizontal Scroll.
  Evidence: A long instruction line blends both axis gestures together.

- [ ] `scroll_view-10` · P3 · proposal. Header padding: Add a consistent 12 px title inset.
  Evidence: Scroll View Demo touches its backing bar left edge.

- [ ] `scroll_view-11` · P2 · proposal. Instruction contrast: Use a darker neutral text token.
  Evidence: The gray gesture instruction is faint on the light panel.

- [ ] `scroll_view-12` · P2 · proposal. Vertical clip sample: Add an intentional partial item caption below the viewport.
  Evidence: Item 5 is partly shown without explanatory framing.

- [ ] `scroll_view-13` · P2 · proposal. Horizontal extent: Add a right-edge continuation cue or adjust the viewport to reveal a partial sixth card.
  Evidence: Five cards are fully visible even though more items exist.

- [ ] `scroll_view-14` · P2 · proposal. Scroll counts: Add 20 items and 15 items beside the pane headings.
  Evidence: The total counts appear only below the panes.

- [ ] `scroll_view-15` · P3 · proposal. Content balance: Tighten the page height or add a concise behavior comparison there.
  Evidence: Both demos occupy the top half with a large unused lower panel.

- [ ] `scroll_view-16` · P2 · proposal. Reset position: Add a shared Reset position action outside both panes.
  Evidence: No visible control returns either view to its start.

- [ ] `scroll_view-17` · P2 · proposal. Inversion scope: Label it Invert both scroll axes if that matches the implementation.
  Evidence: The single Invert Scroll control does not name affected axes.

- [ ] `scroll_view-18` · P3 · proposal. Indicator alignment: Align the indicator with the horizontal viewport left edge.
  Evidence: The extra horizontal indicator is centered while text aligns left.

- [ ] `scroll_view-19` · P2 · proposal. Demonstration state: Add a labeled scrolled-state preset for side-by-side verification.
  Evidence: Both panes show only the initial zero-percent state.

### scrollbar_style_lab

[Baseline](screenshot-baselines/screens/scrollbar_style_lab_720p.png) · [Visual review](design-audit.html#screen=scrollbar_style_lab)

- [ ] `scrollbar_style_lab-01` · P2 · proposal. Main title: Inset it by 16 px to align with the first pane.
  Evidence: The long title begins at the extreme left edge.

- [ ] `scrollbar_style_lab-02` · P2 · proposal. Variant captions: Increase caption size to a readable 13 px actual size.
  Evidence: default, usage: Accent thumb, explicit colours and thicker, no track are small.

- [ ] `scrollbar_style_lab-03` · P2 · proposal. Default variant: Add a two-line token caption beneath it.
  Evidence: The default pane does not name its track or thumb tokens.

- [ ] `scrollbar_style_lab-04` · P2 · proposal. Accent variant: Add an Accent swatch and resolved color value outside the pane.
  Evidence: The magenta thumb is a deliberate theme usage but its token is only in the title.

- [ ] `scrollbar_style_lab-05` · P2 · proposal. Explicit colors variant: Label both RGBA values beneath the pane.
  Evidence: The orange thumb and dark red track have no color values shown.

- [ ] `scrollbar_style_lab-06` · P2 · proposal. Thicker variant: Label its 14 px thickness.
  Evidence: The thicker teal thumb has no numeric width annotation.

- [ ] `scrollbar_style_lab-07` · P2 · proposal. Transparent track: Add an explicit Track alpha 0 caption.
  Evidence: No track can be mistaken for a missing renderer.

- [ ] `scrollbar_style_lab-08` · P2 · proposal. Row count: Add the full row count to each pane's metadata.
  Evidence: Rows 00 through 13 are visible with no total count.

- [ ] `scrollbar_style_lab-09` · P2 · proposal. Scroll state: Add a shared scroll offset readout for comparison.
  Evidence: Equal thumb positions are shown without reported offsets.

- [ ] `scrollbar_style_lab-10` · P2 · proposal. Thumb size: Show viewport/content ratio below the columns.
  Evidence: Thumb length is visibly comparable but undocumented.

- [ ] `scrollbar_style_lab-11` · P3 · proposal. Row text: Increase their type size while keeping row heights fixed.
  Evidence: Row identifiers are tiny relative to the empty pane width.

- [ ] `scrollbar_style_lab-12` · P2 · proposal. Comparison controls: Add shared start, middle and end presets outside the panes.
  Evidence: No control sets every variant to the same scrolled position.

- [ ] `scrollbar_style_lab-13` · P2 · proposal. Track/thumb distinction: Use a small annotated scrollbar key in the lower empty area.
  Evidence: The title mentions track and thumb without a diagram.

- [ ] `scrollbar_style_lab-14` · P3 · proposal. Unused lower area: Use it for the variant specification table.
  Evidence: Nearly half the screen is blank.

- [ ] `scrollbar_style_lab-15` · P2 · proposal. Baseline variant naming: Use consistent human-readable headings and put API tokens in smaller metadata.
  Evidence: default is lowercase while Accent uses a token-like capital.

- [ ] `scrollbar_style_lab-16` · P3 · proposal. Width comparison: Add a width-only comparison swatch alongside its metadata to isolate thickness.
  Evidence: The thick variant changes both color and width.

### secure_tunnel

[Baseline](screenshot-baselines/screens/secure_tunnel_720p.png) · [Visual review](design-audit.html#screen=secure_tunnel)

- [ ] `secure_tunnel-01` · P2 · defect. Server metadata: Increase them to readable 12 to 13 px actual text with darker contrast.
  Evidence: Countries and latency values are very small and pale.

- [ ] `secure_tunnel-02` · P2 · defect. Simulation disclaimer: Place a readable Demo mode badge near the connection state.
  Evidence: Network simulation only is tiny at the bottom center.

- [ ] `secure_tunnel-03` · P2 · proposal. Protocol control: Add Change protocol or a cycle icon beside the value to explain the existing cycle action.
  Evidence: WireGuard appears as plain text with no visible change affordance.

- [ ] `secure_tunnel-04` · P2 · defect. Privacy icon: Replace it with a recognizable centered shield outline.
  Evidence: The shield illustration looks like overlapping thin geometric fragments.

- [ ] `secure_tunnel-05` · P2 · proposal. Rail navigation: Add short labels or an expanded navigation legend for VPN, protection, routing, diagnostics and settings.
  Evidence: Five icon controls have no visible text labels.

- [ ] `secure_tunnel-06` · P2 · proposal. Active rail icon: Use a recognizable connection or globe icon matching the map heading.
  Evidence: The active concentric-ring symbol does not identify VPN.

- [ ] `secure_tunnel-07` · P3 · proposal. Brand mark: Refine it into a consistent solid brand mark with balanced stroke weight.
  Evidence: The top rail logo reads as a thin triangle.

- [ ] `secure_tunnel-08` · P2 · proposal. Connection indicator: Use a larger status dot and explicit Disconnected text.
  Evidence: Not connected has a tiny orange diamond.

- [ ] `secure_tunnel-09` · P2 · proposal. Quick-connect recommendation: Explain the recommendation criterion or recommend the displayed lowest-latency server.
  Evidence: Fastest available server says Lisbon while Reykjavik shows a lower latency.

- [ ] `secure_tunnel-10` · P2 · proposal. Recommendation copy: Increase contrast and separate server name with stronger weight.
  Evidence: Fastest available server - Lisbon is faint under the button.

- [ ] `secure_tunnel-11` · P2 · proposal. Search affordance: Add a leading search icon and a stronger field border.
  Evidence: Search locations looks like a blank pale field without a search icon.

- [ ] `secure_tunnel-12` · P3 · proposal. Location count: Use a compact 6 locations label or count badge beside the heading.
  Evidence: The number 6 floats at the far right of ALL LOCATIONS.

- [ ] `secure_tunnel-13` · P2 · proposal. Selected server: Add a checkmark or Selected label.
  Evidence: Lisbon is tinted but its selected state has only color.

- [ ] `secure_tunnel-14` · P2 · proposal. Selected-server latency: Use a fixed right-aligned latency column and separate chevron gutter.
  Evidence: 27 ms is near the row chevron with little room.

- [ ] `secure_tunnel-15` · P3 · proposal. Flag artwork: Use a consistent flag asset set at equal dimensions.
  Evidence: The small rectangular flags are simplified and visually inconsistent.

- [ ] `secure_tunnel-16` · P2 · proposal. Map geography: Refine the shared map artwork with clearer continental silhouettes while retaining its muted visual weight.
  Evidence: The current screenshot and rendered mock both use coarse continent polygons that obscure recognizable regions.

- [ ] `secure_tunnel-17` · P2 · proposal. Map markers: Use a selected marker plus restrained labeled server markers.
  Evidence: Several dots have no labels or visual connection to listed servers.

- [ ] `secure_tunnel-18` · P2 · proposal. Lisbon callout: Add a small pointer anchored to the selected marker.
  Evidence: The callout sits below the marker without a connector.

- [ ] `secure_tunnel-19` · P2 · proposal. Callout status dot: Give it a meaning label or remove it as redundant decoration.
  Evidence: A tiny blue dot appears at the right of Lisbon metadata.

- [ ] `secure_tunnel-20` · P3 · proposal. Callout metadata: Increase its size and use clear separators with equal spacing.
  Evidence: Portugal - pt-04 - 27 ms is narrow and faint.

- [ ] `secure_tunnel-21` · P2 · proposal. Account button: Show an account avatar with an Account label or recognizable profile icon.
  Evidence: GO in a pale circle could be an action or initials.

- [ ] `secure_tunnel-22` · P2 · proposal. Footer IP state: Emphasize IP not hidden while disconnected.
  Evidence: Not hidden is visually as quiet as other metadata.

- [ ] `secure_tunnel-23` · P2 · proposal. Footer route state: Use Not connected as the route summary, retaining route details in diagnostics.
  Evidence: No route is technical language for a consumer connection screen.

- [ ] `secure_tunnel-24` · P2 · proposal. Diagnostics link: Increase contrast and add the outward arrow shown in the target.
  Evidence: View diagnostics is faint at the far right footer.

- [ ] `secure_tunnel-25` · P3 · proposal. Privacy card size: Reduce its height or add a clear connection-status detail within the reserved space.
  Evidence: The large card carries a short heading and one line of copy.

- [ ] `secure_tunnel-26` · P2 · proposal. Application font parity: Match the target sans-serif family and weights across titles, location rows and metadata.
  Evidence: The current title and body labels mix narrow display text and widely spaced metadata, while the target uses a compact sans-serif hierarchy.

- [ ] `secure_tunnel-27` · P2 · proposal. Map marker parity: Restore the target marker set and use its consistent white ring thickness.
  Evidence: Current map dots have very thin outlines and fewer visible points than the target's white-ring markers.

- [ ] `secure_tunnel-28` · P3 · proposal. Map backdrop parity: Use the target's smooth pale background without the prominent circular boundary.
  Evidence: A large pale circular disk is visible behind the current map but absent from the rendered target.

### self_align

[Baseline](screenshot-baselines/screens/self_align_720p.png) · [Visual review](design-audit.html#screen=self_align)

- [ ] `self_align-01` · P2 · proposal. Parent frame: Label it Column parent with its default align-items value.
  Evidence: The outlined parent has no label identifying its alignment.

- [ ] `self_align-02` · P2 · proposal. Axis context: Add a horizontal cross-axis arrow above the frame.
  Evidence: The subtitle says alignment but does not identify the horizontal cross axis.

- [ ] `self_align-03` · P2 · proposal. Vertical stacking: Add a vertical main-axis arrow outside the frame.
  Evidence: The three examples descend diagonally without explaining stacking.

- [ ] `self_align-04` · P2 · proposal. Start specimen: Annotate the intentional inset so it is not mistaken for alignment error.
  Evidence: FlexStart left is separated from the border by an unlabeled inset.

- [ ] `self_align-05` · P2 · proposal. Center specimen: Add a faint center guide behind the specimen.
  Evidence: Center has no guide showing the parent's center.

- [ ] `self_align-06` · P2 · proposal. End specimen: Add a small right-edge alignment tick.
  Evidence: FlexEnd right has no guide tying its edge to the parent.

- [ ] `self_align-07` · P2 · proposal. API naming: Use consistent API names and put plain-language translations below them.
  Evidence: FlexStart and FlexEnd are mixed with plain Center.

- [ ] `self_align-08` · P3 · proposal. Legend duplication: Replace the pills with a small mapping from SelfAlign values to resulting positions.
  Evidence: Left, Center and Right repeat the box labels without new information.

- [ ] `self_align-09` · P2 · proposal. Specimen text: Increase labels to 14 px actual size while preserving box geometry.
  Evidence: Text inside the large colored boxes is small.

- [ ] `self_align-10` · P2 · proposal. Inherited comparison: Add a separate labeled default-alignment comparison outside the existing fixture.
  Evidence: All three children override alignment, so inheritance is unseen.

- [ ] `self_align-11` · P3 · proposal. Title wording: Use SelfAlign: child alignment overrides.
  Evidence: SelfAlign Feature Demo repeats generic demo wording.

- [ ] `self_align-12` · P3 · proposal. Summary line: Replace it with a concrete explanation that the parent remains a column.
  Evidence: Each element picks its own position repeats the subtitle.

- [ ] `self_align-13` · P2 · proposal. Color key: Add textual value labels to the legend so color is optional for interpretation.
  Evidence: Green, brown and orange have no reason beyond position.

- [ ] `self_align-14` · P3 · proposal. Page footprint: Center the complete title-diagram-legend group vertically with even top and bottom space.
  Evidence: The compact diagram floats inside broad empty margins.

### separators

[Baseline](screenshot-baselines/screens/separators_720p.png) · [Visual review](design-audit.html#screen=separators)

- [ ] `separators-01` · P2 · proposal. Right card heading: Rename the card Mixed separator examples or split the gallery into its own subsection.
  Evidence: Vertical Separators also contains a horizontal color gallery.

- [ ] `separators-02` · P2 · proposal. Default sample label: Add the default pixel thickness to its label.
  Evidence: Basic separator - default style omits the actual thickness.

- [ ] `separators-03` · P2 · proposal. Six-pixel sample: Add a small thickness marker beside the line.
  Evidence: The thick line has a dimension in text but no direct ruler.

- [ ] `separators-04` · P2 · proposal. Partial-width sample: Show a faint full-width guide behind the purple specimen.
  Evidence: 60% centered has no full-width reference.

- [ ] `separators-05` · P2 · proposal. Labeled OR sample: Add a caption naming automatic label gap sizing.
  Evidence: OR has no explanation of its text gap behavior.

- [ ] `separators-06` · P2 · proposal. Labeled Settings sample: Add a note identifying the long-label and accent-color variation.
  Evidence: Settings differs from OR in color and word length simultaneously.

- [ ] `separators-07` · P2 · proposal. Navigation sample: Label the row Static navigation example unless it is interactive.
  Evidence: Home, Settings, Profile and Help look like a working navigation bar.

- [ ] `separators-08` · P2 · proposal. Vertical color changes: Add small color names below the sample or a concise caption.
  Evidence: Three vertical dividers use different colors without a key.

- [ ] `separators-09` · P3 · proposal. Vertical dimensions: Display their dimensions beneath the navigation example.
  Evidence: The vertical separators have no stated height or thickness.

- [ ] `separators-10` · P3 · proposal. Color gallery width: Add one shared Full width label for the gallery.
  Evidence: The four color lines are long but have no width specification.

- [ ] `separators-11` · P2 · proposal. Color gallery semantics: Include token names or hex values beside the color labels.
  Evidence: Blue, Green, Purple and Orange identify hues but not tokens.

- [ ] `separators-12` · P2 · proposal. Section hierarchy: Use a consistent subsection-heading treatment for both.
  Evidence: Color Gallery is embedded in a separator while Labeled Separators is a heading.

- [ ] `separators-13` · P3 · proposal. Footer sentence: Replace it with a note that line dimensions remain fixed across color variants.
  Evidence: Separators help organize and structure UI content states no testable detail.

- [ ] `separators-14` · P2 · proposal. Sample grouping: Use consistent labeled specimen rows with equal text-to-line gaps.
  Evidence: Descriptions and lines are separated only by white space.

- [ ] `separators-15` · P3 · proposal. Left card density: Align subsection baselines to make comparison easier.
  Evidence: The left card has six samples while the right has a large navigation sample and four colors.

- [ ] `separators-16` · P3 · proposal. Heading size: Slightly reduce the title and give the specifications more readable space.
  Evidence: The page title is much larger than sample metadata.

- [ ] `separators-17` · P2 · proposal. Decorative-versus-structural distinction: Add a small labeled before/after content grouping example outside the existing specimens.
  Evidence: The screen gives no example of a separator in ordinary content.

### setting_row_showcase

[Baseline](screenshot-baselines/screens/setting_row_showcase_720p.png) · [Visual review](design-audit.html#screen=setting_row_showcase)

- [ ] `setting_row_showcase-01` · P2 · defect. Footer inventory: Show those specimen rows or restrict the footer to the three visible families.
  Evidence: Toggle, Stepper, Slider, Dropdown, Display lists dropdown and display examples that are not visible.

- [ ] `setting_row_showcase-02` · P2 · proposal. Control columns: Use one shared control column across row types.
  Evidence: Toggles and steppers align at the right edge, while sliders sit near the center.

- [ ] `setting_row_showcase-03` · P2 · proposal. Value column: Place percentages immediately after each slider in a consistent value column.
  Evidence: 80% and 65% sit far from their sliders.

- [ ] `setting_row_showcase-04` · P2 · proposal. Slider track: Add a visible value fill while retaining the green thumb.
  Evidence: The dark tracks do not show a filled portion.

- [ ] `setting_row_showcase-05` · P2 · proposal. Master slider mapping: Add labeled endpoints or fix the visual mapping so the reported percentage matches the usable range.
  Evidence: The 80% thumb appears only about two-thirds across the visible track.

- [ ] `setting_row_showcase-06` · P2 · proposal. Music slider mapping: Use the same explicit range mapping as Master Volume and label its endpoints.
  Evidence: 65% appears just right of the track midpoint.

- [ ] `setting_row_showcase-07` · P2 · proposal. Toggle state text: Add On and Off text beside the switches.
  Evidence: Music and Sound Effects are blue and Vibration gray, without state words.

- [ ] `setting_row_showcase-08` · P2 · proposal. Label hierarchy: Use one label size for all setting rows.
  Evidence: Master Volume and Music Volume are larger than Music, Sound Effects and Vibration.

- [ ] `setting_row_showcase-09` · P2 · proposal. Language arrows: Use clearer chevrons with consistent visible button bounds.
  Evidence: The thin chevrons around English are small and faint.

- [ ] `setting_row_showcase-10` · P2 · proposal. Graphics arrows: Put the three elements in a compact outlined stepper group.
  Evidence: High floats between two tiny chevrons with no grouping.

- [ ] `setting_row_showcase-11` · P3 · proposal. Language option context: Add a small option count in the specimen caption.
  Evidence: English has no indication of position within available choices.

- [ ] `setting_row_showcase-12` · P2 · proposal. Graphics label: Rename it Graphics quality.
  Evidence: Graphics is broad while the selected value is High.

- [ ] `setting_row_showcase-13` · P2 · proposal. Section spacing: Reduce gaps and add thin neutral dividers to clarify grouping.
  Evidence: Large blank gaps separate Toggles, Options and Volume.

- [ ] `setting_row_showcase-14` · P3 · proposal. Title panel: Add a modest gap or divider below the title.
  Evidence: The title bar blends into the larger content panel.

- [ ] `setting_row_showcase-15` · P2 · proposal. Section captions: Increase caption size and align their baselines to a consistent inset.
  Evidence: TOGGLES, OPTIONS and VOLUME are small muted labels.

- [ ] `setting_row_showcase-16` · P2 · proposal. Music relationship: Add a brief note explaining the relationship between enablement and volume.
  Evidence: Music appears as both a toggle and Music Volume in separate groups.

- [ ] `setting_row_showcase-17` · P2 · proposal. Row boundaries: Add subtle full-width row separators to aid horizontal scanning.
  Evidence: Wide empty space separates labels and controls with no row guides.

- [ ] `setting_row_showcase-18` · P3 · proposal. Slider size: Increase track width to make percentage differences easier to compare.
  Evidence: The 152 px tracks occupy a small fraction of the wide panel.

- [ ] `setting_row_showcase-19` · P2 · proposal. Initial values: Add a compact default-state summary below the specimen panel.
  Evidence: The screen has no summary of the demonstrated defaults.

- [ ] `setting_row_showcase-20` · P3 · proposal. Footer contrast: Increase its contrast after correcting the inventory.
  Evidence: The small family list is dim on the black margin.

### shop_interface

[Baseline](screenshot-baselines/screens/shop_interface_720p.png) · [Visual review](design-audit.html#screen=shop_interface)

- [ ] `shop_interface-01` · P2 · defect. List headings: Align headings to the same column grid as the rows.
  Evidence: TYPE, STOCK and PRICE do not align with their data columns.

- [ ] `shop_interface-02` · P2 · defect. Basket count: Increase it to readable 12 px text and bring it closer to Your basket.
  Evidence: 5 items is tiny in the basket heading corner.

- [ ] `shop_interface-03` · P2 · defect. Gold remaining: Increase it to 13 px and give it its own line spacing.
  Evidence: Gold remaining: 3,570 G is very small beneath Total.

- [ ] `shop_interface-04` · P2 · defect. Guild note: Add at least 10 px bottom inset.
  Evidence: The note almost touches the item panel's bottom border.

- [ ] `shop_interface-05` · P2 · defect. Footer input hints: Use readable text and distinct keycaps.
  Evidence: A Select, X Add to cart and Y Remove are tiny against the dark wood.

- [ ] `shop_interface-06` · P2 · proposal. Market context: Increase actual text size and reduce tracking.
  Evidence: WILLOWBROOK - MARKET DISTRICT is widely spaced and small.

- [ ] `shop_interface-07` · P2 · proposal. Day status: Use a readable day label and a recognizable sun icon.
  Evidence: Day 12 and its tiny sun mark are difficult to read.

- [ ] `shop_interface-08` · P3 · proposal. Store ornaments: Replace them with balanced decorative motifs matching the parchment style.
  Evidence: The title is flanked by asterisks rather than the richer target ornaments.

- [ ] `shop_interface-09` · P2 · proposal. Gold total: Add G or a labeled coin unit while retaining YOUR GOLD below.
  Evidence: 3,750 lacks a visible unit beside the number.

- [ ] `shop_interface-10` · P2 · proposal. Buy and Sell tabs: Give both tabs equal dimensions and a clearer active underline.
  Evidence: Sell is text on the header with a weak tab boundary.

- [ ] `shop_interface-11` · P2 · proposal. Selected item: Add a small selected marker or stronger left accent.
  Evidence: The potion row uses a pale fill and thin outline only.

- [ ] `shop_interface-12` · P3 · proposal. Item artwork: Use consistent item artwork with stronger silhouettes at equal visual size.
  Evidence: Sword, armor and shield icons are tiny simplified shapes.

- [ ] `shop_interface-13` · P2 · proposal. Stock values: Increase size and right-align them in a fixed column.
  Evidence: Ordinary stock numbers are small and far from their heading.

- [ ] `shop_interface-14` · P2 · proposal. Low stock: Add a Low stock badge while retaining the numeric count.
  Evidence: 3 left and 2 left use red text but no additional marker.

- [ ] `shop_interface-15` · P2 · proposal. Price formatting: Use a fixed gap and align all numeric values to the right.
  Evidence: Prices combine amount and G with inconsistent perceived spacing.

- [ ] `shop_interface-16` · P2 · proposal. Detail category: Use a readable uppercase category label with normal tracking.
  Evidence: Consumable is extremely letter-spaced and small.

- [ ] `shop_interface-17` · P2 · proposal. Detail potion art: Add fill, highlights and a grounded shadow consistent with the target art.
  Evidence: The large potion is a sparse outline compared with dense text styling.

- [ ] `shop_interface-18` · P2 · proposal. Detail price: Label it Price per item.
  Evidence: 50 G sits below the description without a Unit price label.

- [ ] `shop_interface-19` · P2 · proposal. Add to cart button: Use Add to basket consistently.
  Evidence: The button says Add to cart while the destination is called Your basket.

- [ ] `shop_interface-20` · P2 · proposal. Basket quantities: Add compact minus and plus controls beside each quantity.
  Evidence: x 3 and x 2 are small text with no visible adjustment controls.

- [ ] `shop_interface-21` · P2 · proposal. Remove action: Add a remove control to each basket line.
  Evidence: Remove appears only in the footer with no visible basket affordance.

- [ ] `shop_interface-22` · P2 · proposal. Basket line pricing: Add a small Line total heading or unit-price detail.
  Evidence: 150 G and 30 G are shown without explicit line-total context.

- [ ] `shop_interface-23` · P2 · proposal. Purchase action: Give checkout the stronger primary emphasis and include 180 G in its label.
  Evidence: Purchase goods uses the same dark style as Add to cart.

- [ ] `shop_interface-24` · P2 · proposal. Checkout spacing: Increase the lower inset to match the side padding.
  Evidence: Purchase goods nearly touches the basket panel bottom.

- [ ] `shop_interface-25` · P3 · proposal. Footer status: Use that status area for selected-item or basket feedback instead.
  Evidence: Welcome, adventurer! repeats the greeting at the top.

- [ ] `shop_interface-26` · P2 · proposal. Detail-title typography parity: Increase the detail title weight and keep the description subordinate.
  Evidence: Health Potion is light in the current detail panel while the rendered target uses a bold product heading.

- [ ] `shop_interface-27` · P3 · proposal. Wood backdrop parity: Match the target backdrop so geometric facets do not compete with the parchment panels.
  Evidence: The current background has broad angled dark facets behind the cards; the target uses an even wood-grain field.

- [ ] `shop_interface-28` · P2 · proposal. Basket amount emphasis parity: Use the target's stronger amount weight to align basket scanning with the bold overall total.
  Evidence: 150 G and 30 G are light in the current basket while the target uses bold line totals.

### simple_button

[Baseline](screenshot-baselines/screens/simple_button_720p.png) · [Visual review](design-audit.html#screen=simple_button)

- [ ] `simple_button-01` · P2 · proposal. Main action label: Use Add a click or Increment counter.
  Evidence: Click Me! names the input gesture rather than the counter action.

- [ ] `simple_button-02` · P2 · proposal. Counter state: Show Click count: 0 in a stable-width result region.
  Evidence: No clicks yet... hides the numeric initial value.

- [ ] `simple_button-03` · P2 · proposal. Counter association: Add a small Result caption above the count.
  Evidence: The gray pill has no label identifying it as button feedback.

- [ ] `simple_button-04` · P2 · proposal. Subtitle: Explain that the main action increments the count.
  Evidence: Go ahead, give it a click! repeats the main button wording.

- [ ] `simple_button-05` · P2 · proposal. More Styles heading: Increase it to a readable subsection heading.
  Evidence: More Styles is much smaller than surrounding labels.

- [ ] `simple_button-06` · P2 · proposal. Secondary specimen: Add a caption stating whether variant buttons share the counter behavior.
  Evidence: Secondary identifies a style but not its relationship to the counter.

- [ ] `simple_button-07` · P2 · proposal. Outline specimen: Strengthen its outline without changing the outline-style specimen.
  Evidence: The white button has a very fine lavender edge.

- [ ] `simple_button-08` · P2 · proposal. Bottom specimen labels: Give each specimen a standalone label describing its style.
  Evidence: Small, Pill and Buttons form a phrase instead of naming three variations.

- [ ] `simple_button-09` · P2 · proposal. Pill specimen: Label the intended radius numerically or render an unmistakable capsule in that specimen.
  Evidence: Pill appears almost as rectangular as Small and Buttons.

- [ ] `simple_button-10` · P3 · proposal. Size comparison: Add its height below the row and the main button height in the same legend.
  Evidence: Small is shown without a dimension reference.

- [ ] `simple_button-11` · P3 · proposal. Button alignment: Align all specimen groups to a shared inner card grid.
  Evidence: The main button and lower rows use slightly different left insets.

- [ ] `simple_button-12` · P2 · proposal. Color comparison: Use a small legend that names each token so comparison is explicit.
  Evidence: Pastel green, pink and gold buttons change hue and label simultaneously.

- [ ] `simple_button-13` · P2 · proposal. Reset action: Add a quiet Reset count action near the result.
  Evidence: No reset control is visible for the click count.

- [ ] `simple_button-14` · P3 · proposal. Footer space: Use it for specimen dimensions and shared behavior notes.
  Evidence: A large blank region remains beneath the small buttons inside the card.

- [ ] `simple_button-15` · P3 · proposal. Decoration distribution: Group them closer to the card or reduce their number to keep attention on the examples.
  Evidence: Confetti dots are scattered far from the central card.

- [ ] `simple_button-16` · P2 · proposal. Initial-state coverage: Add a companion activated-state baseline showing the counter feedback.
  Evidence: Only the zero-click state is visible.

### split_pane_lab

[Baseline](screenshot-baselines/screens/split_pane_lab_720p.png) · [Visual review](design-audit.html#screen=split_pane_lab)

- [ ] `split_pane_lab-01` · P2 · defect. Pane labels: Inset each label by 12 px without changing split geometry.
  Evidence: sidebar, preview and details touch their panes' left edges.

- [ ] `split_pane_lab-02` · P2 · proposal. Vertical divider: Add a centered grip indicator within the divider.
  Evidence: The narrow vertical separator is hard to distinguish from a pane edge.

- [ ] `split_pane_lab-03` · P2 · proposal. Horizontal divider: Add a short centered grip with a contrasting stroke.
  Evidence: The horizontal bar has no visible drag handle.

- [ ] `split_pane_lab-04` · P2 · proposal. Ratio caption: Increase it to 13 px actual text.
  Evidence: sidebar 28% preview 62% is tiny beneath the title.

- [ ] `split_pane_lab-05` · P2 · proposal. Ratio meaning: Label it Preview height: 62% of right pane.
  Evidence: Preview 62% could be interpreted as a share of the whole screen.

- [ ] `split_pane_lab-06` · P2 · proposal. Sidebar ratio: Label it Sidebar width: 28% of workspace.
  Evidence: Sidebar 28% has no dimension unit context.

- [ ] `split_pane_lab-07` · P2 · proposal. Details ratio: Show its complementary height percentage.
  Evidence: The details pane has no corresponding size readout.

- [ ] `split_pane_lab-08` · P2 · proposal. Minimum limits: Display the actual minimum and maximum ratios beside the readouts.
  Evidence: The screenshot shows no allowed resize range.

- [ ] `split_pane_lab-09` · P2 · proposal. Reset geometry: Add Reset to 28% / 62% above the workspace.
  Evidence: No reset action is visible.

- [ ] `split_pane_lab-10` · P2 · proposal. Pixel dimensions: Display live width × height in each pane center.
  Evidence: Large empty panes do not show how resizing affects their size.

- [ ] `split_pane_lab-11` · P2 · proposal. Nesting explanation: Add a small diagram that shows the horizontal split belongs only to the right pane.
  Evidence: The title says drag either divider without explaining the nested split.

- [ ] `split_pane_lab-12` · P3 · proposal. Footer hint: Increase its size and align it with the workspace inset.
  Evidence: The resize-cursor instruction is very small.

- [ ] `split_pane_lab-13` · P2 · proposal. Grip hit region: Show an external annotation describing the actual hit-region width.
  Evidence: Thin visual dividers give little indication of their draggable extent.

- [ ] `split_pane_lab-14` · P3 · proposal. Pane headings: Use consistent sentence-case headings with slightly larger type.
  Evidence: Lowercase labels are visually weak over large blank fields.

- [ ] `split_pane_lab-15` · P2 · proposal. Static review evidence: Add labeled narrow-sidebar and tall-preview presets for additional baselines.
  Evidence: Only one split ratio is captured.

- [ ] `split_pane_lab-16` · P3 · proposal. Title line: Separate Split Pane Lab from a short interaction subtitle.
  Evidence: The instruction is embedded in the page title.

### sports_settings

[Baseline](screenshot-baselines/screens/sports_settings_720p.png) · [Visual review](design-audit.html#screen=sports_settings)

- [ ] `sports_settings-01` · P2 · defect. Controller tab hints: Increase glyph size and keycap dimensions.
  Evidence: LB and RB labels are nearly microscopic in pale keycaps.

- [ ] `sports_settings-02` · P2 · proposal. Screen percentage value: Display 100%.
  Evidence: 100 appears without a percent symbol.

- [ ] `sports_settings-03` · P2 · proposal. Resolution formatting: Use 2560 × 1440.
  Evidence: 2,560x1,440 uses thousands separators and a lowercase x.

- [ ] `sports_settings-04` · P2 · proposal. Window mode value: Replace ambiguous dots with clear cycling chevrons or a dropdown indicator.
  Evidence: Borderless window is followed by small dot marks.

- [ ] `sports_settings-05` · P2 · proposal. V Sync value: Use a consistent on/off control treatment.
  Evidence: Enabled is followed by dots instead of a recognizable control.

- [ ] `sports_settings-06` · P2 · proposal. Anti-aliasing method: Expand the method name in the help pane when selected.
  Evidence: TSR is an unexplained acronym.

- [ ] `sports_settings-07` · P2 · proposal. Dynamic resolution: Distinguish binary settings from multi-choice selectors.
  Evidence: Enabled has the same dot pattern as multi-choice settings.

- [ ] `sports_settings-08` · P2 · proposal. Motion blur value: Use On or a labeled intensity percentage according to the real value meaning.
  Evidence: Motion blur shows 1 with no unit or scale.

- [ ] `sports_settings-09` · P2 · proposal. Max FPS units: Display 120 FPS.
  Evidence: 120 has no FPS suffix in the selected row.

- [ ] `sports_settings-10` · P2 · proposal. Framerate target units: Display 60 FPS and explain its relationship to dynamic resolution.
  Evidence: 60 has no FPS suffix.

- [ ] `sports_settings-11` · P2 · proposal. Gamma endpoints: Show minimum and maximum values beneath the track.
  Evidence: The gamma slider shows 2.2 without range labels.

- [ ] `sports_settings-12` · P2 · proposal. FPS endpoints: Label its supported range and any unlimited endpoint.
  Evidence: The Max FPS track has no endpoint labels.

- [ ] `sports_settings-13` · P2 · proposal. Slider fill: Use distinct filled and remaining segments.
  Evidence: Each mint track looks fully filled regardless of thumb position.

- [ ] `sports_settings-14` · P2 · proposal. Selected thumb: Increase thumb size or give it a light outline.
  Evidence: The dark thumb is small against the mint selection row.

- [ ] `sports_settings-15` · P2 · proposal. Selection frame: Add a consistent gutter between the row edge and the help separator.
  Evidence: The selected row touches the divider with little trailing space.

- [ ] `sports_settings-16` · P2 · proposal. Help title: Increase heading prominence and separate it from the body.
  Evidence: Max FPS is smaller than the explanatory body text.

- [ ] `sports_settings-17` · P2 · proposal. Help line length: Constrain it to about 45 to 55 characters per line.
  Evidence: The explanation spans a wide, two-line measure.

- [ ] `sports_settings-18` · P2 · proposal. Default value: Style the default as a labeled value pair with the FPS unit.
  Evidence: Default value: 120 is visually similar to the body copy.

- [ ] `sports_settings-19` · P2 · proposal. Backdrop readability: Increase the opacity of the left backing panel for consistent text contrast.
  Evidence: The fence and figure remain visible behind all setting labels.

- [ ] `sports_settings-20` · P3 · proposal. Divider accent: Use a uniform pane divider or label the scroll indicator if it represents position.
  Evidence: The mint vertical segment resembles a scrollbar without a label.

- [ ] `sports_settings-21` · P2 · proposal. Category context: Add Video settings as the local heading.
  Evidence: GRAPHICS appears under the active VIDEO tab but no page title is shown.

- [ ] `sports_settings-22` · P2 · proposal. Reset scope: Name the actual scope in the footer action.
  Evidence: Reset to default does not identify whether it affects the tab or all settings.

- [ ] `sports_settings-23` · P2 · proposal. Ready status: Use a clear Saved or No pending changes status if supported by the state model.
  Evidence: Ready is indistinguishable from generic footer text.

- [ ] `sports_settings-24` · P3 · proposal. Footer grouping: Separate status from action groups and use consistent keycap spacing.
  Evidence: Ready, Y Reset and B Back are crowded at the right edge.

- [ ] `sports_settings-25` · P2 · proposal. Graphics quality relationship: Clarify whether texture is an override of the overall preset.
  Evidence: Graphics quality and Texture quality both show Ultra with no hierarchy.

- [ ] `sports_settings-26` · P2 · proposal. Settings typography parity: Match target font metrics and weight while preserving the existing row positions.
  Evidence: The current settings labels are lighter and more widely spaced than the target's compact sans-serif labels.

- [ ] `sports_settings-27` · P3 · proposal. Selected-row treatment parity: Restore the target's distinct label and value backgrounds within the selected row.
  Evidence: The current Max FPS row is uniformly mint while the target shades its label half darker.

- [ ] `sports_settings-28` · P3 · proposal. Selection accent parity: Match the target stripe treatment without changing indicator dimensions.
  Evidence: The current Video underline and tall mint indicator are solid; the target uses a diagonal stripe detail.

### stepper_showcase

[Baseline](screenshot-baselines/screens/stepper_showcase_720p.png) · [Visual review](design-audit.html#screen=stepper_showcase)

- [ ] `stepper_showcase-01` · P2 · proposal. Left steppers: Place each arrow-value-arrow group in a subtle outlined region.
  Evidence: Thin arrows float around values without visible grouping.

- [ ] `stepper_showcase-02` · P2 · proposal. Resolution text: Use 2560 × 1440.
  Evidence: 2560x1440 uses a compact lowercase x.

- [ ] `stepper_showcase-03` · P2 · proposal. Framerate value: Show 60 FPS.
  Evidence: 60 lacks a unit.

- [ ] `stepper_showcase-04` · P2 · proposal. Stepper arrows: Increase glyph size and contrast while retaining consistent spacing.
  Evidence: All arrow glyphs are small and muted.

- [ ] `stepper_showcase-05` · P2 · proposal. Choice count: Add a compact index/count in specimen metadata.
  Evidence: None of the five steppers reveals its number of options.

- [ ] `stepper_showcase-06` · P2 · proposal. Boundary behavior: Add a footer note stating the actual wrap or clamp behavior.
  Evidence: The initial values do not explain whether arrows wrap.

- [ ] `stepper_showcase-07` · P2 · proposal. Card-selector labels: Separate choices into clear segments with consistent spacing.
  Evidence: Healer Warrior Mage reads as one line of words.

- [ ] `stepper_showcase-08` · P2 · proposal. Selected class: Add a selected underline or compact marker.
  Evidence: Warrior is distinguished mainly by brighter text.

- [ ] `stepper_showcase-09` · P2 · proposal. Card text hierarchy: Bring the description closer to the title.
  Evidence: Warrior and the description are separated by a large empty gap.

- [ ] `stepper_showcase-10` · P2 · proposal. Card description: Increase it to 14 to 16 px actual size.
  Evidence: Heavy armor, melee attacks, high defense is small for the card size.

- [ ] `stepper_showcase-11` · P3 · proposal. Card artwork: Add a restrained class emblem or compact stat row.
  Evidence: A large solid coral card contains only two text blocks.

- [ ] `stepper_showcase-12` · P2 · proposal. Card association: Align its title with the active tab and keep the active accent consistent.
  Evidence: The Warrior card has no connector to the selector above.

- [ ] `stepper_showcase-13` · P3 · proposal. Card color meaning: Identify it as Warrior accent in the specimen notes.
  Evidence: The coral fill has no explanatory legend.

- [ ] `stepper_showcase-14` · P2 · proposal. Section comparison: Add a subtitle explaining that both cycle through a finite option list.
  Evidence: Stepper Variants and Card Selector do not state the shared underlying behavior.

- [ ] `stepper_showcase-15` · P3 · proposal. Panel whitespace: Use the lower area for option counts and interaction notes.
  Evidence: Both panels leave substantial empty space below their controls.

- [ ] `stepper_showcase-16` · P2 · proposal. Default state: Add a compact Current defaults summary or reset control.
  Evidence: Medium, English, Normal and 60 appear without default markers.

- [ ] `stepper_showcase-17` · P3 · proposal. Heading alignment: Use a consistent heading alignment across the two panes.
  Evidence: The left heading aligns left while Card Selector is centered.

- [ ] `stepper_showcase-18` · P2 · proposal. Keyboard guidance: Add a guide matched to the implemented arrow and focus behavior.
  Evidence: No visible hint explains how the active selector changes choices.

### styled_text_lab

[Baseline](screenshot-baselines/screens/styled_text_lab_720p.png) · [Visual review](design-audit.html#screen=styled_text_lab)

- [ ] `styled_text_lab-01` · P2 · proposal. Top specimen caption: Increase metadata to a readable 12 to 13 px actual size.
  Evidence: DGOne + DGOne@bold is tiny above the sample.

- [ ] `styled_text_lab-02` · P2 · proposal. Regular/bold sample: Inset the specimen text while preserving its measured available width.
  Evidence: Text begins flush against the colored block edge.

- [ ] `styled_text_lab-03` · P2 · proposal. Weight-only specimen: Add a run-by-run weight legend beneath the sample.
  Evidence: one colour two weights does not identify which words request bold.

- [ ] `styled_text_lab-04` · P2 · proposal. Colored bold specimen: Explicitly label this as a combined weight-and-color example.
  Evidence: BOLD changes both weight and color.

- [ ] `styled_text_lab-05` · P2 · proposal. Wrap specimen: Add a source-text caption marking the bold span.
  Evidence: The bold span crosses the line break without a visible run boundary key.

- [ ] `styled_text_lab-06` · P2 · proposal. Wrap width: Annotate the exact content width used for measurement.
  Evidence: The wrapping block has no width value shown.

- [ ] `styled_text_lab-07` · P2 · proposal. Wrap line spacing: Add a separate comparison with increased line height while preserving the current baseline specimen.
  Evidence: Pixel-text lines are tightly packed.

- [ ] `styled_text_lab-08` · P2 · proposal. Hard-break caption: Label it Explicit newline and show the escaped source in metadata.
  Evidence: The caption exposes a literal backslash-n token without explanation.

- [ ] `styled_text_lab-09` · P2 · proposal. Hard-break sample: Add line boxes or baseline guides to make the hard-break behavior inspectable.
  Evidence: Heading and body text have very little vertical separation.

- [ ] `styled_text_lab-10` · P2 · proposal. Second heading: Use a small external run label to distinguish separate weighted spans.
  Evidence: Another heading continues close to the preceding body line.

- [ ] `styled_text_lab-11` · P2 · proposal. Fallback caption: Use a readable two-line caption with the expected regular fallback emphasized.
  Evidence: The no-@bold explanation is tiny and stretches across the block.

- [ ] `styled_text_lab-12` · P2 · proposal. Fallback comparison: Add a same-color comparison so the weight fallback can be assessed directly.
  Evidence: asks for regular and asks for bold also change color.

- [ ] `styled_text_lab-13` · P2 · proposal. Font identity: Show the actual registered family in the caption.
  Evidence: The fallback sample names default face without a readable font-family label.

- [ ] `styled_text_lab-14` · P3 · proposal. Sample numbering: Number them so screenshots can be compared with test definitions.
  Evidence: Five test cases have captions but no stable identifiers.

- [ ] `styled_text_lab-15` · P2 · proposal. Expected results: Add one short expected-result line per case.
  Evidence: No case states its expected visual outcome beside the specimen.

- [ ] `styled_text_lab-16` · P3 · proposal. Page balance: Use the right region for source spans and resolved weights.
  Evidence: All samples occupy the left side, leaving a large blank right region.

- [ ] `styled_text_lab-17` · P2 · proposal. Glyph metric comparison: Add optional baseline and advance-width guides outside the existing text.
  Evidence: Different weights are visible but baseline and advance differences are not annotated.

- [ ] `styled_text_lab-18` · P3 · proposal. Title hierarchy: Use a clearer title size and neutral metadata typography.
  Evidence: Styled Text Lab and sample captions are similarly letter-spaced.

### sync_scroll_lab

[Baseline](screenshot-baselines/screens/sync_scroll_lab_720p.png) · [Visual review](design-audit.html#screen=sync_scroll_lab)

- [ ] `sync_scroll_lab-01` · P2 · proposal. Group-one subtitle: Increase explanatory text to 12 to 13 px actual size.
  Evidence: The explanation of the short pane stopping is tiny.

- [ ] `sync_scroll_lab-02` · P2 · proposal. Document text: Increase specimen text size while retaining the row-height relationship.
  Evidence: Line numbers and code are very small in all three panes.

- [ ] `sync_scroll_lab-03` · P2 · proposal. Before/after content: Use a small actual code difference so before and after are meaningful labels.
  Evidence: Both sides visibly show value = compute(x) on highlighted rows.

- [ ] `sync_scroll_lab-04` · P2 · proposal. Change colors: Add a legend explaining the color mapping or use conventional removed/added mapping.
  Evidence: Before is green and after is red without a key.

- [ ] `sync_scroll_lab-05` · P2 · proposal. Short-pane role: Label it Short content, 24 rows to distinguish content length from viewport height.
  Evidence: Short has fewer rows but the same visible viewport height.

- [ ] `sync_scroll_lab-06` · P2 · proposal. Y status: Use three labeled offset cells with px units.
  Evidence: y: before 0 after 0 short 0 is tiny and compressed.

- [ ] `sync_scroll_lab-07` · P2 · proposal. Sync state: Use readable badges such as Matching offsets and Short pane following.
  Evidence: y-in-sync and short-following are terse implementation-like phrases.

- [ ] `sync_scroll_lab-08` · P2 · proposal. Clamped state: Add a preset that scrolls the long panes beyond the short pane maximum.
  Evidence: The initial frame does not show the short pane at its endpoint.

- [ ] `sync_scroll_lab-09` · P2 · proposal. Group-two explanation: Use a clear heading and one readable instruction line.
  Evidence: Header tracks the table's x is a long tiny line.

- [ ] `sync_scroll_lab-10` · P2 · proposal. Header cells: Increase text size and use consistent inner padding.
  Evidence: col 1 through col 8 are tiny and flush to their cell starts.

- [ ] `sync_scroll_lab-11` · P2 · proposal. Table cells: Add faint column guides and readable row/column identifiers.
  Evidence: r1c1-style labels are hard to scan across wide empty cells.

- [ ] `sync_scroll_lab-12` · P2 · proposal. Header scrollbar: Label it as a synchronized independent scrollbar so the duplicate bars are intentional.
  Evidence: The header shows its own horizontal scrollbar above the table.

- [ ] `sync_scroll_lab-13` · P2 · proposal. X status: Use the same offset-cell treatment as the Y group.
  Evidence: x: header 0 table 0 is difficult to read below the table.

- [ ] `sync_scroll_lab-14` · P2 · proposal. Axis distinction: Use Vertical group and Horizontal header/table group headings.
  Evidence: Group 1 and Group 2 are named numerically rather than by tested axis.

- [ ] `sync_scroll_lab-15` · P2 · proposal. Reset control: Add Reset all offsets outside the scroll regions.
  Evidence: No visible control returns both groups to a known state.

- [ ] `sync_scroll_lab-16` · P3 · proposal. Right-side space: Use the remaining area for the synchronization rules and expected clamping behavior.
  Evidence: The fixture ends around three-quarters of the screen width.

- [ ] `sync_scroll_lab-17` · P2 · proposal. Viewport metrics: Display viewport and content sizes in a compact spec table.
  Evidence: The thumbnail-like panes have no viewport dimensions shown.

- [ ] `sync_scroll_lab-18` · P2 · proposal. Driver instruction: Add a concrete before-pane and table-pane test sequence.
  Evidence: The page does not say which pane the reviewer should scroll first.

### system_profile_lab

[Baseline](screenshot-baselines/screens/system_profile_lab_720p.png) · [Visual review](design-audit.html#screen=system_profile_lab)

- [ ] `system_profile_lab-01` · P2 · proposal. Empty FPS metric: Use an em dash or No data until samples exist.
  Evidence: 0.0 FPS is shown before any frames are recorded.

- [ ] `system_profile_lab-02` · P2 · proposal. Empty frame timing: Render an explicit unmeasured state consistently across timing values.
  Evidence: Avg 0.00 ms/frame reads as a measured zero.

- [ ] `system_profile_lab-03` · P2 · proposal. Percentile labels: Use separate labeled metric blocks with ms units on each.
  Evidence: p50, p95 and p99 are packed into one line.

- [ ] `system_profile_lab-04` · P2 · proposal. Sample window: Give sample count and collection window their own metadata label.
  Evidence: Last 0 frames is appended to the metric sentence.

- [ ] `system_profile_lab-05` · P2 · proposal. Recording state: Place a status badge beside the recording control.
  Evidence: Recording stopped is small and visually detached from Start recording.

- [ ] `system_profile_lab-06` · P2 · proposal. Primary action: Make recording the primary action in the stopped state.
  Evidence: Start recording has the same emphasis as Reset and Sort recent.

- [ ] `system_profile_lab-07` · P2 · proposal. Pause view control: Explain view pause independently of recording or disable it until there is a view to pause.
  Evidence: Pause view appears while no samples exist.

- [ ] `system_profile_lab-08` · P2 · proposal. Sort action: Show Sort: recent average, descending with a matching header marker.
  Evidence: Sort recent does not reveal sorting direction or selected column.

- [ ] `system_profile_lab-09` · P2 · proposal. Chart empty state: Center a helpful empty state with Start recording to collect frame timings.
  Evidence: No samples is placed near the upper-left of a large empty plot.

- [ ] `system_profile_lab-10` · P2 · proposal. Chart identity: Add Frame time history and labeled ms/time axes.
  Evidence: The large plot has no title or axes in its empty state.

- [ ] `system_profile_lab-11` · P2 · proposal. Table empty state: Explain that system timings will appear after recording starts.
  Evidence: No system samples yet repeats the chart's empty message.

- [ ] `system_profile_lab-12` · P2 · proposal. CPU unavailable: Add a concise platform-support note or diagnostic reason when known.
  Evidence: Process CPU: unavailable provides no local explanation.

- [ ] `system_profile_lab-13` · P2 · proposal. Memory unavailable: Use separate CPU and memory metric cards with clear unavailable values.
  Evidence: Resident memory: unavailable is buried in the same sentence.

- [ ] `system_profile_lab-14` · P2 · proposal. Add load action: Label it Add synthetic workload and show its enabled state.
  Evidence: Add load gives no indication of the synthetic load type.

- [ ] `system_profile_lab-15` · P2 · proposal. Hide panel action: Rename it Hide profiler panel.
  Evidence: Hide panel does not identify what disappears.

- [ ] `system_profile_lab-16` · P2 · proposal. Table units: Use one heading format with ms units consistently.
  Evidence: Recent avg and Overall avg use parentheses while Last frame ms does not.

- [ ] `system_profile_lab-17` · P2 · proposal. System/phase hierarchy: Clarify the expected hierarchy with a sample or two-line heading.
  Evidence: System / phase combines two concepts in a single column heading.

- [ ] `system_profile_lab-18` · P3 · proposal. Toolbar layout: Group demo controls separately with a small Demo workload caption.
  Evidence: Add load and Hide panel occupy a separate top row from the profiler controls.

- [ ] `system_profile_lab-19` · P2 · proposal. Reset scope: Rename it Clear samples if that is its actual effect.
  Evidence: Reset is ambiguous between samples, view state and workload.

- [ ] `system_profile_lab-20` · P3 · proposal. Unused lower area: Use a compact bordered table shell sized to the available data area.
  Evidence: The table empty state leaves a broad black field.

### tab_container

[Baseline](screenshot-baselines/screens/tab_container_720p.png) · [Visual review](design-audit.html#screen=tab_container)

- [ ] `tab_container-01` · P2 · proposal. Page identity: Add Tab container demo above the tab strip.
  Evidence: The panel begins directly with tabs and has no demo title.

- [ ] `tab_container-02` · P2 · proposal. Profile heading: Use Profile details for the displayed read-only state.
  Evidence: Profile Settings suggests editable controls but shows only text.

- [ ] `tab_container-03` · P2 · proposal. Username row: Separate label and value into aligned columns.
  Evidence: Username and player_one share the same weight in one string.

- [ ] `tab_container-04` · P2 · proposal. Email row: Use a clear value field with a copy affordance if appropriate to the demo.
  Evidence: The email address is presented as plain prose.

- [ ] `tab_container-05` · P2 · proposal. Subscription value: Use a small subscription badge to distinguish status from identity fields.
  Evidence: Premium is plain text among metadata.

- [ ] `tab_container-06` · P2 · proposal. Last login: Use a stable sample date/time label or identify it as fixture data.
  Evidence: Today at 10:30 AM lacks a time-zone or fixed baseline context.

- [ ] `tab_container-07` · P2 · proposal. Field spacing: Reduce row gaps and use a compact details grid.
  Evidence: Five short rows are spread across a tall empty content panel.

- [ ] `tab_container-08` · P2 · proposal. Content width: Constrain the details region or add a second metadata column.
  Evidence: All content is left-aligned inside a very wide blank white panel.

- [ ] `tab_container-09` · P2 · proposal. OK action: Use a clear primary action style if it is enabled.
  Evidence: OK is dark gray and visually resembles a disabled action.

- [ ] `tab_container-10` · P2 · proposal. Apply action: Show a No pending changes state or omit Apply from this read-only tab.
  Evidence: Apply appears active despite no visible editable fields.

- [ ] `tab_container-11` · P2 · proposal. Cancel action: Use Close for a read-only demonstration or explain the editable state.
  Evidence: Cancel has no context for what is canceled in the static details view.

- [ ] `tab_container-12` · P2 · proposal. Footer alignment: Align the footer group to the content right inset.
  Evidence: The button group ends before the content panel's right edge.

- [ ] `tab_container-13` · P2 · proposal. Tab semantics: Use labels matching the actual account/audio/display content after checking each pane.
  Evidence: Account and Settings are broad labels that do not describe their contents.

- [ ] `tab_container-14` · P3 · proposal. Tab edges: Simplify internal joins to create one coherent tab strip.
  Evidence: All three tabs have rounded top shapes, including internal joins.

- [ ] `tab_container-15` · P2 · proposal. Selected tab cue: Add stronger selected text weight while retaining the underline.
  Evidence: A thin blue underline is the only explicit active marker.

- [ ] `tab_container-16` · P2 · proposal. Demo instructions: Add one concise instruction outside the specimen.
  Evidence: No text explains that changing tabs swaps the content pane.

- [ ] `tab_container-17` · P3 · proposal. Profile data provenance: Add a small Example profile caption.
  Evidence: player@example.com appears without a sample-data cue.

- [ ] `tab_container-18` · P2 · proposal. State coverage: Capture Account and Settings as named companion states for visual review.
  Evidence: Only Profile content is visible in the baseline.

### tabbing

[Baseline](screenshot-baselines/screens/tabbing_720p.png) · [Visual review](design-audit.html#screen=tabbing)

- [ ] `tabbing-01` · P2 · proposal. Title wording: Use Keyboard navigation.
  Evidence: KEYBOARD NAV abbreviates the feature name.

- [ ] `tabbing-02` · P2 · proposal. Instruction text: Increase actual instruction size to 13 px.
  Evidence: Use TAB to navigate, ENTER to click is small.

- [ ] `tabbing-03` · P2 · proposal. Activation language: Use Enter to activate.
  Evidence: ENTER to click uses pointer language for a keyboard action.

- [ ] `tabbing-04` · P2 · proposal. Reverse navigation: Add Shift+Tab for reverse navigation if supported.
  Evidence: Only forward Tab navigation is explained.

- [ ] `tabbing-05` · P2 · proposal. Initial focus framing: Add a caption explaining that focus appears after keyboard navigation begins.
  Evidence: No focused option is shown in this initial screenshot.

- [ ] `tabbing-06` · P2 · proposal. Focus evidence: Add a companion baseline with one keyboard-focused option.
  Evidence: All four button borders look alike in this state.

- [ ] `tabbing-07` · P2 · proposal. Option identities: Add a short demo note that each activation increments the shared counter.
  Evidence: OPTION A through OPTION D name no concrete action.

- [ ] `tabbing-08` · P2 · proposal. Counter label: Rename it Total activations.
  Evidence: Total Clicks counts keyboard activations under a mouse-specific name.

- [ ] `tabbing-09` · P2 · proposal. Counter feedback: Add Last activated: none beside the count.
  Evidence: The counter does not identify which option was activated.

- [ ] `tabbing-10` · P2 · proposal. Tab order: Add small 1 to 4 order badges outside the interactive buttons.
  Evidence: The vertical order is visible but not explicitly numbered as focus order.

- [ ] `tabbing-11` · P2 · proposal. Color roles: Add a note that colors test focus visibility across different backgrounds.
  Evidence: Magenta, cyan, lime and amber are unexplained.

- [ ] `tabbing-12` · P3 · proposal. Focus-ring specification: Show a noninteractive ring swatch with thickness and offset in a small legend.
  Evidence: The screenshot does not document the configured yellow focus-ring appearance.

- [ ] `tabbing-13` · P2 · proposal. Reset state: Add Reset count outside the demonstrated four-option order or label its tab-order role.
  Evidence: No reset action is visible for the activation counter.

- [ ] `tabbing-14` · P3 · proposal. Panel border: Simplify the decorative frame so the keyboard ring is easier to distinguish in active captures.
  Evidence: The purple outer frame has several close parallel outlines.

- [ ] `tabbing-15` · P2 · proposal. Expected outcome: Add an expected sequence line A, B, C, D and back according to the actual traversal rules.
  Evidence: The page has no explicit success statement for traversing all options.

### text

[Baseline](screenshot-baselines/screens/text_720p.png) · [Visual review](design-audit.html#screen=text)

- [ ] `text-01` · P2 · proposal. Font families: Label each specimen's font family.
  Evidence: Serif headings and handwritten body samples appear without font names.

- [ ] `text-02` · P2 · proposal. Display specimen: Show its point or pixel size beside the specimen.
  Evidence: Display Text has no size annotation.

- [ ] `text-03` · P2 · proposal. Headline specimen: Add a small metadata row for size and weight.
  Evidence: Headline Style lacks its size and weight.

- [ ] `text-04` · P2 · proposal. Subheading specimen: Label the intended semantic role and size.
  Evidence: Subheading Text is only differentiated visually.

- [ ] `text-05` · P2 · proposal. Body sample copy: Use a neutral specimen containing uppercase, lowercase, numbers and punctuation.
  Evidence: The sentence claims excellent legibility instead of exposing useful glyphs.

- [ ] `text-06` · P2 · proposal. Body line measure: Show its line width and line-height values.
  Evidence: The wrapping sample has no content-width annotation.

- [ ] `text-07` · P2 · proposal. Caption specimen: Place readable explanatory metadata outside the small-text specimen.
  Evidence: Caption & metadata is very small and also serves as its own description.

- [ ] `text-08` · P2 · proposal. Tracking samples: Repeat the same phrase in all three samples for direct comparison.
  Evidence: TIGHT TRACKING, WIDE TRACKING and ULTRA SPACED use different words.

- [ ] `text-09` · P2 · proposal. Tracking values: Display exact spacing values in the metadata column.
  Evidence: The three tracking labels provide no numeric letter spacing.

- [ ] `text-10` · P2 · proposal. Gold color sample: Add its token and hex value alongside the card.
  Evidence: Primary Gold Text has no color-token value.

- [ ] `text-11` · P2 · proposal. Cream color sample: Add foreground/background swatches and a measured contrast value in external metadata.
  Evidence: Light Cream Text has no contrast measurement context.

- [ ] `text-12` · P2 · proposal. Silver color sample: Show their color values side by side to explain the subtle difference.
  Evidence: Silver Accent Text resembles the nearby muted treatment.

- [ ] `text-13` · P2 · proposal. Muted color sample: Add a short secondary-metadata context sample outside the color specimen.
  Evidence: Muted Secondary Text is presented without a usage example.

- [ ] `text-14` · P2 · proposal. Accent color sample: Label the token and intended accent role.
  Evidence: Accent Color Highlight uses a reddish color without naming the token.

- [ ] `text-15` · P3 · proposal. Color card labels: Use identical sample text and move color labels outside the cards.
  Evidence: All five cards mix the color name with the sample sentence.

- [ ] `text-16` · P2 · proposal. Hierarchy comparison: Add optional faint baseline guides for visual comparison.
  Evidence: Size examples are stacked without baseline or cap-height guides.

- [ ] `text-17` · P3 · proposal. Section heading hierarchy: Make the specimen grouping explicit with separate section spacing.
  Evidence: Letter Spacing uses the same treatment as Text Size Hierarchy but sits within its column.

- [ ] `text-18` · P2 · proposal. Footer sentence: Replace it with the active font families and rendering scale.
  Evidence: Elegant typography creates visual hierarchy is a general claim.

- [ ] `text-19` · P3 · proposal. Gold underline: Reduce its prominence so sample rules and guides have a distinct role.
  Evidence: The heading underline is unlabeled decoration on a specimen-heavy page.

- [ ] `text-20` · P2 · proposal. Baseline rendering context: Add 1280 × 720 and the effective UI scale in a metadata footer.
  Evidence: The page does not state resolution or scale for the type samples.

### text_input

[Baseline](screenshot-baselines/screens/text_input_720p.png) · [Visual review](design-audit.html#screen=text_input)

- [ ] `text_input-01` · P2 · defect. Field labels: Increase actual label size to at least 12 to 14 px.
  Evidence: Username, Email, Password and other labels are extremely small.

- [ ] `text_input-02` · P2 · defect. Footer shortcuts: Increase shortcut text and spell out the associated actions.
  Evidence: Tab / Ctrl+A/C/V/X is barely legible.

- [ ] `text_input-03` · P2 · proposal. Page subtitle: Increase its size and contrast below Edit Profile.
  Evidence: Update your account information is tiny and faint.

- [ ] `text_input-04` · P2 · proposal. Profile context: Group those controls under a separate Input states demo section.
  Evidence: Search, Readonly Field and Disabled Field do not belong to a coherent edit-profile form.

- [ ] `text_input-05` · P2 · proposal. Bio field: Use a multiline specimen or label it Short bio.
  Evidence: Bio is a single short line despite inviting personal description.

- [ ] `text_input-06` · P2 · proposal. Email placeholder: Use Enter email address and show example formatting as helper text.
  Evidence: user@example.com can look like a prefilled address.

- [ ] `text_input-07` · P2 · proposal. Username guidance: Add concise requirements beneath the label if the form validates them.
  Evidence: Enter username gives no visible format or length guidance.

- [ ] `text_input-08` · P2 · proposal. Password context: Use New password and clarify whether leaving it blank preserves the current password.
  Evidence: A password field is mixed into profile editing without naming the operation.

- [ ] `text_input-09` · P2 · proposal. Show password control: Increase label size and use a clear show-password icon with sufficient inset.
  Evidence: Show is small in a narrow gray segment.

- [ ] `text_input-10` · P2 · proposal. Password segment border: Unify the shared outer border and use a clean internal divider.
  Evidence: The Show segment has a different edge treatment from the input.

- [ ] `text_input-11` · P2 · proposal. Search affordance: Add a recognizable search icon and identify what is searched.
  Evidence: Type to search... has no leading search icon.

- [ ] `text_input-12` · P2 · proposal. Read-only specimen: Use a realistic fixed value and a Read-only badge.
  Evidence: readonly text here is a placeholder-like sample.

- [ ] `text_input-13` · P2 · proposal. Disabled specimen: Explain why this sample is disabled outside the specimen.
  Evidence: The dim text is deliberately disabled but has no reason caption.

- [ ] `text_input-14` · P2 · proposal. Input boundaries: Use consistent border joins and document any intentional corner-radius variation.
  Evidence: Several field edges have broken-looking corner joins.

- [ ] `text_input-15` · P2 · proposal. Save label size: Increase action text size while preserving the primary color.
  Evidence: Save is tiny inside a strong yellow button.

- [ ] `text_input-16` · P2 · proposal. Cancel label size: Match the Save label size and vertical alignment.
  Evidence: Cancel is tiny inside its outlined button.

- [ ] `text_input-17` · P2 · proposal. Footer order: Separate input help from the action group with consistent footer spacing.
  Evidence: The shortcut hint and action buttons share a sparse line without clear grouping.

- [ ] `text_input-18` · P2 · proposal. Form status: Add a small Demo form or No changes status near the actions.
  Evidence: No visible status explains whether this is unsaved sample data.

- [ ] `text_input-19` · P3 · proposal. Title hierarchy: Increase title size so it clearly leads the form.
  Evidence: Edit Profile is only slightly larger than input text.

- [ ] `text_input-20` · P2 · proposal. Optional fields: Label optional fields or add a concise required-fields note.
  Evidence: Username, Email, Password and Bio have no required/optional distinction.

- [ ] `text_input-21` · P3 · proposal. Panel balance: Widen the panel modestly to accommodate readable labels and helper text.
  Evidence: The tall narrow panel leaves broad black margins.

- [ ] `text_input-22` · P2 · proposal. Initial blank form: Populate a clearly labeled sample profile or rename the screen Create profile.
  Evidence: Edit Profile contains placeholders rather than visible existing account data.

### text_input_lab

[Baseline](screenshot-baselines/screens/text_input_lab_720p.png) · [Visual review](design-audit.html#screen=text_input_lab)

- [ ] `text_input_lab-01` · P2 · proposal. Primary field label: Label it Selection and caret specimen.
  Evidence: The first input has no local label.

- [ ] `text_input_lab-02` · P2 · proposal. State group: Place them in a compact Inspector panel with aligned value columns.
  Evidence: text, cursor, selection, focused and scroll_x sit directly on the black page.

- [ ] `text_input_lab-03` · P2 · proposal. Cursor index: Label it Caret character index and state the indexing convention.
  Evidence: cursor: 11 does not explain index units.

- [ ] `text_input_lab-04` · P2 · proposal. Selection range: Show Start 0, End 11 and note whether end is exclusive.
  Evidence: selection: 0..11 does not explain endpoint semantics.

- [ ] `text_input_lab-05` · P2 · proposal. Selection length: Add Selected characters: 11.
  Evidence: The full phrase is selected but no count is shown.

- [ ] `text_input_lab-06` · P2 · proposal. Horizontal scroll: Show Horizontal scroll: 0.0 px.
  Evidence: scroll_x: 0.0 lacks units.

- [ ] `text_input_lab-07` · P2 · proposal. Focus readout: Use a distinct Focused badge next to the inspected field name.
  Evidence: focused: yes is embedded among numeric data.

- [ ] `text_input_lab-08` · P2 · proposal. Inspector association: Label the inspector's target or show a separate dump for the overflow field.
  Evidence: Two inputs are visible but only one state dump appears.

- [ ] `text_input_lab-09` · P2 · proposal. Overflow source text: Show the full source string in an external read-only caption.
  Evidence: The lower text is clipped so its full source cannot be read.

- [ ] `text_input_lab-10` · P2 · proposal. Overflow dimensions: Label its exact viewport width and text width.
  Evidence: The lower field has no width annotation.

- [ ] `text_input_lab-11` · P2 · proposal. Expected clipping: Add external edge ticks at the field's clip limits.
  Evidence: The instruction says text must stay inside the box without marking that boundary.

- [ ] `text_input_lab-12` · P2 · proposal. Initial selection: Add a caption Initial state: all text selected.
  Evidence: The first field starts fully selected with no explanation.

- [ ] `text_input_lab-13` · P2 · proposal. Test procedure: Add a short keyboard procedure beneath the inspector.
  Evidence: The page gives no sequence for observing caret, selection and scroll changes.

- [ ] `text_input_lab-14` · P2 · proposal. Reset specimen: Add Reset fixture outside the tested fields.
  Evidence: No control restores hello world and the initial selection.

- [ ] `text_input_lab-15` · P3 · proposal. Empty right region: Use a second column for expected outcomes and event history.
  Evidence: Most of the screen is unused.

- [ ] `text_input_lab-16` · P2 · proposal. Action evidence: Add a small last-action label so captures can be tied to a reproducible input step.
  Evidence: The state dump shows only current values.

### text_input_sizes

[Baseline](screenshot-baselines/screens/text_input_sizes_720p.png) · [Visual review](design-audit.html#screen=text_input_sizes)

- [ ] `text_input_sizes-01` · P2 · defect. Height labels: Increase external dimension labels to readable 12 px text.
  Evidence: 24px through 100px are extremely small.

- [ ] `text_input_sizes-02` · P2 · proposal. Subtitle: Increase explanatory text to 13 px actual size.
  Evidence: Cursor and selection scale with field height is very small.

- [ ] `text_input_sizes-03` · P2 · proposal. Initial content: Prefill matching sample text in a companion state to expose selection height.
  Evidence: Every field is empty and displays a placeholder.

- [ ] `text_input_sizes-04` · P2 · proposal. Caret comparison: Add dedicated focus captures for each height rather than implying all carets can appear together.
  Evidence: Only the first field shows a caret in this capture.

- [ ] `text_input_sizes-05` · P2 · proposal. Selection comparison: Add a companion state with a consistent selected word at each target height.
  Evidence: No selected range is visible.

- [ ] `text_input_sizes-06` · P2 · proposal. Dimension notation: Use a consistent 24 px notation.
  Evidence: Labels use 24px with no space.

- [ ] `text_input_sizes-07` · P2 · proposal. Column identification: Add Height above the label column.
  Evidence: The height labels have no heading.

- [ ] `text_input_sizes-08` · P2 · proposal. Width metadata: Show a shared field-width caption above the stack.
  Evidence: All fields appear equal width but the width is undocumented.

- [ ] `text_input_sizes-09` · P2 · proposal. Font metadata: Document the font size beside the common width caption.
  Evidence: Placeholder text appears similar in size despite changing field heights.

- [ ] `text_input_sizes-10` · P2 · proposal. Padding growth: Annotate the padding rule so it can be checked independently of field height.
  Evidence: Larger fields show progressively larger left insets.

- [ ] `text_input_sizes-11` · P2 · proposal. Vertical centering: Add optional midline guides outside the active input rendering.
  Evidence: The specimen lacks guides for checking text centering.

- [ ] `text_input_sizes-12` · P2 · proposal. Height ruler: Add a thin external height ruler beside each specimen.
  Evidence: The difference between 24 and 32 px is hard to assess precisely.

- [ ] `text_input_sizes-13` · P3 · proposal. Row spacing: Use equal labeled specimen gutters while preserving exact field heights.
  Evidence: The gaps between fields are tight compared with the large last field.

- [ ] `text_input_sizes-14` · P2 · proposal. Small-input context: Label them compact-size test cases rather than default form guidance.
  Evidence: The 24 and 32 px specimens could look like recommended product controls.

- [ ] `text_input_sizes-15` · P2 · proposal. Reset content: Add a shared Reset sample text control outside the stack.
  Evidence: No reset action returns all fields to comparable text.

- [ ] `text_input_sizes-16` · P3 · proposal. Bottom space: Use it for caret, selection and padding measurements.
  Evidence: A large blank area remains below the 100 px field.

### text_overflow

[Baseline](screenshot-baselines/screens/text_overflow_720p.png) · [Visual review](design-audit.html#screen=text_overflow)

- [ ] `text_overflow-01` · P2 · proposal. Debug instruction: Add a zoomed marker key distinguishing debug corners from specimen borders.
  Evidence: The note says red corners appear, while entire sample borders are red.

- [ ] `text_overflow-02` · P2 · proposal. Good/problem colors: Add explicit Pass specimen and Overflow specimen badges.
  Evidence: Green and red headings carry the main classification.

- [ ] `text_overflow-03` · P2 · proposal. Height specimen: Annotate requested font size, measured text height and available height to explain the case.
  Evidence: Height is too small! appears to fit within its red frame.

- [ ] `text_overflow-04` · P2 · proposal. Tiny box annotation: Use a clean callout line and 44 × 44 px label.
  Evidence: Small 44x44 uses a textual arrow and compact notation.

- [ ] `text_overflow-05` · P2 · proposal. First overflow text: Show the source string in external metadata.
  Evidence: This is way too ... is truncated without the full source beside it.

- [ ] `text_overflow-06` · P2 · proposal. Narrow overflow case: Label its width and height next to the specimen.
  Evidence: Long text i... has no dimension annotation.

- [ ] `text_overflow-07` · P2 · proposal. Ellipsis comparison: Add matched-width before/after specimens so ellipsis is the isolated change.
  Evidence: The solution samples are wider than the nearby failing examples.

- [ ] `text_overflow-08` · P2 · proposal. Ellipsis label: Rename it Ellipsis behavior and state when wrapping is preferable.
  Evidence: Solution: Ellipsis Truncation can imply every overflow should truncate.

- [ ] `text_overflow-09` · P2 · proposal. OK specimen: Annotate its width, height and text size.
  Evidence: The small green OK box has no dimensions shown.

- [ ] `text_overflow-10` · P2 · proposal. Large fit specimen: Add text bounds and container bounds in a spec caption.
  Evidence: This text fits perfectly shows no measurable available space.

- [ ] `text_overflow-11` · P2 · proposal. ABC sequence: Label each box's dimensions beneath it.
  Evidence: Five shrinking ABC boxes have no individual size labels.

- [ ] `text_overflow-12` · P2 · proposal. ABC font changes: Display the font size per sample so two variables are explicit.
  Evidence: The ABC font shrinks across the sequence as well as the boxes.

- [ ] `text_overflow-13` · P2 · proposal. Minimum-size note: Separate touch-target guidance from text-measurement behavior.
  Evidence: The note links touch target size to text overflow warnings without distinction.

- [ ] `text_overflow-14` · P2 · proposal. Debug mode state: Use a small ON badge next to a Debug overlay label.
  Evidence: Debug mode is ON is embedded in a long green sentence.

- [ ] `text_overflow-15` · P3 · proposal. Title scale: Reduce title footprint and spend space on readable metadata.
  Evidence: Text Overflow Debug Demo is large relative to tiny diagnostics.

- [ ] `text_overflow-16` · P2 · proposal. Specimen identifiers: Add small case IDs for screenshot-to-test mapping.
  Evidence: Individual examples have no stable IDs.

- [ ] `text_overflow-17` · P2 · proposal. Expected warning: Add Expected: overflow or Expected: fits under each case.
  Evidence: Red-bordered examples have no expected-warning text.

- [ ] `text_overflow-18` · P3 · proposal. Column alignment: Align the first comparison rows while preserving test dimensions.
  Evidence: The two columns begin at different specimen baselines.

- [ ] `text_overflow-19` · P2 · proposal. Clip boundary: Place debug annotations outside the measured container.
  Evidence: Truncated text and red borders are close enough to obscure corner markers.

- [ ] `text_overflow-20` · P2 · proposal. Rendering scale: Add the UI scale and capture resolution to the footer.
  Evidence: The page shows fixed-size claims without the effective scale.

### text_shadow

[Baseline](screenshot-baselines/screens/text_shadow_720p.png) · [Visual review](design-audit.html#screen=text_shadow)

- [ ] `text_shadow-01` · P2 · proposal. Page title shadow: Use a restrained title shadow and reserve extreme offsets for the labeled specimens.
  Evidence: Text Drop Shadow has a heavy offset shadow overlapping its letterforms.

- [ ] `text_shadow-02` · P2 · proposal. Subtitle placement: Align it beneath the title or make it a clearly separate page introduction.
  Evidence: The subtitle starts at the far left while the title is centered.

- [ ] `text_shadow-03` · P2 · proposal. No-shadow comparison: Repeat the same text and move variant labels outside the specimens.
  Evidence: NO SHADOW and WITH SHADOW use different phrases.

- [ ] `text_shadow-04` · P2 · proposal. Comparison offset: Add x/y offset and opacity metadata.
  Evidence: The WITH SHADOW specimen does not show its exact offset.

- [ ] `text_shadow-05` · P2 · proposal. Soft/hard comparison: Use identical specimen text with separate Soft and Hard captions.
  Evidence: SOFT and HARD change the words while comparing shadow presets.

- [ ] `text_shadow-06` · P2 · proposal. Soft-shadow metadata: Show the preset's blur, offset and alpha parameters.
  Evidence: The glow description omits blur and opacity values.

- [ ] `text_shadow-07` · P2 · proposal. Hard-shadow metadata: Add its exact offset and opacity for comparison with Soft.
  Evidence: HARD has no parameter label.

- [ ] `text_shadow-08` · P2 · proposal. Colored shadow: Display both foreground and shadow color tokens.
  Evidence: COLORED is cyan/red but no color values are shown.

- [ ] `text_shadow-09` · P2 · proposal. Combo specimen: Add stroke width and shadow offset in an external caption.
  Evidence: COMBO mixes stroke and shadow without separate measurements.

- [ ] `text_shadow-10` · P2 · defect. LIGHT caption placement: Place one caption directly beneath each LIGHT specimen.
  Evidence: flat look and more depth! both sit beneath the left half of the panel.

- [ ] `text_shadow-11` · P2 · defect. LIGHT bottom inset: Increase panel height or move captions up to leave 10 px bottom inset.
  Evidence: The LIGHT captions nearly touch the dark panel's bottom edge.

- [ ] `text_shadow-12` · P2 · proposal. Offset ladder heading: Label the ladder with the x/y offset convention.
  Evidence: Shadow Offset Examples does not specify direction.

- [ ] `text_shadow-13` · P2 · proposal. Offset labels: Increase label size and use 1 px notation.
  Evidence: 1px through 10px are small at the far right.

- [ ] `text_shadow-14` · P2 · proposal. Ladder spacing: Bring the values closer or add faint row guides.
  Evidence: Large blank horizontal gaps separate SHADOW from its numeric label.

- [ ] `text_shadow-15` · P2 · proposal. Ladder baseline: Add optional baseline ticks outside the text.
  Evidence: No guide shows the unshadowed glyph baseline.

- [ ] `text_shadow-16` · P2 · proposal. Ladder control: Add an external zero-offset swatch for comparison.
  Evidence: The ladder begins at 1 px without a 0 px reference.

- [ ] `text_shadow-17` · P3 · proposal. Description rhythm: Use consistent specimen-to-caption spacing.
  Evidence: Descriptions sit at varying distances from their examples.

- [ ] `text_shadow-18` · P2 · proposal. Bottom tip: Move it closer to the examples or use the region for parameter summaries.
  Evidence: The tip is far below the specimens in a nearly empty lower region.

- [ ] `text_shadow-19` · P2 · proposal. Font identity: Add the font family and size to the specimen metadata.
  Evidence: The bold pixel font is not named.

### text_stroke

[Baseline](screenshot-baselines/screens/text_stroke_720p.png) · [Visual review](design-audit.html#screen=text_stroke)

- [ ] `text_stroke-01` · P1 · defect. WHITE captions: Move the tip down or the specimen up so both captions are fully readable.
  Evidence: The invisible and visible! captions are covered by the bottom tip strip.

- [ ] `text_stroke-02` · P2 · proposal. White control label: Use Low contrast and Outlined for more precise captions.
  Evidence: invisible describes text that is faintly visible and frames the example as absolute.

- [ ] `text_stroke-03` · P2 · proposal. No-stroke comparison: Repeat the same specimen text and place variant labels outside it.
  Evidence: NO STROKE and WITH STROKE use different strings.

- [ ] `text_stroke-04` · P2 · proposal. Comparison width: Add an enlarged glyph detail to distinguish stroke expansion from a background fill.
  Evidence: The dark outline behind WITH STROKE resembles a rectangular backing.

- [ ] `text_stroke-05` · P2 · proposal. Bold specimen: Show font size alongside stroke width to explain relative thickness.
  Evidence: BOLD uses an 8 px stroke but lacks the font size.

- [ ] `text_stroke-06` · P2 · proposal. Contrast colors: Display foreground and outline color values in a separate caption.
  Evidence: CONTRAST is cyan/red without color token metadata.

- [ ] `text_stroke-07` · P2 · proposal. Glow wording: Label it Bright outline or add a separate blurred glow comparison.
  Evidence: GLOW uses a hard bright outline while the caption calls it a glow.

- [ ] `text_stroke-08` · P2 · proposal. Thickness ladder baseline: Add a zero-stroke reference above the ladder.
  Evidence: The ladder starts at 2 px with no zero-stroke control.

- [ ] `text_stroke-09` · P2 · proposal. Thickness labels: Increase label size and use spaced px notation.
  Evidence: 2px through 10px use small distant labels.

- [ ] `text_stroke-10` · P2 · proposal. Ladder row alignment: Align them within clear specimen rows.
  Evidence: The text and thickness descriptions are separated by large blank gaps.

- [ ] `text_stroke-11` · P2 · proposal. Ten-pixel outline: Label it Extreme thickness specimen and provide a close-up for assessment.
  Evidence: The 10 px sample has visible chunky artifacts that are part of the extreme case.

- [ ] `text_stroke-12` · P2 · proposal. Stroke algorithm context: Add a short metadata note identifying the stroke method under test.
  Evidence: The different outline edges have no rendering-method explanation.

- [ ] `text_stroke-13` · P2 · proposal. Font identity: Label BlackOpsOne and its sample size.
  Evidence: The pixel-like display font is unnamed.

- [ ] `text_stroke-14` · P2 · proposal. Title effect: Document its settings or use a neutral title to isolate the specimen effects.
  Evidence: The title also has a stroke but no specification.

- [ ] `text_stroke-15` · P3 · proposal. Subtitle size: Increase its readable size modestly.
  Evidence: The subtitle is small and subdued compared with the large effect samples.

- [ ] `text_stroke-16` · P2 · proposal. White panel metadata: Add background and foreground values outside the control panel.
  Evidence: The white-on-light panel does not state its background color.

- [ ] `text_stroke-17` · P2 · proposal. Caption alignment: Use a consistent left column and text measure for metadata.
  Evidence: Descriptions run across varying widths under the left specimens.

- [ ] `text_stroke-18` · P2 · proposal. Thickness advice: Express guidance as a stroke-to-font-size ratio or include the reference font size.
  Evidence: The tip gives absolute widths without relating them to font size.

- [ ] `text_stroke-19` · P3 · proposal. Case numbering: Add small IDs for comparison across captures.
  Evidence: The effect cases have names but no stable identifiers.

- [ ] `text_stroke-20` · P2 · proposal. Rendering scale: Add capture scale and effective stroke units in the footer.
  Evidence: Pixel-width labels do not show whether UI scaling is applied.

### themes

[Baseline](screenshot-baselines/screens/themes_720p.png) · [Visual review](design-audit.html#screen=themes)

- [ ] `themes-01` · P2 · proposal. Checkbox preview: For a more familiar checkbox presentation, place the check inside a visible square beside its label.
  Evidence: The checkbox preview uses a V check indicator inside a wide blue row.

- [ ] `themes-02` · P2 · proposal. Button labels: Expand them to Primary, Secondary, Accent and Disabled.
  Evidence: P, S, A and D are unexplained one-letter labels.

- [ ] `themes-03` · P2 · proposal. Selected theme marker: Use a checkmark or side marker with readable equal padding.
  Evidence: Ocean Navy is wrapped in greater-than and less-than text.

- [ ] `themes-04` · P2 · proposal. Active-theme banner: Style it as a status badge rather than a control.
  Evidence: Active: Ocean Navy resembles another gold action button.

- [ ] `themes-05` · P2 · proposal. Theme selection header: Use a plain section heading so it is not mistaken for an action.
  Evidence: Select Theme is styled as a blue button.

- [ ] `themes-06` · P2 · proposal. Slider preview: Add Slider and its current numeric value.
  Evidence: The wide green bar with blue thumb has no label or value.

- [ ] `themes-07` · P2 · proposal. Slider purpose: Add a specimen caption with thumb size and track size.
  Evidence: The oversized rectangular thumb does not explain its style configuration.

- [ ] `themes-08` · P2 · proposal. Toggle label: Align label left and switch right with a consistent setting-row structure.
  Evidence: Toggle is centered far from the right-side switch.

- [ ] `themes-09` · P2 · proposal. Toggle state: Add an explicit state value.
  Evidence: The gray switch has no visible On/Off text.

- [ ] `themes-10` · P2 · proposal. Shadow specimens: Label shadow type separately and add a same-color comparison for shadow strength.
  Evidence: Hard, Soft and Accent also change background color.

- [ ] `themes-11` · P2 · proposal. Hard card text: Add consistent inner padding to all three shadow cards.
  Evidence: Hard touches the left edge of its blue card.

- [ ] `themes-12` · P2 · proposal. Text swatches: Inset all text swatches equally.
  Evidence: Primary Text, Secondary Text and Surface Text touch their card edges.

- [ ] `themes-13` · P2 · proposal. Surface swatch: Add a neutral external border and token metadata without changing the specimen colors.
  Evidence: Surface Text sits on a nearly matching navy card.

- [ ] `themes-14` · P2 · proposal. Color values: Add a token table showing foreground and background hex values.
  Evidence: The preview contains many role colors with no resolved values.

- [ ] `themes-15` · P2 · proposal. Separator: Identify it as the separator specimen and show its token.
  Evidence: A green horizontal rule appears without a label.

- [ ] `themes-16` · P2 · proposal. Progress state: Place a readable progress label above the bar.
  Evidence: Loading: 72% is tiny inside the blue fill.

- [ ] `themes-17` · P2 · proposal. Progress semantics: Add an external legend for fill and track tokens rather than silently replacing the theme.
  Evidence: The remaining portion is green, which can look like completion too.

- [ ] `themes-18` · P2 · proposal. Preview title: Increase its hierarchy and identify the active theme beside it.
  Evidence: Component Preview is a small label in a full-width blue bar.

- [ ] `themes-19` · P3 · proposal. Left rail space: Use it for active-theme metadata or a compact palette summary.
  Evidence: The theme list leaves a large unused lower region.

- [ ] `themes-20` · P2 · proposal. Theme comparison: Add named companion captures for all five themes.
  Evidence: Only the selected theme is shown, making differences hard to review.

- [ ] `themes-21` · P2 · proposal. Disabled specimen: Add a Disabled state caption outside the intentionally dim control.
  Evidence: D is dim but has no external explanation.

- [ ] `themes-22` · P2 · proposal. Theme effect scope: Add a concise instruction above the preview.
  Evidence: The page does not explain that switching a theme recolors all examples.

### toasts

[Baseline](screenshot-baselines/screens/toasts_720p.png) · [Visual review](design-audit.html#screen=toasts)

- [ ] `toasts-01` · P2 · defect. Interactive section claim: Rename the section Simulated action feedback or implement an actual undo action in the toast.
  Evidence: INTERACTIVE TOASTS and With Undo Action promise an embedded action, but source only increments a counter and sends a toast.

- [ ] `toasts-02` · P2 · defect. Undo counter label: Rename it Notifications sent unless actual undo operations are introduced.
  Evidence: Undos: 0 labels launcher activations as completed undos.

- [ ] `toasts-03` · P2 · defect. Title inset: Add 16 px inner padding.
  Evidence: Toast Notifications starts flush against the title bar's left edge.

- [ ] `toasts-04` · P2 · defect. Quick duration label: Add at least 12 px horizontal padding.
  Evidence: Quick displays 1s nearly touches both sides of its button.

- [ ] `toasts-05` · P2 · defect. Long duration label: Increase button width or shorten the label to Long, 10 s.
  Evidence: Long displays 10s fills almost the entire button width.

- [ ] `toasts-06` · P2 · proposal. Success color: Add severity icons and text labels so the theme color is not the sole cue.
  Evidence: Success Toast is purple without an external severity key.

- [ ] `toasts-07` · P2 · proposal. Warning color: Use a warning icon in the launcher and toast specimen.
  Evidence: Warning Toast is magenta and close to other accents.

- [ ] `toasts-08` · P2 · proposal. Error launcher: Add a consistent error icon aligned with the other severity controls.
  Evidence: Error Toast uses red but no error symbol.

- [ ] `toasts-09` · P2 · proposal. Info launcher: Add a short example-message caption below the launcher.
  Evidence: Info Toast has no preview of the resulting message.

- [ ] `toasts-10` · P2 · proposal. Duration group title: Rename it Duration and stacking tests.
  Evidence: DURATION & SPAM uses informal stress-test language.

- [ ] `toasts-11` · P2 · proposal. Spam action: Use Show five toasts and describe stacking behavior below it.
  Evidence: Spam x5 (!) obscures the exact outcome.

- [ ] `toasts-12` · P2 · proposal. Custom color control: Add an external swatch value or color label.
  Evidence: Custom Color uses an orange fill without naming the configured color.

- [ ] `toasts-13` · P2 · proposal. Launcher sizing: Use an equal-width severity grid for easier comparison.
  Evidence: Simple toast buttons have uneven widths determined by their labels.

- [ ] `toasts-14` · P2 · proposal. Typography scale: Reduce tracking and use a consistent readable button size.
  Evidence: Large letter-spaced launcher labels crowd their backgrounds.

- [ ] `toasts-15` · P2 · proposal. Empty toast area: Add a dedicated labeled preview region or companion shown-toast captures.
  Evidence: No toast is visible anywhere in the initial capture.

- [ ] `toasts-16` · P2 · proposal. Toast placement: Show a small placement diagram in the unused right side of a section.
  Evidence: The launcher page does not state where notifications appear.

- [ ] `toasts-17` · P2 · proposal. Toast lifetime: Label the default duration used by the simple severity examples.
  Evidence: Only quick and long launchers show duration values.

- [ ] `toasts-18` · P2 · proposal. Clear action: Add Clear toasts outside the fixture previews.
  Evidence: No visible action dismisses the stacking test.

- [ ] `toasts-19` · P2 · proposal. Status reporting: Add a shared Last notification and Active count readout.
  Evidence: Only the undo simulation has a counter.

- [ ] `toasts-20` · P3 · proposal. Section space: Use that space for concise expected-result and duration metadata.
  Evidence: Wide cards leave substantial unused right space.

### toggle_switches

[Baseline](screenshot-baselines/screens/toggle_switches_720p.png) · [Visual review](design-audit.html#screen=toggle_switches)

- [ ] `toggle_switches-01` · P2 · proposal. Page title: Use Toggle and checkbox settings demo.
  Evidence: Settings hides that this is a component comparison.

- [ ] `toggle_switches-02` · P2 · proposal. General labels: Left-align labels with the Preferences rows.
  Evidence: Notifications, Sound Effects and Vibration are centered far from the left edge.

- [ ] `toggle_switches-03` · P2 · proposal. Preference label inset: Add 12 px inner padding.
  Evidence: Dark Mode, Auto-Save and Cloud Sync touch the row left edge.

- [ ] `toggle_switches-04` · P2 · proposal. Preference indicators: Align them in the same right-side control column as the switches.
  Evidence: The [x] and [ ] glyphs float near the row center.

- [ ] `toggle_switches-05` · P2 · proposal. Checkbox rendering: Use a visible checkbox shape with a centered checkmark and retain a caption for its style.
  Evidence: Bracketed text resembles terminal output rather than a circular checkbox specimen.

- [ ] `toggle_switches-06` · P2 · proposal. On-state labels: Add On text beside the switch.
  Evidence: Notifications and Vibration rely on magenta fill and thumb position.

- [ ] `toggle_switches-07` · P2 · proposal. Off-state label: Add Off text in the state column.
  Evidence: Sound Effects uses a gray track without an Off label.

- [ ] `toggle_switches-08` · P2 · proposal. Section naming: Add Pill switches and Checkbox variants as secondary captions.
  Evidence: General and Preferences do not identify the different control families.

- [ ] `toggle_switches-09` · P2 · proposal. Disabled switch: Keep the disabled specimen but label track and thumb states in external metadata.
  Evidence: Locked Setting ON shows a white thumb with an almost invisible track.

- [ ] `toggle_switches-10` · P2 · proposal. Disabled reason: Add a short sample explanation such as Managed by administrator.
  Evidence: Locked Setting gives no reason for the lock.

- [ ] `toggle_switches-11` · P2 · proposal. Unavailable checkbox: Add an external Not available in this demo note.
  Evidence: Unavailable Option OFF has no reason caption.

- [ ] `toggle_switches-12` · P2 · proposal. Disabled text size: Increase external captions while leaving disabled-state styling intact.
  Evidence: Disabled labels are small as well as dim.

- [ ] `toggle_switches-13` · P2 · proposal. State vocabulary: Use consistent On and Off state text across all specimens.
  Evidence: ON/OFF appears only in disabled labels while enabled rows have no words.

- [ ] `toggle_switches-14` · P2 · proposal. Row geometry: Label the shape variation explicitly or use a common row shell around different controls.
  Evidence: General rows are square strips while Preferences rows are rounded.

- [ ] `toggle_switches-15` · P2 · proposal. Section labels: Increase their readable size and use a consistent heading weight.
  Evidence: General, Preferences and Disabled are small and faint.

- [ ] `toggle_switches-16` · P2 · proposal. Control association: Use a structured label/value row with predictable spacing.
  Evidence: Large gaps separate centered labels from the right-side pill controls.

- [ ] `toggle_switches-17` · P3 · proposal. Separator inset: Align headings, labels and dividers to one inner grid.
  Evidence: Dividers align with the outer row edges but labels do not.

- [ ] `toggle_switches-18` · P2 · proposal. Initial state summary: Add a small state key outside the controls.
  Evidence: The mix of on and off states has no compact comparison legend.

- [ ] `toggle_switches-19` · P2 · proposal. Reset defaults: Add Reset demo defaults below the specimen groups.
  Evidence: No visible action restores the initial mixture of states.

- [ ] `toggle_switches-20` · P3 · proposal. Panel margins: Add a modest top margin while preserving space for the disabled examples.
  Evidence: The main card begins at the viewport top with little title breathing room.

### tooltip_lab

[Baseline](screenshot-baselines/screens/tooltip_lab_720p.png) · [Visual review](design-audit.html#screen=tooltip_lab)

- [ ] `tooltip_lab-01` · P2 · proposal. Page identity: Add Tooltip placement lab as the title.
  Evidence: The top bar is only an instruction sentence.

- [ ] `tooltip_lab-02` · P2 · proposal. Instruction size: Increase it to 13 px actual text.
  Evidence: The hover-and-flip instruction is small.

- [ ] `tooltip_lab-03` · P2 · proposal. Header padding: Add a consistent 12 to 16 px text inset.
  Evidence: The instruction starts almost at the bar's left edge.

- [ ] `tooltip_lab-04` · P2 · proposal. Top-left case: Add Normal below as an external case caption.
  Evidence: top left does not say it is the normal-below placement control.

- [ ] `tooltip_lab-05` · P2 · proposal. Top-right case: Add Clamp to right edge as an external caption.
  Evidence: top right does not identify horizontal clamping.

- [ ] `tooltip_lab-06` · P2 · proposal. Bottom-left case: Add Flip above as an external caption.
  Evidence: bottom left does not name the expected flip.

- [ ] `tooltip_lab-07` · P2 · proposal. Bottom-right case: Add Flip above and clamp as an external caption.
  Evidence: bottom right does not name its combined constraints.

- [ ] `tooltip_lab-08` · P2 · proposal. Middle case: Rename it Long content or add that caption.
  Evidence: middle does not reveal that it tests long tooltip text.

- [ ] `tooltip_lab-09` · P2 · proposal. Instant case: Label it Instant, 0 ms.
  Evidence: instant gives no numeric delay.

- [ ] `tooltip_lab-10` · P2 · proposal. Default delay: Add Default delay: 400 ms to the legend.
  Evidence: Other triggers do not show their 0.4 s delay.

- [ ] `tooltip_lab-11` · P2 · proposal. Expected tooltip content: Add a compact external case table listing trigger, message and expected placement.
  Evidence: No tooltip text is visible in the initial state.

- [ ] `tooltip_lab-12` · P2 · proposal. Open-state evidence: Capture one shown-tooltip state per edge case.
  Evidence: The baseline cannot show whether tooltip boxes stay within bounds.

- [ ] `tooltip_lab-13` · P2 · proposal. Viewport bounds: Add an optional edge guide for checking clamping without moving the triggers.
  Evidence: The dark field has no visible safe-area guide.

- [ ] `tooltip_lab-14` · P2 · proposal. Trigger geometry: Show them in a small fixture metadata panel.
  Evidence: Button dimensions and edge distances are undocumented.

- [ ] `tooltip_lab-15` · P2 · proposal. Active-case status: Add a read-only Active case status in the header.
  Evidence: There is no readout identifying the currently hovered specimen.

- [ ] `tooltip_lab-16` · P3 · proposal. Unused central space: Use a compact central legend without changing trigger positions.
  Evidence: Large blank regions are available between the fixed edge triggers.

#### Additional capture: tooltip_lab_shown

[Baseline](baseline_screenshots/tooltip_lab_shown.png)

- [ ] `tooltip_lab_shown-L01` · P2 · proposal. Instant tooltip copy: Use at least the readable caption size used by the trigger descriptions; verify the full sentence at 1280×720.
  Evidence: The shown tooltip sentence is much smaller than the instant trigger label.

- [ ] `tooltip_lab_shown-L02` · P2 · proposal. Instant tooltip padding: Add horizontal and vertical breathing room around the instant-tooltip sentence.
  Evidence: The tooltip sentence almost fills its narrow outlined rectangle.

- [ ] `tooltip_lab_shown-L03` · P3 · proposal. Tooltip border: Use a neutral tooltip edge and reserve red for a documented diagnostic or warning state.
  Evidence: A red outline surrounds a normal informational tooltip.

- [ ] `tooltip_lab_shown-L04` · P3 · proposal. Trigger-to-tooltip relationship: Add a small offset or anchor marker so the tooltip is visually attached to instant rather than a separate control.
  Evidence: The instant tooltip appears immediately below the button without a visible anchor cue.

### tray

[Baseline](screenshot-baselines/screens/tray_720p.png) · [Visual review](design-audit.html#screen=tray)

- [ ] `tray-01` · P2 · proposal. Page explanation: Add a sentence describing grouped arrow-key navigation.
  Evidence: Tray Navigation does not explain what a tray changes about focus.

- [ ] `tray-02` · P2 · proposal. Horizontal caption: Increase the caption to readable 13 px actual size.
  Evidence: Horizontal Tray Left/Right uses tiny text.

- [ ] `tray-03` · P2 · proposal. Vertical caption: Match the horizontal caption's size and spacing.
  Evidence: Vertical Tray Up/Down is similarly small.

- [ ] `tray-04` · P2 · proposal. Horizontal labels: Use Alpha, Beta and Gamma with a clearly labeled horizontal group.
  Evidence: H-Alpha, H-Beta and H-Gamma repeat an orientation prefix already in the heading.

- [ ] `tray-05` · P2 · proposal. Vertical labels: Use the same choice labels as the horizontal comparison.
  Evidence: V-Alpha, V-Beta and V-Gamma repeat the heading.

- [ ] `tray-06` · P2 · proposal. Color comparison: Use matching colors for corresponding choices or document the deliberate contrast test.
  Evidence: The horizontal and vertical groups use different color sets.

- [ ] `tray-07` · P2 · proposal. Tray boundary: Add subtle group frames outside the buttons.
  Evidence: The groups have no outline showing where tray ownership begins and ends.

- [ ] `tray-08` · P2 · proposal. Entry/exit guidance: Add Tab guidance for entering and leaving the tray according to actual behavior.
  Evidence: Only arrow directions are shown.

- [ ] `tray-09` · P2 · proposal. Activation guide: Add the supported activation key beside the navigation hints.
  Evidence: No text says how to activate the current tray item.

- [ ] `tray-10` · P2 · proposal. Selection feedback: Add Last activated: none beneath the groups.
  Evidence: No status shows the last activated item.

- [ ] `tray-11` · P2 · proposal. Active group feedback: Add a read-only Active tray status that updates during navigation.
  Evidence: The initial frame does not identify a keyboard-active tray.

- [ ] `tray-12` · P2 · proposal. Boundary behavior: Add a concise wrap/clamp note matched to implementation.
  Evidence: The three-item groups do not explain wrapping at their ends.

- [ ] `tray-13` · P2 · proposal. State coverage: Add companion captures with a focused item in each tray.
  Evidence: Only the initial unfocused composition is shown.

- [ ] `tray-14` · P3 · proposal. Bottom panel space: Use it for the key guide and status readouts.
  Evidence: A large blank band remains beneath V-Gamma.

- [ ] `tray-15` · P3 · proposal. Group spacing: Use consistent heading-to-group and group-to-group spacing.
  Evidence: The gap after the horizontal row is much larger than its caption gap.

- [ ] `tray-16` · P2 · proposal. Demo reset: Add a small Reset demo action outside the tray examples.
  Evidence: There is no visible way to restore the initial selection state.

### virtual_list_lab

[Baseline](screenshot-baselines/screens/virtual_list_lab_720p.png) · [Visual review](design-audit.html#screen=virtual_list_lab)

- [ ] `virtual_list_lab-01` · P2 · proposal. Instruction size: Increase it to 13 px actual size.
  Evidence: The entity-count explanation is tiny beneath the title.

- [ ] `virtual_list_lab-02` · P2 · proposal. Row identifiers: Increase row text size while retaining the fixed row-height test.
  Evidence: row 00000 through row 00019 are very small.

- [ ] `virtual_list_lab-03` · P2 · proposal. Row padding: Add 12 px text inset without changing the viewport width.
  Evidence: Identifiers begin flush at the list's left edge.

- [ ] `virtual_list_lab-04` · P2 · proposal. Entity-count claim: Add a live rendered-entity count above the list.
  Evidence: The instruction says entity count stays flat but no count is displayed.

- [ ] `virtual_list_lab-05` · P2 · proposal. Visible range: Show Visible rows 0 to 19 of 10,000 outside the viewport.
  Evidence: Twenty rows are visible with no summarized range.

- [ ] `virtual_list_lab-06` · P2 · proposal. Scroll position: Add numeric offset and percentage readouts.
  Evidence: The tiny thumb conveys little precise position information.

- [ ] `virtual_list_lab-07` · P2 · proposal. Total count formatting: Display 10,000 in descriptive text while keeping zero-padded row IDs.
  Evidence: 10000 appears without grouping separators.

- [ ] `virtual_list_lab-08` · P2 · proposal. Marked rows: Add a small marker icon or accent stripe alongside the text.
  Evidence: Rows 00000, 00007 and 00014 say marked but have no separate visual marker.

- [ ] `virtual_list_lab-09` · P2 · proposal. Marked rule: Add a caption naming the marker interval.
  Evidence: The reason every seventh row is marked is not explained.

- [ ] `virtual_list_lab-10` · P2 · proposal. Index convention: Label row IDs as zero-based.
  Evidence: The first row is 00000 without an indexing note.

- [ ] `virtual_list_lab-11` · P2 · proposal. Jump controls: Add Start, Middle and End presets outside the viewport.
  Evidence: Only manual scrolling is suggested for a very long list.

- [ ] `virtual_list_lab-12` · P2 · proposal. Jump-to-index: Add a row-index field for reproducible offscreen-range captures.
  Evidence: No direct location entry is visible.

- [ ] `virtual_list_lab-13` · P2 · proposal. Virtualization evidence: Add companion captures at a distant offset with the entity-count readout.
  Evidence: The initial frame cannot show entity reuse across distant ranges.

- [ ] `virtual_list_lab-14` · P2 · proposal. Viewport metrics: Show both values with the expected visible-row count.
  Evidence: The fixed row height and viewport height are undocumented.

- [ ] `virtual_list_lab-15` · P3 · proposal. Row schema: Use aligned Index and State columns for easier scanning.
  Evidence: Each line combines an index and state in loose text.

- [ ] `virtual_list_lab-16` · P2 · proposal. End-state framing: Add Last row: 09999 in the test metadata.
  Evidence: No indication describes the last row's expected index.

- [ ] `virtual_list_lab-17` · P3 · proposal. Unused row width: Use a compact content column with a subtle grid while preserving the full-width scroll region.
  Evidence: Most of every row is empty.

### vstack_showcase

[Baseline](screenshot-baselines/screens/vstack_showcase_720p.png) · [Visual review](design-audit.html#screen=vstack_showcase)

- [ ] `vstack_showcase-01` · P2 · defect. Top title inset: Add a 12 px title inset.
  Evidence: vstack() Showcase starts at the left edge of the cyan bar.

- [ ] `vstack_showcase-02` · P2 · proposal. Justification section: Add a clear main-axis justification heading above the row.
  Evidence: The top five cards have no shared JustifyContent heading.

- [ ] `vstack_showcase-03` · P2 · proposal. Top captions: Increase their external label size to readable 12 px text.
  Evidence: Start, Center, End, Between and Around are tiny.

- [ ] `vstack_showcase-04` · P2 · proposal. Between naming: Use Space between in the label and show the API name in metadata.
  Evidence: Between abbreviates the actual spacing mode.

- [ ] `vstack_showcase-05` · P2 · proposal. Around naming: Use Space around with a small edge-gap diagram.
  Evidence: Around omits the meaning of edge spacing.

- [ ] `vstack_showcase-06` · P2 · proposal. Main-axis direction: Add a vertical main-axis guide beside the specimens.
  Evidence: The top row demonstrates vertical placement without an axis arrow.

- [ ] `vstack_showcase-07` · P2 · proposal. Start specimen: Add an external top-edge marker.
  Evidence: The three blocks begin at the top but have no start guide.

- [ ] `vstack_showcase-08` · P2 · proposal. Center specimen: Add a faint external centerline tick.
  Evidence: The centered group has no midpoint reference.

- [ ] `vstack_showcase-09` · P2 · proposal. End specimen: Add an external bottom-edge marker.
  Evidence: The block group ends near the bottom without a baseline marker.

- [ ] `vstack_showcase-10` · P2 · proposal. Between gaps: Annotate the equal interior gap size outside the blocks.
  Evidence: The two interior gaps are not measured.

- [ ] `vstack_showcase-11` · P2 · proposal. Around gaps: Label the expected 1:2 edge-to-interior spacing relationship.
  Evidence: The edge gaps and interior gaps are visually different but unexplained.

- [ ] `vstack_showcase-12` · P2 · proposal. Block dimensions: Add a shared legend for their fixed sizes.
  Evidence: Cyan, magenta and green blocks use different heights with no dimensions.

- [ ] `vstack_showcase-13` · P2 · proposal. Cross-axis heading: Increase heading size and reserve enough height for it.
  Evidence: AlignItems cross-axis is tiny in the green strip.

- [ ] `vstack_showcase-14` · P2 · proposal. Cross-axis direction: Add a horizontal cross-axis arrow above the four cards.
  Evidence: The lower specimens do not show the horizontal axis.

- [ ] `vstack_showcase-15` · P2 · proposal. Stretch specimen: Annotate their width constraints and add an auto-width control case to explain what stretch can affect.
  Evidence: The blue blocks retain visibly different widths under Stretch.

- [ ] `vstack_showcase-16` · P2 · proposal. Lower specimen colors: Use matching specimen colors across modes or label color as decorative.
  Evidence: Each alignment mode changes color as well as position.

- [ ] `vstack_showcase-17` · P2 · proposal. Lower captions: Increase caption size and inner padding.
  Evidence: Start, Center, Stretch and End blend into thin green bars.

- [ ] `vstack_showcase-18` · P2 · proposal. Sidebar example: Increase it and identify this as an applied layout example.
  Evidence: Sidebar + Content is a tiny label across a long blue bar.

- [ ] `vstack_showcase-19` · P2 · proposal. Navigation labels: Increase label size without changing the sidebar layout structure.
  Evidence: Home, Settings, Profile and Help are very small.

- [ ] `vstack_showcase-20` · P2 · proposal. Content rows: Add 10 to 12 px inner text padding.
  Evidence: Alerts, Activity and Stats touch their colored row edges.

- [ ] `vstack_showcase-21` · P2 · proposal. Static example semantics: Label them Static layout example unless they are interactive.
  Evidence: The navigation and content rows look like an application without behavior explanation.

- [ ] `vstack_showcase-22` · P3 · proposal. Metadata space: Use it for the vstack default width/height rules and measured dimensions.
  Evidence: The lower content pane has a large unused area.

### widget_gap_gallery

[Baseline](screenshot-baselines/screens/widget_gap_gallery_720p.png) · [Visual review](design-audit.html#screen=widget_gap_gallery)

- [ ] `widget_gap_gallery-01` · P2 · proposal. Page title: Use Widget integration gallery with the included component names beneath it.
  Evidence: Widgets that had no caller describes project history rather than the displayed features.

- [ ] `widget_gap_gallery-02` · P2 · proposal. API headings: Pair each API name with a plain-language specimen title.
  Evidence: button_group, image and icon_row are terse code names.

- [ ] `widget_gap_gallery-03` · P2 · proposal. Button group boundaries: Use clear segmented-group joins or uniform small gaps.
  Evidence: Select, Move, Rotate and Scale touch with rounded edges at every join.

- [ ] `widget_gap_gallery-04` · P2 · proposal. Tool selection state: Use a labeled Selected tool status directly beneath the buttons.
  Evidence: no tool picked yet is small and separated from the group.

- [ ] `widget_gap_gallery-05` · P2 · proposal. Tool action meaning: Add a short caption that the group chooses an editing tool.
  Evidence: Select is both a tool name and a generic action.

- [ ] `widget_gap_gallery-06` · P2 · proposal. Pagination label: Use Pagination, five pages as the visible section heading.
  Evidence: pagination is a small API name without content context.

- [ ] `widget_gap_gallery-07` · P2 · proposal. Previous arrow: Document wrap behavior or render a disabled boundary state if previous is unavailable.
  Evidence: The previous arrow looks enabled while page 1 is selected.

- [ ] `widget_gap_gallery-08` · P2 · proposal. Selected page: Use one clear selected-state outline distinct from keyboard focus styling.
  Evidence: Page 1 has several close white outlines.

- [ ] `widget_gap_gallery-09` · P2 · proposal. Page status: Increase it and align it to the pagination group inset.
  Evidence: page 1 of 5 is small beneath the controls.

- [ ] `widget_gap_gallery-10` · P2 · proposal. Pagination width: Use equal numeral cells with balanced end-control widths.
  Evidence: The page buttons use inconsistent apparent widths beside narrow arrow controls.

- [ ] `widget_gap_gallery-11` · P2 · proposal. Spacer demo label: Add a faint external bracket over the space between head and tail.
  Evidence: spacer pushes the tail to the edge states the effect but not the flexible region.

- [ ] `widget_gap_gallery-12` · P2 · defect. Head label: Inset the label by 10 px.
  Evidence: head touches the left edge of its blue block.

- [ ] `widget_gap_gallery-13` · P2 · defect. Tail label: Apply the same 10 px text inset.
  Evidence: tail touches the left edge of its brown block.

- [ ] `widget_gap_gallery-14` · P2 · proposal. Spacer dimensions: Show fixed item widths and remaining flexible width below the sample.
  Evidence: Head and tail have no fixed-width metadata.

- [ ] `widget_gap_gallery-15` · P2 · proposal. Image specimens: Add captions beneath each image tile.
  Evidence: Gear, star and trophy images have no individual names.

- [ ] `widget_gap_gallery-16` · P2 · proposal. Image sizing: Document source dimensions and display size, and center their visible bounds consistently.
  Evidence: The three images have different visual mass inside equal tiles.

- [ ] `widget_gap_gallery-17` · P2 · proposal. Icon-row repetition: Show three distinct labeled sprites from the same sheet.
  Evidence: Three identical home icons reveal little about spritesheet cell selection.

- [ ] `widget_gap_gallery-18` · P2 · proposal. Icon-row spacing: Add the configured icon size and gap below the row.
  Evidence: The home icons lack a visible gap annotation.

- [ ] `widget_gap_gallery-19` · P2 · proposal. Save icon: Use a save icon or rename the action to Configure.
  Evidence: Save uses a gear icon that usually represents settings.

- [ ] `widget_gap_gallery-20` · P2 · proposal. Next icon: Use a right arrow or rename the action to Favorite.
  Evidence: Next uses a star rather than a directional icon.

- [ ] `widget_gap_gallery-21` · P2 · proposal. Icon-side comparison: Use the same icon and label in both positions for direct comparison.
  Evidence: Left and right icon placement also changes the icon and label.

- [ ] `widget_gap_gallery-22` · P3 · proposal. Section alignment: Align corresponding rows or give each specimen a clearly bounded card.
  Evidence: The left and right specimen sections use different vertical baselines.

- [ ] `widget_gap_gallery-23` · P2 · proposal. Unused lower panels: Use them for concise dimensions, source asset names and expected-result notes.
  Evidence: Both columns leave large blank regions below the final specimens.

## Existing project backlog

- [ ] Review a supported tree row/accessory renderer for icons and metadata columns; wm currently restyles tree_view descendants. See docs/AFTERHOURS_GAPS.md.

- [ ] Review composite drag-preview styling and viewport-aware drag hit testing; see docs/AFTERHOURS_GAPS.md.

- [ ] Make disabled controls noninteractive even when their labels are child entities; see docs/AFTERHOURS_GAPS.md.

- [ ] Make partial rounded outlines match fill masks and correct RoundedCorners::top_round; see docs/AFTERHOURS_GAPS.md.

- [ ] Add configurable visible-row limits and scrolling to native dropdown trays when reviewed upstream; see docs/AFTERHOURS_GAPS.md.

- [ ] Fix fractional-boundary nonprogress in native polyline::draw_dashed; wm quantizes marquee parameters to avoid the hang. See docs/AFTERHOURS_GAPS.md.

- [ ] Review native text-input focus origin across frames and its conflict with generic SelectOnFocus; see docs/AFTERHOURS_GAPS.md.

- [ ] Review native menu outside-click dismissal, disabled focus traversal and item/shortcut presentation options; see `docs/AFTERHOURS_GAPS.md`.

- [ ] Review a presentation/config argument for native convenience dialogs so consumers need not restyle returned descendants; see `docs/AFTERHOURS_GAPS.md`.

- [ ] Review bracket decoration bounds when the decorated component has padding; wm currently puts content inset on a child. See `docs/AFTERHOURS_GAPS.md`.

- [ ] Review text-area wheel scrolling, returned focus target and scaled auto-grow padding; see `docs/AFTERHOURS_GAPS.md`.

- [ ] Review external checkbox state synchronization so app resets do not require editing `HasCheckboxState`; see `docs/AFTERHOURS_GAPS.md`.

- [ ] Add reviewed chart options for series stroke, hover-label visibility and explicit axis bounds; native chart styling limitations remain in `docs/AFTERHOURS_GAPS.md`.

- [ ] Review backend-dependent auto-text fallbacks and contrast-validator foreground precedence; see `docs/AFTERHOURS_GAPS.md`.

- [ ] Review declarative animation elapsed-time clamping and same-property trigger composition in afterhours; see `docs/AFTERHOURS_GAPS.md`.

### First-segment animation easing

- [ ] Make `AnimHandle::sequence()` apply the first segment's easing on fresh and replayed tracks.
  `animation_basic` uses chained `.to()` calls until the library fix is reviewed.
  See [the measured gap](docs/AFTERHOURS_GAPS.md#animation-sequences-ignore-the-first-segment-easing).

### Modal heading font

- [ ] Make modal headings inherit the configured font or accept an explicit title style.
  The open advanced-modal examples retain the library's Gaegu heading. See
  [the gap](docs/AFTERHOURS_GAPS.md#modal-headings-bypass-the-configured-default-font).

### Scroll state after changing overflow

- [ ] Remove obsolete scroll state when an element changes from Auto/Scroll to Hidden/Visible.
  wm clears it explicitly in `adaptive_scaling`. See
  [the gap](docs/AFTERHOURS_GAPS.md#overflow-mode-changes-retain-scroll-state).

### Single-line text insets

- [ ] Fix the single-line renderer ignoring `with_text_inset()` after upstream review.
  Padded label containers work around this in wm. See
  [the gap](docs/AFTERHOURS_GAPS.md#single-line-labels-ignore-explicit-text-insets).


### Charts after the profiling UI

- [ ] Extend UP-12 beyond the charts needed by the default profiling UI.
  Keep unneeded area, bar, scatter and sparkline variants as follow-ups.
  Reassess pie/donut, stacked charts, histograms and pan/zoom when a consumer
  needs them; these options are not individually approved requirements.
  Extend the interactive wm chart test screen with each implemented type.
  See `docs/gap-design-decisions.md`, D-02.

### Screen navigation follow-up

- [x] Improve screen switching and discovery. Investigate the duplicate screens
  reported when cycling with `,` and `.`; show each intended screen once and
  give intentional variants distinct names. Build a searchable tree with
  collapsible categories, keyboard navigation and a clearly marked current
  screen. Search across collapsed categories and preserve category context.
  Implemented and verified; see `docs/screen-navigation.md`.
  The user selected this structure; see `docs/gap-design-decisions.md`, D-05.

### Performance follow-ups — 2026-09-12

- [ ] Move screen artwork into texture atlases. Measure texture switches, draw
  calls and frame time before and after; preserve filtering and crisp edges.
- [ ] Speed up launch. Profile time to the first usable screen, including font
  loading, asset loading and screen initialization; defer work that is not
  needed for that first screen. Record cold and warm startup measurements.
- [ ] Audit CPU and RAM usage. Measure idle and active screens, then repeat
  screen switches to check for memory growth and retained textures/entities.
  Record representative frame times, CPU usage, resident memory and peaks
  before choosing optimizations.

Start with wm-owned changes. Any required afterhours edits remain pending user
review. These follow-ups are separate from the current visual parity audit.

### The short version

Fixed in afterhours: scroll views misplaced margined children; sideways
`expand()` gave 0px; centering was skipped when a thing was bigger than its
box; added `with_corner_radius(px)`; added right-click; menu shortcuts were
cut in half.

Fixed here: corners set to 10px so big panels stop looking like blobs;
`--update-baselines` no longer overwrites test images that were fine; 6 new
screens; the comparison tool had 5 bugs of its own.

**Worth remembering:** three "missing features" already existed and just failed
silently, so people assumed they were missing. Font weights work but need a
font named `"Inter@bold"` and nothing tells you that. Making the library speak
up when it quietly does nothing is probably worth more than the next feature.

**Biggest things left:** scrollbars, and moving the test helpers into
afterhours — cartographer, kart, hanabi and wm each built the same ones.

The rest of this file is the detail.

---

Open work from the 2026-08-15 session: the mock-tool layout sweep, the roundness
API, and the cross-project gap-doc audit. Grouped by where the fix lands, since
that decides who can do it.

Numbers here came from `./mocks/build.sh --no-serve && ./mocks/analyze.py` and
`./mocks/sheets.sh`. Detail lives in `mocks/ISSUES.md`.

---

### Where each fix lands

"Both" means the two halves are separable and want separate commits. "Unknown"
means the sweep found a disagreement but nobody has decided yet whether
afterhours is wrong or the screen is — that decision IS the work.

| # | issue | afterhours | wm | other |
|---|---|:--:|:--:|:--:|
| 1 | Font weights fall back silently | explicit family registration + warn | register variants | |
| 2 | Alpha blending on sokol | sokol blend mode | | verify from floatinghotel |
| 3 | ~~No scrollbar~~ **indicator done** | `RenderScrollbars` | 6 baselines | |
| 4 | Overflow is silent | warning tolerance | over-specified screens | |
| 5 | ~~No right-click~~ **done** | `176ea8f` | `context_menu_lab`, e2e 94 | |
| 6 | ~~`context_menu` clips shortcuts~~ **done** | `c87b9d7` | | |
| 7 | Never-implemented gap requests | most of them | e2e pack upstreams **from** wm | |
| 8 | Slider knob, tab bars, hard wrap | all three | | |
| 9 | ~~Corner radiuses~~ **done** | `d265f0e` | `48db688`, 51 baselines | |
| 10 | ~~Family A~~ **resolved** | | was the mock: strictness + padding | |
| 11 | Family B (font metrics) | | won't-fix, documented | |
| 12 | ~~Family C~~ **done** | fixed, `17b982c` | 9 baselines | |
| 13 | Family D (heights) | likely for `file_tree`/`forms` | text metrics for `multiline_text_lab` | |
| 14 | Family E (diagonal offset) | **unknown** | border theory disproved | |
| 15 | Outliers, `islands_trains` 80 | **likely** | | |
| 16 | ~~Mock limitations~~ **done** | | `mocks/mock.js` | |
| 17 | Five `with_roundness(px)` sites | | | floatinghotel |
| 18 | Gap docs list shipped work | | | 5 docs, 4 repos |

Counts: **6 afterhours-only** (1, 2, 3, 6, 8, 12), **4 both** (4, 5, 7, 9),
**2 other-repo** (17, 18), **2 unknown** (13, 14, 15), **9 done or won't-fix**
(5, 6, 9, 10, 11, 12, 16, 19), plus 15 folded into the sweep numbers.

The two wm-only items are closed: 16 is done and 11 is a documented won't-fix.
Doing 16 also resolved item 10 outright and shrank 12 — the sweep went from 383
origins to 164, and the remaining unknowns are now much more likely to be real.

---

### afterhours (library)

#### 1. Font weights fall back silently — **afterhours** (API), wm (content)
**Earlier entry here was wrong** and is corrected: I filed this as "the API
landed but the capability did not" and said the text path needed variant
selection. It already has it. `FontManager::resolve_weighted`
(`ui_core_components.h:420`) maps `(base, weight)` to a registered
`"<base>@bold"` and the render path calls it in four places.

All five weights render identically on `font_weight_720p` because that screen
uses the default font, and wm only registers one variant in the whole app
(`DGOne@bold`, `font_config.h:85`). Nothing is broken — the fallback is doing
what it says.

The real defect is the *shape of the contract*, and it is the same one
`is_right_click` had:

- The requirement is a **string naming convention** (`"Inter@bold"`) that
  appears nowhere in `with_font_weight`'s signature or docs. A caller has no
  way to discover it.
- `resolve_weighted` falls back to the base font **silently**. You ask for
  SemiBold, you get Regular, and nothing says why. floatinghotel filed "No
  Font Weight Support" — they almost certainly hit exactly this.

Worth fixing as an API question, not a font question: declare a family's
weights explicitly rather than string-encoding them, and warn once when a
requested weight falls back. See the design note below.
#### 2. Alpha blending — verify on sokol — **afterhours** (sokol backend)
The one gap filed independently by two projects (floatinghotel, hanabi #15):
low-alpha `with_custom_background` reported to render opaque. It does **not**
reproduce on raylib — `alpha_blend_repro_720p` steps cleanly from 32 to 255 with
the text readable through the wash, for both a low-alpha `Color` and
`with_opacity`.

Both reporters are sokol. `RL_BLEND_ALPHA` is set only in
`backends/raylib/headless.h:52`; no blend mode is set in the sokol path. Run
`alpha_blend_repro` from floatinghotel before calling it fixed or broken.

#### 3. `HasScrollView` renders no scrollbar — DONE (indicator; drag is follow-up)
hanabi #26. Done: `RenderScrollbars` draws a track and thumb for any
overflowing scroll view, on either axis.

A separate render pass rather than a branch inside each renderer — the two
order their output differently (`RenderBatched` sorts by layer, `RenderImm`
paints in tree order) and the bar has to land on top of its content, so one
pass after both is the only way to avoid two implementations that drift. The
geometry is a pure function, `scrollbar_geometry` in `components.h`, so it is
unit-tested without a renderer and both paths cannot disagree about where the
bar is.

**Still open: dragging the thumb.** Today it reports position without setting
it. That needs hit-testing against a drawn rect that is not an entity, so it is
its own piece of work.

#### 4. Children that do not fit are silently overflowed — **both**
`mocks/ISSUES.md` issue 1. 21 nodes across `powerwash_settings` (9),
`flex_alignment` (5), `layout` (3), `deadspace_settings`, `meters_gauges`,
`rubber_bandits_menu`, `forms`.

Caller error in every case, so the library question is only whether it is
*visible* at authoring time. `strictness = 1` correctly refuses to shrink and
`flex_wrap` is NoWrap by intent, so what is actually left is that the overflow
warning does not fire: its tolerance is `sx + child_margin + 4px`, and
`powerwash_settings` overflows by 8px behind a 20px margin. That allowance was
deliberate (a `percent(1)` child plus margin overflows by exactly its margin and
the caller cannot act on it), so tightening it needs a way to tell that idiom
apart from real overflow — not a smaller constant.

#### 5. No right-click trigger for `context_menu` — DONE
There is no secondary-click support anywhere in afterhours: `MOUSE_BUTTON_RIGHT`
does not appear in the source, and `HasClickListener` has a single `down` with
no button field. So `context_menu()` exists but cannot be opened the one way
users expect — `context_menu_lab` has to drive it from ordinary buttons.

Fixed in afterhours `176ea8f`. `MousePointerState` tracks the secondary button
and `UIContext::is_right_click(id)` answers "a secondary click finished over
this element or something inside it"; pair it with `mouse.pos` for the anchor.
`right_click x y` joins `click` in the e2e vocabulary, and
`94_right_click_context_menu.e2e` drives the whole path. `context_menu_lab` now
uses real right-clicks instead of stand-in buttons.

Caveat worth knowing: the target must be hit-testable for `hot` to land on it,
which today means carrying a click or drag listener. Fine for rows and buttons,
not for a bare panel.

#### 6. `context_menu` clips the shortcut column — DONE
Fixed in afterhours `c87b9d7`. The shortcut is right-aligned, so its box's
right edge is where the last glyph lands, and the box ran out to `width` — the
final character was clipped in half. The gutter already reserved a pad for
this; it was being spent on width instead of on an inset.

#### 7. Never-implemented requests from the gap docs — **afterhours** (one is wm→afterhours)
No upstream implementation at all, so a wm screen cannot be written until the
feature exists:

- ~~**Synchronized scroll views**~~ — DONE, see item 25.
- **Scroll anchoring / preserve-position-on-prepend** (hanabi #30)
- ~~**Shared e2e command pack**~~ — DONE except the CLI surface. The 21
  commands were already upstream; `click_btn`/`expect_label` are `click`/
  `expect_text` renamed. Added `expect_text_i`, `disable_animations`. Still
  open: `afterhours::e2e::Args` + `parse_args` for the 10 library-owned flags.
- ~~**Headless affordances**~~ — DONE. Resolution falls back to
  `window_manager::headless_resolution` instead of (0,0); `measure_text`
  estimates instead of returning {0,0} for an atlas-less font;
  `afterhours::shutdown()` orders entity teardown before the backend;
  `SINGLETON_CLASS_FWD` already existed (`e9c662b`) and is now signposted.
  Verified by `tests/headless_fallback_test.cpp`, which never calls
  InitWindow — wm's `--headless` builds a real GL context, so it cannot
  reproduce any of these.
- ~~**Text metrics / measure+wrap API**~~ — DONE, `ui::measure_text_wrapped`.
- ~~**Animation instant-mode / clear-all**~~ — DONE, `animation::set_instant` /
  `clear_all`. Still open: per-item stagger, exit animations (hanabi AN-8..AN-12).
- ~~`Margin`/`Padding` single-side helpers~~ — already existed as
  `Margin::Left(pixels(10))` (afterhours `390b5b7`, Feb 2026); lowercase `left`
  is impossible, it collides with the field. The real bug found while checking:
  `Margin` and `Padding` declared their fields in *different orders*, so a
  positional `{a,b,c,d}` silently meant different things. Fixed.

#### 8. Long-standing, low priority — **afterhours**
From `docs/AFTERHOURS_GAPS.md`, unchanged: slider handle 0.75 compression;
crowded tab bars still need a smaller font at the call site; word-wrap has no
hard character break.

---

### wm_afterhours

#### 9. Corner radiuses — make them consistent — DONE
afterhours `d265f0e` (precedence) + wm `48db688` (10px in the presets). Today `Theme::roundness = 0.5f` scales with the widget,
so the same theme value is ~8px on a row and 180px on a full-height panel; the
mock sheets show the big containers as blobs. `with_corner_radius(px)` now
exists, so the work is:

1. ~~Precedence~~ — fixed in afterhours. Order is caller px > caller roundness
   > theme px > theme roundness, and the two theme-level spellings now clear
   each other.
2. ~~A pixel default~~ — set to 10px in wm's four presets, **not** in
   afterhours. A default `Theme::corner_radius` would win over downstream code
   that assigns `theme.roundness` directly (wordproc zeroes it for square
   corners), so the value belongs in each app's presets.
3. ~~Re-baseline~~ — 51 of 98 screens repainted.

Still open, low priority: wm has ~350 per-call `with_roundness` sites, many of
which meant px. They are unaffected by the theme change (a caller fraction
wins) and can be converted opportunistically.

Pick the default off the current spread — 1329 painted nodes already resolve to
<4px, 1118 to 4-12px, and only 88 exceed 80px (`tray_bg`, `toggle_bg`,
`text_input_root`, `root_bg` are the 180px ones).

The ~/p/ survey backs the change: across nine projects `with_roundness(0.0f)` —
switching it off — is the single most common call at 307, eleven times more than
the `disable_rounded_corners()` helper that exists for it, and wordproc gives up
and zeroes `theme.roundness` globally.

---

### The html/mock sweep — every issue it found

`./mocks/build.sh --no-serve && ./mocks/sheets.sh` →
`mocks/sheets/all_disagreements.png` (44 screens) plus `sheet01..16.png`.
Dashed red is afterhours' rect over the CSS pane.

**Updated after the wm-side mock fixes (item 16, done).** Three modelling gaps
were doing most of the talking; closing them took the sweep from 44 screens /
718 nodes / 383 origins down to **26 screens / 198 nodes / 164 origins**, and
the nodes excluded for sitting under a scroll/clip container from 206 to 3.

Deltas below are `(dx, dy, dw, dh)` in px, CSS minus afterhours. Counts in the
family tables are POST-fix unless marked.

#### 10. Family A — fixed-width children in a row that does not fit — RESOLVED, mock-side
Was 115 origins (`sports_settings` 62, `powerwash_settings` 36, `flex_alignment`
15, `adaptive_scaling` 4, plus singles). **Now zero.** Two mock bugs, both fixed
in `mocks/mock.js`:

- `strictness` was not modelled at all. CSS defaults `flex-shrink: 1` while
  afterhours defaults `strictness = 1` and refuses to shrink, so the mock was
  shrinking every overflowing row that afterhours deliberately let overflow.
  Now `flex-shrink: 1 - strictness`, plus `flex-grow: 1` for `strictness == 0`
  (which `tax_refund` grows) — suppressed when a sibling uses `Expand`, because
  `tax_refund` hands Expand all the slack and returns.
- **Padding larger than the box.** afterhours clamps the *content* area to zero
  (`fmaxf(0, computed - padd)`) and keeps the box at its stated size; CSS
  border-box instead floors the box at the padding. `sports_settings`' stepper
  arrows are `pixels(40)` with 25.6px of side padding, so CSS made them 51.2 and
  every sibling shifted. The mock now drops padding that exceeds a `Pixels` box.

Worth noting as an afterhours *design* question rather than a bug: padding
exceeding the element is accepted silently and yields a zero-width content area.
That is over-specification in the screens — `pixels(40)` with 51.2px of padding
is not meaningful — and nothing warns.

#### 11. Family B — shrink-to-fit around a label — **wm (mock)**, won't-fix
**Mock-side, expected.** `Children`-sized boxes take their width from text, and
browser font metrics are not raylib's. Listed so they are not re-filed.

`horizontal_drag` (7), `setting_row_showcase` (some of its 11), and one node
each on `layout_bug_repros` (-186) and the `dialog_*` screens. Survives the
fixes above, as expected — nothing about `strictness` or padding changes what a
browser thinks a string is wide.

#### 12. Family C — cross-axis align is skipped when the child overflows — DONE
Fixed in afterhours `17b982c`. **Root cause was verified before changing anything.** `compute_relative_positions` wraps the whole
align block in `if (cross_remaining > 0.f)`
(`vendor/afterhours/src/plugins/autolayout.h:1428`), so when a child is *larger*
than the parent's cross-axis content box, `align_items: Center` silently
degrades to FlexStart. CSS centers regardless, overflowing symmetrically.

Arithmetic, checked on three screens:

| screen | parent | content | child+margin | free | predicted dy | observed |
|---|---|--:|--:|--:|--:|--:|
| `images` | `row1_sprites` | 95.6 | 104.4 | -8.8 | -4.4 | **-4.4** x8 |
| `buttons` | `row1_states` | 51.2 | 59.4 | -8.2 | -4.1 | **-4** x7 |
| `layout` | `row_container` | 91.2 | 108.8 | -17.6 | -8.8 | **-9/-8** x3 |

All three parents are `Row` with `align_items: Center`. Also explains
`layout_patterns` (3), `button_variants` (2), `cards` (1) and part of
`meters_gauges` — about **27 origins across 7 screens**.

Fixed by letting Center and FlexEnd use a negative `cross_remaining` instead of
bailing — one line, `> 0.f` to `!= 0.f`, since both already compute correctly
from a negative remainder. Moved 9 screenshot baselines. Regression tests
`align_items_center_row_child_overflows_cross_axis` and the FlexEnd twin.

**Not** covered by this: `deadspace_settings` (9 origins, dy+9). Its parent
`sidebar` is a `Column` with `align_items: FlexStart` and **+281px** of free
space, so it is main-axis stacking, not cross-axis align. Still unexplained —
track it separately.

#### 13. Family D — height disagreements — **unknown**, mixed
- `multiline_text_lab` — `Pixels/Text`, dh -54 and -72. The only `Dim::Text`
  users in the whole sweep, and the biggest height gaps. Wrapped line count
  differs, so this is text metrics again, but far larger than Family B.
- `file_tree` — x2 `Percent/Expand` (0,-17,0,22). Expand resolving to a
  different height, worth a look after the cross-axis expand fix.
- `decorators` — x2 `Percent/Children` (0,7,0,-14).
- `forms` — x3 `Percent/Percent` (0,-1,0,-13).
- `drag_drop` — x4 `Percent/Percent` (0,0,0,-3).

#### 14. Family E — uniform diagonal offset — **unknown**
`decorative_frame` (2,2) and (8,8); `cozy_cafe` (4,4) and (14,14). dx equals dy
exactly.

**The border hypothesis is wrong.** `HasBorder` exists
(`components.h:398`) but `autolayout.h` never reads it — a border is drawn
inside the rect and does not inset the content box. So adding border width to
the dump would not help, and putting a CSS `border` on these nodes would
actively break them under border-box. Cause still unknown; the equal dx/dy is
the only lead.

#### 15. Single-screen outliers — **unknown**
- **`islands_trains_settings` — 80 origins, the largest single screen.** raylib
  renders a 430px centred modal; the CSS re-solve spreads it far wider (Δx up to
  361 on the buttons). One of the two is wrong about `align_items: Center` on a
  `ScreenPercent` child. Not explained by Family A.
- `themes` — one node at dx 278.
- `meters_gauges` — one node at dx 115.
- `pagination` (x6 dw 3), `parcel_corps_settings` (x6 dw 7) — uniform small
  widths, possibly Family A.
- `cards`, `kirby_options`, `vstack_showcase`, `hstack_showcase` — 1-2 nodes
  each, sub-5px. Noise-adjacent; check last.

#### 16. Known mock limitations — DONE
- ~~No `strictness`~~ — modelled, see item 10.
- ~~Padding exceeding the box~~ — modelled, see item 10.
- ~~No content-size floor~~ — `min-width/height: 0` on every node, since a flex
  item's automatic minimum is its content size and afterhours has no equivalent.
  No measurable effect on its own, but it is correct and stops a labelled
  fixed-size box reading as a disagreement later.
- ~~206 nodes unreachable under scroll/clip~~ — **down to 3** (`aim_chat`). They
  were never really a clipping problem; the cascade from the three bugs above
  was reaching into scroll subtrees.
- Border width in the dump — **not needed**, see item 14.
- Absolute nodes are replayed from afterhours' own answer rather than re-solved.
  Correct (a caller-supplied coordinate has no second opinion) but it means the
  mock cannot catch a bug in absolute placement. Unchanged, by design.

#### 19. `--update-baselines` rewrites every e2e capture — DONE
`baseline_screenshots/` (24 PNGs, used by the `validate_screen` e2e command) is
separate from `screenshot-baselines/screens/` and is refreshed by running the
e2e suite with `--update-baselines`. That flag rewrites **every** capture
regardless of whether it was failing.

Found the hard way on item 12: the run rewrote 14 of the 24, including
`checkboxes` (6.9% different) and `themes` (4.6%) whose clean renders had not
moved at all. Stashing the fix and rebuilding showed pre-change was 100/100, and
re-applying it failed exactly **one** script — so 13 of the 14 were pre-existing
sub-threshold drift being silently baked in. They pass because the threshold is
1%, but they were never pixel-identical.

Fixed: `--update-baselines` now rewrites only captures that are missing or
actually over the threshold, and logs `Kept baseline: <name> (within threshold
at N%)` for the rest so the drift is visible instead of silently absorbed. The
failing threshold and the refresh threshold are now one constant, since they
have to agree or a refresh reintroduces exactly what the check let through.

Verified by corrupting `themes.png` and re-running: one file rewritten, the
other 23 kept, and the drift printed for each (`checkboxes` 0.53%, `forms`
0.33%, `toggle_switches` 0.32% — the pre-existing drift that used to get
committed unnoticed).

#### 20. A real `warn_once()` — DONE
Five warn-once sites now, each hand-rolling its own guard: a
`mutable std::set<EntityID>` (`context.h`, `is_right_click`), a
`mutable std::set<std::string>` (`ui_core_components.h`, font-weight
fallback), a `static std::set<float>` (`component_config.h`,
`with_roundness`), and two bools on `UIComponent`
(`warned_expand_collapse`, `warned_wrap_needs_font_size`).

Replaced by one `warn_once(key, fmt, ...)` in `logging.h`. It sits outside the
logging mode blocks so it expands to whichever `log_warn` is in scope — including
the one a test substitutes via `AFTER_HOURS_REPLACE_LOGGING`. Each call site gets
its own gate (keyed on the address of a `static` local unique to the expansion),
so the same key at two sites cannot silence one of them. All five sites
converted; both `UIComponent` bools deleted.

#### 21. Silent-failure audit — first pass done, little left
Swept for places the library quietly does nothing. Far less than expected,
because the three found this week were already fixed. Recording what was
checked so the sweep is not repeated:

| candidate | verdict |
|---|---|
| `resolve_constraint` ignores `Children`/`Expand` as a min/max | Real, but **nobody downstream writes it** — zero uses across all 20 projects. A diagnostic for a mistake no one makes. |
| `mk()` id collisions (cartographer's top frustration) | **Not silent.** A same-frame collision throws `bad_optional_access` and `mk()` already logs file/line/function plus the fix first. Verified: a deliberate collision aborts with exit 134. Their complaint is about the message being confusing, not absent — worth rewording, not re-plumbing. |
| `theme.ui_scale` ignored in Proportional mode | Real, but a mixed app legitimately has Proportional widgets while ui_scale is set, so a warning would fire on correct setups. |
| `dropdown`/`stepper` with empty options render nothing | Ambiguous — an empty list is legitimate mid-load. Left alone. |
| `with_roundness(0)` vs `disable_rounded_corners()` | Checked: roundness 0 does produce square corners at draw time. Not a trap. |

The pattern holds — the silent failures that mattered were the three already
fixed (font weight, wrap-without-size, `is_right_click`). Not worth a second
pass unless a new report points at one.

#### 22. e2e failures are blamed on the wrong script — **DONE**
Two causes, both fixed. `reset_test_state` doubled as the batch loader's script
separator, so `79_undo_redo.e2e` using it mid-script shifted every later
result by one — that is the whole "blamed on a later script" symptom. The
separator is now an internal `__end_of_script`. Separately, the last command
of a script was finalized in the tick it was dispatched, which is why a
single-script run exited 0 while logging the timeout.

Found on the way: `src/log/log_macros.h` put `assert(false)` *outside* the
`if` in `log_error`, so `if (cond) log_error(...)` crashed unconditionally.
All four macros are `do/while(0)` now.

Regression test: `tests/e2e_scripts/fail_script_attribution.e2e` uses the
`fail_` prefix convention, so it goes red whether the error is lost or lands
on the wrong script. Verified by reintroducing the bug.

<details><summary>original report</summary>
A failing assertion in script N is charged to a script that runs later, so the
suite fails pointing at innocent files.

Reproduce: put `expect_text "row 00400"` (a string that is not there) in
`95_scrollbar_drag.e2e` and run the suite. It fails — reporting
`98_test_themes` and `99_check_all_screens`, never 95. Run 95 alone with
`--test-filter` and it reports **PASS** despite logging the timeout.

Cause looks like ordering in `runner.h` around line 582: the finalize reads
`get_command_error_count()` at script end, but `E2ECommandCleanupSystem`
increments it later, so the error lands in the next script's window. Filtering
to one script leaves nowhere for it to land at all, hence the false PASS.

This is expensive: it cost real time twice this session — once chasing
`98`/`99` failures that were actually a stuck mouse button from script 94, and
again here. Worth fixing before the next debugging session pays for it a third
time.
</details>

#### 23. e2e gap requests — DONE (three of four; the fourth was stale)
cartographer and kart's "shared e2e command pack" is mostly already upstream —
21 commands, and their `click_btn`/`expect_label` are `click`/`expect_text`.

Landed in afterhours `14bb1be`: `expect_text_i`, `animation::set_instant` +
`clear_all` with `disable_animations`/`enable_animations` over them, and a
clearer unknown-command error naming the registration-order rule (which is what
cartographer actually hit — it is loud, just baffling).

**Still open from the plan: the CLI surface.** `E2EArgs` + `parse_e2e_args`
lives only in wm; ten of its twelve flags drive something afterhours owns
(`runner.set_timeout`, `graphics::Config::time_scale`,
`capture_every_n_frames`, ...) and belong upstream. The two that do not —
`--update-baselines` and kart's `--screenshot-dir` — stay down, because the
`screenshot` command takes an app-supplied callback and afterhours has no
opinion on where files land. Not started; it touches every consumer's `main()`.

**Also found:** adding an e2e command needs the handler *and* an entry in
`runner.h`'s per-command parse chain. Miss the second and args arrive empty
with no diagnostic.

#### 29. Virtual list renders ~20 rows below its clip at max scroll — **afterhours**
At the end of `virtual_list_lab`, the bottom visible row is 09979 while rows up
to 09999 are rendered below the pane's clip rect. Clamping stops short of
putting the true last row on screen, so the end of a virtual list is
unreachable.

Masked until now: `95_scrollbar_drag.e2e` asserted `row 09999` and passed only
because the visible-text registry ignored clipping. It now asserts 09979 with a
pointer here.

#### 24. Synthetic `scroll_wheel` is a no-op headless — DONE
The reader ran before the writer: wm registers `register_after_ui_updates`
(HandleScrollInput) before `register_builtin_handlers`
(HandleScrollWheelCommand), so the wheel was read, then set, then wiped. The
injector's comment claimed the opposite ordering, which is why it never got the
one-frame survival pinch has.

Wheel now survives one reset and is not drained on read (matching raylib);
pinch is now drained on read (matching the hardware accessor), which also
closes the double-delivery puzzle/ reported.

Knock-on fixes, all previously-vacuous assertions: `expect_no_text` was missing
from `runner.h`'s parse chain *and* concluded absence against an empty
registry; the visible-text registry ignored clip rects. Written up in
`docs/47_silent_traps.md`.

<details><summary>original report</summary>
Every wheel-driven e2e assertion in the suite is currently meaningless, and it
fails green. `12_scroll_view.e2e:25` already carries a note saying its own
scroll assertions "reflect the actual unscrolled state"; the first draft of
`97a_sync_scroll.e2e` passed identically with the feature under test disabled.

The plumbing looks right on inspection — `input_injector::set_mouse_wheel()`
stores it, and `input_system.h`'s `get_mouse_wheel_move_v()` reads
`consume_wheel()` under `test_mode` — so something between the two eats the
value. Note `consume_wheel()` does not clear, and `reset_frame()` does; that is
the same shape as the pinch double-delivery bug, so suspect frame ordering
first.

Workaround in the meantime: drive scrolling through the scrollbar thumb, which
does work headless (`95_scrollbar_drag.e2e`, `97a_sync_scroll.e2e`).
</details>

#### 27. e2e command to grab the scroll handle — **afterhours**
Scrolling in a test means hand-computing the thumb's pixel position:

```
mouse_move 384 125
mouse_down 384 125
mouse_move 384 400
mouse_up 384 400
```

384 is the pane's right edge minus half the bar thickness, and 125 is "near the
top of the track" — both derived by hand from `scrollbar_geometry()` and both
silently wrong the moment the pane moves or `scrollbar_thickness` changes. A
miss does not fail; it scrolls nothing and the assertions afterwards pass or
fail for the wrong reason.

This is not a nicety, because **the wheel is a no-op headless (item 24)**, so
dragging the bar is currently the only way to scroll in a test at all.

Want something like `scroll_to <debug_name> <percent>` that resolves the view
by name, asks `scrollbar_geometry()` where the thumb is, and drives the drag —
or writes the offset directly and skips the pointer entirely. Fails loudly if
the name does not resolve or the view does not scroll.

First users: `95_scrollbar_drag.e2e`, `97a_sync_scroll.e2e`.

#### 28. Fold `SyncScrollViews` into `HandleScrollInput`'s pass — **afterhours**
Low priority; filed because it was asked about, not because it is measurable.

`SyncScrollViews` iterates `HasScrollView` a second time each frame, over a set
that is ≤5 entities on the busiest wm screen (14 across all 100), with
`if (sync_group == 0) return;` as its first line. `HandleScrollInput` already
walks the same set, so the collect could hoist into it and the apply into its
`after()` — one fewer system, no extra pass.

Not done because that system early-returns on `!was_rendered_to_screen`,
`should_hide`, and the `auto_overflow` no-overflow case. The collect has to sit
*above* all three or members silently stop syncing in those states, and the
symptom would be a green test with a broken pane.

**Rejected alternatives**, so nobody re-proposes them: caching the group→member
index across frames (the UI is immediate-mode — `mk()` rebuilds the tree every
frame, so cached component references dangle), and running the apply at a lower
tick rate (scroll is latency-critical; a throttled apply shears the panes apart
mid-drag).

#### 26. `RefComponent` to match `RefEntity` — **afterhours**
`entity.h:300` has `RefEntity = std::reference_wrapper<Entity>` and a
`RefEntities` vector alias, but there is no component-level equivalent. A
system that collects during `for_each_with` and acts in `after()` — the pattern
forced by `EntityQuery` not seeing UI entities (`docs/47_silent_traps.md`) —
has to spell `std::vector<std::reference_wrapper<HasScrollView>>` by hand, or
reach for raw pointers.

```cpp
template <typename T> using RefComponent = std::reference_wrapper<T>;
template <typename T> using RefComponents = std::vector<RefComponent<T>>;
```

Then `SyncScrollViews::members` is `RefComponents<HasScrollView>`. Trivial, but
it is the difference between the collect-then-apply pattern looking idiomatic
and looking like a workaround. First user is `SyncScrollViews` in
`plugins/ui/systems.h`.

#### 25. Synchronized scroll views — DONE
`HasScrollView::sync_group`; non-zero ids scroll together, only on the axes
each member has enabled. Screen `sync_scroll_lab`, e2e `97a_sync_scroll.e2e`.

Two traps found building it, both written up in `docs/47_silent_traps.md`:
`EntityQuery` returns nothing for UI entities from inside a system, and the
wheel no-op above.

---

### Other repos (not ours to land)

#### 17. floatinghotel: five `with_roundness(px)` sites render as pills — **floatinghotel**
`src/ui/diff_renderer.h:614`, `src/ecs/main_content_system.h:815`,
`src/ecs/sidebar_system.h:700`, `:706`, `:781` pass `4.0f` / `2.0f` into a 0..1
fraction. Both backends clamp the computed radius to half the short side, so on
a 28px row `4.0f` is a 14px radius — a full pill, not 4px corners.
`with_corner_radius(4.f)` is the fix. **Owner will do this on their next bump**;
the new warn will point at each one.

#### 18. Reconcile the five gap docs — they list shipped work as blockers — **other repos**
`floatinghotel/docs/afterhours-gaps.md` (last touched Aug 1) lists six "Missing
Primitives", all with app-local workarounds. Every one now exists upstream —
`menu.h` and `overlay.h` were added Aug 3, two days later:

| doc says missing | actually in afterhours |
|---|---|
| Draggable Divider — BLOCKER P0 | `divider()` `imm_components.h:338` |
| Split Pane — BLOCKER P0 | `hsplit_pane` / `vsplit_pane` `:417` |
| Tree Node — BLOCKER P1 | `tree_view()` `tree_view.h:110` |
| Dropdown Menu — HIGH P0 | `dropdown_menu()` `menu.h:208` |
| Context Menu — HIGH P1 | `context_menu()` `menu.h:237` |
| Anchored Popover — MEDIUM P0 | `popover()` `overlay.h:248` |
| No rich text / multi-color | `with_styled_label(std::vector<TextSpan>)` |
| Virtualized list | `virtual_list()` `imm_components.h:141` |
| Row flex broken with `expand()` | 4 passing tests, incl. `expand_fills_remaining_row` |

hanabi is the same in places (#29 placeholder → `with_placeholder` exists; #23
virtualization → shipped; #18 "no flex-grow" → `expand()` is tested), though it
self-marks several RESOLVED already.

They are maintaining app-local `split_panel.h`, `tree_view.h`, `menu_setup.h`
and `context_menu.h` against gaps that closed. Worth a pass marking each shipped
item with the API to migrate to.

---

### Done this session, for reference

- afterhours `e8c1e18` — scroll-view child positions (the repositioning pass
  double-counted margins and existed twice, drifted); cross-axis `expand()`
  resolving to 0; `FlexWrap` doc said Wrap was the default, it is NoWrap.
- afterhours `49d5b74` — `with_corner_radius(px)`, and `with_roundness(r > 1)`
  now warns once per value.
- wm `cfad38d` — 8 stale baselines refreshed, 11 missing ones added.
- wm `bd7cb64`, `ee388bc` — mock contact sheets; fixed two mock bugs that faked
  ~95% of the reported diffs (a 1px `border` on every node accumulating with
  depth, and absolute nodes placed with screen-space coords as CSS `left/top`).
- wm `25c597e` — six screens covering the previously untested primitives.

---

### What's next

1. ~~Silent-failure audit~~ — done, item 21.
2. **Item 7 remainder** — sync scroll views, scroll anchoring, headless
   affordances, Margin/Padding single-side helpers. The e2e parts are done
   (item 23).
3. **Silent-failure audit** — now that `warn_once` exists, sweep the library
   for the other places that quietly do nothing. Three "missing features" this
   week were really silent fallbacks.
2. **Item 7's e2e command pack** — upstream wm's `disable_animations`,
   `--screenshot-dir`, `--e2e-speed`, case-insensitive `expect_text`.
   Mechanical; wm has all four already and cartographer and kart both asked.
3. **Items 13, 14, 15**
4. **Scrollbar dragging** — the half of item 3 left undone. — the last 40 real sweep candidates, thin and
   individual now.

Blocked on you: **item 1** (which weight-variant font files to ship) and
**item 2** (someone to run `alpha_blend_repro` from floatinghotel).

Current sweep state: **18 screens / 84 origins**, of which 44 are text-metric
(a known mock limitation) and **40 are real candidates**. Started at 44 screens
/ 383 origins. Regenerate with `./mocks/build.sh --no-serve && ./mocks/sheets.sh`.

Cleared since: the cross-axis align fix (item 12, -31 across 7 screens), the
over-padding rule extended to ScreenPercent (`islands_trains_settings` 47 -> 7),
and cross-axis Expand no longer setting flex-grow in the mock
(`deadspace_settings` 9 -> 0).

A subagent triage pass on items 12-15 produced only hypotheses and reversed
itself on two of them (it asserted the Family E border theory at high
confidence after it had already been disproved). Nothing from it is recorded
here that was not independently verified.

---

## 2026-08-29 session — the showcase quality pass

Filed by Gabe as rapid-fire TODOs while agents worked in parallel, plus what
the work turned up. Status here is what was *measured*, not what was claimed:
several agent reports of "fixed" and "not broken" were re-checked by hand and
three of my own regression calls did not survive measurement.

### Done and verified

| Item | Commit |
|---|---|
| checkboxes screen can't interact | `26fb22f` |
| sliders on setting row don't work | `9b5e11a` |
| pagination page buttons don't work | `c2ee76c` |
| split pane bounces at its bounds | `fe456b1` |
| panel styles filled background doesn't show the 9-slice | `5252d8d` |
| double width / double ornate panels wrong | `fde8fd8` |
| example borders unreadable and overlapping | `bef62e6`, `fd75bc3` |
| sidebar to click between screens | `b6713ce` |
| `drag_to` never dragged (no script used it, so nobody noticed) | `50ff29a` |
| composite widgets dropped the caller's custom colours | `04eb4cc` |
| toggle_switch drew its pill outside its own rect | `70cd11c` |

### Partly done

- **decorative frames** — geometry fixed (`b2e6ef9`), still a sparse screen
  with a dead bottom third. Three 130px frames in a 1280x720 canvas.
- **modals look better** — dialog titles only (`5bbc08a`). The broader visual
  work has not started.
- **app mockups** — backup, chat, media grid and VPN landed. Both new ones
  still print debug readouts ("18 of the 24 entries have no widgets at all
  right now", "toggle=off link=Not connected dialing=0.00s"), which cuts
  against the "make them feel real" ask.
- **screen ordering** — the navigator groups by category, so no renaming is
  needed. But six near-duplicate category strings hold ~10 screens:
  `Components` (3) vs `Component Galleries` (34), plus singleton `Widgets`,
  `Text`, `Text Editing`, `Bug Reports`. Ten one-word edits, zero pixels moved.

### Reported not broken — needs your eyes, not more agent time

An agent drove each of these with injected input and found them responsive:
scroll-in-scroll (inner thumb measured 239px against 242 expected), context
menu lab, composer lab, real-world modals / side drawer / bottom sheet.

Either headless does not reproduce what you saw, or the injector passes where a
human cannot. That second possibility is the harness gap below, so do not treat
these as closed until you have clicked them yourself.

### Diagnosed, not fixed

**Focus rings on `self_align`.** `UIComponent::focus_rect()` insets by its
offset, so the ring draws *inside* the element, which matches "too small".
Measured: only the left edge (20px of 48) and the bottom (156px of 184) render,
in colour (193,195,196), and only on the 1px falling outside the button. No top
or right. Draw order says the ring should be on top (background at
`rendering.h:1521`, ring at `:1613`), so the obvious covered-by-fill theory does
not hold and the real cause is still open.

### Not started

Popover lab opens its commit options by default and is ugly. Anchored menus
render text outside their bounds. The layout bug repros screen needs a lot of
work. Config options has contrast and overlap problems. Mini motorways toggles
are ugly. Checkboxes are ugly, separate from the click fix. Horizontal drag
enlarges and clips its label when picked up, and wants a ghosting toggle to
feel nice. Game screens should be interactive enough to tell a real control
from a painted one. More screens in the shape of hover_lab. Add juice / jazz
them up. Guess who. File tree wants the synthetic-vs-root toggle (decided, not
built). Entity index lab should be reframed as a real UI demo. drag_drop wants
consolidating with its sibling. Theme swatches / podcast UI. Language demo
wants a resize screen with icons repositioning per language, and a thicker
Japanese font.

### Infrastructure problems found this session

These are the reason bugs survived a green suite, so they rank above most of
the list above.

1. **Assert-free e2e scripts.** An agent counted 23 that perform a click, key
   or drag and then assert nothing, ending at a screenshot. That is why three
   genuinely dead widgets sat behind a green 113/113, and the list included
   exactly the widgets reported broken. A crude re-count by last-line flagged
   far more, so the real number needs a careful pass rather than a heuristic.
2. **The 1% baseline threshold hides small-widget geometry bugs.** The toggle
   overflow fixed in `70cd11c` moved ~0.16% of the frame, so validate passed
   with the broken render sitting in the committed baseline.
3. **`src/screen_includes.gen` is shared.** One agent's `make SCREEN=X`
   rewrites it, so the next agent's full build silently renders one screen. It
   cost the borders agent three build cycles and produced a screenshot set of 1.
   Wants a per-`OUTPUT_DIR` path.
4. **`file_tree` renders the live working directory**, so every agent's
   `output-*` dir changes that screen. Passing on tolerance today, will drift.
5. **The navigator's interaction has no coverage.** It is registered only in
   `run_screen_demo`, which needs a display and exits rc=1 headless, so click
   and scroll are compile-verified and reasoned, never exercised. This is the
   same shape as problem 1.
6. **Headless renders very few frames** (2 at dt=0.016), so time-based easing
   can never settle before capture. Seed animated state at its resting value
   and put motion behind interaction.

### Library rough edges worked around, not fixed

- `text_input` inherits the *current screen's* theme, so persistent chrome over
  themed content changes colour per screen. The navigator pins explicit colours.
- Swapping screens from a click handler is a use-after-free: `load_screen`
  frees the system `ScreenCyclerSystem` is still iterating. The navigator
  records the index and drains it after `systems.run`.
- `gen_first_enforce` (`entity_query.h:485-491`) logs an error on empty and
  then still returns `values[0]` on the empty vector.
- `toggle_switch` sizes its label from `parent_width - track_w`; the padding
  fix landed but the underlying "compute width by hand to dodge `expand()`
  resolution" workaround is still there.

### Method note

Measure, do not eyeball. Upscaled NEAREST crops produced three wrong regression
calls this session, and a fourth suspicion (that clicking a checkbox label
would strand keyboard focus) died to two probe scripts. Contrast claims should
carry a computed ratio: the transparent-centre border labels went 1.29:1 ->
15.37:1, and that number is the whole argument.

### Polish pass results, 2026-08-29

Screens with layout warnings went 38 -> 28, and the total from 300+ to 112.
What is left is mostly deliberate: `flex_alignment`'s 40 come from the hidden
tab content, `text_overflow` exists to overflow, `config_gap_gallery` has a
clipped row on purpose. `decorators`, `nine_slice_borders`, `parcel_corps` and
`cards` still have a few real ones.

Reworked: the five dialogs (shared mock-app backdrop), checkboxes, dropdowns,
buttons, button_variants, context_menu_lab, deadspace_settings,
decorative_frame, entity_index_lab, popover_lab, file_tree, minesweeper_lab,
flex_alignment, powerwash_settings, animation_spring, animation_declarative,
composer_lab, drag_drop, cozy_cafe, media_library, secure_tunnel,
offsite_backup, layout_bug_repros, casual_settings, decorators, layout,
layout_patterns, toggle_switches, neon_strike, cards, forms, guess_who_lab,
mini_motorways_settings.

#### Things worth remembering

- **Both baseline comparers scored the wrong thing.** Total colour energy over
  a theoretical maximum, so 1% needed roughly a hundredth of the frame to flip
  black to white. Now the share of pixels differing by more than 8. This is why
  broken renders kept passing.
- **The renderer already logs every clipped text and layout overflow.** Render
  one screen and count `LOG_WARN`. That scan found more real defects than
  reading screenshots did, and it caught a regression I introduced.
- **Most defects were arithmetic, not design.** A container a few pixels short
  of children it already had: 80px boxes in a 68px row, a 26px face in a 34px
  box once `text_inset` takes 5px a side, a last child pushed out by its own
  top margin.
- **`mk(parent)` with no index reuses one id per call site.** Build rows in a
  loop with it and they all collapse onto one entity.
- **Headless renders about two frames.** Anything easing from zero is captured
  mid-flight. Seed animated state at its resting value.
- **`git add` is case-insensitive on this filesystem but git is not.** Staging
  `DeadspaceSettings.h` against a tracked `DeadSpaceSettings.h` silently commits
  nothing. Third time this has bitten in this repo.
- **`layout` renders two different ways run to run** in the e2e path while
  being stable in the full-render path. Unexplained; treat that baseline as
  suspect.
- **Screenshot review by subagent mostly did not work.** Four spawned, one
  delivered. The one that did was worth it and its findings were accurate.
