#ifndef SINGLECARDENTITY_H
#define SINGLECARDENTITY_H

#include "3d/entryanimatedentity.h"

class SingleCardEntity : public EntryAnimatedEntity
{
    Q_OBJECT

public:
    explicit SingleCardEntity(const QVector3D& startpos,
                              const QVector3D& endpos,
                              float angle,
                              Qt3DCore::QNode *parentNode = nullptr);

protected:
    virtual void onSceneLoaderStatusChanged() override{
        initMaterials();
    }

public slots:
    void initMaterials();

private:
    QVector3D pos;
};

#endif // SINGLECARDENTITY_H
