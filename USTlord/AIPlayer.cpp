#include "AIPlayer.h"
#include <random>
#include "time.h"

AIPlayer::AIPlayer():Player(-1,"default name"){}
AIPlayer::AIPlayer(int id,string name):Player(id,name){}
AIPlayer::~AIPlayer(){}

const CardsGroup& AIPlayer::choose_hint(const Board& b){
    //TBC
}//choose cardsgroup according to the situation of board (e.g. num of cards of other players, current_pattern)

CardsGroup AIPlayer::play(const Board& b){
    this->calc_hints(*b.get_current_pattern());
    return this->choose_hint(b);
}//use cin or hint (with loops) to Play cards according to current pattern, clear_cards, and reset data members

bool AIPlayer::want_landlord(){
    srand(time(0));
    return rand()%2;//temporary settings (please increase AI's intelligence)
}//choose to be landlord or not