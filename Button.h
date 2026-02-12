#pragma once 
#include "SmoothShadow.h"
#include "SpinnerProgress.h"
#include "../../resources/IconManager.h"

#include <QPushButton>
#include <QPainterPath>
#include <QMouseEvent>
#include <QHideEvent>
#include <QEnterEvent>
#include <QGraphicsEffect>
#include <QGraphicsDropShadowEffect>
#include <QLinearGradient>
#include <QBrush>
#include <QPixmap>
#include <QSize>
#include <QRect>
#include <QHash>

class Button : public QPushButton {
  Q_OBJECT
  Q_PROPERTY(QColor startColor READ getStartColor WRITE setStartColor)
  Q_PROPERTY(QColor endColor READ getEndColor WRITE setEndColor)
  
  public:

  enum DisplayMode { 
    IconOnly, 
    TextOnly, 
    IconText, 
    TextUnderIcon, 
  };

  enum ButtonStateColor {
    PrimaryNormal,
    PrimaryHover,
    PrimaryPressed,

    DisabledPrimary,

    SecondaryNormalLight,
    SecondaryHoverLight,
    SecondaryPressedLight,

    DisabledSecondaryLight,
    DisabledSecondaryDark,

    SecondaryNormalDark,
    SecondaryHoverDark,
    SecondaryPressedDark,

    HyperLinkNormal,
    HyperLinkHover,
  };

  enum ButtonTextColor {
    PrimaryText,
    SecondaryTextLight,
    SecondaryTextDark,

    DisabledPrimaryText,
    DisabledSecondaryTextLight,
    DisabledSecondaryTextDark,
  };

  explicit Button(const QString &text, QWidget *parent = nullptr);
  explicit Button(QWidget *parent = nullptr);

  void setDisplayMode(DisplayMode mode);
  void setIconPaths(const QString &iconLight = QString(), const QString &iconDark = QString());
  void setUnicodeIcon(const QString &unicode, int pointSize);
  void setFixedSize(const QSize &s);
  void setIconSize(const QSize &s);
  void setDarkMode(bool enable);
  void setSecondary(bool enable);
  void setShadow(bool enable);
  void setHyperLink(bool enable);
  /**
   * @brief Sets the background color of button
   */
  void setColor(const ButtonStateColor &state, const QColor &color);
  void setTextColor(const ButtonTextColor &type, const QColor &color);
  void setFontProperties(const QString &family, int pointSize, QFont::Weight weight = QFont::Normal, bool italic = false);
  void setGradientColors(const QString &startColor, const QString &endColor, const QString &hoverColor);
  void setStartColor(const QColor &c);
  void setEndColor(const QColor &c);
  void setLoaderButton(bool enable);
  void setText(const QString &text);
  void setBorderTransparent(bool enable);
  void setNormalBackgroundTransparent(bool enable);
  void setCheckedButtonIcon(const QString &iconPath);
  void setPrimaryButtonIcon(const QString &iconPath);
  /** @warning Button Display Mode should be IconOnly or IconText*/
  void setRightSideIcon(const QString &iconLight, const QString &iconDark);
  void setFontXY(int x, int y);

  inline uint qHash(const ButtonStateColor &color, uint seed = 0) {
    return ::qHash(static_cast<int>(color), seed);
  }

  inline uint qHash(const ButtonTextColor &color, uint seed = 0) {
    return ::qHash(static_cast<int>(color), seed);
  }

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
  bool isUnicodeIcon = false;
  bool isGradient = false;
  bool isHyperLink = false;
  bool isItalic = false;
  bool isLoaderBtn = false;
  bool isBorderTransparent = false;
  bool isNormalBackgroundTransparent = false;
  bool hasRightSideIcon = false;

  // Button State Getters
  bool isDisabledState() const;
  bool isHoverState() const;
  bool isNormalState() const;
  bool isPressedState() const;
  bool isIconOnly() const;

  // Helpers
  void init();
  void loadDefaultColors();
  void drawBorder(QPainter &painter);
  void drawBackground(QPainter &painter, const QColor &bgColor);
  void drawContent(QPainter &painter, const QPixmap &pixmap);

  // Gradient Getters
  QColor getStartColor() const;
  QColor getEndColor() const;

  QColor brushColor(const ButtonStateColor &state) const;
  QColor brush() const;

  QColor penColor(const ButtonTextColor &type) const;
  QColor pen() const;

  QPixmap pixmap() const;
  QFont font() const;

  // Icons
  QPixmap _lightIcon, _darkIcon, _primaryButtonIcon, _checkedButtonIcon, _rightSideLightIcon, _rightSideDarkIcon;

  // Icons Size
  QSize _iconSize = QSize(20, 20);
  QString _unicodeIcon;
  int unicodeIconSize = 16;

  // Display Mode
  DisplayMode displayMode = IconText;
  QSize _customSize;

  // Graphical Effects & Animations
  SmoothShadow *effect;
  QPropertyAnimation *animate;

  // Button Shadow Color
  QColor _shadowColor;

  // Gradient Button Colors
  QColor gradientStart, gradientEnd, hoverGradientColor, color1, color2;
  
  // Button States Colors
  QHash<ButtonStateColor, QColor> _colors;

  // Button Text Colors
  QHash<ButtonTextColor, QColor> _textColors;

  // Font 
  QString fontFamily = "Segoe UI";
  int fontSize = 10;
  QFont::Weight fontWeight = QFont::Normal;
  // Font Adjustment
  int _x = 0, _y = 0;

  // Loader
  SpinnerProgress *spinner = nullptr;
};