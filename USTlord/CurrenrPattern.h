#include <iostream>
#include <vector>
#include <string>
#include "Card.h"
#include "CardsType.h"
#include "CardsGroup.h"

class CurrentPattern{
    static CurrentPattern* current_pattern;
    private:
      CardsGroup cardgroup;//Cardgroup last player played
      int player_index;//index of last player who played these cards
    public:
    CardsGroup find_valid_group(Deck& cards_in_hand);//find card group that can beat current cards
    static CurrentPattern* get_current();//get Current pattern, if no previous cards, create a new current pattern object
};