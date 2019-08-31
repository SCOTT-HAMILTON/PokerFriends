#ifndef EntryAnimatedEntity_H
#define EntryAnimatedEntity_H

#include <QEntity>
#include <QTransform>
#include <QSceneLoader>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

#include <QVector3D>

class EntryAnimatedEntity : public Qt3DCore::QEntity
{
    Q_OBJECT
    Q_PROPERTY(float xpos READ getXpos WRITE setXpos)

public:
    explicit EntryAnimatedEntity(const QVector3D& startpos,
        const QVector3D& endpos,
        float angle,
        const QString& sceneSourceUrl,
        Qt3DCore::QNode *parentNode = nullptr);
    void move(const QVector3D& vec);
    void rotate(float angle);
    void startEntryAnim();
    float getXpos() const{return xpos;}
    void setXpos(float newxpos);
    void setRotation(QVector3D rot);

    float xpos;
    float ypos;
    float zpos;

protected:
    virtual void onSceneLoaderStatusChanged(){}
    Qt3DRender::QSceneLoader* sceneLoader;
    QPropertyAnimation xAnim;

private slots:
    void callSceneLoaderStatusChangedCallBack();

private:
    QVector3D startPos;
    QVector3D endPos;
    float angle;
    bool canMove;


    Qt3DCore::QTransform* transform;



};

#endif // EntryAnimatedEntity_H
