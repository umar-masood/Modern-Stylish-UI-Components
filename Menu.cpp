#include "menu.h"

MenuItem::MenuItem(const QIcon &icon, const QString &text, const QString &shortcut, bool hasSubMenu) : QStandardItem(icon, text) {
    setData(shortcut, Qt::UserRole + 1);
    setData(hasSubMenu, Qt::UserRole + 2);
}

Menu::Menu(QWidget *parent) : QListView(parent), maxVisibleItems(8) {
    setItemSize(QSize(150, 36));
    setMouseTracking(true);
}

void Menu::fadeInAnimation() {
    animation->stop();
    disconnect(animation, &QPropertyAnimation::finished, nullptr, nullptr);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->start();
    if (popup) popup->show();
}

void Menu::fadeOutAnimation() {
    animation->stop();
    disconnect(animation, &QPropertyAnimation::finished, nullptr, nullptr);
    animation->setStartValue(1.0);
    animation->setEndValue(0.0);
    connect(animation, &QPropertyAnimation::finished, this, [this]() {
        if (popup) popup->hide();
    });
    animation->start();
}

bool Menu::eventFilter(QObject *o, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        auto *mouseEvent = dynamic_cast<QMouseEvent *>(event);
        QPoint globalPos = mouseEvent->globalPosition().toPoint();
        QWidget *clickedWidget = QApplication::widgetAt(globalPos);

        if (popup->isVisible() && !popup->isAncestorOf(clickedWidget))
            fadeOutAnimation();
    }

    if (event->type() == QEvent::ApplicationDeactivate)
        if (popup->isVisible())  
            fadeOutAnimation();

    return QObject::eventFilter(o, event);
}

void Menu::setup() {
    popup = new RoundedBox();

    delegate = new Delegate(getItemSize());
    delegate->setAsMenu(true);
    delegate->setIconic(isIconic);

    vScroll = new ScrollBar(Qt::Vertical);
    hScroll = new ScrollBar(Qt::Horizontal);

    popup->setDarkMode(isDarkMode);
    delegate->setDarkMode(isDarkMode);
    vScroll->setDarkMode(isDarkMode);
    hScroll->setDarkMode(isDarkMode);

    setModel(&model);
    setItemDelegate(delegate);
    setAutoScroll(true);
    setViewMode(QListView::ListMode);
    setSelectionMode(QAbstractItemView::NoSelection);
    setResizeMode(QListView::Adjust);
    setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    setUniformItemSizes(true);
    setFrameShape(QFrame::NoFrame);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollBar(vScroll);
    setHorizontalScrollBar(hScroll);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setStyleSheet("QListView::item { margin: 0px; padding: 0px; } QListView { background: transparent; margin: 6px; padding: 0px; border:none; }");

    layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addSpacing(0);
    layout->addWidget(this, 0, Qt::AlignVCenter);
    popup->setLayout(layout);
    popup->hide();

    SmoothOpacity *smooth_opacity = new SmoothOpacity;
    popup->setGraphicsEffect(smooth_opacity);

    animation = new QPropertyAnimation(smooth_opacity, "opacity");
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->setDuration(300);

    fadeInAnimation();
    qApp->installEventFilter(this);

    disconnect(this, Menu::clicked, this, Menu::onItemClicked);
    connect(this, Menu::clicked, this, Menu::onItemClicked);
}

void Menu::addAction(const Actions &menuAction) {
    MenuItem *item = new MenuItem(QIcon(isDarkMode ? menuAction.iconPathDark : menuAction.iconPathLight), menuAction.text, menuAction.shortcut, menuAction.hasSubMenu);
    if (!menuAction.shortcut.isEmpty() && menuAction.hasSubMenu)
        qCritical() << "You are using both shortcut along with submenu causing misbehavior. Keep using only one.";
    model.appendRow(item);
}

void Menu::removeAction(int index) {
    if (index != -1) {
        model.removeRow(index);
        subMenus.remove(index);

        QMap<int, Menu*> updatedSubMenus;
        for (auto it = subMenus.begin(); it != subMenus.end(); ++it) {
            int key = it.key();
            Menu *menu = it.value();
            updatedSubMenus[key > index ? key - 1 : key] = menu;
        }
        subMenus = updatedSubMenus;
        updateMenuHeight();
        update();
    }
}

void Menu::clearAll() {
    model.clear();
    update();
}

void Menu::updateMenuHeight() {
    int spacing = this->spacing();
    int itemsHeight = this->sizeHintForRow(0);
    int totalItems = model.rowCount();
    int visibleItems = std::min(totalItems, maxVisibleItems);

    if (totalItems <= 0) return;

    int finalHeight = (visibleItems * itemsHeight) + spacing * (visibleItems - 1);
    
    setVerticalScrollBarPolicy(totalItems > visibleItems ? Qt::ScrollBarAsNeeded : Qt::ScrollBarAlwaysOff);
    setFixedHeight(finalHeight);
    
    popup->setFixedSize(getItemSize().width(), finalHeight);
}

void Menu::setHoveredIndex(const QModelIndex &index) {
    hoveredIndex = index;
    if (delegate) delegate->setHoveredIndex(index);
    update();
}

void Menu::addSubMenu(int index, Menu *submenu) {
    if (index != -1 && submenu)
        subMenus[index] = submenu;
    else
        qCritical() << "Invalid submenu index.";
}

void Menu::move(const QPoint &point) {
    int x = point.x(); int y = point.y();
    this->move(x,y);
}

void Menu::move(int &x, int &y) {
    if (!popup) return;

    QSize size = popup->size();
    QPoint desiredPos(x, y);

    QScreen *screen = QApplication::screenAt(desiredPos);
    if (!screen) screen = QGuiApplication::primaryScreen();
    
    QRect screenGeometry = screen->availableGeometry();

    popup->move(adjustXY(size, desiredPos, screenGeometry, false));
}

QPoint Menu::adjustXY(const QSize &s, const QPoint &p, QRect &screenGeo, bool isSubMenu) {

    int x = p.x(), y = p.y();

    // Adjust X
    if (x + s.width() > screenGeo.right())
        x = x - s.width() + (isSubMenu ? this->popup->width() : 0) + 3;

    if (x < screenGeo.left())
        x = screenGeo.left(); 

    // Adjust Y
    if (y + s.height() > screenGeo.bottom()) 
        y = screenGeo.bottom() - s.height();
 
    if (y < screenGeo.top())  
        y = screenGeo.top();

    return QPoint(x,y);
}

void Menu::hide() { fadeOutAnimation(); }
void Menu::show() {
    setup();
    updateMenuHeight();
}

void Menu::showAt(QWidget *anchorWidget) {
    show();
    
    if (!anchorWidget) return;
    QPoint globalPos = anchorWidget->mapToGlobal(QPoint(0,0));

    QScreen *screen = QApplication::screenAt(globalPos);
    if (!screen) screen = QApplication::primaryScreen();

    QRect screenGeometry = screen->availableGeometry();  
    QRect anchorWidgetGeometry(globalPos, anchorWidget->size());  
    QSize menuSize = this->popup->size();

    QPoint abovePos(anchorWidgetGeometry.left(), anchorWidgetGeometry.top() - menuSize.height() - 4);
    QPoint belowPos(anchorWidgetGeometry.left(), anchorWidgetGeometry.bottom() + 4);
    QPoint finalPos;

    if (screenGeometry.contains(QRect(belowPos, menuSize)))
       finalPos = belowPos;
    else if (screenGeometry.contains(QRect(abovePos, menuSize)))
       finalPos = abovePos;
    else 
       finalPos = belowPos;

    int x = std::clamp(finalPos.x(), screenGeometry.left(), screenGeometry.right() - menuSize.width());
    int y = std::clamp(finalPos.y(), screenGeometry.top(), screenGeometry.bottom() - menuSize.height());

    move(QPoint(x, y));
    popup->raise();
}

QPoint Menu::adjustSubMenuPosition(Menu* subMenu, const QPoint& intendedPos) {
    if (!subMenu || !subMenu->popup) return intendedPos;

    QSize size = subMenu->popup->size();
    QRect screenGeometry = QApplication::screenAt(intendedPos)->availableGeometry();

    return adjustXY(size, intendedPos, screenGeometry, true);
}

QString Menu::clickedItemText() const { return _clickedItemText; }
QString Menu::clickedItemShortcut() const { return _clickedItemShortcut; }
int Menu::clickedItemIndex() const { return _clickedItemIndex; }
void Menu::setParentMenu(Menu *parentMenu) { this->parentMenu = parentMenu; }
void Menu::setItemSize(QSize size) { itemSize = size.expandedTo(QSize(150, 36)); }
QSize Menu::getItemSize() { return itemSize; }
void Menu::setIconic(bool value) { isIconic = value; }
void Menu::setDarkMode(bool value) { isDarkMode = value; }
void Menu::setMaxVisibleItems(int items) { maxVisibleItems = items; }
int Menu::getMaxVisibleItems() { return maxVisibleItems; }

void Menu::mouseMoveEvent(QMouseEvent *event) {
    const QModelIndex index = indexAt(event->position().toPoint());
    if (!index.isValid()) return;
    QPoint globalPos = mapToGlobal(visualRect(index).topRight()) - QPoint(4,0);

    setHoveredIndex(index);

        if (subMenuTimer) {
            subMenuTimer->stop();
            delete subMenuTimer;
            subMenuTimer = nullptr;
        }

    for (auto it = subMenus.begin(); it != subMenus.end(); ++it) {
        Menu *subMenu = it.value();
        int subMenuIndex = it.key();

        if (subMenuIndex == index.row()) {
            if (subMenu && !subMenu->isVisible() ) {
                subMenuTimer = new QTimer(this);
                subMenuTimer->setSingleShot(true);
                subMenuTimer->start(300);

                connect(subMenuTimer, &QTimer::timeout, [=](){
                    QModelIndex index = indexAt(mapFromGlobal(QCursor::pos()));
                    if (index.isValid() && index.row() == subMenuIndex)  {
                        subMenu->show();
                        QPoint adjusted = adjustSubMenuPosition(subMenu, globalPos);
                        subMenu->move(adjusted);
                    } 
                });
            }
        } else {
            if (subMenu && subMenu->isVisible()) 
                subMenu->hide();
        }
    }
    update();
    QListView::mouseMoveEvent(event);
}

void Menu::leaveEvent(QEvent *event) {
   QTimer::singleShot(200, [this](){
     bool stillHovering = false;
        for (auto *submenu : subMenus) 
            if (submenu && submenu->popup && submenu->popup->underMouse()) 
                stillHovering = true;

        if (!stillHovering) {  
            setHoveredIndex(QModelIndex());
            update();
        }
   });

   QListView::leaveEvent(event);
}

void Menu::onItemClicked(const QModelIndex &index) {
    int idx = index.row();

    bool hasSubMenu = index.data(Qt::UserRole + 2).toBool();
    if (hasSubMenu) 
        return;

    _clickedItemText = index.data(Qt::DisplayRole).toString();
    _clickedItemIndex = idx;
    _clickedItemShortcut = index.data(Qt::UserRole + 1).toString();

    emit itemClicked();
    fadeOutAnimation();
}

