#include <iostream>
#include <vector>
#include <string>
#include "Card.h"
#include "CardsType.h"
#include "CardsGroup.h"
#include "Deck.h"

class Player {
    private:
      Deck* deck;
      string name;
      bool is_landlord = false;
    public:
      //constructor
      Player(string _name) : name(_name), deck(new Deck()) {}
      ~Player();
      void print();//print cards in hand
      CardsGroup tip();//tip function
      void play(CardsGroup& cardgroup);//Play cards//probably contain makeChoive
      void set_name(string name);
      string get_name() const;
      Deck get_deck() const;
        
};
