#ifndef DECK_H_
#define DECK_H_

#include <iostream>
#include <vector>
#include <string>
#include "Card.h"
#include "CardsGroup.h"

class Deck {
    private:
      vector<Card const*> cards;//cards in Deck
      vector<Card const*> cards_in_combination;
      vector<Card const*> cards_in_important_combination;
      void generate_combination();
      bool exist(Card const*);
      bool exist(Card const*,vector<Card const*>);
      //helper function for generate_combination
      const Card* get_certain_card_base(int value,vector<Card const*> chosen);
      bool split_important_combination {false};//a signal of whether this hint must split the important combination. This is used for AI player
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
      const Card* get_certain_card(Card::Color color, int value);
      const Card* get_certain_card(int value,vector<Card const*> chosen );
      const Card* get_certain_card(vector<Card const*> chosen);
      //for all get_certain_card function, before invoke it, make sure such certain card exist, otherwise it will return nullptr
      static vector<int> get_deck_distribution(Deck* deck);
};

#endif /* DECK_H_ */
