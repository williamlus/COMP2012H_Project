#ifndef BOARD_H_
#define BOARD_H_

#include <iostream>
#include <vector>
#include <string>
#include "Card.h"
#include "CardsType.h"
#include "CardsGroup.h"
#include "Deck.h"
#include "CurrentPattern.h"
#include "Player.h"
#include "AIPlayer.h"
#define NUMBER_OF_PLAYER 3
#define NUMBER_OF_CARDS 54

class Board {
private:
    vector<Player*> players;
    vector<Card*> set_of_cards;//A complete set of 54 cards
    CurrentPattern* cp;
    bool game_finish = false, you_win= false, landlord_win = false;
    int landlord_id = -1;
    
public:
    //Default constructor
    Board();//generate 54 cards

    //Destructor.Be careful about the memory leak
    ~Board();//delete 54 cards one by one and players

    void init_game();
    //Initialize the game in offline mode:
    //set players names
    //call shuffle(): shuffle and distribute cards
    //choose landlord and distribute landlord cards

    void start_game();
    //Perform the main process of the game
    //players take turns to play cards
    //check game finish state

    int choose_landlord();
    //Choose landlord according to the rule

    void landlord_bonus();
    //Add 3 bonus cards to landlord

    void shuffle();
    //shuffle cards, distribute cards, call Deck::rearrange() to sort cards

    bool check(CardsGroup* cardgroup);
    //Determine whether the cardgroup played is valid at the moment
};

#endif /* BOARD_H_ */
