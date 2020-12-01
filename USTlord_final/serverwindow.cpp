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
    //show the mainwindow
    this->parentWidget()->show();
    //allow to close the second window
    event->accept();
}

ServerWindow::~ServerWindow()
{
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
    while(true){
        if(!server->listen(QHostAddress::Any,static_cast<quint16>(ui->lineEdit_port->text().toInt()))){
        qDebug() << "There is some error when listening a port: " << server->errorString();
        continue;
    }
    connect(server, &QTcpServer::newConnection,this, &ServerWindow::acceptConnection);
    //create a client window for the server itself
    client_window = new ClientWindow();
    client_window->setPort(ui->lineEdit_port->text().toInt());
    client_window->on_pushButton_join_server_clicked();
    ui->pushButton_create->setEnabled(false);
    break;
}
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
        reply = QMessageBox::question(this,"Game is ready to start : )","Start game?",QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes){
            qDebug()<<"Three players are ready!";
            connect(&tool, &MyTools::transferPackage, this, &ServerWindow::receiveData);
            DataPackage send_data;
            send_data.id = 0;
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
    qDebug()<<"a connection is succesfully accepted";
    clients.push_back(server->nextPendingConnection());
    ui->listWidget_clients->addItem(clients.last()->peerAddress().toString()+": " + QString::number(clients.last()->peerPort()));
}

void ServerWindow::receiveData(DataPackage data){
    qDebug()<<"one data is received!";
    static int received_message = 0;
    if(data.data_type==0){
        qDebug()<<"ready is confirmed: "<<++received_message << data.id;

    if(received_message==6){
        received_message=0;
        qDebug()<< "players are all ready to play!";
        player_info.clear();
        player_to_beat=-1;
        //deal cards
        QVector<Card> cards_to_deal;
        //TO DO: generate 54 cards and shuffle it and store them sequentially in cards_to_deal
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
        received_message+=1;
        //choosing the landlord
        qDebug()<< "now it's time for choosing the landlord!";
        int random_index = QRandomGenerator::global()->bounded(3);
        DataPackage data_landlord;
        data_landlord.data_type=1;
        data_landlord.message << "Want landlord?";
        tool.send(clients[random_index],data_landlord);
    }
    else if(data.data_type==1){
        choose_landlord(data);
    }
    else if(data.data_type==2){
        //update the cards_remain
        player_info[data.id].cards_remain-=data.cards.size();
        DataPackage data_play_cards;

    }
}

void ServerWindow::choose_landlord(DataPackage data){

}




















