# Sun Java Look and Feel Design Guidelines — Audit & Validation Guide

This document provides a comprehensive set of instructions and validation criteria to audit a user interface (UI) or user experience (UX) against Sun Microsystems' *Java Look and Feel Design Guidelines* (1999). It is intended for AI assistants to provide detailed feedback on design implementations targeting Java Swing/JFC applications using the "Metal" look and feel.

The guide is structured into key areas of the Java look and feel design system, with each section containing specific, actionable validation points.

*Source: "Java Look and Feel Design Guidelines", Sun Microsystems, Inc., 1999 (Addison-Wesley, ISBN 0-201-61585-1)*

---

## How to Use This Guide

When auditing a design (screen, interaction, component, or full application):

1. Request a screenshot, description, or code of the UI being audited
2. Walk through each category below systematically
3. For each validation point, note: **PASS**, **FAIL**, or **N/A**
4. Provide specific, actionable feedback for every failure
5. Summarize with a priority-ranked list of issues to fix

---

# PART 1: DESIGN FUNDAMENTALS

## 1.1. Visual Identity — The Three Hallmarks

| Validation Point | Description |
| :--- | :--- |
| **Flush 3D Style** | Do component surfaces appear at the same level as the surrounding canvas? The Java look and feel uses flush 3D (not beveled) for a clean, modern appearance. |
| **Drag Texture** | Is the textured pattern used consistently to indicate draggable items (toolbar drag areas, scroll boxes, split pane dividers, sliders)? |
| **Color Model** | Does the application use the 8-color model (3 primary, 3 secondary, black, white) for compatibility across platforms and color depths? |

## 1.2. Cross-Platform Considerations

| Validation Point | Description |
| :--- | :--- |
| **Platform Window Frames** | Do primary windows use the native platform's borders, title bars, and window controls? (Java L&F controls content only.) |
| **Two-Button Mouse** | Is the design based on a two-button mouse? (Button 1 for selection/activation, Button 2 for contextual menus. No middle button usage.) |
| **PC-Style Keyboard** | Does the design use standard PC keyboard conventions? (Shift, Control, Alt modifiers. No Meta/Command key except as alternate for Control.) |
| **Cross-Platform Colors** | Are graphics designed to dither gracefully on 8-bit displays? Is the pre-dithering pattern technique used for large solid-color areas? |

## 1.3. Accessibility

| Validation Point | Description |
| :--- | :--- |
| **Accessible Names** | Does every component have an `accessibleName` property set? |
| **Accessible Descriptions** | Do interactive components have `accessibleDescription` set (or a descriptive tool tip)? |
| **Full Keyboard Access** | Can all functions be accessed from the keyboard? (Unplug the mouse and test.) |
| **Mnemonics Everywhere** | Are mnemonics provided for all menu titles, menu items, buttons, and labeled controls? |
| **Initial Keyboard Focus** | Is initial keyboard focus assigned to the component users would normally use first in each window? |
| **Tab Traversal Order** | Is a logical tab traversal order specified matching the reading order of the target locale? |
| **Color Not Only Cue** | Is color used only as a secondary means of representing information? Are shape, texture, size, or intensity contrast also used? |

## 1.4. Internationalization

| Validation Point | Description |
| :--- | :--- |
| **Resource Bundles** | Are all text, colors, graphics, fonts, and mnemonics stored in resource bundles? |
| **Layout Managers** | Are layout managers used to accommodate varying text lengths across locales? |
| **Reading Order** | Are components placed according to reading order (left-to-right for English, right-to-left for Arabic/Hebrew)? |
| **No Hardcoded Text** | Is text never written directly in source code? |
| **Word Order** | Are sentences structured so word order changes in translation don't break the UI? (Use label + control, not inline text.) |
| **Global Graphics** | Are globally understood icons and symbols used? Are locale-specific graphics in resource bundles? |

---

# PART 2: VISUAL DESIGN

## 2.1. Color Theme (8-Color Model)

| Validation Point | Description |
| :--- | :--- |
| **Primary 1** | Used for active window borders, shadows of selected items, and labels? Default: RGB 102-102-153 (#666699). |
| **Primary 2** | Used for selected menu titles/items, active scroll boxes, progress bar fill, focus indication? Default: RGB 153-153-204 (#9999CC). |
| **Primary 3** | Used for large colored areas (active title bars, text selection)? Default: RGB 204-204-255 (#CCCCFF). |
| **Secondary 1** | Used for dark border in flush 3D effects? Default: RGB 102-102-102 (#666666). |
| **Secondary 2** | Used for inactive window borders, shadows, pressed buttons, dimmed text? Default: RGB 153-153-153 (#999999). |
| **Secondary 3** | Used for canvas/background color, inactive title bars? Default: RGB 204-204-204 (#CCCCCC). |
| **Black** | Used for user text and control text? Default: RGB 0-0-0 (#000000). |
| **White** | Used for highlights and user text entry background? Default: RGB 255-255-255 (#FFFFFF). |
| **Dark-to-Light Gradation** | Is the dark-to-light gradation maintained? (Primary 1 darkest → Primary 3 lightest; same for Secondary.) |
| **Primary 1 Contrast** | Does Primary 1 (labels) have enough contrast with Secondary 3 (background) to be readable? |

## 2.2. Typography

| Validation Point | Description |
| :--- | :--- |
| **Control Font** | 12-point bold Dialog — used for buttons, checkboxes, menu titles, window titles? |
| **System Font** | 12-point plain Dialog — used for tree views and tool tips? |
| **User Font** | 12-point plain Dialog — used for text fields and tables? |
| **Small Font** | 10-point plain Dialog — used for keyboard shortcuts in menus and tool tips? |
| **No Hardcoded Fonts** | Are font sizes and styles never written directly into source code? |
| **Theme Mechanism** | Are all font changes made through the theme mechanism? |
| **Legible on Targets** | Are font settings legible and well-rendered on target systems? |

## 2.3. Capitalization

| Validation Point | Description |
| :--- | :--- |
| **Headline Caps** | Used for: checkbox text, combo box labels/text, button text, icon names, group labels, menu items/titles, radio button text, slider text, tab names, text field labels, window/dialog titles, tool tips? |
| **Sentence Caps** | Used for: dialog box body text, error/help messages, status labels? |
| **No Automatic Caps** | Is text never capitalized automatically? (User-named titles may be lowercase.) |

## 2.4. Layout and Spacing

| Validation Point | Description |
| :--- | :--- |
| **6-Pixel Base Unit** | Are multiples of 6 pixels used for perceived spacing between components? |
| **5px Within Groups** | Is actual spacing 5px (perceived 6px) between closely related items (grouped checkboxes, radio buttons)? |
| **11px Between Groups** | Is actual spacing 11px (perceived 12px) between sets of components? |
| **12px Border Padding** | Is 12px between titled border edges and the components within? |
| **Design Grid** | Is a consistent grid used for layout with standard margins and column guides? |
| **Reading Order** | Are the most important components placed first in reading order? |
| **Label Orientation** | Are labels placed before (left of or above in English) the components they describe? |
| **Label Alignment** | Are labels aligned with the top of associated components? |

## 2.5. Animation and Progress

| Validation Point | Description |
| :--- | :--- |
| **Not Overused** | Is animation used sparingly and not distracting? |
| **Accessible Description** | Do animated elements have `accessibleDescription` set for screen readers? |
| **Wait Pointer** | Does the pointer change during long operations? |
| **Progress Bar** | Is a progress bar used for operations with known length/count? |
| **2-Second Rule** | Is a progress dialog displayed if an operation takes longer than 2 seconds? |
| **User Configurable** | Can system status animation be configured by users? |

---

# PART 3: APPLICATION GRAPHICS

## 3.1. Icons

| Validation Point | Description |
| :--- | :--- |
| **Standard Sizes** | Are icons 16×16 or 32×32 pixels? |
| **Clearly Representative** | Do icons clearly identify the objects or concepts they represent? |
| **Symbolic Style** | Is the drawing style symbolic rather than photo-realistic? |
| **Consistent Family** | Do related icons share a consistent visual style (palette, size, style)? |
| **Dark Border** | Do icons have a clear, dark exterior border without anti-aliasing at the perimeter? |
| **Transparent Background** | Is the area around the icon graphic defined as transparent pixels in the GIF? |
| **Flush 3D Interior** | Do icons use an interior highlight (top-left) for the flush 3D style? |
| **Pre-Dithered** | Are large solid-color areas pre-dithered for 8-bit display compatibility? |
| **Accessible** | Do all icons have `accessibleDescription` and `accessibleName` set? |

## 3.2. Button Graphics

| Validation Point | Description |
| :--- | :--- |
| **Standard Sizes** | Are button graphics 16×16 or 24×24 pixels? |
| **Flush 3D Effect** | Is the flush 3D effect applied? (Exterior white highlight bottom/right, interior highlight left/top.) |
| **Primary Drawing Area** | Is drawing confined to the primary drawing area (13×13 for 16px, 21×21 for 24px)? |
| **2px Border Clearance** | Are there 2px between the dark border of the graphic and the button border on all sides? |
| **No Embedded Text** | Is text never included as part of button graphics (GIF files)? Use button text instead. |
| **Clear Action** | Does the graphic clearly show the action, state, or mode the button represents? |
| **GIF Format** | Are button graphics in GIF format (not JPEG)? |

## 3.3. Splash Screens & About Boxes

| Validation Point | Description |
| :--- | :--- |
| **JWindow for Splash** | Are splash screens implemented using JWindow (plain window)? |
| **1px Black Border** | Do splash screens include a 1-pixel black border as part of the image? |
| **Company Branding** | Do splash screens include company logo, product name, and visual identifier? |
| **About Box Accessible** | Is the About box accessible from the Help menu ("About *Application*")? |
| **About Box Close** | Does the About box include a Close button? |

---

# PART 4: BEHAVIOR

## 4.1. Mouse Operations

| Validation Point | Description |
| :--- | :--- |
| **Button 1 Actions** | Is mouse button 1 used for selection, activation, dragging, and drop-down menus? |
| **Button 2 Context** | Is mouse button 2 used only for contextual menus? |
| **No Middle Button** | Is the middle mouse button never used? |
| **Click = Select** | Does clicking select or activate? Does double-click select words? Does triple-click select lines? |
| **Pointer Feedback** | Does the pointer change shape to indicate context (default, text, wait, resize, etc.)? |
| **Mouse-over Feedback** | Are toolbar button borders shown on mouse-over? |

## 4.2. Keyboard Operations

| Validation Point | Description |
| :--- | :--- |
| **Tab Navigation** | Does Tab move between major components? Shift-Tab reverses? |
| **Ctrl-Tab for Tabs** | Does Ctrl-Tab work in components that accept Tab (text areas, tables)? |
| **Arrow Keys Within** | Do arrow keys move within groups (radio buttons, menu items, toolbar buttons)? |
| **Spacebar Activates** | Does Spacebar activate/select the focused component? |
| **Enter = Default** | Does Enter activate the default button (when focus is not on Enter-accepting component)? |
| **Escape = Cancel** | Does Escape dismiss menus/dialogs without changes? |
| **F10 = Menu** | Does F10 move focus to the menu bar? |

## 4.3. Keyboard Shortcuts (Common)

These must be supported:

| Key | Function |
|-----|----------|
| Ctrl+N | New (File menu) |
| Ctrl+O | Open (File menu) |
| Ctrl+S | Save (File menu) |
| Ctrl+P | Print (File menu) |
| Ctrl+W | Close (File menu) |
| Ctrl+Z | Undo (Edit menu) |
| Ctrl+Y | Redo (Edit menu) |
| Ctrl+X | Cut (Edit menu) |
| Ctrl+C | Copy (Edit menu) |
| Ctrl+V | Paste (Edit menu) |
| Ctrl+F | Find (Edit menu) |
| Ctrl+G | Find Again (Edit menu) |
| Ctrl+A | Select All (Edit menu) |

## 4.4. Mnemonics

| Validation Point | Description |
| :--- | :--- |
| **All Menu Titles** | Does every menu title have a mnemonic? |
| **All Menu Items** | Does every menu item have a mnemonic? |
| **All Buttons/Controls** | Do all buttons and labeled controls (except default and Cancel) have mnemonics? |
| **No Conflicts** | Are mnemonics unique within their scope (menu, dialog)? |
| **No Default/Cancel** | Do the default button and Cancel button NOT have mnemonics? (Use Enter/Escape instead.) |
| **Common Mnemonics** | Are standard mnemonics used? (File: F, Edit: E, Help: H; New: N, Open: O, Save: S, Exit: X, Undo: U, Cut: T, Copy: C, Paste: P, Find: F, Select All: A) |

## 4.5. Drag and Drop

| Validation Point | Description |
| :--- | :--- |
| **Pointer Change** | Does the pointer change when a drag begins? |
| **Destination Feedback** | Are drop targets highlighted when the pointer is over them? |
| **MIME Types** | Are dragged objects specified by MIME types for cross-application compatibility? |

---

# PART 5: WINDOWS AND CONTAINERS

## 5.1. Primary Windows (JFrame)

| Validation Point | Description |
| :--- | :--- |
| **Native Frame** | Does the window use the native platform's border, title bar, and controls? |
| **Content is JLF** | Does the interior content (menu bar, toolbar, editor) use the Java look and feel? |
| **Close/Exit Menu** | Does the File menu include a Close or Exit item (in addition to the platform close control)? |
| **Secondary Tracking** | When the primary window closes/minimizes, are associated secondary windows closed/hidden? |

## 5.2. Secondary Windows (JDialog)

| Validation Point | Description |
| :--- | :--- |
| **Close Button** | Does every dialog/alert include a way to close it in its own content (not just title bar)? |
| **Appropriate Modality** | Is modeless used whenever possible? Is modal used only when interaction cannot proceed? |

## 5.3. Scroll Panes

| Validation Point | Description |
| :--- | :--- |
| **Proportional Scroll Box** | Does the scroll box size reflect the proportion of visible content? |
| **Minimum 16×16** | Is the scroll box at least 16×16 pixels? |
| **Horizontal Default** | Do horizontal scrollbars appear only when needed? |
| **Vertical Consistency** | If the scroll pane sometimes needs a vertical scrollbar, is it always present? |
| **Overlap on Page** | When clicking in the channel, does the view scroll one page minus one line of overlap? |
| **One Unit on Arrow** | Does clicking a scroll arrow scroll one small unit (one line of text, one row, 10-20px)? |

## 5.4. Tabbed Panes

| Validation Point | Description |
| :--- | :--- |
| **No Nesting** | Are tabbed panes never nested? |
| **Headline Caps** | Do tab names use headline capitalization? |
| **Mnemonics** | Do tabs have mnemonics for keyboard navigation? |
| **Limited Rows** | If multiple rows are required, has the content been reconsidered for multiple dialogs? |

## 5.5. Split Panes

| Validation Point | Description |
| :--- | :--- |
| **Zoom Buttons** | Are zoom buttons included in split panes? |
| **Drag Texture** | Is the Java look and feel drag texture visible on the splitter bar? |

## 5.6. Internal Frames (MDI)

| Validation Point | Description |
| :--- | :--- |
| **JLF Controls** | Do internal frames have Java look and feel borders, title bars, and window controls? |
| **Minimize/Maximize/Close** | Are minimize, maximize, and close controls present? |
| **Desktop Pane** | Are internal frames contained in a desktop pane? |

---

# PART 6: DIALOG BOXES AND ALERTS

## 6.1. Dialog Box Design

| Validation Point | Description |
| :--- | :--- |
| **Title Format** | Is the title formatted as "Application Name: Title"? |
| **Mnemonics** | Are mnemonics provided for all controls except the default button and Cancel? |
| **Initial Focus** | Is initial keyboard focus set to the first interactive component? |
| **12px Top/Left Border** | Is there 12px between the top/left borders and components? |
| **11px Bottom/Right Border** | Is there 11px between the bottom/right borders and components? |
| **Tab Traversal** | Is tab traversal order logical and matches reading order? |
| **Escape = Cancel** | Does Escape activate Cancel? (Must be manually implemented.) |

## 6.2. Command Buttons in Dialogs

| Validation Point | Description |
| :--- | :--- |
| **Bottom Placement** | Are dialog-wide command buttons in a row at the bottom? |
| **Right-Aligned** | Is the button row aligned to the lower-right? |
| **Help Last** | Is the Help button the last (rightmost for LTR) button? |
| **Default Button** | Is the most likely/safe action the default? Does it have a heavy border? |
| **No Unsafe Default** | Is a potentially destructive button never the default? |
| **OK/Cancel** | For settings dialogs: do OK and Cancel dismiss the dialog? |
| **Apply/Reset/Close** | For persistent dialogs: Apply commits, Reset restores, Close dismisses without applying? |
| **5px Spacing** | Are buttons spaced 5px apart? |
| **12px Padding** | Is there 12px padding on each side of the widest button text, with all buttons matching that width? |

## 6.3. Alert Boxes (JOptionPane)

| Validation Point | Description |
| :--- | :--- |
| **Correct Symbol** | Info = blue circle with "i", Warning = yellow triangle with "!", Error = red octagon with rectangle, Question = green rectangle with "?"? |
| **Bold Heading** | Does the message begin with a brief heading in boldface, followed by body text on a separate line? |
| **Info: Close** | Does Info alert use a Close button? |
| **Warning: Two Buttons** | Does Warning alert have at least two buttons (action + cancel)? |
| **Warning: No Unsafe Default** | Is the destructive action NOT the default in a Warning alert? |
| **Error: Error Number** | Does the Error alert include an error number in the title bar? |
| **Error: Explain + Action** | Does the Error message explain what happened, the cause, and what the user can do? |
| **Question: Alignment** | Are additional components aligned with the leading edge of the message? |
| **Descriptive Buttons** | Do buttons have labels that describe the action (Save, Don't Save) rather than generic (Yes, No)? |

## 6.4. Common Dialogs

| Validation Point | Description |
| :--- | :--- |
| **Find** | Is the Find dialog modeless? |
| **Login** | Does the login dialog provide a way to exit without logging in? |
| **Preferences** | Is Preferences modeless? |
| **Print** | Is the AWT-supplied print dialog used? |
| **Progress** | Displayed for ops > 2 seconds? Auto-closes on completion? Has Cancel/Stop button? |

---

# PART 7: MENUS

## 7.1. Menu Bar

| Validation Point | Description |
| :--- | :--- |
| **Position** | Is the menu bar a single line across the top of the primary window, below the title bar? |
| **Single-Word Titles** | Are menu titles single words? |
| **Mnemonics** | Does every menu title have a mnemonic? |
| **No Applet Menu** | If the applet runs in the browser's current window, is there no applet menu bar? |

## 7.2. Standard Menu Order

| Validation Point | Description |
| :--- | :--- |
| **Order** | Are menus ordered: File, Object, Edit, Format, View, [app-specific], Help? |
| **File First** | Is the first menu File (or an appropriate object name like "Project" or "Mailbox")? |
| **Help Last** | Is Help always the last menu? |
| **Exit Not Quit** | Is the term "Exit" used (not "Quit")? |

## 7.3. Standard Menu Contents

| Validation Point | Description |
| :--- | :--- |
| **File Menu** | Contains New, Open, Close, Save, Save As, Page Setup, Print, Preferences, Exit? |
| **Edit Menu** | Contains Undo, Redo, Cut, Copy, Paste, Find, Find Again, Select All? |
| **Format Menu** | Contains formatting commands (Font, Size, Style, Character, Paragraph)? |
| **Help Menu** | Contains Contents, Tutorial, Index, Search, About Application? |

## 7.4. Menu Items

| Validation Point | Description |
| :--- | :--- |
| **Headline Caps** | Are all menu items in headline capitalization? |
| **Mnemonics** | Does every menu item have a unique mnemonic within its menu? |
| **Keyboard Shortcuts** | Are frequently used items given keyboard shortcuts? |
| **Ellipsis Convention** | Do items needing additional specification use ellipsis (...)? Items whose entire effect is showing a window do NOT use ellipsis. |
| **Unavailable = Dimmed** | Are unavailable items dimmed (not removed)? If nothing can make it available, omit entirely. |
| **Menu Stays Open** | If all items in a menu are unavailable, does the menu title remain available? |
| **Separators** | Are related items grouped with separators? |
| **Checkbox Items** | Are checkboxes used for independent (non-exclusive) settings? |
| **Radio Items** | Are radio buttons used for exclusive settings? Are they grouped with separators? |
| **Consistent Shortcuts** | If the same item appears in multiple menus, does it use the same shortcut? |

## 7.5. Submenus

| Validation Point | Description |
| :--- | :--- |
| **No Double Nesting** | Are second-level submenus avoided? (Use a dialog instead.) |
| **Arrow Indicator** | Is a submenu indicated by an arrow next to the parent item? |

## 7.6. Contextual Menus

| Validation Point | Description |
| :--- | :--- |
| **Also in Menu Bar** | Are all contextual menu items also available in the menu bar or toolbar? |
| **Object-Specific** | Do commands apply only to the selected object or group? |
| **Button 2** | Is the contextual menu displayed with mouse button 2 (Control+click on Mac)? |
| **Shortcuts Consistent** | Are keyboard shortcuts and mnemonics consistent with their drop-down menu equivalents? |

---

# PART 8: TOOLBARS

## 8.1. Toolbar Design

| Validation Point | Description |
| :--- | :--- |
| **Position** | Is the toolbar horizontally below the menu bar? |
| **Single Row** | Is there only a single toolbar row? |
| **Drag Area** | Is a textured drag area on the leading edge? |
| **Commonly Used Items** | Are toolbar buttons drawn from commonly used menu items? |
| **Accessible Without Menus** | If no menus exist, do toolbar buttons have text identifiers with mnemonics? |

## 8.2. Toolbar Buttons

| Validation Point | Description |
| :--- | :--- |
| **Graphics 16×16 or 24×24** | Are button graphics one consistent size? |
| **2px Spacing** | Are individual buttons spaced 2px apart? |
| **11px Group Spacing** | Are groups of buttons spaced 11px apart? |
| **3px Top/2px Bottom Padding** | Is there 3px above and 2px below buttons? |
| **Mouse-over Borders** | If rollover borders are used, are buttons spaced 0px apart? |
| **Tool Tips** | Do all toolbar buttons without text have tool tips? |
| **Drop-Down Arrow** | Do toolbar buttons with menus include a drop-down arrow in the graphic? |

---

# PART 9: BASIC CONTROLS

## 9.1. Command Buttons

| Validation Point | Description |
| :--- | :--- |
| **Headline Caps** | Is button text in headline capitalization? |
| **Centered Text** | Is button text centered? |
| **Ellipsis** | Do buttons needing further specification use ellipsis? |
| **Default Heavy Border** | Does the default button have a heavier border? |
| **No Unsafe Default** | Is the default never a data-losing action? |
| **Consistent Width** | Are buttons in a group all the same width (matching the widest)? |
| **12px Inner Padding** | Is there 12px padding on each side of the widest button text? |
| **5px Between Buttons** | Are buttons in a row spaced 5px apart? |
| **Pressed State** | Does the button show a pressed appearance on click? |
| **Unavailable State** | Are unavailable buttons dimmed? |

## 9.2. Toggle Buttons

| Validation Point | Description |
| :--- | :--- |
| **Highlighted = On** | Is the "on" state indicated by a highlighted background? |
| **Independent = Checkbox** | Are independent toggle buttons (like Bold) used in toolbars? (Use checkboxes in dialogs.) |
| **Exclusive = Radio Set** | Are exclusive toggle buttons (like alignment) grouped? |
| **2px Spacing** | Are toggle buttons in a radio set spaced 2px apart? |

## 9.3. Checkboxes

| Validation Point | Description |
| :--- | :--- |
| **Standard Graphic** | Is the JFC-supplied checkbox graphic used? |
| **Text Right of Graphic** | Is text to the right of the checkbox? (Left for RTL locales.) |
| **5px Spacing** | Are checkboxes spaced 5px apart? |
| **Independent Choices** | Are checkboxes used only for independent on/off choices? |

## 9.4. Radio Buttons

| Validation Point | Description |
| :--- | :--- |
| **Standard Graphic** | Is the JFC-supplied radio button graphic used? |
| **Text Right of Graphic** | Is text to the right of the radio button? (Left for RTL locales.) |
| **5px Spacing** | Are radio buttons spaced 5px apart? |
| **Exclusive Choices** | Are radio buttons used only for mutually exclusive choices? |

## 9.5. Combo Boxes

| Validation Point | Description |
| :--- | :--- |
| **Headline Caps** | Are items in headline capitalization? |
| **Label with Mnemonic** | Does every combo box have a label with a mnemonic? |
| **Noneditable = Gray BG** | Do noneditable combo boxes have gray background? |
| **Editable = White BG** | Do editable combo boxes have white background? |
| **Case-Insensitive Input** | Does editable input match case-insensitively where possible? |

## 9.6. Sliders

| Validation Point | Description |
| :--- | :--- |
| **Major/Minor Ticks** | Are tick marks provided with labels for large/small divisions? |
| **Text Field for Exact** | If exact value matters, is a text field provided alongside? |
| **Filling vs Non-Filling** | Is the appropriate slider style chosen for the context? |

## 9.7. Progress Bars

| Validation Point | Description |
| :--- | :--- |
| **Fills Left-to-Right** | Does the progress bar fill left-to-right (or bottom-to-top for vertical)? |
| **Concise Text** | If text is inside the progress bar, is it concise? |
| **No User Interaction** | Is the progress bar read-only? (Use a slider if the user should set a value.) |

---

# PART 10: TEXT COMPONENTS

## 10.1. Labels

| Validation Point | Description |
| :--- | :--- |
| **Brief Text** | Is label text brief and in familiar terminology? |
| **Headline Caps + Colon** | Do identifying labels use headline caps and end with a colon? |
| **Sentence Caps for Status** | Do status labels use sentence capitalization? |
| **12px Before Component** | Is there 12px between the label and its component? |
| **Before in Reading Order** | Is the label before (left of or above in English) the component? |
| **Aligned with Top** | Is the label aligned with the top of the component? |
| **Active/Inactive Match** | Does the label's active/inactive state match its component? |
| **Mnemonic for labelFor** | If a text field needs a mnemonic, is it in the label via `labelFor`? |

## 10.2. Text Fields

| Validation Point | Description |
| :--- | :--- |
| **Noneditable = Gray BG** | Do noneditable text fields have Secondary 3 (gray) background? |
| **Editable = White BG** | Do editable text fields have white background with blinking insertion cursor? |
| **Input Validation** | Are invalid characters rejected immediately (system beep)? Three errors → Error alert? |
| **Enter/Return Commits** | Does Enter/Return or moving focus commit the entry? |

## 10.3. Text Areas and Editor Panes

| Validation Point | Description |
| :--- | :--- |
| **Word Wrap** | Is word wrap enabled by default for text areas? |
| **Scroll Pane** | If text exceeds the area, is it inside a scroll pane? |
| **Correct Editor Kit** | Is the appropriate editor kit used (Default, Styled, RTF, HTML)? |

---

# PART 11: LISTS, TABLES, AND TREES

## 11.1. Lists

| Validation Point | Description |
| :--- | :--- |
| **Whole Lines** | Does the list always display a whole number of lines on resize? |
| **Scroll When Needed** | Do scrollbars appear only when needed (default)? |
| **Correct Selection Model** | Is the correct selection model used? (Single, single range, multiple ranges.) |
| **Shift+Click Extends** | Does Shift+click extend the selection? |
| **Ctrl+Click Toggles** | Does Ctrl+click toggle individual items for disjoint selections? |

## 11.2. Tables

| Validation Point | Description |
| :--- | :--- |
| **Whole Lines** | Does the table always display a whole number of lines on resize? |
| **Column Headers** | Are column headers present (table in scroll pane)? |
| **Focus Inner Border** | Does the focused cell show an inner border in Primary 1? |
| **Sort Indicator** | Is the sorted column indicated (e.g., bold header)? |
| **Sort in View Menu** | Is row sorting also available from a menu? |
| **Correct Selection Model** | Is one of the 9 recommended selection models used? |

## 11.3. Tree Views

| Validation Point | Description |
| :--- | :--- |
| **Root Hidden** | Is the root node hidden (showing second level as highest)? |
| **Turners Everywhere** | Do all containers show turners, including at the highest level? |
| **Right Turner = Collapsed** | Does a right-pointing turner mean collapsed, and down-pointing mean expanded? |
| **Consistent Icons** | Are container and leaf icons consistent (using a "family" style)? |

---

# AUDIT SUMMARY TEMPLATE

After completing the audit, summarize findings:

```
## Audit Summary: [Screen/Feature Name]

### Critical Issues (Fix Immediately)
1. **[Category]**: [Specific observation]
   → **Fix**: [Actionable recommendation with JLF guideline reference]

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

### Overall Java Look and Feel Compliance: X/10
```

---

# Quick Reference: Key Visual Constants

## 8-Color Model (Default Theme)

| Name | RGB | Hex | Role |
|------|-----|-----|------|
| Primary 1 | 102-102-153 | #666699 | Active borders, shadows, labels |
| Primary 2 | 153-153-204 | #9999CC | Selection highlight, focus |
| Primary 3 | 204-204-255 | #CCCCFF | Active title bars, text selection |
| Secondary 1 | 102-102-102 | #666666 | Dark 3D border |
| Secondary 2 | 153-153-153 | #999999 | Inactive borders, dimmed text |
| Secondary 3 | 204-204-204 | #CCCCCC | Background canvas |
| Black | 0-0-0 | #000000 | User/control text |
| White | 255-255-255 | #FFFFFF | Highlights, text entry bg |

## Standard Sizes

| Element | Size |
|---------|------|
| Icon (small) | 16 × 16 px |
| Icon (large) | 32 × 32 px |
| Button graphic (small) | 16 × 16 px |
| Button graphic (large) | 24 × 24 px |
| Drawing area (16px) | 13 × 13 px |
| Drawing area (24px) | 21 × 21 px |
| Scroll box minimum | 16 × 16 px |

## Spacing Rules

| Context | Actual px | Perceived px |
|---------|-----------|-------------|
| Within groups | 5 | 6 |
| Between groups | 11 | 12 |
| Border to component (top/left) | 12 | 12 |
| Border to component (bottom/right) | 11 | 12 |
| Toolbar buttons (within group) | 2 | 3 |
| Toolbar button groups | 11 | 12 |
| Toolbar top padding | 3 | 3 |
| Toolbar bottom padding | 2 | 3 |
| Label to component | 12 | 12 |
| Command buttons in row | 5 | 6 |
| Command button inner padding | 12 | 12 |

## Type Styles

| Style | Default Font | Uses |
|-------|-------------|------|
| Control | 12pt bold Dialog | Buttons, checkboxes, menus, titles |
| Small | 10pt plain Dialog | Shortcuts, tool tips |
| System | 12pt plain Dialog | Tree views, tool tips |
| User | 12pt plain Dialog | Text fields, tables |

---

# Common Violations

## Critical
- **No Keyboard Access** — Functions unreachable without mouse
- **No Mnemonics** — Violates JLF standard; excludes keyboard users
- **Missing Accessible Names** — Screen readers cannot identify components
- **Hardcoded Text** — Blocks internationalization
- **No Escape = Cancel** — Users cannot dismiss dialogs with keyboard

## Major
- **Wrong Border Style** — Beveled instead of flush 3D
- **Inconsistent Colors** — Not using the 8-color model
- **Missing Tool Tips** — Toolbar buttons without text or tips
- **Wrong Capitalization** — Sentence caps on menu items, headline caps on status messages
- **No Progress Indication** — Operations > 2 seconds with no feedback
- **Nested Tabbed Panes** — Explicitly forbidden
- **Unsafe Default Button** — Data-destroying action is the default

## Minor
- **Non-6px Spacing** — Inconsistent inter-component spacing
- **Missing Pre-Dithering** — Solid colors dither badly on 8-bit displays
- **Icon Style Mixing** — 2D and 3D icons in the same family
- **Verbose Labels** — Labels longer than necessary

---

# Audit Depth Levels

## Quick Audit (5 min)
- Check the 3 hallmarks (flush 3D, drag texture, color model)
- Check accessibility basics (keyboard access, mnemonics)
- Note 3-5 biggest issues

## Standard Audit (20 min)
- All 11 categories at high level
- Prioritized issue list
- Component spot-checks

## Deep Audit (60+ min)
- Full checklist, every item
- Every component evaluated
- Accessibility deep dive
- All spacing verified pixel-by-pixel
- Color model compliance checked
- Keyboard navigation tested for all components
- Cross-platform color reproduction tested

---

# Comparison with Other Guidelines

| Aspect | Apple HIG (1987) | Windows 95 | Java Look & Feel | Material Design 3 |
|--------|-----------------|------------|------------------|-------------------|
| Philosophy | Desktop metaphor | Data-centered | Cross-platform consistency | Physical paper surfaces |
| 3D Style | Minimal | Beveled borders | Flush 3D | Tonal elevation |
| Color | Conservative | System colors | 8-color model | 26 semantic roles |
| Motion | Subtle | Minimal | Animated pointers, progress | Physics-based springs |
| Typography | Chicago | MS Sans Serif | Dialog (platform-mapped) | Roboto, 30 styles |
| Spacing | Various | DLU-based | 6px multiples | 8dp grid |
| Touch/Click | 44pt min | Not specified | Not specified (desktop) | 48dp minimum |
| Cross-platform | No | No | Primary goal | Cross-platform |
| Help | Apple Guide | What's This? | JavaHelp | N/A |
| Context Menus | Not standard | Required | Recommended | Long-press |
| Pluggable L&F | No | No | Yes (core feature) | Via theming |

---

*Based on "Java Look and Feel Design Guidelines", Sun Microsystems, Inc., 1999.*
*Addison-Wesley, ISBN 0-201-61585-1.*
