#ifndef ANIMATIONUTILS_H
#define ANIMATIONUTILS_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>
#include <QEasingCurve>

class AnimationUtils {
public:
    // Fade in animation - Luxury style with smooth easing
    static QPropertyAnimation* createFadeInAnimation(QWidget* widget, int duration = 700) {
        QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(widget);
        effect->setOpacity(0);
        widget->setGraphicsEffect(effect);
        
        QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity", widget);
        animation->setDuration(duration);
        animation->setStartValue(0.0);
        animation->setEndValue(1.0);
        animation->setEasingCurve(QEasingCurve::OutExpo);
        
        return animation;
    }
    
    // Fade out animation - Luxury style
    static QPropertyAnimation* createFadeOutAnimation(QWidget* widget, int duration = 700) {
        QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(widget);
        effect->setOpacity(1);
        widget->setGraphicsEffect(effect);
        
        QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity", widget);
        animation->setDuration(duration);
        animation->setStartValue(1.0);
        animation->setEndValue(0.0);
        animation->setEasingCurve(QEasingCurve::InExpo);
        
        return animation;
    }
    
    // Slide in animation from left with luxury easing
    static QPropertyAnimation* createSlideInLeftAnimation(QWidget* widget, int duration = 600) {
        int startX = -widget->width();
        int endX = widget->x();
        
        QPropertyAnimation* animation = new QPropertyAnimation(widget, "pos", widget);
        animation->setDuration(duration);
        animation->setStartValue(QPoint(startX, widget->y()));
        animation->setEndValue(QPoint(endX, widget->y()));
        animation->setEasingCurve(QEasingCurve::OutElastic);
        
        return animation;
    }
    
    // Slide in animation from right with luxury easing
    static QPropertyAnimation* createSlideInRightAnimation(QWidget* widget, int duration = 600) {
        int startX = widget->parentWidget()->width();
        int endX = widget->x();
        
        QPropertyAnimation* animation = new QPropertyAnimation(widget, "pos", widget);
        animation->setDuration(duration);
        animation->setStartValue(QPoint(startX, widget->y()));
        animation->setEndValue(QPoint(endX, widget->y()));
        animation->setEasingCurve(QEasingCurve::OutElastic);
        
        return animation;
    }
    
    // Sophisticated pulse animation with scale and opacity
    static QSequentialAnimationGroup* createPulseAnimation(QWidget* widget, int duration = 1200) {
        QSequentialAnimationGroup* group = new QSequentialAnimationGroup(widget);
        
        // Phase 1: Gentle scale up with light effect
        QPropertyAnimation* scaleUp = new QPropertyAnimation(widget, "geometry", widget);
        scaleUp->setDuration(duration / 3);
        scaleUp->setStartValue(widget->geometry());
        QRect scaledRect = widget->geometry();
        scaledRect.setWidth(scaledRect.width() * 1.08);
        scaledRect.setHeight(scaledRect.height() * 1.08);
        scaledRect.moveCenter(widget->geometry().center());
        scaleUp->setEndValue(scaledRect);
        scaleUp->setEasingCurve(QEasingCurve::OutCubic);
        
        // Phase 2: Hold position
        QPropertyAnimation* hold = new QPropertyAnimation(widget, "geometry", widget);
        hold->setDuration(duration / 3);
        hold->setStartValue(scaledRect);
        hold->setEndValue(scaledRect);
        
        // Phase 3: Scale down smoothly
        QPropertyAnimation* scaleDown = new QPropertyAnimation(widget, "geometry", widget);
        scaleDown->setDuration(duration / 3);
        scaleDown->setStartValue(scaledRect);
        scaleDown->setEndValue(widget->geometry());
        scaleDown->setEasingCurve(QEasingCurve::InCubic);
        
        group->addAnimation(scaleUp);
        group->addAnimation(hold);
        group->addAnimation(scaleDown);
        
        return group;
    }
    
    // Luxury glow effect - animates between normal and highlighted state
    static QPropertyAnimation* createGlowAnimation(QWidget* widget, int duration = 800) {
        QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(widget);
        effect->setOpacity(1.0);
        widget->setGraphicsEffect(effect);
        
        QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity", widget);
        animation->setDuration(duration);
        animation->setStartValue(1.0);
        animation->setEndValue(0.8);
        animation->setEasingCurve(QEasingCurve::InOutSine);
        
        return animation;
    }
    
    // Smooth color animation for backgrounds
    static QPropertyAnimation* createColorAnimation(QWidget* widget, 
                                                   const QColor& startColor,
                                                   const QColor& endColor,
                                                   int duration = 800) {
        QPropertyAnimation* animation = new QPropertyAnimation(widget, "styleSheet", widget);
        animation->setDuration(duration);
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        
        return animation;
    }
    
    // Get luxury color scheme matching project theme
    static QString getLuxuryColorStyle(const QString& baseColor = "#c68e65") {
        return QString(
            "QFrame{"
            "background:qlineargradient(x1:0,y1:0,x2:1,y2:1,"
            "stop:0 %1,stop:0.5 %2,stop:1 %3);"
            "border:2px solid %4;"
            "border-radius:16px;"
            "padding:18px;"
            "box-shadow: 0px 8px 24px rgba(0,0,0,0.12);"
            "}"
            "QLabel{color:#4a3528;font-weight:500;}"
        ).arg(baseColor, "#d4a574", "#8b5e3b", "#a26f4f");
    }
};

#endif // ANIMATIONUTILS_H
