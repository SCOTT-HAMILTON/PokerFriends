#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QList>
#include <vector>
#include <QPoint>

#include "playerwidget.h"
#include "player.h"

namespace Ui {
class GameWidget;
}

struct GridPos{
    int line;
    int column;
};

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(int nbPlayers = 8, QWidget *parent = nullptr);
    ~GameWidget();
    GridPos indexToPos(int index);

signals:
    void readyToStartTheGame();
    void gameStarted();

public slots:
    void fetchPlayers(const QList<Player> list);
    void generateGrid(int nbPlayers);
    void reorder();
    void on_startTheGameButton_clicked();
    void showStartTheGameButton();
    void hideStartTheGameButton();
    void showWaitingForPlayersToBeReady();

private:
    enum class StartButtonMode{
        READY, READY_WAITING, START
    };
    StartButtonMode startButtonMode;
    Ui::GameWidget *ui;
    QString nickname;
    QList<QWidget*> players;

};

#endif // GAMEWIDGET_H
