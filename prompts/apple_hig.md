# Apple Human Interface Guidelines Audit Skill

Use this skill with `/audit-design` to systematically evaluate any UI/UX design against the classic Apple Human Interface Guidelines (1987). This provides a comprehensive checklist to identify violations and suggest improvements.

## How to Use This Skill

When auditing a design (screen, interaction, component, or full application):

1. Request a screenshot, description, or code of the UI being audited
2. Walk through each category below systematically
3. For each principle, note: **PASS**, **FAIL**, or **N/A**
4. Provide specific, actionable feedback for every failure
5. Summarize with a priority-ranked list of issues to fix

---

## CATEGORY 1: Core Philosophy Principles

### 1.1 Metaphors from the Real World
**Question:** Does the interface use concrete, understandable metaphors that connect to real-world experiences?

**Check for:**
- [ ] Icons and visual elements resemble real-world objects users already understand
- [ ] Audio and visual effects support (don't contradict) the metaphor
- [ ] Metaphors are plain and obvious, not abstract or cryptic
- [ ] The desktop metaphor is maintained—objects appear to rest on surfaces
- [ ] Actions feel physical (dragging feels like moving, trash can deletes things)

**Red Flags:**
- Abstract icons with no clear real-world meaning
- Metaphors taken to illogical extremes
- Mixed metaphors that confuse the user

### 1.2 Direct Manipulation
**Question:** Can users directly grab, move, and manipulate objects on screen?

**Check for:**
- [ ] Users can click/drag objects rather than typing commands
- [ ] Physical actions produce immediate physical-feeling results
- [ ] Visual feedback accompanies every action (animation, highlighting)
- [ ] When a document is moved, users "see" it move
- [ ] The mouse/pointer provides a sense of direct control

**Red Flags:**
- Abstract command entry instead of direct interaction
- Actions happen without visual feedback
- No sense of "grabbing" or "moving" objects

### 1.3 See-and-Point (Not Remember-and-Type)
**Question:** Can users discover and select actions by looking, not by remembering commands?

**Check for:**
- [ ] All available actions are visible in menus or on screen
- [ ] Users select first (noun), then act (verb)—"Hey you, do this"
- [ ] Recognition over recall—users shouldn't memorize anything
- [ ] Keyboard shortcuts are *alternatives*, never the only way
- [ ] New users can find any feature by scanning menus

**Red Flags:**
- Hidden commands requiring memorization
- Command-line style text entry for basic tasks
- Features only accessible via keyboard shortcuts
- Cryptic abbreviations or codes

### 1.4 Consistency
**Question:** Is the application consistent internally and with other applications?

**Check for:**
- [ ] Same action produces same result throughout the application
- [ ] Standard elements (menus, windows, controls) used properly
- [ ] Keyboard shortcuts follow platform conventions
- [ ] Visual styles consistent throughout
- [ ] Terminology consistent—same words for same concepts

**Red Flags:**
- Same action doing different things in different contexts
- Custom implementations of standard controls
- Non-standard keyboard shortcuts
- Mixed terminology for identical concepts

### 1.5 WYSIWYG (What You See Is What You Get)
**Question:** Does the screen accurately represent what the user will get?

**Check for:**
- [ ] No secrets—what's on screen is what exists
- [ ] Print output matches screen appearance
- [ ] Formatting is visible, not hidden in codes
- [ ] Document appearance updates immediately when changed
- [ ] No abstract commands that "promise" future results

**Red Flags:**
- Hidden formatting or metadata
- Screen doesn't match printed output
- Delayed rendering of changes
- Results only visible after explicit refresh

### 1.6 User Control
**Question:** Is the user always in charge, never the computer?

**Check for:**
- [ ] User initiates all actions—computer responds
- [ ] Risky actions show warnings but let user proceed if confirmed
- [ ] No forced workflows—user can do things in any order
- [ ] No "protecting" users from their own choices
- [ ] User can always quit, save, or cancel

**Red Flags:**
- Forced linear workflows
- Computer refusing valid user requests
- Patronizing restrictions "for the user's protection"
- Blocking user actions without explanation
- No way to escape or cancel

### 1.7 Feedback and Dialog
**Question:** Is the user always informed about what's happening?

**Check for:**
- [ ] Immediate feedback confirms every action
- [ ] Progress indicators for operations taking more than a moment
- [ ] Clear explanation when operations can't complete
- [ ] Feedback is brief, direct, uses user vocabulary
- [ ] Time estimates for long operations

**Red Flags:**
- Actions without visible feedback
- Long operations with no progress indicator
- Cryptic error messages
- Technical jargon in user-facing text
- Silent failures

### 1.8 Forgiveness
**Question:** Can users recover from mistakes easily?

**Check for:**
- [ ] Undo available for all content-changing operations
- [ ] Users warned before irreversible actions
- [ ] Alert messages are infrequent (not a barrage)
- [ ] Users can explore without fear of breaking things
- [ ] Cancel available in all dialogs

**Red Flags:**
- No undo
- Irreversible actions without warning
- Frequent, nagging alert messages
- Punishing users for exploration
- No escape from modals

### 1.9 Perceived Stability
**Question:** Does the interface feel stable and familiar, not randomly changing?

**Check for:**
- [ ] Menu bar always visible, always in same position
- [ ] Unavailable items are dimmed, not removed
- [ ] Desktop provides familiar landmarks
- [ ] Changes feel like user's doing, not random
- [ ] Core interface elements don't move around

**Red Flags:**
- Menu items appearing/disappearing
- Interface elements jumping around
- Layouts changing unexpectedly
- Loss of familiar reference points
- Jarring visual changes

### 1.10 Aesthetic Integrity
**Question:** Is the design visually clear, attractive, and functional?

**Check for:**
- [ ] Visually confusing displays avoided
- [ ] Different things look different
- [ ] Graphics support understanding, not just decoration
- [ ] Clear visual hierarchy—most important elements stand out
- [ ] Users can personalize their workspace (within reason)

**Red Flags:**
- Visual clutter
- Different things looking the same
- Decoration that distracts from function
- Flat visual hierarchy—everything same prominence
- Forced aesthetic with no user control

---

## CATEGORY 2: Screen Elements

### 2.1 Windows
**Check for:**
- [ ] Standard window components used correctly (title bar, close box, scroll bars, size box, zoom box)
- [ ] Title bar has stripes only when window is active
- [ ] Scroll bars, close box, zoom box disappear when window is inactive
- [ ] Windows can be moved by dragging title bar
- [ ] Windows can be resized with size box (if applicable)
- [ ] Multiple windows maintain front-to-back ordering
- [ ] Clicking inactive window activates it (second click to interact)
- [ ] Window can never be moved completely off screen
- [ ] Active window is visually distinct from inactive windows

**Red Flags:**
- Non-standard window decorations
- Windows that can't be moved or resized
- Active/inactive windows indistinguishable
- Windows disappearing off screen with no way back

### 2.2 Scroll Bars
**Check for:**
- [ ] Scroll bar represents entire document dimension
- [ ] Scroll box shows relative position in document
- [ ] Click scroll arrow = move one unit (line, row, etc.)
- [ ] Press scroll arrow = continuous scrolling
- [ ] Click gray area = scroll by windowful
- [ ] Drag scroll box = jump to position
- [ ] Inactive when document fits in window
- [ ] Proportional scroll box size (optional but good)

**Red Flags:**
- Scroll box doesn't represent position accurately
- Clicking gray area doesn't page through document
- No way to jump to specific position
- Scrolling is jerky or unpredictable

### 2.3 Menus
**Check for:**
- [ ] Menu bar at top of screen with menu titles
- [ ] Apple menu (first), File menu (second), Edit menu (third)
- [ ] Menu items are verbs (actions) or adjectives (attributes)
- [ ] Most frequent operations at top of menu
- [ ] Related items grouped with dotted lines
- [ ] Ellipsis (...) after items that need more info (dialog box)
- [ ] Check marks show current attributes in effect
- [ ] Unavailable items dimmed, not removed
- [ ] Keyboard equivalents shown with ⌘ symbol

**Red Flags:**
- Non-standard menu order
- Nouns as menu items (should be verbs/adjectives)
- Ellipsis missing when dialog will appear
- Items removed instead of dimmed
- Inconsistent keyboard equivalents

### 2.4 Standard Keyboard Equivalents (Reserved)
These MUST NOT be used for other purposes:

| Key | Function |
|-----|----------|
| ⌘-N | New |
| ⌘-O | Open |
| ⌘-S | Save |
| ⌘-Q | Quit |
| ⌘-Z | Undo |
| ⌘-X | Cut |
| ⌘-C | Copy |
| ⌘-V | Paste |
| ⌘-. | Cancel/Stop operation |
| ⌘-? | Help |

### 2.5 Dialog Boxes
**Check for:**
- [ ] Appears when menu item has ellipsis (...)
- [ ] Modal dialogs used sparingly—only when completion required
- [ ] Modeless dialogs for ongoing operations (like Find/Replace)
- [ ] Default button is doubly outlined
- [ ] Cancel button always available
- [ ] Default button activated by Return/Enter
- [ ] Fields have sensible default values
- [ ] Tab moves between fields
- [ ] Most important info at top-left, buttons at bottom-right

**Red Flags:**
- Modal dialogs used when modeless would work
- No cancel option
- No default button indicated
- Poor layout (important stuff at bottom)
- No keyboard navigation

### 2.6 Alert Boxes
**Check for:**
- [ ] Icon indicates severity: Note 📝, Caution ⚠️, Stop 🛑
- [ ] Message is brief, informative, friendly
- [ ] Phrased from user's perspective, not programmer's
- [ ] Default button is the safest/most likely choice
- [ ] Button labels describe action (Save, Don't Save) not Yes/No
- [ ] Message explains what to do, not just what went wrong
- [ ] Self-explanatory—no manual lookup required

**Red Flags:**
- Technical error messages
- Ambiguous "OK" when specific action name would be clearer
- Scary language for minor issues
- No guidance on how to fix the problem
- Generic Yes/No instead of descriptive buttons

### 2.7 Controls

#### Buttons
- [ ] Labels describe the action that will occur
- [ ] Default button doubly outlined
- [ ] Cancel is always called "Cancel"
- [ ] OK can be replaced with more descriptive labels

#### Check Boxes
- [ ] Used for independent on/off options
- [ ] Multiple can be checked simultaneously
- [ ] Accompanied by text labels

#### Radio Buttons
- [ ] Used for mutually exclusive choices
- [ ] Always in groups of 2+
- [ ] Exactly one always selected
- [ ] Groups visually distinct from each other

---

## CATEGORY 3: Color Guidelines

### 3.1 Color Fundamentals
**Check for:**
- [ ] Design works in black and white first
- [ ] Color is supplementary, not required
- [ ] Color coding is redundant with shape/position/pattern
- [ ] Limited palette (4-7 colors max for coding)
- [ ] User can modify or remove application colors

**Red Flags:**
- Color is only way to distinguish elements
- Rainbow of colors overwhelming the interface
- No consideration for colorblind users
- Fixed colors user can't adjust

### 3.2 Color Usage Rules
**Check for:**
- [ ] Interface elements (menus, windows) have consistent, neutral backgrounds
- [ ] Outlines provide clear edge definition
- [ ] Highlighting/selection clearly distinguishes selected content
- [ ] Text and thin lines have sufficient contrast to be easily visible
- [ ] Small objects use high-contrast colors

**Red Flags:**
- Overly colorful chrome elements that distract from content
- Edges defined only by color (hard to focus)
- Low-contrast text or thin lines
- Subtle color differences for important distinctions

---

## CATEGORY 4: Sound Guidelines

### 4.1 When to Use Sound
**Check for:**
- [ ] Sound supplements visual indication, never replaces it
- [ ] Used to get attention for completed operations
- [ ] Used for alerts and errors
- [ ] Used to indicate mode changes

**Red Flags:**
- Sound is only indication of an event
- No visual alternative for hearing impaired
- Important information only communicated via sound

### 4.2 Sound Design
**Check for:**
- [ ] Sounds are subtle, not loud or harsh
- [ ] Different sounds are significantly different
- [ ] No tunes or jingles (become annoying)
- [ ] User can adjust volume or turn off
- [ ] Beeps accompanied by menu bar flash

**Red Flags:**
- Loud, harsh, or startling sounds
- Similar sounds that are hard to distinguish
- Catchy tunes that get annoying
- No way to mute or adjust

---

## CATEGORY 5: Selection and Manipulation

### 5.1 Selection
**Check for:**
- [ ] Select object first (noun), then choose action (verb)
- [ ] Visual feedback shows what's selected (highlighting/inverse)
- [ ] Immediate feedback on selection
- [ ] Click to select, click elsewhere to deselect
- [ ] Selection doesn't commit user to anything

**Red Flags:**
- Actions before selection
- No visual indication of selection
- Delayed feedback
- Selection triggers irreversible action

### 5.2 Mouse Actions
**Check for:**
- [ ] Moving mouse (no button) only moves pointer
- [ ] Click = press and release quickly, mouse stationary
- [ ] Drag = press, move, release
- [ ] Double-click is shortcut, not only way
- [ ] Press on controls = repeated clicking

**Red Flags:**
- Moving mouse causes changes (without button)
- Double-click required (not shortcut)
- Dragging not working consistently

### 5.3 Keyboard
**Check for:**
- [ ] Keyboard for text entry, not commands
- [ ] Arrow keys move insertion point, not pointer
- [ ] Return/Enter dismisses dialogs (activates default button)
- [ ] Tab moves between fields
- [ ] Escape = "let me out" / Cancel
- [ ] Modifier keys used consistently

**Red Flags:**
- Arrow keys moving mouse pointer
- No keyboard navigation in dialogs
- Escape doing something other than cancel
- Modifier keys used inconsistently

---

## CATEGORY 6: Modes

### 6.1 Modelessness
**Question:** Are modes avoided or used appropriately?

**Check for:**
- [ ] Same action produces same result regardless of history
- [ ] Modes restricted to: long-term (app vs app), spring-loaded (hold key), or alerts
- [ ] Acceptable modes: tool palettes, text attributes, real-world analogies
- [ ] Current mode is always visually indicated
- [ ] Easy to get into and out of modes
- [ ] No mode prevents Save or Quit

**Red Flags:**
- Same action doing different things unpredictably
- Modes without visual indication
- Hard to exit modes
- Save/Quit blocked by mode

---

## CATEGORY 7: Plain Language

### 7.1 Text Guidelines
**Check for:**
- [ ] Messages are concise and simple
- [ ] User vocabulary, not programmer vocabulary
- [ ] No jargon or technical terms
- [ ] Error messages explain what to do
- [ ] Constructive suggestions, not orders
- [ ] Friendly without being misleading

**Red Flags:**
- Technical jargon
- Error codes
- Messages requiring manual lookup
- Condescending or robotic tone
- Vague messages

---

## CATEGORY 8: Accessibility

### 8.1 Vision
**Check for:**
- [ ] Color not the only distinguisher
- [ ] Supports zooming/larger text
- [ ] High contrast available
- [ ] Works on monochrome displays

### 8.2 Hearing
**Check for:**
- [ ] Sound never the only indication
- [ ] Visual alternatives for all audio
- [ ] Beeps accompanied by menu bar flash

### 8.3 Motor/Cognitive
**Check for:**
- [ ] Double-click never required (always a single-click alternative)
- [ ] Clear, simple language
- [ ] Icons have obvious meanings
- [ ] Macro/shortcut support for complex sequences
- [ ] Don't require remembering many things

---

## AUDIT SUMMARY TEMPLATE

After completing the audit, summarize findings:

```
## Audit Summary: [Screen/Feature Name]

### Critical Issues (Fix Immediately)
1. [Issue]: [Specific violation] → [Recommendation]
2. ...

### Major Issues (Fix Soon)
1. [Issue]: [Specific violation] → [Recommendation]
2. ...

### Minor Issues (Consider Fixing)
1. [Issue]: [Specific violation] → [Recommendation]
2. ...

### Strengths
- [What's done well]
- ...

### Overall Score: [X/10]
```

---

## Quick Reference: The 10 Fundamental Principles

1. **Metaphors** — Use real-world concepts users already understand
2. **Direct Manipulation** — Let users grab and move objects
3. **See-and-Point** — Recognition over recall; show all options
4. **Consistency** — Same action = same result, everywhere
5. **WYSIWYG** — Screen shows reality, no hidden state
6. **User Control** — User initiates; computer responds
7. **Feedback** — Immediate, visible response to every action
8. **Forgiveness** — Undo everything; warn before destruction
9. **Stability** — Familiar landmarks; don't move things around
10. **Aesthetics** — Clean, clear, functional visual design

---

*Based on Apple Human Interface Guidelines: The Apple Desktop Interface (1987)*
