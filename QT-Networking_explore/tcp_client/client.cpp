#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);
}

Client::~Client()
{
    delete ui;
}


void Client::on_ButtonSend_clicked()
{
    if(socket){
        ui->textEditSend->selectAll();
        socket->write(ui->textEditSend->textCursor().selectedText().toUtf8());
    }
}

void Client::on_ButtonConnect_clicked()
{
    if(!socket){
        socket=new QTcpSocket();
        connect(socket,&QTcpSocket::stateChanged,this,&Client::refreshState);
        connect(socket,SIGNAL(readyRead()),this,SLOT(receiveMessage()));
    }
    socket->connectToHost(ui->lineEditIP->text(),ui->lineEditPort->text().toInt());
}

void Client::receiveMessage()
{
    qDebug() << "receive msg";
    QByteArray bitArr=socket->readAll();
    qDebug() << QString::fromStdString(bitArr.toStdString());
    ui->TextEditreceive->setText(QString::fromStdString(bitArr.toStdString()));
}

void Client::refreshState(QAbstractSocket::SocketState ss)
{
    if(ss==QAbstractSocket::BoundState){
        ui->listWidget->addItem(QString("Bound to ")+ ui->lineEditIP->text());
    }
    else if(ss==QAbstractSocket::ClosingState){
        ui->listWidget->addItem(QString("Closing connection to ")+ ui->lineEditIP->text());
    }
    else if(ss==QAbstractSocket::ConnectedState){
        ui->listWidget->addItem(QString("Connected to ")+ ui->lineEditIP->text());
    }
    else if(ss==QAbstractSocket::ConnectingState){
        ui->listWidget->addItem(QString("Connecting to ")+ ui->lineEditIP->text());
    }
    else if(ss==QAbstractSocket::HostLookupState){
        ui->listWidget->addItem(QString("Host lookup for ")+ ui->lineEditIP->text());
    }
    else if(ss==QAbstractSocket::ListeningState){
        ui->listWidget->addItem(QString("Listening for ")+ ui->lineEditIP->text());
    }
    else if(ss==QAbstractSocket::UnconnectedState){
        ui->listWidget->addItem(QString("Disconnected from ")+ ui->lineEditIP->text());
    }
}
