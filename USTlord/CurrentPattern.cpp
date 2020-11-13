#include <iostream>
#include "CurrentPattern.h"
using namespace std;

int CurrentPattern::get_player_index() const{
    return this->player_index;
}
void CurrentPattern::set_player_index(int id){
    this->player_index=id;
}
bool CurrentPattern::can_be_beaten_by(int id,const CardsGroup& cg) const{
    if(this->player_index==id){
        if(cg.is_valid()){
            //cout << "Next round!\n";
            return true;
        }
        //cout << "Your play is invalid.\n";
        return false;
    }
    else{
        int compare_value=this->compare(cg);
        if(compare_value==-1){
            //cout << "Current Pattern can be beaten.\n";
            return true;
        }
        else if(compare_value==-2){
            //cout << "Not comparable.\n";
            return false;
        }
        else if(compare_value==0){
            //cout << "They are the same.\n";
            return false;
        }
        else if(compare_value==1){
            //cout << "Current Pattern is greater.\n";
            return false;
        }
        return false;
    }
}//check whether current_pattern can be beaten by cardsgroup played by the player with the id
//if both id's are the same, return true
void CurrentPattern::record(int id,const CardsGroup& cg){
    this->reset(cg.get_cards());
    this->player_index=id;
}//record the cardsgroup played by the player with the id
