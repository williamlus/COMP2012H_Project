#include<iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <string>
#include "Board.h"
#include "Card.h"
#define NUMBER_OF_PLAYER 3
#define NUMBER_OF_CARDS 54
#define CURRENT_PLAYER 0

//Default constructor
Board::Board(): players(NUMBER_OF_PLAYER,nullptr){
    //generate 54 cards
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
        set_of_cards.push_back(new Card(color,figure));
    }
};

//Destructor, be careful of memory leaks
Board::~Board(){
    for(int i=0;i<this->set_of_cards.size();i++){
        delete this->set_of_cards[i];
    }
    this->set_of_cards.clear();
    for(int i = 0 ; i < NUMBER_OF_PLAYER; i++){
        delete this->players[i];
    }
    this->players.clear();
    delete this->cp;
};

vector<int> Board::get_players_num_cards() const{
    vector<int> num_cards(0,0);
    for(int id=0;id<NUMBER_OF_PLAYER;id++){
        num_cards.push_back(this->players[id]->get_num_cards());
    }
    return num_cards;
}
CurrentPattern const* Board::get_current_pattern() const{
    return this->cp;
}

//Initialize the game in offline mode
void Board::init_online_game(){
    for(int i=0;i<NUMBER_OF_PLAYER;i++){delete this->players[i];}
    //generate 3 players (1 player, 2 AI)
    this->players[CURRENT_PLAYER]=new Player(0,"You");
    // this->players[1]=new AIPlayer(1,"AI "+to_string(1));//////////////
    // this->players[2]=new AIPlayer(2,"AI "+to_string(2));//////////////
    this->players[1]=new Player(1,"He");////////////////////
    this->players[2]=new Player(2,"She");///////////////////
    this->cp=new CurrentPattern();
    game_finish = false;
    you_win = false;
    landlord_win = false;
    landlord_id = -1;

    while(landlord_id == -1){
        srand((int)time(0));
        shuffle();
        landlord_id = choose_landlord();
    }

    players[landlord_id]->set_is_landlord(true);
    cout << players[landlord_id]->get_name() << " is landlord now" << endl;
    this->landlord_bonus();
    this->cp->set_player_index(landlord_id);
    cout << "Init game success!" << endl;
};

void Board::init_offline_game(){
    for(int i=0;i<NUMBER_OF_PLAYER;i++){delete this->players[i];}
    //generate 3 players (1 player, 2 AI)
    this->players[CURRENT_PLAYER]=new Player(0,"You");
    this->players[1]=new AIPlayer(1,"AI "+to_string(1));//////////////
    this->players[2]=new AIPlayer(2,"AI "+to_string(2));//////////////
    this->cp=new CurrentPattern();
    game_finish = false;
    you_win = false;
    landlord_win = false;
    landlord_id = -1;

    while(landlord_id == -1){
        srand((int)time(0));
        shuffle();
        landlord_id = choose_landlord();
    }

    players[landlord_id]->set_is_landlord(true);
    cout << players[landlord_id]->get_name() << " is landlord now" << endl;
    this->landlord_bonus();
    this->cp->set_player_index(landlord_id);
    cout << "Init game success!" << endl;
}

//Perform the main process of the game
void Board::start_game(){
    string action_name;
    CardsGroup action_cards;

    for(int turn = landlord_id; !game_finish; turn++){

        if(turn >= NUMBER_OF_PLAYER){
            turn = 0;
        }
        cout << endl;
        vector<int> players_num_cards{players[0]->get_num_cards(),players[1]->get_num_cards(),players[2]->get_num_cards()};
        this->cp->record(players[turn]->get_id(),players[turn]->play(*this->get_current_pattern(),players_num_cards),players[turn]->get_id()==landlord_id);
        this->cp->display();

        //check game state
        if(players[turn]->get_deck()->get_num_cards() == 0){
            game_finish = true;
            landlord_win = (turn == landlord_id);
            you_win = (turn == CURRENT_PLAYER);
            //output message
            if(landlord_win){ cout << "LANDLORD WIN!" << endl; }
            if(you_win){ cout << "YOU WIN!" << endl;}
            else{ cout << "YOU LOSE!" << endl; }
        }
    }
    cout << "Start game success!" << endl;
};

//Choose landlord according to the rule
int Board::choose_landlord(){
    cout << "--------- CHOOSE LANDLORD ---------" << endl;
    int first = -1, last = 0, cnt = 0;
    int i, j = rand()% NUMBER_OF_PLAYER;
    bool decision = 0;

    for(int i = 0; i < NUMBER_OF_PLAYER; i++, j == 2? j=0: j++){
        decision = players[j]->want_landlord();
        cout << "decision success" << endl;

        if(decision){
            cnt++;
            last = j;
            if(first == -1){
                first = j;
            }
            cout << this->players[j]->get_name() << " wants to be the Landlord." << endl;
        }else{
            cout << this->players[j]->get_name() << " chooses not to be the Landlord." << endl;
        }
    }

    if(cnt == 0){
        cout << "Nobody choose to be the Landlord. Redistribute cards now." << endl;
        return -1;
    }
    if(cnt == 1){
        return first;
    }
    else{
        if(first == CURRENT_PLAYER){
            decision = players[CURRENT_PLAYER]->want_landlord();
        }else{
            decision = rand()%2;
        }

        if(decision){
            cout << this->players[first]->get_name() <<  " is the Landlord now." << endl;
            return first;
        }else{
            cout << this->players[first]->get_name() << " chooses not to be the Landlord." << endl;
            cout << this->players[last]->get_name() << " is to be the Landlord." << endl;
            return last;
        }
    }
};

//Add 3 bonus cards to landlord
void Board::landlord_bonus(){

    cout << "3 cards for the Landlord." << endl;

    for(int i = NUMBER_OF_PLAYER*17 ; i < NUMBER_OF_CARDS; i++){
        cout << this->set_of_cards[i]->get_string() << ' '; //display cards
        players[landlord_id]->receive_card(set_of_cards[i]); //add cards into landlord player's cards
    }

    cout << endl;
    players[landlord_id]->display_cards(); //sort cards

};

//Shuffle the Deck
void Board::shuffle(){
    int i = 0, j = 0, k = 0;
    for(i = 0; i < NUMBER_OF_CARDS; i++){
        swap(this->set_of_cards[i], this->set_of_cards[rand()%NUMBER_OF_CARDS]);
    }

    //distribute cards to players
    for(k = 0, i = 0; i < NUMBER_OF_PLAYER; i++){
        this->players[i]->abandom_cards();
        for(j = 0; j < 17; j++){
            this->players[i]->receive_card(this->set_of_cards[k++]);
        }
        this->players[i]->display_cards();
    }
    cout << "successfully shuffle" << endl;
};
