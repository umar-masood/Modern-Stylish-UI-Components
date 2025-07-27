#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include <QtWidgets>

class ScrollBar : public QScrollBar {
  Q_OBJECT
  Q_PROPERTY(qreal opacity READ getOpacity WRITE setOpacity)
  Q_PROPERTY(qreal handle READ getHandle WRITE setHandle)

public:
  explicit ScrollBar(Qt::Orientation orientation, QWidget *parent = nullptr);
  explicit ScrollBar(QWidget *parent = nullptr);
  ~ScrollBar() noexcept override = default;

  void setDarkMode(bool value);

  void setOpacity(qreal o);
  qreal getOpacity() const;

  void setHandle(qreal w);
  qreal getHandle();

protected:
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void enterEvent(QEnterEvent *event) override;
  void leaveEvent(QEvent *event) override;

private:
  void init();

  bool isPressed = false;
  bool isHover = false;
  bool isDarkMode = false;

  qreal opacity = 0.0;
  QPropertyAnimation *animation1 = nullptr;

  qreal handle = 2.0;
  QPropertyAnimation *animation2 = nullptr;
};

#endif // SCROLLBAR_H
