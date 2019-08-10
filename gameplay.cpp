#include "gameplay.h"

#include <algorithm>
#include <QDebug>

GamePlay::GamePlay(PlayersRessource* playersRessource, QObject *parent) :
    QObject(parent),
    playersRessource(playersRessource), gameStarted(false)
{
}

bool GamePlay::isTheGameStarted() const
{
    return gameStarted;
}

void GamePlay::startTheGame(QString nickname)
{
    setWhoseTourIsIt(nickname);
    gameStarted = true;
}

void GamePlay::setWhoseTourIsIt(QString nickname)
{
    qDebug() << "It's the turn of " << nickname;
    /*TODO test if player tour is set*/
    if (playersRessource->playerExistWithNickname(nickname) || nickname == "me"){
        tourPlayerNickname = nickname;
        qDebug() << "YEp, nickname ok for " << nickname;
    }else{
        qDebug() << "Error, GamePlay -> setWhoseTourIsIt : Player nickname doesn't exist " << nickname;
    }
}

