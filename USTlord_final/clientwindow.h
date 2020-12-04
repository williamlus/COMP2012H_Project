#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QCloseEvent>
#include "source/datapackage.h"
#include "playwindow.h"

namespace Ui {
class ClientWindow;
}

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientWindow(QWidget *parent = nullptr);

    void closeEvent(QCloseEvent *event) override;
    //Override closeEvent
    //Show MainWindow when ClientWindow is closed

    ~ClientWindow();

public slots:
    void handleStateChanged(QAbstractSocket::SocketState state);
    //Close the ClientWindow whenever the QTcpSocket is disconnected from the QTcpServer

    void handle_server_message();
    //Receive the QByteArray from server and translate it into DataPackage
    //Perform actions according to DataPackage instructions from QTcpServer
    //Send back DataPackage to QTcpServer if required
    //Pass the DataPackage to PlayWindow

    void send_to_server(DataPackage data);
    //Transform the DataPackage into QByteArray
    //Send the QByteArray to QTcpServer

    void received_from_playwindow(DataPackage data);
    //Receive DataPackage emitted from PlayWindow
    //And call send_to_server() to send the data to QTcpServer

    void set_port(QString port);
    //Set the port in ui line edit

    void set_serverIP(QString ip);
    //Set the Server IP Address in ui line edit

public slots:
    void on_pushButton_join_server_clicked();
    //Create the QTcpSocket and join the server

    void on_pushButton_stop_joining_clicked();
    //Close the QTcpSocket

    void on_pushButton_quit_clicked();
    //Close the ClientWindow

private:
    Ui::ClientWindow *ui;
    QTcpSocket *socket{nullptr};//Used to communicate with ServerWindow
    PlayWindow *play_window{nullptr};//Create a PlayWindow when the game start
    int id{-1};//This player id
};

#endif // CLIENTWINDOW_H
