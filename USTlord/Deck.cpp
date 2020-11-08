#include <iostream>
#include <vector>
#include <string>
#include "Card.h"
#include "CardsType.h"
#include "CardsGroup.h"
#include "Deck.h"
#include <algorithm>

using namespace std;

Deck::Deck(){
    number_of_cards=54;
    Card::COLOR color;
    
    for(int i=0;i<54;++i){
        char figure;

        if(i<52){
            figure = figures_to_int[i%13];
            int j = i/13;
            switch(j){
                case 0:
                color = Card::COLOR::SPADE;
                break;
                case 1:
                color = Card::COLOR::HEART;
                break;
                case 2:
                color = Card::COLOR::DIAMOND;
                break;
                case 3:
                color = Card::COLOR::CLUB;
                break;
            }
        }
        else if(i==52){
            figure = figures_to_int[13];
            color = Card::COLOR::BLACK_JOKER;
        }
        else{
            figure = figures_to_int[14];
            color = Card::COLOR::RED_JOKER;
        }
        Card *temp = new Card(figure,color);
        cards.push_back(temp);
    }
}

Deck::Deck(vector<Card*> card){
    number_of_cards=card.capacity();
    cards=card;
    }

Card* Deck::operator[](int i){
    return this->cards[i];
}


bool cmpcards(Card* a,Card*b);
void Deck::rearrange(){
    sort(this->cards.begin(),this->cards.end(),cmpcards);
}


