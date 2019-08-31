#ifndef DISCOLIGHTENTITY_H
#define DISCOLIGHTENTITY_H

#include <QEntity>
#include <QPointLight>
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include <QTransform>

class DiscoLightEntity : public Qt3DCore::QEntity
{
    Q_OBJECT
    Q_PROPERTY(int hue READ getHue WRITE setHue)
public:
    explicit DiscoLightEntity(Qt3DCore::QNode *parentNode = nullptr);
    virtual ~DiscoLightEntity();

    int getHue() const{return hue;}
    void setHue(int newhue);

public slots:
    void reverseAnim();

private:
    Qt3DRender::QPointLight* discoLight;
    int hue;
    QSequentialAnimationGroup colorAnimSeq;
    QPropertyAnimation colorAnim1;
    QPropertyAnimation colorAnim2;

    Qt3DCore::QTransform *transform;
    int inverser;
};

#endif // DISCOLIGHTENTITY_H
