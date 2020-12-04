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
      const CardsGroup choose_hint(const CurrentPattern& cp);
      /*
       * AI player with some common sense strategy:

       * if the AI player is the first one to play or the CurrentPattern is played by himself
         he will play cards with the following logic:

         First find the smallest card in hand,
         if it is inside a bomb, then DO NOT play it, and find the second smallest one unless he only has one card left
         if not, then if this card is inside any other cards_combination, then just play this combination
         otherwise just play this SINGLE card

       * if the AI player is landlord, then try to beat everyone

       * if the AI player is farmer, then beat landlord all the time
         if the CurrentPattern is played by another farmer
         Only give cards in these cases:
         * the AI player can directly win the game
         * the partner played a small SINGLE or PAIR

       */

      CardsGroup play(const CurrentPattern& cp) override;//use calc_hints, get_hint, choose_hint to play cards according to current pattern, clear_cards to be played, and reset data members
      bool want_landlord() override;//choose to be landlord or not automatically
      /*
       * the probability of choosing lord is determined by how good the cards in hand are
         The evaluation is basically determined by the number of cards_combination
         The cards_combination considered here are:
         TRIO, BOMB, ROCKET
       */
};

#endif /* AIPLAYER_H_ */
