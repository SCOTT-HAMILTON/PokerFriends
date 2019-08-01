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
    void addPlayer(const QString &nickname, const QString &peernick);
    void removePlayer(const QString &peernick);
    const QList<Player> &getPlayers() const;
    const QList<Player>::const_iterator findPlayerWithPeerNick(const QString &peerNick) const;

    QString playersReport() const;

signals:

public slots:

private:
    QList<Player> players;
};

#endif // GAMEPLAY_H
