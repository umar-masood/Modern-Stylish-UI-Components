#include "Seperator.h"

Seperator::Seperator(QWidget *parent, int height, int width, Qt::Orientation o) : QWidget(parent), orientation(o) {
    setFixedSize(width, height);
}

void Seperator::setColor(const QString &hex) {
    color = hex;
    update();
}

void Seperator::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QColor lineColor = !color.isEmpty() ? color : "#8D8D8D";
    painter.setPen(QPen(QBrush(lineColor), 1));

    if (orientation == Qt::Horizontal) 
        painter.drawLine(0, height() / 2, width(), height() / 2);
    else if (orientation == Qt::Vertical) 
        painter.drawLine(width() / 2, 0, width() / 2, height());
}