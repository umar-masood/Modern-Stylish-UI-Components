#include "Button.h"

Button::Button(const QString &text, QWidget *parent) : QPushButton(text, parent) { init(); }

Button::Button(QWidget *parent) : QPushButton(parent) { init(); }

void Button::init() {
  setCursor(Qt::PointingHandCursor);
  setFocusPolicy(Qt::NoFocus);

  // Shadow Effect
  effect = new SmoothShadow(this);
  effect->setOffset(0, 0);
  effect->setColor(QColor(0, 0, 0, 0));
  effect->setBlurRadius(0);

  if (isShadowEnabled) setGraphicsEffect(effect);
  
  // Smooth Shadow Effect animate
  animate = new QPropertyAnimation(effect, "blurRadius", this);
  animate->setDuration(300);
  animate->setEasingCurve(QEasingCurve::InOutQuad);

  // Loader Spinner
  spinner = new SpinnerProgress(this);
  spinner->setIndeterminate(true);
  spinner->setFixedSize(QSize(20, 20), true);
}

void Button::setShadow(bool value) {
  isShadowEnabled = value;
  (isShadowEnabled) ? setGraphicsEffect(effect) : setGraphicsEffect(nullptr);
}

void Button::setHyperLink(bool value) {
  isHyperLink = value;
  if (isSecondary) isSecondary = false; 
}

void Button::setHyperLinkColors(const QColor &normalState, const QColor &hoverState) {
  hyperlinkNormal = normalState; hyperlinkHover = hoverState;
}

void Button::setFontProperties(const QString &family, int pointSize, bool bold, bool italic) {
  fontFamily = family; fontSize = pointSize; isBold = bold; isItalic = italic;
}

void Button::setGradientColor(bool value, const QString &hex1, const QString &hex2) {
  isGradient = value;

  baseStart = QColor(hex1);
  baseEnd = QColor(hex2);

  color1 = baseStart;
  color2 = baseEnd;

  update();
}

void Button::setDisplayMode(DisplayMode mode) {
  displayMode = mode;
  if (displayMode == IconOnly) setSecondary(true);
  update();
}

void Button::setIconPaths(const QString &iconLight, const QString &iconDark) {
  lightIcon = iconLight;
  darkIcon = iconDark;
  isUnicodeIcon = false;
}

void Button::setUnicodeIcon(const QString &unicode, int pointSize) {
  unicodeIcon = unicode;
  unicodeIconSize = pointSize;
  isUnicodeIcon = true;
  update();
}

void Button::setIconSize(QSize s) {
  IconSize = s;
  isUnicodeIcon = false;
}

void Button::setFixedSize(const QSize &s) { 
  if (s.isValid()) customSize = s; 
  QPushButton::setFixedSize(s);
}

void Button::setDarkMode(bool value) { 
  isDarkMode = value; 
  if (isLoaderBtn && spinner) 
    spinner->setDarkMode(value); 
}

void Button::setSecondary(bool value) { isSecondary = value; }

bool Button::isDisabledState() const { return !isEnabled(); }
bool Button::isHoverState() const { return isHover && !isPressed; }
bool Button::isNormalState() const { return !isHover && !isPressed; }
bool Button::isPressedState() const { return isPressed; }
bool Button::isIconOnly() const { return displayMode == IconOnly; }

void Button::setHoverGradientColor(const QString &hex) { hoverColor = QColor(hex); }
void Button::setStartColor(const QColor &c) { color1 = c.name(); update(); }
void Button::setEndColor(const QColor &c) { color2 = c.name(); update(); }

void Button::setLoaderButton(bool value) { isLoaderBtn = value; }

void Button::setText(const QString &text) {
  if (isLoaderBtn && spinner) {
    if (text.isEmpty()) {
      spinner->start();
      spinner->move((width() - spinner->width()) / 2, (height() - spinner->height()) / 2);
    } else {
      spinner->stop();
    }
  }

  QPushButton::setText(text);
}

QColor Button::getStartColor() const { return QColor(color1); }
QColor Button::getEndColor() const { return QColor(color2); }

QColor Button::getBackgroundColor() const {
  // Colors
  QColor disabledSecondary   = isDarkMode ? "#555555" : "#E0E0E0";
  QColor disabledPrimary     = "#B0E0FF";
  QColor normalSecondary     = isDarkMode ? "#2D2D2D" : "#FBFBFB";
  QColor normalPrimary       = "#008EDE";
  QColor hoverSecondary      = isDarkMode ? "#323232" : "#F6F6F6";
  QColor hoverPrimary        = "#1BB3E6";
  QColor pressedSecondary    = isDarkMode ? "#1F1F1F" : "#FFFFFF";
  QColor pressedPrimary      = "#109AC7";

  if (isDisabledState()) 
    return isIconOnly() ? Qt::transparent : (isSecondary ? disabledSecondary : disabledPrimary);

  if (isNormalState()) 
    return isIconOnly() ? Qt::transparent : (isSecondary ? normalSecondary : normalPrimary);
  
  if (isHoverState()) 
    return (isSecondary || isIconOnly()) ? hoverSecondary : hoverPrimary;
  
  if (isPressedState()) 
    return (isSecondary || isIconOnly()) ? pressedSecondary : pressedPrimary;
  
  return normalPrimary; 
}

QColor Button::getTextColor() const {
  if (isSecondary)
    return isDarkMode ? QColor("#F0F0F0") : QColor("#000000");
  else if (isHyperLink)
    return isDisabledState() ? QColor("#555555") : (isHoverState() ? hyperlinkHover : hyperlinkNormal);
  else
    return QColor("#FFFFFF");
}

void Button::drawBorder(QPainter &painter) {
  bool showBorder = isSecondary && !isIconOnly();

  if (!showBorder || isGradient || isHyperLink)
    painter.setPen(Qt::NoPen);
  else {
    QPen pen(isDarkMode ? "#4D4D4D" : "#CCCCCC");
    pen.setWidthF(0.5);
    pen.setStyle(Qt::SolidLine);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
  }
}

void Button::drawBackground(QPainter &painter, const QColor &bgColor) {
  if (isGradient) {
    QLinearGradient gradient(rect().topLeft(), rect().topRight());
    gradient.setColorAt(0, color1);
    if (!isDisabledState())
      gradient.setColorAt(1, color2);
    painter.setBrush(gradient);
  } else if (isHyperLink) {
    painter.setBrush(Qt::NoBrush);
  } else {
    painter.setBrush(bgColor);
  }

  if (!isHyperLink) {
    QPainterPath path;
    path.addRoundedRect(rect().adjusted(1,1,-1,-1), 6, 6);
    painter.drawPath(path);
  }
}

void Button::drawContent(QPainter &painter, const QPixmap &pixmap) {
  QFontMetrics fm(painter.font());
  QSize tSize = fm.size(Qt::TextSingleLine, this->text());
  int textW = tSize.width();
  int textH = tSize.height();
  
  switch (displayMode) {
  case IconText: {
    const int spacing = 8;    
    const int iconX = 10;
    int iconH = isUnicodeIcon ? unicodeIconSize : pixmap.height();
    int iconW = isUnicodeIcon ? unicodeIconSize : pixmap.width();
    int iconY = (height() - iconH) / 2;
    int textX = iconX + iconW + spacing;
    int textY = 0;
    int buttonWidth = iconX + iconW + spacing + textW + 2 * spacing;
 
    if (!customSize.isValid()) 
      QPushButton::setFixedSize(buttonWidth, 36);

    QRect textRect(textX, textY, textW , height());
    painter.drawText(textRect, Qt::AlignVCenter, text());

    if (isUnicodeIcon)
      painter.drawText(QRect(iconX, 0, unicodeIconSize + 4, height()), Qt::AlignCenter, unicodeIcon);
    else
      painter.drawPixmap(iconX, iconY, pixmap);
    break;
  }

  case IconOnly: {
    if (isUnicodeIcon)
      painter.drawText(rect(), Qt::AlignCenter, unicodeIcon);
    else {
      int x = (width() - pixmap.width()) / 2;
      int y = (height() - pixmap.height()) / 2;
      painter.drawPixmap(x, y, pixmap);
    }
    break;
  }

  case TextOnly: {
    int padding = 12;
    int buttonWidth = padding + textW + padding;

    if (customSize.isValid()) {
      textW = width() - 2 * padding;
      setFixedSize(customSize);
    } else {
      QPushButton::setFixedSize(buttonWidth, 36);
    }

    QRect textRect = isHyperLink ? QRect(0, 0, width(), height()) : QRect(padding, 0, textW, height());
    painter.drawText(textRect, isHyperLink ? (Qt::AlignLeft | Qt::AlignVCenter) : (Qt::AlignCenter | Qt::TextWordWrap), text());
    break;
  }

  case TextUnderIcon: {
    const int hspacing = 12;
    const int vSpacing = 6;
    const int gap = 4;
    
    int textY = vSpacing + (isUnicodeIcon ? unicodeIconSize : pixmap.height()) + gap;

    int buttonWidth = hspacing + textW + hspacing;
    int buttonHeight = vSpacing + pixmap.height() + gap + textH + vSpacing;

    if (customSize.isValid()) {
      setFixedSize(customSize);
    } else {
      QPushButton::setFixedSize(buttonWidth, buttonHeight);
    }

    if (isUnicodeIcon)
      painter.drawText(QRect(0, vSpacing, width(), unicodeIconSize + 4), Qt::AlignCenter, unicodeIcon);
    else {
      int x = (width() - pixmap.width()) / 2;
      painter.drawPixmap(x, vSpacing, pixmap);
    }

    QRect textRect(0, textY, width(), textH);  
    painter.drawText(textRect, Qt::AlignHCenter, text());

    break;
  }

  default: break;
  }
}

void Button::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

  // Background
  QColor bgColor = getBackgroundColor();

  // Border
  drawBorder(painter);

  // Brush
  drawBackground(painter, bgColor);

  // Icon
  QPixmap pixmap;
  if (!isUnicodeIcon) {
    QString path = (!isSecondary) ? darkIcon : (isDarkMode ? darkIcon : lightIcon);
      if (!path.isEmpty()) {
        pixmap.load(path);
        pixmap = pixmap.scaled(IconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
      }
  }

  // Font
  QFont font;
  if (isUnicodeIcon) {
    font.setFamily("Segoe Fluent Icons");
    font.setPixelSize(unicodeIconSize);
  } else {
    font.setFamily(fontFamily);
    font.setPointSize(fontSize);
    font.setWeight(isBold ? QFont::DemiBold : QFont::Normal);
    font.setItalic(isItalic);
  }
  
  painter.setFont(font);

  // Text color & opacity
  painter.setPen(getTextColor());
  painter.setOpacity(isPressed ? 0.6 : 1.0);

  // Draw icon & text
  drawContent(painter, pixmap);
}

void Button::mousePressEvent(QMouseEvent *event) {
  isPressed = true;
  QPushButton::mousePressEvent(event);
  update();
}

void Button::mouseReleaseEvent(QMouseEvent *event) {
  isPressed = false;
  QPushButton::mouseReleaseEvent(event);
  update();
}

void Button::hideEvent(QHideEvent *event) {
  isPressed = false;
  isHover = false;
  update();
}

void Button::enterEvent(QEnterEvent *event) {
  isHover = true;

  if (isShadowEnabled && !isIconOnly()) {
    if (!isSecondary) 
      shadow_color = QColor::fromString("#008EDE");
    else 
      shadow_color = isDarkMode ? QColor::fromString("#333333") : QColor::fromString("#FFFFFF");

    effect->setColor(shadow_color);
    animate->setStartValue(effect->blurRadius());
    animate->setEndValue(25);
    animate->start();
  }

  if (isGradient && hoverColor.isValid()) {
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

void Button::leaveEvent(QEvent *event) {
  isHover = false;

  if (isShadowEnabled) {
    animate->setStartValue(effect->blurRadius());
    animate->setEndValue(0);
    animate->start();
  }

  if (isGradient) {
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
