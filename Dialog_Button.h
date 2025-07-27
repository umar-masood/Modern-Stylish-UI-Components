#ifndef DIALOG_BUTTON_H
#define DIALOG_BUTTON_H

#include <QPushButton>
#include <QGraphicsDropShadowEffect>
#include <QTimer>

inline void dialog_button(QPushButton *button, const QString &text)
{
    button->setText(text);
    button->setFixedSize(130,35);
    button->setStyleSheet("QPushButton {"
                          "border-radius: 5px;"
                          "font-size: 15px;"
                          "font-weight: Bold;"
                          "background: #F74032 ;"
                          "color: white;"
                          "border: 1.5px solid #E03A2F;"
                          "}"
                          "QPushButton:disabled {"
                          "border: 1.5px solid #F7DFDD ;"
                          "background: #F7DFDD;"
                          "color: white;"
                          "}"
                          "QPushButton:pressed {"
                          "border: 1.5px solid #AD2D24 ;"
                          "background: #E03A2F;"
                          "}");

    // Hover Shadow Effect
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(button);
    shadowEffect->setBlurRadius(15);
    shadowEffect->setOffset(0, 0);
    shadowEffect->setColor(QColor("#F7B5B0"));
    button->setGraphicsEffect(shadowEffect);
    shadowEffect->setEnabled(false);

    QTimer *timer = new QTimer(button);
    timer->setInterval(100);
    timer->start();

    QObject::connect(timer, &QTimer::timeout, [button, shadowEffect](){
        if (button->underMouse()) {
            shadowEffect->setEnabled(true);
        } else {
            shadowEffect->setEnabled(false);
        }
    });
}

// Success

inline void success_button(QPushButton *button, const QString &text)
{
    button->setText(text);
    button->setFixedSize(130,35);
    button->setStyleSheet("QPushButton {"
                          "border-radius: 5px;"
                          "font-size: 15px;"
                          "font-weight: Bold;"
                          "background: #24D57A ;"
                          "color: white;"
                          "border: 1.5px solid #1CA860;"
                          "}"
                          "QPushButton:disabled {"
                          "border: 1.5px solid #DFFFED ;"
                          "background: #DFFFED;"
                          "color: white;"
                          "}"
                          "QPushButton:pressed {"
                          "border: 1.5px solid #1DA660 ;"
                          "background: #16804A;"
                          "}");

    // Hover Shadow Effect
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(button);
    shadowEffect->setBlurRadius(15);
    shadowEffect->setOffset(0, 0);
    shadowEffect->setColor(QColor("#AFFFD6"));
    button->setGraphicsEffect(shadowEffect);
    shadowEffect->setEnabled(false);

    QTimer *timer = new QTimer(button);
    timer->setInterval(100);
    timer->start();

    QObject::connect(timer, &QTimer::timeout, [button, shadowEffect](){
        if (button->underMouse()) {
            shadowEffect->setEnabled(true);
        } else {
            shadowEffect->setEnabled(false);
        }
    });
}


#endif // DIALOG_BUTTON_H
