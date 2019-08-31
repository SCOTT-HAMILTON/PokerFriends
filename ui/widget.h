#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QList>
#include <QVariant>
#include <QTimer>
#include <QQuickView>
#include <QVBoxLayout>

#include "networking/client.h"
#include "ui/initialmenu.h"
#include "ui/gamewidget.h"
#include "core/gameplay.h"
#include "core/playersressource.h"
#include "networking/networkprotocol.h"
#include <QtQuickWidgets/QQuickWidget>

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

signals:
    void newNetworkMessage(const QString &, const QString&);

public slots:
    void switchToGameParty();
    void repromptNickname();
    void resetNetworkAndEnableInitialMenu();
    void readyToStartTheGame();

    void sendGameStarted();
    void implStartTheGame(QString);
    void implStopTheGameBecauseOfNotAllReady();

    void showStartTheGameButton();

private:
    QVBoxLayout *mainLay;

    GameWidget* gamewidget;
    GamePlay* gameplay;

    NetworkProtocol *networkProtocol;
    PlayersRessource *playersRessource;

    QTimer errorPauseTimer;
    QQuickWidget* view;
};

#endif // WIDGET_H
