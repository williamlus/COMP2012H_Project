#include "CardsType.h"
#include <vector>
#include <string>
using std::string;

CardsType::CardsType(){}
CardsType::~CardsType(){}
bool CardsType::is_equal(const CardsType& ct) const{}//compare whether two cardstype object are equal(can also be defined as bool operator==(const CardsTpe& ct);)
string CardsType::to_string() const{
	string buffer;
	switch (this->type){
	case Type::SINGLE:
		buffer = "Single Card";
		break;
	case Type::PAIR:
		buffer = "Pair";
		break;
	case Type::TRIO:
		buffer = "Triple";
		break;
	case Type::TRIO_WITH_ONE:
		buffer = "Triple With One";
		break;
	case Type::FOUR_WITH_TWO:
		buffer = "Four With Two";
		break;
	case Type::SINGLE_CONTINUOUS:
		buffer = "Single Continuous";
		break;
	case Type::PAIR_CONTINUOUS:
		buffer = "Pair Continuous";
		break;
	case Type::TRIO_CONTINUOUS:
		buffer = "Triple Continuous";
		break;
	case Type::PLANE:
		buffer = "Plane With Wings";
		break;
	case Type::BOMB:
		buffer = "Bomb";
		break;
	case Type::ROCKET:
		buffer = "Rocket";
		break;
	case Type::EMPTY:
		buffer = "Empty";
		break;
	default:
		buffer = "Empty";
	}
}//return the type by string

//accessors
Type CardsType::get_type() const{
	return this->type;
}
int CardsType::get_num_cards() const{
	return this->num_cards;
}
int CardsType::get_num_same_value() const{
	return this->num_same_value;
}
bool CardsType::is_continuous() const{
	return this->continuous;
}

//mutators
void CardsType::set_type(Type type){
	this->type = type;
}
void CardsType::set_num_cards(int num_cards){
	this->num_cards = num_cards;
}
void CardsType::set_num_same_value(int num_same_value){
	this->num_same_value = num_same_value;
}
void CardsType::set_continuous(bool continuous){
	this->continuous = continuous;
}

