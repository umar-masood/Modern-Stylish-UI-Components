#include "ScrollBar.h"

ScrollBar::ScrollBar(Qt::Orientation orientation, QWidget *parent) : QScrollBar(orientation, parent) {
  init();
}

ScrollBar::ScrollBar(QWidget *parent) : QScrollBar(parent) {
  init();
}

void ScrollBar::setDarkMode(bool value) {
  isDarkMode = value;
  update();
}

void ScrollBar::init() {
  isDarkMode = false;

  animation1 = new QPropertyAnimation(this, "opacity");
  animation1->setDuration(300);
  animation1->setEasingCurve(QEasingCurve::InOutQuad);

  animation2 = new QPropertyAnimation(this, "handle");
  animation2->setDuration(300);
  animation2->setEasingCurve(QEasingCurve::InOutQuad);

  setSingleStep(20);  
  setPageStep(20); 
}

void ScrollBar::setOpacity(qreal o) {
  opacity = o;
  update();
}

qreal ScrollBar::getOpacity() const { return opacity; }
void ScrollBar::setHandle(qreal w) { 
  handle = w;
  update();
}

qreal ScrollBar::getHandle() { return handle; }
void ScrollBar::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  QColor light_area = QColor("#F0F0F0");
  QColor dark_area = QColor("#383838");

  QColor currentColorArea = isHover ? (isDarkMode ? dark_area : light_area) : Qt::transparent;

  QRect area = rect();
  int areaWidth, areaHeight, radius = 4;
  if (orientation() == Qt::Vertical) {
    areaWidth = isHover ? 6 : 0;
    areaHeight = area.height();
  } else {
    areaWidth = area.width();
    areaHeight = isHover ? 6 : 0;
  }

  painter.setPen(Qt::NoPen);
  painter.setBrush(currentColorArea);
  painter.setOpacity(opacity);
  painter.drawRoundedRect(QRect(area.x() + 2, area.y(), areaWidth, areaHeight), radius, radius);

  QColor light_handle = QColor("#CCCCCC");
  QColor dark_handle = QColor("#707070");

  QColor currentColorHandle = isDarkMode ? dark_handle : light_handle;

  int height, width, x, y;
  int range = maximum() - minimum();
  if (range < 0) return;

  if (orientation() == Qt::Vertical) {
    height = std::max(20, static_cast<int>(double(pageStep()) / (range + pageStep()) * areaHeight) + 20);
    width = static_cast<int>(handle);
    x = 2;
    y = static_cast<int>((area.height() - height) * (static_cast<double>(value()) / range));
  } else {
    height = static_cast<int>(handle);
    width = std::max(20, static_cast<int>(double(pageStep()) / (range + pageStep()) * areaWidth));
    x = static_cast<int>((area.width() - width) * (static_cast<double>(value()) / range));
    y = 2;
  }

  painter.setPen(Qt::NoPen);
  painter.setBrush(currentColorHandle);
  painter.setOpacity(1.0);
  painter.drawRoundedRect(QRect(x, y, width, height), radius, radius);
}

void ScrollBar::mousePressEvent(QMouseEvent *event) {
  isPressed = true;
  QScrollBar::mousePressEvent(event);
  update();
}

void ScrollBar::enterEvent(QEnterEvent *event) {
  isHover = true;
  animation1->stop();
  animation1->setStartValue(opacity);
  animation1->setEndValue(1.0);
  animation1->start();

  animation2->stop();
  animation2->setStartValue(handle);
  animation2->setEndValue(6.0);
  animation2->start();

  QScrollBar::enterEvent(event);
  update();
}

void ScrollBar::leaveEvent(QEvent *event) {
  isHover = false;
  animation1->stop();
  animation1->setStartValue(opacity);
  animation1->setEndValue(0.0);
  animation1->start();

  animation2->stop();
  animation2->setStartValue(handle);
  animation2->setEndValue(2.0);
  animation2->start();

  QScrollBar::leaveEvent(event);
  update();
}
