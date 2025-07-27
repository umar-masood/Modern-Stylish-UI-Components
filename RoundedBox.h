#ifndef ROUNDEDBOX_H
#define ROUNDEDBOX_H

#include <QtWidgets>

class RoundedBox : public QWidget {
    Q_OBJECT

public:
    explicit RoundedBox(const QString &txt = "", QWidget *parent = nullptr);
    virtual ~RoundedBox() = default;
    void setDarkMode(bool value);
    void setAsToolTip(bool value);
    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void updateSizeForText(); 

    bool isDarkMode;
    bool useAsToolTip;
    QString text;
};

#endif // ROUNDEDBOX_H
