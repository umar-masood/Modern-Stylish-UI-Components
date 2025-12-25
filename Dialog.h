#pragma once

#include "Button.h"
#include "../windows/subWindow/SubWindow.h"

#include <QScreen>
#include <QCursor>
#include <QResizeEvent>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QEvent>
#include <QApplication>

class Overlay : public QWidget {
   Q_OBJECT
   public:
   explicit Overlay(QWidget *parent = nullptr);

   protected:
   void paintEvent(QPaintEvent *event) override;
};

class Dialog : public SubWindow 
{
   Q_OBJECT

public:
   Dialog(QWidget *centralWidget = nullptr, QWidget *parent = nullptr, bool closeBtn = true);
   Q_INVOKABLE void setDarkMode(bool value);

protected:
   void showEvent(QShowEvent *event) override;
   void resizeEvent(QResizeEvent *event) override;
   void closeEvent(QCloseEvent *event) override;
   bool eventFilter(QObject* obj, QEvent* event) override;
   
private:
   void centerInParent();
   void setup();

   QWidget *contentWidget = nullptr;
   Overlay *overlay = nullptr;

   bool isDarkMode = false;
   bool setupDone = false;
};

