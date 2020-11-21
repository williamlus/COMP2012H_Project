#include "AIPlayer.h"
#include <random>
#include <ctime>
#include <cstdlib>
#include <vector>

AIPlayer::AIPlayer():Player(-1,"default name"){}
AIPlayer::AIPlayer(int id,string name):Player(id,name){}
AIPlayer::~AIPlayer(){}

const CardsGroup AIPlayer::choose_hint(const CurrentPattern& cp){
    //if this AIPlayer is the first one to play card or the last currentPattern is played by himself
    //he can play any CardsGroup
    if(cp.get_player_index()==this->get_id()||cp.get_cards_type().get_type()==CardsType::Type::EMPTY){
        //for simplification, just play the smallest CardsGroup
        //but here we should cover all combination to ensure the AIPlayer can play out all the cards
        vector<int> count = deck->get_deck_distribution(deck);
        int smallest_index=0;
        for(;smallest_index<NUMBER_OF_FIGURES;++smallest_index){
            if(count[smallest_index]!=0){break;}
        }
        //then we get the smallest_index of the current deck
        //check whether this card regarding to the smallest_index is in some combination
        //if it's bomb, don't play it
        if(count[smallest_index]==4){
            //find the second smallest_index
            int temp = smallest_index;
            for(smallest_index+=1;smallest_index<NUMBER_OF_FIGURES;++smallest_index){
                if(count[smallest_index]!=0){break;}
            }
            if(smallest_index==NUMBER_OF_FIGURES){
                //this implies that the player has only a BOMB left, then just play it
                smallest_index=temp;
                vector<Card const*> card_to_add;
                for(int i=0;i<4;++i){
                    Card const* to_add= deck->get_certain_card(smallest_index,card_to_add);
                    card_to_add.push_back(to_add);
                }
                return CardsGroup(card_to_add);
                }
        }
        //check whether it's TRIO
        //for simplification, just play TRIO_WITH_SINGLE
        if(count[smallest_index]==3){
            vector<Card const*> card_to_add;
            for(int i = 0;i<3;++i){
                Card const* to_add = deck->get_certain_card(smallest_index,card_to_add);
                card_to_add.push_back(to_add);
            }
            Card const* single_to_add = deck->get_certain_card(card_to_add);
            card_to_add.push_back(single_to_add);
            return CardsGroup(card_to_add);
        }
        //check whether it's PAIR
        //if it's PAIR, check whether it's PAIR_CONTINUOUS
        if(count[smallest_index]==2){
            //first check whether it's PAIR_CONTINUOUS
            int count_continuous =1;
            while(count[smallest_index+count_continuous]==2){++count_continuous;}
            if(count_continuous>=3){
                vector<Card const*> card_to_add;
                for(int i=0;i<count_continuous;++i){
                    Card const* first_to_add = deck->get_certain_card(smallest_index+count_continuous,card_to_add);
                    card_to_add.push_back(first_to_add);
                    Card const* second_to_add = deck->get_certain_card(smallest_index+count_continuous,card_to_add);
                    card_to_add.push_back(second_to_add);
                }
                return CardsGroup(card_to_add);
            }
            //if it is not PAIR_CONTINUOUS, then just play the PAIR
            vector<Card const*> card_to_add;
            Card const* first_to_add = deck->get_certain_card(smallest_index,card_to_add);
            card_to_add.push_back(first_to_add);
            Card const* second_to_add = deck->get_certain_card(smallest_index,card_to_add);
            card_to_add.push_back(second_to_add);
            return CardsGroup(card_to_add);
        }
        //check whether it's SINGLE_CONTINUOUS
        int count_continuous =1;
        while(count[smallest_index+count_continuous]!=0){++count_continuous;}
        if(count_continuous>=5){//this implies that the smallest_index is in a SINGLE_CONTINUOUS CardsGroup
            vector<Card const*> card_to_add;
            for(int i = 0;i<count_continuous;++i){
                Card const* to_add = deck->get_certain_card(smallest_index+i,card_to_add);
                card_to_add.push_back(to_add);
            }
            return CardsGroup(card_to_add);
        }
        
    }
    //if current AIPlayer is landlord, then try to beat everyone
    if(is_landlord){
        if(hints.empty()){return CardsGroup();}
        return this->hints[0];
    }
    else{
        if(cp.current_is_landlord()){
            if(hints.empty()){return CardsGroup();}
            return this->hints[0];
          
        }
        else{
            //if current AIPlayer is farmer, then avoid to beat its partner 
            //when its partner play same combination except small PAIR and small SINGLE unless can win
            if(cp.get_cards_type().get_type()==CardsType::Type::SINGLE){
                if(deck->get_num_cards()==1){return this->hints[0];}
                //never beat partner if he plays a SINGLE greater or equal 2
                if(cp.get_reference_card()->get_value()>=12){return CardsGroup();}
                else{
                    if(hints.empty()){return CardsGroup();}
                    return this->hints[0];
                    }
            }
            else if(cp.get_cards_type().get_type()==CardsType::Type::PAIR){
                if(deck->get_num_cards()==2){
                    if(hints.empty()){return CardsGroup();}
                    return this->hints[0];}
                //if cannot win, never beat partner if he plays a SINGLE greater or equal than AA
                if(cp.get_reference_card()->get_value()>=11){return CardsGroup();}
                else{
                    if(hints.empty()){return CardsGroup();}
                    return this->hints[0];
                    }
            }
            else{
                //partner plays some combination, never beat him unless itself can win 
                if(deck->get_num_cards()==cp.get_cards_type().get_num_cards()){
                    if(hints.empty()){return CardsGroup();}
                    return this->hints[0];
                }
                else{
                    return CardsGroup();
                }
            }
            
        }
    }
    

    
    
}//choose cardsgroup according to the situation of board (e.g. num of cards of other players, current_pattern)

CardsGroup AIPlayer::play(const CurrentPattern& cp, vector<int> players_num_cards){
    CardsGroup cg=CardsGroup();
    if(!deck->split_important_combination){
    this->calc_hints(cp);
    cg=this->choose_hint(cp);
    }
    this->hints.clear();
    this->current_hint=0;
    this->clear_cards(cg);
    return cg;
    }
//use cin or hint (with loops) to Play cards according to current pattern, clear_cards, and reset data members

bool AIPlayer::want_landlord(){
    //if num_of_important_combination >=3, the probability of want_lanlord is 0.75
    //if num_of_important_combination==2, the probability of want_landload is 0.6
    //if num_of_important_combination<2, the probability of want_landlord is 0.3
    srand((unsigned)time(NULL));
    float prob = static_cast<float>(rand()/RAND_MAX);
    int num_of_important_combination = deck->get_num_important_combination();
    if(num_of_important_combination>=3){
        return (prob<0.75)? true : false;
    }
    else if(num_of_important_combination==2){
        return (prob<0.6)? true : false;
    }
    else{
        return (prob<0.3)? true : false;
    }
    
}//choose to be landlord or not
