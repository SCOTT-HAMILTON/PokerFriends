#ifndef PLAYERENTITY_H
#define PLAYERENTITY_H

#include "3d/entryanimatedentity.h"

class PlayerEntity : public EntryAnimatedEntity
{
    Q_OBJECT

public:
    explicit PlayerEntity(const QVector3D& startpos,
                          const QVector3D& endpos,
                          float angle,
                          Qt3DCore::QNode *parentNode = nullptr);
};

#endif // PLAYERENTITY_H
