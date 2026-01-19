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

