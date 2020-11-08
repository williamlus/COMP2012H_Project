#include <iostream>
#include <vector>
#include <string>
using std::string;

class CardsType {
private:
    int typeID;
    string type;
    int num_cards;//number of cards in this group
    int num_same_value;//number of cards with same value
    bool continuous;//whether the cards are continuous
public:
    CardsType();
    ~CardsType();
    bool is_equal(const CardsType& ct) const;//compare whether two cardstype object are equal(can also be defined as bool operator==(const CardsTpe& ct);)
    
    //accessors
    int get_typeID() const;
    string get_type() const;
    int get_num_cards() const;
    int get_num_same_value() const;
    bool is_continuous() const;

    //mutators
    void set_typeID(int typeID);
    void set_type(string type);
    void set_num_cards(int num_cards);
    void set_num_same_value(int num_same_value);
    void set_continuous(bool continuous);

};
