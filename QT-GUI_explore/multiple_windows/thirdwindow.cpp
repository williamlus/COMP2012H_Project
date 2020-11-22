#include "thirdwindow.h"
#include "ui_thirdwindow.h"
#include <QCloseEvent>
#include <QMessageBox>

ThirdWindow::ThirdWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ThirdWindow)
{
    ui->setupUi(this);
}

ThirdWindow::~ThirdWindow()
{
    delete ui;
}

void ThirdWindow::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton answer = QMessageBox::question( this, "ThirdWindow",
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

void ThirdWindow::on_pushButton_clicked()
{
    this->close();
}
