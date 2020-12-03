#ifndef DATAPACKAGE_H
#define DATAPACKAGE_H

#include <QString>
#include <QVector>
#include <QStringList>
#include <QDebug>

//namespace  {

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
        const static QString REQUEST;
        const static QString ACCEPT;
        const static QString REJECT;
        const static QString BE_LANDLORD;
        const static QString DO_NOT_PLAY;
        const static QString WIN_GAME;
        const static QString LOSE_GAME;
        const static QString END_GAME;
        const static QString QUIT;
    };

public:
    //server id=-1, player id=0,1,2
    int sender{-2};
    int actioner{-2};
    DataPackage::Action action{NONE};
    QString content{};

public:
    DataPackage();
    DataPackage(int,int,DataPackage::Action,QString);
    void reset(int,int,DataPackage::Action,QString);
    QString to_string() const;
    QByteArray serialize() const;
    static QString cards_to_string(QVector<QString/*Card*/> cards);
    static DataPackage parse(QByteArray arr);
};

//const QString DataPackage::Content::REQUEST = "request";
//const QString DataPackage::Content::ACCEPT="accept";
//const QString DataPackage::Content::REJECT="reject";
//const QString DataPackage::Content::BE_LANDLORD="be";
//const QString DataPackage::Content::DO_NOT_PLAY="noplay";
//const QString DataPackage::Content::WIN_GAME="win";
//const QString DataPackage::Content::LOSE_GAME="lose";
//const QString DataPackage::Content::END_GAME="end";
//const QString DataPackage::Content::QUIT="quit";

//}

#endif // DATAPACKAGE_H
