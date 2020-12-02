#include "serverwindow.h"
#include "ui_serverwindow.h"
#include <QMessageBox>

ServerWindow::ServerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerWindow)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("Landlord - connecting - page"));
    server = new QTcpServer();
    //get the local IP info
    auto ipVec = MyTools::getLocalIP();
    foreach (auto& ip, ipVec){
        ui->listWidget_serverIP->addItem(ip);
    }
}

void ServerWindow::closeEvent(QCloseEvent *event)
{
    if(server){server->close();}
    qDebug() << "close server_window";
    //show the mainwindow
    this->parentWidget()->show();
    //allow to close the second window
    event->accept();
}

ServerWindow::~ServerWindow()
{
    qDebug() << "Call destructor of server_window";
    delete ui;
}

void ServerWindow::on_pushButton_quit_clicked()
{
    this->close();
}

void ServerWindow::on_pushButton_create_clicked()
{
    //TODO:
    //Create a Server object if it does not exist
    //Record the server's IP address(es) in listWidget_serverIP in ui
    //Start listening according to the port in line edit in ui
    //Record the connection state in dialogs in ui
    //Add clients to server (including yourself i.e. local address and new port)
    //Record the clients' addresses in listWidget_clients in ui
    server->close();
    ui->listWidget_dialogs->addItem(QString("Port: ")+ui->lineEdit_port->text());
    if(!server->listen(QHostAddress::Any,static_cast<quint16>(ui->lineEdit_port->text().toInt()))){
        qDebug() << "Error when listening a port: " << server->errorString();
        ui->listWidget_dialogs->addItem(QString("Error: "+server->errorString()));
        return;
    }
    connect(server, &QTcpServer::newConnection,this, &ServerWindow::acceptConnection);
    //create a client window for the server itself
    client_window = new ClientWindow(this);
    client_window->setServerIP(QString("127.0.0.1"));
    client_window->setPort(ui->lineEdit_port->text());
    client_window->on_pushButton_join_server_clicked();
    ui->pushButton_create->setEnabled(false);
}

void ServerWindow::on_pushButton_start_game_clicked()
{
    //TODO:
    //If there is less than three persons, nothing happens
    //Otherwise, create a PlayWindow with conversion constructor : PlayWindow(Client* myself)
    //Generate a deck of cards and pre-distribute the cards as three groups and three extra cards
    //Send each group of cards and the three extra cards to each client and start the game
    //Hide ServerWindow and show PlayWindow
    //Start the game
    if(clients.size()==3){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this,"Game is ready to start : )",tr("Start game?"),QMessageBox::No|QMessageBox::Yes);
        if(reply == QMessageBox::Yes){
            qDebug()<<"Three players are ready!";
            connect(&tool, &MyTools::transferPackage, this, &ServerWindow::receiveData);
            DataPackage send_data;
            send_data.id = 0;
            send_data.data_type = 0;
            send_data.message << QString::number(QRandomGenerator::global()->bounded(2));
            foreach (auto* socket, clients){
                connect(socket, &QTcpSocket::readyRead, this, [=]{tool.read(socket);});
                tool.send(socket,send_data);
                send_data.id+=1;
            }
            qDebug()<<"is written";
            client_window->move((this->pos()));
            client_window->resize(this->size());
            this->hide();
        }
    }
}

void ServerWindow::acceptConnection(){
    if(clients.size()>=3){server->nextPendingConnection();return;}
    qDebug()<<"a connection is succesfully accepted";
    clients.push_back(server->nextPendingConnection());
    ui->listWidget_clients->addItem("Order: "+QString::number(clients.size())+" "+clients.last()->peerAddress().toString()+": " + QString::number(clients.last()->peerPort()));
    connect(clients[clients.size()-1],&QTcpSocket::stateChanged,this,&ServerWindow::handleException);//if clients disconnected, close it
}

void ServerWindow::handleException(QAbstractSocket::SocketState state)
{
    if(state==QAbstractSocket::SocketState::UnconnectedState){
        for(int i=0;i<clients.size();++i){
            if(clients[i]->state()==state){
                disconnect(clients[i],&QTcpSocket::stateChanged,this,&ServerWindow::handleException);
                QString feedback="Closing " + clients[i]->peerAddress().toString()+": "+QString::number(clients[i]->peerPort());
                qDebug() << feedback;
                ui->listWidget_clients->addItem(feedback);
                clients[i]->close();
                clients.remove(i);
                --i;
            }
        }
    }
}

void ServerWindow::receiveData(DataPackage data){
    qDebug()<<"one data is received!";
    static int received_message = 0;
    if(data.data_type==0){
        qDebug()<<"ready is confirmed: "<<++received_message << data.id;

    if(received_message==3){
        received_message=0;
        qDebug()<< "players are all ready to play!";
        player_info.clear();
        //deal cards
        QVector<Card> cards_to_deal;
        //TO DO: generate 54 cards and shuffle it and store them sequentially in cards_to_deal
        init_game(cards_to_deal);
        for(int i = 0;i<3;++i){
            player_info.push_back(Player_Info(i,17,-1));
        }
        DataPackage data_dealCards[3];
        for(int i=0;i<3;++i){
            data_dealCards[i].data_type=2;
            data_dealCards->player_info=player_info;
            for(int j=0;j<17;++j){
                data_dealCards[i].cards.push_back(cards_to_deal[j+i*17]);
            }
        }
        for(int i=0;i<3;++i){
            tool.send(clients[i],data_dealCards[i]);
        }
    }}
    else if(data.data_type==2){

        //choosing the landlord
        received_message = 0;
        qDebug()<< "now it's time for choosing the landlord!";
        int random_index = QRandomGenerator::global()->bounded(3);
        DataPackage data_landlord;
        data_landlord.data_type=1;
        data_landlord.message << "want landlord?";
        tool.send(clients[random_index],data_landlord);
    }
    else if(data.data_type==1){
        choose_landlord(data);
    }
    else if(data.data_type==3){
        //update the cards_remain
        data.player_info[data.id].cards_remain-=data.cards.size();
        DataPackage data_play_cards = data;
        for(int i=0;i<3;++i){
            tool.send(clients[i],data_play_cards);
        }



    }
}

void ServerWindow::choose_landlord(DataPackage data){
    static int landlord_id = -1;
    static QVector<int> player_ids;
    player_ids.push_back(data.id);
    bool want_landlord = false;
    if(data.message[0] == "yes"){
        landlord_id = data.id;
        if(player_ids.size()==3){
            //now all players respond, we can determine the landlord
            qDebug() << "decide the landlord: " << landlord_id;
            want_landlord = true;
        }
        else{
            DataPackage data_landlord;
            data_landlord.data_type=1;
            data_landlord.message << "want landlord?";
            tool.send(clients[(data.id+1)%3],data_landlord);
        }
    }
    else{
        if(player_ids.size()==3){
            landlord_id = (landlord_id == -1) ? player_ids[0] : landlord_id;
            qDebug() << "Landlord is decided: " << landlord_id;
            want_landlord = true;
        }
        else{
            DataPackage data_landlord;
            data_landlord.data_type = 1;
            data_landlord.message << "want landlord?";
            tool.send(clients[(data.id+1)%3],data_landlord);
        }
    }
    if(want_landlord){
        //give the bonus card to the landlord player
        //the player_info will indicate the identity of three players
        //the cards referring to three bonus cards in this case
        DataPackage data_choosing_landlord;
        data_choosing_landlord.data_type=1;
        data_choosing_landlord.message << "landlord is finalized";
        data_choosing_landlord.cards=bonus_cards;
        for(int i = 0;i<3;++i){
            if(i==landlord_id){
                player_info[i].role = 0;
            }
            else{
                player_info[i].role = 1;
            }
        }
        data_choosing_landlord.player_info = player_info;
        for(int i=0;i<3;++i){
            tool.send(clients[i],data_choosing_landlord);     
        }
        landlord_id = -1;
        player_ids.clear();
    }

}

void ServerWindow::init_game(QVector<Card>& cards_to_deal){
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
                    color = 'S';
                    break;
                    case 1:
                    color = 'H';
                    break;
                    case 2:
                    color = 'D';
                    break;
                    case 3:
                    color = 'C';
                    break;
                }
            }
            else if(i==52){
                figure = figures_to_int[NUMBER_OF_FIGURES-1];
                color = 'B';
            }
            else{
                figure = figures_to_int[NUMBER_OF_FIGURES-1];
                color = 'R';
            }
            set_of_cards.push_back(Card(color.toLatin1(),figure));
        }

        //shuffle the set_of_cards
        int i = 0;
        srand(time(0));
        for(i = 0; i < 54; i++){
            swap(set_of_cards[i], set_of_cards[rand()%NUMBER_OF_CARDS]);
        }

        cards_to_deal=set_of_cards;
        bonus_cards.push_back(cards_to_deal[51]);
        bonus_cards.push_back(cards_to_deal[52]);
        bonus_cards.push_back(cards_to_deal[53]);


}
