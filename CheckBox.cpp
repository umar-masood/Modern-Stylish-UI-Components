#include "CheckBox.h"

CheckBox::CheckBox(const QString &text, QWidget *parent) : QWidget(parent), text(text) {
   setAttribute(Qt::WA_TranslucentBackground);
   setAttribute(Qt::WA_Hover);

   pixmap = QPixmap(IconManager::icon(Icons::CheckBox_Check)).scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation);

   setFixedSize(20 + 12 + QFontMetrics(font()).horizontalAdvance(text), 22);
}

void CheckBox::setDarkMode(bool value) {
   isDarkMode = value;
   update();
}

bool CheckBox::isChecked() const {
   return _isChecked;
}

void CheckBox::setChecked(bool value) {
   if (_isChecked == value)
      return;
   _isChecked = value;

   emit toggled(_isChecked);
   update();
}

QFont CheckBox::font() const {
   QFont fnt;
   fnt.setPointSize(10);
   fnt.setFamily("Segoe UI");
   fnt.setWeight(QFont::Normal);
   return fnt;
}

void CheckBox::paintEvent(QPaintEvent *) {
   QPainter painter(this);
   painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

   QRect rec(1, 1, 20 ,20);

   QPen pen;
   pen.setWidthF(isChecked() ? 1.0 : 0.5);
   pen.setColor(isChecked() ? QColor("#0191DF") : (isDarkMode ? QColor("#4D4D4D") : QColor("#CCCCCC")));
   pen.setStyle(Qt::SolidLine);
   pen.setJoinStyle(Qt::RoundJoin);
   painter.setPen(pen);

   QColor bg_color;
   if (isChecked()) 
      bg_color = isHover ? QColor("#1BB3E6") : QColor("#0191DF");
   else if (isHover) 
      bg_color = isDarkMode ? QColor("#323232") : QColor("#F0F0F0");
   else 
      bg_color = isDarkMode ? QColor("#2D2D2D") : QColor("#FBFBFB");
    
   painter.setBrush(bg_color);
   painter.drawRoundedRect(rec, 6, 6);

   if (!pixmap.isNull() && isChecked()) {
      int xPos = rec.x() + (rec.width() - pixmap.width()) / 2;
      int yPos = rec.y() + (rec.height() - pixmap.height()) / 2;
      painter.drawPixmap(xPos, yPos, pixmap);
   }

   painter.setFont(font());
   painter.setPen(QPen(QColor("#8D8D8D")));

   QRect textRec(rec.right() + 12, 2, width() - rec.width() - 12, height());
   painter.drawText(textRec, text);
}

void CheckBox::enterEvent(QEnterEvent *event) {
   isHover = true;
   QWidget::enterEvent(event);
   update();
}

void CheckBox::leaveEvent(QEvent *event) {
   isHover = false;
   QWidget::leaveEvent(event);
   update();
}

void CheckBox::mousePressEvent(QMouseEvent *event) {
   setChecked(!_isChecked);
   QWidget::mousePressEvent(event);
}
