#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "source/server.h"
#include "clientwindow.h"

namespace Ui {
class ServerWindow;
}

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerWindow(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *event) override;
    ~ServerWindow();

private slots:
    void on_pushButton_quit_clicked();

    void on_pushButton_create_clicked();

    void on_pushButton_start_game_clicked();

    void acceptConnection();

    void receiveData(DataPackage data);

    void choose_landlord(DataPackage data);

private:
    Ui::ServerWindow *ui;

    MyTools tool;
    QVector<Card> cards{};
    QVector<Player_Info> player_info{};
    QVector<QString> message{};
    int player_to_beat;



    QTcpServer* server;
    QVector<QTcpSocket*> clients;
    ClientWindow* client_window;
    //DataPackage* data_to_send{nullptr};
    QVector<DataPackage*> data_received{};
};

#endif // SERVERWINDOW_H
