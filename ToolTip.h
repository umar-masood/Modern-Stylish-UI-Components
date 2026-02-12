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
  explicit ToolTip(QWidget *target = nullptr, const QString &text = "", QObject *parent = nullptr);
  void setText(const QString &text);
  void setDarkMode(bool enable);
  void setTargetWidget(QWidget *target);
  void hide();

  protected:
  bool eventFilter(QObject *obj, QEvent *event) override;

  private:
  void position(QWidget *target);
  void showToolTip();
  void fadeInAnimation();
  void fadeOutAnimation();

  bool isDarkMode = false;
  bool isHovering = false;

  QTimer timer;
  QWidget *_target = nullptr;
  RoundedBox *tooltipWidget = nullptr;
  QPropertyAnimation *animation = nullptr;

  signals:
  void textEntered(const QString &text);
  void themeModeChanged(bool enable);

  private slots:
  void onTextEntered(const QString &text);
  void onThemeModeChanged(bool enable);
  void onTimeout();
};

