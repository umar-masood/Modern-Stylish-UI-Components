#include "TextField.h"

TextField::TextField(const QString &text, QWidget *parent)
    : QLineEdit(text, parent) {
    init();
}

TextField::TextField(QWidget *parent)
    : QLineEdit(parent) {
    init();
}

void TextField::setShadow(bool value) {
    hasShadow = value;
}

void TextField::setDarkMode(bool value) {
    isDarkMode = value;
    updateStyle();
}

void TextField::setSize(QSize s) {
    const int fixedHeight = 36;
    const int minWidth = 200;

    int finalWidth = std::max(s.width(), minWidth);
    int finalHeight = fixedHeight;

    setFixedSize(finalWidth, finalHeight);
}


void TextField::setTextFieldIcon(bool value) {
    textFieldIcon = value;
    updateStyle();
}

void TextField::setTextFieldIconSize(QSize s) {
    if (textFieldIcon) textFieldIconSize = s;
}

void TextField::setIconPaths(const QString &light_icon_path, const QString &dark_icon_path) {
    if (textFieldIcon) {
        light_icon = light_icon_path;
        dark_icon = dark_icon_path;
    } else {
        light_icon.clear();
        dark_icon.clear();
    }
}

void TextField::setDropDownPadding(bool value) {
    dropDownPadding = value;
    updateStyle();
}

void TextField::setReadOnly(bool value)
{
    isReadOnly = value;
    QLineEdit::setReadOnly(value);
}

void TextField::setEnabled(bool value)
{
    isEnabled = value;
    QLineEdit::setEnabled(value);
}

void TextField::setClearButton(bool value) {
    clearButton = value;

    if (clearButton && !clear) {
        clear = new Button("", this);
        clear->setShadow(false);
        clear->setDisplayMode(Button::IconOnly);
        clear->setIconSize(this->textFieldIconSize);
        clear->setSize(QSize(28, 28));
        clear->setIconPaths(":/Assets/Icons/x.svg",":/Assets/Icons/x.svg");
        clear->setEnabled(isEnabled);

        int x = width() - (12 + clear->width()) + 3;
        int y = (height() - clear->height()) / 2;
        clear->move(x, y);
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
    passwordButton = value;

    if (passwordButton && !password && !clearButton) {
        password = new Button("", this);
        password->setShadow(false);
        password->setDisplayMode(Button::IconOnly);
        password->setIconSize(this->textFieldIconSize);
        password->setSize(QSize(28, 28));
        password->setEnabled(isEnabled);
        this->setEchoMode(QLineEdit::Password);

        int x = width() - (12 + password->width()) + 3;
        int y = (height() - password->height()) / 2;
        password->move(x, y);
        password->hide();

        password->setIconPaths(eye_icon_path, eye_icon_path);

        connect(password, &Button::pressed, this, [this]() {
            QTimer::singleShot(250, this, [this]() {
                isPasswordVisible = !isPasswordVisible;
                if (isPasswordVisible) {
                    password->setIconPaths(eyeclosed_icon_path, eyeclosed_icon_path);
                    this->setEchoMode(QLineEdit::Normal);
                } else {
                    password->setIconPaths(eye_icon_path, eye_icon_path);
                    this->setEchoMode(QLineEdit::Password);
                }
            });
        });

        connect(this, &QLineEdit::textChanged, this, [this](const QString &text) {
            password->setVisible(isFocused && !text.isEmpty());
            if (isPasswordVisible) {
                password->setIconPaths(eye_icon_path, eye_icon_path);
                this->setEchoMode(QLineEdit::Password);
                isPasswordVisible = false;
            }
        });

        updateStyle();
    }
    update();
}

void TextField::resizeEvent(QResizeEvent *event) {
    QLineEdit::resizeEvent(event);

    if (clear) {
        int x = width() - (12 + clear->width()) + 3;
        int y = (height() - clear->height()) / 2;
        clear->move(x, y);
    }

    if (password) {
        int x = width() - (12 + password->width()) + 3;
        int y = (height() - password->height()) / 2;
        password->move(x, y);
    }
}

void TextField::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

    QRect rec = rect().adjusted(1, 1, -1, -1);

    QPen pen;
    pen.setWidthF(isFocused ? 1.0 : 0.5);
    pen.setColor(isFocused ? QColor("#109AC7") : (isDarkMode ? QColor("#4D4D4D") : QColor("#CCCCCC")));
    pen.setStyle(Qt::SolidLine);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);

    QColor bg_color = (isFocused) ? (isDarkMode ? QColor("#1F1F1F") : QColor("#FFFFFF")) :
                        isHover ? (isDarkMode ? QColor("#323232") : QColor("#F6F6F6")) :
                        (isDarkMode ? QColor("#2D2D2D") : QColor("#FBFBFB"));

    painter.setBrush(bg_color);

    QPainterPath path;
    path.addRoundedRect(rec, 6, 6);
    painter.drawPath(path);

    if (textFieldIcon) {
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
    if (echoMode() == QLineEdit::Password) return;

    Menu *menu = new Menu(this);
    menu->setMaxVisibleItems(7);
    menu->setItemSize(QSize(220, 36));
    menu->setDarkMode(isDarkMode);
    menu->setIconic(true);

    const bool hasText = !this->text().isEmpty();
    const bool hasSelection = this->hasSelectedText();

    if (hasSelection) menu->addAction({ "Copy", false, ":/Assets/Icons/copy_light.svg", ":/Assets/Icons/copy.svg", "Ctrl + C" });
    if (hasSelection) menu->addAction({ "Cut", false, ":/Assets/Icons/cut_light.svg", ":/Assets/Icons/cut.svg", "Ctrl + X" });
    
    menu->addAction({ "Paste", false, ":/Assets/Icons/clipboard_light.svg", ":/Assets/Icons/clipboard.svg", "Ctrl + V" });
    
    if (hasSelection) menu->addAction({ "Delete", false, ":/Assets/Icons/delete_light.svg", ":/Assets/Icons/delete.svg", "Delete" });
    
    if (hasText && !hasSelection) menu->addAction({ "Select All", false, ":/Assets/Icons/select-all_light.svg", ":/Assets/Icons/select-all.svg", "Ctrl + A" });
    
    if (this->isUndoAvailable()) menu->addAction({ "Undo", false, ":/Assets/Icons/undo_light.svg", ":/Assets/Icons/undo.svg", "Ctrl + Z" });
    if (this->isRedoAvailable()) menu->addAction({ "Redo", false, ":/Assets/Icons/redo_light.svg", ":/Assets/Icons/redo.svg", "Ctrl + Y" });

    connect(menu, &Menu::itemClicked, this, [=]() {
        QString action = menu->clickedItemText();
        if (action == "Copy") this->copy();
        else if (action == "Cut") this->cut();
        else if (action == "Paste") this->paste();
        else if (action == "Delete") this->del();
        else if (action == "Select All") this->selectAll();
        else if (action == "Undo") this->undo();
        else if (action == "Redo") this->redo();
        else qDebug() << "Unknown context menu action:" << action;
    });

    menu->show();
    menu->move(event->globalPos());
}

void TextField::init() {
    setSize(QSize(0, 0));
    setFocusPolicy(Qt::TabFocus);
    setFocusPolicy(Qt::ClickFocus);
    updateStyle();
    
    effect = new SmoothDropShadow(this);
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
    if (clear) clear->setDarkMode(isDarkMode); 
    if (password) password->setDarkMode(isDarkMode);
    QString txt_color = isDarkMode ? "#FFFFFF" : "#000000";
    QString selection_bg = "#32CCFE";
    QString selected_txt = "#FFFFFF";
    QString placeholder_color = isDarkMode ? "#757575" : "#ACABAB";
    int padding_left = textFieldIcon ? (12 + 20 + 12) - 3 : 12;
    int padding_right = (clearButton || passwordButton || dropDownPadding) ? (24 + 28) - 9 : 12;

    QString styleSheet = QString(R"(
      QLineEdit {
        font-family: Segoe UI;
        font-size: 11pt;
        background-color: transparent;
        border-radius: 0px;
        border: none;
        outline: none;
        color: %1;
        selection-background-color: %2;
        selection-color: %3;
        placeholder-text-color: %4;
        padding-left: %5px;
        padding-right: %6px;
      })")
        .arg(txt_color)
        .arg(selection_bg)
        .arg(selected_txt)
        .arg(placeholder_color)
        .arg(padding_left)
        .arg(padding_right);

    setStyleSheet(styleSheet);
}
