#include "RoundedBox.h"

RoundedBox::RoundedBox(const QString &txt, QWidget *parent, bool isToolTip) : QWidget(nullptr), isDarkMode(false), text(txt), _isToolTip(isToolTip) {
    Qt::WindowFlags flags = Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint;

    if (isToolTip) {
        flags |= Qt::ToolTip;           
        setAttribute(Qt::WA_ShowWithoutActivating);
        setFocusPolicy(Qt::NoFocus);
    } else {
        flags |= Qt::WindowStaysOnTopHint;
        flags |= Qt::Tool;   
    }

    setWindowFlags(flags);
    setAttribute(Qt::WA_TranslucentBackground);
}

void RoundedBox::setDarkMode(bool value) {
    isDarkMode = value;
    update();
}

void RoundedBox::setAsToolTip(bool value) {
    _isToolTip = value;
    update();
}

void RoundedBox::setText(const QString &text) {
    this->text = text;
    update();
}

void RoundedBox::updateSizeForText() { resize(sizeHint()); }

QSize RoundedBox::sizeHint() const {
    QFont font;
    font.setPointSize(9);
    font.setFamily("Segoe UI");

    QFontMetrics fm(font);
    int MAX_W = 400;
    QSize s = fm.boundingRect(0, 0, MAX_W, 0, Qt::TextWordWrap, text).size();
    return QSize(s.width() + 24, s.height() + 12);
}

void RoundedBox::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    // Colors
    QColor BG = isDarkMode ? QColor("#1F1F1F") : QColor("#FFFFFF");
    QColor BR = "#AFAFAF";

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setBrush(BG);
    QPen pen(BR);
    pen.setWidthF(0.2);
    painter.setPen(pen);

    QPainterPath path;
    path.addRoundedRect(rect().adjusted(1, 1, -1, -1), 6, 6);
    painter.drawPath(path);

    // Text
    if (_isToolTip) {
        QFont font;
        font.setPointSize(9);
        font.setFamily("Segoe UI");
        painter.setFont(font);
        painter.setPen(isDarkMode ? QColor("#F0F0F0") : QColor("#000000"));
        QRect text_area(12, 0, width() - 24, height());
        painter.drawText(text_area, Qt::AlignCenter | Qt::TextWordWrap, text);
    }
}

