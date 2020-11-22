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


void MainWindow::on_pushButton_clicked()
{
    //hide the the mainwindow
    this->hide();
    //create a SecondWindow object
    this->secWindow=new SecondWindow(this);
    //show the secWindow
    secWindow->show();
}
