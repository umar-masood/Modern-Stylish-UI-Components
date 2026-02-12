#pragma once
#include "SmoothShadow.h"

#include <QPushButton>
#include <QPainter>
#include <QPaintEvent>
#include <QEnterEvent>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QColor>
#include <QRect>
#include <algorithm>

class Toggle : public QPushButton {
  Q_OBJECT

  Q_PROPERTY(qreal scale READ getScale WRITE setScale)
  Q_PROPERTY(qreal offset READ getOffset WRITE setOffset)
  Q_PROPERTY(int KW READ getKW WRITE setKW)

  public:
  explicit Toggle(QWidget *parent = nullptr);

  void setShadow(bool value);
  void setDarkMode(bool value);

  protected:
  void paintEvent(QPaintEvent *event) override;
  void enterEvent(QEnterEvent *event) override;
  void leaveEvent(QEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;

  private:
  void init();

  qreal getScale();
  void setScale(qreal value);

  qreal getOffset();
  void setOffset(qreal value);

  int getKW();
  void setKW(int value);

  bool isHover = false;
  bool isDarkMode = false;
  bool hasShadow = true;
  bool isToggled = false;

  qreal scale;
  QPropertyAnimation *animation1;

  qreal offset;
  QPropertyAnimation *animation2;

  int KW;
  QPropertyAnimation *animation3;

  SmoothShadow *effect;
  QPropertyAnimation *animation4;
  
  bool isDragging = false;
  qreal dragOffset = 0.0;
  int dragStartX = 0;
};
