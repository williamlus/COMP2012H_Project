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

    friend QDataStream& operator>>(QDataStream& in, Player_Info player_info){
        in >> player_info.player_index >>player_info.cards_remain >> player_info.role;
        return in;
    }

    friend QDataStream& operator<<(QDataStream& out, Player_Info player_info){
        out << player_info.player_index << player_info.cards_remain << player_info.role;
        return out;
    }

};

class DataPackage{
    public:

    //some clarification of the concrete usage of datapackage
    /*
     * At the confirm_ready stage, data_type = 0, there is NO message in this stage
       and all the players are intialitialized with sequential id (0~2), 17 cards_remain, null role (-1)

     * At the deal cards stage, data_type = 2, a random_index will be generate for the first player to choose landlord
       SERVER will send back a datapackage ONLY to the first player with RANDOM_INDEX with:
       message: "want lardlord?"

     * At the choose landlord stage, data_type = 1, there will be two different usages of datapackage
       If the information is not enough to determine a landlord, a datapackage will ONLY send to one player at a time
       For the message sended by CLIENT: the 0-index of datapackage will be either "yes" or "no"
       For the message sended by SERVER: the 0-index of datapackage will be "want landlord?"

       If landlord is decided, SERVER will send a datapackage to inform the client
       Then the cards in this datapackage will be three bonus cards
       The message will be "landlord is finalized"
       With role index refreshed for each player_id


     * At the play cards stage, data_type = 3
       There is NO message in this case
       Only with cards storing the cards to play and id equaling active_player_index
       This datapackage will first be sent out from the active_player's client,
       then broadcast to every client by server

     */

    int data_type;
    /* 0: confirm_ready
     * 1: choose landlord
     * 2: deal cards
     * 3: play cards
     */
    int id; // -1 for server,
            // 0,1,2 for client, which also stands for the id for the player
    QVector <Card> cards;
    QVector <Player_Info> player_info;
    QVector <QString> message;
    /*

     */


    DataPackage(int data_type=-1, int id=-2, QVector <Card> cards={}, QVector <Player_Info> player_info={}, QVector<QString> message={}) :
    data_type(data_type), id(id), cards(cards), player_info(player_info), message(message) {}

    //overloaded >> and << for Card:
    //QChar + int (stands for color and value)
    /*QChar for color:
    'E': EMPTY
    'S': SPADE
    'H': HEART
    'C': CLUB
    'D': DIAMOND
    'B': BLACK_JOKER
    'R': RED_JOKER
    */
    //overloaded << and >> for Player_Info:
    //player_index+cards_remain+role
    friend QDataStream& operator>>(QDataStream& in,  DataPackage& data){
        in >> data.data_type >> data.id >> data.cards >> data.player_info ;
        return in;
    }

    friend QDataStream& operator<<(QDataStream& out, const DataPackage& data){
        out << data.data_type << data.id << data.cards << data.player_info ;
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


    void read(QTcpSocket* socket);

    template <class T>
    void installMyEventFilter(T* _obj)
    {
        _obj->installEventFilter(this);
    }

};





#endif  /*TOOLSFORNETWORKING_H*/
