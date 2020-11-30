#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "client.h"
#include "ToolsForNetworking.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
private:
    QTcpServer* serverSocket;
    MyTools tool;
    QVector<Card> cards{};
    QVector<Player_Info> player_info{};
    QVector<QString> message{};
    int active_player_id{-1};

    QVector<Client*> clients{};
    DataPackage* data_to_send{nullptr};
    QVector<DataPackage*> data_received{};

signals:

};

#endif // SERVER_H
