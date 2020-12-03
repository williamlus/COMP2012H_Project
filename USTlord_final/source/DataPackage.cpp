#include "datapackage.h"

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
    return whole_msg;
}

QByteArray DataPackage::serialize() const
{
    //TODO
    QByteArray arr=this->to_string().toUtf8();
    return arr;
}

QString DataPackage::cards_to_string(QVector<QString/*Card*/> cards)
{
    //TODO
    return QString();
}

DataPackage DataPackage::parse(QByteArray arr)
{
    //TODO
    qDebug() << "Parsing QByteArray";
    QString whole_msg=QString::fromStdString(arr.toStdString());
    QStringList list=whole_msg.split(QLatin1Char(';'), Qt::SkipEmptyParts);
    for(int i=0;i<list.size();++i){
        QString msg=list[i];
        QStringList sublist=msg.split(QLatin1Char(':'),Qt::SkipEmptyParts);
        qDebug() <<
    }
    return DataPackage();
}

void DataPackage::read(DataPackage data, QString raw_data){
    //the format of raw_data should be:
    //(int sender_id);(int actioner_id);(Action action);(QString content)
    //each seperated by ;
    //for the content part, if the content is cards
    //then it is seperated one COMMA
    
    //first split the raw_data into four parts
    QStringList derived_data = raw_data.split(QLatin1Char(';'));
    data.sender = derived_data[0].toInt();
    data.actioner = derived_data[1].toInt();
    data.action = static_cast<Action>(derived_data[2].toInt());
    
    derived_data[3] = derived_data[3].simplified();
    data.content = derived_data[3];
    
}

QVector<Card> DataPackage::generate_cards(){
    // ensure the content consists of cards
    if(!content.contains(',')){
        qDebug() << "Invalid content!"
        return {};
        
    }
    QVector<Card> cards_vector;
    QStringList cards = content.split(',');
    for(QStringList::iterator it = cards.begin();it!=cards.end();++it){
        if((*it).size()==2){
            char color = (*it).at(0).toAscii();
            int value = (*it).at(1).toInt();
        }
        else if(*it.size()==3){
            char color = (*it).at(0).toAscii();
            QStringRef str_value(&(*it),1,2);
            int value = str_value.toInt();
        }
        else{
            qDebug()<<"Invalid card format in content!";
            return;
        }
        Card card_to_add (color,value);
        cards_vector.push_back(card_to_add);
    }
    return cards_vector;
}
