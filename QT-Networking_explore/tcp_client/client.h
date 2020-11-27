#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QByteArray>

QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Client : public QWidget
{
    Q_OBJECT

public:
    Client(QWidget *parent = nullptr);
    ~Client();

private slots:
    void on_ButtonSend_clicked();

    void on_ButtonConnect_clicked();

    void receiveMessage();

    void refreshState(QAbstractSocket::SocketState);

private:
    Ui::Client *ui;
    QTcpSocket *socket{nullptr};
};
#endif // CLIENT_H
