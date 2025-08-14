#ifndef TEXT_H
#define TEXT_H

#include <QLabel>
#include <QString>

class Text : public QLabel
{
   Q_OBJECT

public:
   explicit Text(QWidget *parent = nullptr);

   void setTextColor(const QString &color);
   void setBackgroundColor(const QString &color);
   void setFontSize(int size);
   void setFontFamily(const QString &family);
   void setFontBold(bool bold);
   void setFontItalic(bool italic);
   void setFontUnderline(bool underline);
   void setFontStrikeOut(bool strike);
   void setLetterSpacing(int spacing);
   void setWordSpacing(int spacing);
   void setTextAlignment(Qt::Alignment alignment);
   void setWordWrap(bool wrap);

   void setFont(const QString &family, int size);
   void setFont(const QString &family, int size, bool bold);
   void setFont(const QString &family, int size, bool bold, bool italic);
   void setFont(const QString &family, int size, bool bold, bool italic, Qt::Alignment alignment);

   void setBorderColor(const QString &color);
   void setBorderWidth(int width);
   void setBorderStyle(const QString &style);
   void setOutlineColor(const QString &color);
   void setOutlineWidth(int width);
   void setOutlineStyle(const QString &style);
   void setTopLeftRadius(int radius);
   void setTopRightRadius(int radius);
   void setBottomLeftRadius(int radius);
   void setBottomRightRadius(int radius);
   void setRadius(int topLeft, int topRight, int bottomLeft, int bottomRight);

private:
   QString textColor;
   QString backgroundColor;
   int fontSize;
   QString fontFamily;
   bool fontBold;
   bool fontItalic;
   bool fontUnderline;
   bool fontStrikeOut;
   int letterSpacing;
   int wordSpacing;
   Qt::Alignment textAlignment;
   bool wordWrap;

   QString borderColor;
   int borderWidth;
   QString borderStyle;
   QString outlineColor;
   int outlineWidth;
   QString outlineStyle;
   int topLeftRadius;
   int topRightRadius;
   int bottomLeftRadius;
   int bottomRightRadius;

   void updateStyle();
   void updateFont();
};

#endif 
