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
Board::Board(){};

//Destructor, be careful of memory leaks
Board::~Board(){
    for(int i = 0 ; i < NUMBER_OF_PLAYER; i++){
        delete this->players[i];
    }
    players->clear();

    for(int i = 0 ; i < NUMBER_OF_CARDS; i++){
        delete this->cards.delete_card();
    }
};

//Initialize the game in offline mode
void Board::init_game(){

    players[CURRENT_PLAYER]->set_name("default1");
    players[1]->set_name("default2");
    players[2]->set_name("default3");
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
    CurrentPattern::get_current()->set_player_index(players[landlord_id]);
};

//Perform the main process of the game
void Board::start_game(){
    string action_name;
    CardGroup action_cards;

    for(int turn = landlord_id; !game_finish; turn++){

         if(turn >= NUMBER_OF_PLAYER){
             turn = 0;
         }

        if(turn == CURRENT_PLAYER){
            cout << endl;
            players[turn]->print();
            cout << "Please choose cards: \n Z = RED JOKER, Y = BLACK JOKER, 0 = 10 \n Example: 890JQ 9JQ8A \n" << endl;

            do{ cin >> action_name;
                action_cards.init(action_name,players[turn]->get_deck()->get_cards());
            } while( check(&action_cards) == false );
        }else{
            if(players[turn] == CurrentPattern::get_current()->get_player_index()){
                action_card.init(players[turn]->tip()); //check last turn, give tips if the same player
            }else{
                action_card.init(CurrentPattern::get_current()->find_valid_group((players[turn]->get_deck())->get_cards()));
            }
        }

        players[turn]->play(action_cards); //player play cards

        //check game state
        if((players[turn]->get_deck())->get_number_of_cards() == 0){
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
void Board::choose_landlord(){
    cout << "--------- CHOOSE LANDLORD ---------" << endl;
    int first = -1, last = 0, cnt = 0;
    int i, j = rand()% NUMBER_OF_PLAYER;
    bool decision = 0;

    for(int i = 0; i < NUMBER_OF_PLAYERS; i++, j == 2? j=0: j++){
        if( j == CURRENT_PLAYER){
            decision = make_choice(); //choose a tip to take
        }else{
            decision = rand()%2;
        }

        if(decision){
            cnt++;
            last = j;
            if(first == -1){
                first = j;
            }
            cout << this->players[j]->get_name() << " becomes the Landlord now." << endl;
        }else{
            cout << this->playes[j]->get_name() << " chooses not to be the Landlord." << endl;
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
            decision = make_choice();
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
        this->cards[i]->print(); //display cards
        ((players[landlord_id]->get_deck())->get_cards()).push_back(cards[i]); //add cards into landlord player's cards
    }

    cout << endl;
    players[landlord_id]->rearrange(); //sort cards

};

void Board::shuffle(){
    int i = 0, j = 0, k = 0;
    for(i = 0; i < NUMBER_OF_CARDS; i++){
        swap(this->cards[i], this->cards[rand()%NUMBER_OF_CARDS]);
    }

    //distribute cards to players
    for(k = 0, i = 0; i < NUMBER_OF_CARDS; i++){
        ((this->players[i]->get_deck())->get_cards()).clear();
        for(j = 0; j < 17; j++){
            ((this->players[i]->get_deck())->get_cards()).push_back((this->card.get_cards())[k++]);
        }
        this->players[i]->rearrange(); //sort cards
        this->players[i]->print();
    }
};//Shuffle the Deck

bool Board::check(CardsGroup* cardgroup){
    if(cardgroup->get_cards_type()->get_type() == -1){
        cout << "Action error. Please enter again: " << endl;
        return false;
    }else if(cardgroup->get_cards_type()->to_string() == "EMPTY"){
        return true;
    }else if(CurrentPattern::get_current()->get_player_index()!= players[CURRENT_PLAYER]
             && !cardgroup->is_valid(CurrentPattern::get_current()->get_cardgroup()){
        cout << "Invalid choice. Please enter again: " << endl;
        return false;
    }else{
        return true;
    }
};//Determine whether the cardgroup played is valid at the moment