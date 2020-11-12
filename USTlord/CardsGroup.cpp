#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Card.h"
#include "CardsType.h"
#include "CardsGroup.h"
using namespace std;

//Constructor and Destructor
CardsGroup::CardsGroup():cards_type(),reference_card(nullptr),cards(0,nullptr){}
CardsGroup::CardsGroup(vector<const Card*> cards):cards_type(),reference_card(nullptr){
	this->reset(cards);
}
CardsGroup::~CardsGroup() {}

//Helper Functions
void CardsGroup::arrange(){
	//discard nullptr(s) in cards to prevent nullptr dereferencing
	for(int i=0;i<cards.size();++i){
		if(cards[i]==nullptr){
			cards.erase(cards.begin()+i);
			--i;
		}
	}
	
	//sort cards by figures and colors
	sort(cards.begin(),cards.end(),Card::strictly_compare);
	
	//return if cards is empty
	if(cards.size()==0){return;}

	//use figures_distribution to record the situation of cards
	vector<int> figures_distribution=this->get_figures_distribution();

	//sort the distribution by value and find the maximum repetition of a card
	int max_repeat=0;
	vector<int> fig_dis_sorted=figures_distribution;
	sort(fig_dis_sorted.begin(),fig_dis_sorted.end(),std::greater<int>());//sort it in descending order
	max_repeat=fig_dis_sorted[0];

	/*Case I : max_repeat==1*/
	if(max_repeat==1){
		//default settings
		this->cards_type.reset(CardsType::Type::EMPTY,cards.size(),1,false);
		//consider valid cases
		//consider the case of SINGLE
		if(fig_dis_sorted[1]==0){
			this->cards_type.reset(CardsType::Type::SINGLE,1,1,false);
		}
		//consider the case of SINGLE_CONTINUOUS
		bool is_cont=check_continuous(figures_distribution,1);
		if(is_cont){
			this->cards_type.reset(CardsType::Type::SINGLE_CONTINUOUS,cards.size(),1,true);
		}
	}

	/*Case II : max_repeat==2*/
	else if(max_repeat==2){
		//default settings
		this->cards_type.reset(CardsType::Type::EMPTY,cards.size(),2,false);
		//consider valid cases
		if(fig_dis_sorted[1]==0){
			//consider the case of PAIR
			if(figures_distribution[NUMBER_OF_FIGURES-1]==0){
				this->cards_type.reset(CardsType::Type::PAIR,2,2,false);
			}
			//consider the case of ROCKET
			else{
				this->cards_type.reset(CardsType::Type::ROCKET,2,2,false);
			}
		}
		else{
			//consider the case of PAIR_CONTINUOUS
			bool is_cont=check_continuous(figures_distribution,2);
			if(is_cont){
				this->cards_type.reset(CardsType::Type::PAIR_CONTINUOUS,cards.size(),2,true);
			}
		}
	}

	/*Case III : max_repeat==3*/
	else if(max_repeat==3){
		//default settings
		this->cards_type.reset(CardsType::Type::EMPTY,cards.size(),3,false);
		//consider valid cases
		if(fig_dis_sorted[1]==0){
			//consider the case of TRIO
			if(figures_distribution[NUMBER_OF_FIGURES-1]==0){
				this->cards_type.reset(CardsType::Type::TRIO,3,3,false);
			}
		}
		//consider the case of TRIO_WITH_ONE
		else if(fig_dis_sorted[1]==1 && fig_dis_sorted[2]==0){
			this->cards_type.reset(CardsType::Type::TRIO_WITH_ONE,4,3,false);
		}
		//consider the case of TRIO_WITH_PAIR
		else if(fig_dis_sorted[1]==2 && fig_dis_sorted[2]==0){
			this->cards_type.reset(CardsType::Type::TRIO_WITH_PAIR,5,3,false);
		}
		else{
			int max_continuous_times=count_max_continuous_times(figures_distribution,3);
			//consider the case of TRIO_CONTINUOUS
			bool is_cont=check_continuous(figures_distribution,3);
			if(is_cont){
				this->cards_type.reset(CardsType::Type::TRIO_CONTINUOUS,cards.size(),3,true);
			}
			//consider the case of PLANE_WITH_SMALL_WINGS and PLANE_WITH_BIG_WINGS
			else if(max_continuous_times>=2){
				int num_extra_cards=cards.size()-max_continuous_times*3;
				//consider small wings
				if(num_extra_cards==max_continuous_times){
					this->cards_type.reset(CardsType::Type::PLANE_WITH_SMALL_WINGS,cards.size(),3,false);
				}
				//consider big wings
				else if(num_extra_cards==max_continuous_times*2){
					bool is_valid=true;
					for(int i=max_continuous_times;i<max_continuous_times*2;++i){
						if(fig_dis_sorted[i]!=2){
							is_valid=false;
							break;
						}
					}
					if(is_valid){
						this->cards_type.reset(CardsType::Type::PLANE_WITH_BIG_WINGS,cards.size(),3,false);
					}
				}
			}
		}
	}
	/*Case IV : max_repeat==4*/
	else if(max_repeat==4){
		//default settings
		this->cards_type.reset(CardsType::Type::EMPTY,cards.size(),4,false);
		//consider valid cases
		//consider the case of BOMB
		if(fig_dis_sorted[1]==0){
			if(figures_distribution[NUMBER_OF_FIGURES-1]==0){
				this->cards_type.reset(CardsType::Type::BOMB,4,4,false);
			}
		}
		//consider the case of FOUR_WITH_TWO
		else if(cards.size()==6){
			if(figures_distribution[NUMBER_OF_FIGURES-1]<=2){
				this->cards_type.reset(CardsType::Type::FOUR_WITH_TWO,6,4,false);
			}
		}
	}
	else{
		this->cards_type.reset(CardsType::Type::EMPTY,cards.size(),max_repeat,false);
	}
}//sort and calculate cards type

void CardsGroup::choose_ref_card(){
	vector<int> figures_distribution=this->get_figures_distribution();
	//assume the cards is sorted, and finish arrange() function (i.e. know cards_type)
	CardsType::Type current_type=this->cards_type.get_type();
	if(current_type==CardsType::Type::EMPTY){
		this->reference_card=nullptr;
	}
	if(current_type==CardsType::Type::TRIO_WITH_ONE){
		this->reference_card=this->cards[1];
	}
	else if(current_type==CardsType::Type::TRIO_WITH_PAIR || current_type==CardsType::Type::FOUR_WITH_TWO){
		this->reference_card=this->cards[2];
	}
	else if(current_type==CardsType::Type::PLANE_WITH_SMALL_WINGS || current_type==CardsType::Type::PLANE_WITH_BIG_WINGS){
		int figure_index;
		//find the minimum value with repeat_times==3
		for(figure_index=0;figure_index<NUMBER_OF_FIGURES;++figure_index){
			if(figures_distribution[figure_index]==3){
				break;
			}
		}
		//find the card with corresponding value in cards and record it
		for(int i=0;i<cards.size();++i){
			if(cards[i]->get_value()==figure_index){
				this->reference_card=cards[i];
				break;
			}
		}
	}
	else{
		this->reference_card=this->cards[0];
	}//if all cards are the same figure, or is continuous(choose the smallest card for reference)
}//choose the corresponding reference cards 

//Member Function
int CardsGroup::compare(const CardsGroup& a) const{
	if(this->is_valid() && a.is_valid()){
		//if not comparable, return -2
		if(!CardsType::is_comparable(this->cards_type,a.cards_type)){return -2;}
		//if both are the same type, compare the reference card
		if(this->cards_type==a.cards_type){
			if(this->reference_card->get_value()<a.reference_card->get_value()){
				return -1;
			}
			else if(this->reference_card->get_value()==a.reference_card->get_value()){
				return 0;
			}
			else{
				return 1;
			}
		}
		//consider BOMB or ROCKET
		//make use of the order of enum Type to compare
		else{
			if(this->cards_type.get_type()<a.cards_type.get_type()){
				return -1;
			}
			else{return 1;}
		}
		
	}
	else{
		return -2;
	}
}//-2 when not comparable, -1 when *this<a, 0 when *this==a, 1 when *this>a 

//Mutator
void CardsGroup::reset(vector<const Card*> cards){
	this->cards=cards;
	this->arrange();
	this->choose_ref_card();
}//reset CardsGroup

//Accessors
const Card* CardsGroup::operator[](int i) const{
	if(i<0 || i>=this->cards.size()){return nullptr;}
	return this->cards[i];
}

vector<const Card*> CardsGroup::get_cards() const{
	return this->cards;
}

CardsType CardsGroup::get_cards_type() const{
	return this->cards_type;
}

const Card* CardsGroup::get_reference_card() const{
	return this->reference_card;
}

bool CardsGroup::is_valid() const{
	if(this->cards_type.get_type()==CardsType::Type::EMPTY){
		return false;
	}
	return true;
}//check whether this is a legal group to play

vector<int> CardsGroup::get_figures_distribution() const{
	//use figures_distribution to record the situation of cards
	vector<int> figures_distribution(NUMBER_OF_FIGURES,0);
	for(int i=0;i<cards.size();++i){
		int temp=cards[i]->get_value();
		if(temp>=0 && temp<NUMBER_OF_FIGURES){
			++figures_distribution[temp];
		}
	}
	return figures_distribution;
}

//Static Functions
int CardsGroup::count_max_continuous_times(vector<int> figures_distribution,int repeat_times){
	//to ensure the length of figures_distribution == NUMBER_OF_FIGURES
	if(figures_distribution.size()!=NUMBER_OF_FIGURES){
		cerr<<"figures_distribution size is unmatched with NUMBER_OF_FIGURES.\n";
		return ERROR;
	}

	//count the maximum continuous times
	int max_cont_times=0;
	//discard '2' and 'Joker'
	for(int i=0;i<NUMBER_OF_FIGURES-2;++i){
		if(figures_distribution[i]!=repeat_times){continue;}
		else{
			int count_continuous=0;
			for(int j=i;j<NUMBER_OF_FIGURES-2;++j){
				if(figures_distribution[j]==repeat_times){
					++count_continuous;
				}
				else{
					i=j;
					break;
				}
			}
			if(count_continuous>max_cont_times){
				max_cont_times=count_continuous;
			}
		}
	}
	return max_cont_times;
}

bool CardsGroup::check_continuous(vector<int> figures_distribution,int repeat_times){
	//to ensure the length of figures_distribution == NUMBER_OF_FIGURES
	if(figures_distribution.size()!=NUMBER_OF_FIGURES){
		cerr<<"figures_distribution size is unmatched with NUMBER_OF_FIGURES.\n";
		return false;	
	}
	if(repeat_times<=0 || repeat_times>3){return false;}

	//sum up the figures_distribution to find the size of cards
	int size=0;
	for(int i=0;i<figures_distribution.size();++i){
		size+=figures_distribution[i];
	}
	if(size<5){return false;}

	//calculate the maximum continuous times
	int continuous_times=count_max_continuous_times(figures_distribution,repeat_times);

	//return false if there is no pattern like 34567... or 334455... or 333444...
	if(continuous_times*repeat_times<5){return false;}
	//return false if there are extra cards besides continuous group
	if(continuous_times*repeat_times!=size){return false;}
	//else return true
	return true;
	
}//check whether CONTINUOUS exists
