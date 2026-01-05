#include "ComboBox.h"

class EventFilter : public QObject {
public:
    EventFilter(QWidget *textField, QWidget *popup, ComboBox *box, QObject *parent = nullptr)
        : QObject(parent), textField(textField), popup(popup), box(box) {}

   bool eventFilter(QObject *obj, QEvent *event) override {
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        QPoint globalPos = mouseEvent->globalPosition().toPoint();
        QWidget *clickedWidget = QApplication::widgetAt(globalPos);

        bool clickedInsidePopup = popup->isAncestorOf(clickedWidget);
        bool clickedOnTextField = textField->isAncestorOf(clickedWidget);

        if (popup->isVisible() && !clickedInsidePopup && !clickedOnTextField) 
            box->fadeOutAnimation();
    }

    if (event->type() == QEvent::ApplicationDeactivate) 
        if (popup->isVisible()) 
            box->fadeOutAnimation();
        
    return QObject::eventFilter(obj, event);
}

private:
    QWidget *popup = nullptr;
    QWidget *textField = nullptr;
    ComboBox *box = nullptr;
};

ComboBox::ComboBox(QWidget *parent) : TextField(parent) {
    setFixedSize(QSize(250, 36));
    setShadow(true);
    setDropDownPadding(true);
    setDropDownButton();
    setMaxVisibleItems(8);
    setupPopup();
    setTextFieldIconSize(QSize(20, 20));
    setEditable(false);
}

void ComboBox::setFixedSize(const QSize &s) { 
    TextField::setFixedSize(s); 
    update(); 
}

void ComboBox::setIconic(bool value) {
    isIconic = value;
    d->setIconic(isIconic);
    this->setTextFieldIcon(value);
    updateModel();
}

void ComboBox::setEditable(bool value) {
    isEditable = value;
    this->setReadOnly(!isEditable);
    this->setCursor(isEditable ? Qt::IBeamCursor : Qt::ArrowCursor);
}

void ComboBox::setDarkMode(bool value) {
    isDarkMode = value;
    TextField::setDarkMode(value);
    if (dropdown) dropdown->setDarkMode(value);
    if (popup) popup->setDarkMode(value);
    if (d) d->setDarkMode(value);
    if (vScroll && hScroll) vScroll->setDarkMode(value); hScroll->setDarkMode(value);
}

void ComboBox::positionDropDownButton() {
    if (dropdown) {
        int x = width() - (12 + dropdown->width()) + 3;
        int y = (height() - dropdown->height()) / 2;
        dropdown->move(x, y);
    }
}

void ComboBox::setPlaceholderText(const QString &text) { TextField::setPlaceholderText(text); }
void ComboBox::setDropDownButton() {
    dropdown = new Button(this);
    dropdown->setShadow(false);
    dropdown->setDisplayMode(Button::IconOnly);
    dropdown->setIconSize(QSize(20, 20));
    dropdown->setFixedSize(QSize(28, 28));
    dropdown->setIconPaths(arrowDown, arrowDown);
    positionDropDownButton();
    dropdown->raise();
}

void ComboBox::addItems(QStringList items) {
    _items = items;
    updateModel();
    updatePopupListHeight();
}

void ComboBox::addIcons(QStringList Light, QStringList dark) {
    if (isIconic) {
        itemsIconsLight = Light;
        itemsIconsDark = dark;
    }
    updateModel();
    updatePopupListHeight();
}

void ComboBox::fadeInAnimation() {
    animation->stop();
    disconnect(animation, &QPropertyAnimation::finished, nullptr, nullptr);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->start();
    popup->show();
}

void ComboBox::fadeOutAnimation() {
    animation->stop();
    disconnect(animation, &QPropertyAnimation::finished, nullptr, nullptr);
    animation->setStartValue(1.0);
    animation->setEndValue(0.0);
    connect(animation, &QPropertyAnimation::finished, this, [this]() { popup->hide(); });
    animation->start();
}

void ComboBox::setupPopup() {
    popup = new RoundedBox();
    list = new QListView();
    d = new Delegate(this->size());

    vScroll = new ScrollBar(Qt::Vertical);
    hScroll = new ScrollBar(Qt::Horizontal);

    list->setModel(&model);
    list->setItemDelegate(d);
    list->setAutoScroll(true);
    list->setViewMode(QListView::ListMode);
    list->setSelectionMode(QAbstractItemView::SingleSelection);
    list->setResizeMode(QListView::Adjust);
    list->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    list->setUniformItemSizes(true);
    list->setFrameShape(QFrame::NoFrame);
    list->setEditTriggers(QAbstractItemView::NoEditTriggers);
    list->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    list->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    list->setVerticalScrollBar(vScroll);
    list->setHorizontalScrollBar(hScroll);
    list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    list->setSpacing(3);
    list->setStyleSheet("QListView::item { margin: 0px; padding: 0px; } QListView { background: transparent; margin-bottom: 8px; margin-top: 8px; margin-left: 8px; margin-right: 8px; padding: 0px; border:none; }");

    layout = new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->addSpacing(0);
    layout->addWidget(list, 0, Qt::AlignVCenter);
    popup->setLayout(layout);
    popup->hide();
    
    smooth_opacity = new SmoothOpacity;
    popup->setGraphicsEffect(smooth_opacity);

    animation = new QPropertyAnimation(smooth_opacity, "opacity");
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->setDuration(300);

    qApp->installEventFilter(new EventFilter(this, popup, this, this));

    if (dropdown) 
        connect(dropdown, &Button::clicked, this, [this]() {
            if (popup->isVisible()) 
               fadeOutAnimation();
            else 
               popupPositioning();
        });

    connect(list, &QListView::clicked, this, [this]() {
        this->setText(currentText());
        fadeOutAnimation();
    });
}

void ComboBox::popupPositioning() {
    QPoint globalPos = this->mapToGlobal(QPoint(0, 0));
    QScreen *screenAtCursor = QApplication::screenAt(globalPos);
    if (!screenAtCursor) 
        screenAtCursor = QApplication::primaryScreen();
    
    QRect screenGeometry = screenAtCursor->availableGeometry();
    QRect comboBoxRect = QRect(globalPos, this->size());
    QSize popupSize = popup->size();

    QPoint abovePos(comboBoxRect.left(), comboBoxRect.top() - popupSize.height() - 4);
    QPoint belowPos(comboBoxRect.left(), comboBoxRect.bottom() + 4);
    QPoint centerPos(comboBoxRect.left(), comboBoxRect.center().y() - popupSize.height() / 2);

    QPoint finalPopupPos;
    if (screenGeometry.contains(QRect(centerPos, popupSize))) 
        finalPopupPos = centerPos;
    else if (screenGeometry.contains(QRect(belowPos, popupSize))) 
        finalPopupPos = belowPos;
    else if (screenGeometry.contains(QRect(abovePos, popupSize))) 
        finalPopupPos = abovePos;
    else
        finalPopupPos = belowPos;

    int finalX = std::clamp(finalPopupPos.x(), screenGeometry.left(), screenGeometry.right() - popupSize.width());
    int finalY = std::clamp(finalPopupPos.y(), screenGeometry.top(), screenGeometry.bottom() - popupSize.height());

    popup->move(QPoint(finalX, finalY));
    popup->raise();

    fadeInAnimation();
}


void ComboBox::setMaxVisibleItems(int items) { maxVisibleItems = items; }
int ComboBox::getMaxVisibleItems() { return maxVisibleItems; }

void ComboBox::updatePopupListHeight() {
    int maxVisibleItems = getMaxVisibleItems(); 
    int spacing = list->spacing();
    int itemsHeight = list->sizeHintForRow(0); 
    int totalItems = list->model()->rowCount();
    int visibleItems = std::min(totalItems, maxVisibleItems);
    if (totalItems <= 0)  return; 
    
    int height = (visibleItems * itemsHeight) + spacing * (visibleItems - 1);
    int finalHeight = 0;

    if (totalItems > visibleItems) {
        finalHeight = height;
        list->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    } else {
        finalHeight = height + (22 + (visibleItems - 1) * 3);
        list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    list->setFixedHeight(finalHeight);
    popup->setFixedSize(this->width(), finalHeight);   
}

void ComboBox::deleteItem(int index) {
    if (index >= 0 && index < model.rowCount()) {
        model.removeRow(index);
        _items.removeAt(index);
        if (index < itemsIconsLight.size())
            itemsIconsLight.removeAt(index);
            itemsIconsDark.removeAt(index);
        updatePopupListHeight();
    }
}

QString ComboBox::currentText() {
    QModelIndex index = list->currentIndex();
    if (index.isValid()) {
        if (isIconic) 
            setIconPaths(itemsIconsLight[index.row()], itemsIconsDark[index.row()]);
        return index.data(Qt::DisplayRole).toString();
    }
    return "";
}

int ComboBox::currentIndex() { return list->currentIndex().row(); }
void ComboBox::setCurrentItem(int index) {
    QModelIndex i = list->model()->index(index, 0);
    list->setCurrentIndex(i);
    setText(i.data(Qt::DisplayRole).toString());

    if (isIconic)
    setIconPaths((isDarkMode ? (itemsIconsDark[index]) : (itemsIconsLight[index])) , 
                (isDarkMode ? (itemsIconsDark[index]) : (itemsIconsLight[index])));

    update();
}

void ComboBox::updateModel() {
    model.clear();
    for (int i = 0; i < _items.size(); ++i) {
        QStandardItem *item;
        if (isIconic && i < itemsIconsLight.size() && !itemsIconsLight[i].isEmpty()) 
            item = new QStandardItem(QIcon(isDarkMode ? (itemsIconsDark[i]) : (itemsIconsLight[i])), _items[i]);
        else 
            item = new QStandardItem(_items[i]);
        
        model.appendRow(item);
    }

    if (list) list->setModel(&model);

    completer = new QCompleter(&model, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::InlineCompletion);
    completer->setFilterMode(Qt::MatchContains);
    this->setCompleter(completer);
}

void ComboBox::clearAll() {
    _items.clear();
    itemsIconsLight.clear();
    itemsIconsDark.clear();
    model.clear();
    if (list) list->reset();
}

void ComboBox::mousePressEvent(QMouseEvent *event) {
    if (!isEditable) {
        if (popup->isVisible()) 
            fadeOutAnimation();
        else 
            QTimer::singleShot(300, this, [this]() { popupPositioning(); });
    } else {
        QLineEdit::mousePressEvent(event);
    }
}

void ComboBox::mouseMoveEvent(QMouseEvent *event) {
    if (!isEditable) 
        event->accept();
    else 
        QLineEdit::mouseMoveEvent(event);
}

void ComboBox::keyPressEvent(QKeyEvent *event) {
    if (!isEditable) {
        event->accept();
    } else {
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
            QString text = this->text();
            int index = _items.indexOf(text);
            if (index != -1) 
                setCurrentItem(index);
            else 
                return;
        } else {
            QLineEdit::keyPressEvent(event);
        }
    }
}

void ComboBox::contextMenuEvent(QContextMenuEvent *event) {
    if (!isEditable) 
        event->accept();
    else 
        TextField::contextMenuEvent(event);
}

void ComboBox::mouseDoubleClickEvent(QMouseEvent *event) {
    if (!isEditable) 
        event->accept();
    else 
        QLineEdit::mouseDoubleClickEvent(event);
}

void ComboBox::resizeEvent(QResizeEvent *event) {
    TextField::resizeEvent(event);
    positionDropDownButton();
}