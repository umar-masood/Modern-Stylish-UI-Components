#include <QtWidgets>
#pragma once 
#include "SmoothShadow.h"

class Button : public QPushButton
{
  Q_OBJECT
  Q_PROPERTY(QColor startColor READ getStartColor WRITE setStartColor)
  Q_PROPERTY(QColor endColor READ getEndColor WRITE setEndColor)
  
public:
  enum DisplayMode
  {
    IconOnly,
    TextOnly,
    IconText,
    TextUnderIcon,
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
  void setHyperLink(bool value);
  void setHyperLinkColors(const QColor &normalState, const QColor &hoverState);\
  void setFontProperties(const QString &family, int pointSize, bool bold = false, bool italic = false);
  void setGradientColor(bool enable, const QString &hex1, const QString &hex2);
  void setHoverGradientColor(const QString &hex);
  void setStartColor(const QColor &c);
  void setEndColor(const QColor &c);
  QColor getStartColor() const;
  QColor getEndColor() const;

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
  bool useGradient = false;
  bool hyperLink = false;
  bool isBold = false;
  bool isItalic = false;

  bool isDisabledState() const;
  bool isHoverState() const;
  bool isNormalState() const;
  bool isPressedState() const;
  bool isIconOnly() const;

  // Icons
  QString light_icon;
  QString dark_icon;

  // Icons Size
  QSize IconSize = QSize(20, 20);
  QString unicodeIcon;
  int unicodeIconSize = 16;

  // Display Mode
  DisplayMode displayMode = IconText;

  // Graphical Effects & Animations
  SmoothDropShadow *effect;
  QColor shadow_color;
  QPropertyAnimation *animate;

  // Gradient colours
  QColor baseStart, baseEnd;   
  QColor hoverColor;         
  QColor color1, color2;
  
  // HyperLink Colors
  QColor hyperlinkNormal, hyperlinkHover;

  // Font 
  QString fontFamily = "Segoe UI";
  int fontSize = 11;
};
