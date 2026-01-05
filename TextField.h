#pragma once

#include "Button.h"
#include "Menu.h"
#include "SmoothShadow.h"

#include <QResizeEvent>
#include <QFocusEvent>
#include <QContextMenuEvent>
#include <QTimer>
#include <QDebug>
#include <algorithm>
#include <QLineEdit>

class TextField : public QLineEdit {
  Q_OBJECT

  public:
  explicit TextField(const QString &text, QWidget *parent = nullptr);
  explicit TextField(QWidget *parent = nullptr);

  void setShadow(bool value);
  virtual void setDarkMode(bool value);
  void setFixedSize(QSize s);
  void setTextFieldIcon(bool value);
  void setTextFieldIconSize(QSize s);
  void setIconPaths(const QString &lightIcon = "", const QString &darkIcon = "");
  void setClearButton(bool value);
  void setPasswordTextField(bool value);
  void setDropDownPadding(bool value);
  void setReadOnly(bool value);
  void setEnabled(bool value);
  void setFontProperties(const QString &family, int pointSize = 12, bool bold = false, bool italic = false);
  void setSpacingRight(bool value);
  void setContextMenu(bool value);

  protected:
  void paintEvent(QPaintEvent *event) override;
  void enterEvent(QEnterEvent *event) override;
  void leaveEvent(QEvent *event) override;
  void focusInEvent(QFocusEvent *event) override;
  void focusOutEvent(QFocusEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  void contextMenuEvent(QContextMenuEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;


  private:
  void init();
  void updateStyle();
  void buttonPositioning(Button *button);

  // States
  bool isHover = false;
  bool isFocused = false;
  bool isDarkMode = false;
  bool hasShadow = false;
  bool textFieldIcon = false;
  bool clearButton = false;
  bool passwordButton = false;
  bool isPasswordVisible = false;
  bool dropDownPadding = false;
  bool isReadOnly = false;
  bool isEnabled = true;
  bool isBold = false;
  bool isItalic = false;
  bool rightSpacing = false;
  bool cxtMenu = true;

  // Graphics
  SmoothShadow *effect = nullptr;
  QPropertyAnimation *animate = nullptr;

  // Icons
  QString light_icon;
  QString dark_icon;
  const QString eyeIcon = ":/icons/ComponentsIcons/eye.svg";
  const QString eyeClosedIcon = ":/icons/ComponentsIcons/eye-closed.svg";
  const QString clearIcon = ":/icons/ComponentsIcons/x.svg";
  QSize textFieldIconSize = QSize(20, 20);

  // Buttons
  Button *clear = nullptr;
  Button *password = nullptr;

  // Font 
  QString fontFamily = "Segoe UI";
  int fontSize = 10;

  // Size
  const int fixedHeight = 36;
  const int minWidth = 36;

  // Menu
  Menu *menu = nullptr;
};
