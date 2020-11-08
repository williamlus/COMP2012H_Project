#include <iostream>
#include <vector>
#include <string>
#include "Card.h"
#include "CardsType.h"
#include "CardsGroup.h"

CardsGroup::CardsGroup(vector<Card*> cards){
	//conversion constructor, deep copy of cards

}
CardsGroup::~CardsGroup(){}
Card* CardsGroup::operator[](int i){}
CardsType CardsGroup::get_card_type() const{}
void CardsGroup::convert_card_type(){}//classify selected cards and give them a card_type
void CardsGroup::init(string str, Deck* cards_held){}//initialize the cardgroup with given data(probably this will be rewritten with better designed constructor)
void CardsGroup::init(vector<Card*> new_cards){}
bool CardsGroup::is_valid() const{}//check whether this is a legal group to play
int CardsGroup::calculate_value() const{}//calculate reference value
void CardsGroup::arrange(){}//sort and calculate cards type

//accessor
vector<const Card*> CardsGroup::getCards() const{}
CardsType CardsGroup::get_card_type() const{}
int CardsGroup::get_reference_value() const{}

//mutator
void CardsGroup::setCards(const vector<Card*>& cards){}
void CardsGroup::setCardsType(const CardsType& card_type){}