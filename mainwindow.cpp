#include <QPixmap>
#include <QPainter>
#include <QFont>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->start_offline_button->setStyleSheet(
                                          "QPushButton{"
                                          "background-color:white;"//color
                                          "border-style:outset;"                  //inset/outset
                                          "border-width:4px;"
                                          "border-radius:10px;"
                                          "border-color:black;"    //border color
                                          "font:bold 25px;"                       //character and size
                                          "color:black;"                //character color
                                          "padding:6px;"                          //padding
                                          "}");
    ui->start_offline_button->setCursor(Qt::PointingHandCursor);
    ui->join_room_button->setStyleSheet(
                                          "QPushButton{"
                                          "background-color:white;"//color
                                          "border-style:outset;"                  //inset/outset
                                          "border-width:4px;"
                                          "border-radius:10px;"
                                          "border-color:black;"    //border color
                                          "font:bold 25px;"                       //character and size
                                          "color:black;"                //character color
                                          "padding:6px;"                          //padding
                                          "}");
    ui->join_room_button->setCursor(Qt::PointingHandCursor);
    ui->create_room_button->setStyleSheet(
                                          "QPushButton{"
                                          "background-color:white;"//color
                                          "border-style:outset;"                  //inset/outset
                                          "border-width:4px;"
                                          "border-radius:10px;"
                                          "border-color:black;"    //border color
                                          "font:bold 25px;"                       //character and size
                                          "color:black;"                //character color
                                          "padding:6px;"                          //padding
                                          "}");
    ui->create_room_button->setCursor(Qt::PointingHandCursor);
    ui->exit_button->setStyleSheet(
                                          "QPushButton{"
                                          "background-color:white;"//color
                                          "border-style:outset;"                  //inset/outset
                                          "border-width:4px;"
                                          "border-radius:10px;"
                                          "border-color:black;"    //border color
                                          "font:bold 25px;"                       //character and size
                                          "color:black;"                //character color
                                          "padding:6px;"                          //padding
                                          "}");
    ui->exit_button->setCursor(Qt::PointingHandCursor);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    static QPixmap backpic(":/background/images/Homepage.jpg");
    QPainter painter(this);
    painter.drawPixmap(this->rect(),backpic);
}

void MainWindow::on_create_room_button_clicked()
{
    server_window=new ServerWindow(this);
    this->hide();
    server_window->show();
}

void MainWindow::on_start_offline_button_clicked()
{
    play_window=new PlayWindow(this);
    this->hide();
    play_window->show();
}

void MainWindow::on_join_room_button_clicked()
{
    client_window=new ClientWindow(this);
    this->hide();
    client_window->show();
}

void MainWindow::on_exit_button_clicked()
{
    close();
}
