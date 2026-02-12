#include "Popup.h"

Popup::Popup(QWidget *parent) : RoundedBox(parent) {
    // Items List
    _list = new QListView(this);

    // Scrollbar
    vScroll = new ScrollBar(Qt::Vertical);

    // List Properties
    _list->setAutoScroll(true);
    _list->setViewMode(QListView::ListMode);
    _list->setResizeMode(QListView::Adjust);
    _list->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    _list->setUniformItemSizes(true);
    _list->setFrameShape(QFrame::NoFrame);
    _list->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _list->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    _list->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    _list->setVerticalScrollBar(vScroll);
    _list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _list->setStyleSheet(
        "QListView::item { margin: 0px; padding: 0px; } "
        "QListView { background: transparent; margin: 4px; padding: 0px; border:none; }"
    );

    // Layout of Popup
    layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->addSpacing(0);
    layout->addWidget(_list, 0, Qt::AlignVCenter);

    // Opacity Effect
    smooth_opacity = new SmoothOpacity(this);
    setGraphicsEffect(smooth_opacity);

    // Opacity Effect Animation
    animation = new QPropertyAnimation(smooth_opacity, "opacity", this);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->setDuration(300);

    hide();
}

void Popup::setPopupWidth(int width) {
   explicitWidth = width;
   updatePopup();
}

void Popup::updatePopup() {
    if (!_list || !_list->model()) return;

    int itemsHeight = _list->sizeHintForRow(0); 
    int totalItems = _list->model()->rowCount();
    int visibleItems = std::min(totalItems, maxVisibleItems);

    if (totalItems <= 0) return;

    int height = (visibleItems * itemsHeight) + (_list->spacing()) * (visibleItems - 1) + 8;

    _list->setVerticalScrollBarPolicy(totalItems > visibleItems ? Qt::ScrollBarAsNeeded : Qt::ScrollBarAlwaysOff);
    _list->setFixedHeight(height);
    
    int widthToUse = (explicitWidth > 0) ? explicitWidth 
                                        : (parentWidget() ? parentWidget()->width() : 150);

    setFixedSize(widthToUse, height);
}

void Popup::setSelectionMode(QAbstractItemView::SelectionMode mode) {
    if (_list) _list->setSelectionMode(mode);
}

void Popup::setModel(QAbstractItemModel *model) {
    if (_list) _list->setModel(model);
}

void Popup::setItemDelegate(QAbstractItemDelegate *delegate) {
    if (_list) _list->setItemDelegate(delegate);
}

void Popup::fadeIn() {
    if (!animation) return;

    animation->stop();
    disconnect(animation, &QPropertyAnimation::finished, nullptr, nullptr);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    show();
    animation->start();
}

void Popup::fadeOut() {
    if (!animation) return;

    animation->stop();
    disconnect(animation, &QPropertyAnimation::finished, nullptr, nullptr);
    animation->setStartValue(1.0);
    animation->setEndValue(0.0);
    connect(animation, &QPropertyAnimation::finished, this, [this]() { hide(); });
    animation->start();
}

void Popup::setMaxVisibleItems(int items) {
    maxVisibleItems = items;
}

void Popup::setDarkMode(bool value) {
    if (vScroll) 
        vScroll->setDarkMode(value);

    RoundedBox::setDarkMode(value);
}

QListView* Popup::list() const {
    return _list;
}
