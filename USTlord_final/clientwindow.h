#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "source/ToolsForNetworking.h"
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

    ~ClientWindow();

    void setServerIP(QString ip);//modify the ServerIP by line edit
    void setPort(QString port);//modify the port by line edit

public slots:
    void on_pushButton_quit_clicked();

    void on_pushButton_join_server_clicked();

    void on_pushButton_stop_joining_clicked();

    void readyRead();//let my_tool to process the raw data

    void displayError(QAbstractSocket::SocketError);//display error, close the client and set to nullptr

    void received_from_server(DataPackage data);
    //when ready_read signal occurs, trigger received_from_server
    //if the data is "start game", create playwindow by conversion constructor ( i.e. PlayWindow(DataPackage,this) )
    //else, send the data to playwindow by play_window->receive_package(DataPackage)
    void received_from_playwindow(DataPackage data);
    //when playwindow emit signal send_datapackage(Datapackage), trigger received_from_playwindow
    //send the data to server by my_tool

private:
    Ui::ClientWindow *ui;
    QTcpSocket *client{nullptr};
    MyTools my_tool;
    PlayWindow* play_window{nullptr};
};

#endif // CLIENTWINDOW_H
