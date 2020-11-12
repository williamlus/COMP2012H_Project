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
    Card::Color color;
    
    for(int i=0;i<54;++i){
        char figure;

        if(i<52){
            figure = figures_to_int[i%13];
            int j = i/13;
            switch(j){
                case 0:
                color = Card::Color::SPADE;
                break;
                case 1:
                color = Card::Color::HEART;
                break;
                case 2:
                color = Card::Color::DIAMOND;
                break;
                case 3:
                color = Card::Color::CLUB;
                break;
            }
        }
        else if(i==52){
            figure = figures_to_int[NUMBER_OF_FIGURES-1];
            color = Card::Color::BLACK_JOKER;
        }
        else{
            figure = figures_to_int[NUMBER_OF_FIGURES-1];
            color = Card::Color::RED_JOKER;
        }
        Card *temp = new Card(figure,color);
        cards.push_back(temp);
    }
}

Deck::Deck(vector<Card const*> cards):cards(cards),number_of_cards(cards.size()){}

Card const* Deck::operator[](int i){
    return this->cards[i];
}

void Deck::rearrange(){
    sort(this->cards.begin(),this->cards.end(),Card::strictly_compare);
}


