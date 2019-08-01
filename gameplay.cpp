#include "gameplay.h"

#include <algorithm>
#include <QDebug>

GamePlay::GamePlay(QObject *parent) : QObject(parent)
{

}

bool GamePlay::playerExist(const QString &peerNick) const
{
    return findPlayerWithPeerNick(peerNick) != players.end();
}

void GamePlay::addPlayer(const QString &nickname, const QString &peerNick)
{
    players.append(Player{nickname, peerNick});
}

void GamePlay::removePlayer(const QString &peerNick)
{
    qDebug() << "erasing peerNick : " << peerNick;
    qDebug() << "size before : " << players.size();
    players.erase(std::remove_if(players.begin(), players.end(), [&peerNick](const Player &p){ return p.peernickname == peerNick;}));
    qDebug() << "size after : " << players.size();
}

const QList<Player> &GamePlay::getPlayers() const
{
    return players;
}

const QList<Player>::const_iterator GamePlay::findPlayerWithPeerNick(const QString &peerNick) const
{
    return std::find_if(players.begin(), players.end(), [&peerNick](const Player &p){ return p.peernickname == peerNick;});
}

QString GamePlay::playersReport() const
{
    QString out;
    out += QString::number(players.size()) + " players are in the party : \n";
    for (auto &p : players){
        out += " - " + p.peernickname+" alias "+p.nickname+'\n';
    }
    out += '\n';
    return out;
}
