#include "Toggle.h"

Toggle::Toggle(QWidget *parent) : QPushButton(parent), scale(1.0), offset(0.0), KW(14) {
  init();
}

void Toggle::setShadow(bool value) {
  hasShadow = value;
  if (hasShadow)  setGraphicsEffect(effect);  else  setGraphicsEffect(nullptr);
}

void Toggle::setDarkMode(bool value) { isDarkMode = value; }

void Toggle::init() {
  setFixedSize(38, 22);
  setFocusPolicy(Qt::NoFocus);
  setCheckable(true);
  loadDefaultColors();

  connect(this, &QPushButton::toggled, this, [this](bool checked) {
    isToggled = checked;
    
    animation2->stop();
    animation2->setStartValue(offset);
    animation2->setEndValue(isToggled ? 1.0 : 0.0);
    animation2->start();

    animation3->stop();
    animation3->setKeyValueAt(0.0, 14);
    animation3->setKeyValueAt(0.2, 18);
    animation3->setKeyValueAt(0.4, 20);
    animation3->setKeyValueAt(0.6, 22);
    animation3->setKeyValueAt(0.8, 18);
    animation3->setKeyValueAt(1.0, 14);
    animation3->start();

    if (hasShadow) {
      animation4->stop();
      animation4->setStartValue(effect->blurRadius());
      animation4->setEndValue(isToggled ? 18.0 : 0.0);  
      animation4->start();
    }
    update();
  });

  // Animation # 1 : Scaling
  animation1 = new QPropertyAnimation(this, "scale");
  animation1->setDuration(300);
  animation1->setEasingCurve(QEasingCurve::InOutQuad);

  // Animation # 2 : Offset
  animation2 = new QPropertyAnimation(this, "offset");
  animation2->setDuration(300);
  animation2->setEasingCurve(QEasingCurve::OutQuad);

  // Animation # 2 : Streching/Shrinking
  animation3 = new QPropertyAnimation(this, "KW");
  animation3->setDuration(400);
  animation3->setEasingCurve(QEasingCurve::OutQuad);

  // Shadow Effect
  effect = new SmoothShadow(this);
  effect->setOffset(0, 0);
  effect->setColor(QColor::fromString("#32CCFE"));
  effect->setBlurRadius(0);
  
  if(hasShadow) setGraphicsEffect(effect);
  
  // Animation # 4 : Smooth Shadow Effect
  animation4 = new QPropertyAnimation(effect, "blurRadius", this);
  animation4->setDuration(300);
  animation4->setEasingCurve(QEasingCurve::InOutQuad);
}

qreal Toggle::getScale() { return scale; }
void Toggle::setScale(qreal value) {
  scale = value;
  update();
}

qreal Toggle::getOffset() { return offset; }
void Toggle::setOffset(qreal value) {
  offset = value;
  update();
}

void Toggle::setKW(int value) {
  KW = value;
  update();
}

int Toggle::getKW() { return KW; }

QColor Toggle::color(const ToggleColor &state) const {  return _colors.value(state, Qt::transparent); }

void Toggle::setColor(const ToggleColor &state, const QColor &color) {
  _colors.insert(state, color);
  update();
}

void Toggle::loadDefaultColors() {
  // Background Colors
  _colors[ToggleColor::NormalLight]   = QColor::fromString("#ECECEC");
  _colors[ToggleColor::NormalDark]    = QColor::fromString("#202020");
  _colors[ToggleColor::HoverLight]    = QColor::fromString("#E4E4E4");
  _colors[ToggleColor::HoverDark]     = QColor::fromString("#2D2D2D");
  _colors[ToggleColor::ToggledNormal] = QColor::fromString("#109AC7");
  _colors[ToggleColor::ToggledHover]  = QColor::fromString("#1BB3E6");

  // Border Colors
  _colors[ToggleColor::BorderLight]   = QColor::fromString("#8A8A8A");
  _colors[ToggleColor::BorderDark]    = QColor::fromString("#9F9F9F");
  _colors[ToggleColor::BorderToggled] = QColor::fromString("#109AC7");

  // Knob Colors
  _colors[ToggleColor::NormalKnobLight]  = _colors[ToggleColor::BorderLight];
  _colors[ToggleColor::NormalKnobDark]   = _colors[ToggleColor::BorderDark];
  _colors[ToggleColor::ToggledKnobLight] = QColor::fromString("#FFFFFF");
  _colors[ToggleColor::ToggledKnobDark]  = QColor::fromString("#000000");
}

void Toggle::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setRenderHints(QPainter::Antialiasing);
  
  // Background
  QColor brushColor;
  if (isHover) 
    brushColor = isToggled ? color(ToggleColor::ToggledHover) 
                   : (isDarkMode ? color(ToggleColor::HoverDark) : color(ToggleColor::HoverLight));
  else if (isToggled) 
    brushColor = color(ToggleColor::ToggledNormal);
  else 
    brushColor = isDarkMode ? color(ToggleColor::NormalDark) : color(ToggleColor::NormalLight);

  // Border
  QColor penColor = isToggled ? color(ToggleColor::BorderToggled)
                            : (isDarkMode ? color(ToggleColor::BorderDark) : color(ToggleColor::BorderLight));

  // Knob
  QColor knobBrushColor = isToggled ? (isDarkMode ? color(ToggleColor::ToggledKnobDark) : color(ToggleColor::ToggledKnobLight))
                         : (isDarkMode ? color(ToggleColor::NormalKnobDark)  : color(ToggleColor::NormalKnobLight));

  // Outer
  painter.setPen(penColor);
  painter.setBrush(brushColor);

  QRect rec = rect().adjusted(1, 1, -1, -1);
  painter.drawRoundedRect(rec, height() / 2, height() / 2);

  // Knob
  painter.setPen(Qt::NoPen);
  painter.setBrush(knobBrushColor);
  painter.save();

  int KH = 14;
  int range = width() - 8 - KW;
  int X = 4 + offset * range;
  int Y = (height() - KH) / 2;

  QRect knob(X, Y, KW, KH);

  // Scaling
  QPointF c = knob.center();
  painter.translate(c);
  painter.scale(scale, scale);
  painter.translate(-c);

  painter.drawRoundedRect(knob, knob.height() / 2, knob.height() / 2);
  painter.restore();
}

void Toggle::enterEvent(QEnterEvent *event) {
  isHover = true;

  animation1->stop();
  animation1->setStartValue(scale);
  animation1->setEndValue(1.15);
  animation1->start();
  
  QPushButton::enterEvent(event);
  update();
}

void Toggle::leaveEvent(QEvent *event) {
  isHover = false;

  animation1->stop();
  animation1->setStartValue(scale);
  animation1->setEndValue(1.0);
  animation1->start();

  QPushButton::leaveEvent(event);
  update();
}

void Toggle::mousePressEvent(QMouseEvent *event) {
  if(event->button() == Qt::LeftButton) {
    isDragging = false;
    dragStartX = event->pos().x();
    dragOffset = offset;

    event->accept();
    return;
  } 

  QPushButton::mousePressEvent(event);
}

void Toggle::mouseMoveEvent(QMouseEvent *event) {
  if (event->buttons() & Qt::LeftButton) {
    int currentX = event->pos().x();
    int dx =  currentX - dragStartX;

    if (!isDragging && std::abs(dx) >= 4)
      isDragging = true;
    
    if (isDragging) {
      int range = width() - 8 - KW;
      qreal newOffset = dragOffset + (qreal)dx / range;
      newOffset = qBound(0.0, newOffset, 1.0);

      setOffset(newOffset);

      update();
      event->accept();
      return;
    }
  }

  QPushButton::mouseMoveEvent(event);
}

void Toggle::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    if (isDragging) {
      isDragging = false;
      bool toggleState = (offset >= 0.5);

      if (toggleState != isToggled) {
        setChecked(toggleState);
      } else {
        animation2->stop();
        animation2->setStartValue(offset);
        animation2->setEndValue(isToggled ? 1.0 : 0.0);
        animation2->start();
      }
      
      event->accept();
      return;

    } else {
      setChecked(!isToggled);

      event->accept();
      return;
    }
  }

  QPushButton::mouseReleaseEvent(event);
}
