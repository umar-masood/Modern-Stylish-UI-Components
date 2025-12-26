#include <QGraphicsDropShadowEffect>
#include <QWidget>

class SmoothDropShadow : public QGraphicsDropShadowEffect {
   Q_OBJECT
   Q_PROPERTY(qreal blurRadius READ get_blur_radius WRITE set_blur_radius)
 
   public:
   explicit SmoothDropShadow(QWidget *parent = nullptr);
   void set_blur_radius(qreal br);
   qreal get_blur_radius() const;
 
   private:
   qreal m_radius;
};
