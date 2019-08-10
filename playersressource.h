#ifndef PLAYERSRESSOURCE_H
#define PLAYERSRESSOURCE_H

#include <QObject>
#include "player.h"

class PlayersRessource : public QObject
{
    Q_OBJECT
public:
    explicit PlayersRessource(QObject *parent = nullptr);

    /* *ACCESS*   */
    bool nicknameExists(const QString &nickname) const;
    QList<Player> getPlayers() const;
    int playersCount() const;
    const QList<Player>::iterator findPlayerWithPeerNick(const QString &peerNick);
    const QList<Player>::iterator findPlayerWithNickname(const QString &nickname);
    const QList<Player>::const_iterator findPlayerWithPeerNick(const QString &peerNick) const;
    const QList<Player>::const_iterator findPlayerWithNickname(const QString &nickname) const;
    bool playerExistWithNickname(const QString &nickname) const;
    bool playerExistWithPeerNick(const QString &peerNick) const;
    QString playersReport() const;
    bool isPlayerReady(const QString &peerNick) const;
    bool allPlayersAreReady() const;

    /* *MODIFYING */
    void addPlayer(const QString &nickname, const QString &peernick);
    void removePlayer(const QString &peernick);
    void setPlayerReady(const QString &peerNick);


signals:
    void playersCountChanged(QList<Player>&);

public slots:

private:
    QList<Player> players;
};

#endif // PLAYERSRESSOURCE_H
