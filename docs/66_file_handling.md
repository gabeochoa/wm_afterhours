# File Handling Components

**Status:** Not implemented  
**Priority:** Low  
**Source:** Component Gallery Analysis

---

## File Display

### Description

A representation of a file such as an uploaded attachment or a downloadable item.

**Also known as:** Attachment, Download

### Use Cases in Game UI
- Screenshot display
- Replay file display
- Mod file listings
- Export/import file display
- Save game file cards

### Suggested Implementation

```cpp
struct FileInfo {
    std::string name;
    std::string extension;
    size_t size_bytes;
    std::optional<std::string> date_modified;
    std::optional<TextureConfig> thumbnail;
};

struct FileDisplayConfig {
    bool show_size = true;
    bool show_date = false;
    bool show_icon = true;
    bool deletable = false;
    bool downloadable = false;
};

ElementResult file_display(HasUIContext auto &ctx, EntityParent ep_pair,
                           const FileInfo &file,
                           FileDisplayConfig config = FileDisplayConfig());

// Usage:
file_display(ctx, mk(parent, 0), 
             FileInfo{.name = "screenshot_001", .extension = "png", 
                      .size_bytes = 1024000, .thumbnail = thumb});
```

### Features Needed
- File icon based on type
- File name with extension
- Size display (formatted: KB, MB)
- Optional thumbnail preview
- Delete button (if deletable)
- Download button (if downloadable)
- Click to open/preview

---

## File Upload / Dropzone

### Description

An input which allows users to upload a file from their device.

**Also known as:** File input, File uploader, Dropzone

### Use Cases in Game UI
- Custom avatar upload
- Mod installation
- Save file import
- Screenshot sharing
- Replay upload

### Suggested Implementation

```cpp
struct FileUploadConfig {
    std::vector<std::string> accepted_extensions;  // {".png", ".jpg"}
    size_t max_size_bytes = 0;  // 0 = no limit
    bool multiple = false;
    bool drag_drop = true;
    std::string placeholder = "Drop files here or click to browse";
};

struct UploadedFile {
    std::string name;
    std::string path;
    size_t size;
    bool is_valid = true;
    std::string error_message;
};

ElementResult file_upload(HasUIContext auto &ctx, EntityParent ep_pair,
                          std::vector<UploadedFile> &files,
                          FileUploadConfig config = FileUploadConfig());

// Usage:
file_upload(ctx, mk(parent, 0), uploaded_files,
            FileUploadConfig{
                .accepted_extensions = {".png", ".jpg", ".jpeg"},
                .max_size_bytes = 5 * 1024 * 1024  // 5MB
            });
```

### Features Needed
- Click to open file browser
- Drag and drop support
- File type validation
- Size validation
- Progress indicator during upload
- Preview of uploaded files
- Remove uploaded file button
- Error display for invalid files

---

## File Browser (Native Dialog)

### Description

Opens native OS file browser for selecting files.

### Note

This requires platform-specific integration. See `17_pluggable_backends.md` for backend abstraction.

### Suggested Implementation

```cpp
namespace file_dialog {
    struct Filter {
        std::string name;  // "Images"
        std::vector<std::string> extensions;  // {"png", "jpg"}
    };
    
    // Returns selected file path, or empty if cancelled
    std::optional<std::string> open_file(
        const std::string &title = "Open File",
        const std::vector<Filter> &filters = {},
        const std::string &default_path = ""
    );
    
    std::optional<std::string> save_file(
        const std::string &title = "Save File",
        const std::vector<Filter> &filters = {},
        const std::string &default_name = ""
    );
    
    std::optional<std::string> select_folder(
        const std::string &title = "Select Folder"
    );
}

// Usage:
if (auto path = file_dialog::open_file("Select Avatar", 
                                        {{"Images", {"png", "jpg"}}})) {
    load_avatar(*path);
}
```

---

## Example Screen: FileHandlingShowcase

**File:** `src/systems/screens/FileHandlingShowcase.h`
**CLI:** `--screen=file_handling`
**Category:** Widgets

### Layout

A file management demo screen:

1. **File Display Cards** — A list of 4 `file_display()` cards showing mock files: "screenshot.png" (1.2 MB, with thumbnail), "save_game.dat" (256 KB), "config.json" (4 KB), "replay.rpl" (12 MB). Each shows icon, name, size, and optional delete button.

2. **File Upload Zone** — A `file_upload()` dropzone with dashed border: "Drop files here or click to browse". Accepted types: `.png`, `.jpg`. Max size: 5 MB. Dropping a valid file shows it in a list below. Invalid files show error messages.

3. **Upload Progress** — A simulated upload progress bar that fills from 0% to 100% over 2 seconds after a file is "uploaded".

4. **File Browser Button** — A button "Open File..." that would trigger `file_dialog::open_file()`. In the showcase, it simulates the dialog by showing a mock path result.

### Features Exercised

- `file_display()` with file info, thumbnail, delete button
- `file_upload()` with accepted extensions, max size validation
- File size formatting (KB, MB)
- File type icon selection based on extension
- Upload progress indication

### Verification

- File cards show correctly formatted sizes (1.2 MB, not 1228800)
- File type icons differ for .png vs .dat vs .json
- Upload zone rejects files > 5 MB with error message
- Upload zone rejects non-image files with error message
- Delete button removes the file card from the list

### E2E Test Plan

**Test file:** `src/testing/tests/FileHandlingTest.h`

#### New Custom Commands Needed

None — uses existing `click_button`, `expect_ui_exists`, `expect_ui_not_exists`, `capture_snapshot`. File drag-and-drop from the OS cannot be simulated in E2E tests; the test covers button-based upload and display.

#### Screenshots

1. `file_handling_initial` — file display cards and upload zone visible
2. `file_handling_card_detail` — file cards showing formatted sizes and icons
3. `file_handling_upload_progress` — progress bar mid-fill during simulated upload
4. `file_handling_card_deleted` — after deleting a file card, list updated

#### Test Script

```cpp
TEST(file_handling_display) {
  co_await TestApp::wait_for_frames(5);

  // Verify file cards with formatted sizes
  TestApp::expect_ui_exists("screenshot.png");
  TestApp::expect_ui_exists("1.2 MB");
  TestApp::expect_ui_exists("save_game.dat");
  TestApp::expect_ui_exists("256 KB");

  auto snap = TestApp::capture_snapshot("file_handling_initial");
}

TEST(file_handling_delete) {
  co_await TestApp::wait_for_frames(5);

  // Delete screenshot.png
  TestApp::click_button("Delete screenshot.png");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  TestApp::expect_ui_not_exists("screenshot.png");
  auto snap = TestApp::capture_snapshot("file_handling_card_deleted");
}

TEST(file_handling_upload_simulate) {
  co_await TestApp::wait_for_frames(5);

  // Click "Open File..." button (simulated dialog)
  TestApp::click_button("Open File...");
  co_await TestApp::wait_for_frames(1);
  TestApp::release_mouse_button();
  co_await TestApp::wait_for_frames(5);

  // Should show mock path result
  // Wait for upload progress
  co_await TestApp::wait_for_frames(60);  // mid-progress
  auto snap = TestApp::capture_snapshot("file_handling_upload_progress");
}
```

