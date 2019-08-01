#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include <iostream>

Widget::Widget(QWidget *parent) :
    QWidget(parent), gamewidget(nullptr)
{

    connect(&client, &Client::newMessage,
            this, &Widget::implNewMessage);

    connect(&client, &Client::newParticipant,
            this, &Widget::implNewParticipant);
    connect(&client, &Client::participantLeft,
            this, &Widget::implParticipantLeft);

    initialmenu = new InitialMenu(this);

    connect(initialmenu, &InitialMenu::joinServerRequest, this, &Widget::switchToGameParty);
}

Widget::~Widget()
{
}

void Widget::implNewMessage(const QString &peerNick, const QString &msg)
{
    if (!gamewidget || !gamewidget->isVisible()){
        qDebug() << "received message but not yet ready... here is it : ";
        qDebug() << msg << " from " << peerNick;
        return;
    }
    qDebug() << "NEW MESSAGE from " << peerNick << " : " << msg;
    if (msg.startsWith("Hey, I'm ")){
        qDebug() << "A new player entered in the party!! Let's welcome him";
        QString playerNickname = msg.mid(9, -1);
        qDebug() << "nickname is " << playerNickname;
        gameplay.addPlayer(playerNickname, peerNick);
        client.sendMessage("Welcome, I'm "+nickname);
    }
    else if (msg.startsWith("Welcome, I'm ")){
        QString playerNickname = msg.mid(13, -1);
        if (!gameplay.playerExist(peerNick)){
            qDebug() << "Player welcomed us, his name is " << playerNickname;
            gameplay.addPlayer(playerNickname, peerNick);
        }else{
            qDebug() << "Player welcomed us, we already now him, it's " << playerNickname;
        }

    }
   qDebug() << gameplay.playersReport();
}

void Widget::implNewParticipant(const QString &peerNick)
{
    qDebug() << "NEW PARTICIPANT : " << peerNick;
}

void Widget::implParticipantLeft(const QString &peerNick)
{
    qDebug() << "A PARTICIPANT LEFT : " << peerNick;
    if (gameplay.playerExist(peerNick)){
        gameplay.removePlayer(peerNick);
        std::cerr << gameplay.playersReport().toStdString() << '\n';
    }else{
        qDebug() << "unknow player, can't remove it (!";
    }
}

void Widget::switchToGameParty(QString nick)
{
    QString devicename = client.nickName();
    nickname = nick;
    client.sendMessage("Hey, I'm "+nickname);
    initialmenu->hide();
    gamewidget = new GameWidget(7, this);
    gamewidget->show();
}


