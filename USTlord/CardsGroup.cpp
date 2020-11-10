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
	//TBC
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

	//find the maximum repetition of a card
	int max_repeat=0;
	for(int i=0;i<figures_distribution.size();++i){
		if(figures_distribution[i]>max_repeat){
			max_repeat=figures_distribution[i];
		}
	}
	if(max_repeat!=repeat_times){return false;}

	//consider all cases of continuous
	//Reference: https://www.gameabc.com/news/201903/5038.html
	//find the smallest figure
	int sf;//smallest figure
	for(sf=0;sf<NUMBER_OF_FIGURES;++sf){
		if(figures_distribution[sf]>0){break;}
	}
	//check if it is continuous
	int count=0;//count how long is the continuous cardsgroup
	//only accept 3,4,5,6,7,8,9,10,J,Q,K,A
	for(int i=sf;i<NUMBER_OF_FIGURES-2;++i){
		if(figures_distribution[i]==repeat_times){++count;}
		else{break;}
	}
	//check if the least length is met
	//and if size of cards > count, then the cards is discontinuous
	if(count<5 || size>count*repeat_times){
		return false;
	}
	else{return true;}
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

	//find the maximum repetition of a card
	int max_repeat=0;
	vector<int> fig_dis_sorted=figures_distribution;
	sort(fig_dis_sorted.begin(),fig_dis_sorted.end(),std::greater<int>());//sort it in descending order
	max_repeat=fig_dis_sorted[0];

	/*Case I : max_repeat==1*/
	if(max_repeat==1){
		//this->cards_type.set_max_repeat_times(1);
		
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
		else{
			//consider the case of TRIO_CONTINUOUS
			bool is_cont=check_continuous(figures_distribution,3);
			if(is_cont){
				this->cards_type.reset(CardsType::Type::PAIR_CONTINUOUS,cards.size(),3,true);
			}
			
			//consider PLANE!!!!!
			//Add helper func int count_max_cont_times(fig_dis,rep_times);
			//need to modify check_continuous (for PAIR_CONT,TRIO_CONT, it suffices to find cards.size>=5)
			else if(!is_cont){}

			else{
				this->cards_type.reset(CardsType::Type::EMPTY,cards.size(),3,false);
			}
		}
	}
	/*Case IV : max_repeat==4*/
	if(max_repeat==4){
		//TBC
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
