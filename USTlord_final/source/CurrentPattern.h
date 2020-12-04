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
      bool is_landlord;
    public:
      CurrentPattern():CardsGroup(),player_index(0){};
      CurrentPattern(vector<const Card*>selected_cards,int active_player_id):CardsGroup(selected_cards),player_index(active_player_id){}
      CurrentPattern(CardsGroup last_player,int active_player_id):CardsGroup(last_player),player_index(active_player_id){}

      void display();
      int get_player_index() const;
      void set_player_index(int id);
      bool can_be_beaten_by(int id,const CardsGroup& cg) const;//check whether current_pattern can be beaten by cardsgroup played by the player with the id
      //if both id's are the same, return true
      void record(int id,const CardsGroup& cg,bool is_landlord);//record the cardsgroup played by the player with the id
      bool current_is_landlord() const;//return the state of current player is landlord or not
};

#endif /* CURRENTPATTERN_H_ */
