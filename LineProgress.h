#pragma once
#include "SmoothOpacity.h"

#include <QWidget>
#include <QPropertyAnimation>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QColor>
#include <QEasingCurve>
#include <QFont>
#include <QtMath>
#include <algorithm>

class LineProgress : public QWidget {
   Q_OBJECT

   public:
   explicit LineProgress(QWidget *parent = nullptr);
    
   void setFixedSize(QSize s);
   void setDarkMode(bool value);
   void setText(const QString &text);
   void setIndeterminate(bool value);   

   void start();
   void stop();

   void setValue(int value);
   int getValue() const;

   protected:
   void paintEvent(QPaintEvent *event) override;

   private: 
   void fadeInAnimation();
   void fadeOutAnimation();
   
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
