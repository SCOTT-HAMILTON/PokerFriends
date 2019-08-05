#include "gameplay.h"

#include <algorithm>
#include <QDebug>

GamePlay::GamePlay(QObject *parent) : QObject(parent), gameStarted(false)
{

}

bool GamePlay::isTheGameStarted() const
{
    return gameStarted;
}

void GamePlay::startTheGame()
{
    gameStarted = true;
}

