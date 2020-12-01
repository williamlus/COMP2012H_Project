#include "ToolsForNetworking.h"
#include <QtNetwork>

MyTools::MyTools(){}

void MyTools::read(QTcpSocket* socket)
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






