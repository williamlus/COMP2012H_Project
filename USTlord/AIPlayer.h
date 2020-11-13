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
#include "Board.h"

class AIPlayer : public Player {
    public:
      //constructor
      AIPlayer();
      AIPlayer(int id,string name);
      ~AIPlayer();
      const CardsGroup& choose_hint(const Board& b);//choose cardsgroup according to the situation of board (e.g. num of cards of other players, current_pattern)
      CardsGroup play(const Board& b) override;//use cin or hint (with loops) to Play cards according to current pattern, clear_cards, and reset data members
      bool want_landlord() override;//choose to be landlord or not
};

#endif /* AIPLAYER_H_ */
