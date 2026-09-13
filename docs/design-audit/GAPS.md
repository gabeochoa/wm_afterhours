# Baseline design gaps

Audit of the committed 1280×720 screenshots on 2026-09-12. The review covers visible composition, typography, controls, artwork and the clarity of test demonstrations. Static captures cannot establish hover behavior, keyboard support, animation quality or performance.

117 current screens, 32 additional captures, 2531 findings. 58 are classified as visible defects and 2473 as design proposals. The additional captures contribute 4 more proposals. 58 screens reach the requested 25 findings. Sparse screens have fewer where further findings would repeat an existing issue or invent a problem.

Entries retain the original review evidence; completion is tracked in [todo.md](../../todo.md). Ownership starts in wm; none establishes a missing afterhours API. Preserve intentional test fixtures. A proposal is a possible improvement, not proof of broken behavior. P1 means unreadable or obscured content, P2 affects ordinary reading or comparison, and P3 is polish or added explanation.

[Open the visual review](../../design-audit.html) · [Existing product mocks](../../mocks.html) · [Main gaps file](../AFTERHOURS_GAPS.md)

## Coverage

| Screen | Findings | Defects | Web mock |
|---|---:|---:|---|
| [absolute_positioning](#absolute-positioning) | 25 | 2 | not authored |
| [accessibility](#accessibility) | 25 | 0 | not authored |
| [adaptive_scaling](#adaptive-scaling) | 25 | 0 | not authored |
| [advanced_modals](#advanced-modals) | 16 | 0 | not authored |
| [aim_chat](#aim-chat) | 25 | 0 | yes |
| [alpha_blend_repro](#alpha-blend-repro) | 20 | 0 | not authored |
| [angry_birds_settings](#angry-birds-settings) | 25 | 0 | yes |
| [animation_basic](#animation-basic) | 18 | 0 | not authored |
| [animation_declarative](#animation-declarative) | 25 | 2 | not authored |
| [animation_interactive](#animation-interactive) | 18 | 0 | not authored |
| [animation_looping](#animation-looping) | 20 | 0 | not authored |
| [animation_spring](#animation-spring) | 20 | 0 | not authored |
| [auto_text_color](#auto-text-color) | 25 | 0 | not authored |
| [bevel_borders](#bevel-borders) | 18 | 0 | not authored |
| [blend_mode_lab](#blend-mode-lab) | 25 | 0 | not authored |
| [button_variants](#button-variants) | 25 | 0 | not authored |
| [buttons](#buttons) | 25 | 0 | not authored |
| [cards](#cards) | 25 | 2 | not authored |
| [casual_settings](#casual-settings) | 25 | 0 | yes |
| [chart_lab](#chart-lab) | 25 | 1 | not authored |
| [checkboxes](#checkboxes) | 25 | 2 | not authored |
| [circular_progress](#circular-progress) | 25 | 0 | not authored |
| [colors](#colors) | 25 | 0 | not authored |
| [composer_lab](#composer-lab) | 25 | 0 | not authored |
| [config_gap_gallery](#config-gap-gallery) | 25 | 0 | not authored |
| [context_menu_lab](#context-menu-lab) | 25 | 1 | not authored |
| [cozy_cafe](#cozy-cafe) | 25 | 0 | yes |
| [deadspace_settings](#deadspace-settings) | 25 | 0 | yes |
| [decorative_frame](#decorative-frame) | 20 | 0 | not authored |
| [decorators](#decorators) | 25 | 2 | not authored |
| [dialog_confirm](#dialog-confirm) | 18 | 0 | not authored |
| [dialog_danger](#dialog-danger) | 18 | 0 | not authored |
| [dialog_fyi](#dialog-fyi) | 18 | 0 | not authored |
| [dialog_info](#dialog-info) | 16 | 0 | not authored |
| [dialog_prompt](#dialog-prompt) | 20 | 0 | not authored |
| [divider_lab](#divider-lab) | 22 | 1 | not authored |
| [drag_drop](#drag-drop) | 25 | 1 | not authored |
| [dropdowns](#dropdowns) | 25 | 0 | not authored |
| [empire_tycoon](#empire-tycoon) | 25 | 0 | yes |
| [entity_index_lab](#entity-index-lab) | 25 | 0 | not authored |
| [example_borders](#example-borders) | 25 | 0 | not authored |
| [fighter_menu](#fighter-menu) | 25 | 0 | yes |
| [file_tree](#file-tree) | 25 | 0 | not authored |
| [fit_content_lab](#fit-content-lab) | 16 | 0 | not authored |
| [flex_alignment](#flex-alignment) | 25 | 0 | not authored |
| [flight_options](#flight-options) | 25 | 0 | yes |
| [font_weight](#font-weight) | 16 | 0 | not authored |
| [forms](#forms) | 25 | 0 | not authored |
| [grid_lab](#grid-lab) | 18 | 0 | not authored |
| [guess_who_lab](#guess-who-lab) | 25 | 2 | yes |
| [horizontal_drag](#horizontal-drag) | 20 | 0 | not authored |
| [hover_lab](#hover-lab) | 18 | 0 | not authored |
| [hstack_showcase](#hstack-showcase) | 25 | 0 | not authored |
| [images](#images) | 25 | 1 | not authored |
| [islands_trains_settings](#islands-trains-settings) | 25 | 0 | yes |
| [kart_select](#kart-select) | 25 | 0 | yes |
| [kirby_options](#kirby-options) | 25 | 0 | yes |
| [language_demo](#language-demo) | 20 | 0 | not authored |
| [layout](#layout) | 16 | 0 | not authored |
| [layout_bug_repros](#layout-bug-repros) | 25 | 1 | not authored |
| [layout_patterns](#layout-patterns) | 10 | 0 | not authored |
| [marlo_kart](#marlo-kart) | 25 | 0 | yes |
| [media_library](#media-library) | 25 | 0 | yes |
| [menu_showcase](#menu-showcase) | 18 | 0 | not authored |
| [meters_gauges](#meters-gauges) | 25 | 1 | not authored |
| [minesweeper_lab](#minesweeper-lab) | 25 | 0 | yes |
| [mini_motorways_settings](#mini-motorways-settings) | 25 | 0 | yes |
| [modals](#modals) | 25 | 0 | not authored |
| [multiline_text_lab](#multiline-text-lab) | 25 | 0 | not authored |
| [navigation_bar_demo](#navigation-bar-demo) | 16 | 0 | not authored |
| [neon_strike](#neon-strike) | 25 | 0 | yes |
| [nine_slice_borders](#nine-slice-borders) | 25 | 0 | not authored |
| [offsite_backup](#offsite-backup) | 25 | 1 | yes |
| [pagination](#pagination) | 20 | 0 | not authored |
| [parcel_corps_settings](#parcel-corps-settings) | 25 | 0 | yes |
| [pinch_zoom_lab](#pinch-zoom-lab) | 15 | 0 | not authored |
| [popover_lab](#popover-lab) | 25 | 0 | not authored |
| [potion_crafting](#potion-crafting) | 25 | 0 | yes |
| [powerwash_settings](#powerwash-settings) | 28 | 4 | yes |
| [race_results](#race-results) | 28 | 4 | yes |
| [radio_buttons](#radio-buttons) | 18 | 0 | not authored |
| [real_world_modals](#real-world-modals) | 16 | 1 | not authored |
| [rect_algebra_lab](#rect-algebra-lab) | 20 | 0 | not authored |
| [rubber_bandits_menu](#rubber-bandits-menu) | 27 | 2 | yes |
| [scroll_click_bug](#scroll-click-bug) | 15 | 0 | not authored |
| [scroll_clip_bug](#scroll-clip-bug) | 16 | 0 | not authored |
| [scroll_view](#scroll-view) | 19 | 2 | not authored |
| [scrollbar_style_lab](#scrollbar-style-lab) | 16 | 0 | not authored |
| [secure_tunnel](#secure-tunnel) | 28 | 3 | yes |
| [self_align](#self-align) | 14 | 0 | not authored |
| [separators](#separators) | 17 | 0 | not authored |
| [setting_row_showcase](#setting-row-showcase) | 20 | 1 | not authored |
| [shop_interface](#shop-interface) | 28 | 5 | yes |
| [simple_button](#simple-button) | 16 | 0 | not authored |
| [split_pane_lab](#split-pane-lab) | 16 | 1 | not authored |
| [sports_settings](#sports-settings) | 28 | 1 | yes |
| [stepper_showcase](#stepper-showcase) | 18 | 0 | not authored |
| [styled_text_lab](#styled-text-lab) | 18 | 0 | not authored |
| [sync_scroll_lab](#sync-scroll-lab) | 18 | 0 | not authored |
| [system_profile_lab](#system-profile-lab) | 20 | 0 | not authored |
| [tab_container](#tab-container) | 18 | 0 | not authored |
| [tabbing](#tabbing) | 15 | 0 | not authored |
| [text](#text) | 20 | 0 | not authored |
| [text_input](#text-input) | 22 | 2 | not authored |
| [text_input_lab](#text-input-lab) | 16 | 0 | not authored |
| [text_input_sizes](#text-input-sizes) | 16 | 1 | not authored |
| [text_overflow](#text-overflow) | 20 | 0 | not authored |
| [text_shadow](#text-shadow) | 19 | 2 | not authored |
| [text_stroke](#text-stroke) | 20 | 1 | not authored |
| [themes](#themes) | 22 | 0 | not authored |
| [toasts](#toasts) | 20 | 5 | not authored |
| [toggle_switches](#toggle-switches) | 20 | 0 | not authored |
| [tooltip_lab](#tooltip-lab) | 16 | 0 | not authored |
| [tray](#tray) | 16 | 0 | not authored |
| [virtual_list_lab](#virtual-list-lab) | 17 | 0 | not authored |
| [vstack_showcase](#vstack-showcase) | 22 | 1 | not authored |
| [widget_gap_gallery](#widget-gap-gallery) | 23 | 2 | not authored |

## absolute positioning

[Reviewed image](../../docs/design-audit/originals/absolute_positioning_720p.png) · [Current baseline](../../screenshot-baselines/screens/absolute_positioning_720p.png) · [Visual review](../../design-audit.html#screen=absolute_positioning)

Baseline SHA-256: `5d431252bcb87f1bbd4a2a4d4f40cab3615323fec57c7e2661ae43107e3d87c3`

Four positioning fixtures expose boundary and text-placement problems. Preserve absolute positioning examples; surrounding measurement aids are proposals.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| absolute_positioning-01 | P1 | defect | Translate example: Pink translate(300,10) overlaps the green label's final characters. | Size the samples so both complete coordinate labels remain visible. |
| absolute_positioning-02 | P2 | defect | Corner-pinning right edge: TR and BR extend beyond the inset demo well. | Compute right anchors from the actual measured well width. |
| absolute_positioning-03 | P2 | proposal | Corner-pinning labels: TL, TR, BL and BR require interpreting abbreviations. | Add an external legend spelling out the four corners. |
| absolute_positioning-04 | P2 | proposal | Corner-pinning bounds: The well's right boundary disappears behind the corner blocks. | Outline the reference rectangle independently of its children. |
| absolute_positioning-05 | P2 | proposal | Corner-pinning geometry: Corner samples look wider than tall. | Print measured width and height beneath the reference rectangle. |
| absolute_positioning-06 | P2 | proposal | Corner-pinning spacing: The two left samples nearly fill the short well vertically. | Increase the well height to make top versus bottom anchoring clearer. |
| absolute_positioning-07 | P2 | proposal | Corner-pinning origin: The top-left block has no marked anchor point. | Draw a small crosshair at the container origin. |
| absolute_positioning-08 | P2 | proposal | Translate reference: The green and pink boxes have no origin or grid. | Add reference ticks at x=0,100,300 outside the specimens. |
| absolute_positioning-09 | P2 | proposal | Translate y offsets: Both translate labels show 10 without a visible vertical measurement. | Add a 10-pixel offset bracket at the left edge. |
| absolute_positioning-10 | P2 | proposal | Translate labels: Coordinate text uses the handwritten display face. | Use a compact monospace face for numeric coordinates. |
| absolute_positioning-11 | P2 | proposal | Title: The cyan title starts flush against its rounded panel edge. | Inset the title text to align with the section headings. |
| absolute_positioning-12 | P2 | proposal | Section headings: Four long technical headings are visually similar to specimen labels. | Increase heading weight while keeping specimen text smaller. |
| absolute_positioning-13 | P2 | proposal | Section numbering: Numbers share the same baseline and weight as full headings. | Use a narrow numbered gutter for faster section scanning. |
| absolute_positioning-14 | P2 | proposal | Correct-pattern caption: The first heading calls the pattern correct without stating the rule. | Add a short note that offsets are relative to the demo container. |
| absolute_positioning-15 | P2 | proposal | Flow labels: Flow A, B and C sit directly on the left edges of their blocks. | Add consistent internal label padding. |
| absolute_positioning-16 | P2 | proposal | Flow spacing: Large gaps between Flow A, B and C have no measurement cues. | Annotate the space-around gaps below the row. |
| absolute_positioning-17 | P2 | proposal | Overlay badge: BADGE is left aligned within a wide pink pill. | Center the badge label with equal horizontal inset. |
| absolute_positioning-18 | P2 | proposal | Overlay relationship: The badge crosses Flow C and the well without explaining its anchor. | Add a leader from the badge to its container anchor. |
| absolute_positioning-19 | P2 | proposal | Overlay layer: The overlay example does not identify its stacking order. | Label the badge layer outside the colored blocks. |
| absolute_positioning-20 | P2 | proposal | Percentage samples: 30% and 20% blocks describe screen sizing but omit reference width. | Display viewport width beside the section heading. |
| absolute_positioning-21 | P2 | proposal | Percentage heights: Both blocks show 8% without an absolute height comparison. | Add computed height text below the specimens. |
| absolute_positioning-22 | P2 | proposal | Percentage labels: Text begins at each block's left boundary. | Inset percentage labels by a consistent small padding. |
| absolute_positioning-23 | P2 | proposal | Percentage reference: There is no visual scale for the unused half of screen width. | Add a thin 0–100% ruler outside the samples. |
| absolute_positioning-24 | P2 | proposal | Bottom space: A large empty band follows the fourth fixture. | Use it for a compact coordinate-system legend. |
| absolute_positioning-25 | P2 | proposal | Demo wells: Inset wells and outer section panels are close in tone. | Add subtle well borders so measured container boundaries remain visible. |

## accessibility

[Reviewed image](../../docs/design-audit/originals/accessibility_720p.png) · [Current baseline](../../screenshot-baselines/screens/accessibility_720p.png) · [Visual review](../../design-audit.html#screen=accessibility)

Baseline SHA-256: `f27180479a4b3c01ce4b29d9c698c8396e21114d8db2cf9bfecc393689a11a31`

The screenshot demonstrates readable automatic and manual contrast. Recommendations improve evidence and scope of the claims rather than treating intentionally varied swatches as defects.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| accessibility-01 | P2 | proposal | WCAG badge: Theme Passes WCAG AA does not name the checked color pairs. | Add the number of validated theme pairs under the badge. |
| accessibility-02 | P2 | proposal | WCAG badge scope: The badge reads like approval of the entire interface. | Rename it to Theme color pairs pass AA contrast. |
| accessibility-03 | P2 | proposal | WCAG threshold: No required contrast ratio appears beside the badge. | Show the threshold used for normal text. |
| accessibility-04 | P2 | proposal | Manual column heading: Without Automatic Contrast describes implementation only. | Add a subtitle saying text colors were chosen manually. |
| accessibility-05 | P2 | proposal | Automatic column heading: With Automatic Contrast offers no selection rule. | Add a short dark-or-light text explanation. |
| accessibility-06 | P2 | proposal | Dark manual sample: Works Fine is vague compared with the neighboring labels. | Use Manual light text for parallel labeling. |
| accessibility-07 | P2 | proposal | Dark sample boundary: The dark manual tile blends into the surrounding panel. | Add a neutral outline outside the test color area. |
| accessibility-08 | P2 | proposal | Dark automatic boundary: The dark automatic tile also lacks a clear extent. | Outline its bounds with the same reference stroke. |
| accessibility-09 | P2 | proposal | Light pair: The two light samples look identical without a result annotation. | Show chosen foreground values below each tile. |
| accessibility-10 | P2 | proposal | Manual note: Manual text color applied is detached from its two samples. | Place it in the manual column footer with consistent inset. |
| accessibility-11 | P2 | proposal | Automatic note: Automatically picks best contrast uses a bright accent unlike the manual footer. | Give both explanatory footers equal typographic emphasis. |
| accessibility-12 | P2 | proposal | Swatch heading: Various Backgrounds offers no description of the two rows. | Label the rows Light backgrounds and Dark backgrounds. |
| accessibility-13 | P2 | proposal | Swatch identity: All sixteen tiles have the same Auto label. | Give each tile a compact background color identifier. |
| accessibility-14 | P2 | proposal | Swatch foreground: Black or white text choices are visible but unnamed. | Add the chosen foreground value to each swatch caption. |
| accessibility-15 | P2 | proposal | Swatch ratios: No tile exposes its measured contrast. | Show a ratio beneath each test tile. |
| accessibility-16 | P2 | proposal | Swatch ordering: The bright row changes hue without a visible sorting principle. | Group or order samples by luminance and label the order. |
| accessibility-17 | P2 | proposal | Dark row pairing: Dark swatches do not visually establish pairs with the row above. | Add column labels if pairs are intended. |
| accessibility-18 | P2 | proposal | Swatch gutters: Horizontal gaps are much smaller than the gap separating the comparison columns. | Use a clearer equal grid gutter around the swatch set. |
| accessibility-19 | P2 | proposal | Swatch type: Small widely tracked lettering makes color comparisons harder to read. | Use a conventional text face inside the test swatches. |
| accessibility-20 | P2 | proposal | Title style: The title uses a different face from the rest of the panel. | Use one readable family with weight differences for hierarchy. |
| accessibility-21 | P2 | proposal | Footer claim: Text always remains readable is an absolute statement. | Limit the wording to the tested foreground choices. |
| accessibility-22 | P2 | proposal | Footer placement: The footer sits well below the swatches with a large blank gap. | Move the explanation closer to the grid. |
| accessibility-23 | P2 | proposal | Separators: Both divider lines are inset farther than the column content. | Align separator ends to the content grid. |
| accessibility-24 | P2 | proposal | Theme identity: The panel does not name the theme being validated. | Add the theme name alongside validation status. |
| accessibility-25 | P2 | proposal | Test context: The demonstration gives no sample font size near its AA claim. | Display the text size used in the contrast specimens. |

## adaptive scaling

[Reviewed image](../../docs/design-audit/originals/adaptive_scaling_720p.png) · [Current baseline](../../screenshot-baselines/screens/adaptive_scaling_720p.png) · [Visual review](../../design-audit.html#screen=adaptive_scaling)

Baseline SHA-256: `a44d5a368be19a39854736506d6af2265b579ebbeb07a5a5a3b55b6dfcfd16e3`

The active wide 100% state is legible, but small diagnostic text and unmeasured examples make scaling difficult to evaluate.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| adaptive_scaling-01 | P2 | proposal | Logical dimensions: Logical: 1280x720 is tiny beside the scale controls. | Increase diagnostic text to a readable body size. |
| adaptive_scaling-02 | P2 | proposal | Header status: Adaptive 100% repeats the mode and scale controls. | Use the header slot for physical viewport dimensions. |
| adaptive_scaling-03 | P2 | proposal | Mode button: Mode: Adaptive does not signal that another mode exists. | Add a change-mode indicator and name the alternate mode in helper text. |
| adaptive_scaling-04 | P2 | proposal | Scale decrement: The minus button contains no scale-step hint. | Label the scale control group with its increment. |
| adaptive_scaling-05 | P2 | proposal | Scale readout: 100% is styled much like its neighboring buttons. | Give the numeric readout a distinct non-action field treatment. |
| adaptive_scaling-06 | P2 | proposal | Pixel card description: This card is 280x160 px is extremely small. | Increase its text size and align it to the card heading. |
| adaptive_scaling-07 | P2 | proposal | Pixel color squares: Four colored blocks have no dimensions shown. | Add width-by-height captions outside each square. |
| adaptive_scaling-08 | P2 | proposal | Pixel square comparison: Color is the only distinction among identical-size samples. | Label their intended comparison explicitly. |
| adaptive_scaling-09 | P2 | proposal | Pixel buttons: Button A and Button B do not identify their sizing rule. | Use labels that include their fixed pixel dimensions. |
| adaptive_scaling-10 | P2 | proposal | Button widths: Both pixel buttons span nearly the full card. | Show a measured width annotation below the pair. |
| adaptive_scaling-11 | P2 | proposal | Padding card: Padding: 14px all sides lacks visible dimension markers. | Add four small padding brackets around Inner content area. |
| adaptive_scaling-12 | P2 | proposal | Padding inner text: Inner content area sits against the dark rectangle's left edge. | Inset its label without changing the measured outer padding. |
| adaptive_scaling-13 | P2 | proposal | Expand heading: Expand & Flex does not define the 1x, 2x, 3x ratio. | Add a brief ratio caption above the colored segments. |
| adaptive_scaling-14 | P2 | proposal | Expand segment boundaries: Red, amber and green segments touch directly. | Add thin neutral dividers that preserve the measured flex widths. |
| adaptive_scaling-15 | P2 | proposal | Expand numeric labels: 1x, 2x and 3x are small in tall blocks. | Increase their size for distance comparison. |
| adaptive_scaling-16 | P2 | proposal | Expand explanation: The long explanation is tiny and widely tracked. | Use larger text and a shorter two-line explanation. |
| adaptive_scaling-17 | P2 | proposal | Pixel comparison heading: screen_pct vs pixels omits the h720 sample shown below. | Name all three sizing systems in the heading. |
| adaptive_scaling-18 | P2 | proposal | px specimen: px omits its configured magnitude. | Label the specimen with its numeric pixel width. |
| adaptive_scaling-19 | P2 | proposal | h720 specimen: h720 is unexplained technical shorthand. | Show its configured value and reference height in a caption. |
| adaptive_scaling-20 | P2 | proposal | scr% specimen: scr% does not state its percentage. | Show the exact screen percentage outside the block. |
| adaptive_scaling-21 | P2 | proposal | Comparison alignment: Three unequal specimens provide no common ruler. | Add a horizontal pixel scale beneath them. |
| adaptive_scaling-22 | P2 | proposal | Breakpoint chips: narrow, medium, wide and short mix width and height categories. | Separate width breakpoints from height breakpoints. |
| adaptive_scaling-23 | P2 | proposal | Active wide chip: The wide state is communicated mainly by a blue fill. | Add a checkmark or Active suffix. |
| adaptive_scaling-24 | P2 | proposal | Breakpoint thresholds: No width or height thresholds appear beside the chips. | Show the numeric threshold for each breakpoint. |
| adaptive_scaling-25 | P2 | proposal | Empty lower area: Most of the bottom fifth is unused. | Add a compact before-and-after size table for the visible examples. |

## advanced modals

[Reviewed image](../../docs/design-audit/originals/advanced_modals_720p.png) · [Current baseline](../../screenshot-baselines/screens/advanced_modals_720p.png) · [Visual review](../../design-audit.html#screen=advanced_modals)

Baseline SHA-256: `de1d0ba753e60e3640e31973637edbe78b25c09823a213873115e97c5f86029c`

Only the launcher is captured. Sixteen defensible improvements address its sparse composition and discoverability; modal interiors and behavior are outside this screenshot's evidence.

Current example states: [Current example: login validation](../../docs/design-audit/examples/advanced_modals_login_validation_720p.png) · [Current example: wizard setup](../../docs/design-audit/examples/advanced_modals_wizard_step1_720p.png) · [Current example: settings](../../docs/design-audit/examples/advanced_modals_settings_open_720p.png) · [Current example: feedback](../../docs/design-audit/examples/advanced_modals_feedback_open_720p.png)

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| advanced_modals-01 | P2 | proposal | Title bar: Advanced Modals starts directly at the blue strip's left edge. | Add consistent text inset. |
| advanced_modals-02 | P2 | proposal | Title hierarchy: The heading occupies a full-width strip despite little supporting content. | Add one sentence describing the four examples beneath it. |
| advanced_modals-03 | P2 | proposal | Outer canvas: A black border surrounds the navy page with uneven apparent margins. | Center the page within equal outer margins. |
| advanced_modals-04 | P2 | proposal | Header gap: A large empty gap separates the title from the first launcher row. | Reduce it to the normal section spacing. |
| advanced_modals-05 | P2 | proposal | Row gap: The two button rows are separated by about a button-height of empty space. | Use a compact two-column card grid. |
| advanced_modals-06 | P2 | proposal | Login launcher: Login Form offers no hint of the demonstrated validation. | Add a caption identifying required-field validation. |
| advanced_modals-07 | P2 | proposal | Wizard launcher: Multi-Step Wizard gives no visible step count. | Show a Three-step setup caption. |
| advanced_modals-08 | P2 | proposal | Settings launcher: Settings Panel does not identify its control types. | Add a short caption naming the visible settings categories. |
| advanced_modals-09 | P2 | proposal | Feedback launcher: Feedback Form gives no indication of its example purpose. | Add a brief rating-and-comment caption. |
| advanced_modals-10 | P2 | proposal | Launcher colors: Blue, ochre, green and dusty red assign unrelated meanings. | Use a shared action style with small category accents. |
| advanced_modals-11 | P2 | proposal | Button shape: Square buttons sit inside strongly rounded row panels. | Choose a deliberate shared corner system for launchers and panels. |
| advanced_modals-12 | P2 | proposal | Button balance: Very wide gaps separate paired actions. | Bring each button and its description into a single card. |
| advanced_modals-13 | P2 | proposal | Launch verbs: All four labels are nouns. | Prefix them with Open to clarify the immediate action. |
| advanced_modals-14 | P2 | proposal | Status instruction: Open a modal above to get started is small inside a wide strip. | Increase the instruction size and align it to the grid. |
| advanced_modals-15 | P2 | proposal | Status strip inset: The instruction starts flush against its background edge. | Add horizontal padding equal to the title inset. |
| advanced_modals-16 | P2 | proposal | Default screenshot state: No modal specimen appears in the baseline. | Provide a separate clearly labeled example-open baseline alongside this launcher. |

## aim chat

[Reviewed image](../../docs/design-audit/originals/aim_chat_720p.png) · [Current baseline](../../screenshot-baselines/screens/aim_chat_720p.png) · [Visual review](../../design-audit.html#screen=aim_chat)

Baseline SHA-256: `8bb37a0d438af6d0320177bf5224f6efe12158ac3004a185514ad5de21cb847b`

The retro desktop is coherent and matches the mock's deliberate HTTP-log example. Proposals refine user-facing content, period-appropriate iconography and hierarchy.

Current example states: [Current example: developer log pane](../../docs/design-audit/examples/aim_chat_developer_log_720p.png)

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| aim_chat-01 | P2 | proposal | Conversation history: The mock's HTTP GET/POST example interrupts the conversation between messages and its timestamp. | Move the developer-log specimen into a separately labeled demo pane. |
| aim_chat-02 | P2 | proposal | Desktop computer icon: My Computer uses an empty square outline. | Replace it with a recognizable period-style computer bitmap. |
| aim_chat-03 | P2 | proposal | Desktop folder icon: My Documents uses an outlined parallelogram. | Use a folder icon consistent with the runner artwork. |
| aim_chat-04 | P2 | proposal | Window controls: Maximize is rendered as literal [] text. | Draw a small square window glyph. |
| aim_chat-05 | P2 | proposal | Title minimize: The minimize character appears as text at the bottom of a tiny control. | Use a centered pixel-aligned minimize line. |
| aim_chat-06 | P2 | proposal | Menu rows: Each menu item looks like a raised command button. | Use flat menu labels until selected, matching the mock's menu bar. |
| aim_chat-07 | P2 | proposal | Active window: Both title bars use equally vivid blue gradients. | Give the inactive buddy window a subdued title bar. |
| aim_chat-08 | P2 | proposal | Buddy groups: v and > are used as disclosure markers. | Use consistent small triangle glyphs aligned in one column. |
| aim_chat-09 | P2 | proposal | Buddy count: Buddies (3/8) does not explain the denominator. | Use a compact online/total caption in the group header. |
| aim_chat-10 | P2 | proposal | Offline group: Offline (12) lacks the count format used above it. | Use one consistent count convention across groups. |
| aim_chat-11 | P2 | proposal | Buddy selection: SmarterChild's selected row starts farther right than group labels. | Retain indentation but add a small status icon column. |
| aim_chat-12 | P2 | proposal | Buddy presence: Unselected buddy names carry no individual presence marker. | Add compact online indicators beside each name. |
| aim_chat-13 | P2 | proposal | Buddy action labels: IM and Chat appear side by side without visible distinction. | Rename the group action to Chat room or clarify the distinction. |
| aim_chat-14 | P2 | proposal | Availability bar: I'm Available resembles a text field. | Add a small disclosure arrow to mark it as a status control. |
| aim_chat-15 | P2 | proposal | Buddy footer banner: The large AOL banner has more contrast than the buddy list header. | Reduce its height to return emphasis to contacts. |
| aim_chat-16 | P2 | proposal | Chat identity: SmarterChild is repeated in the title, profile header and first line. | Keep the profile name but reduce its red emphasis. |
| aim_chat-17 | P2 | proposal | Warning level: Warning level: 0% occupies the far edge of the profile header. | Align it with the presence row and reduce its visual priority. |
| aim_chat-18 | P2 | proposal | Message spacing: The three short chat lines have substantial vertical spacing. | Tighten line spacing to match a compact instant-message transcript. |
| aim_chat-19 | P2 | proposal | Timestamp: Last message received at 2: 02 PM is faint gray on white. | Darken the text enough to read at the existing small size. |
| aim_chat-20 | P2 | proposal | Formatting toolbar: A, B, I, U, A, :) and / have unevenly understandable meanings. | Use recognizable font, color, emoticon and link icons. |
| aim_chat-21 | P2 | proposal | Toolbar grouping: Text formatting and insertion actions form one undifferentiated row. | Insert narrow separators between formatting and insertion groups. |
| aim_chat-22 | P2 | proposal | Composer label: Send to: SmarterChild is much smaller than message text. | Increase the label slightly and align it with composer inset. |
| aim_chat-23 | P2 | proposal | Composer placeholder: Type a message is light gray within a large empty white area. | Increase placeholder contrast while keeping it secondary. |
| aim_chat-24 | P2 | proposal | Send action: Send has the same weight as Warn and Block. | Give Send the default-button outline and separate moderation actions. |
| aim_chat-25 | P2 | proposal | Connection footer: Direct Connection Established (local demo) is tiny at the bottom edge. | Use a readable status line with a small connection indicator. |

## alpha blend repro

[Reviewed image](../../docs/design-audit/originals/alpha_blend_repro_720p.png) · [Current baseline](../../screenshot-baselines/screens/alpha_blend_repro_720p.png) · [Visual review](../../design-audit.html#screen=alpha_blend_repro)

Baseline SHA-256: `f3a03b7ef25b26fbf8f6ee230b471a13b1addd557845f10c811028254ac6a2e9`

Fading and disappearance under increasingly opaque overlays are intentional. Twenty presentation improvements are defensible; no blend failure is inferred from these fixtures.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| alpha_blend_repro-01 | P2 | proposal | Title claim: The heading says text should stay readable through the wash, including opaque rows. | Qualify the expectation by opacity range. |
| alpha_blend_repro-02 | P2 | proposal | First section heading: The full with_custom_background expression is tiny. | Use a readable monospace caption above the fixture. |
| alpha_blend_repro-03 | P2 | proposal | Second section heading: with_opacity appears in small handwritten lettering. | Use the same monospace heading treatment as the first section. |
| alpha_blend_repro-04 | P2 | proposal | Alpha units: 32 through 255 have no visible alpha-unit label. | Label the right column Alpha / 255. |
| alpha_blend_repro-05 | P2 | proposal | Opacity units: 15% through 100% lack a column heading. | Label that column Overlay opacity. |
| alpha_blend_repro-06 | P2 | proposal | Opaque alpha row: The final blue row has no visible specimen text, as expected. | Add an external Opaque reference annotation. |
| alpha_blend_repro-07 | P2 | proposal | Opaque opacity row: The 100% row is also intentionally blank. | Label it Expected complete occlusion outside the overlay. |
| alpha_blend_repro-08 | P2 | proposal | Unwashed reference: Every text sample is covered by some overlay. | Add one zero-opacity reference row. |
| alpha_blend_repro-09 | P2 | proposal | Underlying text: The pangram is printed only inside the tested wash. | Repeat the reference text outside the fixtures. |
| alpha_blend_repro-10 | P2 | proposal | Row labels: Numeric values are very small and far from the specimen start. | Increase their size and use aligned tabular numerals. |
| alpha_blend_repro-11 | P2 | proposal | Row padding: Pangram text starts flush with the wash's left boundary. | Add consistent text inset without changing the overlay extent. |
| alpha_blend_repro-12 | P2 | proposal | Row gaps: Several blue rows visually merge together. | Use equal narrow neutral gutters between all rows. |
| alpha_blend_repro-13 | P2 | proposal | Cross-method comparison: The alpha and opacity sections use different sample values. | Add one matched-value pair for direct comparison. |
| alpha_blend_repro-14 | P2 | proposal | Overlay color: The blue test color is named only in code syntax. | Add a small labeled RGB swatch outside the test region. |
| alpha_blend_repro-15 | P2 | proposal | Backing surface: The black background's role is not named. | Label the surface over which alpha is being composited. |
| alpha_blend_repro-16 | P2 | proposal | Expected result: The two methods have no nearby explanation of their expected relationship. | Add a one-line equivalence note. |
| alpha_blend_repro-17 | P2 | proposal | Text foreground: The source text color is not stated. | Show its foreground value beside the untouched reference. |
| alpha_blend_repro-18 | P2 | proposal | Section grouping: Two long stacks run almost edge to edge. | Place each stack in an outlined test region with a shared label gutter. |
| alpha_blend_repro-19 | P2 | proposal | Lower empty area: The lower quarter contains no interpretive aid. | Use a compact expected-versus-observed comparison legend. |
| alpha_blend_repro-20 | P2 | proposal | Capture context: The reproduction omits renderer information in the visible frame. | Add a small diagnostic footer with renderer and viewport. |

## angry birds settings

[Reviewed image](../../docs/design-audit/originals/angry_birds_settings_720p.png) · [Current baseline](../../screenshot-baselines/screens/angry_birds_settings_720p.png) · [Visual review](../../design-audit.html#screen=angry_birds_settings)

Baseline SHA-256: `d40ee477215dcc5d20e24691ede3c4b334571f1536ea3cb5f834115683cb2b12`

The baseline closely follows the approved playful settings mock. Findings are design refinements, not rendering failures.

Current example states: [Current example: Unsaved audio settings](../../docs/design-audit/examples/angry_birds_pending_720p.png) · [Current example: General settings](../../docs/design-audit/examples/angry_birds_general_720p.png) · [Current example: Language selection](../../docs/design-audit/examples/angry_birds_language_720p.png)

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| angry_birds_settings-01 | P2 | proposal | Audio selected tab: AUDIO relies on green fill to distinguish it from blue tabs. | Add a small selected underline or check marker. |
| angry_birds_settings-02 | P2 | proposal | Vibration state: The OFF icon looks disabled while MUSIC and SOUND look enabled. | Give off-but-interactive controls an explicit toggle marker. |
| angry_birds_settings-03 | P2 | proposal | State captions: ON and OFF are much smaller than MUSIC, SOUND and VIBRATION. | Increase state caption size and weight. |
| angry_birds_settings-04 | P2 | proposal | Sound wording: SOUND can include the separate music setting. | Rename it SOUND EFFECTS. |
| angry_birds_settings-05 | P2 | proposal | Vibration icon: The phone is tilted and flanked by marks at different angles. | Use a more balanced vibration glyph. |
| angry_birds_settings-06 | P2 | proposal | Music icon: The note occupies less width than the speaker icon. | Normalize icon optical size within all three buttons. |
| angry_birds_settings-07 | P2 | proposal | Close control: The red close button overlaps the outer board edge. | Bring it slightly inward to preserve a consistent safe margin. |
| angry_birds_settings-08 | P2 | proposal | Close emphasis: Close is the strongest red object on a non-destructive screen. | Soften the red or reduce its size relative to confirmation. |
| angry_birds_settings-09 | P2 | proposal | Footer action order: OK appears before CANCEL and APPLY in a right-aligned group. | Place the primary confirmation at the far right. |
| angry_birds_settings-10 | P2 | proposal | OK wording: OK does not explain whether settings save and close. | Use DONE or SAVE & CLOSE. |
| angry_birds_settings-11 | P2 | proposal | Apply action: APPLY looks active in the unchanged default state. | Visually distinguish its no-pending-changes state when applicable. |
| angry_birds_settings-12 | P2 | proposal | Cancel action: CANCEL has the same glossy emphasis as APPLY. | Use a quieter secondary treatment. |
| angry_birds_settings-13 | P2 | proposal | Footer spacing: The footer actions cluster on the right while the left half is empty. | Add a concise saved/unsaved status on the left. |
| angry_birds_settings-14 | P2 | proposal | Audio instruction: The tagline says little about the controls. | Add a brief Tap an icon to toggle caption. |
| angry_birds_settings-15 | P2 | proposal | Tab content association: The audio controls sit far below the active tab. | Reduce the gap between tab navigation and audio description. |
| angry_birds_settings-16 | P2 | proposal | Info tab wording: INFO is broad for credits, support and version details. | Use ABOUT or a more descriptive label. |
| angry_birds_settings-17 | P2 | proposal | Board size: The board nearly fills the available height. | Reduce outer ornament height to create breathing room. |
| angry_birds_settings-18 | P2 | proposal | Outer shadow: A heavy brown shadow adds a thick band under the board. | Soften it to keep the footer visually lighter. |
| angry_birds_settings-19 | P2 | proposal | Tab shadows: Dark tab shadows compete with text outlines. | Reduce shadow depth while retaining the toy-like style. |
| angry_birds_settings-20 | P2 | proposal | Button border color: Gray-green outlines sit beside warm brown panel outlines. | Use a consistent warmer outline palette. |
| angry_birds_settings-21 | P2 | proposal | Audio label color: Brown labels are muted against the peach surface. | Darken labels slightly for small-screen readability. |
| angry_birds_settings-22 | P2 | proposal | Separator: The thin footer rule is easy to lose against the peach background. | Increase its contrast modestly. |
| angry_birds_settings-23 | P2 | proposal | Background blocks: Stacked dark blocks on the right remain visually prominent. | Blur or dim them further behind the modal. |
| angry_birds_settings-24 | P2 | proposal | Bottom white line: An isolated pale line spans much of the screen below the board. | Remove it or make its purpose clear as a device indicator. |
| angry_birds_settings-25 | P2 | proposal | Behind-status context: The mock includes heart and currency status behind the board, absent here. | Restore subdued game context above the panel if matching the reference remains the target. |

## animation basic

[Reviewed image](../../docs/design-audit/originals/animation_basic_720p.png) · [Current baseline](../../screenshot-baselines/screens/animation_basic_720p.png) · [Visual review](../../design-audit.html#screen=animation_basic)

Baseline SHA-256: `410e822f449b8551124446c7bc5b7518ac28580db5c842f1cdb607f7ffc91127`

The three animations are captured at their endpoints. Eighteen grounded proposals improve comparison; this image does not establish motion defects.

Current example states: [Current example: All three playing](../../docs/design-audit/examples/animation_basic_running_720p.png) · [Current example: Independent scale replay](../../docs/design-audit/examples/animation_basic_scale_running_720p.png)

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| animation_basic-01 | P2 | proposal | Header location: A large empty region sits above Animation Basics. | Move the header upward and reclaim space for diagnostics. |
| animation_basic-02 | P2 | proposal | Title/subtitle bands: Title and subtitle occupy separate touching rounded strips. | Combine them into one padded header block. |
| animation_basic-03 | P2 | proposal | Main specimen group: The three examples occupy a small area amid large gaps. | Use three equal cards with shared alignment. |
| animation_basic-04 | P2 | proposal | Slide label order: Subtitle says Fade, Slide, Scale while columns show Slide, Fade, Scale. | Match subtitle order to the actual columns. |
| animation_basic-05 | P2 | proposal | Slide measurement: x offset: 0px shows only the endpoint. | Add start and end values in a caption. |
| animation_basic-06 | P2 | proposal | Slide reference: The green square has no start-position marker. | Draw a ghost origin outside its final bounds. |
| animation_basic-07 | P2 | proposal | Fade reference: The blue square is fully opaque in the captured state. | Add a checkerboard or reference swatch behind its test area. |
| animation_basic-08 | P2 | proposal | Scale reference: The purple square has no visible original-size boundary. | Draw a thin fixed bounds outline behind it. |
| animation_basic-09 | P2 | proposal | Animation durations: No example displays its duration. | Add a duration label below each animation name. |
| animation_basic-10 | P2 | proposal | Animation easing: Bounce and other timing differences are not described. | Display the easing name per specimen. |
| animation_basic-11 | P2 | proposal | Metric chips: The slide chip is wider than the other metric chips. | Use a shared width and aligned metric baseline. |
| animation_basic-12 | P2 | proposal | Metric typography: The metrics use tiny handwritten numerals. | Use readable tabular numeric text. |
| animation_basic-13 | P2 | proposal | Animation phase: No status says the examples have completed. | Show Ready, Running or Complete beside restart. |
| animation_basic-14 | P2 | proposal | Restart label: Restart Animations is the only action but has low dark-teal emphasis. | Increase its contrast relative to the specimen captions. |
| animation_basic-15 | P2 | proposal | Restart position: The restart button is distant from the diagnostic row. | Move it nearer the specimens while preserving separation. |
| animation_basic-16 | P2 | proposal | Independent playback: One restart action serves all three examples. | Add small individual replay controls for focused comparison. |
| animation_basic-17 | P2 | proposal | Timeline: There is no way to compare progress in a static frame. | Add a shared progress track with an elapsed-time marker. |
| animation_basic-18 | P2 | proposal | Motion comparison: All endpoints are identical-size blocks. | Provide a clearly labeled strip of start, midpoint and endpoint thumbnails. |

## animation declarative

[Reviewed image](../../docs/design-audit/originals/animation_declarative_720p.png) · [Current baseline](../../screenshot-baselines/screens/animation_declarative_720p.png) · [Visual review](../../design-audit.html#screen=animation_declarative)

Baseline SHA-256: `78051ceaed6f003df9e4b7f12ccdb51de0f13f58f9dc00a7ea560a919cd3a1f9`

Eleven specimens combine triggers and motion types. Presentation proposals preserve the examples and add enough context to distinguish them in a settled screenshot.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| animation_declarative-01 | P2 | defect | Appear label: Appear is flush against the orange tile's left edge while adjacent labels are centered. | Center the label within the specimen. |
| animation_declarative-02 | P2 | defect | Pulse label: Pulse is flush against its orange tile's left edge. | Apply the same centered alignment as neighboring examples. |
| animation_declarative-03 | P2 | proposal | Row grouping: Three rows have no section labels. | Label trigger examples, motion examples and composition examples. |
| animation_declarative-04 | P2 | proposal | Hover tile: Hover names the trigger but not the resulting animation. | Add a caption identifying hover scale. |
| animation_declarative-05 | P2 | proposal | Click tile: Click! names an input but hides its scale behavior. | Use Click to scale with a compact parameter caption. |
| animation_declarative-06 | P2 | proposal | Both tile: Both! requires guessing which two inputs combine. | Label it Hover + click. |
| animation_declarative-07 | P2 | proposal | Appear tile: Appear has no indication that it runs on entry. | Add an On entry caption. |
| animation_declarative-08 | P2 | proposal | Slide tile: Slide does not state direction or distance. | Show a direction arrow and travel distance. |
| animation_declarative-09 | P2 | proposal | Bounce tile: Bounce does not reveal its final scale or overshoot. | Add a concise bounce parameter line. |
| animation_declarative-10 | P2 | proposal | Fade tile: Fade does not state that hover triggers it. | Add the trigger beneath the tile. |
| animation_declarative-11 | P2 | proposal | Pulse tile: Pulse does not name its repeating behavior. | Add Looping pulse under the specimen. |
| animation_declarative-12 | P2 | proposal | Slide+Rot tile: Rot abbreviates a motion unlike the full names above. | Spell out Slide + rotate. |
| animation_declarative-13 | P2 | proposal | Rot+Slide tile: Rot+Slide looks almost identical to its neighboring composition. | Add numbered transform-order labels. |
| animation_declarative-14 | P2 | proposal | All 3 tile: All 3 does not identify the included transforms. | List Scale + slide + rotate. |
| animation_declarative-15 | P2 | proposal | Composition reference: The bottom row provides no fixed reference bounds. | Add neutral outlines to expose transform offsets. |
| animation_declarative-16 | P2 | proposal | Third-row layout: Three bottom tiles sit left of the four-column grid's center. | Center the composition group under its heading. |
| animation_declarative-17 | P2 | proposal | Fourth empty cell: An unmarked gap remains at the bottom right. | Use it for a small transform-order legend. |
| animation_declarative-18 | P2 | proposal | Click count: Total clicks: 0 is separated from the clickable specimens. | Place it beside the trigger section heading. |
| animation_declarative-19 | P2 | proposal | Click attribution: The total counter does not identify which tile was activated. | Add a last-trigger label next to the counter. |
| animation_declarative-20 | P2 | proposal | Footer code: A full API expression is extremely small across the footer. | Use a readable monospace code block. |
| animation_declarative-21 | P2 | proposal | Footer claim: Old: ~50 lines of boilerplate is not illustrated. | Replace it with the exact current example configuration. |
| animation_declarative-22 | P2 | proposal | Subtitle: No boilerplate is promotional wording without explaining the concept. | State that each tile declares its own trigger and animation. |
| animation_declarative-23 | P2 | proposal | Color meaning: Blue, green, purple and orange repeat with no stated mapping. | Add a legend or assign colors consistently by trigger. |
| animation_declarative-24 | P2 | proposal | Replay discoverability: Entry animations have no visible restart control. | Add a replay-entry-examples action. |
| animation_declarative-25 | P2 | proposal | Phase evidence: The static view cannot distinguish idle and completed examples. | Add small phase labels outside the moving specimens. |

## animation interactive

[Reviewed image](../../docs/design-audit/originals/animation_interactive_720p.png) · [Current baseline](../../screenshot-baselines/screens/animation_interactive_720p.png) · [Visual review](../../design-audit.html#screen=animation_interactive)

Baseline SHA-256: `5d70ef1709f1616d4a9b8e9c9b46cfbbd118d5930a678694903fc45434feebac`

The screenshot shows three unpressed animation buttons. Eighteen distinct presentation proposals are supported; unseen press behavior is not judged.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| animation_interactive-01 | P2 | proposal | Header placement: The title begins below a very large empty top area. | Move it toward the top of the page. |
| animation_interactive-02 | P2 | proposal | Header strips: Title and instruction have touching rounded backgrounds. | Combine them into one header card. |
| animation_interactive-03 | P2 | proposal | Instruction wording: Click buttons to see press animations does not explain the difference between them. | Name the compared press effects. |
| animation_interactive-04 | P2 | proposal | Blue button: Click! gives no animation information. | Label it with its press-scale amount. |
| animation_interactive-05 | P2 | proposal | Green button: Press! reads like a synonym of Click!. | Use a label naming its distinct response. |
| animation_interactive-06 | P2 | proposal | Purple button: Tap! implies a different device without describing motion. | Name the actual animation variant. |
| animation_interactive-07 | P2 | proposal | Specimen captions: None of the buttons has a timing label. | Show duration under each button. |
| animation_interactive-08 | P2 | proposal | Scale comparison: All three buttons are identical at rest. | Add fixed outlines to make compression visible. |
| animation_interactive-09 | P2 | proposal | Easing comparison: The screen does not identify the timing curves. | Display each easing name below its specimen. |
| animation_interactive-10 | P2 | proposal | Button grouping: Three isolated squares lack card or section framing. | Create equal comparison cards with shared diagnostic rows. |
| animation_interactive-11 | P2 | proposal | Color mapping: Blue, green and purple have no legend. | Associate each color with the named animation variant. |
| animation_interactive-12 | P2 | proposal | Counter label: Total clicks counts all variants together. | Show individual counts alongside the total. |
| animation_interactive-13 | P2 | proposal | Last action: The counter cannot identify the most recently pressed specimen. | Add a Last pressed readout. |
| animation_interactive-14 | P2 | proposal | Counter reset: The count is displayed without a visible reset action. | Add a small Reset count control. |
| animation_interactive-15 | P2 | proposal | Footer type: The with_scale() note is small and widely tracked. | Use a readable monospace API label. |
| animation_interactive-16 | P2 | proposal | Footer separation: The implementation note sits far below the examples. | Place it beneath the comparison cards. |
| animation_interactive-17 | P2 | proposal | Motion metrics: No current scale value appears in the screenshot. | Add live scale percentages outside the animated buttons. |
| animation_interactive-18 | P2 | proposal | Playback aid: Only direct pressing is represented. | Add a labeled slow-preview action for comparing the three effects. |

## animation looping

[Reviewed image](../../docs/design-audit/originals/animation_looping_720p.png) · [Current baseline](../../screenshot-baselines/screens/animation_looping_720p.png) · [Visual review](../../design-audit.html#screen=animation_looping)

Baseline SHA-256: `5fb9423d03357b68de07bef30e5c1c6f36c12648c66d0dc6075e1cee522291ea`

Loop phase explains the dim glow and moving marker. Twenty supported proposals improve motion diagnosis without judging the captured phase as a defect.

Current example states: [Current example: Paused cycle samples](../../docs/design-audit/examples/animation_looping_overlay_720p.png)

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| animation_looping-01 | P2 | proposal | Header placement: A large blank area precedes the title. | Move the header upward to make space for diagnostics. |
| animation_looping-02 | P2 | proposal | Header construction: Title and subtitle occupy touching rounded strips. | Combine them in one padded header. |
| animation_looping-03 | P2 | proposal | Breathe label: Breathe does not state that the sample translates. | Add a Translation caption. |
| animation_looping-04 | P2 | proposal | Glow label: Glow does not identify opacity as the animated property. | Add an Opacity caption. |
| animation_looping-05 | P2 | proposal | Bounce label: Bounce does not identify its direction. | Add a vertical motion arrow outside the specimen. |
| animation_looping-06 | P2 | proposal | Spin label: Spin sits above a stationary-looking brown base and orange marker. | Identify the marker as the orbiting element. |
| animation_looping-07 | P2 | proposal | Breathe reference: The blue block has no fixed position outline. | Add an unmoving outline at its center position. |
| animation_looping-08 | P2 | proposal | Glow reference: The green sample is dim at this capture phase. | Add a fully opaque reference chip outside the animation. |
| animation_looping-09 | P2 | proposal | Bounce bounds: The purple block has no marked travel range. | Draw endpoints beside its motion area. |
| animation_looping-10 | P2 | proposal | Spin path: The orange marker's circular path is invisible. | Add a faint orbit guide behind it. |
| animation_looping-11 | P2 | proposal | Spin center: The brown base does not mark the orbit center. | Add a small center crosshair. |
| animation_looping-12 | P2 | proposal | Period labels: None of the four examples shows its cycle duration. | Print the period below each name. |
| animation_looping-13 | P2 | proposal | Phase labels: The captured examples provide no numeric phase. | Add a phase readout beneath each sample. |
| animation_looping-14 | P2 | proposal | Amplitude labels: The motion extent is not specified. | Display translation range or opacity range per example. |
| animation_looping-15 | P2 | proposal | Pause control: Continuous motion has no visible pause control. | Add a Pause all action above the specimen group. |
| animation_looping-16 | P2 | proposal | Replay control: There is no visible common phase reset. | Add Restart cycles beside Pause. |
| animation_looping-17 | P2 | proposal | Specimen balance: Spin's small marker is much less prominent than the three solid blocks. | Increase marker visibility without changing its orbit. |
| animation_looping-18 | P2 | proposal | Color explanation: Four colors do not map to a stated property legend. | Use captions that pair each color with its animated property. |
| animation_looping-19 | P2 | proposal | Footer note: The sin/cos statement is far from the samples and small. | Place a readable formula note under the motion diagnostics. |
| animation_looping-20 | P2 | proposal | Static capture aid: A single frame cannot show travel history. | Offer a labeled trail overlay for screenshot comparison. |

## animation spring

[Reviewed image](../../docs/design-audit/originals/animation_spring_720p.png) · [Current baseline](../../screenshot-baselines/screens/animation_spring_720p.png) · [Visual review](../../design-audit.html#screen=animation_spring)

Baseline SHA-256: `206808e13affc56cfa413a2ea5ca5162534a59b8845dadb15c0d761acd9b21d2`

The five spring samples are settled. Twenty proposals describe clearer comparison and controls; differing corner shapes are preserved as specimen choices.

Current example states: [Current example: Quarter-speed replay](../../docs/design-audit/examples/animation_spring_slow_preview_720p.png)

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| animation_spring-01 | P2 | proposal | Title wording: Springy! does not identify the technical comparison. | Add Spring animation variants as a subtitle. |
| animation_spring-02 | P2 | proposal | Title space: The title starts beneath a large empty top region. | Move it upward to reduce unused space. |
| animation_spring-03 | P2 | proposal | Instruction strip: The instruction stretches across the screen while specimens are narrow. | Align it to the specimen group's width. |
| animation_spring-04 | P2 | proposal | Boing caption: Boing! names a personality rather than a motion property. | Add its spring stiffness and damping below. |
| animation_spring-05 | P2 | proposal | Jelly caption: Jelly lacks a description of the deformation. | Add a short axis or scale descriptor. |
| animation_spring-06 | P2 | proposal | Balloon caption: Balloon does not state its motion direction. | Add a rise/scale descriptor matching the example. |
| animation_spring-07 | P2 | proposal | Drop caption: Drop has no marked starting height. | Show a ghost start marker above the square. |
| animation_spring-08 | P2 | proposal | Rocker caption: Rocker does not show its pivot. | Add a small pivot marker and angular range. |
| animation_spring-09 | P2 | proposal | Specimen bounds: None of the five blocks has a neutral resting outline. | Add fixed reference bounds outside the animated fill. |
| animation_spring-10 | P2 | proposal | Color-only identification: The colored blocks contain no persistent identifiers. | Add numbered external labels matching the parameter captions. |
| animation_spring-11 | P2 | proposal | Label size: The five names are tiny compared with Springy!. | Increase specimen-name size. |
| animation_spring-12 | P2 | proposal | Label plates: 120-pixel plates extend wider than their 100-pixel blocks. | Align each label and specimen within a shared card width. |
| animation_spring-13 | P2 | proposal | Spring parameters: No visible numeric parameters distinguish the samples. | Add a compact five-column parameter table. |
| animation_spring-14 | P2 | proposal | Bounce counter: Bounces: 0 does not define what is counted. | Rename it Activations if it counts clicks, or state the counting rule. |
| animation_spring-15 | P2 | proposal | Counter placement: The counter sits alone well below the specimens. | Place it adjacent to the restart control. |
| animation_spring-16 | P2 | proposal | Counter attribution: The count does not identify the last activated spring. | Add a last-sample label. |
| animation_spring-17 | P2 | proposal | Restart wording: Restart! omits whether it affects all samples. | Use Restart all five. |
| animation_spring-18 | P2 | proposal | Restart spacing: A large empty gap separates the counter and restart button. | Bring them into one control row. |
| animation_spring-19 | P2 | proposal | Motion phase: No label identifies idle, moving or settled samples. | Show each specimen's phase below its card. |
| animation_spring-20 | P2 | proposal | Slow comparison: The page offers no visible slower playback aid. | Add a slow-preview toggle for inspecting overshoot. |

## auto text color

[Reviewed image](../../docs/design-audit/originals/auto_text_color_720p.png) · [Current baseline](../../screenshot-baselines/screens/auto_text_color_720p.png) · [Visual review](../../design-audit.html#screen=auto_text_color)

Baseline SHA-256: `793b23efb505f4f33bd2faca3f7cd534321ed898c64eff637679f51957b3e015`

The varied foreground colors are intentional tests. Proposals clarify the automatic-color contract and make the specimens easier to compare.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| auto_text_color-01 | P2 | proposal | New default badge: NEW DEFAULT is a large yellow badge without a version. | Replace it with a smaller Default enabled annotation or version. |
| auto_text_color-02 | P2 | proposal | Title balance: The badge pulls the combined heading off the visual center. | Center title and status as a balanced group. |
| auto_text_color-03 | P2 | proposal | Subtitle claim: Against any background reads as an unlimited contrast guarantee. | State that the system chooses between the configured light and dark colors. |
| auto_text_color-04 | P2 | proposal | Default heading: No config needed omits the relevant API setting. | Add the default flag name in a monospace caption. |
| auto_text_color-05 | P2 | proposal | Dark/light pairs: Near Black over Near White is the only obvious named pair. | Label columns as paired background families. |
| auto_text_color-06 | P2 | proposal | Color values: Near Black and Dark Blue give no actual values. | Show compact RGB or hex captions outside the tiles. |
| auto_text_color-07 | P2 | proposal | Chosen foreground: The samples show results without naming the chosen text color. | Add Light text or Dark text result captions. |
| auto_text_color-08 | P2 | proposal | Contrast ratios: Best contrast appears twice without measured ratios. | Display contrast ratios for the tested foreground/background pairs. |
| auto_text_color-09 | P2 | proposal | Dark tile borders: Thin outlines distinguish some dark samples from the page. | Use an explicitly labeled specimen boundary separate from color output. |
| auto_text_color-10 | P2 | proposal | Light tile row: The lighter row has no row label. | Label the rows Dark backgrounds and Light backgrounds. |
| auto_text_color-11 | P2 | proposal | Mid-tone spelling: Dk Magenta abbreviates the color inconsistently. | Spell out Dark magenta. |
| auto_text_color-12 | P2 | proposal | Mid-tone comparison: Six hues appear without a luminance ordering. | Order by luminance or state the intended sequence. |
| auto_text_color-13 | P2 | proposal | Gray specimen: Gray's dark text choice is difficult to compare against alternatives. | Add a companion light-text reference outside the primary fixture. |
| auto_text_color-14 | P2 | proposal | Teal specimen: Teal shares a similar brightness with nearby swatches but lacks numeric context. | Show its luminance in the caption. |
| auto_text_color-15 | P2 | proposal | Explicit options heading: The options look like actions even though they are demonstration specimens. | Label the row Configuration examples. |
| auto_text_color-16 | P2 | proposal | Disabled wording: Disabled (theme font) can be mistaken for a disabled control. | Use Auto color off: theme font. |
| auto_text_color-17 | P2 | proposal | Auto default specimen: Auto (default) does not show its selected foreground value. | Add the resolved foreground below it. |
| auto_text_color-18 | P2 | proposal | Custom red specimen: Custom Red Text does not state that it overrides automatic selection. | Add an Explicit override caption. |
| auto_text_color-19 | P2 | proposal | Explicit comparison: The middle example uses a different background from the two outer examples. | Add a matched-background comparison row. |
| auto_text_color-20 | P2 | proposal | Theme heading: Works with Theme Colors Too is longer than neighboring section names. | Use Theme role examples. |
| auto_text_color-21 | P2 | proposal | Background role: Background has no visible tile boundary because it matches the page. | Add an external outline marking its specimen bounds. |
| auto_text_color-22 | P2 | proposal | Theme identity: Primary, Accent and Secondary do not name the active theme. | Add the active theme name above the role row. |
| auto_text_color-23 | P2 | proposal | Role ordering: Primary, Accent, Secondary uses an unusual sequence. | Group Primary and Secondary before Accent. |
| auto_text_color-24 | P2 | proposal | Separators: Rules start farther inward than section headings. | Align dividers with the headings' content edge. |
| auto_text_color-25 | P2 | proposal | Specimen typography: All examples use widely spaced handwritten labels. | Use a readable body face while retaining the exact tested colors. |

## bevel borders

[Reviewed image](../../docs/design-audit/originals/bevel_borders_720p.png) · [Current baseline](../../screenshot-baselines/screens/bevel_borders_720p.png) · [Visual review](../../design-audit.html#screen=bevel_borders)

Baseline SHA-256: `873b7f4a6f5ecb8455292eeb4b56c812420ff07f28d4bf94cd29d8bac1740f0d`

The 1–6 px raised and sunken border matrix is intentional and clean. Eighteen evidence-based proposals improve inspection; twenty-five would require padding this simple fixture.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| bevel_borders-01 | P2 | proposal | Top blank area: The matrix begins after nearly two hundred pixels of empty space. | Move the title and matrix upward. |
| bevel_borders-02 | P2 | proposal | Title strip: BEVEL BORDERS occupies an almost full-width bordered bar. | Match the title width to the matrix. |
| bevel_borders-03 | P2 | proposal | Title casing: All-capital handwritten title is less readable than the subtitle. | Use sentence case with a stronger weight. |
| bevel_borders-04 | P2 | proposal | Explanation: The raised/sunken lighting rule is written as one dense line. | Split it into two labeled directional diagrams. |
| bevel_borders-05 | P2 | proposal | Row labels: Raised and Sunken sit far from their first specimen. | Reduce the gap to the row-label gutter. |
| bevel_borders-06 | P2 | proposal | Column labels: Each thickness is repeated above and inside its squares. | Keep the column heading and replace inner text with a neutral sample mark. |
| bevel_borders-07 | P2 | proposal | Numeric type: Thickness values use handwritten numerals. | Use tabular numeric text for pixel measurements. |
| bevel_borders-08 | P2 | proposal | One-pixel detail: The thinnest border is difficult to inspect at normal size. | Add a magnified 1px corner inset. |
| bevel_borders-09 | P2 | proposal | Six-pixel detail: The widest border lacks a dimension bracket. | Add a 6px bracket outside its specimen. |
| bevel_borders-10 | P2 | proposal | Lighting direction: No icon indicates the common top-left light source. | Add a small top-left light-direction arrow. |
| bevel_borders-11 | P2 | proposal | Raised edges: Light and dark edges are not individually identified. | Label one reference square's highlight and shadow edges. |
| bevel_borders-12 | P2 | proposal | Sunken inversion: The lower row repeats the rule without showing correspondence. | Add a small inversion annotation connecting one raised/sunken pair. |
| bevel_borders-13 | P2 | proposal | Base fill: The matrix does not name its shared face color. | Add the face-color value in a legend. |
| bevel_borders-14 | P2 | proposal | Edge colors: Highlight and shadow colors lack a reference swatch. | Show labeled highlight and shadow chips. |
| bevel_borders-15 | P2 | proposal | Baseline control: All displayed boxes have bevels. | Add a zero-width border reference. |
| bevel_borders-16 | P2 | proposal | Outer dimensions: The matrix does not state whether outer box sizes are fixed. | Show the shared specimen dimensions. |
| bevel_borders-17 | P2 | proposal | Interior size: Thicker borders visibly reduce the usable interior. | Add a compact interior-dimension caption per column. |
| bevel_borders-18 | P2 | proposal | Lower blank area: The bottom quarter is empty. | Use it for a single enlarged annotated bevel cross-section. |

## blend mode lab

[Reviewed image](../../docs/design-audit/originals/blend_mode_lab_720p.png) · [Current baseline](../../screenshot-baselines/screens/blend_mode_lab_720p.png) · [Visual review](../../design-audit.html#screen=blend_mode_lab)

Baseline SHA-256: `fa1082878c46a906e6ff63dc7f6ee19f18457d88c16ef6e31b81ee8d2f68fe19`

Dark multiply output and brighter additive output are expected. Proposals explain the blend inputs, ordering and batch-transition comparison.

Current example states: [Current example: Prepared premultiplied input](../../docs/design-audit/examples/blend_mode_prepared_720p.png)

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| blend_mode_lab-01 | P2 | proposal | Title: Blend modes. Real transitions last frame: 24 merges heading and live metric. | Separate the page title from the transition counter. |
| blend_mode_lab-02 | P2 | proposal | Title inset: The title begins at the rounded strip's left edge. | Add horizontal padding. |
| blend_mode_lab-03 | P2 | proposal | Transition definition: Real transitions is undefined in the visible frame. | Label it GPU blend-state changes or the precise measured operation. |
| blend_mode_lab-04 | P2 | proposal | Transition scope: 24 does not say whether it includes both specimen rows. | Show a short scope label. |
| blend_mode_lab-05 | P2 | proposal | Alpha caption: Alpha omits the configured opacity. | Add the input alpha value. |
| blend_mode_lab-06 | P2 | proposal | Additive caption: Additive provides no blending equation. | Add a compact source-plus-destination caption. |
| blend_mode_lab-07 | P2 | proposal | Multiply caption: Multiplied differs grammatically from the other mode names. | Use Multiply. |
| blend_mode_lab-08 | P2 | proposal | Premultiply caption: AlphaPremultiply is a compressed API identifier. | Use Premultiplied alpha with the API name beneath. |
| blend_mode_lab-09 | P2 | proposal | Mode title type: The four mode labels are tiny. | Increase label size while preserving specimen area. |
| blend_mode_lab-10 | P2 | proposal | Input colors: Red, green and blue inputs are visible but unspecified. | Add a labeled RGB input legend. |
| blend_mode_lab-11 | P2 | proposal | Circle order: Three overlaps do not state draw order. | Number the input circles outside their bounds. |
| blend_mode_lab-12 | P2 | proposal | Alpha overlap: The center intersection has no measurement label. | Add a sampled center-color swatch below the panel. |
| blend_mode_lab-13 | P2 | proposal | Multiply visibility: Dark output is expected but hard to compare to the background. | Add a labeled magnified sample beside the panel. |
| blend_mode_lab-14 | P2 | proposal | Premultiply contract: The panel does not say whether colors are pre-multiplied. | State the source-color preparation in its caption. |
| blend_mode_lab-15 | P2 | proposal | Backing color: All modes share a dark panel but its color is unnamed. | Show the shared destination color value. |
| blend_mode_lab-16 | P2 | proposal | Circle geometry: The specimens have no fixed geometry caption. | Document shared radius and overlap distance once. |
| blend_mode_lab-17 | P2 | proposal | Top panel alignment: Captions sit very close to panel tops. | Give all four captions a consistent clear gap. |
| blend_mode_lab-18 | P2 | proposal | Batch row headers: One scope per blit and round the batch are very small. | Increase their size and simplify the wording. |
| blend_mode_lab-19 | P2 | proposal | Per-blit count: 2 transitions each leaves the total implicit. | Show 8 blits × 2 = 16 transitions. |
| blend_mode_lab-20 | P2 | proposal | Batch total: 2 total does not identify the number of blits. | Show 8 blits, 2 transitions. |
| blend_mode_lab-21 | P2 | proposal | Batch item identity: Eight blue squares per panel have no identifiers. | Number them lightly outside their fills. |
| blend_mode_lab-22 | P2 | proposal | Batch equivalence: The two rows look identical without an explicit expected-result note. | State that output should match while state changes differ. |
| blend_mode_lab-23 | P2 | proposal | Batch boundaries: The shared scope is described only in text. | Draw one bracket over the batch and separate brackets over per-blit samples. |
| blend_mode_lab-24 | P2 | proposal | Blank lower region: A large lower region is unused. | Add a small comparison table of mode and transition count. |
| blend_mode_lab-25 | P2 | proposal | Capture metadata: The live count has no frame marker beyond last frame. | Show a stable capture/frame identifier beside the metric. |

### Additional capture: blend_mode_lab

[Image](../../baseline_screenshots/blend_mode_lab.png)

Active e2e baseline updated with the verified blend-mode design; original review evidence is retained with the main screen.

## button variants

[Reviewed image](../../docs/design-audit/originals/button_variants_720p.png) · [Current baseline](../../screenshot-baselines/screens/button_variants_720p.png) · [Visual review](../../design-audit.html#screen=button_variants)

Baseline SHA-256: `d157b9e5c026abd219326fd4e4584cbb183776982fcb9fce7c9c856b6aecb6ed`

The matrix intentionally includes ghost and disabled buttons. Proposals improve its labeling, measurement and theme comparison without changing those test states.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| button_variants-01 | P2 | proposal | Title inset: Button Variants touches the header's left edge. | Align it with the row-label inset. |
| button_variants-02 | P2 | proposal | Matrix headings: Normal, Accent, Secondary and Disabled repeat inside every row. | Add column headings above the matrix. |
| button_variants-03 | P2 | proposal | Normal terminology: Normal occupies the same category position as theme color roles. | Rename the role Primary. |
| button_variants-04 | P2 | proposal | Disabled comparison: Disabled mixes availability with three color roles. | Label the last column Disabled primary. |
| button_variants-05 | P2 | proposal | Disabled fixture framing: Disabled boundaries blend into the panel. | Add neutral external specimen outlines. |
| button_variants-06 | P2 | proposal | Ghost framing: Ghost controls look like disconnected text labels. | Draw optional diagnostic bounds outside the test controls. |
| button_variants-07 | P2 | proposal | Outline comparison: Thin colored outlines are hard to inspect. | Add enlarged corner samples for the outline row. |
| button_variants-08 | P2 | proposal | Row separation: Filled, Outline and Ghost panels touch with scalloped seams. | Use a consistent narrow row gutter. |
| button_variants-09 | P2 | proposal | Row-label alignment: Labels sit far left of their specimen grid. | Bring them closer in a fixed label column. |
| button_variants-10 | P2 | proposal | Specimen sizing: Main matrix controls have no dimensions shown. | Add one shared width and height caption. |
| button_variants-11 | P2 | proposal | Small specimen: Small lacks its numeric size. | Add measured dimensions below it. |
| button_variants-12 | P2 | proposal | Medium specimen: Medium has no padding or text-size annotation. | Add a compact token caption. |
| button_variants-13 | P2 | proposal | Large specimen: Large differs in typography as well as box size. | State both font and control dimensions. |
| button_variants-14 | P2 | proposal | Large outline label: Lg Outline abbreviates a word used fully elsewhere. | Use Large outline. |
| button_variants-15 | P2 | proposal | Size row comparison: Large outline also changes color role. | Keep the role constant or label the extra variable. |
| button_variants-16 | P2 | proposal | Theme cards: Neon Dark, Cozy Kraft and Sage are left clustered. | Distribute the three cards evenly across the specimen area. |
| button_variants-17 | P2 | proposal | Theme titles: Theme names share the small size of specimen labels. | Give card headings a clearer weight. |
| button_variants-18 | P2 | proposal | Theme button density: Filled and Outline buttons touch inside the cards. | Add a small consistent vertical gap. |
| button_variants-19 | P2 | proposal | Theme ghost position: Ghost text sits immediately below outline specimens. | Align it in a reserved equal-height row. |
| button_variants-20 | P2 | proposal | Theme dimensions: Theme samples are much smaller than the main matrix. | State that these are compact previews. |
| button_variants-21 | P2 | proposal | Theme output: No resolved foreground values accompany the theme examples. | Add a concise color-pair caption per card. |
| button_variants-22 | P2 | proposal | Main theme identity: The navy matrix does not identify its active theme. | Name the main theme beside the title. |
| button_variants-23 | P2 | proposal | Footer instruction: Try any variant above is flush against the footer edge. | Add matching horizontal padding. |
| button_variants-24 | P2 | proposal | Footer feedback: The initial instruction does not explain the result of pressing a specimen. | State that the footer reports the chosen variant. |
| button_variants-25 | P2 | proposal | Legend: Variant appearance, color role and size are interleaved without a guide. | Add a compact key distinguishing those three dimensions. |

## buttons

[Reviewed image](../../docs/design-audit/originals/buttons_720p.png) · [Current baseline](../../screenshot-baselines/screens/buttons_720p.png) · [Visual review](../../design-audit.html#screen=buttons)

Baseline SHA-256: `b8ed32e2717dc06fcbdc6c73fe8c9551383a671c49a5730a9bb7c2af4a863ca6`

The disabled treatment and square-corner sample are intentional. Twenty-five proposals improve the gallery's explanatory value and alignment.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| buttons-01 | P2 | proposal | Title padding: Button Gallery starts at the strip's edge. | Add the same inset as row labels. |
| buttons-02 | P2 | proposal | Header size: The title band is much taller than its text requires. | Reduce its height or add a short purpose subtitle. |
| buttons-03 | P2 | proposal | States label: States contains Accent and Secondary, which are color roles. | Rename the row Roles and availability. |
| buttons-04 | P2 | proposal | Normal button: Normal does not identify the primary theme role. | Label it Primary. |
| buttons-05 | P2 | proposal | Disabled specimen: The deliberately dim label is difficult to identify independently. | Add a readable external Disabled caption. |
| buttons-06 | P2 | proposal | Disabled bounds: The disabled outline is hard to distinguish from the panel. | Add an external diagnostic boundary. |
| buttons-07 | P2 | proposal | State dimensions: The four specimens show no shared measurement. | Add a dimensions caption for this row. |
| buttons-08 | P2 | proposal | Size naming: Large Button differs from Small and Medium naming. | Use Large for parallel labels. |
| buttons-09 | P2 | proposal | Size comparison: Small and Medium appear similar in height. | Print their exact height and padding below them. |
| buttons-10 | P2 | proposal | Size width: Different label lengths influence perceived button size. | Use a shared test label with external size captions. |
| buttons-11 | P2 | proposal | Size alignment: Controls are centered vertically but no baseline aid is shown. | Add an optional baseline guide. |
| buttons-12 | P2 | proposal | Group spacing: One through Four are separated widely despite group-specific corner shapes. | Tighten spacing to show a coherent segmented group. |
| buttons-13 | P2 | proposal | Group semantics: One, Two, Three and Four provide no shared task. | Use four related view options for the group example. |
| buttons-14 | P2 | proposal | Group boundary: There is no outer label explaining the asymmetric corner shapes. | Add a caption identifying first, middle and last corners. |
| buttons-15 | P2 | proposal | Group selection: No visible marker explains whether this is a selection group or independent actions. | State the group's intended behavior in a caption. |
| buttons-16 | P2 | proposal | Custom heading: Custom combines color and corner experiments. | Split color examples from the corner example visually. |
| buttons-17 | P2 | proposal | Coral sample: Coral shows no color value. | Add its configured hex value below it. |
| buttons-18 | P2 | proposal | Teal sample: Teal's text treatment differs from Coral without explanation. | Add a foreground-selection caption. |
| buttons-19 | P2 | proposal | Sharp sample: Sharp lacks a rounded reference beside it. | Add a matched rounded control for direct comparison. |
| buttons-20 | P2 | proposal | Main color mapping: Blue, ochre and green are unnamed theme colors. | Add a small role-color legend. |
| buttons-21 | P2 | proposal | Row width: Most of each full-width row remains empty on the right. | Use the space for short specimen notes. |
| buttons-22 | P2 | proposal | Row alignment: The largest-size specimen extends farther right than other rows. | Use a shared four-column comparison grid. |
| buttons-23 | P2 | proposal | Panel gaps: Vertical spacing dominates the compact specimen content. | Reduce gaps to make room for useful captions. |
| buttons-24 | P2 | proposal | Footer padding: Try any button above begins at the bar edge. | Inset footer text consistently. |
| buttons-25 | P2 | proposal | Footer meaning: The instruction does not say what activation reports. | Describe the selected-button feedback shown there. |

## cards

[Reviewed image](../../docs/design-audit/originals/cards_720p.png) · [Current baseline](../../screenshot-baselines/screens/cards_720p.png) · [Visual review](../../design-audit.html#screen=cards)

Baseline SHA-256: `70021708e5133a12f2e03373432ad76527f8b0bc9166601659492ec6b05ff9a9`

The gallery's custom colors, sharp corners and translucent example are intentional. Findings target label placement and clearer comparison.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| cards-01 | P2 | defect | Style specimen labels: Surface, Primary, Sharp and Custom begin directly on their left borders. | Give each label visible internal padding. |
| cards-02 | P2 | defect | Nested item labels: Item 1, Item 2 and Item 3 touch their colored blocks' left edges. | Add consistent inset to nested labels. |
| cards-03 | P2 | proposal | Title inset: Cards & Panels Gallery is flush with the brown header's edge. | Align it to the main content inset. |
| cards-04 | P2 | proposal | Header width: The header is narrower than the white style and nested rows. | Use a consistent content width. |
| cards-05 | P2 | proposal | Style row label: Styles is a dark button-shaped block. | Use a plain section heading to distinguish labels from specimens. |
| cards-06 | P2 | proposal | Surface example: The white card almost merges into the white row. | Add an external labeled bounds guide. |
| cards-07 | P2 | proposal | Primary example: Primary has no theme-color value shown. | Add its role and resolved color beneath it. |
| cards-08 | P2 | proposal | Sharp example: Sharp also changes fill and border weight. | Add a matched rounded specimen or state both changed variables. |
| cards-09 | P2 | proposal | Custom example: Custom does not identify what was customized. | Caption the background, border and radius values. |
| cards-10 | P2 | proposal | Style dimensions: The card specimens have no size annotation. | Show the shared outer dimensions once. |
| cards-11 | P2 | proposal | Style gaps: Wide gaps separate four small cards. | Use a regular matrix with room for parameter captions. |
| cards-12 | P2 | proposal | Nested section label: Nested is vertically centered in a button-like plaque. | Move it above the nested examples as a heading. |
| cards-13 | P2 | proposal | Nested card hierarchy: Nested Cards has similar typography to its item labels. | Increase heading weight and size. |
| cards-14 | P2 | proposal | Nested boundaries: The outer nested card border is faint. | Add a clearer external outline marking the parent bounds. |
| cards-15 | P2 | proposal | Nested padding: Parent-child insets are visible but unmeasured. | Annotate the parent padding. |
| cards-16 | P2 | proposal | Nested item spacing: The three inner items have no gap value shown. | Add a shared gap caption. |
| cards-17 | P2 | proposal | Translucent heading: Semi-Transparent sits in a bright white title strip. | Make the title treatment consistent with Nested Cards. |
| cards-18 | P2 | proposal | Opacity evidence: The translucent panel overlays a nearly flat backing color. | Add a checkerboard or stripe behind the sample to expose blending. |
| cards-19 | P2 | proposal | Opacity value: Uses opacity does not state the actual alpha. | Display the configured percentage. |
| cards-20 | P2 | proposal | Opacity copy: The description wraps into an awkward short second line. | Adjust text width or shorten the explanation. |
| cards-21 | P2 | proposal | Theme palette label: Theme Colors is a brown tile that resembles the primary swatch. | Use a neutral section label. |
| cards-22 | P2 | proposal | Theme palette values: Five role chips show names without color values. | Add compact resolved values beneath the chips. |
| cards-23 | P2 | proposal | Background swatch: Background nearly matches the page around the palette. | Give the specimen an external boundary. |
| cards-24 | P2 | proposal | Theme identity: The kraft palette has no visible theme name. | Name Cozy Kraft near the title. |
| cards-25 | P2 | proposal | Vertical spacing: Large empty bands separate the three rows. | Tighten spacing and allocate room for card measurements. |

### Additional capture: cards

[Image](../../baseline_screenshots/cards.png)

Active e2e baseline refreshed after visual review. The previous capture remains in originals until audit cleanup.

## casual settings

[Reviewed image](../../docs/design-audit/originals/casual_settings_720p.png) · [Current baseline](../../screenshot-baselines/screens/casual_settings_720p.png) · [Visual review](../../design-audit.html#screen=casual_settings)

Baseline SHA-256: `213201c1164f7468f31308d33e64f839d85c223e7602bd1c0a53bd635bed7440`

The screen closely matches the playful settings mock. Proposals clarify saved state, icon meanings and the hierarchy between preferences and support links.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| casual_settings-01 | P2 | proposal | Music button: The note icon has no visible label or ON text. | Add MUSIC and its current state beneath the icon. |
| casual_settings-02 | P2 | proposal | Sound button: The speaker icon lacks a separate effects label. | Add SOUND EFFECTS and its current state. |
| casual_settings-03 | P2 | proposal | Vibration button: Gray fill may read as unavailable rather than off. | Add VIBRATION OFF beneath it. |
| casual_settings-04 | P2 | proposal | Audio group: Three icon buttons have no group heading. | Add a compact AUDIO heading. |
| casual_settings-05 | P2 | proposal | Save/load label: SAVE/LOAD PROGRESS combines two distinct operations. | Use CLOUD SAVE with a descriptive secondary caption. |
| casual_settings-06 | P2 | proposal | Save/load icon: A Wi-Fi symbol can imply connectivity rather than stored progress. | Use a cloud-save icon. |
| casual_settings-07 | P2 | proposal | Save status: The save button shows no account or last-sync status. | Add a short saved-state line nearby. |
| casual_settings-08 | P2 | proposal | Notifications control: NOTIFICATIONS: OFF looks like a navigation button. | Add a visible switch indicator. |
| casual_settings-09 | P2 | proposal | Notifications label: The full label is denser than neighboring actions. | Separate the setting name from the OFF state. |
| casual_settings-10 | P2 | proposal | Language button: LANGUAGE does not display the current language. | Show English or the selected language as a secondary value. |
| casual_settings-11 | P2 | proposal | Credits priority: CREDITS has the same prominence as core preferences. | Reduce its size or move it into an About group. |
| casual_settings-12 | P2 | proposal | Support priority: SUPPORT visually competes with save/load. | Group it with help and legal actions. |
| casual_settings-13 | P2 | proposal | Legal button: TERMS AND PRIVACY occupies a large glossy action. | Use a quieter legal-link treatment. |
| casual_settings-14 | P2 | proposal | Build identifier: 15555-1-114203-20-10200-01 lacks a label. | Prefix it with Build. |
| casual_settings-15 | P2 | proposal | Version text: The version uses playful heavy lettering for technical data. | Use a compact readable numeric face. |
| casual_settings-16 | P2 | proposal | Player identifier: Player ID is a long uninterrupted number. | Group digits for scanning while preserving the exact value. |
| casual_settings-17 | P2 | proposal | Metadata alignment: Build, Version and Player ID are centered as one dense block. | Left-align a compact two-column label/value block. |
| casual_settings-18 | P2 | proposal | Metadata placement: Technical identifiers sit under Language without a section label. | Put them in an About footer. |
| casual_settings-19 | P2 | proposal | Close control: The close button extends beyond the board's inner edge. | Move it inward slightly for a cleaner safe margin. |
| casual_settings-20 | P2 | proposal | Close semantics: There is no visible saved-state reassurance near the sole exit. | Add Changes save automatically if that is the intended behavior. |
| casual_settings-21 | P2 | proposal | Background currency: 1000 coins appears faintly at the top right without an icon. | Restore a coherent subdued currency chip. |
| casual_settings-22 | P2 | proposal | Grid grouping: Preferences and support actions alternate across the two columns. | Group settings on one side and account/help actions on the other. |
| casual_settings-23 | P2 | proposal | Save text wrapping: SAVE/LOAD PROGRESS uses a tight two-line block. | Increase line spacing and rebalance icon spacing. |
| casual_settings-24 | P2 | proposal | Button shadows: Heavy dark shadows crowd the gaps between rows. | Reduce shadow depth to open the layout. |
| casual_settings-25 | P2 | proposal | Bottom line: A long pale line below the board has no clear role. | Remove it unless it represents a deliberate device affordance. |

## chart lab

[Reviewed image](../../docs/design-audit/originals/chart_lab_720p.png) · [Current baseline](../../screenshot-baselines/screens/chart_lab_720p.png) · [Visual review](../../design-audit.html#screen=chart_lab)

Baseline SHA-256: `4523faaa397c00e5b02c254990c31030cbe5d0ee8a444261eff6650facb9d863`

The chart renders two visible series and a selected first sample. Findings address axis readability, selection context and fixture controls.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| chart_lab-01 | P2 | defect | Axis labels: The Y endpoints 11 and 2 are tiny at the far left. | Increase tick-label size and reserve a proper axis gutter. |
| chart_lab-02 | P2 | proposal | X units: Seconds appears only in the bottom instruction. | Place seconds next to the X axis. |
| chart_lab-03 | P2 | proposal | Y units: Milliseconds appears only in the bottom instruction. | Label the Y axis ms. |
| chart_lab-04 | P2 | proposal | X ticks: Only 0 and 1.98 are shown across a wide plot. | Add evenly spaced intermediate time ticks. |
| chart_lab-05 | P2 | proposal | Y ticks: Several horizontal grid lines have no numeric values. | Label each major Y tick. |
| chart_lab-06 | P2 | proposal | Legend: Update and Render labels are extremely small. | Increase their size and add short line swatches. |
| chart_lab-07 | P2 | proposal | Series distinction: The two series differ only by color. | Give one a dashed stroke or different marker shape. |
| chart_lab-08 | P2 | proposal | Line weight: Both series are very thin against the dark plot. | Increase line width modestly. |
| chart_lab-09 | P2 | proposal | Grid color: Purple grid lines introduce a third color unrelated to the series. | Use a neutral subdued grid color. |
| chart_lab-10 | P2 | proposal | Selected sample: Dots at the left mark the first sample without a shared guide. | Draw a vertical selection line through the selected time. |
| chart_lab-11 | P2 | proposal | Selected time: Sample 1 of 120 omits the selected time. | Show the corresponding timestamp next to the count. |
| chart_lab-12 | P2 | proposal | Value labels: 8.00 ms and 6.00 ms float near the first points. | Use a compact selected-sample readout outside the plot. |
| chart_lab-13 | P2 | proposal | Sample counter: The count is detached from both value labels. | Group index, time and both series values. |
| chart_lab-14 | P2 | proposal | Previous action: Previous sample looks active while sample 1 is selected. | Show the boundary state when no prior sample exists. |
| chart_lab-15 | P2 | proposal | Preset selection: Empty, Single, Constant, Negative and Multiple all look identical. | Mark the active dataset preset. |
| chart_lab-16 | P2 | proposal | Multiple wording: Multiple does not identify multiple series. | Rename it Multiple series. |
| chart_lab-17 | P2 | proposal | Single wording: Single does not distinguish one sample from one series. | Rename it Single sample. |
| chart_lab-18 | P2 | proposal | Fixture controls: Dataset buttons and Start live share one appearance. | Separate fixtures from playback controls. |
| chart_lab-19 | P2 | proposal | Live status: Start live has no persistent paused/live indicator beside it. | Add a Live off status chip. |
| chart_lab-20 | P2 | proposal | Chart title: Charts / frame timings mixes gallery and metric names. | Use Frame timing chart with a demo subtitle. |
| chart_lab-21 | P2 | proposal | Plot padding: The chart begins close to the legend and endpoint labels. | Add consistent internal plot padding. |
| chart_lab-22 | P2 | proposal | Negative fixture cue: Negative is available without any explanation of why timings can be negative. | Label it Negative-value test data. |
| chart_lab-23 | P2 | proposal | Instruction density: Hover, Tab, Enter and axis units are packed into one line. | Separate input help from chart metadata. |
| chart_lab-24 | P2 | proposal | Sample navigation: Long Previous sample and Next sample buttons dominate the footer. | Use compact arrows with a clear shared Sample label. |
| chart_lab-25 | P2 | proposal | Frame budget: The chart has no contextual timing threshold. | Add an optional labeled frame-budget reference line. |

## checkboxes

[Reviewed image](../../docs/design-audit/originals/checkboxes_720p.png) · [Current baseline](../../screenshot-baselines/screens/checkboxes_720p.png) · [Visual review](../../design-audit.html#screen=checkboxes)

Baseline SHA-256: `8803f8f0120ccde187bb361b14b6d4674b1c3e0872d2a2df3e86b126308d25f5`

The screenshot intentionally includes disabled and minimum-selection examples. Findings improve control geometry, grouping and explanation.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| checkboxes-01 | P2 | defect | Left labels: Primary, Secondary and Accent touch their colored row edges. | Add consistent label inset. |
| checkboxes-02 | P2 | defect | Right labels: Option A through D begin directly at the row boundary. | Inset labels to match the left column. |
| checkboxes-03 | P2 | proposal | Checkbox glyphs: Selections use literal [x] and [ ] text. | Render clear square checkbox marks. |
| checkboxes-04 | P2 | proposal | Box-only geometry: Box Only specimens look like wide pills. | Use square specimens or label the pill geometry as intentional. |
| checkboxes-05 | P2 | proposal | Box-only identity: Four unlabeled controls have no external identifiers. | Add numbered captions outside the controls. |
| checkboxes-06 | P2 | proposal | Box-only states: Two empty pills have no explicit off-state reference. | Add an external on/off legend. |
| checkboxes-07 | P2 | proposal | With Label heading: The section heading uses the same teal fill as its controls. | Give headings a neutral treatment. |
| checkboxes-08 | P2 | proposal | Color role row: Primary, Secondary and Accent change row fill along with selection state. | Add a matched-state comparison caption. |
| checkboxes-09 | P2 | proposal | Disabled heading: Disabled uses bright teal while its examples are intentionally dim. | Use a neutral section header. |
| checkboxes-10 | P2 | proposal | Disabled ON wording: ON/OFF terminology differs from checkbox checked state. | Use Checked and Unchecked. |
| checkboxes-11 | P2 | proposal | Disabled specimens: Dim text is the only clear explanation of disabled status. | Add readable external Disabled captions. |
| checkboxes-12 | P2 | proposal | Multi-select heading: Selected: 2 of 4 is embedded in a long header. | Separate the count into a compact badge. |
| checkboxes-13 | P2 | proposal | Option names: Option A through D carry no meaningful choice example. | Use four related preference names. |
| checkboxes-14 | P2 | proposal | Count duplication: 2 of 4 appears in both the header and tiny footer. | Use one prominent count near the group. |
| checkboxes-15 | P2 | proposal | Constrained heading: Select 1–2 options is followed by generic Choice names. | Use a concrete preference-selection example. |
| checkboxes-16 | P2 | proposal | Choice 1 state: The selected minimum-bound choice appears disabled with no explanation. | Add a note that one selection must remain. |
| checkboxes-17 | P2 | proposal | Selection rule: The maximum of two has no visible state summary beyond the heading. | Show Min 1, max 2 beside the count. |
| checkboxes-18 | P2 | proposal | Choice color: Constrained choices use purple while unconstrained choices use teal. | Explain the category mapping or neutralize row fills. |
| checkboxes-19 | P2 | proposal | Checkbox alignment: Check marks are far from their labels in long filled rows. | Bring marks closer to the label or align them in a clear trailing column. |
| checkboxes-20 | P2 | proposal | Column titles: Left column starts With Label while right starts Multi-Select. | Add equal top-level labels for primitive and grouped examples. |
| checkboxes-21 | P2 | proposal | Row corner seams: Touching rounded rows create scalloped edges. | Add narrow regular gutters or shared group corners. |
| checkboxes-22 | P2 | proposal | Footer size: Both selection summaries are very small. | Increase summary typography. |
| checkboxes-23 | P2 | proposal | Footer alignment: The summaries cluster near the middle instead of centering below their groups. | Align each beneath its associated column. |
| checkboxes-24 | P2 | proposal | Lower blank space: The bottom portion of the panel is mostly empty. | Use it for the constrained-selection rule explanation. |
| checkboxes-25 | P2 | proposal | Title scale: Checkbox Components dominates the small row labels. | Reduce the title slightly and strengthen section hierarchy. |

### Additional capture: checkboxes

[Image](../../baseline_screenshots/checkboxes.png)

Active e2e baseline refreshed after visual review. The previous capture remains in originals until audit cleanup.

## circular progress

[Reviewed image](../../docs/design-audit/originals/circular_progress_720p.png) · [Current baseline](../../screenshot-baselines/screens/circular_progress_720p.png) · [Visual review](../../design-audit.html#screen=circular_progress)

Baseline SHA-256: `13e82898d147812c3b0984295efe57900734d6183fb92d31dc1bebf2e8bee9fd`

Ring thickness, size and live progress vary intentionally. Twenty-five proposals make those variables and values easier to compare.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| circular_progress-01 | P2 | proposal | Basic naming: Basic does not identify its ring width. | Add the stroke width below the title. |
| circular_progress-02 | P2 | proposal | Animated naming: Animated shows 35% but no update interval. | Display the cycle duration in its caption. |
| circular_progress-03 | P2 | proposal | Thick ring: Thick Ring gives no numerical thickness. | Show its stroke width in pixels. |
| circular_progress-04 | P2 | proposal | Thin ring: Thin Ring repeats its heading in the description. | Replace the repeated caption with its stroke width. |
| circular_progress-05 | P2 | proposal | Basic subtitle: Static value repeats what the absence of motion already suggests. | State the configured progress and start angle. |
| circular_progress-06 | P2 | proposal | Top row comparison: All four rings use different values and colors. | Add a matched-value reference for thickness comparison. |
| circular_progress-07 | P2 | proposal | Start direction: The arcs have no stated origin or direction. | Add a small shared clockwise-from-top legend. |
| circular_progress-08 | P2 | proposal | Animated phase: 35% has no timestamp or phase context. | Add a live status and elapsed-time label. |
| circular_progress-09 | P2 | proposal | Live controls: No pause control is visible for the animated sample. | Add a compact Pause action outside the ring. |
| circular_progress-10 | P2 | proposal | Track contrast: Unfilled tracks are close to the card background. | Increase track contrast modestly while preserving example colors. |
| circular_progress-11 | P2 | proposal | Thin track: The thin ring's unfilled remainder is difficult to locate. | Add a magnified thin-ring inset. |
| circular_progress-12 | P2 | proposal | Percent placement: Percent values sit well below the rings. | Bring each value nearer its ring or center it inside. |
| circular_progress-13 | P2 | proposal | Size card heights: Five size-variation cards have different bottom edges. | Use equal-height containers while retaining ring sizes. |
| circular_progress-14 | P2 | proposal | Size ring alignment: Ring centers move downward as diameter increases. | Align centers on a shared horizontal baseline. |
| circular_progress-15 | P2 | proposal | 28px value: The smallest example shows 28px but no percentage. | Display its progress outside the ring. |
| circular_progress-16 | P2 | proposal | 40px value: The second example also lacks a percentage. | Show its value consistently with larger examples. |
| circular_progress-17 | P2 | proposal | Size units: 28px through 80px do not state diameter versus radius. | Label the row Diameter. |
| circular_progress-18 | P2 | proposal | Size fill variation: The size row changes progress as well as diameter. | Use equal percentages for the size-only comparison. |
| circular_progress-19 | P2 | proposal | Size thickness variation: Stroke thickness also changes without a caption. | Show stroke width beneath each diameter. |
| circular_progress-20 | P2 | proposal | Completed ring: 100% is visually a pink ring without a completion cue. | Add a small Completed caption. |
| circular_progress-21 | P2 | proposal | Size colors: Five colors lack a stated relationship to size. | Use one color for size comparison or explain the mapping. |
| circular_progress-22 | P2 | proposal | Section gap: The Size Variations heading floats between rows. | Align it with the card grid and reduce surrounding gap. |
| circular_progress-23 | P2 | proposal | Subtitle type: Radial progress visualization is small and muted. | Increase it slightly or replace it with useful parameter guidance. |
| circular_progress-24 | P2 | proposal | Footer space: The bottom fifth is unused. | Add a compact parameter table for all ring specimens. |
| circular_progress-25 | P2 | proposal | Card padding: Top specimen titles sit closer to the card edge than lower captions. | Normalize vertical padding within the four main cards. |

### Additional capture: circular_progress

[Image](../../baseline_screenshots/circular_progress.png)

Active e2e baseline refreshed after visual review. The previous capture remains in originals until audit cleanup.

## colors

[Reviewed image](../../docs/design-audit/originals/colors_720p.png) · [Current baseline](../../screenshot-baselines/screens/colors_720p.png) · [Visual review](../../design-audit.html#screen=colors)

Baseline SHA-256: `a722389bdcea1d33575c9f7ea3aaec86b88725d2e18c7052497a88198bbcc26e`

The Midnight palette is intentionally strong and dark. Recommendations clarify semantic roles and numeric color references.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| colors-01 | P2 | proposal | Theme identity: Midnight appears only in the bottom sentence. | Move the theme name beside the title. |
| colors-02 | P2 | proposal | Title color: The red title strip resembles the Primary specimen. | Label it as theme chrome or use a neutral heading background. |
| colors-03 | P2 | proposal | Theme values: Primary through Error/Danger omit their hex values. | Show resolved values below each role. |
| colors-04 | P2 | proposal | Surface boundary: The Surface swatch nearly merges with its black tray. | Add an external boundary or neutral backing. |
| colors-05 | P2 | proposal | Error role: Error/Danger combines two semantic names. | Choose one canonical role name and explain aliases separately. |
| colors-06 | P2 | proposal | Primary/error comparison: Primary red and error coral are close in hue. | Add a caption distinguishing action and error uses. |
| colors-07 | P2 | proposal | Secondary value: The gray Secondary tile has no luminance or contrast context. | Add its foreground contrast ratio. |
| colors-08 | P2 | proposal | Accent foreground: The yellow Accent uses black text unlike other roles. | Show the resolved foreground choice. |
| colors-09 | P2 | proposal | Role ordering: Surface is placed between Accent and Error. | Group action roles, surfaces and status colors explicitly. |
| colors-10 | P2 | proposal | Theme row heading: Theme Colors repeats the page title's meaning. | Use Semantic roles. |
| colors-11 | P2 | proposal | Custom heading: Custom Colors does not explain independence from theme roles. | Use Fixed custom values. |
| colors-12 | P2 | proposal | Custom values type: Hex codes use handwritten characters. | Use monospace text for reliable reading. |
| colors-13 | P2 | proposal | Custom text alignment: Color names and hex values form left-aligned blocks inside centered tiles. | Align all blocks to equal internal insets. |
| colors-14 | P2 | proposal | Red naming: Red is visually coral-pink. | Use a precise descriptive name while keeping the value. |
| colors-15 | P2 | proposal | Yellow naming: Yellow is visibly amber compared with the theme accent. | Name it Amber or Warm yellow. |
| colors-16 | P2 | proposal | Palette correspondence: Custom Red matches Error/Danger without explanation. | Show a small equality annotation. |
| colors-17 | P2 | proposal | Custom foreground: All custom swatches use black text without a contrast label. | Add the measured foreground ratio below. |
| colors-18 | P2 | proposal | Tray bounds: The rightmost tiles extend beyond the black inner tray. | Expand tray padding so all five swatches sit within equal margins. |
| colors-19 | P2 | proposal | Tile spacing: The horizontal gaps are inconsistent at the right edge. | Use a five-column grid with uniform gaps. |
| colors-20 | P2 | proposal | Header alignment: The title strip and content use different horizontal insets. | Align the title text to the swatch content. |
| colors-21 | P2 | proposal | Section spacing: Custom Colors sits close to the upper tray. | Increase the gap before the second section label. |
| colors-22 | P2 | proposal | Footer density: One long footer sentence repeats the row organization. | Replace it with a concise theme identifier and legend. |
| colors-23 | P2 | proposal | Footer gap: A large empty space separates swatches from the footer. | Move reference details directly under the specimens. |
| colors-24 | P2 | proposal | Missing background role: The gallery shows Surface but not a labeled Background specimen. | Add a background swatch within the theme row. |
| colors-25 | P2 | proposal | Reference context: The swatches show colors only on black backing. | Add a small light-backing comparison strip outside the primary palette. |

### Additional capture: colors

[Image](../../baseline_screenshots/colors.png)

Active e2e baseline refreshed after visual review. The previous capture remains in originals until audit cleanup.

## composer lab

[Reviewed image](../../docs/design-audit/originals/composer_lab_720p.png) · [Current baseline](../../screenshot-baselines/screens/composer_lab_720p.png) · [Visual review](../../design-audit.html#screen=composer_lab)

Baseline SHA-256: `73d0972a89851ea849389acbcc7820b7cff5ff67a417f000fd33d388dda71b65`

Clipped no-wrap text and the scrolled twelve-line field are explicit fixtures. Proposals preserve them and improve readable diagnostics and test context.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| composer_lab-01 | P2 | proposal | Title width: Composer Lab uses a long technical sentence as its title. | Separate the title from a short behavior subtitle. |
| composer_lab-02 | P2 | proposal | Section 1 heading: auto-grow + submit_on_enter is tiny. | Use a readable heading with API flags on a second line. |
| composer_lab-03 | P2 | proposal | First field help: The editable value doubles as the full instruction. | Move persistent guidance outside the input. |
| composer_lab-04 | P2 | proposal | Send count: Sends: 0 is buried in the first heading. | Show a compact submission counter beside the field. |
| composer_lab-05 | P2 | proposal | Submitted output: The first example has no visible result area for sent text. | Add a labeled last-submitted preview. |
| composer_lab-06 | P2 | proposal | Row-cap context: Capped at 5 rows is named but not visually marked. | Draw a neutral maximum-height outline behind the field. |
| composer_lab-07 | P2 | proposal | Current rows: The rows 3 diagnostic is far to the right. | Place current/cap row counts immediately below the first field. |
| composer_lab-08 | P2 | proposal | State table type: Composer state keys and values are very small. | Increase diagnostic text size. |
| composer_lab-09 | P2 | proposal | State alignment: Keys and values are separated by a wide empty gutter. | Use a compact aligned two-column table. |
| composer_lab-10 | P2 | proposal | Cursor units: Cursor 136 does not state bytes, characters or codepoints. | Label the unit explicitly. |
| composer_lab-11 | P2 | proposal | Row/column indexing: 2/32 has no zero-based or one-based explanation. | State the indexing convention. |
| composer_lab-12 | P2 | proposal | Scroll units: scroll y0.0 lacks units. | Display the unit used by the field. |
| composer_lab-13 | P2 | proposal | Widest measurement: widest 404px does not identify what width was measured. | Rename it Widest visual line. |
| composer_lab-14 | P2 | proposal | Second field heading: Word wrap on competes with the long Enter explanation. | Split behavior label from input instruction. |
| composer_lab-15 | P2 | proposal | Second field height: The empty lower area has no visible row guides. | Add optional baseline guides to explain its fixed capacity. |
| composer_lab-16 | P2 | proposal | No-wrap fixture: The third line clips intentionally with no external endpoint cue. | Add a small Clipped by design annotation outside the field. |
| composer_lab-17 | P2 | proposal | No-wrap hidden extent: The visible line does not show how much content continues. | Add a text-length or horizontal-overflow diagnostic. |
| composer_lab-18 | P2 | proposal | Scroll fixture: The fourth field shows ten, eleven, twelve without its current position. | Display Rows 10–12 of 12 beside it. |
| composer_lab-19 | P2 | proposal | Scroll cue: The scroll fixture has no visible scrollbar in this capture. | Add an external scroll-range indicator without changing clipping. |
| composer_lab-20 | P2 | proposal | Key reference: Keyboard help is tiny and densely packed. | Increase type and group movement, editing and selection. |
| composer_lab-21 | P2 | proposal | Platform shortcuts: Alt/Ctrl and Cmd/Ctrl syntax is hard to scan. | Show the active platform's keys first. |
| composer_lab-22 | P2 | proposal | Mouse help: Mouse instructions wrap into two distant short lines. | Use a compact list below keyboard help. |
| composer_lab-23 | P2 | proposal | Two-column balance: The content occupies only the left two-thirds of the screen. | Widen the examples and diagnostic table proportionally. |
| composer_lab-24 | P2 | proposal | Section spacing: The first-to-second field gap is much larger than later gaps. | Use consistent section spacing. |
| composer_lab-25 | P2 | proposal | Focused example label: The state table says focused yes without naming the field. | Show the active example number beside the state heading. |

## config gap gallery

[Reviewed image](../../docs/design-audit/originals/config_gap_gallery_720p.png) · [Current baseline](../../screenshot-baselines/screens/config_gap_gallery_720p.png) · [Visual review](../../design-audit.html#screen=config_gap_gallery)

Baseline SHA-256: `2e263b4b7f300c1a7b220cf0f0a3b171ca1d09cbb742392eb0dd2d62d4985161`

Clipping, faceted corners and low-contrast explicit text color are intentional. Insets that look equal and the 0x0 measurement need clearer comparison before any rendering diagnosis.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| config_gap_gallery-01 | P2 | proposal | Page title: Config options that had no caller describes repository history. | Use Component configuration examples. |
| config_gap_gallery-02 | P2 | proposal | Panel headings: API-heavy headings use tiny handwritten text. | Use readable monospace parameter labels. |
| config_gap_gallery-03 | P2 | proposal | Shadow labels: hard and soft touch their rectangles' left edges. | Add internal padding or external captions. |
| config_gap_gallery-04 | P2 | proposal | Shadow comparison: Hard and soft also differ in fill and radius. | Add matched-shape controls to isolate shadow style. |
| config_gap_gallery-05 | P2 | proposal | Hard shadow: The offset is visible but unspecified. | Show its x/y offset in a caption. |
| config_gap_gallery-06 | P2 | proposal | Soft shadow: Blur and offset are not labeled. | Display both configured values. |
| config_gap_gallery-07 | P2 | proposal | Segment example: 3 seg uses an abbreviation and no smooth reference. | Spell out 3 corner segments and add a matched smooth sample. |
| config_gap_gallery-08 | P2 | proposal | Corner radius: The three radii differ without numeric labels. | Show 4px, 18px and 20px under the samples. |
| config_gap_gallery-09 | P2 | proposal | Clamp heading: The heading promises min/max but omits the limits. | Display min 90px and max 200px. |
| config_gap_gallery-10 | P2 | proposal | Clamp values: 25%, 55% and 85% show requested widths only. | Add actual resolved pixel widths. |
| config_gap_gallery-11 | P2 | proposal | Clamp state: The first and last samples do not identify which bound applies. | Label Min-clamped and Max-clamped. |
| config_gap_gallery-12 | P2 | proposal | Clamp reference: No unclamped widths are visible. | Draw thin reference-length lines behind the specimens. |
| config_gap_gallery-13 | P2 | proposal | Clip boundary: The clipped region ends immediately under partial content. | Outline its 76px viewport boundary externally. |
| config_gap_gallery-14 | P2 | proposal | Clip row count: Rows 0 and 1 are visible without a total count. | Add 4 source rows, viewport 76px. |
| config_gap_gallery-15 | P2 | proposal | Hide target: Hide row 2 targets a row mostly below the clip boundary. | Add a small source-row diagram beside the control. |
| config_gap_gallery-16 | P2 | proposal | Marquee text: selection begins outside the dashed rectangle's inner edge. | Center it or inset it within the marquee. |
| config_gap_gallery-17 | P2 | proposal | Marquee parameters: The dashed rectangle gives no dash or gap measurements. | Display dash 10px, gap 7px outside it. |
| config_gap_gallery-18 | P2 | proposal | Draw-layer example: on_draw_bg and on_draw_fg are not visually identified. | Add small labels marking background fill and foreground stroke. |
| config_gap_gallery-19 | P2 | proposal | Explicit text color: The deliberately dark red sentence is difficult to read. | Repeat its exact text in a readable external caption. |
| config_gap_gallery-20 | P2 | proposal | Text inset comparison: inset 24px and inset 2px appear almost equally flush left. | Add pixel guides and measured inset readouts to expose the comparison. |
| config_gap_gallery-21 | P2 | proposal | Wrap example: The wrap-enabled sentence remains one line in the screenshot. | Add a narrow paired specimen that visibly wraps. |
| config_gap_gallery-22 | P2 | proposal | Activation buttons: on Press and on Release do not name pointer timing. | Use On pointer down and On pointer up captions. |
| config_gap_gallery-23 | P2 | proposal | Counter reset: Both activation counters have no visible reset action. | Add a small shared Reset counters control. |
| config_gap_gallery-24 | P2 | proposal | Measurement output: measure_config says 0x0 has no visible probe or expectation. | Show the measured text specimen and expected sizing rule alongside. |
| config_gap_gallery-25 | P2 | proposal | Focus-selection field: focus me, all selected is the only explanation of a behavior. | Add an external Select all on focus caption. |

## context menu lab

[Reviewed image](../../docs/design-audit/originals/context_menu_lab_720p.png) · [Current baseline](../../screenshot-baselines/screens/context_menu_lab_720p.png) · [Visual review](../../design-audit.html#screen=context_menu_lab)

Baseline SHA-256: `eb760be98e16654c0b1003c5fdea3225f19eb4d4c3ab53604dc7b181fd37c3fa`

The open context menu provides direct evidence of a clipped first label. Remaining proposals explain point anchoring and improve file/menu hierarchy.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| context_menu_lab-01 | P1 | defect | Stage file label: The open menu shows tage file with the initial S obscured by the selection border. | Inset menu text clear of its selection outline. |
| context_menu_lab-02 | P2 | proposal | Menu padding: Unstage and Copy path begin at the menu's left boundary. | Add consistent horizontal item padding. |
| context_menu_lab-03 | P2 | proposal | Menu target: The open menu obscures rows without identifying its target file. | Show the target filename in a small menu header. |
| context_menu_lab-04 | P2 | proposal | Row selection: No distinct selected-file treatment ties the menu to a row. | Highlight the context-menu target row. |
| context_menu_lab-05 | P2 | proposal | Menu action relationship: Stage file and Unstage are both visually prominent. | Make applicability clear for the target's current staged state. |
| context_menu_lab-06 | P2 | proposal | Disabled action: Discard changes is intentionally dim but unexplained. | Add an external note explaining why it is unavailable for this fixture. |
| context_menu_lab-07 | P2 | proposal | Copy shortcut: Cmd+C is dimmer than Copy path and crowded against the right edge. | Increase shortcut contrast and right inset. |
| context_menu_lab-08 | P2 | proposal | Menu grouping: Stage and Unstage are separated from Copy path only by a narrow gap. | Use a clear separator between repository and utility actions. |
| context_menu_lab-09 | P2 | proposal | Menu outline: The menu border is faint against the file rows. | Increase edge separation or add a subtle shadow. |
| context_menu_lab-10 | P2 | proposal | Menu row heights: The four items have little breathing room around their text. | Use consistent padded menu rows. |
| context_menu_lab-11 | P2 | proposal | File list headings: Files and statuses have no column labels. | Add File and State headers. |
| context_menu_lab-12 | P2 | proposal | Status type: modified, staged and untracked are tiny. | Increase status-label size. |
| context_menu_lab-13 | P2 | proposal | State recognition: All statuses use similar muted text. | Use small labeled status chips. |
| context_menu_lab-14 | P2 | proposal | README state: README.md has no status while other rows do. | Show Clean if the blank means unchanged. |
| context_menu_lab-15 | P2 | proposal | Filename type: Paths use the decorative handwritten font. | Use a readable monospace filename face. |
| context_menu_lab-16 | P2 | proposal | File rows: Six rounded rectangles have large inter-row gaps. | Tighten the list for a clearer file-browser pattern. |
| context_menu_lab-17 | P2 | proposal | Title wording: The title packs the point-anchoring rule into its name. | Use Context menus plus a short subtitle. |
| context_menu_lab-18 | P2 | proposal | Top instruction: right-click either row contradicts the six visible file rows. | Change it to Right-click any file row. |
| context_menu_lab-19 | P2 | proposal | Explanation heading: How this differs from a menu bar is longer than necessary. | Use Point anchoring. |
| context_menu_lab-20 | P2 | proposal | Explanation body: Five small widely spaced lines spread across a large card. | Use a compact readable paragraph with a diagram. |
| context_menu_lab-21 | P2 | proposal | Cursor anchor: The shown menu has no marked anchor point. | Add an optional crosshair at the invocation point. |
| context_menu_lab-22 | P2 | proposal | First test button: right-click me lacks the purpose of its test case. | Label it Open at pointer. |
| context_menu_lab-23 | P2 | proposal | Lower test button: right-click me too includes a long parenthetical. | Label it Bottom-edge flip test. |
| context_menu_lab-24 | P2 | proposal | Lower caption: The bottom explanation is tiny and distant from its test button. | Place it directly above that button. |
| context_menu_lab-25 | P2 | proposal | Empty center: A large unused middle region separates the test cases. | Add a small placement diagram showing below versus flipped above. |

## cozy cafe

[Reviewed image](../../docs/design-audit/originals/cozy_cafe_720p.png) · [Current baseline](../../screenshot-baselines/screens/cozy_cafe_720p.png) · [Visual review](../../design-audit.html#screen=cozy_cafe)

Baseline SHA-256: `55f011da55c9cf312034d99b6f95908098c9273e55a5f98d636fbd9d78e54a24`

The baseline follows the Blossom Cafe mock. Proposals improve customer urgency, action context and readability while retaining the illustrated cafe style.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| cozy_cafe-01 | P2 | proposal | Outlined title: Blossom Cafe uses a very thin dark outline around pale letters. | Strengthen the outline or fill for easier reading. |
| cozy_cafe-02 | P2 | proposal | Gold label: Gold: $1347 mixes a fantasy currency name with a dollar symbol. | Use one currency convention and an icon. |
| cozy_cafe-03 | P2 | proposal | Gold number: The outlined digits are harder to read than neighboring text. | Use solid tabular digits for the balance. |
| cozy_cafe-04 | P2 | proposal | Rating stars: Filled and empty stars differ subtly. | Increase the distinction between earned and unearned stars. |
| cozy_cafe-05 | P2 | proposal | Rating value: Only stars communicate the numeric rating. | Add a small 4/5 value. |
| cozy_cafe-06 | P2 | proposal | Customers served: Customers Served: 23 is tight within the rating box. | Increase inset or put the count on its own quieter line. |
| cozy_cafe-07 | P2 | proposal | Special selection: Lavender Latte differs mainly by border weight. | Add a small Selected marker. |
| cozy_cafe-08 | P2 | proposal | Special names: Outlined white lettering is delicate on pastel fills. | Use stronger solid brown text or a thicker outline. |
| cozy_cafe-09 | P2 | proposal | Special economics: No prices or benefits accompany the three specials. | Add a concise value caption per item. |
| cozy_cafe-10 | P2 | proposal | Promote action: Promote Special does not name the selected special. | Include Lavender Latte in a small action caption. |
| cozy_cafe-11 | P2 | proposal | Promote clock: The clock icon does not state cost or duration. | Show the promotion duration beside it. |
| cozy_cafe-12 | P2 | proposal | Customer title: Customers gives no queue length. | Add a 2 waiting count. |
| cozy_cafe-13 | P2 | proposal | Order text: Emma - Cappuccio puts name and order on one long line. | Separate customer name from the order description. |
| cozy_cafe-14 | P2 | proposal | Customer rows: Emma and Oliver have no visual divider. | Add a subtle separator or distinct row spacing. |
| cozy_cafe-15 | P2 | proposal | Wait times: 2m and 5m do not say whether they are elapsed or remaining. | Label the badges Waited or Remaining. |
| cozy_cafe-16 | P2 | proposal | Patience labels: Patience and Patience: LOW are very small. | Increase their text size. |
| cozy_cafe-17 | P2 | proposal | Low patience: The red warning relies on tiny colored text. | Add a clear low-patience icon or badge. |
| cozy_cafe-18 | P2 | proposal | Patience bars: Bars are narrow and visually weak. | Increase their height and show a numeric or verbal status. |
| cozy_cafe-19 | P2 | proposal | Serve target: Serve Next does not reveal which customer will be served. | Show Serve Emma if queue order is fixed. |
| cozy_cafe-20 | P2 | proposal | Urgency ordering: Oliver is low on patience but remains visually secondary. | Use a modest urgency accent on Oliver's row. |
| cozy_cafe-21 | P2 | proposal | Music layout: The slider appears above its label rather than beside it. | Place Music 70% in the same control row. |
| cozy_cafe-22 | P2 | proposal | Music track: The brown unfilled track blends into the warm outline. | Increase filled/unfilled distinction. |
| cozy_cafe-23 | P2 | proposal | Chat hierarchy: Two chat messages share one heavy brown block without a header. | Add a small Guild chat label. |
| cozy_cafe-24 | P2 | proposal | Research icon: A gift icon is labeled Research. | Use a book, flask or research-related symbol. |
| cozy_cafe-25 | P2 | proposal | Tool badges: Inventory 2 and Research! badges are tiny at button corners. | Enlarge badges slightly and explain their counts in tool captions. |

## deadspace settings

[Reviewed image](../../docs/design-audit/originals/deadspace_settings_720p.png) · [Current baseline](../../screenshot-baselines/screens/deadspace_settings_720p.png) · [Visual review](../../design-audit.html#screen=deadspace_settings)

Baseline SHA-256: `8ec07e7c81831bbac6abef46bd628914522e3e325a1927dc3f68c52ed107feab`

Layer overlap, subdued rear menu and scan lines are deliberate holographic styling. Findings are refinement proposals grounded in the mock and visible composition.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| deadspace_settings-01 | P2 | proposal | Front placement: The active menu occupies the left half with a large empty right region. | Shift the active stack toward center or reserve the right area for category details. |
| deadspace_settings-02 | P2 | proposal | More settings title: MORE SETTINGS is small relative to the wide active panel. | Increase its spacing and prominence modestly. |
| deadspace_settings-03 | P2 | proposal | Settings header: // SETTINGS repeats the title without navigation context. | Use it as a breadcrumb identifying the parent menu. |
| deadspace_settings-04 | P2 | proposal | Active gameplay: Gameplay uses brighter fill and outline but no shape marker. | Add a slim selection chevron. |
| deadspace_settings-05 | P2 | proposal | Active typography: Gameplay text has similar weight to inactive rows. | Give the selected label a slight weight increase. |
| deadspace_settings-06 | P2 | proposal | Scanline intensity: Horizontal lines run through every label. | Reduce line opacity immediately behind text while preserving the effect. |
| deadspace_settings-07 | P2 | proposal | Text edge treatment: Cyan fringe and thin letters reduce clarity. | Tighten the glow around letter edges. |
| deadspace_settings-08 | P2 | proposal | Row interiors: Labels sit near the left bevel transitions. | Increase left inset beyond the clipped corners. |
| deadspace_settings-09 | P2 | proposal | Front header gap: The heading and Controls row have a larger gap than other rows. | Use a deliberate shared header spacing. |
| deadspace_settings-10 | P2 | proposal | Row rhythm: Eight identical rows provide no category grouping. | Separate legal/about items from gameplay preferences. |
| deadspace_settings-11 | P2 | proposal | Policies label: Policies and Licenses is long and visually equal to core settings. | Place it in a quieter secondary group. |
| deadspace_settings-12 | P2 | proposal | Credits placement: Credits sits directly above the footer with little distinction. | Add a small gap before the footer. |
| deadspace_settings-13 | P2 | proposal | Rear menu prominence: Initial Settings remains detailed enough to compete with the active stack. | Reduce rear text emphasis a little further. |
| deadspace_settings-14 | P2 | proposal | Rear active row: More Settings extends out from behind the front stack. | Use a connector or breadcrumb cue to explain the relationship. |
| deadspace_settings-15 | P2 | proposal | Rear title: INITIAL SETTINGS sits close to the screen's left edge. | Increase its left safe margin. |
| deadspace_settings-16 | P2 | proposal | Overlapping footer: The front footer overlays the rear active-row region. | Offset the rear panel vertically to reduce dense intersections. |
| deadspace_settings-17 | P2 | proposal | Footer key labels: Enter SELECT and Esc BACK are plain text with similar weight. | Render Enter and Esc as distinct keycaps. |
| deadspace_settings-18 | P2 | proposal | Footer spacing: Select and Back cluster at the right end. | Give the two actions consistent grouped spacing. |
| deadspace_settings-19 | P2 | proposal | Footer alignment: The footer extends slightly beyond the active row stack. | Align its outer width with the main panel. |
| deadspace_settings-20 | P2 | proposal | Menu depth: The baseline stacks are flatter than the mock's skewed holographic layers. | Add restrained perspective or skew matching the mock. |
| deadspace_settings-21 | P2 | proposal | Selected description: Gameplay has no descriptive content in the empty right area. | Show a one-sentence category preview. |
| deadspace_settings-22 | P2 | proposal | Navigation context: No visible cue states this is a submenu. | Add Initial settings > More settings as a compact breadcrumb. |
| deadspace_settings-23 | P2 | proposal | Category symbols: Long text rows require reading every item. | Add restrained monochrome category symbols if consistent with the reference. |
| deadspace_settings-24 | P2 | proposal | Background grid: The faint grid ends abruptly behind the active panel region. | Fade its edges more gradually. |
| deadspace_settings-25 | P2 | proposal | Scale balance: Large black margins dominate the composition at 720p. | Increase active-menu scale slightly while retaining the layered layout. |

## decorative frame

[Reviewed image](../../docs/design-audit/originals/decorative_frame_720p.png) · [Current baseline](../../screenshot-baselines/screens/decorative_frame_720p.png) · [Visual review](../../design-audit.html#screen=decorative_frame)

Baseline SHA-256: `681bedb5212f51c9e4a666e7cf75a88d72a406101473ea610d3e7c25b3f5c036`

Frame differences are intentional. Twenty defensible proposals clarify the comparison; the simple three-card fixture does not support twenty-five independent defects.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| decorative_frame-01 | P2 | proposal | Subtitle claim: The same card is shown with three different titles and body texts. | Use identical card content for a controlled comparison. |
| decorative_frame-02 | P2 | proposal | Frame labels: KraftPaper is a code-style identifier beside ordinary Simple and Inset labels. | Use Kraft paper and place the API name below. |
| decorative_frame-03 | P2 | proposal | Caption type: Layered + corners and other captions are tiny. | Increase caption size. |
| decorative_frame-04 | P2 | proposal | Card headings: Field Notes, Tide Table and Plate 14 vary in apparent line length. | Use the same heading to compare frames fairly. |
| decorative_frame-05 | P2 | proposal | Body text: The small handwritten body is difficult to inspect from a distance. | Increase body type slightly. |
| decorative_frame-06 | P2 | proposal | Body alignment: All body text is centered despite short prose fragments. | Use consistent left-aligned content inset. |
| decorative_frame-07 | P2 | proposal | Kraft corner detail: Four dark corner tabs have no explanation. | Add a small annotated corner enlargement. |
| decorative_frame-08 | P2 | proposal | Kraft layers: The layered border has no thickness labels. | Show frame-band and inner-padding measurements. |
| decorative_frame-09 | P2 | proposal | Simple border: A very thin blue outline is hard to inspect. | Add an enlarged reference corner outside the specimen. |
| decorative_frame-10 | P2 | proposal | Inset lighting: The gray border's light direction is unnamed. | Add a subtle top-left lighting arrow. |
| decorative_frame-11 | P2 | proposal | Inset shadow: Sunken shadow does not state the shadow dimensions. | Show the configured inset thickness. |
| decorative_frame-12 | P2 | proposal | Frame dimensions: The cards appear similar in size but dimensions are unstated. | Print shared outer dimensions above the row. |
| decorative_frame-13 | P2 | proposal | Content padding: Different frame widths leave different visual insets. | Add content-bound guides to compare usable space. |
| decorative_frame-14 | P2 | proposal | Frame colors: Brown, blue and gray alter more than border treatment. | Add a neutral-color comparison strip. |
| decorative_frame-15 | P2 | proposal | Caption alignment: Frame names sit close to the bottoms of large cards. | Use equal caption blocks with consistent gap. |
| decorative_frame-16 | P2 | proposal | Card proportion: Large empty interiors surround two-line texts. | Reduce card height or add more representative content. |
| decorative_frame-17 | P2 | proposal | Footer language: Pick by weight uses subjective guidance. | State concrete use cases for each frame. |
| decorative_frame-18 | P2 | proposal | Footer typography: The recommendation is one small line across a wide bar. | Increase size and divide guidance by frame. |
| decorative_frame-19 | P2 | proposal | Footer border: The recommendation itself has a decorative frame that competes with samples. | Use a quieter note treatment. |
| decorative_frame-20 | P2 | proposal | Lower whitespace: A large empty band follows the recommendation. | Use it for a compact parameter legend. |

## decorators

[Reviewed image](../../docs/design-audit/originals/decorators_720p.png) · [Current baseline](../../screenshot-baselines/screens/decorators_720p.png) · [Visual review](../../design-audit.html#screen=decorators)

Baseline SHA-256: `230e50b4dc19158819202eeedf3a3424f12541ffa75298df251addcf4770b30b`

Sharp borders, grids and accent bars are intentional. The small green panel's bracket extent and the first quote's text placement show concrete alignment issues.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| decorators-01 | P2 | defect | Green status brackets: Green corner brackets extend well beyond the ONLINE panel and around its caption. | Anchor the brackets to the visible ONLINE panel bounds. |
| decorators-02 | P2 | defect | First quote padding: The first quote starts almost against the thick teal accent bar. | Add a consistent left content inset. |
| decorators-03 | P2 | proposal | Page title: UI Decorators is small relative to the unused screen area. | Increase title size modestly. |
| decorators-04 | P2 | proposal | Subtitle type: Composable visual effects via .decorate() is tiny. | Use a readable subtitle with monospace API text. |
| decorators-05 | P2 | proposal | Column headings: with_brackets(), with_grid_bg() and with_quote() are small and low-emphasis. | Add human-readable headings above the API names. |
| decorators-06 | P2 | proposal | Bracket dimensions: The cyan and amber brackets have no length annotation. | Show bracket length and stroke width in captions. |
| decorators-07 | P2 | proposal | Status title: SYSTEM STATUS is small inside a large panel. | Increase title size or reduce panel height. |
| decorators-08 | P2 | proposal | Status message: All systems operational uses very tiny lettering. | Increase body text size. |
| decorators-09 | P2 | proposal | Warning text: Hull integrity at 47% is smaller than the available panel allows. | Enlarge it and separate the percentage. |
| decorators-10 | P2 | proposal | Warning punctuation: ! WARNING ! uses text punctuation as the icon. | Use a distinct warning symbol. |
| decorators-11 | P2 | proposal | Online marker: The green dot is tiny relative to its status word. | Increase it slightly and align its center with the label. |
| decorators-12 | P2 | proposal | Bracket caption: The caption sits inside the overextended green brackets. | Move it outside specimen bounds after correcting the anchor. |
| decorators-13 | P2 | proposal | Grid labels: The first two grids have no individual names. | Label them Coarse 20px and Dense grid with actual spacing. |
| decorators-14 | P2 | proposal | Grid dot: The teal center dot has no stated role. | Label it as the center marker. |
| decorators-15 | P2 | proposal | Dense grid: Dense lines have no scale reference. | Add a small spacing ruler outside the panel. |
| decorators-16 | P2 | proposal | Sector grid: SECTOR 7-G has no explanation of combined decorators. | Caption it Grid + brackets. |
| decorators-17 | P2 | proposal | Grid contrast: The three grid specimens vary in brightness without numeric captions. | Show line opacity values for comparison. |
| decorators-18 | P2 | proposal | Quote examples: The first quote lacks attribution while the others include it. | Label it No attribution to clarify the variant. |
| decorators-19 | P2 | proposal | First quote wrapping: The line breaks after you and leaves do. on a short second line. | Adjust content width or text size for a balanced wrap. |
| decorators-20 | P2 | proposal | Quote attribution: Commander Shepard and GLaDOS labels are tiny. | Increase attribution size while keeping it secondary. |
| decorators-21 | P2 | proposal | Quote bar widths: Teal, amber and green accent bars vary markedly. | Show their configured widths. |
| decorators-22 | P2 | proposal | Quote alignment: The first quote starts farther left than the later quotes. | Use a shared content inset across examples. |
| decorators-23 | P2 | proposal | Column height: The three specimen stacks end on different baselines. | Use aligned example rows with separate captions. |
| decorators-24 | P2 | proposal | Lower blank space: Nearly the lower third is empty. | Add annotated decorator-parameter references. |
| decorators-25 | P2 | proposal | Color mapping: Teal, amber and green denote both status and different examples. | Add captions separating semantic color from decorator behavior. |

## dialog confirm

[Reviewed image](../../docs/design-audit/originals/dialog_confirm_720p.png) · [Current baseline](../../screenshot-baselines/screens/dialog_confirm_720p.png) · [Visual review](../../design-audit.html#screen=dialog_confirm)

Baseline SHA-256: `d843a0a32899ab5415002b53ce8c1135365767d681d3358c3b0df865c50ffbda`

A clear two-action confirmation is captured over an intentionally dimmed backdrop. Eighteen defensible refinements are listed; focus, dismissal and renderer behavior cannot be judged from this image.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| dialog_confirm-01 | P2 | proposal | Dialog title: Apply changes? does not name the affected settings. | Use Apply display changes?. |
| dialog_confirm-02 | P2 | proposal | Change summary: The dialog describes new settings without listing changed values. | Show a compact old-to-new summary. |
| dialog_confirm-03 | P2 | proposal | Restart wording: Restart the renderer uses implementation terminology. | Describe the visible display interruption. |
| dialog_confirm-04 | P2 | proposal | Recovery wording: Revert from the settings menu afterwards gives no precise route. | Name the Display settings path. |
| dialog_confirm-05 | P2 | proposal | Display recovery: The message assumes the user can still see the screen after applying. | Offer an explicit timed-revert design for unsupported display changes. |
| dialog_confirm-06 | P2 | proposal | Body length: The explanation wraps across three widely tracked lines. | Use a standard body face and shorter sentences. |
| dialog_confirm-07 | P2 | proposal | Body spacing: The title and message are separated by a large gap. | Reduce the title-to-body spacing. |
| dialog_confirm-08 | P2 | proposal | Button size: Cancel and Apply labels are small within their buttons. | Increase action-label size. |
| dialog_confirm-09 | P2 | proposal | Apply label: Apply repeats the vague title verb. | Use Apply display settings. |
| dialog_confirm-10 | P2 | proposal | Cancel meaning: Cancel does not state that current settings remain. | Use Keep current settings where space permits. |
| dialog_confirm-11 | P2 | proposal | Button alignment: The action row is inset farther than the message text. | Align its right edge with the body text column. |
| dialog_confirm-12 | P2 | proposal | Button gap: Cancel and Apply are separated by a large empty gap. | Use a consistent compact action-group gap. |
| dialog_confirm-13 | P2 | proposal | Dialog bottom: Substantial blank space follows the buttons. | Reduce dialog height to fit the content. |
| dialog_confirm-14 | P2 | proposal | Dialog edge: The dark panel has no clear border or shadow against the backdrop. | Add subtle edge separation. |
| dialog_confirm-15 | P2 | proposal | Current context: Visible 2560×1440 and 120Hz values do not identify pending versus current. | Mark the pending fields in the backdrop before dimming. |
| dialog_confirm-16 | P2 | proposal | Changed-field cue: All backdrop rows share the same appearance. | Add a small changed marker to edited settings. |
| dialog_confirm-17 | P2 | proposal | Modal heading hierarchy: The dialog title is only modestly larger than body text. | Increase heading weight. |
| dialog_confirm-18 | P2 | proposal | Backdrop chrome: Harbour Studio and navigation remain visible but do not identify the confirmation source. | Add a Display changes caption within the dialog. |

## dialog danger

[Reviewed image](../../docs/design-audit/originals/dialog_danger_720p.png) · [Current baseline](../../screenshot-baselines/screens/dialog_danger_720p.png) · [Visual review](../../design-audit.html#screen=dialog_danger)

Baseline SHA-256: `f33510b0feb265c69d7b72d0e4cf8a292fb2c112a72ba156becb98ffe68d631e`

The destructive action is clearly red and the warning is explicit. Eighteen grounded improvements cover save identification, wording and the reused background fixture.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| dialog_danger-01 | P2 | proposal | Save title: Delete save? omits the save's identity. | Use the title Delete Chapter 3 autosave? |
| dialog_danger-02 | P2 | proposal | Save name wrapping: Chapter 3 - Autosave is split across body lines. | Place the save name on its own line. |
| dialog_danger-03 | P2 | proposal | Save metadata: Only the save name identifies the destructive target. | Add timestamp and playtime for verification. |
| dialog_danger-04 | P2 | proposal | Save preview: The dialog has no visual identifier for the save slot. | Add a small slot thumbnail or chapter emblem. |
| dialog_danger-05 | P2 | proposal | Warning hierarchy: Permanent deletion and the object name use equal weight. | Emphasize the save name and irreversible consequence separately. |
| dialog_danger-06 | P2 | proposal | Warning redundancy: Permanently deletes and cannot be undone repeat the same point. | Use one direct irreversible-action sentence. |
| dialog_danger-07 | P2 | proposal | Delete label: Delete is generic. | Use Delete save. |
| dialog_danger-08 | P2 | proposal | Cancel label: Cancel is less explicit than the destructive alternative. | Use Keep save. |
| dialog_danger-09 | P2 | proposal | Danger icon: Only the button color marks the destructive action visually. | Add a restrained warning icon beside the title. |
| dialog_danger-10 | P2 | proposal | Body font: The save name uses handwritten characters. | Use a readable body font for the target identifier. |
| dialog_danger-11 | P2 | proposal | Title gap: A large gap separates title from the warning. | Tighten the vertical rhythm. |
| dialog_danger-12 | P2 | proposal | Button text: Action labels are small within 132px buttons. | Increase label size. |
| dialog_danger-13 | P2 | proposal | Action spacing: Cancel and Delete have a wide gap without a grouping cue. | Use a standard action-row gap. |
| dialog_danger-14 | P2 | proposal | Bottom padding: Large empty space follows the action row. | Reduce panel height. |
| dialog_danger-15 | P2 | proposal | Dialog boundary: The panel blends into the dark display rows behind it. | Add a subtle border or shadow. |
| dialog_danger-16 | P2 | proposal | Backdrop section: Save slots appears above Resolution and Refresh rate settings. | Use a small save-slot list behind this fixture. |
| dialog_danger-17 | P2 | proposal | Sidebar context: Display remains selected behind the save deletion dialog. | Use a Saves navigation item for this demonstration. |
| dialog_danger-18 | P2 | proposal | Selection context: No visible background item corresponds to Chapter 3 autosave. | Highlight the target save slot behind the scrim. |

## dialog fyi

[Reviewed image](../../docs/design-audit/originals/dialog_fyi_720p.png) · [Current baseline](../../screenshot-baselines/screens/dialog_fyi_720p.png) · [Visual review](../../design-audit.html#screen=dialog_fyi)

Baseline SHA-256: `f5c5a1e0d5fc04b61888ef4d7c8bc6502e1f5ee5819a71f2349cba386e0f54d8`

The three-action unsaved-changes fixture is readable. Eighteen proposals improve the decision and backdrop context; no unseen save or dismissal behavior is claimed broken.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| dialog_fyi-01 | P2 | proposal | Document identity: The warning refers only to this document. | Show the document name prominently. |
| dialog_fyi-02 | P2 | proposal | Title question: Unsaved changes states a condition without naming the decision. | Use Save changes before leaving?. |
| dialog_fyi-03 | P2 | proposal | Trigger context: The text does not identify whether closing, switching or quitting caused the prompt. | Name the pending action. |
| dialog_fyi-04 | P2 | proposal | Body verbosity: What would you like to do? adds length without decision information. | Replace it with the consequence of leaving. |
| dialog_fyi-05 | P2 | proposal | Save label: Save does not say whether it also completes the pending action. | Use Save and close when that is the demonstrated flow. |
| dialog_fyi-06 | P2 | proposal | Keep editing: Keep editing clearly preserves work but is visually secondary to Save. | Keep it secondary and add a concise return-to-editor cue. |
| dialog_fyi-07 | P2 | proposal | Discard wording: Discard changes does not mention permanent loss of unsaved edits. | Use Discard unsaved changes or supporting text. |
| dialog_fyi-08 | P2 | proposal | Discard styling: Discard changes looks like plain text rather than an action. | Add a clear low-emphasis button boundary. |
| dialog_fyi-09 | P2 | proposal | Destructive distinction: The discard action uses the same pale text as ordinary content. | Use a restrained destructive accent. |
| dialog_fyi-10 | P2 | proposal | Action spacing: Three actions have uneven apparent widths and gaps. | Use a structured row with discard separate from keep/save. |
| dialog_fyi-11 | P2 | proposal | Button labels: Keep editing and Save use very small type. | Increase action-label size. |
| dialog_fyi-12 | P2 | proposal | Body measure: The sentence runs across most of a wide dialog. | Constrain body text to a comfortable line length. |
| dialog_fyi-13 | P2 | proposal | Heading weight: The title differs little from the body style. | Increase heading weight. |
| dialog_fyi-14 | P2 | proposal | Bottom space: A large blank band remains below the buttons. | Reduce the panel height. |
| dialog_fyi-15 | P2 | proposal | Modal boundary: The dark panel has little separation from the dimmed background. | Add a subtle outline or shadow. |
| dialog_fyi-16 | P2 | proposal | Backdrop content: Editor is shown above display-setting rows. | Use a document-editor preview behind the fixture. |
| dialog_fyi-17 | P2 | proposal | Backdrop navigation: Display remains selected for an editor warning. | Use a matching Documents or Editor navigation item. |
| dialog_fyi-18 | P2 | proposal | Unsaved cue: No background document tab or dirty marker identifies pending edits. | Add an unsaved marker to the previewed document. |

## dialog info

[Reviewed image](../../docs/design-audit/originals/dialog_info_720p.png) · [Current baseline](../../screenshot-baselines/screens/dialog_info_720p.png) · [Visual review](../../design-audit.html#screen=dialog_info)

Baseline SHA-256: `02a6bcee771ca3e86d5a818c38925a8afb96a088d0833a7d95f84af6d0f27fd1`

This one-button connection notice supports sixteen distinct refinements. More would require inventing interaction defects or padding a sparse specimen.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| dialog_info-01 | P2 | proposal | Action label: OK does not name the destination described in the message. | Use Return to main menu. |
| dialog_info-02 | P2 | proposal | Action timing: Returning to the main menu sounds automatic despite the acknowledgment button. | State whether pressing the button starts the return. |
| dialog_info-03 | P2 | proposal | Connection identity: The message names only the host. | Show the session or host name when available. |
| dialog_info-04 | P2 | proposal | Loss explanation: Disconnected from the host repeats Connection lost. | Use the body to explain the known consequence. |
| dialog_info-05 | P2 | proposal | Progress consequence: The notice does not say whether match progress is retained. | Add a concise save/progress statement where supported. |
| dialog_info-06 | P2 | proposal | Recovery option: The visible action offers no reconnect path. | If the demo supports it, add Retry beside return. |
| dialog_info-07 | P2 | proposal | Connection symbol: No icon distinguishes the notice from ordinary information. | Add a small disconnected-network symbol. |
| dialog_info-08 | P2 | proposal | Body spacing: Two short lines sit far below the title. | Reduce title-to-body spacing. |
| dialog_info-09 | P2 | proposal | Action alignment: The single button is offset toward the right of the panel. | Align it consistently with the content column or center it deliberately. |
| dialog_info-10 | P2 | proposal | Action type: OK is tiny inside the teal button. | Increase its label size. |
| dialog_info-11 | P2 | proposal | Panel height: The panel has substantial empty space below the action. | Fit its height to the content. |
| dialog_info-12 | P2 | proposal | Panel boundary: The notice lacks strong edge separation from the dark backdrop. | Add a subtle border or shadow. |
| dialog_info-13 | P2 | proposal | Backdrop section: Multiplayer appears above display settings. | Use a lobby or session list behind the fixture. |
| dialog_info-14 | P2 | proposal | Sidebar selection: Display remains selected under a multiplayer notice. | Use a Multiplayer navigation item. |
| dialog_info-15 | P2 | proposal | Session context: No visible lobby identifies the disconnected session. | Show its selected session row behind the scrim. |
| dialog_info-16 | P2 | proposal | Heading style: Connection lost uses the same handwritten face as generic demo text. | Use a clearer UI heading face. |

## dialog prompt

[Reviewed image](../../docs/design-audit/originals/dialog_prompt_720p.png) · [Current baseline](../../screenshot-baselines/screens/dialog_prompt_720p.png) · [Visual review](../../design-audit.html#screen=dialog_prompt)

Baseline SHA-256: `412549308ed04ca639a1f080295d87809a6208253e88efafcea28235ba13c299`

The rename field is visibly preselected, which is appropriate for this fixture. Twenty proposals improve target identity, field layout and supporting context without judging unseen validation.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| dialog_prompt-01 | P2 | proposal | Current file: Rename file does not show the existing name outside the editable field. | Add a current-file caption. |
| dialog_prompt-02 | P2 | proposal | File location: The prompt does not identify the file's folder. | Show a compact parent-folder path. |
| dialog_prompt-03 | P2 | proposal | Field label: Enter a new name for this file is a long instruction. | Use the persistent label New name. |
| dialog_prompt-04 | P2 | proposal | Input alignment: The input begins farther right than the title and instruction. | Align the field to the content column. |
| dialog_prompt-05 | P2 | proposal | Input width: The field ends before the action group's right edge. | Use a shared right alignment. |
| dialog_prompt-06 | P2 | proposal | Filename typography: untitled_map uses handwritten characters. | Use a clear filename text face. |
| dialog_prompt-07 | P2 | proposal | Selection fill: The selected text has a low-contrast blue highlight over purple. | Increase selected-text foreground/background separation. |
| dialog_prompt-08 | P2 | proposal | Field boundary: The purple field has no distinct border against its surrounding panel. | Add a subtle input border. |
| dialog_prompt-09 | P2 | proposal | Field color: Purple is stronger than the primary Rename action. | Use a quieter field fill while preserving selection visibility. |
| dialog_prompt-10 | P2 | proposal | Extension handling: The visible name has no extension context. | Show the preserved extension separately if the fixture represents a file with one. |
| dialog_prompt-11 | P2 | proposal | Name constraints: No filename rules are displayed. | Add concise supported-name guidance below the input. |
| dialog_prompt-12 | P2 | proposal | Duplicate context: The prompt has no space reserved for a duplicate-name message. | Reserve a compact validation line without asserting a current error. |
| dialog_prompt-13 | P2 | proposal | Rename preview: Only the editable value communicates the result. | Add a small resulting-filename preview when extension handling matters. |
| dialog_prompt-14 | P2 | proposal | Rename label: Rename is clear but small. | Increase action-label size. |
| dialog_prompt-15 | P2 | proposal | Cancel placement: Cancel begins well inside the input's left edge. | Align the action group to the field's right edge with a regular gap. |
| dialog_prompt-16 | P2 | proposal | Bottom space: The panel leaves a large empty region below the actions. | Reduce dialog height. |
| dialog_prompt-17 | P2 | proposal | Title spacing: The heading and instruction are separated by a large gap. | Tighten the vertical spacing. |
| dialog_prompt-18 | P2 | proposal | Backdrop content: Files appears above Resolution and Refresh rate rows. | Use a compact file list behind the fixture. |
| dialog_prompt-19 | P2 | proposal | Backdrop navigation: Display is selected despite a file rename. | Select Files in the demo navigation. |
| dialog_prompt-20 | P2 | proposal | Target selection: No background file row matches untitled_map. | Highlight the renamed file in the backdrop. |

## divider lab

[Reviewed image](../../docs/design-audit/originals/divider_lab_720p.png) · [Current baseline](../../screenshot-baselines/screens/divider_lab_720p.png) · [Visual review](../../design-audit.html#screen=divider_lab)

Baseline SHA-256: `39a242480c241637e3e611c5ac408eb11681f5c73870d3f88c7d64f1bcee449b`

The two draggable dividers and the non-moving rule are intentional. Twenty-two grounded proposals make sizing and the distinction between specimens clearer.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| divider_lab-01 | P2 | defect | Pane labels: left 300px, right fills the rest, top 150px and bottom fills the rest touch pane edges. | Add a consistent small text inset. |
| divider_lab-02 | P2 | proposal | Title: The title combines the component name and all instructions. | Separate Divider examples from a short instruction. |
| divider_lab-03 | P2 | proposal | Vertical divider: The narrow bar has no visible grip. | Add a centered grip marker. |
| divider_lab-04 | P2 | proposal | Horizontal divider: The horizontal bar is a plain line like the non-moving rule. | Add a grip marker to distinguish it. |
| divider_lab-05 | P2 | proposal | Divider labels: Neither draggable bar has its own caption. | Label Width divider and Height divider outside the panes. |
| divider_lab-06 | P2 | proposal | Left dimension: left 300px omits whether 300 is requested or actual width. | Show the measured width explicitly. |
| divider_lab-07 | P2 | proposal | Right dimension: right fills the rest gives no resolved measurement. | Display the current right-pane width. |
| divider_lab-08 | P2 | proposal | Top dimension: top 150px omits a measurement guide. | Add a vertical dimension bracket. |
| divider_lab-09 | P2 | proposal | Bottom dimension: bottom fills the rest gives no resulting height. | Display the current bottom-pane height. |
| divider_lab-10 | P2 | proposal | Pane total: The total split width is not shown. | Add the overall container width beside the first example. |
| divider_lab-11 | P2 | proposal | Height total: The total split height is not shown. | Add the overall height beside the second example. |
| divider_lab-12 | P2 | proposal | Minimum widths: No boundary values are visible for the vertical split. | Show its configured min/max values. |
| divider_lab-13 | P2 | proposal | Minimum heights: No range is given for the horizontal split. | Show its configured height limits. |
| divider_lab-14 | P2 | proposal | Ratio comparison: Only pixel measurements appear. | Add the resulting percentage split beside each pixel value. |
| divider_lab-15 | P2 | proposal | Reset control: No visible action restores 300px and 150px. | Add Reset dividers. |
| divider_lab-16 | P2 | proposal | Drag feedback: No numeric delta is reserved near the bars. | Add a drag-delta readout beside each specimen. |
| divider_lab-17 | P2 | proposal | Pane content: The large empty panes provide little reflow evidence. | Add a short paragraph or grid that responds to resizing. |
| divider_lab-18 | P2 | proposal | Vertical specimen height: The first split consumes a large area for two short labels. | Reduce its height enough to fit measurement captions. |
| divider_lab-19 | P2 | proposal | Horizontal specimen labeling: Top and bottom text float at vertical centers. | Place labels in consistent top-left positions. |
| divider_lab-20 | P2 | proposal | Rule caption: The bottom API explanation is tiny. | Increase its size and use monospace for divider(). |
| divider_lab-21 | P2 | proposal | Rule semantics: Movement ignored describes implementation rather than visual purpose. | Label it Static 1px separator. |
| divider_lab-22 | P2 | proposal | Rule boundary: The thin bottom rule blends into the black background. | Add endpoint markers outside it for inspection. |

## drag drop

[Reviewed image](../../docs/design-audit/originals/drag_drop_720p.png) · [Current baseline](../../screenshot-baselines/screens/drag_drop_720p.png) · [Visual review](../../design-audit.html#screen=drag_drop)

Baseline SHA-256: `e65a7d9f2c2b1c78ce6b397b586481a1d4e5fb0705af045f1b93a8605bab8293`

The three-column board is a drag-and-drop demonstration. Proposals improve card affordance and board information without asserting unseen drag or keyboard failures.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| drag_drop-01 | P2 | defect | Card padding: Design mockups, API endpoints and other titles begin almost on their card borders. | Add comfortable horizontal text padding. |
| drag_drop-02 | P2 | proposal | Board title: Sprint Board uses a large handwritten heading. | Use a clearer heading face while retaining the casual style. |
| drag_drop-03 | P2 | proposal | Instruction type: The instruction is almost as large as column headings. | Reduce it slightly and improve hierarchy. |
| drag_drop-04 | P2 | proposal | Instruction scope: Drag cards between columns omits reordering within a column. | Mention reorder behavior if supported by this demo. |
| drag_drop-05 | P2 | proposal | Card grip: Cards have no visible drag-handle cue. | Add a small grip on each card. |
| drag_drop-06 | P2 | proposal | Column counts: Counts are embedded in oversized headings. | Use compact count badges beside the titles. |
| drag_drop-07 | P2 | proposal | To Do heading: To Do differs in capitalization from In Progress. | Use consistent sentence casing. |
| drag_drop-08 | P2 | proposal | Column boundaries: Columns and cards are close gray tones. | Increase boundary distinction modestly. |
| drag_drop-09 | P2 | proposal | Card borders: Very thin outlines are hard to see. | Use a slightly stronger card border or subtle shadow. |
| drag_drop-10 | P2 | proposal | Card density: Tall cards contain only one short text line. | Reduce card height or add useful metadata. |
| drag_drop-11 | P2 | proposal | Board whitespace: Large empty areas remain under all three stacks. | Add a quiet drop-area cue at each stack's end. |
| drag_drop-12 | P2 | proposal | Done status: Done cards look identical to unfinished cards. | Add a completion mark or quieter completed treatment. |
| drag_drop-13 | P2 | proposal | Task type: Fix login bug is visually identical to design and documentation tasks. | Add a small labeled task-type tag. |
| drag_drop-14 | P2 | proposal | Priority: The board has no visible priority distinction. | Add a concise priority marker to representative sample cards. |
| drag_drop-15 | P2 | proposal | Assignees: No card shows who owns a task. | Add compact assignee initials to enrich the realistic specimen. |
| drag_drop-16 | P2 | proposal | Task identity: Titles are the only card identifiers. | Add short task IDs for tracking moves. |
| drag_drop-17 | P2 | proposal | Column accent: All columns share the same color despite distinct states. | Add restrained status accents to headers. |
| drag_drop-18 | P2 | proposal | Active target explanation: The screenshot gives no preview of valid drop locations. | Add a static annotated example or separate drag-state capture. |
| drag_drop-19 | P2 | proposal | Move feedback: No visible status area is available to report a completed move. | Add a small last-move message below the board. |
| drag_drop-20 | P2 | proposal | Alternative action: Only dragging is advertised. | Add a visible Move to control for an accessible alternative. |
| drag_drop-21 | P2 | proposal | Reset demo: There is no visible way to restore the initial arrangement. | Add Reset board. |
| drag_drop-22 | P2 | proposal | Sprint context: Sprint Board has no sprint name or dates. | Add a compact sample sprint identifier. |
| drag_drop-23 | P2 | proposal | Progress overview: 3, 3, 2 counts require mental aggregation. | Add an overall 2 of 8 completed summary. |
| drag_drop-24 | P2 | proposal | Column spacing: Wide gutters consume space that could improve card padding. | Reduce gutter width modestly. |
| drag_drop-25 | P2 | proposal | Outer frame: The board ends above a broad black bottom band. | Center the board vertically or use the space for instructions and move feedback. |

## dropdowns

[Reviewed image](../../docs/design-audit/originals/dropdowns_720p.png) · [Current baseline](../../screenshot-baselines/screens/dropdowns_720p.png) · [Visual review](../../design-audit.html#screen=dropdowns)

Baseline SHA-256: `b54aa692fcd71d11e5f288d32dddeb502353d5bad29835ca1962c6e23f982c73`

All six controls are closed, including an intentionally disabled specimen. Findings improve labeling and comparison; open-menu behavior is not inferred.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| dropdowns-01 | P2 | proposal | Title inset: Dropdown Showcase begins at the header's left edge. | Add internal title padding. |
| dropdowns-02 | P2 | proposal | Section header treatment: Card headings use the same teal fill as dropdown controls. | Use neutral headings so controls are identifiable. |
| dropdowns-03 | P2 | proposal | Basic label: Alpha names the value without the selected category. | Add a persistent external label. |
| dropdowns-04 | P2 | proposal | Basic chevron: The closed indicator is a literal v. | Use a consistent chevron glyph. |
| dropdowns-05 | P2 | proposal | With Label row: Pet and Cat share one teal pill with weak separation. | Separate label and control visually. |
| dropdowns-06 | P2 | proposal | Pet alignment: Pet is centered in a wide label region. | Left-align the label near its control. |
| dropdowns-07 | P2 | proposal | Many options: Many Options 12 does not name the month category. | Use Month, 12 options. |
| dropdowns-08 | P2 | proposal | Month control width: January occupies a narrower field than its card header. | Use a shared field width across comparable examples. |
| dropdowns-09 | P2 | proposal | Scrolling explanation: Twelve options, so the list scrolls does not give the visible-row limit. | State the menu's visible-row count. |
| dropdowns-10 | P2 | proposal | Custom marker: Option A[-] uses punctuation whose meaning is unexplained. | Show a small open/closed marker legend. |
| dropdowns-11 | P2 | proposal | Custom variant label: Option A is generic beside the concrete Pet and Month examples. | Use a meaningful category label. |
| dropdowns-12 | P2 | proposal | Disabled specimen: Frozen is deliberately dim but only its heading explains availability. | Add a readable external state caption. |
| dropdowns-13 | P2 | proposal | Disabled explanation: Not focusable and does not open describes internal behavior. | Use Disabled example with concise supporting detail. |
| dropdowns-14 | P2 | proposal | Custom style caption: Custom border, fill and corner radius omits values. | List the configured style values. |
| dropdowns-15 | P2 | proposal | Ruby naming: Ruby implies a gemstone or color but has no category label. | Add Gemstone as its external label. |
| dropdowns-16 | P2 | proposal | Custom comparison: Ruby has no adjacent default-styled control. | Add a matched default specimen for comparison. |
| dropdowns-17 | P2 | proposal | Description type: All six descriptions are very small. | Increase body text size. |
| dropdowns-18 | P2 | proposal | Description placement: Descriptions sit far below controls in tall cards. | Tighten the control-to-description gap. |
| dropdowns-19 | P2 | proposal | Card height: Each card contains substantial unused lower space. | Reduce height or add useful parameter captions. |
| dropdowns-20 | P2 | proposal | Card outlines: Bright thin outlines compete with subtle custom control borders. | Quiet the card outline and preserve specimen emphasis. |
| dropdowns-21 | P2 | proposal | Footer padding: The selection summary begins at its bar's edge. | Inset it consistently. |
| dropdowns-22 | P2 | proposal | Footer coverage: The summary mentions Alpha, Cat and January but omits other selectable specimens. | Include all active example values or label the summary scope. |
| dropdowns-23 | P2 | proposal | Footer phrasing: A long sentence is harder to scan than labeled values. | Use Basic: Alpha, Pet: Cat, Month: January as separate fields. |
| dropdowns-24 | P2 | proposal | Open-state evidence: The gallery shows no expanded menu. | Add a separate baseline with one representative menu open. |
| dropdowns-25 | P2 | proposal | Menu placement context: Closed controls do not reveal the intended popup direction or bounds. | Add a small placement diagram outside the specimens. |

## empire tycoon

[Reviewed image](../../docs/design-audit/originals/empire_tycoon_720p.png) · [Current baseline](../../screenshot-baselines/screens/empire_tycoon_720p.png) · [Visual review](../../design-audit.html#screen=empire_tycoon)

Baseline SHA-256: `6b4ca431ea1b1b5d34a5f0d0a8a115102b559643e63ca20359f4296ac7815a61`

The dashboard follows the Dream Incorporated mock. Proposals improve metric semantics, project actions and readability within the playful illustrated design.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| empire_tycoon-01 | P2 | proposal | Happiness face: The 85% happiness gauge uses a neutral or unhappy-looking face. | Use an expression consistent with high happiness. |
| empire_tycoon-02 | P2 | proposal | Resources symbol: Resources repeats a face-like center symbol. | Use a gear or resource icon matching the mock. |
| empire_tycoon-03 | P2 | proposal | Gauge values: 85% and 60% appear in very small outlined text. | Use solid readable numeric labels. |
| empire_tycoon-04 | P2 | proposal | Gauge scale: The semicircles have ticks but no endpoint labels. | Mark 0 and 100 consistently. |
| empire_tycoon-05 | P2 | proposal | Gauge fill distinction: Resources fill is close in color to its track. | Increase filled-versus-unfilled contrast. |
| empire_tycoon-06 | P2 | proposal | Cash emphasis: The large balance dominates the actionable dashboard. | Reduce its scale slightly or strengthen section headings. |
| empire_tycoon-07 | P2 | proposal | Currency symbol: A star coin accompanies a dollar amount. | Choose one coherent currency convention. |
| empire_tycoon-08 | P2 | proposal | Selected department: Rides has no strong selected marker distinct from the other colored tabs. | Add an inset selection outline or check marker. |
| empire_tycoon-09 | P2 | proposal | Department icons: Thin line icons are small inside large tab buttons. | Increase icon weight and optical size. |
| empire_tycoon-10 | P2 | proposal | Upgrade badge: The red exclamation overlaps the tab edge. | Inset it or reserve a badge corner. |
| empire_tycoon-11 | P2 | proposal | Production units: 5,200/min etc. do not identify units produced. | Use items/min or an appropriate explicit unit. |
| empire_tycoon-12 | P2 | proposal | Production arrows: Green up arrows lack a stated comparison period. | Add a trend caption such as versus previous minute. |
| empire_tycoon-13 | P2 | proposal | Production status: All systems running smoothly is extremely small. | Increase its size and move it into a clear status row. |
| empire_tycoon-14 | P2 | proposal | Project title bars: Cyan title backgrounds resemble progress fills. | Separate titles from progress tracks more clearly. |
| empire_tycoon-15 | P2 | proposal | Project progress: 80% and 10% sit far from the bars' starting edges. | Align values consistently at each track's end. |
| empire_tycoon-16 | P2 | proposal | Robot subtitle: 3,100/min under a mascot project has no named metric. | Label the rate or replace it with remaining time. |
| empire_tycoon-17 | P2 | proposal | Rollercoaster subtitle: Expand the fun is promotional rather than useful status. | Show remaining time or expected benefit. |
| empire_tycoon-18 | P2 | proposal | Project selection: No project is visibly selected for Prioritize. | Add a selected-project marker. |
| empire_tycoon-19 | P2 | proposal | Prioritize action: Prioritize does not name its target. | Show Prioritize rollercoaster or a target caption. |
| empire_tycoon-20 | P2 | proposal | Speed Up action: Speed Up gives no cost or duration. | Show its resource cost and effect before activation. |
| empire_tycoon-21 | P2 | proposal | New Project action: All three actions share equal yellow emphasis. | Give New Project or the principal action a clear hierarchy. |
| empire_tycoon-22 | P2 | proposal | Milestone identity: Next Milestone 65% does not name the milestone. | Show its goal and reward. |
| empire_tycoon-23 | P2 | proposal | Milestone number: 65% is tiny inside the progress bar. | Increase the numeric label or place it above. |
| empire_tycoon-24 | P2 | proposal | Chat channels: GlobalChat and DevTeam messages are very small in a corner panel. | Increase text size and distinguish channel labels. |
| empire_tycoon-25 | P2 | proposal | Bottom navigation: Shop, Settings and Leaderboard labels are tiny outlined text. | Use solid text at a readable size and equal icon spacing. |

## entity index lab

[Reviewed image](../../docs/design-audit/originals/entity_index_lab_720p.png) · [Current baseline](../../screenshot-baselines/screens/entity_index_lab_720p.png) · [Visual review](../../design-audit.html#screen=entity_index_lab)

Baseline SHA-256: `5b64ec912ff78533d2eab5f8b937f66c47909529a1ed0bfd6b9f1bdb6b6f96b7`

The device-to-port relationship reads clearly. The performance comparison that motivates this demo is visually subordinate; proposals preserve its index demonstration.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| entity_index_lab-01 | P2 | proposal | Patch Bay title: The title names an audio tool but not the indexing demonstration. | Add a small Secondary index demo subtitle beside Patch Bay. |
| entity_index_lab-02 | P2 | proposal | Introductory sentence: The single sentence stretches across most of the screen in small spaced lettering. | Break the instruction and implementation explanation into separate lines. |
| entity_index_lab-03 | P2 | proposal | Metric footer: The green sentence places 6228 and 24 inside prose. | Present Scan 6,228 and Index 24 as adjacent labeled counters. |
| entity_index_lab-04 | P2 | proposal | Collection size: The collection size 1030 sits at the end of the metric sentence. | Give collection size its own neutral counter beside the comparison. |
| entity_index_lab-05 | P2 | proposal | Equality statement: The result-equivalence statement is detached below the performance line. | Place a Results match: 24 indicator with the counters. |
| entity_index_lab-06 | P3 | proposal | Footer explanation: Both spellings return 24 requires knowledge of two code expressions. | Name Scan and Indexed lookup explicitly in the explanation. |
| entity_index_lab-07 | P2 | proposal | Mixer A card: The selected device is communicated only through its blue fill. | Add a selected marker at the leading edge of Mixer A. |
| entity_index_lab-08 | P3 | proposal | Device count: DEVICES does not state the number of listed devices. | Label the section Devices · 6. |
| entity_index_lab-09 | P2 | proposal | Port table title: Mixer A has the same small type scale as device names. | Increase the table title and pair it with a 4 ports summary. |
| entity_index_lab-10 | P3 | proposal | Port slot header: SLOT numbers start at 0 with no explanation. | Label the column Slot index to explain zero-based numbering. |
| entity_index_lab-11 | P2 | proposal | Slot column: The SLOT heading begins left of the numeric cell alignment. | Align the heading and all slot numbers to one inset. |
| entity_index_lab-12 | P2 | proposal | Port column: PORT starts noticeably left of Input L and Input R. | Align the PORT heading with the port-name cells. |
| entity_index_lab-13 | P2 | proposal | State column: STATE starts left of connected and idle. | Align the STATE heading with its cell labels. |
| entity_index_lab-14 | P2 | proposal | Status encoding: Connected is green while idle is muted italic text. | Add a filled status dot for connected and an outlined dot for idle. |
| entity_index_lab-15 | P3 | proposal | Device names: Mixer A and Preamp 1 mix letter and number identifiers without explanation. | Use a secondary device-type label to distinguish names from categories. |
| entity_index_lab-16 | P2 | proposal | Device badges: Each 4 ports count is pale blue and distant from the device name. | Use a compact count badge with brighter text at the same trailing inset. |
| entity_index_lab-17 | P3 | proposal | List panel: The device list has about 100 pixels of empty space below Monitor. | Reduce its height or add a compact total of 24 ports there. |
| entity_index_lab-18 | P2 | proposal | Detail panel: Four short table rows occupy less than half the tall panel. | Fit the table panel to its content and bring comparison counters upward. |
| entity_index_lab-19 | P3 | proposal | Panel spacing: The gap between device and detail panels is wider than either panel's internal padding. | Use a consistent 24-pixel inter-panel gap. |
| entity_index_lab-20 | P2 | proposal | Metric hierarchy: The main learning result is near the bottom, below a large empty panel region. | Move the scan/index comparison directly under the table. |
| entity_index_lab-21 | P3 | proposal | Port relationship: Device and port panels have no shared visual connector. | Add a small Selected device label or connector between Mixer A and its table. |
| entity_index_lab-22 | P2 | proposal | Technical typography: The numeric counters and device slots use the same handwritten face as titles. | Use a readable tabular face for slot indices and measurements. |
| entity_index_lab-23 | P3 | proposal | Send row: Send is the only idle port but looks otherwise identical to connected rows. | Give idle status a small neutral badge while retaining the same row geometry. |
| entity_index_lab-24 | P3 | proposal | Comparison scale: The footer states vastly different work counts without a visual scale. | Add proportionate scan and index bars under their counters. |
| entity_index_lab-25 | P3 | proposal | Selection instruction: Pick a device appears only in the page-wide sentence. | Place the short instruction directly under DEVICES. |

## example borders

[Reviewed image](../../docs/design-audit/originals/example_borders_720p.png) · [Current baseline](../../screenshot-baselines/screens/example_borders_720p.png) · [Visual review](../../design-audit.html#screen=example_borders)

Baseline SHA-256: `51cc14cf36b1a5a35d88dae3865353f39c1c53f782121bb1553552f40e49a64c`

Border variations are intentional fixtures. Improve labels and comparison framing; do not normalize the borders, colors, corners, or shadows under test.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| example_borders-01 | P2 | proposal | Thin Medium Thick cards: The width examples show names without numeric widths. | Add 1 px, 3 px, and 6 px captions outside the samples. |
| example_borders-02 | P2 | proposal | Blue Cyan Orange cards: Color names are visible but exact tested values are absent. | Add small color-code captions below these samples. |
| example_borders-03 | P2 | proposal | Width and color row: Width variants and hue variants share one undivided row. | Separate the row into labeled Width and Color groups. |
| example_borders-04 | P2 | proposal | Rounded card: Rounded names the effect without its radius. | Show the configured roundness in an external caption. |
| example_borders-05 | P2 | proposal | Pill card: The pill demonstration is nearly square. | Add a wider companion pill to demonstrate the same corner treatment on an elongated shape. |
| example_borders-06 | P2 | proposal | Top Only corner card: Top Only is also used in the per-side row for a different property. | Rename this sample Top corners only. |
| example_borders-07 | P2 | proposal | Diagonal corner card: Diagonal does not say which opposite corners are rounded. | Add a miniature corner diagram or explicit corner names. |
| example_borders-08 | P2 | proposal | Hard Shadow card: The shadow parameter is not identified in its label. | Caption the offset and spread below the sample. |
| example_borders-09 | P2 | proposal | Soft Shadow card: The soft shadow blends into the dark page without a comparison reference. | Place a neutral comparison backdrop behind the shadow examples only. |
| example_borders-10 | P3 | proposal | Corner and shadow grouping: Two different properties share the Corners & Shadows label. | Create separate subgroup captions for corners and shadows. |
| example_borders-11 | P2 | proposal | Dark Mode sample: Dark Mode blends into the page around its thin outline. | Give it an external sample-name caption so its boundary remains the test subject. |
| example_borders-12 | P3 | proposal | Neon Glow sample: The glow label and glow border use the same bright cyan. | Move the descriptive label below the sample to leave the glow unobscured. |
| example_borders-13 | P3 | proposal | Cozy sample: Cozy describes a mood instead of border behavior. | Add a subtitle naming its warm fill and soft border treatment. |
| example_borders-14 | P2 | proposal | Danger and Success: The two semantic samples use different punctuation prefixes. | Use a consistent icon-and-label pattern for both examples. |
| example_borders-15 | P2 | proposal | Outline Only sample: Outline Only uses much smaller text than neighboring themed samples. | Move the long name into an external caption at the shared caption size. |
| example_borders-16 | P2 | proposal | Interactive Button: Only the button's resting state is shown. | Add labeled static Rest, Hover, and Pressed previews beside the live control. |
| example_borders-17 | P2 | proposal | Interactive row: The large button and nested panel have different heights and top edges. | Align their sample frames while preserving each component's intrinsic dimensions. |
| example_borders-18 | P2 | proposal | Nested Panel: The nested example has no indication of which borders belong to parent and children. | Add a parent/child key outside the outlined panel. |
| example_borders-19 | P3 | proposal | Inner Card and Second: Second gives no information about the second inner border. | Rename it Outline child to distinguish its treatment. |
| example_borders-20 | P2 | proposal | Per-Side row: Top Only and Bottom Only omit the line widths. | Add edge-width captions outside each sample. |
| example_borders-21 | P2 | proposal | Left+Right sample: The label uses a compact plus sign unlike the other edge names. | Use Left and right with a tiny two-edge diagram. |
| example_borders-22 | P2 | proposal | Mixed sample: Three colors appear without identifying their edge assignment. | Add a four-edge legend naming each color and width. |
| example_borders-23 | P2 | proposal | Dotted samples: Thin, Medium, and Thick do not expose dash size or spacing. | Add numeric dash and gap captions beneath the dotted examples. |
| example_borders-24 | P2 | proposal | Dotted row placement: The last row sits only about 12 pixels above the bottom edge. | Increase the bottom safe margin or fit the gallery into a scrollable frame with a visible boundary. |
| example_borders-25 | P3 | proposal | Gallery subtitle: The small pipe-separated subtitle repeats the row names. | Replace it with a brief explanation that samples intentionally vary one property at a time. |

## fighter menu

[Reviewed image](../../docs/design-audit/originals/fighter_menu_720p.png) · [Current baseline](../../screenshot-baselines/screens/fighter_menu_720p.png) · [Visual review](../../design-audit.html#screen=fighter_menu)

Baseline SHA-256: `7e2e44add4913a4c124d23fbf2d3630ca98ca208f6a3ad0b720dc9442d3ad6c8`

The selected category is recognizable, but competing neon layers and decorative shapes weaken the menu hierarchy. Proposals retain the fighting-game composition represented in mocks.html.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| fighter_menu-01 | P2 | proposal | MainMenu heading: MainMenu visually joins the two words. | Set the heading as Main Menu with visible word spacing. |
| fighter_menu-02 | P2 | proposal | Heading shadow: The green offset duplicates large white letterforms. | Reduce the green offset to keep the title silhouette crisp. |
| fighter_menu-03 | P2 | proposal | Currency counter: 25000 P$ appears without numeric grouping. | Format the amount as 25,000 P$. |
| fighter_menu-04 | P3 | proposal | Currency panel: The currency banner has two faint decorative lines above the amount. | Simplify the panel to one top rule and align the value vertically. |
| fighter_menu-05 | P2 | proposal | Tab labels: Offline, Online, and Customize are low-contrast gray on black. | Increase inactive-tab label contrast while preserving the active green underline. |
| fighter_menu-06 | P2 | proposal | Selected Options tab: Orange letters on a pale cyan fill compete with the neon underline. | Use a dark label and reserve neon green for the active indicator. |
| fighter_menu-07 | P2 | proposal | Shoulder hints: The L and R key boxes differ in spacing from the adjacent tabs. | Use equal inset and equal-size keycaps at both ends. |
| fighter_menu-08 | P3 | proposal | Tab row: The active tab is taller than inactive tabs without a consistent baseline. | Align all tab label baselines and express selection with the top edge or underline. |
| fighter_menu-09 | P2 | proposal | Offline Mode backdrop label: The central Offline Mode label is partially covered by the selected menu ribbon. | Move or remove this secondary label so its full text remains visible. |
| fighter_menu-10 | P2 | proposal | System Options ribbon: The bright green selected row is far wider than its text. | Shorten its point and keep a predictable menu-column width. |
| fighter_menu-11 | P2 | proposal | Menu icon column: Slanted dark icon tiles use different apparent icon sizes. | Normalize the visual size and center each icon in its tile. |
| fighter_menu-12 | P2 | proposal | Game Options icon: Game Options and Button Settings share essentially the same controller silhouette. | Use a distinct button-mapping icon for Button Settings. |
| fighter_menu-13 | P2 | proposal | Unselected menu labels: Widely spaced gray serif labels require effort against black ribbons. | Raise contrast and reduce letter spacing in the six menu labels. |
| fighter_menu-14 | P3 | proposal | Menu row spacing: Thin pale gaps between the black ribbons vary in visual weight because of angled ends. | Use consistent row separation across the left column. |
| fighter_menu-15 | P2 | proposal | System preview heading: System Options appears twice in adjacent large and small treatments. | Keep one strong title and use the second line for category context. |
| fighter_menu-16 | P2 | proposal | Large System Options text: The white title uses a bright green offset over pale green; the web mock gives it a dark outline. | Use a dark outline or solid backing so System Options stays legible over the pale preview. |
| fighter_menu-17 | P2 | proposal | Gear illustration: The gear dominates the right preview while the category caption is very small. | Reduce the gear slightly and enlarge SYSTEM to a legible supporting label. |
| fighter_menu-18 | P3 | proposal | Preview frame: Several translucent green frames overlap at the right edge. | Keep one primary frame and one subtle offset accent. |
| fighter_menu-19 | P2 | proposal | Description text: The description occupies three centered lines with large letter spacing. | Left-align the description with a shorter measure and normal spacing. |
| fighter_menu-20 | P3 | proposal | Description panel: The black description panel has a large protruding diagonal left point. | Reduce the point so the reading area has a more regular shape. |
| fighter_menu-21 | P2 | proposal | Background character: The small character intersects the menu ribbons and pale floor geometry. | Place the character within an unobstructed central background zone. |
| fighter_menu-22 | P3 | proposal | Background pattern: Crosshairs and diagonal patterns remain strong around the navigation row. | Lower their contrast behind interactive labels. |
| fighter_menu-23 | P2 | proposal | Footer Select hint: A plus sign stands in for directional input while other actions use keycaps. | Use a consistent directional-pad glyph with the same keycap styling. |
| fighter_menu-24 | P2 | proposal | Footer change entry hint: L R share one wide keycap unlike the separate top shoulder hints. | Use two individual L and R keycaps consistently. |
| fighter_menu-25 | P2 | proposal | Footer spacing: The hints run across a dark gradient with uneven gaps around colons. | Use evenly spaced icon-label groups without colon punctuation. |

## file tree

[Reviewed image](../../docs/design-audit/originals/file_tree_720p.png) · [Current baseline](../../screenshot-baselines/screens/file_tree_720p.png) · [Visual review](../../design-audit.html#screen=file_tree)

Baseline SHA-256: `6ea213320a8544e7e4d099d449dedf3f3d022b0258684235b7525f41fe203dda`

The sample tree is stable and readable, but its information density and hierarchy are weak for the large window. Suggestions keep the synthetic tree behavior intact.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| file_tree-01 | P2 | proposal | File Tree heading: The title is small within a wide gray title bar. | Increase its size and give the bar a clear window-title treatment. |
| file_tree-02 | P2 | proposal | Outer panel: The top of the rounded panel touches the viewport boundary. | Add a consistent top margin around the complete window. |
| file_tree-03 | P2 | proposal | Tree body: Files occupy the left quarter of a very wide empty panel. | Reduce the panel width or add a selected-file detail pane. |
| file_tree-04 | P2 | proposal | File sizes: 47.1 KB, 6.0 KB, and 21.0 KB begin at different horizontal positions. | Right-align sizes in a dedicated column. |
| file_tree-05 | P2 | proposal | Column labels: The filename and size values have no column headers. | Add Name and Size headings above the first tree row. |
| file_tree-06 | P2 | proposal | Folder indicators: Expanded folders use a small handwritten v. | Replace it with a crisp disclosure chevron at a consistent size. |
| file_tree-07 | P2 | proposal | File versus folder rows: Folder identity relies mostly on a trailing slash. | Add folder and file icons while retaining the slash convention if desired. |
| file_tree-08 | P2 | proposal | Nested engine files: Indentation is the only guide connecting renderer files to engine. | Add subtle vertical hierarchy guides. |
| file_tree-09 | P2 | proposal | Filename typography: Handwritten letters make renderer.cpp and renderer.h less scannable. | Use a compact readable face for filenames and tabular numerals for sizes. |
| file_tree-10 | P2 | proposal | Root path: ~/projects/harbour is tiny inside a nearly full-width dark pill. | Increase its text size and present the path as a breadcrumb. |
| file_tree-11 | P2 | proposal | Sample tree control: Sample tree resembles an ordinary action with no mode status. | Show Sample data as a selected mode beside the path. |
| file_tree-12 | P3 | proposal | Home control: Home is detached from any visible location hierarchy. | Pair it with a home icon and align it with the breadcrumb start. |
| file_tree-13 | P3 | proposal | Up control: The short Up label has the same emphasis as Refresh. | Group Home and Up as navigation, separated from refresh actions. |
| file_tree-14 | P3 | proposal | Refresh control: Refresh has no icon in a toolbar of small text pills. | Add a compact refresh glyph to improve scanning. |
| file_tree-15 | P2 | proposal | Toolbar dimensions: The toolbar buttons are short relative to the file row text. | Increase button height and maintain a common label baseline. |
| file_tree-16 | P2 | proposal | Toolbar background: The dark toolbar strip spans empty space far past the last button. | Use a compact toolbar group or add a right-aligned mode indicator. |
| file_tree-17 | P2 | proposal | No selection footer: The status text is extremely small and low contrast. | Increase it to the same supporting-text size as the path. |
| file_tree-18 | P3 | proposal | Selection guidance: No selection reports state without suggesting the next step. | Use Select a file to view details in the empty detail region. |
| file_tree-19 | P3 | proposal | Tree summary: The window contains no summary of the visible sample. | Add a footer count of folders and files. |
| file_tree-20 | P2 | proposal | src assets tests hierarchy: Top-level folders share the same weight as leaf files. | Give top-level folder labels a slightly stronger weight. |
| file_tree-21 | P3 | proposal | Directory groups: assets and tests follow immediately after the preceding subtree. | Add a small gap before each top-level directory. |
| file_tree-22 | P3 | proposal | README and makefile: Root-level files visually blend into the preceding tests subtree. | Separate the root-file group with a small consistent gap. |
| file_tree-23 | P2 | proposal | Font asset size: Archivo.ttf and atlas.png sizes share the same tone as their names. | Use a muted but legible size column to emphasize names. |
| file_tree-24 | P3 | proposal | Window border: A thin gray outline and nearly identical surrounding grays flatten the window. | Use a clearer surface contrast with a restrained border. |
| file_tree-25 | P3 | proposal | Bottom whitespace: A broad unused strip remains below the tree window. | Center the reduced window vertically or expand useful content into that space. |

## fit content lab

[Reviewed image](../../docs/design-audit/originals/fit_content_lab_720p.png) · [Current baseline](../../screenshot-baselines/screens/fit_content_lab_720p.png) · [Visual review](../../design-audit.html#screen=fit_content_lab)

Baseline SHA-256: `dc9a3cba8c9defc7e15b17a922be3563fad2e164a915178f37a504e58298960a`

Both columns correctly demonstrate content-sized bubbles with different caps. Only 16 distinct design proposals are defensible from this intentionally minimal fixture; no wrapping defect is claimed.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| fit_content_lab-01 | P2 | proposal | Page heading: The heading combines an API name and a long explanation at one small size. | Use Fit-content bubbles as the title and a separate concise API subtitle. |
| fit_content_lab-02 | P2 | proposal | Column labels: cap 380px and cap 200px are much smaller than the already small body text. | Increase the labels and use consistent spacing before px. |
| fit_content_lab-03 | P2 | proposal | Column framing: The two repeated conversations have no visible panel boundary. | Place each conversation in a subtle labeled frame. |
| fit_content_lab-04 | P2 | proposal | Column comparison: The narrow cap is only described in text. | Add 380 px and 200 px measurement guides above the longest bubbles. |
| fit_content_lab-05 | P2 | proposal | Short hi bubbles: The hi background hugs the letters with almost no visible inset. | Add equal minimum horizontal and vertical bubble padding while preserving content sizing. |
| fit_content_lab-06 | P2 | proposal | Long technical message: The long message has tightly packed lines and a handwritten face. | Use a readable body face and more line height for both comparisons. |
| fit_content_lab-07 | P2 | proposal | Speaker distinction: Left and right alignment plus fill color are the only speaker cues. | Label the two roles outside the samples with a compact legend. |
| fit_content_lab-08 | P3 | proposal | Repeated content: The columns do not explicitly say they contain identical messages. | Add Same text, different maximum width beneath the title. |
| fit_content_lab-09 | P2 | proposal | Horizontal empty space: The short left and right messages sit at opposite edges of very wide columns. | Use narrower comparison frames that keep message relationships easy to follow. |
| fit_content_lab-10 | P3 | proposal | Bottom empty region: Most of the viewport below the six messages is unused. | Add a compact explanation of intrinsic width versus maximum width below the examples. |
| fit_content_lab-11 | P3 | proposal | Wide long bubble: Its exact three-line wrap is visible but unannotated. | Add an external line-count caption to support comparison with the narrow bubble. |
| fit_content_lab-12 | P3 | proposal | Narrow long bubble: Its increased height is visible without a height reference. | Add an external height guide beside the narrow example. |
| fit_content_lab-13 | P2 | proposal | Message ordering: The second conversation ends lower because of wrapping, obscuring one-to-one row comparison. | Add unobtrusive message numbers beside matching bubbles. |
| fit_content_lab-14 | P3 | proposal | Code-oriented prose: The example story mentions a layout bug but does not identify the tested sizing rule. | Place the technical story in a sample badge and explain the sizing rule separately. |
| fit_content_lab-15 | P3 | proposal | Title strip: The near-full-width title strip has rounded ends unrelated to the bubble comparison. | Reduce the strip to a plain header with a divider above the frames. |
| fit_content_lab-16 | P2 | proposal | Final bubbles: Anyway it is fixed appears far from the preceding incoming message. | Use consistent vertical conversation spacing after wrapped messages. |

### Additional capture: fit_content_lab

[Image](../../baseline_screenshots/fit_content_lab.png)

Active e2e baseline refreshed after visual review. The previous capture remains in originals until audit cleanup.

## flex alignment

[Reviewed image](../../docs/design-audit/originals/flex_alignment_720p.png) · [Current baseline](../../screenshot-baselines/screens/flex_alignment_720p.png) · [Visual review](../../design-audit.html#screen=flex_alignment)

Baseline SHA-256: `4ad0674ff17ce937b4896b21d705318a37a01e9381c3de0f50c6e904c9f9ca8f`

The alignment examples expose their intended differences. Improve the explanatory frame without moving the sample items away from the alignment positions under test.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| flex_alignment-01 | P2 | proposal | Tab typography: Vertical and Horizontal are tiny beside the large page title. | Increase all six tab labels to a readable supporting size. |
| flex_alignment-02 | P2 | proposal | All tab: Selection uses a muted underline at the far bottom of a tall tab. | Bring a brighter active indicator closer to the All label. |
| flex_alignment-03 | P2 | proposal | Gap category: Gap appears in navigation but is absent from the All overview. | Add a compact gap comparison or rename All to Overview. |
| flex_alignment-04 | P2 | proposal | Section headings: Long API names compete with the plain-language titles. | Put API names in smaller secondary captions. |
| flex_alignment-05 | P2 | proposal | Vertical examples: Start, End, and Center omit the main-axis direction. | Add a downward main-axis arrow beside the column samples. |
| flex_alignment-06 | P2 | proposal | Horizontal examples: The row direction is named only in a long heading. | Add a rightward main-axis arrow over the row samples. |
| flex_alignment-07 | P2 | proposal | Cross-axis examples: Cross-Axis Align has no visual axis reference. | Add a vertical cross-axis guide outside the sample boxes. |
| flex_alignment-08 | P2 | proposal | SelfAlign panel: Auto and Center occupy the same height without explaining why. | Caption the parent AlignItems::Center setting above the panel. |
| flex_alignment-09 | P3 | proposal | Auto item: Auto has a unique purple fill with no legend. | Label it Inherits parent in an external caption. |
| flex_alignment-10 | P3 | proposal | SelfAlign colors: Start, Auto, Center, and End use four colors without a key. | Add a small legend mapping colors to the four overrides. |
| flex_alignment-11 | P2 | proposal | Vertical item numerals: Numbers sit near the left edge of teal rounded boxes. | Center the numerals without changing the box positions. |
| flex_alignment-12 | P2 | proposal | Horizontal numerals: Tiny numerals are tightly packed inside narrow pink pills. | Use a more legible numeral face at the same sample dimensions. |
| flex_alignment-13 | P2 | proposal | Between label: Between abbreviates the actual spacing mode. | Use Space between in a caption outside the narrow sample. |
| flex_alignment-14 | P2 | proposal | Around label: Around leaves the relationship between edge and internal gaps implicit. | Add an edge-gap diagram beneath the sample. |
| flex_alignment-15 | P3 | proposal | Start versus End: Their leftover space is visible but unmeasured. | Add subtle brackets showing the unused region for both cases. |
| flex_alignment-16 | P3 | proposal | Center column: The centered group's relationship to the container midpoint is implicit. | Draw a faint midpoint guide behind the sample. |
| flex_alignment-17 | P3 | proposal | Between column: The distribution is visible without identifying equal internal gaps. | Add equal-gap markers alongside the three items. |
| flex_alignment-18 | P3 | proposal | Cross-axis item heights: The three varied heights are unexplained. | Caption the fixed test heights outside the comparison. |
| flex_alignment-19 | P2 | proposal | Start cross-axis sample: The top edges align but no reference line marks that edge. | Add a subtle alignment guide along the tested top edge. |
| flex_alignment-20 | P2 | proposal | End cross-axis sample: The common bottom edge is hard to compare against rounded framing. | Add a straight bottom reference guide inside the frame. |
| flex_alignment-21 | P3 | proposal | Horizontal sample frames: Their large empty lower halves make the short row hard to inspect. | Reduce framing height while preserving the row's measured container. |
| flex_alignment-22 | P2 | proposal | Nested panel fills: Four similar blue-gray surface layers obscure grouping. | Remove one decorative background layer around each example group. |
| flex_alignment-23 | P3 | proposal | Lower panel space: Both outer columns extend well below their content. | Fit the panels to their sections or use the spare space for a legend. |
| flex_alignment-24 | P3 | proposal | Heading insets: Section headings begin almost at the rounded panel edges. | Use consistent horizontal padding in every section header. |
| flex_alignment-25 | P2 | proposal | Overview explanation: No short sentence states what changes and what stays fixed between samples. | Add a one-line comparison rule under Flexbox Alignment Demo. |

### Additional capture: flex_alignment

[Image](../../baseline_screenshots/flex_alignment.png)

Active e2e baseline refreshed after visual review. The previous capture remains in originals until audit cleanup.

## flight options

[Reviewed image](../../docs/design-audit/originals/flight_options_720p.png) · [Current baseline](../../screenshot-baselines/screens/flight_options_720p.png) · [Visual review](../../design-audit.html#screen=flight_options)

Baseline SHA-256: `b323899f546bd6e5e1c9abe7da6f33f563fc94945b281d07e3cd85c8e7d569f0`

The sparse flight-menu composition matches the existing mock. The main opportunities are more legible secondary options and clearer relationships among navigation, settings, and help.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| flight_options-01 | P2 | proposal | Flight System suboptions: Four setting names are much smaller than category navigation. | Increase suboption text while retaining the two-level hierarchy. |
| flight_options-02 | P2 | proposal | Suboption spacing: Control Type and High-G Turn Settings sit in tightly spaced rows. | Increase row height to form distinct selection targets. |
| flight_options-03 | P2 | proposal | Suboption location: The suboptions start beside the middle of the navigation list. | Align their heading with the selected Flight System category. |
| flight_options-04 | P2 | proposal | Submenu heading: No heading identifies the short list as Flight System settings. | Add a restrained Flight System subheading over the controls. |
| flight_options-05 | P2 | proposal | Control Type value: Control Type shows no current value. | Add a right-aligned current setting value. |
| flight_options-06 | P2 | proposal | High-G Turn value: High-G Turn Settings shows only its name. | Expose the current mode beside the label. |
| flight_options-07 | P2 | proposal | Collision damage value: Reduced Collision Damage has no visible on/off state. | Show a compact On or Off value with the setting. |
| flight_options-08 | P2 | proposal | Vibration row: Vibration is extremely dim even though its unavailable status is useful information. | Raise disabled text contrast while keeping it visibly inactive. |
| flight_options-09 | P2 | proposal | Unavailable explanation: Vibration says Unavailable without a reason. | Add a short requirement note beneath the disabled row. |
| flight_options-10 | P3 | proposal | Selected category: A thin gold marker identifies Flight System but stops short of its full row height. | Use a taller marker aligned with the row's text block. |
| flight_options-11 | P3 | proposal | Selection wash: The faint gold wash stretches far beyond Flight System. | Limit it to the navigation column width. |
| flight_options-12 | P2 | proposal | Options heading: OPTIONS is only slightly larger than category labels. | Increase its size or weight to establish a clearer page heading. |
| flight_options-13 | P2 | proposal | Main navigation width: Category labels sit beside overlapping technical wire lines. | Provide a quiet text zone behind the navigation list. |
| flight_options-14 | P3 | proposal | Menu spacing: The nine category rows form a dense uninterrupted block. | Add subtle groups for input, display, and audio/language settings. |
| flight_options-15 | P2 | proposal | Help sentence: Select the in-flight system is far below the related list. | Move the help closer to the settings panel. |
| flight_options-16 | P3 | proposal | Help content: The help sentence repeats the selected category without explaining it. | Describe the currently highlighted setting in one concise sentence. |
| flight_options-17 | P2 | proposal | Footer Enter key: Enter is very small inside a narrow outlined keycap. | Widen the keycap enough for the full label at readable size. |
| flight_options-18 | P2 | proposal | Footer Esc key: Esc uses a tiny type size compared with BACK. | Use a consistent readable key-label size for Enter and Esc. |
| flight_options-19 | P2 | proposal | Footer OK label: OK does not describe whether it opens or confirms the selected item. | Use Select or Apply according to the action represented. |
| flight_options-20 | P3 | proposal | Footer grouping: Enter OK and Esc BACK have little separation from one another. | Increase the gap between action groups. |
| flight_options-21 | P3 | proposal | Footer background lines: Diagonal wire lines pass behind the footer keycaps. | Clear the wire treatment from the footer's reading area. |
| flight_options-22 | P3 | proposal | Right-side radar: A large faint ellipse occupies much of the empty right half. | Use the radar area for a restrained setting preview or explanatory illustration. |
| flight_options-23 | P3 | proposal | Right angular outline: An isolated L-shaped outline has no relation to the displayed settings. | Integrate it into the preview frame or simplify it. |
| flight_options-24 | P3 | proposal | Page balance: Nearly all informative content sits within the left 500 pixels. | Give the setting detail area more width while retaining the sparse background. |
| flight_options-25 | P3 | proposal | Technical grid: Grid lines remain equally visible under text and empty areas. | Reduce grid contrast behind navigation and descriptions. |

## font weight

[Reviewed image](../../docs/design-audit/originals/font_weight_720p.png) · [Current baseline](../../screenshot-baselines/screens/font_weight_720p.png) · [Visual review](../../design-audit.html#screen=font_weight)

Baseline SHA-256: `4628ab71e38244ce6cffd58397280bf934c356638851311dfd407fdb471d189a`

The Light fallback is intentional and explicitly documented in source. Sixteen grounded presentation proposals are recorded; extra defects would require inventing problems in this small typography fixture.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| font_weight-01 | P2 | proposal | Title: The family name and test instruction share one long small heading. | Separate Font weight from Archivo Narrow and the comparison instruction. |
| font_weight-02 | P2 | proposal | Weight labels: Light 300 through Bold 700 use a handwritten face unlike the samples. | Use a neutral label face so letterform style does not distract from weight comparison. |
| font_weight-03 | P2 | proposal | Light fallback label: The fallback explanation is packed into the long row label. | Separate Requested 300 from Rendered 400 in adjacent columns. |
| font_weight-04 | P2 | proposal | Row headers: No headers distinguish weight metadata from sample text. | Add Requested weight and Rendered sample column headings. |
| font_weight-05 | P2 | proposal | Sample size: The pangram is small relative to the available canvas. | Add a larger companion specimen while preserving the current-size test. |
| font_weight-06 | P3 | proposal | Baseline reference: The rows have no visible baseline guides. | Add optional subtle guides beneath each specimen. |
| font_weight-07 | P3 | proposal | Weight progression: Rows have equal spacing but no connecting visual scale. | Add a compact 300-to-700 weight scale beside the labels. |
| font_weight-08 | P3 | proposal | Fallback comparison: Light and Regular look alike as expected, but the connection is textual only. | Group those rows with a fallback bracket. |
| font_weight-09 | P2 | proposal | SemiBold label: SemiBold capitalization differs from familiar sentence-style naming. | Use Semibold 600 consistently with the other row names. |
| font_weight-10 | P3 | proposal | Pangram numbers: Only 0123 represent numeric glyphs. | Add a separate 0123456789 line for number-weight comparison. |
| font_weight-11 | P3 | proposal | Pangram punctuation: The sample includes little punctuation. | Add a short punctuation specimen in the unused right area. |
| font_weight-12 | P2 | proposal | Mixed-weight example: theme.h appears below a large gap without a section heading. | Label this section Mixed weights on one baseline. |
| font_weight-13 | P2 | proposal | Mixed-weight alignment: theme.h and src/ui are separated by a wide blank gap. | Bring the runs closer while keeping the shared baseline test. |
| font_weight-14 | P2 | proposal | Mixed-weight explanation: Regular weight, same baseline is small and dim. | Move it into a legible caption below the specimen. |
| font_weight-15 | P3 | proposal | Right empty region: The entire right half is unused. | Use a second specimen column for a close-up comparison of 500 and 600. |
| font_weight-16 | P3 | proposal | Specimen metadata: The screenshot names the family but not the rendered size. | Add the font size and loaded-face names in a compact footer. |

## forms

[Reviewed image](../../docs/design-audit/originals/forms_720p.png) · [Current baseline](../../screenshot-baselines/screens/forms_720p.png) · [Visual review](../../design-audit.html#screen=forms)

Baseline SHA-256: `c5492252702fa3fc71ea0aa3fb5e1a79549d8456edd5e45a88eba07a0394464d`

The page demonstrates sliders, checkboxes, progress, scrolling, and clipboard state. Its scroll clipping is intentional; clearer control anatomy and section hierarchy would make the examples easier to inspect.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| forms-01 | P2 | proposal | Page title: Form Components touches the top of its cyan strip. | Add a top safe margin and consistent title padding. |
| forms-02 | P2 | proposal | Slider labels: Volume, Brightness, and Difficulty are embedded inside the filled halves. | Place labels above their tracks so fills remain visually measurable. |
| forms-03 | P2 | proposal | Slider thumb contrast: Teal thumbs sit on teal filled tracks. | Use a brighter outlined thumb that remains distinct on both track colors. |
| forms-04 | P2 | proposal | Slider values: Percent values are buried inside the labels. | Place values in a shared trailing column. |
| forms-05 | P2 | proposal | Volume track: The thick track resembles a segmented button. | Reduce the track height while preserving a generous thumb. |
| forms-06 | P3 | proposal | Difficulty percentage: Difficulty is represented as 30% without a scale explanation. | Add Easy and Hard endpoint labels or a named level. |
| forms-07 | P3 | proposal | Brightness control: Brightness has no low/high endpoint references. | Add a dim-to-bright icon pair outside the track. |
| forms-08 | P2 | proposal | Progress section header: Progress Bars uses a red strip while Sliders uses cyan, without a legend. | Use a shared header style and reserve colors for the sample contents. |
| forms-09 | P2 | proposal | Audio progress label: Audio Level 75% sits on the colored bar. | Move the text outside the bar for an unobstructed progress comparison. |
| forms-10 | P2 | proposal | Level progress label: Level Progress 75/100 is embedded in the fill. | Separate the label and numeric value from the progress track. |
| forms-11 | P3 | proposal | Progress units: One bar uses percent and the other a fraction without explanation. | Caption the two demos Percentage and Custom range. |
| forms-12 | P2 | proposal | Checkbox marks: Checked states appear as literal [x] strings. | Use a clear square checkbox and centered check glyph. |
| forms-13 | P2 | proposal | Checkbox position: Checkbox marks sit near the center of long cyan rows. | Place each mark consistently before its label or at the trailing edge. |
| forms-14 | P2 | proposal | Checkbox label size: Music and Fullscreen are much smaller than section headings. | Increase checkbox labels to match slider-label readability. |
| forms-15 | P3 | proposal | Checkbox grouping: Audio and display options form one undifferentiated list. | Add small Audio and Display group labels. |
| forms-16 | P3 | proposal | SFX label: SFX is abbreviated while Music is spelled out. | Use Sound effects for clearer scanning. |
| forms-17 | P3 | proposal | V-Sync label: V-Sync has no supporting description. | Add a short explanatory subtitle in the demo's available width. |
| forms-18 | P2 | proposal | Scroll boundary: A sliver of the next checkbox appears at the bottom edge. | Keep clipping intentional but add a fade or More options cue at the boundary. |
| forms-19 | P2 | proposal | Scrollbar: The thin light thumb sits far to the right of the option rows. | Move it closer to the checkbox list and give its track a clearer boundary. |
| forms-20 | P2 | proposal | Status strip: Volume, Resolution, and Quality appear as one small pipe-separated line. | Use three labeled value groups at a readable size. |
| forms-21 | P3 | proposal | Status relation: Resolution and Quality are summarized while their controls are offscreen. | Label the strip Current settings to explain that it summarizes the whole form. |
| forms-22 | P2 | proposal | Clipboard explanation: The clipboard help sentence is tiny and widely spaced. | Increase it and wrap the copy/paste explanation into two short clauses. |
| forms-23 | P2 | proposal | Copy Status action: Copy Status and Paste use unrelated cyan and red fills. | Use a clear primary/secondary pairing without danger-like red for Paste. |
| forms-24 | P2 | proposal | Clipboard result: Clipboard empty is small inline text without a visible output boundary. | Place the result in a labeled read-only output field. |
| forms-25 | P2 | proposal | Clipboard bottom row: Buttons sit flush against the bottom edge of their enclosing panel. | Add bottom padding without reducing the scrollable test region. |

### Additional capture: forms

[Image](../../baseline_screenshots/forms.png)

Current capture removes Volume focus outline. The same fixed-height scroller clips the next control row and the metadata footer remains small. Keep intentional scroll behavior when revising its presentation.

## grid lab

[Reviewed image](../../docs/design-audit/originals/grid_lab_720p.png) · [Current baseline](../../screenshot-baselines/screens/grid_lab_720p.png) · [Visual review](../../design-audit.html#screen=grid_lab)

Baseline SHA-256: `78badc2c90a1d684e23b14a11efd4510aff84c81ae292ff8884de95680a2b868`

The table geometry and ragged content are intentional tests. Eighteen distinct proposals improve inspection; the sparse fixture does not support 25 independent defects.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| grid_lab-01 | P2 | proposal | Header strip: The API explanation is a small single-line title. | Use Grid layout as a heading and keep the explanation as a subtitle. |
| grid_lab-02 | P2 | proposal | Equal-track caption: Equal columns, ragged text is tiny relative to the table width. | Increase the caption and identify this as Example 1. |
| grid_lab-03 | P2 | proposal | Equal-track cell text: Cell text is very small despite broad columns. | Increase the specimen text or add a larger companion specimen. |
| grid_lab-04 | P2 | proposal | Cell insets: id, name, role, and notes sit almost against grid lines. | Add consistent horizontal cell padding. |
| grid_lab-05 | P2 | proposal | Column headings: Header text has little weight distinction from the body. | Use a stronger header face while preserving track dimensions. |
| grid_lab-06 | P3 | proposal | ID column: The short numeric column occupies the same width as long role text by design. | Add an Equal 1fr tracks annotation above the table. |
| grid_lab-07 | P3 | proposal | Ragged name example: a and a much longer name illustrate width variation without a reference. | Add a caption explaining that content length does not move column boundaries. |
| grid_lab-08 | P3 | proposal | Notes dash: The dash in the first notes row has no explanatory context. | Label it as an empty-value fixture in the caption. |
| grid_lab-09 | P2 | proposal | Second caption: Explicit tracks does not show the actual track widths. | Display the four numeric track sizes above their corresponding columns. |
| grid_lab-10 | P2 | proposal | Spanning header: One cell, spanning three tracks is only communicated through text. | Add a subtle three-column bracket above the merged cell. |
| grid_lab-11 | P3 | proposal | Blank corner cell: The second table's first header cell is empty. | Caption it as the row-index gutter outside the table. |
| grid_lab-12 | P2 | proposal | r1c1 labels: Coordinate-style content is harder to scan in the small handwritten face. | Use a compact monospaced face for the coordinate fixtures. |
| grid_lab-13 | P3 | proposal | Striped second table: The middle body row has a distinct blue fill without an explanation. | Label the alternating-row treatment in the sample metadata. |
| grid_lab-14 | P2 | proposal | Thick-border claim: The caption says thick border but the thin displayed lines are hard to judge at this scale. | Add an enlarged border detail and the requested border width. |
| grid_lab-15 | P3 | proposal | Table comparison: The first table spans the viewport while the second occupies less than half. | Give each a clearly bounded sample frame with its own width annotation. |
| grid_lab-16 | P3 | proposal | Unused lower area: Most of the screen below the second table is empty. | Place a compact track-definition diagram below the rendered examples. |
| grid_lab-17 | P3 | proposal | Grid-line contrast: Grid lines and small pale text compete for attention. | Reduce internal line emphasis while keeping the outer test border unchanged. |
| grid_lab-18 | P2 | proposal | Expected result: The page describes setup without a short success criterion. | Add Columns stay aligned across every row beneath the examples. |

### Additional capture: grid_lab

[Image](../../baseline_screenshots/grid_lab.png)

Active e2e baseline refreshed after visual review. The previous capture remains in originals until audit cleanup.

## guess who lab

[Reviewed image](../../docs/design-audit/originals/guess_who_lab_720p.png) · [Current baseline](../../screenshot-baselines/screens/guess_who_lab_720p.png) · [Visual review](../../design-audit.html#screen=guess_who_lab)

Baseline SHA-256: `b938b2f8802a0c099a5107bcdae43c1a4882b638c6d013acba4ff1ac1ee94cbc`

The full board is readable and the illustrated cast supports the game. Refine question composition, note controls, and turn feedback while retaining the playful mock design.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| guess_who_lab-01 | P2 | defect | New game label: The reset glyph touches the end of New game. | Add a visible gap between the label and reset icon. |
| guess_who_lab-02 | P2 | defect | Ask question label: The arrow touches the final letter of question. | Separate the arrow with consistent button-icon spacing. |
| guess_who_lab-03 | P2 | proposal | Question sentence: DOES YOUR PERSON runs directly into the first dropdown. | Add a clear gap between the prompt and trait field. |
| guess_who_lab-04 | P2 | proposal | Answer field: Yes has no visible label explaining whose answer it represents. | Label the field Answer above the compact selector. |
| guess_who_lab-05 | P2 | proposal | Trait selector: The selector ends with a literal v. | Replace it with a crisp down chevron aligned to the trailing inset. |
| guess_who_lab-06 | P2 | proposal | Per-face note selectors: Twenty-four Not sure v labels use very small text. | Increase note text and give the arrow a separate trailing slot. |
| guess_who_lab-07 | P2 | proposal | Note versus elimination: Each face has both a portrait and note control without a local distinction. | Label the note row as a note and keep the flip instruction beside the board. |
| guess_who_lab-08 | P2 | proposal | Remaining count: 24 faces left is isolated at the right of the question bar. | Pair it with a small board-status label or progress indicator. |
| guess_who_lab-09 | P3 | proposal | Turn indicator: A red dot accompanies YOUR TURN without explaining red's meaning. | Use an active-player token or arrow consistent with the board palette. |
| guess_who_lab-10 | P2 | proposal | Turn heading: YOUR TURN and Find the mystery person lack player identity. | Add Player 1 or the active player's name beside the turn label. |
| guess_who_lab-11 | P2 | proposal | Game objective: Find the mystery person does not explain the question-and-answer sequence. | Add a short first-turn instruction near the question bar. |
| guess_who_lab-12 | P3 | proposal | Logo scale: The stacked logo fills nearly the entire header height. | Reduce its height slightly to give the header a calmer top margin. |
| guess_who_lab-13 | P2 | proposal | Portrait details: Glasses, eye color, and hair details are small within the 8-column board. | Increase facial-detail contrast while retaining all 24 visible cards. |
| guess_who_lab-14 | P3 | proposal | Hat cropping: Tall hats touch or disappear beneath the top card edge. | Provide consistent headroom for hatted portraits. |
| guess_who_lab-15 | P2 | proposal | Name strips: Names sit in narrow cream bands very close to portrait art. | Increase the name-strip separation from each face. |
| guess_who_lab-16 | P3 | proposal | Card borders: Thick yellow double borders compete with distinguishing facial features. | Reduce border emphasis and reserve stronger framing for a chosen candidate. |
| guess_who_lab-17 | P2 | proposal | Board frame: Multiple blue bevel bands consume height around the grid. | Simplify the frame to reclaim space for portraits and notes. |
| guess_who_lab-18 | P3 | proposal | Card row gaps: The note bar nearly touches the next row's top border. | Increase vertical gap between complete card groups. |
| guess_who_lab-19 | P2 | proposal | Footer help: Click a face to flip it down is far from the card controls. | Place the instruction immediately above or below the board near its leading edge. |
| guess_who_lab-20 | P3 | proposal | Feedback sentence: A little question. A big clue. occupies the feedback area without showing game state. | Use an instructional starting message until the first question is asked. |
| guess_who_lab-21 | P2 | proposal | Question history: The composition has no visible record area for previous clues. | Reserve a compact clue-history strip near the remaining count. |
| guess_who_lab-22 | P3 | proposal | Trait grammar: Does your person plus wear glasses is clear but tightly merged with answer and action. | Group trait selection separately from answer confirmation through spacing. |
| guess_who_lab-23 | P2 | proposal | Reset action hierarchy: New game has a large outline pill near the turn status. | Reduce its visual emphasis so the question action remains primary. |
| guess_who_lab-24 | P3 | proposal | Card coordinate reference: The dense grid provides only personal names for locating faces. | Add subtle row markers to support discussing or testing a specific card. |
| guess_who_lab-25 | P2 | proposal | Board status semantics: All note rows say Not sure while the header says 24 faces left. | Add a legend distinguishing note state from eliminated state. |

## horizontal drag

[Reviewed image](../../docs/design-audit/originals/horizontal_drag_720p.png) · [Current baseline](../../screenshot-baselines/screens/horizontal_drag_720p.png) · [Visual review](../../design-audit.html#screen=horizontal_drag)

Baseline SHA-256: `e2d3f3912f5c3cf5bad163eddd4409883d83e9cf12794fe1e4a528abbff2a51d`

Three priority lanes and seven tags form a small drag demonstration. Twenty distinct proposals are supported by the baseline; none assumes a failure of unseen drag behavior.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| horizontal_drag-01 | P2 | proposal | Page title: Horizontal Drag & Drop dominates the small amount of content. | Reduce the title size and give the lanes more visual emphasis. |
| horizontal_drag-02 | P2 | proposal | Subtitle: Drag tags between priority lanes is almost title-sized. | Use a smaller supporting size with a clear gap below the heading. |
| horizontal_drag-03 | P2 | proposal | Lane counts: Counts are embedded in handwritten heading text. | Use compact count badges aligned consistently after each priority name. |
| horizontal_drag-04 | P2 | proposal | Lane labels: Red, yellow, and green headings communicate priority mainly through color and words. | Add a severity icon or short High/Medium/Low marker beside each heading. |
| horizontal_drag-05 | P2 | proposal | Tag affordance: The colored tags look like ordinary buttons in the resting image. | Add a small drag grip to each tag. |
| horizontal_drag-06 | P2 | proposal | Tag color meaning: Seven unrelated tag fills have no visible legend. | Use a shared neutral tag fill or explain the tag-color categories. |
| horizontal_drag-07 | P2 | proposal | Drop-space cue: Large blank areas to the right of the tags have no instruction. | Add a faint Drop here cue within each lane's empty region. |
| horizontal_drag-08 | P2 | proposal | Lane dimensions: All lanes span nearly the entire viewport while holding two or three tags. | Reduce lane width or use the space for a visible drop target boundary. |
| horizontal_drag-09 | P2 | proposal | Empty lower region: The bottom quarter of the panel contains no content. | Fit the panel to the three lanes and center it vertically. |
| horizontal_drag-10 | P3 | proposal | Row borders: Thin outlines are nearly invisible against the dark lane fill. | Increase lane boundary contrast slightly. |
| horizontal_drag-11 | P2 | proposal | High-priority tags: Security patch and Data loss fix use two different red tones without distinct meanings. | Use one high-priority tag treatment or label their separate categories. |
| horizontal_drag-12 | P3 | proposal | Refactor auth label: Auth is abbreviated in an otherwise plain-language board. | Use Refactor authentication if space permits. |
| horizontal_drag-13 | P3 | proposal | Update deps label: Deps is a developer abbreviation. | Use Update dependencies for a self-contained sample. |
| horizontal_drag-14 | P3 | proposal | Rename var label: Var is abbreviated despite the wide empty lane. | Use Rename variable. |
| horizontal_drag-15 | P3 | proposal | Readme typo label: Readme differs from the conventional filename styling. | Use README typo to identify the document clearly. |
| horizontal_drag-16 | P2 | proposal | Tag spacing: The gap between tags varies because each group uses different widths. | Use one fixed inter-tag gap across the three lanes. |
| horizontal_drag-17 | P3 | proposal | Lane heading gap: Headings sit very close to their first tag row. | Add a consistent gap between lane title and tags. |
| horizontal_drag-18 | P2 | proposal | Demo result: Only lane counts report the result of moving a tag. | Reserve a small Last moved status line beneath the lanes. |
| horizontal_drag-19 | P3 | proposal | Starting arrangement: The screen shows no reference for restoring the initial priority distribution. | Add a compact Reset example action near the subtitle. |
| horizontal_drag-20 | P2 | proposal | Instruction specificity: The subtitle says move between lanes but does not mention ordering within a lane. | State whether the demonstration also supports horizontal reordering. |

## hover lab

[Reviewed image](../../docs/design-audit/originals/hover_lab_720p.png) · [Current baseline](../../screenshot-baselines/screens/hover_lab_720p.png) · [Visual review](../../design-audit.html#screen=hover_lab)

Baseline SHA-256: `51653f821997a6d6cba947ea89bba9905964c7e243d56a14d5fac41750bc6e09`

All three rows are captured at rest. Their hover and click differences are intentional. Eighteen proposals improve the experiment presentation without claiming that the unseen interactions are broken.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| hover_lab-01 | P2 | proposal | Title: The title mixes Hover Lab with two technical concepts on one line. | Use a short title and a separate experiment summary. |
| hover_lab-02 | P2 | proposal | Case captions: The three long captions use tiny handwritten text. | Increase caption size and wrap each explanation above its row. |
| hover_lab-03 | P2 | proposal | Case naming: Cases are identified by code expressions before their purpose. | Add names Direct hover, Subtree hover, and Pointer passthrough. |
| hover_lab-04 | P2 | proposal | Star glyphs: The interactive stars are rendered as small asterisks. | Use a recognizable star glyph while preserving the hit-test geometry. |
| hover_lab-05 | P2 | proposal | Row labels: Thread row 1 through 3 do not name the tested behavior. | Use concise behavior names in the row labels. |
| hover_lab-06 | P2 | proposal | Caption and row grouping: Each caption sits close to the preceding blank gap without a enclosing group. | Frame each case with a consistent sample boundary. |
| hover_lab-07 | P2 | proposal | Diagnostic table: Three diagnostic lines have no column headings. | Add Case, Row lit, Star hot, and Clicks headings. |
| hover_lab-08 | P2 | proposal | Diagnostic values: lit=n and star_hot=n use terse boolean abbreviations. | Show Yes and No badges beside the API terms. |
| hover_lab-09 | P2 | proposal | Diagnostic alignment: Variable-length expressions push values into uneven columns. | Use fixed-width columns for the three state fields. |
| hover_lab-10 | P2 | proposal | Diagnostics separation: All diagnostics sit below all samples. | Place each case's live status directly beneath its row. |
| hover_lab-11 | P2 | proposal | Row 1 explanation: The phrase star steals hotness requires internal terminology. | Explain that entering the star removes the row highlight. |
| hover_lab-12 | P2 | proposal | Row 2 explanation: The subtree behavior is described in a long code-heavy sentence. | State Row stays highlighted; star still receives clicks. |
| hover_lab-13 | P2 | proposal | Row 3 explanation: The star looks identical to the other two despite intentionally ignoring input. | Add an external Pointer passthrough badge to identify the test variant. |
| hover_lab-14 | P2 | proposal | Footer instruction: Click each star is small and far below the actual stars. | Move the instruction above the samples near the title. |
| hover_lab-15 | P3 | proposal | Initial-state capture: All rows have the same resting appearance. | Add labeled expected-state thumbnails beside the live examples. |
| hover_lab-16 | P3 | proposal | Unused right area: Most of the canvas is blank. | Use the right half for a simple row-versus-child hit-test diagram. |
| hover_lab-17 | P3 | proposal | Comparison outcome: No visual legend explains which case keeps the row wash. | Add a small expected-results matrix covering hover and clicks. |
| hover_lab-18 | P3 | proposal | Click counter readability: star_clicks=0 is embedded at the end of each small diagnostic line. | Give each click count a compact, prominent numeric badge. |

## hstack showcase

[Reviewed image](../../docs/design-audit/originals/hstack_showcase_720p.png) · [Current baseline](../../screenshot-baselines/screens/hstack_showcase_720p.png) · [Visual review](../../design-audit.html#screen=hstack_showcase)

Baseline SHA-256: `4c617c180b93c99ff752743f13c381255ffb23e04a1c9a3bee3477a14945ec57`

The intentional colors and item positions demonstrate hstack behavior. Improve semantic labels and comparison guides without standardizing the alignment samples.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| hstack_showcase-01 | P2 | proposal | Heading: hstack() Showcase has no plain-language explanation. | Add Horizontal stack layout as a subtitle. |
| hstack_showcase-02 | P2 | proposal | Basic row: Item 1, Item 2, and Item 3 have no section label. | Label this row Equal-width children. |
| hstack_showcase-03 | P3 | proposal | Basic-row gaps: Large gaps are visible but their configured size is not. | Add an external gap measurement caption. |
| hstack_showcase-04 | P2 | proposal | Justification section: Five pink headers appear without a shared section title. | Add Main-axis distribution above the five examples. |
| hstack_showcase-05 | P2 | proposal | Between and Around: The shortened names omit Space. | Use Space between and Space around in external captions. |
| hstack_showcase-06 | P2 | proposal | Pink header text: Small black handwritten labels sit close to the left edge. | Increase caption size and apply equal horizontal padding. |
| hstack_showcase-07 | P2 | proposal | Unnumbered cyan boxes: The repeated boxes contain no identity markers. | Add 1, 2, and 3 labels to reveal item order. |
| hstack_showcase-08 | P3 | proposal | Start example: Its packed group has no left-edge reference. | Add a faint tested-edge guide in the frame. |
| hstack_showcase-09 | P3 | proposal | End example: The right-aligned group has no right-edge reference. | Add a faint right-edge guide. |
| hstack_showcase-10 | P3 | proposal | Center example: The centered group lacks a midpoint marker. | Add a subtle centerline behind the sample. |
| hstack_showcase-11 | P3 | proposal | Between example: Equal gaps are shown without measurement cues. | Add small equal-gap brackets outside the item row. |
| hstack_showcase-12 | P3 | proposal | Around example: The half-size edge gaps are not explained. | Annotate edge versus internal spacing beneath the frame. |
| hstack_showcase-13 | P2 | proposal | Cross-axis section: Green headers Start, Center, and End lack a group label. | Add Cross-axis alignment above them. |
| hstack_showcase-14 | P3 | proposal | Unequal item heights: Green, orange, and purple item heights vary without a legend. | Caption their fixed heights as part of the test setup. |
| hstack_showcase-15 | P2 | proposal | Alignment guides: Rounded frames make common top and bottom edges less obvious. | Add straight reference guides inside the three samples. |
| hstack_showcase-16 | P2 | proposal | Toolbar Pattern heading: The label is tiny relative to the blue title strip. | Increase its text and align it with toolbar content. |
| hstack_showcase-17 | P2 | proposal | Toolbar commands: File, Edit, and View are much smaller than the large surrounding examples. | Use a readable command size within the toolbar specimen. |
| hstack_showcase-18 | P3 | proposal | Settings placement: The right-aligned Settings button demonstrates spare-space use without explanation. | Annotate it as the trailing action using remaining width. |
| hstack_showcase-19 | P2 | proposal | Card Row title: The title does not explain which hstack behavior it demonstrates. | Rename it Four equal cards with fixed gaps. |
| hstack_showcase-20 | P2 | proposal | A B C D cards: Single letters sit against the left edges of the small cards. | Center the letters or add a consistent inset. |
| hstack_showcase-21 | P2 | proposal | Dashboard row: Users, Revenue, Orders, Growth, Active, and Churn appear without a section heading. | Label the row Six expanding cards. |
| hstack_showcase-22 | P2 | proposal | Dashboard text: Each metric label sits flush against its card's left edge. | Add a consistent text inset without changing the card widths. |
| hstack_showcase-23 | P3 | proposal | Dashboard values: The metric cards have labels but no sample values. | Add small fixed values to make the dashboard example recognizable. |
| hstack_showcase-24 | P3 | proposal | Dashboard color key: Six bright colors suggest meaning without a legend. | Caption them as identity colors or reduce unrelated semantic associations. |
| hstack_showcase-25 | P3 | proposal | Bottom empty space: A broad blank region remains below the dashboard row. | Use it for the stack width and gap configuration summary. |

## images

[Reviewed image](../../docs/design-audit/originals/images_720p.png) · [Current baseline](../../screenshot-baselines/screens/images_720p.png) · [Visual review](../../design-audit.html#screen=images)

Baseline SHA-256: `71d7b50a816120335ffcbc75e3c765af1e147812c32cf300572f43703604c81a`

The sprites render cleanly, and the resting button examples are intact. Proposals clarify icon semantics and the differences among the four demonstrations.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| images-01 | P2 | defect | Play sprite: The icon labeled Play shows two right-pointing triangles, conventionally fast-forward. | Use a single play triangle or rename the specimen Fast forward. |
| images-02 | P2 | proposal | Page title: Image & Sprite Components uses small text across a wide title bar. | Increase the heading and shorten the bar to the content width. |
| images-03 | P2 | proposal | Section label styles: Sprites and With Background are cyan while Image Buttons and Icon Row are red. | Use one section-label treatment unless color has a documented meaning. |
| images-04 | P2 | proposal | Sprite names: Gear and Trophy become Settings and Awards in the icon row. | Show glyph name and intended action as separate consistent labels. |
| images-05 | P2 | proposal | Favorite naming: Favorite in the button row becomes Favorites in the icon row. | Choose one action label across both examples. |
| images-06 | P2 | proposal | Sprite row alignment: Captions are offset left of the apparent icon centers. | Center each caption directly beneath its sprite. |
| images-07 | P2 | proposal | Icon-row captions: Settings and Play use small widely spaced lettering. | Increase caption size and reduce tracking. |
| images-08 | P3 | proposal | Gear visual size: The gear appears smaller than the home and play silhouettes. | Normalize optical icon size in the comparison row. |
| images-09 | P3 | proposal | Star specimen: The star has a smaller visual footprint than the trophy. | Add consistent image bounds behind the specimens to expose size differences. |
| images-10 | P2 | proposal | Sprite metadata: The first row provides no source dimensions or scale. | Add a compact 100 × 100 source metadata caption. |
| images-11 | P3 | proposal | Image-button frame: The three buttons use unrelated fill colors. | Use a shared frame color or label each theme variant. |
| images-12 | P2 | proposal | Button versus sprite: The difference between the first and second rows is implied only by titles. | Add Static image and Clickable image badges beside their headings. |
| images-13 | P2 | proposal | Click counter: Clicks: 0 sits in open space away from the three buttons. | Place it in a visible shared result badge adjacent to the button group. |
| images-14 | P3 | proposal | Counter scope: Clicks: 0 does not say whether all buttons share the counter. | Label it Total image-button clicks. |
| images-15 | P2 | proposal | With Background sample: The purple Settings rectangle is not described as a compound image-and-label control. | Rename the section Image with text background. |
| images-16 | P2 | proposal | Compound Settings spacing: The gear and Settings text have a large unstructured gap. | Use a consistent icon-to-label gap and centered vertical alignment. |
| images-17 | P3 | proposal | Third row height: A single small sample sits in a long full-width panel. | Fit the sample panel to its contents or add its geometry annotation. |
| images-18 | P3 | proposal | Icon row interaction: The icon row looks similar to the button row but its role is unclear. | Label whether these are static icons or actions. |
| images-19 | P2 | proposal | Section label widths: Sprites and Icon Row use narrower labels than the other rows. | Use a fixed section-label column to align all sample starts. |
| images-20 | P2 | proposal | Sample columns: The first and fourth rows place matching icons at different x positions. | Align matching glyphs vertically across both rows. |
| images-21 | P3 | proposal | Right empty space: Each row leaves a broad unused area on the right. | Use the area for source-rectangle and scale annotations. |
| images-22 | P3 | proposal | Bottom empty space: The page leaves a large empty band beneath Icon Row. | Center the gallery vertically or add a concise rendering-specification footer. |
| images-23 | P2 | proposal | Home silhouette: The filled house is a strong shape compared with the detailed outlined gear. | Use consistent visual weight in the action row while retaining raw sprites in the first row. |
| images-24 | P3 | proposal | Sprite background contrast: All raw sprites are white on dark with no alternative backdrop reference. | Add a small light-background companion sample for transparency inspection. |
| images-25 | P3 | proposal | Image boundaries: The visible glyphs do not expose their rectangular image bounds. | Offer thin optional bounds around the raw sprite samples for inspection. |

### Additional capture: images

[Image](../../baseline_screenshots/images.png)

Active e2e baseline refreshed after visual review. The previous capture remains in originals until audit cleanup.

## islands trains settings

[Reviewed image](../../docs/design-audit/originals/islands_trains_settings_720p.png) · [Current baseline](../../screenshot-baselines/screens/islands_trains_settings_720p.png) · [Visual review](../../design-audit.html#screen=islands_trains_settings)

Baseline SHA-256: `1869c9b236db424413262914b45f4859eacccf71486cc54ca5019f84852f75d4`

The paper-strip settings design matches the mock. Its muted palette and compact rows need stronger reading hierarchy and clearer value displays.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| islands_trains_settings-01 | P2 | proposal | Settings title: The title is small and muted at the top of the tall paper. | Increase title contrast and size while retaining the hand-lettered style. |
| islands_trains_settings-02 | P2 | proposal | Section headings: DISPLAY, CONTROLS, AUDIO, and TUTORIAL are pale cyan on cream. | Darken the section-heading color for clearer grouping. |
| islands_trains_settings-03 | P2 | proposal | Setting labels: Olive-gray labels have little contrast against sage rows. | Use darker olive text on the same soft row backgrounds. |
| islands_trains_settings-04 | P2 | proposal | Row height: Display and audio controls occupy very short strips. | Increase row height and vertical text padding. |
| islands_trains_settings-05 | P2 | proposal | Mode value: Borderless is not visually distinguished from Mode. | Use a stronger value weight and a fixed value column. |
| islands_trains_settings-06 | P2 | proposal | Resolution formatting: 2560x1440 uses a cramped lowercase x. | Use 2560 × 1440 with readable spacing. |
| islands_trains_settings-07 | P2 | proposal | Cycle arrows: Small angle-bracket characters sit near the values. | Use consistent chevron buttons with visible hit-area framing. |
| islands_trains_settings-08 | P2 | proposal | Camera labels: Cam panning speed nearly touches its decrease arrow. | Increase the label-to-control gap or shorten the label to Pan speed. |
| islands_trains_settings-09 | P2 | proposal | Camera capitalization: Cam rotating Speed capitalizes Speed inconsistently. | Use Camera rotation speed in sentence case. |
| islands_trains_settings-10 | P2 | proposal | Segment contrast: Active cyan and inactive tan segments are similar in lightness. | Increase their distinction while keeping the pastel palette. |
| islands_trains_settings-11 | P2 | proposal | Camera values: The two camera controls display segments without numeric values. | Add current step numbers beside both segment groups. |
| islands_trains_settings-12 | P2 | proposal | Effects value: Effects Volume has no numeric percentage. | Show its percentage in a compact trailing value field. |
| islands_trains_settings-13 | P2 | proposal | Music value: Music Volume has no numeric percentage. | Show its percentage aligned with the effects value. |
| islands_trains_settings-14 | P3 | proposal | Segment scale: Ten short bars have no minimum or maximum annotation. | Add one shared Low to High legend for the segmented controls. |
| islands_trains_settings-15 | P2 | proposal | Keyboard action: KEYBOARD resembles another passive setting row. | Give it a navigation arrow and label it Keyboard controls. |
| islands_trains_settings-16 | P2 | proposal | Tutorial action: PLAY TUTORIAL has the same subdued appearance as the setting rows. | Use a distinct secondary-action style with a play icon. |
| islands_trains_settings-17 | P3 | proposal | Tutorial section: The section contains only an action with no explanatory text. | Add a brief note describing what the tutorial covers. |
| islands_trains_settings-18 | P2 | proposal | Reset placement: RESET TO DEFAULTS sits almost on the bottom edge. | Move it upward into a clear footer safe area. |
| islands_trains_settings-19 | P2 | proposal | Reset affordance: The reset action is plain low-contrast text. | Give it a bordered secondary-button treatment. |
| islands_trains_settings-20 | P3 | proposal | Reset scope: Reset to defaults does not state which settings it affects. | Add a short All settings caption beside the action. |
| islands_trains_settings-21 | P2 | proposal | Close control: The X is isolated far from the narrow paper panel. | Align it with the panel's top corner or add a clear Close label. |
| islands_trains_settings-22 | P3 | proposal | Paper width: The narrow panel leaves large blank side fields. | Widen the paper enough to accommodate readable controls and labels. |
| islands_trains_settings-23 | P3 | proposal | Lower paper void: A large blank region separates Play tutorial from Reset. | Reduce that gap and group reset within the settings footer. |
| islands_trains_settings-24 | P3 | proposal | Section spacing: The gap before AUDIO is notably larger than before CONTROLS. | Use a consistent section-spacing rhythm. |
| islands_trains_settings-25 | P3 | proposal | Save behavior communication: No visible note explains whether changes apply immediately. | Add a small Changes apply immediately message if that matches behavior. |

## kart select

[Reviewed image](../../docs/design-audit/originals/kart_select_720p.png) · [Current baseline](../../screenshot-baselines/screens/kart_select_720p.png) · [Visual review](../../design-audit.html#screen=kart_select)

Baseline SHA-256: `856192ba3e5d5faea51a2cf4c47839fd73618d02fcb2c3d4f94a1cfdb2598fe6`

The selected racer and vehicle are visible. Improve consistency between the portrait, kart preview, selection badges, and stat presentation.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| kart_select-01 | P2 | proposal | Preview driver cap: The Drift portrait carries a P on its cap while the large driver cap is blank. | Repeat the portrait cap detail in the large driver preview to strengthen character continuity. |
| kart_select-02 | P2 | proposal | Vehicle thumbnails: Each vehicle thumbnail includes a differently colored driver, suggesting a racer change. | Render the selected racer consistently across all vehicle thumbnails. |
| kart_select-03 | P2 | proposal | Selected vehicle color: Standard shows a red kart while the large preview is orange. | Match the thumbnail to the selected vehicle's preview appearance. |
| kart_select-04 | P2 | proposal | Selected racer badge: P1 floats above the card and interrupts its border. | Place the badge within a reserved top corner of the selected card. |
| kart_select-05 | P2 | proposal | Player indicator duplication: P1 appears in both the header and selected portrait with different shapes. | Use a consistent badge shape and color in both locations. |
| kart_select-06 | P2 | proposal | Racer identity: Large Drift text appears below the vehicle without a clear section label. | Label the preview Selected racer and vehicle. |
| kart_select-07 | P2 | proposal | Vehicle identity: The preview names Drift but does not repeat Standard. | Add Standard beneath the racer name. |
| kart_select-08 | P2 | proposal | Stats values: Four segmented bars have no numeric scores. | Add values such as 6/10 in a trailing column. |
| kart_select-09 | P2 | proposal | Stats scale: The bars contain many narrow ticks without a labeled range. | Use a clearly stated common maximum scale. |
| kart_select-10 | P2 | proposal | Stat labels: Speed and Acceleration are small compared with the preview name. | Increase stat labels and align their baselines with the bars. |
| kart_select-11 | P3 | proposal | Weight interpretation: Weight uses the same positive yellow fill as speed without context. | Add a short tradeoff description for heavier handling. |
| kart_select-12 | P2 | proposal | Portrait name strips: Racer names are small in thin pale strips. | Increase name text or strip height slightly. |
| kart_select-13 | P3 | proposal | Portrait art: Eight racers share nearly identical facial silhouettes. | Add more distinct facial or costume features to aid recognition. |
| kart_select-14 | P2 | proposal | Selected portrait border: Yellow fill and a thick yellow border share little contrast. | Add a dark selected outline or check marker inside the card. |
| kart_select-15 | P2 | proposal | Vehicle selection: Standard relies largely on yellow fill to mark selection. | Add a check or Selected label to its card. |
| kart_select-16 | P2 | proposal | Vehicle section title: CHOOSE YOUR VEHICLE is small and close to the cards. | Increase the label and add a clear section gap. |
| kart_select-17 | P3 | proposal | Vehicle naming: Off-Road is styled differently from plain one-word names. | Use consistent title casing across all vehicle labels. |
| kart_select-18 | P2 | proposal | Preview illustration: The preview driver is far less detailed than the selection portraits. | Bring the large driver art to the same character style as the cards. |
| kart_select-19 | P3 | proposal | Preview platform: The oversized pale oval occupies substantial vertical space. | Reduce platform thickness to give the stat card more breathing room. |
| kart_select-20 | P2 | proposal | Footer hints: Arrows to choose and A Confirm B Back are tiny beside the Back button. | Increase the hint size and use clearer keycaps. |
| kart_select-21 | P2 | proposal | Back duplication: A large Back action and a B Back hint repeat the same function side by side. | Combine them into one labeled action with its key hint. |
| kart_select-22 | P2 | proposal | Ready action: READY! does not state the next step. | Add a small Start Grand Prix or Continue caption appropriate to behavior. |
| kart_select-23 | P3 | proposal | Ready button padding: The action text and trailing arrow are spread across the large skewed button. | Use a controlled icon-to-label gap. |
| kart_select-24 | P3 | proposal | Header context: Grand Prix · Mushroom Cup is smaller and distant from the screen title. | Group race context under the title or closer to the player indicator. |
| kart_select-25 | P3 | proposal | Background checks: The checker texture is strong behind the portrait grid. | Lower texture contrast behind cards and stat text. |

## kirby options

[Reviewed image](../../docs/design-audit/originals/kirby_options_720p.png) · [Current baseline](../../screenshot-baselines/screens/kirby_options_720p.png) · [Visual review](../../design-audit.html#screen=kirby_options)

Baseline SHA-256: `283ee819fc76147bc7c685b06a0b33c0360f4fe1d8bb6a822d3e7ac1f25496ce`

The stationery-board composition matches the mock. Refine the interactive hierarchy so decorative notebook elements and large category tiles do not compete with profile editing.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| kirby_options-01 | P2 | proposal | Name control: Name is a generic placeholder in the largest yellow control. | Display a sample profile name and a smaller Edit name cue. |
| kirby_options-02 | P2 | proposal | Name icon: The large profile silhouette consumes much of the name pill. | Reduce it to make room for the actual profile value. |
| kirby_options-03 | P2 | proposal | Delete Data action: A small pawn-like symbol does not communicate deletion. | Use a clear delete-data icon with the existing label. |
| kirby_options-04 | P2 | proposal | Delete action hierarchy: Delete Data sits beside the primary name control without a destructive distinction. | Use a restrained warning accent and separate it from profile editing. |
| kirby_options-05 | P2 | proposal | Delete label: The label is much smaller than all other action names. | Increase it to a readable secondary-action size. |
| kirby_options-06 | P2 | proposal | Common section tag: Common is white on a pale gray strip and small. | Increase its contrast and use a stronger section-label treatment. |
| kirby_options-07 | P2 | proposal | Bottom help: The long help sentence sits outside the board near the viewport edge. | Move it into a padded footer region inside the board. |
| kirby_options-08 | P2 | proposal | Help context: Enter a new name and customize your controls describes multiple actions at once. | Use one specific description for the current selection. |
| kirby_options-09 | P2 | proposal | Top tabs: Network, controller, home, mail, and star are icon-only. | Add compact text labels beneath each icon or a visible selected-tab caption. |
| kirby_options-10 | P2 | proposal | Options selection: Options is indicated by a purple tab and separate purple banner. | Connect the banner visually to the selected gear tab. |
| kirby_options-11 | P3 | proposal | Tab spacing: The top icon tabs have irregular apparent gaps due to rotation. | Use a consistent transformed baseline and gap rhythm. |
| kirby_options-12 | P2 | proposal | L and R hints: Small purple key labels float outside the tabs. | Use consistent keycap boxes aligned with the tab row. |
| kirby_options-13 | P2 | proposal | Controller tile: The controller icon is tilted independently from the text and board. | Use a consistent icon orientation within each tile. |
| kirby_options-14 | P2 | proposal | Display tile: Display uses a thin monitor outline beside heavier filled icons. | Normalize icon stroke and visual weight. |
| kirby_options-15 | P2 | proposal | Accessibility tile: The wide Accessibility label approaches the cell edges. | Increase label room or reduce its size slightly within the shared type scale. |
| kirby_options-16 | P2 | proposal | Sound and Online layout: Bottom-row labels sit beside icons while top-row labels sit below them. | Use one icon-label layout or explicitly distinguish the two groups. |
| kirby_options-17 | P3 | proposal | Tile grid proportions: Two broad bottom cells differ strongly from the three upper cells. | Use consistent cell proportions or make the grouping meaningful with labels. |
| kirby_options-18 | P2 | proposal | Grid lines: Thin beige dividers are faint against off-white paper. | Increase divider contrast slightly to clarify the clickable tile boundaries. |
| kirby_options-19 | P3 | proposal | Notebook checkboxes: Five empty notebook boxes resemble interactive tasks without readable labels. | Render them as clearly decorative sketches or add meaningful task labels. |
| kirby_options-20 | P3 | proposal | Notebook block text: Opaque square marks substitute for writing. | Use believable low-emphasis handwritten lines to support the stationery theme. |
| kirby_options-21 | P2 | proposal | Notebook star button: The green oval star looks interactive but has no visible label. | Add a Profile customization caption near the oval. |
| kirby_options-22 | P3 | proposal | Pencil decoration: Large outlined pencils cover the notebook's last two rows. | Move them into a margin where they do not obscure the faux content. |
| kirby_options-23 | P3 | proposal | Notebook footprint: The decorative notebook occupies almost a third of the screen. | Reduce it slightly to give the options board more whitespace. |
| kirby_options-24 | P3 | proposal | Board shadow: Strong paper shadows and dark notebook shadows compete with tile boundaries. | Soften decorative shadows around the main reading area. |
| kirby_options-25 | P3 | proposal | Footer underline: The purple line extends nearly to the screen edge below the help text. | Align it with the inner board margins and shorten it to the help region. |

## language demo

[Reviewed image](../../docs/design-audit/originals/language_demo_720p.png) · [Current baseline](../../screenshot-baselines/screens/language_demo_720p.png) · [Visual review](../../design-audit.html#screen=language_demo)

Baseline SHA-256: `396cda5942420cb0978a10f28f21a6979b322f60303c88cc33b393755ceb110d`

The three scripts render in the current baseline. Twenty distinct proposals improve language selection and the comparison display; no missing-glyph or switching failure is inferred.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| language_demo-01 | P2 | proposal | Language selectors: EN, KO, and JA show ISO codes without native names. | Add English, 한국어, and 日本語 in a wider selector group. |
| language_demo-02 | P2 | proposal | Selected language: EN is blue while the other languages are green, without a legend. | Use one control palette and a clear selected marker. |
| language_demo-03 | P2 | proposal | Selector spacing: The three language pills touch one another. | Add small uniform gaps between the controls. |
| language_demo-04 | P2 | proposal | Keyboard hints: [1], [2], and [3] are part of each label string. | Show the digits in separate compact keycaps. |
| language_demo-05 | P2 | proposal | Title bar: The title occupies an oversized blue region that merges into EN. | Separate the heading visually from the language picker. |
| language_demo-06 | P2 | proposal | Active panel heading: Hello! occupies a large mustard strip without saying it is the active language. | Add Current language: English above the greeting specimen. |
| language_demo-07 | P2 | proposal | Greeting typography: Hello! is left-aligned while all menu labels are centered. | Use a deliberate specimen alignment with consistent content insets. |
| language_demo-08 | P2 | proposal | Menu comparison scope: The left panel shows five actions while the right compares only Hello and Start. | Add aligned translations for the same set of menu labels. |
| language_demo-09 | P2 | proposal | Comparison rows: EN, KO, and JA text are embedded in unstructured sentences. | Use Language, Greeting, and Start columns. |
| language_demo-10 | P2 | proposal | Script scale: Korean appears markedly smaller than the handwritten English example. | Normalize optical reading size while retaining appropriate fonts per script. |
| language_demo-11 | P2 | proposal | Japanese baseline: The Japanese sample uses a different apparent vertical alignment. | Align row baselines or document the script-specific metrics in captions. |
| language_demo-12 | P2 | proposal | Comparison heading: All Languages is small above a broad empty panel. | Increase the heading and add a short font-comparison subtitle. |
| language_demo-13 | P3 | proposal | Unused comparison area: Most of the right panel below the three examples is empty. | Add a multiline localized specimen to use the available area. |
| language_demo-14 | P2 | proposal | Continue action: Continue shares the same styling as Start, Settings, About, and Exit. | Separate the primary action from the menu specimen list. |
| language_demo-15 | P2 | proposal | Exit ordering: Exit appears directly above Continue without grouping. | Move Exit into a distinct secondary action group. |
| language_demo-16 | P2 | proposal | Font metadata: Active: ENGLISH and Font: Gaegu-Bold share one footer line. | Split language and active-font metadata into labeled values. |
| language_demo-17 | P3 | proposal | Font coverage: Only the active English font is named. | Show the font used for each script in the comparison rows. |
| language_demo-18 | P2 | proposal | Footer instructions: Press 1/2/3 to switch languages repeats the key labels without mapping them. | Show 1 English · 2 Korean · 3 Japanese in the footer. |
| language_demo-19 | P3 | proposal | Panel junction: The two side panels meet the footer with small triangular background gaps. | Use consistent panel corner treatment at their junction. |
| language_demo-20 | P3 | proposal | Viewport composition: The content ends well above the bottom of the black viewport. | Center the complete demo panel vertically or fit its background to the viewport. |

### Additional capture: language_demo

[Image](../../baseline_screenshots/language_demo.png)

Active e2e baseline refreshed after visual review. The previous capture remains in originals until audit cleanup.

## layout

[Reviewed image](../../docs/design-audit/originals/layout_720p.png) · [Current baseline](../../screenshot-baselines/screens/layout_720p.png) · [Visual review](../../design-audit.html#screen=layout)

Baseline SHA-256: `4b54ed57c809259cea0fdfdcb280df7c001d2a498583d57454596ea2f2feead3`

The row and column arrangements are intentional examples. Sixteen distinct proposals improve teaching and measurements; additional findings would be arbitrary changes to this small fixture.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| layout-01 | P2 | proposal | Page title: Layout System Demo does not identify the two demonstrated arrangements. | Add Row and column layouts as a concise subtitle. |
| layout-02 | P2 | proposal | Top sample: Left, Center, and Right panels have no local section heading. | Label the top frame Row · centered children. |
| layout-03 | P2 | proposal | Lower sample: Stacked Item 1 through 3 have no local section heading. | Label the lower frame Column · stacked children. |
| layout-04 | P2 | proposal | Guide order: The footer describes Column before Row while the examples show Row first. | Order the guide descriptions to match the examples. |
| layout-05 | P2 | proposal | Guide proximity: Row and Column descriptions are far below their corresponding samples. | Place each explanation directly beneath its sample. |
| layout-06 | P3 | proposal | Panel names: Left Panel and Right Panel describe position rather than the layout rule. | Use numbered items and keep position names in the frame caption. |
| layout-07 | P3 | proposal | Top row gaps: The gaps between three fixed-width panels are visible but unmeasured. | Add a fixed-gap annotation outside the samples. |
| layout-08 | P3 | proposal | Column gaps: The tightly stacked items have no spacing caption. | Add a vertical gap bracket with the configured value. |
| layout-09 | P3 | proposal | Margins statement: Margins control spacing has no visual reference. | Draw an external margin guide around one sample frame. |
| layout-10 | P2 | proposal | Column inner frame: A gray rounded panel nests inside a large white panel without naming their roles. | Label Parent and Child stack outside their boundaries. |
| layout-11 | P3 | proposal | Sample color mapping: Green, brown, and rust are reused without an explicit item-identity key. | Add a small color key linking the same three items across both layouts. |
| layout-12 | P2 | proposal | Sample typography: Panel names and stacked-item text are small relative to their large colored blocks. | Increase sample-label size with equal insets. |
| layout-13 | P3 | proposal | Divider lengths: The thin separators are narrower than the white sample panels. | Align divider endpoints with the shared content inset. |
| layout-14 | P2 | proposal | Theme footer: Using Sage Natural theme with responsive layouts is tiny at the bottom. | Increase footer readability and separate theme metadata from the layout lesson. |
| layout-15 | P3 | proposal | Responsive claim: The footer names responsive layouts without exposing container dimensions. | Show current viewport and parent widths as demo metadata. |
| layout-16 | P3 | proposal | Vertical composition: The demo's pale root panel leaves a black band below it. | Center the complete composition vertically within the viewport. |

### Additional capture: layout

[Image](../../baseline_screenshots/layout.png)

Active e2e baseline refreshed after visual review. The previous capture remains in originals until audit cleanup.

## layout bug repros

[Reviewed image](../../docs/design-audit/originals/layout_bug_repros_720p.png) · [Current baseline](../../screenshot-baselines/screens/layout_bug_repros_720p.png) · [Visual review](../../design-audit.html#screen=layout_bug_repros)

Baseline SHA-256: `e7daac628b2df245860ce0969569516aa7511f9c52dbfd9a2f51a795b673e8ef`

Preserve all repro geometry, diagnostic colors, origin marker, overlap, and intentional differences. Findings concern presentation and misleading expectation copy rather than declaring historical bugs still active.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| layout_bug_repros-01 | P2 | defect | Z-order caption: Case 4 says NOT A BUG but its expected-result line calls the visible blue-on-top order a bug. | Rewrite the expectation to state creation-order rendering and identify blue-on-top as the expected result. |
| layout_bug_repros-02 | P2 | proposal | Case status labels: FIXED and NOT A BUG are embedded in long small captions. | Use separate status badges outside the test geometry. |
| layout_bug_repros-03 | P2 | proposal | Case headings: All seven descriptions use small handwritten text. | Increase heading readability without changing the repro coordinates. |
| layout_bug_repros-04 | P2 | proposal | Expected-result lines: The expected and bug descriptions are tiny and dense. | Put expected behavior in a clear two-line caption beneath each case. |
| layout_bug_repros-05 | P2 | proposal | Number sequence: Cases 1–5 run down the left and 6–7 start again at the top right. | Add clear column labels or a numbered index to establish reading order. |
| layout_bug_repros-06 | P2 | proposal | Origin marker: The red plus at the top-left looks like an unexplained floating control. | Add an Origin 0,0 caption nearby outside its measured location. |
| layout_bug_repros-07 | P3 | proposal | Percent sample: Red and cyan overlap by design but their dimensions are not shown. | Add parent 300 × 80 and child 100% dimension captions. |
| layout_bug_repros-08 | P3 | proposal | Percent expectation: Red fits inside cyan is stated in prose only. | Add a small expected-geometry diagram outside the live sample. |
| layout_bug_repros-09 | P3 | proposal | Row sample: A, B, and C show fixed boxes without width annotations. | Caption each child's fixed 100-pixel width. |
| layout_bug_repros-10 | P3 | proposal | Row leftover space: The gray parent extends far beyond C without explanation. | Label the parent's full width to explain the unused region. |
| layout_bug_repros-11 | P2 | proposal | Flow sample caption: The flow sample is far to the right of its case heading. | Connect the heading to the yellow parent with a subtle leader outside the test bounds. |
| layout_bug_repros-12 | P3 | proposal | Flow coordinates: The heading embeds 400,310 without units or a coordinate key. | Show x and y labels explicitly in the metadata. |
| layout_bug_repros-13 | P3 | proposal | Flow children: Child 1 and Child 2 have no dimension metadata. | Add external child-height and gap labels. |
| layout_bug_repros-14 | P2 | proposal | Z-order label obstruction: The red box's identifier is covered by the blue box as part of the test. | Repeat both entity identifiers in an unobstructed external legend. |
| layout_bug_repros-15 | P2 | proposal | Z-order question: BLUE mk(2000) ON TOP? reads as unresolved despite the status badge. | Use a neutral Blue entity · created second label outside the overlap. |
| layout_bug_repros-16 | P3 | proposal | Sizing comparison: The green and blue buttons are stacked far below their explanation. | Add explicit Auto width and 250 px reference captions beside them. |
| layout_bug_repros-17 | P3 | proposal | Sizing difference: The two button widths are close but difficult to compare vertically. | Add aligned width guides extending from their outer edges. |
| layout_bug_repros-18 | P2 | proposal | Tab fixture: Tab A's yellow underline has no textual selected-state caption. | Add Selected: Tab A in the fixture metadata. |
| layout_bug_repros-19 | P3 | proposal | Tab container bounds: The green outline is thin and hard to distinguish from surrounding dark space. | Add an external labeled outline key instead of altering the tested border. |
| layout_bug_repros-20 | P2 | proposal | Toggle labels: Toggle A, B, and C are far from their switches across a wide panel. | Add light row guides outside the toggle geometry to connect labels and controls. |
| layout_bug_repros-21 | P3 | proposal | Toggle state: The yellow/gray switch states are not named. | Add an external On, Off, On state summary. |
| layout_bug_repros-22 | P3 | proposal | Toggle measurement: The vertical-fit case names the concept without showing measured height. | Display the declared container height and summed child heights. |
| layout_bug_repros-23 | P2 | proposal | Right empty region: Most of the lower-right canvas is unused. | Use it for a compact fixture legend and expected-results summary. |
| layout_bug_repros-24 | P3 | proposal | Diagnostic color key: Red, cyan, green, yellow, blue, and purple carry different roles across cases. | Add a per-case parent/child color key. |
| layout_bug_repros-25 | P2 | proposal | Page purpose: Layout Bug Repros alone does not distinguish current regressions from retained historical cases. | Add Retained regression fixtures; badges describe current expectations beneath the title. |

## layout patterns

[Reviewed image](../../docs/design-audit/originals/layout_patterns_720p.png) · [Current baseline](../../screenshot-baselines/screens/layout_patterns_720p.png) · [Visual review](../../design-audit.html#screen=layout_patterns)

Baseline SHA-256: `617fc8c69a4f01dd6f0db1589d22318d151422289cbd9e617d4a8c3f1c1aa042`

Source identifies this as a minimal fixed-size fixture for assert_ui checks. Ten external presentation proposals are defensible; preserve the three boxes and their exact geometry rather than inventing 25 defects.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| layout_patterns-01 | P2 | proposal | Page identification: The screenshot contains three numbered boxes with no title. | Add a caption outside the measured fixture naming the fixed horizontal layout test. |
| layout_patterns-02 | P2 | proposal | Test purpose: Nothing visible explains why the three boxes exist. | Add a one-line explanation of the expected row arrangement. |
| layout_patterns-03 | P2 | proposal | Parent dimensions: The gray parent has no dimension annotation. | Show its width and height outside the tested rectangle. |
| layout_patterns-04 | P3 | proposal | Child dimensions: The three equal boxes have no size labels. | Add an external 120-pixel child-width annotation. |
| layout_patterns-05 | P3 | proposal | Gap measurement: The equal spaces between boxes are visible but unnamed. | Add a 12-pixel gap marker outside the fixture. |
| layout_patterns-06 | P3 | proposal | Padding measurement: The gray parent padding is visible without a value. | Add a 16-pixel padding guide outside the test geometry. |
| layout_patterns-07 | P2 | proposal | Fixture position: The sample sits at the upper-left of a mostly blank canvas. | Mark its fixed origin in an external caption so the placement reads as intentional. |
| layout_patterns-08 | P3 | proposal | Expected result: No visual statement describes the successful geometry. | Add Three equal children fit without overflow beneath the sample. |
| layout_patterns-09 | P3 | proposal | Number legibility: Black handwritten numerals are small within the bright boxes. | Provide matching enlarged number labels in the external legend while preserving the tested content. |
| layout_patterns-10 | P3 | proposal | Color meaning: Cyan, pink, and green are not explained. | Add an external legend stating that colors identify child order. |

### Additional capture: layout_patterns

[Image](../../baseline_screenshots/layout_patterns.png)

Active e2e baseline refreshed after visual review. The previous capture remains in originals until audit cleanup.

## marlo kart

[Reviewed image](../../docs/design-audit/originals/marlo_kart_720p.png) · [Current baseline](../../screenshot-baselines/screens/marlo_kart_720p.png) · [Visual review](../../design-audit.html#screen=marlo_kart)

Baseline SHA-256: `3a6553a940a4bfb68d6d7c185750570b125c09d79cd80260deaf1c38ff0d2397`

The title state is recognizable and the selected engine class is visible. Refine the title-screen hierarchy and racing illustration without assuming anything about later game phases.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| marlo_kart-01 | P2 | proposal | Engine-class group: 50cc, 100cc, and 150cc have no group label. | Add Engine class above the three choices. |
| marlo_kart-02 | P2 | proposal | Engine explanation: The classes provide displacement values without difficulty context. | Add short speed/difficulty descriptors below each option. |
| marlo_kart-03 | P2 | proposal | Selected 150cc: The selected state relies on yellow fill. | Add a check or Selected label to 150cc. |
| marlo_kart-04 | P3 | proposal | Engine typography: The labels combine number and cc tightly. | Use consistent spacing and tabular numerals for the three values. |
| marlo_kart-05 | P2 | proposal | Start label: START YOUR ENGINES is broad promotional copy for entering the next phase. | Add a concise Select racer sublabel to clarify the next step. |
| marlo_kart-06 | P2 | proposal | Start control size: The start button is much wider than the engine selector group. | Align its outer width with the group above. |
| marlo_kart-07 | P2 | proposal | Start text padding: The long label nearly fills the button width. | Increase horizontal padding or reduce the text size slightly. |
| marlo_kart-08 | P2 | proposal | Control group spacing: The class buttons almost touch the large action beneath them. | Increase the gap between selection and confirmation. |
| marlo_kart-09 | P2 | proposal | Keyboard hints: Arrows: engine class and Enter: start are tiny near the bottom. | Use readable keycap-and-label groups beside the controls. |
| marlo_kart-10 | P3 | proposal | Keyboard alignment: The two keyboard hints have uneven separation and no shared structure. | Align them on one baseline with equal group spacing. |
| marlo_kart-11 | P2 | proposal | Game-format sentence: Four tracks. Eight racers. One trophy. is very small. | Increase supporting-copy size and contrast. |
| marlo_kart-12 | P3 | proposal | Subtitle: AFTERHOURS GRAND PRIX is tiny under the oversized logo. | Give the subtitle a stronger size and stable baseline. |
| marlo_kart-13 | P3 | proposal | Logo spacing: MARLO and KART have a large irregular vertical gap. | Tighten the two-line logo while retaining its playful tilt. |
| marlo_kart-14 | P3 | proposal | Logo outline: The heavy dark outline creates uneven visual weight around the letters. | Use a more consistent outline and shadow treatment. |
| marlo_kart-15 | P2 | proposal | Kart badge: The red kart has a blank white nose badge; the web mock places a red M inside it. | Restore the red M brand emblem inside the white nose badge. |
| marlo_kart-16 | P2 | proposal | Driver face: The large driver has a single dot and no other facial detail. | Bring the illustration closer to the expressive racer portrait style. |
| marlo_kart-17 | P3 | proposal | Driver hands: Broad overlapping arm shapes obscure the steering wheel. | Clarify hand positions and the wheel silhouette. |
| marlo_kart-18 | P3 | proposal | Kart wheel perspective: Front and rear wheels use noticeably different angle and scale cues. | Refine their perspective to match the chassis plane. |
| marlo_kart-19 | P3 | proposal | Exhaust: The pale exhaust shape resembles a detached rectangular strip. | Add a clearer pipe connection and consistent shading. |
| marlo_kart-20 | P3 | proposal | Kart shadow: A large dark green oval competes with the wheel shapes. | Soften and reduce the shadow to ground the kart. |
| marlo_kart-21 | P2 | proposal | Racing context: The vehicle sits on flat green ground with no visible track. | Add a restrained road or track curve behind the kart. |
| marlo_kart-22 | P3 | proposal | Mountain backdrop: Large flat triangles create a static horizon behind the logo. | Add depth through a second subdued ridge layer. |
| marlo_kart-23 | P3 | proposal | Horizon stripe: A bright horizontal green band cuts through the class controls. | Place controls on a consistent backing or move the horizon away from them. |
| marlo_kart-24 | P3 | proposal | Composition balance: The kart dominates the right while controls cluster low on the left. | Move the controls slightly upward to balance the logo and vehicle. |
| marlo_kart-25 | P2 | proposal | Secondary navigation: The title state exposes only engine class and start. | Add compact Settings and Quit/Back actions if available in this title flow. |

## media library

[Reviewed image](../../docs/design-audit/originals/media_library_720p.png) · [Current baseline](../../screenshot-baselines/screens/media_library_720p.png) · [Visual review](../../design-audit.html#screen=media_library)

Baseline SHA-256: `b2623fee8eef0f49f715eebe698f084cc77dac22ae09c946f6e042f006cb11b2`

The library has a useful grid-detail structure and coherent artwork. Improve readability, selection alignment, and the distinction between navigation, filtering, and media metadata.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| media_library-01 | P2 | proposal | Search placeholder: Search your library is tiny inside a wide field. | Increase placeholder size and add a recognizable search icon. |
| media_library-02 | P2 | proposal | Header actions: Home, Server, and Settings are small and low contrast. | Increase their text size and separate them with consistent spacing. |
| media_library-03 | P2 | proposal | Profile badge: The lone G appears in a square without context. | Use a recognizable avatar treatment and a visible account label when space permits. |
| media_library-04 | P2 | proposal | Sidebar headings: YOUR MEDIA and SERVER are extremely small. | Increase their size and contrast without competing with navigation. |
| media_library-05 | P2 | proposal | Sidebar alignment: Navigation text is centered while server text is left-aligned. | Left-align navigation labels to the same sidebar inset. |
| media_library-06 | P2 | proposal | Library count: 24 is detached from Library and faint. | Present it as a compact 24 items badge beside the heading. |
| media_library-07 | P2 | proposal | Repeated Library names: Library appears in the sidebar, page heading, and a tab. | Rename the inner tab All titles or another distinct view name. |
| media_library-08 | P2 | proposal | Recommended selection: The selected tab uses only a thin yellow underline. | Increase selected label contrast and keep the underline tightly aligned. |
| media_library-09 | P2 | proposal | All media filter: All media looks like static text without a dropdown indicator. | Add a trailing chevron and visible filter-control boundary. |
| media_library-10 | P2 | proposal | By title sort: By title appears as another loose label. | Show Sort: Title with a chevron or direction indicator. |
| media_library-11 | P2 | proposal | Grid List switch: Grid and List use tiny text with a faint selected background. | Use a clear segmented control with icons and readable labels. |
| media_library-12 | P2 | proposal | Selected poster frame: The Harbour Lights outline spans the whole tile width while the poster is narrower. | Wrap the selection frame around the actual poster or the full card including its metadata. |
| media_library-13 | P2 | proposal | Poster gutters: Portrait posters sit within wide dark horizontal gutters. | Use consistent poster-width cards to give the grid a stronger rhythm. |
| media_library-14 | P2 | proposal | Media titles: Harbour Lights and other titles are small relative to their artwork. | Increase title size and weight slightly. |
| media_library-15 | P2 | proposal | Year and type rows: Years sit left while Feature and Series sit far right in each tile. | Group metadata with a dot separator beneath the title. |
| media_library-16 | P2 | proposal | Type distinctions: Feature, Series, and Album are tiny plain words. | Use compact type badges to distinguish mixed-media results. |
| media_library-17 | P3 | proposal | Grid spacing: The second-row poster begins close to the first row's metadata. | Increase the vertical gap between complete cards. |
| media_library-18 | P2 | proposal | Selected detail metadata: 2019 - 118 min - Feature is small and dim. | Increase size and use clear centered-dot separators. |
| media_library-19 | P2 | proposal | Rating stars: Four yellow stars and one gray star have no numeric label. | Add 4/5 or the intended rating value beside the stars. |
| media_library-20 | P2 | proposal | Description: The plot sentence is small and occupies a long narrow line. | Increase body size and allow a readable two-line description. |
| media_library-21 | P2 | proposal | Director credit: dir. Ana Weir is faint and abbreviated. | Use Directed by Ana Weir with legible supporting contrast. |
| media_library-22 | P2 | proposal | Play action: The main Play button has small text and no play glyph. | Increase its label and add a single-triangle play icon. |
| media_library-23 | P2 | proposal | Watchlist action: A tiny plus floats beside Play with no visible label. | Use an outlined Add to watchlist button or labeled icon. |
| media_library-24 | P2 | proposal | Pagination: Page numbers 1–4 are tiny and close to the bottom edge. | Increase page-control size and bottom padding. |
| media_library-25 | P2 | proposal | Server status: Home server and Local demo library are faint and do not expose connection state. | Add a clear local-library badge or status dot with text. |

## menu showcase

[Reviewed image](../../docs/design-audit/originals/menu_showcase_720p.png) · [Current baseline](../../screenshot-baselines/screens/menu_showcase_720p.png) · [Visual review](../../design-audit.html#screen=menu_showcase)

Baseline SHA-256: `970db7dced46987c72bbefa3238d417a2bdd6cc81f71d70c789195b0138dfac5`

The open Bottom menu is intentionally captured to demonstrate upward placement. Eighteen proposals improve anchor, menu, and state explanation without inferring unseen keyboard or corner-menu failures.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| menu_showcase-01 | P2 | proposal | Title: Anchored menus and the behavior explanation share one small line. | Use Anchored menus as a heading with a separate edge-flip subtitle. |
| menu_showcase-02 | P2 | proposal | Top menu group: File and Edit look like two isolated large buttons. | Place them in a modest menu-bar frame. |
| menu_showcase-03 | P2 | proposal | Trigger labels: Bottom and Corner describe position without identifying the demonstrated behavior. | Use Bottom edge and Bottom-right corner. |
| menu_showcase-04 | P2 | proposal | Direction cues: The upper and lower triggers do not show opening directions. | Add small down/up indicators beside their labels. |
| menu_showcase-05 | P2 | proposal | Status message: Click a menu appears near the center far from every trigger. | Move the instruction below the heading. |
| menu_showcase-06 | P2 | proposal | Open-state status: The status still says click a menu while Bottom is already open. | Show Bottom menu open · placed above anchor in the baseline state. |
| menu_showcase-07 | P2 | proposal | Open menu text: New, Open, Save, and Revert are small and widely spaced. | Increase menu-label size and reduce tracking. |
| menu_showcase-08 | P2 | proposal | New row padding: The highlighted New row is tightly bounded by its outline. | Increase internal text inset while preserving the focus treatment. |
| menu_showcase-09 | P2 | proposal | Shortcut contrast: Cmd+N, Cmd+O, and Cmd+S are very dim against teal. | Increase shortcut contrast to a readable secondary level. |
| menu_showcase-10 | P2 | proposal | Shortcut alignment: Shortcuts sit close to the menu's right edge. | Add a consistent trailing inset and a fixed shortcut column. |
| menu_showcase-11 | P2 | proposal | Disabled Revert: Revert is extremely faint inside the gray disabled row. | Keep it inactive but raise label contrast enough to identify the option. |
| menu_showcase-12 | P3 | proposal | Disabled explanation: Revert has no visible reason for its disabled example state. | Add an external demo caption explaining that it is intentionally disabled. |
| menu_showcase-13 | P2 | proposal | Menu and trigger boundary: The open menu and Bottom trigger use the same teal fill and meet directly. | Add a subtle border or separator distinguishing the popup from its anchor. |
| menu_showcase-14 | P3 | proposal | Upward-placement proof: The expected flip is visible without a boundary marker. | Draw a faint viewport-edge guide and an upward placement arrow outside the popup. |
| menu_showcase-15 | P3 | proposal | Corner example: The Corner trigger is closed, so horizontal clamping is not demonstrated in the capture. | Add a labeled expected-placement diagram beside it. |
| menu_showcase-16 | P3 | proposal | Empty canvas: Most of the central viewport is unused. | Use it for a compact placement diagram connecting top, bottom, and corner cases. |
| menu_showcase-17 | P3 | proposal | Trigger alignment: The bottom-left and bottom-right triggers have no shared footer frame. | Add an unobtrusive baseline guide to emphasize their common edge position. |
| menu_showcase-18 | P2 | proposal | Sample identification: The page does not distinguish live triggers from explanatory text. | Add a short Live menu examples label above the trigger area. |

## meters gauges

[Reviewed image](../../docs/design-audit/originals/meters_gauges_720p.png) · [Current baseline](../../screenshot-baselines/screens/meters_gauges_720p.png) · [Visual review](../../design-audit.html#screen=meters_gauges)

Baseline SHA-256: `4238f42dec031f8b73b99813d1e2516871137477ee5fbe535f74cafb1b57b7ee`

The page contains several useful meter forms. The animated 1% capture is a moment in its cycle, not a failure. Improve labels, fill semantics, and linked-control explanation.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| meters_gauges-01 | P2 | defect | Stacked Bars heading: Health and Shield are side by side while the heading calls them Stacked Bars. | Rename the section Paired bars or add an actual stacked companion example. |
| meters_gauges-02 | P2 | proposal | Inline abbreviations: HP, MP, and XP have no expanded names. | Add Health, Mana, and Experience captions. |
| meters_gauges-03 | P2 | proposal | Inline meter colors: All three bars use the same blue fill and green remainder. | Give each metric a labeled identity color or explain the common theme treatment. |
| meters_gauges-04 | P2 | proposal | Track color: Green remainder segments look like completed progress as strongly as blue fills. | Use a neutral remainder track and reserve color for the filled portion. |
| meters_gauges-05 | P2 | proposal | Inline percentages: 65%, 39%, and 84% sit at the left of their fills. | Move values to a fixed trailing value column. |
| meters_gauges-06 | P2 | proposal | Inline row grouping: HP, MP, and XP labels sit close to the preceding bar. | Increase inter-meter group spacing and tighten each label-to-bar gap. |
| meters_gauges-07 | P2 | proposal | Health label: Health: 65% is embedded in the track. | Place label and value outside the bar. |
| meters_gauges-08 | P2 | proposal | Shield label: Shield: 29% is embedded in its smaller filled region. | Use the same external label/value pattern as Health. |
| meters_gauges-09 | P3 | proposal | Health-shield relationship: The heading joins the two metrics but gives no shared total or relation. | Add a short caption explaining independent health and shield values. |
| meters_gauges-10 | P2 | proposal | Animated value: 1% sits in a tiny sliver of blue fill. | Keep the animated percentage in a stable field beside the bar. |
| meters_gauges-11 | P2 | proposal | Animation spinner: An unlabeled ring sits next to Animated Progress. | Label it as a spinner or circular progress companion according to its role. |
| meters_gauges-12 | P3 | proposal | Animation timing: The screenshot does not describe the animation cycle. | Add a small 0–100% looping caption with duration. |
| meters_gauges-13 | P3 | proposal | Animated sample width: The long bar and small ring leave a large blank right region. | Align the pair within a compact sample frame. |
| meters_gauges-14 | P2 | proposal | Gauge labels: The circular gauges show only 25%, 50%, and 90%. | Add meaningful sample names or Low, Half, and High labels. |
| meters_gauges-15 | P2 | proposal | Gauge value placement: Values sit below empty ring centers. | Move the percentages into the centers for a tighter readout. |
| meters_gauges-16 | P2 | proposal | Gauge track contrast: The unfilled ring segments blend into the navy panel. | Increase track contrast while preserving colored arcs. |
| meters_gauges-17 | P3 | proposal | Gauge color semantics: Red, gold, and green suggest thresholds without naming them. | Add a threshold legend outside the gauges. |
| meters_gauges-18 | P3 | proposal | Gauge spacing: Three small rings are spread across almost the entire panel. | Bring them closer into a comparison group. |
| meters_gauges-19 | P2 | proposal | Interactive control label: Value: 65% does not say which meters the control affects. | Name the controlled metric and list linked examples. |
| meters_gauges-20 | P2 | proposal | Slider thumb: The blue thumb sits on a blue-and-green track with weak separation. | Use a contrasting thumb outline and a distinct handle shape. |
| meters_gauges-21 | P2 | proposal | Slider anatomy: The interactive slider resembles the progress bars above it. | Use a thinner track and explicit drag handle to distinguish input from output. |
| meters_gauges-22 | P3 | proposal | Slider endpoints: The control has no minimum or maximum labels. | Add 0% and 100% endpoint captions. |
| meters_gauges-23 | P2 | proposal | Section typography: Section labels are small and widely spaced compared with available panel area. | Increase their size and use consistent left insets. |
| meters_gauges-24 | P3 | proposal | Panel heights: Large dark panels contain small low-height examples. | Reduce decorative vertical padding and allocate more room to labels. |
| meters_gauges-25 | P3 | proposal | Demo footer: No legend explains shared values or theme behavior. | Add a compact setup summary below the interactive control. |

## minesweeper lab

[Reviewed image](../../docs/design-audit/originals/minesweeper_lab_720p.png) · [Current baseline](../../screenshot-baselines/screens/minesweeper_lab_720p.png) · [Visual review](../../design-audit.html#screen=minesweeper_lab)

Baseline SHA-256: `87397a8c2165611f20828963c18f4536a275582430aa244f11b1e03c61354031`

The retro desktop styling is deliberate and should remain. Proposals improve game-state explanation and pixel-art consistency without replacing the classic Windows frame.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| minesweeper_lab-01 | P2 | proposal | Mine counter: The red 038 counter has no label. | Add a small Mines remaining caption in the status area or beside the display. |
| minesweeper_lab-02 | P2 | proposal | Timer: The 000 display has no time unit or label. | Add Time · seconds beneath or above the timer. |
| minesweeper_lab-03 | P2 | proposal | Counter typography: The thick pixel digits make 038 visually dense. | Tune segment spacing so each digit is distinct while preserving the digital style. |
| minesweeper_lab-04 | P2 | proposal | Reset face: The smiley button has no visible reset label. | Add a small New game caption near the face control. |
| minesweeper_lab-05 | P2 | proposal | Difficulty identity: The 16-by-16 board is visible but the difficulty is unnamed. | Show Intermediate · 16 × 16 in the game status area. |
| minesweeper_lab-06 | P2 | proposal | Starting state: Some cells are revealed and two flags are placed while the timer reads zero. | Mark the baseline as a prepared sample state in the demo frame. |
| minesweeper_lab-07 | P2 | proposal | Status instruction: Left click to reveal. Right click to mark a mine uses small text at the bottom. | Increase status text readability within the classic status bar. |
| minesweeper_lab-08 | P2 | proposal | Flagging wording: Mark a mine could imply certainty rather than a player annotation. | Use Place or remove a flag in the instruction. |
| minesweeper_lab-09 | P2 | proposal | Board coordinates: The dense grid has no row or column reference. | Offer subtle external coordinate labels for demo inspection. |
| minesweeper_lab-10 | P3 | proposal | Flag glyphs: Tiny red flags have little separation from the dark mast. | Increase flag silhouette clarity within the existing cell size. |
| minesweeper_lab-11 | P3 | proposal | Revealed numbers: Blue 1 and green 2 are thin compared with the surrounding bevels. | Increase numeral weight while keeping classic number colors. |
| minesweeper_lab-12 | P2 | proposal | Revealed-cell state: Empty revealed cells and numbered cells use almost the same flat gray as recessed boundaries. | Clarify the exposed-cell fill without changing covered-cell bevels. |
| minesweeper_lab-13 | P3 | proposal | Cell bevel weight: Bright repeated bevels dominate the board more than the few revealed numbers. | Slightly reduce bevel contrast while preserving the retro grid structure. |
| minesweeper_lab-14 | P3 | proposal | Board outer frame: Several nested bevels consume space around the cells. | Reduce one redundant outer inset while retaining the classic sunken frame. |
| minesweeper_lab-15 | P2 | proposal | Game menu: Game and Help are small and closely packed. | Use consistent menu-item padding and readable text size. |
| minesweeper_lab-16 | P2 | proposal | Window title: Minesweeper is small within the blue title bar. | Increase title text slightly and align it cleanly with the app icon. |
| minesweeper_lab-17 | P2 | proposal | Maximize control: The middle window control appears as an almost blank square. | Render a clear maximize glyph at the pixel grid. |
| minesweeper_lab-18 | P3 | proposal | Close glyph: The x is tiny compared with its gray button. | Center and enlarge the glyph within the classic control. |
| minesweeper_lab-19 | P3 | proposal | Window-control spacing: The minimize, maximize, and close buttons nearly touch. | Add one consistent pixel-grid gap between them. |
| minesweeper_lab-20 | P3 | proposal | Desktop icon label: My Computer uses a different handwritten-looking face from the window chrome. | Use the same retro bitmap text style as the desktop shell. |
| minesweeper_lab-21 | P3 | proposal | Desktop icon scale: The monitor icon is small compared with the label's wide footprint. | Balance icon scale with the centered label width. |
| minesweeper_lab-22 | P3 | proposal | Taskbar app button: The Minesweeper task button leaves a large unused interior span. | Use a width closer to the app label or show active-window emphasis more clearly. |
| minesweeper_lab-23 | P3 | proposal | Taskbar clock: 12:00 PM sits in a wide recessed region. | Tighten horizontal padding to match the classic clock treatment. |
| minesweeper_lab-24 | P2 | proposal | Game progress: The status area shows input instructions but no revealed-cell progress. | Add a compact safe-cells-remaining readout as optional demo metadata. |
| minesweeper_lab-25 | P3 | proposal | Desktop empty space: The centered window leaves broad blank teal fields. | Use a restrained external demo caption to explain the prepared game state without crowding the board. |

## mini motorways settings

[Reviewed image](../../docs/design-audit/originals/mini_motorways_settings_720p.png) · [Current baseline](../../screenshot-baselines/screens/mini_motorways_settings_720p.png) · [Visual review](../../design-audit.html#screen=mini_motorways_settings)

Baseline SHA-256: `0e4c9f905c109fc046178e22c99369d03be82a60261aacc372f344a0b30ae870`

The sparse road-map settings composition matches the mock. Improve label contrast, state semantics, and the relationship between navigation and controls.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| mini_motorways_settings-01 | P2 | proposal | Inactive navigation: Video, Audio, and other white labels sit on pale cyan blocks. | Darken the text or backing to improve contrast. |
| mini_motorways_settings-02 | P2 | proposal | Selected Game tab: White Game text sits on a light yellow block. | Use dark selected-tab text while retaining the yellow accent. |
| mini_motorways_settings-03 | P2 | proposal | Navigation widths: Each tab block follows its text width and begins at a different x position. | Give the tabs a consistent alignment edge and controlled padding. |
| mini_motorways_settings-04 | P2 | proposal | Language tab: Language extends far left relative to Video and Audio. | Use a shared navigation width so long labels do not alter the column silhouette. |
| mini_motorways_settings-05 | P2 | proposal | Cross-Save label: Cross-Save is a wide technical term among shorter category names. | Use a consistent text size and add a short account-sync subtitle if needed. |
| mini_motorways_settings-06 | P2 | proposal | Back icon: A large bare arrow floats in the upper-left corner. | Add a Back label or frame it as a consistent navigation control. |
| mini_motorways_settings-07 | P2 | proposal | Toggle semantics: Night Mode and Vibration use an X inside a circle for Off. | Add explicit Off text so the X is not mistaken for an error or close action. |
| mini_motorways_settings-08 | P2 | proposal | Enabled toggle: Hold to Draw/Delete uses a checkmark without an On label. | Pair the check with On using the same state layout as disabled rows. |
| mini_motorways_settings-09 | P2 | proposal | Toggle sizing: Large circular toggles compete with setting names. | Reduce circles slightly and align them in a shared trailing column. |
| mini_motorways_settings-10 | P2 | proposal | Hold label: Hold to Draw/Delete combines two actions in a long phrase. | Use Hold to draw or delete with a short explanation below. |
| mini_motorways_settings-11 | P2 | proposal | Night Mode description: Night Mode has no indication of what changes. | Add a brief preview or description of the darker palette. |
| mini_motorways_settings-12 | P2 | proposal | Vibration context: Vibration has no note about controller applicability. | Add a compact Controller feedback subtitle. |
| mini_motorways_settings-13 | P2 | proposal | Sensitivity typography: Controller Cursor Sensitivity is much smaller than adjacent setting labels. | Use a consistent setting-label scale. |
| mini_motorways_settings-14 | P2 | proposal | Sensitivity layout: The value Default and large arrows span a very wide row. | Bring arrows closer to the value in a compact selector. |
| mini_motorways_settings-15 | P2 | proposal | Sensitivity value meaning: Default gives no measurable or relative value. | Show Default · 1.0× or another meaningful scale if supported. |
| mini_motorways_settings-16 | P3 | proposal | Selector arrows: Solid triangles dominate the sensitivity row. | Use lighter chevrons with consistent button bounds. |
| mini_motorways_settings-17 | P2 | proposal | Separator: The horizontal rule under sensitivity ends before the rightmost controls. | Align it to the full settings column width. |
| mini_motorways_settings-18 | P2 | proposal | Colorblind Mode label: A single binary label does not explain the alternative palette. | Add a short palette preview beside the setting. |
| mini_motorways_settings-19 | P2 | proposal | Content heading: The right-side settings have no Game settings heading. | Add a small heading aligned with the first row. |
| mini_motorways_settings-20 | P3 | proposal | Row spacing: The gap around sensitivity is larger than between the toggles above. | Use a consistent vertical rhythm while keeping a deliberate accessibility group break. |
| mini_motorways_settings-21 | P2 | proposal | Tutorial action: Tutorial is large white text on pale cyan near the bottom edge. | Increase contrast and give it a clear secondary-action treatment. |
| mini_motorways_settings-22 | P3 | proposal | Tutorial arrow: The large arrow is close to the trailing edge. | Use equal horizontal padding around label and arrow. |
| mini_motorways_settings-23 | P2 | proposal | Version footer: A long release identifier occupies prominent bottom-left space. | Reduce its emphasis and shorten the default visible version string. |
| mini_motorways_settings-24 | P3 | proposal | Road decoration: The white road bends directly behind the navigation column. | Lower its prominence where it intersects tab backgrounds. |
| mini_motorways_settings-25 | P3 | proposal | Background grid: Faint grid lines pass through every setting row. | Keep grid detail quieter behind the text column. |

## modals

[Current baseline](../../screenshot-baselines/screens/modals_720p.png) · [Visual review](../../design-audit.html#screen=modals)

Baseline SHA-256: `21f847d8bbbcf7a6c3270902928c1d535a7e57e8ee039f9b3497731e45b627d1`

No modal is open in this baseline. Findings target the launcher and result display; dialog appearance, focus trapping, and background blocking cannot be judged from this resting image.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| modals-01 | P2 | proposal | Page title: Modal Dialogs is the only explanation of the page. | Add Choose a dialog example and inspect its result beneath the title. |
| modals-02 | P2 | proposal | Basic category: Basic does not explain the difference between Simple and Composable. | Add a short description under each launcher. |
| modals-03 | P2 | proposal | Simple Modal label: The launcher names the implementation rather than the visible behavior. | Add Plain message dialog as its specimen subtitle. |
| modals-04 | P2 | proposal | Composable Modal label: Composable is technical without describing its content. | Add Form controls with Save and Cancel beneath the label. |
| modals-05 | P2 | proposal | Helpers category: Helpers is an API term without user-visible meaning. | Rename it Standard dialog types. |
| modals-06 | P2 | proposal | Info Dialog and Notice: The two launchers have similar names without distinguishing behavior. | Add brief purpose captions for informational acknowledgment and notice. |
| modals-07 | P2 | proposal | Confirmation color: Confirmation uses dark red although no destructive action is named. | Use a neutral confirmation style or label the destructive scenario. |
| modals-08 | P2 | proposal | Launcher palette: Cyan, purple, and red fills have no legend. | Use consistent launch-button styling and identify variants through labels. |
| modals-09 | P2 | proposal | Button labels: Small widely spaced text sits inside broad rectangular buttons. | Increase readable label size and reduce tracking. |
| modals-10 | P2 | proposal | Button heights: Launch controls are low relative to their wide row panels. | Increase vertical padding within each control. |
| modals-11 | P2 | proposal | Category column: Basic, Helpers, Stacking, and Blocking share space with long action names. | Use a fixed left category column and align every action group. |
| modals-12 | P2 | proposal | Stacking action: Settings + Nested Confirm compresses two steps into one label. | Use Open settings and add Then open reset confirmation as a caption. |
| modals-13 | P3 | proposal | Stacking visualization: The row has no visual indication of two dialog layers. | Add a small two-layer thumbnail beside the launcher. |
| modals-14 | P2 | proposal | Blocking category: Blocking does not say what is being tested. | Rename it Background input blocking test. |
| modals-15 | P2 | proposal | Background Button: The label does not tell the user to compare its count with a modal open. | Add an instruction explaining the intended click-count experiment. |
| modals-16 | P2 | proposal | Clicks counter: Clicks: 0 is left-aligned in an isolated dark rectangle. | Use a compact Background clicks badge adjacent to the button. |
| modals-17 | P2 | proposal | Results strip: Confirm: Pending, FYI: Pending, OK:0, and X:0 run together. | Use separate labeled result fields. |
| modals-18 | P2 | proposal | FYI result name: FYI does not match the visible Notice launcher. | Use the same name in launcher and result display. |
| modals-19 | P2 | proposal | OK and X counters: The counters have no description of what they count. | Label them Acknowledged and Dismissed or the actual events. |
| modals-20 | P2 | proposal | Pending state: Pending suggests an active request although no dialog is open. | Use Not opened for untouched examples if accurate. |
| modals-21 | P2 | proposal | Default capture: The screenshot shows no actual dialog specimen. | Add a labeled static preview alongside the launchers or capture a complementary open state. |
| modals-22 | P3 | proposal | Wide empty rows: Each row extends far beyond its controls. | Use the spare width for short expected-behavior captions. |
| modals-23 | P3 | proposal | Vertical whitespace: Large black gaps separate identical launcher panels. | Tighten section spacing to create one coherent gallery. |
| modals-24 | P3 | proposal | Results association: The status strip is detached from the launchers that populate it. | Add matching labels or small result badges beside each relevant launcher. |
| modals-25 | P3 | proposal | Reset experiment: The page has no visible way to restore result counters. | Add a compact Reset results action in the result strip. |

### Additional capture: modals

[Image](../../baseline_screenshots/modals.png)

Current capture removes Simple Modal focus outline. Both show the launcher rather than an open dialog. Neither capture establishes the appearance or usability of open and stacked modals.

## multiline text lab

[Reviewed image](../../docs/design-audit/originals/multiline_text_lab_720p.png) · [Current baseline](../../screenshot-baselines/screens/multiline_text_lab_720p.png) · [Visual review](../../design-audit.html#screen=multiline_text_lab)

Baseline SHA-256: `a62fba35cf889378fbf68a9fd23b65af5de6e88e289e57420fb6b247866dfaa2`

The blocks intentionally compare hard breaks, wrapping, styled runs, preserved blank lines, and resize-derived height. Preserve the text and geometry under test; improve external labels and comparison aids.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| multiline_text_lab-01 | P2 | proposal | Title: Multi-line Text Lab does not state the success criterion. | Add Text and measured bounds should agree as a subtitle. |
| multiline_text_lab-02 | P2 | proposal | Case captions: Small handwritten captions are difficult to scan. | Use a larger neutral caption face outside the test boxes. |
| multiline_text_lab-03 | P2 | proposal | Hard-break terminology: The first two captions use a literal newline escape without a plain-language label. | Label them Explicit line breaks and retain the escape in metadata. |
| multiline_text_lab-04 | P2 | proposal | Auto-fit sample: Auto-fit size gives no rendered font size. | Add the resulting font size in an external value label. |
| multiline_text_lab-05 | P2 | proposal | Fixed-font sample: font_size 18 mixes API syntax with human-facing metadata. | Show Fixed font size · 18 px. |
| multiline_text_lab-06 | P3 | proposal | Hard-break comparison: The two top samples have no shared group title. | Add Same text, different sizing modes above the pair. |
| multiline_text_lab-07 | P2 | proposal | Fixed-wrap dimensions: 280x110 uses cramped dimension formatting. | Show Width 280 px · Height 110 px in the caption. |
| multiline_text_lab-08 | P2 | proposal | Self-sizing label: Dim::Text appears without explanation. | Use Content-derived height with the API name in smaller text. |
| multiline_text_lab-09 | P3 | proposal | Wrapped-line counts: The fixed and self-sized samples show three lines without a line-count label. | Add external Rendered lines: 3 values. |
| multiline_text_lab-10 | P3 | proposal | Self-sized bounds: The short right box ends close to its final line. | Add an external measured-height guide for easier inspection of the intended fit. |
| multiline_text_lab-11 | P2 | proposal | Styled-run legend: Red removed, green added, and gray context text have no external legend. | Add a small diff-color key outside the sample. |
| multiline_text_lab-12 | P3 | proposal | Styled-run font: The code-like diff sample uses the same handwritten face as prose. | Add a monospaced companion specimen while preserving the existing test. |
| multiline_text_lab-13 | P2 | proposal | Blank-line caption: The preserved blank-line explanation uses tiny escape notation. | Label it Two line breaks preserve one blank line. |
| multiline_text_lab-14 | P3 | proposal | Blank-line measurement: The gap between above and below is not quantified. | Add a faint external line-height bracket beside the blank line. |
| multiline_text_lab-15 | P2 | proposal | Resize instruction: Drag the corner handle appears in small text above the panel. | Increase the instruction and show a small resize-arrow glyph. |
| multiline_text_lab-16 | P2 | proposal | Resize handle: The cyan rounded square looks like a generic button. | Add diagonal grip marks inside the existing handle bounds. |
| multiline_text_lab-17 | P2 | proposal | Dynamic dimensions: 300 × 120 appears in parentheses inside the instruction. | Show Width and Height as separate live value badges. |
| multiline_text_lab-18 | P2 | proposal | Comparison distance: The same-width green sample sits far to the right of the draggable panel. | Bring its explanatory caption closer or connect the two with a labeled comparison line. |
| multiline_text_lab-19 | P3 | proposal | Comparison alignment: The draggable and content-height samples share a top edge without an explicit guide. | Add a faint alignment guide outside their bounds. |
| multiline_text_lab-20 | P3 | proposal | Green sample meaning: Only the right sample uses green, without a color key. | Label green as the content-derived-height reference. |
| multiline_text_lab-21 | P2 | proposal | Diagnostic footer: Mouse, handle, hot, held, and drags values are tiny at the bottom edge. | Move them into a readable diagnostic strip with bottom padding. |
| multiline_text_lab-22 | P2 | proposal | Boolean diagnostics: hot=n and held=n require decoding. | Display Hot: No and Dragging: No. |
| multiline_text_lab-23 | P2 | proposal | Handle geometry diagnostic: 320,612 28x28 combines position and size without column labels. | Separate X, Y, Width, and Height fields. |
| multiline_text_lab-24 | P3 | proposal | Case identification: The eight specimens have no stable case numbers. | Number the examples to support visual-regression discussions. |
| multiline_text_lab-25 | P3 | proposal | Right upper empty space: The upper-right region is unused. | Place a compact expected line-count and sizing-mode summary there. |

## navigation bar demo

[Reviewed image](../../docs/design-audit/originals/navigation_bar_demo_720p.png) · [Current baseline](../../screenshot-baselines/screens/navigation_bar_demo_720p.png) · [Visual review](../../design-audit.html#screen=navigation_bar_demo)

Baseline SHA-256: `42d980e0283b6ee8e8fbe4d519f0bab26f163015c0ad04c7aaf2614b570d6de1`

Three legible cycle selectors are under-explained. Sixteen distinct proposals are defensible for this small resting-state fixture; no wraparound or keyboard failure is inferred.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| navigation_bar_demo-01 | P2 | proposal | Title: Navigation Bar Component Showcase is a long technical heading. | Use Cycle selectors with the component name in a subtitle. |
| navigation_bar_demo-02 | P2 | proposal | Settings category count: Graphics appears without the total number of categories. | Add a current-position value such as 1 of N. |
| navigation_bar_demo-03 | P2 | proposal | Game mode count: Arcade gives no clue to the other available modes. | Add an option count or short adjacent option preview. |
| navigation_bar_demo-04 | P2 | proposal | Level position: The caption says 6 options while the bar says Level 1. | Show Level 1 of 6 inside the selector. |
| navigation_bar_demo-05 | P2 | proposal | Arrow grouping: Chevrons sit far from their centered values. | Use bounded arrow buttons around a more compact value field. |
| navigation_bar_demo-06 | P2 | proposal | Arrow affordance: Chevrons share the same blue surface as the value. | Add subtle separators around the arrow buttons. |
| navigation_bar_demo-07 | P2 | proposal | Section labels: Settings Categories and Game Mode Selector are small and muted. | Increase label contrast and use consistent naming. |
| navigation_bar_demo-08 | P3 | proposal | Graphics context: No selected-category result appears below Graphics. | Add a Selected category: Graphics result line. |
| navigation_bar_demo-09 | P3 | proposal | Arcade meaning: The mode name has no description. | Add a short description below the selector. |
| navigation_bar_demo-10 | P3 | proposal | Level identity: Level 1 is generic despite the wide bar. | Add a sample level name or thumbnail. |
| navigation_bar_demo-11 | P2 | proposal | Instruction: No visible text explains the selectors. | Add Use the arrows to cycle through each example. |
| navigation_bar_demo-12 | P3 | proposal | Boundary behavior: The screenshot does not state whether options wrap. | Add Wraps or Stops at ends matching the configuration. |
| navigation_bar_demo-13 | P2 | proposal | Control widths: Wide bars contain only a short value and chevrons. | Reduce their widths for easier visual association. |
| navigation_bar_demo-14 | P3 | proposal | Top empty area: A large blank navy area precedes the title. | Move the group upward or center the whole group vertically. |
| navigation_bar_demo-15 | P3 | proposal | Right empty area: The controls are left-shifted inside the panel. | Center them with equal side margins. |
| navigation_bar_demo-16 | P3 | proposal | Value typography: Graphics, Arcade, and Level 1 use handwritten text. | Use a clearer value face while retaining informal headings. |

### Additional capture: navigation_bar_demo

[Image](../../baseline_screenshots/navigation_bar_demo.png)

Active e2e baseline refreshed after visual review. The previous capture remains in originals until audit cleanup.

## neon strike

[Reviewed image](../../docs/design-audit/originals/neon_strike_720p.png) · [Current baseline](../../screenshot-baselines/screens/neon_strike_720p.png) · [Visual review](../../design-audit.html#screen=neon_strike)

Baseline SHA-256: `80a906841abee13c2e6b8e27bbbf39c5a7d355ef9a9f3fe406760ad8fd2ff89c`

The tactical HUD intentionally leaves the center clear. Improve peripheral legibility and the grouping of map, health, ammunition, objective, and equipment data.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| neon_strike-01 | P2 | proposal | Grenade count: The grenade count sits outside its tile while the knife count sits inside; the web mock repeats this asymmetry. | Place both counts at the same lower-right inset inside their tiles. |
| neon_strike-02 | P2 | proposal | Equipment key hints: G and V are tiny in the tile corners. | Increase their size and use compact keycaps. |
| neon_strike-03 | P2 | proposal | Equipment icons: Grenade and knife line weights differ from the heavier streak icons. | Normalize optical icon weight within the HUD. |
| neon_strike-04 | P2 | proposal | Selected equipment: A thin gold outline is the grenade's only selection cue. | Add a small selected marker or brighter bottom rule. |
| neon_strike-05 | P2 | proposal | Ammo readout: 35/210 gives magazine and reserve counts similar emphasis. | Emphasize 35 and reduce reserve-count weight. |
| neon_strike-06 | P2 | proposal | Weapon identity: The ammo readout has no weapon name. | Add a small current-weapon label above it. |
| neon_strike-07 | P2 | proposal | Health label: 80 HEALTH is tiny beside a large skull tile. | Increase health text and reduce skull emphasis. |
| neon_strike-08 | P2 | proposal | Armor label: 45% ARMOR is extremely small below two thin bars. | Increase text and place it beside its own bar. |
| neon_strike-09 | P2 | proposal | Health and armor bars: Both bars use similar cyan fills without direct labels. | Give each bar a directly adjacent label or icon. |
| neon_strike-10 | P2 | proposal | Health scale: 80 is shown without a maximum. | Show 80/100 or the actual maximum. |
| neon_strike-11 | P2 | proposal | Map caption: SECTOR 7B and GRID 042 are tiny along the bottom edge. | Move them into a readable map footer. |
| neon_strike-12 | P2 | proposal | Map markers: Cyan rings appear without a legend. | Add a compact ally-marker key near the map. |
| neon_strike-13 | P2 | proposal | Objective marker: B is small inside a thin amber region. | Increase its contrast and label its objective role. |
| neon_strike-14 | P3 | proposal | Player direction: The white triangle overlaps a tan view cone. | Strengthen the triangle outline and soften the cone. |
| neon_strike-15 | P2 | proposal | Killstreak header: Three squares follow KILLSTREAK without explanation. | Label them as charge/progress or use a clearer segmented meter. |
| neon_strike-16 | P2 | proposal | Streak hotkeys: 1 through 4 are tiny outside the ability tiles. | Use readable, consistently aligned key badges. |
| neon_strike-17 | P3 | proposal | Gear decorations: A gear repeats beside each ability without explanation. | Remove repeated gears or label their configuration role. |
| neon_strike-18 | P2 | proposal | Ability labels: UAV, RECON, SHIELD, and STRIKE are tiny. | Increase their size and reduce tracking. |
| neon_strike-19 | P2 | proposal | Strike icon: A large X represents STRIKE. | Use a recognizable strike symbol distinct from close or cancel. |
| neon_strike-20 | P2 | proposal | Objective banner: SECURE OBJECTIVE B sits tight against the right edge. | Add trailing padding inside a stable safe area. |
| neon_strike-21 | P2 | proposal | Score shield: An unlabeled shield follows SCORE: 15,000. | Separate score from protection status or explain the symbol. |
| neon_strike-22 | P2 | proposal | Kill feed: Three dense text lines sit beneath the compass. | Distinguish actor, action, and target and increase line spacing. |
| neon_strike-23 | P3 | proposal | Repeated alert: Alpha_Six Under fire appears in both feed and voice area. | Differentiate radio activity from event history with labels. |
| neon_strike-24 | P2 | proposal | Compass labels: Bearing values and direction letters are very small. | Increase compass text while retaining thin guide lines. |
| neon_strike-25 | P3 | proposal | Background flecks: Gold flecks remain visible behind HUD text. | Reduce their density behind peripheral readouts. |

## nine slice borders

[Reviewed image](../../docs/design-audit/originals/nine_slice_borders_720p.png) · [Current baseline](../../screenshot-baselines/screens/nine_slice_borders_720p.png) · [Visual review](../../design-audit.html#screen=nine_slice_borders)

Baseline SHA-256: `1970c9e49fafdb73a6fb529e47c4d7a75c4ee34bdc9e79fadb1c00a40459106a`

Border shapes, colors, and transparency variants are intentional. Improve parameter labels, transparency framing, and specimen spacing.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| nine_slice_borders-01 | P2 | proposal | Title frame: The full-width white frame outweighs small border specimens. | Use a simpler heading treatment. |
| nine_slice_borders-02 | P2 | proposal | Panel names: Simple, Ornate, and Heavy omit source identifiers. | Add texture IDs beneath the samples. |
| nine_slice_borders-03 | P3 | proposal | Slice parameters: The first row does not state corner-slice dimensions. | Add a shared slice-size caption. |
| nine_slice_borders-04 | P2 | proposal | Corner details: Ornate and Double Line corner details are small. | Add a magnified corner inset beside the row. |
| nine_slice_borders-05 | P3 | proposal | Thick sample: Thick gives no border measurement. | Show its border or slice width externally. |
| nine_slice_borders-06 | P2 | proposal | Decorative and Heavy: Broad gold patterns reduce the usable white center. | Add optional content-inset guides for comparison. |
| nine_slice_borders-07 | P2 | proposal | Border-only grouping: Border A–C have dark centers while Trans A–B have white backing. | Separate the transparency modes with subgroup captions. |
| nine_slice_borders-08 | P2 | proposal | Border A B C names: Letter names do not explain corner differences. | Add texture names or short corner descriptions. |
| nine_slice_borders-09 | P2 | proposal | Trans labels: Trans abbreviates the tested behavior. | Spell out Transparent border in external captions. |
| nine_slice_borders-10 | P3 | proposal | Transparency proof: Flat backgrounds make transparent regions hard to identify. | Add checkerboard companion backgrounds. |
| nine_slice_borders-11 | P2 | proposal | Size metadata: Small, Medium, and Large have no dimensions. | Add width × height captions. |
| nine_slice_borders-12 | P3 | proposal | Size isolation: Different styles appear within the stretching row. | Add a same-source comparison across every size. |
| nine_slice_borders-13 | P2 | proposal | Wide panel copy: Good for menus is packed inside the specimen label. | Move usage advice below the frame. |
| nine_slice_borders-14 | P2 | proposal | Tall alignment: Tall begins above the other size samples. | Use a shared caption baseline or consistent sample framing. |
| nine_slice_borders-15 | P3 | proposal | Tall caption: Tall uses much smaller type than neighboring names. | Use uniform external caption sizes. |
| nine_slice_borders-16 | P2 | proposal | Double-width grouping: Border width and tint share one heading. | Add separate Width and Tint subgroup labels. |
| nine_slice_borders-17 | P2 | proposal | Double-width spacing: Thick decorations nearly meet adjacent samples. | Increase gaps around these specimens. |
| nine_slice_borders-18 | P3 | proposal | Double metadata: Double omits its actual slice size. | Caption the 32-pixel slice parameter. |
| nine_slice_borders-19 | P3 | proposal | Tint values: Gold, Blue, Red, and Green are names without values. | Add exact tint values below them. |
| nine_slice_borders-20 | P2 | proposal | Tint text adaptation: Text colors vary independently of border tint. | Explain that text contrast adapts separately. |
| nine_slice_borders-21 | P2 | proposal | Interactive button: Click Me! does not say what response to inspect. | Add an external expected-response caption. |
| nine_slice_borders-22 | P2 | proposal | Dialog specimen: The small corners-stay-crisp sentence is buried in a large pale box. | Add a clear dialog title and external border caption. |
| nine_slice_borders-23 | P2 | proposal | Nested labels: Inner and Panel do not distinguish the two children. | Use Gold child and Blue child. |
| nine_slice_borders-24 | P3 | proposal | Nested parent border: The pale outer border blends with its fill. | Add a companion contrast backdrop without changing the texture. |
| nine_slice_borders-25 | P3 | proposal | Bottom margin: The last examples end close to the viewport edge. | Add a consistent gallery bottom margin. |

## offsite backup

[Reviewed image](../../docs/design-audit/originals/offsite_backup_720p.png) · [Current baseline](../../screenshot-baselines/screens/offsite_backup_720p.png) · [Visual review](../../design-audit.html#screen=offsite_backup)

Baseline SHA-256: `64a26a3f494b615b0ef65c4143eb4a63f85aad21adc40294e5bb0c16714382e9`

The backup overview has a coherent two-column structure. Improve status readability and reconcile total progress with folder progress before treating it as a trustworthy product screen.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| offsite_backup-01 | P2 | defect | Summary versus folders: 493 GB uploaded is reported, but visible folder sizes and percentages imply about 242 GB uploaded. | Derive totals and ring progress from the same folder data or label differing scopes explicitly. |
| offsite_backup-02 | P2 | proposal | Backup ring: 63% has a tiny COMPLETE caption. | Increase the caption and center the value group. |
| offsite_backup-03 | P2 | proposal | Pause backup: Pause backup is tiny and pale. | Use a readable secondary button with a pause icon. |
| offsite_backup-04 | P2 | proposal | Transfer speed: 24.8 MB/s is faint in the transfer row. | Increase contrast and emphasize the value. |
| offsite_backup-05 | P2 | proposal | Remaining time: About 3 hours remaining is low-contrast metadata. | Give the estimate a stronger value treatment. |
| offsite_backup-06 | P2 | proposal | Healthy badge: Healthy has tiny text and a square marker. | Use a recognizable status dot and readable text. |
| offsite_backup-07 | P2 | proposal | Plan subtitle: Personal Backup - 2 TB plan is very small. | Increase readability and use a centered-dot separator. |
| offsite_backup-08 | P2 | proposal | Folder table headings: FOLDERS is the only heading above progress, state, and size columns. | Add Progress and Size headings. |
| offsite_backup-09 | P2 | proposal | Completed state: done is plain lowercase text beside green bars. | Use Done with a check icon consistently. |
| offsite_backup-10 | P2 | proposal | Uploading state: Photos, Music, and Archive have no activity distinction. | Mark the active upload and identify queued folders if applicable. |
| offsite_backup-11 | P2 | proposal | Folder sizes: Size values are tiny and pale. | Increase their size and retain right alignment. |
| offsite_backup-12 | P2 | proposal | Rescan: Rescan is tiny red text. | Use a readable refresh action with an icon. |
| offsite_backup-13 | P2 | proposal | Schedule selectors: Every 4 hours and 1 year lack chevrons or strong boundaries. | Add visible select affordances. |
| offsite_backup-14 | P2 | proposal | Schedule labels: Run backup and Keep file versions are faint. | Increase contrast and align both rows. |
| offsite_backup-15 | P2 | proposal | Bandwidth meaning: 62% does not identify the full-speed reference. | Pair it directly with the actual 24.8 MB/s cap. |
| offsite_backup-16 | P2 | proposal | Bandwidth thumb: The small coral thumb blends into the fill. | Add a contrasting border and larger grip. |
| offsite_backup-17 | P2 | proposal | Slider endpoints: Slower and Faster are extremely small. | Use readable endpoint labels or measured speeds. |
| offsite_backup-18 | P2 | proposal | Battery checkbox: The checked state is a solid red square without a check. | Render a clear checkmark inside it. |
| offsite_backup-19 | P2 | proposal | External-drive checkbox: The white unchecked square blends into the panel. | Add a visible outline. |
| offsite_backup-20 | P2 | proposal | Checkbox labels: Both settings use tiny text. | Increase label size and row spacing. |
| offsite_backup-21 | P2 | proposal | Storage values: 787 GB of 2 TB has weak emphasis. | Strengthen the values and add percent used. |
| offsite_backup-22 | P2 | proposal | Manage account: The link is tiny coral text. | Increase readability and add a navigation cue. |
| offsite_backup-23 | P2 | proposal | Last completed backup: The success line is faint beneath the folders. | Increase contrast and add a checkmark. |
| offsite_backup-24 | P2 | proposal | Security footer: A green square precedes the encryption statement. | Use a recognizable lock and readable text. |
| offsite_backup-25 | P2 | proposal | Help button: The question mark nearly disappears in its white square. | Increase icon contrast and button size. |

## pagination

[Reviewed image](../../docs/design-audit/originals/pagination_720p.png) · [Current baseline](../../screenshot-baselines/screens/pagination_720p.png) · [Visual review](../../design-audit.html#screen=pagination)

Baseline SHA-256: `4df52ecb438e854f893c7fbac734877bdaee6303e295c37dd2011deabb015c97`

The fixture reuses buttons for pages, difficulty, and named options. Twenty distinct proposals clarify the roles and preserve boundary-state examples.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| pagination-01 | P2 | proposal | Title: Pagination Component Showcase is long and technical. | Use Pagination and option selectors with a short subtitle. |
| pagination-02 | P2 | proposal | Page labels: Page repeats in five wide buttons. | Use compact numeric buttons under a shared Pages label. |
| pagination-03 | P2 | proposal | Current page: Page 1 relies on red fill. | Add a current-page marker or stronger label weight. |
| pagination-04 | P2 | proposal | Disabled arrow: The first previous arrow is very faint. | Keep it disabled but identifiable and caption its boundary state. |
| pagination-05 | P2 | proposal | Chevron style: Thin handwritten-looking arrows differ from geometric control shapes. | Use consistent centered geometric chevrons. |
| pagination-06 | P2 | proposal | Page result: Selected: Page 1 repeats the button without total context. | Show Page 1 of 5. |
| pagination-07 | P2 | proposal | Page content: No sample content connects to the paginator. | Add a small preview reflecting the selected page. |
| pagination-08 | P2 | proposal | Difficulty role: The difficulty group appears under Pagination without explaining the reuse. | Label it Single-choice selector using the same control. |
| pagination-09 | P2 | proposal | Difficulty meaning: Easy through Expert have no descriptions. | Add short descriptions or an intensity scale. |
| pagination-10 | P2 | proposal | Selected difficulty: Medium uses only red fill plus a distant result. | Add an in-control marker matching current-page selection. |
| pagination-11 | P2 | proposal | Named color options: All color names use the same cyan button fill. | State that they are named options or add small swatches. |
| pagination-12 | P2 | proposal | Blue selection: The Blue option has a red selected background. | Use a neutral selection outline to avoid conflicting color meanings. |
| pagination-13 | P2 | proposal | Header affordance: Broad cyan headings resemble cyan option buttons. | Use a quieter noninteractive heading treatment. |
| pagination-14 | P2 | proposal | Header widths: Headings span the panel while controls occupy about half. | Fit frames to controls or use spare width for explanations. |
| pagination-15 | P2 | proposal | Status spacing: Result captions and section gaps lack a consistent rhythm. | Use shared control-to-result and section spacing. |
| pagination-16 | P3 | proposal | Boundary behavior: Arrow wrap behavior is unstated. | Add Stops at first and last option if that matches configuration. |
| pagination-17 | P3 | proposal | Counts: Only difficulty and named options state option counts. | Use consistent counts across all three sections. |
| pagination-18 | P2 | proposal | Text tracking: Widely spaced labels consume unnecessary width. | Reduce tracking and use a clearer option face. |
| pagination-19 | P3 | proposal | Outer padding: The title area and bottom blank region make the panel sparse. | Tighten decorative outer spacing. |
| pagination-20 | P3 | proposal | Independent state: Three selected states are shown without explaining independence. | Add Each row keeps its own selection below the subtitle. |

### Additional capture: pagination

[Image](../../baseline_screenshots/pagination.png)

Active e2e baseline refreshed after visual review. The previous capture remains in originals until audit cleanup.

## parcel corps settings

[Reviewed image](../../docs/design-audit/originals/parcel_corps_settings_720p.png) · [Current baseline](../../screenshot-baselines/screens/parcel_corps_settings_720p.png) · [Visual review](../../design-audit.html#screen=parcel_corps_settings)

Baseline SHA-256: `9f13c3a494f26fed9e4769236bd9878efab854a8e06a193827dfbe680e00fae0`

The phone settings overlay follows the mock composition. Improve phone-control clarity, reduce HUD competition, and preserve the illustrated courier setting.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| parcel_corps_settings-01 | P2 | proposal | Phone title: Settings is separated from the first row by a large blank gap. | Tighten the gap to give the lower controls more room. |
| parcel_corps_settings-02 | P2 | proposal | Text Language value: King's English is small and close to the right border. | Increase value readability and trailing padding. |
| parcel_corps_settings-03 | P2 | proposal | Category symbols: T, D, X, S, and asterisks form an unexplained icon column. | Use recognizable language, display, motion, sync, and audio symbols. |
| parcel_corps_settings-04 | P2 | proposal | Category color meaning: Green, orange, red, and blue badges have no key. | Group rows by category or add a compact category legend. |
| parcel_corps_settings-05 | P2 | proposal | Resolution value: 2560 x 1440 is small in a dense row. | Use a clear multiplication sign and a stable value column. |
| parcel_corps_settings-06 | P2 | proposal | Full Screen label: Full Screen differs from the common single-word setting name. | Use Fullscreen consistently. |
| parcel_corps_settings-07 | P2 | proposal | MSAA meaning: MSAA is shown without an explanation. | Add an Anti-aliasing subtitle or expand the label. |
| parcel_corps_settings-08 | P2 | proposal | MSAA arrows: Small angle-bracket characters fill large dark buttons. | Use centered geometric chevrons and equal button insets. |
| parcel_corps_settings-09 | P2 | proposal | Quality value: High is squeezed between the two arrow buttons. | Give the value a fixed-width centered region. |
| parcel_corps_settings-10 | P2 | proposal | Toggle states: Several vivid green switches have no explicit On text. | Add a consistent state label where space allows. |
| parcel_corps_settings-11 | P2 | proposal | Motion Blur icon: The red X badge resembles an error despite the toggle being on. | Use a motion-blur symbol rather than an error-like X. |
| parcel_corps_settings-12 | P2 | proposal | VSync naming: VSync lacks the spacing and casing used elsewhere in settings. | Use V-sync or Vertical sync consistently. |
| parcel_corps_settings-13 | P2 | proposal | Settings open text: Settings open interrupts the list between VSync and audio controls. | Move this status into a footer or replace it with an Audio section heading. |
| parcel_corps_settings-14 | P2 | proposal | Audio sliders: Thin tracks and small white thumbs sit in short rows. | Increase track and thumb readability while keeping compact row heights. |
| parcel_corps_settings-15 | P2 | proposal | Audio values: Audio Volume and Music Volume have no numeric percentages. | Show the current percentage beside each slider. |
| parcel_corps_settings-16 | P2 | proposal | Bottom controls: Music Volume sits close to the phone's lower inner edge. | Add bottom padding or a visible scroll boundary. |
| parcel_corps_settings-17 | P2 | proposal | Phone close path: The phone shows Settings without a visible back or close control. | Add a back chevron near the title if the settings flow supports it. |
| parcel_corps_settings-18 | P3 | proposal | Phone status bar: A blank white pill and green battery-like shape replace the mock's recognizable signal and battery indicators. | Use recognizable battery and signal symbols with consistent spacing. |
| parcel_corps_settings-19 | P2 | proposal | Objective copy: The business sign-up objective repeats twice in the blue panel. | Combine it into one objective with a distinct completion state. |
| parcel_corps_settings-20 | P2 | proposal | Objective marker: The small Lv0 badge overlaps the objective's last line. | Place the level badge in a dedicated corner outside the text block. |
| parcel_corps_settings-21 | P2 | proposal | Live-chat text: Tiny bright usernames and messages compete with settings. | Reduce chat prominence while the phone is open and keep message text legible. |
| parcel_corps_settings-22 | P3 | proposal | Live header: 08, 10, and EDDCOATES form an unexplained compact cluster. | Add clear viewer/chat icons or labels around the counts. |
| parcel_corps_settings-23 | P2 | proposal | Speed display: 009 and M/h appear in a small panel at the lower-right edge. | Use a conventional speed unit and a consistent safe-area inset. |
| parcel_corps_settings-24 | P3 | proposal | Background rider: The rider's simplified face and hands are hard to distinguish against the clothing. | Clarify the silhouette and face details without increasing background contrast. |
| parcel_corps_settings-25 | P3 | proposal | Overlay competition: The phone, objective panel, live chat, and speed panel all remain prominent. | Dim secondary HUD elements while settings has visual priority. |

## pinch zoom lab

[Reviewed image](../../docs/design-audit/originals/pinch_zoom_lab_720p.png) · [Current baseline](../../screenshot-baselines/screens/pinch_zoom_lab_720p.png) · [Visual review](../../design-audit.html#screen=pinch_zoom_lab)

Baseline SHA-256: `74d40e1d470b9e8fc4ad2bc5eb7b72acca341f33b0f7f91218ab85468199e1f2`

The resting square demonstrates 1.00× scale with zero events. Fifteen proposals improve gesture instructions and diagnostic clarity; no unsupported-gesture or input failure is inferred.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| pinch_zoom_lab-01 | P2 | proposal | Title: Pinch Zoom - trackpad magnification mixes title and explanation. | Use Pinch zoom with a separate gesture instruction. |
| pinch_zoom_lab-02 | P2 | proposal | Gesture instruction: No visible text says how to perform the gesture. | Add Pinch two fingers on the trackpad to resize the square. |
| pinch_zoom_lab-03 | P2 | proposal | Diagnostic row: Zoom, events, and pinching state are tiny. | Increase their text size and separate them into labeled fields. |
| pinch_zoom_lab-04 | P2 | proposal | Scale formatting: 1.00x uses a lowercase x. | Use 1.00× consistently for magnification. |
| pinch_zoom_lab-05 | P2 | proposal | Square label: 1.00x sits against the square's left edge. | Center the value inside the square or add a consistent inset. |
| pinch_zoom_lab-06 | P3 | proposal | Reference size: The square has no fixed-size comparison reference. | Add a faint 1.00× outline behind the scaling sample. |
| pinch_zoom_lab-07 | P3 | proposal | Dimension metadata: Only scale is shown, not the resulting square dimensions. | Add Width and Height readouts beside zoom. |
| pinch_zoom_lab-08 | P2 | proposal | Event counter meaning: events 0 does not explain what increments it. | Label it Pinch updates received. |
| pinch_zoom_lab-09 | P2 | proposal | Gesture state: pinching no is terse and visually indistinct from the counter. | Use a clear Idle or Pinching state badge. |
| pinch_zoom_lab-10 | P2 | proposal | Build requirement footer: A long compiler-flags sentence is tiny at the bottom. | Move requirements into a readable Demo prerequisites panel. |
| pinch_zoom_lab-11 | P2 | proposal | Unsupported ambiguity: Reads 0 otherwise means no gesture and unavailable support can look the same. | Show a separate Gesture support available/unavailable diagnostic based on capability. |
| pinch_zoom_lab-12 | P3 | proposal | Technical footer hierarchy: AppKit and a long feature flag dominate the only supporting explanation. | Keep the user gesture instruction primary and build details secondary. |
| pinch_zoom_lab-13 | P3 | proposal | Reset reference: No visible action restores the square to the reference scale. | Add Reset to 1.00× near the scale readout. |
| pinch_zoom_lab-14 | P3 | proposal | Scale limits: The screenshot gives no minimum or maximum zoom range. | Add the configured range to the sample metadata. |
| pinch_zoom_lab-15 | P3 | proposal | Empty space: A small square occupies the center of a nearly empty canvas. | Use an unobtrusive grid or ruler to make changes in scale easier to judge. |

## popover lab

[Reviewed image](../../docs/design-audit/originals/popover_lab_720p.png) · [Current baseline](../../screenshot-baselines/screens/popover_lab_720p.png) · [Visual review](../../design-audit.html#screen=popover_lab)

Baseline SHA-256: `a477f7db51342b7777f22a5893b51934b900793e8d7ea0b76772cca07238cff3`

The Branch panel is intentionally pinned open and placement examples are fixtures. Improve sample labeling and control hierarchy without treating the resting Commit options trigger or focus behavior as broken.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| popover_lab-01 | P2 | proposal | Page title: The heading is a long sentence combining component name and purpose. | Use Popovers with a separate Anchored panels containing controls subtitle. |
| popover_lab-02 | P2 | proposal | Instruction: Click a trigger to open its panel appears even though Branch is already open. | Explain that Branch is pinned open for comparison. |
| popover_lab-03 | P2 | proposal | Commit options trigger: The closed trigger gives no preview of its form contents. | Add a short Amend and sign-off form caption beneath it. |
| popover_lab-04 | P2 | proposal | Branch trigger: Branch does not identify the current branch. | Show Branch: main or a separate current-branch badge. |
| popover_lab-05 | P2 | proposal | Branch heading: Switch to is tiny above the four branch buttons. | Increase its size and clarify Switch branch. |
| popover_lab-06 | P2 | proposal | Branch names: main and release/2.4 use small handwritten text. | Use a readable monospaced face for branch names. |
| popover_lab-07 | P2 | proposal | Current branch: All four branch rows have identical cyan fills. | Mark the current branch with a check or Current label. |
| popover_lab-08 | P2 | proposal | Branch row padding: Branch names sit close to the left edges of their pills. | Add consistent horizontal insets. |
| popover_lab-09 | P2 | proposal | Popover anchor relationship: The panel begins directly below the trigger without a pointer or gap. | Add a small anchor cue or consistent offset that makes ownership clear. |
| popover_lab-10 | P2 | proposal | Explanation heading: What a popover is for is small within the large dark panel. | Give it a stronger heading size. |
| popover_lab-11 | P2 | proposal | Explanation body: The body is tiny, widely spaced, and broken into isolated lines. | Use readable body text with paragraph spacing. |
| popover_lab-12 | P2 | proposal | Focus explanation: Focus landing inside must not close it describes an internal rule. | Rephrase as You can use controls inside without dismissing the panel. |
| popover_lab-13 | P3 | proposal | Menu comparison: The note mentions menus without showing the difference. | Add a small static menu-versus-form illustration. |
| popover_lab-14 | P2 | proposal | Placement heading: Placement, and what happens when there is no room is a long low-emphasis line. | Use Placement and edge flipping as a clear section heading. |
| popover_lab-15 | P2 | proposal | Above sample caption: anchored Above is tiny and left-flush inside its panel. | Use a centered readable caption with consistent inset. |
| popover_lab-16 | P2 | proposal | Right sample caption: anchored Right touches the panel's left edge. | Add internal padding without changing its anchor placement. |
| popover_lab-17 | P2 | proposal | Left sample caption: anchored Left sits close to the outlined boundary. | Inset the caption while retaining the diagnostic focus outline. |
| popover_lab-18 | P3 | proposal | Placement direction: Above, Right, and Left labels require mentally tracking their panels. | Add small directional arrows outside each trigger-panel pair. |
| popover_lab-19 | P3 | proposal | Placement grouping: The three examples occupy irregular widths and alignments. | Frame each pair within a labeled comparison region without changing its anchor geometry. |
| popover_lab-20 | P2 | proposal | Bottom trigger: near the bottom is lowercase and differs from the other trigger labels. | Rename it Bottom-edge flip using the shared label style. |
| popover_lab-21 | P2 | proposal | Bottom expected result: The bottom panel is closed, so its flipping behavior is not visible. | Add a small expected-placement diagram or companion open-state capture. |
| popover_lab-22 | P3 | proposal | Trigger versus panel colors: Cyan triggers and dark panels lack a shared legend. | Add a compact Trigger / Popover color key. |
| popover_lab-23 | P3 | proposal | Pinned-state identity: The Branch panel resembles a user-opened panel without stating its fixture role. | Add Pinned example as a small external badge. |
| popover_lab-24 | P3 | proposal | Empty central space: Broad blank regions separate the upper and lower demonstrations. | Use the space for anchor and fallback-placement guides. |
| popover_lab-25 | P2 | proposal | Result feedback: The only status text is the initial instruction. | Reserve a labeled Last action line for branch or commit outcomes. |

## potion crafting

[Reviewed image](../../docs/design-audit/originals/potion_crafting_720p.png) · [Current baseline](../../screenshot-baselines/screens/potion_crafting_720p.png) · [Visual review](../../design-audit.html#screen=potion_crafting)

Baseline SHA-256: `4998d4b25be8c71676d896bbceacaebe36f66ebf36da4b2f907cf6d3cfb42da2`

The parchment workshop composition is coherent. Improve text contrast, ingredient accounting, and the distinction between an active brew and a new brewing action.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| potion_crafting-01 | P2 | proposal | Parchment texture: Fine horizontal lines run directly behind every label. | Reduce texture contrast in text-heavy regions. |
| potion_crafting-02 | P2 | proposal | Recipe descriptions: Restores 30 MP and other effects are small and pale. | Increase effect-text size and contrast. |
| potion_crafting-03 | P2 | proposal | Recipe times: 30s to brew and 45s to brew are tiny. | Use readable duration labels with consistent spacing before seconds. |
| potion_crafting-04 | P2 | proposal | Selected recipe: Mana Elixir uses a pale rectangular border with little contrast. | Add a clear selected marker or stronger leading rule. |
| potion_crafting-05 | P2 | proposal | Recipe icons: Small potion icons occupy different visual weights by fill color. | Normalize bottle scale and outline weight across recipes. |
| potion_crafting-06 | P2 | proposal | Center subtitle: The remedy for an exhausted mind is faint under Mana Elixir. | Increase contrast and keep it subordinate to the recipe title. |
| potion_crafting-07 | P2 | proposal | Brew state: 65% brewed appears above a button still labeled Brew potion. | Label the active state Brewing and make the action reflect the current process. |
| potion_crafting-08 | P2 | proposal | Progress value: 65% brewed is tiny below a thin low-contrast bar. | Increase value size and give the fill stronger separation from the track. |
| potion_crafting-09 | P2 | proposal | Progress placement: A thick wooden shelf is more prominent than the progress bar beneath it. | Reduce shelf emphasis or increase progress-bar prominence. |
| potion_crafting-10 | P2 | proposal | Required ingredient counts: Sea Salt x 3 and Star Dust x 1 are small below the illustration. | Use readable names and multiplication signs. |
| potion_crafting-11 | P2 | proposal | Availability states: Available is pale green without the mock's clear checkmark. | Add a check icon and stronger text contrast. |
| potion_crafting-12 | P2 | proposal | Inventory relation: Required counts and inventory counts are far apart without a shared notation. | Show Required 3 / Owned 12 and Required 1 / Owned 1 beside the recipe. |
| potion_crafting-13 | P2 | proposal | Last Star Dust: The recipe requires the entire visible Star Dust stock of one. | Add a small After brew: 0 note to make the inventory consequence visible. |
| potion_crafting-14 | P2 | proposal | Brew action contrast: The olive button blends with the brown parchment palette. | Increase text and border contrast while keeping the muted aesthetic. |
| potion_crafting-15 | P2 | proposal | Brew duration footer: 45 seconds / Produces 1 potion is tiny near the bottom edge. | Move it closer to the action and use a readable size. |
| potion_crafting-16 | P2 | proposal | Ingredient tile names: Moonpetal and Fire Root use small centered serif labels. | Increase label size and maintain equal bottom padding. |
| potion_crafting-17 | P2 | proposal | Inventory quantity placement: x 5, x 3, and other quantities sit tiny in tile corners. | Use consistent compact quantity badges. |
| potion_crafting-18 | P3 | proposal | Herb silhouettes: Moonpetal, Fire Root, Honey Dew, and Shadow Moss share very similar stem shapes. | Make their leaf silhouettes more distinct in addition to color. |
| potion_crafting-19 | P2 | proposal | Ingredient selection: All six inventory tiles use the same frame with no indication of which are required. | Highlight Sea Salt and Star Dust as recipe ingredients. |
| potion_crafting-20 | P2 | proposal | Alchemist note: The instruction is small and faint beneath a decorative heading. | Increase body contrast and line height. |
| potion_crafting-21 | P3 | proposal | Note instruction sequence: Add Star Dust only after the water turns clear is detached from the brewing status. | Pair the note with a visible current brewing step. |
| potion_crafting-22 | P2 | proposal | Recipe-book quotation: Three faint lines compete for space below the recipe list. | Reduce decorative quote prominence or move it to a smaller note block. |
| potion_crafting-23 | P2 | proposal | Footer statistics: Alchemy Level, recipes known, and potions brewed are tiny. | Use a consistent readable metadata scale and clearer group spacing. |
| potion_crafting-24 | P2 | proposal | Keyboard footer: Tab: move Enter: choose is small and omits the mock's Close workshop action. | Add readable keycaps and a visible close/back action if available. |
| potion_crafting-25 | P3 | proposal | Currency identity: 2,450 has a generic circular icon without a currency name. | Add Gold as a small caption or a clearer coin symbol. |

## powerwash settings

[Reviewed image](../../docs/design-audit/originals/powerwash_settings_720p.png) · [Current baseline](../../screenshot-baselines/screens/powerwash_settings_720p.png) · [Visual review](../../design-audit.html#screen=powerwash_settings)

Baseline SHA-256: `083a89475060533ed215da2150839f6e60c4ae38c41907054637817230d2ee01`

The tablet hierarchy is readable, but value padding, tiny symbols and weak action guidance need attention. Product proposals preserve the blue tablet design in mocks.html. Compared visually with the rendered mocks.html target at 1280 × 720.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| powerwash_settings-01 | P2 | defect | Resolution value: 2560 x 1440 touches the value box left border. | Inset every value 10 px from its border. |
| powerwash_settings-02 | P2 | defect | Footer Select hint: The Enter keycap nearly touches Select. | Add an 8 px keycap-to-label gap. |
| powerwash_settings-03 | P2 | defect | Footer Reset hint: Reset and the next Enter keycap sit nearly together. | Give each action group a 20 px trailing gap. |
| powerwash_settings-04 | P2 | defect | Top counters: O 10.00, O 200 and [] 1 are barely legible. | Render recognizable resource icons and increase counter text to 14 px actual size. |
| powerwash_settings-05 | P2 | proposal | Dropdown arrows: Nine values end in tiny v glyphs. | Use consistent 12 px chevrons aligned to a shared inset. |
| powerwash_settings-06 | P2 | proposal | Anti-Aliasing selection: Only the label half gets the pale selection treatment. | Extend a selection outline around the complete label and value row. |
| powerwash_settings-07 | P2 | proposal | Selected label: The Anti-Aliasing text is subdued blue on pale gray. | Use the dark navy text token for the selected label. |
| powerwash_settings-08 | P2 | proposal | Help heading: Anti-Aliasing is close in size to its body copy. | Increase the heading weight and add 8 px before the paragraph. |
| powerwash_settings-09 | P2 | proposal | Help explanation: The help text spans several long lines across the right pane. | Constrain paragraphs to about 48 characters per line. |
| powerwash_settings-10 | P2 | proposal | FPS wording: The explanation says reduce FPS load. | Replace with reduce graphics processing load so the performance effect is clear. |
| powerwash_settings-11 | P2 | proposal | Render Scale value: Off appears in a control otherwise associated with scaling percentages. | Display the actual native scale, such as 100%, with a native label. |
| powerwash_settings-12 | P3 | proposal | Resolution notation: Resolution uses a lowercase x between dimensions. | Use the multiplication sign with consistent spacing. |
| powerwash_settings-13 | P3 | proposal | Vsync label: Vsync is styled differently from the familiar VSync spelling. | Use VSync and expand it in the associated help. |
| powerwash_settings-14 | P2 | proposal | SSAO row: SSAO is the only unexplained acronym in the list. | Expose Ambient occlusion in the label or a short adjacent qualifier. |
| powerwash_settings-15 | P3 | proposal | Model and texture quality: High and Highest use different quality vocabularies. | Use one consistent scale across both quality controls. |
| powerwash_settings-16 | P2 | proposal | Settings categories: The category tabs sit below a tall content block. | Add a small Video settings breadcrumb by SETTINGS to identify the current section immediately. |
| powerwash_settings-17 | P2 | proposal | Header icons: Six unlabeled symbols appear above the settings rows. | Add short captions or an explicit application-navigation group label. |
| powerwash_settings-18 | P3 | proposal | Active gear icon: The selected gear has a brighter tile but no text. | Add a small Settings caption to establish what the selected icon represents. |
| powerwash_settings-19 | P3 | proposal | Tablet backdrop: Large peach tiles surround a dense blue panel. | Add the soft tablet shadow from mocks.html to separate the panel from the scene. |
| powerwash_settings-20 | P3 | proposal | Watermark: FUTURLAB / POWERWASH SIMULATOR is very faint at the pane bottom. | Increase the watermark size modestly while keeping it secondary. |
| powerwash_settings-21 | P2 | proposal | Tab controls A and D: A and D keycaps sit alone at opposite ends of the strip. | Add a concise Change category hint alongside the footer controls. |
| powerwash_settings-22 | P2 | proposal | Close button: A plain X in a blue square is separated from its footer explanation. | Use a standard close glyph with a stable centered alignment. |
| powerwash_settings-23 | P2 | proposal | Reset action: The footer says Reset without naming its scope. | Rename to Reset video settings to match the active tab. |
| powerwash_settings-24 | P2 | proposal | Change status: No visible saved or pending state appears beside the settings. | Add a compact Changes apply immediately message near the footer if that is the actual behavior. |
| powerwash_settings-25 | P3 | proposal | Row borders: Every row has strong double blue divisions. | Reduce inactive border weight and reserve the strongest edge for selection. |
| powerwash_settings-26 | P2 | proposal | Product typography parity: The current help and row labels are more widely spaced and lighter than the compact condensed type in the rendered mock. | Match the target font face, weight and tracking, then recheck wrapping and value padding. |
| powerwash_settings-27 | P3 | proposal | Scene perspective parity: The current peach background is a flat grid while the target shows a receding tiled corner around the tablet. | Restore the target's perspective planes and tile geometry. |
| powerwash_settings-28 | P3 | proposal | Active Video tab parity: The current Video tab has a strong four-sided outline; the target uses a blue fill and bright bottom edge. | Match the target tab treatment with a clear bottom selection rule. |

## race results

[Reviewed image](../../docs/design-audit/originals/race_results_720p.png) · [Current baseline](../../screenshot-baselines/screens/race_results_720p.png) · [Visual review](../../design-audit.html#screen=race_results)

Baseline SHA-256: `236c7b0761d04da724a163adc1673d9c4a4f06db34bd3dcad8261b284552b69b`

The result table has a strong player highlight. Smaller text, skewed table geometry and ambiguous standings context weaken an otherwise clear results screen. Compared visually with the rendered mocks.html target at 1280 × 720.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| race_results-01 | P2 | defect | Table headers: POS., RACER, TIME and PTS are tiny beside the racer names. | Increase headers to a readable 12 to 14 px actual size. |
| race_results-02 | P2 | defect | Points column: The +4 and +3 values almost touch the sloping right edge. | Give the points column a constant 20 px right inset. |
| race_results-03 | P2 | defect | Personal best caption: New personal best! is extremely small under 0:48.220. | Increase the caption to at least 12 px and place it closer to the time. |
| race_results-04 | P2 | defect | Controller help: Arrows to choose A Confirm B Back is tiny in the footer. | Increase help text and separate key labels into readable groups. |
| race_results-05 | P2 | proposal | Position headline: 2nd appears above CUP STANDINGS without a race or cup qualifier. | Label it Race finish or Cup position according to the intended metric. |
| race_results-06 | P2 | proposal | Player chip: P1 in the top-right competes with the second-place result. | Label it Player 1 so it cannot read as finishing position. |
| race_results-07 | P2 | proposal | Table shape: Rows progressively shift left while column content does not follow the same edge. | Keep decorative skew on a backing panel and align the data table to a rectangular grid. |
| race_results-08 | P3 | proposal | Bottom table gap: An empty pale band remains below Pebble. | End the table after the last row or use the band for a labeled total. |
| race_results-09 | P2 | proposal | Race progress: Race 3/4 is embedded in the circuit title. | Give race progress a small four-step marker near the title. |
| race_results-10 | P2 | proposal | Race context: Rainbow Circuit shares one line with progress. | Separate circuit name from championship progress with clear hierarchy. |
| race_results-11 | P2 | proposal | Points label: PTS does not distinguish earned points from cumulative standings points. | Rename the left column Race points. |
| race_results-12 | P2 | proposal | Time label: TIME does not say whether this is total race time. | Use Race time as the column heading. |
| race_results-13 | P2 | proposal | Player identity: The highlighted racer is only named YOU. | Include the driver's name with a You tag for continuity with other screens. |
| race_results-14 | P3 | proposal | Player portrait: The player portrait has the same visual treatment as every opponent. | Add a small player badge next to the green portrait. |
| race_results-15 | P2 | proposal | Standings subset: Only four of eight racers are shown in CUP STANDINGS. | Label the section Top 4 of 8 or provide a compact full-standings affordance. |
| race_results-16 | P2 | proposal | Championship gap: 38 pts and 42 pts require mental subtraction. | Add 4 points behind leader beside the player's cumulative score. |
| race_results-17 | P2 | proposal | Race gap: The player's 2:33.812 requires comparison with 2:31.450. | Show a +2.362 gap alongside the player time. |
| race_results-18 | P3 | proposal | Best lap association: The right card does not identify whose best lap is shown. | Rename BEST LAP to YOUR BEST LAP. |
| race_results-19 | P2 | proposal | Best lap achievement: Personal best is shown with no prior benchmark. | Add the previous best or improvement delta when available. |
| race_results-20 | P3 | proposal | Standings row separators: Thin separators are weak against the landscape-tinted panel. | Increase separator contrast slightly and use even row padding. |
| race_results-21 | P3 | proposal | Side-panel background: The landscape silhouette crosses the standings and lap areas. | Use a more opaque backing behind text for consistent contrast. |
| race_results-22 | P2 | proposal | Next race action: NEXT RACE gives no destination context. | Add the upcoming track name as secondary button text or adjacent copy. |
| race_results-23 | P3 | proposal | Back action: Back uses a text less-than symbol while Next uses a greater-than symbol. | Use matching navigation chevrons with equal label spacing. |
| race_results-24 | P3 | proposal | Footer balance: The help text hugs Back while a large empty middle area remains. | Align help centrally between Back and Next race. |
| race_results-25 | P3 | proposal | Standings emphasis: YOU is yellow but has no shape-based indicator in the side list. | Add a small player marker to match the highlighted result row. |
| race_results-26 | P2 | proposal | Result typography parity: The current title and 2nd are upright, while the rendered target uses italic racing typography. | Match the italic display treatment for the title, finish position and standings heading. |
| race_results-27 | P3 | proposal | Footer action geometry parity: Back and Next race are rectangular in the current image; the target uses slanted button silhouettes. | Apply the target slant to button backings while keeping labels level and padded. |
| race_results-28 | P2 | proposal | Result-row separators parity: The current table relies on alternating pale fills, while the target adds visible cyan row rules. | Add the target row separators to strengthen horizontal scanning. |

## radio buttons

[Reviewed image](../../docs/design-audit/originals/radio_buttons_720p.png) · [Current baseline](../../screenshot-baselines/screens/radio_buttons_720p.png) · [Visual review](../../design-audit.html#screen=radio_buttons)

Baseline SHA-256: `fd1b3220deab73516389f0046c0cb0d0f48f4f1c62bcf77acdd002a4ce68c8bb`

The three groups clearly demonstrate single selection. Eighteen defensible proposals improve comparison and documentation; a static initial state does not justify 25 distinct defects. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| radio_buttons-01 | P2 | proposal | Group card height: Fruit and Size leave over half their card empty. | Size cards to their options and reserve a shared footer for demo notes. |
| radio_buttons-02 | P2 | proposal | Main heading: Radio Button Groups sits flush against its teal bar left edge. | Add a consistent 16 px text inset. |
| radio_buttons-03 | P2 | proposal | Group headings: Favorite Fruit, Size and Color hug their header left edges. | Align headings with the option-label column or apply a clear shared inset. |
| radio_buttons-04 | P2 | proposal | Selected markers: Orange, Small and Blue use a dark magenta dot on a dark card. | Add an adjacent selected badge or stronger outline while retaining the theme swatch. |
| radio_buttons-05 | P3 | proposal | Color options: Red, Green, Blue and Yellow are all text-only. | Add small labeled color swatches without changing the radio selection color. |
| radio_buttons-06 | P2 | proposal | Summary syntax: The footer reads Fruit=Orange, Size=Small, Color=Blue. | Use human-readable labeled values separated into three columns. |
| radio_buttons-07 | P2 | proposal | Summary alignment: Selected begins flush against the footer edge. | Add 16 px inner padding consistent with the title. |
| radio_buttons-08 | P2 | proposal | Selection instructions: The title gives no explanation of one choice per group. | Add a one-line instruction above the cards. |
| radio_buttons-09 | P2 | proposal | Keyboard guidance: No key guide is visible beside the groups. | Add a footer note describing the actual Tab and arrow behavior confirmed in source. |
| radio_buttons-10 | P3 | proposal | Group numbering: Three equal cards have no explicit group identifiers beyond names. | Add small independent group labels so the comparison purpose is evident. |
| radio_buttons-11 | P3 | proposal | Defaults: Orange, Small and Blue are selected with no default annotation. | Mark the initial selection as the fixture default in the summary. |
| radio_buttons-12 | P2 | proposal | Fruit label width: Watermelon is much longer than Apple and Orange. | Add a visible long-label example caption that explains the width test. |
| radio_buttons-13 | P3 | proposal | Size labels: Small, Medium and Large contain no size context. | Name the group Item size so the choices read as a coherent sample form. |
| radio_buttons-14 | P3 | proposal | Unequal option counts: Color has four rows while the other cards have three. | Align card bottoms to the option region and label option counts for comparison. |
| radio_buttons-15 | P3 | proposal | Card border semantics: All three cards use the same magenta as selected radio dots. | Use neutral card outlines so magenta is reserved for selection. |
| radio_buttons-16 | P3 | proposal | Unused lower screen: A broad black band remains below the main panel. | Vertically center the compact demo within the viewport. |
| radio_buttons-17 | P2 | proposal | Summary association: The status line is separated from the cards by a large gutter. | Move each selected value directly beneath its group and keep a compact combined summary. |
| radio_buttons-18 | P3 | proposal | Spec visibility: The enlarged circles and dots are visible but their dimensions are undocumented. | Add a small ring 28 px / dot 18 px caption outside the test controls. |

### Additional capture: radio_buttons

[Image](../../baseline_screenshots/radio_buttons.png)

Active e2e baseline refreshed after visual review. The previous capture remains in originals until audit cleanup.

## real world modals

[Reviewed image](../../docs/design-audit/originals/real_world_modals_720p.png) · [Current baseline](../../screenshot-baselines/screens/real_world_modals_720p.png) · [Visual review](../../design-audit.html#screen=real_world_modals)

Baseline SHA-256: `2417d8153cdac1cbaba619981da860c49c184ba098ea950997db4ba7dd224e98`

This baseline shows launchers rather than open modal states. Sixteen proposals improve the launcher composition; modal contents and behavior cannot be judged from this screenshot. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| real_world_modals-01 | P2 | defect | Status line: No action yet is dark green on the navy background. | Use a readable neutral status color until an action completes. |
| real_world_modals-02 | P2 | proposal | Page framing: The six launchers occupy only the upper third of a large panel. | Use a compact centered launcher panel or fill the lower area with pattern descriptions. |
| real_world_modals-03 | P2 | proposal | Introductory copy: Real-World Modals gives no instruction for opening examples. | Add Choose a pattern to preview its dialog beneath the heading. |
| real_world_modals-04 | P2 | proposal | Delete Item button: Delete Item sounds like an immediate destructive action on the launcher screen. | Label it Preview delete confirmation. |
| real_world_modals-05 | P2 | proposal | Command Palette button: The launcher names the pattern but gives no example of its purpose. | Add Search commands as secondary copy beneath it. |
| real_world_modals-06 | P2 | proposal | Terms of Service button: The label does not reveal that the demo includes scrollable legal text. | Add a short Scrollable agreement description. |
| real_world_modals-07 | P2 | proposal | Side Drawer button: The name gives no placement preview. | Add a miniature right-edge drawer diagram beside the caption. |
| real_world_modals-08 | P2 | proposal | Bottom Sheet button: The launcher has the same visual weight as centered modal examples. | Add a bottom-edge preview diagram to distinguish placement. |
| real_world_modals-09 | P2 | proposal | Cookie Banner button: The launcher does not identify this as a noncentral banner pattern. | Add a narrow banner preview below its label. |
| real_world_modals-10 | P3 | proposal | Pattern grouping: All six buttons form an undifferentiated grid. | Group centered dialogs separately from edge overlays with small headings. |
| real_world_modals-11 | P2 | proposal | Status context: No action yet sits alone without a label. | Place it in an Action result region with a visible heading. |
| real_world_modals-12 | P3 | proposal | Destructive accent: The red Delete Item launcher dominates the otherwise blue grid. | Reserve solid red for the destructive action inside the preview and use a red outline here. |
| real_world_modals-13 | P3 | proposal | Button widths: Delete Item is visibly narrower than the other launcher buttons. | Use equal column widths for the three-column grid. |
| real_world_modals-14 | P2 | proposal | Page title scale: The title is only moderately larger than the letter-spaced button text. | Increase title size and reduce letter spacing on launcher labels. |
| real_world_modals-15 | P2 | proposal | Dismiss instructions: No guidance explains how demo overlays can be closed. | Add a short dismissal note matched to the actual supported controls. |
| real_world_modals-16 | P3 | proposal | Initial-state documentation: The baseline contains no visual specimen of any modal. | Add a labeled static pattern thumbnail region or separate open-state baselines for review. |

## rect algebra lab

[Reviewed image](../../docs/design-audit/originals/rect_algebra_lab_720p.png) · [Current baseline](../../screenshot-baselines/screens/rect_algebra_lab_720p.png) · [Visual review](../../design-audit.html#screen=rect_algebra_lab)

Baseline SHA-256: `82930845574da68b09e7c22afcba18e6b82d6eb4b8844c20c1435ab0b88cb119`

The geometry matches the deliberate rect operations in source. Twenty proposals add explanatory framing without treating fixed strips, ticks or empty panes as layout defects. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| rect_algebra_lab-01 | P2 | proposal | Left diagram heading: The tiled rectangle has no local title. | Label it Window tiling above the drawing. |
| rect_algebra_lab-02 | P2 | proposal | Right diagram heading: The orange box diagram has no local title. | Label it Cut header, pad body, center box. |
| rect_algebra_lab-03 | P2 | proposal | Sidebar rectangle: The tall narrow left pane has no identifier. | Place Sidebar in an external callout tied to that rectangle. |
| rect_algebra_lab-04 | P2 | proposal | Main canvas: The largest blue rectangle is unlabeled. | Add a Canvas label outside its measured geometry. |
| rect_algebra_lab-05 | P2 | proposal | Inspector pane: The right narrow blue pane cannot be distinguished by purpose. | Add an Inspector callout with its 110 px cut width. |
| rect_algebra_lab-06 | P2 | proposal | Bottom strip: The gray strip along the bottom has no description. | Label it Status strip, cut_bottom 26 px. |
| rect_algebra_lab-07 | P2 | proposal | Top strip: The short gray strip above the canvas has no description. | Label it Tabs, cut_top 24 px. |
| rect_algebra_lab-08 | P2 | proposal | Operation order: The left result shows no sequence for the nested cuts. | Add a numbered sequence in a small legend under the diagram. |
| rect_algebra_lab-09 | P2 | proposal | Right header strip: The blue top strip has no stated height. | Add a callout for the 22 px header cut. |
| rect_algebra_lab-10 | P2 | proposal | Inner outline: The purple rectangle does not explain the 6 px inset. | Annotate the inset distance outside the outline. |
| rect_algebra_lab-11 | P2 | proposal | Orange box: The centered orange box has no dimensions. | Label it 64 × 36 near the diagram. |
| rect_algebra_lab-12 | P2 | proposal | Orange outer outline: The dim orange outline can read as a decorative border. | Label it expand 12 px hit target. |
| rect_algebra_lab-13 | P2 | proposal | Top green tick: The green marker is unnamed. | Add align_top to the legend with its green swatch. |
| rect_algebra_lab-14 | P2 | proposal | Edge marker system: Four green markers indicate alignment without named edges. | Add a compact legend naming top, bottom, left and right alignment. |
| rect_algebra_lab-15 | P3 | proposal | Diagram key: Blue, purple, orange and green have no semantic key. | Provide one legend mapping each color to its operation. |
| rect_algebra_lab-16 | P2 | proposal | Center verification: The orange rectangle appears centered but has no reference axes. | Add faint optional center guides that intersect at the box center. |
| rect_algebra_lab-17 | P3 | proposal | Root title: rect algebra: cut, pad, align omits expand despite showing it. | Include expand in the title or subtitle. |
| rect_algebra_lab-18 | P3 | proposal | Unused lower area: A large dark strip remains below both diagrams. | Use that area for the operation legend and formulas. |
| rect_algebra_lab-19 | P2 | proposal | Static demo framing: The diagram looks like an empty application shell. | Add a Static geometry demonstration caption. |
| rect_algebra_lab-20 | P3 | proposal | Measured padding: The outer and inner margins are visible but undocumented. | Label the 10 px left-panel and 20 px right-panel padding in the legend. |

### Additional capture: rect_algebra_lab

[Image](../../baseline_screenshots/rect_algebra_lab.png)

Pixel-identical. Left blue cut regions and right inset bounds are operation visualizations. Improvements should label those operations and dimensions without treating different rectangles as a layout error.

## rubber bandits menu

[Reviewed image](../../docs/design-audit/originals/rubber_bandits_menu_720p.png) · [Current baseline](../../screenshot-baselines/screens/rubber_bandits_menu_720p.png) · [Visual review](../../design-audit.html#screen=rubber_bandits_menu)

Baseline SHA-256: `0f27fae182c9c4efd8f95ebedad7a96029286929a5bc4465f6283cf62ecb7afc`

The menu captures the playful yellow and turquoise direction, but small utility text and crowded character-selection framing need work. Compared visually with the rendered mocks.html target at 1280 × 720.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| rubber_bandits_menu-01 | P2 | defect | Version label: 1.6.0 / RUBBER BANDITS is nearly microscopic at bottom left. | Increase it to a readable 11 px actual size. |
| rubber_bandits_menu-02 | P2 | defect | Character favorite label: FAVOURITE is squeezed between the name and bottom strip. | Give it a dedicated line with at least 12 px readable text. |
| rubber_bandits_menu-03 | P2 | proposal | Top account strip: The user name and Online status share one tiny line. | Separate account name from a labeled connection badge. |
| rubber_bandits_menu-04 | P2 | proposal | Top-right counters: A circled 1 and green 20 have no meaning labels. | Add level and currency captions or recognizable matching icons. |
| rubber_bandits_menu-05 | P2 | proposal | Selected Options item: The selected word starts nearly at the turquoise pill edge. | Add equal left padding to match the other menu labels. |
| rubber_bandits_menu-06 | P3 | proposal | Selection marker: The diamond at the right of Options has little association with navigation. | Use a consistent selection pointer with the same size as the menu text cap height. |
| rubber_bandits_menu-07 | P2 | proposal | Back spacing: Back is separated from Play Credits by a much larger gap. | Make the separation an explicit secondary navigation group with a divider. |
| rubber_bandits_menu-08 | P2 | proposal | Play Credits wording: Play Credits reads like gameplay next to Armory. | Use Credits unless the action actually starts a distinct playable mode. |
| rubber_bandits_menu-09 | P2 | proposal | Leaderboard width: LEADERBOARD is considerably longer than other entries. | Maintain a common menu text width and align the selection pill to that width. |
| rubber_bandits_menu-10 | P2 | proposal | Supporter promo: Get more bandits shares three equally emphasized lines. | Use a clear Supporter pack heading and one supporting sentence. |
| rubber_bandits_menu-11 | P2 | proposal | Promo action: The supporter card has no explicit action label. | Add View pack within the card. |
| rubber_bandits_menu-12 | P3 | proposal | Promo icon: The lightning coin uses yellow against pale yellow. | Add a darker outline so its silhouette reads at a glance. |
| rubber_bandits_menu-13 | P2 | proposal | Character name: EARL THE BIRD overlaps a busy trail of banknotes. | Put the name on a solid or translucent backing panel. |
| rubber_bandits_menu-14 | P2 | proposal | LB and RB hints: The bumper hints are separated by the character name and currency art. | Align both on a quiet common baseline around the selection panel. |
| rubber_bandits_menu-15 | P2 | proposal | Selected character: The large bird overlaps several surrounding characters. | Add a subtle ground marker to identify the selected character without hiding the lineup. |
| rubber_bandits_menu-16 | P3 | proposal | Character count: The lineup contains several figures but no selected index. | Show Earl the Bird with a position indicator such as 3 of 7. |
| rubber_bandits_menu-17 | P3 | proposal | Background crown: A flat yellow block shape rises behind the center figures. | Refine its crown silhouette or reduce its prominence to avoid a placeholder appearance. |
| rubber_bandits_menu-18 | P3 | proposal | Ground shadows: Several characters appear to float above disconnected oval shadows. | Align each shadow beneath its feet using the scene perspective. |
| rubber_bandits_menu-19 | P3 | proposal | Cash props: Dense banknotes cross the bottom selection controls. | Reserve a clear lower control band and keep props above it. |
| rubber_bandits_menu-20 | P3 | proposal | Rightmost prop: A dark circle sits beside the cowboy and looks detached. | Connect it visually to its prop or remove that ambiguous shape. |
| rubber_bandits_menu-21 | P2 | proposal | Menu help strip: Change the game's settings sits at the extreme bottom edge. | Add vertical breathing room around the help text. |
| rubber_bandits_menu-22 | P2 | proposal | Input guide: Only character bumpers are shown despite the main menu being selected. | Add compact confirm and back hints beside the menu help. |
| rubber_bandits_menu-23 | P3 | proposal | Logo prominence: The large logo outweighs the actual selected menu action. | Reduce its footprint slightly to give the menu more breathing room. |
| rubber_bandits_menu-24 | P2 | proposal | Online context: Online appears in the top strip while Back has no destination. | Label Back to main menu when that is the actual destination. |
| rubber_bandits_menu-25 | P3 | proposal | Character art density: The right side packs six smaller figures close together. | Increase silhouette separation so hats and faces do not visually merge. |
| rubber_bandits_menu-26 | P2 | proposal | Menu typography parity: The current LEADERBOARD and PLAY CREDITS occupy more horizontal space than the rendered target. | Match the target font metrics and tracking so menu labels retain the intended compact width. |
| rubber_bandits_menu-27 | P3 | proposal | Supporter card border parity: The current supporter card has one thin white edge; the target has a rounded double outline. | Restore the double border and inner inset to match the promotional-card treatment. |

## scroll click bug

[Reviewed image](../../docs/design-audit/originals/scroll_click_bug_720p.png) · [Current baseline](../../screenshot-baselines/screens/scroll_click_bug_720p.png) · [Visual review](../../design-audit.html#screen=scroll_click_bug)

Baseline SHA-256: `6beb45fb74e2b9a473a885a1d88bafa2190242fefb9e2fab3af96b17134fd3ea`

The initial screenshot can document the click fixture but cannot establish a click-offset bug. Fifteen proposals improve reproducibility and visual evidence without changing the clipped list. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| scroll_click_bug-01 | P2 | proposal | Test title: Scroll Click Bug Test names a bug without a visible expected outcome. | Add a subtitle stating that the clicked label must match the reported index after scrolling. |
| scroll_click_bug-02 | P2 | proposal | Initial status: No button clicked yet does not identify the output field. | Prefix it with Last clicked and show a neutral empty value. |
| scroll_click_bug-03 | P2 | proposal | List context: Only Button 0 through part of Button 6 are visible. | Display 20 buttons above the viewport. |
| scroll_click_bug-04 | P2 | proposal | Scroll position: The initial scrollbar gives no numeric offset. | Add Scroll offset: 0 px beside the status. |
| scroll_click_bug-05 | P2 | proposal | Visible range: The partial Button 6 has no range summary. | Show the visible range separately from the last-click result. |
| scroll_click_bug-06 | P2 | proposal | Reproduction steps: The large blank area below the list contains no procedure. | Place a short scroll-then-click procedure there. |
| scroll_click_bug-07 | P2 | proposal | Expected result: The status line has no comparison target. | Display Expected label and Actual clicked label side by side after selection. |
| scroll_click_bug-08 | P3 | proposal | Title bar alignment: The title text touches the left of its white backing. | Add 12 px inner padding. |
| scroll_click_bug-09 | P3 | proposal | List inset: A wide blank gutter separates button ends from the scrollbar. | Reduce the gutter to a consistent 12 to 16 px if it is not part of the hit-test fixture. |
| scroll_click_bug-10 | P2 | proposal | Button identifiers: Centered Button N labels make row identity far from the left edge. | Add a small left-aligned index column while preserving the button hit area. |
| scroll_click_bug-11 | P3 | proposal | Status legibility: The initial status is light gray and small. | Increase actual text size and use a darker neutral color. |
| scroll_click_bug-12 | P2 | proposal | Reset control: The initial composition offers no visible way to return to its starting state. | Add Reset scroll and result outside the viewport. |
| scroll_click_bug-13 | P3 | proposal | Fixture frame: A large light panel surrounds a comparatively small test area. | Tighten the outer frame around the list and explanatory text. |
| scroll_click_bug-14 | P2 | proposal | Clipping boundary: The partially visible final button could look accidental to a reviewer. | Label the viewport bottom as the intentional clip boundary. |
| scroll_click_bug-15 | P3 | proposal | Index convention: Buttons begin at 0 with no explanation. | Add a Zero-based indices note beside the count. |

## scroll clip bug

[Reviewed image](../../docs/design-audit/originals/scroll_clip_bug_720p.png) · [Current baseline](../../screenshot-baselines/screens/scroll_clip_bug_720p.png) · [Visual review](../../design-audit.html#screen=scroll_clip_bug)

Baseline SHA-256: `b6523f529fdbfa843935c27539c10429561613560e0ffd0a42a582113aa2a0ab`

Four intentional clipping variants are present. Sixteen proposals improve the regression fixture framing; the nested inner container extends below its outer viewport, which requires a dedicated boundary comparison rather than an inferred interaction failure. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| scroll_clip_bug-01 | P2 | proposal | Instruction line: The all-four-panels instruction is extremely small. | Increase it to a readable 13 px actual size. |
| scroll_clip_bug-02 | P2 | proposal | Panel labels: Control, ClipChild rows, Nested wrapper and Scroll in Scroll are tiny colored captions. | Increase caption size and keep text white with a separate colored swatch. |
| scroll_clip_bug-03 | P2 | proposal | Control caption: Control gives no structural explanation. | Add normal rows as its secondary caption. |
| scroll_clip_bug-04 | P2 | proposal | ClipChild caption: ClipChild rows exposes an implementation term without defining it. | Add each row clips its children below the caption. |
| scroll_clip_bug-05 | P2 | proposal | Nested wrapper caption: Nested wrapper does not show which boundary belongs to the wrapper. | Add a small structure diagram above or below the panel. |
| scroll_clip_bug-06 | P2 | proposal | Nested scroll caption: Scroll in Scroll does not distinguish inner and outer scroll regions. | Label both boundaries with matching callout markers. |
| scroll_clip_bug-07 | P2 | proposal | Bottom nested border: The inner dark frame extends below the common outer bottom line. | Add a visible horizontal outer-boundary guide to make intended clipping reviewable. |
| scroll_clip_bug-08 | P2 | proposal | Scroll offsets: No panel reports its current scroll offset. | Add one numeric offset per panel outside the clip region. |
| scroll_clip_bug-09 | P2 | proposal | Comparison state: All panels start at row 1, so the target scrolled state is absent. | Add a companion preset that places all panels at the same nonzero offset. |
| scroll_clip_bug-10 | P2 | proposal | Row labels: Ctrl, Clip, Nest and Inner prefixes differ across comparison panels. | Add matching numeric markers at a common x inset for easier alignment comparison. |
| scroll_clip_bug-11 | P3 | proposal | Row text: Row identifiers are very small inside 40 px colored bands. | Increase label size without changing the measured row dimensions. |
| scroll_clip_bug-12 | P2 | proposal | Test count: The screenshot does not state the total rows per pane. | Add total count to each pane caption. |
| scroll_clip_bug-13 | P2 | proposal | Expected boundary rule: The top note mentions colored borders, which are thin and subtle. | Add a clearly keyed boundary legend outside the fixture. |
| scroll_clip_bug-14 | P2 | proposal | Nested scrolling instructions: The screenshot gives no way to distinguish moving inner versus outer content. | Add a concise two-step nested-scroll procedure beneath the title. |
| scroll_clip_bug-15 | P3 | proposal | Bottom margin: The test panes run nearly to the viewport edge. | Reserve a small external margin for boundary labels without resizing the test viewports. |
| scroll_clip_bug-16 | P2 | proposal | Reset comparison: No reset or synchronized positioning control is visible. | Add Reset all and Set comparison offset controls above the four panes. |

## scroll view

[Reviewed image](../../docs/design-audit/originals/scroll_view_720p.png) · [Current baseline](../../screenshot-baselines/screens/scroll_view_720p.png) · [Visual review](../../design-audit.html#screen=scroll_view)

Baseline SHA-256: `ceee72c7cf0d82f36ff893392d462cfa7439720586f9f8efd807fdddd0826780`

The two-axis demo is understandable, but its inversion control and position indicators are ambiguous. Nineteen distinct improvements are supportable; intentional partial rows remain part of the scroll demonstration. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| scroll_view-01 | P2 | defect | Vertical row text: Item labels touch their blue cards' left edges. | Inset the labels by 12 px. |
| scroll_view-02 | P2 | defect | Horizontal item text: Numbers 1 through 5 sit flush against card left edges. | Center the numbers or use the same 12 px inset as vertical rows. |
| scroll_view-03 | P2 | proposal | Invert Scroll control: Invert Scroll is a gray rectangle with no visible on/off state. | Render a labeled toggle with an explicit current state. |
| scroll_view-04 | P2 | proposal | Horizontal progress bar: A blue segment sits in a gray bar below a 0% label. | Label it visible fraction or scroll position according to what it represents. |
| scroll_view-05 | P2 | proposal | Duplicate horizontal bars: The horizontal view has a scrollbar and another unlabeled bar below it. | Explain the second indicator or combine the redundant visualizations. |
| scroll_view-06 | P2 | proposal | Position labels: Item 1 of 20 and Item 1 of 15 can imply one visible item. | Use First visible item and include the full visible range. |
| scroll_view-07 | P3 | proposal | Panel heights: The vertical viewport is taller than the horizontal viewport. | Align outer demo cards while retaining different internal viewport heights. |
| scroll_view-08 | P2 | proposal | Horizontal blank space: A large empty white area sits below the horizontal cards. | Reduce the viewport to content height or label the intentional spare area. |
| scroll_view-09 | P2 | proposal | Scroll hints: A long instruction line blends both axis gestures together. | Place the vertical gesture near Vertical Scroll and the horizontal gesture near Horizontal Scroll. |
| scroll_view-10 | P3 | proposal | Header padding: Scroll View Demo touches its backing bar left edge. | Add a consistent 12 px title inset. |
| scroll_view-11 | P2 | proposal | Instruction contrast: The gray gesture instruction is faint on the light panel. | Use a darker neutral text token. |
| scroll_view-12 | P2 | proposal | Vertical clip sample: Item 5 is partly shown without explanatory framing. | Add an intentional partial item caption below the viewport. |
| scroll_view-13 | P2 | proposal | Horizontal extent: Five cards are fully visible even though more items exist. | Add a right-edge continuation cue or adjust the viewport to reveal a partial sixth card. |
| scroll_view-14 | P2 | proposal | Scroll counts: The total counts appear only below the panes. | Add 20 items and 15 items beside the pane headings. |
| scroll_view-15 | P3 | proposal | Content balance: Both demos occupy the top half with a large unused lower panel. | Tighten the page height or add a concise behavior comparison there. |
| scroll_view-16 | P2 | proposal | Reset position: No visible control returns either view to its start. | Add a shared Reset position action outside both panes. |
| scroll_view-17 | P2 | proposal | Inversion scope: The single Invert Scroll control does not name affected axes. | Label it Invert both scroll axes if that matches the implementation. |
| scroll_view-18 | P3 | proposal | Indicator alignment: The extra horizontal indicator is centered while text aligns left. | Align the indicator with the horizontal viewport left edge. |
| scroll_view-19 | P2 | proposal | Demonstration state: Both panes show only the initial zero-percent state. | Add a labeled scrolled-state preset for side-by-side verification. |

## scrollbar style lab

[Reviewed image](../../docs/design-audit/originals/scrollbar_style_lab_720p.png) · [Current baseline](../../screenshot-baselines/screens/scrollbar_style_lab_720p.png) · [Visual review](../../design-audit.html#screen=scrollbar_style_lab)

Baseline SHA-256: `28fcd043a647faca491c9ab3417db32e881454fb884762f82a0135e2b145e205`

The four styling variants are intentional, including the low-contrast accent and transparent track. Sixteen proposals make the comparison measurable without replacing those specimens. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| scrollbar_style_lab-01 | P2 | proposal | Main title: The long title begins at the extreme left edge. | Inset it by 16 px to align with the first pane. |
| scrollbar_style_lab-02 | P2 | proposal | Variant captions: default, usage: Accent thumb, explicit colours and thicker, no track are small. | Increase caption size to a readable 13 px actual size. |
| scrollbar_style_lab-03 | P2 | proposal | Default variant: The default pane does not name its track or thumb tokens. | Add a two-line token caption beneath it. |
| scrollbar_style_lab-04 | P2 | proposal | Accent variant: The magenta thumb is a deliberate theme usage but its token is only in the title. | Add an Accent swatch and resolved color value outside the pane. |
| scrollbar_style_lab-05 | P2 | proposal | Explicit colors variant: The orange thumb and dark red track have no color values shown. | Label both RGBA values beneath the pane. |
| scrollbar_style_lab-06 | P2 | proposal | Thicker variant: The thicker teal thumb has no numeric width annotation. | Label its 14 px thickness. |
| scrollbar_style_lab-07 | P2 | proposal | Transparent track: No track can be mistaken for a missing renderer. | Add an explicit Track alpha 0 caption. |
| scrollbar_style_lab-08 | P2 | proposal | Row count: Rows 00 through 13 are visible with no total count. | Add the full row count to each pane's metadata. |
| scrollbar_style_lab-09 | P2 | proposal | Scroll state: Equal thumb positions are shown without reported offsets. | Add a shared scroll offset readout for comparison. |
| scrollbar_style_lab-10 | P2 | proposal | Thumb size: Thumb length is visibly comparable but undocumented. | Show viewport/content ratio below the columns. |
| scrollbar_style_lab-11 | P3 | proposal | Row text: Row identifiers are tiny relative to the empty pane width. | Increase their type size while keeping row heights fixed. |
| scrollbar_style_lab-12 | P2 | proposal | Comparison controls: No control sets every variant to the same scrolled position. | Add shared start, middle and end presets outside the panes. |
| scrollbar_style_lab-13 | P2 | proposal | Track/thumb distinction: The title mentions track and thumb without a diagram. | Use a small annotated scrollbar key in the lower empty area. |
| scrollbar_style_lab-14 | P3 | proposal | Unused lower area: Nearly half the screen is blank. | Use it for the variant specification table. |
| scrollbar_style_lab-15 | P2 | proposal | Baseline variant naming: default is lowercase while Accent uses a token-like capital. | Use consistent human-readable headings and put API tokens in smaller metadata. |
| scrollbar_style_lab-16 | P3 | proposal | Width comparison: The thick variant changes both color and width. | Add a width-only comparison swatch alongside its metadata to isolate thickness. |

### Additional capture: scrollbar_style_lab

[Image](../../baseline_screenshots/scrollbar_style_lab.png)

Active e2e baseline refreshed after visual review. The previous capture remains in originals until audit cleanup.

## secure tunnel

[Reviewed image](../../docs/design-audit/originals/secure_tunnel_720p.png) · [Current baseline](../../screenshot-baselines/screens/secure_tunnel_720p.png) · [Visual review](../../design-audit.html#screen=secure_tunnel)

Baseline SHA-256: `b00daf8458f69f7850ec3a7cf335d77472c52b9e4f23f4efc053d7893fef0052`

The disconnected state and chosen server are visible, but tiny metadata and approximate map/icon artwork fall short of the clearer product framing in mocks.html. Compared visually with the rendered mocks.html target at 1280 × 720.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| secure_tunnel-01 | P2 | defect | Server metadata: Countries and latency values are very small and pale. | Increase them to readable 12 to 13 px actual text with darker contrast. |
| secure_tunnel-02 | P2 | defect | Simulation disclaimer: Network simulation only is tiny at the bottom center. | Place a readable Demo mode badge near the connection state. |
| secure_tunnel-03 | P2 | proposal | Protocol control: WireGuard appears as plain text with no visible change affordance. | Add Change protocol or a cycle icon beside the value to explain the existing cycle action. |
| secure_tunnel-04 | P2 | defect | Privacy icon: The shield illustration looks like overlapping thin geometric fragments. | Replace it with a recognizable centered shield outline. |
| secure_tunnel-05 | P2 | proposal | Rail navigation: Five icon controls have no visible text labels. | Add short labels or an expanded navigation legend for VPN, protection, routing, diagnostics and settings. |
| secure_tunnel-06 | P2 | proposal | Active rail icon: The active concentric-ring symbol does not identify VPN. | Use a recognizable connection or globe icon matching the map heading. |
| secure_tunnel-07 | P3 | proposal | Brand mark: The top rail logo reads as a thin triangle. | Refine it into a consistent solid brand mark with balanced stroke weight. |
| secure_tunnel-08 | P2 | proposal | Connection indicator: Not connected has a tiny orange diamond. | Use a larger status dot and explicit Disconnected text. |
| secure_tunnel-09 | P2 | proposal | Quick-connect recommendation: Fastest available server says Lisbon while Reykjavik shows a lower latency. | Explain the recommendation criterion or recommend the displayed lowest-latency server. |
| secure_tunnel-10 | P2 | proposal | Recommendation copy: Fastest available server - Lisbon is faint under the button. | Increase contrast and separate server name with stronger weight. |
| secure_tunnel-11 | P2 | proposal | Search affordance: Search locations looks like a blank pale field without a search icon. | Add a leading search icon and a stronger field border. |
| secure_tunnel-12 | P3 | proposal | Location count: The number 6 floats at the far right of ALL LOCATIONS. | Use a compact 6 locations label or count badge beside the heading. |
| secure_tunnel-13 | P2 | proposal | Selected server: Lisbon is tinted but its selected state has only color. | Add a checkmark or Selected label. |
| secure_tunnel-14 | P2 | proposal | Selected-server latency: 27 ms is near the row chevron with little room. | Use a fixed right-aligned latency column and separate chevron gutter. |
| secure_tunnel-15 | P3 | proposal | Flag artwork: The small rectangular flags are simplified and visually inconsistent. | Use a consistent flag asset set at equal dimensions. |
| secure_tunnel-16 | P2 | proposal | Map geography: The current screenshot and rendered mock both use coarse continent polygons that obscure recognizable regions. | Refine the shared map artwork with clearer continental silhouettes while retaining its muted visual weight. |
| secure_tunnel-17 | P2 | proposal | Map markers: Several dots have no labels or visual connection to listed servers. | Use a selected marker plus restrained labeled server markers. |
| secure_tunnel-18 | P2 | proposal | Lisbon callout: The callout sits below the marker without a connector. | Add a small pointer anchored to the selected marker. |
| secure_tunnel-19 | P2 | proposal | Callout status dot: A tiny blue dot appears at the right of Lisbon metadata. | Give it a meaning label or remove it as redundant decoration. |
| secure_tunnel-20 | P3 | proposal | Callout metadata: Portugal - pt-04 - 27 ms is narrow and faint. | Increase its size and use clear separators with equal spacing. |
| secure_tunnel-21 | P2 | proposal | Account button: GO in a pale circle could be an action or initials. | Show an account avatar with an Account label or recognizable profile icon. |
| secure_tunnel-22 | P2 | proposal | Footer IP state: Not hidden is visually as quiet as other metadata. | Emphasize IP not hidden while disconnected. |
| secure_tunnel-23 | P2 | proposal | Footer route state: No route is technical language for a consumer connection screen. | Use Not connected as the route summary, retaining route details in diagnostics. |
| secure_tunnel-24 | P2 | proposal | Diagnostics link: View diagnostics is faint at the far right footer. | Increase contrast and add the outward arrow shown in the target. |
| secure_tunnel-25 | P3 | proposal | Privacy card size: The large card carries a short heading and one line of copy. | Reduce its height or add a clear connection-status detail within the reserved space. |
| secure_tunnel-26 | P2 | proposal | Application font parity: The current title and body labels mix narrow display text and widely spaced metadata, while the target uses a compact sans-serif hierarchy. | Match the target sans-serif family and weights across titles, location rows and metadata. |
| secure_tunnel-27 | P2 | proposal | Map marker parity: Current map dots have very thin outlines and fewer visible points than the target's white-ring markers. | Restore the target marker set and use its consistent white ring thickness. |
| secure_tunnel-28 | P3 | proposal | Map backdrop parity: A large pale circular disk is visible behind the current map but absent from the rendered target. | Use the target's smooth pale background without the prominent circular boundary. |

## self align

[Reviewed image](../../docs/design-audit/originals/self_align_720p.png) · [Current baseline](../../screenshot-baselines/screens/self_align_720p.png) · [Visual review](../../design-audit.html#screen=self_align)

Baseline SHA-256: `716c7715cb4afb29a2f24882f5c07448f7ba994f9e2e2c81fa4b01a239f55167`

The staggered positions are intentional SelfAlign examples. Fourteen proposals clarify the geometry and labels; the screenshot does not support 25 independent issues. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| self_align-01 | P2 | proposal | Parent frame: The outlined parent has no label identifying its alignment. | Label it Column parent with its default align-items value. |
| self_align-02 | P2 | proposal | Axis context: The subtitle says alignment but does not identify the horizontal cross axis. | Add a horizontal cross-axis arrow above the frame. |
| self_align-03 | P2 | proposal | Vertical stacking: The three examples descend diagonally without explaining stacking. | Add a vertical main-axis arrow outside the frame. |
| self_align-04 | P2 | proposal | Start specimen: FlexStart left is separated from the border by an unlabeled inset. | Annotate the intentional inset so it is not mistaken for alignment error. |
| self_align-05 | P2 | proposal | Center specimen: Center has no guide showing the parent's center. | Add a faint center guide behind the specimen. |
| self_align-06 | P2 | proposal | End specimen: FlexEnd right has no guide tying its edge to the parent. | Add a small right-edge alignment tick. |
| self_align-07 | P2 | proposal | API naming: FlexStart and FlexEnd are mixed with plain Center. | Use consistent API names and put plain-language translations below them. |
| self_align-08 | P3 | proposal | Legend duplication: Left, Center and Right repeat the box labels without new information. | Replace the pills with a small mapping from SelfAlign values to resulting positions. |
| self_align-09 | P2 | proposal | Specimen text: Text inside the large colored boxes is small. | Increase labels to 14 px actual size while preserving box geometry. |
| self_align-10 | P2 | proposal | Inherited comparison: All three children override alignment, so inheritance is unseen. | Add a separate labeled default-alignment comparison outside the existing fixture. |
| self_align-11 | P3 | proposal | Title wording: SelfAlign Feature Demo repeats generic demo wording. | Use SelfAlign: child alignment overrides. |
| self_align-12 | P3 | proposal | Summary line: Each element picks its own position repeats the subtitle. | Replace it with a concrete explanation that the parent remains a column. |
| self_align-13 | P2 | proposal | Color key: Green, brown and orange have no reason beyond position. | Add textual value labels to the legend so color is optional for interpretation. |
| self_align-14 | P3 | proposal | Page footprint: The compact diagram floats inside broad empty margins. | Center the complete title-diagram-legend group vertically with even top and bottom space. |

### Additional capture: self_align

[Image](../../baseline_screenshots/self_align.png)

Active e2e baseline refreshed after visual review. The previous capture remains in originals until audit cleanup.

## separators

[Reviewed image](../../docs/design-audit/originals/separators_720p.png) · [Current baseline](../../screenshot-baselines/screens/separators_720p.png) · [Visual review](../../design-audit.html#screen=separators)

Baseline SHA-256: `dbea58271005366101f89dd8efa4b79f9962c06847ee207aa89d151068759a36`

The screen clearly presents horizontal, labeled and vertical separators. Seventeen proposals improve specimen organization while preserving deliberate thickness and color differences. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| separators-01 | P2 | proposal | Right card heading: Vertical Separators also contains a horizontal color gallery. | Rename the card Mixed separator examples or split the gallery into its own subsection. |
| separators-02 | P2 | proposal | Default sample label: Basic separator - default style omits the actual thickness. | Add the default pixel thickness to its label. |
| separators-03 | P2 | proposal | Six-pixel sample: The thick line has a dimension in text but no direct ruler. | Add a small thickness marker beside the line. |
| separators-04 | P2 | proposal | Partial-width sample: 60% centered has no full-width reference. | Show a faint full-width guide behind the purple specimen. |
| separators-05 | P2 | proposal | Labeled OR sample: OR has no explanation of its text gap behavior. | Add a caption naming automatic label gap sizing. |
| separators-06 | P2 | proposal | Labeled Settings sample: Settings differs from OR in color and word length simultaneously. | Add a note identifying the long-label and accent-color variation. |
| separators-07 | P2 | proposal | Navigation sample: Home, Settings, Profile and Help look like a working navigation bar. | Label the row Static navigation example unless it is interactive. |
| separators-08 | P2 | proposal | Vertical color changes: Three vertical dividers use different colors without a key. | Add small color names below the sample or a concise caption. |
| separators-09 | P3 | proposal | Vertical dimensions: The vertical separators have no stated height or thickness. | Display their dimensions beneath the navigation example. |
| separators-10 | P3 | proposal | Color gallery width: The four color lines are long but have no width specification. | Add one shared Full width label for the gallery. |
| separators-11 | P2 | proposal | Color gallery semantics: Blue, Green, Purple and Orange identify hues but not tokens. | Include token names or hex values beside the color labels. |
| separators-12 | P2 | proposal | Section hierarchy: Color Gallery is embedded in a separator while Labeled Separators is a heading. | Use a consistent subsection-heading treatment for both. |
| separators-13 | P3 | proposal | Footer sentence: Separators help organize and structure UI content states no testable detail. | Replace it with a note that line dimensions remain fixed across color variants. |
| separators-14 | P2 | proposal | Sample grouping: Descriptions and lines are separated only by white space. | Use consistent labeled specimen rows with equal text-to-line gaps. |
| separators-15 | P3 | proposal | Left card density: The left card has six samples while the right has a large navigation sample and four colors. | Align subsection baselines to make comparison easier. |
| separators-16 | P3 | proposal | Heading size: The page title is much larger than sample metadata. | Slightly reduce the title and give the specifications more readable space. |
| separators-17 | P2 | proposal | Decorative-versus-structural distinction: The screen gives no example of a separator in ordinary content. | Add a small labeled before/after content grouping example outside the existing specimens. |

### Additional capture: separators

[Image](../../baseline_screenshots/separators.png)

Active e2e baseline refreshed after visual review. The previous capture remains in originals until audit cleanup.

## setting row showcase

[Reviewed image](../../docs/design-audit/originals/setting_row_showcase_720p.png) · [Current baseline](../../screenshot-baselines/screens/setting_row_showcase_720p.png) · [Visual review](../../design-audit.html#screen=setting_row_showcase)

Baseline SHA-256: `9634e0131a8695bb1d4acabafa4ba52ae6843cb28cf7e9e5aafa9a23c8017620`

The rows demonstrate several control families, but the visible footer advertises specimens that are absent. Twenty proposals and observed layout issues are supportable without inferring control behavior. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| setting_row_showcase-01 | P2 | defect | Footer inventory: Toggle, Stepper, Slider, Dropdown, Display lists dropdown and display examples that are not visible. | Show those specimen rows or restrict the footer to the three visible families. |
| setting_row_showcase-02 | P2 | proposal | Control columns: Toggles and steppers align at the right edge, while sliders sit near the center. | Use one shared control column across row types. |
| setting_row_showcase-03 | P2 | proposal | Value column: 80% and 65% sit far from their sliders. | Place percentages immediately after each slider in a consistent value column. |
| setting_row_showcase-04 | P2 | proposal | Slider track: The dark tracks do not show a filled portion. | Add a visible value fill while retaining the green thumb. |
| setting_row_showcase-05 | P2 | proposal | Master slider mapping: The 80% thumb appears only about two-thirds across the visible track. | Add labeled endpoints or fix the visual mapping so the reported percentage matches the usable range. |
| setting_row_showcase-06 | P2 | proposal | Music slider mapping: 65% appears just right of the track midpoint. | Use the same explicit range mapping as Master Volume and label its endpoints. |
| setting_row_showcase-07 | P2 | proposal | Toggle state text: Music and Sound Effects are blue and Vibration gray, without state words. | Add On and Off text beside the switches. |
| setting_row_showcase-08 | P2 | proposal | Label hierarchy: Master Volume and Music Volume are larger than Music, Sound Effects and Vibration. | Use one label size for all setting rows. |
| setting_row_showcase-09 | P2 | proposal | Language arrows: The thin chevrons around English are small and faint. | Use clearer chevrons with consistent visible button bounds. |
| setting_row_showcase-10 | P2 | proposal | Graphics arrows: High floats between two tiny chevrons with no grouping. | Put the three elements in a compact outlined stepper group. |
| setting_row_showcase-11 | P3 | proposal | Language option context: English has no indication of position within available choices. | Add a small option count in the specimen caption. |
| setting_row_showcase-12 | P2 | proposal | Graphics label: Graphics is broad while the selected value is High. | Rename it Graphics quality. |
| setting_row_showcase-13 | P2 | proposal | Section spacing: Large blank gaps separate Toggles, Options and Volume. | Reduce gaps and add thin neutral dividers to clarify grouping. |
| setting_row_showcase-14 | P3 | proposal | Title panel: The title bar blends into the larger content panel. | Add a modest gap or divider below the title. |
| setting_row_showcase-15 | P2 | proposal | Section captions: TOGGLES, OPTIONS and VOLUME are small muted labels. | Increase caption size and align their baselines to a consistent inset. |
| setting_row_showcase-16 | P2 | proposal | Music relationship: Music appears as both a toggle and Music Volume in separate groups. | Add a brief note explaining the relationship between enablement and volume. |
| setting_row_showcase-17 | P2 | proposal | Row boundaries: Wide empty space separates labels and controls with no row guides. | Add subtle full-width row separators to aid horizontal scanning. |
| setting_row_showcase-18 | P3 | proposal | Slider size: The 152 px tracks occupy a small fraction of the wide panel. | Increase track width to make percentage differences easier to compare. |
| setting_row_showcase-19 | P2 | proposal | Initial values: The screen has no summary of the demonstrated defaults. | Add a compact default-state summary below the specimen panel. |
| setting_row_showcase-20 | P3 | proposal | Footer contrast: The small family list is dim on the black margin. | Increase its contrast after correcting the inventory. |

## shop interface

[Reviewed image](../../docs/design-audit/originals/shop_interface_720p.png) · [Current baseline](../../screenshot-baselines/screens/shop_interface_720p.png) · [Visual review](../../design-audit.html#screen=shop_interface)

Baseline SHA-256: `371f2b8c669479b5327342849c1389870297bed2d6823ab136d900dea26c5b4a`

The store has a coherent parchment treatment. Small metadata, basket controls and alignment need refinement against the richer mocks.html target. Compared visually with the rendered mocks.html target at 1280 × 720.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| shop_interface-01 | P2 | defect | List headings: TYPE, STOCK and PRICE do not align with their data columns. | Align headings to the same column grid as the rows. |
| shop_interface-02 | P2 | defect | Basket count: 5 items is tiny in the basket heading corner. | Increase it to readable 12 px text and bring it closer to Your basket. |
| shop_interface-03 | P2 | defect | Gold remaining: Gold remaining: 3,570 G is very small beneath Total. | Increase it to 13 px and give it its own line spacing. |
| shop_interface-04 | P2 | defect | Guild note: The note almost touches the item panel's bottom border. | Add at least 10 px bottom inset. |
| shop_interface-05 | P2 | defect | Footer input hints: A Select, X Add to cart and Y Remove are tiny against the dark wood. | Use readable text and distinct keycaps. |
| shop_interface-06 | P2 | proposal | Market context: WILLOWBROOK - MARKET DISTRICT is widely spaced and small. | Increase actual text size and reduce tracking. |
| shop_interface-07 | P2 | proposal | Day status: Day 12 and its tiny sun mark are difficult to read. | Use a readable day label and a recognizable sun icon. |
| shop_interface-08 | P3 | proposal | Store ornaments: The title is flanked by asterisks rather than the richer target ornaments. | Replace them with balanced decorative motifs matching the parchment style. |
| shop_interface-09 | P2 | proposal | Gold total: 3,750 lacks a visible unit beside the number. | Add G or a labeled coin unit while retaining YOUR GOLD below. |
| shop_interface-10 | P2 | proposal | Buy and Sell tabs: Sell is text on the header with a weak tab boundary. | Give both tabs equal dimensions and a clearer active underline. |
| shop_interface-11 | P2 | proposal | Selected item: The potion row uses a pale fill and thin outline only. | Add a small selected marker or stronger left accent. |
| shop_interface-12 | P3 | proposal | Item artwork: Sword, armor and shield icons are tiny simplified shapes. | Use consistent item artwork with stronger silhouettes at equal visual size. |
| shop_interface-13 | P2 | proposal | Stock values: Ordinary stock numbers are small and far from their heading. | Increase size and right-align them in a fixed column. |
| shop_interface-14 | P2 | proposal | Low stock: 3 left and 2 left use red text but no additional marker. | Add a Low stock badge while retaining the numeric count. |
| shop_interface-15 | P2 | proposal | Price formatting: Prices combine amount and G with inconsistent perceived spacing. | Use a fixed gap and align all numeric values to the right. |
| shop_interface-16 | P2 | proposal | Detail category: Consumable is extremely letter-spaced and small. | Use a readable uppercase category label with normal tracking. |
| shop_interface-17 | P2 | proposal | Detail potion art: The large potion is a sparse outline compared with dense text styling. | Add fill, highlights and a grounded shadow consistent with the target art. |
| shop_interface-18 | P2 | proposal | Detail price: 50 G sits below the description without a Unit price label. | Label it Price per item. |
| shop_interface-19 | P2 | proposal | Add to cart button: The button says Add to cart while the destination is called Your basket. | Use Add to basket consistently. |
| shop_interface-20 | P2 | proposal | Basket quantities: x 3 and x 2 are small text with no visible adjustment controls. | Add compact minus and plus controls beside each quantity. |
| shop_interface-21 | P2 | proposal | Remove action: Remove appears only in the footer with no visible basket affordance. | Add a remove control to each basket line. |
| shop_interface-22 | P2 | proposal | Basket line pricing: 150 G and 30 G are shown without explicit line-total context. | Add a small Line total heading or unit-price detail. |
| shop_interface-23 | P2 | proposal | Purchase action: Purchase goods uses the same dark style as Add to cart. | Give checkout the stronger primary emphasis and include 180 G in its label. |
| shop_interface-24 | P2 | proposal | Checkout spacing: Purchase goods nearly touches the basket panel bottom. | Increase the lower inset to match the side padding. |
| shop_interface-25 | P3 | proposal | Footer status: Welcome, adventurer! repeats the greeting at the top. | Use that status area for selected-item or basket feedback instead. |
| shop_interface-26 | P2 | proposal | Detail-title typography parity: Health Potion is light in the current detail panel while the rendered target uses a bold product heading. | Increase the detail title weight and keep the description subordinate. |
| shop_interface-27 | P3 | proposal | Wood backdrop parity: The current background has broad angled dark facets behind the cards; the target uses an even wood-grain field. | Match the target backdrop so geometric facets do not compete with the parchment panels. |
| shop_interface-28 | P2 | proposal | Basket amount emphasis parity: 150 G and 30 G are light in the current basket while the target uses bold line totals. | Use the target's stronger amount weight to align basket scanning with the bold overall total. |

## simple button

[Reviewed image](../../docs/design-audit/originals/simple_button_720p.png) · [Current baseline](../../screenshot-baselines/screens/simple_button_720p.png) · [Visual review](../../design-audit.html#screen=simple_button)

Baseline SHA-256: `82cc31ae8fe0da493612dcabbf7bb074e203bbea5aac2edded40cf392a50b3b9`

The compact card is coherent. Sixteen distinct proposals improve the button specimens and feedback; the static image does not justify 25 separate issues. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| simple_button-01 | P2 | proposal | Main action label: Click Me! names the input gesture rather than the counter action. | Use Add a click or Increment counter. |
| simple_button-02 | P2 | proposal | Counter state: No clicks yet... hides the numeric initial value. | Show Click count: 0 in a stable-width result region. |
| simple_button-03 | P2 | proposal | Counter association: The gray pill has no label identifying it as button feedback. | Add a small Result caption above the count. |
| simple_button-04 | P2 | proposal | Subtitle: Go ahead, give it a click! repeats the main button wording. | Explain that the main action increments the count. |
| simple_button-05 | P2 | proposal | More Styles heading: More Styles is much smaller than surrounding labels. | Increase it to a readable subsection heading. |
| simple_button-06 | P2 | proposal | Secondary specimen: Secondary identifies a style but not its relationship to the counter. | Add a caption stating whether variant buttons share the counter behavior. |
| simple_button-07 | P2 | proposal | Outline specimen: The white button has a very fine lavender edge. | Strengthen its outline without changing the outline-style specimen. |
| simple_button-08 | P2 | proposal | Bottom specimen labels: Small, Pill and Buttons form a phrase instead of naming three variations. | Give each specimen a standalone label describing its style. |
| simple_button-09 | P2 | proposal | Pill specimen: Pill appears almost as rectangular as Small and Buttons. | Label the intended radius numerically or render an unmistakable capsule in that specimen. |
| simple_button-10 | P3 | proposal | Size comparison: Small is shown without a dimension reference. | Add its height below the row and the main button height in the same legend. |
| simple_button-11 | P3 | proposal | Button alignment: The main button and lower rows use slightly different left insets. | Align all specimen groups to a shared inner card grid. |
| simple_button-12 | P2 | proposal | Color comparison: Pastel green, pink and gold buttons change hue and label simultaneously. | Use a small legend that names each token so comparison is explicit. |
| simple_button-13 | P2 | proposal | Reset action: No reset control is visible for the click count. | Add a quiet Reset count action near the result. |
| simple_button-14 | P3 | proposal | Footer space: A large blank region remains beneath the small buttons inside the card. | Use it for specimen dimensions and shared behavior notes. |
| simple_button-15 | P3 | proposal | Decoration distribution: Confetti dots are scattered far from the central card. | Group them closer to the card or reduce their number to keep attention on the examples. |
| simple_button-16 | P2 | proposal | Initial-state coverage: Only the zero-click state is visible. | Add a companion activated-state baseline showing the counter feedback. |

## split pane lab

[Reviewed image](../../docs/design-audit/originals/split_pane_lab_720p.png) · [Current baseline](../../screenshot-baselines/screens/split_pane_lab_720p.png) · [Visual review](../../design-audit.html#screen=split_pane_lab)

Baseline SHA-256: `013a4fc6c2231b2592e8411cf135e089508fae079c51f060bcec76af4a996383`

The nested split geometry is deliberate. Sixteen proposals expose the dimensions and draggable boundaries without claiming unobserved dragging failures. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| split_pane_lab-01 | P2 | defect | Pane labels: sidebar, preview and details touch their panes' left edges. | Inset each label by 12 px without changing split geometry. |
| split_pane_lab-02 | P2 | proposal | Vertical divider: The narrow vertical separator is hard to distinguish from a pane edge. | Add a centered grip indicator within the divider. |
| split_pane_lab-03 | P2 | proposal | Horizontal divider: The horizontal bar has no visible drag handle. | Add a short centered grip with a contrasting stroke. |
| split_pane_lab-04 | P2 | proposal | Ratio caption: sidebar 28% preview 62% is tiny beneath the title. | Increase it to 13 px actual text. |
| split_pane_lab-05 | P2 | proposal | Ratio meaning: Preview 62% could be interpreted as a share of the whole screen. | Label it Preview height: 62% of right pane. |
| split_pane_lab-06 | P2 | proposal | Sidebar ratio: Sidebar 28% has no dimension unit context. | Label it Sidebar width: 28% of workspace. |
| split_pane_lab-07 | P2 | proposal | Details ratio: The details pane has no corresponding size readout. | Show its complementary height percentage. |
| split_pane_lab-08 | P2 | proposal | Minimum limits: The screenshot shows no allowed resize range. | Display the actual minimum and maximum ratios beside the readouts. |
| split_pane_lab-09 | P2 | proposal | Reset geometry: No reset action is visible. | Add Reset to 28% / 62% above the workspace. |
| split_pane_lab-10 | P2 | proposal | Pixel dimensions: Large empty panes do not show how resizing affects their size. | Display live width × height in each pane center. |
| split_pane_lab-11 | P2 | proposal | Nesting explanation: The title says drag either divider without explaining the nested split. | Add a small diagram that shows the horizontal split belongs only to the right pane. |
| split_pane_lab-12 | P3 | proposal | Footer hint: The resize-cursor instruction is very small. | Increase its size and align it with the workspace inset. |
| split_pane_lab-13 | P2 | proposal | Grip hit region: Thin visual dividers give little indication of their draggable extent. | Show an external annotation describing the actual hit-region width. |
| split_pane_lab-14 | P3 | proposal | Pane headings: Lowercase labels are visually weak over large blank fields. | Use consistent sentence-case headings with slightly larger type. |
| split_pane_lab-15 | P2 | proposal | Static review evidence: Only one split ratio is captured. | Add labeled narrow-sidebar and tall-preview presets for additional baselines. |
| split_pane_lab-16 | P3 | proposal | Title line: The instruction is embedded in the page title. | Separate Split Pane Lab from a short interaction subtitle. |

## sports settings

[Reviewed image](../../docs/design-audit/originals/sports_settings_720p.png) · [Current baseline](../../screenshot-baselines/screens/sports_settings_720p.png) · [Visual review](../../design-audit.html#screen=sports_settings)

Baseline SHA-256: `f4147107cdcb705670f6843646fcb157139a6042cef16627e4b7980ba50763f4`

The sports settings preserve the mint accent and blurred scene, but unusual value markers, unclear units and dense metadata need refinement. Compared visually with the rendered mocks.html target at 1280 × 720.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| sports_settings-01 | P2 | defect | Controller tab hints: LB and RB labels are nearly microscopic in pale keycaps. | Increase glyph size and keycap dimensions. |
| sports_settings-02 | P2 | proposal | Screen percentage value: 100 appears without a percent symbol. | Display 100%. |
| sports_settings-03 | P2 | proposal | Resolution formatting: 2,560x1,440 uses thousands separators and a lowercase x. | Use 2560 × 1440. |
| sports_settings-04 | P2 | proposal | Window mode value: Borderless window is followed by small dot marks. | Replace ambiguous dots with clear cycling chevrons or a dropdown indicator. |
| sports_settings-05 | P2 | proposal | V Sync value: Enabled is followed by dots instead of a recognizable control. | Use a consistent on/off control treatment. |
| sports_settings-06 | P2 | proposal | Anti-aliasing method: TSR is an unexplained acronym. | Expand the method name in the help pane when selected. |
| sports_settings-07 | P2 | proposal | Dynamic resolution: Enabled has the same dot pattern as multi-choice settings. | Distinguish binary settings from multi-choice selectors. |
| sports_settings-08 | P2 | proposal | Motion blur value: Motion blur shows 1 with no unit or scale. | Use On or a labeled intensity percentage according to the real value meaning. |
| sports_settings-09 | P2 | proposal | Max FPS units: 120 has no FPS suffix in the selected row. | Display 120 FPS. |
| sports_settings-10 | P2 | proposal | Framerate target units: 60 has no FPS suffix. | Display 60 FPS and explain its relationship to dynamic resolution. |
| sports_settings-11 | P2 | proposal | Gamma endpoints: The gamma slider shows 2.2 without range labels. | Show minimum and maximum values beneath the track. |
| sports_settings-12 | P2 | proposal | FPS endpoints: The Max FPS track has no endpoint labels. | Label its supported range and any unlimited endpoint. |
| sports_settings-13 | P2 | proposal | Slider fill: Each mint track looks fully filled regardless of thumb position. | Use distinct filled and remaining segments. |
| sports_settings-14 | P2 | proposal | Selected thumb: The dark thumb is small against the mint selection row. | Increase thumb size or give it a light outline. |
| sports_settings-15 | P2 | proposal | Selection frame: The selected row touches the divider with little trailing space. | Add a consistent gutter between the row edge and the help separator. |
| sports_settings-16 | P2 | proposal | Help title: Max FPS is smaller than the explanatory body text. | Increase heading prominence and separate it from the body. |
| sports_settings-17 | P2 | proposal | Help line length: The explanation spans a wide, two-line measure. | Constrain it to about 45 to 55 characters per line. |
| sports_settings-18 | P2 | proposal | Default value: Default value: 120 is visually similar to the body copy. | Style the default as a labeled value pair with the FPS unit. |
| sports_settings-19 | P2 | proposal | Backdrop readability: The fence and figure remain visible behind all setting labels. | Increase the opacity of the left backing panel for consistent text contrast. |
| sports_settings-20 | P3 | proposal | Divider accent: The mint vertical segment resembles a scrollbar without a label. | Use a uniform pane divider or label the scroll indicator if it represents position. |
| sports_settings-21 | P2 | proposal | Category context: GRAPHICS appears under the active VIDEO tab but no page title is shown. | Add Video settings as the local heading. |
| sports_settings-22 | P2 | proposal | Reset scope: Reset to default does not identify whether it affects the tab or all settings. | Name the actual scope in the footer action. |
| sports_settings-23 | P2 | proposal | Ready status: Ready is indistinguishable from generic footer text. | Use a clear Saved or No pending changes status if supported by the state model. |
| sports_settings-24 | P3 | proposal | Footer grouping: Ready, Y Reset and B Back are crowded at the right edge. | Separate status from action groups and use consistent keycap spacing. |
| sports_settings-25 | P2 | proposal | Graphics quality relationship: Graphics quality and Texture quality both show Ultra with no hierarchy. | Clarify whether texture is an override of the overall preset. |
| sports_settings-26 | P2 | proposal | Settings typography parity: The current settings labels are lighter and more widely spaced than the target's compact sans-serif labels. | Match target font metrics and weight while preserving the existing row positions. |
| sports_settings-27 | P3 | proposal | Selected-row treatment parity: The current Max FPS row is uniformly mint while the target shades its label half darker. | Restore the target's distinct label and value backgrounds within the selected row. |
| sports_settings-28 | P3 | proposal | Selection accent parity: The current Video underline and tall mint indicator are solid; the target uses a diagonal stripe detail. | Match the target stripe treatment without changing indicator dimensions. |

## stepper showcase

[Reviewed image](../../docs/design-audit/originals/stepper_showcase_720p.png) · [Current baseline](../../screenshot-baselines/screens/stepper_showcase_720p.png) · [Visual review](../../design-audit.html#screen=stepper_showcase)

Baseline SHA-256: `a86885f7ca278450b8e862de3b2f8b4bc1f0e5da5522374987de6dc823856aa0`

The value and card selectors are visible, but grouping and specimen metadata are weak. Eighteen actionable proposals are supported by this static state. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| stepper_showcase-01 | P2 | proposal | Left steppers: Thin arrows float around values without visible grouping. | Place each arrow-value-arrow group in a subtle outlined region. |
| stepper_showcase-02 | P2 | proposal | Resolution text: 2560x1440 uses a compact lowercase x. | Use 2560 × 1440. |
| stepper_showcase-03 | P2 | proposal | Framerate value: 60 lacks a unit. | Show 60 FPS. |
| stepper_showcase-04 | P2 | proposal | Stepper arrows: All arrow glyphs are small and muted. | Increase glyph size and contrast while retaining consistent spacing. |
| stepper_showcase-05 | P2 | proposal | Choice count: None of the five steppers reveals its number of options. | Add a compact index/count in specimen metadata. |
| stepper_showcase-06 | P2 | proposal | Boundary behavior: The initial values do not explain whether arrows wrap. | Add a footer note stating the actual wrap or clamp behavior. |
| stepper_showcase-07 | P2 | proposal | Card-selector labels: Healer Warrior Mage reads as one line of words. | Separate choices into clear segments with consistent spacing. |
| stepper_showcase-08 | P2 | proposal | Selected class: Warrior is distinguished mainly by brighter text. | Add a selected underline or compact marker. |
| stepper_showcase-09 | P2 | proposal | Card text hierarchy: Warrior and the description are separated by a large empty gap. | Bring the description closer to the title. |
| stepper_showcase-10 | P2 | proposal | Card description: Heavy armor, melee attacks, high defense is small for the card size. | Increase it to 14 to 16 px actual size. |
| stepper_showcase-11 | P3 | proposal | Card artwork: A large solid coral card contains only two text blocks. | Add a restrained class emblem or compact stat row. |
| stepper_showcase-12 | P2 | proposal | Card association: The Warrior card has no connector to the selector above. | Align its title with the active tab and keep the active accent consistent. |
| stepper_showcase-13 | P3 | proposal | Card color meaning: The coral fill has no explanatory legend. | Identify it as Warrior accent in the specimen notes. |
| stepper_showcase-14 | P2 | proposal | Section comparison: Stepper Variants and Card Selector do not state the shared underlying behavior. | Add a subtitle explaining that both cycle through a finite option list. |
| stepper_showcase-15 | P3 | proposal | Panel whitespace: Both panels leave substantial empty space below their controls. | Use the lower area for option counts and interaction notes. |
| stepper_showcase-16 | P2 | proposal | Default state: Medium, English, Normal and 60 appear without default markers. | Add a compact Current defaults summary or reset control. |
| stepper_showcase-17 | P3 | proposal | Heading alignment: The left heading aligns left while Card Selector is centered. | Use a consistent heading alignment across the two panes. |
| stepper_showcase-18 | P2 | proposal | Keyboard guidance: No visible hint explains how the active selector changes choices. | Add a guide matched to the implemented arrow and focus behavior. |

## styled text lab

[Reviewed image](../../docs/design-audit/originals/styled_text_lab_720p.png) · [Current baseline](../../screenshot-baselines/screens/styled_text_lab_720p.png) · [Visual review](../../design-audit.html#screen=styled_text_lab)

Baseline SHA-256: `e24fbf539d8d6ee7e258d13f2d497c72e2b525e0fc41118f39afeb6d1d3db698`

The font-weight and fallback cases are intentional and confirmed in source. Eighteen proposals improve annotation and legibility without treating fallback-to-regular as a defect. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| styled_text_lab-01 | P2 | proposal | Top specimen caption: DGOne + DGOne@bold is tiny above the sample. | Increase metadata to a readable 12 to 13 px actual size. |
| styled_text_lab-02 | P2 | proposal | Regular/bold sample: Text begins flush against the colored block edge. | Inset the specimen text while preserving its measured available width. |
| styled_text_lab-03 | P2 | proposal | Weight-only specimen: one colour two weights does not identify which words request bold. | Add a run-by-run weight legend beneath the sample. |
| styled_text_lab-04 | P2 | proposal | Colored bold specimen: BOLD changes both weight and color. | Explicitly label this as a combined weight-and-color example. |
| styled_text_lab-05 | P2 | proposal | Wrap specimen: The bold span crosses the line break without a visible run boundary key. | Add a source-text caption marking the bold span. |
| styled_text_lab-06 | P2 | proposal | Wrap width: The wrapping block has no width value shown. | Annotate the exact content width used for measurement. |
| styled_text_lab-07 | P2 | proposal | Wrap line spacing: Pixel-text lines are tightly packed. | Add a separate comparison with increased line height while preserving the current baseline specimen. |
| styled_text_lab-08 | P2 | proposal | Hard-break caption: The caption exposes a literal backslash-n token without explanation. | Label it Explicit newline and show the escaped source in metadata. |
| styled_text_lab-09 | P2 | proposal | Hard-break sample: Heading and body text have very little vertical separation. | Add line boxes or baseline guides to make the hard-break behavior inspectable. |
| styled_text_lab-10 | P2 | proposal | Second heading: Another heading continues close to the preceding body line. | Use a small external run label to distinguish separate weighted spans. |
| styled_text_lab-11 | P2 | proposal | Fallback caption: The no-@bold explanation is tiny and stretches across the block. | Use a readable two-line caption with the expected regular fallback emphasized. |
| styled_text_lab-12 | P2 | proposal | Fallback comparison: asks for regular and asks for bold also change color. | Add a same-color comparison so the weight fallback can be assessed directly. |
| styled_text_lab-13 | P2 | proposal | Font identity: The fallback sample names default face without a readable font-family label. | Show the actual registered family in the caption. |
| styled_text_lab-14 | P3 | proposal | Sample numbering: Five test cases have captions but no stable identifiers. | Number them so screenshots can be compared with test definitions. |
| styled_text_lab-15 | P2 | proposal | Expected results: No case states its expected visual outcome beside the specimen. | Add one short expected-result line per case. |
| styled_text_lab-16 | P3 | proposal | Page balance: All samples occupy the left side, leaving a large blank right region. | Use the right region for source spans and resolved weights. |
| styled_text_lab-17 | P2 | proposal | Glyph metric comparison: Different weights are visible but baseline and advance differences are not annotated. | Add optional baseline and advance-width guides outside the existing text. |
| styled_text_lab-18 | P3 | proposal | Title hierarchy: Styled Text Lab and sample captions are similarly letter-spaced. | Use a clearer title size and neutral metadata typography. |

## sync scroll lab

[Reviewed image](../../docs/design-audit/originals/sync_scroll_lab_720p.png) · [Current baseline](../../screenshot-baselines/screens/sync_scroll_lab_720p.png) · [Visual review](../../design-audit.html#screen=sync_scroll_lab)

Baseline SHA-256: `825c267f26c6c4ed5287f8901368c5e682946dd468d3aeb29513e1270e7108ce`

The unequal document lengths and header-only horizontal synchronization are deliberate. Eighteen proposals clarify the measured comparison; the zero-offset image cannot prove or disprove synchronization. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| sync_scroll_lab-01 | P2 | proposal | Group-one subtitle: The explanation of the short pane stopping is tiny. | Increase explanatory text to 12 to 13 px actual size. |
| sync_scroll_lab-02 | P2 | proposal | Document text: Line numbers and code are very small in all three panes. | Increase specimen text size while retaining the row-height relationship. |
| sync_scroll_lab-03 | P2 | proposal | Before/after content: Both sides visibly show value = compute(x) on highlighted rows. | Use a small actual code difference so before and after are meaningful labels. |
| sync_scroll_lab-04 | P2 | proposal | Change colors: Before is green and after is red without a key. | Add a legend explaining the color mapping or use conventional removed/added mapping. |
| sync_scroll_lab-05 | P2 | proposal | Short-pane role: Short has fewer rows but the same visible viewport height. | Label it Short content, 24 rows to distinguish content length from viewport height. |
| sync_scroll_lab-06 | P2 | proposal | Y status: y: before 0 after 0 short 0 is tiny and compressed. | Use three labeled offset cells with px units. |
| sync_scroll_lab-07 | P2 | proposal | Sync state: y-in-sync and short-following are terse implementation-like phrases. | Use readable badges such as Matching offsets and Short pane following. |
| sync_scroll_lab-08 | P2 | proposal | Clamped state: The initial frame does not show the short pane at its endpoint. | Add a preset that scrolls the long panes beyond the short pane maximum. |
| sync_scroll_lab-09 | P2 | proposal | Group-two explanation: Header tracks the table's x is a long tiny line. | Use a clear heading and one readable instruction line. |
| sync_scroll_lab-10 | P2 | proposal | Header cells: col 1 through col 8 are tiny and flush to their cell starts. | Increase text size and use consistent inner padding. |
| sync_scroll_lab-11 | P2 | proposal | Table cells: r1c1-style labels are hard to scan across wide empty cells. | Add faint column guides and readable row/column identifiers. |
| sync_scroll_lab-12 | P2 | proposal | Header scrollbar: The header shows its own horizontal scrollbar above the table. | Label it as a synchronized independent scrollbar so the duplicate bars are intentional. |
| sync_scroll_lab-13 | P2 | proposal | X status: x: header 0 table 0 is difficult to read below the table. | Use the same offset-cell treatment as the Y group. |
| sync_scroll_lab-14 | P2 | proposal | Axis distinction: Group 1 and Group 2 are named numerically rather than by tested axis. | Use Vertical group and Horizontal header/table group headings. |
| sync_scroll_lab-15 | P2 | proposal | Reset control: No visible control returns both groups to a known state. | Add Reset all offsets outside the scroll regions. |
| sync_scroll_lab-16 | P3 | proposal | Right-side space: The fixture ends around three-quarters of the screen width. | Use the remaining area for the synchronization rules and expected clamping behavior. |
| sync_scroll_lab-17 | P2 | proposal | Viewport metrics: The thumbnail-like panes have no viewport dimensions shown. | Display viewport and content sizes in a compact spec table. |
| sync_scroll_lab-18 | P2 | proposal | Driver instruction: The page does not say which pane the reviewer should scroll first. | Add a concrete before-pane and table-pane test sequence. |

## system profile lab

[Current baseline](../../screenshot-baselines/screens/system_profile_lab_720p.png) · [Visual review](../../design-audit.html#screen=system_profile_lab)

Baseline SHA-256: `0681d6834ac760cf6550b90c194c4a281e06124ce8187822804ce5fb959dc831`

The profiler is stopped with no samples, so zero data and unavailable process metrics are not evidence of a runtime failure. Twenty proposals improve the empty-state design and measurement labels. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| system_profile_lab-01 | P2 | proposal | Empty FPS metric: 0.0 FPS is shown before any frames are recorded. | Use an em dash or No data until samples exist. |
| system_profile_lab-02 | P2 | proposal | Empty frame timing: Avg 0.00 ms/frame reads as a measured zero. | Render an explicit unmeasured state consistently across timing values. |
| system_profile_lab-03 | P2 | proposal | Percentile labels: p50, p95 and p99 are packed into one line. | Use separate labeled metric blocks with ms units on each. |
| system_profile_lab-04 | P2 | proposal | Sample window: Last 0 frames is appended to the metric sentence. | Give sample count and collection window their own metadata label. |
| system_profile_lab-05 | P2 | proposal | Recording state: Recording stopped is small and visually detached from Start recording. | Place a status badge beside the recording control. |
| system_profile_lab-06 | P2 | proposal | Primary action: Start recording has the same emphasis as Reset and Sort recent. | Make recording the primary action in the stopped state. |
| system_profile_lab-07 | P2 | proposal | Pause view control: Pause view appears while no samples exist. | Explain view pause independently of recording or disable it until there is a view to pause. |
| system_profile_lab-08 | P2 | proposal | Sort action: Sort recent does not reveal sorting direction or selected column. | Show Sort: recent average, descending with a matching header marker. |
| system_profile_lab-09 | P2 | proposal | Chart empty state: No samples is placed near the upper-left of a large empty plot. | Center a helpful empty state with Start recording to collect frame timings. |
| system_profile_lab-10 | P2 | proposal | Chart identity: The large plot has no title or axes in its empty state. | Add Frame time history and labeled ms/time axes. |
| system_profile_lab-11 | P2 | proposal | Table empty state: No system samples yet repeats the chart's empty message. | Explain that system timings will appear after recording starts. |
| system_profile_lab-12 | P2 | proposal | CPU unavailable: Process CPU: unavailable provides no local explanation. | Add a concise platform-support note or diagnostic reason when known. |
| system_profile_lab-13 | P2 | proposal | Memory unavailable: Resident memory: unavailable is buried in the same sentence. | Use separate CPU and memory metric cards with clear unavailable values. |
| system_profile_lab-14 | P2 | proposal | Add load action: Add load gives no indication of the synthetic load type. | Label it Add synthetic workload and show its enabled state. |
| system_profile_lab-15 | P2 | proposal | Hide panel action: Hide panel does not identify what disappears. | Rename it Hide profiler panel. |
| system_profile_lab-16 | P2 | proposal | Table units: Recent avg and Overall avg use parentheses while Last frame ms does not. | Use one heading format with ms units consistently. |
| system_profile_lab-17 | P2 | proposal | System/phase hierarchy: System / phase combines two concepts in a single column heading. | Clarify the expected hierarchy with a sample or two-line heading. |
| system_profile_lab-18 | P3 | proposal | Toolbar layout: Add load and Hide panel occupy a separate top row from the profiler controls. | Group demo controls separately with a small Demo workload caption. |
| system_profile_lab-19 | P2 | proposal | Reset scope: Reset is ambiguous between samples, view state and workload. | Rename it Clear samples if that is its actual effect. |
| system_profile_lab-20 | P3 | proposal | Unused lower area: The table empty state leaves a broad black field. | Use a compact bordered table shell sized to the available data area. |

### Additional capture: system_profile_lab

[Image](../../baseline_screenshots/system_profile_lab.png)

Current capture removes Add load focus outline. Both are the stopped, empty state with zero timing statistics and unavailable CPU/RAM. Neither demonstrates live refresh rate or recording performance.

## tab container

[Current baseline](../../screenshot-baselines/screens/tab_container_720p.png) · [Visual review](../../design-audit.html#screen=tab_container)

Baseline SHA-256: `e8f83335c9f12baf65135a56351884cb4c65d4f805133f7fa4d8ff65f73f3f32`

The active Profile tab is clear, but the static profile content and footer actions need stronger demo framing. Eighteen supportable findings avoid claiming unseen tab behavior. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| tab_container-01 | P2 | proposal | Page identity: The panel begins directly with tabs and has no demo title. | Add Tab container demo above the tab strip. |
| tab_container-02 | P2 | proposal | Profile heading: Profile Settings suggests editable controls but shows only text. | Use Profile details for the displayed read-only state. |
| tab_container-03 | P2 | proposal | Username row: Username and player_one share the same weight in one string. | Separate label and value into aligned columns. |
| tab_container-04 | P2 | proposal | Email row: The email address is presented as plain prose. | Use a clear value field with a copy affordance if appropriate to the demo. |
| tab_container-05 | P2 | proposal | Subscription value: Premium is plain text among metadata. | Use a small subscription badge to distinguish status from identity fields. |
| tab_container-06 | P2 | proposal | Last login: Today at 10:30 AM lacks a time-zone or fixed baseline context. | Use a stable sample date/time label or identify it as fixture data. |
| tab_container-07 | P2 | proposal | Field spacing: Five short rows are spread across a tall empty content panel. | Reduce row gaps and use a compact details grid. |
| tab_container-08 | P2 | proposal | Content width: All content is left-aligned inside a very wide blank white panel. | Constrain the details region or add a second metadata column. |
| tab_container-09 | P2 | proposal | OK action: OK is dark gray and visually resembles a disabled action. | Use a clear primary action style if it is enabled. |
| tab_container-10 | P2 | proposal | Apply action: Apply appears active despite no visible editable fields. | Show a No pending changes state or omit Apply from this read-only tab. |
| tab_container-11 | P2 | proposal | Cancel action: Cancel has no context for what is canceled in the static details view. | Use Close for a read-only demonstration or explain the editable state. |
| tab_container-12 | P2 | proposal | Footer alignment: The button group ends before the content panel's right edge. | Align the footer group to the content right inset. |
| tab_container-13 | P2 | proposal | Tab semantics: Account and Settings are broad labels that do not describe their contents. | Use labels matching the actual account/audio/display content after checking each pane. |
| tab_container-14 | P3 | proposal | Tab edges: All three tabs have rounded top shapes, including internal joins. | Simplify internal joins to create one coherent tab strip. |
| tab_container-15 | P2 | proposal | Selected tab cue: A thin blue underline is the only explicit active marker. | Add stronger selected text weight while retaining the underline. |
| tab_container-16 | P2 | proposal | Demo instructions: No text explains that changing tabs swaps the content pane. | Add one concise instruction outside the specimen. |
| tab_container-17 | P3 | proposal | Profile data provenance: player@example.com appears without a sample-data cue. | Add a small Example profile caption. |
| tab_container-18 | P2 | proposal | State coverage: Only Profile content is visible in the baseline. | Capture Account and Settings as named companion states for visual review. |

### Additional capture: tab_container

[Image](../../baseline_screenshots/tab_container.png)

Current capture removes Profile focus outline and leaves the blue selected-tab underline clear. The large profile content well and three small footer actions remain unchanged.

## tabbing

[Current baseline](../../screenshot-baselines/screens/tabbing_720p.png) · [Visual review](../../design-audit.html#screen=tabbing)

Baseline SHA-256: `bec573360ceb5fe28ebd2b9c68ed790b4bb4559f35d8b4d1c2d7af6f3ea6b396`

The initial frame contains no active focus ring, which is not itself a keyboard defect. Fifteen proposals improve the fixture instructions and evidence capture. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| tabbing-01 | P2 | proposal | Title wording: KEYBOARD NAV abbreviates the feature name. | Use Keyboard navigation. |
| tabbing-02 | P2 | proposal | Instruction text: Use TAB to navigate, ENTER to click is small. | Increase actual instruction size to 13 px. |
| tabbing-03 | P2 | proposal | Activation language: ENTER to click uses pointer language for a keyboard action. | Use Enter to activate. |
| tabbing-04 | P2 | proposal | Reverse navigation: Only forward Tab navigation is explained. | Add Shift+Tab for reverse navigation if supported. |
| tabbing-05 | P2 | proposal | Initial focus framing: No focused option is shown in this initial screenshot. | Add a caption explaining that focus appears after keyboard navigation begins. |
| tabbing-06 | P2 | proposal | Focus evidence: All four button borders look alike in this state. | Add a companion baseline with one keyboard-focused option. |
| tabbing-07 | P2 | proposal | Option identities: OPTION A through OPTION D name no concrete action. | Add a short demo note that each activation increments the shared counter. |
| tabbing-08 | P2 | proposal | Counter label: Total Clicks counts keyboard activations under a mouse-specific name. | Rename it Total activations. |
| tabbing-09 | P2 | proposal | Counter feedback: The counter does not identify which option was activated. | Add Last activated: none beside the count. |
| tabbing-10 | P2 | proposal | Tab order: The vertical order is visible but not explicitly numbered as focus order. | Add small 1 to 4 order badges outside the interactive buttons. |
| tabbing-11 | P2 | proposal | Color roles: Magenta, cyan, lime and amber are unexplained. | Add a note that colors test focus visibility across different backgrounds. |
| tabbing-12 | P3 | proposal | Focus-ring specification: The screenshot does not document the configured yellow focus-ring appearance. | Show a noninteractive ring swatch with thickness and offset in a small legend. |
| tabbing-13 | P2 | proposal | Reset state: No reset action is visible for the activation counter. | Add Reset count outside the demonstrated four-option order or label its tab-order role. |
| tabbing-14 | P3 | proposal | Panel border: The purple outer frame has several close parallel outlines. | Simplify the decorative frame so the keyboard ring is easier to distinguish in active captures. |
| tabbing-15 | P2 | proposal | Expected outcome: The page has no explicit success statement for traversing all options. | Add an expected sequence line A, B, C, D and back according to the actual traversal rules. |

### Additional capture: tabbing

[Image](../../baseline_screenshots/tabbing.png)

Current capture removes initial Option A focus ring. The old image is useful separate evidence of the yellow-and-white keyboard focus treatment. The four colored choices remain otherwise unchanged.

## text

[Current baseline](../../screenshot-baselines/screens/text_720p.png) · [Visual review](../../design-audit.html#screen=text)

Baseline SHA-256: `47ec135185d09029d5db399b030d9f8141857693b38b3c6b5ecceb0faa28b649`

The typography specimens deliberately vary typeface, size, tracking and color. Twenty proposals improve their documentation and comparison without treating the intentional styling as a defect. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| text-01 | P2 | proposal | Font families: Serif headings and handwritten body samples appear without font names. | Label each specimen's font family. |
| text-02 | P2 | proposal | Display specimen: Display Text has no size annotation. | Show its point or pixel size beside the specimen. |
| text-03 | P2 | proposal | Headline specimen: Headline Style lacks its size and weight. | Add a small metadata row for size and weight. |
| text-04 | P2 | proposal | Subheading specimen: Subheading Text is only differentiated visually. | Label the intended semantic role and size. |
| text-05 | P2 | proposal | Body sample copy: The sentence claims excellent legibility instead of exposing useful glyphs. | Use a neutral specimen containing uppercase, lowercase, numbers and punctuation. |
| text-06 | P2 | proposal | Body line measure: The wrapping sample has no content-width annotation. | Show its line width and line-height values. |
| text-07 | P2 | proposal | Caption specimen: Caption & metadata is very small and also serves as its own description. | Place readable explanatory metadata outside the small-text specimen. |
| text-08 | P2 | proposal | Tracking samples: TIGHT TRACKING, WIDE TRACKING and ULTRA SPACED use different words. | Repeat the same phrase in all three samples for direct comparison. |
| text-09 | P2 | proposal | Tracking values: The three tracking labels provide no numeric letter spacing. | Display exact spacing values in the metadata column. |
| text-10 | P2 | proposal | Gold color sample: Primary Gold Text has no color-token value. | Add its token and hex value alongside the card. |
| text-11 | P2 | proposal | Cream color sample: Light Cream Text has no contrast measurement context. | Add foreground/background swatches and a measured contrast value in external metadata. |
| text-12 | P2 | proposal | Silver color sample: Silver Accent Text resembles the nearby muted treatment. | Show their color values side by side to explain the subtle difference. |
| text-13 | P2 | proposal | Muted color sample: Muted Secondary Text is presented without a usage example. | Add a short secondary-metadata context sample outside the color specimen. |
| text-14 | P2 | proposal | Accent color sample: Accent Color Highlight uses a reddish color without naming the token. | Label the token and intended accent role. |
| text-15 | P3 | proposal | Color card labels: All five cards mix the color name with the sample sentence. | Use identical sample text and move color labels outside the cards. |
| text-16 | P2 | proposal | Hierarchy comparison: Size examples are stacked without baseline or cap-height guides. | Add optional faint baseline guides for visual comparison. |
| text-17 | P3 | proposal | Section heading hierarchy: Letter Spacing uses the same treatment as Text Size Hierarchy but sits within its column. | Make the specimen grouping explicit with separate section spacing. |
| text-18 | P2 | proposal | Footer sentence: Elegant typography creates visual hierarchy is a general claim. | Replace it with the active font families and rendering scale. |
| text-19 | P3 | proposal | Gold underline: The heading underline is unlabeled decoration on a specimen-heavy page. | Reduce its prominence so sample rules and guides have a distinct role. |
| text-20 | P2 | proposal | Baseline rendering context: The page does not state resolution or scale for the type samples. | Add 1280 × 720 and the effective UI scale in a metadata footer. |

### Additional capture: text

[Image](../../baseline_screenshots/text.png)

Pixel-identical. Serif display specimens contrast with handwritten body and caption samples. Current typography-review findings apply without assuming the mixed families are accidental.

## text input

[Reviewed image](../../docs/design-audit/originals/text_input_720p.png) · [Current baseline](../../screenshot-baselines/screens/text_input_720p.png) · [Visual review](../../design-audit.html#screen=text_input)

Baseline SHA-256: `2442effdca504a050be7f25a5b1e767334aadc34efe700601042d8ea2c194081`

The form fits, but its labels are unusually small and its profile context mixes ordinary fields with test-only specimens. Twenty-two findings are supported by the initial screenshot. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| text_input-01 | P2 | defect | Field labels: Username, Email, Password and other labels are extremely small. | Increase actual label size to at least 12 to 14 px. |
| text_input-02 | P2 | defect | Footer shortcuts: Tab / Ctrl+A/C/V/X is barely legible. | Increase shortcut text and spell out the associated actions. |
| text_input-03 | P2 | proposal | Page subtitle: Update your account information is tiny and faint. | Increase its size and contrast below Edit Profile. |
| text_input-04 | P2 | proposal | Profile context: Search, Readonly Field and Disabled Field do not belong to a coherent edit-profile form. | Group those controls under a separate Input states demo section. |
| text_input-05 | P2 | proposal | Bio field: Bio is a single short line despite inviting personal description. | Use a multiline specimen or label it Short bio. |
| text_input-06 | P2 | proposal | Email placeholder: user@example.com can look like a prefilled address. | Use Enter email address and show example formatting as helper text. |
| text_input-07 | P2 | proposal | Username guidance: Enter username gives no visible format or length guidance. | Add concise requirements beneath the label if the form validates them. |
| text_input-08 | P2 | proposal | Password context: A password field is mixed into profile editing without naming the operation. | Use New password and clarify whether leaving it blank preserves the current password. |
| text_input-09 | P2 | proposal | Show password control: Show is small in a narrow gray segment. | Increase label size and use a clear show-password icon with sufficient inset. |
| text_input-10 | P2 | proposal | Password segment border: The Show segment has a different edge treatment from the input. | Unify the shared outer border and use a clean internal divider. |
| text_input-11 | P2 | proposal | Search affordance: Type to search... has no leading search icon. | Add a recognizable search icon and identify what is searched. |
| text_input-12 | P2 | proposal | Read-only specimen: readonly text here is a placeholder-like sample. | Use a realistic fixed value and a Read-only badge. |
| text_input-13 | P2 | proposal | Disabled specimen: The dim text is deliberately disabled but has no reason caption. | Explain why this sample is disabled outside the specimen. |
| text_input-14 | P2 | proposal | Input boundaries: Several field edges have broken-looking corner joins. | Use consistent border joins and document any intentional corner-radius variation. |
| text_input-15 | P2 | proposal | Save label size: Save is tiny inside a strong yellow button. | Increase action text size while preserving the primary color. |
| text_input-16 | P2 | proposal | Cancel label size: Cancel is tiny inside its outlined button. | Match the Save label size and vertical alignment. |
| text_input-17 | P2 | proposal | Footer order: The shortcut hint and action buttons share a sparse line without clear grouping. | Separate input help from the action group with consistent footer spacing. |
| text_input-18 | P2 | proposal | Form status: No visible status explains whether this is unsaved sample data. | Add a small Demo form or No changes status near the actions. |
| text_input-19 | P3 | proposal | Title hierarchy: Edit Profile is only slightly larger than input text. | Increase title size so it clearly leads the form. |
| text_input-20 | P2 | proposal | Optional fields: Username, Email, Password and Bio have no required/optional distinction. | Label optional fields or add a concise required-fields note. |
| text_input-21 | P3 | proposal | Panel balance: The tall narrow panel leaves broad black margins. | Widen the panel modestly to accommodate readable labels and helper text. |
| text_input-22 | P2 | proposal | Initial blank form: Edit Profile contains placeholders rather than visible existing account data. | Populate a clearly labeled sample profile or rename the screen Create profile. |

## text input lab

[Reviewed image](../../docs/design-audit/originals/text_input_lab_720p.png) · [Current baseline](../../screenshot-baselines/screens/text_input_lab_720p.png) · [Visual review](../../design-audit.html#screen=text_input_lab)

Baseline SHA-256: `f2ab6e280cf18132eccf2ac281500a67da3e9dc1e776ccaf71a564b2479aad3c`

The selected hello world text and the clipped overflow field are intentional internal-state fixtures. Sixteen proposals improve state readability and reproducibility. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| text_input_lab-01 | P2 | proposal | Primary field label: The first input has no local label. | Label it Selection and caret specimen. |
| text_input_lab-02 | P2 | proposal | State group: text, cursor, selection, focused and scroll_x sit directly on the black page. | Place them in a compact Inspector panel with aligned value columns. |
| text_input_lab-03 | P2 | proposal | Cursor index: cursor: 11 does not explain index units. | Label it Caret character index and state the indexing convention. |
| text_input_lab-04 | P2 | proposal | Selection range: selection: 0..11 does not explain endpoint semantics. | Show Start 0, End 11 and note whether end is exclusive. |
| text_input_lab-05 | P2 | proposal | Selection length: The full phrase is selected but no count is shown. | Add Selected characters: 11. |
| text_input_lab-06 | P2 | proposal | Horizontal scroll: scroll_x: 0.0 lacks units. | Show Horizontal scroll: 0.0 px. |
| text_input_lab-07 | P2 | proposal | Focus readout: focused: yes is embedded among numeric data. | Use a distinct Focused badge next to the inspected field name. |
| text_input_lab-08 | P2 | proposal | Inspector association: Two inputs are visible but only one state dump appears. | Label the inspector's target or show a separate dump for the overflow field. |
| text_input_lab-09 | P2 | proposal | Overflow source text: The lower text is clipped so its full source cannot be read. | Show the full source string in an external read-only caption. |
| text_input_lab-10 | P2 | proposal | Overflow dimensions: The lower field has no width annotation. | Label its exact viewport width and text width. |
| text_input_lab-11 | P2 | proposal | Expected clipping: The instruction says text must stay inside the box without marking that boundary. | Add external edge ticks at the field's clip limits. |
| text_input_lab-12 | P2 | proposal | Initial selection: The first field starts fully selected with no explanation. | Add a caption Initial state: all text selected. |
| text_input_lab-13 | P2 | proposal | Test procedure: The page gives no sequence for observing caret, selection and scroll changes. | Add a short keyboard procedure beneath the inspector. |
| text_input_lab-14 | P2 | proposal | Reset specimen: No control restores hello world and the initial selection. | Add Reset fixture outside the tested fields. |
| text_input_lab-15 | P3 | proposal | Empty right region: Most of the screen is unused. | Use a second column for expected outcomes and event history. |
| text_input_lab-16 | P2 | proposal | Action evidence: The state dump shows only current values. | Add a small last-action label so captures can be tied to a reproducible input step. |

## text input sizes

[Reviewed image](../../docs/design-audit/originals/text_input_sizes_720p.png) · [Current baseline](../../screenshot-baselines/screens/text_input_sizes_720p.png) · [Visual review](../../design-audit.html#screen=text_input_sizes)

Baseline SHA-256: `c00174f90736840d5e4d5f79b10652b8a9a044bf324ca611f1776df7000798e4`

The seven input heights intentionally test caret and selection scaling. Sixteen proposals improve comparison; the small fields themselves should remain as test cases. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| text_input_sizes-01 | P2 | defect | Height labels: 24px through 100px are extremely small. | Increase external dimension labels to readable 12 px text. |
| text_input_sizes-02 | P2 | proposal | Subtitle: Cursor and selection scale with field height is very small. | Increase explanatory text to 13 px actual size. |
| text_input_sizes-03 | P2 | proposal | Initial content: Every field is empty and displays a placeholder. | Prefill matching sample text in a companion state to expose selection height. |
| text_input_sizes-04 | P2 | proposal | Caret comparison: Only the first field shows a caret in this capture. | Add dedicated focus captures for each height rather than implying all carets can appear together. |
| text_input_sizes-05 | P2 | proposal | Selection comparison: No selected range is visible. | Add a companion state with a consistent selected word at each target height. |
| text_input_sizes-06 | P2 | proposal | Dimension notation: Labels use 24px with no space. | Use a consistent 24 px notation. |
| text_input_sizes-07 | P2 | proposal | Column identification: The height labels have no heading. | Add Height above the label column. |
| text_input_sizes-08 | P2 | proposal | Width metadata: All fields appear equal width but the width is undocumented. | Show a shared field-width caption above the stack. |
| text_input_sizes-09 | P2 | proposal | Font metadata: Placeholder text appears similar in size despite changing field heights. | Document the font size beside the common width caption. |
| text_input_sizes-10 | P2 | proposal | Padding growth: Larger fields show progressively larger left insets. | Annotate the padding rule so it can be checked independently of field height. |
| text_input_sizes-11 | P2 | proposal | Vertical centering: The specimen lacks guides for checking text centering. | Add optional midline guides outside the active input rendering. |
| text_input_sizes-12 | P2 | proposal | Height ruler: The difference between 24 and 32 px is hard to assess precisely. | Add a thin external height ruler beside each specimen. |
| text_input_sizes-13 | P3 | proposal | Row spacing: The gaps between fields are tight compared with the large last field. | Use equal labeled specimen gutters while preserving exact field heights. |
| text_input_sizes-14 | P2 | proposal | Small-input context: The 24 and 32 px specimens could look like recommended product controls. | Label them compact-size test cases rather than default form guidance. |
| text_input_sizes-15 | P2 | proposal | Reset content: No reset action returns all fields to comparable text. | Add a shared Reset sample text control outside the stack. |
| text_input_sizes-16 | P3 | proposal | Bottom space: A large blank area remains below the 100 px field. | Use it for caret, selection and padding measurements. |

## text overflow

[Current baseline](../../screenshot-baselines/screens/text_overflow_720p.png) · [Visual review](../../design-audit.html#screen=text_overflow)

Baseline SHA-256: `a528046ab0491541c4ebf55fc1b95520fbbffed95717deca72563ca56cef05d0`

Clipping and shrinking samples are intentional. Twenty proposals improve the debug legend and comparison; the screenshot alone does not establish that the overflow detector is wrong. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| text_overflow-01 | P2 | proposal | Debug instruction: The note says red corners appear, while entire sample borders are red. | Add a zoomed marker key distinguishing debug corners from specimen borders. |
| text_overflow-02 | P2 | proposal | Good/problem colors: Green and red headings carry the main classification. | Add explicit Pass specimen and Overflow specimen badges. |
| text_overflow-03 | P2 | proposal | Height specimen: Height is too small! appears to fit within its red frame. | Annotate requested font size, measured text height and available height to explain the case. |
| text_overflow-04 | P2 | proposal | Tiny box annotation: Small 44x44 uses a textual arrow and compact notation. | Use a clean callout line and 44 × 44 px label. |
| text_overflow-05 | P2 | proposal | First overflow text: This is way too ... is truncated without the full source beside it. | Show the source string in external metadata. |
| text_overflow-06 | P2 | proposal | Narrow overflow case: Long text i... has no dimension annotation. | Label its width and height next to the specimen. |
| text_overflow-07 | P2 | proposal | Ellipsis comparison: The solution samples are wider than the nearby failing examples. | Add matched-width before/after specimens so ellipsis is the isolated change. |
| text_overflow-08 | P2 | proposal | Ellipsis label: Solution: Ellipsis Truncation can imply every overflow should truncate. | Rename it Ellipsis behavior and state when wrapping is preferable. |
| text_overflow-09 | P2 | proposal | OK specimen: The small green OK box has no dimensions shown. | Annotate its width, height and text size. |
| text_overflow-10 | P2 | proposal | Large fit specimen: This text fits perfectly shows no measurable available space. | Add text bounds and container bounds in a spec caption. |
| text_overflow-11 | P2 | proposal | ABC sequence: Five shrinking ABC boxes have no individual size labels. | Label each box's dimensions beneath it. |
| text_overflow-12 | P2 | proposal | ABC font changes: The ABC font shrinks across the sequence as well as the boxes. | Display the font size per sample so two variables are explicit. |
| text_overflow-13 | P2 | proposal | Minimum-size note: The note links touch target size to text overflow warnings without distinction. | Separate touch-target guidance from text-measurement behavior. |
| text_overflow-14 | P2 | proposal | Debug mode state: Debug mode is ON is embedded in a long green sentence. | Use a small ON badge next to a Debug overlay label. |
| text_overflow-15 | P3 | proposal | Title scale: Text Overflow Debug Demo is large relative to tiny diagnostics. | Reduce title footprint and spend space on readable metadata. |
| text_overflow-16 | P2 | proposal | Specimen identifiers: Individual examples have no stable IDs. | Add small case IDs for screenshot-to-test mapping. |
| text_overflow-17 | P2 | proposal | Expected warning: Red-bordered examples have no expected-warning text. | Add Expected: overflow or Expected: fits under each case. |
| text_overflow-18 | P3 | proposal | Column alignment: The two columns begin at different specimen baselines. | Align the first comparison rows while preserving test dimensions. |
| text_overflow-19 | P2 | proposal | Clip boundary: Truncated text and red borders are close enough to obscure corner markers. | Place debug annotations outside the measured container. |
| text_overflow-20 | P2 | proposal | Rendering scale: The page shows fixed-size claims without the effective scale. | Add the UI scale and capture resolution to the footer. |

### Additional capture: text_overflow

[Image](../../baseline_screenshots/text_overflow.png)

Pixel-identical. Red oversized and clipped samples are the subject of the demo. Improvements must target their surrounding explanation and distinguish expected warning examples from real failures.

## text shadow

[Current baseline](../../screenshot-baselines/screens/text_shadow_720p.png) · [Visual review](../../design-audit.html#screen=text_shadow)

Baseline SHA-256: `a3630e5e8b01270685a7fd9cd44562fa743a9c3e382299a52329ed1da84b6759`

The offset ladder and exaggerated shadows are intentional. Nineteen proposals improve specimen framing; the LIGHT captions also need corrected association and inset. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| text_shadow-01 | P2 | proposal | Page title shadow: Text Drop Shadow has a heavy offset shadow overlapping its letterforms. | Use a restrained title shadow and reserve extreme offsets for the labeled specimens. |
| text_shadow-02 | P2 | proposal | Subtitle placement: The subtitle starts at the far left while the title is centered. | Align it beneath the title or make it a clearly separate page introduction. |
| text_shadow-03 | P2 | proposal | No-shadow comparison: NO SHADOW and WITH SHADOW use different phrases. | Repeat the same text and move variant labels outside the specimens. |
| text_shadow-04 | P2 | proposal | Comparison offset: The WITH SHADOW specimen does not show its exact offset. | Add x/y offset and opacity metadata. |
| text_shadow-05 | P2 | proposal | Soft/hard comparison: SOFT and HARD change the words while comparing shadow presets. | Use identical specimen text with separate Soft and Hard captions. |
| text_shadow-06 | P2 | proposal | Soft-shadow metadata: The glow description omits blur and opacity values. | Show the preset's blur, offset and alpha parameters. |
| text_shadow-07 | P2 | proposal | Hard-shadow metadata: HARD has no parameter label. | Add its exact offset and opacity for comparison with Soft. |
| text_shadow-08 | P2 | proposal | Colored shadow: COLORED is cyan/red but no color values are shown. | Display both foreground and shadow color tokens. |
| text_shadow-09 | P2 | proposal | Combo specimen: COMBO mixes stroke and shadow without separate measurements. | Add stroke width and shadow offset in an external caption. |
| text_shadow-10 | P2 | defect | LIGHT caption placement: flat look and more depth! both sit beneath the left half of the panel. | Place one caption directly beneath each LIGHT specimen. |
| text_shadow-11 | P2 | defect | LIGHT bottom inset: The LIGHT captions nearly touch the dark panel's bottom edge. | Increase panel height or move captions up to leave 10 px bottom inset. |
| text_shadow-12 | P2 | proposal | Offset ladder heading: Shadow Offset Examples does not specify direction. | Label the ladder with the x/y offset convention. |
| text_shadow-13 | P2 | proposal | Offset labels: 1px through 10px are small at the far right. | Increase label size and use 1 px notation. |
| text_shadow-14 | P2 | proposal | Ladder spacing: Large blank horizontal gaps separate SHADOW from its numeric label. | Bring the values closer or add faint row guides. |
| text_shadow-15 | P2 | proposal | Ladder baseline: No guide shows the unshadowed glyph baseline. | Add optional baseline ticks outside the text. |
| text_shadow-16 | P2 | proposal | Ladder control: The ladder begins at 1 px without a 0 px reference. | Add an external zero-offset swatch for comparison. |
| text_shadow-17 | P3 | proposal | Description rhythm: Descriptions sit at varying distances from their examples. | Use consistent specimen-to-caption spacing. |
| text_shadow-18 | P2 | proposal | Bottom tip: The tip is far below the specimens in a nearly empty lower region. | Move it closer to the examples or use the region for parameter summaries. |
| text_shadow-19 | P2 | proposal | Font identity: The bold pixel font is not named. | Add the font family and size to the specimen metadata. |

### Additional capture: text_shadow

[Image](../../baseline_screenshots/text_shadow.png)

Pixel-identical. Heading shadow competes with the heading itself, and LIGHT captions sit on the lower panel edge. The 1px to 10px samples deliberately expose strong shadow offsets.

## text stroke

[Current baseline](../../screenshot-baselines/screens/text_stroke_720p.png) · [Visual review](../../design-audit.html#screen=text_stroke)

Baseline SHA-256: `609191a71324ec3d5a3b7fc5ff28b5384fa1c7be1b82883c5831a58870305653`

The thick outlines and white-on-light control are intentional. Twenty findings preserve those samples while correcting covered captions and adding measurable comparison metadata. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| text_stroke-01 | P1 | defect | WHITE captions: The invisible and visible! captions are covered by the bottom tip strip. | Move the tip down or the specimen up so both captions are fully readable. |
| text_stroke-02 | P2 | proposal | White control label: invisible describes text that is faintly visible and frames the example as absolute. | Use Low contrast and Outlined for more precise captions. |
| text_stroke-03 | P2 | proposal | No-stroke comparison: NO STROKE and WITH STROKE use different strings. | Repeat the same specimen text and place variant labels outside it. |
| text_stroke-04 | P2 | proposal | Comparison width: The dark outline behind WITH STROKE resembles a rectangular backing. | Add an enlarged glyph detail to distinguish stroke expansion from a background fill. |
| text_stroke-05 | P2 | proposal | Bold specimen: BOLD uses an 8 px stroke but lacks the font size. | Show font size alongside stroke width to explain relative thickness. |
| text_stroke-06 | P2 | proposal | Contrast colors: CONTRAST is cyan/red without color token metadata. | Display foreground and outline color values in a separate caption. |
| text_stroke-07 | P2 | proposal | Glow wording: GLOW uses a hard bright outline while the caption calls it a glow. | Label it Bright outline or add a separate blurred glow comparison. |
| text_stroke-08 | P2 | proposal | Thickness ladder baseline: The ladder starts at 2 px with no zero-stroke control. | Add a zero-stroke reference above the ladder. |
| text_stroke-09 | P2 | proposal | Thickness labels: 2px through 10px use small distant labels. | Increase label size and use spaced px notation. |
| text_stroke-10 | P2 | proposal | Ladder row alignment: The text and thickness descriptions are separated by large blank gaps. | Align them within clear specimen rows. |
| text_stroke-11 | P2 | proposal | Ten-pixel outline: The 10 px sample has visible chunky artifacts that are part of the extreme case. | Label it Extreme thickness specimen and provide a close-up for assessment. |
| text_stroke-12 | P2 | proposal | Stroke algorithm context: The different outline edges have no rendering-method explanation. | Add a short metadata note identifying the stroke method under test. |
| text_stroke-13 | P2 | proposal | Font identity: The pixel-like display font is unnamed. | Label BlackOpsOne and its sample size. |
| text_stroke-14 | P2 | proposal | Title effect: The title also has a stroke but no specification. | Document its settings or use a neutral title to isolate the specimen effects. |
| text_stroke-15 | P3 | proposal | Subtitle size: The subtitle is small and subdued compared with the large effect samples. | Increase its readable size modestly. |
| text_stroke-16 | P2 | proposal | White panel metadata: The white-on-light panel does not state its background color. | Add background and foreground values outside the control panel. |
| text_stroke-17 | P2 | proposal | Caption alignment: Descriptions run across varying widths under the left specimens. | Use a consistent left column and text measure for metadata. |
| text_stroke-18 | P2 | proposal | Thickness advice: The tip gives absolute widths without relating them to font size. | Express guidance as a stroke-to-font-size ratio or include the reference font size. |
| text_stroke-19 | P3 | proposal | Case numbering: The effect cases have names but no stable identifiers. | Add small IDs for comparison across captures. |
| text_stroke-20 | P2 | proposal | Rendering scale: Pixel-width labels do not show whether UI scaling is applied. | Add capture scale and effective stroke units in the footer. |

### Additional capture: text_stroke

[Image](../../baseline_screenshots/text_stroke.png)

Pixel-identical. The invisible/visible captions beneath the WHITE sample are covered by the tip strip. Chunky 8px and 10px outlines are intentional stroke specimens.

## themes

[Current baseline](../../screenshot-baselines/screens/themes_720p.png) · [Visual review](../../design-audit.html#screen=themes)

Baseline SHA-256: `18190e9f97324b024987873c8fe2ac8b0708c4fede764e1ae32300f4b424895b`

Ocean Navy intentionally recolors the preview. Twenty-two findings improve token identification and specimen clarity while preserving the palette choices. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| themes-01 | P2 | proposal | Checkbox preview: The checkbox preview uses a V check indicator inside a wide blue row. | For a more familiar checkbox presentation, place the check inside a visible square beside its label. |
| themes-02 | P2 | proposal | Button labels: P, S, A and D are unexplained one-letter labels. | Expand them to Primary, Secondary, Accent and Disabled. |
| themes-03 | P2 | proposal | Selected theme marker: Ocean Navy is wrapped in greater-than and less-than text. | Use a checkmark or side marker with readable equal padding. |
| themes-04 | P2 | proposal | Active-theme banner: Active: Ocean Navy resembles another gold action button. | Style it as a status badge rather than a control. |
| themes-05 | P2 | proposal | Theme selection header: Select Theme is styled as a blue button. | Use a plain section heading so it is not mistaken for an action. |
| themes-06 | P2 | proposal | Slider preview: The wide green bar with blue thumb has no label or value. | Add Slider and its current numeric value. |
| themes-07 | P2 | proposal | Slider purpose: The oversized rectangular thumb does not explain its style configuration. | Add a specimen caption with thumb size and track size. |
| themes-08 | P2 | proposal | Toggle label: Toggle is centered far from the right-side switch. | Align label left and switch right with a consistent setting-row structure. |
| themes-09 | P2 | proposal | Toggle state: The gray switch has no visible On/Off text. | Add an explicit state value. |
| themes-10 | P2 | proposal | Shadow specimens: Hard, Soft and Accent also change background color. | Label shadow type separately and add a same-color comparison for shadow strength. |
| themes-11 | P2 | proposal | Hard card text: Hard touches the left edge of its blue card. | Add consistent inner padding to all three shadow cards. |
| themes-12 | P2 | proposal | Text swatches: Primary Text, Secondary Text and Surface Text touch their card edges. | Inset all text swatches equally. |
| themes-13 | P2 | proposal | Surface swatch: Surface Text sits on a nearly matching navy card. | Add a neutral external border and token metadata without changing the specimen colors. |
| themes-14 | P2 | proposal | Color values: The preview contains many role colors with no resolved values. | Add a token table showing foreground and background hex values. |
| themes-15 | P2 | proposal | Separator: A green horizontal rule appears without a label. | Identify it as the separator specimen and show its token. |
| themes-16 | P2 | proposal | Progress state: Loading: 72% is tiny inside the blue fill. | Place a readable progress label above the bar. |
| themes-17 | P2 | proposal | Progress semantics: The remaining portion is green, which can look like completion too. | Add an external legend for fill and track tokens rather than silently replacing the theme. |
| themes-18 | P2 | proposal | Preview title: Component Preview is a small label in a full-width blue bar. | Increase its hierarchy and identify the active theme beside it. |
| themes-19 | P3 | proposal | Left rail space: The theme list leaves a large unused lower region. | Use it for active-theme metadata or a compact palette summary. |
| themes-20 | P2 | proposal | Theme comparison: Only the selected theme is shown, making differences hard to review. | Add named companion captures for all five themes. |
| themes-21 | P2 | proposal | Disabled specimen: D is dim but has no external explanation. | Add a Disabled state caption outside the intentionally dim control. |
| themes-22 | P2 | proposal | Theme effect scope: The page does not explain that switching a theme recolors all examples. | Add a concise instruction above the preview. |

### Additional capture: themes

[Image](../../baseline_screenshots/themes.png)

Current capture removes Cozy Kraft focus outline while Ocean Navy remains the active theme in both. P/S/A/D abbreviations and the unlabeled slider remain difficult to interpret.

## toasts

[Reviewed image](../../docs/design-audit/originals/toasts_720p.png) · [Current baseline](../../screenshot-baselines/screens/toasts_720p.png) · [Visual review](../../design-audit.html#screen=toasts)

Baseline SHA-256: `09e5c316980c035277c80cb1fcab44194c4a060f73cd2157f48d2bb62c0c024c`

The baseline contains launchers only. Twenty findings improve the launcher design and correct an interactive-undo claim that source shows is only a simulated counter notification. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| toasts-01 | P2 | defect | Interactive section claim: INTERACTIVE TOASTS and With Undo Action promise an embedded action, but source only increments a counter and sends a toast. | Rename the section Simulated action feedback or implement an actual undo action in the toast. |
| toasts-02 | P2 | defect | Undo counter label: Undos: 0 labels launcher activations as completed undos. | Rename it Notifications sent unless actual undo operations are introduced. |
| toasts-03 | P2 | defect | Title inset: Toast Notifications starts flush against the title bar's left edge. | Add 16 px inner padding. |
| toasts-04 | P2 | defect | Quick duration label: Quick displays 1s nearly touches both sides of its button. | Add at least 12 px horizontal padding. |
| toasts-05 | P2 | defect | Long duration label: Long displays 10s fills almost the entire button width. | Increase button width or shorten the label to Long, 10 s. |
| toasts-06 | P2 | proposal | Success color: Success Toast is purple without an external severity key. | Add severity icons and text labels so the theme color is not the sole cue. |
| toasts-07 | P2 | proposal | Warning color: Warning Toast is magenta and close to other accents. | Use a warning icon in the launcher and toast specimen. |
| toasts-08 | P2 | proposal | Error launcher: Error Toast uses red but no error symbol. | Add a consistent error icon aligned with the other severity controls. |
| toasts-09 | P2 | proposal | Info launcher: Info Toast has no preview of the resulting message. | Add a short example-message caption below the launcher. |
| toasts-10 | P2 | proposal | Duration group title: DURATION & SPAM uses informal stress-test language. | Rename it Duration and stacking tests. |
| toasts-11 | P2 | proposal | Spam action: Spam x5 (!) obscures the exact outcome. | Use Show five toasts and describe stacking behavior below it. |
| toasts-12 | P2 | proposal | Custom color control: Custom Color uses an orange fill without naming the configured color. | Add an external swatch value or color label. |
| toasts-13 | P2 | proposal | Launcher sizing: Simple toast buttons have uneven widths determined by their labels. | Use an equal-width severity grid for easier comparison. |
| toasts-14 | P2 | proposal | Typography scale: Large letter-spaced launcher labels crowd their backgrounds. | Reduce tracking and use a consistent readable button size. |
| toasts-15 | P2 | proposal | Empty toast area: No toast is visible anywhere in the initial capture. | Add a dedicated labeled preview region or companion shown-toast captures. |
| toasts-16 | P2 | proposal | Toast placement: The launcher page does not state where notifications appear. | Show a small placement diagram in the unused right side of a section. |
| toasts-17 | P2 | proposal | Toast lifetime: Only quick and long launchers show duration values. | Label the default duration used by the simple severity examples. |
| toasts-18 | P2 | proposal | Clear action: No visible action dismisses the stacking test. | Add Clear toasts outside the fixture previews. |
| toasts-19 | P2 | proposal | Status reporting: Only the undo simulation has a counter. | Add a shared Last notification and Active count readout. |
| toasts-20 | P3 | proposal | Section space: Wide cards leave substantial unused right space. | Use that space for concise expected-result and duration metadata. |

## toggle switches

[Current baseline](../../screenshot-baselines/screens/toggle_switches_720p.png) · [Visual review](../../design-audit.html#screen=toggle_switches)

Baseline SHA-256: `c61e6fd9f63b8ee366ead5e8a6db9cd8072803971d14000d0e53f308e7e4e823`

Pill toggles and checkbox variants are intentional. Twenty proposals improve alignment, state labels and disabled-example framing without treating disabled colors as failures. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| toggle_switches-01 | P2 | proposal | Page title: Settings hides that this is a component comparison. | Use Toggle and checkbox settings demo. |
| toggle_switches-02 | P2 | proposal | General labels: Notifications, Sound Effects and Vibration are centered far from the left edge. | Left-align labels with the Preferences rows. |
| toggle_switches-03 | P2 | proposal | Preference label inset: Dark Mode, Auto-Save and Cloud Sync touch the row left edge. | Add 12 px inner padding. |
| toggle_switches-04 | P2 | proposal | Preference indicators: The [x] and [ ] glyphs float near the row center. | Align them in the same right-side control column as the switches. |
| toggle_switches-05 | P2 | proposal | Checkbox rendering: Bracketed text resembles terminal output rather than a circular checkbox specimen. | Use a visible checkbox shape with a centered checkmark and retain a caption for its style. |
| toggle_switches-06 | P2 | proposal | On-state labels: Notifications and Vibration rely on magenta fill and thumb position. | Add On text beside the switch. |
| toggle_switches-07 | P2 | proposal | Off-state label: Sound Effects uses a gray track without an Off label. | Add Off text in the state column. |
| toggle_switches-08 | P2 | proposal | Section naming: General and Preferences do not identify the different control families. | Add Pill switches and Checkbox variants as secondary captions. |
| toggle_switches-09 | P2 | proposal | Disabled switch: Locked Setting ON shows a white thumb with an almost invisible track. | Keep the disabled specimen but label track and thumb states in external metadata. |
| toggle_switches-10 | P2 | proposal | Disabled reason: Locked Setting gives no reason for the lock. | Add a short sample explanation such as Managed by administrator. |
| toggle_switches-11 | P2 | proposal | Unavailable checkbox: Unavailable Option OFF has no reason caption. | Add an external Not available in this demo note. |
| toggle_switches-12 | P2 | proposal | Disabled text size: Disabled labels are small as well as dim. | Increase external captions while leaving disabled-state styling intact. |
| toggle_switches-13 | P2 | proposal | State vocabulary: ON/OFF appears only in disabled labels while enabled rows have no words. | Use consistent On and Off state text across all specimens. |
| toggle_switches-14 | P2 | proposal | Row geometry: General rows are square strips while Preferences rows are rounded. | Label the shape variation explicitly or use a common row shell around different controls. |
| toggle_switches-15 | P2 | proposal | Section labels: General, Preferences and Disabled are small and faint. | Increase their readable size and use a consistent heading weight. |
| toggle_switches-16 | P2 | proposal | Control association: Large gaps separate centered labels from the right-side pill controls. | Use a structured label/value row with predictable spacing. |
| toggle_switches-17 | P3 | proposal | Separator inset: Dividers align with the outer row edges but labels do not. | Align headings, labels and dividers to one inner grid. |
| toggle_switches-18 | P2 | proposal | Initial state summary: The mix of on and off states has no compact comparison legend. | Add a small state key outside the controls. |
| toggle_switches-19 | P2 | proposal | Reset defaults: No visible action restores the initial mixture of states. | Add Reset demo defaults below the specimen groups. |
| toggle_switches-20 | P3 | proposal | Panel margins: The main card begins at the viewport top with little title breathing room. | Add a modest top margin while preserving space for the disabled examples. |

### Additional capture: toggle_switches

[Image](../../baseline_screenshots/toggle_switches.png)

Current capture removes Notifications focus outline. General switches and Preferences bracket markers deliberately compare two representations. Locked Setting ON still loses its track against the dark disabled row.

## tooltip lab

[Current baseline](../../screenshot-baselines/screens/tooltip_lab_720p.png) · [Visual review](../../design-audit.html#screen=tooltip_lab)

Baseline SHA-256: `3da02ddc32405e3289d5458682bad2c56fbff920047f030c2b17f6375437ec9d`

Trigger positions deliberately exercise edge flipping and clamping. Sixteen proposals improve the test legend and capture coverage; no tooltip is open in this baseline. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| tooltip_lab-01 | P2 | proposal | Page identity: The top bar is only an instruction sentence. | Add Tooltip placement lab as the title. |
| tooltip_lab-02 | P2 | proposal | Instruction size: The hover-and-flip instruction is small. | Increase it to 13 px actual text. |
| tooltip_lab-03 | P2 | proposal | Header padding: The instruction starts almost at the bar's left edge. | Add a consistent 12 to 16 px text inset. |
| tooltip_lab-04 | P2 | proposal | Top-left case: top left does not say it is the normal-below placement control. | Add Normal below as an external case caption. |
| tooltip_lab-05 | P2 | proposal | Top-right case: top right does not identify horizontal clamping. | Add Clamp to right edge as an external caption. |
| tooltip_lab-06 | P2 | proposal | Bottom-left case: bottom left does not name the expected flip. | Add Flip above as an external caption. |
| tooltip_lab-07 | P2 | proposal | Bottom-right case: bottom right does not name its combined constraints. | Add Flip above and clamp as an external caption. |
| tooltip_lab-08 | P2 | proposal | Middle case: middle does not reveal that it tests long tooltip text. | Rename it Long content or add that caption. |
| tooltip_lab-09 | P2 | proposal | Instant case: instant gives no numeric delay. | Label it Instant, 0 ms. |
| tooltip_lab-10 | P2 | proposal | Default delay: Other triggers do not show their 0.4 s delay. | Add Default delay: 400 ms to the legend. |
| tooltip_lab-11 | P2 | proposal | Expected tooltip content: No tooltip text is visible in the initial state. | Add a compact external case table listing trigger, message and expected placement. |
| tooltip_lab-12 | P2 | proposal | Open-state evidence: The baseline cannot show whether tooltip boxes stay within bounds. | Capture one shown-tooltip state per edge case. |
| tooltip_lab-13 | P2 | proposal | Viewport bounds: The dark field has no visible safe-area guide. | Add an optional edge guide for checking clamping without moving the triggers. |
| tooltip_lab-14 | P2 | proposal | Trigger geometry: Button dimensions and edge distances are undocumented. | Show them in a small fixture metadata panel. |
| tooltip_lab-15 | P2 | proposal | Active-case status: There is no readout identifying the currently hovered specimen. | Add a read-only Active case status in the header. |
| tooltip_lab-16 | P3 | proposal | Unused central space: Large blank regions are available between the fixed edge triggers. | Use a compact central legend without changing trigger positions. |

### Additional capture: tooltip_lab_idle

[Image](../../baseline_screenshots/tooltip_lab_idle.png)

Current capture removes top-left focus outline. Both idle views show six triggers and no tooltip. Initial focus absence is not proof that keyboard tooltips fail.

### Additional capture: tooltip_lab_shown

[Image](../../baseline_screenshots/tooltip_lab_shown.png)

Additional interaction state. The instant trigger is highlighted and a narrow tooltip reads delay 0, shows the moment you arrive. This tooltip is absent from the current idle baseline by design.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| tooltip_lab_shown-L01 | P2 | proposal | Instant tooltip copy: The shown tooltip sentence is much smaller than the instant trigger label. | Use at least the readable caption size used by the trigger descriptions; verify the full sentence at 1280×720. |
| tooltip_lab_shown-L02 | P2 | proposal | Instant tooltip padding: The tooltip sentence almost fills its narrow outlined rectangle. | Add horizontal and vertical breathing room around the instant-tooltip sentence. |
| tooltip_lab_shown-L03 | P3 | proposal | Tooltip border: A red outline surrounds a normal informational tooltip. | Use a neutral tooltip edge and reserve red for a documented diagnostic or warning state. |
| tooltip_lab_shown-L04 | P3 | proposal | Trigger-to-tooltip relationship: The instant tooltip appears immediately below the button without a visible anchor cue. | Add a small offset or anchor marker so the tooltip is visually attached to instant rather than a separate control. |

## tray

[Reviewed image](../../docs/design-audit/originals/tray_720p.png) · [Current baseline](../../screenshot-baselines/screens/tray_720p.png) · [Visual review](../../design-audit.html#screen=tray)

Baseline SHA-256: `55223ab764a9e003c08a9f331d999102f2f0d332c1ca6a26e222aa187a2b4f05`

The two tray orientations are visible. Sixteen proposals clarify navigation and feedback; the initial screenshot does not establish keyboard or focus failures. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| tray-01 | P2 | proposal | Page explanation: Tray Navigation does not explain what a tray changes about focus. | Add a sentence describing grouped arrow-key navigation. |
| tray-02 | P2 | proposal | Horizontal caption: Horizontal Tray Left/Right uses tiny text. | Increase the caption to readable 13 px actual size. |
| tray-03 | P2 | proposal | Vertical caption: Vertical Tray Up/Down is similarly small. | Match the horizontal caption's size and spacing. |
| tray-04 | P2 | proposal | Horizontal labels: H-Alpha, H-Beta and H-Gamma repeat an orientation prefix already in the heading. | Use Alpha, Beta and Gamma with a clearly labeled horizontal group. |
| tray-05 | P2 | proposal | Vertical labels: V-Alpha, V-Beta and V-Gamma repeat the heading. | Use the same choice labels as the horizontal comparison. |
| tray-06 | P2 | proposal | Color comparison: The horizontal and vertical groups use different color sets. | Use matching colors for corresponding choices or document the deliberate contrast test. |
| tray-07 | P2 | proposal | Tray boundary: The groups have no outline showing where tray ownership begins and ends. | Add subtle group frames outside the buttons. |
| tray-08 | P2 | proposal | Entry/exit guidance: Only arrow directions are shown. | Add Tab guidance for entering and leaving the tray according to actual behavior. |
| tray-09 | P2 | proposal | Activation guide: No text says how to activate the current tray item. | Add the supported activation key beside the navigation hints. |
| tray-10 | P2 | proposal | Selection feedback: No status shows the last activated item. | Add Last activated: none beneath the groups. |
| tray-11 | P2 | proposal | Active group feedback: The initial frame does not identify a keyboard-active tray. | Add a read-only Active tray status that updates during navigation. |
| tray-12 | P2 | proposal | Boundary behavior: The three-item groups do not explain wrapping at their ends. | Add a concise wrap/clamp note matched to implementation. |
| tray-13 | P2 | proposal | State coverage: Only the initial unfocused composition is shown. | Add companion captures with a focused item in each tray. |
| tray-14 | P3 | proposal | Bottom panel space: A large blank band remains beneath V-Gamma. | Use it for the key guide and status readouts. |
| tray-15 | P3 | proposal | Group spacing: The gap after the horizontal row is much larger than its caption gap. | Use consistent heading-to-group and group-to-group spacing. |
| tray-16 | P2 | proposal | Demo reset: There is no visible way to restore the initial selection state. | Add a small Reset demo action outside the tray examples. |

## virtual list lab

[Current baseline](../../screenshot-baselines/screens/virtual_list_lab_720p.png) · [Visual review](../../design-audit.html#screen=virtual_list_lab)

Baseline SHA-256: `ba1f0c8ef168650cbccfbde56c69c550e8a99f62e791f60d6ac1f8a27ab92ce1`

The 10,000-row count and short scrollbar thumb are intentional. Seventeen proposals make the virtualization claim observable without changing the large-list test. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| virtual_list_lab-01 | P2 | proposal | Instruction size: The entity-count explanation is tiny beneath the title. | Increase it to 13 px actual size. |
| virtual_list_lab-02 | P2 | proposal | Row identifiers: row 00000 through row 00019 are very small. | Increase row text size while retaining the fixed row-height test. |
| virtual_list_lab-03 | P2 | proposal | Row padding: Identifiers begin flush at the list's left edge. | Add 12 px text inset without changing the viewport width. |
| virtual_list_lab-04 | P2 | proposal | Entity-count claim: The instruction says entity count stays flat but no count is displayed. | Add a live rendered-entity count above the list. |
| virtual_list_lab-05 | P2 | proposal | Visible range: Twenty rows are visible with no summarized range. | Show Visible rows 0 to 19 of 10,000 outside the viewport. |
| virtual_list_lab-06 | P2 | proposal | Scroll position: The tiny thumb conveys little precise position information. | Add numeric offset and percentage readouts. |
| virtual_list_lab-07 | P2 | proposal | Total count formatting: 10000 appears without grouping separators. | Display 10,000 in descriptive text while keeping zero-padded row IDs. |
| virtual_list_lab-08 | P2 | proposal | Marked rows: Rows 00000, 00007 and 00014 say marked but have no separate visual marker. | Add a small marker icon or accent stripe alongside the text. |
| virtual_list_lab-09 | P2 | proposal | Marked rule: The reason every seventh row is marked is not explained. | Add a caption naming the marker interval. |
| virtual_list_lab-10 | P2 | proposal | Index convention: The first row is 00000 without an indexing note. | Label row IDs as zero-based. |
| virtual_list_lab-11 | P2 | proposal | Jump controls: Only manual scrolling is suggested for a very long list. | Add Start, Middle and End presets outside the viewport. |
| virtual_list_lab-12 | P2 | proposal | Jump-to-index: No direct location entry is visible. | Add a row-index field for reproducible offscreen-range captures. |
| virtual_list_lab-13 | P2 | proposal | Virtualization evidence: The initial frame cannot show entity reuse across distant ranges. | Add companion captures at a distant offset with the entity-count readout. |
| virtual_list_lab-14 | P2 | proposal | Viewport metrics: The fixed row height and viewport height are undocumented. | Show both values with the expected visible-row count. |
| virtual_list_lab-15 | P3 | proposal | Row schema: Each line combines an index and state in loose text. | Use aligned Index and State columns for easier scanning. |
| virtual_list_lab-16 | P2 | proposal | End-state framing: No indication describes the last row's expected index. | Add Last row: 09999 in the test metadata. |
| virtual_list_lab-17 | P3 | proposal | Unused row width: Most of every row is empty. | Use a compact content column with a subtle grid while preserving the full-width scroll region. |

## vstack showcase

[Reviewed image](../../docs/design-audit/originals/vstack_showcase_720p.png) · [Current baseline](../../screenshot-baselines/screens/vstack_showcase_720p.png) · [Visual review](../../design-audit.html#screen=vstack_showcase)

Baseline SHA-256: `90463f83044553876ff635b1be85f7b12bbd04ff10c42ce9b880680e956c5d01`

The stack positions and bright colors are deliberate layout specimens. Twenty-two proposals add readable labels and geometry guides without normalizing away the tested alignment differences. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| vstack_showcase-01 | P2 | defect | Top title inset: vstack() Showcase starts at the left edge of the cyan bar. | Add a 12 px title inset. |
| vstack_showcase-02 | P2 | proposal | Justification section: The top five cards have no shared JustifyContent heading. | Add a clear main-axis justification heading above the row. |
| vstack_showcase-03 | P2 | proposal | Top captions: Start, Center, End, Between and Around are tiny. | Increase their external label size to readable 12 px text. |
| vstack_showcase-04 | P2 | proposal | Between naming: Between abbreviates the actual spacing mode. | Use Space between in the label and show the API name in metadata. |
| vstack_showcase-05 | P2 | proposal | Around naming: Around omits the meaning of edge spacing. | Use Space around with a small edge-gap diagram. |
| vstack_showcase-06 | P2 | proposal | Main-axis direction: The top row demonstrates vertical placement without an axis arrow. | Add a vertical main-axis guide beside the specimens. |
| vstack_showcase-07 | P2 | proposal | Start specimen: The three blocks begin at the top but have no start guide. | Add an external top-edge marker. |
| vstack_showcase-08 | P2 | proposal | Center specimen: The centered group has no midpoint reference. | Add a faint external centerline tick. |
| vstack_showcase-09 | P2 | proposal | End specimen: The block group ends near the bottom without a baseline marker. | Add an external bottom-edge marker. |
| vstack_showcase-10 | P2 | proposal | Between gaps: The two interior gaps are not measured. | Annotate the equal interior gap size outside the blocks. |
| vstack_showcase-11 | P2 | proposal | Around gaps: The edge gaps and interior gaps are visually different but unexplained. | Label the expected 1:2 edge-to-interior spacing relationship. |
| vstack_showcase-12 | P2 | proposal | Block dimensions: Cyan, magenta and green blocks use different heights with no dimensions. | Add a shared legend for their fixed sizes. |
| vstack_showcase-13 | P2 | proposal | Cross-axis heading: AlignItems cross-axis is tiny in the green strip. | Increase heading size and reserve enough height for it. |
| vstack_showcase-14 | P2 | proposal | Cross-axis direction: The lower specimens do not show the horizontal axis. | Add a horizontal cross-axis arrow above the four cards. |
| vstack_showcase-15 | P2 | proposal | Stretch specimen: The blue blocks retain visibly different widths under Stretch. | Annotate their width constraints and add an auto-width control case to explain what stretch can affect. |
| vstack_showcase-16 | P2 | proposal | Lower specimen colors: Each alignment mode changes color as well as position. | Use matching specimen colors across modes or label color as decorative. |
| vstack_showcase-17 | P2 | proposal | Lower captions: Start, Center, Stretch and End blend into thin green bars. | Increase caption size and inner padding. |
| vstack_showcase-18 | P2 | proposal | Sidebar example: Sidebar + Content is a tiny label across a long blue bar. | Increase it and identify this as an applied layout example. |
| vstack_showcase-19 | P2 | proposal | Navigation labels: Home, Settings, Profile and Help are very small. | Increase label size without changing the sidebar layout structure. |
| vstack_showcase-20 | P2 | proposal | Content rows: Alerts, Activity and Stats touch their colored row edges. | Add 10 to 12 px inner text padding. |
| vstack_showcase-21 | P2 | proposal | Static example semantics: The navigation and content rows look like an application without behavior explanation. | Label them Static layout example unless they are interactive. |
| vstack_showcase-22 | P3 | proposal | Metadata space: The lower content pane has a large unused area. | Use it for the vstack default width/height rules and measured dimensions. |

## widget gap gallery

[Current baseline](../../screenshot-baselines/screens/widget_gap_gallery_720p.png) · [Visual review](../../design-audit.html#screen=widget_gap_gallery)

Baseline SHA-256: `8abe23cefb8f3c725a04bae2a8d600aa6a493f946535585ef2abe82d30c07b09`

The gallery covers several previously unused widgets. Twenty-three findings clarify API specimen labels, icon choices and grouping without inferring behavior from the initial state. Fewer than 25 are listed because further findings from this static state would duplicate these points or require unseen interaction evidence.

| ID | Priority | Type | Visible evidence | Proposed change |
|---|---|---|---|---|
| widget_gap_gallery-01 | P2 | proposal | Page title: Widgets that had no caller describes project history rather than the displayed features. | Use Widget integration gallery with the included component names beneath it. |
| widget_gap_gallery-02 | P2 | proposal | API headings: button_group, image and icon_row are terse code names. | Pair each API name with a plain-language specimen title. |
| widget_gap_gallery-03 | P2 | proposal | Button group boundaries: Select, Move, Rotate and Scale touch with rounded edges at every join. | Use clear segmented-group joins or uniform small gaps. |
| widget_gap_gallery-04 | P2 | proposal | Tool selection state: no tool picked yet is small and separated from the group. | Use a labeled Selected tool status directly beneath the buttons. |
| widget_gap_gallery-05 | P2 | proposal | Tool action meaning: Select is both a tool name and a generic action. | Add a short caption that the group chooses an editing tool. |
| widget_gap_gallery-06 | P2 | proposal | Pagination label: pagination is a small API name without content context. | Use Pagination, five pages as the visible section heading. |
| widget_gap_gallery-07 | P2 | proposal | Previous arrow: The previous arrow looks enabled while page 1 is selected. | Document wrap behavior or render a disabled boundary state if previous is unavailable. |
| widget_gap_gallery-08 | P2 | proposal | Selected page: Page 1 has several close white outlines. | Use one clear selected-state outline distinct from keyboard focus styling. |
| widget_gap_gallery-09 | P2 | proposal | Page status: page 1 of 5 is small beneath the controls. | Increase it and align it to the pagination group inset. |
| widget_gap_gallery-10 | P2 | proposal | Pagination width: The page buttons use inconsistent apparent widths beside narrow arrow controls. | Use equal numeral cells with balanced end-control widths. |
| widget_gap_gallery-11 | P2 | proposal | Spacer demo label: spacer pushes the tail to the edge states the effect but not the flexible region. | Add a faint external bracket over the space between head and tail. |
| widget_gap_gallery-12 | P2 | defect | Head label: head touches the left edge of its blue block. | Inset the label by 10 px. |
| widget_gap_gallery-13 | P2 | defect | Tail label: tail touches the left edge of its brown block. | Apply the same 10 px text inset. |
| widget_gap_gallery-14 | P2 | proposal | Spacer dimensions: Head and tail have no fixed-width metadata. | Show fixed item widths and remaining flexible width below the sample. |
| widget_gap_gallery-15 | P2 | proposal | Image specimens: Gear, star and trophy images have no individual names. | Add captions beneath each image tile. |
| widget_gap_gallery-16 | P2 | proposal | Image sizing: The three images have different visual mass inside equal tiles. | Document source dimensions and display size, and center their visible bounds consistently. |
| widget_gap_gallery-17 | P2 | proposal | Icon-row repetition: Three identical home icons reveal little about spritesheet cell selection. | Show three distinct labeled sprites from the same sheet. |
| widget_gap_gallery-18 | P2 | proposal | Icon-row spacing: The home icons lack a visible gap annotation. | Add the configured icon size and gap below the row. |
| widget_gap_gallery-19 | P2 | proposal | Save icon: Save uses a gear icon that usually represents settings. | Use a save icon or rename the action to Configure. |
| widget_gap_gallery-20 | P2 | proposal | Next icon: Next uses a star rather than a directional icon. | Use a right arrow or rename the action to Favorite. |
| widget_gap_gallery-21 | P2 | proposal | Icon-side comparison: Left and right icon placement also changes the icon and label. | Use the same icon and label in both positions for direct comparison. |
| widget_gap_gallery-22 | P3 | proposal | Section alignment: The left and right specimen sections use different vertical baselines. | Align corresponding rows or give each specimen a clearly bounded card. |
| widget_gap_gallery-23 | P2 | proposal | Unused lower panels: Both columns leave large blank regions below the final specimens. | Use them for concise dimensions, source asset names and expected-result notes. |
