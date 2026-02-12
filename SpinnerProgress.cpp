#include "SpinnerProgress.h"

SpinnerProgress::SpinnerProgress(QWidget *parent) : QWidget(parent) {
   hide();
   setAttribute(Qt::WA_TranslucentBackground, true);

   opacity = new SmoothOpacity;
   setGraphicsEffect(opacity);

   animation = new QPropertyAnimation(opacity, "opacity");
   animation->setDuration(300);
   animation->setEasingCurve(QEasingCurve::InOutQuad);

   loadDefaultColors();
}

void SpinnerProgress::setFixedSize(QSize s, bool isButton) {
   const QSize minimumSize(250, 140);
   int width, height;

   isButtonMode = isButton;

   if (!isButtonMode) {
      width = qMax(s.width(), minimumSize.width());
      height = minimumSize.height();
   } else {
      width = s.width(); height = s.height();
   }

   QWidget::setFixedSize(width, height);
   update();
}

void SpinnerProgress::fadeIn() {
   animation->stop();
   disconnect(animation, &QPropertyAnimation::finished, nullptr, nullptr);
   animation->setStartValue(0.0);
   animation->setEndValue(1.0);
   animation->start();
   show();
}

void SpinnerProgress::fadeOut() {
   animation->stop();
   disconnect(animation, &QPropertyAnimation::finished, nullptr, nullptr);
   animation->setStartValue(1.0);
   animation->setEndValue(0.0);
   connect(animation, &QPropertyAnimation::finished, this, &QWidget::hide);
   animation->start();
}

void SpinnerProgress::start() {
   if (timer && !timer->isActive() && isIndeterminate) 
      timer->start(16); // ~60 FPS
   
   fadeIn();
}

void SpinnerProgress::stop() {
   if (isIndeterminate && timer) 
     timer->stop();
   
   fadeOut();
}

void SpinnerProgress::setText(const QString &text) {
   loaderText = text;
   update();
}

void SpinnerProgress::setDarkMode(bool enable) {
   isDarkMode = enable;
   update();
}

void SpinnerProgress::setIndeterminate(bool enable) {
   isIndeterminate = enable;
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
      qWarning() << "Error: Spinner Progress :: Your Progress current value is invalid";
      return;
   }

   currentValue = value;
   update();
}

int SpinnerProgress::getValue() const { return currentValue; }

void SpinnerProgress::loadDefaultColors() {
   _colors[BackgroundLight]   = QColor("#F0F0F0");
   _colors[BackgroundDark]    = QColor("#383838");
   _colors[ForegroundLight]   = QColor("#0191DF");
   _colors[ForegroundDark]    = QColor("#0191DF");
   _colors[ButtonForeground]  = QColor("#FFFFFF");
}

void SpinnerProgress::setColor(const SpinnerColor &state, const QColor &color) {
   _colors[state] = color;
   update();
}

QColor SpinnerProgress::color(const SpinnerColor &state) const { return _colors.value(state, Qt::transparent); }

void SpinnerProgress::paintEvent(QPaintEvent *event) {
   QPainter painter(this);
   painter.setRenderHint(QPainter::Antialiasing);
   painter.setBrush(Qt::NoBrush);

   // Circular background arc
   QPen pen;
   pen.setWidth(isButtonMode ? 3 : 6);
   pen.setCapStyle(Qt::RoundCap);
   pen.setJoinStyle(Qt::RoundJoin);
   pen.setColor(isDarkMode ? color(BackgroundDark) : color(BackgroundLight));

   int size;
   if (isButtonMode)
      size = qMin(width(), height()) - 4;
   else
      size = qMin(width() - 160, height()) - 2 * margin;

   QRectF rec((width() - size) / 2, (height() - size) / 2, size, size);
   double percent = 0.0;
   int spanAngle = 0;

   if (!isButtonMode) {
      painter.setPen(pen);
      painter.drawArc(rec, 0, 360 * 16);
   }

   // Rotating foreground arc
   pen.setColor(isButtonMode ? color(ButtonForeground) : color(ForegroundLight));
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
   painter.setPen(isDarkMode ? color(BackgroundLight) : QColor("#000000"));

   if (isIndeterminate) {
      QRect text_area(0, height() - 25, width(), 25);
      painter.drawText(text_area, Qt::AlignHCenter | Qt::AlignVCenter, loaderText); 
   } else {
      percent = static_cast<double>(currentValue - minimum) / (maximum - minimum);
      QString percentText = QString::number(static_cast<int>(percent * 100)) + "%";
      painter.drawText(rec, Qt::AlignHCenter | Qt::AlignVCenter, percentText); 
   }
}

void SpinnerProgress::showEvent(QShowEvent * event) {
   QWidget::showEvent(event);

   if (parentWidget()) {
      int x = (parentWidget()->width() - width()) / 2;
      int y = (parentWidget()->height() - height()) / 2;
      move(x, y);
   }
}
