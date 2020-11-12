#include "Player.h"

Player::Player():name(),deck(new Deck()){}
Player::Player(string name) : name(name), deck(new Deck()) {}
Player::~Player(){}
void Player::set_name(string name){}
string Player::get_name() const{}
Deck const* Player::get_deck() const{}
int Player::get_num_cards() const{}
void Player::receive_card(Card const* c){}//receive a card given by board
void Player::display_cards() const{}//display cards in hand (deck) and the selected cards to the player
bool Player::want_landlord(){}//choose to be landlord or not
void Player::select_card(Card const * c){}//select one card from deck and push its pointer to selected_cards
void Player::unselect_card(Card const * c){}//unselect one selected card and remove its pointer from selected_cards
void Player::calc_hints(const CurrentPattern& cp){}//find all possible hints according to current pattern, and stored them in hints.
CardsGroup Player::get_hint(){}//get one hint according to hints and current_hint, update the selected_cards, and display_cards()
bool Player::selected_can_beat(const CurrentPattern& cp){}//check whether the player's selected cards can beat the last player's CardsGroup
CardsGroup Player::play(const Board& b){}//use cin or hint (with loops) to Play cards according to current pattern, clear_cards, and reset data members
void Player::clear_cards(const CardsGroup& cg){}//clear the played cards' pointers(don't delete)
