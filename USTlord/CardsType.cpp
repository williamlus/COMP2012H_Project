#include "CardsType.h"
#include <vector>
#include <string>
using std::string;

//Constructor
CardsType::CardsType():type(Type::EMPTY),num_cards(0),max_repeat_times(0),continuous(false){}
CardsType::CardsType(Type type,int num_cards,int max_repeat_times,bool continuous):
	type(type),num_cards(num_cards),max_repeat_times(max_repeat_times),continuous(continuous){}

//Destructor
CardsType::~CardsType(){}

//Binary Operation
bool CardsType::operator==(const CardsType& ct) const{
	if (this->type == ct.type && this->num_cards == ct.num_cards 
	&& this->max_repeat_times == ct.max_repeat_times && this->continuous == ct.continuous) {
		return true;
	}
	return false;
}//compare whether two cardstype object are equal(can also be defined as bool operator==(const CardsTpe& ct);)

//Accessors
string CardsType::to_string() const{
	string temp{};
	if (this->type==Type::SINGLE){ temp+="Single Card";}
	else if(this->type==Type::PAIR){temp+="Pair";}
	else if(this->type==Type::TRIO){temp+="Triple";}
	else if(this->type==Type::TRIO_WITH_ONE){temp+="Triple With One";}
	else if(this->type==Type::TRIO_WITH_PAIR){temp+="Triple With Pair";}
	else if(this->type==Type::FOUR_WITH_TWO){temp+="Four With Two";}
	else if(this->type==Type::SINGLE_CONTINUOUS){temp+="Single Continuous";}
	else if(this->type==Type::PAIR_CONTINUOUS){temp+="Pair Continuous";}
	else if(this->type==Type::TRIO_CONTINUOUS){temp+="Triple Continuous";}
	else if(this->type==Type::PLANE){temp+="Plane With Wings";}
	else if(this->type==Type::BOMB){temp+="Bomb";}
	else if(this->type==Type::ROCKET){temp+="Rocket";}
	else if(this->type==Type::EMPTY){temp+="Empty";}
	else{temp+="Empty";}
	return temp;
}//return the type by string

CardsType::Type CardsType::get_type() const{
	return this->type;
}
int CardsType::get_num_cards() const{
	return this->num_cards;
}
int CardsType::get_max_repeat_times() const{
	return this->max_repeat_times;
}
bool CardsType::is_continuous() const{
	return this->continuous;
}

//Mutators
void CardsType::reset(Type type,int num_cards,int max_repeat_times,bool continuous){
	this->type = type;
	this->num_cards = num_cards;
	this->max_repeat_times = max_repeat_times;
	this->continuous = continuous;
}
void CardsType::set_type(Type type){
	this->type = type;
}
void CardsType::set_num_cards(int num_cards){
	this->num_cards = num_cards;
}
void CardsType::set_max_repeat_times(int max_repeat_times){
	this->max_repeat_times = max_repeat_times;
}
void CardsType::set_continuous(bool continuous){
	this->continuous = continuous;
}

//Static Functions
bool CardsType::is_equal(const CardsType& a,const CardsType& b){
	if(a.operator==(b)){return true;}
	return false;
}//return true if two types are exactly the same

bool CardsType::is_comparable(const CardsType& a,const CardsType& b){
	if(is_equal(a,b)){return true;}
	else if(a.type==Type::BOMB || a.type==Type::ROCKET || b.type==Type::BOMB || b.type==Type::ROCKET){
		return true;
	}
	return false;
}//return true if two types are the same or ROCKET/BOMB exists


