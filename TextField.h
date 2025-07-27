#ifndef TEXTFIELD_H
#define TEXTFIELD_H

#include <QtWidgets>
#include "Button.h"
#include "SmoothShadow.h"
#include "Menu.h"

class TextField : public QLineEdit {
  Q_OBJECT

public:
  explicit TextField(const QString &text, QWidget *parent = nullptr);
  explicit TextField(QWidget *parent = nullptr);

  void setShadow(bool value);
  void setDarkMode(bool value);
  void setSize(QSize s);
  void setTextFieldIcon(bool value);
  void setTextFieldIconSize(QSize s);
  void setIconPaths(const QString &light_icon_path = "", const QString &dark_icon_path = "");
  void setClearButton(bool value);
  void setPasswordTextField(bool value);
  void setDropDownPadding(bool value);
  void setReadOnly(bool value);
  void setEnabled(bool value);

protected:
  void paintEvent(QPaintEvent *event) override;
  void enterEvent(QEnterEvent *event) override;
  void leaveEvent(QEvent *event) override;
  void focusInEvent(QFocusEvent *event) override;
  void focusOutEvent(QFocusEvent *event) override;
  void contextMenuEvent(QContextMenuEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;

private:
  void init();
  void updateStyle();

  // States
  bool isHover = false;
  bool isFocused = false;
  bool isDarkMode = false;
  bool hasShadow = true;
  bool textFieldIcon = false;
  bool clearButton = false;
  bool passwordButton = false;
  bool isPasswordVisible = false;
  bool dropDownPadding = false;
  bool isReadOnly = false;
  bool isEnabled = true;

  // Graphics
  SmoothDropShadow *effect = nullptr;
  QPropertyAnimation *animate = nullptr;

  // Icons
  QString light_icon;
  QString dark_icon;
  const QString eye_icon_path = ":/eye.svg";
  const QString eyeclosed_icon_path = ":/eye-closed.svg";
  QSize textFieldIconSize = QSize(20, 20);

  // Buttons
  Button *clear = nullptr;
  Button *password = nullptr;
};

#endif