#ifndef CARD_H_
#define CARD_H_

#define NUMBER_OF_CARDS 54
#define NUMBER_OF_FIGURES 14
#define ERROR -1

#include <string>
#include <QDataStream>
#include "source/CardPicture.h"

using namespace std;

const char figures_to_int[] = "3456789TJQKA2W";
class Card {
    public:
    //arranged in conventional ascending order
    //designed for better GUI
    enum Color{
        EMPTY,
        DIAMOND,
        CLUB,
        HEART,
        SPADE,
        BLACK_JOKER,
        RED_JOKER
    };
    private:
      Color color;
      QString str_color{"E"};
      int value;
      CardPicture* card_picture{nullptr};
    public:

      //Constructors and Destructor
      Card();
      Card(Color color, int value);//other constructor
      Card(char color, char figure);//other constructor
      Card(const Card& c);//copy constructor
      ~Card();//destructor
      
      //Accessors
      string get_string() const;//return the value according to figures_to_int
      int get_value() const;//get the value of the card
      Color get_color() const;//get the color of the card
      bool is_valid() const;//check if the card is valid
      CardPicture* get_card_picture() const;//get card picture
      int get_index() const;

      //Mutators
      void reset(char color, char figure);
      void reset(Color color, int value);
      void set_value(int value);
      void set_color(Color color);
      void set_card_picture(CardPicture* card_pic);

      //Binary Operations
      bool operator<(const Card& a) const;//Compare the value of two cards
      bool operator==(const Card& a) const;//Compare the value of two cards
      bool operator>(const Card& a) const;//Compare the value of two cards
      bool operator<=(const Card& a) const;//Compare the value of two cards
      bool operator>=(const Card& a) const;//Compare the value of two cards
      bool operator!=(const Card& a) const;//Compare the value of two cards
      friend QDataStream& operator >>(QDataStream& in,Card card){
          in >> card.str_color >>  card.value;
          return in;
      }
      friend QDataStream& operator <<(QDataStream& out,Card card){
          out << card.str_color << card.value;
          return out;
      }

      //Static Functions
      static string to_string(Color color,int value);//transform color, value into string
      static Color to_color(char color);//transform user input color into Color color
      static int to_value(char figure);//transform char figure into int value
      static bool compare_value(const Card* a, const Card* b);//return true if a's value is smaller than b's value, otherwise false
      static bool strictly_compare(const Card* a, const Card* b);//strictly compare two cards in terms of value and color
      static bool strictly_equal(const Card* a, const Card* b);
};

#endif /* CARD_H_ */
