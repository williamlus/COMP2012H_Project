#include <iostream>
#include <vector>
#define NUMBER_OF_CARDS 54
#define ERROR -1

const char figures_to_int[] = "34567890JQKA2YZ";
class Card {
    public:
    enum class COLOR{
        SPADE,
        HEART,
        DIAMOND,
        CLUB,
        BLACK_JOKER,
        RED_JOKER
    };
    private:
      int value;
      COLOR color;
    public:
      Card(char figure, COLOR _color) : value(trans_val(figure)), color(_color){};
      Card(const Card& c);//copy constructor
      int trans_val(char figure) {
        for(int i = 0; figures_to_int[i]; i++) {
          if(figures_to_int[i] == figure) return i;
        }
        return ERROR;
    }//transform user input char into int value
      void print() const;//print this card in console
      bool cmpcards(Card* a, Card* b);//Compare the priority of two cards
};//Please add the mutators and accessors yourself
