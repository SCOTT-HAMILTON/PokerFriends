#ifndef PARTYWIDGET_H
#define PARTYWIDGET_H

#include <QWidget>
#include <QVBoxLayout>

#include "3d/scene3d.h"

class PartyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PartyWidget(const QSize& size, QWidget *parent = nullptr);

signals:

public slots:

public:
    QVBoxLayout* mainLay;
    Scene3D* scene3D;
};

#endif // PARTYWIDGET_H
