#include "clientwindow.h"
#include "ui_clientwindow.h"
#include <QMessageBox>

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
    ui->listWidget_dialogs->addItem("Handle server msg");
    if(socket){
        QByteArray arr=socket->readAll();
        ui->listWidget_dialogs->addItem(QString::fromStdString(arr.toStdString()));
        DataPackage data=DataPackage::parse(arr);
        ui->listWidget_dialogs->addItem("Received from server:"+data.to_string());
//        qDebug()<< "data.action==DataPackage::Action::GIVE_ID?";/////
//        qDebug() << (data.action==DataPackage::Action::GIVE_ID);//////////
        if(data.action==DataPackage::Action::GIVE_ID){
            QMessageBox::information(this,"Start","Let's start!",QMessageBox::Ok,QMessageBox::Ok);
            id=data.content.toInt();
            ui->listWidget_dialogs->addItem("data.conent="+data.content);///////
            DataPackage data_to_send(id,id,DataPackage::GIVE_ID,ui->lineEdit_name->text()+"#"+QString::number(id));
            send_to_server(data_to_send);
        }////////
        else if(data.action==DataPackage::Action::CONFIRM_READY){
            QStringList names=data.content.split(QLatin1Char(','),Qt::SkipEmptyParts);
            QVector<QString> names_vec{};
            for(int i=0;i<names.size();i++){
                names_vec.push_back(names[i]);
                qDebug() << names_vec[i];
            }
            play_window=new PlayWindow(id,names_vec,this);
            connect(play_window,&PlayWindow::send_to_client,this,&ClientWindow::received_from_playwindow);
            play_window->show();/////////
//            this->hide();
            DataPackage data_to_send(id,-1,DataPackage::Action::CONFIRM_READY,DataPackage::Content::ACCEPT);
            send_to_server(data_to_send);
        }/////////
        else if(play_window){
            ui->listWidget_dialogs->addItem("send msg to playwindow"+data.to_string());/////////////
            play_window->receive_from_client(DataPackage::parse(arr));
        }
    }
}

void ClientWindow::send_to_server(DataPackage data){
    ui->listWidget_dialogs->addItem("Send to server"+data.to_string());//////
    qDebug() << "Send to server"+data.to_string();
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

void ClientWindow::set_port(QString port)
{
    ui->lineEdit_server_port->setText(port);
}

void ClientWindow::set_serverIP(QString ip)
{
    ui->lineEdit_serverIP->setText(ip);
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
