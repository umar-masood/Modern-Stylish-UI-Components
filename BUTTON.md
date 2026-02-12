# 🔘 Button
*A modern, animated, and highly customizable Qt button*. The **Button** component is a custom `QPushButton` replacement built using **QPainter**, **Qt animations**, and **state-aware rendering**.

## ✨ Features
- Multiple display modes (text, icon, mixed)
- Animated shadow on hover
- Dark mode support
- Primary / Secondary styles
- Customizable buttn background and text colors
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
btn->setGradientColors("#008EDE", "#1BB3E6", "#3FC1FF");
```
> [!IMPORTANT]
> Currently, this button only supports linear gradient with two color stops. Gradient is disabled automatically in Hyperlink mode.

### HyperLink Button
```cpp
btn->setHyperLink(true);
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

> [!IMPORTANT]
> Loader will not appear unless `setText("")` is called with empty string.

### Transparent Button
```cpp
btn->setNormalBackgroundTransparent(true));
btn->setBorderTransparent(true);
```
> [!WARNING]
> The background color and border of button will remove except pressed color.

### Font Customization
```cpp
btn->setFontProperties(
  "Segoe UI",
  10,
  QFont::Medium,   // font weight
  false   // italic
);
```

### Button Colors Customization
#### Background Colors
```cpp
btn->setColor(ButtonStateColor::PrimaryNormal, QColor("#FFFFFF"));
```
The following are the available button states for background coloring:
- `PrimaryNormal` – Default primary button background  
- `PrimaryHover` – Background when mouse hovers over primary button  
- `PrimaryPressed` – Background when primary button is pressed  
- `DisabledPrimary` – Background when primary button is disabled  
- `SecondaryNormalLight` – Default secondary background in light mode  
- `SecondaryHoverLight` – Hover background in light mode  
- `SecondaryPressedLight` – Pressed background in light mode  
- `DisabledSecondaryLight` – Disabled secondary background in light mode
- `SecondaryNormalDark` – Default secondary background in dark mode  
- `SecondaryHoverDark` – Hover background in dark mode  
- `SecondaryPressedDark` – Pressed background in dark mode  
- `DisabledSecondaryDark` – Disabled secondary background in dark mode
- `HyperLinkNormal` – Default hyperlink-style background  
- `HyperLinkHover` – Hover state for hyperlink-style button  

#### Text Colors
```cpp
btn->setTextColor(ButtonTextColor::SecondaryTextLight, QColor("#000000"));
```
The following are the available button states for text coloring:
- `PrimaryText` – Default text color for primary button  
- `DisabledPrimaryText` – Text color when primary button is disabled  
- `SecondaryTextLight` – Default secondary text color in light mode  
- `DisabledSecondaryTextLight` – Text color when secondary button is disabled in light mode  
- `SecondaryTextDark` – Default secondary text color in dark mode  
- `DisabledSecondaryTextDark` – Text color when secondary button is disabled in dark mode  

> [!NOTE]
> Unicode icons always use Segoe Fluent Icons, regardless of font settings.

> [!NOTE]
> All of the following are false by default:
> `setShadow()`
> `setDarkMode()`
> `setSecondary()`
> `setHyperLink()`
> `setLoaderButton()`
