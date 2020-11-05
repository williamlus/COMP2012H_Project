#include <iostream>
#include <vector>
#include <string>
using std::string;

class CardsType {
private:
    int typeID;
    string type;
    int cards_number;//number of cards in this group
    int number_same_value;//number of cards with same value
    bool is_continuous;//whether the cards are continuous
public:
    CardsType();
    ~CardsType();
    bool is_equal(const CardsType& ct) const;//compare whether two cardstype object are equal(can also be defined as bool operator==(const CardsTpe& ct);)
};//Please add the mutators and accessors yourself
