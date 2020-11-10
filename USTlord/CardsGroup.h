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
    
    int count_max_continuous_times(vector<int> figures_distribution,int repeat_times);
    bool check_continuous(vector<int> figures_distribution,int repeat_times);//check whether CONTINUOUS exists
public:
    CardsGroup(vector<const Card*> cards);
    ~CardsGroup();
    
    void reset(vector<const Card*> cards);//reset CardsGroup
    
    void arrange();//sort cards and calculate cards type
    void choose_ref_card();//choose the corresponding reference cards 
    

    //accessor
    const Card* operator[](int i) const;
    vector<const Card*> get_cards() const;
    CardsType get_cards_type() const;
    const Card* get_reference_card() const;
    bool is_valid() const;//check whether this is a legal group to play
};


