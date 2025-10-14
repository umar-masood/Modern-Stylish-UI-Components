#include "Seperator.h"

Seperator::Seperator(QWidget *parent, int height, int width, Qt::Orientation o) : QWidget(parent), orientation(o) {
    setFixedSize(width, height);
}

void Seperator::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(QPen(QBrush("#8D8D8D"), 1));

    if (orientation == Qt::Horizontal) {
        painter.drawLine(0, height() / 2, width(), height() / 2);
    } else if (orientation == Qt::Vertical) {
        painter.drawLine(width() / 2, 0, width() / 2, height());
    }
}