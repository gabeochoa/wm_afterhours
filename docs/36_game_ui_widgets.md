# Game UI Widgets

**Status:** Not implemented  
**Priority:** Low

---

## Currency/Resource Display

### Problem

Currency pills (icon + formatted number) are common but manually composed.

### Pattern Structure
```
┌──────────────────────┐
│ [💰] $1,250,980      │
└──────────────────────┘
```

### Suggested Implementation

```cpp
ElementResult currency_display(HasUIContext auto &ctx, EntityParent ep_pair,
                               int64_t amount,
                               ComponentConfig config = ComponentConfig());

ElementResult resource_display(HasUIContext auto &ctx, EntityParent ep_pair,
                               TextureConfig icon,
                               const std::string &value,
                               ComponentConfig config = ComponentConfig());
```

---

## Chat/Message List

### Problem

Chat message displays appear in multiple screens.

### Suggested Implementation

```cpp
struct ChatMessage {
    std::string username;
    std::string message;
    std::optional<TextureConfig> avatar;
    std::optional<Color> username_color;
};

ElementResult chat_box(HasUIContext auto &ctx, EntityParent ep_pair,
                       const std::vector<ChatMessage> &messages,
                       ComponentConfig config = ComponentConfig(),
                       size_t max_visible = 3);
```

---

## Equipment/Inventory Slot

### Problem

Equipment slots with icon, quantity badge, and selection border are common.

### Suggested Implementation

```cpp
struct EquipmentSlotConfig {
    std::optional<TextureConfig> icon;
    std::optional<std::string> quantity_text;
    bool is_selected = false;
    bool is_disabled = false;
};

ElementResult equipment_slot(HasUIContext auto &ctx, EntityParent ep_pair,
                              const EquipmentSlotConfig &slot_config,
                              ComponentConfig config = ComponentConfig());
```

