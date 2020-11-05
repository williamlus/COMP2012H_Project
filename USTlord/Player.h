#include <iostream>
#include <vector>
#include <string>
#include "Card.h"
#include "CardsType.h"
#include "CardsGroup.h"
#include "Deck.h"

class Player : Deck {
    private:
      string name;
      bool is_landlord = false;
    public:
      //constructor
      Player(string _name) : name(_name), Deck() {}
      ~Player();
      void print();//print cards in hand
      CardsGroup tip();//tip function
      void play(CardsGroup& cardgroup);//Play cards
};