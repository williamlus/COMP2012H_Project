#include "AIPlayer.h"
#include <random>
#include "time.h"

AIPlayer::AIPlayer():Player(-1,"default name"){}
AIPlayer::AIPlayer(int id,string name):Player(id,name){}
AIPlayer::~AIPlayer(){}

const CardsGroup& AIPlayer::choose_hint(const CurrentPattern& cp, vector<int> players_num_cards){
    return this->hints[0];
    //TBC
}//choose cardsgroup according to the situation of board (e.g. num of cards of other players, current_pattern)

CardsGroup AIPlayer::play(const CurrentPattern& cp, vector<int> players_num_cards){
    if(!deck->split_important_combination){
    this->calc_hints(cp);
    return this->choose_hint(cp, players_num_cards);}
    else{
        return CardsGroup();
    }
}//use cin or hint (with loops) to Play cards according to current pattern, clear_cards, and reset data members

bool AIPlayer::want_landlord(){
    
    srand(time(0));
    return rand()%2;//temporary settings (please increase AI's intelligence)
}//choose to be landlord or not
