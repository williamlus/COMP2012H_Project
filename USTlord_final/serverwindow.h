#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostInfo>
#include <QVector>
#include "clientwindow.h"
#include "source/Card.h"
#include "source/datapackage.h"

namespace Ui {
class ServerWindow;
}

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerWindow(QWidget *parent = nullptr);

    void closeEvent(QCloseEvent *event) override;
    //Override closeEvent
    //Show the MainWindow when ServerWindow is closed

    QVector<QString> get_local_IP() const;
    //Find all the local IPv4/IPv6 addresses and store them in a QVector<QString>

    ~ServerWindow();

    signals:
    //void receive_data(DataPackage data);

private slots:
    void on_pushButton_stop_clicked();
    //Close the QTcpServer and close all QTcpSocket(s)

    void on_pushButton_create_clicked();
    //Create a QTcpServer and a local QTcpSocket
    //Create a ClientWindow for the host to connect to ServerWindow

    void on_pushButton_start_game_clicked();
    //Start the game if there are 3 clients connected to the ServerWindow

    void handleConnection();
    //Add new client to server

    void handleStateChanged(QAbstractSocket::SocketState state);
    //Remove the disconnected sockets

    void handle_clients_message();
    //Receive clients' messages if there are bytes avaible to read

    void sendData(QTcpSocket* socket,DataPackage data);
    //Send the data to the corresponding socket (client)

    void receiveData(DataPackage data);
    //Receive the data from clients and perform corresponding actions
    //Send feedback DataPackage to clients if required

    //Game logic controlled by server
    void give_id();
    void confirm_ready();
    void deal_cards();
    void choose_landlord(DataPackage data);
    void init_game();

private:
    Ui::ServerWindow *ui;

    QTcpServer* server{nullptr};
    QVector<QTcpSocket*> clients{};
    ClientWindow* client_window{nullptr};
    DataPackage* data_to_send{nullptr};
    QVector<DataPackage> data_received{};
    QString cards;
    QString bonus_cards;
    QVector<QString> names={"","",""};//players' names
};

#endif // SERVERWINDOW_H
