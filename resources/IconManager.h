#pragma once
#include <QString>
#include <QPainter>
#include <QPixmap>
#include <QSvgRenderer>
#include <QSize>
#include <QDebug>

#include "Icons.h"

class IconManager {
    
    public:
    static QString icon(Icons icon);
    static QPixmap renderSvg(const QString &iconPath, const QSize &size);
};