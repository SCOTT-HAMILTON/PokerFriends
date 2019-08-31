#include "3d/entryanimatedentity.h"

#include <QDebug>

EntryAnimatedEntity::EntryAnimatedEntity(const QVector3D& startpos,
    const QVector3D& endpos,
    float angle,
    const QString& sceneSourceUrl,
    Qt3DCore::QNode *parentNode
) :
    Qt3DCore::QEntity(parentNode),
    xpos(0), ypos(0), zpos(0),
    xAnim(this, "xpos"),
    startPos(startpos), endPos(endpos), angle(angle), canMove(true)
{
    transform = new Qt3DCore::QTransform(this);
    transform->setTranslation(startPos);
    transform->setRotation(Qt3DCore::QTransform::fromAxesAndAngles(
                              QVector3D(0,1,0),0.0f,QVector3D(0,0,1),0.0f));
    transform->setRotationY(angle);

    sceneLoader = new Qt3DRender::QSceneLoader(this);
    sceneLoader->setSource(QUrl(sceneSourceUrl));
    connect(sceneLoader, &Qt3DRender::QSceneLoader::statusChanged, this, &EntryAnimatedEntity::callSceneLoaderStatusChangedCallBack);

    addComponent(sceneLoader);
    addComponent(transform);

    xAnim.setStartValue(startPos.x());
    xAnim.setEndValue(endPos.x());
    xAnim.setDuration(1000);
}

void EntryAnimatedEntity::move(const QVector3D &vec)
{
    transform->setTranslation(transform->translation()+vec);
}

void EntryAnimatedEntity::rotate(float newangle)
{
    angle = newangle;
    transform->setRotationY(angle);
}

void EntryAnimatedEntity::startEntryAnim()
{
    xAnim.start();
}

void EntryAnimatedEntity::setXpos(float newxpos)
{
    xpos = newxpos;
    float xrange = endPos.x()-startPos.x();
    float yrange = endPos.y()-startPos.y();
    float zrange = endPos.z()-startPos.z();
    float frac = qAbs((xpos-startPos.x())/xrange);
    ypos = (frac*yrange)+startPos.y();
    zpos = (frac*zrange)+startPos.z();
    transform->setTranslation(QVector3D(xpos, ypos, zpos));
}

void EntryAnimatedEntity::setRotation(QVector3D rot)
{
    transform->setRotationX(rot.x());
    transform->setRotationY(rot.y());
    transform->setRotationZ(rot.z());
}

void EntryAnimatedEntity::callSceneLoaderStatusChangedCallBack()
{
    onSceneLoaderStatusChanged();
}
