#pragma once 
#include "SmoothShadow.h"
#include "SpinnerProgress.h"

#include <QPushButton>
#include <QPropertyAnimation>
#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QHideEvent>
#include <QEnterEvent>
#include <QGraphicsEffect>
#include <QGraphicsDropShadowEffect>
#include <QLinearGradient>
#include <QPen>
#include <QBrush>
#include <QPixmap>
#include <QFont>
#include <QEasingCurve>
#include <QColor>
#include <QSize>
#include <QRect>
#include <QString>
#include <QWidget>

class Button : public QPushButton
{
  Q_OBJECT
  Q_PROPERTY(QColor startColor READ getStartColor WRITE setStartColor)
  Q_PROPERTY(QColor endColor READ getEndColor WRITE setEndColor)
  
public:
  enum DisplayMode { IconOnly, TextOnly, IconText, TextUnderIcon, };

  explicit Button(const QString &text, QWidget *parent = nullptr);
  explicit Button(QWidget *parent = nullptr);

  void setDisplayMode(DisplayMode mode);
  void setIconPaths(const QString &iconLight = "", const QString &iconDark = "");
  void setUnicodeIcon(const QString &unicode, int pointSize);
  void setSize(QSize s);
  void setIconSize(QSize s);
  void setDarkMode(bool value);
  void setSecondary(bool value);
  void setShadow(bool value);
  void setHyperLink(bool value);
  void setHyperLinkColors(const QColor &normalState, const QColor &hoverState);
  void setFontProperties(const QString &family, int pointSize, bool bold = false, bool italic = false);
  void setGradientColor(bool enable, const QString &hex1, const QString &hex2);
  void setHoverGradientColor(const QString &hex);
  void setStartColor(const QColor &c);
  void setEndColor(const QColor &c);
  void setLoaderButton(bool value);
  void setText(const QString &text);

  protected:
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void enterEvent(QEnterEvent *event) override;
  void leaveEvent(QEvent *event) override;
  void hideEvent(QHideEvent *event) override;

private:
  // Flags
  bool isHover = false;
  bool isPressed = false;
  bool isDarkMode = false;
  bool isSecondary = false;
  bool isShadowEnabled = false;
  bool isCustomSize = false;
  bool isUnicodeIcon = false;
  bool isGradient = false;
  bool isHyperLink = false;
  bool isBold = false;
  bool isItalic = false;
  bool isLoaderBtn = false;
  
  bool isDisabledState() const;
  bool isHoverState() const;
  bool isNormalState() const;
  bool isPressedState() const;
  bool isIconOnly() const;

  void init();
  void drawBorder(QPainter &painter);
  void drawBackground(QPainter &painter, const QColor &bgColor);
  void drawContent(QPainter &painter, const QPixmap &pixmap);

  QColor getStartColor() const;
  QColor getEndColor() const;

  QColor getBackgroundColor() const;
  QColor getTextColor() const;

  // Icons
  QString lightIcon;
  QString darkIcon;

  // Icons Size
  QSize IconSize = QSize(20, 20);
  QString unicodeIcon;
  int unicodeIconSize = 16;

  // Display Mode
  DisplayMode displayMode = IconText;

  // Graphical Effects & Animations
  SmoothShadow *effect;
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

  // Loader
  SpinnerProgress *spinner = nullptr;
};
