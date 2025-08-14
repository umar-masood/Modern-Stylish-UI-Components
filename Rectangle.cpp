#include "Rectangle.h"

Rectangle::Rectangle(QWidget *parent) : QWidget(parent)
{
   setAttribute(Qt::WA_StyledBackground, true);

   backgroundColor = "transparent";
   borderWidth = 0;
   borderStyle = "solid";
   outlineColor = "transparent";
   outlineWidth = 0;
   outlineStyle = "solid";
   topLeftRadius = 0;
   topRightRadius = 0;
   bottomLeftRadius = 0;
   bottomRightRadius = 0;
   updateStyleSheet();
}

void Rectangle::setBackgroundColor(const QString &color)
{
   backgroundColor = color;
   updateStyleSheet();
}

void Rectangle::setBorderColor(const QString &color)
{
   borderColor = color;
   updateStyleSheet();
}

void Rectangle::setBorderWidth(int width)
{
   borderWidth = width;
   updateStyleSheet();
}

void Rectangle::setBorderStyle(const QString &style)
{
   borderStyle = style;
   updateStyleSheet();
}

void Rectangle::setOutlineColor(const QString &color)
{
   outlineColor = color;
   updateStyleSheet();
}

void Rectangle::setOutlineWidth(int width)
{
   outlineWidth = width;
   updateStyleSheet();
}

void Rectangle::setOutlineStyle(const QString &style)
{
   outlineStyle = style;
   updateStyleSheet();
}

void Rectangle::setTopLeftRadius(int radius)
{
   topLeftRadius = radius;
   updateStyleSheet();
}

void Rectangle::setTopRightRadius(int radius)
{
   topRightRadius = radius;
   updateStyleSheet();
}

void Rectangle::setBottomLeftRadius(int radius)
{
   bottomLeftRadius = radius;
   updateStyleSheet();
}

void Rectangle::setBottomRightRadius(int radius)
{
   bottomRightRadius = radius;
   updateStyleSheet();
}

void Rectangle::setRadius(int topLeft, int topRight, int bottomLeft, int bottomRight)
{
   topLeftRadius = topLeft;
   topRightRadius = topRight;
   bottomLeftRadius = bottomLeft;
   bottomRightRadius = bottomRight;
   updateStyleSheet();
}

void Rectangle::updateStyleSheet() {
   QString style = QString(
     "background-color: %1;"
     "border: %2px %3 %4;"
     "outline: %5px %6 %7;"
     "border-top-left-radius: %8px;"
     "border-top-right-radius: %9px;"
     "border-bottom-left-radius: %10px;"
     "border-bottom-right-radius: %11px;"
   )
   .arg(backgroundColor)
   .arg(borderWidth)
   .arg(borderStyle)
   .arg(borderColor)
   .arg(outlineWidth)
   .arg(outlineStyle)
   .arg(outlineColor)
   .arg(topLeftRadius)
   .arg(topRightRadius)
   .arg(bottomLeftRadius)
   .arg(bottomRightRadius);

   setStyleSheet(style);
}

