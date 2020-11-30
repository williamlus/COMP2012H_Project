#include "serverwindow.h"
#include "ui_serverwindow.h"

ServerWindow::ServerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerWindow)
{
    ui->setupUi(this);
}

void ServerWindow::closeEvent(QCloseEvent *event)
{
    //show the mainwindow
    this->parentWidget()->show();
    //allow to close the second window
    event->accept();
}

ServerWindow::~ServerWindow()
{
    delete ui;
}

void ServerWindow::on_pushButton_quit_clicked()
{
    this->close();
}

void ServerWindow::on_pushButton_listen_clicked()
{
    //TODO:
    //Create a Server object if it does not exist
    //Record the server's IP address(es) in listWidget_serverIP in ui
    //Start listening according to the port in line edit in ui
    //Record the connection state in dialogs in ui
    //Add clients to server (including yourself i.e. local address and new port)
    //Record the clients' addresses in listWidget_clients in ui
}

void ServerWindow::on_pushButton_stop_clicked()
{
    //TODO:
    //Stop listening
    //Record this action in the dialogs in ui
    //Delete the server and set it to nullptr
}

void ServerWindow::on_pushButton_start_game_clicked()
{
    //TODO:
    //If there is less than three persons, nothing happens
    //Otherwise, create a PlayWindow with conversion constructor : PlayWindow(Client* myself)
    //Generate a deck of cards and pre-distribute the cards as three groups and three extra cards
    //Send each group of cards and the three extra cards to each client and start the game
    //Hide ServerWindow and show PlayWindow
    //Start the game
}
