#include "Dialog.h"

Dialog::Dialog(QWidget *parent) : RoundedBox("", parent)
{
   setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::NoDropShadowWindowHint);
   setWindowModality(Qt::ApplicationModal);
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

void Dialog::setUninteractiveWidgets(QWidget *mainContent, QWidget *titleBar)
{
   contentWidget = mainContent;
   titleBarWidget = titleBar;
}

void Dialog::centerInParent()
{
   if (parentWidget())
   {
      QScreen *screen = QApplication::screenAt(QCursor::pos());
      if (!screen)
         screen = QApplication::primaryScreen();

      int x = (parentWidget()->width() - width()) / 2;
      int y = (parentWidget()->height() - height()) / 2;
      move(x, y);
   }
}

void Dialog::disableWidgets()
{
   if (contentWidget) contentWidget->setEnabled(false);
   if (titleBarWidget) titleBarWidget->setEnabled(false);
}


void Dialog::enableWidgets()
{ 
   if (contentWidget) contentWidget->setEnabled(true);
   if (titleBarWidget) titleBarWidget->setEnabled(true);
}

void Dialog::showEvent(QShowEvent *event)
{
   if (!setupDone)
   {
      setup();
      setupDone = true;
   }
   disableWidgets();
   centerInParent();
   RoundedBox::showEvent(event);
}

void Dialog::resizeEvent(QResizeEvent *event)
{
   centerInParent();
   RoundedBox::resizeEvent(event);
}

void Dialog::closeEvent(QCloseEvent *event)
{
   enableWidgets();
   centerInParent();
   RoundedBox::closeEvent(event);
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