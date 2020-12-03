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
    QVector<QString> get_local_IP() const;
    ~ServerWindow();

    signals:
    //void receive_data(DataPackage data);

private slots:
    void on_pushButton_stop_clicked();

    void on_pushButton_create_clicked();

    void on_pushButton_start_game_clicked();

    void handleConnection();

    void handleStateChanged(QAbstractSocket::SocketState state);
    void handle_clients_message();
    void sendData(QTcpSocket* socket,DataPackage data);
    void receiveData(DataPackage data);

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
};

#endif // SERVERWINDOW_H
