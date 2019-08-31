#ifndef SCENE3D_H
#define SCENE3D_H

#include <Qt3DCore/QEntity>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QCameraLens>
#include <Qt3DCore/QTransform>

#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include <QRenderSettings>
#include <QInputSettings>
#include <QSceneLoader>

#include "qt3dwindow.h"

#include <QWidget>
#include <QTimer>
#include <functional>
#include <array>

#include "3d/playerentity.h"
#include "3d/fpscounterentity.h"
#include "3d/discolightentity.h"
#include "3d/singlecardentity.h"

class Scene3D : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(float camOrbitAngle READ getCamOrbitAngle WRITE setCamOrbitAngle)

public:
    explicit Scene3D(const QSize& size, QWidget *parent = nullptr);
    Qt3DCore::QEntity *createScene();
    void setCameraOrbitAnimation(QSequentialAnimationGroup* anim);
    float getCamOrbitAngle() const{return camOrbitAngle;}
    void setCamOrbitAngle(float newangle);
    QMatrix4x4 calculateCameraTransformMatrix() const noexcept;

signals:

public slots:
    void movePlayer(QVector3D vec, size_t player);
    void rotatePlayer(float angle, size_t player);
    void startCamOrbitAnim();
    void startPlayerEntryAnim(size_t player);
    void startPlayersEntryAnim();
    void startCardsDistributionAnim();
    void initCardsDeckMaterial();
    void disableAllPlayers();
    void disablePlayer(size_t player);
    void enableAllPlayers();
    void enablePlayer(size_t player);

private slots:
    void animateNextCardDistribution();

private:
    float camOrbitAngle;

    Qt3DExtras::Qt3DWindow* view;
    QWidget *container;
    Qt3DCore::QEntity* scene;

    Qt3DRender::QCamera* camera;


    Qt3DRender::QCameraLens* camLens;
    Qt3DCore::QTransform* camTransform;

    QSequentialAnimationGroup cameraOrbitAnim;
    Qt3DCore::QTransform *lightTransform;

    Qt3DRender::QSceneLoader* pokerTableLoader;
    std::array<PlayerEntity*, 7> players;

    std::array<SingleCardEntity*, 7> singleCards;

    QTimer cardsDistribAnim;
    size_t currentDistributedCard;

    QTimer cardsDistributionDelay;

    DiscoLightEntity* walllightEntity;
    FpsCounterEntity *fpsCounter;
};

#endif // SCENE3D_H
