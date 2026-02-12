#include "Delegate.h"

Delegate::Delegate(const QSize& itemSize, QObject *parent) : QStyledItemDelegate(parent), m_itemSize(itemSize) {}

void Delegate::setDarkMode(bool value) { isDarkMode = value; }
void Delegate::setIconic(bool value) { isIconic = value; }
void Delegate::setAsMenu(bool value) { isMenu = value; }
void Delegate::setDelegateSize(QSize size) { m_itemSize = size; }
void Delegate::setHoveredIndex(const QModelIndex &index) { hoveredIndex = index; }
void Delegate::setSelectionDotIndicator(bool enable) { 
    hasDotIndicator = enable; 
    if (hasCheckIndicator)
        hasCheckIndicator = false;
}

void Delegate::setSelectionCheckIndicator(bool enable) { 
    hasCheckIndicator = enable; 
    if (hasDotIndicator)
        hasDotIndicator = false;
}

void Delegate::setActiveIndex(const QModelIndex &index) { activeIndex = index; }

void Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    painter->save();
    
    // ----------------- Fetching Data from Index ------------------------
    QString text = index.data(Qt::DisplayRole).toString();
    QString iconPath = index.data(Qt::UserRole + 3).toString();
    QString shortcutText = index.data(Qt::UserRole + 1).toString();
    bool hasSubMenu = index.data(Qt::UserRole + 2).toBool();

    // ------------------ States ---------------------------
    bool isHovered = (index == hoveredIndex);
    bool isSelected = option.state & QStyle::State_Selected;
    bool isMouseOver = option.state & QStyle::State_MouseOver;

    // -------------------------- Background  ----------------------------
    QRect fullRec = option.rect;
    painter->setPen(Qt::NoPen);
    painter->setBrush(
        (isHovered || isSelected || isMouseOver) ? (isDarkMode ? QColor("#383838") : QColor("#F0F0F0")) : Qt::transparent
    );
    painter->drawRoundedRect(fullRec.adjusted(1, 1, -1, -1), 6, 6); // Adjusted for small spacing between items

    // Icon geometry
    const QSize IconSize = QSize(18, 18);
    int iconX = 12;
    int iconY = fullRec.y() + (fullRec.height() - IconSize.height()) / 2;

    // -------------------------- Indicators --------------------------------
    // Drawing Dot Indicator
    bool isActive = (index == activeIndex);
    if (hasDotIndicator && isActive) 
        painter->drawPixmap(iconX, iconY, IconManager::renderSvg(IconManager::icon(Icons::Dot), IconSize));

    // Drawing Check Indicator
    if (hasCheckIndicator && isActive) 
        painter->drawPixmap(iconX, iconY, IconManager::renderSvg(IconManager::icon(Icons::Delegate_Check), IconSize));
    
    // ---------------------  Adjusting Item Text X Y ------------------------
    const int tY = fullRec.y() - 1;
    int tX = iconX;

    if (isIconic)
        tX += IconSize.width() + 12;

    if (hasDotIndicator || hasCheckIndicator)
        tX += IconSize.width() + 12;

    // --------------------- Adjusting Shortcut width based on content -----------
    int shortcutW = 0;
    QFont shortcutFont("Segoe UI", 10, QFont::Medium);
    QFontMetrics fmShortcut(shortcutFont);

    if (isMenu && !shortcutText.isEmpty()) 
        shortcutW = qMin(fmShortcut.horizontalAdvance(shortcutText), 120);

    // ---------------- Items Icon -----------------------------------------------
    QPixmap icon = IconManager::renderSvg(iconPath, IconSize);
    if (!icon.isNull() && isIconic) {
        if (hasCheckIndicator || hasDotIndicator) 
            iconX = 12 + IconSize.width() + 12;
        painter->drawPixmap(iconX, iconY, icon);
    }

    // -------------- SubMenu Arrow Right Icon -----------------------------------
    if (shortcutText.isEmpty() && hasSubMenu) {
        QPixmap submenuIcon = IconManager::renderSvg(IconManager::icon(Icons::ArrowRight), IconSize);
        iconY = fullRec.y() + (fullRec.height() - submenuIcon.height()) / 2;
        painter->drawPixmap(fullRec.right() - IconSize.width() - 12, iconY, submenuIcon);
    }

    // ------------- Item Text ---------------------------------------------------
    const int spacing = (shortcutW > 0) ? 12 : 0;
    const int tW = fullRec.width() - (tX + spacing + shortcutW + 12);
    const int tH = fullRec.height();

    QRect textRect(tX, tY, tW, tH);

    QFont font("Segoe UI", 10, QFont::Medium);
    QFontMetrics f(font);
    QString elidedText = f.elidedText(text, Qt::ElideRight, tW);

    painter->setFont(font);
    painter->setPen( isSelected ? QColor::fromString("#0191DF") : (isDarkMode ? Qt::white : Qt::black));
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, elidedText);

    // ----------------- Shortcut Text -----------------------------------------
    QRect shortcutRect(fullRec.right() - shortcutW - 12, tY, shortcutW, tH);

    if (shortcutW > 0) {
        painter->setFont(shortcutFont);
        painter->setPen("#8D8D8D");
        painter->drawText(shortcutRect, Qt::AlignRight | Qt::AlignVCenter, shortcutText);
    }

    painter->restore();
}

QSize Delegate::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const { 
    return QSize(m_itemSize.width(), m_itemSize.height()); 
}
