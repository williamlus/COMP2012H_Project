#ifndef PLAYWINDOW_H
#define PLAYWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QCloseEvent>

namespace Ui {
class PlayWindow;
}

class PlayWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayWindow(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *event) override;
    ~PlayWindow();

private slots:
    void on_pushButton_play_clicked();

    void on_pushButton_hint_clicked();

    void on_pushButton_give_up_clicked();

    void on_pushButton_want_landlord_clicked();

    void on_pushButton_want_farmer_clicked();

private:
    Ui::PlayWindow *ui;
};

#endif // PLAYWINDOW_H
