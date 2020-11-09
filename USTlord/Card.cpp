#include <iostream>
#include <string>
#include "Card.h"
using namespace std;

/*Constructors*/
Card::Card(Color color, int value) : color(color), value(value){}//other constructor
Card::Card(char color, char figure) : color(to_color(color)), value(to_value(figure)){}//other constructor
Card::Card(const Card& c) : value(c.value),color(c.color){}//copy constructor

/*Destructor*/
Card::~Card(){}//destructor

/*Accessors*/
string Card::get_string() const{
    return to_string(this->color,this->value);
}//return the value according to figures_to_int

void Card::print() const{
    std::cout << this->get_string() << " ";
}//print this card in console

int Card::get_value() const{
    return this->value;
}//get the value of the card

Card::Color Card::get_color() const{
    return this->color;
}//get the color of the card

/*Mutators*/
void Card::set_value(int value){
    this->value=value;
}

void Card::set_color(Color color){
    this->color=color;
}

/*Binary Operations*/
bool Card::operator<(const Card& a){
    if(this->value<a.value){return true;}
    return false;
}//Compare the value of two cards

bool Card::operator==(const Card& a){
    if(this->value==a.value){return true;}
    return false;
}//Compare the value of two cards

bool Card::operator>(const Card& a){
    if(this->value>a.value){return true;}
    return false;
}//Compare the value of two cards

bool Card::operator<=(const Card& a){
    if(this->value<=a.value){return true;}
    return false;
}//Compare the value of two cards

bool Card::operator>=(const Card& a){
    if(this->value>=a.value){return true;}
    return false;
}//Compare the value of two cards

bool Card::operator!=(const Card& a){
    if(this->value!=a.value){return true;}
    return false;
}//Compare the value of two cards

/*Static Functions*/
string Card::to_string(Color color,int value){
    string temp{};
    
    //Color to char
    if(color==Color::SPADE){temp+='s';}
    else if(color==Color::HEART){temp+='h';}
    else if(color==Color::DIAMOND){temp+='d';}
    else if(color==Color::CLUB){temp+='c';}
    else if(color==Color::BLACK_JOKER){temp+='b';}
    else if(color==Color::RED_JOKER){temp+='r';}
    else{temp+=' ';}
    
    //value to char
    if(value<0 || value>=NUMBER_OF_FIGURES){temp+=' ';}
    else{temp+=figures_to_int[value];}

    return temp;
}//transform color, value into string

Card::Color Card::to_color(char color){
    if(color=='s'){return Color::SPADE;}
    else if(color=='h'){return Color::HEART;}
    else if(color=='d'){return Color::DIAMOND;}
    else if(color=='c'){return Color::CLUB;}
    else if(color=='b'){return Color::BLACK_JOKER;}
    else if(color=='r'){return Color::RED_JOKER;}
    else{return Color::EMPTY;}
}//transform user input color into Color color

int Card::to_value(char figure) {
    for(int i = 0; i<NUMBER_OF_FIGURES ; i++) {
        if(figures_to_int[i] == figure) return i;
    }
    return ERROR;
}//transform char figure into int value

bool Card::compare_value(const Card* a, const Card* b){
    if(a->value<b->value){return true;}
    return false;
}//return true if a's value is smaller than b's value, otherwise false

bool Card::strictly_compare(const Card* a, const Card* b){
    if(a->value<b->value){return true;}
    else if(a->value==b->value && a->color<b->color){return true;}
    return false;
}//strictly compare two cards in terms of value and color
