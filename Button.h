#ifndef BUTTON_H
#define BUTTON_H

#include <QtWidgets>
#include "SmoothShadow.h"

class Button : public QPushButton
{
  Q_OBJECT

public:
  enum DisplayMode
  {
    IconOnly,
    TextOnly,
    IconText,
    TextUnderIcon,
    TextFieldButton,
    ToolButton
  };

  explicit Button(const QString &text, QWidget *parent = nullptr);
  explicit Button(QWidget *parent = nullptr);

  void setDisplayMode(DisplayMode mode);
  void setIconPaths(const QString &icon_light = "", const QString &icon_dark = "");
  void setUnicodeIcon(const QString &unicode, int pointSize);
  void setSize(QSize s);
  void setIconSize(QSize s);
  void setDarkMode(bool value);
  void setSecondary(bool value);
  void setShadow(bool value);

protected:
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void enterEvent(QEnterEvent *event) override;
  void leaveEvent(QEvent *event) override;
  void hideEvent(QHideEvent *event) override;

private:
  void init();

  // States
  bool isHover = false;
  bool isPressed = false;
  bool isDarkMode = false;
  bool secondary = false;
  bool hasShadow = false;
  bool customSize = false;
  bool useUnicodeIcon = false;

  bool isDisabledState() const;
  bool isHoverState() const;
  bool isNormalState() const;
  bool isPressedState() const;
  bool isToolOrFieldMode() const;


  // Icons
  QString light_icon;
  QString dark_icon;

  // IconSize
  QSize IconSize = QSize(20, 20);

  DisplayMode displayMode = IconText;

  // Graphical Effects & Animations
  SmoothDropShadow *effect;
  QColor shadow_color;

  QPropertyAnimation *animate;

  QString unicodeIcon;
  int unicodeIconSize = 16;
};