#include "Button.h"  

Button::Button(const QString &text, QWidget *parent)
    : QPushButton(text, parent) {
  init();
}

Button::Button(QWidget *parent)
    : QPushButton(parent) {
  init();
}

void Button::init() {
  setCursor(Qt::PointingHandCursor);
  setMinimumSize(120, 34);
  setFocusPolicy(Qt::NoFocus);

  // Shadow Effect
  effect = new SmoothDropShadow(this);
  effect->setOffset(0, 0);
  effect->setColor(QColor(0, 0, 0, 0));
  effect->setBlurRadius(0);
  
  if(hasShadow) {
    setGraphicsEffect(effect);
  }

  // Smooth Shadow Effect animate
  animate = new QPropertyAnimation(effect, "blurRadius", this);
  animate->setDuration(300);
  animate->setEasingCurve(QEasingCurve::InOutQuad);
}

void Button::setShadow(bool value) {
  hasShadow = value;
  if (hasShadow) {
    setGraphicsEffect(effect);
  } else {
    setGraphicsEffect(nullptr);
  }
}

void Button::setDisplayMode(DisplayMode mode) {
  displayMode = mode;
  if (displayMode == TextFieldButton) { setSecondary(true); }
  if(!customSize) { setSize(QSize(0,0)); }
  update();
}

void Button::setIconPaths(const QString &icon_light, const QString &icon_dark ) {
  light_icon = icon_light;
  dark_icon = icon_dark;
}

void Button::setIconSize(QSize s) {
  IconSize = s;
}

void Button::setSize(QSize s) {
  customSize = true;

  const QSize Icon_text_size(120, 36);
  const QSize Icon_only_size(36, 36);
  const QSize Text_under_icon_size(56, 50);
  const QSize Text_Field_Button(28,28);

  switch (displayMode) {
    case IconText:
      setFixedSize(s.expandedTo(Icon_text_size));
      break;
    case IconOnly:
      setFixedSize(s.expandedTo(Icon_only_size));
      break;
    case ToolButton:
    case TextUnderIcon:
      setFixedSize(s.expandedTo(Text_under_icon_size));
      break;
    case TextOnly:
      setFixedSize(s.expandedTo(Icon_text_size));
      break;
    case TextFieldButton:
      setFixedSize(s.expandedTo(Text_Field_Button));
      break;
  }
  update();
}

void Button::setDarkMode(bool value) {
  isDarkMode = value;
}

void Button::setSecondary(bool value) {
  secondary = value;
}

void Button::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

  // Background colors
  QColor disabledSecondaryColor   = isDarkMode ? QColor("#555555") : QColor("#E0E0E0");
  QColor disabledPrimaryColor     = QColor("#B0E0FF");

  QColor normalSecondaryColor     = isDarkMode ? QColor("#2D2D2D") : QColor("#FBFBFB");
  QColor normalPrimaryColor       = QColor("#32CCFE");

  QColor hoverSecondaryColor      = isDarkMode ? QColor("#323232") : QColor("#F6F6F6");
  QColor hoverPrimaryColor        = QColor("#1BB3E6");

  QColor pressedSecondaryColor    = isDarkMode ? QColor("#1F1F1F") : QColor("#FFFFFF");
  QColor pressedPrimaryColor      = QColor("#109AC7");

  QColor transparentColor         = QColor(0, 0, 0, 0);

  // Choose background color
  QColor backgroundColor;

  const bool isToolOrTextField = (displayMode == ToolButton || displayMode == TextFieldButton);

  if (!isEnabled()) { // Disabled
    if (displayMode == ToolButton || displayMode == TextFieldButton)
      backgroundColor = Qt::transparent;
    else
      backgroundColor = (secondary) ? disabledSecondaryColor : disabledPrimaryColor;
  }
  else if (!isHover && !isPressed) { // Normal 
    backgroundColor = isToolOrTextField ? transparentColor : (secondary ? normalSecondaryColor : normalPrimaryColor);
  }
  else if (isHover && !isPressed) {  // Hover
    backgroundColor = (secondary || isToolOrTextField) ? hoverSecondaryColor : hoverPrimaryColor;
  }
  else { // Pressed
    backgroundColor = (secondary || displayMode == ToolButton) ? pressedSecondaryColor : pressedPrimaryColor;
  }
  
  QColor borderColor;
  bool showBorder = false;

  if (secondary) {
    if (displayMode == ToolButton || displayMode == TextFieldButton) {
      showBorder = false;
    } else {
      showBorder = true;
    }
  }

  if (!showBorder) {
    painter.setPen(Qt::NoPen);
  } else {
    borderColor = isDarkMode ? QColor("#4D4D4D") : QColor("#CCCCCC");
    QPen pen(borderColor);
    pen.setWidth(1);
    pen.setStyle(Qt::SolidLine);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
  }

  painter.setBrush(backgroundColor);
  QPainterPath path;
  path.addRoundedRect(rect().adjusted(1, 1, -1, -1), 6, 6);
  painter.drawPath(path);

  // Load and scale icon
  QString iconPath;
  if (!secondary) {
    iconPath = dark_icon;
  } else {
    iconPath = (isDarkMode ? dark_icon : light_icon);
  }

  QPixmap pixmap;
  if (!iconPath.isEmpty()) {
    pixmap.load(iconPath);
    pixmap = pixmap.scaled(IconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  }

  // Font setup
  QFont font;
  font.setPointSize(displayMode == TextUnderIcon ? 10 : 11);
  font.setFamily("Segoe UI");
  font.setWeight(QFont::Normal);
  painter.setFont(font);

  painter.setPen(secondary ? (isDarkMode ? QColor("#F0F0F0") : QColor("#000000")) : QColor("#FFFFFF"));

  // Calculate layout
  int spacing;  int iconW = pixmap.width(); int iconH = pixmap.height(); 
  int iconX, iconY, textX, textY, textH; 

  if (isPressed) {
    painter.setOpacity(0.6);
  } else {
    painter.setOpacity(1.0);
  }

  // Icon & Text 
  switch (displayMode) {
    case IconText: {
      const int iconX = 12;
      spacing = iconX;
      iconY = (rect().height() - iconH) / 2;
      textX = iconX + iconW + spacing;

      QRect text_area(textX, 0, width() - textX - iconX, height());

      painter.drawText(text_area, Qt::AlignVCenter, text());
      painter.drawPixmap(iconX, iconY, pixmap);

      break;
    }
   
    case IconOnly:
    case TextFieldButton: {  
      iconX = (rect().width() - iconW) / 2;
      iconY = (rect().height() - iconH) / 2;
      painter.drawPixmap(iconX, iconY, pixmap);
      break;
    }

    case TextOnly: {
      QRect text_area(12, 0, width() - 24, height());
      painter.drawText(text_area, Qt::AlignCenter | Qt::TextWordWrap, text());
      break;
    }
    
    case ToolButton:
    case TextUnderIcon: {
      spacing = std::round(height() * 0.13); // 6px mimimum spacing
  
      iconX = (rect().width() - iconW) / 2;
      iconY = spacing;
  
      textY = iconY + std::round(iconH / 1.7) + spacing;
      textH = height() - iconH / 2 - 3 * spacing;
     
      QRect text_area(spacing, textY, width() - 2 * spacing, textH);

      painter.drawPixmap(iconX, iconY, pixmap);
      painter.drawText(text_area, Qt::AlignCenter, text());
      break;
    }

    default:
      return;
  }
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

  if (hasShadow) {
    if (!secondary) {
      shadow_color = QColor::fromString("#32CCFE");
    } else {
      shadow_color = isDarkMode ? QColor::fromString("#333333") : QColor::fromString("#FFFFFF");
    }

    effect->setColor(shadow_color);
    animate->setStartValue(effect->blurRadius());
    animate->setEndValue(25);
    animate->start();
  }

  QPushButton::enterEvent(event);
  update();
}

void Button::leaveEvent(QEvent *event) {
  isHover = false;

  if (hasShadow) {
    animate->setStartValue(effect->blurRadius());
    animate->setEndValue(0);
    animate->start();
  }

  QPushButton::leaveEvent(event);
  update();
}