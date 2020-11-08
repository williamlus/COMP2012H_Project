#include <iostream>
#include <vector>
#include <string>
using std::string;

class CardsType {
public:
    enum class Type {
        ROCKET, BOMB, SINGLE, PAIR, TRIO, TRIO_WITH_ONE,
        SINGLE_CONTINUOUS, PAIR_CONTINUOUS, TRIO_CONTINUOUS,
        PLANE, FOUR_WITH_TWO
    };
private:
    Type type;
    int num_cards;//number of cards in this group
    int num_same_value;//number of cards with same value
    bool continuous;//whether the cards are continuous
public:
    CardsType();
    ~CardsType();
    bool is_equal(const CardsType& ct) const;//compare whether two cardstype object are equal(can also be defined as bool operator==(const CardsTpe& ct);)
    string to_string() const;//return the type by string

    //accessors
    Type get_type() const;
    int get_num_cards() const;
    int get_num_same_value() const;
    bool is_continuous() const;

    //mutators
    void set_type(Type type);
    void set_num_cards(int num_cards);
    void set_num_same_value(int num_same_value);
    void set_continuous(bool continuous);
};
