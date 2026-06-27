# Ant Design Analysis

Source: https://ant.design/docs/spec/introduce/
GitHub: https://github.com/ant-design/ant-design

Ant Design is an enterprise-grade design system created by Ant Group
(Alibaba/Ant Financial). It's the most comprehensive design system in
terms of *documented design principles and patterns* — it goes far
beyond component APIs into layout theory, composition, motion psychology,
feedback taxonomy, and interaction design principles. This makes it
extremely valuable as an **audit resource**.

---

## 1. Design Values (4 Pillars)

### Natural
- Visual elements (layout, color, icons) should follow natural laws to
  reduce cognitive cost
- Interaction should match user expectations from the real world
- Sensory channels beyond vision (audio, haptics) can enrich experience

### Certain
- Reduce "cooperative entropy" — the more participants in a design, the
  more inconsistency creeps in
- **Keep restraint**: Don't decide before you understand; minimize elements
- **Object-oriented**: Abstract design rules as reusable "objects"
- **Modular**: Encapsulate complex parts behind limited interfaces
- **Consistency across products**: Same appearance and interaction patterns

### Meaningful
- **Meaning of result**: Clear goals, immediate feedback for every action
- **Meaning of process**: Moderate challenge, don't distract; let users
  enter "flow state"
- Don't add entities unless necessary

### Growing
- Product capabilities and user skills should grow together
- Help users discover value through progressive disclosure

**Audit applicability:** These four values can be used as high-level
evaluation criteria: "Does this UI feel natural?", "Is it consistent?",
"Is every element meaningful?", "Does it support user growth?"

---

## 2. Design Principles (Auditable Rules)

Ant Design defines **10 interaction design principles**. These are
concrete, checkable guidelines perfect for auditing:

### 2.1 Proximity
- Related items should be visually close; unrelated items should be far
- **Spacing formula**: `y = 8 + 8n` (where n >= 0)
  - Small spacing: 8px
  - Medium spacing: 16px
  - Large spacing: 24px
- Add visual guides (lines, backgrounds) to reinforce grouping

**Audit check:** "Are related controls grouped by proximity? Is spacing
consistent within groups vs between groups?"

### 2.2 Alignment
- Elements should share alignment edges to create visual order
- Text, form labels, and values should align to consistent columns
- Avoid ragged layouts where nothing lines up

**Audit check:** "Do elements share clear alignment edges? Are form
labels consistently aligned?"

### 2.3 Contrast
- Use contrast to establish hierarchy: size, color, weight differences
- Primary actions should be visually dominant
- Secondary/tertiary actions should recede
- Don't create competing focal points

**Audit check:** "Is there a clear visual hierarchy? Can the user
instantly identify the primary action? Are there competing elements?"

### 2.4 Repetition
- Repeat visual patterns to create consistency and rhythm
- Same types of elements should look the same across screens
- Once a pattern is established, don't break it without reason

**Audit check:** "Are similar elements styled consistently? Does the UI
have visual rhythm or is each section ad-hoc?"

### 2.5 Make It Direct
- "Where there is output, let there be input" (Alan Cooper)
- Prefer **in-place editing** over navigating to a separate edit screen
- Click-to-edit, inline editing, drag-and-drop over modal forms
- Three states for inline editing: browse, hover (invitation), edit

**Audit check:** "Can the user edit content directly or do they have to
navigate away? Are there unnecessary page transitions?"

### 2.6 Stay on the Page
- Minimize full-page transitions
- Use overlays, inline expansion, drawers instead of new pages
- Keep the user's context visible during sub-operations

**Audit check:** "Does this operation take the user away from their
context unnecessarily? Could it be done inline?"

### 2.7 Keep It Lightweight (Fitts's Law)
- **Fitts's Law**: Time to reach a target is proportional to distance
  and inversely proportional to target size
- Tools should be close to where users need them (contextual)
- Three progressive disclosure levels:
  1. **Always-visible tools** — for critical actions
  2. **Hover-reveal tools** — for secondary actions
  3. **Toggle-reveal tools** — for rare actions
- **Visible area != Clickable area**: Make hit targets larger than the
  visual element (especially for mobile/touch)

**Audit check:** "Are primary actions close and large? Are secondary
actions properly hidden? Is the clickable area at least as large as the
visual element?"

### 2.8 Provide an Invitation
- Rich interactions (drag-and-drop, inline editing) lack discoverability
- Use **signifiers** to indicate what's possible:
  - **Static invitations**: Text hints, blank slate prompts, incomplete
    state indicators, tours
  - **Dynamic invitations**: Hover affordances, inference-based
    suggestions, "more content" indicators

**Audit check:** "Can a new user discover all interactive features? Are
hidden interactions (drag, right-click, swipe) indicated?"

### 2.9 Use Transition
- Transitions maintain spatial context during state changes
- Four purposes:
  1. **Maintain context** while changing views (slide, carousel, accordion)
  2. **Explain what happened** (add/delete/modify animation)
  3. **Improve perceived performance** (skeleton screens, progressive load)
  4. **Natural motion** (physics-based, not arbitrary)

**Audit check:** "Do state changes have transitions? Do transitions help
the user understand what changed? Are they distracting?"

### 2.10 React Immediately
- Every action should produce immediate visible feedback
- Loading states should appear within 100ms
- Results should be communicated clearly
- The system should feel responsive even during async operations

**Audit check:** "Does every click/tap produce immediate feedback? Are
loading states present? Do operations feel instant?"

---

## 3. Global Styles (Token System)

### 3.1 Color System

**Two-level architecture:**

1. **System-level** (120 base colors):
   - 12 hue families: Dust Red, Volcano, Sunset Orange, Calendula Gold,
     Sunrise Yellow, Lime, Polar Green, Cyan, Daybreak Blue, Geek Blue,
     Golden Purple, French Magenta
   - Each hue has 10 shades (hue-0 through hue-9)
   - 10-step neutral gray palette
   - Algorithmic palette generation from any primary color

2. **Product-level** (semantic colors):
   - **Brand color**: Primary hue, shade 6 (e.g., `blue-6 = #1677ff`)
   - **Functional colors**: Success (green), Error (red), Warning (gold),
     Info/Link (blue) — fixed semantic mapping
   - **Neutral colors** with WCAG-compliant opacity values:
     - Heading text: `#000000E0` (light) / `#FFFFFFD9` (dark)
     - Body text: `#000000E0` / `#FFFFFFD9`
     - Secondary text: `#000000A6` / `#FFFFFFA6`
     - Disabled text: `#00000040` / `#FFFFFF40`
     - Border: `#D9D9D9` / `#424242`
     - Separator: `#0505050F` / `#FDFDFD1F`
     - Background: `#F5F5F5` / `#000000`

**Audit check:** "Does the color palette follow a systematic structure?
Are semantic colors used consistently? Do text colors maintain WCAG AAA
contrast (7:1)?"

### 3.2 Typography

- **Base font size**: 14px (optimized for 50cm reading distance)
- **Font scale**: Inspired by pentatonic musical scale + natural
  logarithm — 10 sizes with corresponding line heights
- **Font weight**: Restrained — only Regular (400) and Medium (500);
  Semibold (600) for English bold
- **Font color**: Four-level hierarchy (heading, body, secondary, disabled)
- **Key rule**: 3-5 font sizes maximum per design system
- **Tabular numbers**: `font-variant-numeric: tabular-nums` for
  vertically aligned numbers

**Audit check:** "Are there more than 5 distinct font sizes on one
screen? Is font weight used consistently? Are numbers tabular-aligned?"

### 3.3 Spacing

- **Formula**: `y = 8 + 8n` where n >= 0
- Three tiers: 8px (tight), 16px (standard), 24px (loose)
- Horizontal: Grid-based layout for responsive flexibility
- Vertical: Spacing hierarchy communicates information grouping

**Audit check:** "Is spacing consistent? Does it follow an 8px grid?
Do spacing differences correctly communicate grouping?"

### 3.4 Shadow

- **4 elevation layers**:
  - Layer 0: No shadow (grounded elements like input fields)
  - Layer 1: Low elevation (hover states, cards)
  - Layer 2: Medium elevation (dropdowns, popovers)
  - Layer 3: High elevation (modals, dialogs)
- **Directional shadows** based on position:
  - Down: Page content, general components
  - Up: Bottom toolbars
  - Left: Right-side navigation/drawers
  - Right: Left-side navigation/drawers
- **Three-layer shadow** compositing for realistic soft shadows

**Audit check:** "Are shadows consistent with elevation? Do directional
shadows match the component's position? Are there more than 4 shadow
levels (too complex)?"

---

## 4. Motion Design

Ant Design's motion philosophy (Ant Motion) has three principles:

### Natural
- Animations should follow physical laws (gravity, momentum)
- Button press analogy: "foliage on water" — press down, bounce back,
  create ripples
- Motion should feel justified, not decorative

### Performant
- Transitions should be **as short as possible** to serve their purpose
- Disappearing animations should be faster than appearing animations
  (don't attract attention to removal)
- List item exit: all items disappear simultaneously (no stagger on exit)
- Avoid delays between sequential animations

### Concise
- Avoid dramatic, complicated animations
- A menu arrow rotating 180 degrees is sufficient — don't add bounce,
  color change, or size change
- If the user's focus is on the *content*, don't animate the *container*

### Transition Purposes
1. **Maintain context**: Slide In/Out, Carousel, Accordion
2. **Explain what happened**: Adding, deleting, modifying objects
3. **Improve perceived performance**: Skeleton screens, progressive render
4. **Natural motion**: Physics-based (spring, ease-out)

**Audit checks for motion:**
- "Is every animation justified? What does it communicate?"
- "Are exit animations faster than enter animations?"
- "Are animations concise or dramatic/distracting?"
- "Do transitions help maintain spatial context?"
- "Is perceived performance improved (skeleton screens for loading)?"

---

## 5. Feedback Taxonomy

Ant Design categorizes feedback into a comprehensive hierarchy:

### Prompt Messages (Static)
| Type | When to Use | Duration |
|---|---|---|
| **Alert** | Non-blocking, stays on page | Persistent (manual close) |
| **Notification** | Important system-pushed info | Top-right, auto-dismiss |
| **Badge** | Aggregate counts on icons | Persistent until cleared |
| **Tooltip** | Precise description of pointed object | On hover |
| **Popover** | Richer content than tooltip (links, buttons) | On hover/click |

### Process Feedback
| Type | When to Use |
|---|---|
| **Spinner** | Short wait (< 2 seconds) |
| **Progress bar** | Long operation with measurable progress |
| **Skeleton** | Initial page load placeholder |

### Input Feedback
| Type | When to Use |
|---|---|
| **Inline validation** | Real-time field validation |
| **Popconfirm** | Lightweight confirmation near target |

### Result Feedback
| Type | When to Use | Duration |
|---|---|---|
| **Message** | Lightweight result (top-center, auto-dismiss) | 3 seconds |
| **Modal/Dialog** | Important result requiring acknowledgment | Manual close |
| **Result page** | Operation outcome (success/error/info) | Navigated to |

**Audit checks:**
- "Is feedback provided for every user action?"
- "Is the feedback level appropriate? (Don't use a modal for a save)"
- "Are loading states present for operations > 100ms?"
- "Are error messages near the source, not in a generic banner?"

---

## 6. Component Inventory

### General (4)
Button, FloatButton, Icon, Typography

### Layout (7)
Divider, Flex, Grid, Layout, Masonry, Space, Splitter

### Navigation (7)
Anchor, Breadcrumb, Dropdown, Menu, Pagination, Steps, Tabs

### Data Entry (18)
AutoComplete, Cascader, Checkbox, ColorPicker, DatePicker, Form, Input,
InputNumber, Mentions, Radio, Rate, Select, Slider, Switch, TimePicker,
Transfer, TreeSelect, Upload

### Data Display (20)
Avatar, Badge, Calendar, Card, Carousel, Collapse, Descriptions, Empty,
Image, List, Popover, QRCode, Segmented, Statistic, Table, Tag,
Timeline, Tooltip, Tour, Tree

### Feedback (11)
Alert, Drawer, Message, Modal, Notification, Popconfirm, Progress,
Result, Skeleton, Spin, Watermark

### Other (4)
Affix (sticky), App, ConfigProvider, Util

**Total: 71 components**

### Cross-reference with afterhours

| AntD Component | afterhours Status | Notes |
|---|---|---|
| Button | Exists | |
| Checkbox | Exists | |
| Radio | Exists | |
| Slider | Exists | |
| Switch (Toggle) | Exists | |
| Input | Exists (text_input) | |
| Select (Dropdown) | Exists | |
| Tabs | Exists | |
| Tooltip | Exists | |
| Modal/Dialog | Exists | |
| Progress | Exists | |
| **AutoComplete/Combobox** | **Missing** | High priority |
| **Popover** | **Missing** | High priority |
| **Breadcrumb** | **Missing** | Medium priority |
| **Steps** | **Missing** | Medium — wizard/tutorial flows |
| **Collapse/Accordion** | **Missing** | Medium priority |
| **Tag/Chip** | **Missing** | Medium priority |
| **Badge** | **Missing** | Medium — notification counts |
| **Drawer** | **Missing** | Medium — side panels |
| **Tour** | **Missing** | Useful for onboarding in games |
| **Skeleton** | **Missing** | Loading placeholder |
| **Alert** | **Missing** | Non-blocking messages |
| **Notification/Toast** | **Missing** | Auto-dismiss messages |
| **Timeline** | **Missing** | Event history display |
| **Statistic** | **Missing** | Number display with formatting |
| **Empty State** | **Missing** | Placeholder for empty lists |
| **Segmented** | **Missing** | Segmented control (like iOS) |
| **Rate** | **Missing** | Star rating (DaisyUI also has) |
| **Carousel** | **Missing** | (Covered in Swiper analysis) |
| **Transfer** | **Missing** | Dual-list selector |
| **Tree** | **Missing** | Hierarchical data display |
| **Table** | **Missing** | Data grid (complex, low priority) |
| **Calendar** | N/A | Rarely needed in games |
| **DatePicker/TimePicker** | N/A | Rarely needed in games |
| **Upload** | N/A | Web-specific |
| **QRCode** | N/A | Web-specific |
| **Watermark** | N/A | Web-specific |
| **Affix** | N/A | Web-specific (CSS sticky) |

---

## 7. Audit Skill Assessment

Ant Design is **highly suitable** as an audit resource. Its design spec
covers both visual design *and* interaction design principles, which
makes it more comprehensive than pure visual guidelines (like USWDS or
Material).

### What makes it audit-worthy

1. **Concrete, checkable principles** — Proximity, Alignment, Contrast,
   Repetition are the classic PARC design fundamentals. These can be
   evaluated against any UI screenshot.

2. **Spacing formula** — The `8 + 8n` rule is simple and verifiable.
   An auditor can check if spacing follows a consistent system.

3. **Feedback taxonomy** — The categorization of feedback types (alert
   vs notification vs message vs modal) with clear "when to use" rules
   creates specific things to check: "Is this the right feedback level?"

4. **Motion principles** — "Performant" (exit faster than enter),
   "Concise" (don't over-animate), "Natural" (physics-based) are all
   evaluatable.

5. **Shadow/elevation model** — 4-layer system with directional rules
   creates clear structural expectations.

6. **Interaction design principles** — "Make it Direct", "Stay on Page",
   "Keep it Lightweight" (Fitts's Law), "Provide an Invitation",
   "React Immediately" are all high-level heuristics that can flag real
   UX issues.

### Proposed audit categories

An Ant Design audit could check **12 categories**:

| # | Category | Source | Example Violation |
|---|---|---|---|
| 1 | **Proximity/Grouping** | PARC principles | Related controls spaced equally to unrelated ones |
| 2 | **Alignment** | PARC principles | Form labels don't share a consistent edge |
| 3 | **Contrast/Hierarchy** | PARC principles | Two equal-weight buttons competing for attention |
| 4 | **Repetition/Consistency** | PARC principles | Same action styled differently on two screens |
| 5 | **Spacing System** | 8px grid | Spacing values that don't follow `8+8n` |
| 6 | **Typography Restraint** | Font spec | More than 5 font sizes on one screen |
| 7 | **Color Semantics** | Color system | Red used for a non-error purpose |
| 8 | **Elevation/Shadow** | Shadow spec | Dropdown has same shadow as page content |
| 9 | **Feedback Appropriateness** | Feedback patterns | Modal used for a trivial confirmation |
| 10 | **Motion Quality** | Motion principles | Bouncy exit animation that draws attention |
| 11 | **Directness** | Interaction principles | Editing requires navigating to a separate screen |
| 12 | **Fitts's Law** | Keep It Lightweight | Primary action button is small and far from content |

### Comparison with existing audit skills

| Audit | Focus | Strength |
|---|---|---|
| Apple HIG | Platform conventions, native feel | Polish, platform consistency |
| Google Material | Material metaphor, elevation, motion | Systematic, elevation/shadow |
| USWDS | Government/accessibility, trust | Accessibility, plain language |
| Sun Java L&F | Desktop consistency, control behavior | Classic desktop patterns |
| Win95 | Simplicity, direct manipulation | Minimal, keyboard-first |
| **Ant Design** | **Enterprise UX, PARC + interaction design** | **Most comprehensive principles, feedback taxonomy, Fitts's Law** |

Ant Design fills a gap none of the existing audits cover well:
**interaction design heuristics** (directness, staying on page, Fitts's
Law, invitation/discoverability) and **feedback pattern selection** (when
to use toast vs modal vs inline error).

---

## 8. Recommended: Create `audit-antd-design` Skill

### Guidelines document: `prompts/antd_design_principles.md`

Should include:
1. The 10 interaction principles as a checklist
2. The PARC rules (Proximity, Alignment, Contrast, Repetition)
3. Spacing system (8px grid, 3-tier formula)
4. Typography rules (3-5 sizes, 2 weights, tabular numbers)
5. Color semantics (functional colors, neutral hierarchy)
6. Shadow/elevation (4 layers, directional rules)
7. Feedback taxonomy (when to use each feedback type)
8. Motion rules (natural, performant, concise)

### Audit checklist (12 items):

```
1. PROXIMITY: Are related elements grouped by spacing?
2. ALIGNMENT: Do elements share consistent alignment edges?
3. CONTRAST: Is there a clear visual hierarchy with one focal point?
4. REPETITION: Are similar elements styled identically across screens?
5. SPACING: Does spacing follow an 8px (or consistent base) grid?
6. TYPOGRAPHY: Are font sizes limited to 3-5? Weights to 2?
7. COLOR: Are semantic colors (error=red, success=green) used consistently?
8. ELEVATION: Do shadows correctly indicate layer depth?
9. FEEDBACK: Is every action's feedback level appropriate?
10. MOTION: Are animations justified, concise, and physics-based?
11. DIRECTNESS: Can users act directly or must they navigate away?
12. FITTS'S LAW: Are primary targets large and close to the content?
```

### Add to multi-audit

Update `.cursor/skills/multi-audit/SKILL.md` to include Ant Design as a
6th parallel audit, bringing the total to 6 subagents.

---

## 9. What afterhours Can Adopt from Ant Design

### Design Token System

| Ant Design Token | afterhours Equivalent | Action |
|---|---|---|
| 12-hue × 10-shade palette | Theme has limited colors | Expand to 10-step shades per hue |
| Functional colors (success/error/warning/info) | Partial | Add `success`, `info` semantic roles |
| 4-level text color hierarchy | Single text color | Add `text_secondary`, `text_disabled` |
| 8px spacing grid | `enforce_spacing_rhythm` exists | Already aligned |
| 4-layer shadow system | `depth` variable (DaisyUI) | Formalize 4-level elevation |
| 3-5 font sizes | Font size in theme | Add `enforce_max_font_sizes` validation |

### New Validation Flags (from Ant Design)

```cpp
// Typography restraint
bool enforce_max_font_sizes = false;
int max_distinct_font_sizes = 5;  // Flag if more than N sizes used

// Feedback appropriateness (design guidance, hard to automate)
// But we can check: does every clickable element produce feedback?
bool enforce_action_feedback = false;

// Fitts's Law: primary action targets should meet minimum size
// (Already proposed as enforce_min_touch_target, but also consider
// checking that primary buttons are larger than secondary ones)
bool enforce_button_hierarchy_size = false;

// Elevation consistency
bool enforce_shadow_hierarchy = false;
// Check that popover shadows > card shadows > flat element shadows
```

### Components to Prioritize

Based on Ant Design's enterprise patterns that also apply to game UI:

1. **Steps/Wizard** — Perfect for tutorials, quests, achievement progress
2. **Tour** — Onboarding overlay that highlights UI elements in sequence
3. **Empty State** — Placeholder when a list/grid has no content
4. **Badge** — Notification count overlays on icons (inventory, mail)
5. **Notification/Toast** — Auto-dismissing status messages
6. **Statistic** — Formatted number display with label (scores, currency)
7. **Segmented Control** — iOS-style toggle between views
8. **Tag/Chip** — Categorization labels, filters, status indicators
9. **Collapse/Accordion** — Expandable content sections
10. **Drawer** — Side panel overlay (inventory, settings)

---

## 10. Key Takeaway

Ant Design's greatest contribution to afterhours is not its components
(which are web-centric) but its **design principles as auditable rules**.
The combination of PARC fundamentals + interaction design heuristics +
feedback taxonomy creates the most comprehensive audit checklist of any
design system we've surveyed.

The 10 principles (Proximity, Alignment, Contrast, Repetition, Make It
Direct, Stay on Page, Keep It Lightweight, Provide an Invitation, Use
Transition, React Immediately) are universal — they apply equally to
web apps and game UIs. They should become a core audit resource.
