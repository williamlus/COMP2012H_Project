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
    Card::Color color;
    for(int i=0;i<54;++i){
        char figure;
        if(i<52){
            figure = figures_to_int[i%13];
            int j = i/13;
            switch(j){
                case 0:
                color = Card::Color::SPADE;
                break;
                case 1:
                color = Card::Color::HEART;
                break;
                case 2:
                color = Card::Color::DIAMOND;
                break;
                case 3:
                color = Card::Color::CLUB;
                break;
            }
        }
        else if(i==52){
            figure = figures_to_int[NUMBER_OF_FIGURES-1];
            color = Card::Color::BLACK_JOKER;
        }
        else{
            figure = figures_to_int[NUMBER_OF_FIGURES-1];
            color = Card::Color::RED_JOKER;
        }
        set_of_cards.push_back(new Card(figure,color));
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

//Initialize the game in offline mode
void Board::init_game(){
    //generate 3 players (1 player, 2 AI)
    this->players[CURRENT_PLAYER]=new Player("You");
    this->players[1]=new AIPlayer("AI "+to_string(1));
    this->players[2]=new AIPlayer("AI "+to_string(2));
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

    cout << players[landlord_id]->get_name() << " is landlord now" << endl;
    this->landlord_bonus();
    this->cp->set_player_index(landlord_id);
};

//Perform the main process of the game
void Board::start_game(){
    string action_name;
    CardsGroup action_cards;

    for(int turn = landlord_id; !game_finish; turn++){

        if(turn >= NUMBER_OF_PLAYER){
            turn = 0;
        }
        cout << endl;
        players[turn]->display_cards();
        players[turn]->play(*this);

        //check game state
        if(players[turn]->get_deck()->get_num_cards() == 0){
            game_finish = true;
            landlord_win = (turn == landlord_id);
            you_win = (turn == CURRENT_PLAYER);
        }

        //output message
        if(landlord_win){ cout << "LANDLORD WIN!" << endl; }
        if(you_win){ cout << "YOU WIN!" << endl;}
            else{ cout << "YOU LOSE!" << endl; }
    }
};

//Choose landlord according to the rule
int Board::choose_landlord(){
    cout << "--------- CHOOSE LANDLORD ---------" << endl;
    int first = -1, last = 0, cnt = 0;
    int i, j = rand()% NUMBER_OF_PLAYER;
    bool decision = 0;

    for(int i = 0; i < NUMBER_OF_PLAYER; i++, j == 2? j=0: j++){
        decision = players[j]->want_landlord();

        if(decision){
            cnt++;
            last = j;
            if(first == -1){
                first = j;
            }
            cout << this->players[j]->get_name() << " becomes the Landlord now." << endl;
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
    for(k = 0, i = 0; i < NUMBER_OF_CARDS; i++){
        for(j = 0; j < 17; j++){
            this->players[i]->receive_card(this->set_of_cards[k++]);
        }
        this->players[i]->display_cards();
    }
};
