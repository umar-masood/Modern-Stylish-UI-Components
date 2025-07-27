#ifndef SMOOTHOPACITY_H
#define SMOOTHOPACITY_H

#include <QtWidgets>

class SmoothOpacity : public QGraphicsOpacityEffect {
   Q_OBJECT
   Q_PROPERTY(qreal opacity READ getOpacity WRITE setOpacity)
   
   public:
   explicit SmoothOpacity(QWidget *parent = nullptr);
   void setOpacity(qreal op);
   qreal getOpacity();
};

#endif