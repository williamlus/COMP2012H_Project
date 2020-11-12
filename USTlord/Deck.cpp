#include <iostream>
#include <vector>
#include <string>
#include "Card.h"
#include "CardsType.h"
#include "CardsGroup.h"
#include "Deck.h"
#include <algorithm>

using namespace std;

Deck::Deck():cards(0,nullptr){}

Deck::Deck(vector<Card const*> cards):cards(cards){}

Card const* Deck::operator[](int i){
    return this->cards[i];
}

void Deck::rearrange(){
    sort(this->cards.begin(),this->cards.end(),Card::strictly_compare);
}

void Deck::clear_cards(const CardsGroup& cg){
    vector<Card const*> cg_cards=cg.get_cards();
    for(int i=0;i<cg_cards.size();++i){
        for(int j=0;j<this->cards.size();++j){
            if(Card::strictly_equal(cg_cards[i],cards[j])){
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


