#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "clientwindow.h"
#include "playwindow.h"
#include "serverwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_start_offline_clicked();

    void on_pushButton_create_room_clicked();

    void on_pushButton_join_room_clicked();

private:
    Ui::MainWindow *ui;
    ClientWindow* client_window{nullptr};
    PlayWindow* play_window{nullptr};
    ServerWindow* server_window{nullptr};
};
#endif // MAINWINDOW_H
