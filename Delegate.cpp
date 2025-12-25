#include "Delegate.h"

Delegate::Delegate(const QSize& itemSize, QObject *parent)
    : QStyledItemDelegate(parent), m_itemSize(itemSize) {}

void Delegate::setDarkMode(bool value) {
    isDarkMode = value;
}

void Delegate::setIconic(bool value) {
    isIconic = value;
}

void Delegate::setAsMenu(bool value) {
    isMenu = value;
}

void Delegate::setDelegateSize(QSize size) {
    m_itemSize = size;
}

void Delegate::setHoveredIndex(const QModelIndex &index) {
    hoveredIndex = index;
}

void Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    painter->save();

    // Background Colors
    QColor Normal = Qt::transparent;
    QColor Light = QColor::fromString("#F0F0F0");
    QColor Dark = QColor::fromString("#383838");

    QColor current;

    bool isHovered = (index == hoveredIndex);
    bool isSelected = option.state & QStyle::State_Selected;
    bool isMouseOver = option.state & QStyle::State_MouseOver;
    
    if (isHovered || isSelected || isMouseOver) {
        current = isDarkMode ? Dark : Light;
    } else {
        current = Normal;
    }

    QRect fullRec = option.rect;
    painter->setPen(Qt::NoPen);
    painter->setBrush(current);
    painter->drawRoundedRect(fullRec, 6, 6);

    // Fetching Data from Index
    QString text = index.data(Qt::DisplayRole).toString();
    QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
    QString shortcutText = index.data(Qt::UserRole + 1).toString();
    bool hasSubMenu = index.data(Qt::UserRole + 2).toBool();

    // Icon geometry
    const QSize IconSize = QSize(20, 20);
    const int iconX = 12;
    int iconY;

    const int tX = isIconic ? (iconX + IconSize.width() + 12) : iconX;
    const int tY = fullRec.y();

    // Shortcut width based on content
    int shortcutW = 0;
    QFont shortcutFont("Segoe UI", 10, QFont::Normal);
    QFontMetrics fmShortcut(shortcutFont);

    if (isMenu && !shortcutText.isEmpty()) {
        shortcutW = qMin(fmShortcut.horizontalAdvance(shortcutText), 120);
    }

    const int spacing = (shortcutW > 0) ? 12 : 0;
    const int tW = fullRec.width() - (tX + spacing + shortcutW + 12);
    const int tH = fullRec.height();

    QRect textRect(tX, tY, tW, tH);
    QRect shortcutRect(fullRec.right() - shortcutW - 12, tY, shortcutW, tH);

    if (!icon.isNull() && isIconic) {
    QPixmap pixmap = icon.pixmap(IconSize);
    iconY = fullRec.y() + (fullRec.height() - IconSize.height()) / 2;
    painter->drawPixmap(iconX, iconY, pixmap);
    }

    if (shortcutText.isEmpty() && hasSubMenu) {
    QPixmap pixmap(":/icons/arrow-right.svg");
    pixmap = pixmap.scaled(IconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    iconY = fullRec.y() + (fullRec.height() - pixmap.height()) / 2;
    painter->drawPixmap(fullRec.right() - IconSize.width() - 12, iconY, pixmap);
    }

    // Text Font & Color
    QFont font("Segoe UI", 11, QFont::Normal);
    painter->setFont(font);
    QFontMetrics f(font);
    QString elidedText = f.elidedText(text, Qt::ElideRight, tW);

    QColor _NormalLight = Qt::black;
    QColor _NormalDark = Qt::white;
    QColor _Selected = QColor::fromString("#109AC7");
    QColor _current = (option.state & QStyle::State_Selected) ? _Selected : (isDarkMode ? _NormalDark : _NormalLight);

    painter->setPen(_current);
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, elidedText);

    // Draw Shortcut Text if exists
    if (shortcutW > 0) {
        painter->setFont(shortcutFont);
        QColor color = QColor("#A1A1A1");
        painter->setPen(color);
        painter->drawText(shortcutRect, Qt::AlignRight | Qt::AlignVCenter, shortcutText);
    }
    painter->restore();
}

QSize Delegate::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const {
    return QSize(m_itemSize.width(), m_itemSize.height());
}
