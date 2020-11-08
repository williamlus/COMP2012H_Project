#include <iostream>
#include <vector>
#include <string>
#include "Card.h"
#include "CardsType.h"
#include "CurrenrPattern.h"

class Deck {
    private:
      vector<Card*> cards;//cards in Deck
      int number_of_cards;//Number of cards in deck
    public:
      Deck();//default constructor
      Deck(int i，vector<Card*> cards);//Conersion constructor, i means the number of cards in this deck
      ~Deck();
      Card* operator[](int i);
      void shuffle();//shuffle cards in deck
      void rearrange();//Sort cards in deck according to their type and value
};
