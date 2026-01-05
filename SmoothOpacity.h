#pragma once

#include <QGraphicsOpacityEffect>
#include <QWidget>

class SmoothOpacity : public QGraphicsOpacityEffect {
   Q_OBJECT
   Q_PROPERTY(qreal opacity READ getOpacity WRITE setOpacity)
   
   public:
   explicit SmoothOpacity(QWidget *parent = nullptr);
   void setOpacity(qreal op);
   qreal getOpacity();
};
