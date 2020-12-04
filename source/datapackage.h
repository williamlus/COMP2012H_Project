#ifndef DATAPACKAGE_H
#define DATAPACKAGE_H

#include <QString>
#include <QVector>
#include "source/Card.h"

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
        /*For each Action, defined the related content as following:
         * NONE: ""
         * GIVE_ID: 0,1,2
         * CONFIRM_READY: Content::REQUEST, Content::ACCEPT, Content:: REJECT
         * DEAL_CARDS: Content::DO_NOT_PLAY,
                       "s3,h5"
           rules for formatting a set of cards:
           (char)color+(int)value or (char)color + (char)figure
           Remark: only JOKER and A are in the form (char)color + (char)figure
               color is defined as:
               * s: SPADE
               * h: HEART
               * c: CLUB
               * d: DIAMOND
               * bW: BLACK_JOKER
               * rW: RED_JOKER

         * CHOOSE_LANDLORD: Content::REQUEST, Content::ACCEPT, Content:: REJECT, Content:: BE_LANDLORD
         * LANDLORD_BONUS: bonus cards in formatted card form
         * PLAY_CARDS: cards_to_play in formatted card form
         * ANNOUNCE: Content:WIN_GAME, Content:: LOSE_GAME, Content:: END_GAME
         * EXCEPTION::Content.quit
         */
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
    //server id=-1, player id=0,1,2
    int sender{-2};
    int actioner{-2};
    Action action{NONE};
    QString content{};

public:
    DataPackage();
    DataPackage(int,int,Action,QString);
    DataPackage(QString);
    void reset(int,int,Action,QString);
    QString to_string() const;
    QByteArray serialize() const;
    static QString cards_to_string(QVector<Card> cards);
    static DataPackage parse(QByteArray arr);
    static void read(DataPackage&,QString);//reading raw data to DataPackage
    QVector<Card*> generate_cards();//generate cards if the content consists of cards
};

/*
  * REQUEST = "request";
  * ACCEPT= "accept";
  * REJECT="reject";
  * BE_LANDLORD="be";
  * DO_NOT_PLAY="noplay";
  * WIN_GAME="win";
  * LOSE_GAME="lose";
  * END_GAME="end";
  * QUIT="quit";
*/

#endif // DATAPACKAGE_H
