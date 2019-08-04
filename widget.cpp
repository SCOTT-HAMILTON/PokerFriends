#include "widget.h"
#include "ui_widget.h"
#include "size.h"

#include <QDebug>
#include <iostream>
#include <algorithm>
#include <QRegularExpression>
#include <map>

Widget::Widget(QWidget *parent) :
    QWidget(parent), gamewidget(nullptr), accepted(false),
    participantinit(false)
{

#ifdef Q_OS_ANDROID
    setFixedSize(Size::APP_SIZEW, Size::APP_SIZEH);
    resize(720, 1310);
#endif

    connect(&client, &Client::newMessage,
            this, &Widget::implNewMessage);

    connect(&client, &Client::newParticipant,
            this, &Widget::implNewParticipant);
    connect(&client, &Client::participantLeft,
            this, &Widget::implParticipantLeft);

    initialmenu = new InitialMenu(this);

    connect(initialmenu, &InitialMenu::joinServerRequest, this, &Widget::switchToGameParty);

    //ANSWERS
    AnswersForNumberOfPlayers.timeOfFirstAnswer.setInterval(1000);
    AnswersForNumberOfPlayers.timeOfFirstAnswer.setSingleShot(true);

    AnswersForNicknameIsFine.timeOfFirstAnswer.setInterval(1000);
    AnswersForNicknameIsFine.timeOfFirstAnswer.setSingleShot(true);

    //PARTICIPANT COUNT
    participantInitTimer.setInterval(0);
    participantInitTimer.setSingleShot(true);
    participantInitTimer.start();
    initialmenu->setEnabled(false);
    connect(&participantInitTimer, &QTimer::timeout, this, &Widget::finishParticipantCount);

    connect(&AnswersForNumberOfPlayers.timeOfFirstAnswer, &QTimer::timeout, this, &Widget::updatePlayersNumberFromAnswers);
    connect(this, &Widget::numberOfPlayersAnswered, this, &Widget::checkPlayersNumberAndTryValidateNickname);

    connect(&AnswersForNicknameIsFine.timeOfFirstAnswer, &QTimer::timeout, this, &Widget::timeOutCheckPlayersNumberAndTryValidateNickname);

    connect(&pauseTimer, &QTimer::timeout, this, &Widget::resetAndEnableInitialMenu);
}

Widget::~Widget()
{
}

QString &Widget::cleanMessagePonctuation(QString &msg)
{
    return msg.remove(QRegularExpression("[.!?]"));
}

QString &Widget::cleanMessagePonctuation(QString &&msg)
{
    return msg.remove(QRegularExpression("[.!?]"));
}

void Widget::implNewMessage(const QString &peerNick, const QString &message)
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
    if (!gamewidget || !gamewidget->isVisible()){
        qDebug() << "received message but not yet ready... here is it : ";
        qDebug() << msg << " from " << peerNick;
        return;
    }
    qDebug() << "NEW MESSAGE from " << peerNick << " : " << msg;
    if (!AnswersForNicknameIsFine.answered){
        qDebug() << "number of answers for nickname is fine before : " << AnswersForNicknameIsFine.answers.size();
    }
    //MESSAGE recognition
    if (msg.startsWith("Hello, I'm ")){
        qDebug() << "A new player entered in the party!! Let's welcome him";
        QString playerNickname = msg.mid(11, -1);
        qDebug() << "nickname is " << playerNickname;
        gameplay.addPlayer(playerNickname, peerNick);
        gamewidget->fetchPlayers(gameplay.getPlayers());
        client.sendMessage("Welcome "+playerNickname+", I'm "+nickname);//W
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
        if (welcomedPlayer == nickname && !gameplay.playerExist(peerNick)){
            qDebug() << "Player welcomed us, his name is " << welcomingPlayer;
            gameplay.addPlayer(welcomingPlayer, peerNick);
            gamewidget->fetchPlayers(gameplay.getPlayers());
        }else{
            qDebug() << "Player welcomed us, we already now him, it's " << welcomingPlayer;
        }
    }
    else if (msg.startsWith("Is there any ")){
        QString playerNickname = msg.mid(13, -1);
        if (gameplay.nicknameExists(peerNick) || playerNickname == nickname){
            qDebug() << "Yes, sending that the nickname " << playerNickname << "exists.";
            client.sendMessage("This nick already exists "+playerNickname);//
        }else{
            qDebug() << "No, sending that the nickname " << playerNickname << " doesn't exist.";
            client.sendMessage("This nick is fine "+playerNickname);//
        }
    }else if(msg.startsWith("How many players are you")){//
        if (accepted){
            qDebug() << "Sending the number of players which is " << gameplay.playersCount()+1;
            client.sendMessage("The number of players is "+QString::number(gameplay.playersCount()+1));//
        }else{
            qDebug() << "Not yet accepted can't answer to the question How many players are you.";
        }
    }else if(msg.startsWith("The number of players is ")){//
        QString number = msg.mid(25, -1);
        bool ok=false;
        int num = 0;
        num = number.toInt(&ok);
        if ( ok == true){
            qDebug() << "Number of players would be " << num;

            if (!accepted){
                if (AnswersForNumberOfPlayers.answers.size() == 0){
                    qDebug() << "First answer for players number, initiate timer.";
                    if (AnswersForNumberOfPlayers.timeOfFirstAnswer.isActive()){
                        qDebug() << "Timer for players number is already running, stopping it...";
                        AnswersForNumberOfPlayers.timeOfFirstAnswer.stop();
                    }
                    qDebug() << "starting timer players number answers timer.";
                    AnswersForNumberOfPlayers.timeOfFirstAnswer.start();
                }
                qDebug() << "adding answer " << num << " from " << peerNick << " to players Number answers";
                AnswersForNumberOfPlayers.answers.append(num);
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
        if (!accepted){
            if (playerNickname == nickname){
                qDebug() << "One more validation that our nickname is fine.";
                AnswersForNicknameIsFine.answers.append(true);
                checkPlayersNumberAndTryValidateNickname();
            }else{
                qDebug() << "This isn't our nickname " << playerNickname << ", our is " << nickname;
            }
        }else{
            qDebug() << "Already accepted, we don't care of nickname validation.";
        }

    }else if (msg.startsWith("This nick already exists ")){//
        QString playerNickname = msg.mid(25, -1);
        qDebug() << "Player nickname " << playerNickname << " seems to already be used.";
        if (!accepted){
            if (playerNickname == nickname){
                qDebug() << "One more answer, our nickname isn't fine.";
                AnswersForNicknameIsFine.answers.append(false);
                checkPlayersNumberAndTryValidateNickname();
            }else{
                qDebug() << "This isn't our nickname " << playerNickname << ", our is " << nickname;
            }
        }else{
            qDebug() << "Already accepted, we don't care of nickname validation.";
        }
    }
    if (!AnswersForNicknameIsFine.answered){
        qDebug() << "number of answers for nickname is fine after : " << AnswersForNicknameIsFine.answers.size();
    }
   qDebug() << gameplay.playersReport();
}

void Widget::implNewParticipant(const QString &peerNick)
{
    qDebug() << "NEW PARTICIPANT : " << peerNick;
}

void Widget::implParticipantLeft(const QString &peerNick)
{
    qDebug() << "A PARTICIPANT LEFT : " << peerNick;
    if (gameplay.playerExist(peerNick)){
        gameplay.removePlayer(peerNick);
        gamewidget->fetchPlayers(gameplay.getPlayers());
        std::cerr << gameplay.playersReport().toStdString() << '\n';
    }else{
        qDebug() << "unknow player, can't remove it (!";
    }
}

void Widget::switchToGameParty(QString nick)
{

    QString devicename = client.nickName();
    nickname = nick;
    client.sendMessage("How many players are you ?");
    AnswersForNumberOfPlayers.timeOfFirstAnswer.start();
    client.sendMessage("Is there any "+nickname+'?');
    AnswersForNicknameIsFine.timeOfFirstAnswer.start();
    initialmenu->hide();
    gamewidget = new GameWidget(7, this);
    gamewidget->fetchPlayers(gameplay.getPlayers());
    gamewidget->show();
}

void Widget::updatePlayersNumberFromAnswers()
{
    QList<QVariant> PlayersAnswers = AnswersForNumberOfPlayers.answers;
    AnswersForNumberOfPlayers.answers.clear();
    if (PlayersAnswers.size() == 0){
        AnswersForNumberOfPlayers.answered = true;
        qDebug() << "zero answers for Number of Players, setting it to 0";
        numberOfPlayers = 0;
        emit numberOfPlayersAnswered(numberOfPlayers);
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
    AnswersForNumberOfPlayers.answered = true;
    numberOfPlayers = static_cast<size_t>(answer);
    emit numberOfPlayersAnswered(numberOfPlayers);
}

void Widget::TryValidateNicknameFromPlayersAnswers()
{
    if (accepted){
        qDebug() << "No need to try validate nickname, we're already accepted.";
        return;
    }
    QList<QVariant> PlayersAnswers = AnswersForNicknameIsFine.answers;
    AnswersForNicknameIsFine.answers.clear();
    if (PlayersAnswers.size() == 0){
        AnswersForNicknameIsFine.answered = true;
        qDebug() << "\n\nZERO LOL answers for nickname is fine, accepting ourself\n\n";
        client.sendMessage("Hello, I'm "+nickname);
        accepted = true;
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
        if (!accepted){
            qDebug() << "\n\NICKNAME is validated\n\n";
            client.sendMessage("Hello, I'm "+nickname);
            accepted = true;
        }
    }else{
        qDebug() << "Nickname is refused";
        promptNicknameAndtRetryValidation();
    }
    AnswersForNicknameIsFine.answered = true;
}

void Widget::promptNicknameAndtRetryValidation()
{
    qDebug() << "\n\n\nRETRY\n\n\n";
    initialmenu->showNicknameRefusedLabel();
    AnswersForNumberOfPlayers.timeOfFirstAnswer.stop();
    AnswersForNumberOfPlayers.answered = false;
    AnswersForNumberOfPlayers.answers.clear();

    AnswersForNicknameIsFine.timeOfFirstAnswer.stop();
    AnswersForNicknameIsFine.answered = false;
    if (AnswersForNicknameIsFine.answered){
        qDebug() << "Err, retry can't unanswered...";
        return;
    }else{
        qDebug() << "Yep, unanswered...";
    }
    AnswersForNicknameIsFine.answers.clear();
    accepted = false;
    numberOfPlayers = 0;
    initialmenu->show();
    gamewidget->hide();
    initialmenu->setEnabled(false);
    pauseTimer.setInterval(2000);
    pauseTimer.setSingleShot(true);
    pauseTimer.start();
}

void Widget::checkPlayersNumberAndTryValidateNickname()
{
    qDebug() << "CHECK and try validate nickname";
    if (AnswersForNicknameIsFine.answered){
        qDebug() << "FUCK FUCK FUCK No need to check, already answered";
        return;
    }
    if (AnswersForNumberOfPlayers.answered &&
            AnswersForNicknameIsFine.answers.size() >= static_cast<int>(numberOfPlayers)){
        qDebug() << "CHECK and try validate nickname, "
                    "As the number of given answers for nickname is fine is greater than or equal as the number of players";
        qDebug() << "CHECK and try validate nickname"
                    "we will try to validate the answers given.";
        if (AnswersForNicknameIsFine.answers.size() > static_cast<int>(numberOfPlayers)){
            qDebug() << "strange, number of answers bigger than the number of players.. never mind : ";
            qDebug() << "is the Number of players answered? : " << AnswersForNumberOfPlayers.answered;
            qDebug() << "Number of answers for nickname is valid : " << AnswersForNicknameIsFine.answers.size();
            qDebug() << "Number of players (if answered) : " << numberOfPlayers;
        }
        TryValidateNicknameFromPlayersAnswers();
    }else{
        qDebug() << "Not enough answers for nickname is valid or number of players unknown yet : ";
        qDebug() << "Number of players answered : " << AnswersForNumberOfPlayers.answered;
        qDebug() << "Number of answers for nickname is valid : " << AnswersForNicknameIsFine.answers.size();
        qDebug() << "Number of players (if answered) : " << numberOfPlayers;
    }
}

void Widget::timeOutCheckPlayersNumberAndTryValidateNickname()
{
    //IF time is out and no answers were given, we automatically accept ourself
    if (accepted){
        qDebug() << "No need to try validate nickname, we're already accepted.";
        return;
    }
    if (AnswersForNicknameIsFine.answers.size() == 0){
        AnswersForNicknameIsFine.answered = true;
        qDebug() << "\n\nZERO TIME OUT answers for nickname is fine and time is out, accepting ourself\n\n";
        client.sendMessage("Hello, I'm "+nickname);
        accepted = true;
        return;
    }
    checkPlayersNumberAndTryValidateNickname();
}

void Widget::finishParticipantCount()
{
    participantinit = true;
    initialmenu->setEnabled(true);
}

void Widget::resetAndEnableInitialMenu()
{
    AnswersForNumberOfPlayers.timeOfFirstAnswer.stop();
    AnswersForNumberOfPlayers.answered = false;
    AnswersForNumberOfPlayers.answers.clear();

    AnswersForNicknameIsFine.timeOfFirstAnswer.stop();
    AnswersForNicknameIsFine.answered = false;
    if (AnswersForNicknameIsFine.answered){
        qDebug() << "Err, retry can't unanswered...";
        return;
    }else{
        qDebug() << "Yep, unanswered...";
    }
    AnswersForNicknameIsFine.answers.clear();
    accepted = false;
    numberOfPlayers = 0;
    initialmenu->setEnabled(true);
}


