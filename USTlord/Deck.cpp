#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Deck.h"

using namespace std;

Deck::Deck():cards(0,nullptr){}

Deck::Deck(vector<Card const*> cards):cards(cards){}

Deck::~Deck(){}

void Deck::set_cards(vector<Card const*> cards){
    this->cards=cards;
}

Card const* Deck::operator[](int i){
    return this->cards[i];
}

void Deck::add_card(Card const* c){
    this->cards.push_back(c);
}

void Deck::rearrange(){
    sort(this->cards.begin(),this->cards.end(),Card::strictly_compare);
}

void Deck::clear_cards(vector <Card const*> c){
    for(int i=0;i<c.size();++i){
        for(int j=0;j<this->cards.size();++j){
            if(Card::strictly_equal(c[i],cards[j])){
                cards.erase(cards.begin()+j);
                break;
            }
        }
    }
}//clear the cards' pointers according to cardsgroup (do not delete)
int Deck::get_num_cards() const{
    return this->cards.size();
}
vector<Card const*> Deck::get_cards() const{
    return this->cards;
}

const Card* Deck::get_certain_card(Card::Color color, int value) {
    Card* temp_card = new Card(color, value);
    for (int i = 0; i < cards.size(); ++i) {
        if(Card::strictly_equal(temp_card,cards[i])){return cards[i]; }
    }
    delete temp_card;
        return nullptr;
}

const Card* Deck::get_certain_card(int value,vector<Card const*> chosen){
    for(int i = 0;i<cards.size();++i){
        if(cards[i]->get_value()==value){
            //check whether it's inside chosen, if not, return it
            bool check=true;
            for(int j = 0;j<chosen.size();++j){
                if(Card::strictly_equal(chosen[j],cards[i])){check=false; break;}
            }
            //if not exist
            if(check){
                return cards[i];
            }
        }
    }
    return nullptr;

}

const Card* Deck::get_certain_card_base(int value,vector<Card const*> chosen){
    for(int i = 0;i<cards.size();++i){
        if(cards[i]->get_value()==value){
            //check whether it's inside chosen, if not, return it
            bool check=true;
            for(int j = 0;j<chosen.size();++j){
                if(Card::strictly_equal(chosen[j],cards[i])){check=false; break;}
            }
            //if not exist
            if(check){
                return cards[i];
            }
        }
    }
    return nullptr;
}

void Deck::generate_combination(){
    vector<int> count = get_deck_distribution(this);
    //find TRIO
    for(int i = 0;i<NUMBER_OF_FIGURES;++i){
        if(count[i]==3){
            for(int j=0;j<3;++j){
                Card const* to_add = get_certain_card_base(i,cards_in_combination);
                cards_in_combination.push_back(to_add);
                cards_in_important_combination.push_back(to_add);
                num_of_important_combination+=1;
            }
        }
    }
    //find if there is BOMB
    for(int i = 0 ; i<NUMBER_OF_FIGURES;++i){
        if(count[i]==4){
            for(int j=0;j<4;++j){
                Card const* to_add = get_certain_card_base(i,cards_in_combination);
                cards_in_combination.push_back(to_add);
                cards_in_important_combination.push_back(to_add);
                num_of_important_combination+=1;
            }

        }
    }
    //find if there is PAIR
    for(int i=0;i<NUMBER_OF_FIGURES;++i){
        if(count[i]==2){
            Card const* first_to_add = get_certain_card_base(i,cards_in_combination);
            cards_in_combination.push_back(first_to_add);
            Card const* second_to_add = get_certain_card_base(i,cards_in_combination);
            cards_in_combination.push_back(second_to_add);
        }
    }
}

const Card* Deck::get_certain_card(vector<Card const*> chosen){
    //if the i-th card in not in the chosen one, use it
    //first not considering return card in cards_in_combination
    for(int i=0;i<cards.size();++i){   
        if(!exist(cards[i],chosen)&&!exist(cards[i],cards_in_combination)){return cards[i];}
    }
    //if no cards left outside cards_in_combination, try to not split the important combination
    for(int i=0;i<cards.size();++i){
        if(!exist(cards[i],chosen)&&!exist(cards[i],cards_in_important_combination)){return cards[i];}
    }
    //if no cards left outside cards_in_important_combination, then split the important_combination for player
    //also set split_important_combination to true, this is used for AI player
    for(int i=0;i<cards.size();++i){
        if(!exist(cards[i],chosen)){return cards[i];}
    }

    return nullptr;
}


bool Deck::exist(Card const* card){
    
    for(int i=0;i<cards.size();++i){
        if(Card::strictly_equal(card,cards[i]))
        return true;
    }
    return false;

}

bool Deck::exist(Card const* card,vector<Card const*> chosen){
    for(int i=0;i<chosen.size();++i){
        if(Card::strictly_equal(chosen[i],card)){return true;}
    }
    return false;
}

vector<int> Deck::get_deck_distribution(Deck* deck) {
    vector<int> count(NUMBER_OF_FIGURES, 0);
    vector<Card const*> current_cards = deck->get_cards();
    for (int i = 0; i < current_cards.size(); ++i) {
        int temp_value = current_cards[i]->get_value();
        count[temp_value]++;
    }
    return count;
}

int Deck::get_num_important_combination(){
    return num_of_important_combination;
}



