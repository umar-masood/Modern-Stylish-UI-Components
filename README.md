🌟 QtNovaUI - UI Library for Windows & MacOS (Supported Cross-Platform)
=====================================================
Crafted with Qt, C++ , QSS — Art Meets Function. This collection of custom UI components redefines desktop application design in Qt. Every element has been hand-built with precision using C++ and Qt, and styled with the elegance of Qt Style Sheets (QSS) to evoke a modern, smooth, and responsive feel.

🧩 Core Components
===================
# 🔘 Button
*A modern, animated, and highly customizable Qt button*. The **Button** component is a custom `QPushButton` replacement built using **QPainter**, **Qt animations**, and **state-aware rendering**.

## ✨ Features
- Multiple display modes (text, icon, mixed)
- Animated shadow on hover
- Dark mode support
- Primary / Secondary styles
- Gradient backgrounds with hover animation
- Unicode & Pixmap icon support
- Built-in loader (spinner)
- Hyperlink-style buttons
- Automatic size calculation

### Basic Usage
```cpp
#include "Button.h"

Button *btn = new Button;
btn->setText("Click Here");
btn->setDisplayMode(Button::TextOnly);
btn->setFixedSize(QSize(200, 36));
btn->setSecondary(true);
btn->setDarkMode(true);
btn->setShadow(true);
```
> [!TIP]
> If no fixed size is set, the button automatically adjusts its size based on content and display mode.

### Display Modes
```setDisplayMode(Button::DisplayMode mode);```
1. TextOnly      	  
2. IconOnly	        
3. IconText	        
4. TextUnderIcon	 
> [!IMPORTANT]
> You must set a display mode using setDisplayMode().

### Icon Usage
**Pixmap Icons**
```cpp
btn->setIconPaths(":/icons/light.svg", ":/icons/dark.svg");
btn->setIconSize(QSize(18, 18));
```
Automatically switches icons based on dark mode

**Unicode Icons**
```cpp
btn->setUnicodeIcon(QChar(0xE8FB), 18);
```
Uses Segoe Fluent Icons. Ideal for fluent-style UI

> [!WARNING]
> You cannot use Unicode icons and Pixmap icons together.

### Styling Options
**Primary / Secondary**
```cpp
btn->setSecondary(true);
```
> [!NOTE]
> When Secondary is enabled, the shadow is automatically disabled.

### Dark Mode
```cpp
btn->setDarkMode(true);
```
Adjusts colors, borders, icons, shadows. Loader spinner adapts automatically.

### Shadow Effect
```cpp
btn->setShadow(true);
```
Disabled automatically for Secondary buttons.

### Gradient Button
```cpp
btn->setGradientColor(true, "#008EDE", "#1BB3E6");
btn->setHoverGradientColor("#3FC1FF");
```
> [!IMPORTANT]
> Gradient is disabled automatically in Hyperlink mode.

### HyperLink Button
```cpp
btn->setHyperLink(true);
btn->setHyperLinkColors(QColor("#008EDE"), QColor("#1BB3E6"));
```
- Text-only rendering
- No background or border
- Hover color transition
- Ideal for inline actions like “Learn more”.

### Loader Button
```cpp
btn->setLoaderButton(true);
btn->setText("");
```
- Empty text triggers spinner
- Spinner is centered automatically
- Text restores when non-empty

> [!WARNING]
> Loader will not appear unless setText("") is used.

### Font Customization
```cpp
btn->setFontProperties(
  "Segoe UI",
  10,
  true,   // bold
  false   // italic
);
```
> [!NOTE]
> Unicode icons always use Segoe Fluent Icons, regardless of font settings.

> [!NOTE]
> All of the following are false by default:
> `setShadow()`
> `setDarkMode()`
> `setSecondary()`
> `setHyperLink()`
> `setLoaderButton()`
> `setGradientColor()`


# 📥 ComboBox

A modern, animated, and highly customizable **Qt ComboBox**, built on top of `TextField`.  
This component replaces `QComboBox` with a **fully custom popup**, **smooth animations**, **icon support**, and **dark-mode awareness**.


## ✨ Features

- Custom dropdown popup (no native QComboBox)
- Smooth fade-in / fade-out animation
- Built on top of **TextField**
- Editable & non-editable modes
- Icon + text items (Iconic mode)
- Automatic dark mode synchronization
- Smart popup positioning (above / below / center)
- Max visible item control
- Custom item delegate
- Custom scrollbars
- Auto-complete with `QCompleter`
- Auto close on outside click or app deactivation

## 🧱 Architecture

| Part | Implementation |
|----|----|
| Base class | `TextField` |
| Popup container | `RoundedBox` |
| Item view | `QListView` |
| Data model | `QStandardItemModel` |
| Animation | `QPropertyAnimation + SmoothOpacity` |
| Delegate | Custom `Delegate` |
| Scrollbars | Custom `ScrollBar` |

## 🚀 Basic Usage

```cpp
ComboBox *box = new ComboBox(this);
box->setPlaceholderText("Select an option");
box->addItems({"Apple", "Banana", "Mango"});
```
## Adding Items
```cpp
box->addItems(QStringList{
    "Item One",
    "Item Two",
    "Item Three"
});
```
## Iconic Mode (Icons + Text)
```cpp
box->setIconic(true);
box->addItems({"Home", "Settings", "Profile"});
box->addIcons(
    {":/icons/light/home.svg", ":/icons/light/settings.svg", ":/icons/light/user.svg"},
    {":/icons/dark/home.svg",  ":/icons/dark/settings.svg",  ":/icons/dark/user.svg"}
);
```
> [!CAUTION]
> `setIconic(true)` must be enabled before adding icons.

## Editable Mode
```cpp
box->setEditable(true);
```
- User can type directly
- Inline auto-completion enabled
- Press Enter to select a matching item
- Context menu and text selection enabled

> [!NOTE]
> When it is disabled, all text interaction is blocked.

## Dark Mode
```cpp
box->setDarkMode(true);
```

## Max Visible Items
```cpp
box->setMaxVisibleItems(6);
```
> [!NOTE]
> By default, its value is 8

## Placeholder Text
```cpp
box->setPlaceholderText("Select an option");
```
Displays hint text when:
- No item is selected
- The ComboBox text is empty
The placeholder behaves exactly like a QLineEdit placeholder and is inherited from TextField.

## Popup Animation
- Fade-in when opening
- Fade-out when closing
- Uses QPropertyAnimation
- Popup closes automatically on:
- Outside click
- Application deactivation
- Item selection
No manual animation handling required.

## Smart Popup Positioning
Popup placement is calculated dynamically:
- Centered (preferred)
- Below the ComboBox
- Above the ComboBox
Also:
- Respects screen boundaries
- Works on multi-monitor setups
- Prevents clipping

## Current Selection
```cpp
int index = box->currentIndex();
QString text = box->currentText();
```
**Programmatic Selection**
```cpp
box->setCurrentItem(2);
```

## Remove Item
```cpp
box->deleteItem(1);
```

## Clear All Items
```cpp
box->clearAll();
```

# CheckBox 
A modern, lightweight, and fully custom **Qt CheckBox**, built using `QPainter` for pixel-perfect rendering.  
This component replaces `QCheckBox` with **custom visuals**, **hover states**, and **dark mode support**, while staying minimal and fast.

## 🚀 Basic Usage
```cpp
CheckBox *check = new CheckBox("Remember me", this);
```

## Checked State
**Set Checked**
```cpp
check->setChecked(true);
```
**Get Checked State**
```cpp
bool value = check->isChecked();
```

## Signal
```cpp
connect(check, &CheckBox::toggled, this, [](bool checked){
    qDebug() << "Checkbox state:" << checked;
});
```
> [!NOTE]
> This signal will emit whenever the checkbox state changes (mouse or programmatic).

## Dark Mode
```cpp
check->setDarkMode(true);
```

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
- Smart padding adjustment (icons, buttons, dropdowns)
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
> - Right spacing
> - Drop-down padding
  
