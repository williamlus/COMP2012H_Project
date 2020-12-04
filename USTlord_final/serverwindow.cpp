#include "serverwindow.h"
#include "ui_serverwindow.h"
#include <QMessageBox>
#include <QRandomGenerator>

ServerWindow::ServerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerWindow)
{
    ui->setupUi(this);
}

void ServerWindow::closeEvent(QCloseEvent *event)
{
    on_pushButton_stop_clicked();
    qDebug() << "Closing server window";
    //show the mainwindow
    this->parentWidget()->show();
    //allow to close the second window
    event->accept();
}

QVector<QString> ServerWindow::get_local_IP() const
{
    QVector<QString> ipv4, ipv6;
    QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
    foreach (auto addr, info.addresses())
    {
        if (addr.protocol() == QAbstractSocket::IPv4Protocol)
            ipv4 << addr.toString();
        else if (addr.protocol() == QAbstractSocket::IPv6Protocol)
            ipv6 << addr.toString();
    }
    ipv4 << ipv6;
    return ipv4;

}

ServerWindow::~ServerWindow()
{
    delete ui;
}

void ServerWindow::on_pushButton_stop_clicked()
{
    if(server){
        ui->listWidget_dialogs->addItem("Closing server");
        this->server->close();
    }
    ui->listWidget_dialogs->addItem("Disconnect all "+QString::number(clients.size())+" clients");
    for(int i=0;i<clients.size();++i){
        //disconnect signal and slot
        disconnect(clients[i],&QTcpSocket::stateChanged,this,&ServerWindow::handleStateChanged);
        disconnect(clients[i],&QTcpSocket::readyRead,this,&ServerWindow::handle_clients_message);
        clients[i]->close();
        clients[i]->deleteLater();
        clients.remove(i);
        --i;
    }
    ui->listWidget_dialogs->addItem("All clients closed");
    ui->listWidget_dialogs->addItem("Clients left: " + QString::number(clients.size()));
    ui->pushButton_create->setEnabled(true);
}

void ServerWindow::on_pushButton_create_clicked()
{
    if(server==nullptr){
        server=new QTcpServer(this);
        ui->listWidget_dialogs->addItem("Server is created");
        connect(server,&QTcpServer::newConnection,this,&ServerWindow::handleConnection);
        //list all ip addresses
        auto ipVec = get_local_IP();
        foreach (auto& ip, ipVec){
            ui->listWidget_serverIP->addItem(ip);
        }
        ////////////////////////
        //create a client window
        client_window=new ClientWindow(this);
        client_window->show();
        client_window->set_port(ui->lineEdit_port->text());
        client_window->set_serverIP("127.0.0.1");
        client_window->on_pushButton_join_server_clicked();
    }
    server->listen(QHostAddress::Any,ui->lineEdit_port->text().toInt());
    if(server->isListening()){
        ui->listWidget_dialogs->addItem("Server is listening to port: "+ui->lineEdit_port->text());
        ui->pushButton_create->setEnabled(false);
    }
    else{
        ui->listWidget_dialogs->addItem("Server fails to listen");
    }

    ///////////////
    //debugging
//    DataPackage dp(1,2,DataPackage::Action::CHAT,DataPackage::Content::WIN_GAME);
//    ui->listWidget_dialogs->addItem(dp.to_string());////
//    ui->listWidget_dialogs->addItem("After parsing");
//    ui->listWidget_dialogs->addItem(DataPackage::parse(dp.serialize()).to_string());
//    DataPackage dd(1,2,DataPackage::Action::PLAY_CARDS,"s3,h4,d5,d6,rW,bW,s2,cA");
//    ui->listWidget_dialogs->addItem(dd.to_string());////
//    ui->listWidget_dialogs->addItem("After parsing");
//    ui->listWidget_dialogs->addItem(DataPackage::parse(dd.serialize()).to_string());
}

void ServerWindow::on_pushButton_start_game_clicked()
{
    if(clients.size()==3){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this,"START",tr("Start game? :)"),QMessageBox::Yes|QMessageBox::No);
        if(reply==QMessageBox::Yes){
            qDebug()<<"Three players are ready!";
            give_id();
            //this->hide();
        }
        else{
            qDebug()<<"Players refuse to start game >.<";
            return;
        }

    }
}

void ServerWindow::handleConnection()
{
    QTcpSocket* client=server->nextPendingConnection();
    connect(client,&QTcpSocket::stateChanged,this,&ServerWindow::handleStateChanged);
    connect(client,&QTcpSocket::readyRead,this,&ServerWindow::handle_clients_message);
    this->clients.push_back(client);
    ui->listWidget_dialogs->addItem("New connection"+client->peerAddress().toString() + ":" + QString::number(client->peerPort())+ " accepted");
    ui->listWidget_dialogs->addItem("Clients left: " + QString::number(clients.size()));
//    //debugging
//    DataPackage dd(-2,-2,DataPackage::Action::PLAY_CARDS,"s3,h4,d5,d6,rW,bW,s2,cA");
//    client->write(dd.serialize());
}

void ServerWindow::handleStateChanged(QAbstractSocket::SocketState state)
{
    for(int i=0;i<clients.size();++i){
        if(clients[i]->state()!=state){continue;}
        if(state==QAbstractSocket::SocketState::ConnectedState){
            qDebug() << clients[i]->peerAddress().toString() << ":" << QString(clients[i]->peerPort()) << " connected";
            ui->listWidget_dialogs->addItem(clients[i]->peerAddress().toString() + ":" + QString::number(clients[i]->peerPort()) + " connected");
        }
        else if(state==QAbstractSocket::SocketState::UnconnectedState){
            qDebug() << "Removed " << clients[i]->peerAddress().toString() << ":" << clients[i]->peerPort();
            ui->listWidget_dialogs->addItem("Removed " + clients[i]->peerAddress().toString() + ":" + QString::number(clients[i]->peerPort()));
            //disconnect signal and slot
            disconnect(clients[i],&QTcpSocket::stateChanged,this,&ServerWindow::handleStateChanged);
            disconnect(clients[i],&QTcpSocket::readyRead,this,&ServerWindow::handle_clients_message);
            clients[i]->close();
            clients[i]->deleteLater();
            clients.remove(i);
            --i;
            qDebug() << "Clients left: " << clients.size();
            ui->listWidget_dialogs->addItem("Clients left: " + QString::number(clients.size()));
        }
    }
}

void ServerWindow::handle_clients_message()
{
    for(int i=0;i<clients.size();++i){
        if(clients[i]->bytesAvailable()>0){
            QByteArray arr=clients[i]->readAll();
            QString msg=QString::fromStdString(arr.toStdString());
            QString addr=clients[i]->peerAddress().toString() + ":" + QString::number(clients[i]->peerPort());
            ui->listWidget_dialogs->addItem(addr+" : "+msg);
            DataPackage data(msg);
            receiveData(data);
        }
    }


}

void ServerWindow::sendData(QTcpSocket* socket, DataPackage data)
{
    QByteArray arr=data.serialize();

        socket->write(arr);
        ui->listWidget_dialogs->addItem("Send text to client: " + socket->peerAddress().toString() + ":" + QString::number(socket->peerPort()));
        ui->listWidget_dialogs->addItem(data.to_string());//////////////////
}

void ServerWindow::give_id()
{
    for(int i=0;i<3;++i){
        DataPackage confirm_data(-1,-1,DataPackage::Action::GIVE_ID,QString::number(i));//////////
        sendData(clients[i],confirm_data);

    }
}

void ServerWindow::confirm_ready()
{

}

void ServerWindow::deal_cards()
{
    DataPackage data(-1,-1,DataPackage::Action::DEAL_CARDS,cards);
    for(int i =0;i<3;++i){

        sendData(clients[i],data);
    }
}

void ServerWindow::choose_landlord(DataPackage data)
{
    static int landlord_id = -1;
    static QVector<int> player_ids;
    player_ids.push_back(data.sender);
    bool want_landlord = false;
    if(data.content == DataPackage::Content::ACCEPT){
        landlord_id = data.sender;
        if(player_ids.size()==3){
            //now all players respond, we can determine the landlord
            qDebug()<< "decide the landlord: "<< landlord_id;
            want_landlord=true;
        }
        else{
            DataPackage data_landlord(-1,-1,DataPackage::Action::CHOOSE_LANDLORD,DataPackage::Content::REQUEST);
            sendData(clients[(data.sender+1)%3],data_landlord);
        }
    }
    else{
        if(player_ids.size()==3){
            landlord_id = (landlord_id==-1)? player_ids[0] : landlord_id;
            qDebug()<< "decide the landlord: "<< landlord_id;
            want_landlord=true;
        }
        else{
            DataPackage data_landlord(-1,-1,DataPackage::Action::CHOOSE_LANDLORD,DataPackage::Content::REQUEST);
            sendData(clients[(data.sender+1)%3],data_landlord);
        }
    }
    if(want_landlord){
        //first anounce the landlord
        DataPackage data_landlord_id(-1,landlord_id,DataPackage::Action::CHOOSE_LANDLORD,DataPackage::Content::BE_LANDLORD);

        //give bonus card
        //DataPackage data_bonus_cards(-1,-1,DataPackage::Action::LANDLORD_BONUS,bonus_cards);

        for(int i=0;i<3;++i){
            sendData(clients[i],data_landlord_id);
            //sendData(clients[i],data_bonus_cards);////////////////////////
        }

    }
}

void ServerWindow::init_game()
{
    //generate 54 cards
            QVector<Card> set_of_cards;
            QChar color;
            for(int i=0;i<54;++i){
                char figure;
                if(i<52){
                    figure = figures_to_int[i%13];
                    int j = i/13;
                    switch(j){
                        case 0:
                        color = 's';
                        break;
                        case 1:
                        color = 'h';
                        break;
                        case 2:
                        color = 'd';
                        break;
                        case 3:
                        color = 'c';
                        break;
                    }
                }
                else if(i==52){
                    figure = figures_to_int[NUMBER_OF_FIGURES-1];
                    color = 'b';
                }
                else{
                    figure = figures_to_int[NUMBER_OF_FIGURES-1];
                    color = 'r';
                }
                set_of_cards.push_back(Card(color.toLatin1(),figure));
            }

            //shuffle the set_of_cards
            int i = 0;
            srand(time(0));
            for(i = 0; i < 54; i++){
                swap(set_of_cards[i], set_of_cards[rand()%NUMBER_OF_CARDS]);
            }
            string str_cards;
            for(int i=0;i<54;++i){
                if(i<53)
                str_cards=str_cards+set_of_cards[i].get_string()+",";
                if(i==53)
                    str_cards+=set_of_cards[i].get_string();
            }
            cards = QString::fromStdString(str_cards);

            deal_cards();
            bonus_cards=bonus_cards+QString::fromStdString(set_of_cards[51].get_string())+",";
            bonus_cards=bonus_cards+QString::fromStdString(set_of_cards[52].get_string())+",";
            bonus_cards=bonus_cards+QString::fromStdString(set_of_cards[53].get_string());


}

void ServerWindow::receiveData(DataPackage data){
    qDebug()<<"one data is received!";
    static int received_message = 0;
    cards.clear();
    /////
    if(data.action==DataPackage::Action::GIVE_ID){
        qDebug() << "Record names: "<<++received_message << data.sender;
        names[data.actioner]=data.content;
        if(received_message==3){
            qDebug()<<"players all have names!";
            received_message=0;

            QString names_str;
            for(int i=0;i<names.size();++i){
                if(i!=2){names_str+=names[i]+",";}
                else{names_str+=names[i];}
            }
            DataPackage data(-1,-1,DataPackage::CONFIRM_READY,names_str);
            for(int i=0;i<3;++i){
                sendData(clients[i],data);
            }
        }
    }
    else if(data.action==DataPackage::Action::CONFIRM_READY){///////
        qDebug() << "ready is confirmed: "<<++received_message << data.sender;
        if(received_message==3){
            qDebug()<<"players are all ready to play!";
            init_game();
            received_message=0;
        }
    }
    else if(data.action==DataPackage::Action::DEAL_CARDS){
        qDebug() << "received cards: "<<++received_message << data.sender;
        if(received_message==3){
            //randomly choose a player to be the first one to determine being a landlord or not
            int random_index = QRandomGenerator::global()->bounded(3);
            DataPackage data(-1,-1,DataPackage::Action::CHOOSE_LANDLORD,DataPackage::Content::REQUEST);
            sendData(clients[random_index],data);
            received_message=0;
        }
    }
    else if(data.action==DataPackage::Action::CHOOSE_LANDLORD){
        choose_landlord(data);
    }
    else if(data.action==DataPackage::Action::PLAY_CARDS){
        DataPackage data_play(-1,data.actioner,data.action,data.content);
        for(int i=0;i<3;++i){
            sendData(clients[i],data_play);
        }
    }
    else if(data.action==DataPackage::Action::ANNOUNCE){
        DataPackage data_end_game(-1,-1,DataPackage::Action::ANNOUNCE,DataPackage::Content::END_GAME);
        for(int i=0;i<3;++i){
            sendData(clients[i],data_end_game);
        }
    }
}










