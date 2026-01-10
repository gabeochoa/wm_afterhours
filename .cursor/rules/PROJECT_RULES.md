# Project Rules

## Git Commit Format
Use short, descriptive commit messages with prefixes:
- no prefix for new features
- `bf -` for bug fixes  
- `be -` for backend/engine changes or refactoring
- `ui -` for UI changes

Rules:
- Use all lowercase
- Avoid special characters like ampersands (&) - use "and" instead
- Keep messages concise and descriptive

Examples:
- `implement basic typing system`
- `bf - fix window not closing properly`
- `be - refactor order matching system`

## Code Style
- Keep functions focused and single-purpose
- Prefer early returns to reduce nesting
- Dont add any comments unless explicitly asked 
- use a function instead of a line with multiple ternary expressions
- Avoid using `auto` for non-template types - use explicit types instead
- Use `for (Entity &entity : EntityQuery().gen())` instead of `for (auto &ref : ...)` with `ref.get()` - the range-for will auto-extract the entity from the reference wrapper
- Prefer references over pointers when possible - when iterating over containers of pointers, dereference immediately: `for (Type *ptr : container) { Type &ref = *ptr; ... }` instead of using `ptr->` throughout

## Project Structure
- `src/` contains main game code
- `vendor/` contains third-party libraries (afterhours as git submodule)
- `resources/` contains assets (images, sounds, fonts)
- `output/` contains build artifacts

## Build System
- Use `make` to build project
- Executable is `ui_tester.exe`
- Run `make` to build game
- Use `make clean` to clean build artifacts
- Use `make run` to build and run game

## MCP Server (Model Context Protocol)

The project includes MCP support for AI-assisted testing and automation. MCP allows external tools (like Cursor) to take screenshots and control the game via keyboard/mouse input.

### Building with MCP
```bash
make ENABLE_MCP=1
```

### Running with MCP
```bash
./output/ui_tester.exe --mcp --simple_button
```

### Available MCP Tools
- `screenshot` - Capture current game frame as PNG
- `get_screen_size` - Get screen dimensions (returns JSON: `{"width": 1280, "height": 720}`)
- `mouse_move` - Move cursor to coordinates: `{"x": 100, "y": 200}`
- `mouse_click` - Click at coordinates: `{"x": 100, "y": 200, "button": "left"}`
- `key_press` - Press and release a key: `{"key": "enter"}`
- `key_down` / `key_up` - Hold or release a key

### Key Names for Input
Letters: `a`-`z`, Numbers: `0`-`9`, Special: `space`, `enter`, `escape`, `tab`, `backspace`, `up`, `down`, `left`, `right`, `shift`, `ctrl`, `alt`

### Cursor Integration
The `.cursor/mcp.json` file configures Cursor to use the MCP server:
```json
{
  "mcpServers": {
    "ui-tester": {
      "command": "./output/ui_tester.exe",
      "args": ["--mcp", "--simple_button"],
      "env": {}
    }
  }
}
```

### Testing MCP
Run the test script to verify MCP functionality:
```bash
python3 test_mcp.py
```

### MCP Implementation Files
- `vendor/afterhours/src/plugins/mcp_server.h` - MCP protocol handler
- `src/engine/input_injector.h/cpp` - Input simulation module
- `src/game.cpp` - MCP integration in game loop

## Debugging
- Use `log_info()`, `log_warn()`, `log_error()` for logging
- Add debug logs for complex systems
- Remove verbose debug logs before committing

## TODO Comments
- Use `TODO` for incomplete features or known issues
- Use `TODO` for performance optimizations (e.g., "TODO this allocates...")
- Use `TODO` for future improvements (e.g., "TODO add random generator")

## Component Patterns
- All components inherit from `BaseComponent`
- Use `struct` for components, not `class`
- Components should be simple data containers
- Use `std::optional` for nullable fields
- Use `enum class` for component state enums

## System Patterns
- Systems inherit from `System<Components...>`
- Override `for_each_with(Entity&, Components&..., float)` for main logic
- Use `virtual void for_each_with(...) override` syntax
- Systems should be focused and single-purpose
- Use early returns to reduce nesting

## Singleton Patterns
- Use `SINGLETON_FWD(Type)` and `SINGLETON(Type)` macros
- Singletons should have `get()` static method
- Use `SINGLETON_PARAM` for parameterized singletons
- Delete copy constructor and assignment operator

## Enum Patterns
- Use `enum struct` for type safety
- Use `magic_enum` for enum utilities
- Use `magic_enum::enum_names<EnumType>()` for string lists
- Use `magic_enum::enum_count<EnumType>()` for count

## Naming Conventions
- Use `camelCase` or snake_case for variables and functions
- Use `PascalCase` for structs, classes, and enums
- Use `UPPER_CASE` for constants and macros
- Use descriptive names that indicate purpose
- Use `has_` prefix for boolean components (e.g., `HasHealth`)
- Use `Can_` prefix for capability components (e.g., `CanDamage`)

## File Organization
- Use `#pragma once` at top of header files
- Group includes: standard library, third-party, project headers
- Use forward declarations when possible
- Keep header files focused and minimal

## Query and Filtering Patterns
- Prefer `EntityQuery` when possible over manual entity iteration
- Use `whereLambda` for complex filtering conditions
- Use `orderByLambda` for sorting entities instead of `std::sort`
- Chain multiple `where` clauses for better performance
- Use `gen_first()` for finding single entities instead of loops
- Extract complex query logic into helper functions for reusability
- Use `RefEntities` (from query results) instead of manually creating vectors of references

## Component Design Principles
- Prefer pure tag components over components with member variables
- Use composition over configuration (multiple small components vs one large component)
- Each component should have a single, clear responsibility
- Use separate components for data instead of boolean flags (e.g., `IsDisabled` instead of `enabled` field)
- Tag components should have no members: `struct IsGrabbed : BaseComponent {};`
- Data components should contain only related data: `struct TypingBuffer : BaseComponent { std::string buffer; };`

## System File Naming
- All system header files must use UpperCamelCase naming convention
- Examples: `ProcessTypingInput.h`, `MatchItemToOrder.h`, `RenderOrders.h`
- Update include statements when renaming system files
- Check for internal includes within system files that reference other systems

## Refactoring and Development Workflow
- Extract helper functions to reduce code duplication
- Use consistent patterns across similar systems
- Update related systems when changing component structures
- Test builds after each major refactoring step
- Run `make` after each significant change to catch compilation errors
- Fix build errors immediately before continuing development
- Include all necessary component headers in system files
- Use early returns for error conditions in systems

