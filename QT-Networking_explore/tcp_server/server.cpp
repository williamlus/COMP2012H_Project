#include "server.h"

Server::Server(QObject *parent) : QTcpServer(parent)
{
}

void Server::sendMsgToClient(QByteArray msgToClient)
{
    if(this->sock!=nullptr){
        this->sock->write(msgToClient);
    }
}

void Server::incomingConnection(qintptr handle){
    const char* temp="Incoming Connection.";
    qDebug() << temp;
    std::cout << temp;
    sock=new QTcpSocket(this);
    sock->setSocketDescriptor(handle);
    QString ip=sock->peerAddress().toString();
    connect(sock,&QTcpSocket::readyRead,this,&Server::receiveMessage);
    connect(sock,&QTcpSocket::disconnected,this,&Server::clientDisconnected);
    emit msgChangedTo(temp,strlen(temp));
    emit clientIPChangedTo(ip,ip.size());
}

void Server::receiveMessage(){
    if(sock==nullptr){return;}
    msg_len=sock->bytesAvailable();
    if(msg_len>0){
        msg=new char[msg_capacity]{};
        sock->read(msg,msg_len);
        qDebug() << msg;
        std::cout << msg;
        if(strlen(whole_msg)>=(uint)msg_capacity-1-strlen(msg)){
            for(int i=0;i<msg_capacity;++i){
                whole_msg[i]=whole_msg[i+strlen(msg)];
            }
        }
        qDebug() << "strlen(whole_msg) : " << strlen(whole_msg);
        strcat(whole_msg,msg);
        emit msgChangedTo(whole_msg,strlen(whole_msg));
    }
}

void Server::clientDisconnected(){
    delete msg;
    //delete sock; >_<
    sock->deleteLater(); //:)
    // Don't need to delete it manually
    // because parent will delete it automatically
    sock=nullptr;
    whole_msg[0]='\0';
    msg=new char[msg_capacity];
    strcpy(msg,"Client Disconnected.");
    msg_len=strlen(msg);
    qDebug() << "Client disconnected.";
    qDebug() << "Delete Messages.";
    std::cout << "Client disconnected.";
    std::cout << "Delete Messages.";
    emit msgChangedTo(msg,msg_len);
    QString qs("Client IP : ");
    emit clientIPChangedTo(qs,qs.size());
}
