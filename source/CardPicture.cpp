#include "CardPicture.h"
#include <QDebug>

CardPicture::CardPicture(QWidget *parent) : QWidget(parent)
{
    turned = true;
    selected = false;
    resize(60, 80);
    //setFixedSize(60, 80);
    QPixmap pic(":/background/images/card2.png");
    card_back = pic.copy(2*80,4*105,80,105);
}

void CardPicture::paintEvent(QPaintEvent* event)
{

    QPainter painter(this);
    if(turned)
        painter.drawPixmap(this->rect(), card_back);
    else
        painter.drawPixmap(this->rect(), card_pic);
}

void CardPicture::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton) {
        emit selected_signal();
        qDebug() << "Signal emitted!";
    }
}

void CardPicture::setPic(QPixmap& pic) {
    card_pic = pic.copy(0, 0, 110, 154);
}

void CardPicture::turn(bool t) {
    if(turned != t) turned = t;
    return;
}

bool CardPicture::is_turned() { return turned; }

bool CardPicture::is_selected() { return selected; }

void CardPicture::select(bool s) {
    if(selected != s) { selected = s; }
    return;
}
