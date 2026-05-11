#ifndef BUTTONANIMATION_H
#define BUTTONANIMATION_H

#include <QPushButton>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QGraphicsDropShadowEffect>
#include <QEasingCurve>

class ButtonAnimationHelper
{
public:
    static void setupLuxuryButtonAnimation(QPushButton* button)
    {
        if (!button) return;

        if (button->property("luxuryAnimReady").toBool()) {
            return;
        }
        button->setProperty("luxuryAnimReady", true);

        // Ajouter un effet d'ombre pour le style luxe
        QGraphicsDropShadowEffect* shadow = qobject_cast<QGraphicsDropShadowEffect*>(button->graphicsEffect());
        if (!shadow) {
            shadow = new QGraphicsDropShadowEffect(button);
            button->setGraphicsEffect(shadow);
        }
        shadow->setBlurRadius(8);
        shadow->setColor(QColor(0, 0, 0, 80));
        shadow->setOffset(0, 4);

        // Connecter le clic pour l'animation
        QObject::connect(button, &QPushButton::clicked, [button]() {
            animateLuxuryButtonClick(button);
        });
    }

private:
    static void animateLuxuryButtonClick(QPushButton* button)
    {
        if (!button) {
            return;
        }

        // Utiliser l'ombre portée plutôt que la géométrie pour rester compatible
        // avec les layouts (la géométrie est souvent réécrite instantanément).
        QGraphicsDropShadowEffect* shadow = qobject_cast<QGraphicsDropShadowEffect*>(button->graphicsEffect());
        if (!shadow) {
            shadow = new QGraphicsDropShadowEffect(button);
            shadow->setColor(QColor(0, 0, 0, 80));
            shadow->setOffset(0, 4);
            shadow->setBlurRadius(8);
            button->setGraphicsEffect(shadow);
        }

        const qreal baseBlur = shadow->blurRadius();
        const qreal baseYOffset = shadow->yOffset();

        QPropertyAnimation* blurUp = new QPropertyAnimation(shadow, "blurRadius", button);
        blurUp->setDuration(90);
        blurUp->setStartValue(baseBlur);
        blurUp->setEndValue(baseBlur + 8.0);
        blurUp->setEasingCurve(QEasingCurve::OutCubic);

        QPropertyAnimation* offsetUp = new QPropertyAnimation(shadow, "yOffset", button);
        offsetUp->setDuration(90);
        offsetUp->setStartValue(baseYOffset);
        offsetUp->setEndValue(baseYOffset + 2.0);
        offsetUp->setEasingCurve(QEasingCurve::OutCubic);

        QParallelAnimationGroup* upGroup = new QParallelAnimationGroup(button);
        upGroup->addAnimation(blurUp);
        upGroup->addAnimation(offsetUp);

        QPropertyAnimation* blurDown = new QPropertyAnimation(shadow, "blurRadius", button);
        blurDown->setDuration(130);
        blurDown->setStartValue(baseBlur + 8.0);
        blurDown->setEndValue(baseBlur);
        blurDown->setEasingCurve(QEasingCurve::InOutCubic);

        QPropertyAnimation* offsetDown = new QPropertyAnimation(shadow, "yOffset", button);
        offsetDown->setDuration(130);
        offsetDown->setStartValue(baseYOffset + 2.0);
        offsetDown->setEndValue(baseYOffset);
        offsetDown->setEasingCurve(QEasingCurve::InOutCubic);

        QParallelAnimationGroup* downGroup = new QParallelAnimationGroup(button);
        downGroup->addAnimation(blurDown);
        downGroup->addAnimation(offsetDown);

        QSequentialAnimationGroup* group = new QSequentialAnimationGroup(button);
        group->addAnimation(upGroup);
        group->addAnimation(downGroup);
        QObject::connect(group, &QSequentialAnimationGroup::finished, [group]() {
            group->deleteLater();
        });
        group->start();
    }
};

#endif // BUTTONANIMATION_H
