#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QList>

class Player{
public:
    Player(QString nick, QString peerNick, bool ready=false) :
        nickname(nick), peernickname(peerNick), ready(ready)
    {
    }
    QString nickname;
    QString peernickname;
    bool ready;
};

#endif // PLAYER_H
