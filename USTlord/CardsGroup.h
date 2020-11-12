#ifndef CARDSGROUP_H_
#define CARDSGROUP_H_

#include <iostream>
#include <vector>
#include <string>
#include "Card.h"
#include "CardsType.h"
using std::vector;

class CardsGroup {
private:
    vector<const Card*> cards;
    CardsType cards_type;
    const Card* reference_card;//the value to compare with the same type
    
    //Helper Functions
    void arrange();//sort cards and calculate cards type
    void choose_ref_card();//choose the corresponding reference cards
    
public:
    //Constructor and Destructor
    CardsGroup();
    CardsGroup(vector<Card const*> cards);
    //shallow copy of cards, all operations are based on pointers
    ~CardsGroup();
    
    
    //Member Function
    int compare(const CardsGroup& a) const;
    //-2 when not comparable, -1 when *this<a, 0 when *this==a, 1 when *this>a 

    //Mutator
    void reset(vector<Card const*> cards);//reset CardsGroup
    //it means that you can reuse the same object

    //Accessors
    const Card* operator[](int i) const;
    //get the i+1-th pointer to Card, return nullptr if index out of range 
    
    vector<const Card*> get_cards() const;
    //return a copy of cards (i.e. a vector of Card pointers)
    
    CardsType get_cards_type() const;
    //return a CardsType object
    
    const Card* get_reference_card() const;
    //return the pointer to reference card (use it to compare with other cards_group when CardsType(s) are the same)
    //(more specifically, the smallest card (ONLY value, color is ignored) that can be used to compare)
    //e.g. 345678, ref_card is 3; 44455566639A, ref_card can be any 4 
    
    bool is_valid() const;//check whether this is a legal group to play
    //only consider whether CardsType is EMPTY(i.e. illegal)
    
    vector<int> get_figures_distribution() const;
    //get the distribution of cards according to their value
    //e.g. 444483, its figures_distribution is [1,4,0,0,0,1,0,0,0,0,0,0,0,0]
    //REMINDER: figures_distribution will always have size of NUMBER_OF_FIGURES

    //Static Functions
    static int count_max_continuous_times(vector<int> figures_distribution,int repeat_times);
    //for figures_distribution, see get_figures_distribution()
    //count the maximum continuous times according to repeat times
    //ignore 2 and W by landlord rules
    //e.g. 34555666778899TJQKAA2W, its figures_distribution is [1,1,3,3,2,2,2,1,1,1,1,2,1,1]
    //count_max_continuous_times(figures_distribution,3)==2;
    //count_max_continuous_times(figures_distribution,2)==3;
    //count_max_continuous_times(figures_distribution,1)==4;
    //return -1 when error occurs
    
    static bool check_continuous(vector<int> figures_distribution,int repeat_times);//check whether CONTINUOUS exists
    //check whether the distribution is continuous with repeat_times
    //return false when error occurs or it is discontinuous
};

#endif /* CARDSGROUP_H_ */
