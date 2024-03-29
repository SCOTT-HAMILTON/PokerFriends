﻿#ifndef NETWORKPROTOCOL_H
#define NETWORKPROTOCOL_H

#include <QObject>
#include <QTimer>
#include "networking/client.h"
#include "core/playersressource.h"

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
    bool ready;
    bool gameStarted;
};

class NetworkProtocol : public QObject
{
    Q_OBJECT
public:
    explicit NetworkProtocol(PlayersRessource *playersRessource, QObject *parent = nullptr);

signals:
    void newPlayerAdded(const QString&, const QString&);
    void maybeNewPlayer(const QString&, const QString&);
    void participantLeft(const QString &peerNick);
    void connectionRefusedBecauseOfNickname();
    void numberOfPlayersAnswered(size_t answer);
    void startTheGame(QString);
    void stopTheGameBecauseOfNotAllReady();
    void allPlayersAreReady();
    void waitingForPlayersToBeReady();

public slots:
    void analyzeNewMessage(const QString& peerNick, const QString& message);
    void enable();
    void reset();
    void startConnection(const QString &nickname);

    void tryValidateNicknameFromPlayersAnswers();
    void checkPlayersNumberAndTryValidateNickname();

    void removeParticipant(const QString &peerNick);

    void sendReadyToStartTheGame();
    void sendStartTheGame();

    //ANSWERS
    void updatePlayersNumberFromAnswers();
    void timeOutCheckPlayersNumberAndTryValidateNickname();

    /*UNUSED*/
    void newParticipant(const QString &peerNick);

private:
    Client client;
    PlayersRessource *playersRessource;

    QString &cleanMessagePonctuation(QString &msg);
    QString &cleanMessagePonctuation(QString &&msg);
    bool enabled;

    ConnectionSettings connectionSettings;


};

#endif // NETWORKPROTOCOL_H
