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

struct Answer{
    QList<QVariant> answers;
    QTimer timeOfFirstAnswer;
    bool answered = false;
};

struct ConnectionSettings{
    QString nickname;
    size_t numberOfPlayers;
    Answer AnswersForNumberOfPlayers;
    Answer AnswersForNicknameIsFine;
    bool accepted;
    QTimer errorPauseTimer;
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    QString &cleanMessagePonctuation(QString &msg);
    QString &cleanMessagePonctuation(QString &&msg);

signals:
    void numberOfPlayersAnswered(size_t answer);

public slots:
    void implNewMessage(const QString& peerNick, const QString& msg);
    void implNewParticipant(const QString &peerNick);
    void implParticipantLeft(const QString &peerNick);
    void switchToGameParty(QString);
    void updatePlayersNumberFromAnswers();
    void TryValidateNicknameFromPlayersAnswers();
    void promptNicknameAndtRetryValidation();
    void checkPlayersNumberAndTryValidateNickname();
    void timeOutCheckPlayersNumberAndTryValidateNickname();

    void resetAndEnableInitialMenu();

    void readyToStartTheGame();


private:
    Client client;

    GameWidget *gamewidget;
    InitialMenu *initialmenu;
    GamePlay gameplay;

    ConnectionSettings connectionSettings;
};

#endif // WIDGET_H
