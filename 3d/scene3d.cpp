#include "3d/scene3d.h"

#include <QDirectionalLight>
#include <QPropertyAnimation>
#include <string>
#include <Qt3DExtras/QTextureMaterial>
#include <QTexture>

Scene3D::Scene3D(const QSize& size, QWidget *parent) : QWidget(parent),
    camOrbitAngle(10),currentDistributedCard(0)
{
    view = new Qt3DExtras::Qt3DWindow;

    scene = createScene();

    // Camera
    camera = view->camera();
    camera->setPosition(QVector3D(0.0f, 42.7695f, 38.9287f ));
    camera->setViewCenter(QVector3D(2.24804f, -17.2143f, 19.375f));

    camLens = new Qt3DRender::QCameraLens(camera);
    camLens->setAspectRatio(4.0f/4.0f);
    camLens->setProjectionType(Qt3DRender::QCameraLens::PerspectiveProjection);
    camLens->setFieldOfView(80.0f);
    camLens->setNearPlane(0.1f);
    camLens->setFarPlane(1000.0f);
    camera->addComponent(camLens);

    camTransform = new Qt3DCore::QTransform(camera);
    camTransform->setMatrix(calculateCameraTransformMatrix());
    camera->addComponent(camTransform);
    {
        setCameraOrbitAnimation(&cameraOrbitAnim);
//        startCamOrbitAnim();
//        startPlayersEntryAnim();
//        startCardsDistributionAnim();
    }
    //SceneRoot Components
    Qt3DInput::QInputSettings* inputSettings = new Qt3DInput::QInputSettings(scene);
    scene->addComponent(inputSettings);
    pokerTableLoader = new Qt3DRender::QSceneLoader(scene);
    connect(pokerTableLoader, &Qt3DRender::QSceneLoader::statusChanged,
            this, &Scene3D::initCardsDeckMaterial);
    pokerTableLoader->setSource(QUrl("qrc:/asset/poker_table.gltf"));
    scene->addComponent(pokerTableLoader);

    //    auto cardsMaterial = new Qt3DExtras::QTextureMaterial;

    container = QWidget::createWindowContainer(view, this);
    container->resize(size);

    //TODO temporary, just for testing
    cardsDistributionDelay.setInterval(5000);
    cardsDistributionDelay.setSingleShot(true);
    connect(&cardsDistributionDelay, &QTimer::timeout, this, &Scene3D::startCardsDistributionAnim);

    view->setRootEntity(scene);
}

Qt3DCore::QEntity *Scene3D::createScene()
{


    // Root entity
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity;

    // Main light
    QVector3D worldDir(QVector3D(180,50,-30));
    QVector3D lightTrans(-20, 50, 49);
    {
        Qt3DCore::QEntity *pokerTableSpotlightEntity = new Qt3DCore::QEntity(rootEntity);
        Qt3DRender::QDirectionalLight* directionalLight = new Qt3DRender::QDirectionalLight(pokerTableSpotlightEntity);
        directionalLight->setColor(QColor(255, 255, 255, 255));
        directionalLight->setIntensity(2.0f);
        directionalLight->setEnabled(true);
        directionalLight->setWorldDirection(worldDir);
        pokerTableSpotlightEntity->addComponent(directionalLight);
        lightTransform = new Qt3DCore::QTransform(pokerTableSpotlightEntity);
        lightTransform->setTranslation(lightTrans);
        pokerTableSpotlightEntity->addComponent(lightTransform);
    }
    {
        Qt3DCore::QEntity *pokerTableSpotlightEntity = new Qt3DCore::QEntity(rootEntity);
        Qt3DRender::QDirectionalLight* directionalLight = new Qt3DRender::QDirectionalLight(pokerTableSpotlightEntity);
        directionalLight->setColor(QColor(255, 255, 255, 255));
        directionalLight->setIntensity(2.0f);
        directionalLight->setEnabled(true);
        directionalLight->setWorldDirection(QVector3D(180,50,30));
        pokerTableSpotlightEntity->addComponent(directionalLight);
        lightTransform = new Qt3DCore::QTransform(pokerTableSpotlightEntity);
        lightTransform->setTranslation(QVector3D(20, 70, -31));
        pokerTableSpotlightEntity->addComponent(lightTransform);
    }

    // Disco light
    walllightEntity = new DiscoLightEntity(rootEntity);

    // Players
    players[0] = new PlayerEntity(
                QVector3D(-20.0f, 10.0f, -106.0f),
                QVector3D(-5.0, 10.0, -1.0),
                116.0f,
                rootEntity
    );
    players[1] = new PlayerEntity(
                QVector3D(30, 10, -115),
                QVector3D(12.1143f, 10, -3.94286f),
//                lightTrans,
//                QVector3D(5,50,19),
                63.0f,
                rootEntity
    );
//    players[1]->setRotation(worldDir);
    players[2] = new PlayerEntity(
                QVector3D(-35, 10, -76),
                QVector3D(-12.8875f, 10, 4),
                141,
                rootEntity
    );

    players[3] = new PlayerEntity(
                QVector3D(50, 10, -85),
                QVector3D(17.6571f, 10, 1.28571f),
                28,
                rootEntity
    );
    players[4] = new PlayerEntity(
                QVector3D(-50, 10, -66),
                QVector3D(-16.2304f, 10, 12.2607f),
                175,
                rootEntity
    );
    players[5] = new PlayerEntity(
                QVector3D(65, 10, -70),
                QVector3D(19.1607f, 10, 10.9893f),
                178,
                rootEntity
    );
    players[6] = new PlayerEntity(
                QVector3D(-60, 10, -51),
                QVector3D(-15.3446f, 10, 20.2286f),
//                lightTransform->translation(),
                34,
                rootEntity
    );

    singleCards[0] = new SingleCardEntity(QVector3D(3, 21.6f, 22.5f),
                                            QVector3D(-0.25, 21.6f, 11.6875f), 0, rootEntity);

    singleCards[1] = new SingleCardEntity(QVector3D(3, 21.6f, 22.5f),
                                            QVector3D(7.5625f, 21.6f, 12.75f), 0, rootEntity);

    singleCards[2] = new SingleCardEntity(QVector3D(3, 21.6f, 22.5f),
                                            QVector3D(-4.5625f, 21.6f, 15.3125f), 0, rootEntity);

    singleCards[3] = new SingleCardEntity(QVector3D(3, 21.6f, 22.5f),
                                            QVector3D(11, 21.6f, 15.75f), 0, rootEntity);

    singleCards[4] = new SingleCardEntity(QVector3D(3, 21.6f, 22.5f),
                                            QVector3D(-6, 21.6f, 20.5625f), 0, rootEntity);

    singleCards[5] = new SingleCardEntity(QVector3D(3, 21.6f, 22.5f),
                                            QVector3D(11.5f, 21.6f, 20.8125f), 0, rootEntity);

    singleCards[6] = new SingleCardEntity(QVector3D(3, 21.6f, 22.5f),
                                            QVector3D(-3.9375f, 21.6f, 25.375f), 0, rootEntity);

    cardsDistribAnim.setInterval(200);
    cardsDistribAnim.setTimerType(Qt::TimerType::CoarseTimer);
    cardsDistribAnim.setSingleShot(false);
    currentDistributedCard = 0;
    connect(&cardsDistribAnim, &QTimer::timeout, this, &Scene3D::animateNextCardDistribution);

    fpsCounter = new FpsCounterEntity(rootEntity);

    return rootEntity;
}

void Scene3D::setCameraOrbitAnimation(QSequentialAnimationGroup* anim)
{
    auto anim1 = new QPropertyAnimation(this, "camOrbitAngle");
    anim1->setEndValue(360);
    anim1->setDuration(3000);
    anim1->setEasingCurve(QEasingCurve::InOutBack);
    auto anim2 = new QPropertyAnimation(this, "camOrbitAngle");
    anim2->setEndValue(370);
    anim2->setDuration(1000);
    anim2->setEasingCurve(QEasingCurve::InOutCirc);
    anim->addAnimation(anim1);
    anim->addAnimation(anim2);
}

void Scene3D::setCamOrbitAngle(float newangle)
{
    camOrbitAngle = newangle;
    camTransform->setMatrix(calculateCameraTransformMatrix());
}

QMatrix4x4 Scene3D::calculateCameraTransformMatrix() const noexcept
{
    QMatrix4x4 matrix;
    QVector3D center(0.230118f, 20.7431f, 20.7023f);
    QVector3D vec(center.x(), center.y()+10, center.z()+9);
    matrix.rotate(camOrbitAngle, QVector3D(0,1,0));
    matrix.translate(QVector3D(vec.x(), vec.y(), vec.z()+10));
    return matrix;
}

void Scene3D::movePlayer(QVector3D vec, size_t player)
{

//    lightTransform->setTranslation(lightTransform->translation()+vec);
    Q_ASSERT_X(player>0 && player < players.size(),
               "Scene3D::movePlayer",
               QString(
                   "Unknown player number : "+
                   QString::number(player)).toStdString().c_str());
    players[player]->move(vec);
}

void Scene3D::rotatePlayer(float angle, size_t player)
{
//    player = -1;
    Q_ASSERT_X(player>0 && player < players.size(),
               "Scene3D::rotatePlayer",
               QString(
                   "Unknown player number : "+
                   QString::number(player)).toStdString().c_str());
    players[player]->rotate(angle);
}

void Scene3D::startCamOrbitAnim()
{
    if (camOrbitAngle>360) camOrbitAngle-=360;
    else if(camOrbitAngle<0) camOrbitAngle+=360;
    cameraOrbitAnim.start();
}

void Scene3D::startPlayerEntryAnim(size_t player)
{
    Q_ASSERT_X(player>0 && player < players.size(),
               "Scene3D::rotatePlayer",
               QString(
                   "Unknown player number : "+
                   QString::number(player)).toStdString().c_str());
    players[player]->startEntryAnim();
}

void Scene3D::startPlayersEntryAnim()
{
    std::vector<PlayerEntity*> enabled_players(players.size());
    std::vector<PlayerEntity*>::iterator it =
            std::copy_if(players.begin(), players.end(), enabled_players.begin(),
                         [](const PlayerEntity* x){return x->isEnabled();});
    enabled_players.resize(static_cast<size_t>(std::distance(
                                            enabled_players.begin(),it)));

    for (auto player : enabled_players)player->startEntryAnim();
}

void Scene3D::startCardsDistributionAnim()
{
    singleCards[0]->startEntryAnim();
    cardsDistribAnim.start();
}

void Scene3D::initCardsDeckMaterial()
{
    if (pokerTableLoader->status() != Qt3DRender::QSceneLoader::Ready)return;
    qDebug() << "names : " << pokerTableLoader->entityNames();

    auto cards = pokerTableLoader->entity("cards");
    Q_ASSERT_X(cards, "Scene3D::initCardsMaterial", "Cards entity loaded aren't found...");

    auto cardsMaterial = new Qt3DExtras::QTextureMaterial(cards);
    auto texture = new Qt3DRender::QTexture2D(cardsMaterial);
    auto textureImage = new Qt3DRender::QTextureImage(cardsMaterial);
    textureImage->setSource(QUrl("qrc:/asset/cards_deck.jpg"));
    texture->addTextureImage(textureImage);
    cardsMaterial->setTexture(texture);
    cards->addComponent(cardsMaterial);
}

void Scene3D::disableAllPlayers()
{
    for (auto player : players)player->setEnabled(false);
    for (auto card : singleCards)card->setEnabled(false);
}

void Scene3D::disablePlayer(size_t player)
{
    Q_ASSERT_X(player>0 && player < players.size(),
               "Scene3Scene3D::disablePlayer",
               QString(
                   "Unknown player number : "+
                   QString::number(player)).toStdString().c_str());
    players[player]->setEnabled(false);
    singleCards[player]->setEnabled(false);
}

void Scene3D::enableAllPlayers()
{
    for (auto player : players)player->setEnabled(true);
    for (auto card : singleCards)card->setEnabled(true);
}

void Scene3D::enablePlayer(size_t player)
{
    Q_ASSERT_X(player>0 && player < players.size(),
               "Scene3Scene3D::enablePlayer",
               QString(
                   "Unknown player number : "+
                   QString::number(player)).toStdString().c_str());
    players[player]->setEnabled(true);
    singleCards[player]->setEnabled(true);
}

void Scene3D::animateNextCardDistribution()
{
    ++currentDistributedCard;

    std::vector<SingleCardEntity*> enabled_singlecards(singleCards.size());
    std::vector<SingleCardEntity*>::iterator it =
            std::copy_if(singleCards.begin(), singleCards.end(), enabled_singlecards.begin(),
                         [](const SingleCardEntity* x){return x->isEnabled();});
    enabled_singlecards.resize(static_cast<size_t>(std::distance(
                                   enabled_singlecards.begin(),it)));

    if (currentDistributedCard >= enabled_singlecards.size()) {
        currentDistributedCard = 0;
        cardsDistribAnim.stop();
        return;
    }

    enabled_singlecards[currentDistributedCard]->startEntryAnim();
}
