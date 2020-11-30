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


void MainWindow::on_pushButton_start_offline_clicked()
{
    play_window=new PlayWindow(this);
    this->hide();
    play_window->show();
}

void MainWindow::on_pushButton_create_room_clicked()
{
    server_window=new ServerWindow(this);
    this->hide();
    server_window->show();
}

void MainWindow::on_pushButton_join_room_clicked()
{
    client_window=new ClientWindow(this);
    this->hide();
    client_window->show();
}
