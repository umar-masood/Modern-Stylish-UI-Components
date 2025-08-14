#include "Image.h"
#include <QPainter>
#include <QTransform>
#include <QPainterPath>

Image::Image(QWidget *parent) : QLabel(parent)
{
   setAttribute(Qt::WA_TranslucentBackground, true);
   setAttribute(Qt::WA_StyledBackground, false);

   scaled = false;
   scaledSize = QSize();
   aspectMode = Qt::KeepAspectRatio;
   transformMode = Qt::SmoothTransformation;
   rotationAngle = 0;
   flipH = false;
   flipV = false;
   opacity = 1.0;
   borderRadius = 0;

   setAlignment(Qt::AlignCenter);
}

void Image::setPath(const QString &filePath) { setPixmap(QPixmap(filePath)); }
void Image::setPixmap(const QPixmap &pixmap)
{
   originalPixmap = pixmap;
   updatePixmap();
}
void Image::setScaled(bool enable)
{
   scaled = enable;
   updatePixmap();
}
void Image::setScaled(int w, int h) { setScaled(QSize(w, h)); }
void Image::setScaled(const QSize &size)
{
   scaled = true;
   scaledSize = size;
   updatePixmap();
}
void Image::setTransform(qreal angle, bool h, bool v)
{
   rotationAngle = angle;
   flipH = h;
   flipV = v;
   updatePixmap();
}
void Image::setTransform(qreal angle, bool h, bool v, int w, int hsize)
{
   setTransform(angle, h, v);
   setScaled(w, hsize);
}
void Image::setAspectRatioMode(Qt::AspectRatioMode mode)
{
   aspectMode = mode;
   updatePixmap();
}
void Image::setTransformationMode(Qt::TransformationMode mode)
{
   transformMode = mode;
   updatePixmap();
}
void Image::setOpacity(qreal op)
{
   opacity = qBound(0.0, op, 1.0);
   updatePixmap();
}
void Image::setBorderRadius(int radius)
{
   borderRadius = radius;
   updatePixmap();
}
void Image::setAlignment(Qt::Alignment alignment) { QLabel::setAlignment(alignment); }

void Image::resizeEvent(QResizeEvent *event)
{
   QLabel::resizeEvent(event);
   if (scaled && scaledSize.isEmpty())
      updatePixmap();
}

void Image::updatePixmap()
{
   if (originalPixmap.isNull())
      return;

   QPixmap temp = originalPixmap;

   if (flipH || flipV)
      temp = temp.transformed(QTransform().scale(flipH ? -1 : 1, flipV ? -1 : 1));

   if (rotationAngle != 0)
      temp = temp.transformed(QTransform().rotate(rotationAngle));

   if (scaled)
   {
      QSize targetSize = scaledSize.isEmpty() ? size() : scaledSize;
      temp = temp.scaled(targetSize, aspectMode, transformMode);
   }

   if (opacity < 1.0)
   {
      QPixmap tempOpacity(temp.size());
      tempOpacity.fill(Qt::transparent);
      QPainter p(&tempOpacity);
      p.setOpacity(opacity);
      p.drawPixmap(0, 0, temp);
      p.end();
      temp = tempOpacity;
   }

   if (borderRadius > 0)
   {
      QPixmap rounded(temp.size());
      rounded.fill(Qt::transparent);
      QPainter p(&rounded);
      p.setRenderHint(QPainter::Antialiasing);
      QPainterPath path;
      path.addRoundedRect(temp.rect(), borderRadius, borderRadius);
      p.setClipPath(path);
      p.drawPixmap(0, 0, temp);
      p.end();
      temp = rounded;
   }

   displayedPixmap = temp;
   QLabel::setPixmap(displayedPixmap);
}
