#pragma once

#include <QStyledItemDelegate>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QIcon>
#include <QPixmap>
#include <QFont>
#include <QFontMetrics>
#include <QVariant>
#include <QColor>
#include <QSize>
#include <QRect>
#include <QString>

#include "../../resources/IconManager.h"

class Delegate : public QStyledItemDelegate {
    Q_OBJECT

    public:
    explicit Delegate(const QSize& itemSize, QObject *parent = nullptr);
    void setDarkMode(bool value);
    void setIconic(bool value);
    void setAsMenu(bool value);
    void setDelegateSize(QSize size);
    void setHoveredIndex(const QModelIndex &index);
    void setSelectionDotIndicator(bool enable);
    void setSelectionCheckIndicator(bool enable);
    void setActiveIndex(const QModelIndex &index);

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    private:
    QSize m_itemSize;
    QModelIndex hoveredIndex, activeIndex;

    bool isDarkMode = false;
    bool isIconic = false;
    bool isMenu = true;
    bool hasDotIndicator = false;
    bool hasCheckIndicator = false;
};