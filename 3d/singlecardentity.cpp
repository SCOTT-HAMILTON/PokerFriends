#include "3d/singlecardentity.h"

#include <QDebug>
#include <QMaterial>
#include <QPhongMaterial>

SingleCardEntity::SingleCardEntity(const QVector3D& startpos,
                                   const QVector3D& endpos,
                                   float angle,
                                   Qt3DCore::QNode *parentNode) :
    EntryAnimatedEntity (startpos, endpos, angle, "qrc:/asset/single_card.gltf",parentNode)
{
    xAnim.setDuration(500);
    xAnim.setEasingCurve(QEasingCurve::InOutCubic);
}


void SingleCardEntity::initMaterials()
{
    if (sceneLoader->status() != Qt3DRender::QSceneLoader::Ready)return;
    qDebug() << "single card names : " << sceneLoader->entityNames();
    for (auto& name : sceneLoader->entityNames()){
        auto item = sceneLoader->entity(name);
        for (int i = item->components().size()-1; i >= 0 ; i--){
            try {
                auto effect = item->components()[i]->property("effect");
                if (effect.isValid()){
                    auto material = static_cast<Qt3DRender::QMaterial*>(item->components()[i]);
                    material->deleteLater();
                    auto newmat = new Qt3DExtras::QPhongMaterial(item);
                    std::string color(item->objectName().toStdString());
                    color.erase(std::remove_if(color.begin(),
                                               color.end(),
                                               [](unsigned char x){return std::isdigit(x);}),
                                color.end());
                    newmat->setAmbient(color.c_str());
                    newmat->setDiffuse(color.c_str());
                    item->addComponent(newmat);
                }
            }catch(std::exception&){}
        }
    }
}
