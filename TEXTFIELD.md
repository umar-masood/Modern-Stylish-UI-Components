# 📝 TextField
A modern, animated, and fully custom Qt TextField, built as a replacement for QLineEdit.
Designed with QPainter, QSS, and Qt animations, this component delivers a smooth, polished input experience with dark mode, icons, clear & password buttons, custom context menu, and focus animations.

## ✨ Features
- Fully custom painted background & border
- Smooth focus glow animation (shadow)
- Dark mode support
- Left-side input icon support
- Built-in clear (✕) button
- Built-in password visibility toggle
- Custom context menu (Copy / Paste / Undo / Redo…)
- Hover & focus-aware rendering
- Font customization (family, size, weight, style)
- Smart padding adjustment
- Read-only & disabled states

## Usage
```cpp
#include "TextField.h"

TextField *field = new TextField(this);
field->setPlaceholderText("Enter username");
field->setFixedSize(QSize(360, 0));
```
> [!NOTE]
> Height is fixed internally for visual consistency. Only width is adjustable.

## Dark Mode
```cpp
field->setDarkMode(true);
```

## Shadow (Focus Glow)
```cpp
field->setShadow(true);
```
> [!NOTE]
> Shadow animation is only visible while focused.

## TextField Icon (Left Icon)
```cpp
field->setTextFieldIcon(true);
field->setTextFieldIconSize(QSize(18, 18));
field->setIconPaths(
    ":/icons/light/user.svg",
    ":/icons/dark/user.svg"
);
```
> [!IMPORTANT]
> `setTextFieldIcon(true)` must be enabled before setting icon paths or size.

## Clear Button
```cpp
field->setClearButton(true);
```
> [!WARNING]
> Clear button will not appear if password mode is enabled.

## Password TextField
```cpp
field->setPasswordTextField(true);
```
> [!IMPORTANT]
> Password button and clear button cannot be used together.

## Font Customization
```cpp
field->setFontProperties(
    "Segoe UI",
    10,
    true,   // bold
    false   // italic
);
```
## Read-Only & Disabled States
**Read-Only**
```cpp
field->setReadOnly(true);
```

**Disabled**
```cpp
field->setEnabled(false);
```

## Custom Context Menu
```cpp
field->setContextMenu(true);
```

## Colors Customization
**Selected Text Color**
```cpp
field->setTextSelectionColor("#FFFFFF");
```

**Normal Text Color**
```cpp
field->setTextColor("#000000");
```

**Selected Text Background Color**
```cpp
field->setTextSelectedBackgroundColor("#109AC7");
```

**Placeholder Text Color**
```cpp
field->setPlaceHolderTextColor("#F3F3F3");
```

## Padding
```cpp
field->setPadding(5, 2, 5, 2);
```

Replaces the native context menu with a fully custom Menu component.
**Supported Actions**
- Copy
- Cut
- Paste
- Delete
- Select All
- Undo
- Redo

> [!NOTE]
> If disabled, all Ctrl-based shortcuts (C/V/X/A) are blocked manually.

> [!IMPORTANT]
> Default Values, all of the following features are disabled by default:
> - Shadow
> - Dark mode
> - TextField icon
> - Clear button
> - Password mode
> - Custom context menu
