#include "gamewidget.h"
#include "size.h"
#include "ui_gamewidget.h"

#include <QDebug>
#include <algorithm>
#include <QQmlContext>
#include <QQuickItem>

GameWidget::GameWidget(PlayersRessource *playersRessource, int nbPlayers, QWidget *parent) :
    QWidget(parent), playersRessource(playersRessource),
    startButtonMode(StartButtonMode::READY)
{
    setFixedSize(Size::APP_SIZEW, Size::APP_SIZEH);

    partyWidget = new PartyWidget(this);
    partyWidget->hide();

    generateGrid(nbPlayers);
    fetchPlayers();

    connect(playersRessource, &PlayersRessource::playersCountChanged, this, &GameWidget::fetchPlayers);

    view = new QQuickWidget(this);
    view->setWindowFlags(Qt::WindowStaysOnTopHint);
    view->rootContext()->setContextProperty("APP_SIZEW", Size::APP_SIZEW);
    view->rootContext()->setContextProperty("APP_SIZEH", Size::APP_SIZEH);
    view->rootContext()->setContextProperty("SIZE_FACTOR", Size::SIZE_FACTOR);
    const int screen_pc = 30;
    view->rootContext()->setContextProperty("SCREEN_PERCENT", screen_pc);
    view->move(0, Size::APP_SIZEH-(Size::APP_SIZEH*screen_pc/100));
    view->setSource(QUrl("qrc:/StartTheGamePanel.qml"));
    if (view->status() == QQuickWidget::Error)
        qDebug() << "Error, view can't load main.qml source !!!";
    view->setResizeMode(QQuickWidget::SizeRootObjectToView);
    view->setFixedSize(Size::APP_SIZEW, Size::APP_SIZEH*screen_pc/100);
    view->show();

    connect(view->rootObject(), SIGNAL(readyWaiting()), this, SLOT(emitReadyToStartTheGame()));
    connect(view->rootObject(), SIGNAL(gameStarted()), this, SLOT(emitGameStarted()));
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

void GameWidget::fetchPlayers()
{
    auto list = playersRessource->getPlayers();
    if (list.size()<=7){
        generateGrid(list.size());
        for (int i = 1; i < players.size(); i++){
            PlayerWidget *player = static_cast<PlayerWidget*>(players[i]);
            player->setName(list[static_cast<int>(i-1)].nickname);
        }
    }else{
        qDebug() << "Error, too much or no players to fetch : " << list.size();
    }
    updateNames();
}

void GameWidget::updateNames()
{
    auto list = playersRessource->getPlayers();
    if (list.size()+1 != players.size()){
        qDebug() << "Error GameWidget->updateNames : playerswiget list and players list don't have the same size";
    }
    static_cast<PlayerWidget*>(players[0])->setName("me");

    for (int i =1; i < players.size(); i++){
        static_cast<PlayerWidget*>(players[i])->setName(list[i-1].nickname);
    }
}

void GameWidget::generateGrid(int nbPlayers)
{
    if (players.size() > 0){
        for (QWidget *widget : players){
            if (!widget)continue;
            partyWidget->ui->gridLayout->removeWidget(widget);
            widget->deleteLater();
        }
    }
    players.clear();

    if (nbPlayers>7){
        qDebug() << "Error, too much players given at generate grid, GameWidget : " << nbPlayers;
        return;
    }

    for (int i = 0; i < nbPlayers+1; i++){
        PlayerWidget *playerwidget = new PlayerWidget(this);
        playerwidget->setObjectName("Player "+QString::number(i+1));
        playerwidget->setName("Player "+QString::number(i+1));
        players.push_back(playerwidget);
        int l = 0, c = 0;

        auto pos = indexToPos(i);
        l = pos.line;
        c = pos.column;
        playerwidget->setProperty("layoutLine", l);
        playerwidget->setProperty("layoutColumn", c);
        partyWidget->ui->gridLayout->addWidget(playerwidget, l, c);
    }
    reorder();
    if (players.size()>0){
        static_cast<PlayerWidget*>(players[0])->setName("me");
    }else{
        qDebug() << "Error, not enough players... at generate grid, GameWidget.";
    }
}

void GameWidget::reorder()
{
    players.clear();
    int l = 3;
    int c = 0;
    for (int i = 0; i < 4; i++){
        int* index = nullptr;
        int sens = 1;
        switch (i){
        case 0:{
            l = 3;
            c = 0;
            sens = 1;
            index = &c;
            break;
        }
        case 1:{
            l = 3;
            c = 3;
            sens = -1;
            index = &l;
            break;
        }
        case 2:{
            l = 0;
            c = 3;
            sens = -1;
            index = &c;
            break;
        }
        case 3:{
            l = 0;
            c = 0;
            sens = 1;
            index = &l;
            break;
        }
        }
        if (!index){
            qDebug() << "error, index is nullptr.";
            continue;
        }
        for (*index = *index; *index < 4 && *index >=0; (*index)+=sens){
            auto item = partyWidget->ui->gridLayout->itemAtPosition(l, c);
            if (!item){
                continue;
            }
            QWidget *widget = item->widget();
            if (!widget){
                qDebug() << "Item but no widget at position " << l << ", " << c;
                continue;
            }
            players.push_back(widget);
        }
    }
    for (int i = 0; i < players.size(); i++){
        static_cast<PlayerWidget*>(players[i])->setName("Player "+QString::number(i+1));
    }
}

void GameWidget::on_startTheGameButton_clicked()
{
    if (startButtonMode == StartButtonMode::READY){
        startButtonMode = StartButtonMode::READY_WAITING;
        emit readyToStartTheGame();
    }else if (startButtonMode == StartButtonMode::START){
        qDebug() << "game started emitted!!!";
        emit gameStarted();
    }else{
        qDebug() << "NOPE, : ";
        switch (startButtonMode){
        case StartButtonMode::READY: {
            qDebug("READY");
            break;
        }case StartButtonMode::READY_WAITING: {
            qDebug("READY_WAITING");
            break;
        }case StartButtonMode::START: {
            qDebug("START");
            break;
        }

        }
    }
}

void GameWidget::showStartTheGameButton()
{
    partyWidget->ui->startTheGameButton->show();
    partyWidget->ui->startTheGameButton->setEnabled(true);
    partyWidget->ui->startTheGameButton->setText(tr("Start the game!!!"));
    startButtonMode = StartButtonMode::START;
    qDebug() << "START THE GAME!!!";
    QMetaObject::invokeMethod(view->rootObject(), "setAllPlayersReady");
}

void GameWidget::hideStartTheGameButton()
{
    partyWidget->ui->startTheGameButton->hide();
}

void GameWidget::showWaitingForPlayersToBeReady()
{
    qDebug() << "WAITING...";
    startButtonMode = StartButtonMode::READY_WAITING;
    partyWidget->ui->startTheGameButton->setText(tr("Waiting for players to be ready..."));
    partyWidget->ui->startTheGameButton->setEnabled(false);
}

void GameWidget::updatePlayerTour(QString nickname)
{
    QList<QWidget*>::iterator player = std::find_if(players.begin(), players.end(), [&nickname](const QWidget *widget){
        auto player = static_cast<const PlayerWidget*>(widget);
        return player->name() == nickname;
    });

    if (player == players.end()){
        qDebug() << "Erro GameWidget->updatePlayerTour : not found PlayerWidget with nickname " << nickname;
        return;
    }
    qDebug() << "Player turn : " << static_cast<PlayerWidget>(*player).name();
    static_cast<PlayerWidget*>(*player)->setStatusText("My turn...");
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


