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

    bool isTheGameStarted() const;
    void startTheGame();



signals:

public slots:

private:

    bool gameStarted;
};

#endif // GAMEPLAY_H
