#ifndef PLAYWINDOW_H
#define PLAYWINDOW_H

#include <QMainWindow>
#include "source/datapackage.h"

namespace Ui {
class PlayWindow;
}

class PlayWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayWindow(QWidget *parent = nullptr);
    explicit PlayWindow(DataPackage data,QWidget *parent=nullptr);
    ~PlayWindow();

public slots:
    void receive_from_client(DataPackage data);

signals:
    void send_to_client(DataPackage data);

private:
    Ui::PlayWindow *ui;
};

#endif // PLAYWINDOW_H
