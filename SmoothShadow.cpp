#include "SmoothShadow.h"

SmoothDropShadow::SmoothDropShadow(QWidget *parent) : QGraphicsDropShadowEffect(parent), m_radius(0) {}

void SmoothDropShadow::set_blur_radius(qreal br) {
  m_radius = br;
  QGraphicsDropShadowEffect::setBlurRadius(br);
}

qreal SmoothDropShadow::get_blur_radius() const { return QGraphicsDropShadowEffect::blurRadius(); }
