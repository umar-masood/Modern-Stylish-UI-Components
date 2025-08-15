#include "SpinnerProgress.h"

SpinnerProgress::SpinnerProgress(QWidget *parent) : QWidget(parent) {
   hide();
   setAttribute(Qt::WA_TranslucentBackground, true);

   op = new SmoothOpacity;
   setGraphicsEffect(op);

   animation = new QPropertyAnimation(op, "opacity");
   animation->setDuration(300);
   animation->setEasingCurve(QEasingCurve::InOutQuad);
}

void SpinnerProgress::setSize(QSize s) {
   const QSize minimumSize(250, 140);
    
   int width = qMax(s.width(), minimumSize.width());
   int height = minimumSize.height();
    
   setFixedSize(width, height);
   update();
}

void SpinnerProgress::fadeInAnimation() {
   animation->stop();
   disconnect(animation, &QPropertyAnimation::finished, nullptr, nullptr);
   animation->setStartValue(0.0);
   animation->setEndValue(1.0);
   animation->start();
   show();
}

void SpinnerProgress::fadeOutAnimation() {
   animation->stop();
   disconnect(animation, &QPropertyAnimation::finished, nullptr, nullptr);
   animation->setStartValue(1.0);
   animation->setEndValue(0.0);
   connect(animation, &QPropertyAnimation::finished, this, [this]() {
      this->hide();
   });
   animation->start();
}

void SpinnerProgress::start() {
   if (timer && !timer->isActive() && isIndeterminate) {
      timer->start(16); // ~60 FPS
   }
   fadeInAnimation();
}

void SpinnerProgress::stop() {
   if (isIndeterminate && timer) {
     timer->stop();
   }
   fadeOutAnimation();
}

void SpinnerProgress::setText(const QString &text) {
   loaderText = text;
   update();
}

void SpinnerProgress::setDarkMode(bool value) {
   isDarkMode = value;
   update();
}

void SpinnerProgress::setIndeterminate(bool value) {
   isIndeterminate = value;
   if (isIndeterminate && !timer) {
      timer = new QTimer(this);
      connect(timer, &QTimer::timeout, this, [=]() {
         angle = (angle + 5) % 360;
         update();
      });
   }
}

void SpinnerProgress::setRange(int min, int max) {
   if (min < 0 || max > 100 || min >= max) {
      minimum = 0; maximum = 100;
   } else {
      minimum = min; maximum = max;
   }
}

void SpinnerProgress::setValue(int value) {
   if (value > 100 || value < 0) {
      qWarning() << "Your Progress current value is invalid";
      return;
   }
   currentValue = value;
   update();
}

int SpinnerProgress::getValue() const {
   return currentValue;
}

void SpinnerProgress::paintEvent(QPaintEvent *event) {
   QPainter painter(this);
   painter.setRenderHint(QPainter::Antialiasing);
   painter.setBrush(Qt::NoBrush);

   // Circular background arc
   QPen pen;
   pen.setColor(isDarkMode ? QColor("#383838") : QColor("#F0F0F0"));
   pen.setWidth(6);
   pen.setCapStyle(Qt::RoundCap);
   pen.setJoinStyle(Qt::RoundJoin);
   painter.setPen(pen);

   int size = qMin(width() - 160, height()) - 2 * margin;
   QRectF rec((width() - size) / 2, (height() - size) / 2, size, size);
   double percent = 0.0;
   int spanAngle = 0;
   
   painter.drawArc(rec, 0, 360 * 16);

   // Rotating foreground arc
   pen.setColor(QColor("#109AC7"));
   painter.setPen(pen);
   QPoint center(width() / 2, height() / 2);

   if (isIndeterminate) {
      painter.save();
      painter.translate(center);
      painter.rotate(angle);
      painter.translate(-center);
      painter.drawArc(rec, 0, 60 * 16);
      painter.restore();
   } else {
      percent = static_cast<double>(currentValue - minimum) / (maximum - minimum);
      spanAngle = static_cast<int>(percent * 360 * 16);
      painter.drawArc(rec, 90 * 16, -spanAngle);
   }

   // Text below loader
   QFont font;
   font.setPointSize(11);
   font.setFamily("Segoe UI");
   font.setWeight(QFont::Normal);
   painter.setFont(font);
   painter.setPen(isDarkMode ? QColor("#F0F0F0") : QColor("#000000"));

   if (isIndeterminate) {
      QRect text_area(0, height() - 25, width(), 25);
      painter.drawText(text_area, Qt::AlignHCenter | Qt::AlignVCenter, loaderText); 
   } else {
      QString percentText = QString::number(static_cast<int>(percent * 100)) + "%";
      painter.drawText(rec, Qt::AlignHCenter | Qt::AlignVCenter, percentText); 
   }
}

void SpinnerProgress::showEvent(QShowEvent * event)
{
   QWidget::showEvent(event);

   if (parentWidget()) {
      int x = (parentWidget()->width() - width()) / 2;
      int y = (parentWidget()->height() - height()) / 2;
      move(x, y);
   }
}
