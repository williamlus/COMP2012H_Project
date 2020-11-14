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
#include <vector>

class Player {
    protected:
      int id;
      string name;
      Deck* deck;
      CardsGroup selected_cards_group;
      vector<CardsGroup> hints;
      int current_hint;
      bool is_landlord = false;
      vector<string*> request_cards_string();//only check the format during the request
    public:
      //constructor
      Player();
      Player(int id,string name);
      ~Player();
      //Mutator
      void set_id(int id);
      void set_name(string name);
      void set_is_landlord(bool is_landlord);
      void receive_card(Card const* c);//receive a card given by board
      void abandom_cards();//clear all cards in hand
      //Accessors
      int get_id() const;
      string get_name() const;
      Deck const* get_deck() const;
      int get_num_cards() const;
      void display_cards() const;//display cards in hand (deck) and the selected cards to the player
      //Member functions
      virtual bool want_landlord();//choose to be landlord or not
      void calc_hints(const CurrentPattern& cp);//find all possible hints according to current pattern, and stored them in hints.
      CardsGroup get_hint();//get one hint according to hints and current_hint, update the selected_cards_group, and display_cards()
      void select_card(Card const * c);//select one card from deck and push its pointer to selected_cards_group, if card exists in selected_cards_group, then unselect it
      bool selected_can_beat(const CurrentPattern& cp);//check whether the player's selected cards can beat the last player's CardsGroup
      virtual CardsGroup play(const CurrentPattern& cp);//use cin or hint (with loops) to play cards according to current pattern, clear_cards, and reset data members
      void clear_cards(const CardsGroup& cg);//clear the played cards' pointers(don't delete)
};

#endif /* PLAYER_H_ */