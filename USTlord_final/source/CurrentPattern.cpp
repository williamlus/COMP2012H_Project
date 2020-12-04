#include <iostream>
#include "CurrentPattern.h"
using namespace std;

void CurrentPattern::display(){
    cout << "Current Pattern ( by Player " << this->player_index << " ) : " << endl;
    vector <Card const*> cards=this->cards;
    cout << "Cards : ";
    for(int i=0;i<cards.size();i++){
        cout << cards[i]->get_string() << ' ';
    }
    cout << endl;
    cout << "CardsType : " << this->cards_type.to_string() << endl;
    cout << "Reference Card : " << (this->reference_card? this->reference_card->get_string():"nullptr") << endl;
}
int CurrentPattern::get_player_index() const{
    return this->player_index;
}
void CurrentPattern::set_player_index(int id){
    this->player_index=id;
}
bool CurrentPattern::can_be_beaten_by(int id,const CardsGroup& cg) const{
    if(cg.is_valid() && this->get_cards().size()==0){return true;}
    if(this->player_index==id){
        if(cg.is_valid()){
            cout << "Next round!\n";
            return true;
        }
        cout << "Your play is invalid.\n";
        return false;
    }
    else{
        int compare_value=this->compare(cg);
        if(compare_value==-1){
            cout << "Current Pattern can be beaten.\n";
            return true;
        }
        else if(compare_value==-2){
            cout << "Not comparable.\n";
            return false;
        }
        else if(compare_value==0){
            cout << "They are the same.\n";
            return false;
        }
        else if(compare_value==1){
            cout << "Current Pattern is greater.\n";
            return false;
        }
        return false;
    }
}//check whether current_pattern can be beaten by cardsgroup played by the player with the id
//if both id's are the same, return true

void CurrentPattern::record(int id,const CardsGroup& cg,bool is_landlord){
    if(cg.get_cards_type().get_num_cards()==0){
        return;
    }
    this->reset(cg.get_cards());
    this->player_index=id;
    this->is_landlord = is_landlord;
}//record the cardsgroup played by the player with the id

bool CurrentPattern::current_is_landlord() const{
    return is_landlord;
}

