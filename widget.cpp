#include "widget.h"
#include "ui_widget.h"
#include "size.h"

#include <QDebug>
#include <iostream>
#include <algorithm>
#include <QRegularExpression>
#include <map>

Widget::Widget(QWidget *parent) :
    QWidget(parent), gamewidget(nullptr)
{


    setFixedSize(Size::APP_SIZEW, Size::APP_SIZEH);


    initialmenu = new InitialMenu(this);
    playersRessource = new PlayersRessource(this);

    networkProtocol = new NetworkProtocol(playersRessource);

    connect(initialmenu, &InitialMenu::joinServerRequest, this, &Widget::switchToGameParty);

    connect(networkProtocol, &NetworkProtocol::newPlayerAdded, this, &Widget::fetchPlayersToGUI);
    connect(networkProtocol, &NetworkProtocol::participantLeft, this, &Widget::fetchPlayersToGUI);


    connect(networkProtocol, &NetworkProtocol::connectionRefusedBecauseOfNickname, this, &Widget::repromptNickname);
    connect(&errorPauseTimer, &QTimer::timeout, this, &Widget::resetNetworkAndEnableInitialMenu);
}

Widget::~Widget()
{
}

void Widget::switchToGameParty(QString nick)
{
    networkProtocol->enable();
    networkProtocol->startConnection(nick);
    initialmenu->hide();
    if (!gamewidget){
        gamewidget = new GameWidget(7, this);
        connect(gamewidget, &GameWidget::readyToStartTheGame, this, &Widget::readyToStartTheGame);
    }
    gamewidget->fetchPlayers(playersRessource->getPlayers());
    gamewidget->show();
}

void Widget::repromptNickname()
{
    qDebug() << "\n\n\nRETRY\n\n\n";
    initialmenu->showNicknameRefusedLabel();
    initialmenu->show();
    gamewidget->hide();
    initialmenu->setEnabled(false);
    errorPauseTimer.setInterval(2000);
    errorPauseTimer.setSingleShot(true);
    errorPauseTimer.start();
}

void Widget::resetNetworkAndEnableInitialMenu()
{
    initialmenu->setEnabled(true);
    networkProtocol->reset();
}

void Widget::readyToStartTheGame()
{
    qDebug() << "Ready to start the game!!!";
}

void Widget::fetchPlayersToGUI()
{
    gamewidget->fetchPlayers(playersRessource->getPlayers());
}


