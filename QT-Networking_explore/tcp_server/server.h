#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <iostream>
#include <cstring>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    ~Server(){qDebug() << "Server Killed.";}

private:
    QTcpSocket *sock=nullptr;
    char* msg=nullptr;
    int msg_capacity=16;
    int msg_len=0;
    char* whole_msg=new char[msg_capacity]{};

signals:
    void msgChangedTo(const char*,int);
    void clientIPChangedTo(const QString,int);

private slots:
    void incomingConnection(qintptr handle) override;
    void receiveMessage();
    void clientDisconnected();
};

#endif // SERVER_H