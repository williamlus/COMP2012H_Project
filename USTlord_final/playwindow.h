#ifndef PLAYWINDOW_H
#define PLAYWINDOW_H

#include <vector>
#include <string>
#include <QMainWindow>
#include <QSound>
#include <QLabel>
#include "source/Card.h"
#include "source/CardsType.h"
#include "source/CardsGroup.h"
#include "source/Deck.h"
#include "source/CurrentPattern.h"
#include "source/Player.h"
#include "source/AIPlayer.h"
#include "source/datapackage.h"

namespace Ui {
class PlayWindow;
}

enum GAME_STAGE {
    DEFAULT,
    DEALING_CARDS,
    CALLING_LORD,
    PLAYING
};

enum GAME_MODE {
    OFFLINE,
    ONLINE
};

class PlayWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit PlayWindow(QWidget *parent = nullptr);
    explicit PlayWindow(int id, QVector<QString> names, QWidget *parent=nullptr);
    ~PlayWindow();
    //Game initializer
    void initialize_music();
    void initialize_window();
    void initialize_players();
    void initialize_players(QVector<QString> names);
    void initialize_cards();
    void initialize_offline_game();
    void initialize_online_game();
    //Game Process Functions
    void landlord_bonus(int landlord_id); //Add 3 bonus cards to landlord
    void shuffle(); //shuffle cards, distribute cards, call Deck::rearrange() to sort cards
    void choose_landlord();
    void game_finished(int current_player);
    void restart();
    //Helper Functions
    void set_chara_pic();
    void update_player_cards(int player_id);
    void sleep(unsigned int msec);
    void hide_past_cards();
    void reveal_current_selection();
    void AIplayer_action(int active_AIplayer);
    void clear_cp(int player_id);
    void closeEvent(QCloseEvent *event) override;
    void paintEvent(QPaintEvent * event) override;

private slots:
    void on_start_button_clicked();
    void card_selected();
    void on_call_lord_button_clicked();
    void on_refuse_button_clicked();
    void on_hit_button_clicked();
    void on_hint_button_clicked();
    void on_give_up_button_clicked();

public slots:
    void receive_from_client(DataPackage data);

signals:
    void send_to_client(DataPackage data);

private:
    Ui::PlayWindow *ui;
    //game  control
    GAME_STAGE game_status{DEFAULT};
    GAME_MODE mode{OFFLINE};
    //game process
    QVector<Player*> players;
    QVector<Card*> set_of_cards;//A complete set of 54 card
    QMap<CardPicture*, Card*> card_pic_map;
    QVector<int> call_landlord_list;
    vector<const Card*> current_selection;
    CurrentPattern* cp;
    bool game_finish = false, you_win= false, landlord_win = false;
    int landlord_id{-1};
    int hint_id{-1};
    int active_player_id{-1};
    int my_id{-1};

    //sound
    QSound* bgm;
    QSound* win_music;
    QSound* lose_music;
    QSound* shuffle_music;
    //player picture
    QLabel* player1_pic;
    QLabel* player2_pic;
    QLabel* player3_pic;
};

#endif // PLAYWINDOW_H

