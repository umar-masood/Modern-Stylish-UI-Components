#include "TextField.h"

TextField::TextField(const QString &text, QWidget *parent) : QLineEdit(text, parent) { init(); }
TextField::TextField(QWidget *parent) : QLineEdit(parent) { init(); }
void TextField::setShadow(bool value) { hasShadow = value; }

void TextField::setDarkMode(bool value) {
    isDarkMode = value;

    if (clear) 
        clear->setDarkMode(isDarkMode); 

    if (password) 
        password->setDarkMode(isDarkMode);

    updateStyle();
}

void TextField::setFixedSize(QSize s) {
    int finalWidth = std::max(s.width(), minWidth);
    int finalHeight = s.height();

    QLineEdit::setFixedSize(finalWidth, finalHeight);
}


void TextField::setTextFieldIcon(bool value) {
    hasTextFieldIcon = value;
    updateStyle();
}

void TextField::setTextFieldIconSize(QSize s) { if (hasTextFieldIcon) textFieldIconSize = s; }

void TextField::setIconPaths(const QString &lightIcon, const QString &darkIcon) {
    if (hasTextFieldIcon) {
        light_icon = lightIcon;
        dark_icon = darkIcon;
    } else {
        light_icon.clear();
        dark_icon.clear();
    }
}

void TextField::setReadOnly(bool value) {
    isReadOnly = value;
    QLineEdit::setReadOnly(value);
}

void TextField::setEnabled(bool value) {
    isEnabled = value;
    QLineEdit::setEnabled(value);
}

void TextField::setContextMenu(bool value) { hasContextMenu = value; }
void TextField::setFontProperties(const QString &family, int pointSize, bool bold, bool italic) {
    isItalic = italic; isBold = bold; fontSize = pointSize; fontFamily = family;
    updateStyle(); 
}

void TextField::setClearButton(bool value) {
    hasClearButton = value;

    if (hasClearButton && !clear) {
        clear = new Button(this);
        clear->setCursor(Qt::PointingHandCursor);
        clear->setDisplayMode(Button::IconOnly);
        clear->setNormalBackgroundTransparent(true);
        clear->setBorderTransparent(true);
        clear->setIconSize(this->textFieldIconSize);
        clear->setFixedSize(QSize(28, 28));
        clear->setIconPaths(ClearIcon, ClearIcon);
        clear->setEnabled(isEnabled);
        positionButton(clear);
        
        clear->raise();
        clear->hide();

        connect(clear, &Button::pressed, this, [this]() {
            QTimer::singleShot(250, this, SLOT(clear()));
        });

        connect(this, &QLineEdit::textChanged, this, [this](const QString &text) {
            clear->setVisible(isFocused && !text.isEmpty());
        });

        updateStyle();
    }
}

void TextField::setPasswordTextField(bool value) {
    hasPasswordButton = value;

    if (hasPasswordButton && !password && !hasClearButton) {
        password = new Button(this);
        password->setCursor(Qt::PointingHandCursor);
        password->setNormalBackgroundTransparent(true);
        password->setBorderTransparent(true);
        password->setDisplayMode(Button::IconOnly);
        password->setIconSize(this->textFieldIconSize);
        password->setFixedSize(QSize(28, 28));
        password->setEnabled(isEnabled);
        this->setEchoMode(QLineEdit::Password);
        positionButton(password);

        password->hide();
        password->setIconPaths(HideIcon, HideIcon);

        connect(password, &Button::pressed, this, [this]() {
            isPasswordVisible = !isPasswordVisible;

            if (isPasswordVisible) {
                password->setIconPaths(ShowIcon, ShowIcon);
                this->setEchoMode(QLineEdit::Normal);
            } else {
                password->setIconPaths(HideIcon, HideIcon);
                this->setEchoMode(QLineEdit::Password);
            }
        });

        connect(this, &QLineEdit::textChanged, this, [this](const QString &text) {
            password->setVisible(isFocused && !text.isEmpty());

            if (isPasswordVisible) {
                password->setIconPaths(HideIcon, HideIcon);
                this->setEchoMode(QLineEdit::Password);
                isPasswordVisible = false;
            }
        });

        updateStyle();
    }
    update();
}

void TextField::setPadding(int left, int top, int right, int bottom) {
    _left = left; 
    _right = right, 
    _bottom = bottom, 
    _top = top;

    updateStyle();
}

void TextField::setTextSelectedBackgroundColor(const QString &hex) { _selected_text_background_color = hex; updateStyle(); }
void TextField::setTextSelectionColor(const QString &hex) { _selected_text_color = hex; updateStyle(); }
void TextField::setTextColor(const QString &hex) { _text_color = hex; updateStyle(); }
void TextField::setPlaceHolderTextColor(const QString &hex) { _placeholder_text_color = hex; updateStyle(); }

void TextField::resizeEvent(QResizeEvent *event) {
    QLineEdit::resizeEvent(event);
    positionButton(clear);
    positionButton(password);
}

void TextField::positionButton(Button *button) {
    if (button) {
        int x = width() - (12 + button->width()) + 3;
        int y = (height() - button->height()) / 2;
        button->move(x, y);
    }
}

void TextField::setBorderTransparent(bool value) { isBorderTransparent = value; }
void TextField::setNormalBackgroundTransparent(bool value) { isBackgroundTransparent = value; }

void TextField::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

    QRect rec = rect().adjusted(1, 1, -1, -1);

    QPen pen;
    pen.setWidthF(isFocused ? 1.0 : 0.5);

    QColor border_color;
    if (isBorderTransparent) 
        border_color = Qt::transparent;
    else
        border_color = isFocused ? QColor("#0191DF") : (isDarkMode ? QColor("#4D4D4D") : QColor("#CCCCCC"));

    pen.setColor(border_color);
    pen.setStyle(Qt::SolidLine);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);

    QColor bg_color;

    if (isFocused)
        bg_color = isDarkMode ? QColor("#242424") : QColor("#FFFFFF");
    else if (isHover)
        bg_color = isDarkMode ? QColor("#323232") : QColor("#F0F0F0");
    else if (isBackgroundTransparent)
        bg_color = Qt::transparent;
    else
        bg_color = isDarkMode ? QColor("#2D2D2D") : QColor("#FBFBFB");

    painter.setBrush(bg_color);

    QPainterPath path;
    path.addRoundedRect(rec, 6, 6);
    painter.drawPath(path);

    if (hasTextFieldIcon) {
        QString iconPath = isDarkMode ? dark_icon : light_icon;
        if (!iconPath.isEmpty()) {
            QPixmap pixmap(iconPath);
            pixmap = pixmap.scaled(textFieldIconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            if (!pixmap.isNull()) {
                int yPos = (rect().height() - pixmap.height()) / 2;
                int xPos = 12;
                painter.drawPixmap(xPos, yPos, pixmap);
            }
        }
    }

    QLineEdit::paintEvent(event);
}

void TextField::keyPressEvent(QKeyEvent *event) {
    if (!hasContextMenu && event->modifiers() & Qt::ControlModifier) {
        switch(event->key()) {
            case Qt::Key_C:
            case Qt::Key_V:
            case Qt::Key_X:
            case Qt::Key_A:
                return;
            default:
                break;
        }
    }

    QLineEdit::keyPressEvent(event);
}

void TextField::enterEvent(QEnterEvent *event) {
    isHover = true;
    QLineEdit::enterEvent(event);
    update();
}

void TextField::leaveEvent(QEvent *event) {
    isHover = false;
    QLineEdit::leaveEvent(event);
    update();
}

void TextField::focusInEvent(QFocusEvent *event) {
    isFocused = true;
    
    if (clear) clear->setVisible(!text().isEmpty());

    if (hasShadow) {
        effect->setColor(QColor("#32CCFE"));
        animate->setStartValue(effect->blurRadius());
        animate->setEndValue(18);
        animate->start();
    }

    QLineEdit::focusInEvent(event);
    update();
}

void TextField::focusOutEvent(QFocusEvent *event) {
    isFocused = false;
 
    if (clear) clear->setVisible(false);

    if (hasShadow) {
        animate->setStartValue(effect->blurRadius());
        animate->setEndValue(0);
        animate->start();
    }

    QLineEdit::focusOutEvent(event);
    update();
}

void TextField::contextMenuEvent(QContextMenuEvent *event) {
    if (!hasContextMenu) 
        return;

    menu = new Menu(this);
    menu->setMaxVisibleItems(12);
    menu->setItemSize(QSize(180, 36));
    menu->setDarkMode(isDarkMode);
    menu->setIconic(true);

    const bool hasText = !this->text().isEmpty();
    const bool hasSelection = this->hasSelectedText();

    if (hasSelection) {
        menu->addAction({ "Copy",    false,  "Ctrl + C",  CopyIcon,    CopyIcon });
        menu->addAction({ "Cut",     false,  "Ctrl + X",  CutIcon,     CutIcon });
        menu->addAction({ "Delete",  false,  "Delete",    DeleteIcon,  DeleteIcon });
    }

    menu->addAction({ "Paste", false, "Ctrl + V", PasteIcon, PasteIcon});
    
    if (hasText && !hasSelection) menu->addAction({ "Select All",  false,  "Ctrl + A",  SelectAllIcon,  SelectAllIcon});
    if (this->isUndoAvailable())  menu->addAction({ "Undo",        false,  "Ctrl + Z",  UndoIcon,       UndoIcon});
    if (this->isRedoAvailable())  menu->addAction({ "Redo",        false,  "Ctrl + Y",  RedoIcon,       RedoIcon});

    connect(menu, &Menu::itemClicked, this, [=]() {
        QString action = menu->clickedItemText();
        if (action == "Copy") copy();
        else if (action == "Cut") cut();
        else if (action == "Paste") paste();
        else if (action == "Delete") del();
        else if (action == "Select All") selectAll();
        else if (action == "Undo") undo();
        else if (action == "Redo") redo();
        else qDebug() << "Unknown context menu action:" << action;
    });

    menu->show();
    menu->move(event->globalPos());
}

void TextField::init() {
    setFixedSize(QSize(0, 0));
    setFocusPolicy(Qt::ClickFocus);
    updateStyle();
    
    effect = new SmoothShadow(this);
    effect->setOffset(0, 0);
    effect->setColor(QColor(0, 0, 0, 0));
    effect->setBlurRadius(0);

    if (hasShadow)
        setGraphicsEffect(effect);

    animate = new QPropertyAnimation(effect, "blurRadius", this);
    animate->setDuration(300);
    animate->setEasingCurve(QEasingCurve::InOutQuad);
}

void TextField::updateStyle() {
    QString styleSheet = QString(R"(
      QLineEdit {
        font-family: '%1';
        font-size: %2pt;
        font-weight: %3;
        font-style: %4;
        background-color: transparent;
        border-radius: 0px;
        border: none;
        outline: none;
        color: %5;
        selection-background-color: %6;
        selection-color: %7;
        placeholder-text-color: %8;
        padding-left: %9px;
        padding-right: %10px;
        padding-bottom: %11px;
        padding-top: %12px;
      })")
        .arg(fontFamily)
        .arg(fontSize)
        .arg(isBold ? "bold" : "normal")
        .arg(isItalic ? "italic" : "normal")
        .arg(!_text_color.isEmpty() ? _text_color : (isDarkMode ? "#FFFFFF" : "#000000"))
        .arg(!_selected_text_background_color.isEmpty() ? _selected_text_background_color : "#32CCFE")
        .arg(!_selected_text_color.isEmpty() ? _selected_text_color : "#FFFFFF")
        .arg(!_placeholder_text_color.isEmpty() ? _placeholder_text_color : (isDarkMode ? "#757575" : "#ACABAB"))
        .arg(_left != 0 ? _left : (hasTextFieldIcon ? (12 + 20 + 12) - 3 : 12))
        .arg(_right != 0 ? _right : ((hasClearButton || hasPasswordButton) ? (24 + 28) - 9 : 12))
        .arg(_bottom != 0 ? _bottom : 2)
        .arg(_top != 0 ? _top : 0);

    setStyleSheet(styleSheet);
}
