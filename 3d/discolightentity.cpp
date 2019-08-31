#include "3d/discolightentity.h"

DiscoLightEntity::DiscoLightEntity(Qt3DCore::QNode *parentNode) :
    Qt3DCore::QEntity (parentNode), colorAnim1(this, "hue"),
    colorAnim2(this, "hue"),
    inverser(0)
{
    discoLight = new Qt3DRender::QPointLight(this);
    discoLight->setProperty("hue", 0.0f);
    discoLight->setColor(QColor::fromHsl(hue, 255, 127));
    discoLight->setIntensity(0.5);
    discoLight->setEnabled(true);

    addComponent(discoLight);

    transform = new Qt3DCore::QTransform(this);
    transform->setTranslation(QVector3D(-20, 30,-49));
    addComponent(transform);
    int max = 359;
    int min = 0;
    colorAnim1.setStartValue(min);
    colorAnim1.setEndValue(max);
    colorAnim1.setDuration(8000);
    colorAnim1.setLoopCount(1);

    colorAnim2.setStartValue(max);
    colorAnim2.setEndValue(min);
    colorAnim2.setDuration(8000);
    colorAnim2.setLoopCount(1);

    colorAnimSeq.addAnimation(&colorAnim1);
    colorAnimSeq.addAnimation(&colorAnim2);
    colorAnimSeq.setLoopCount(-1);
    colorAnimSeq.start();

}

DiscoLightEntity::~DiscoLightEntity()
{
}

void DiscoLightEntity::setHue(int newhue)
{
    hue = newhue;
    discoLight->setColor(QColor::fromHsl(hue, 255, 127));
}

void DiscoLightEntity::reverseAnim()
{
    inverser = 255-inverser;
}
