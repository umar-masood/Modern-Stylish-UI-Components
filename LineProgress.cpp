#include "LineProgress.h"

LineProgress::LineProgress(QWidget *parent) : QWidget(parent) {
   hide();   
   setAttribute(Qt::WA_TranslucentBackground, true);

   opacity = new SmoothOpacity;
   setGraphicsEffect(opacity);    

   animation = new QPropertyAnimation(opacity, "opacity");
   animation->setDuration(300);
   animation->setEasingCurve(QEasingCurve::InOutQuad);

   loadDefaultColors();

}

void LineProgress::loadDefaultColors() {
   _colors[BackgroundLight] = QColor("#F0F0F0");
   _colors[BackgroundDark]  = QColor("#383838");
   _colors[ForegroundLight] = QColor("#0191DF");
   _colors[ForegroundDark]  = QColor("#0191DF");
}

void LineProgress::setColor(const LineColor &state, const QColor &color) {
   _colors[state] = color;
   update();
}

QColor LineProgress::color(const LineColor &state) const {
   return _colors.value(state, Qt::transparent);
}

void LineProgress::setFixedSize(QSize s) {
   const QSize minimumSize(250, 50);
   int width = qMax(s.width(), minimumSize.width());
   int height = minimumSize.height();

   QWidget::setFixedSize(width, height);
   update();
}

void LineProgress::fadeIn() {
   animation->stop();
   disconnect(animation, &QPropertyAnimation::finished, nullptr, nullptr);
   animation->setStartValue(0.0);
   animation->setEndValue(1.0);
   animation->start();
   show();
}

void LineProgress::fadeOut() {
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
   fadeIn();
}

void LineProgress::stop() {
   if (timer && isIndeterminate) timer->stop();
   fadeOut();
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

   // ------------- Track Line -------------------
   painter.setPen(Qt::NoPen);
   painter.setBrush(isDarkMode ? color(BackgroundDark) : color(BackgroundLight));

   int trackW = width() - 2 * margin;
   QRect trackRec(margin, 0, trackW, lineHeight);

   painter.drawRoundedRect(trackRec, radius, radius);

   //------------ Progress Line --------------------
   painter.setBrush(isDarkMode ? color(ForegroundDark) : color(ForegroundLight));

   int wBar = trackW / 3;
   int xBar = margin + int(offset * trackW);

   if (isIndeterminate) {
      if (xBar + wBar > trackW + margin) {
         int overflow = (xBar + wBar) - (trackW + margin);

         painter.drawRoundedRect(QRect(xBar, 0, wBar - overflow, lineHeight), radius, radius);
         painter.drawRoundedRect(QRect(margin, 0, overflow, lineHeight), radius, radius);
      } else 
         painter.drawRoundedRect(QRect(xBar, 0, wBar, lineHeight), radius, radius);
      
   } else {
      int barWidth = int(currentValue * trackW);
      QRect barRec(margin,0, barWidth, lineHeight);
      painter.drawRoundedRect(barRec, radius, radius);
   }

   // --------------- Loader Text -------------------
   QFont font;
   font.setPointSize(10);
   font.setFamily("Segoe UI");
   font.setWeight(QFont::Normal);
   painter.setFont(font);
   painter.setPen(isDarkMode ? QColor("#F0F0F0") : QColor("#000000"));

   QRect text_area(0, height() - 25, width(), 30);
   painter.drawText(text_area, Qt::AlignHCenter, loaderText);
}
