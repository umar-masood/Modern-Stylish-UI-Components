#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QtWidgets>
#include "TextField.h"
#include "RoundedBox.h"
#include "Delegate.h"
#include "ScrollBar.h"


class ComboBox : public TextField {
    Q_OBJECT

public:
    ComboBox(QWidget *parent = nullptr);

    void setIconic(bool value);
    void setEditable(bool value);
    void setDarkMode(bool value);
    void setDropDownButton();
    void addItems(QStringList items);
    void addIcons(QStringList Light = {}, QStringList dark = {});
    void Popup();
    void updatePopupListHeight();
    void deleteItem(int index);
    QString currentText();
    int currentIndex();
    void setCurrentItem(int index);
    void updateModel();
    void clearAll();
    void popupPos();
    void setSize(QSize s);
    void positionDropDownButton();
    void setPlaceholderText(const QString &text);
    
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
    bool isEditable = false;
    bool isIconic = false;
    bool isDarkMode = false;

    int maxVisibleItems = 0;

    Button *dropdown = nullptr;
    RoundedBox *popup = nullptr;
    QListView *list = nullptr;
    Delegate *d = nullptr;
    QStandardItemModel model;
    QVBoxLayout *layout = nullptr;

    QStringList _items;
    QStringList _itemsIconsLight;
    QStringList _itemsIconsDark;

    QCompleter *completer = nullptr;

    QPropertyAnimation *animation = nullptr;

    ScrollBar *vScroll = nullptr;
    ScrollBar *hScroll = nullptr;
};
#endif
