#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QList>
#include <iostream>
#include <QDebug>

class Player{
public:
    Player(QString nick, QString peerNick, bool ready=false) :
        nickname(nick), peernickname(peerNick), ready(ready)
    {
    }

    friend QDebug& operator<<(QDebug& os, const Player player){
        os << player.nickname <<'('<< player.peernickname << ')';
        return os;
    }

    QString nickname;
    QString peernickname;
    bool ready;
};

#endif // PLAYER_H
