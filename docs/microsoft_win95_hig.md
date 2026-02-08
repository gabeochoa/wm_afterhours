# Microsoft Windows Interface Guidelines — Design Audit & Validation Guide

This document provides a comprehensive set of instructions and validation criteria to audit a user interface (UI) or user experience (UX) against Microsoft's *The Windows Interface Guidelines — A Guide for Designing Software* (February 1995). It is intended to be used as a skill for AI assistants to provide detailed feedback on design implementations targeting Windows 95-era applications.

The guide is structured into key areas of the Windows interface design system, with each section containing specific, actionable validation points.

*Source: "The Windows Interface Guidelines — A Guide for Designing Software", Microsoft Corporation, February 1995.*
*Reference PDF: https://ics.uci.edu/~kobsa/courses/ICS104/course-notes/Microsoft_WindowsGuidelines.pdf*

---

## How to Use This Guide

When auditing a design (screen, interaction, component, or full application):

1. Request a screenshot, description, or code of the UI being audited
2. Walk through each category below systematically
3. For each validation point, note: **PASS**, **FAIL**, or **N/A**
4. Provide specific, actionable feedback for every failure
5. Summarize with a priority-ranked list of issues to fix

---

# PART 1: DESIGN PRINCIPLES

The foundational principles on which the Windows interface is built.

## 1.1. User in Control

| Validation Point | Description |
| :--- | :--- |
| **User Initiates Actions** | Does the user initiate all actions, rather than the computer? The user should play an active, not reactive, role. |
| **Customizability** | Can users customize aspects of the interface? Does the software reflect user settings for system properties (color, fonts, options)? |
| **Avoid Modes** | Are modes avoided wherever possible? When modes are necessary, are they obvious, visible, the result of explicit user choice, and easy to cancel? |
| **Responsiveness** | Is the software interactive and responsive? Does the user never face a "dead" screen that is unresponsive to input? |

## 1.2. Directness

| Validation Point | Description |
| :--- | :--- |
| **Direct Manipulation** | Can users directly manipulate software representations of information (drag to relocate, navigate to a location)? |
| **Visible Actions** | Do users see how their actions affect objects on screen? |
| **Recognition Over Recall** | Can users recognize commands easier than recall syntax? Are choices and information visible? |
| **Familiar Metaphors** | Are metaphors used that provide a direct, intuitive interface to user tasks? Do they help users transfer knowledge and experience? |
| **Metaphor Not Limiting** | Are computer-based implementations not unnecessarily limited to real-world counterparts? (e.g., a folder can hold printers) |

## 1.3. Consistency

| Validation Point | Description |
| :--- | :--- |
| **Within Product** | Are common functions presented using a consistent set of commands and interfaces? Does the same command behave the same way in different situations? |
| **Within Operating Environment** | Does the software maintain consistency with Windows interaction and interface conventions? |
| **With Metaphors** | Is behavior consistent with what the metaphor implies? (e.g., a wastebasket vs. an incinerator) |
| **Visual Consistency** | Are visual styles (colors, fonts, spacing, borders) consistent throughout? |
| **Terminology** | Is the same word used for the same concept everywhere? |

## 1.4. Forgiveness

| Validation Point | Description |
| :--- | :--- |
| **Interactive Discovery** | Does the interface allow for exploration and learning by trial and error? |
| **Undo / Reversibility** | Are actions reversible or recoverable? Is Undo available for content-changing operations? |
| **Warnings** | Are users warned about potentially damaging, irreversible actions? |
| **Error Prevention** | Does the design avoid situations that are likely to result in errors? |
| **Error Recovery** | When errors do occur, is it easy for the user to recover? |

## 1.5. Feedback

| Validation Point | Description |
| :--- | :--- |
| **Immediate Feedback** | Is visual (and sometimes audio) feedback provided for every user interaction? |
| **Timely** | Is feedback presented as close to the point of interaction as possible? |
| **Progress Indicators** | For operations taking more than a few seconds, is a progress indicator displayed? Can the user cancel long operations? |
| **Appropriate Type** | Is the type of feedback appropriate to the task? (pointer change for simple info, message box for complex) |

## 1.6. Aesthetics

| Validation Point | Description |
| :--- | :--- |
| **Pleasant Environment** | Does the visual design provide a pleasant environment that contributes to user understanding? |
| **Visual Clarity** | Does every visual element that appears on screen serve a purpose? Minimal clutter? |
| **Clear Hierarchy** | Is there a clear visual hierarchy — most important elements stand out? |

## 1.7. Simplicity

| Validation Point | Description |
| :--- | :--- |
| **Minimal Presentation** | Is information presentation reduced to the minimum required to communicate adequately? |
| **Concise Labels** | Are command names and messages brief and succinct? |
| **Progressive Disclosure** | Is information organized so it is shown only at the appropriate time? (e.g., menus hide choices, dialog boxes reduce menu options) |
| **Natural Mappings** | Are elements arranged together when they are associated? |

---

# PART 2: WINDOWS

Windows provide the fundamental way a user views and interacts with data.

## 2.1. Primary Window Components

| Validation Point | Description |
| :--- | :--- |
| **Standard Components** | Does the primary window include the standard components: frame/border, title bar, menu bar, scroll bars (when needed), and optionally toolbars and status bars? |
| **Sizable Border** | For sizable windows, does the border provide handles for resizing? |
| **Size Grip** | Is a size grip included in the lower right corner for sizable windows? (at junction of scroll bars or at right end of status bar) |

## 2.2. Title Bars

| Validation Point | Description |
| :--- | :--- |
| **Title Bar Icon** | Does a small icon appear in the upper left corner representing the object being viewed? |
| **Title Text Order** | For document windows: is the filename first, followed by a dash and the application name? (e.g., "My Report - WordPad") |
| **Tool App Title** | For tool applications (no data files): is the application name displayed? |
| **No Extensions** | Are file extensions and path names avoided in the title bar? |
| **Active/Inactive** | Is the active window title bar visually distinguished from inactive windows? (system active vs. inactive caption color) |

## 2.3. Title Bar Buttons

| Validation Point | Description |
| :--- | :--- |
| **Close Button** | Is a Close button present in the title bar? |
| **Minimize Button** | Is a Minimize button present (if the window supports minimizing)? |
| **Maximize Button** | Is a Maximize button present (if the window supports maximizing)? |
| **Restore Button** | When maximized, does the Maximize button change to a Restore button? |
| **Button Functions** | Do the buttons perform their correct operations (Close, Minimize, Maximize, Restore)? |

## 2.4. Window Operations

| Validation Point | Description |
| :--- | :--- |
| **Move by Title Bar** | Can the user move the window by dragging its title bar? |
| **Resize by Border** | Can sizable windows be resized by dragging the border? |
| **Always Accessible** | Can the user never reposition a window such that it cannot be accessed? |
| **State Preservation** | When reopening a window, is its previous size, position, and view state restored? |
| **Activation** | Does clicking any part of a window (including its interior) activate it? |
| **Selection Preserved** | When reactivating a window, is the previous selection and focus restored? |

## 2.5. Scroll Bars

| Validation Point | Description |
| :--- | :--- |
| **Present When Needed** | Are scroll bars displayed when the content exceeds the window size? |
| **Consistent Display** | Do scroll bars remain displayed even when the window becomes inactive or resized so content fits? |
| **Scroll Arrows** | Do scroll arrows scroll one unit in the direction of the arrow? Are they disabled when scrolling to the end? |
| **Proportional Scroll Box** | Does the scroll box size reflect the proportion of visible content to total content? |
| **Scroll Box Position** | Does the scroll box position indicate relative position in the document? |
| **Drag Scroll Box** | Can the user drag the scroll box to scroll? Is the view updated continuously? |
| **Shaft Click** | Does clicking in the scroll bar shaft scroll by one screenful with overlap? |
| **Auto-Scroll** | When selecting past the edge of a scrollable area, does auto-scrolling occur? |

---

# PART 3: MENUS

Menus list the commands available to the user, leveraging recognition over recall.

## 3.1. Menu Bar

| Validation Point | Description |
| :--- | :--- |
| **Position** | Is the menu bar directly below the title bar, spanning the width of the window? |
| **Single Word Titles** | Are menu titles single words? (Multiple words can be confused with two separate titles) |
| **Access Keys** | Does each menu title have a unique underlined access key character? |
| **Standard Menus** | Are standard menus present where applicable? File, Edit, View, Window (MDI), Help? |

## 3.2. Common Drop-Down Menus

| Validation Point | Description |
| :--- | :--- |
| **File Menu** | Does the File menu include Open, Save, Send To, Print? Is Exit at the bottom preceded by a separator? |
| **Edit Menu** | Does the Edit menu include Undo, Cut, Copy, Paste? Optionally Repeat, Find/Replace, Delete, Duplicate? |
| **View Menu** | Does the View menu contain commands that affect the view (Zoom, Outline, Show Ruler) and not the data? |
| **Help Menu** | Does the Help menu include Help Topics? Is About *application name* present for version/copyright info? |

## 3.3. Menu Items

| Validation Point | Description |
| :--- | :--- |
| **Ellipsis Convention** | Do menu items that require additional information display an ellipsis (...)? Items that simply display a window (like Properties) do NOT use an ellipsis. |
| **Disabled vs. Removed** | Are unavailable items disabled (grayed), not removed? Items are only removed if no longer relevant. |
| **Disabled Title** | If all items in a menu are disabled, is the menu title itself disabled? |
| **Check Marks** | Are check marks used for independent settings (like Bold, Italic)? |
| **Option Marks** | Are option button marks used for interdependent/mutually exclusive settings (like Left, Center, Right alignment)? |
| **Separators** | Are related menu items grouped with separator lines? |
| **Book Title Caps** | Do menu item names use book title capitalization? (Capitalize first letter of every word except articles/conjunctions/prepositions) |
| **Shortcut Keys** | Are keyboard shortcuts displayed right-aligned in drop-down menus? Formatted as "Ctrl+key"? |
| **Access Keys** | Does each menu item have a unique access key within its menu? |
| **Default Item** | Is the default item (if any) displayed in bold? |

## 3.4. Pop-up Menus (Context Menus)

| Validation Point | Description |
| :--- | :--- |
| **Present** | Are pop-up menus provided for objects, even if a menu bar exists? |
| **Button 2 Click** | Are pop-up menus displayed by clicking mouse button 2? |
| **Context-Specific** | Do pop-up menus contain commands specific to the object or context? |
| **Primary Commands First** | Are the object's primary commands first, then transfer commands (Cut, Copy, Paste), then other commands? |
| **Properties Last** | Is the Properties command the last item on the pop-up menu? |
| **Small Size** | Is the pop-up menu kept small, limited to common, frequent actions? |
| **No Shortcut Keys** | Are shortcut keys avoided in pop-up menus? (Pop-up menus are already a shortcut) |

## 3.5. Cascading Menus

| Validation Point | Description |
| :--- | :--- |
| **Used Sparingly** | Are cascading menus used sparingly? Limited to a single submenu level? |
| **Not for Frequent Commands** | Are cascading menus avoided for frequent, repetitive commands? |
| **Triangle Arrow** | Is a triangular arrow displayed adjacent to items with cascading menus? |

---

# PART 4: CONTROLS

Controls are graphic objects that represent properties or operations.

## 4.1. Command Buttons (Push Buttons)

| Validation Point | Description |
| :--- | :--- |
| **Clear Labels** | Do button labels clearly represent the action the button starts? |
| **Title Caps** | Do text labels use book title capitalization? |
| **Ellipsis** | Do buttons requiring additional information include an ellipsis (...)? |
| **Immediate Effect** | Is the effect of choosing a button immediate with respect to its context? |
| **Pressed Appearance** | Does the button show its pressed appearance when the user presses the mouse button on it? |
| **Option-Set Appearance** | When used to represent a state/mode, does the button show the option-set appearance (checkerboard pattern)? |
| **Unavailable Appearance** | When disabled, does the button show the engraved unavailable appearance? |

## 4.2. Option Buttons (Radio Buttons)

| Validation Point | Description |
| :--- | :--- |
| **Mutually Exclusive** | Are option buttons used only for mutually exclusive choices? |
| **Groups of 2+** | Do option buttons always appear in groups of two or more? |
| **One Always Set** | Is exactly one option always selected in a group? |
| **Sentence Caps** | Do labels use sentence capitalization? |
| **≤ 7 Choices** | Is the number of choices limited to seven or fewer? (Use a list box for more) |

## 4.3. Check Boxes

| Validation Point | Description |
| :--- | :--- |
| **Independent Choices** | Are check boxes used for independent (non-exclusive) on/off choices? |
| **Obvious Opposites** | Are the two states of the check box clearly opposite and unambiguous? |
| **Toggle Behavior** | Does clicking toggle the check box state? |
| **Mixed-Value State** | When representing mixed values for a multiple selection, is the checkerboard mixed-value appearance used? |

## 4.4. List Boxes

| Validation Point | Description |
| :--- | :--- |
| **Height** | Is the list box tall enough to show 3-8 items? |
| **Scroll Bar** | Does it include a vertical scroll bar? Are scroll arrows disabled at the ends? |
| **Label** | Is a descriptive static text label provided with an access key? |
| **Logical Order** | Are entries ordered logically (alphabetical, chronological, etc.)? |
| **Width** | Is the width sufficient to display the average entry? |

## 4.5. Drop-Down List Boxes

| Validation Point | Description |
| :--- | :--- |
| **Current Value Shown** | In its closed state, does the control show the current value? |
| **Open Height** | When opened, does the list show 3-8 items? |
| **Menu-Like Interaction** | Does the interaction follow menu conventions (press to display, click item to select and close)? |

## 4.6. Text Boxes (Edit Controls)

| Validation Point | Description |
| :--- | :--- |
| **Border** | Is an outline border displayed (typically in toolbar or secondary window)? |
| **Immediate Validation** | Is user input validated immediately where possible? (ignoring inappropriate characters or providing feedback) |
| **Label** | Is a static text label with access key provided? |

## 4.7. Combo Boxes

| Validation Point | Description |
| :--- | :--- |
| **Text + List** | Does the combo box combine a text box with a list box? |
| **Dependent Relationship** | As text is typed, does the list scroll to the nearest match? When an item is selected in the list, does it replace the text box content? |

## 4.8. Other Controls

| Validation Point | Description |
| :--- | :--- |
| **Group Boxes** | Are group boxes used to visually organize sets of related controls? |
| **Tabs** | Are tab controls used for multiple logical pages/sections within the same window? |
| **Sliders** | Are sliders used for setting values on a continuous range? |
| **Progress Indicators** | Are progress bars used for lengthy operations? |
| **Tooltips** | Are tooltips provided for controls without text labels (especially toolbar buttons)? |

---

# PART 5: TOOLBARS AND STATUS BARS

## 5.1. Toolbars

| Validation Point | Description |
| :--- | :--- |
| **Quick Access** | Does the toolbar provide quick access to specific commands or options? |
| **Tooltips** | Do all toolbar buttons without text labels have tooltips? |
| **Button Sizes** | Are toolbar buttons provided in standard sizes (24×22 or 32×30 pixels, with images 16×16 or 24×24)? |
| **Standard Images** | Are standard toolbar button images used for standard functions (New, Open, Save, Print, Undo, Redo, Cut, Copy, Paste, Bold, Italic, Underline)? |
| **User Configurable** | Is the toolbar user-configurable? Can the user show/hide it? |
| **Consistent Images** | Are standard button images used only for their designated functions? |
| **Immediate Effect** | Do toolbar button changes apply directly to the current selection (no confirmation needed)? |

## 5.2. Status Bars

| Validation Point | Description |
| :--- | :--- |
| **Position** | Is the status bar at the bottom of the window? |
| **Contextual Information** | Does the status bar display current state information about the content (page number, key states, time, etc.)? |
| **Menu Descriptions** | Does the status bar display descriptive messages when the user highlights menu items or toolbar buttons? |
| **Non-Essential** | Is the status bar not the exclusive means of access to essential operations? (User can hide it) |
| **Size Grip** | If the window is sizable, does the status bar include a size grip at its far corner? |
| **Status Field Border** | Do status fields use the status field border style (sunken outer only)? |

---

# PART 6: SECONDARY WINDOWS

Secondary windows supplement primary windows (dialog boxes, property sheets, message boxes, palette windows).

## 6.1. General Characteristics

| Validation Point | Description |
| :--- | :--- |
| **No Taskbar Entry** | Do secondary windows NOT appear on the taskbar? |
| **Title Text** | Does the title text describe the purpose of the window? For dialog boxes, is it the command name (without ellipsis)? |
| **No Min/Max** | Do secondary windows lack Minimize and Maximize buttons? |
| **Close Button** | Is a Close button included to dismiss the window? |
| **On Top of Parent** | Does a dependent secondary window always appear on top of its primary window? |
| **Closed with Parent** | When the primary window is closed/minimized, are its secondary windows also closed/hidden? |

## 6.2. Dialog Boxes

| Validation Point | Description |
| :--- | :--- |
| **OK and Cancel** | Do dialog boxes include OK and Cancel command buttons? |
| **Default Button** | Is the most common/safe button designated as the default button (bold outline)? |
| **Not Destructive Default** | Is a potentially destructive button NOT the default? |
| **Layout Direction** | Are controls oriented left-to-right, top-to-bottom (for western languages)? |
| **Primary Field First** | Is the primary interaction field located near the upper left corner? |
| **Button Placement** | Are major command buttons stacked along the upper right or lined up across the bottom? |
| **Tab Navigation** | Does the TAB key move between controls in a logical sequence? |
| **ESC = Cancel** | Does the ESC key activate the Cancel button? |
| **ENTER = Default** | Does the ENTER key activate the default button? |
| **Input Validation** | Is user input validated as close to the point of input as possible? |

## 6.3. Property Sheets

| Validation Point | Description |
| :--- | :--- |
| **Title Text** | Is the title text the object's name + "Properties"? |
| **Tabbed Pages** | Are properties organized into tabbed pages? |
| **OK, Apply, Cancel** | Are OK, Apply, and Cancel buttons present outside the pages? |
| **Modeless** | Is the property sheet modeless? |
| **Persistence** | Is the last-viewed page restored when the property sheet is reopened? |

## 6.4. Message Boxes

| Validation Point | Description |
| :--- | :--- |
| **Symbols** | Does each message box include the appropriate symbol? Information (i), Warning (!), Critical (X)? |
| **Title Identifies Context** | Does the title bar identify the context (document name, application name)? NOT "Error" or "Warning". |
| **Clear Message Text** | Is the message clear, concise, in user terms, with no technical jargon? |
| **Problem + Cause + Action** | Does the message state the problem, its probable cause, and what the user can do? |
| **Specific Actions** | Are button labels specific actions (Save, Don't Save) rather than generic (Yes, No)? |
| **Safe Default** | Is the most frequent or least destructive option the default button? |
| **No "Error" Word** | Is the word "error" avoided in title text? |
| **No User Blame** | Does the message avoid blaming the user? ("Cannot find filename" not "Filename error") |
| **One Message Per Condition** | Is only one message box displayed for a specific condition, not a sequence? |

## 6.5. Common Dialog Boxes

| Validation Point | Description |
| :--- | :--- |
| **Standard Dialogs Used** | Are system-provided common dialog boxes used where appropriate? (Open, Save As, Find/Replace, Print, Page Setup, Font, Color) |
| **Consistent Extensions** | If custom dialogs are used, are they consistent with the standard ones? |
| **Settings Preserved** | Are the user's latest dialog box settings preserved for subsequent openings? |

---

# PART 7: VISUAL DESIGN

Visual information communicates nonverbally but very powerfully.

## 7.1. Composition and Organization

| Validation Point | Description |
| :--- | :--- |
| **Information Hierarchy** | Is information placed based on relative importance? Most important information at the top/left? |
| **Focus and Emphasis** | Is there a clear focal point? Are priority items emphasized? |
| **Structure and Balance** | Is there an underlying structure and balance? |
| **Relationship of Elements** | Does spatial placement communicate relationships? (e.g., button near the list it affects) |
| **Readability and Flow** | Are ideas communicated directly and simply? Can the user easily step through the interface? |
| **Unity** | Is the visual design unified with the general Windows interface? |

## 7.2. Color

| Validation Point | Description |
| :--- | :--- |
| **Secondary Information** | Is color used as an additive, redundant form of information, not the only means? |
| **Limited Palette** | Are colors limited to a small, purposeful set? Are muted, complementary colors preferred over bright, saturated ones? |
| **User Changeable** | Can the user change colors? Are system color settings used? |
| **Works in Monochrome** | Does the design work on monochrome displays? |
| **System Colors** | Are interface element colors based on system color settings? |

## 7.3. Fonts

| Validation Point | Description |
| :--- | :--- |
| **System Font** | Is the standard system font used for common interface elements? |
| **Limited Fonts** | Is the number of fonts and styles limited? |
| **Avoid Italic/Serif** | Are italic and serif fonts avoided for interface text (hard to read on screen)? |
| **Scalable** | Do fonts scale appropriately when system font settings change? |

## 7.4. Dimensionality (3D Effects)

| Validation Point | Description |
| :--- | :--- |
| **Light Source** | Is the light source from the upper left? (highlights on top/left, shadows on bottom/right) |
| **Real-World Appearance** | Do 3D objects look like their real-world counterparts? |
| **Interactive Only** | Are 3D effects used only for interactive elements, not for decoration? |
| **Sufficient Detail** | Is there enough detail to communicate the real-world association and no more? |

## 7.5. Border Styles

| Validation Point | Description |
| :--- | :--- |
| **Window Borders** | Do primary and secondary windows use the window border style? (raised outer + raised inner) |
| **Button Borders** | Do command buttons use the button border style? (raised outer + raised inner, swapped; sunken when pressed) |
| **Field Borders** | Do text boxes, check boxes, list boxes, combo boxes use the field border style? (sunken outer + sunken inner) |
| **Status Field Borders** | Do status bar fields use the status field border style? (sunken outer only) |
| **Grouping Borders** | Do group boxes and menu separators use the grouping border style? (sunken outer + raised inner) |
| **System Colors** | Are border colors based on system color settings (button face, button highlight, button shadow, window frame)? |

## 7.6. Visual States for Controls

| Validation Point | Description |
| :--- | :--- |
| **Pressed** | Do controls show a pressed appearance on the down transition of the mouse button? (button down border, label shifts 1px right/down) |
| **Option-Set** | When a button represents a set state, does it show the option-set appearance? (checkerboard pattern on button face) |
| **Mixed-Value** | When representing mixed values, is the mixed-value (indeterminate) appearance used? |
| **Unavailable** | When disabled, is the engraved unavailable appearance shown? (button highlight + button shadow offset) |
| **Input Focus** | Is the input focus clearly indicated? |

## 7.7. Icon Design

| Validation Point | Description |
| :--- | :--- |
| **Multiple Sizes** | Are icons provided in multiple sizes (48×48, 32×32, 16×16)? |
| **Recognizable** | Are icons recognizable at all sizes, especially at 16×16? |
| **Consistent Style** | Do icons use a consistent visual style? |
| **3D Perspective** | Do object icons use a 3D perspective with the light source from the upper left? |
| **Distinct** | Are different icons clearly distinguishable from each other? |

## 7.8. Layout and Spacing

| Validation Point | Description |
| :--- | :--- |
| **System Font** | Is the system font (MS Sans Serif, 8pt default) used for interface text? |
| **DLU-Based Spacing** | Are dialog box dimensions based on dialog units (DLUs) rather than pixels? |
| **Capitalization** | Are book title caps used for menu items, button labels, and title bars? Sentence caps for option buttons, check boxes, and other control labels? |
| **Grouping** | Are related controls grouped together with group boxes or spatial proximity? |
| **Alignment** | Are controls and labels aligned for a clean layout? |
| **Button Placement** | Are command buttons at the upper right or bottom of dialog boxes? |

---

# PART 8: USER ASSISTANCE

## 8.1. Context-Sensitive Help (What's This?)

| Validation Point | Description |
| :--- | :--- |
| **What's This? Button** | Is a What's This? button provided in the title bar of secondary windows? |
| **Help Menu Command** | Is a What's This? command available from the Help drop-down menu? |
| **Shift+F1** | Is SHIFT+F1 supported as the shortcut for context-sensitive Help mode? |
| **Pop-up Window** | Is context-sensitive Help displayed in a pop-up window near the object? |
| **Brief and Complete** | Is the Help text brief but complete, starting with a verb, using common terms? |

## 8.2. Tooltips

| Validation Point | Description |
| :--- | :--- |
| **Toolbar Buttons** | Do all toolbar buttons without text labels have tooltips? |
| **Timing** | Is the tooltip displayed after a short time-out when the pointer rests over the control? |
| **Dismissal** | Is the tooltip removed when the user clicks the control or moves off it? |

## 8.3. Status Bar Messages

| Validation Point | Description |
| :--- | :--- |
| **Menu Descriptions** | Are descriptive messages shown when the user highlights menu items? |
| **Present Tense Verbs** | Do messages begin with a verb in present tense? |
| **Disabled Messages** | Do disabled items show an appropriate message explaining why they're unavailable? |

## 8.4. Wizards

| Validation Point | Description |
| :--- | :--- |
| **Navigation Buttons** | Are < Back, Next >, Finish, and Cancel buttons provided? |
| **First Page Graphic** | Does the first page include a graphic on the left side? |
| **Conversational Style** | Is the text conversational, using "you" and "your"? |
| **Simple Pages** | Are pages simple with few choices, rather than complex with many options? |
| **Reasonable Defaults** | Are default values provided for all controls? |

---

# PART 9: INPUT AND INTERACTION

## 9.1. Mouse Input

| Validation Point | Description |
| :--- | :--- |
| **Pointer Feedback** | Does the pointer change appearance to indicate context (arrow, I-beam, resize, wait, etc.)? |
| **Press = Identify** | Does pressing the mouse button identify the operation, and releasing activate it? |
| **Not Exclusive** | Are basic tasks available without requiring double-clicking or dragging? |
| **Button 2 = Pop-up** | Does clicking with mouse button 2 display a pop-up (context) menu? |

## 9.2. Keyboard Input

| Validation Point | Description |
| :--- | :--- |
| **Access Keys** | Are unique access keys defined for menu titles, menu items, and control labels? |
| **Standard Shortcuts** | Are standard shortcut keys used for common operations? (Ctrl+N, Ctrl+O, Ctrl+S, Ctrl+P, Ctrl+Z, Ctrl+X, Ctrl+C, Ctrl+V, F1, etc.) |
| **Not Reassigned** | Are standard shortcut keys NOT reassigned to other operations? |
| **Tab Navigation** | Does the TAB key navigate between controls in secondary windows? |
| **Arrow Navigation** | Do arrow keys navigate within groups (option buttons, list items)? |
| **ESC to Cancel** | Does ESC cancel or stop the current operation? |

## 9.3. Selection

| Validation Point | Description |
| :--- | :--- |
| **Object-Action Paradigm** | Is the object-action paradigm followed? (Select object first, then choose action) |
| **Visual Feedback** | Is the selection always visually indicated (highlighting, handles)? |
| **Immediate Feedback** | Is selection feedback immediate? |
| **Non-Committing** | Does selection not commit the user to any action? |
| **SHIFT+Click Extend** | Does SHIFT+Click extend the selection? |
| **CTRL+Click Toggle** | Does CTRL+Click toggle individual items in a disjoint selection? |

## 9.4. Transfer Operations

| Validation Point | Description |
| :--- | :--- |
| **Cut, Copy, Paste** | Are Cut, Copy, and Paste supported? |
| **Ctrl+X, C, V** | Are Ctrl+X, Ctrl+C, Ctrl+V shortcuts supported? |
| **Drag and Drop** | Is drag and drop supported for transfer operations? |
| **Transfer Feedback** | Is visual feedback provided during drag and drop? (pointer changes for move/copy/link, destination highlighting) |

---

# PART 10: ACCESSIBILITY

## 10.1. Visual

| Validation Point | Description |
| :--- | :--- |
| **Color Not Only Cue** | Is color never the only means of conveying information? |
| **High Contrast** | Does the software support the system's High Contrast Mode? |
| **Scalable Elements** | Are screen elements scalable when the user changes system font/size settings? |
| **System Colors** | Are interface colors based on system color settings? |

## 10.2. Keyboard Access

| Validation Point | Description |
| :--- | :--- |
| **Full Keyboard Access** | Can all interactive elements be reached and activated via keyboard? |
| **Navigate All Objects** | Can the user navigate to all objects? |
| **Single-Click Alternatives** | Are basic functions available without requiring double-click, drag-and-drop, or modifier+mouse actions? |

## 10.3. Screen Reader Compatibility

| Validation Point | Description |
| :--- | :--- |
| **Standard Controls** | Are standard Windows controls used wherever possible? |
| **All Controls Labeled** | Does every control have a label, even if invisible? |
| **Label Placement** | Are static text labels placed before (above or left of) the associated control? |
| **Window Titles** | Do all windows have titles? |
| **Text Output** | Is text output done with standard system interfaces (not bitmapped text)? |

## 10.4. Timing

| Validation Point | Description |
| :--- | :--- |
| **No Critical Time-outs** | Is critical feedback not displayed briefly and then automatically removed? |
| **Configurable Timing** | For time-out-based interfaces, can the user configure the timing? |
| **Navigation Non-Triggering** | Does basic TAB navigation NOT carry out actions (like setting a check box)? |

---

# PART 11: SYSTEM INTEGRATION

## 11.1. Standard Keyboard Shortcuts (Reserved)

These MUST NOT be reassigned:

| Key | Function |
|-----|----------|
| Ctrl+N | New |
| Ctrl+O | Open |
| Ctrl+S | Save |
| Ctrl+P | Print |
| Ctrl+Z | Undo |
| Ctrl+X | Cut |
| Ctrl+C | Copy |
| Ctrl+V | Paste |
| Ctrl+A | Select All |
| Ctrl+F | Find |
| Ctrl+H | Replace |
| F1 | Help |
| Shift+F1 | What's This? mode |
| Shift+F10 | Display pop-up menu |
| Alt+F4 | Close window |
| Alt+Tab | Switch windows |
| ESC | Cancel |
| Delete | Delete |

## 11.2. Title Bar Conventions

| Validation Point | Description |
| :--- | :--- |
| **Document: Name — App** | For documents: "filename — Application Name" |
| **Tool: App Name** | For tools: "Application Name" |
| **MDI: App — Child** | For MDI: "Application Name — filename" (when child is maximized) |
| **Default Names** | For unnamed files: use short type name + number (e.g., "Document 1") |
| **Long Filenames** | Display the filename without extension, matching the file system display |

---

# AUDIT SUMMARY TEMPLATE

After completing the audit, summarize findings:

```
## Audit Summary: [Screen/Feature Name]

### Critical Issues (Fix Immediately)
1. **[Category]**: [Specific observation]
   → **Fix**: [Actionable recommendation with specific Win95 guideline reference]

### Major Issues (Fix Before Launch)
1. **[Category]**: [Specific observation]
   → **Fix**: [Actionable recommendation]

### Minor Issues (Nice to Have)
1. **[Category]**: [Specific observation]
   → **Fix**: [Actionable recommendation]

### Accessibility Violations
1. **[Category]**: [Specific observation]
   → **Fix**: [Actionable recommendation]

### Strengths
- [What's done well]

### Overall Windows Interface Compliance: X/10
```

---

# Quick Reference: The 7 Design Principles

| # | Principle | Question |
|---|-----------|----------|
| 1 | **User in Control** | Does the user always feel in control, not the software? |
| 2 | **Directness** | Can users directly manipulate objects? Are metaphors familiar? |
| 3 | **Consistency** | Same action = same result? Within product, with OS, with metaphors? |
| 4 | **Forgiveness** | Can users recover from mistakes? Undo? Warnings? |
| 5 | **Feedback** | Is there immediate response to every action? Progress for long ops? |
| 6 | **Aesthetics** | Is the visual design clear, purposeful, and uncluttered? |
| 7 | **Simplicity** | Is the interface easy to learn? Progressive disclosure? Concise? |

---

# Common Violations

## Critical
- **No Undo** — Users can't recover from mistakes
- **No Keyboard Access** — Cannot reach controls via keyboard
- **Touch Targets** — Buttons or controls too small to reliably click
- **Blocked User** — Modal dialog prevents Save/Quit
- **Silent Failures** — Errors without any feedback
- **Non-Standard Shortcuts** — Ctrl+C, Ctrl+V, etc. reassigned to other functions

## Major
- **Missing Scroll Bars** — Content exceeds window but no way to scroll
- **No Tooltips** — Toolbar buttons without labels or tooltips
- **Inconsistent Borders** — Wrong border styles for controls (sunken for buttons, raised for fields)
- **Wrong Capitalization** — Sentence caps on menu items, title caps on check boxes
- **No Context Menus** — Missing pop-up menus on right-click
- **Generic Button Labels** — "Yes/No" instead of specific action names
- **Technical Error Messages** — Jargon, error codes, blame language

## Minor
- **Verbose Labels** — Menu items or messages that could be more concise
- **Non-Standard Menu Order** — Menus not following File, Edit, View convention
- **Missing Accelerators** — Common operations without shortcut keys
- **No State Preservation** — Dialog boxes not remembering last settings
- **Weak Visual Hierarchy** — Everything at same visual prominence

---

# Key Win95 Visual Constants

## System Colors (GetSysColor)
| Constant | Typical Value | Use |
|----------|---------------|-----|
| COLOR_BTNFACE | Light gray (192, 192, 192) | Button and toolbar backgrounds |
| COLOR_BTNHIGHLIGHT | White (255, 255, 255) | Light edge of 3D borders |
| COLOR_BTNSHADOW | Dark gray (128, 128, 128) | Dark edge of 3D borders |
| COLOR_WINDOWFRAME | Black (0, 0, 0) | Outermost dark border edge |
| COLOR_WINDOW | White (255, 255, 255) | Text field/list box backgrounds |
| COLOR_WINDOWTEXT | Black (0, 0, 0) | Text in windows |
| COLOR_ACTIVECAPTION | Navy blue (0, 0, 128) | Active title bar |
| COLOR_CAPTIONTEXT | White (255, 255, 255) | Active title bar text |
| COLOR_INACTIVECAPTION | Gray (128, 128, 128) | Inactive title bar |
| COLOR_MENU | Light gray (192, 192, 192) | Menu backgrounds |
| COLOR_MENUTEXT | Black (0, 0, 0) | Menu text |
| COLOR_HIGHLIGHT | Navy blue (0, 0, 128) | Selection highlight |
| COLOR_HIGHLIGHTTEXT | White (255, 255, 255) | Selected text |
| COLOR_GRAYTEXT | Dark gray (128, 128, 128) | Disabled text |

## Standard Sizes
| Element | Size |
|---------|------|
| Toolbar button (small) | 24 × 22 pixels |
| Toolbar button (large) | 32 × 30 pixels |
| Toolbar icon (small) | 16 × 16 pixels |
| Toolbar icon (large) | 24 × 24 pixels |
| Icon (full) | 32 × 32 pixels |
| Icon (large) | 48 × 48 pixels |
| Icon (small) | 16 × 16 pixels |
| System font | MS Sans Serif, 8pt |

---

# Audit Depth Levels

## Quick Audit (5 min)
- Check the 7 design principles only
- Note 3-5 biggest issues
- Immediate recommendations

## Standard Audit (20 min)
- All 11 categories at high level
- Prioritized issue list
- Component spot-checks

## Deep Audit (60+ min)
- Full checklist, every item
- Every component evaluated
- Accessibility deep dive
- All border styles verified
- System color compliance checked
- Keyboard navigation tested

---

# Comparison with Other Guidelines

| Aspect | Apple HIG (1987) | Windows 95 Guidelines | Material Design 3 |
|--------|------------------|-----------------------|-------------------|
| Philosophy | Desktop metaphor | Data-centered design | Physical paper surfaces |
| Elevation | Minimal, flat | 3D beveled borders | 6 tonal levels |
| Color | Conservative | System color-based | 26 semantic roles |
| Motion | Subtle | Minimal | Physics-based springs |
| Typography | Chicago | MS Sans Serif | Roboto, 30 styles |
| Spacing | Various | DLU-based | 8dp grid |
| Touch/Click | 44pt minimum | Not specified (desktop) | 48dp minimum |
| Windows | MDI not recommended | MDI fully supported | N/A (mobile) |
| Help | Apple Guide | What's This?, Wizards | N/A |
| Context Menus | Not standard | Required everywhere | Long-press |
| Property Sheets | N/A | Tabbed, modeless | Bottom sheets |

---

*Based on "The Windows Interface Guidelines — A Guide for Designing Software", Microsoft Corporation, February 1995.*
*Full PDF: https://ics.uci.edu/~kobsa/courses/ICS104/course-notes/Microsoft_WindowsGuidelines.pdf*
