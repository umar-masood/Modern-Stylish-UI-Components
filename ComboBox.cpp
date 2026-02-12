#include "ComboBox.h"

ComboBox::ComboBox(QWidget *parent) : TextField(parent) {
    setFixedSize(QSize(250, 36));
    setDropDownButton();
    setMaxVisibleItems(8);
    init();
}

void ComboBox::init() {
    // Item Delegate
    delegate = new Delegate(this->size());
    
    // Popup
    popup = new Popup(this);
    popup->setModel(&model);
    popup->setItemDelegate(delegate);
    popup->setSelectionMode(QAbstractItemView::SingleSelection);
    popup->setPopupWidth(this->width());
    
    // Auto Completer
    completer = new QCompleter(&model, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::InlineCompletion);
    completer->setFilterMode(Qt::MatchContains);
    setCompleter(completer);

    // Install Event Filter
    qApp->installEventFilter(this);

    // Dropdown Button Signal Slot
    if (dropdown)  connect(dropdown, &Button::clicked, this, &ComboBox::onDropDownButtonClicked);

    // List item Signal Slot
    connect(popup->list(), &QListView::clicked, this, &ComboBox::onComboItemClicked);
}

bool ComboBox::eventFilter(QObject *obj, QEvent *event) {
    if (!popup || !isVisible()) 
        return false;

    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        QPoint globalPos = mouseEvent->globalPosition().toPoint();
        QWidget *clickedWidget = QApplication::widgetAt(globalPos);

        bool clickedInsidePopup = popup->isAncestorOf(clickedWidget);
        bool clickedOnTextField = this->isAncestorOf(clickedWidget);

        if (popup->isVisible() && !clickedInsidePopup && !clickedOnTextField) 
            popup->fadeOut();
    }

    if (event->type() == QEvent::ApplicationDeactivate) 
        if (popup->isVisible()) popup->fadeOut();

    return QObject::eventFilter(obj, event);
}

void ComboBox::setFieldSize(const QSize &fieldSize) {
    TextField::setFixedSize(fieldSize); 
    popup->setPopupWidth(this->width());
    update(); 
}

void ComboBox::setPopupWidth(int width) { popup->setPopupWidth(width); }

void ComboBox::setIconic(bool value) {
    isIconic = value;
    delegate->setIconic(isIconic);
    setTextFieldIcon(isIconic);
}

void ComboBox::setEditable(bool value) {
    isEditable = value;
    setReadOnly(!isEditable);
    setCursor(isEditable ? Qt::IBeamCursor : Qt::ArrowCursor);
}

void ComboBox::setDarkMode(bool value) {
    isDarkMode = value;
    TextField::setDarkMode(value);

    dropdown->setDarkMode(value);
    popup->setDarkMode(value);
    delegate->setDarkMode(value);

    updateItemIcons();
}

void ComboBox::setDropDownButton() {
    dropdown = new Button(this);
    dropdown->setDisplayMode(Button::IconOnly);
    dropdown->setNormalBackgroundTransparent(true);
    dropdown->setBorderTransparent(true);
    dropdown->setCursor(Qt::PointingHandCursor);
    dropdown->setIconSize(QSize(20, 20));
    dropdown->setFixedSize(QSize(28, 28));
    dropdown->setIconPaths(ArrowDown, ArrowDown);
    dropdown->raise();    
    positionDropDownButton();
}

void ComboBox::positionDropDownButton() {
    if (dropdown) {
        int x = width() - (12 + dropdown->width()) + 3;
        int y = (height() - dropdown->height()) / 2;
        dropdown->move(x, y);
    }
}

void ComboBox::addItem(const QString &text, const QString &lightIcon, const QString &darkIcon) {
    items.append({text, lightIcon, darkIcon});
    
    QStandardItem *item;
    if (!lightIcon.isEmpty() && !darkIcon.isEmpty()) 
        item = new QStandardItem(QIcon(isDarkMode ? darkIcon : lightIcon), text);
    else 
        item = new QStandardItem(text);
    
    model.appendRow(item);
    popup->updatePopup();
}

void ComboBox::addItems(const QVector<ComboItem> comboItems) {
    for (const ComboItem &ci : comboItems) 
        addItem(ci.text, ci.lightIcon, ci.darkIcon);
}

void ComboBox::onComboItemClicked(const QModelIndex &index) {
    setText(currentText());

    if (isIconic && index.row() < items.size()) {
        const ComboItem &ci = items[index.row()];
        setIconPaths(ci.lightIcon, ci.darkIcon);
    }

    repaint();
    popup->fadeOut();
}

void ComboBox::onDropDownButtonClicked() {
    popup->isVisible() ? popup->fadeOut() : positionPopup(); 
}

void ComboBox::positionPopup() {
    QPoint globalPos = this->mapToGlobal(QPoint(0, 0));
    QScreen *screenAtCursor = QApplication::screenAt(globalPos);
    if (!screenAtCursor) 
        screenAtCursor = QApplication::primaryScreen();
    
    QRect screenGeometry = screenAtCursor->availableGeometry();
    QRect cbRect = QRect(globalPos, this->size());
    QSize popupSize = popup->size();

    QPoint abovePos(cbRect.left(), cbRect.top() - popupSize.height() - 4);
    QPoint belowPos(cbRect.left(), cbRect.bottom() + 4);
    QPoint centerPos(cbRect.left(), cbRect.center().y() - popupSize.height() / 2);

    QPoint currPosition;
    if (screenGeometry.contains(QRect(centerPos, popupSize))) 
        currPosition = centerPos;
    else if (screenGeometry.contains(QRect(belowPos, popupSize))) 
        currPosition = belowPos;
    else if (screenGeometry.contains(QRect(abovePos, popupSize))) 
        currPosition = abovePos;
    else
        currPosition = belowPos;

    int x = std::clamp(currPosition.x(), screenGeometry.left(), screenGeometry.right() - popupSize.width());
    int y = std::clamp(currPosition.y(), screenGeometry.top(), screenGeometry.bottom() - popupSize.height());

    popup->move(QPoint(x, y));
    popup->raise();
    popup->fadeIn();
}

void ComboBox::setMaxVisibleItems(int items) { _maxVisibleItems = items; }
int ComboBox::maxVisibleItems() const { return _maxVisibleItems; }

void ComboBox::updateItemIcons() {
    for (int i = 0; i < items.size(); ++i) {
        const ComboItem &ci = items[i];

        if (ci.lightIcon.isEmpty() || ci.darkIcon.isEmpty())
            continue;

        QStandardItem *item = model.item(i);
        if (!item)
            continue;

        item->setIcon(QIcon(isDarkMode ? ci.darkIcon : ci.lightIcon));
    }

    popup->list()->viewport()->update();
}

void ComboBox::deleteItem(int index) {
    if (index < 0 || index >= items.size()) 
        return;
    items.removeAt(index);
    model.removeRow(index);

    popup->updatePopup();
}

QString ComboBox::currentText() const {
    QModelIndex index = popup->list()->currentIndex();
    if (index.isValid() && index.row() < items.size())
        return index.data(Qt::DisplayRole).toString();

    return QString();
}

int ComboBox::currentIndex() const { return popup->list()->currentIndex().row(); }

void ComboBox::setCurrentItem(int index) {
    if (index < 0 || index >= items.size()) return;

    QModelIndex idx = popup->list()->model()->index(index, 0);
    popup->list()->setCurrentIndex(idx);
    setText(items[index].text);

    if (isIconic)
        setIconPaths(items[index].lightIcon, items[index].darkIcon);

    update();
}

void ComboBox::clearAll() {
    items.clear();
    model.clear();
    if (popup->list()) 
        popup->list()->reset();
}

void ComboBox::mousePressEvent(QMouseEvent *event) {
    if (!isEditable)
        popup->isVisible() ? popup->fadeOut() : (QTimer::singleShot(300, this, [this]() { positionPopup(); }));
    else 
        QLineEdit::mousePressEvent(event);
}

void ComboBox::mouseMoveEvent(QMouseEvent *event) {
    !isEditable ? event->accept() : QLineEdit::mouseMoveEvent(event);
}

void ComboBox::keyPressEvent(QKeyEvent *event) {
    if (!isEditable) {
        event->accept();
    } else {
        switch (event->key()) {
            case Qt::Key_Up:
            case Qt::Key_Down: {
                QApplication::sendEvent(popup->list(), event);
                break;
            }
        
            case Qt::Key_Return:
            case Qt::Key_Enter: {
                int index = currentIndex();
                if (index != -1) {
                    setCurrentItem(currentIndex());
                    popup->fadeOut();
                }
               
                event->accept();
                break;
            }
        
            case Qt::Key_Escape: {
                if (popup->isVisible())
                    popup->fadeOut();
                event->accept();
                break;
            }
            
            default:  break;
        }
    }
}

void ComboBox::contextMenuEvent(QContextMenuEvent *event) {
    !isEditable ? event->accept() : TextField::contextMenuEvent(event);
}

void ComboBox::mouseDoubleClickEvent(QMouseEvent *event) {
   !isEditable ? event->accept() : QLineEdit::mouseDoubleClickEvent(event);
}

void ComboBox::resizeEvent(QResizeEvent *event) {
    TextField::resizeEvent(event);
    positionDropDownButton();
}