#ifndef TOOLTIP_H
#define TOOLTIP_H

#include <QtWidgets>
#include "RoundedBox.h"
#include "SmoothOpacity.h"

class ToolTip : public QObject {
  Q_OBJECT
public:
  static void setToolTip(QWidget *widget, const QString &text, bool isDarkMode);

private:
  explicit ToolTip(QWidget *target, const QString &text, bool isDarkMode);

  bool eventFilter(QObject *obj, QEvent *event) override;

  void position(QWidget *target);
  void showToolTip();
  void fadeInAnimation();
  void fadeOutAnimation();

  QTimer timer;
  QWidget *target = nullptr;
  RoundedBox *tooltipWidget = nullptr;
  bool isHovering = false;
  QPropertyAnimation *animation = nullptr;
};

#endif 
