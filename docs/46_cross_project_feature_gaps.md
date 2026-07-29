# Cross-Project Feature Gaps

Analysis of four projects using afterhours to identify missing UI features, ergonomics improvements, and new primitives that would benefit all of them.

## Status
- **#5 Confirmation Dialog — DONE.** It already existed as `modal::confirm` (plus
  `modal::info`, `modal::fyi`); those were overhauled for looks (wrapping message,
  right-aligned action row, visible neutral buttons, no overlap/escape) and two
  presets were added: **`modal::confirm_danger`** (destructive, Error-red action) and
  **`modal::prompt`** (text-input dialog). See `vendor/afterhours/src/plugins/modal.h`,
  the `Dialog*Showcase` screens (headless-validated), and `tests/dialog_test.cpp`.
- Remaining proposals below are still open, in the priority order in the matrix.

---

## Project Summaries

### T.O.S.S. (macintosh)
**Type:** Foddy-like 3D juggling game with diegetic CRT-terminal UI
**UI Needs:** Boot sequence (typewriter text), COMMS dialogue box (portrait + typewriter text + dismiss), HUD telemetry overlay, main menu, pause menu, settings, failure screen with diagnostic data
**Aesthetic:** Neon-industrial, CRT shader, wireframe, aggressive typography

### Pub Panic! (pharmasea)
**Type:** Overcooked-like 3D co-op bar management with automation (local + networked multiplayer)
**UI Needs:** In-world speech bubbles, recipe book, shop/upgrade screen, settings with i18n, save slot management, planning phase build UI, pause menu, customer order indicators, progress bars on machines, tooltip hints
**Aesthetic:** 3D isometric, warm/casual, physical interactions

### Wordproc (wordproc)
**Type:** Word processor application with Win95-style UI
**UI Needs:** Menu bar with dropdowns, toolbar with icon buttons, scroll views, text editing canvas, find/replace dialog, file open/save dialogs, status bar, ruler, formatting panel, font picker, spellcheck underlines
**Aesthetic:** Win95 retro, precise pixel-aligned widgets, document-centric

### Endless Dance Chaos (endless_dance_chaos)
**Type:** Mini Metro-style festival crowd management
**UI Needs:** Top status bar, Google Calendar-style vertical timeline sidebar, build tool bar with cycling selection, minimap with camera viewport, density heatmap overlay, game over stats screen, main menu, pause menu
**Aesthetic:** RCT2-style pastel isometric, Miami day / EDC neon night palettes

### Cartographer (cartographer)
**Type:** First-person exploration/cartography game
**UI Needs:** Full-screen drawing canvas (pencil/eraser/straightedge), layer management panel, tool palette, compass HUD element, boss letter display (styled text), map submission comparison screen, zoom/pan/rotate controls
**Aesthetic:** Hand-drawn, contemplative, minimal HUD

---

## Feature Gap Analysis

### Already Covered by Afterhours
- Flex layout, buttons, sliders, checkboxes, radio groups, dropdowns, toggle switches
- Text input, text area with selection/cursor
- Scroll views, tab containers, pagination
- Modal dialogs, toast notifications
- Progress bars, circular progress
- Themes, animations, focus/tab navigation
- E2E testing framework
- Translation/i18n support

### What's Missing (Cross-Project)

The gaps below are ordered by how many projects would benefit.

---

## New Feature Proposals

### For T.O.S.S. (macintosh)

#### 1. Typewriter Text Component
**Problem:** T.O.S.S. needs typewriter text in the boot sequence, COMMS dialogue box, and failure diagnostics. This is manually implemented with `char_timer` and `visible_chars` counters in every place it appears. Pharmasea would also use this for NPC dialogue.

**Proposed API:**
```cpp
struct TypewriterConfig {
    float char_delay = 0.03f;       // Seconds between characters
    bool skip_on_input = true;      // Button press completes instantly
    std::function<void()> on_char;  // Called per character (for blip SFX)
    std::function<void()> on_complete;
};

ElementResult typewriter_text(HasUIContext auto &ctx, EntityParent ep_pair,
                              const std::string &full_text,
                              TypewriterConfig config = TypewriterConfig());
```

**Who benefits:** T.O.S.S. (boot, COMMS, diagnostics), Pharmasea (future NPC dialogue), Cartographer (boss letters)

#### 2. Diegetic / In-World UI Anchoring
**Problem:** T.O.S.S. needs HUD elements that "glitch on" as modules load — they must feel like part of the robot's OS, not floating menus. There's no way to anchor a UI element to a 3D world position or to a specific region of the render texture with shader effects applied.

**Proposed API:**
```cpp
ComponentConfig{}
    .with_world_anchor(vec3{x, y, z}, camera)  // Project 3D → 2D screen pos
    .with_screen_region(region)                 // Restrict to sub-rect of screen
    .with_glitch_effect(intensity, seed)        // CRT-style visual glitch
```

**Who benefits:** T.O.S.S. (diegetic HUD), Pharmasea (speech bubbles above 3D characters), Cartographer (compass anchored to hand model)

#### 3. Dialogue Box / COMMS Component
**Problem:** T.O.S.S. needs a recurring pattern: portrait + speaker name/title + typewriter text + dismiss-on-input. This is a specific composition but appears in every zone with different speakers.

**Proposed API:**
```cpp
struct DialogueBoxConfig {
    std::string speaker_name;
    std::string speaker_title;
    std::optional<TextureConfig> portrait;  // 128x128
    TypewriterConfig typewriter;
    bool dismiss_on_any_input = true;
    Color border_color = Theme::Usage::Accent;
};

ElementResult dialogue_box(HasUIContext auto &ctx, EntityParent ep_pair,
                           const std::string &text,
                           bool &is_visible,
                           DialogueBoxConfig config = DialogueBoxConfig());
```

**Who benefits:** T.O.S.S. (COMMS box), Cartographer (boss letters), Endless Dance Chaos (future tutorial text)

---

### For Pub Panic! (pharmasea)

#### 4. World-Space Progress Bars / Indicators
**Problem:** Pharmasea needs progress indicators on machines (filling, blending, serving), patience timers above customers, and bladder indicators — all anchored to 3D entities, not screen space. Currently these are manually drawn with raylib primitives.

**Proposed API:**
```cpp
struct WorldIndicatorConfig {
    vec3 world_position;
    vec2 screen_offset = {0, -20};  // Offset from projected position
    float max_width = 40.0f;        // Pixels at reference zoom
    bool scale_with_distance = true;
    bool fade_with_distance = true;
    float fade_start = 20.0f;
    float fade_end = 40.0f;
};

ElementResult world_progress_bar(HasUIContext auto &ctx, EntityParent ep_pair,
                                 float value,
                                 const Camera &camera,
                                 WorldIndicatorConfig config = WorldIndicatorConfig());
```

**Who benefits:** Pharmasea (machine timers, patience bars), T.O.S.S. (object tracking brackets), Endless Dance Chaos (facility service indicators)

#### 5. Confirmation Dialog Preset
**Problem:** Pharmasea needs confirmation dialogs for resolution changes, language switches, and deleting required items. T.O.S.S. needs "Are you sure you want to quit?" These are all modal + two buttons + message, but everyone re-implements them.

**Proposed API:**
```cpp
enum class ConfirmResult { Pending, Confirmed, Cancelled };

ConfirmResult confirm_dialog(HasUIContext auto &ctx, EntityParent ep_pair,
                             bool &is_open,
                             const std::string &title,
                             const std::string &message,
                             const std::string &confirm_label = "Confirm",
                             const std::string &cancel_label = "Cancel");

// Usage:
auto result = confirm_dialog(ctx, mk(parent), show_quit_dialog,
    "Quit Game", "Are you sure?", "Quit", "Cancel");
if (result == ConfirmResult::Confirmed) { exit(); }
```

**Who benefits:** All five projects — everyone needs confirm dialogs

#### 6. Grid/Slot Inventory Layout
**Problem:** Pharmasea has equipment slots, save slots, shop grids, and recipe grids. Endless Dance Chaos has a build tool bar. These are all grids of selectable cells with icons/states, but there's no grid selection primitive.

**Proposed API:**
```cpp
struct GridSlotConfig {
    int columns = 4;
    vec2 cell_size = {64, 64};
    float gap = 4.0f;
    bool wrap = true;
    bool keyboard_nav = true;  // Arrow keys move selection
};

struct GridSlotResult {
    int selected_index = -1;    // Currently selected
    int clicked_index = -1;     // Just clicked this frame
    Entity grid_entity;
};

GridSlotResult grid_slots(HasUIContext auto &ctx, EntityParent ep_pair,
                          int item_count,
                          GridSlotConfig config = GridSlotConfig());

// Then render each cell:
for (int i = 0; i < item_count; ++i) {
    auto cell = grid_slot_cell(ctx, mk(result.grid_entity, i),
        ComponentConfig{}.with_background(i == result.selected_index
            ? accent : background));
    // ... render item icon inside cell
}
```

**Who benefits:** Pharmasea (shop, recipes, save slots), Endless Dance Chaos (build bar), Wordproc (template picker), Cartographer (tool palette)

---

### For Wordproc (wordproc)

#### 7. Menu Bar with Keyboard Accelerators
**Problem:** Wordproc has a full Win95-style menu bar (File, Edit, View, Insert, Format, Tools, Help) with nested dropdowns, keyboard shortcuts displayed, checkmarks, and separators. It's hand-built with `imm_menu.h`. The afterhours library doesn't have a menu bar primitive — only the dropdown menu spec in `24_context_menu.md`.

**Proposed API:**
```cpp
struct MenuBarResult {
    bool any_open = false;
    std::optional<std::string> activated_action;  // ID of selected item
};

MenuBarResult menu_bar(HasUIContext auto &ctx, EntityParent ep_pair,
                       const std::vector<MenuCategory> &categories);

// Where MenuCategory reuses the MenuItem from 24_context_menu.md
struct MenuCategory {
    std::string label;              // "File", "Edit", etc.
    std::vector<MenuItem> items;
    std::string accelerator_key;    // "Alt+F" underline hint
};
```

This is the obvious integration point for `24_context_menu.md`'s `dropdown_menu()` — the menu bar is just a row of dropdown triggers.

**Who benefits:** Wordproc (primary), Cartographer (future tool menus), Pharmasea (settings categories)

#### 8. Status Bar Component
**Problem:** Wordproc needs a status bar at the bottom (line/column count, word count, zoom level, insert/overwrite mode). Endless Dance Chaos needs a top bar (time, deaths, attendees). T.O.S.S. has a HUD header. These are all fixed-position bars with segmented info cells.

**Proposed API:**
```cpp
struct StatusBarSegment {
    std::string text;
    float min_width = 0;       // 0 = auto-fit text
    float flex_grow = 0;       // > 0 = take remaining space
    std::optional<std::function<void()>> on_click;
};

ElementResult status_bar(HasUIContext auto &ctx, EntityParent ep_pair,
                         const std::vector<StatusBarSegment> &segments,
                         ComponentConfig config = ComponentConfig());

// Usage:
status_bar(ctx, mk(root), {
    {"Ln 42, Col 17"},
    {"Words: 1,234", .flex_grow = 1},
    {"100%", .on_click = []{ show_zoom_picker(); }},
    {"INS"},
});
```

**Who benefits:** Wordproc (status bar), Endless Dance Chaos (top bar), T.O.S.S. (HUD header)

#### 9. Keyboard Shortcut Display / Hints
**Problem:** Wordproc menus show shortcuts like "Ctrl+S" right-aligned. The build bar in Endless Dance Chaos shows "L/R Bumper to cycle." T.O.S.S. shows "Press any button to dismiss." There's no standard way to display keyboard hints in the UI.

**Proposed API:**
```cpp
// Right-aligned shortcut text in a menu item or button
ComponentConfig{}
    .with_shortcut_hint("Ctrl+S")
    .with_shortcut_hint_color(Theme::Usage::TextSecondary);

// Or as a standalone hint overlay
ElementResult shortcut_hint(HasUIContext auto &ctx, EntityParent ep_pair,
                            const std::string &hint,
                            ComponentConfig config = ComponentConfig());
```

**Who benefits:** Wordproc (menu shortcuts), Endless Dance Chaos (build bar hints), T.O.S.S. (dismiss prompts), Cartographer (tool hotkeys)

---

### For Endless Dance Chaos (endless_dance_chaos)

#### 10. Vertical Timeline Component
**Problem:** Endless Dance Chaos needs a Google Calendar-style vertical timeline where time scrolls upward, a "NOW" marker stays fixed, and events are blocks sized by duration. No existing widget handles this.

**Proposed API:**
```cpp
struct TimelineEvent {
    float start_time;         // In game hours (e.g., 14.0 = 2pm)
    float duration_hours;
    std::string label;
    std::string detail;       // "~200 people"
    Color color;
};

struct TimelineConfig {
    float current_time;           // NOW marker position
    float now_position = 0.2f;   // NOW at 20% from top
    float pixels_per_hour = 60.0f;
    bool auto_scroll = true;     // Follow current time
    float visible_hours = 8.0f;
};

ElementResult vertical_timeline(HasUIContext auto &ctx, EntityParent ep_pair,
                                const std::vector<TimelineEvent> &events,
                                TimelineConfig config = TimelineConfig());
```

**Who benefits:** Endless Dance Chaos (primary), Pharmasea (could use for day schedule display)

#### 11. Minimap Component
**Problem:** Endless Dance Chaos needs a 150x150 minimap showing the game map with a camera viewport rectangle, click-to-jump, and drag-to-pan. Cartographer could use this for the drawing canvas overview. This is a common game UI pattern with no library support.

**Proposed API:**
```cpp
struct MinimapConfig {
    vec2 size = {150, 150};
    vec2 world_bounds;              // Total world size
    vec2 camera_position;           // Current camera center
    vec2 camera_viewport;           // Camera visible area
    Color viewport_color = colors::white_alpha(128);
    Color background_color = colors::dark_gray;
    bool click_to_jump = true;
    bool drag_to_pan = true;
};

struct MinimapResult {
    bool clicked = false;
    vec2 click_world_position;  // Where in world coordinates
    bool dragging = false;
    vec2 drag_world_position;
};

MinimapResult minimap(HasUIContext auto &ctx, EntityParent ep_pair,
                      MinimapConfig config,
                      std::function<void(Entity&, vec2, vec2)> render_content);
                      // Callback to draw map content (paths, buildings) into minimap space
```

**Who benefits:** Endless Dance Chaos (primary), Cartographer (canvas overview), Pharmasea (could show bar layout)

#### 12. Tool Palette / Cycling Selector
**Problem:** Endless Dance Chaos has a build bar where you cycle tools with L/R bumpers. Cartographer has a tool palette (pencil sizes, eraser, straightedge). These are selection bars where one item is active, with gamepad cycling support.

**Proposed API:**
```cpp
struct ToolItem {
    std::string label;
    std::optional<TextureConfig> icon;
    std::optional<std::string> shortcut;  // "1", "L/R"
    bool disabled = false;
};

struct ToolPaletteConfig {
    bool horizontal = true;
    bool cycle_wrap = true;       // L past first → goes to last
    float selected_scale = 1.2f;  // Scale up selected item
};

struct ToolPaletteResult {
    int selected_index;
    bool changed;  // True on the frame selection changed
};

ToolPaletteResult tool_palette(HasUIContext auto &ctx, EntityParent ep_pair,
                               const std::vector<ToolItem> &tools,
                               int &selected,
                               ToolPaletteConfig config = ToolPaletteConfig());
```

**Who benefits:** Endless Dance Chaos (build bar), Cartographer (drawing tools), Wordproc (formatting toolbar)

---

### For Cartographer (cartographer)

#### 13. Canvas / Drawing Surface
**Problem:** Cartographer's core mechanic is a full-screen drawing canvas with pencil, eraser, straightedge, zoom/pan/rotate. There's no canvas primitive in afterhours — the drawing system is entirely custom. A basic canvas widget that handles input→stroke conversion, zoom, and pan would be reusable for any drawing/annotation feature.

**Proposed API:**
```cpp
struct CanvasConfig {
    vec2 canvas_size = {1024, 1024};  // Logical size in pixels
    float min_zoom = 0.25f;
    float max_zoom = 4.0f;
    bool enable_pan = true;       // Middle-click drag
    bool enable_zoom = true;      // Scroll wheel
    bool enable_rotate = false;   // Optional rotation
    Color background = colors::white;
};

struct CanvasState {
    float zoom = 1.0f;
    vec2 pan_offset = {0, 0};
    float rotation = 0.0f;
};

struct CanvasResult {
    Entity canvas_entity;
    vec2 cursor_canvas_pos;       // Mouse position in canvas coordinates
    bool is_drawing;              // Mouse down in canvas
    CanvasState state;
};

CanvasResult canvas(HasUIContext auto &ctx, EntityParent ep_pair,
                    CanvasState &state,
                    CanvasConfig config = CanvasConfig());
```

The canvas doesn't draw strokes itself — it just provides the coordinate transform and input handling. The game renders strokes into the canvas's render texture.

**Who benefits:** Cartographer (primary), Endless Dance Chaos (could use for map editor in future)

#### 14. Layer Panel
**Problem:** Cartographer has a 2-layer system (sketch + final) with plans for unlimited layers. Layers need visibility toggles, reordering, active selection, and merge operations. This is a specialized list widget.

**Proposed API:**
```cpp
struct LayerItem {
    std::string name;
    bool visible = true;
    bool locked = false;
    float opacity = 1.0f;
};

struct LayerPanelResult {
    int active_layer;
    int visibility_toggled = -1;   // Index of layer whose visibility changed
    int reorder_from = -1;         // Drag reorder source
    int reorder_to = -1;           // Drag reorder destination
};

LayerPanelResult layer_panel(HasUIContext auto &ctx, EntityParent ep_pair,
                             std::vector<LayerItem> &layers,
                             int &active_layer);
```

**Who benefits:** Cartographer (primary), Wordproc (could use for document views)

#### 15. Styled Letter / Document Display
**Problem:** Cartographer displays boss letters before each assignment — styled text documents with headers, paragraph breaks, signatures, and possibly wax seal decorations. T.O.S.S. has the failure diagnostic report. These are read-only rich text displays with specific visual treatments.

This overlaps with `22_rich_text_label.md` but the use case is simpler: a pre-formatted document with a few style regions, not a full rich text editor.

**Proposed API:**
```cpp
struct StyledParagraph {
    std::string text;
    enum Style { Body, Header, Signature, Monospace, Italic } style = Body;
};

ElementResult styled_document(HasUIContext auto &ctx, EntityParent ep_pair,
                              const std::vector<StyledParagraph> &paragraphs,
                              ComponentConfig config = ComponentConfig());

// Usage (Cartographer boss letter):
styled_document(ctx, mk(parent), {
    {.text = "Royal Cartographic Society", .style = Header},
    {.text = "To our esteemed cartographer,"},
    {.text = "You are hereby assigned to survey the northern territories..."},
    {.text = "The following names have been approved by the Crown:"},
    {.text = "  - Mount Whitmore\n  - Lake Victoria\n  - Fort George", .style = Monospace},
    {.text = "Yours faithfully,", .style = Italic},
    {.text = "Lord Pemberton", .style = Signature},
});
```

**Who benefits:** Cartographer (boss letters, map comparison), T.O.S.S. (diagnostic reports, failure screen), Wordproc (document preview)

---

## Priority Matrix

| # | Feature | TOSS | Pharmasea | Wordproc | EDC | Cartographer | Total |
|---|---------|------|-----------|----------|-----|--------------|-------|
| 5 | Confirmation Dialog | x | x | x | x | x | **5** |
| 9 | Keyboard Shortcut Hints | x | | x | x | x | **4** |
| 6 | Grid/Slot Layout | | x | x | x | x | **4** |
| 1 | Typewriter Text | x | x | | | x | **3** |
| 2 | World-Space Anchoring | x | x | | | x | **3** |
| 4 | World Progress Bars | x | x | | x | | **3** |
| 8 | Status Bar | x | | x | x | | **3** |
| 12 | Tool Palette | | | x | x | x | **3** |
| 3 | Dialogue Box | x | | | | x | **2** |
| 7 | Menu Bar | | | x | | x | **2** |
| 15 | Styled Document | x | | x | | x | **3** |
| 10 | Vertical Timeline | | x | | x | | **2** |
| 11 | Minimap | | x | | x | x | **3** |
| 13 | Canvas/Drawing Surface | | | | | x | **1** |
| 14 | Layer Panel | | | x | | x | **2** |

**Recommended implementation order:** 5 → 9 → 6 → 1 → 8 → 12 → 4 → 2 → 15 → 11 → 7 → 10 → 3 → 14 → 13
