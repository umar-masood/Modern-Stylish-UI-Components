#pragma once

#include "Button.h"
#include "Menu.h"
#include "SmoothShadow.h"
#include "../../resources/IconManager.h"

#include <QResizeEvent>
#include <QFocusEvent>
#include <QContextMenuEvent>
#include <QTimer>
#include <QDebug>
#include <algorithm>
#include <QLineEdit>
#include <QHash>

class TextField : public QLineEdit {
  Q_OBJECT

  public:
  enum TextFieldTextColor {
    NormalTxtLight,
    NormalTxtDark,
    SelectedTxt,
    PlaceHolderTxt,
    SelectionBackgroundTxt,
  };

  enum TextFieldColor {
    BorderFocused,
    BorderLight,
    BorderDark,

    FocusedLight,
    FocusedDark,

    HoverLight,
    HoverDark,

    NormalLight,
    NormalDark,
  };

  explicit TextField(const QString &text, QWidget *parent = nullptr);
  explicit TextField(QWidget *parent = nullptr);
  
  void setShadow(bool enable);
  virtual void setDarkMode(bool enable);
  void setFixedSize(QSize s);
  void setIconic(bool enable);
  void setIconSize(QSize s);
  void setIconPaths(const QString &lightIcon = QString(), const QString &darkIcon = QString());
  void setClearButton(bool enable);
  void setPasswordMode(bool enable);
  void setPadding(int left = 0, int top = 0, int right = 0, int bottom = 0);
  void setReadOnly(bool enable);
  void setEnabled(bool enable);
  void setFontProperties(const QString &family, int pointSize = 12, bool bold = false, bool italic = false);
  void setContextMenu(bool enable);
  void setBorderTransparent(bool enable);
  void setNormalBackgroundTransparent(bool enable);
  void setColor(const TextFieldColor &state, const QColor &color);
  void setTextColor(const TextFieldTextColor &state, const QColor &color);

  inline uint qHash(const TextFieldColor &state, uint seed = 0) {
    return ::qHash(static_cast<int>(state), seed);
  }

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
  void loadDefaultColors();
  void updateStyle();
  void positionButton(Button *button);

  QColor color(const TextFieldColor &state) const;
  QColor textColor(const TextFieldTextColor &state) const;

  // States
  bool isHover = false;
  bool isFocused = false;
  bool isDarkMode = false;

  bool hasShadow = false;
  bool hasTextFieldIcon = false;
  bool hasClearButton = false;
  bool hasPasswordButton = false;
  bool hasContextMenu = true;

  bool isPasswordVisible = false;
  bool isReadOnly = false;
  bool isEnabled = true;
  bool isBold = false;
  bool isItalic = false;
  bool isBorderTransparent = false;
  bool isBackgroundTransparent = false;

  // Padding
  int _left = 0, _right = 0, _top = 0, _bottom = 0;

  // Border & BG Colors
  QHash<TextFieldColor, QColor> _colors;

  // Text Colors
  QHash<TextFieldTextColor, QColor> _textColors;

  // Icons
  const QString CopyIcon      =  IconManager::icon(Icons::Copy);
  const QString CutIcon       =  IconManager::icon(Icons::Cut);
  const QString PasteIcon     =  IconManager::icon(Icons::Paste);
  const QString DeleteIcon    =  IconManager::icon(Icons::MenuDelete);
  const QString SelectAllIcon =  IconManager::icon(Icons::SelectAll);
  const QString UndoIcon      =  IconManager::icon(Icons::Undo);
  const QString RedoIcon      =  IconManager::icon(Icons::Redo);
  const QString ShowIcon      =  IconManager::icon(Icons::Show);     
  const QString HideIcon      =  IconManager::icon(Icons::Hide);   
  const QString ClearIcon     =  IconManager::icon(Icons::Clear);   

  // Graphics
  SmoothShadow *effect = nullptr;
  QPropertyAnimation *animate = nullptr;

  // TextField Icons
  QString light_icon, dark_icon;
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
