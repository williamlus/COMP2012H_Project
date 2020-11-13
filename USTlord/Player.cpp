#include "Player.h"

Player::Player():id(-1),name(),deck(new Deck()){}
Player::Player(int id,string name) : id(id), name(name), deck(new Deck()) {}
Player::~Player(){
    delete deck;
}
void Player::set_id(int id){
    this->id=id;
}
void Player::set_name(string name){
    this->name=name;
}
string Player::get_name() const{
    return this->name;
}
Deck const* Player::get_deck() const{
    return this->deck;
}
int Player::get_num_cards() const{
    return this->deck->get_num_cards();
}
void Player::receive_card(Card const* c){
    this->deck->add_card(c);
}//receive a card given by board
void Player::display_cards() const{
    cout << "All your cards : " << endl;
    vector<Card const*> cards(0,nullptr);
    for(int i=0;i<this->get_num_cards();i++){
        cout << cards[i]->get_string()+' ';
    }
    cout << endl << "The selected cards : " << endl;
    cards=this->selected_cards.get_cards();
    for(int i=0;i<cards.size();i++){
        cout << cards[i]->get_string()+' ';
    }
    cout << endl;
}//display cards in hand (deck) and the selected cards to the player
bool Player::want_landlord(){
    char ans='\0';
    while (ans!='Y' || ans!='y' || ans!='N' || ans!='n'){
        cout << "Do you want to be the lanlord? (Y/N) : ";
        cin >> ans;
        cin.ignore(100);
        cout << endl;
    }
    if(ans=='Y' || ans=='y'){
        return true;
    }
    else{return false;}
}//choose to be landlord or not
void Player::select_card(Card const * c){
    bool card_exist=false, card_unselected=true;
    vector<Card const*> cards=this->deck->get_cards();
    for(int i=0;i<cards.size();i++){
        if(*cards[i]==*c){
            card_exist=true;
            break;
        }
    }
    if(card_exist){
        cards=this->selected_cards.get_cards();
        int index;
        for(index=0;index<cards.size();index++){
            if(*cards[index]==*c){
                card_unselected=false;
                break;
            }
        }
        if(card_unselected){
            cards.push_back(c);
            this->selected_cards.reset(cards);
        }
        else{
            cards.erase(cards.begin()+index);
            this->selected_cards.reset(cards);
        }
    }
}//select one card from deck and push its pointer to selected_cards, if card exists in selected_cards, then unselect it
void Player::calc_hints(const CurrentPattern& cp){

}//find all possible hints according to current pattern, and stored them in hints.
CardsGroup Player::get_hint(){
    int temp=this->current_hint;
    ++this->current_hint;
    return this->hints[temp];
}//get one hint according to hints and current_hint, update the selected_cards, and display_cards()
bool Player::selected_can_beat(const CurrentPattern& cp){
    if(cp.can_be_beaten_by(this->id,this->selected_cards)){
        return true;
    }
    return false;
}//check whether the player's selected cards can beat the last player's CardsGroup
CardsGroup Player::play(const Board& b){
    this->calc_hints(*b.get_current_pattern());
    char choice;
    while (true){
        cout << "Enter 'h' to get one hint, enter 's' to select cards, enter 'p' to play cards, enter 'q' to give up: ";
        cin >> choice;
        cout << endl;
        if (choice=='h'){
            this->selected_cards=this->get_hint();
        }
        else if (choice=='s'){
            cout << "Please enter a set of cards, e.g. s3 h3 : ";
            //TBC
        }
        else if (choice=='p'){
            break;
        }
        else if (choice=='q'){
            this->hints.clear();
            this->current_hint=0;
            vector<Card const*> empty_cards(0,nullptr);
            this->selected_cards.reset(empty_cards);
            break;
        }
        else{
            cout << "Invalid choice, please enter again." << endl;
        }
    }
    this->clear_cards(this->selected_cards);
    CardsGroup temp=this->selected_cards;
    vector<Card const*> empty_cards(0,nullptr);
    this->selected_cards.reset(empty_cards);
    return temp;
}//use cin or hint (with loops) to Play cards according to current pattern, clear_cards, and reset data members
void Player::clear_cards(const CardsGroup& cg){
    this->deck->clear_cards(cg.get_cards());
}//clear the played cards' pointers(don't delete)
