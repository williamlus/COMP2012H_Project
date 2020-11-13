#ifndef CURRENTPATTERN_H_
#define CURRENTPATTERN_H_

#include <iostream>
#include <vector>
#include <string>
#include "Card.h"
#include "CardsType.h"
#include "CardsGroup.h"

class CurrentPattern : public CardsGroup{
    private:
      int player_index;//index of last player who played these cards
    public:
      CurrentPattern():CardsGroup(),player_index(-1){};
      int get_player_index() const;
      void set_player_index(int id);
      bool can_be_beaten_by(int id,const CardsGroup& cg) const;//check whether current_pattern can be beaten by cardsgroup played by the player with the id
      //if both id's are the same, return true
      void record(int id,const CardsGroup& cg);//record the cardsgroup played by the player with the id
};

#endif /* CURRENTPATTERN_H_ */
