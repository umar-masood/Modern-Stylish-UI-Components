#include "Button.h"

Button::Button(const QString &text, QWidget *parent) : QPushButton(text, parent) { init(); }
Button::Button(QWidget *parent) : QPushButton(parent) { init(); }

void Button::init()
{
  setCursor(Qt::PointingHandCursor);
  setFocusPolicy(Qt::NoFocus);

  // Shadow Effect
  effect = new SmoothDropShadow(this);
  effect->setOffset(0, 0);
  effect->setColor(QColor(0, 0, 0, 0));
  effect->setBlurRadius(0);
  if (hasShadow) setGraphicsEffect(effect);
  
  // Smooth Shadow Effect animate
  animate = new QPropertyAnimation(effect, "blurRadius", this);
  animate->setDuration(300);
  animate->setEasingCurve(QEasingCurve::InOutQuad);
}

void Button::setShadow(bool value)
{
  hasShadow = value;
  if (hasShadow) setGraphicsEffect(effect);
  else setGraphicsEffect(nullptr);
}

void Button::setHyperLink(bool value)
{
  hyperLink = value;
  if (secondary) secondary = false; 
}

void Button::setHyperLinkColors(const QColor &normalState, const QColor &hoverState)
{
  hyperlinkNormal = normalState; hyperlinkHover = hoverState;
}

void Button::setFontProperties(const QString &family, int pointSize, bool bold, bool italic)
{
  fontFamily = family; fontSize = pointSize; isBold = bold; isItalic = italic;
}

void Button::setGradientColor(bool value, const QString &hex1, const QString &hex2)
{
  useGradient = value;

  baseStart = QColor(hex1);
  baseEnd = QColor(hex2);

  color1 = baseStart;
  color2 = baseEnd;

  update();
}

void Button::setHoverGradientColor(const QString &hex) { hoverColor = QColor(hex); }
void Button::setStartColor(const QColor &c) { color1 = c.name(); update(); }
void Button::setEndColor(const QColor &c) { color2 = c.name(); update(); }
QColor Button::getStartColor() const { return QColor(color1); }
QColor Button::getEndColor() const { return QColor(color2); }

void Button::setDisplayMode(DisplayMode mode)
{
  displayMode = mode;
  if (displayMode == IconOnly) setSecondary(true);
  if (!customSize) setSize(QSize(0, 0));
  update();
}

void Button::setIconPaths(const QString &icon_light, const QString &icon_dark)
{
  light_icon = icon_light;
  dark_icon = icon_dark;
  useUnicodeIcon = false;
}

void Button::setUnicodeIcon(const QString &unicode, int pointSize)
{
  unicodeIcon = unicode;
  unicodeIconSize = pointSize;
  useUnicodeIcon = true;
  update();
}

void Button::setIconSize(QSize s)
{
  IconSize = s;
  useUnicodeIcon = false;
}

void Button::setSize(QSize s)
{
  customSize = true;

  const QSize Icon_text(100, 36);
  const QSize Icon_only(20, 20);
  const QSize Text_under_icon(60, 50);
  const QSize Text_only(100, 36);

  switch (displayMode)
  {
    case IconText:         setFixedSize(s.expandedTo(Icon_text)); break;
    case IconOnly:         setFixedSize(s.expandedTo(Icon_only)); break;
    case TextUnderIcon:    setFixedSize(s.expandedTo(Text_under_icon)); break;
    case TextOnly:         setFixedSize(s.expandedTo(Icon_text)); break;
  }
  update();
}

void Button::setDarkMode(bool value)
{
  isDarkMode = value;
}

void Button::setSecondary(bool value)
{
  secondary = value;
}

bool Button::isDisabledState() const
{
  return !isEnabled();
}

bool Button::isHoverState() const
{
  return isHover && !isPressed;
}

bool Button::isNormalState() const
{
  return !isHover && !isPressed;
}

bool Button::isPressedState() const
{
  return isPressed;
}

bool Button::isIconOnly() const {
  return displayMode == IconOnly;
}

void Button::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

  // Background colors
  QColor backgroundColor;

  QColor disabledSecondaryColor = isDarkMode ? "#555555" : "#E0E0E0";
  QColor disabledPrimaryColor = "#B0E0FF";
  if (isDisabledState()) 
    if (isIconOnly()) backgroundColor = Qt::transparent;  else  backgroundColor = (secondary) ? disabledSecondaryColor : disabledPrimaryColor;
  
  QColor normalSecondaryColor = isDarkMode ? "#2D2D2D" : "#FBFBFB";
  QColor normalPrimaryColor = "#32CCFE";
  if (isNormalState()) backgroundColor = isIconOnly() ? Qt::transparent : (secondary ? normalSecondaryColor : normalPrimaryColor);

  QColor hoverSecondaryColor = isDarkMode ? "#323232" : "#F6F6F6";
  QColor hoverPrimaryColor = "#1BB3E6";
  if (isHoverState()) backgroundColor = (secondary || isIconOnly()) ? hoverSecondaryColor : hoverPrimaryColor;

  QColor pressedSecondaryColor = isDarkMode ? "#1F1F1F" : "#FFFFFF";
  QColor pressedPrimaryColor = "#109AC7";
  if (isPressedState()) backgroundColor = (secondary || isIconOnly()) ? pressedSecondaryColor : pressedPrimaryColor;

  // Border Colors
  QColor borderColor = isDarkMode ? "#4D4D4D" : "#CCCCCC";
  bool showBorder = false;
  if (secondary)
    if (isIconOnly()) showBorder = false;  else  showBorder = true;

  if (!showBorder || useGradient || hyperLink) {
    painter.setPen(Qt::NoPen);
  } else {
    QPen pen(borderColor);
    pen.setWidth(1);
    pen.setStyle(Qt::SolidLine);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
  }

  if (useGradient)
  {
    QLinearGradient gradient(rect().topLeft(), rect().topRight());
    gradient.setColorAt(0, color1);
    gradient.setColorAt(1, color2);
    painter.setBrush(gradient);
  } else if (hyperLink) {
    painter.setBrush(QBrush(Qt::NoBrush));
  } else {
    painter.setBrush(backgroundColor);
  }

  QPainterPath path;
  path.addRoundedRect(rect().adjusted(1, 1, -1, -1), 6, 6);
  painter.drawPath(path);

  // Icon handling
  QPixmap pixmap;

  if (!useUnicodeIcon) {
    QString iconPath = (!secondary) ? dark_icon : (isDarkMode ? dark_icon : light_icon);
    if (!iconPath.isEmpty()) {
      pixmap.load(iconPath);
      pixmap = pixmap.scaled(IconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
  }

  // Set font
  if (useUnicodeIcon) {
    QFont iconFont("Segoe Fluent Icons");
    iconFont.setPixelSize(unicodeIconSize); 
    painter.setFont(iconFont);
  } else {
    QFont font;
    font.setPointSize(displayMode == TextUnderIcon ? 10 : fontSize);
    font.setFamily(fontFamily);
    font.setBold(isBold);
    font.setItalic(isItalic);
    painter.setFont(font);
  }

  QColor textColor;
  if (secondary) {
    if (isDarkMode)
      textColor = QColor("#F0F0F0");
    else
      textColor = QColor("#000000");
  } else if (hyperLink) {
    textColor = isHoverState() ? hyperlinkHover : hyperlinkNormal;
  } else {
    textColor = QColor("#FFFFFF");
  }

  painter.setPen(textColor);
  painter.setOpacity(isPressed ? 0.6 : 1.0);

  // Draw based on mode
  switch (displayMode)
  {
  case IconText:
  {
    int iconX = 12;
    int spacing = iconX;
    int iconY = (rect().height() - (useUnicodeIcon ? unicodeIconSize : pixmap.height())) / 2;
    int textX = iconX + (useUnicodeIcon ? unicodeIconSize : pixmap.width()) + spacing;

    QRect text_area(textX, 0, width() - textX - iconX, height());
    painter.drawText(text_area, Qt::AlignVCenter, text());

    if (useUnicodeIcon)
      painter.drawText(QRect(iconX, 0, unicodeIconSize + 4, height()), Qt::AlignCenter, unicodeIcon);
    else 
      painter.drawPixmap(iconX, iconY, pixmap);
  
    break;
  }

  case IconOnly:
  {
    if (useUnicodeIcon) {
      painter.drawText(rect(), Qt::AlignCenter, unicodeIcon);
    } else {
      int iconX = (rect().width() - pixmap.width()) / 2;
      int iconY = (rect().height() - pixmap.height()) / 2;
      painter.drawPixmap(iconX, iconY, pixmap);
    }
    break;
  }

  case TextOnly:
  {
    if (hyperLink) {
      QRect text_area(0, 0, width(), height());
      painter.drawText(text_area, Qt::AlignLeft | Qt::AlignVCenter, text());
    } else {
      QRect text_area(12, 0, width() - 24, height());
      painter.drawText(text_area, Qt::AlignCenter | Qt::TextWordWrap, text());
    }
    break;
  }

  case TextUnderIcon:
  {
    const int spacing = 6;
    int iconY = spacing;

    int textY = iconY + (useUnicodeIcon ? unicodeIconSize : pixmap.height());
    int textH = height() - unicodeIconSize - 3 * spacing;

    QRect text_area(spacing, textY, width() - 2 * spacing, textH);

    if (useUnicodeIcon) {
      painter.drawText(QRect(0, iconY, width(), unicodeIconSize + 4), Qt::AlignCenter, unicodeIcon);
    } else {
      int iconX = (rect().width() - pixmap.width()) / 2;
      painter.drawPixmap(iconX, iconY, pixmap);
    }

    painter.drawText(text_area, Qt::AlignCenter, text());
    break;
  }

  default:
    break;
  }
}

void Button::mousePressEvent(QMouseEvent *event)
{
  isPressed = true;
  QPushButton::mousePressEvent(event);
  update();
}

void Button::mouseReleaseEvent(QMouseEvent *event)
{
  isPressed = false;
  QPushButton::mouseReleaseEvent(event);
  update();
}

void Button::hideEvent(QHideEvent *event)
{
  isPressed = false;
  isHover = false;
  update();
}

void Button::enterEvent(QEnterEvent *event)
{
  isHover = true;

  if (hasShadow)
  {
    if (!secondary) shadow_color = QColor::fromString("#32CCFE");
    else shadow_color = isDarkMode ? QColor::fromString("#333333") : QColor::fromString("#FFFFFF");

    effect->setColor(shadow_color);
    animate->setStartValue(effect->blurRadius());
    animate->setEndValue(25);
    animate->start();
  }

  if (useGradient && hoverColor.isValid())
  {
    auto *a1 = new QPropertyAnimation(this, "startColor", this);
    a1->setDuration(300);
    a1->setStartValue(color1);
    a1->setEndValue(hoverColor);
    a1->start();

    auto *a2 = new QPropertyAnimation(this, "endColor", this);
    a2->setDuration(300);
    a2->setStartValue(color2);
    a2->setEndValue(hoverColor);
    a2->start();
  }

  QPushButton::enterEvent(event);
  update();
}

void Button::leaveEvent(QEvent *event)
{
  isHover = false;

  if (hasShadow)
  {
    animate->setStartValue(effect->blurRadius());
    animate->setEndValue(0);
    animate->start();
  }

  if (useGradient)
  {
    auto *a1 = new QPropertyAnimation(this, "startColor", this);
    a1->setDuration(300);
    a1->setStartValue(color1);
    a1->setEndValue(baseStart);
    a1->start();

    auto *a2 = new QPropertyAnimation(this, "endColor", this);
    a2->setDuration(300);
    a2->setStartValue(color2);
    a2->setEndValue(baseEnd);
    a2->start();
  }

  QPushButton::leaveEvent(event);
  update();
}
