#include "partywidget.h"

#include <QQmlContext>
#include "size.h"

PartyWidget::PartyWidget(const QSize& size, QWidget *parent) : QWidget(parent)
{

    QPalette pa(QColor(255, 00, 00));
    setPalette(pa);
    setAutoFillBackground(true);
    mainLay = new QVBoxLayout;
    mainLay->setMargin(0);

    scene3D = new Scene3D(size);

    mainLay->addWidget(scene3D);

    setLayout(mainLay);
}
