#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Card.h"
#include "CardsType.h"
#include "CardsGroup.h"
#include "Deck.h"
using namespace std;

CardsGroup::CardsGroup(vector<const Card*> cards):cards_type(),reference_card(nullptr){
	this->reset(cards);
}
CardsGroup::~CardsGroup() {}

void CardsGroup::reset(vector<const Card*> cards){
	this->cards=cards;
	this->arrange();
	this->choose_ref_card();
}//reset CardsGroup

//helper function
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
	vector<int> figures_distribution(NUMBER_OF_FIGURES,0);
	for(int i=0;i<cards.size();++i){
		int temp=cards[i]->get_value();
		if(temp>=0 && temp<NUMBER_OF_FIGURES){
			++figures_distribution[temp];
		}
	}

	//sort the distribution by value and find the maximum repetition of a card
	int max_repeat=0;
	vector<int> fig_dis_sorted=figures_distribution;
	sort(fig_dis_sorted.begin(),fig_dis_sorted.end(),std::greater<int>());//sort it in descending order
	max_repeat=fig_dis_sorted[0];

	/*Case I : max_repeat==1*/
	if(max_repeat==1){
		//consider the case of SINGLE
		if(fig_dis_sorted[1]==0){
			this->cards_type.reset(CardsType::Type::SINGLE,1,1,false);
		}
		//consider the case of SINGLE_CONTINUOUS
		bool is_cont=check_continuous(figures_distribution,1);
		if(is_cont){
			this->cards_type.reset(CardsType::Type::SINGLE_CONTINUOUS,cards.size(),1,true);
		}
		else{
			this->cards_type.reset(CardsType::Type::EMPTY,cards.size(),1,false);
		}
	}

	/*Case II : max_repeat==2*/
	if(max_repeat==2){
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
			else{
				this->cards_type.reset(CardsType::Type::EMPTY,cards.size(),2,false);
			}
		}
	}

	/*Case III : max_repeat==3*/
	if(max_repeat==3){
		if(fig_dis_sorted[1]==0){
			//consider the case of TRIO
			if(figures_distribution[NUMBER_OF_FIGURES-1]==0){
				this->cards_type.reset(CardsType::Type::TRIO,3,3,false);
			}
			else{
				this->cards_type.reset(CardsType::Type::EMPTY,3,3,false);
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
					else{this->cards_type.reset(CardsType::Type::EMPTY,cards.size(),3,false);}
				}
			}
			else{this->cards_type.reset(CardsType::Type::EMPTY,cards.size(),3,false);}
		}
	}
	/*Case IV : max_repeat==4*/
	if(max_repeat==4){
		//consider the case of BOMB
		if(fig_dis_sorted[1]==0){
			if(figures_distribution[NUMBER_OF_FIGURES-1]==0){
				this->cards_type.reset(CardsType::Type::BOMB,4,4,false);
			}
			else{this->cards_type.reset(CardsType::Type::EMPTY,4,4,false);}
		}
		//consider the case of FOUR_WITH_TWO
		if(cards.size()==6){
			if(figures_distribution[NUMBER_OF_FIGURES-1]<=2){
				this->cards_type.reset(CardsType::Type::FOUR_WITH_TWO,6,4,false);
			}
			else{this->cards_type.reset(CardsType::Type::EMPTY,6,4,false);}
		}
		else{
			this->cards_type.reset(CardsType::Type::EMPTY,cards.size(),4,false);
		}
	}
	else{
		this->cards_type.reset(CardsType::Type::EMPTY,cards.size(),max_repeat,false);
	}
}//sort and calculate cards type

void CardsGroup::choose_ref_card(){}//choose the corresponding reference cards 

//accessor
const Card* CardsGroup::operator[](int i) const{
	if(i<0 || i>=this->cards.size()){return nullptr;}
	return this->cards[i];
}
vector<const Card*> CardsGroup::get_cards() const{}
CardsType CardsGroup::get_cards_type() const{
	return this->cards_type;
}
const Card* CardsGroup::get_reference_card() const{}
bool CardsGroup::is_valid() const{}//check whether this is a legal group to play
