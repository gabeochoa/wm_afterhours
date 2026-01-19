# Tab Bars

**Status:** Not implemented  
**Priority:** Medium

## Problem

Vertical sidebar navigation with icons is common (EmpireTycoon left nav, MiniMotorways category tabs).

## Suggested Implementation

```cpp
enum struct TabBarOrientation { Horizontal, Vertical };

struct TabConfig {
    std::string label;
    std::optional<TextureConfig> icon;
    std::optional<Color> bg_color;
    std::optional<std::string> badge_text;
};

ElementResult tab_bar(HasUIContext auto &ctx, EntityParent ep_pair,
                      const std::vector<TabConfig> &tabs,
                      size_t &active_tab,
                      TabBarOrientation orientation = TabBarOrientation::Horizontal,
                      ComponentConfig config = ComponentConfig());
```

