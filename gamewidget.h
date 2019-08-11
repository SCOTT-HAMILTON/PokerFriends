#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QList>
#include <vector>
#include <QPoint>
#include <QQuickWidget>
#include <QDialog>
#include <QVBoxLayout>

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
    void updatePlayerTour(QString nickname);
    void emitReadyToStartTheGame();
    void emitGameStarted();
    void makeGameStarted();



    /* OLD
     *
    */
    void updateNames();

    void generateGrid(int nbPlayers);
    void fetchPlayers();
    void reorder();
    void on_startTheGameButton_clicked();
    void showStartTheGameButton();

private:
    QVBoxLayout *mainLay;
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
