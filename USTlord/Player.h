#ifndef PLAYER_H_
#define PLAYER_H_

#include <iostream>
#include <vector>
#include <string>
#include "Card.h"
#include "CardsType.h"
#include "CardsGroup.h"
#include "Deck.h"
#include "CurrentPattern.h"

class Player {
    private:
      string name;
      Deck* deck;
      CardsGroup selected_cards;
      vector<CardsGroup> hints;
      int current_hint;
      bool is_landlord = false;
    public:
      //constructor
      Player():name(),deck(new Deck()){}
      Player(string name) : name(name), deck(new Deck()) {}
      ~Player();
      bool want_landlord();//choose to be landlord or not
      void receive_card(Card const* c);//receive a card given by board
      void display_cards() const;//display cards in hand (deck) and the selected cards to the player
      void calc_hints(const CurrentPattern& cp);//find all possible hints according to current pattern, and stored them in hints.
      CardsGroup get_hint();//get one hint according to hints and current_hint, update the selected_cards, and display_cards()
      void select_card(Card const * c);//select one card from deck and push its pointer to selected_cards
      void unselect_card(Card const * c);//unselect one selected card and remove its pointer from selected_cards
      bool selected_can_beat(const CurrentPattern& cp);//check whether the player's selected cards can beat the last player's CardsGroup
      CardsGroup play(const Board& b);//use cin or hint (with loops) to Play cards according to current pattern, clear_cards, and reset data members
      void clear_cards(const CardsGroup& cg);//clear the played cards' pointers(don't delete)
      void set_name(string name);
      string get_name() const;
      Deck const* get_deck() const;
      int get_num_cards() const;
        
};

#endif /* PLAYER_H_ */