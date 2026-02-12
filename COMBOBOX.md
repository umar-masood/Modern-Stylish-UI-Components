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
> [!IMPORTANT]
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
