#pragma once
#include <QListView>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QAbstractItemModel>
#include <QAbstractItemDelegate>
#include "RoundedBox.h"
#include "ScrollBar.h"
#include "SmoothOpacity.h"

class Popup : public RoundedBox {
    Q_OBJECT
    
    public:
    explicit Popup(QWidget *parent = nullptr);

    void setPopupWidth(int width = 0);
    void updatePopup();
    void setSelectionMode(QAbstractItemView::SelectionMode mode = QAbstractItemView::SingleSelection);
    void setModel(QAbstractItemModel *model);
    void setItemDelegate(QAbstractItemDelegate *delegate);
    void fadeIn();
    void fadeOut();
    void setMaxVisibleItems(int items);
    void setDarkMode(bool value);

    QListView* list() const;

    private:    
    int maxVisibleItems = 8;
    int explicitWidth = 0;

    ScrollBar *vScroll = nullptr;

    QListView *_list = nullptr;

    SmoothOpacity *smooth_opacity = nullptr;
    QPropertyAnimation *animation = nullptr;
    
    QVBoxLayout *layout = nullptr;
};
