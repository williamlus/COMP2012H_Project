#include "Player.h"

Player::Player():id(-1),name("default name"),deck(new Deck()){}
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
    this->deck->rearrange();
}//receive a card given by board

void Player::display_cards() const{
    cout << "All your cards : " << endl;
    vector<Card const*> cards=deck->get_cards();
    for(int i=0;i<cards.size();i++){
        cout << cards[i]->get_string()+' ';
    }
    cout << endl << "The selected cards : " << endl;
    cards=this->selected_cards_group.get_cards();
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
    //check whether the player has the card c
    vector<Card const*> cards=this->deck->get_cards();
    for(int i=0;i<cards.size();i++){
        if(*cards[i]==*c){
            card_exist=true;
            break;
        }
    }
    if(card_exist){
        //check whether the player has already selected the card c
        cards=this->selected_cards_group.get_cards();
        int index;
        for(index=0;index<cards.size();index++){
            if(*cards[index]==*c){
                card_unselected=false;
                break;
            }
        }
        //if not selected, select the card c, and reset selected_cards_group
        if(card_unselected){
            cards.push_back(c);
            this->selected_cards_group.reset(cards);
        }
        //if selected, unselect the card c, and reset selected_cards_group
        else{
            cards.erase(cards.begin()+index);
            this->selected_cards_group.reset(cards);
        }
    }
}//select one card from deck and push its pointer to selected_cards_group, if card exists in selected_cards_group, then unselect it

void Player::calc_hints(const CurrentPattern& cp){
    //TBC
}//find all possible hints according to current pattern, and stored them in hints.

CardsGroup Player::get_hint(){
    int temp=this->current_hint;
    ++this->current_hint;
    return this->hints[temp];
}//get one hint according to hints and current_hint, update the selected_cards_group, and display_cards()

bool Player::selected_can_beat(const CurrentPattern& cp){
    if(cp.can_be_beaten_by(this->id,this->selected_cards_group)){
        return true;
    }
    return false;
}//check whether the player's selected cards can beat the last player's CardsGroup

vector<string*> Player::request_cards_string(){
    vector<string*> cards(0,nullptr);
    //check whether the input of cards is valid (only consider the format)
    int valid=false;
    while(!valid){
        cout << "Please enter the set of cards : " << endl;
        string card_name="";
        char temp='\0';

        while(true){
            temp=cin.get();
            if(temp=='\n'){
                if(card_name.size()==2){
                    cards.push_back(new string(card_name));
                    card_name="";
                }
                else if(card_name.size()!=0){
                    cout << "Invalid input." << endl;
                    for(int i=0;i<cards.size();++i){
                        delete cards[i];
                    }
                    cards.clear();
                    break;
                }
                break;
            }
            else if(temp==' '){
                if(card_name.size()==2){
                    cards.push_back(new string(card_name));
                    card_name="";
                }
                else if(card_name.size()!=0){
                    cout << "Invalid input." << endl;
                    for(int i=0;i<cards.size();++i){
                        delete cards[i];
                    }
                    cards.clear();
                    break;
                }
                continue;
            }
            else{
                card_name+=temp;
            }
        }
        if(cards.size()>0){
            valid=true;
            cout << "This is the cards you choose : " << endl;
            for(int i=0;i<cards.size();i++){
                cout << *cards[i] << endl;
            }
        }
        if(temp!='\n'){
            cin.ignore(999,'\n');
        }
    }
    return cards;
}

CardsGroup Player::play(const Board& b){
    this->calc_hints(*b.get_current_pattern());
    char choice;
    while (true){
        cout << "Enter 'h' to get one hint, enter 's' to select cards, enter 'p' to play cards, enter 'q' to give up: ";
        cin >> choice;
        cout << endl;
        if (choice=='h'){
            this->selected_cards_group=this->get_hint();
            this->display_cards();
        }
        else if (choice=='s'){
            vector<string*> cards_string(0,nullptr);
            cout << "Please enter a set of cards, e.g. s3 h3 : ";
            cards_string=this->request_cards_string();
            vector<Card const*> cards=this->deck->get_cards();
            for(int i=0;i<cards_string.size();i++){
                for(int j=0;j<cards.size();j++){
                    if(cards[j]->get_string()==*cards_string[i]){
                        this->select_card(cards[j]);
                    }
                }
            }
        }
        else if (choice=='p'){
            if(this->selected_cards_group.is_valid()){
                break;
            }
            else{
                cout << "Invalid CardsGroup, please enter again." << endl;
            }
        }
        else if (choice=='q'){
            //unselect all selected_cards_group
            vector<Card const*> empty_cards(0,nullptr);
            this->selected_cards_group.reset(empty_cards);
            break;
        }
        else{
            cout << "Invalid choice, please enter again." << endl;
        }
    }
    //clear all the hints
    this->hints.clear();
    this->current_hint=0;
    //clear the cards to be played in deck
    this->clear_cards(this->selected_cards_group);
    CardsGroup temp=this->selected_cards_group;
    //clear selected_cards_group
    vector<Card const*> empty_cards(0,nullptr);
    this->selected_cards_group.reset(empty_cards);
    return temp;
}//use cin or hint (with loops) to Play cards according to current pattern, clear_cards, and reset data members

void Player::clear_cards(const CardsGroup& cg){
    this->deck->clear_cards(cg.get_cards());
}//clear the played cards' pointers(don't delete)

vector<int> Player::get_deck_distribution() {
    vector<int> count(NUMBER_OF_FIGURES, 0);
    vector<Card const*> current_cards = deck->get_cards();
    for (int i = 0; i < current_cards.size(); ++i) {
        int temp_value = current_cards[i]->get_value();
        count[temp_value]++;
    }
    return count;
}

void Player::calc_hints(const CurrentPattern& cp) {
    //first know what's the card type of the current pattern
    CardsType::Type type = (cp.get_cards_type()).get_type();
    vector<Card const*> current_cards = deck->get_cards();
    //first we can find whether there are bombs in current_cards
    vector<int> count = get_deck_distribution();
    for (int i = 0; i < NUMBER_OF_FIGURES; ++i) {
        else if (i == NUMBER_OF_FIGURES - 1) {
            if(count[i]==2){
                Card* red_joker = deck->get_certain_card(Card::Color::)
                vector<Card const*> temp_cards_combination = 
                bombs.push_back()}
        }
    }
    //then we do searching in the players_to_player's deck, to check whether he has the same type or boom
    switch(type) {
    //SINGLE case
    case SINGLE:
        //first get all valid SINGLE
        for (int i = 0; i < deck->get_num_cards(); ++i) {
            CardsGroup  temp(current_cards[i]);
            if (temp.compare(cp) == 1) {
                hints.push_back(temp);
            }
        }
        //then get all possible BOMB
    }
}
