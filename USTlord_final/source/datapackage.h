#ifndef DATAPACKAGE_H
#define DATAPACKAGE_H

#include <QString>
#include <QVector>
#include "source/Card.h"

class DataPackage
{
public:
    enum Action{NONE=-1,
                GIVE_ID,//0,1,2
                CONFIRM_READY,//request, accept, reject
                DEAL_CARDS,//s3 h3...
                CHOOSE_LANDLORD,//request, accept, reject, be_lanlord
                LANDLORD_BONUS,//s3 h3...
                PLAY_CARDS,//do_not_play, s3 h3...
                ANNOUNCE,//win_game, lose_game, end_game
                CHAT,//any QString
                EXCEPTION//quit
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
    static void read(DataPackage,QString);//reading raw data to DataPackage
    QVector<Card*> generate_cards();//generate cards if the content consists of cards
};

//QString DataPackage::Content::REQUEST = "request";
//QString DataPackage::Content::ACCEPT="accept";
//QString DataPackage::Content::REJECT="reject";
//QString DataPackage::Content::BE_LANDLORD="be";
//QString DataPackage::Content::DO_NOT_PLAY="noplay";
//QString DataPackage::Content::WIN_GAME="win";
//QString DataPackage::Content::LOSE_GAME="lose";
//QString DataPackage::Content::END_GAME="end";
//QString DataPackage::Content::QUIT="quit";

#endif // DATAPACKAGE_H
