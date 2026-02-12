#include "ToolTip.h"

ToolTip::ToolTip(QWidget *target, const QString &text, QObject *parent) : QObject(parent), _target(target) {
  tooltipWidget = new RoundedBox(true, nullptr);
  tooltipWidget->setAsToolTip(true);
  tooltipWidget->setDarkMode(isDarkMode);
  tooltipWidget->hide();

  if (_target) 
    _target->installEventFilter(this);

  tooltipWidget->installEventFilter(this);

  timer.setSingleShot(true);

  SmoothOpacity *opacityEffect = new SmoothOpacity;
  tooltipWidget->setGraphicsEffect(opacityEffect);

  animation = new QPropertyAnimation(opacityEffect, "opacity");
  animation->setEasingCurve(QEasingCurve::InOutQuad);
  animation->setDuration(300);

  connect(this, &ToolTip::textEntered, this, &ToolTip::onTextEntered);
  connect(&timer, &QTimer::timeout, this, &ToolTip::onTimeout);
  connect(this, &ToolTip::themeModeChanged, this, &ToolTip::onThemeModeChanged);
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
  /*
  if the current obj points to target:
  -> When cursor enters that widget, a timer of 2 secs will start. If a user stays on that widget even after time passed then tooltip     will appear.
  -> For other events, we stop the timer and starts another timer of 150 ms. If a user cursor is not inside a tooltip, it will disappear.
  */
  if (obj == _target) {
    switch (event->type()) {
      case QEvent::Enter:
        timer.start(2000);
        break;

      case QEvent::Leave:
      case QEvent::MouseButtonPress:
      case QEvent::MouseButtonDblClick:
      case QEvent::Wheel:
      case QEvent::FocusIn:
      case QEvent::Destroy:
      case QEvent::Hide:
        timer.stop();
        QTimer::singleShot(150, this, [this](){
          if (tooltipWidget && !isHovering) 
            fadeOutAnimation();
        });

      default: break;
    }
  }

  /*
    if the current obj points to tooltipWidget:
    -> When cursor enters the tooltip, we mark it as hovering so it stays visible.
      This prevents the tooltip from disappearing when the cursor moves from
      target widget to tooltip.

    -> When cursor leaves the tooltip, we wait for a short delay (150 ms).
      If during this time the cursor does NOT return to the target widget,
      the tooltip will fade out and disappear.
  */

  if (obj == tooltipWidget) {
    switch (event->type()) {
      case QEvent::Enter:
        isHovering = true;
        break;

      case QEvent::Leave:
        isHovering = false;
        QTimer::singleShot(150, this, [this](){
          if (tooltipWidget && !_target->underMouse())
            fadeOutAnimation();
        });
        break;

      default: break;
    }
  }

  if (event->type() == QEvent::ApplicationStateChange) {
    timer.stop();
    fadeOutAnimation();
  }
  
  return false;
}

void ToolTip::position(QWidget *target) {
  QPoint globalPos = target->mapToGlobal(QPoint(0, 0));
  QScreen *screen = QApplication::screenAt(globalPos);
  if (!screen) 
    screen = QApplication::primaryScreen();
  
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

void ToolTip::onTextEntered(const QString &text) {
  if (tooltipWidget) 
    tooltipWidget->setText(text);
}

void ToolTip::onTimeout() { showToolTip(); }
void ToolTip::setText(const QString &text){ emit textEntered(text); }

void ToolTip::showToolTip() {
  if (tooltipWidget) {
    position(_target);
    fadeInAnimation();
  }
}

void ToolTip::setDarkMode(bool enable) { 
  if (isDarkMode == enable)
    return;
  
  isDarkMode = enable;
  emit themeModeChanged(isDarkMode); 
}

void ToolTip::setTargetWidget(QWidget *target) {
  if (_target) 
    _target->removeEventFilter(this);

  _target = target;

  if (!target) {
    timer.stop();

    if (animation) 
      animation->stop();

    hide();
    return;
  }

  _target->installEventFilter(this);
}

void ToolTip::onThemeModeChanged(bool enable) {
  if (tooltipWidget) 
    tooltipWidget->setDarkMode(enable);
}

void ToolTip::hide() {
  if (tooltipWidget) 
    tooltipWidget->hide();
}