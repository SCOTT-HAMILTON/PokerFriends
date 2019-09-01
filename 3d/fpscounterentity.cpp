#include "3d/fpscounterentity.h"

#include <QDebug>

FpsCounterEntity::FpsCounterEntity(Qt3DCore::QNode *parentNode) :
    Qt3DCore::QEntity (parentNode),
    frameCounter(0),frameCounterAvg(0),
    counter(0),fps(0),
    fpsAvg(0),rotation(0)
{

    rotationAnim = new QPropertyAnimation(this, "rotation");
    rotationAnim->setStartValue(0);
    rotationAnim->setEndValue(360);
    rotationAnim->setLoopCount(-1);
    rotationAnim->start();

    timer.setInterval(2000);
    timer.setSingleShot(false);

    QObject::connect(&timer, &QTimer::timeout, this, &FpsCounterEntity::updateFps);

    timer.start();
}

float FpsCounterEntity::getRotation() const
{
    return rotation;
}

void FpsCounterEntity::setRotation(float rot)
{
    ++frameCounter;
    rotation = rot;
}

void FpsCounterEntity::updateFps()
{
    frameCounterAvg += frameCounter;
    fps = frameCounter/2;
    ++counter;
    frameCounter = 0;
    if (counter >= 3) {
        fpsAvg = frameCounterAvg/(2*counter);
        frameCounterAvg = 0;
        counter = 0;
    }
    qDebug() << "fps : " << fps;
}

int FpsCounterEntity::getFps() const
{
    return fps;
}

