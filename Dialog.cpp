#include "Dialog.h"

Overlay::Overlay(QWidget *parent) : QWidget(parent) {
   setWindowFlags(Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
   setAttribute(Qt::WA_TranslucentBackground);
   setAttribute(Qt::WA_TransparentForMouseEvents, false);
}

void Overlay::paintEvent(QPaintEvent *event)
{
   Q_UNUSED(event);

   // Colors
   QColor BG = QColor(0, 0, 0, 120);

   QPainter painter(this);
   painter.setRenderHints(QPainter::Antialiasing);
   painter.setBrush(BG);
   painter.setPen(Qt::NoPen);

   QPainterPath path;
   path.addRoundedRect(rect().adjusted(1.5, 1.5, -1.5, -1.5), 6, 6);
   painter.drawPath(path);
}

Dialog::Dialog(QWidget *parent) : RoundedBox("", parent)
{
   setWindowFlag(Qt::WindowStaysOnTopHint);
   setWindowModality(Qt::ApplicationModal);

   if (parent) {
      overlay = new Overlay(parent);
      overlay->setGeometry(parent->rect());
      overlay->hide();
      parent->installEventFilter(this); 
   }
}

void Dialog::setText(const QString &text)
{
   dialogText = text;
}

void Dialog::setButtonText(const QString &text)
{
   btnText = text;
}

void Dialog::setIcon(const QString &iconLight, const QString &iconDark)
{
   iconLightPath = iconLight;
   iconDarkPath = iconDark;
}

void Dialog::setIconSize(QSize s)
{
   iconSize = s;
}

void Dialog::setSize(QSize s)
{
   QSize minimumSize(340, 200);
   setFixedSize(s.expandedTo(minimumSize));
}

QPixmap Dialog::pixmap(const QString &iconLight, const QString &iconDark, QSize s)
{
   return QPixmap(iconLight).scaled(s, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
}

void Dialog::setDarkMode(bool value)
{
   isDarkMode = value;
   applyStyleSheet();
   RoundedBox::setDarkMode(value);
}

void Dialog::centerInParent()
{
   if (parentWidget())
   {
      QScreen *screen = QApplication::screenAt(QCursor::pos());
      if (!screen)
         screen = QApplication::primaryScreen();

        QRect parentRect = parentWidget()->geometry();
        int x = parentRect.x() + (parentRect.width() - width()) / 2;
        int y = parentRect.y() + (parentRect.height() - height()) / 2;
        move(x, y);
   }
}

void Dialog::showEvent(QShowEvent *event)
{
   if (!setupDone)
   {
      setup();
      setupDone = true;
   }

   if (overlay)
   {
      overlay->setGeometry(parentWidget()->rect());
      overlay->show();
      overlay->raise();
   }

   centerInParent();
   this->raise();
   RoundedBox::showEvent(event);
}

void Dialog::resizeEvent(QResizeEvent *event)
{
   if (overlay) overlay->setGeometry(parentWidget()->rect());
   RoundedBox::resizeEvent(event);
}

void Dialog::closeEvent(QCloseEvent *event)
{
   if (overlay) overlay->hide();
   RoundedBox::closeEvent(event);
}

bool Dialog::eventFilter(QObject *obj, QEvent *event)
{
   if (obj == parentWidget() && overlay)
      if (event->type() == QEvent::Resize || event->type() == QEvent::Move)
          overlay->setGeometry(parentWidget()->rect());
   
   return RoundedBox::eventFilter(obj, event);
}

void Dialog::setup()
{
   layout = new QVBoxLayout;
   layout->setContentsMargins(10, 10, 10, 10);
   layout->setSpacing(0);

   label = new QLabel(dialogText);
   QFont font("Outfit", 12);
   font.setStyleName("SemiBold");
   label->setFont(font);
   label->setWordWrap(true);
   label->setMinimumWidth(340);
   label->setAlignment(Qt::AlignCenter);
   dialogIcon = new QLabel;
   dialogIcon->setPixmap(pixmap(iconLightPath, iconDarkPath, iconSize));

   Button *btn = new Button;
   btn->setSize(QSize(320, 36));
   btn->setDarkMode(true);
   btn->setShadow(true);
   btn->setText(btnText);
   btn->setDisplayMode(Button::TextOnly);
   connect(btn, &Button::clicked, this, [this]() {  
      emit onDialogButtonClicked();
      close(); 
   });

   layout->addStretch();
   layout->addWidget(dialogIcon, 0, Qt::AlignCenter);
   layout->addStretch();
   layout->addWidget(label, 0, Qt::AlignCenter);
   layout->addStretch();
   layout->addWidget(btn, 0, Qt::AlignHCenter);

   setLayout(layout);
   applyStyleSheet();
}

void Dialog::applyStyleSheet() {
   if (label) label->setStyleSheet(QString("background-color: transparent; color: %1;").arg(isDarkMode ? "#CCCCCC" : "#2B2B2B"));
}
