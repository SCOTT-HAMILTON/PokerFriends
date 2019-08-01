#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QList>
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
    explicit GameWidget(int nbPlayers, QWidget *parent = nullptr);
    ~GameWidget();
    GridPos indexToPos(int index);

public slots:
    void fetchPlayers(const QList<Player> &list);
    void generateGrid(int nbPlayers);
    void reorder();

private:
    Ui::GameWidget *ui;
    QString nickname;
    QList<QWidget*> players;

};

#endif // GAMEWIDGET_H
