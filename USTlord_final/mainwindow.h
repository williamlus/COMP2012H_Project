#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QEvent>
#include <QSound>
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
    void paintEvent(QPaintEvent * event) override;
    void initialize_window();

private slots:
    void on_create_room_button_clicked();

    void on_start_offline_button_clicked();

    void on_join_room_button_clicked();

    void on_exit_button_clicked();

private:
    Ui::MainWindow *ui;
    ClientWindow* client_window{nullptr};
    PlayWindow* play_window{nullptr};
    ServerWindow* server_window{nullptr};
    QSound* bgm;
};
#endif // MAINWINDOW_H
