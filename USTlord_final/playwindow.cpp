#include <iostream>
#include <QPainter>
#include <QTime>
#include <QPixmap>
#include <QEvent>
#include <QSound>
#include <QDebug>
#include <QMessageBox>
#include <QLabel>
#include "playwindow.h"
#include "ui_playwindow.h"
#include "source/Card.h"

#define CARD_WIDTH 60
#define CARD_HEIGHT 80
#define OPPONENT_CARD_WIDTH 12
#define OPPONENT_CARD_HEIGHT 16
#define NUMBER_OF_PLAYERS 3
#define CARDS_INTERVAL 60
#define PLAYER1_X 20
#define PLAYER1_Y 280
#define PLAYER2_X 250
#define PLAYER2_Y 10
#define PLAYER3_X 880
#define PLAYER3_Y 10

void PlayWindow::receive_from_client(DataPackage data)
{
    //TODO
    if(data.action==DataPackage::EXCEPTION){
        if(data.content==DataPackage::Content::QUIT){
            this->close();
        }
    }
    else if(data.action == DataPackage::CHAT) {
        ui->chat_box->addItem(QString::fromStdString(players[data.actioner]->get_name())+":"+data.content);
    }
    else if(data.action == DataPackage::DEAL_CARDS) {
        set_of_cards = data.generate_cards();
        qDebug() << "number of cards:" << set_of_cards.size();///////
        initialize_cards();
        qDebug() << "initailize success";
        for(int i=0; i < NUMBER_OF_PLAYERS; i++) update_player_cards(i);
        qDebug() << "render landlord cards";
        for(int i=51; i<NUMBER_OF_CARDS; i++) {
            CardPicture* landlord_card = set_of_cards[i]->get_card_picture();
            landlord_card->select(false);
            landlord_card->turn(true);
            landlord_card->setGeometry(510+(i-51)*CARD_WIDTH, 160, CARD_WIDTH, CARD_HEIGHT);
            landlord_card->show();
        }
        qDebug() << "emit deal cards accept";
        emit send_to_client(DataPackage(my_id,my_id,DataPackage::Action::DEAL_CARDS,DataPackage::Content::ACCEPT));
    }
    else if(data.action == DataPackage::CHOOSE_LANDLORD) {
        if(data.content == DataPackage::Content::REQUEST) {
            ui->call_lord_button->setVisible(true);
            ui->refuse_button->setVisible(true);
        }
        else if(data.content==DataPackage::Content::BE_LANDLORD){////////////
            landlord_id = data.actioner;
            choose_landlord();
            landlord_bonus(landlord_id);
            if(my_id == landlord_id) {
                ui->hit_button->setVisible(true);
                ui->hint_button->setVisible(true);
                ui->give_up_button->setVisible(true);
            }
        }
    }
    else if(data.action == DataPackage::PLAY_CARDS && data.content!=DataPackage::Content::DO_NOT_PLAY && data.actioner != my_id) {
        current_selection.clear();
        QVector<Card*> generated_cards=data.generate_cards();
        for(int i=0,n=generated_cards.size();i<n;++i){
            for(int j=0,m=set_of_cards.size();j<m;++j){
                if(Card::strictly_equal(generated_cards[i],set_of_cards[j])){
                    qDebug() << "xixi";
                    current_selection.push_back(set_of_cards[j]);
                    qDebug() << "Other player play:"<<QString::fromStdString(set_of_cards[j]->get_string());
                    break;
                }
            }
        }
        qDebug() << "All cards found";
        for(int i=0,n=generated_cards.size();i<n;++i){
            delete generated_cards[i];
            generated_cards[i]=nullptr;
        }
        qDebug() << "delete temp generated cards";
        players[data.actioner]->set_selected_cards(current_selection);
        players[data.actioner]->set_choice('p');
        qDebug() << players[data.actioner]->play(*cp).get_cards();
        players[data.actioner]->set_selected_cards(players[data.actioner]->play(*cp).get_cards());
        hide_past_cards();
        sleep(200);
        reveal_current_selection();
        cp->record(data.actioner,current_selection,data.actioner==landlord_id);
        qDebug() << "current pattern:" << QString::fromStdString(cp->get_cards_type().to_string());
        qDebug() << "current pattern player index: " << cp->get_player_index();
        players[data.actioner]->clear_hint(); //clear the cards to be played in deck
        hint_id=-1;
        players[data.actioner]->clear_cards(current_selection);
        vector<Card const*> empty_cards(0,nullptr);
        players[my_id]->set_selected_cards(empty_cards);
        ui->info_bar->setText(QString::fromStdString("Player " + to_string(data.actioner) + " play " + cp->get_cards_type().to_string()));
        players[(data.actioner+1)%3]->set_turn_end(false);
        update_player_cards(data.actioner);
        current_selection.clear();//////
        qDebug() << "My id:" << my_id;
        for(int i=0; i<NUMBER_OF_PLAYERS; i++) {
            if(players[i]->get_num_cards()==0) game_finished(i);
        }
        if((my_id+2)%3 == data.actioner) {
            ui->hit_button->setVisible(true);
            ui->hint_button->setVisible(true);
            ui->give_up_button->setVisible(true);
        }
    }
    else if(data.action == DataPackage::PLAY_CARDS && data.content == DataPackage::Content::DO_NOT_PLAY && data.actioner!=my_id) {
        players[data.actioner]->set_choice('g');
        players[data.actioner]->set_selected_cards(players[data.actioner]->play(*cp).get_cards());
        players[data.actioner]->clear_hint(); //clear the cards to be played in deck
        hint_id=-1;
        current_selection.clear();
        update_player_cards(data.actioner);
        players[(data.actioner+1)%3]->set_turn_end(false);
        if((my_id+2)%3 == data.actioner) {
            ui->hit_button->setVisible(true);
            ui->hint_button->setVisible(true);
            ui->give_up_button->setVisible(true);
        }
    }
}

/*
 *Constructors
 * Initialize foundamentals for both online and offline game
 * including window setting, music and cards
 */
PlayWindow::PlayWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlayWindow)
{
    ui->setupUi(this);
    initialize_window();
    initialize_music();
}

PlayWindow::PlayWindow(int id, QVector<QString> names, QWidget *parent): //initialize online game.
    QMainWindow(parent),
    ui(new Ui::PlayWindow)
{
    //TODO
    ui->setupUi(this);
    my_id = id;
    mode = ONLINE;
    initialize_window();
    initialize_music();
    initialize_players(names);
}

/*
 * Destructor
 */
PlayWindow::~PlayWindow()
{
    delete ui;
    /*for(int i=0;i < static_cast<int>(this->set_of_cards.size());i++){
        delete this->set_of_cards[i];
    }
    this->set_of_cards.clear();
    for(int i = 0 ; i < NUMBER_OF_PLAYERS; i++){
        delete this->players[i];
    }
    this->players.clear();
    delete this->cp;*/
}

void PlayWindow::initialize_window() {
    setFixedSize(1200, 600);
    if(mode == OFFLINE) {
       ui->chat_box->setVisible(false);
       ui->line_edit->setVisible(false);
       ui->enter_button->setVisible(false);
    }
    ui->info_bar->setStyleSheet("color:white;");
    ui->start_button->setStyleSheet(
                                          "QPushButton{"
                                          "background-color:white;"//color
                                          "border-style:outset;"                  //inset/outset
                                          "border-width:4px;"
                                          "border-radius:10px;"
                                          "border-color:black;"    //border color
                                          "font:bold 25px;"                       //character and size
                                          "color:black;"                //character color
                                          "padding:6px;"                          //padding
                                          "}");
    ui->start_button->setCursor(Qt::PointingHandCursor);
    if(mode==ONLINE){ui->start_button->setVisible(false);}
    ui->hit_button->setVisible(false);
    ui->hit_button->setStyleSheet(
                                          "QPushButton{"
                                          "background-color:white;"//color
                                          "border-style:outset;"                  //inset/outset
                                          "border-width:4px;"
                                          "border-radius:10px;"
                                          "border-color:black;"    //border color
                                          "font:bold 25px;"                       //character and size
                                          "color:black;"                //character color
                                          "padding:6px;"                          //padding
                                          "}");
    ui->hit_button->setCursor(Qt::PointingHandCursor);
    ui->call_lord_button->setVisible(false);
    ui->call_lord_button->setStyleSheet(
                                          "QPushButton{"
                                          "background-color:white;"//color
                                          "border-style:outset;"                  //inset/outset
                                          "border-width:4px;"
                                          "border-radius:10px;"
                                          "border-color:black;"    //border color
                                          "font:bold 10px;"                       //character and size
                                          "color:black;"                //character color
                                          "padding:6px;"                          //padding
                                          "}");
    ui->call_lord_button->setCursor(Qt::PointingHandCursor);
    ui->hint_button->setVisible(false);
    ui->hint_button->setStyleSheet(
                                          "QPushButton{"
                                          "background-color:white;"//color
                                          "border-style:outset;"                  //inset/outset
                                          "border-width:4px;"
                                          "border-radius:10px;"
                                          "border-color:black;"    //border color
                                          "font:bold 25px;"                       //character and size
                                          "color:black;"                //character color
                                          "padding:6px;"                          //padding
                                          "}");
    ui->hint_button->setCursor(Qt::PointingHandCursor);
    ui->refuse_button->setVisible(false);
    ui->refuse_button->setStyleSheet(
                                          "QPushButton{"
                                          "background-color:white;"//color
                                          "border-style:outset;"                  //inset/outset
                                          "border-width:4px;"
                                          "border-radius:10px;"
                                          "border-color:black;"    //border color
                                          "font:bold 15px;"                       //character and size
                                          "color:black;"                //character color
                                          "padding:6px;"                          //padding
                                          "}");
    ui->refuse_button->setCursor(Qt::PointingHandCursor);
    ui->give_up_button->setVisible(false);
    ui->give_up_button->setStyleSheet(
                                          "QPushButton{"
                                          "background-color:white;"//color
                                          "border-style:outset;"                  //inset/outset
                                          "border-width:4px;"
                                          "border-radius:10px;"
                                          "border-color:black;"    //border color
                                          "font:bold 14px;"                       //character and size
                                          "color:black;"                //character color
                                          "padding:6px;"                          //padding
                                          "}");
    ui->give_up_button->setCursor(Qt::PointingHandCursor);
}

void PlayWindow::initialize_music() { //initialize music
    bgm = new QSound(":/sound/sound/bgm2.wav");
    win_music = new QSound(":/sound/sound/win.wav");
    lose_music = new QSound(":/sound/sound/loseMusic.wav");
    shuffle_music = new QSound(":/sound/sound/shuffleCardsMusic.wav");
    bgm->play();
    bgm->setLoops(-1);
}

void PlayWindow::initialize_cards() { //initialize 54 cards
    if(mode == OFFLINE) {
        char color;
        for(int i=0;i<54;++i){
            char figure;
            if(i<52){
                figure = figures_to_int[i%13];
                int j = i/13;
                switch(j){
                case 0:
                    color = 's';
                    break;
                case 1:
                    color = 'h';
                    break;
                case 2:
                    color = 'd';
                    break;
                case 3:
                    color = 'c';
                    break;
                }
            }
            else if(i==52){
                figure = figures_to_int[NUMBER_OF_FIGURES-1];
                color = 'b';
            }
            else{
                figure = figures_to_int[NUMBER_OF_FIGURES-1];
                color = 'r';
            }

            string pic_name = ":/card/images/" + to_string(i+1) + ".png";
            Card* card = new Card(color, figure);
            QPixmap pic;
            pic.load(QString::fromStdString(pic_name));
            qDebug() << QString::fromStdString(pic_name);
            CardPicture* card_picture = new CardPicture(this);
            connect(card_picture, SIGNAL(selected_signal()),this,SLOT(card_selected()));
            card_picture->setPic(pic);
            card_picture->hide();
            card_picture->setGeometry(570, 260, CARD_WIDTH, CARD_HEIGHT);
            card->set_card_picture(card_picture);
            set_of_cards.push_back(card);
            card_pic_map.insert(card_picture, card);
        }
    }
    else {
        for(int i=0, j=0; i < NUMBER_OF_PLAYERS; i++) {
            for(int k = 0; k < 17; k++) {
                Card* card = set_of_cards[j++];
                string pic_name = ":/card/images/" + to_string(card->get_index()+1) + ".png";
                QPixmap pic;
                pic.load(QString::fromStdString(pic_name));
                qDebug() << QString::fromStdString(pic_name);
                CardPicture* card_picture = new CardPicture(this);
                connect(card_picture, SIGNAL(selected_signal()),this,SLOT(card_selected()));
                card_picture->setPic(pic);
                card_picture->hide();
                card_picture->setGeometry(570, 260, CARD_WIDTH, CARD_HEIGHT);
                card->set_card_picture(card_picture);
                card_pic_map.insert(card_picture, card);
                players[i]->receive_card(card);
                update_player_cards(i);
                sleep(50);////////
            }
        }
        for(int i=51; i<NUMBER_OF_CARDS; i++) {
            Card* card = set_of_cards[i];
            string pic_name = ":/card/images/" + to_string(card->get_index()+1) + ".png";
            QPixmap pic;
            pic.load(QString::fromStdString(pic_name));
            qDebug() << QString::fromStdString(pic_name);
            CardPicture* card_picture = new CardPicture(this);
            connect(card_picture, SIGNAL(selected_signal()),this,SLOT(card_selected()));
            card_picture->setPic(pic);
            card_picture->hide();
            card_picture->setGeometry(570, 260, CARD_WIDTH, CARD_HEIGHT);
            card->set_card_picture(card_picture);
            card_pic_map.insert(card_picture, card);
        }
    }
}

void PlayWindow::initialize_offline_game() { //initialize offline game

    my_id = 0;

    initialize_cards();
    ui->start_button->setVisible(false);
    initialize_players();
    shuffle();
    for(int p = 0; p < NUMBER_OF_PLAYERS; p++) update_player_cards(p);
    this->cp=new CurrentPattern();
    game_finish = false;
    you_win = false;
    landlord_win = false;
    landlord_id = -1;
    ui->call_lord_button->setVisible(true);
    ui->refuse_button->setVisible(true);
}

void PlayWindow::initialize_players() { // initialize players in offline mode
    qDebug() << "Azhe";
    Player* player1 = new Player(0,"You");
    Player* player2 = new AIPlayer(1,"AI "+to_string(1));
    Player* player3 = new AIPlayer(2,"AI "+to_string(2));
    this->players.append(player1);
    this->players.append(player2);
    this->players.append(player3);

    player1_pic = new QLabel(this);
    player1_pic->setGeometry(PLAYER1_X, PLAYER1_Y, 84, 120);
    player2_pic = new QLabel(this);
    player2_pic->setGeometry(PLAYER2_X, PLAYER2_Y, 84, 120);
    player3_pic = new QLabel(this);
    player3_pic->setGeometry(PLAYER3_X, PLAYER3_Y, 84, 120);
    //set names displayed in ui.
    ui->player0_name->setText("YOU");
    ui->player0_name->setStyleSheet("color:white;");
    ui->player1_name->setText("PLAYER 1");
    ui->player1_name->setStyleSheet("color:white;");
    ui->player2_name->setText("PLAYER 2");
    ui->player2_name->setStyleSheet("color:white;");
}

void PlayWindow::initialize_players(QVector<QString> names) { // initialize players in online mode
    for(int i=0; i<NUMBER_OF_PLAYERS; i++) {
        Player* new_player;
        new_player = new Player(i, names[i].toStdString());
        this->players.append(new_player);
        new_player->set_turn_end(true);
        }
    player1_pic = new QLabel(this);
    player1_pic->setGeometry(PLAYER1_X, PLAYER1_Y, 84, 120);
    player2_pic = new QLabel(this);
    player2_pic->setGeometry(PLAYER2_X, PLAYER2_Y, 84, 120);
    player3_pic = new QLabel(this);
    player3_pic->setGeometry(PLAYER3_X, PLAYER3_Y, 84, 120);
    //set names displayed in ui.
    ui->player0_name->setText("YOU #" +QString::number(my_id));
    ui->player0_name->setStyleSheet("color:white;");
    ui->player1_name->setText(QString::fromStdString(players[(my_id+1)%NUMBER_OF_PLAYERS]->get_name()));
    ui->player1_name->setStyleSheet("color:white;");
    ui->player2_name->setText(QString::fromStdString(players[(my_id+2)%NUMBER_OF_PLAYERS]->get_name()));
    ui->player2_name->setStyleSheet("color:white;");
}

 /*
 * Game Processing Functions
 * shuffle
 * card_selected
 * AIplayer_action
 * choose_landlord
 * landlord_bonus
 * restart
 * game_finished
 */

void PlayWindow::shuffle() //shuffle cards, distribute cards and sort cards
{
    for(int i = 0; i < NUMBER_OF_CARDS; i++) {
        swap(set_of_cards[i], set_of_cards[rand()%NUMBER_OF_CARDS]);
    }
    CardPicture* card_pic;
    bgm->stop();
    shuffle_music->play();
    shuffle_music->setLoops(-1);
    for(int k = 0, i = 0; i < NUMBER_OF_PLAYERS; i++) {
        players[i]->abandom_cards();
        for(int j = 0; j < 17; j++) {
            card_pic = this->set_of_cards[k]->get_card_picture();
            card_pic->select(false);
            if(i > 0) {
                card_pic->turn(true);
                card_pic->resize(OPPONENT_CARD_WIDTH, OPPONENT_CARD_HEIGHT);
            }
            else {
                card_pic->turn(false);
                card_pic->resize(CARD_WIDTH, CARD_HEIGHT);
            }
            card_pic->show();
            this->players[i]->receive_card(this->set_of_cards[k++]);
            update_player_cards(i);
            sleep(50);
        }
    }
    for(int k = 51; k < NUMBER_OF_CARDS; k++) {
        card_pic = this->set_of_cards[k]->get_card_picture();
        card_pic->select(false);
        card_pic->turn(true);
        card_pic->show();
        card_pic->setGeometry(510+CARD_WIDTH*(k-51), 160, CARD_WIDTH, CARD_HEIGHT);
    }
    shuffle_music->stop();
    bgm->play();
}

void PlayWindow::card_selected() { // handle card selection in ui
    if(game_status == DEFAULT || game_status == DEALING_CARDS) return;
    if(players[my_id]->turn_end()) return;
    qDebug() << "Signal Received!";
    CardPicture* card_pic = (CardPicture*) sender();
    qDebug() << card_pic->is_selected();
    if(card_pic->is_selected())
    {
        card_pic->select(false);
        for(vector<const Card*>::iterator it = current_selection.begin(); it != current_selection.end(); it++)
        {
            if(*it == card_pic_map[card_pic])
            {
                qDebug() << "Selection Removed!";
                current_selection.erase(it);
                break;
            }
        }
    }
    else
    {
        qDebug() << "Select!";
        card_pic->select(true);
        qDebug() << "Selection added!";
        current_selection.push_back(card_pic_map[card_pic]);
        //m_Selected.insert(cardpic); add this card to selected set
    }

    update_player_cards(my_id);
}

void PlayWindow::AIplayer_action(int active_AIplayer){ //AI player play cards

    sleep(1000);
    CardsGroup tmp = players[active_AIplayer]->play(*cp);
    vector<const Card*> tmpcard = tmp.get_cards();
    sleep(1000);
    if(!tmp.get_cards().empty()){
//        qDebug() << "here1";
        for(int i = 0; i < tmpcard.size(); i++){
            current_selection.push_back(tmpcard[i]);
        }
        hide_past_cards();
        sleep(300);
        reveal_current_selection();
        sleep(200);
        cp->record(active_AIplayer,tmp,active_AIplayer==landlord_id);
        qDebug() << QString::fromStdString(" play " + cp->get_cards_type().to_string());
        ui->info_bar->setText(QString::fromStdString("Player ") +QString::number(active_AIplayer)+ QString::fromStdString(" plays " + cp->get_cards_type().to_string()));
        update_player_cards(active_AIplayer);
        current_selection.clear();
        qDebug() << QString::fromStdString(" play " + cp->get_cards_type().to_string());
        sleep(1000);
        current_selection.clear();
    }else{
        ui->info_bar->setText(QString::fromStdString("Player ") +QString::number(active_AIplayer)+ QString::fromStdString(" gives up."));
        sleep(1000);
    }
}

void PlayWindow::choose_landlord(){ //handle choose landlord game process
    players[my_id]->set_turn_end(true);
    players[(my_id+1)%NUMBER_OF_PLAYERS]->set_turn_end(true);
    players[(my_id+2)%NUMBER_OF_PLAYERS]->set_turn_end(true);
    if(mode == OFFLINE) {
        bool decision;
        decision = players[1]->want_landlord();
        players[(my_id+1)%NUMBER_OF_PLAYERS]->display_cards();
        if(decision) {
            ui->info_bar->setText("Player 1 calls landlord!");
            sleep(800);
            call_landlord_list.push_front((my_id+1)%NUMBER_OF_PLAYERS);
        }
        decision = players[(my_id+2)%NUMBER_OF_PLAYERS]->want_landlord();
        players[(my_id+2)%NUMBER_OF_PLAYERS]->display_cards();
        if(decision) {
            ui->info_bar->setText("Player 2 calls landlord!");
            sleep(800);
            call_landlord_list.push_front((my_id+2)%NUMBER_OF_PLAYERS);
        }
        if(call_landlord_list.size() == 0) {
            ui->info_bar->setText("No one calls landlord! Redistribute the cards now.");
            sleep(800);
            restart();
            return;
        }
        else {
            landlord_id = call_landlord_list[0];
            qDebug() << landlord_id << "----1";
            landlord_bonus(landlord_id);
            game_status = PLAYING;
            if(landlord_id == my_id){
                ui->info_bar->setText(QString::fromStdString("You are landlord!"));
            }
            else{
            ui->info_bar->setText(QString::fromStdString("Player ") + QString::number(landlord_id) + QString::fromStdString(" is the landlord now!"));
            }
            sleep(800);
            set_chara_pic();
        }
        for(int i = landlord_id; i != my_id;){
            ui->info_bar->setText(QString::fromStdString("Player ") + QString::number(i) + QString::fromStdString("'s turn now"));
            sleep(800);
            AIplayer_action(i);
            i = (++i)%NUMBER_OF_PLAYERS;
        }

        if(cp->get_player_index()!= my_id){
           ui->info_bar->setText(QString::fromStdString("Player ") + QString::number(cp->get_player_index()) + QString::fromStdString(" played. \n Now it's your turn."));}
        else{
           ui->info_bar->setText("Now it's your turn.");
        }
        players[my_id]->set_turn_end(false);
        sleep(1000);
        ui->hit_button->setVisible(true);
        ui->hint_button->setVisible(true);
        ui->give_up_button->setVisible(true);
    }
    else {
        game_status = PLAYING;
        ui->info_bar->setText(QString::fromStdString("Player ") + QString::number(landlord_id) + QString::fromStdString(" is the landlord now!"));
        players[landlord_id]->set_turn_end(false);
        vector<const Card*> empty_cards{};
        cp=new CurrentPattern(empty_cards,landlord_id);
        set_chara_pic();
    }
}

void PlayWindow::landlord_bonus(int landlord_id) // distribute 3 landlord bonus cards to the landlord
{
    for(int i = NUMBER_OF_PLAYERS * 17 ; i < NUMBER_OF_CARDS; i++){
        CardPicture* card_picture = this->set_of_cards[i]->get_card_picture();
        card_picture->turn(false);
        card_picture->repaint();
    }
    sleep(1000);
    for(int i = NUMBER_OF_PLAYERS * 17 ; i < NUMBER_OF_CARDS; i++){
        players[landlord_id]->receive_card(set_of_cards[i]); //add cards into landlord player's cards
    }
    for(int i = NUMBER_OF_PLAYERS * 17 ; i < NUMBER_OF_CARDS; i++){
        CardPicture* card_picture = this->set_of_cards[i]->get_card_picture();
        card_picture->turn(true);
        card_picture->repaint();
    }
    qDebug()<< "Current num cards:" << players[landlord_id]->get_num_cards();
    update_player_cards(landlord_id);
};

void PlayWindow::restart() { //restart the game if no one calls landlord
    qDebug() << "No landlord! Restart!";
    for(int k = 0; k < NUMBER_OF_CARDS; k++) {
        CardPicture* card_pic = this->set_of_cards[k]->get_card_picture();
        card_pic->hide();
    }
    players.clear();
    initialize_offline_game();
}

void PlayWindow::game_finished(int current_player){ //handle game finished process
    game_finish = true;

        if(current_player == landlord_id){
            landlord_win = true;
        }else{
            landlord_win = false;
        }

        if((my_id != landlord_id && !landlord_win)||(my_id == landlord_id && landlord_win)){
            you_win = true;
        }else{
            you_win = false;
        }

        if(!you_win && landlord_win){
            bgm->stop();
            lose_music->play();
             QMessageBox::information(NULL, "", "Game End!! \n You Lose na :(! \n Landlord wins!", QMessageBox::Yes, QMessageBox::Yes);
        }
        else if(you_win && landlord_win){
            bgm->stop();
            win_music->play();
             QMessageBox::information(NULL, "", "Game End!! \n You Win na :)! \n Landlord win!", QMessageBox::Yes, QMessageBox::Yes);
        }
        else if(!you_win && !landlord_win){
            bgm->stop();
            lose_music->play();
             QMessageBox::information(NULL, "", "Game End!! \n You Lose na :(! \n Farmers win!", QMessageBox::Yes, QMessageBox::Yes);
        }
        else if(you_win && !landlord_win){
            bgm->stop();
            win_music->play();
             QMessageBox::information(NULL, "", "Game End!! \n You Win na :)! \n Farmers win!", QMessageBox::Yes, QMessageBox::Yes);
        }
    close();
}

/*
 * Button slots
 * START
 * CALL LORD
 * REFUSE
 * HIT
 * HINT
 * GIVE UP
*/
void PlayWindow::on_start_button_clicked() // handle game start when clicking start button
{
    if(mode == OFFLINE) {
        ui->info_bar->setText("Game Start");
        //sleep(300);
        initialize_offline_game();
        ui->info_bar->setText("Please choose landlord");
    }
    else {
        ui->info_bar->setText("Game Start");
        ui->info_bar->setText("Please choose landlord");
    }
}

void PlayWindow::on_call_lord_button_clicked() // handle call_lord action if player choose to call landlord when calling landlord
{
    ui->info_bar->setText("You call landlord!");
    sleep(500);
    if(mode == OFFLINE) {
        call_landlord_list.push_front((my_id+0)%NUMBER_OF_PLAYERS);
        ui->refuse_button->setVisible(false);
        ui->call_lord_button->setVisible(false);
        choose_landlord();
    }
    else {
        ui->call_lord_button->setVisible(false);
        ui->refuse_button->setVisible(false);
        emit send_to_client(DataPackage(my_id, my_id, DataPackage::CHOOSE_LANDLORD, DataPackage::Content::ACCEPT));
    }
}

void PlayWindow::on_refuse_button_clicked() // handle refuse action if player choose to refuse when calling landlord
{
    ui->info_bar->setText("You refuse to be the landlord!");
    sleep(500);

    if(mode == OFFLINE) {
        ui->refuse_button->setVisible(false);
        ui->call_lord_button->setVisible(false);
        choose_landlord();
    }
    else {
        ui->call_lord_button->setVisible(false);
        ui->refuse_button->setVisible(false);
        emit send_to_client(DataPackage(my_id, my_id, DataPackage::CHOOSE_LANDLORD, DataPackage::Content::REJECT));
    }
}

void PlayWindow::on_hit_button_clicked() // handle play cards action when the player clicks hit button
{
    ui->info_bar->clear();

    sleep(100);
    players[my_id]->set_selected_cards(current_selection);
    players[my_id]->set_choice('p');
    players[my_id]->set_selected_cards(players[my_id]->play(*cp).get_cards());

    if(players[my_id]->get_selected_cards().get_cards().size() == 0){
        ui->info_bar->setText("Invalid cards group! \n Please choose again!");
        sleep(500);
        if(cp->get_player_index()!=0){
           ui->info_bar->setText(QString::fromStdString("Player ") + QString::number(cp->get_player_index()) + QString::fromStdString(" played. \n Now it's your turn."));}
        else{
           ui->info_bar->setText("Now it's your turn.");
        }
        return;
    }
    else {
            hide_past_cards();
            sleep(200);
            cp->record(my_id,current_selection,my_id == landlord_id);
            players[my_id]->clear_hint(); //clear the cards to be played in deck
            hint_id=-1;
            players[my_id]->clear_cards(current_selection);
            vector<Card const*> empty_cards(0,nullptr);
            players[my_id]->set_selected_cards(empty_cards);
            ui->info_bar->setText(QString::fromStdString("You play " + cp->get_cards_type().to_string()));
            update_player_cards(my_id);
            current_selection.clear();

            ui->hit_button->setVisible(false);
            ui->hint_button->setVisible(false);
            ui->give_up_button->setVisible(false);
            sleep(800);


            if(mode == OFFLINE) {
                if(players[0]->is_winner()){
                    ui->info_bar->setText("Game Finished!");
                    sleep(800);
                    game_finished((my_id+0)%NUMBER_OF_PLAYERS);
                }

                ui->info_bar->setText("Player 1's turn!");
                AIplayer_action((my_id+1)%NUMBER_OF_PLAYERS);
                sleep(800);
                if(players[1]->is_winner()){
                    ui->info_bar->setText("Game Finished!");
                    sleep(800);
                    game_finished((my_id+1)%NUMBER_OF_PLAYERS);
                }

                ui->info_bar->setText("Player 2's turn!");
                AIplayer_action((my_id+2)%NUMBER_OF_PLAYERS);
                sleep(800);
                if(players[2]->is_winner()){
                    ui->info_bar->setText("Game Finished!");
                    sleep(800);
                    game_finished((my_id+2)%NUMBER_OF_PLAYERS);
                }

                players[1]->set_turn_end(false);
                players[2]->set_turn_end(false);
                players[0]->set_turn_end(false);
                sleep(800);

                if(cp->get_player_index()!=0){
                   ui->info_bar->setText(QString::fromStdString("Player ") + QString::number(cp->get_player_index()) + QString::fromStdString(" played. \n Now it's your turn."));}
                else{
                   ui->info_bar->setText("Now it's your turn.");
                }

                ui->hit_button->setVisible(true);
                ui->hint_button->setVisible(true);
                ui->give_up_button->setVisible(true);
            }
            else {
                vector<const Card*> cards = cp->get_cards();
                QVector<Card> cards_to_send{};
                for(int i=0,n=cards.size();i<n;++i){
                    Card c=*cards[i];
                    cards_to_send.push_back(c);
                }
                DataPackage data(DataPackage(my_id, my_id, DataPackage::PLAY_CARDS, DataPackage::cards_to_string(cards_to_send)));
                qDebug() << "Play cards:" <<data.to_string();
                emit send_to_client(data);
                ui->hit_button->setVisible(false);
                ui->hint_button->setVisible(false);
                ui->give_up_button->setVisible(false);
                if(players[my_id]->get_num_cards()==0) game_finished(my_id);
            }
        }
}

void PlayWindow::on_hint_button_clicked() //handle hint generation and display when player clicks hint button
{
    for(auto card : players[my_id]->get_deck()->get_cards()) {
        card->get_card_picture()->select(false);
    }
    players[my_id]->calc_hints(*cp);
    if(players[my_id]->get_hints().size() == 0){
        ui->info_bar->setText("No hints.");
        current_selection.clear();
        update_player_cards(my_id);
        sleep(800);

        if(cp->get_player_index()!=0){
           ui->info_bar->setText(QString::fromStdString("Player ") + QString::number(cp->get_player_index()) + QString::fromStdString(" played. \n Now it's your turn."));}
        else{
           ui->info_bar->setText("Now it's your turn.");
        }

        return;
    }else{
        current_selection.clear();
        if(hint_id == -1){ hint_id = 0;}
        else{
            hint_id = (hint_id+1)%players[my_id]->get_hints().size();
        }
        for(auto card : players[my_id]->get_hints()[hint_id].get_cards()) {
            card->get_card_picture()->select(true);
            current_selection.push_back(card);
        }
        players[my_id]->set_selected_cards(players[my_id]->get_hints()[hint_id].get_cards());
        update_player_cards(my_id);    //display calculated hints
    }
}

void PlayWindow::on_give_up_button_clicked() //handle give up action when player click give up
{
        ui->info_bar->setText("You give up, next player's turn.");
        sleep(1000);
        players[my_id]->set_choice('g');
        players[my_id]->set_selected_cards(players[my_id]->play(*cp).get_cards()); //player gives up
        current_selection.clear();
        update_player_cards(my_id);

        for(int i = 0; i < players[my_id]->get_num_cards(); i++){
            players[my_id]->get_deck()->get_cards()[i]->get_card_picture()->select(false);
        }
        update_player_cards(my_id);

        if(mode == OFFLINE) {
            if(players[my_id]->turn_end()){ // player action finished
                ui->hit_button->setVisible(false);
                ui->hint_button->setVisible(false);
                ui->give_up_button->setVisible(false);
                hint_id = -1;

                if(cp->get_player_index() == my_id){//last time still me, clear the current pattern stored.
                   hide_past_cards();
                    //recreate a new pattern.
                   if(!cp->get_cards().empty()){ delete cp;}
                   cp = new CurrentPattern();
                   cp->set_player_index((my_id+1)%NUMBER_OF_PLAYERS);
                   hint_id=-1;
                 }

                if(cp->get_player_index() == (my_id+1)%NUMBER_OF_PLAYERS){
                   hide_past_cards();
                }
                AIplayer_action((my_id+1)%NUMBER_OF_PLAYERS);
                sleep(300);
                if(players[(my_id+1)%NUMBER_OF_PLAYERS]->is_winner()){//check if it is winner.
                    ui->info_bar->setText("Game Finished!");
                    sleep(800);
                    game_finished((my_id+1)%NUMBER_OF_PLAYERS);
                }

                if(cp->get_player_index() == (my_id+2)%NUMBER_OF_PLAYERS){
                   hide_past_cards();
                }
                AIplayer_action((my_id+2)%NUMBER_OF_PLAYERS);
                sleep(300);
                if(players[(my_id+2)%NUMBER_OF_PLAYERS]->is_winner()){
                    ui->info_bar->setText("Game Finished!");
                    sleep(800);
                    game_finished((my_id+2)%NUMBER_OF_PLAYERS);
                }

                players[1]->set_turn_end(false);
                players[2]->set_turn_end(false);
                players[0]->set_turn_end(false);

                sleep(800);

                ui->hit_button->setVisible(true);
                ui->hint_button->setVisible(true);
                ui->give_up_button->setVisible(true);
        }
    }
    else {
        players[my_id]->clear_hint();/////
        ui->hit_button->setVisible(false);
        ui->hint_button->setVisible(false);
        ui->give_up_button->setVisible(false);
        emit send_to_client(DataPackage(my_id, my_id, DataPackage::PLAY_CARDS, DataPackage::Content::DO_NOT_PLAY));
    }
}

void PlayWindow::on_enter_button_clicked() {
    if(ui->line_edit->text().isEmpty()) return;
    else send_to_client(DataPackage(my_id, my_id, DataPackage::CHAT, ui->line_edit->text()));
    ui->line_edit->clear();
}

/*
 * Helper Functions
 * sleep
 * set_chara_pic
 * hide_past_cards
 * update_player_cards
 * reveal current selection
 */
void PlayWindow::sleep(unsigned int msec)
{
    QTime reach_time = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < reach_time) QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void PlayWindow::set_chara_pic() { // set character pictures
    if(landlord_id == -1) return;
    QPixmap farmer_pixmap(":/icon/images/farmer.png");
    QPixmap lord_pixmap(":/icon/images/lord.png");
    if(landlord_id == my_id) {
        player1_pic->setPixmap(lord_pixmap);
        player1_pic->show();
    }
    else {
        player1_pic->setPixmap(farmer_pixmap);
        player1_pic->show();
    }
    if(landlord_id == (my_id+1)%3) {
        player2_pic->setPixmap(lord_pixmap);
        player2_pic->show();
    }
    else {
        player2_pic->setPixmap(farmer_pixmap);
        player2_pic->show();
    }
    if(landlord_id == (my_id+2)%3) {
        player3_pic->setPixmap(lord_pixmap);
        player3_pic->show();
    }
    else {
        player3_pic->setPixmap(farmer_pixmap);
        player3_pic->show();
    }
}

void PlayWindow::hide_past_cards() { //hide past cards in the ui
    for(auto card : cp->get_cards()) {
        card->get_card_picture()->hide();
    }
}

void PlayWindow::update_player_cards(int player_id)
{
    if(player_id < 0 || player_id > 2) return;
    players[player_id]->get_deck()->rearrange();
    //render cards in hand
    if(player_id == my_id)
    {
        for(int j = 0; j < players[player_id]->get_deck()->get_num_cards(); j++)
        {
            int top_border = 450;                           //upper border
            int left_border = (1200-CARD_WIDTH*players[my_id]->get_num_cards())/2;  //left border
            CardPicture* card_picture = players[player_id]->get_deck()->get_cards()[j]->get_card_picture();
            card_picture->turn(false);
            if(card_picture->is_selected()) top_border-=20;
            card_picture->move(left_border+j*CARDS_INTERVAL, top_border);
            card_picture->repaint();
            card_picture->show();
            players[my_id]->display_cards();
        }
    }
    else if(player_id == (my_id+1)%NUMBER_OF_PLAYERS) {
        int left_border = 40;
        int top_border = 40;
        for(int j = 0; j < players[player_id]->get_deck()->get_num_cards(); j++) {
            CardPicture* card_picture = players[player_id]->get_deck()->get_cards()[j]->get_card_picture();
            card_picture->setGeometry(left_border+j*12, top_border, OPPONENT_CARD_WIDTH, OPPONENT_CARD_HEIGHT);
            card_picture->turn(true);
            card_picture->repaint();
            card_picture->show();
        }
    }
    else {
        int left_border = 956;
        int top_border = 40;
        for(int j = 0; j < players[player_id]->get_deck()->get_num_cards(); j++) {
            CardPicture* card_picture = players[player_id]->get_deck()->get_cards()[j]->get_card_picture();
            card_picture->setGeometry(left_border+j*12, top_border, OPPONENT_CARD_WIDTH, OPPONENT_CARD_HEIGHT);
            card_picture->turn(true);
            card_picture->repaint();
            card_picture->show();
        }
    }
    if(game_status == DEFAULT || game_status == DEALING_CARDS) return;
    if(players[player_id]->turn_end()) {
        if(current_selection.size() == 0) return;
        else {
            int i = 0;
            for(auto card : current_selection) {
                card->get_card_picture()->setGeometry((1200-CARD_WIDTH*current_selection.size())/2+CARD_WIDTH*i, 160, CARD_WIDTH, CARD_HEIGHT);
                i++;
            }
        }
    }
}

void PlayWindow::reveal_current_selection() {
    for(auto card : current_selection) {
        card->get_card_picture()->turn(false);
        card->get_card_picture()->repaint();
    }
}

/*
 * Event handler
 * paintEvent
 * closeEvent
 */
void PlayWindow::paintEvent(QPaintEvent * event) {
    static QPixmap backpic(":/background/images/background.jpg");
    QPainter painter(this);
    painter.drawPixmap(this->rect(),backpic);
}

void PlayWindow::closeEvent(QCloseEvent *event) {
//     qDebug() << "ahzeeee";
    bgm->stop();
    shuffle_music->stop();
//    this->close();
    this->parentWidget()->show();
    emit close_window();
    event->accept();
//    exit(0);
}

void PlayWindow::keyPressEvent(QKeyEvent *e) {
   if(e->key() == Qt::Key_Enter) on_enter_button_clicked();
}
