#pragma once

#include <QWidget>
#include <QPainter>
#include <QEvent>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QEnterEvent>
#include <QPixmap>
#include <QFont>
#include <QFontMetrics>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QRect>
#include <QString>

class CheckBox : public QWidget
{
   Q_OBJECT
   
   public:
   explicit CheckBox(const QString &text = "", QWidget *parent = nullptr);
   void setDarkMode(bool value);
   bool isChecked() const;
   void setChecked(bool value);

   protected:
   void paintEvent(QPaintEvent *event) override;
   void enterEvent(QEnterEvent *event) override;
   void leaveEvent(QEvent *event) override;
   void mousePressEvent(QMouseEvent *event) override;

   private:
   QFont font() const;

   bool isDarkMode = false;
   bool isHover = false;
   bool _isChecked = false;

   QPixmap pixmap;
   QString text;

   signals:
   void toggled(bool checked);
};
