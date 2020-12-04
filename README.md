# USTlandlord Game

By COMP2012H 2020FALL Group21, ZHENG Hantao, SU Hong, LU Weiqi, GUO Bingcan



### 1. Introduction

Dou dizhu is described as easy to learn but hard to master, requiring mathematical and strategic thinking as well as carefully planned execution. Suits are irrelevant in playing doudizhu. Players can easily play the game with a set of dou dizhu playing cards, without the suits printed on the cards. Less popular variations of the game do exist in China, such as four-player and five-player dou dizhu played with two packs of cards. 

Making a Qt GUIbased game has moderate difficulty and can be expanded with many other features such as multi-player, background music and even a chat box for players to communicate with their teammates and opponents, while the project is relatively s itable to apply beginner’slevel OOP to achieve.

### 2. Overview

Apply OOP to develop a landlord game with GUI based on QT with followingfeatures:

1. A computer “AI” player written based on simple game logic as human

2. Multip-player mode, which is the most important part of the game.

3. Background music and fancy GUI.

   

**Basic Rules and Strateges**

1.**A player can deal with an unrelated or useless card by making it the kicker card.**

2.**Rocket and bomb are the ultimate weapons in the game.** 

If they either is in one's hand, one should make full use of it when time comes. If players do not possess high confidence in winning, they may think twice about dealing a rocket or bomb, because once the dealt, the risk stake of the round will be doubled (players will either win or lose double) with each rocket or bomb dealt.

3.**Peasants must co-operate to fight against the landlord.**

For example, since the two peasants come after one another, the first peasant might play a low single card if the second peasant has a sole card left.    
(Reference: https://en.wikipedia.org/wiki/Dou_dizhu#Rules)




### 3. Design

* **Class Design**

  Console Game：

  * Board
  * Cards
  * CardsGroup
  * CardsType
  * CurrentPattern
  * Deck 
  * Player

  Networking：

  * Datapackage
  * ServerWindow

  GUI：

  * playerwindow
  * mainwindow
  * serverwindow
  * clientwindow

  

* **AI Player Design**

  AI Logic：  

  `const CardsGroup choose_hint(const CurrentPattern& cp);`

    * AI player with some common sense strategies:

    * if the AI player is the first one to play or the CurrentPattern is played by himself
      he will play cards with the following logic:

      First find the smallest card in hand,
      if it is inside a bomb, then DO NOT play it, and find the second smallest one unless he only has one card left
      if not, then if this card is inside any other cards_combination, then just play this combination
      otherwise just play this SINGLE card

    * if the AI player is landlord, then try to beat everyone

    * if the AI player is farmer, then beat landlord all the time

    * if the CurrentPattern is played by another farmer
      Only give cards in these cases:

      * the AI player can directly win the game
      * the partner played a small SINGLE or PAIR     



  `bool want_landlord() override;//choose to be landlord or not automatically`
      

    * the probability of choosing lord is determined by how good the cards in hand are

      The evaluation is basically determined by the number of cards_combination

      The cards_combination considered here are:

      TRIO,BOMB, ROCKET


* **UI Design**

1. Distribute cards effect.
2. Show buttons according to game process.
3. Info bar displays game process to players.
4. Play cards effect and view other players' cards.
5. Serverwindow and Clientwindow design.



* **Networking Design**

**Transfer principle:** 
      Using Transmission Control Protocol (TCP)\
      The one created room will simultaneously be SERVER and CLIENT, the rest two are just CLIENT\
      Before sending and after receiving the raw_data, the information is stored in user_defined object: **Datapackage**\
      For more details of the usages of DataPackage, please refer to `DataPackage.h`
      

        class DataPackage
              {
              public:
                  enum Action{
                                  NONE=-1,
                                  GIVE_ID,
                                  CONFIRM_READY,
                                  DEAL_CARDS,
                                  CHOOSE_LANDLORD,
                                  LANDLORD_BONUS,
                                  PLAY_CARDS,
                                  ANNOUNCE,
                                  CHAT,
                                  EXCEPTION
                                 };
                      
        struct Content{
                      static const QString REQUEST;
                      static const QString ACCEPT;
                      static const QString REJECT;
                      static const QString BE_LANDLORD;
                      static const QString DO_NOT_PLAY;
                      static const QString WIN_GAME;
                      static const QString LOSE_GAME;
                      static const QString END_GAME;
                      static const QString QUIT;
                  };
    
        public:
                  
                  int sender;
                  int actioner;
                  Action action;
                  QString content;
    
        };


**DataPackage** is equivalent to a formatted QString: "sender:`(int) sender_id`;actioner:`(int) actioner_id`;Action;Content"

Before sending  **DataPackage**, we will first convert it to the formatted QString, then serialize it to QByteArray

After receiving **raw_data(QByteArray)**, we will first convert it to QString, then convert it to DataPackage



### 4. UI Display

Main window

Single Player game:

* Choose landlord：
* Player cards:
* View hints:
* Game over:

Multiplayer Player game:



### 5. Summary
  
  **This project design is a comprehensive OOP program design. The development of USTlord game involves object-oriented programming, data structure, TCP networking and GUI design. We encounted and overcame quite a lot problems due to lack of knowledge and communication problems of multi-person collaboration. The experience we gain are as follows:  **
  
  -Deenpen the understanding of object-oriented programming
  
      Different from process-oriented program，object-oriented programming involves class, object, inheritance, polymorphism etc., which greatly enhance the resuability of the codes, and also provide convinience for deleveloping projects in team.
    
 -Modern development concept
 
    In a program involving multiple modules, the more modern development method is encapsulation. After determining the structure of the program, encapsulate the different modules. On the one hand, it is easy to combine and use the modules. On the other hand, clear and clear interfaces are also the cornerstone of efficient cooperation.
   
-Development tools and multi-person collaboration

     Modern program development is not alone. Multi-person remote collaboration and project management are a very important part of program specification development. The use of Git for version control and project management in this program design has brought great convenience to the development of the program, and also allowed us to deepen our understanding of the actual software engineering process.
