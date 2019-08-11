#include "widget.h"
#include "ui_widget.h"
#include "size.h"

#include <QDebug>
#include <iostream>
#include <algorithm>
#include <QRegularExpression>
#include <map>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>
#include <QApplication>
#include <QDesktopWidget>

Widget::Widget(QWidget *parent) :
    QWidget(parent), gamewidget(nullptr)
{
        // Resize refer to desktop
    resize( Size::SCREENA_SIZEW, Size::SCREENA_SIZEH );
    mainLay = new QVBoxLayout;
    mainLay->setMargin(0);
    view = new QQuickWidget(this);
    view->move(Size::APP_X, Size::APP_Y);
    view->rootContext()->setContextProperty("APP_X", Size::APP_X);
    view->rootContext()->setContextProperty("APP_Y", Size::APP_Y);
    view->rootContext()->setContextProperty("SCREENA_SIZEW", Size::SCREENA_SIZEW);
    view->rootContext()->setContextProperty("SCREENA_SIZEH", Size::SCREENA_SIZEH);
    view->rootContext()->setContextProperty("APP_SIZEW", Size::APP_SIZEW);
    view->rootContext()->setContextProperty("APP_SIZEH", Size::APP_SIZEH);
    view->rootContext()->setContextProperty("SIZE_FACTOR", Size::SIZE_FACTOR);

    view->setSource(QUrl("qrc:/main.qml"));
    if (view->status() == QQuickWidget::Error)
        qDebug() << "Error, view can't load main.qml source !!!";
    view->setResizeMode(QQuickWidget::SizeRootObjectToView);

    playersRessource = new PlayersRessource(this);
    networkProtocol  = new NetworkProtocol(playersRessource);
    gameplay         = new GamePlay(playersRessource);
    QObject *window = view->rootObject();


    connect(window, SIGNAL(joinLocalServer()), this, SLOT(switchToGameParty()));

    connect(networkProtocol, &NetworkProtocol::connectionRefusedBecauseOfNickname, this, &Widget::repromptNickname);
    connect(&errorPauseTimer, &QTimer::timeout, this, &Widget::resetNetworkAndEnableInitialMenu);

    connect(networkProtocol, &NetworkProtocol::startTheGame,
            this, &Widget::implStartTheGame);
    connect(networkProtocol, &NetworkProtocol::stopTheGameBecauseOfNotAllReady,
            this, &Widget::implStopTheGameBecauseOfNotAllReady);
    connect(networkProtocol, &NetworkProtocol::startTheGame, this, &Widget::implStartTheGame);

    connect(networkProtocol, &NetworkProtocol::allPlayersAreReady,
            this, &Widget::showStartTheGameButton);

    mainLay->addWidget(view);
    setLayout(mainLay);

}

Widget::~Widget()
{
}

void Widget::switchToGameParty()
{
    QString nick("Scott");
    QObject *nicknameInput = view->rootObject()->findChild<QObject*>("nicknameInput");
    if (nicknameInput){
        nick = nicknameInput->property("text").toString();
    }
    else qDebug() << "Error, nicknameInput is nullptr";


    networkProtocol->enable();
    networkProtocol->startConnection(nick);
    view->hide();
    if (!gamewidget){
        gamewidget = new GameWidget(playersRessource, 7, this);
        connect(gamewidget, &GameWidget::readyToStartTheGame, this, &Widget::readyToStartTheGame);
        connect(gamewidget, &GameWidget::gameStarted, this, &Widget::sendGameStarted);
        mainLay->addWidget(gamewidget);
    }
    view->hide();
    gamewidget->show();
}

void Widget::repromptNickname()
{
    qDebug() << "\n\n\nRETRY\n\n\n";
    QObject *nicknameRefusedLabel = view->rootObject()->findChild<QObject*>("nicknameRefusedLabel");
    if (nicknameRefusedLabel){
       nicknameRefusedLabel->setProperty("visible", true);
    }
    else qDebug() << "Erro, nicknameRefusedLabel is nicknameRefusedLabel";
    view->show();
    gamewidget->hide();
    view->rootObject()->setEnabled(false);
    errorPauseTimer.setInterval(2000);
    errorPauseTimer.setSingleShot(true);
    errorPauseTimer.start();
}

void Widget::resetNetworkAndEnableInitialMenu()
{
    view->rootObject()->setEnabled(true);
    networkProtocol->reset();
}

void Widget::readyToStartTheGame()
{
    qDebug() << "Ready to start the game!!!";
    networkProtocol->sendReadyToStartTheGame();
}

void Widget::sendGameStarted()
{
    implStartTheGame("me");
    networkProtocol->sendStartTheGame();
}

void Widget::implStartTheGame(QString nickname)
{
    qDebug() << "The GAME STARTED!!!";
    gamewidget->makeGameStarted();
    gameplay->startTheGame(nickname);
    qDebug() << "given nickname : " << nickname;
    gamewidget->updatePlayerTour(nickname);
}

void Widget::implStopTheGameBecauseOfNotAllReady()
{
    qDebug() << "The GAME STOPPED because not all players were ready...";
}

void Widget::showStartTheGameButton()
{

    qDebug() << "Showing start the game button!!";
    gamewidget->showStartTheGameButton();
}


