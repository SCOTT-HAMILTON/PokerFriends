#include "3d/playerentity.h"

#include <QDebug>

PlayerEntity::PlayerEntity(const QVector3D& startpos,
                           const QVector3D& endpos,
                           float angle, Qt3DCore::QNode *parentNode) :
    EntryAnimatedEntity (startpos, endpos, angle, "qrc:/asset/player.gltf",parentNode)
{
}
