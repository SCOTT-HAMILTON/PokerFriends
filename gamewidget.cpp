#include "gamewidget.h"
#include "ui_gamewidget.h"

#include <QDebug>

GameWidget::GameWidget(int nbPlayers, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWidget)
{
    ui->setupUi(this);
    //eui

    generateGrid(nbPlayers);
}

GameWidget::~GameWidget()
{
    delete ui;
}

GridPos GameWidget::indexToPos(int index)
{
    int l = 0, c = 0;
    switch (index) {
    case 0: {
        l = 0;
        c = 1;
        break;
    }
    case 1: {
        l = 2;
        c = 0;
        break;
    }
    case 2: {
        l = 1;
        c = 3;
        break;
    }
    case 3: {
        l = 3;
        c = 2;
        break;
    }
    case 4: {
        l = 0;
        c = 2;
        break;
    }
    case 5: {
        l = 1;
        c = 0;
        break;
    }
    case 6: {
        l = 2;
        c = 3;
        break;
    }
    case 7: {
        l = 3;
        c = 1;
        break;
    }
    }
    l = 4-(l+1);
    return GridPos{l, c};
}

void GameWidget::fetchPlayers(const QList<Player> &list)
{
    if (list.size()<=7){
        generateGrid(list.size());
        // One to uncount us;
        for (int i = 1; i < players.size(); i++){
            PlayerWidget *player = static_cast<PlayerWidget*>(players[i]);
            player->setName(list[i].nickname);
        }
    }else{
        qDebug() << "Error, too much players to fetch : " << list.size();
    }
}

void GameWidget::generateGrid(int nbPlayers)
{
    if (nbPlayers>7){
        qDebug() << "Error, too much players given at generate grid, GameWidget : " << nbPlayers;
        return;
    }
    for (QObject *obj : ui->gridLayout->children()){
        QWidget *widget = static_cast<QWidget*>(obj);
        ui->gridLayout->removeWidget(widget);
        widget->deleteLater();
    }
    players.clear();
    for (int i = 0; i < nbPlayers+1; i++){
        PlayerWidget *playerwidget = new PlayerWidget(this);
        playerwidget->setObjectName("Player "+QString::number(i+1));
        playerwidget->setName("Player "+QString::number(i+1));
        players.append(playerwidget);
        int l = 0, c = 0;

        auto pos = indexToPos(i);
        l = pos.line;
        c = pos.column;
        playerwidget->setProperty("layoutLine", l);
        playerwidget->setProperty("layoutColumn", c);
        ui->gridLayout->addWidget(playerwidget, l, c);
    }
    reorder();
    if (players.size()>0){
        static_cast<PlayerWidget*>(players[0])->setName("me");
    }else{
        qDebug() << "Error, not enough players... at generate grid, GameWidget.";
    }
}

void GameWidget::reorder()
{
    players.clear();
    int l = 3;
    int c = 0;
    for (int i = 0; i < 4; i++){
        int* index = nullptr;
        int sens = 1;
        switch (i){
        case 0:{
            l = 3;
            c = 0;
            sens = 1;
            index = &c;
            break;
        }
        case 1:{
            l = 3;
            c = 3;
            sens = -1;
            index = &l;
            break;
        }
        case 2:{
            l = 0;
            c = 3;
            sens = -1;
            index = &c;
            break;
        }
        case 3:{
            l = 0;
            c = 0;
            sens = 1;
            index = &l;
            break;
        }
        }
        if (!index){
            qDebug() << "error, index is nullptr.";
            continue;
        }
        for (*index = *index; *index < 4 && *index >=0; (*index)+=sens){
            auto item = ui->gridLayout->itemAtPosition(l, c);
            if (!item){
                continue;
            }
            if (!item->widget()){
                qDebug() << "Item but no widget at position " << l << ", " << c;
            }
            players.append(item->widget());
        }
    }
    for (int i = 0; i < players.size(); i++){
        static_cast<PlayerWidget*>(players[i])->setName("Player "+QString::number(i+1));
    }
}