#pragma once

#include "Button.h"
#include "TextField.h"
#include "SmoothShadow.h"
#include "SmoothOpacity.h"
#include "RoundedBox.h"
#include "ScrollBar.h"
#include "Delegate.h"

#include <QObject>
#include <QEvent>
#include <QApplication>
#include <QPoint>
#include <QScreen>
#include <QKeyEvent>
#include <QListView>
#include <QAbstractItemView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QCompleter>
#include <QVBoxLayout>
#include <QFrame>
#include <QStringList>

class ComboBox : public TextField {
    Q_OBJECT

    public:
    explicit ComboBox(QWidget *parent = nullptr);

    void setIconic(bool value);
    void setEditable(bool value);
    void setDarkMode(bool value);
    void setCurrentItem(int index);
    void setFixedSize(const QSize &s);
    void setPlaceholderText(const QString &text);

    void addItems(QStringList items);
    void addIcons(QStringList Light = {}, QStringList dark = {});

    void deleteItem(int index);
    void clearAll();

    QString currentText();
    int currentIndex();

    void setMaxVisibleItems(int items);
    int getMaxVisibleItems();

    void fadeInAnimation();
    void fadeOutAnimation();

    protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    private:
    void positionDropDownButton();
    void setupPopup();
    void popupPositioning();
    void updatePopupListHeight();
    void setDropDownButton();  
    void updateModel();

    // Flags
    bool isEditable = false;
    bool isIconic = false;
    bool isDarkMode = false;

    // Max Visible Items
    int maxVisibleItems = 0;
  
    // Dropdown Button
    Button *dropdown = nullptr;
    const QString arrowDown = ":/icons/ComponentsIcons/arrow-down.svg";

    // Rounded Popup
    RoundedBox *popup = nullptr;

    // List & Delegate for data items inside Popup
    QListView *list = nullptr;
    Delegate *d = nullptr;
    QStandardItemModel model;
    QVBoxLayout *layout = nullptr;

    // List for items, icon paths
    QStringList _items;
    QStringList itemsIconsLight;
    QStringList itemsIconsDark;

    // Auto Completer
    QCompleter *completer = nullptr;

    // Animation & Effects
    SmoothOpacity *smooth_opacity = nullptr;
    QPropertyAnimation *animation = nullptr;

    // Scrollbars of Popup
    ScrollBar *vScroll = nullptr;
    ScrollBar *hScroll = nullptr;
};
