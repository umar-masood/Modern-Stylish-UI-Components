#include "Button.h"

Button::Button(const QString &text, QWidget *parent) : QPushButton(text, parent) { init(); }
Button::Button(QWidget *parent) : QPushButton(parent) { init(); }

void Button::init() {
  setFocusPolicy(Qt::NoFocus);

  // Shadow Effect
  effect = new SmoothShadow(this);
  effect->setOffset(0, 0);
  effect->setColor(QColor(0, 0, 0, 0));
  effect->setBlurRadius(0);

  // Smooth Shadow Effect animate
  animate = new QPropertyAnimation(effect, "blurRadius", this);
  animate->setDuration(300);
  animate->setEasingCurve(QEasingCurve::InOutQuad);

  // Loader Spinner
  spinner = new SpinnerProgress(this);
  spinner->setIndeterminate(true);
  spinner->setFixedSize(QSize(20, 20), true);

  // Loading Default Colors
  loadDefaultColors();
}

void Button::setShadow(bool enable) {
  isShadowEnabled = enable;
  (isShadowEnabled) ? setGraphicsEffect(effect) : setGraphicsEffect(nullptr);
}

void Button::setFontProperties(const QString &family, int pointSize, QFont::Weight weight, bool italic) {
  fontFamily = family; 
  fontSize = pointSize; 
  fontWeight = weight; 
  isItalic = italic;
}

void Button::setGradientColors(const QString &startColor, const QString &endColor, const QString &hoverColor) {
  isGradient = true;

  hoverGradientColor = QColor(hoverColor);
  gradientStart = QColor(startColor);
  gradientEnd = QColor(endColor);

  color1 = gradientStart;
  color2 = gradientEnd;

  update();
}

void Button::setDisplayMode(DisplayMode mode) {
  displayMode = mode;
  
  if (displayMode == IconOnly) 
    setSecondary(true);
  
  update();
}

void Button::setIconPaths(const QString &lightIcon, const QString &darkIcon) {
  isUnicodeIcon = false;

  // Preload pixmaps
  if (!lightIcon.isEmpty()) 
    _lightIcon = IconManager::renderSvg(lightIcon, _iconSize);
  
  if (!darkIcon.isEmpty()) 
    _darkIcon = IconManager::renderSvg(darkIcon, _iconSize);
}

void Button::setUnicodeIcon(const QString &unicode, int pointSize) {
  _unicodeIcon = unicode;
  unicodeIconSize = pointSize;
  isUnicodeIcon = true;

  update();
}

void Button::setIconSize(const QSize &s) {
  _iconSize = s;
  isUnicodeIcon = false;
}

void Button::setFixedSize(const QSize &s) { 
  if (s.isValid()) 
    _customSize = s; 
  
  QPushButton::setFixedSize(s);
}

void Button::setDarkMode(bool enable) { 
  isDarkMode = enable; 

  if (isLoaderBtn && spinner) 
    spinner->setDarkMode(enable); 
}

void Button::setCheckedButtonIcon(const QString &iconPath) { 
  if (!iconPath.isEmpty()) 
    _checkedButtonIcon = IconManager::renderSvg(iconPath, _iconSize);
}

void Button::setPrimaryButtonIcon(const QString &iconPath) { 
  if (!iconPath.isEmpty()) 
    _primaryButtonIcon = IconManager::renderSvg(iconPath, _iconSize);
}

void Button::setRightSideIcon(const QString &iconLight, const QString &iconDark) {
  if (displayMode == Button::IconOnly || displayMode == Button::IconText) {
    hasRightSideIcon = true;

    if (!iconLight.isEmpty()) 
      _rightSideLightIcon = IconManager::renderSvg(iconLight, _iconSize);
    
    if (!iconDark.isEmpty()) 
      _rightSideDarkIcon =  IconManager::renderSvg(iconDark, _iconSize);

  } else {
    qWarning() << "Error: setRightSideIcon() -> Button display mode is not correct.";
    return;
  }
}

void Button::setStartColor(const QColor &c) { color1 = c.name(); update(); }
void Button::setEndColor(const QColor &c) { color2 = c.name(); update(); }
void Button::setSecondary(bool enable) { isSecondary = enable; }
void Button::setHyperLink(bool enable) { 
  isHyperLink = true;
  
  if (isSecondary) 
    isSecondary = false; 
}

void Button::setColor(const ButtonStateColor &state, const QColor &color) {
  _colors[state] = color;
  update();
}

void Button::setTextColor(const ButtonTextColor &type, const QColor &color) {
  _textColors[type] = color;
  update();
}

void Button::setLoaderButton(bool enable) { isLoaderBtn = enable; }
void Button::setBorderTransparent(bool enable) { isBorderTransparent = enable; }
void Button::setNormalBackgroundTransparent(bool enable) { isNormalBackgroundTransparent = enable; }
void Button::setFontXY(int x, int y) { 
  _x = x;
  _y = y;
  update(); 
}

void Button::setText(const QString &text) {
  if (isLoaderBtn && spinner) {
    if (text.isEmpty()) {
      spinner->start();
      spinner->move((width() - spinner->width()) / 2, (height() - spinner->height()) / 2);
    } else 
      spinner->stop();
  }

  QPushButton::setText(text);
}

void Button::loadDefaultColors() {
  _colors.clear();

  // Primary
  _colors[PrimaryNormal]      = QColor("#008EDE");
  _colors[PrimaryHover]       = QColor("#1BB3E6");
  _colors[PrimaryPressed]     = QColor("#109AC7");
  _colors[DisabledPrimary]    = QColor("#B0E0FF");

  // Secondary Light
  _colors[SecondaryNormalLight]   = QColor("#FBFBFB");
  _colors[SecondaryHoverLight]    = QColor("#F0F0F0");
  _colors[SecondaryPressedLight]  = QColor("#FFFFFF");
  _colors[DisabledSecondaryLight] = QColor("#E0E0E0");

  // Secondary Dark
  _colors[SecondaryNormalDark]   = QColor("#2D2D2D");
  _colors[SecondaryHoverDark]    = QColor("#323232");
  _colors[SecondaryPressedDark]  = QColor("#242424");
  _colors[DisabledSecondaryDark] = QColor("#555555");

  // Hyperlink
  _colors[HyperLinkNormal]       = QColor("#008EDE");
  _colors[HyperLinkHover]        = QColor("#15F2FF");

  // Text Colors
  // Normal
  _textColors[PrimaryText]           = QColor("#FFFFFF");
  _textColors[SecondaryTextLight]    = QColor("#000000");
  _textColors[SecondaryTextDark]     = QColor("#F0F0F0");

  // Disabled
  _textColors[DisabledPrimaryText]   = QColor("#FFFFFF");          
  _textColors[DisabledSecondaryTextLight] = QColor("#9E9E9E");   
  _textColors[DisabledSecondaryTextDark] = QColor("#777777");    
}

// ------------------------------- Getters ---------------------------------------------------------------------
bool Button::isDisabledState() const { return !isEnabled(); }
bool Button::isHoverState() const { return isHover && !isPressed; }
bool Button::isNormalState() const { return !isHover && !isPressed; }
bool Button::isPressedState() const { return isPressed; }
bool Button::isIconOnly() const { return displayMode == IconOnly; }

QColor Button::brushColor(const ButtonStateColor &state) const { return _colors.value(state, Qt::transparent); }
QColor Button::penColor(const ButtonTextColor &type) const { return _textColors.value(type, Qt::transparent); }

// For Gradient
QColor Button::getStartColor() const { return QColor(color1); }
QColor Button::getEndColor() const { return QColor(color2); }

// ---------------------------------- Button Background, Border Colors, Icon (Getters) ---------------------------
QColor Button::brush() const {
  if (isDisabledState()) 
    return isSecondary ? brushColor(isDarkMode ? DisabledSecondaryDark : DisabledSecondaryLight)
                       : brushColor(DisabledPrimary);

  if (isCheckable() && isChecked())
    return brushColor(PrimaryPressed);

  if (isNormalState())
    if (isNormalBackgroundTransparent)
      return Qt::transparent;
    else
      return isSecondary ? brushColor(isDarkMode ? SecondaryNormalDark : SecondaryNormalLight)
                         : brushColor(PrimaryNormal);

  if (isHoverState())
    return (isSecondary || isIconOnly()) ? brushColor(isDarkMode ? SecondaryHoverDark : SecondaryHoverLight)
                                         : brushColor(PrimaryHover);

  if (isPressedState() && !isCheckable())
    return (isSecondary || isIconOnly()) ? brushColor(isDarkMode ? SecondaryPressedDark : SecondaryPressedLight)
                                         : brushColor(PrimaryPressed);

  return brushColor(PrimaryNormal);
}

QColor Button::pen() const {
  // Hyperlink button
  if (isHyperLink)
    return isDisabledState() ? QColor("#555555") : (isHoverState() ? brushColor(HyperLinkHover) : brushColor(HyperLinkNormal));

  // Disabled button
  if (isDisabledState())
    if (isSecondary)
      return isDarkMode ? penColor(DisabledSecondaryTextDark) : penColor(DisabledSecondaryTextLight);
    else
      return penColor(DisabledPrimaryText);

  // Secondary button
  if (isSecondary)
    return isDarkMode ? penColor(SecondaryTextDark) : penColor(SecondaryTextLight);

  // Primary button
  return penColor(PrimaryText);
}

QPixmap Button::pixmap() const {
  if (isUnicodeIcon)
    return QPixmap();  

  if (isCheckable() && isChecked())
    return _checkedButtonIcon;
  else if (!isSecondary)
    return _primaryButtonIcon;
  else
    return isDarkMode ? _darkIcon : _lightIcon;
}

QFont Button::font() const {
  QFont fnt;
  
  if (isUnicodeIcon) {
    fnt.setFamily("Segoe Fluent Icons");
    fnt.setPixelSize(unicodeIconSize);
    return fnt;
  }

  fnt.setFamily(fontFamily);
  fnt.setPointSize(fontSize);
  fnt.setWeight(fontWeight);
  fnt.setItalic(isItalic);

  return fnt;
}

// ----------------------------------- Button Designing --------------------------
void Button::drawBorder(QPainter &painter) {
  if (isGradient || isHyperLink || isBorderTransparent)
    painter.setPen(Qt::NoPen);
  else {
    QPen pen(isDarkMode ? "#4D4D4D" : "#CCCCCC");
    pen.setWidthF(0.2);
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

  } else if (isHyperLink) 
    painter.setBrush(Qt::NoBrush);
  else 
    painter.setBrush(bgColor);

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
      const int spacing = 10;    
      const int iconX = spacing;

      int iconH = isUnicodeIcon ? unicodeIconSize : pixmap.height();
      int iconW = isUnicodeIcon ? unicodeIconSize : pixmap.width();
      int iconY = (height() - iconH) / 2;

      int textX = _x != 0 ? _x : iconX + iconW + spacing;
      int textY = _y != 0 ? _y : 0;
      
      int buttonWidth = iconX + (hasRightSideIcon ? (2 * iconW + 2 * spacing) : (iconW + spacing)) + textW + spacing; // Remove unnecessary spacing

      if (!_customSize.isValid()) 
        QPushButton::setFixedSize(buttonWidth, 36);

      QRect textRect(textX, textY, textW , height());
      painter.drawText(textRect, Qt::AlignVCenter, text());

      if (isUnicodeIcon)
        painter.drawText(QRect(iconX, 0, unicodeIconSize + 4, height()), Qt::AlignCenter, _unicodeIcon);
      else
        painter.drawPixmap(iconX, iconY, pixmap);

      if (hasRightSideIcon) 
        painter.drawPixmap((width() - 10 - iconW), iconY, (isDarkMode ? _rightSideDarkIcon : _rightSideLightIcon)); 

      break;
    }

    case IconOnly: {
      if (isUnicodeIcon)
        painter.drawText(rect(), Qt::AlignCenter, _unicodeIcon);
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

      if (_customSize.isValid()) {
        textW = width() - 2 * padding;
        setFixedSize(_customSize);
      } else
        QPushButton::setFixedSize(buttonWidth, 36);

      int textX = _x != 0 ? _x : (isHyperLink ? 0 : padding);
      int textY = _y != 0 ? _y : 0;

      QRect textRect = isHyperLink ? QRect(textX, textY, width(), height()) : QRect(textX, textY, textW, height());

      painter.drawText( textRect, isHyperLink ? (Qt::AlignLeft | Qt::AlignVCenter) : (Qt::AlignCenter | Qt::TextWordWrap), text());

      break;
    }

    case TextUnderIcon: {
      const int hspacing = 12;
      const int vSpacing = 6;
      const int gap = 4;

      int textY = vSpacing + (isUnicodeIcon ? unicodeIconSize : pixmap.height()) + gap;

      int buttonWidth = hspacing + textW + hspacing;
      int buttonHeight = vSpacing + pixmap.height() + gap + textH + vSpacing;

      if (_customSize.isValid()) 
        setFixedSize(_customSize);
      else 
        QPushButton::setFixedSize(buttonWidth, buttonHeight);

      if (isUnicodeIcon)
        painter.drawText(QRect(0, vSpacing, width(), unicodeIconSize + 4), Qt::AlignCenter, _unicodeIcon);
      else {
        int x = (width() - pixmap.width()) / 2;
        painter.drawPixmap(x, vSpacing, pixmap);
      }

      QRect textRect(0, textY, width(), textH);  
      painter.drawText(textRect, Qt::AlignHCenter, text());

      break;
    }

    default:  break;
  }
}

void Button::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

  // Border
  drawBorder(painter);

  // Background
  drawBackground(painter, brush());

  // Font
  painter.setFont(font());

  // Text color & opacity
  painter.setPen(pen());
  painter.setOpacity(isPressed ? 0.6 : 1.0);

  // Draw icon & text
  drawContent(painter, pixmap());
}

// ------------------------------------ Events -------------------------------------
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
      _shadowColor = QColor::fromString("#008EDE");
    else 
      _shadowColor = isDarkMode ? QColor::fromString("#333333") : QColor::fromString("#FFFFFF");

    effect->setColor(_shadowColor);
    animate->setStartValue(effect->blurRadius());
    animate->setEndValue(25);
    animate->start();
  }

  if (isGradient && hoverGradientColor.isValid()) {
    auto *a1 = new QPropertyAnimation(this, "startColor", this);
    a1->setDuration(300);
    a1->setStartValue(color1);
    a1->setEndValue(hoverGradientColor);
    a1->start();

    auto *a2 = new QPropertyAnimation(this, "endColor", this);
    a2->setDuration(300);
    a2->setStartValue(color2);
    a2->setEndValue(hoverGradientColor);
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
    a1->setEndValue(gradientStart);
    a1->start();

    auto *a2 = new QPropertyAnimation(this, "endColor", this);
    a2->setDuration(300);
    a2->setStartValue(color2);
    a2->setEndValue(gradientEnd);
    a2->start();
  }

  QPushButton::leaveEvent(event);
  update();

}

