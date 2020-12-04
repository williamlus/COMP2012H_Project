#include "clientwindow.h"
#include "ui_clientwindow.h"

ClientWindow::ClientWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClientWindow)
{
    ui->setupUi(this);
}

void ClientWindow::closeEvent(QCloseEvent *event){
    if(socket){
        socket->close();
        socket->deleteLater();
        qDebug() << "Closing socket";
    }
    this->parentWidget()->show();
    event->accept();
}

ClientWindow::~ClientWindow()
{
    delete ui;
}

void ClientWindow::handleStateChanged(QAbstractSocket::SocketState state)
{
    if(state==QAbstractSocket::SocketState::UnconnectedState){
        if(play_window){
            play_window->close();
        }
        socket->close();
        ui->listWidget_dialogs->addItem("Disconnected from server");
        ui->listWidget_dialogs->addItem("Closing the socket");
    }
}

void ClientWindow::handle_server_message()
{
    if(socket){
        QByteArray arr=socket->readAll();
        ui->listWidget_dialogs->addItem(QString::fromStdString(arr.toStdString()));
        DataPackage data=DataPackage::parse(arr);
        if(data.action==DataPackage::Action::GIVE_ID){
            id=data.content.toInt();
            DataPackage data_to_send(id,-1,DataPackage::Action::CONFIRM_READY,DataPackage::Content::ACCEPT);
            send_to_server(data_to_send);
            play_window=new PlayWindow(id,this);
            play_window->show();
            //this->hide();
            connect(play_window,&PlayWindow::send_to_client,this,&ClientWindow::received_from_playwindow);
        }
        else if(play_window){
            play_window->receive_from_client(DataPackage::parse(arr));
        }
    }
}

void ClientWindow::send_to_server(DataPackage data){
    if(socket){
        QByteArray arr=data.serialize();
        socket->write(arr);
    }
}
void ClientWindow::received_from_playwindow(DataPackage data){
    if(socket){
        this->send_to_server(data);
    }
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
    this->close();
}
