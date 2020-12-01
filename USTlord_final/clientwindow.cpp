#include "clientwindow.h"
#include "ui_clientwindow.h"

ClientWindow::ClientWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClientWindow)
{
    ui->setupUi(this);
    this->setFixedSize(800,600);

}

void ClientWindow::closeEvent(QCloseEvent *event)
{
    //show the mainwindow
    this->parentWidget()->show();
    //allow to close the second window
    event->accept();
}

ClientWindow::~ClientWindow()
{
    delete ui;
}

void ClientWindow::setPort(const int port)
{
    ui->label_port->setText(QString(port));
}

void ClientWindow::on_pushButton_quit_clicked()
{
    this->close();
}

void ClientWindow::on_pushButton_join_server_clicked()
{
    if(client){
        client->close();
    }
    client = new QTcpSocket();
    // connected with the server
    connect(client, &QTcpSocket::connected, [=]{ qDebug() << "connected"; });
    connect(client,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
    // "new data is available for reading from the device's current read channel"
    connect(client, &QTcpSocket::readyRead, this, &ClientWindow::readyRead);
    connect(&my_tool, &MyTools::transferPackage, this, &ClientWindow::received_from_server);
    client->connectToHost(ui->label_serverIP->text(), static_cast<quint16>(ui->label_port->text().toInt()));
}

void ClientWindow::on_pushButton_stop_joining_clicked()
{
    disconnect(client,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
    // "new data is available for reading from the device's current read channel"
    disconnect(client, &QTcpSocket::readyRead, this, &ClientWindow::readyRead);
    client->close();
    client=nullptr;
}

void ClientWindow::readyRead()
{
    my_tool.read(client);
}

void ClientWindow::displayError(QAbstractSocket::SocketError)
{
    qDebug() << "error" << client->errorString();
    on_pushButton_stop_joining_clicked();
}//display error and close the client

void ClientWindow::received_from_server(DataPackage data)
{
    if(data.data_type==-1){return;}
    if(data.data_type==0){
        play_window = new PlayWindow(data,this);
        connect(play_window,&PlayWindow::send_datapackage,this,&ClientWindow::received_from_playwindow);
        this->hide();
        play_window->show();
    }
    else{
        qDebug() << "Received id: " + QString::number(data.active_player_id);
        play_window->receive_datapackage(data);
    }
}

void ClientWindow::received_from_playwindow(DataPackage data){
    if(client!=nullptr){
        my_tool.send(client,data);
    }
}
