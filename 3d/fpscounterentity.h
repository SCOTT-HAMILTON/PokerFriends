#ifndef FPSCOUNTERENTITY_H
#define FPSCOUNTERENTITY_H

#include <QEntity>
#include <QPropertyAnimation>
#include <QTimer>

class FpsCounterEntity : public Qt3DCore::QEntity
{
    Q_OBJECT
    Q_PROPERTY(float rotation READ getRotation WRITE setRotation)
public:
    explicit FpsCounterEntity(Qt3DCore::QNode *parentNode = nullptr);
    float getRotation() const;
    void setRotation(float rot);
    void updateFps();
    int getFps() const;

private:
    int frameCounter;
    int frameCounterAvg;
    int counter;
    int fps;
    int fpsAvg;

    QPropertyAnimation *rotationAnim;
    float rotation;

    QTimer timer;
};

#endif // FPSCOUNTERENTITY_H
