#ifndef DECK_H_
#define DECK_H_

#include <iostream>
#include <vector>
#include <string>
#include "Card.h"
#include "CardsType.h"
#include "CurrenrPattern.h"

class Deck {
    private:
      vector<Card const*> cards;//cards in Deck
      int number_of_cards;//Number of cards in deck
    public:
      Deck();//default constructor, generate an empty deck
      Deck(vector<const Card*> card);//Conversion constructor
      ~Deck();
      Card const* operator[](int i);
      void rearrange();//Sort cards in deck according to their type and value
      void clear_cards(const CardsGroup& cards);//clear the cards' pointers according to cardsgroup (do not delete)
      int get_num_cards() const;
      vector<Card const*> get_cards() const;
};

#endif /* DECK_H_ */