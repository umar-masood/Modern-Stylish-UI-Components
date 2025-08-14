#include "Text.h"
#include <QFont>

Text::Text(QWidget *parent) : QLabel(parent)
{
   setAttribute(Qt::WA_StyledBackground, true);
   setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

   textColor = "#000000";
   backgroundColor = "transparent";
   fontSize = 12;
   fontFamily = "Arial";
   fontBold = false;
   fontItalic = false;
   fontUnderline = false;
   fontStrikeOut = false;
   letterSpacing = 0;
   wordSpacing = 0;
   textAlignment = Qt::AlignLeft | Qt::AlignVCenter;
   wordWrap = false;

   borderColor = "transparent";
   borderWidth = 0;
   borderStyle = "solid";
   outlineColor = "transparent";
   outlineWidth = 0;
   outlineStyle = "solid";
   topLeftRadius = 0;
   topRightRadius = 0;
   bottomLeftRadius = 0;
   bottomRightRadius = 0;

   updateFont();
   updateStyle();
}

void Text::setTextColor(const QString &color)
{
   textColor = color;
   updateStyle();
}
void Text::setBackgroundColor(const QString &color)
{
   backgroundColor = color;
   updateStyle();
}
void Text::setFontSize(int size)
{
   fontSize = size;
   updateFont();
}
void Text::setFontFamily(const QString &family)
{
   fontFamily = family;
   updateFont();
}
void Text::setFontBold(bool bold)
{
   fontBold = bold;
   updateFont();
}
void Text::setFontItalic(bool italic)
{
   fontItalic = italic;
   updateFont();
}
void Text::setFontUnderline(bool underline)
{
   fontUnderline = underline;
   updateFont();
}
void Text::setFontStrikeOut(bool strike)
{
   fontStrikeOut = strike;
   updateFont();
}
void Text::setLetterSpacing(int spacing)
{
   letterSpacing = spacing;
   updateFont();
}
void Text::setWordSpacing(int spacing)
{
   wordSpacing = spacing;
   updateFont();
}
void Text::setTextAlignment(Qt::Alignment alignment)
{
   textAlignment = alignment;
   setAlignment(alignment);
}
void Text::setWordWrap(bool wrap)
{
   wordWrap = wrap;
   QLabel::setWordWrap(wrap);
}

void Text::setFont(const QString &family, int size)
{
   setFontFamily(family);
   setFontSize(size);
}

void Text::setFont(const QString &family, int size, bool bold)
{
   setFont(family, size);
   setFontBold(bold);
}

void Text::setFont(const QString &family, int size, bool bold, bool italic)
{
   setFont(family, size, bold);
   setFontItalic(italic);
}

void Text::setFont(const QString &family, int size, bool bold, bool italic, Qt::Alignment alignment)
{
   setFont(family, size, bold, italic);
   setTextAlignment(alignment);
}

void Text::setBorderColor(const QString &color)
{
   borderColor = color;
   updateStyle();
}
void Text::setBorderWidth(int width)
{
   borderWidth = width;
   updateStyle();
}
void Text::setBorderStyle(const QString &style)
{
   borderStyle = style;
   updateStyle();
}
void Text::setOutlineColor(const QString &color)
{
   outlineColor = color;
   updateStyle();
}
void Text::setOutlineWidth(int width)
{
   outlineWidth = width;
   updateStyle();
}
void Text::setOutlineStyle(const QString &style)
{
   outlineStyle = style;
   updateStyle();
}
void Text::setTopLeftRadius(int radius)
{
   topLeftRadius = radius;
   updateStyle();
}
void Text::setTopRightRadius(int radius)
{
   topRightRadius = radius;
   updateStyle();
}
void Text::setBottomLeftRadius(int radius)
{
   bottomLeftRadius = radius;
   updateStyle();
}
void Text::setBottomRightRadius(int radius)
{
   bottomRightRadius = radius;
   updateStyle();
}
void Text::setRadius(int topLeft, int topRight, int bottomLeft, int bottomRight)
{
   topLeftRadius = topLeft;
   topRightRadius = topRight;
   bottomLeftRadius = bottomLeft;
   bottomRightRadius = bottomRight;
   updateStyle();
}

void Text::updateFont()
{
   QFont font(fontFamily, fontSize);
   font.setBold(fontBold);
   font.setItalic(fontItalic);
   font.setUnderline(fontUnderline);
   font.setStrikeOut(fontStrikeOut);
   font.setLetterSpacing(QFont::AbsoluteSpacing, letterSpacing);
   QLabel::setFont(font);
}

void Text::updateStyle()
{
   QString style = QString(
     "color: %1;"
     "background-color: %2;"
     "border: %3px %4 %5;"
     "outline: %6px %7 %8;"
     "border-top-left-radius: %9px;"
     "border-top-right-radius: %10px;"
     "border-bottom-left-radius: %11px;"
     "border-bottom-right-radius: %12px;"
   )
   .arg(textColor)
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
