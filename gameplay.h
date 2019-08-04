#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <QObject>
#include <QString>
#include <QList>

#include "player.h"

class GamePlay : public QObject
{
    Q_OBJECT
public:
    explicit GamePlay(QObject *parent = nullptr);
    bool playerExist(const QString &peernick) const;
    bool nicknameExists(const QString &nickname) const;
    void addPlayer(const QString &nickname, const QString &peernick);
    void removePlayer(const QString &peernick);
    QList<Player> getPlayers() const;
    int playersCount() const;
    const QList<Player>::const_iterator findPlayerWithPeerNick(const QString &peerNick) const;
    const QList<Player>::const_iterator findPlayerWithNickname(const QString &nickname) const;

    QString playersReport() const;

signals:

public slots:

private:
    QList<Player> players;
};

#endif // GAMEPLAY_H
