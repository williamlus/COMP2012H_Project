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
    vector<Player*> palyers[NUMBER_OF_PLAYER];
    bool win = false;//determine whether there is a winner
    Deck cards();//A complete set of 54 cards
public:
    Board();//Default constructor, also does the initialization of cards, i.e insert 54 cards into the deck 
    ~Board();//Destructor, be careful of memory leaks
    void init_game();//Initialize the game in offline mode
    void start_game();//Perform the main process of the game
    void choose_landlord();//Choose landlord according to the rule
    void landlord_bonus();//Add 3 bonus cards to landlord
    void shuffle();//Shuffle the Deck
    bool check(CardsGroup* cardgroup);//Determine whether the cardgroup played is valid at the moment
};
