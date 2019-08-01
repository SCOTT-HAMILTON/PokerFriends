#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "client.h"
#include "initialmenu.h"
#include "gamewidget.h"
#include "gameplay.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void implNewMessage(const QString& peerNick, const QString& msg);
    void implNewParticipant(const QString &peerNick);
    void implParticipantLeft(const QString &peerNick);
    void switchToGameParty(QString);


private:
    Client client;
    QString nickname;
    GameWidget *gamewidget;
    InitialMenu *initialmenu;
    GamePlay gameplay;
};

#endif // WIDGET_H
