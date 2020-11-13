#include <iostream>
#include <string>
#include "Card.h"
using namespace std;

/*Constructors*/
Card::Card():color(Color::EMPTY),value(ERROR){}
Card::Card(Color color, int value) : color(color), value(value){}//other constructor
Card::Card(char color, char figure) : color(to_color(color)), value(to_value(figure)){}//other constructor
Card::Card(const Card& c) : value(c.value),color(c.color){}//copy constructor

/*Destructor*/
Card::~Card(){}//destructor

/*Accessors*/
string Card::get_string() const{
    return to_string(this->color,this->value);
}//return the value according to figures_to_int

int Card::get_value() const{
    return this->value;
}//get the value of the card

Card::Color Card::get_color() const{
    return this->color;
}//get the color of the card

bool Card::is_valid() const{
    if(this->color==EMPTY || this->value==ERROR){
        return false;
    }
    else if(this->value==NUMBER_OF_FIGURES-1){
        if(this->color!=RED_JOKER && this->color!=BLACK_JOKER){
            return false;
        }
    }
    return true;
}//check if the card is valid

/*Mutators*/
void Card::reset(char color, char figure){
    this->color=to_color(color);
    this->value=to_value(figure);
}
void Card::reset(Color color, int value){
    this->color=color;
    this->value-value;
}

void Card::set_value(int value){
    this->value=value;
}

void Card::set_color(Color color){
    this->color=color;
}

/*Binary Operations*/
bool Card::operator<(const Card& a) const{
    return Card::compare_value(this,&a);
}//Compare the value of two cards

bool Card::operator==(const Card& a) const{
    //is neither smaller nor larger
    return ((!compare_value(this,&a)) && (!compare_value(&a,this)));
}//Compare the value of two cards

bool Card::operator>(const Card& a) const{
    return Card::compare_value(&a,this);
}//Compare the value of two cards

bool Card::operator<=(const Card& a) const{
    return (!Card::compare_value(&a,this));
}//Compare the value of two cards

bool Card::operator>=(const Card& a) const{
    return (!Card::compare_value(this,&a));
}//Compare the value of two cards

bool Card::operator!=(const Card& a) const{
    return (compare_value(this,&a) || compare_value(&a,this));
}//Compare the value of two cards

/*Static Functions*/
string Card::to_string(Color color,int value){
    string temp{};
    
    //Color to char
    if(color==Color::EMPTY){temp+=' ';}
    else if(color==Color::SPADE){temp+='s';}
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
    //Consider the case of Joker
    else if(a->value==b->value && a->value==NUMBER_OF_FIGURES-1 && a->color<b->color){
        return true;
    }
    return false;
}//return true if a's value is smaller than b's value, otherwise false

bool Card::strictly_compare(const Card* a, const Card* b){
    if(a->value<b->value){return true;}
    else if(a->value==b->value && a->color<b->color){return true;}
    return false;
}//strictly compare two cards in terms of value and color

bool Card::strictly_equal(const Card* a, const Card* b){
    if(a->value==b->value && a->color==b->color){return true;}
    return false;
}
