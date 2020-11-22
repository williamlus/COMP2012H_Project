#include "secondwindow.h"
#include "ui_secondwindow.h"
#include <QCloseEvent>
#include <QMessageBox>
#include "thirdwindow.h"

SecondWindow::SecondWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SecondWindow)
{
    ui->setupUi(this);
}

SecondWindow::~SecondWindow()
{
    delete ui;
}

void SecondWindow::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton answer = QMessageBox::question( this, "SecondWindow",
                                                                tr("Are you sure?\n"),
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

void SecondWindow::on_pushButton_clicked()
{
    this->hide();
    ThirdWindow* thirdWindow=new ThirdWindow(this);
    thirdWindow->show();
}
