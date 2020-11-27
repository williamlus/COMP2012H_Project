#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class GameWindow; }
QT_END_NAMESPACE

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();
    void initiate_window();
    void paintEvent(QPaintEvent * event);
    void initiate_offline_button();
    void initiate_online_button();
    void initiate_exit_button();
    void initiate_game_title();

private:
    Ui::GameWindow *ui;

    QPushButton* offline_button;
    QPushButton* online_button;
    QPushButton* exit_button;
    QPushButton* game_title;
    //non-UI data member
    bool offline_clickable{false};//Bool variable used to prevent button repeatly pressed
    bool online_clickable{false};
};
#endif // GAMEWINDOW_H
