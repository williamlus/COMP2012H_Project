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
    qDebug() << "?";
    initialize_window();
    qDebug() << "?";
    initialize_music();
    qDebug() << "?";
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

void PlayWindow::initialize_music() {
    bgm = new QSound(":/sound/sound/bgm2.wav");
    win_music = new QSound(":/sound/sound/win.wav");
    lose_music = new QSound(":/sound/sound/loseMusic.wav");
    shuffle_music = new QSound(":/sound/sound/shuffleCardsMusic.wav");
    bgm->play();
    bgm->setLoops(-1);
}

void PlayWindow::initialize_cards() {
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
        qDebug() << "?";
    }
}

void PlayWindow::initialize_offline_game() {
    initialize_cards();
    ui->start_button->setVisible(false);
    ui->player_name_1->setText("PLAYER 1");
    ui->player_name_2->setText("PLAYER 2");
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
    qDebug() << "strange";
}

void PlayWindow::initialize_players() {
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
    qDebug() << "ok";
}
/*
 * Game Processing Functions
 */
void PlayWindow::card_selected() {
    if(game_status == DEFAULT || game_status == DEALING_CARDS) return;
    if(players[0]->turn_end()) return;
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

    update_player_cards(0);
}

void PlayWindow::update_player_cards(int player_id)
{
    if(player_id < 0 || player_id > 2) return;
    players[player_id]->get_deck()->rearrange();
    //render cards in hand
    if(player_id == 0)
    {
        for(int j = 0; j < players[player_id]->get_deck()->get_num_cards(); j++)
        {
            int top_border = 400;                           //upper border
            int left_border = (1200-CARD_WIDTH*players[0]->get_num_cards())/2;  //left border
            CardPicture* card_picture = players[player_id]->get_deck()->get_cards()[j]->get_card_picture();
            card_picture->turn(false);
            if(card_picture->is_selected()) top_border-=20;
            card_picture->move(left_border+j*CARDS_INTERVAL, top_border);
            card_picture->repaint();
        }
    }
    else if(player_id == 1) {
        int left_border = 40;
        int top_border = 40;
        for(int j = 0; j < players[player_id]->get_deck()->get_num_cards(); j++) {
            CardPicture* card_picture = players[player_id]->get_deck()->get_cards()[j]->get_card_picture();
            card_picture->setGeometry(left_border+j*12, top_border, OPPONENT_CARD_WIDTH, OPPONENT_CARD_HEIGHT);
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

void PlayWindow::AIplayer_action(int active_AIplayer){

    sleep(1000);
    CardsGroup tmp = players[active_AIplayer]->play(*cp);
    vector<const Card*> tmpcard = tmp.get_cards();
    sleep(1000);
    current_selection.clear();
    if(!tmp.get_cards().empty()){
        for(int i = 0; i < tmpcard.size(); i++){
            current_selection.push_back(tmpcard[i]);
        }
        hide_past_cards();
        sleep(300);
        reveal_current_selection();
        sleep(200);
        qDebug() << QString::fromStdString(" play " + cp->get_cards_type().to_string());
        cp = new CurrentPattern(tmp,active_AIplayer);
        qDebug() << QString::fromStdString(" play " + cp->get_cards_type().to_string());
        ui->info_bar->setText(QString::fromStdString("Player ") +QString::number(active_AIplayer)+ QString::fromStdString(" plays " + cp->get_cards_type().to_string()));
        update_player_cards(active_AIplayer);
        qDebug() << QString::fromStdString(" play " + cp->get_cards_type().to_string());
        sleep(1000);
        current_selection.clear();
    }else{
        ui->info_bar->setText(QString::fromStdString("Player ") +QString::number(active_AIplayer)+ QString::fromStdString(" gives up."));
        sleep(1000);
//            ui->player_2_message->setVisible(true);
//            sleep(1000);
//            ui->player_2_message->setVisible(false);
        }
    }

void PlayWindow::choose_landlord(){
    bool decision;

    decision = players[1]->want_landlord();
//    players[1]->display_cards();
    if(decision){
       ui->info_bar->setText("Player 1 calls landlord!");
       sleep(1000);
       call_landlord_list.push_front(1);
    }
    decision = players[2]->want_landlord();
//    players[2]->display_cards();
    if(decision) {
        ui->info_bar->setText("Player 2 calls landlord!");
        sleep(1000);
        call_landlord_list.push_front(2);
    }
    if(call_landlord_list.size() == 0)
    {
        ui->info_bar->setText("No one calls landlord! Redistribute the cards now.");
        sleep(1000);
        restart();
        return;
    }
    else {
        landlord_id = call_landlord_list[0];
        qDebug() << landlord_id << "----1";
        landlord_bonus(landlord_id);
        game_status = PLAYING;
        if(landlord_id!=0){
            ui->info_bar->setText(QString::fromStdString("Player ") + QString::number(landlord_id) + QString::fromStdString(" is the landlord now!"));}
        else{
            ui->info_bar->setText("You are landlord now.");
        }
        sleep(1000);
        set_chara_pic();
    }

    for(int i = landlord_id; i != 0;){
        ui->info_bar->setText(QString::fromStdString("Player ") + QString::number(i) + QString::fromStdString("'s turn now"));
        sleep(1000);
        AIplayer_action(i);
        i = (++i)%3;
    }
    if(cp->get_player_index()!=0){
        ui->info_bar->setText(QString::fromStdString("Player ") + QString::number(cp->get_player_index()) + QString::fromStdString(" played. \n Now it's your turn."));}
    else{
        ui->info_bar->setText("Now it's your turn.");
    }

    sleep(1000);
    qDebug() << "ok1";
    ui->hit_button->setVisible(true);
    qDebug() << "ok2";
    ui->hint_button->setVisible(true);
    qDebug() << "ok3";
    ui->give_up_button->setVisible(true);
}

void PlayWindow::on_hint_button_clicked()
{
    for(auto card : players[0]->get_deck()->get_cards()) {
        card->get_card_picture()->select(false);
    }
    players[0]->calc_hints(*cp);
    if(players[0]->get_hints().size() == 0){
        ui->info_bar->setText("No hints.");
        sleep(1000);
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
            hint_id = (hint_id+1)%players[0]->get_hints().size();
        }
        for(auto card : players[0]->get_hints()[hint_id].get_cards()) {
            card->get_card_picture()->select(true);
            current_selection.push_back(card);
        }
        players[0]->set_selected_cards(players[0]->get_hints()[hint_id].get_cards());
        update_player_cards(0);
    }
    //display calculated hints
}

void PlayWindow::on_give_up_button_clicked()
{

    ui->info_bar->setText("You give up, next player's turn.");
    sleep(1000);
    players[0]->set_choice('g');
    players[0]->set_selected_cards(players[0]->play(*cp).get_cards());
    current_selection.clear();
    update_player_cards(0);
    qDebug() << cp->get_player_index();
    for(int i = 0; i < players[0]->get_num_cards(); i++){
        players[0]->get_deck()->get_cards()[i]->get_card_picture()->select(false);
    }
    update_player_cards(0);
    if(players[0]->turn_end()){
        ui->hit_button->setVisible(false);
        ui->hint_button->setVisible(false);
        ui->give_up_button->setVisible(false);
        players[0]->clear_hint();

       if(cp->get_player_index() == 1){ hide_past_cards();}
       AIplayer_action(1);
       sleep(300);
       if(players[1]->is_winner()){
           ui->info_bar->setText("Game Finished!");
           sleep(1000);
           game_finished(1);
       }


       if(cp->get_player_index() == 2){ hide_past_cards();}
       AIplayer_action(2);
       sleep(300);
       if(players[2]->is_winner()){
           ui->info_bar->setText("Game Finished!");
           sleep(1000);
           game_finished(2);
       }


       players[1]->set_turn_end(false);
       players[2]->set_turn_end(false);
       players[0]->set_turn_end(false);
    }

    sleep(1000);
    ui->hit_button->setVisible(true);
    ui->hint_button->setVisible(true);
    ui->give_up_button->setVisible(true);
}

void PlayWindow::landlord_bonus(int landlord_id)
{
    qDebug() << "xixi";
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
    update_player_cards(landlord_id);
};

//shuffle cards, distribute cards, call Deck::rearrange() to sort cards
void PlayWindow::shuffle()
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

void PlayWindow::restart() {
    qDebug() << "No landlord! Restart!";
    for(int k = 0; k < NUMBER_OF_CARDS; k++) {
        CardPicture* card_pic = this->set_of_cards[k]->get_card_picture();
        card_pic->hide();
    }
    players.clear();
    initialize_offline_game();
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
void PlayWindow::on_start_button_clicked()
{
    ui->info_bar->setText("Game Start");
    initialize_offline_game();
    ui->info_bar->setText("Please choose landlord");
}

void PlayWindow::on_call_lord_button_clicked()
{
     ui->info_bar->setText("You call landlord!");
     sleep(1000);
    call_landlord_list.push_front(0);
    ui->refuse_button->setVisible(false);
    ui->call_lord_button->setVisible(false);
    choose_landlord();
}

void PlayWindow::on_refuse_button_clicked()
{
    ui->info_bar->setText("You refuse to be the landlord!");
    sleep(1000);
    ui->refuse_button->setVisible(false);
    ui->call_lord_button->setVisible(false);
    choose_landlord();
}

void PlayWindow::on_hit_button_clicked()
{
    sleep(10);
    //current_selected //current+_pattern
    players[0]->set_selected_cards(current_selection);
    players[0]->set_choice('p');
    players[0]->set_selected_cards(players[0]->play(*cp).get_cards());
    if(players[0]->get_selected_cards().get_cards().size() == 0){
        ui->info_bar->setText("Invalid cards group! \n Please choose again!");
        sleep(500);
        return;
    }
    else{
            hide_past_cards();
            sleep(200);
            if(!cp->get_cards().empty()){
                delete cp;
            }
            qDebug() << current_selection.size();
            cp = new CurrentPattern(current_selection,0);
            qDebug() << QString::fromStdString(cp->get_cards_type().to_string());

            players[0]->clear_hint(); //clear the cards to be played in deck
            players[0]->clear_cards(current_selection);
            CardsGroup temp = current_selection; //clear selected_cards_group
            vector<Card const*> empty_cards(0,nullptr);
            players[0]->set_selected_cards(empty_cards);

            qDebug() << "***";
            players[0]->display_cards();
            qDebug() << "***";

            ui->info_bar->setText(QString::fromStdString("You play " + cp->get_cards_type().to_string()));
            update_player_cards(0);

            ui->hit_button->setVisible(false);
            ui->hint_button->setVisible(false);
            ui->give_up_button->setVisible(false);
            sleep(1500);
            if(players[0]->is_winner()){
                ui->info_bar->setText("Game Finished!");
                sleep(1000);
                game_finished(0);
            }

        ui->info_bar->setText("Player 1's turn!");
        AIplayer_action(1);
        sleep(1000);
        if(players[1]->is_winner()){
            ui->info_bar->setText("Game Finished!");
            sleep(1000);
            game_finished(1);
        }

        ui->info_bar->setText("Player 2's turn!");
        AIplayer_action(2);
        sleep(1000);
        if(players[2]->is_winner()){
            ui->info_bar->setText("Game Finished!");
            sleep(1000);
            game_finished(2);
        }

        players[1]->set_turn_end(false);
        players[2]->set_turn_end(false);
        players[0]->set_turn_end(false);

        sleep(1000);
        if(cp->get_player_index() !=0 ){
        ui->info_bar->setText(QString::fromStdString("Player ") + QString::number(cp->get_player_index()) + QString::fromStdString(" played. \n Now it's your turn."));}
        else{
            ui->info_bar->setText("Now it's your turn.");}
        }
        ui->hit_button->setVisible(true);
        ui->hint_button->setVisible(true);
        ui->give_up_button->setVisible(true);
    }
/*
 * Helper Functions
 */
void PlayWindow::sleep(unsigned int msec)
{
    QTime reach_time = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < reach_time) QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void PlayWindow::set_chara_pic() {
    if(landlord_id == -1) return;
    QPixmap farmer_pixmap(":/icon/images/farmer.png");
    QPixmap lord_pixmap(":/icon/images/lord.png");
    if(landlord_id == 0) {
        player1_pic->setPixmap(lord_pixmap);
        player1_pic->show();
    }
    else {
        player1_pic->setPixmap(farmer_pixmap);
        player1_pic->show();
    }
    if(landlord_id == 1) {
        player2_pic->setPixmap(lord_pixmap);
        player2_pic->show();
    }
    else {
        player2_pic->setPixmap(farmer_pixmap);
        player2_pic->show();
    }
    if(landlord_id == 2) {
        player3_pic->setPixmap(lord_pixmap);
        player3_pic->show();
    }
    else {
        player3_pic->setPixmap(farmer_pixmap);
        player3_pic->show();
    }
}

void PlayWindow::hide_past_cards() {
    for(auto card : cp->get_cards()) {
        card->get_card_picture()->hide();
    }
}

void PlayWindow::game_finished(int current_player){
    game_finish = true;
    if(current_player == 0){
        you_win = true;
    }else{
        you_win = false;
    }
    if(current_player == landlord_id){
        landlord_win = true;
    }else{
        landlord_win = false;
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
         QMessageBox::information(NULL, "", "Game End!! \n You Lose na :(! \n Farmer wins!", QMessageBox::Yes, QMessageBox::Yes);
    }
    else if(you_win && !landlord_win){
        bgm->stop();
        win_music->play();
         QMessageBox::information(NULL, "", "Game End!! \n You Win na :)! \n Farmer win! ", QMessageBox::Yes, QMessageBox::Yes);
    }
    close();
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
    qDebug() << "ahzeeee";
    bgm->stop();
    shuffle_music->stop();
    exit(0);
}



