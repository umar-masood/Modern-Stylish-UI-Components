#pragma once
#include <QLabel>
#include <QPixmap>
#include <QSize>
#include <QTransform>
#include <QPainter>

class Image : public QLabel
{
   Q_OBJECT

public:
   explicit Image(QWidget *parent = nullptr);

   void setPath(const QString &filePath);
   void setPixmap(const QPixmap &pixmap);

   void setScaled(bool enable);
   void setScaled(int w, int h);
   void setScaled(const QSize &size);

   void setTransform(qreal angle, bool h = false, bool v = false);
   void setTransform(qreal angle, bool h, bool v, int w, int hsize);

   void setAspectRatioMode(Qt::AspectRatioMode mode);
   void setTransformationMode(Qt::TransformationMode mode);
   void setOpacity(qreal op);

   void setBorderRadius(int radius);

   void setAlignment(Qt::Alignment alignment);

protected:
   void resizeEvent(QResizeEvent *event) override;

private:
   void updatePixmap();

   QPixmap originalPixmap;
   QPixmap displayedPixmap;

   bool scaled;
   QSize scaledSize;
   Qt::AspectRatioMode aspectMode;
   Qt::TransformationMode transformMode;

   qreal rotationAngle;
   bool flipH;
   bool flipV;
   qreal opacity;

   int borderRadius;
};
