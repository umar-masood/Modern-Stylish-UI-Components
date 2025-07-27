#ifndef LINEPROGRESS_H
#define LINEPROGRESS_H

#include <QtWidgets>
#include "SmoothOpacity.h"

class LineProgress : public QWidget {
   Q_OBJECT

public:
   explicit LineProgress(QWidget *parent = nullptr);
    
   void setSize(QSize s);
   void fadeInAnimation();
   void fadeOutAnimation();
   void setDarkMode(bool value);
   void start();
   void stop();
   void setText(const QString &text);
   void setValue(int value);
   void setIndeterminate(bool value);
   int getValue() const;

protected:
   void paintEvent(QPaintEvent *event) override;

private:
   bool isDarkMode = false;
   bool isIndeterminate = false;

   double offset = 0.0;
   double currentValue = 0.0;

   double minimum = 0.0;
   double maximum = 1.0;

   static constexpr int margin = 12;
   static constexpr int lineHeight = 8;
   static constexpr int radius = lineHeight / 2;

   QTimer *timer = nullptr;   
   QPropertyAnimation *animation = nullptr;
   SmoothOpacity *op = nullptr;

   QString loaderText;

};

#endif 
