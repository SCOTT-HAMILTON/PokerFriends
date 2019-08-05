#include "networkprotocol.h"

#include <QDebug>
#include <QRegularExpression>

NetworkProtocol::NetworkProtocol(PlayersRessource *playersRessource, QObject *parent) : QObject(parent),
    playersRessource(playersRessource), enabled(false)
{
    //SHOULD BE ENABLE WHEN GAME WIDGET IS READY AND SHOWED
    connectionSettings.accepted = false;

    //CLIENT
    connect(&client, &Client::newMessage,
            this, &NetworkProtocol::analyzeNewMessage);
    connect(&client, &Client::newParticipant,
            this, &NetworkProtocol::newParticipant);
    connect(&client, &Client::participantLeft,
            this, &NetworkProtocol::removeParticipant);

    //ANSWERS
    connectionSettings.AnswersForNumberOfPlayers.timeOfFirstAnswer.setInterval(1000);
    connectionSettings.AnswersForNumberOfPlayers.timeOfFirstAnswer.setSingleShot(true);

    connectionSettings.AnswersForNicknameIsFine.timeOfFirstAnswer.setInterval(1000);
    connectionSettings.AnswersForNicknameIsFine.timeOfFirstAnswer.setSingleShot(true);

    //PARTICIPANT COUNT

    connect(&connectionSettings.AnswersForNumberOfPlayers.timeOfFirstAnswer, &QTimer::timeout, this, &NetworkProtocol::updatePlayersNumberFromAnswers);
    connect(this, &NetworkProtocol::numberOfPlayersAnswered, this, &NetworkProtocol::checkPlayersNumberAndTryValidateNickname);

    connect(&connectionSettings.AnswersForNicknameIsFine.timeOfFirstAnswer, &QTimer::timeout, this, &NetworkProtocol::timeOutCheckPlayersNumberAndTryValidateNickname);

}

void NetworkProtocol::analyzeNewMessage(const QString &peerNick, const QString &message)
{
    /*
     * Helle, I'm <nickname>
     * Welcome <nickname>, I'm <nickname>
     * Is there any <nickname>
     * This nick is fine <nickname>
     * This nick already exists
     * How many players are you?
     * The number of players is
    */
    QString msg = message;
    msg = cleanMessagePonctuation(msg);
    if (!enabled){
        qDebug() << "received message but not yet ready... here is it : ";
        qDebug() << msg << " from " << peerNick;
        return;
    }
    qDebug() << "NEW MESSAGE from " << peerNick << " : " << msg;
    if (!connectionSettings.AnswersForNicknameIsFine.answered){
        qDebug() << "number of answers for nickname is fine before : " << connectionSettings.AnswersForNicknameIsFine.answers.size();
    }
    //MESSAGE recognition
    if (msg.startsWith("Hello, I'm ")){
        qDebug() << "A new player entered in the party!! Let's welcome him";
        QString playerNickname = msg.mid(11, -1);
        qDebug() << "nickname is " << playerNickname;
        playersRessource->addPlayer(playerNickname, peerNick);
        /* TODO TO TEST CONNECTION WITH WIDGET*/
        emit newPlayerAdded(playerNickname, peerNick);

        client.sendMessage("Welcome "+playerNickname+", I'm "+connectionSettings.nickname);//W
    }
    else if (msg.startsWith("Welcome ")){//W
        int pos = msg.indexOf(',');
        if (pos < 9){
            qDebug() << "Erreur, name is too short or inexistant...or invalid message was given.";
            return;
        }
        QString welcomedPlayer = msg.mid(8, pos-8);
        qDebug() << "welcomed player : " << welcomedPlayer;
        QString welcomingPlayer = msg.split(" ").last();
        qDebug() << "welcoming player is " << welcomingPlayer;

        if (welcomedPlayer == connectionSettings.nickname){
            if (!playersRessource->playerExist(peerNick)){
                playersRessource->addPlayer(welcomingPlayer, peerNick);
                emit newPlayerAdded(welcomingPlayer, peerNick);
            }
            qDebug() << "Player welcomed us, his name is " << welcomingPlayer;


        }else{
            qDebug() << "Player welcomed us, we already now him, it's " << welcomingPlayer;
        }
    }
    else if (msg.startsWith("Is there any ")){
        QString playerNickname = msg.mid(13, -1);
        if (playersRessource->nicknameExists(peerNick) || playerNickname == connectionSettings.nickname){
            qDebug() << "Yes, sending that the nickname " << playerNickname << "exists.";
            client.sendMessage("This nick already exists "+playerNickname);//
        }else{
            qDebug() << "No, sending that the nickname " << playerNickname << " doesn't exist.";
            client.sendMessage("This nick is fine "+playerNickname);//
        }
    }else if(msg.startsWith("How many players are you")){//
        if (connectionSettings.accepted){
            qDebug() << "Sending the number of players which is " << playersRessource->playersCount()+1;
            client.sendMessage("The number of players is "+QString::number(playersRessource->playersCount()+1));//
        }else{
            qDebug() << "Not yet connectionSettings.accepted can't answer to the question How many players are you.";
        }
    }else if(msg.startsWith("The number of players is ")){//
        QString number = msg.mid(25, -1);
        bool ok=false;
        int num = 0;
        num = number.toInt(&ok);
        if ( ok == true){
            qDebug() << "Number of players would be " << num;

            if (!connectionSettings.accepted){
                if (connectionSettings.AnswersForNumberOfPlayers.answers.size() == 0){
                    qDebug() << "First answer for players number, initiate timer.";
                    if (connectionSettings.AnswersForNumberOfPlayers.timeOfFirstAnswer.isActive()){
                        qDebug() << "Timer for players number is already running, stopping it...";
                        connectionSettings.AnswersForNumberOfPlayers.timeOfFirstAnswer.stop();
                    }
                    qDebug() << "starting timer players number answers timer.";
                    connectionSettings.AnswersForNumberOfPlayers.timeOfFirstAnswer.start();
                }
                qDebug() << "adding answer " << num << " from " << peerNick << " to players Number answers";
                connectionSettings.AnswersForNumberOfPlayers.answers.append(num);
                /*
                 * TODO :
                 * Answers system
                 * > answers validation
                 * tests
                 * accept validation
                 * > good number of players
                 * > nickname duplicatio avoidance
                 * Tests
                 *
                 * players UI update from fetched players
                 *
               */
            }
        }
    }else if (msg.startsWith("This nick is fine ")){//
        QString playerNickname = msg.mid(18, -1);
        qDebug() << "Player nickname " << playerNickname << " seems to be fine.";
        if (!connectionSettings.accepted){
            if (playerNickname == connectionSettings.nickname){
                qDebug() << "One more validation that our nickname is fine.";
                connectionSettings.AnswersForNicknameIsFine.answers.append(true);
                checkPlayersNumberAndTryValidateNickname();
            }else{
                qDebug() << "This isn't our nickname " << playerNickname << ", our is " << connectionSettings.nickname;
            }
        }else{
            qDebug() << "Already connectionSettings.accepted, we don't care of nickname validation.";
        }

    }else if (msg.startsWith("This nick already exists ")){//
        QString playerNickname = msg.mid(25, -1);
        qDebug() << "Player nickname " << playerNickname << " seems to already be used.";
        if (!connectionSettings.accepted){
            if (playerNickname == connectionSettings.nickname){
                qDebug() << "One more answer, our nickname isn't fine.";
                connectionSettings.AnswersForNicknameIsFine.answers.append(false);
                checkPlayersNumberAndTryValidateNickname();
            }else{
                qDebug() << "This isn't our nickname " << playerNickname << ", our is " << connectionSettings.nickname;
            }
        }else{
            qDebug() << "Already connectionSettings.accepted, we don't care of nickname validation.";
        }
    }
    if (!connectionSettings.AnswersForNicknameIsFine.answered){
        qDebug() << "number of answers for nickname is fine after : " << connectionSettings.AnswersForNicknameIsFine.answers.size();
    }
    qDebug() << playersRessource->playersReport();
}

void NetworkProtocol::enable()
{
    enabled = true;
}

void NetworkProtocol::reset()
{
    connectionSettings.AnswersForNumberOfPlayers.timeOfFirstAnswer.stop();
    connectionSettings.AnswersForNumberOfPlayers.answered = false;
    connectionSettings.AnswersForNumberOfPlayers.answers.clear();

    connectionSettings.AnswersForNicknameIsFine.timeOfFirstAnswer.stop();
    connectionSettings.AnswersForNicknameIsFine.answered = false;
    connectionSettings.AnswersForNicknameIsFine.answers.clear();
    connectionSettings.accepted = false;
    connectionSettings.numberOfPlayers = 0;
}

void NetworkProtocol::startConnection(const QString &nickname)
{
    connectionSettings.nickname = nickname;
    client.sendMessage("How many players are you ?");
    connectionSettings.AnswersForNumberOfPlayers.timeOfFirstAnswer.start();
    client.sendMessage("Is there any "+connectionSettings.nickname+'?');
    connectionSettings.AnswersForNicknameIsFine.timeOfFirstAnswer.start();
}

void NetworkProtocol::tryValidateNicknameFromPlayersAnswers()
{
    if (connectionSettings.accepted){
        qDebug() << "No need to try validate nickname, we're already connectionSettings.accepted.";
        return;
    }
    QList<QVariant> PlayersAnswers = connectionSettings.AnswersForNicknameIsFine.answers;
    connectionSettings.AnswersForNicknameIsFine.answers.clear();
    if (PlayersAnswers.size() == 0){
        connectionSettings.AnswersForNicknameIsFine.answered = true;
        qDebug() << "\n\nZERO LOL answers for nickname is fine, accepting ourself\n\n";
        client.sendMessage("Hello, I'm "+connectionSettings.nickname);
        connectionSettings.accepted = true;
        return;
    }

    std::map<bool, int> answers;
    for (int i = 0; i < PlayersAnswers.size(); ++i){
        bool answer = PlayersAnswers[i].toBool();
        if ( answers.find(answer) != answers.end()){
            ++answers[answer];
        }else{
            answers.insert({answer, 1});
        }
    }

    qDebug() <<"answers count by their value : ";
    std::for_each(answers.begin(), answers.end(), [](const std::pair<bool, int>& p){
            qDebug() << "ANSWER " << p.first << " given " << p.second << " times";
    });
    auto it = std::max_element(answers.begin(), answers.end(), [](const std::pair<bool,int> &p1, const std::pair<bool,int>& p2) {
        return p1.second < p2.second; });
    if (it == answers.end()){
        qDebug() << "error, Widget TryValidateNicknameFromPlayersAnswers, max_element not found.";
        return;
    }
    bool answer = it->first;
    qDebug() << "Most given answer is, nickname is valid " << answer;
    if (answer){
        if (!connectionSettings.accepted){
            qDebug() << "\n\nNICKNAME is validated\n\n";
            client.sendMessage("Hello, I'm "+connectionSettings.nickname);
            connectionSettings.accepted = true;
        }
    }else{
        qDebug() << "Nickname is refused";
        reset();
        emit connectionRefusedBecauseOfNickname();
    }
    connectionSettings.AnswersForNicknameIsFine.answered = true;
}

void NetworkProtocol::checkPlayersNumberAndTryValidateNickname()
{
    qDebug() << "CHECK and try validate nickname";
    if (connectionSettings.AnswersForNicknameIsFine.answered){
        qDebug() << "FUCK FUCK FUCK No need to check, already answered";
        return;
    }
    if (connectionSettings.AnswersForNumberOfPlayers.answered &&
            connectionSettings.AnswersForNicknameIsFine.answers.size() >= static_cast<int>(connectionSettings.numberOfPlayers)){
        qDebug() << "CHECK and try validate nickname, "
                    "As the number of given answers for nickname is fine is greater than or equal as the number of players";
        qDebug() << "CHECK and try validate nickname"
                    "we will try to validate the answers given.";
        if (connectionSettings.AnswersForNicknameIsFine.answers.size() > static_cast<int>(connectionSettings.numberOfPlayers)){
            qDebug() << "strange, number of answers bigger than the number of players.. never mind : ";
            qDebug() << "is the Number of players answered? : " << connectionSettings.AnswersForNumberOfPlayers.answered;
            qDebug() << "Number of answers for nickname is valid : " << connectionSettings.AnswersForNicknameIsFine.answers.size();
            qDebug() << "Number of players (if answered) : " << connectionSettings.numberOfPlayers;
        }
        tryValidateNicknameFromPlayersAnswers();
    }else{
        qDebug() << "Not enough answers for nickname is valid or number of players unknown yet : ";
        qDebug() << "Number of players answered : " << connectionSettings.AnswersForNumberOfPlayers.answered;
        qDebug() << "Number of answers for nickname is valid : " << connectionSettings.AnswersForNicknameIsFine.answers.size();
        qDebug() << "Number of players (if answered) : " << connectionSettings.numberOfPlayers;
    }
}

void NetworkProtocol::removeParticipant(const QString &peerNick)
{
    qDebug() << "A PARTICIPANT LEFT : " << peerNick;
    if (playersRessource->playerExist(peerNick)){
        playersRessource->removePlayer(peerNick);
        emit participantLeft(peerNick);
        qDebug() << playersRessource->playersReport() << '\n';
    }else{
        qDebug() << "unknow player, can't remove it (!";
    }
}

void NetworkProtocol::updatePlayersNumberFromAnswers()
{
    QList<QVariant> PlayersAnswers = connectionSettings.AnswersForNumberOfPlayers.answers;
    connectionSettings.AnswersForNumberOfPlayers.answers.clear();
    if (PlayersAnswers.size() == 0){
        connectionSettings.AnswersForNumberOfPlayers.answered = true;
        qDebug() << "zero answers for Number of Players, setting it to 0";
        connectionSettings.numberOfPlayers = 0;
        emit numberOfPlayersAnswered(connectionSettings.numberOfPlayers);
        return;
    }
    qDebug() << "answers timer is finished, there is " << PlayersAnswers.size() << " answer(s).";
    std::map<int, int> answers;

    for (int i = 0; i < PlayersAnswers.size(); ++i){
        bool ok = false;
        int answer = PlayersAnswers[i].toInt(&ok);
        if (!ok){
            qDebug() << "error, Widget updatePlayersNumberFromAnswers, conversion error QVariant.";
            return;
        }
        if ( answers.find(answer) != answers.end()){
            ++answers[answer];
        }else{
            answers.insert({answer, 1});
        }
    }

    qDebug() <<"answers count by their value : ";
    std::for_each(answers.begin(), answers.end(), [](const std::pair<int, int>& p){
            qDebug() << "ANSWER " << p.first << " given " << p.second << " times";
    });
    auto it = std::max_element(answers.begin(), answers.end(), [](const std::pair<int,int> &p1, const std::pair<int,int>& p2) {
        return p1.second < p2.second; });
    if (it == answers.end()){
        qDebug() << "error, Widget updatePlayersNumberFromAnswers, max_element not found.";
        return;
    }
    int answer = it->first;
    qDebug() << "Most given answer is, number of player is " << answer;
    if (answer<0){
        qDebug() << "Error, number of players is negative " << answer;
        return;
    }if (answer>50){
        qDebug() << "Error, number of players is too big " << answer;
        return;
    }
    connectionSettings.AnswersForNumberOfPlayers.answered = true;
    connectionSettings.numberOfPlayers = static_cast<size_t>(answer);
    emit numberOfPlayersAnswered(connectionSettings.numberOfPlayers);
}

void NetworkProtocol::timeOutCheckPlayersNumberAndTryValidateNickname()
{
    //IF time is out and no answers were given, we automatically accept ourself
    if (connectionSettings.accepted){
        qDebug() << "No need to try validate nickname, we're already accepted.";
        return;
    }
    if (connectionSettings.AnswersForNicknameIsFine.answers.size() == 0){
        connectionSettings.AnswersForNicknameIsFine.answered = true;
        qDebug() << "\n\nZERO TIME OUT answers for nickname is fine and time is out, accepting ourself\n\n";
        client.sendMessage("Hello, I'm "+connectionSettings.nickname);
        connectionSettings.accepted = true;
        return;
    }
    checkPlayersNumberAndTryValidateNickname();
}

void NetworkProtocol::newParticipant(const QString &peerNick)
{
    qDebug() << "NEW PARTICIPANT : " << peerNick;
}


QString &NetworkProtocol::cleanMessagePonctuation(QString &msg)
{
    return msg.remove(QRegularExpression("[.!?]"));
}

QString &NetworkProtocol::cleanMessagePonctuation(QString &&msg)
{
    return msg.remove(QRegularExpression("[.!?]"));
}
