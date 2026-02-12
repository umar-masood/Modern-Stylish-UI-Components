#include "SmoothShadow.h"

SmoothShadow::SmoothShadow(QWidget *parent) : QGraphicsDropShadowEffect(parent), m_radius(0) {}

void SmoothShadow::set_blur_radius(qreal br) {
  m_radius = br;
  QGraphicsDropShadowEffect::setBlurRadius(br);
}

qreal SmoothShadow::get_blur_radius() const {
  return QGraphicsDropShadowEffect::blurRadius();
}