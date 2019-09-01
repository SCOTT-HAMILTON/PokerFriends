#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QList>
#include <vector>
#include <QPoint>
#include <QDialog>
#include <QVBoxLayout>
#include <QQuickWidget>

#include "ui/partywidget.h"
#include "core/player.h"
#include "core/playersressource.h"
#include "ui/partywidget.h"

struct GridPos{
    int line;
    int column;
};

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(PlayersRessource *playersRessource, QWidget *parent = nullptr);
    ~GameWidget();
    GridPos indexToPos(int index);

signals:
    void readyToStartTheGame();
    void gameStarted();

public slots:
    void emitReadyToStartTheGame();
    void emitGameStarted();
    void makeGameStarted();
    void showStartTheGameButton();
    void updatePartyPlayersCount();

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
