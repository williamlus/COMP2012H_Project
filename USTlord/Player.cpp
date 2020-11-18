#include "Player.h"

Player::Player():id(-1),name("default name"),deck(new Deck()){}
Player::Player(int id,string name) : id(id), name(name), deck(new Deck()) {}
Player::~Player(){
    delete this->deck;
}
void Player::set_id(int id){
    this->id=id;
}
void Player::set_name(string name){
    this->name=name;
}
void Player::set_is_landlord(bool is_landlord){
    this->is_landlord=is_landlord;
}
int Player::get_id() const{
    return this->id;
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

void Player::abandom_cards(){
    vector <Card const*> c(0,nullptr);
    this->deck->set_cards(c);
}//clear all cards in hand

void Player::display_cards() const{
    cout << "Player " << this->id << " " << this->name << " ( " << (this->is_landlord?"landlord":"farmer") << " ) " << endl;
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
    while (ans!='Y' && ans!='y' && ans!='N' && ans!='n'){
        cout << "Player"<<this->id<<" : "<<this->name<< endl; 
        cout << "Do you want to be the lanlord? (Y/N) : ";
        cin >> ans;
        cin.ignore(999,'\n');
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
            if(cards[index]->get_string()==c->get_string()){
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
                cout << *cards[i] << ' ';
            }
            cout << endl;
        }
        if(temp!='\n'){
            cin.ignore(999,'\n');
        }
    }
    return cards;
}

CardsGroup Player::play(const CurrentPattern& cp){
    this->calc_hints(cp);
    char choice;
    while (true){
        this->display_cards();
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
                    if(cards[j]->get_string()==*(cards_string[i])){
                        this->select_card(cards[j]);
                        break;
                    }
                }
            }
        }
        else if (choice=='p'){
            if(this->selected_cards_group.is_valid()){
                if(cp.can_be_beaten_by(this->id,this->selected_cards_group)){
                    break;
                }
                else{
                    cout << "Cannot beat, please enter again." << endl;
                }
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

vector<int> Player::get_deck_distribution(Deck* deck) {
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
    vector<CardsGroup> bombs;
    vector<Card const*> bomb_cards;
    int num_input = cp.get_cards_type().get_num_cards();
    //first we can find whether there are bombs in current_cards
    vector<int> count = get_deck_distribution(this->deck);
    for (int i = 0; i < NUMBER_OF_FIGURES; ++i) {
        //this is for normal bomb case
        if(i< NUMBER_OF_FIGURES-1){
            if(count[i]==4){
                Card const* first = deck->get_certain_card(Card::Color::SPADE,i);
                Card const* second = deck->get_certain_card(Card::Color::HEART,i);
                Card const* third = deck->get_certain_card(Card::Color::CLUB,i);
                Card const* fourth = deck->get_certain_card(Card::Color::DIAMOND,i);
                
                vector<Card const*> temp_cards_combination {first,second,third,fourth};
                CardsGroup group_to_add(temp_cards_combination);
                bombs.push_back(group_to_add);
            }
        }
        //for the rocket case
        else if (i == NUMBER_OF_FIGURES - 1) {
            if(count[i]==2){
                Card const * red_joker = deck->get_certain_card(Card::Color::RED_JOKER,NUMBER_OF_FIGURES-1);
                Card const * black_joker = deck->get_certain_card(Card::Color::BLACK_JOKER,NUMBER_OF_FIGURES-1);
                vector<Card const*> temp_cards_combination {red_joker,black_joker};
                CardsGroup group_to_add(temp_cards_combination);
                bombs.push_back(group_to_add);
                }
        }
    }
    //then we do searching in the players_to_player's deck, to check whether he has the same type or bomb

    //SINGLE case
    if(type==CardsType::Type::SINGLE){
        //first get all valid SINGLE
        for (int i = 0; i < deck->get_num_cards(); ++i) {
            vector<Card const* > card_to_add {current_cards[i]};
            CardsGroup  temp(card_to_add);
            if (temp.compare(cp) == 1) {
                hints.push_back(temp);
            }
        }
        //then get all possible BOMB
        if(bombs.size()>0){
            
            for(int j=0;j<bombs.size();++j){
                hints.push_back(bombs[j]);
            }
        }
        }
    //PAIR case
    else if(type==CardsType::PAIR){
        //first get all valid PAIR
        for(int i=0;i<NUMBER_OF_FIGURES;++i){
            if(count[i]>=2){
                Card const* first = deck->get_certain_card(i,{});
                vector<Card const* > card_to_add {first};
                Card const* second = deck->get_certain_card(i,card_to_add);
                card_to_add.push_back(second);
                CardsGroup  temp(card_to_add);
                if (temp.compare(cp) == 1) {
                    hints.push_back(temp);
                }

            }
        }
        //then get all possible BOMB
        if(bombs.size()>0){
            
            for(int j=0;j<bombs.size();++j){
                hints.push_back(bombs[j]);
            }
        }
        
        }
    //TRIO
    else if(type==CardsType::Type::TRIO){
    //first get all valid TRIO
    for(int i=0;i<NUMBER_OF_FIGURES;++i){
        if(count[i]>=3){
                Card const* first = deck->get_certain_card(i,{});
                vector<Card const* > card_to_add {first};
                Card const* second = deck->get_certain_card(i,card_to_add);
                card_to_add.push_back(second);
                Card const* third = deck->get_certain_card(i,card_to_add);
                card_to_add.push_back(third);
                CardsGroup  temp(card_to_add);
                if (temp.compare(cp) == 1) {
                    hints.push_back(temp);
                }
        }
    }
    //then get all possible BOMB
        if(bombs.size()>0){
            
            for(int j=0;j<bombs.size();++j){
                hints.push_back(bombs[j]);
            }
        }
    }
    //TRIO_WITH_ONE case
    else if(type==CardsType::Type::TRIO_WITH_ONE){
        //check after chosing 3, is there any 1 single card left
        //first get all valid TRIO
        for(int i=0;i<NUMBER_OF_FIGURES;++i){
            if(count[i]>=3){
                Card const* first = deck->get_certain_card(i,{});
                vector<Card const* > card_to_add {first};
                Card const* second = deck->get_certain_card(i,card_to_add);
                card_to_add.push_back(second);
                Card const* third = deck->get_certain_card(i,card_to_add);
                card_to_add.push_back(third);
                Card const* single = deck->get_certain_card(card_to_add);
                if(single!=nullptr){
                    card_to_add.push_back(single);
                    CardsGroup temp(card_to_add);
                    if (temp.compare(cp)==1){
                        hints.push_back(temp);
                    }
                }
            }
        }
        //then get all possible BOMB
        if(bombs.size()>0){
            
            for(int j=0;j<bombs.size();++j){
                hints.push_back(bombs[j]);
            }
        }
    }
    //TRIO_WITH_PAIR case
    else if(type==CardsType::Type::TRIO_WITH_PAIR&&deck->get_num_cards()>=5){
        //check after chosing 3, is there any 1 single card left
        //first get all valid TRIO
        for(int i=0;i<NUMBER_OF_FIGURES;++i){
            if(count[i]>=3){
                Card const* first = deck->get_certain_card(i,{});
                vector<Card const* > card_to_add {first};
                Card const* second = deck->get_certain_card(i,card_to_add);
                card_to_add.push_back(second);
                Card const* third = deck->get_certain_card(i,card_to_add);
                card_to_add.push_back(third);
                //now find the remaining pair
                for(int j = 0;j<NUMBER_OF_FIGURES;++j){
                    if(count[j]>=2&&j!=i){
                        Card const* first_single = deck->get_certain_card(j,card_to_add);
                        card_to_add.push_back(first_single);
                        Card const* second_single = deck->get_certain_card(j,card_to_add);
                        card_to_add.push_back(second_single);
                        break;
                    }
                }
                CardsGroup temp=(card_to_add);
                if(temp.compare(cp)==1){
                    hints.push_back(temp);
                }
            }
        }
        //then get all possible BOMB
        if(bombs.size()>0){
            
            for(int j=0;j<bombs.size();++j){
                hints.push_back(bombs[j]);
            }
        }
    }
    //FOUR_WITH_TWO
    else if(type==CardsType::Type::FOUR_WITH_TWO&&deck->get_num_cards()>=6){
        if(bombs.size()>0){
            for(int i=0;i<NUMBER_OF_FIGURES;++i){
                if(count[i]==4){
                    Card const* first = deck->get_certain_card(Card::Color::SPADE,i);
                    Card const* second = deck->get_certain_card(Card::Color::HEART,i);
                    Card const* third = deck->get_certain_card(Card::Color::CLUB,i);
                    Card const* fourth = deck->get_certain_card(Card::Color::DIAMOND,i);
                    vector<Card const*> card_to_add {first,second,third,fourth};
                    for(int j = 0;i<NUMBER_OF_FIGURES;++j){
                        if(count[j]>=2&&j!=i){
                            Card const* first_single = deck->get_certain_card(j,card_to_add);
                            card_to_add.push_back(first_single);
                            Card const* second_single = deck->get_certain_card(j,card_to_add);
                            card_to_add.push_back(second_single);
                            break;
                        }
                    }
                    CardsGroup temp=(card_to_add);
                        if(temp.compare(cp)==1){
                            hints.push_back(temp);
                        }
            }
            }
            //then get all possible BOMB
            if(bombs.size()>0){
            
            for(int j=0;j<bombs.size();++j){
                hints.push_back(bombs[j]);
            }
        }
        }
    }
    //SINGLE_CONTINUOUS case
    else if(type==CardsType::Type::SINGLE_CONTINUOUS){
        //do searching iff there exists possible consecutive terms
        if(CardsGroup::count_max_continuous_times(count,1)>=num_input){
        int num_continuous = cp.get_cards_type().get_num_cards();
        int value_of_ref = cp.get_reference_card()->get_value();
        //find the starting point of such consecutive terms
        for(int i=value_of_ref;i<value_of_ref+num_continuous;++i){
        if(i+num_continuous<=11){
            bool check = true;
        for( int j =i;j<value_of_ref+num_continuous;++j){
            if(count[j]==0){check=false;break;}
        }
        //check is true means that such consecutive terms are found, then add them to hint
        if(check){
            Card const* first = deck->get_certain_card(i,{});
            vector<Card const*> card_to_add{first};
            for(int k =1,p=i+1;k<num_continuous;++k,++p){
                Card const* to_add = deck->get_certain_card(p,card_to_add);
                card_to_add.push_back(to_add);
            }
            CardsGroup temp(card_to_add);
                if(temp.compare(cp)==1){
                        hints.push_back(temp);
                    }
        }
        
        }
        }
        }
        //then get all possible BOMB
            if(bombs.size()>0){
            
            for(int j=0;j<bombs.size();++j){
                hints.push_back(bombs[j]);
            }
        }
    }
    //PAIR_CONTINUOUS case
    else if(type==CardsType::Type::PAIR_CONTINUOUS){
        //do searching iff there exists possible consecutive terms
        if(CardsGroup::count_max_continuous_times(count,2)>=num_input/2){
        int num_continuous = num_input/2;
        int value_of_ref = cp.get_reference_card()->get_value();
        for(int i = value_of_ref;i<value_of_ref+num_continuous;++i){
            //consequent checking is meaningful iff the last card in the consecutive term is less or equal than A
            if(i+num_continuous<=11){
                bool check = true;
                for(int j = i;j<value_of_ref+num_continuous;++j){
                    if(count[j]<2){check == false; return;}
                }
                //check is true means that such consecutive terms are found, then add them to hint
                if(check){
                    vector<Card const*> card_to_add {};
                    for(int k = 0, p = i; k<num_continuous;++k,++p){
                        Card const* first_to_add = deck->get_certain_card(p,card_to_add);
                        card_to_add.push_back(first_to_add);
                        Card const* second_to_add = deck->get_certain_card(p,card_to_add);
                        card_to_add.push_back(second_to_add);
                    }
                    CardsGroup temp(card_to_add);
                    if(temp.compare(cp)==1){
                        hints.push_back(temp);
                    }
                }
            }
        }
    }
    //then get all possible BOMB
            if(bombs.size()>0){
            
            for(int j=0;j<bombs.size();++j){
                hints.push_back(bombs[j]);
            }
        }
    }
    //TRIO_CONTINUOUS and PLANE case
    //since these three cases share the same TRIO, we can discuss them together
    else if(type==CardsType::Type::TRIO_CONTINUOUS||type==CardsType::Type::PLANE_WITH_SMALL_WINGS||type==CardsType::Type::PLANE_WITH_BIG_WINGS){
        //do searching iff there exists possible consecutive terms
        vector<int> cp_distribution = cp.get_figures_distribution();
        //get the number of and reference card of consecutive TRIO
        int num_continuous = CardsGroup::count_max_continuous_times(cp_distribution,3);
        int value_of_ref = cp.get_reference_card()->get_value();
        
        if(CardsGroup::count_max_continuous_times(count,3)>=num_continuous){
            
            for(int i = value_of_ref;i<value_of_ref+num_continuous;++i){
                //consequent checking is meaningul iff the last card in the consecutive term is less or equal than A
                if(i+num_continuous<=11){
                    bool check = true;
                    for(int j = i;j<value_of_ref+num_continuous;++j){
                        if(count[j]<3){check = false; return;}
                    }
                    //check is true means that such consecutive terms are found, then add them to hint
                    if(check){
                        vector<Card const*> card_to_add {};
                        for(int k =0, p=i;k<num_continuous;++k,++p){
                            Card const* first_to_add = deck->get_certain_card(p,card_to_add);
                            card_to_add.push_back(first_to_add);
                            Card const* second_to_add = deck->get_certain_card(p,card_to_add);
                            card_to_add.push_back(second_to_add);
                            Card const* third_to_add = deck->get_certain_card(p,card_to_add);
                            card_to_add.push_back(third_to_add);
                        }
                        //for TRIO case
                        if(type==CardsType::Type::TRIO){
                        CardsGroup temp(card_to_add);
                        if(temp.compare(cp)==1){
                            hints.push_back(temp);
                        }
                        }
                        //for PLANE_WITH_SMALL_WINGS case
                        else if(type==CardsType::Type::PLANE_WITH_SMALL_WINGS){
                            //find if there has enough SINGLE cards remaining
                            if(deck->get_num_cards()-3*num_continuous>=num_continuous){
                                for(int k = 0;k<num_continuous;++k){
                                    Card const* to_add = deck->get_certain_card(card_to_add);
                                    card_to_add.push_back(to_add);
                                }
                                CardsGroup temp(card_to_add);
                                if(temp.compare(cp)==1){
                                    hints.push_back(temp);
                                }
                            }
                        }
                        //for PLANE_WITH_BIG_WINGS case
                        else if(type==CardsType::Type::PLANE_WITH_BIG_WINGS){
                            //find if there has enough cards remaining
                            if(deck->get_num_cards()-3*num_continuous>=2*num_continuous){
                                int count_pair = 0;
                                vector<int> pair_index;
                                //then check whether there exists required PAIR
                                for(int  j =0;j<NUMBER_OF_FIGURES;++j){
                                    //be careful that the pair chosen cannot be the TRIO chosen!!
                                    if(count[j]>=2&&(j<i||j>i+num_continuous-1)){
                                        count_pair+=1;
                                        pair_index.push_back(j);
                                    }
                                }
                                //exist required pair
                                if(count_pair>=num_continuous){
                                    //store the pair
                                    for(int p =0;p<num_continuous;++p){
                                        int index = pair_index[p];
                                        Card const* first_to_add = deck->get_certain_card(index,card_to_add);
                                        card_to_add.push_back(first_to_add);
                                        Card const* second_to_add = deck->get_certain_card(index,card_to_add);
                                        card_to_add.push_back(second_to_add);
                                    }
                                    CardsGroup temp(card_to_add);
                                    if(temp.compare(cp)==1){
                                        hints.push_back(temp);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        //then get all possible BOMB
            if(bombs.size()>0){
            
            for(int j=0;j<bombs.size();++j){
                hints.push_back(bombs[j]);
            }
        }
    }
    //BOMB case
    if(bombs.size()>0){
        //compare the bomb
        for(int i =0;i<bombs.size();++i){
            if(bombs[i].compare(cp)==1){hints.push_back(bombs[i]);}
        }
    }
    }
