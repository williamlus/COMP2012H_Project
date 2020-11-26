#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_InitServerButton_clicked()
{
    if(server==nullptr){
        server=new Server(this);
        connect(server,&Server::msgChangedTo,this,&MainWindow::setMsg);
        connect(server,&Server::clientIPChangedTo,this,&MainWindow::setClientIP);
        ui->message->setText("A TcpServer has been created!");
        server->listen(QHostAddress::Any,6666);
        ui->message->setText("I am listening!");
        qDebug() << "Object Created.";
        std::cout << "Object Created.";
    }
}

void MainWindow::setMsg(const char* msg,int msg_len){
    qDebug() << "setMsg";
    std::cout << "setMsg";
    QString qs{};
    for(int i=0;i<msg_len;++i){
        qs+=msg[i];
    }
    ui->message->setText(qs);
}
void MainWindow::setClientIP(const QString ip,int ip_len){
    qDebug() << "set client IP";
    qDebug() << "IP Length : " << ip_len;
    ui->ClientIPlabel->setText(ip);
}

void MainWindow::on_StopServerButton_clicked()
{
    if(this->server){
        qDebug() << "Stop the TcpServer.";
        std::cout << "Stop the TcpServer.";
        delete this->server;
        this->server=nullptr;
        ui->message->setText("No Server...");
    }
}
