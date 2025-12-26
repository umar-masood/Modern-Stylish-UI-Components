#include "SmoothOpacity.h"

SmoothOpacity::SmoothOpacity(QWidget *parent) : QGraphicsOpacityEffect(parent) { }

void SmoothOpacity::setOpacity(qreal op) { QGraphicsOpacityEffect::setOpacity(op); }

qreal SmoothOpacity::getOpacity() {  return QGraphicsOpacityEffect::opacity(); }
