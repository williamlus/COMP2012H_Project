#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "source/server.h"

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

    void on_pushButton_listen_clicked();

    void on_pushButton_stop_clicked();

    void on_pushButton_start_game_clicked();

private:
    Ui::ServerWindow *ui;
    Server *server{nullptr};
};

#endif // SERVERWINDOW_H
