#include "IconManager.h"

QString IconManager::icon(Icons icon) {
    switch (icon) {   
        case Icons::ArrowDown:         return ":/icons/arrow-down.svg";
        case Icons::ArrowRight:        return ":/icons/arrow-right.svg";

        case Icons::CheckBox_Check:    return ":/icons/checkbox-check.svg";
        case Icons::Delegate_Check:    return ":/icons/delegate-check.svg";
        case Icons::Dot:               return ":/icons/dot.svg";

        case Icons::Search:            return ":/icons/search.svg";
        case Icons::Cut:               return ":/icons/cut.svg";
        case Icons::Copy:              return ":/icons/copy.svg";
        case Icons::MenuDelete:        return ":/icons/menuDelete.svg";   
        case Icons::Hide:              return ":/icons/hide.svg";
        case Icons::Show:              return ":/icons/show.svg";
        case Icons::Paste:             return ":/icons/paste.svg";
        case Icons::Redo:              return ":/icons/redo.svg";
        case Icons::SelectAll:         return ":/icons/select-all.svg";
        case Icons::Undo:              return ":/icons/undo.svg";
        case Icons::Clear:             return ":/icons/x.svg";
    }

    qWarning("The requested icon is not found");
    return QString();
}

QPixmap IconManager::renderSvg(const QString &iconPath, const QSize &size) {
    QSvgRenderer svg(iconPath);
    svg.setAspectRatioMode(Qt::KeepAspectRatio);
    
    if (!svg.isValid()) {
        qWarning() << "SVG not valid or not found:" << iconPath;
        return QPixmap();
    }

    // --- Render SVG into 16x16 pixmap ---
    QPixmap pm(size);
    pm.fill(Qt::transparent);

    QPainter p(&pm);
    p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    svg.render(&p, QRectF(0, 0, pm.width(), pm.height()));
    p.end();

    return pm;
}