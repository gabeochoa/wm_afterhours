# E2E Testing Framework

**Status:** Not implemented  
**Priority:** Large

## Problem

No built-in end-to-end UI testing or scripting framework.

## Working Implementation (wordproc)
- `src/testing/e2e_script.h` (DSL + runner)
- `src/testing/e2e_runner.h/.cpp`
- `src/extracted/e2e_testing.h` (single-file)

## Suggested Implementation

```cpp
namespace afterhours::testing {
  class E2ERunner {
   public:
    void load_script(const std::string &path);
    void load_scripts_from_directory(const std::string &dir);
    void set_timeout_frames(int frames);
    void tick();
    bool is_finished() const;
    bool has_failed() const;
  };

  namespace visible_text {
    void register_text(const std::string &text);
    bool contains(const std::string &text);
  }
}
```

## Related
- See `42_test_input_hooks.md` for the input injection system

