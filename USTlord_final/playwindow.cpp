#include "playwindow.h"
#include "ui_playwindow.h"

PlayWindow::PlayWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlayWindow)
{
    ui->setupUi(this);
}

PlayWindow::PlayWindow(DataPackage data,QWidget *parent):QMainWindow(parent),ui(new Ui::PlayWindow)
{
    data=DataPackage();//just for debugging
    ui->setupUi(this);
}

void PlayWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton answer = QMessageBox::question( this, "PlayWindow",
                                                                tr("Are you sure to quit the game?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes);
    if (answer != QMessageBox::Yes) {
        //do not allow to close the second window
        event->ignore();
    }
    else {
        //show the mainwindow
        this->parentWidget()->show();
        //allow to close the second window
        event->accept();
    }
}

PlayWindow::~PlayWindow()
{
    delete ui;
}

void PlayWindow::on_pushButton_play_clicked()
{

}

void PlayWindow::on_pushButton_hint_clicked()
{

}

void PlayWindow::on_pushButton_give_up_clicked()
{

}

void PlayWindow::on_pushButton_want_landlord_clicked()
{

}

void PlayWindow::on_pushButton_want_farmer_clicked()
{

}

void PlayWindow::receive_datapackage(DataPackage dp){}

