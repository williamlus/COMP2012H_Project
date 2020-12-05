#include "source/datapackage.h"
#include <QDebug>
#include <QChar>
#include <QString>

DataPackage::DataPackage()
{

}

DataPackage::DataPackage(int sender, int actioner, DataPackage::Action action, QString content):
    sender(sender),actioner(actioner),action(action),content(content)
{

}

DataPackage::DataPackage(QString raw_data){
    read(*this,raw_data);
}

void DataPackage::reset(int sender, int actioner, DataPackage::Action action, QString content)
{
    this->sender=sender;
    this->actioner=actioner;
    this->action=action;
    this->content=content;
}

QString DataPackage::to_string() const
{
    //TODO
    QString whole_msg{};
    QString sender_msg="sender:"+QString::number(sender)+";";
    QString actioner_msg="actioner:"+QString::number(actioner)+";";
    QString action_msg="action:"+QString::number(static_cast<int>(action))+";";
    QString content_msg="content:"+content+";";
    whole_msg=sender_msg+actioner_msg+action_msg+content_msg;
    //debugging
    qDebug() << "datapackage to string";
    qDebug() << whole_msg;
    return whole_msg;
}

QByteArray DataPackage::serialize() const
{
    //TODO
    QByteArray arr=this->to_string().toUtf8();
    //debugging
    qDebug() << "serializing";
    qDebug() << QString::fromStdString(arr.toStdString());
    return arr;
}

QString DataPackage::cards_to_string(QVector<Card> cards)
{
    //TODO
    QString str_cards;
    for(auto it = cards.begin();it!=cards.end();it++){
        Card::Color enum_color = (*it).get_color();
        int value = (*it).get_value();
        string str = Card::to_string(enum_color,value);
        QString qstr = QString::fromStdString(str);
        if((it+1)!=cards.end())
            str_cards = str_cards+qstr+",";
        else
            str_cards += qstr;
    }
    //debugging
    qDebug() << "cards to string";
    qDebug() << str_cards;
    return str_cards;
}

DataPackage DataPackage::parse(QByteArray arr)
{
    //TODO
    qDebug() << "Parsing QByteArray";
    QString whole_msg=QString::fromStdString(arr.toStdString());
    return DataPackage(whole_msg);
}

void DataPackage::read(DataPackage& data, QString raw_data){/////////////
    //the format of raw_data should be:
    //(int sender_id);(int actioner_id);(Action action);(QString content)
    //each seperated by ;
    //for the content part, if the content is cards
    //then it is seperated one COMMA

    //first split the raw_data into four parts
    QStringList derived_data = raw_data.split(QLatin1Char(';'),Qt::SkipEmptyParts);
    for(int i=0;i<derived_data.size();++i){
        QStringList sub_data=derived_data[i].split(QLatin1Char(':'),Qt::SkipEmptyParts);
        if(sub_data.size()>=2){
            switch (i) {
            case 0:
                data.sender = sub_data[1].toInt();
                break;
            case 1:
                data.actioner = sub_data[1].toInt();
                break;
            case 2:
                data.action = static_cast<Action>(sub_data[1].toInt());
                break;
            case 3:
                if(data.action==DataPackage::CHAT){
                    int index=derived_data[i].indexOf(':');
                    data.content=derived_data[i].mid(index+1);
                }
                else{
                    data.content=sub_data[1];
                }
                break;
            }
        }
    }


}

QVector<Card*> DataPackage::generate_cards(){

    QVector<Card*> cards_vector;
    QStringList cards = content.split(',');
    for(QStringList::iterator it = cards.begin();it!=cards.end();++it){
        if((*it).size()==2){
            char color = (*it)[0].toLatin1();
            int value = (int)(*it)[1].toLatin1();
            Card* card_to_add = new Card(color, value);
            cards_vector.push_back(card_to_add);
        }
        else if((*it).size()==3){
            char color = (*it)[0].toLatin1();
            QStringRef str_value(&(*it),1,2);
            int value = str_value.toInt();
            Card* card_to_add = new Card(color, value);
            cards_vector.push_back(card_to_add);
        }
        else{
            qDebug()<<"Invalid card format in content!";
            return {};
        }

    }
    return cards_vector;
}
