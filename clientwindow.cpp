#include "clientwindow.h"
#include "ui_clientwindow.h"

ClientWindow::ClientWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClientWindow)
{
    ui->setupUi(this);
}

ClientWindow::~ClientWindow()
{
    delete ui;
}

void ClientWindow::handleStateChanged(QAbstractSocket::SocketState state)
{

}

void ClientWindow::handle_server_message()
{
    if(socket){
        QByteArray arr=socket->readAll();
        ui->listWidget_dialogs->addItem(QString::fromStdString(arr.toStdString()));
        DataPackage data=DataPackage::parse(arr);
        if(data.action==DataPackage::Action::GIVE_ID){
            id=data.content.toInt();
        }
        if(play_window){
            play_window->receive_from_client(DataPackage::parse(arr));
        }
    }
}

void ClientWindow::send_to_server(DataPackage data){

}
void ClientWindow::received_from_playwindow(DataPackage data){

}

void ClientWindow::on_pushButton_join_server_clicked()
{
    if(socket==nullptr){
        socket=new QTcpSocket(this);
        ui->listWidget_dialogs->addItem("New socket is created");
        connect(socket,&QTcpSocket::connected,[=](){ui->listWidget_dialogs->addItem("Connected to server");});
        connect(socket,&QTcpSocket::stateChanged,this,&ClientWindow::handleStateChanged);
        connect(socket,&QTcpSocket::readyRead,this,&ClientWindow::handle_server_message);
    }
    socket->close();
    socket->connectToHost(ui->lineEdit_serverIP->text(),ui->lineEdit_server_port->text().toInt());
}

void ClientWindow::on_pushButton_stop_joining_clicked()
{
    if(socket){
        socket->close();
    }
}

void ClientWindow::on_pushButton_quit_clicked()
{

}
