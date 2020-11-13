#ifndef DECK_H_
#define DECK_H_

#include <iostream>
#include <vector>
#include <string>
#include "Card.h"

class Deck {
    private:
      vector<Card const*> cards;//cards in Deck
    public:
      Deck();//default constructor, generate an empty deck
      Deck(vector<Card const*> card);//Conversion constructor
      ~Deck();
      void set_cards(vector<Card const*> cards);
      Card const* operator[](int i);
      void add_card(Card const* c);
      void rearrange();//Sort cards in deck according to their type and value
      void clear_cards(vector <Card const*> c);//clear the cards' pointers according to cardsgroup (do not delete)
      int get_num_cards() const;
      vector<Card const*> get_cards() const;
};

#endif /* DECK_H_ */