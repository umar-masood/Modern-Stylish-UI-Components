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
> setSecondary()
> setShadow()
> setDarkMode()
> setHyperLink()
> setLoaderButton()
> setGradientColor()


📥 ComboBox
------------
A modern take on a classic. Styled dropdowns with beautiful borders, soft shadows, and a graceful opening animation. Supports icons, themes, and custom delegates for rich content.

🔁 Toggle
--------------
A sleek switch component designed for simplicity and clarity. Inspired by mobile UX, this toggle provides smooth animations, color transitions, and intuitive control states.

📦 RoundedBox
--------------
Stylish containers with soft, rounded corners and customizable backgrounds. Ideal for wrapping content in modern "card-like" layouts.

📑 Menu
-------------
A reimagined menu system that uses transparency, icons, and soft shadows to create a floating, elevated UI experience. Fully theme-adaptable.

🎛️ Input & Feedback
===================

✏️ TextField
--------------
Smooth focus effects, placeholder transitions, and theme-aware coloring make this a standout input field. Designed for readability and elegance.

📊 LineProgress
----------------
A lightweight linear progress bar with a custom QSS-powered gradient, suitable for both subtle background loads or full attention-grabbing animations.

🔄 SpinnerProgress
--------------------
An animated spinner that balances beauty and utility. Built with Qt animations and perfect for long operations without locking the UI.

🎨 Visual Enhancements
=======================

🌫 SmoothShadow
================
Real-time drop shadows without harsh edges. Powered by QPropertyAnimation and QGraphicsBlur, these shadows bring a native, modern feel to floating UI elements.

🌟 SmoothOpacity
-------------------
Fade-in and fade-out transitions made smooth using Qt’s animation framework. Perfect for tooltips, dialogs, and ephemeral widgets.

🖼️ ToolTip
--------------
Custom tooltips with rounded corners, blur backgrounds, and theme-aware text. Ideal for enhancing user guidance while preserving aesthetic consistency.

🧰 Structural Components
========================

📜 ScrollBar
--------------
A redefined scrollbar with slim, hover-expandable design. Styled with QSS and fully themeable to blend seamlessly into any layout.

🗂 Delegate
-----------
Provides model/view customization for advanced UI like list views and combo boxes, enabling rich item rendering with full theme support.

🗨️ Dialog
----------
A centered, softly-shadowed dialog box with polished animation and responsiveness. Works in harmony with Dialog_Button and RoundedBox.

We are improving this set of Modern UI Components time to time.

Best,
Umar Masood
