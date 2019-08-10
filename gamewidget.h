#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QList>
#include <vector>
#include <QPoint>
#include <QQuickWidget>
#include <QDialog>

#include "playerwidget.h"
#include "player.h"
#include "playersressource.h"
#include "partywidget.h"

struct GridPos{
    int line;
    int column;
};

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(PlayersRessource *playersRessource, int nbPlayers = 8, QWidget *parent = nullptr);
    ~GameWidget();
    GridPos indexToPos(int index);

signals:
    void readyToStartTheGame();
    void gameStarted();

public slots:
    void fetchPlayers();
    void generateGrid(int nbPlayers);
    void reorder();
    void on_startTheGameButton_clicked();
    void showStartTheGameButton();
    void hideStartTheGameButton();
    void showWaitingForPlayersToBeReady();
    void updatePlayerTour(QString nickname);

    void updateNames();

private:
    PartyWidget *partyWidget;

    enum class StartButtonMode{
        READY, READY_WAITING, START
    };

    PlayersRessource *playersRessource;

    StartButtonMode startButtonMode;

    QString nickname;
    QList<QWidget*> players;

    QQuickWidget *view;
    QDialog* qmlDialog;

};

#endif // GAMEWIDGET_H
