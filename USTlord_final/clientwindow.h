#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "source/client.h"

namespace Ui {
class ClientWindow;
}

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientWindow(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *event) override;
    ~ClientWindow();

private slots:
    void on_pushButton_quit_clicked();

    void on_pushButton_join_server_clicked();

    void on_pushButton_stop_joining_clicked();

private:
    Ui::ClientWindow *ui;
    Client *client{nullptr};
};

#endif // CLIENTWINDOW_H
