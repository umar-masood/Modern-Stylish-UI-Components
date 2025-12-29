🌟 QtNovaUI - UI Library for Windows & MacOS (Supported Cross-Platform)
=====================================================
Crafted with Qt, C++ , QSS — Art Meets Function. This collection of custom UI components redefines desktop application design in Qt. Every element has been hand-built with precision using C++ and Qt, and styled with the elegance of Qt Style Sheets (QSS) to evoke a modern, smooth, and responsive feel.

🧩 Core Components
===================
🔘 Button
---
Minimal yet powerful. This custom button is designed with elegant hover transitions, shadows, and support for icons. The Dialog_Button variant harmonizes seamlessly with dialogs, offering consistent styling across modals.

**How to use it?**
```cpp
#include "Button.h"

Button *btn = new Button;
btn->setText("Click Here");
btn->setFixedSize(QSize(200, 36));
btn->setSecondary(true);
btn->setDarkMode(true);
btn->setShadow(true);
```
> [!NOTE]
> By default the value of setSecondary(), setShadow(), setDarkMode(), setHyperLink(), setLoaderButton(), setGradientColor() is false unless you set it explicitly to True.
> When the button is secondary, the shadow will automatically disabled.
> If you want to use loader inside a button, then you must pass empty string to setText() along with True value to setLoaderButton(), it will trigger the loader to show visually.

> [!CAUTION]
> Make sure you have set Display Mode of button using setDisplayMode().
> You can either set Unicode Icon or Pixmap. You cannot set both at same time.

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
