#include "datapackage.h"

DataPackage::DataPackage()
{

}

DataPackage::DataPackage(int sender, int actioner, DataPackage::Action action, QString content):
    sender(sender),actioner(actioner),action(action),content(content)
{

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
        qDebug() << sublist[0];
        qDebug() << sublist[1];
        //cond
    }
    return DataPackage();
}
