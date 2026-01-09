#pragma once
#include "RoundedBox.h"
#include "SmoothOpacity.h"

#include <QObject>
#include <QPropertyAnimation>
#include <QTimer>
#include <QEvent>
#include <QScreen>
#include <QApplication>
#include <algorithm>

class ToolTip : public QObject {
  Q_OBJECT
  
  public:
  explicit ToolTip(QWidget *target = nullptr, const QString &text = "", bool isDarkMode = false, QObject *parent = nullptr);
  void setText(const QString &text);
  void setDarkMode(bool isDarkMode);
  void setTargetWidget(QWidget *target);
  void hide();

  private:
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

  signals:
  void textEntered(const QString &text);
  void themeModeChanged(bool enable);

  private slots:
  void onTextEntered(const QString &text);
  void onThemeModeChanged(bool enable);
  void onTimeout();
};
