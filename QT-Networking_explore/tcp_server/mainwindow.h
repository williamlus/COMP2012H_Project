#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QString>
#include <iostream>
#include "server.h"

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
    void on_InitServerButton_clicked();
    void setMsg(const char* msg,int msg_len);
    void setClientIP(const QString ip,int ip_len);

    void on_StopServerButton_clicked();

    void on_sendButton_clicked();

private:
    Ui::MainWindow *ui;
    Server* server{nullptr};
};
#endif // MAINWINDOW_H
