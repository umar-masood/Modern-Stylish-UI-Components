#ifndef SPINNERPROGRESS_H
#define SPINNERPROGRESS_H

#include <QtWidgets>
#include "SmoothOpacity.h"

class SpinnerProgress : public QWidget {
   Q_OBJECT

public:
   explicit SpinnerProgress(QWidget *parent = nullptr);

   void start();
   void stop();
   void setText(const QString &text);
   void setDarkMode(bool value);
   void setSize(QSize s);
   void setIndeterminate(bool value);
   void setValue(int value);
   int getValue() const;
   void setRange(int min, int max);

protected:
   void paintEvent(QPaintEvent *event) override;
   void showEvent(QShowEvent *event) override;

private:
   void fadeInAnimation();
   void fadeOutAnimation();

   static constexpr int margin = 12;

   bool isDarkMode = false;
   bool isIndeterminate = false;

   int angle = 0;
   int minimum = 0;
   int maximum = 0;
   int currentValue = 0;

   QTimer *timer = nullptr;
   QString loaderText;

   QPropertyAnimation *animation = nullptr;
   SmoothOpacity *op = nullptr;
};

#endif 

