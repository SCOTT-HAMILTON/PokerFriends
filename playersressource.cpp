#include "playersressource.h"

#include <QDebug>

PlayersRessource::PlayersRessource(QObject *parent) : QObject(parent)
{

}

bool PlayersRessource::nicknameExists(const QString &nickname) const
{
    return findPlayerWithNickname(nickname) != players.end();
}

void PlayersRessource::addPlayer(const QString &nickname, const QString &peerNick)
{
    players.append(Player(nickname, peerNick));
    emit playersCountChanged(players);
}

void PlayersRessource::removePlayer(const QString &peerNick)
{
    players.erase(std::remove_if(players.begin(), players.end(), [&peerNick](const Player &p){ return p.peernickname == peerNick;}));
    emit playersCountChanged(players);
}

void PlayersRessource::setPlayerReady(const QString &peerNick)
{
    auto player = findPlayerWithPeerNick(peerNick);
    if (player != players.end()){
        player->ready = true;
    }else{
        qDebug() << "Error PlayersRessource -> setPlayerReady : unknown player " << peerNick;
    }
}

QList<Player> PlayersRessource::getPlayers() const
{
    return players;
}

int PlayersRessource::playersCount() const
{
    return players.size();
}

const QList<Player>::iterator PlayersRessource::findPlayerWithPeerNick(const QString &peerNick)
{
    return std::find_if(players.begin(), players.end(), [&peerNick](const Player &p){ return p.peernickname == peerNick;});
}

const QList<Player>::const_iterator PlayersRessource::findPlayerWithPeerNick(const QString &peerNick) const
{
    return std::find_if(players.cbegin(), players.cend(), [&peerNick](const Player &p){ return p.peernickname == peerNick;});
}

const QList<Player>::iterator PlayersRessource::findPlayerWithNickname(const QString &nickname)
{
    return std::find_if(players.begin(), players.end(), [&nickname](const Player &p){ return p.nickname == nickname;});
}

const QList<Player>::const_iterator PlayersRessource::findPlayerWithNickname(const QString &nickname) const
{
    return std::find_if(players.cbegin(), players.cend(), [&nickname](const Player &p){ return p.nickname == nickname;});
}

bool PlayersRessource::playerExistWithNickname(const QString &nickname) const
{
    return findPlayerWithNickname(nickname) != players.end();
}

bool PlayersRessource::playerExistWithPeerNick(const QString &peerNick) const
{
    return findPlayerWithPeerNick(peerNick) != players.end();
}

QString PlayersRessource::playersReport() const
{
    QString out;
    out += QString::number(players.size()) + " players are in the party : \n";
    for (auto &p : players){
        out += " - " + p.peernickname+" alias "+p.nickname+'\n';
    }
    out += '\n';
    return out;
}

bool PlayersRessource::isPlayerReady(const QString &peerNick) const
{
    auto player = findPlayerWithNickname(peerNick);
    if (player != players.end()){
        return player->ready;
    }else{
        qDebug() << "Error PlayersRessource -> isPlayerReady : unknown player " << peerNick;
        return false;
    }
}

bool PlayersRessource::allPlayersAreReady() const
{
    if (players.size() == 0)return true;
    for (auto &player : players){
        if (!player.ready)return false;
    }
    return true;
}
