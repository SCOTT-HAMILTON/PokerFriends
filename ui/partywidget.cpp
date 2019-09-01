#include "partywidget.h"

#include <QQmlContext>
#include "size.h"
#include <list>

PartyWidget::PartyWidget(const QSize& size, QWidget *parent) : QWidget(parent),
    previousPlayersCount(0)
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

void PartyWidget::updatePlayersCount(size_t playersCount)
{
    scene3D->disableAllPlayers();

    {
        std::list<size_t> range(playersCount);
        std::for_each(range.begin(), range.end(), [this](size_t i){
            scene3D->enablePlayer(i);
        });
    }
    if (previousPlayersCount<playersCount){
        scene3D->startPlayersEntryAnim();
        scene3D->startCamOrbitAnim();
    }
    previousPlayersCount = playersCount;
}
