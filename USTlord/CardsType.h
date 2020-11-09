#include <iostream>
#include <vector>
#include <string>
using std::string;

class CardsType {
public:
    enum Type {
        SINGLE, PAIR, TRIO,
        TRIO_WITH_ONE, FOUR_WITH_TWO,
        SINGLE_CONTINUOUS, PAIR_CONTINUOUS, TRIO_CONTINUOUS, PLANE,
        BOMB, ROCKET, 
        EMPTY
    };
private:
    Type type;
    int num_cards;//number of cards in this group
    int num_same_value;//number of cards with same value
    bool continuous;//whether the cards are continuous
public:
    //Constructors and Destructor
    CardsType();
    CardsType(Type type,int num_cards,int num_same_value,bool continuous);
    ~CardsType();

    //Binary Operation
    bool operator==(const CardsType& ct) const;//compare whether two cardstype object are equal(can also be defined as bool operator==(const CardsTpe& ct);)
    

    //Accessors
    string to_string() const;//return the type by string
    Type get_type() const;
    int get_num_cards() const;
    int get_num_same_value() const;
    bool is_continuous() const;

    //Mutators
    void set_type(Type type);
    void set_num_cards(int num_cards);
    void set_num_same_value(int num_same_value);
    void set_continuous(bool continuous);

    //Static Functions
    static bool is_equal(const CardsType& a,const CardsType& b);//return true if two types are exactly the same
    static bool is_comparable(const CardsType& a,const CardsType& b);//return true if two types are the same or ROCKET/BOMB exists
};
