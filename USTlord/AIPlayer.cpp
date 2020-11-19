#include "AIPlayer.h"
#include <random>
#include <ctime>
#include <cstdlib>

AIPlayer::AIPlayer():Player(-1,"default name"){}
AIPlayer::AIPlayer(int id,string name):Player(id,name){}
AIPlayer::~AIPlayer(){}

const CardsGroup AIPlayer::choose_hint(const CurrentPattern& cp, vector<int> players_num_cards){
    //if current AIPlayer is landlord, then try to beat everyone
    if(is_landlord){
        return this->hints[0];
    }
    else{
        if(cp.current_is_landlord()){
            return this->hints[0];
          
        }
        else{
            //if current AIPlayer is farmer, then avoid to beat its partner 
            //when its partner play same combination except small PAIR and small SINGLE unless can win
            if(cp.get_cards_type().get_type()==CardsType::Type::SINGLE){
                if(deck->get_num_cards()==1){return this->hints[0];}
                //never beat partner if he plays a SINGLE greater or equal 2
                if(cp.get_reference_card()->get_value()>=12){return CardsGroup();}
                else{return this->hints[0];}
            }
            else if(cp.get_cards_type().get_type()==CardsType::Type::PAIR){
                if(deck->get_num_cards()==2){return this->hints[0];}
                //if cannot win, never beat partner if he plays a SINGLE greater or equal than AA
                if(cp.get_reference_card()->get_value()>=11){return CardsGroup();}
                else{return this->hints[0];}
            }
            else{
                //partner plays some combination, never beat him unless itself can win 
                if(deck->get_num_cards()==cp.get_cards_type().get_num_cards()){
                    return this->hints[0];
                }
                else{
                    return CardsGroup();
                }
            }
            
        }
    }
    

    
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
    //if num_of_important_combination >=3, the probability of want_lanlord is 0.8
    //if num_of_important_combination==2, the probability of want_landload is 0.6
    //if num_of_important_combination<2, the probability of want_landlord is 0.3
    srand((unsigned)time(NULL));
    float prob = static_cast<float>(rand()/RAND_MAX);
    int num_of_important_combination = deck->get_num_important_combination();
    if(num_of_important_combination>=3){
        return (prob<0.8)? true : false;
    }
    else if(num_of_important_combination==2){
        return (prob<0.6)? true : false;
    }
    else{
        return (prob<0.3)? true : false;
    }
    
}//choose to be landlord or not
