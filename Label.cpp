#include "Label.h"

/* Custom Text Label (With and Without Icon)*/
Label::Label(bool isIconic, 
            const QString &family, 
            int pointSize, 
            QFont::Weight weight, 
            bool italic, 
            const QString &text, 
            Qt::Alignment alignment, 
            QWidget *parent) : QLabel(parent) {

   setAttribute(Qt::WA_TranslucentBackground);
   setAlignment(alignment);

   // If not iconic, set the font and text
   if (!isIconic) {
      setText(text);

      QFont fnt;
      fnt.setPointSize(pointSize);
      fnt.setFamily(family);
      fnt.setWeight(weight);
      fnt.setItalic(italic);
      setFont(fnt);
   }
}

AnimatedLabel::AnimatedLabel(bool isIconic, 
            const QString &family, 
            int pointSize, 
            QFont::Weight weight, 
            bool italic, 
            const QString &text, 
            Qt::Alignment alignment, 
            QWidget *parent) : QLabel(parent) {

   setAttribute(Qt::WA_TranslucentBackground);
   setAlignment(alignment);

   // If not iconic, set the font and text
   if (!isIconic) {
      QFont fnt;
      fnt.setPointSize(pointSize);
      fnt.setFamily(family);
      fnt.setWeight(weight);
      fnt.setItalic(italic);
      setFont(fnt); 
      setText(text);
   }

   // Opacity effect
   effect = new SmoothOpacity(this);
   effect->setOpacity(0.0);
   setGraphicsEffect(effect);

   // Fade In Animation
   fadeIn = new QPropertyAnimation(effect, "opacity", this);
   fadeIn->setStartValue(0.0);
   fadeIn->setEndValue(1.0);
   fadeIn->setDuration(500);
   fadeIn->setEasingCurve(QEasingCurve::InOutQuad);

   // Fade Out Animation
   fadeOut = new QPropertyAnimation(effect, "opacity", this);
   fadeOut->setStartValue(1.0);
   fadeOut->setEndValue(0.0);
   fadeOut->setDuration(500);
   fadeOut->setEasingCurve(QEasingCurve::InOutQuad);

   connect(fadeOut, &QPropertyAnimation::finished, this, [this]() {
      QLabel::hide();
      setGraphicsEffect(nullptr);
      effect->setOpacity(1.0);
   });

}

void AnimatedLabel::show() {
   if (!isVisible()) {
      effect->setOpacity(0.0);
      QLabel::show();
   }

   fadeIn->start();
}

void AnimatedLabel::hide() {  
   fadeOut->start();
}

void AnimatedLabel::setAnimatedText(const QString &text) {
   hide();
   QTimer::singleShot(500, this, [this, text]() {
      QLabel::setText(text);
      show(); 
   });
}
