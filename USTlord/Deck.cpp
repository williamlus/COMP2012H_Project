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

const Card* Deck::get_certain_card(int value,Card const* check){
    Card* temp_card = new Card(Card::Color::SPADE, value);
    for (int i = 0; i < cards.size(); ++i) {
        if(*temp_card==*cards[i]&&cards[i]!=check){return cards[i]; }
    }
    delete temp_card;
        return nullptr;
}

void Deck::generate_combination(){
    //find if there is bomb
}

const Card* Deck::get_certain_card(vector<Card const*> chosen){
    for(int i=0;i<cards.size();++i){
        //if the i-th card in not in the chosen one, use it
         if(!exist(cards[i])){
             return cards[i];
         }
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



