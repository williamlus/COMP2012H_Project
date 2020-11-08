#include <iostream>
#include <vector>
#include <string>
#include "Card.h"
#include "CardsType.h"
using std::vector;
class CardsGroup {
private:
    vector<Card*> cards;
    CardsType card_type;
public:
    CardsGroup();
    ~CardsGroup();
    Card* operator[](int i);
    CardsType get_card_type() const;
    void convert_card_type();//classify selected cards and give them a card_type
    void init(string str, Deck* cards_held);//initialize the cardgroup with given data(probably this will be rewritten with better designed constructor)
    void init(vector<Card*> new_cards);
    bool is_valid() const;//check whether this is a legal group to play
    int calculate_value() const;//calculate cards value
    void arrange();//sort and calculate cards type

    //accessor
    vector<const Card*> getCards() const;
    CardsType get_card_type() const;

    //mutator
    void setCards(vector<Card*>& cards);
    void setCardsType(CardsType& card_type);
};


