#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <QObject>
#include <QString>
#include <QList>

#include "core/playersressource.h"
#include "core/player.h"

class GamePlay : public QObject
{
    Q_OBJECT
public:
    explicit GamePlay(PlayersRessource* playersRessource, QObject *parent = nullptr);

    bool isTheGameStarted() const;
    QString getTourPlayerNickname() const;

signals:

public slots:
    void setWhoseTourIsIt(QString nickname);
    void startTheGame(QString);

private:
    PlayersRessource* playersRessource;

    QString tourPlayerNickname;
    bool gameStarted;
};

#endif // GAMEPLAY_H
