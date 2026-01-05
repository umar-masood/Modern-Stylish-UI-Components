#pragma once

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QColor>
#include <QFont>
#include <QFontMetrics>
#include <QPaintEvent>
#include <QSize>
#include <QString>

class RoundedBox : public QWidget {
    Q_OBJECT

    public:
    explicit RoundedBox(const QString &txt = "", QWidget *parent = nullptr);
    virtual ~RoundedBox() = default;

    void setDarkMode(bool value);
    void setAsToolTip(bool value);
    void setText(const QString &text);
    QSize sizeHint() const;

    protected:
    void paintEvent(QPaintEvent *event) override;

    private:
    void updateSizeForText(); 

    bool isDarkMode;
    bool useAsToolTip;
    QString text;
};