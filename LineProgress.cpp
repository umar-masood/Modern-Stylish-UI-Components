#include "LineProgress.h"
#include <QtMath>

LineProgress::LineProgress(QWidget *parent) : QWidget(parent) {
   hide();   

   op = new SmoothOpacity;
   setGraphicsEffect(op);    

   animation = new QPropertyAnimation(op, "opacity");
   animation->setDuration(300);
   animation->setEasingCurve(QEasingCurve::InOutQuad);
}

void LineProgress::setSize(QSize s) {
   const QSize minimumSize(250, 50);
   int width = qMax(s.width(), minimumSize.width());
   int height = minimumSize.height();
   setFixedSize(width, height);
   update();
}

void LineProgress::fadeInAnimation() {
   animation->stop();
   disconnect(animation, &QPropertyAnimation::finished, nullptr, nullptr);
   animation->setStartValue(0.0);
   animation->setEndValue(1.0);
   animation->start();
   show();
}

void LineProgress::fadeOutAnimation() {
   animation->stop();
   disconnect(animation, &QPropertyAnimation::finished, nullptr, nullptr);
   animation->setStartValue(1.0);
   animation->setEndValue(0.0);
   connect(animation, &QPropertyAnimation::finished, this, [this]() {
      this->hide();
   });
   animation->start();
}

void LineProgress::setDarkMode(bool value) {
   isDarkMode = value;
   update();
}

void LineProgress::start() {
   if (timer && !timer->isActive() && isIndeterminate) timer->start(10); 
   fadeInAnimation();
}

void LineProgress::stop() {
   if (timer && isIndeterminate) timer->stop();
   fadeOutAnimation();
}

void LineProgress::setText(const QString &text) {
   loaderText = text;
   update();
}

void LineProgress::setIndeterminate(bool value) {
   isIndeterminate = value;
   if (isIndeterminate && !timer) {
      timer = new QTimer(this);
      connect(timer, &QTimer::timeout, this, [=]() {
         offset = std::fmod(offset + 0.01, 1.0);
         update();
      });
   }
}

void LineProgress::setValue(int value) {
   if (value > 100 || value < 0) {
      qWarning() << "Your Progress current value is invalid";
      return;
   }
   currentValue = std::clamp(static_cast<double>(value) / 100.0, 0.0, 1.0);
   update();
}

int LineProgress::getValue() const {
   return static_cast<int>(std::round(currentValue * 100));
}

void LineProgress::paintEvent(QPaintEvent *event) {
   QPainter painter(this);
   painter.setRenderHint(QPainter::Antialiasing);

   painter.setPen(Qt::NoPen);
   painter.setBrush(isDarkMode ? QColor::fromString("#383838") : QColor::fromString("#F0F0F0"));

   int trackW = width() - 2 * margin;
   QRect trackRec = QRect(margin, 0, trackW, lineHeight);
   painter.drawRoundedRect(trackRec, radius, radius);

   painter.setBrush(QColor("#109AC7"));

   int wBar = trackW / 3;
   int xBar = margin + int(offset * trackW);

   if (isIndeterminate) {
      if (xBar + wBar > trackW + margin) {
         int overflow = (xBar + wBar) - (trackW + margin);
         painter.drawRoundedRect(QRect(xBar, 0, wBar - overflow, lineHeight), radius, radius);
         painter.drawRoundedRect(QRect(margin, 0, overflow, lineHeight), radius, radius);
      } else {
         painter.drawRoundedRect(QRect(xBar, 0, wBar, lineHeight), radius, radius);
      }
   } else {
      int barWidth = int(currentValue * trackW);
      QRect barRec(margin,0, barWidth, lineHeight);
      painter.drawRoundedRect(barRec, radius, radius);
   }

   QFont font;
   font.setPointSize(11);
   font.setFamily("Segoe UI");
   font.setWeight(QFont::Normal);
   painter.setFont(font);
   painter.setPen(isDarkMode ? QColor("#F0F0F0") : QColor("#000000"));

   QRect text_area(0, height() - 25, width(), 30);
   painter.drawText(text_area, Qt::AlignHCenter, loaderText);
}
