#include <iostream>
#include <vector>
#include <string>
using std::string;

class CardsType {
public:
    enum Type {
        SINGLE, PAIR, TRIO,
        TRIO_WITH_ONE, TRIO_WITH_PAIR ,FOUR_WITH_TWO,
        SINGLE_CONTINUOUS, PAIR_CONTINUOUS, TRIO_CONTINUOUS, 
        PLANE_WITH_SMALL_WINGS, PLANE_WITH_BIG_WINGS,
        BOMB, ROCKET, 
        EMPTY
    };
private:
    Type type;
    int num_cards;//number of cards in this group
    int max_repeat_times;//number of cards with same value
    bool continuous;//whether the cards are continuous
public:
    //Constructors and Destructor
    CardsType();
    CardsType(Type type,int num_cards,int max_repeat_times,bool continuous);
    ~CardsType();

    //Binary Operation
    bool operator==(const CardsType& ct) const;//compare whether two cardstype object are equal(can also be defined as bool operator==(const CardsTpe& ct);)
    

    //Accessors
    string to_string() const;//return the type by string
    Type get_type() const;
    int get_num_cards() const;
    int get_max_repeat_times() const;
    bool is_continuous() const;

    //Mutators
    void reset(Type type,int num_cards,int max_repeat_times,bool continuous);
    void set_type(Type type);
    void set_num_cards(int num_cards);
    void set_max_repeat_times(int max_repeat_times);
    void set_continuous(bool continuous);

    //Static Functions
    static bool is_equal(const CardsType& a,const CardsType& b);//return true if two types are exactly the same
    static bool is_comparable(const CardsType& a,const CardsType& b);//return true if two types are the same or ROCKET/BOMB exists
};
