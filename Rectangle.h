#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QWidget>
#include <QString>

class Rectangle : public QWidget
{
   Q_OBJECT

public:
   explicit Rectangle(QWidget *parent = nullptr);

   void setBackgroundColor(const QString &color);
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
   QString backgroundColor;
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

   void updateStyleSheet();
};

#endif
