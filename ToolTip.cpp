#include "ToolTip.h"

void ToolTip::setToolTip(QWidget *widget, const QString &text, bool isDarkMode) {
  new ToolTip(widget, text, isDarkMode);
}

ToolTip::ToolTip(QWidget *target, const QString &text, bool isDarkMode)
  : QObject(target), target(target) {
  tooltipWidget = new RoundedBox(text);
  tooltipWidget->setAsToolTip(true);
  tooltipWidget->setDarkMode(isDarkMode);
  tooltipWidget->hide();

  target->installEventFilter(this);
  tooltipWidget->installEventFilter(this);

  timer.setSingleShot(true);

  SmoothOpacity *opacityEffect = new SmoothOpacity;
  tooltipWidget->setGraphicsEffect(opacityEffect);

  animation = new QPropertyAnimation(opacityEffect, "opacity");
  animation->setEasingCurve(QEasingCurve::InOutQuad);
  animation->setDuration(300);

  connect(&timer, &QTimer::timeout, this, [this]() {
    showToolTip();
  });
}

void ToolTip::fadeInAnimation() {
  animation->stop();
  disconnect(animation, &QPropertyAnimation::finished, nullptr, nullptr);
  animation->setStartValue(0.0);
  animation->setEndValue(1.0);
  tooltipWidget->show();
  animation->start();
}

void ToolTip::fadeOutAnimation() {
  animation->stop();
  disconnect(animation, &QPropertyAnimation::finished, nullptr, nullptr);
  animation->setStartValue(1.0);
  animation->setEndValue(0.0);
  connect(animation, &QPropertyAnimation::finished, this, [this]() {
    if (tooltipWidget) tooltipWidget->hide();
  });
  animation->start();
}

bool ToolTip::eventFilter(QObject *obj, QEvent *event) {
  if (obj == target) {
    if (event->type() == QEvent::Enter) {
      timer.start(2000);
    } else if (event->type() == QEvent::Leave) {
      timer.stop();
      QTimer::singleShot(200, this, [this]() {
        if (tooltipWidget && !isHovering) {
          fadeOutAnimation();
        }
      });
    }
  }

  if (obj == tooltipWidget) {
    if (event->type() == QEvent::Enter) {
      isHovering = true;
    } else if (event->type() == QEvent::Leave) {
      isHovering = false;
      QTimer::singleShot(200, this, [this]() {
        if (tooltipWidget && !target->underMouse()) {
          fadeOutAnimation();
        }
      });
    }
  }

  return false;
}

void ToolTip::position(QWidget *target) {
  QPoint globalPos = target->mapToGlobal(QPoint(0, 0));
  QScreen *screen = QApplication::screenAt(globalPos);
  if (!screen) {
    screen = QApplication::primaryScreen();
  }
  QRect screenArea = screen->availableGeometry();

  QRect targetRect = QRect(globalPos, target->size());
  QSize tooltipSize = tooltipWidget->sizeHint();

  QPoint abovePos(targetRect.center().x() - tooltipSize.width() / 2, targetRect.top() - tooltipSize.height() - 3);
  QPoint belowPos(targetRect.center().x() - tooltipSize.width() / 2, targetRect.bottom() + 3);
  QPoint leftPos(targetRect.left() - tooltipSize.width() - 3, targetRect.center().y() - tooltipSize.height() / 2);
  QPoint rightPos(targetRect.right() + 3, targetRect.center().y() - tooltipSize.height() / 2);

  QPoint finalPos;
  if (screenArea.contains(QRect(abovePos, tooltipSize)))
    finalPos = abovePos;
  else if (screenArea.contains(QRect(belowPos, tooltipSize)))
    finalPos = belowPos;
  else if (screenArea.contains(QRect(leftPos, tooltipSize)))
    finalPos = leftPos;
  else if (screenArea.contains(QRect(rightPos, tooltipSize)))
    finalPos = rightPos;
  else
    finalPos = belowPos;

  int x = std::clamp(finalPos.x(), screenArea.left(), screenArea.right() - tooltipSize.width());
  int y = std::clamp(finalPos.y(), screenArea.top(), screenArea.bottom() - tooltipSize.height());

  tooltipWidget->move(QPoint(x, y));
}

void ToolTip::showToolTip() {
  if (tooltipWidget) {
    position(target);
    fadeInAnimation();
  }
}
