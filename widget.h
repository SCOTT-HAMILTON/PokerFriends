#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QList>
#include <QVariant>
#include <QTimer>

#include "client.h"
#include "initialmenu.h"
#include "gamewidget.h"
#include "gameplay.h"
#include "playersressource.h"
#include "networkprotocol.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

signals:
    void newNetworkMessage(const QString &, const QString&);

public slots:
    void switchToGameParty(QString);
    void repromptNickname();
    void resetNetworkAndEnableInitialMenu();
    void readyToStartTheGame();

    void fetchPlayersToGUI();


private:
    GameWidget *gamewidget;
    InitialMenu *initialmenu;
    GamePlay gameplay;

    NetworkProtocol *networkProtocol;
    PlayersRessource *playersRessource;

    QTimer errorPauseTimer;

};

#endif // WIDGET_H
