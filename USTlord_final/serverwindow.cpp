#include "serverwindow.h"
#include "ui_serverwindow.h"

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
    }
    server->listen(QHostAddress::Any,ui->lineEdit_port->text().toInt());
    if(server->isListening()){
        ui->listWidget_dialogs->addItem("Server is listening to port: "+ui->lineEdit_port->text());
        ui->pushButton_create->setEnabled(false);
    }
    else{
        ui->listWidget_dialogs->addItem("Server fails to listen");
    }

    //debugging
    DataPackage dp(1,2,DataPackage::Action::CHAT,DataPackage::Content::WIN_GAME);
    ui->listWidget_dialogs->addItem(dp.to_string());////
}

void ServerWindow::on_pushButton_start_game_clicked()
{

}

void ServerWindow::handleConnection()
{
    QTcpSocket* client=server->nextPendingConnection();
    connect(client,&QTcpSocket::stateChanged,this,&ServerWindow::handleStateChanged);
    connect(client,&QTcpSocket::readyRead,this,&ServerWindow::handle_clients_message);
    this->clients.push_back(client);
    ui->listWidget_dialogs->addItem("New connection"+client->peerAddress().toString() + ":" + QString::number(client->peerPort())+ " accepted");
    ui->listWidget_dialogs->addItem("Clients left: " + QString::number(clients.size()));
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
        }
    }
}

void ServerWindow::sendData(DataPackage data)
{
    QByteArray arr=data.serialize();
    for(int i=0;i<clients.size();++i){
        clients[i]->write(arr);
        ui->listWidget_dialogs->addItem("Send text to client: " + clients[i]->peerAddress().toString() + ":" + QString::number(clients[i]->peerPort()));
    }
}

void ServerWindow::give_id()
{

}

void ServerWindow::confirm_ready()
{

}

void ServerWindow::deal_cards()
{

}

void ServerWindow::choose_landlord()
{

}

void ServerWindow::init_game()
{

}
