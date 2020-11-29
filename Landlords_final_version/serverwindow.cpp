#include "serverwindow.h"
#include "ui_serverwindow.h"

#include "playwindow.h"
#include "clientwindow.h"
#include "ToolsForNetworking.h"

ServerWindow::ServerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerWindow)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("Landlord - 斗地主 - 连接页面"));
    serverSocket = new QTcpServer();
    // ui->pushButton->click();
    // show local IP info
    auto ipVec = MyTools::getLocalIP();
    foreach (auto& ip, ipVec)
    {
        ui->infoListWidget->addItem(ip);
    }
}

ServerWindow::~ServerWindow()
{
    delete ui;
}

void ServerWindow::on_pushButton_clicked()
{
    serverSocket->close();
    while (true)
    {
        if (!serverSocket->listen(QHostAddress::Any, static_cast<quint16>(ui->portSpinBox->value())))
        {
            qDebug() << "error when listen port: " << serverSocket->errorString();
            continue;
        }
        connect(serverSocket, &QTcpServer::newConnection, this, &ServerWindow::acceptConnection);
        // confirmed = 0;
        myself = new ClientWindow();
        myself->setPort(ui->portSpinBox->value());
        myself->clickConnectButton();    // need to run after complete debugging!
        ui->pushButton->setEnabled(false);
        break;
    }
}

void ServerWindow::acceptConnection()
{
    qDebug() << "accepted a connection";
    sockets.push_back(serverSocket->nextPendingConnection());
    ui->listWidget->addItem(sockets.last()->peerAddress().toString() + ": " + QString::number(sockets.last()->peerPort()));
    if (sockets.size() == 3)
    {
        qDebug() << "Three players are here!";
        connect(&myTool, &MyTools::transferPackage, this, &ServerWindow::receivePackage);
        DataPackage sendId;
        sendId.sender_id = 0;  // use this variable to pass id info
        //sendId.msg << QString::number(QRandomGenerator::global()->bounded(2));
        foreach (auto* socket, sockets)
        {
            connect(socket, &QTcpSocket::readyRead, this, [=]{ myTool.read(socket); });
            myTool.send(socket, sendId);
            sendId.sender_id++;
        }
        qDebug() << "written";
        myself->move(this->pos());
        myself->resize(this->size());
        this->hide();
        //this->close();
    }
}

void ServerWindow::receivePackage(DataPackage data)
{
    qDebug() << "received one!";
    static int receivedMsg = 0;
    if (data.data_type == 0)
    {
        qDebug() << "confirmed: " << ++receivedMsg << data.id;
        if (receivedMsg == 6)
        {
            receivedMsg = 0;
            qDebug() << "all players has been ready!";
            // clear
            playerInfo.clear();
            cardToBeat.clear();
            playerToBeat = -1;
            // deal 17*3 cards
            // QVector<Card> cardVector;
            if (cardVector.empty())
            {
                QVector<QChar> cardPattern = { 'C', 'D', 'H', 'S' };
                foreach (auto& pattern, cardPattern)
                {
                    for (int i = 3; i <= 15; i++)
                    {
                        Card tmp(pattern, i);
                        cardVector.push_back(tmp);
                    }
                }
                Card tmp1('K', 16);
                cardVector.push_back(tmp1);
                Card tmp2('K', 17);
                cardVector.push_back(tmp2);
            }
            auto seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::shuffle(cardVector.begin(), cardVector.end(), std::default_random_engine(static_cast<unsigned>(seed)));
            for (int id = 0; id < 3; id++)
                playerInfo.push_back(Player_Info(id, 17, -1));
            DataPackage dealCard[3];
            for (int id = 0; id < 3; id++)
            {
                dealCard[id].data_type = 1;  // deal cards
                dealCard[id].player_info = playerInfo;
                for (int i = 0; i < 17; i++)
                    dealCard[id].cards.push_back(cardVector[i + 17 * id]);
            }
            for (int id = 0; id < 3; id++)
                myTool.send(sockets[id], dealCard[id]);
        }
    }
    else if (data.data_type == 1)
    {
        receivedMsg++;
        if (receivedMsg == 3)
        {
            receivedMsg = 0;
            // decide the landlord
            qDebug() << "start to decide the landlord";
            int randomPlayer = QRandomGenerator::global()->bounded(3);
            DataPackage dataToSend;
            dataToSend.data_type = 3;
            dataToSend.message << "landlord?";
            myTool.send(sockets[randomPlayer], dataToSend);
        }
    }
    else if (data.data_type == 3)
    {
        decideLandlord(data);
    }
    else if (data.data_type == 2)    // received a player's played cards
    {
        playerInfo[data.id].cardsRemain -= data.cards.size();   // update the player's info
        DataPackage dataToSend;
        dataToSend.data_type = 2;
        dataToSend.message << "wait";
        if (data.message[0] == "pass")
        {
            dataToSend.cards = cardToBeat;
            dataToSend.message << "passed" << QString::number(data.id);
            if (MyTools::nextId(data.sender_id) == playerToBeat)
                cardToBeat = dataToSend.cards = {};
        }
        else
        {
            cardToBeat = dataToSend.cards = data.cards; // save the cards played
            playerToBeat = data.sender_id;
            dataToSend.msg << QStringLiteral("出牌") << QString::number(data.id);
        }
        dataToSend.player_info = playerInfo;
        if (data.message.size() == 1)
        {
            myTool.send(sockets[data.sender_id], dataToSend);
            myTool.send(sockets[MyTools::nextId(data.sender_id + 1)], dataToSend);
            dataToSend.message[0] = "your turn";
            myTool.send(sockets[MyTools::nextId(data.sender_id)], dataToSend);
        }
        else
        {
            // WIN!
            dataToSend.msg.clear();
            dataToSend.msg << "end!" << playerInfo[data.id].role;
            for (int id = 0; id <= 2; id++)
                myTool.send(sockets[id], dataToSend);
        }
    }
}

void ServerWindow::decideLandlord(DataPackage data)
{
    static int landlord = -1;
    static QVector<int> phase;
    phase.push_back(data.sender_id);
    bool flag = false;
    if (data.message[0] == "yes")
    {
        landlord = data.sender_id;
        if (phase.size() == 3)  // can be decided
        {
            qDebug() << "decide the landlord: " << landlord;
            flag = true;
        }
        else
        {
            DataPackage dataToSend;
            dataToSend.data_type = 3;
            dataToSend.message << "landlord??";
            myTool.send(sockets[MyTools::nextId(data.id)], dataToSend);
        }
    }
    else
    {
        if (phase.size() == 3)  // can be decided
        {
            landlord = landlord == -1 ? phase[0] : landlord;
            qDebug() << "decide the landlord: " << landlord;
            flag = true;
        }
        else
        {
            DataPackage dataToSend;
            dataToSend.data_type = 3;
            if (landlord == -1)
                dataToSend.message << "landlord?";
            else
                dataToSend.message << "landlord??";
            myTool.send(sockets[MyTools::nextId(data.sender_id)], dataToSend);
        }
    }
    if (flag)
    {
        // boardcast the landlord and send the last 3 cards
        QList<Card> cardsAtBottom;
        for (auto iter = cardVector.rbegin(); iter != cardVector.rbegin() + 3; iter++)
        {
            cardsAtBottom << *iter;
        }
        MyTools::sortCards(cardsAtBottom);
        DataPackage decision;
        decision.data_type = 3;
        decision.msg << "have landlord";
        decision.cards = cardsAtBottom;
        for (int id = 0; id <= 2; id++)
        {
            if (id == landlord)
            {
                playerInfo[landlord].role = 0;//0 stands for landlord
                playerInfo[landlord].cards_remain += 3;
            }
            else
                playerInfo[id].role = 1;//1 stands for farmer
        }
        decision.player_info = playerInfo;
        for (int id = 0; id < 3; id++)
            myTool.send(sockets[id], decision);

        landlord = -1;
        phase.clear();
    }
}
