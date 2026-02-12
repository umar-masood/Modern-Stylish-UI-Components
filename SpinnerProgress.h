#pragma once
#include "SmoothOpacity.h"

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <QColor>
#include <QFont>
#include <QTimer>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QShowEvent>
#include <QString>
#include <QtMath>
#include <algorithm>
#include <QHash>

class SpinnerProgress : public QWidget {
   Q_OBJECT

   public:
   enum SpinnerColor {
     BackgroundLight,
     BackgroundDark,
     ForegroundLight,
     ForegroundDark,
     ButtonForeground
   };

   explicit SpinnerProgress(QWidget *parent = nullptr);

   void start();
   void stop();
   void setText(const QString &text);
   void setDarkMode(bool enable);
   void setFixedSize(QSize s, bool isButton = false);
   void setIndeterminate(bool enable);
   void setRange(int min, int max);
   void setColor(const SpinnerColor &state, const QColor &color);

   void setValue(int value);
   int getValue() const;

   inline uint qHash(const SpinnerColor &state, uint seed = 0) {
      return ::qHash(static_cast<int>(state), seed);
   }

   protected:
   void paintEvent(QPaintEvent *event) override;
   void showEvent(QShowEvent *event) override;

   private:
   void fadeIn();
   void fadeOut();
   void loadDefaultColors();
   QColor color(const SpinnerColor &state) const;

   bool isDarkMode = false;
   bool isIndeterminate = false;
   bool isButtonMode = false;

   const int margin = 12;
   int angle = 0;
   int minimum = 0;
   int maximum = 0;
   int currentValue = 0;

   // Colors
   QHash<SpinnerColor, QColor> _colors;

   QTimer *timer = nullptr;
   QString loaderText;

   QPropertyAnimation *animation = nullptr;
   SmoothOpacity *opacity = nullptr;
};

