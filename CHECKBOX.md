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
