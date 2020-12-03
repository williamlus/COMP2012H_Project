#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
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
    ~ClientWindow();

public slots:
    void handleStateChanged(QAbstractSocket::SocketState state);
    void handle_server_message();
    void send_to_server(DataPackage data);
    void received_from_playwindow(DataPackage data);

private slots:
    void on_pushButton_join_server_clicked();

    void on_pushButton_stop_joining_clicked();

    void on_pushButton_quit_clicked();

private:
    Ui::ClientWindow *ui;
    QTcpSocket *socket{nullptr};
    PlayWindow *play_window{nullptr};
    int id{-1};
};

#endif // CLIENTWINDOW_H
