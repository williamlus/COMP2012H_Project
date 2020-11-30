#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include "ToolsForNetworking.h"

class Client : public QTcpSocket
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
private:
    DataPackage* data_to_send{nullptr};
    DataPackage* data_received{nullptr};

signals:

};

#endif // CLIENT_H
