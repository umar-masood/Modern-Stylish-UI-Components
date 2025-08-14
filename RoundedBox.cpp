#include "RoundedBox.h"
#include <QPainter>
#include <QPainterPath>
#include <QBitmap>
#include <QFontMetrics>

RoundedBox::RoundedBox(const QString &txt, QWidget *parent)
    : QWidget(nullptr), isDarkMode(false), text(txt), useAsToolTip(false)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlag(Qt::Tool);
    setAsToolTip(false);
}

void RoundedBox::setDarkMode(bool value)
{
    isDarkMode = value;
    update();
}

void RoundedBox::setAsToolTip(bool value) {
    useAsToolTip = value;
    update();
}

void RoundedBox::updateSizeForText() {
    resize(sizeHint());
}

QSize RoundedBox::sizeHint() const {
    QFont font;
    font.setPointSize(9);
    font.setFamily("Segoe UI");

    QFontMetrics fm(font);
    int MAX_W = 400;
    QSize s = fm.boundingRect(0, 0, MAX_W, 0, Qt::TextWordWrap, text).size();
    return QSize(s.width() + 24, s.height() + 12);
}


void RoundedBox::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    // Colors
    QColor BG = isDarkMode ? QColor("#1F1F1F") : QColor("#FFFFFF");
    QColor BR = isDarkMode ? QColor("#4D4D4D") : QColor("#CCCCCC");

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setBrush(BG);
    QPen pen(BR);
    pen.setWidth(1);
    painter.setPen(pen);

    QPainterPath path;
    path.addRoundedRect(rect().adjusted(1.5, 1.5, -1.5, -1.5), 6, 6);
    painter.drawPath(path);

    // Text
    if (useAsToolTip) {
        QFont font;
        font.setPointSize(9);
        font.setFamily("Segoe UI");
        painter.setFont(font);
        painter.setPen(isDarkMode ? QColor("#F0F0F0") : QColor("#000000"));
        QRect text_area(12, 0, width() - 24, height());
        painter.drawText(text_area, Qt::AlignCenter | Qt::TextWordWrap, text);
    }
}

