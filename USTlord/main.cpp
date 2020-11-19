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

bool make_choice(string);

int main() {
    Board* board = new Board();
    char mode='\0';
    do{
        cout << "online or offline mode? (O=online/F=offline) : ";
        cin >> mode;
        cin.ignore(999,'\n');
    }while(mode!='O' &&mode!='o' && mode!='F' && mode!='f');
    do{
        if(mode=='O' || mode=='o'){
            board->init_online_game();
        }
        else{
            board->init_offline_game();
        }
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
