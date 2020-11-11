#include <iostream>
#include <vector>
#include <string>
#include "Card.h"
#include "CardsType.h"
#include "CardsGroup.h"
#include "Deck.h"
#include "CurrenrPattern.h"
#include "Player.h"
#define NUMBER_OF_PLAYER 3
#define NUMBER_OF_CARDS 54

class Board {
private:
    vector<Player*> players[NUMBER_OF_PLAYER];
    bool game_finish = false, you_win= false, landlord_win = false;
    int landlord_id = -1;

    Deck cards;//A complete set of 54 cards
    
public:
    //Default constructor
    Board();

    //Destructor.Be careful about the memory leak
    ~Board();
    //Delete all cards and players

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
    //Shuffle the Deck
    //shuffle cards, distribute cards, call Deck::rearrange() to sort cards

    bool check(CardsGroup* cardgroup);
    //Determine whether the cardgroup played is valid at the moment
};
