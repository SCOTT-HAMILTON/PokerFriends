#include "gamewidget.h"
#include "size.h"
#include "ui_gamewidget.h"
#include <QDesktopWidget>

#include <QDebug>
#include <algorithm>
#include <QQmlContext>
#include <QQuickItem>
#include <QStringList>

GameWidget::GameWidget(PlayersRessource *playersRessource, QWidget *parent) :
    QWidget(parent), playersRessource(playersRessource),
    startButtonMode(StartButtonMode::READY)
{
    QPalette pa(QColor(239, 239, 239));
    setPalette(pa);
    resize( Size::SCREENA_SIZEW, Size::SCREENA_SIZEH );
    setAutoFillBackground(true);

    mainLay = new QVBoxLayout;
    mainLay->setMargin(0);
    const int screen_pc = 30;
    partyWidget = new PartyWidget(QSize(Size::SCREENA_SIZEW, Size::APP_SIZEH-(Size::APP_SIZEH*(screen_pc)/100)), this);
    partyWidget->setAutoFillBackground(true);
    partyWidget->setMinimumWidth(Size::APP_SIZEW);
    updatePartyPlayersCount();
    mainLay->addWidget(partyWidget);

    view = new QQuickWidget(this);
    view->setMaximumSize(Size::SCREENA_SIZEW, Size::APP_SIZEH*screen_pc/100);
    view->rootContext()->setContextProperty("APP_X", Size::APP_X);
    view->rootContext()->setContextProperty("APP_Y", Size::APP_Y);
    view->rootContext()->setContextProperty("SCREENA_SIZEW", Size::SCREENA_SIZEW);
    view->rootContext()->setContextProperty("SCREENA_SIZEH", Size::SCREENA_SIZEH);
    view->rootContext()->setContextProperty("APP_SIZEW", Size::APP_SIZEW);
    view->rootContext()->setContextProperty("APP_SIZEH", Size::APP_SIZEH);
    view->rootContext()->setContextProperty("SIZE_FACTOR", Size::SIZE_FACTOR);

    view->rootContext()->setContextProperty("SCREEN_PERCENT", screen_pc);
    view->setSource(QUrl("qrc:/ui/StartTheGamePanel.qml"));
    if (view->status() == QQuickWidget::Error)
        qDebug() << "Error, view can't load main.qml source !!!";
    view->setResizeMode(QQuickWidget::SizeRootObjectToView);
    view->show();
    mainLay->addWidget(view);

    connect(view->rootObject(), SIGNAL(readyWaiting()), this, SLOT(emitReadyToStartTheGame()));
    connect(view->rootObject(), SIGNAL(gameStarted()), this, SLOT(emitGameStarted()));

    connect(playersRessource, &PlayersRessource::playersCountChanged,
            this, &GameWidget::updatePartyPlayersCount);

    setLayout(mainLay);
}

GameWidget::~GameWidget()
{
}

GridPos GameWidget::indexToPos(int index)
{
    int l = 0, c = 0;
    switch (index) {
    case 0: {
        l = 0;
        c = 1;
        break;
    }
    case 1: {
        l = 2;
        c = 0;
        break;
    }
    case 2: {
        l = 1;
        c = 3;
        break;
    }
    case 3: {
        l = 3;
        c = 2;
        break;
    }
    case 4: {
        l = 0;
        c = 2;
        break;
    }
    case 5: {
        l = 1;
        c = 0;
        break;
    }
    case 6: {
        l = 2;
        c = 3;
        break;
    }
    case 7: {
        l = 3;
        c = 1;
        break;
    }
    }
    l = 4-(l+1);
    return GridPos{l, c};
}

void GameWidget::showStartTheGameButton()
{
    qDebug() << "START THE GAME!!!";
    QMetaObject::invokeMethod(view->rootObject(), "setAllPlayersReady");
}

void GameWidget::updatePartyPlayersCount()
{
    partyWidget->updatePlayersCount(
                static_cast<size_t>(playersRessource->playersCount()));
}

void GameWidget::emitReadyToStartTheGame()
{
    emit readyToStartTheGame();
}

void GameWidget::emitGameStarted()
{
    emit gameStarted();
}

void GameWidget::makeGameStarted()
{
    QMetaObject::invokeMethod(view->rootObject(), "startTheGame");
}


