#pragma once

#include "RoundedBox.h"
#include "Button.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QSize>
#include <QApplication>
#include <QShowEvent>

class Dialog : public RoundedBox
{
   Q_OBJECT

public:
   Dialog(QWidget *parent = nullptr);

   void setText(const QString &text);
   void setButtonText(const QString &text);
   void setIcon(const QString &iconLight, const QString &iconDark);
   void setIconSize(QSize s);
   void setSize(QSize s);
   void setDarkMode(bool value);
   void setUninteractiveWidgets(QWidget *mainContent, QWidget *titleBar);

signals:
   void onDialogButtonClicked();

protected:
   void showEvent(QShowEvent *event) override;
   void resizeEvent(QResizeEvent *event) override;
   void closeEvent(QCloseEvent *event) override;
   
private:
   void setup();
   void centerInParent();
   void disableWidgets();
   void enableWidgets();
   void applyStyleSheet();

   QWidget *contentWidget = nullptr;
   QWidget *titleBarWidget = nullptr;

   QPixmap pixmap(const QString &iconLight, const QString &iconDark, QSize s);
   bool isDarkMode = false;
   bool setupDone = false;
   QString iconLightPath, iconDarkPath, dialogText, btnText;
   QLabel *dialogIcon = nullptr;
   QVBoxLayout *layout = nullptr;
   QSize iconSize = QSize(200, 200);
   QLabel *label = nullptr;
};

