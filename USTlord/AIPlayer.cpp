#include "AIPlayer.h"

AIPlayer::AIPlayer():name(),deck(new Deck()){}
AIPlayer::AIPlayer(string name) : name(name), deck(new Deck()) {}
AIPlayer::~AIPlayer(){}
const CardsGroup& AIPlayer::choose_hint(const Board& b){}//choose cardsgroup according to the situation of board (e.g. num of cards of other players, current_pattern)
CardsGroup AIPlayer::play(const Board& b){}//use cin or hint (with loops) to Play cards according to current pattern, clear_cards, and reset data members
bool AIPlayer::want_landlord(){}//choose to be landlord or not