#include <iostream>
#include <vector>
#include <string>
#include "Card.h"
#include "CardsType.h"
#include "CardsGroup.h"
#include "CurrentPattern.h"
#include "Deck.h"
#include "Player.h"
#include "Board.h"

using std::cout;
using std::cin;
using std::endl;

int main() {
    Board* board = new Board();
    do{
        board->init_game();
        board->start_game();
    }while(make_choice("\nStart another game? (Y=yes/N=no) :"));
    delete board;
    return 0;
}

bool make_choice(string msg) {
    cout << msg;
    string input;
    cin >> input;
    return input=="Y"||input=="y";
}
