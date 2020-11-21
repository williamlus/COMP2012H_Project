#ifndef AIPLAYER_H_
#define AIPLAYER_H_

#include <iostream>
#include <vector>
#include <string>
#include "Card.h"
#include "CardsType.h"
#include "CardsGroup.h"
#include "Deck.h"
#include "CurrentPattern.h"
#include "Player.h"

class AIPlayer : public Player {
    public:
      //constructor
      AIPlayer();
      AIPlayer(int id,string name);
      ~AIPlayer();
      const CardsGroup choose_hint(const CurrentPattern& cp);//choose cardsgroup according to the situation of board (e.g. num of cards of other players, current_pattern)
      CardsGroup play(const CurrentPattern& cp, vector<int> players_num_cards) override;//use calc_hints, get_hint, choose_hint to play cards according to current pattern, clear_cards to be played, and reset data members
      bool want_landlord() override;//choose to be landlord or not automatically
};

#endif /* AIPLAYER_H_ */
