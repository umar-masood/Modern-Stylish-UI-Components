#pragma once

#include "ScrollBar.h"
#include "RoundedBox.h"
#include "Delegate.h"
#include "SmoothOpacity.h"

#include <QStandardItem>
#include <QListView>
#include <QStandardItemModel>
#include <QMouseEvent>
#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include <QVBoxLayout>
#include <QAbstractItemView>
#include <QAbstractScrollArea>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QFrame>
#include <QTimer>
#include <QCursor>
#include <QMap>
#include <QEvent>
#include <algorithm>

class MenuItem : public QStandardItem {
public:
   explicit MenuItem(const QIcon &icon, const QString &text, const QString &shortcut, bool hasSubMenu);
};

class Menu : public QListView {
   Q_OBJECT
public:
   struct Actions {
     const QString text;
     bool hasSubMenu = false;
     const QString iconPathLight = QString();
     const QString iconPathDark = QString();
     const QString shortcut = QString();
   };

   explicit Menu(QWidget *parent = nullptr);

   void fadeInAnimation();
   void fadeOutAnimation();

   /**
    * @param Text QString
    * @param hasSubmenu bool
    * @param LightModeIconPath QString
    * @param DarkModeIconPath QString
    * @param Shortcut QString
    * @brief Make sure to only use shortcut or hasSubMenu, using both will cause misbehvaior
    */
   void addAction(const Actions &menuAction);
   void removeAction(int index);
   void clearAll();
   /**
    * @attention Must pass the parent Menu to setParentMenu() if you`re using submenus
    */
   void addSubMenu(int index, Menu *submenu);
   void move(const QPoint &point);
   /**
    * @attention Must call show() before using move() in order to setup the Menu properly
    */
   void move(int &x, int &y);
   void hide();
   /**
    * @attention Call move() after show()
    */
   void show();

   /**
    * @attention No Need to call move() or show() when using showAt()
    */
   void showAt(QWidget *anchorWidget);

   QPoint adjustSubMenuPosition(Menu *subMenu, const QPoint &intendedPos);

   QString clickedItemText() const;
   QString clickedItemShortcut() const;
   int clickedItemIndex() const;

   void setParentMenu(Menu *parentMenu);
   void setItemSize(QSize size);
   QSize getItemSize();
   void setIconic(bool value);
   void setDarkMode(bool value);
   void setMaxVisibleItems(int items);
   int getMaxVisibleItems();

signals:
   void itemClicked();

private slots:
   void onItemClicked(const QModelIndex &index);

protected:
   void mouseMoveEvent(QMouseEvent *event) override;
   void leaveEvent(QEvent *event) override;
   bool eventFilter(QObject *o, QEvent *event) override;
   
   private:
   void setup();   
   void updateMenuHeight();
   QPoint adjustXY(const QSize &s, const QPoint &p, QRect &screenGeo, bool isSubMenu);
   void setHoveredIndex(const QModelIndex &index);

   bool isIconic = false;
   bool isDarkMode = false;
   int maxVisibleItems;
   
   QString _clickedItemShortcut;
   QString _clickedItemText;
   int _clickedItemIndex = -1;
   
   QMap<int, Menu*> subMenus;
   
   RoundedBox *popup = nullptr;
   Delegate *delegate = nullptr;

   QStandardItemModel model;
   QVBoxLayout *layout = nullptr;
   
   ScrollBar *vScroll = nullptr;
   ScrollBar *hScroll = nullptr;
   
   QPropertyAnimation *animation = nullptr;
   Menu *parentMenu = nullptr;
   
   QSize itemSize;
   QModelIndex hoveredIndex;

   QTimer *subMenuTimer = nullptr;
};

