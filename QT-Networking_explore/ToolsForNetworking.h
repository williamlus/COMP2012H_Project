#ifndef TOOLSFORNETWORKING_H
#define TOOLSFORNETWORKING_H

#include <QtNetwork>
#include <QGraphicsItem>
#include <QtMath>
#include "Card.h"

class Player_Info{
    public:

    int player_index;
    unsigned int cards_remain;
    int role;//-1 for null, 0 for landlord, 1 for farmer

    Player_Info(int player_index=-1, int cards_remain=-1, int role=-1) : player_index(player_index), cards_remain(cards_remain), role(role) {}

};

class DataPackage{
    public:

    int data_type;
    /* 0: confirm_ready
     * 2: deal cards
     * 1: choose landlord
     * 3: play cards
     */
    int sender_id; // 0 for server, 1 for client
    QVector <Card> cards;
    QVector <Player_Info> player_info;
    int active_player_id;//-1 for default

    DataPackage(int data_type=-1, int sender_id=-1, QVector <Card> cards={}, QVector <Player_Info> player_info={}, int active_player_id=-1) :
    data_type(data_type), sender_id(sender_id), cards(cards), player_info(player_info), active_player_id(active_player_id) {}

    friend QDataStream& operator>>(QDataStream& in, DataPackage& data){
        in >> data.data_type >> data.sender_id >> data.cards >> data.player_info >> data.active_player_id;
        return in;
    }

    friend QDataStream& operator<<(QDataStream& out, DataPackage& data){
        out << data.data_type << data.sender_id << data.cards << data.player_info << data.active_player_id;
        return out;
    }
};

class MyTools : public QObject
{
    Q_OBJECT

signals:
    void transferPackage(DataPackage data);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (event->type() == QEvent::GraphicsSceneMousePress)
        {
            qDebug() << "event filtered";
            return true;
        }
        else
            return QObject::eventFilter(watched, event);
    }

public:
    MyTools();

    static QVector<QString> getLocalIP()
    {
        QVector<QString> ipv4, ipv6;
        QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
        foreach (auto addr, info.addresses())
        {
            if (addr.protocol() == QAbstractSocket::IPv4Protocol)
                ipv4 << addr.toString();
            else if (addr.protocol() == QAbstractSocket::IPv6Protocol)
                ipv6 << addr.toString();
        }
        ipv4 << ipv6;
        return ipv4;
    }

    static QString getLocalIPString()
    {
        QString ipInfo;
        auto ipVec = getLocalIP();
        foreach (auto& ip, ipVec)
        {
            ipInfo += ip;
            ipInfo += "\n";
        }
        return ipInfo;
    }

    static int nextId(const int _id)
    {
        return (_id + 1) % 3;
    }



    template <class T>
    void send(QTcpSocket* socket, const T& data)
    {
        QByteArray bytes;
        QDataStream stream(&bytes, QIODevice::WriteOnly);
        stream.setVersion(QDataStream::Qt_5_12);
        stream.setByteOrder(QDataStream::BigEndian);
        stream << qint32(0);
        stream << data;
        stream.device()->seek(0);
        // qDebug() << bytes.size() - sizeof(qint32);
        stream << qint32(static_cast<qint32>(bytes.size()) - static_cast<qint32>(sizeof(qint32)));
        socket->write(bytes);
        qDebug() << "sent one!";
    }

    void read(QTcpSocket* socket)
    {
        DataPackage data;
        QDataStream in(socket);
        in.setVersion(QDataStream::Qt_5_12);
        in.setByteOrder(QDataStream::BigEndian);
        static qint32 bytesToRead = 0;
        //qDebug() << "socket->bytesAvailable(): " << socket->bytesAvailable();
        if (static_cast<qint32>(socket->bytesAvailable()) < static_cast<qint32>(sizeof(qint32)))
            return; // the head info of size is incomplete
        while (socket->bytesAvailable())
        {
            if (static_cast<qint32>(socket->bytesAvailable()) >= static_cast<qint32>(sizeof(qint32)) && bytesToRead == 0)
                in >> bytesToRead;  // read the size of following data
            //qDebug() << "socket->bytesAvailable(): " << socket->bytesAvailable();
            if (socket->bytesAvailable() >= bytesToRead)
            {
                // read data
                //qDebug() << "start reading data...";
                char* temp = new char[static_cast<unsigned long>(bytesToRead + 1)]{ 0 };
                in.readRawData(temp, bytesToRead);
                //qDebug() << "socket->bytesAvailable(): " << socket->bytesAvailable();
                QByteArray buffer(temp, bytesToRead);
                QDataStream stream(&buffer, QIODevice::ReadWrite);
                stream >> data;
                bytesToRead = 0;
                emit transferPackage(data);
            }
           else
                break;  // the data is incomplete; read it next time
        }
    }

    template <class T>
    void installMyEventFilter(T* _obj)
    {
        _obj->installEventFilter(this);
    }

};





#endif  /*TOOLSFORNETWORKING_H*/
