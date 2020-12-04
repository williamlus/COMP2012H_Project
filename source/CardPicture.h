#ifndef CARDPICTURE_H
#define CARDPICTURE_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>

class CardPicture : public QWidget
{
    Q_OBJECT
private:
    QPixmap card_pic;
    QPixmap card_back;
    bool turned{true};
    bool selected{false};
//    Card* card{nullptr};

public:
    CardPicture(QWidget *parent = 0);
    void setPic(QPixmap& pic);
    void turn(bool t);
    bool is_turned();
    void select(bool s);
    bool is_selected();
//    void set_card();
//    Card get_matched_card();

    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent* event);

    QPixmap &getPic();
signals:
    void selected_signal();
};

#endif // CARDPICTURE_H
