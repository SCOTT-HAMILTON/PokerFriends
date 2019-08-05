#include "playersressource.h"

#include <QDebug>

PlayersRessource::PlayersRessource(QObject *parent) : QObject(parent)
{

}

bool PlayersRessource::playerExist(const QString &peerNick) const
{
    return findPlayerWithPeerNick(peerNick) != players.end();
}

bool PlayersRessource::nicknameExists(const QString &nickname) const
{
    return findPlayerWithNickname(nickname) != players.end();
}

void PlayersRessource::addPlayer(const QString &nickname, const QString &peerNick)
{
    players.append(Player{nickname, peerNick});
}

void PlayersRessource::removePlayer(const QString &peerNick)
{
    qDebug() << "erasing peerNick : " << peerNick;
    qDebug() << "size before : " << players.size();
    players.erase(std::remove_if(players.begin(), players.end(), [&peerNick](const Player &p){ return p.peernickname == peerNick;}));
    qDebug() << "size after : " << players.size();
}

QList<Player> PlayersRessource::getPlayers() const
{
    return players;
}

int PlayersRessource::playersCount() const
{
    return players.size();
}

const QList<Player>::const_iterator PlayersRessource::findPlayerWithPeerNick(const QString &peerNick) const
{
    return std::find_if(players.begin(), players.end(), [&peerNick](const Player &p){ return p.peernickname == peerNick;});
}

const QList<Player>::const_iterator PlayersRessource::findPlayerWithNickname(const QString &nickname) const
{
    return std::find_if(players.begin(), players.end(), [&nickname](const Player &p){ return p.nickname == nickname;});
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
