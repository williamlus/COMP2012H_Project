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

void ClientWindow::on_pushButton_quit_clicked()
{
    this->close();
}

void ClientWindow::on_pushButton_join_server_clicked()
{

}

void ClientWindow::on_pushButton_stop_joining_clicked()
{

}
